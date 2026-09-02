// GreenDamTan_GocHelperLink.cpp
// 中文说明：
// 1. 本文件承接 CGocHelper 召唤/释放链路（AllHelperRelease/HelperRelease 及其
//    XMaze::AddHelper/DeleteHelper 支撑）从 IDA 反编译的精确还原。
// 2. 之所以使用 GreenDamTan_ 前缀新建编译单元而不是直接启用既有
//    actor/component/GocHelper.cpp，是因为该旧文件整文件尚未修复编译
//    （GetGOC 调用形态、UpdateStat、Finalize 等历史问题），而当前活跃链路
//    (CWorldProcess::ReqWorldEnter -> CGocHelper::AllHelperRelease) 只需要本
//    文件所载的精确函数集；待旧文件整体修复后本单元应按原始归属收敛回
//    actor/component/GocHelper.cpp (PDB OBJ: gochelper.obj)。
// 3. 所有函数体均按 IDA 反编译逐分支精确还原，未实现的部分以标准 TODO 标记。

#include "Soulworker/GameServer/XGameServer/actor/component/GocHelper.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GOComponent.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocQuest.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocEntity.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XGameServer/Ai.h"
#include "Soulworker/GameServer/XGameServer/Maze.h"
#include "Soulworker/GameServer/XGameServer/XWorldResMgr.h"
#include "Soulworker/GameServer/XCore/XArea/XArea.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/Common/XNet/XCommon/Packet/XSendPacket.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include <vector>
#include <map>

// ============================================================================
// XMaze::AddHelper - IDA @ 0x1403282C0
// 已精确还原 - pMonster 非空时 push_back 到 m_listSummonedHelper
// ============================================================================
void XMaze::AddHelper(CMonster* pMonster) {
    if (pMonster) {
        m_listSummonedHelper.push_back(pMonster);
    }
}

// ============================================================================
// XMaze::DeleteHelper - IDA @ 0x140328300
// 已精确还原 - 两个阶段:
// 1) 遍历 m_listSummonedHelper: dynamic_cast<CMonster*> 且
//    GetParentID 相等 && GetID 相等者从链表 erase 后 break;
// 2) 遍历 m_mapActor: XActor::GetType()==2 的 CMonster 且
//    GetParentID 相等 && GetActorID 相等者 ->
//    SetOnDie(false) + ClearStatus(2) + SetDieReason(0xD, GetHP())
//    + SetDie(nMotion, 1) 后 return。
// ============================================================================
void XMaze::DeleteHelper(CMonster* pMonster) {
    if (!pMonster)
        return;

    for (auto it = m_listSummonedHelper.begin();
         it != m_listSummonedHelper.end(); ++it) {
        CMonster* pMonsterIter = dynamic_cast<CMonster*>(*it);
        if (pMonsterIter) {
            if (pMonster->GetParentID() == pMonsterIter->GetParentID()) {
                if (pMonsterIter->GetID() == pMonster->GetID()) {
                    m_listSummonedHelper.erase(it);
                    break;
                }
            }
        }
    }

    for (auto& pair : m_mapActor) {
        XActor* pActor = pair.second;
        if (!pActor)
            continue;
        if (pActor->GetType() != 2)
            continue;
        CMonster* pMon = dynamic_cast<CMonster*>(pActor);
        if (!pMon)
            continue;
        if (pMon->GetParentID() == pMonster->GetParentID()) {
            if (pMon->GetActorID() == pMonster->GetActorID()) {
                pMon->SetOnDie(false);
                pMon->ClearStatus(2u);
                pMon->SetDieReason(0xD, pMon->GetHP());
                pMon->SetDie(1, 1);
                return;
            }
        }
    }
}

// ============================================================================
// CGocHelper::GetSummonedHelper - IDA @ 0x140092AD0
// 已精确还原 - m_mapSummonedHelper.find(dwHelperID)，未找到返回空
// ============================================================================
CMonster* CGocHelper::GetSummonedHelper(std::uint32_t dwHelperID) {
    auto it = m_mapSummonedHelper.find(dwHelperID);
    if (it == m_mapSummonedHelper.end())
        return nullptr;
    return it->second;
}

// ============================================================================
// CGocHelper::GetHelperInfo - IDA @ 0x140092700
// 已精确还原 - m_mapHelper.find(dwHelperID) 后整体赋值拷贝
// ============================================================================
bool CGocHelper::GetHelperInfo(std::uint32_t dwHelperID, ST_HELPER_INFO& stInfo) {
    auto it = m_mapHelper.find(dwHelperID);
    if (it == m_mapHelper.end())
        return false;
    stInfo = it->second;
    return true;
}

// ============================================================================
// CGocHelper::SetHelperSummonState - IDA @ 0x140092C20
// 已精确还原 - m_mapHelper.find 后写 bSummon 字段
// (IDA 偏移: v4[7].second.__vftable 首字节 = ST_HELPER_INFO::bSummon @ +0x190)
// ============================================================================
void CGocHelper::SetHelperSummonState(std::uint32_t dwHelperID, bool bSummonState) {
    auto it = m_mapHelper.find(dwHelperID);
    if (it != m_mapHelper.end()) {
        it->second.bSummon = bSummonState;
    }
}

// ============================================================================
// CGocHelper::HelperSupportRelease - IDA @ 0x140096750 (K 重载)
// 已精确还原 - m_mapHelper 命中且 stFriendSupport 非空时:
// (pUser UCID, helperID) 组 PS_DB_HELPER_SUPPORT_RELEASE ->
// XSendDBPacket(0x26, 4) << psRelease -> SendDBGame。
// (IDA 判空字段: iter->[6].second.ptrs 即 ST_HELPER_SUPPORT_INFO 首字段)
// ============================================================================
void CGocHelper::HelperSupportRelease(std::uint32_t dwHelperID) {
    auto it = m_mapHelper.find(dwHelperID);
    if (it == m_mapHelper.end())
        return;

    ST_HELPER_INFO& stInfo = it->second;
    // Per IDA: stFriendSupport 首字段 (dwFriendUCID) 非空才发送
    if (!stInfo.stFriendSupport.dwFriendUCID)
        return;

    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser)
        return;

    PS_DB_HELPER_SUPPORT_RELEASE psRelease{};
    psRelease.dwUCID = CQuestCondition::GetQuestID(pUser->GetActorID().dwActorID);
    psRelease.dwHelperID = stInfo.dwHelperID;

    XSendDBPacket xSendDBPacket(static_cast<XSocket*>(pUser), 0x26, 4);
    xSendDBPacket << psRelease;
    XGameServer::Instance()->SendDBGame(xSendDBPacket);
}

// ============================================================================
// CGocHelper::HelperSupportRelease - IDA @ 0x1400966D0 (无参重载)
// 已精确还原 - 遍历 m_mapSummonedHelper, 对每个 key 逐个调 K 重载
// ============================================================================
void CGocHelper::HelperSupportRelease() {
    for (auto at = m_mapSummonedHelper.begin(); at != m_mapSummonedHelper.end(); ++at) {
        HelperSupportRelease(at->first);
    }
}

// ============================================================================
// CGocHelper::HelperRelease - IDA @ 0x140094300
// 已精确还原 - 校验链:
//   GetOwner 无 Area / 非 XMaze / 非 CUser / 无召唤实例 -> 0;
//   GetAi -> SetEnalbeHelperWarp(0);
//   GetHelperInfo 成功:
//     bSummon=0 + SetHelperSummonState(0) + HelperSupportRelease(dwHelperID)
//     + XMaze::DeleteHelper + m_mapSummonedHelper.erase
//     + ST_LOG_GAME(16,3) DB 日志
//     + PS_HELPER_SUMMON_RES (0x27,2) bSummon=0 广播:
//       XMaze::IsParty -> PartySend; 否则 CGocNetwork::Send(pUser)
// ============================================================================
bool CGocHelper::HelperRelease(std::uint32_t dwHelperID) {
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser || !pUser->GetArea())
        return false;
    XMaze* pMaze = dynamic_cast<XMaze*>(pUser->GetArea());
    if (!pMaze)
        return false;

    CMonster* pMonster = GetSummonedHelper(dwHelperID);
    if (!pMonster)
        return false;

    if (pMonster->GetAi()) {
        CAi* pAi = pMonster->GetAi();
        pAi->SetEnalbeHelperWarp(false);
    }

    std::uint32_t dwMonsterActorID =
        CQuestCondition::GetQuestID(pMonster->GetActorID().dwActorID);

    ST_HELPER_INFO stHelperInfo{};
    if (GetHelperInfo(dwHelperID, stHelperInfo)) {
        stHelperInfo.bSummon = false;
        SetHelperSummonState(stHelperInfo.dwHelperID, false);
        HelperSupportRelease(dwHelperID);
        pMaze->DeleteHelper(pMonster);
        m_mapSummonedHelper.erase(dwHelperID);

        ST_LOG_GAME stLog{};
        stLog._nUAID = static_cast<int>(pUser->GetUAID());
        stLog._nUCID = CQuestCondition::GetQuestID(pUser->GetActorID().dwActorID);
        stLog._sMainType = 16;
        stLog._sSubType = 3;
        stLog.nParam0 = static_cast<int>(dwHelperID);
        stLog.nParam1 = pMaze->GetTBMapID();
        // Per IDA: vtable+384 虚调用即 CGocEntity::GetNetCafe 的 bool 值
        CGocEntity* pEntity = pUser->GetGOC<CGocEntity>();
        stLog.nParam2 = (pEntity && pEntity->GetNetCafe()) ? 1 : 0;
        // Per IDA: wcscpy_s(stLog.szComment, L"Release") (word_140B69560)
        std::wcscpy(stLog.szComment, L"Release");
        XGameServer::Instance()->SendDBLog(stLog);

        PS_HELPER_SUMMON_RES psSummon{};
        psSummon.dwUCID = CQuestCondition::GetQuestID(pUser->GetActorID().dwActorID);
        psSummon.dwActorID = dwMonsterActorID;
        psSummon.bSummon = false;
        psSummon.stHelper = stHelperInfo;

        XSendPacket xSendPacket(0x27, 2);
        xSendPacket << psSummon;
        if (pMaze->IsParty()) {
            pMaze->PartySend(xSendPacket);
        } else {
            CGocNetwork::Send(pUser, xSendPacket);
        }
        return true;
    }
    return false;
}

// ============================================================================
// CGocHelper::AllHelperRelease - IDA @ 0x140094AF0
// 已精确还原 - 收集 m_mapSummonedHelper 全部非空实例的 GetTableID 到
// vecReleaseList 后逐个 HelperRelease。
// ============================================================================
void CGocHelper::AllHelperRelease() {
    std::vector<std::uint32_t> vecReleaseList;

    for (auto& pair : m_mapSummonedHelper) {
        CMonster* pMonster = pair.second;
        if (pMonster) {
            int nTableID = pMonster->GetTableID();
            vecReleaseList.push_back(static_cast<std::uint32_t>(nTableID));
        }
    }

    for (std::size_t i = 0; i < vecReleaseList.size(); ++i) {
        HelperRelease(vecReleaseList[i]);
    }
}
