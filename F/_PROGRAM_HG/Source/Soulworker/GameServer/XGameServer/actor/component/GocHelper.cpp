#include "GocHelper.h"
#include "GocNetwork.h"
#include "GocAttribute.h"
#include "GocNpcAttribute.h"
#include "GocParty.h"
#include "../User.h"
#include "../Monster.h"
#include "../Ai.h"
#include "../GameServer.h"
#include "../../XCore/XArea/XMaze.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include <ctime>

// ============================================================================
// CGocHelper - Game Object Component for helper/assistant system
// ============================================================================

// IDA: ??0CGocHelper@@QEAA@XZ (0x140091E90)
// 对齐 IDA: 构造函数 - 初始化成员并调用 Clear
CGocHelper::CGocHelper()
    : GOComponent()
    , m_tLastSummon(0)
    , m_mapSummonedHelper()
    , m_mapHelper()
    , m_stMySupport()
    , m_bMySupportResister(0)
    , m_byMySupportReward(0)
    , m_byAutoSummonFlag(1)
{
    Clear();
}

// IDA: ??1CGocHelper@@UEAA@XZ (0x140091F80)
// 对齐 IDA: 析构函数 - 清理并释放资源
CGocHelper::~CGocHelper() {
    Clear();
    m_mapHelper.clear();
    m_mapSummonedHelper.clear();
}

// GOComponent interface
bool CGocHelper::Initialize() {
    return true;
}

void CGocHelper::Shutdown() {
    Clear();
}

void CGocHelper::Update(float fDeltaTime) {
    (void)fDeltaTime;
    // TODO: 汇编还原 - Update logic if needed
}

// IDA: ?Init@CGocHelper@@QEAAXXZ (0x140091FE0)
void CGocHelper::Init() {
    Clear();
}

// IDA: ?Clear@CGocHelper@@QEAAXXZ (0x140092000)
// 对齐 IDA: 清空所有成员变量
void CGocHelper::Clear() {
    m_tLastSummon = 0;
    m_mapSummonedHelper.clear();
    m_mapHelper.clear();
    m_bMySupportResister = 0;
    m_byMySupportReward = 0;
    m_byAutoSummonFlag = 1;
    memset(&m_stMySupport, 0, sizeof(m_stMySupport));
}

// IDA: ?Reset@CGocHelper@@QEAAXXZ (0x140092080)
// 对齐 IDA: 重置召唤相关数据
void CGocHelper::Reset() {
    m_tLastSummon = 0;
    m_mapSummonedHelper.clear();
}

// IDA: ?GetFamilyID@CGocHelper@@SAHXZ (0x140039020)
// 对齐 IDA: 返回组件家族ID
int CGocHelper::GetFamilyID() {
    return 16;
}

// IDA: ?SetHelperList@CGocHelper@@QEAA_NAEAUPS_HELPER_LIST_RES@@@Z (0x1400920C0)
// 对齐 IDA: 设置助手列表并自动召唤
bool CGocHelper::SetHelperList(PS_HELPER_LIST_RES& psList) {
    SetAutoSummonFlag(psList.byAutoSummon);

    for (size_t i = 0; i < psList.vecHelper.size(); ++i) {
        ST_HELPER_INFO stAdd = psList.vecHelper[i];
        AddMyHelper(stAdd, false);
    }

    SendHelperList();
    return true;
}

// IDA: ?GetHelperList@CGocHelper@@QEAA_NAEAV?$vector@UST_HELPER_INFO@@V?$allocator@UST_HELPER_INFO@@@std@@@std@@_N@Z (0x1400922B0)
// 对齐 IDA: 获取助手列表，支持按顺序排序
bool CGocHelper::GetHelperList(std::vector<ST_HELPER_INFO>& vecHelper, bool isOrder) {
    vecHelper.clear();

    if (isOrder) {
        // Sort by order
        std::map<std::uint8_t, ST_HELPER_INFO> mapHelperOrder;

        for (auto it = m_mapHelper.begin(); it != m_mapHelper.end(); ++it) {
            mapHelperOrder[it->second.byOrder] = it->second;
        }

        for (auto it = mapHelperOrder.begin(); it != mapHelperOrder.end(); ++it) {
            vecHelper.push_back(it->second);
        }
    } else {
        // No ordering, just iterate
        for (auto it = m_mapHelper.begin(); it != m_mapHelper.end(); ++it) {
            vecHelper.push_back(it->second);
        }
    }

    return true;
}

// IDA: ?SendHelperList@CGocHelper@@QEAAXXZ (0x140092560)
// 对齐 IDA: 发送助手列表到客户端
// IDA 反编译:
// void __fastcall CGocHelper::SendHelperList(CGocHelper *this, float a2, float a3)
// {
//   VBitmask *v3; // rax
//   unsigned __int8 v4; // r9
//   PS_HELPER_LIST_RES psHelper; // [rsp+20h] [rbp-100A8h] BYREF
//   XSendPacket xSendPacket; // [rsp+60h] [rbp-10068h] BYREF
//   _BYTE v7[8]; // [rsp+10080h] [rbp-48h] BYREF
//   __int64 v8; // [rsp+10088h] [rbp-40h]
//   DynArray_cl<int> *p_m_ChunkSizeTempMemOfs; // [rsp+10090h] [rbp-38h]
//   BOOL IsAutoSummon; // [rsp+10098h] [rbp-30h]
//   VChunkFile *v11; // [rsp+100A0h] [rbp-28h]
//   XActor *pActor; // [rsp+100A8h] [rbp-20h]
//
//   v8 = -2;
//   PS_HELPER_LIST_RES::PS_HELPER_LIST_RES(&psHelper);
//   p_m_ChunkSizeTempMemOfs = &std::list<CBattleZone *>::size((VChunkLocker *)this)[3].m_ChunkSizeTempMemOfs;
//   v3 = (VBitmask *)((__int64 (__fastcall *)(DynArray_cl<int> *, _BYTE *))p_m_ChunkSizeTempMemOfs->__vftable[7].dtr_DynArray_cl<int>)(p_m_ChunkSizeTempMemOfs, v7);
//   psHelper.dwUCID = CQuestCondition::GetQuestID(v3);
//   IsAutoSummon = CGocHelper::IsAutoSummon(this);
//   psHelper.byAutoSummon = IsAutoSummon;
//   if ( CGocHelper::GetHelperList(this, &psHelper.vecHelper, 0) == 1 )
//   {
//     XSendPacket::XSendPacket(&xSendPacket, 0x27u, 1u);  // Main=0x27, Sub=1
//     operator<<(&xSendPacket, &psHelper);
//     v11 = std::list<CBattleZone *>::size((VChunkLocker *)this);
//     if ( v11 )
//       pActor = (XActor *)&v11[3].m_ChunkSizeTempMemOfs;
//     else
//       pActor = nullptr;
//     CGocNetwork::Send(pActor, &xSendPacket);
//   }
//   PS_HELPER_LIST_RES::~PS_HELPER_LIST_RES(&psHelper);
// }
void CGocHelper::SendHelperList() {
    // TODO: 需要实现 - 需要 CGocNetwork::Send 和 XSendPacket
    // 1. 构建 PS_HELPER_LIST_RES
    // 2. 获取 owner 的 UCID
    // 3. 设置 byAutoSummon = IsAutoSummon()
    // 4. 调用 GetHelperList(psHelper.vecHelper, false)
    // 5. 发送包 (Main=0x27, Sub=1)
}

// IDA: ?IsAutoSummon@CGocHelper@@QEAA_NXZ (0x140091DC0)
// 对齐 IDA: 检查自动召唤标志
bool CGocHelper::IsAutoSummon() const {
    return m_byAutoSummonFlag != 0;
}

// IDA: ?SetHelperSummonTime@CGocHelper@@QEAAXXZ (0x140091DF0)
// 对齐 IDA: 设置最后召唤时间为当前时间
void CGocHelper::SetHelperSummonTime() {
    // IDA: ATL::CTime::GetTickCount(&result)
    m_tLastSummon = std::time(nullptr);
}

// IDA: ?SetAutoSummonFlag@CGocHelper@@QEAAXE@Z (0x140091DA0)
// 对齐 IDA: 设置自动召唤标志
void CGocHelper::SetAutoSummonFlag(std::uint8_t byFlag) {
    m_byAutoSummonFlag = byFlag;
}

// IDA: ?GetHelperInfo@CGocHelper@@QEAA_NKAEAUST_HELPER_INFO@@@Z (0x140092700)
// 对齐 IDA: 获取助手信息
bool CGocHelper::GetHelperInfo(std::uint32_t dwHelperID, ST_HELPER_INFO& stInfo) {
    auto it = m_mapHelper.find(dwHelperID);
    if (it != m_mapHelper.end()) {
        stInfo = it->second;
        return true;
    }
    return false;
}

// IDA: ?FindHelper@CGocHelper@@QEAA_NK@Z (0x1400928A0)
// 对齐 IDA: 查找助手是否存在
bool CGocHelper::FindHelper(std::uint32_t dwHelperID) {
    return m_mapHelper.find(dwHelperID) != m_mapHelper.end();
}

// IDA: ?SetHelperSummonState@CGocHelper@@QEAAXK_N@Z (0x140092C20)
// 对齐 IDA: 设置助手召唤状态
void CGocHelper::SetHelperSummonState(std::uint32_t dwHelperID, bool bSummon) {
    auto it = m_mapHelper.find(dwHelperID);
    if (it != m_mapHelper.end()) {
        it->second.bSummon = bSummon;
    }
}

// IDA: ?GetSummonedHelper@CGocHelper@@QEAAPEAVCMonster@@K@Z (0x140092AD0)
// 对齐 IDA: 获取已召唤的助手怪物
CMonster* CGocHelper::GetSummonedHelper(std::uint32_t dwHelperID) {
    auto it = m_mapSummonedHelper.find(dwHelperID);
    if (it != m_mapSummonedHelper.end()) {
        return it->second;
    }
    return nullptr;
}

// IDA: ?GetSummonedHelperList@CGocHelper@@QEAAXAEAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@@Z (0x140092B40)
// 对齐 IDA: 获取所有已召唤助手列表
void CGocHelper::GetSummonedHelperList(std::map<std::uint32_t, std::uint32_t>& mapSummonHelperList) {
    mapSummonHelperList.clear();

    for (auto it = m_mapSummonedHelper.begin(); it != m_mapSummonedHelper.end(); ++it) {
        std::uint32_t dwHelperID = it->first;
        CMonster* pMonster = it->second;

        if (pMonster) {
            // TODO: 汇编还原 - Get actor ID from CMonster
            // mapSummonHelperList[dwHelperID] = pMonster->GetActorID();
        }
    }
}

// IDA: ?CheckSummonHelper@CGocHelper@@QEAA_NK@Z (0x140092D10)
// 对齐 IDA: 检查是否可以召唤指定助手
// 返回 true 如果助手存在且未召唤
bool CGocHelper::CheckSummonHelper(std::uint32_t dwHelperID) {
    // 检查助手是否存在于 m_mapHelper
    if (m_mapHelper.find(dwHelperID) == m_mapHelper.end()) {
        return false;
    }
    // 检查是否已经召唤（如果已召唤则返回 false）
    return m_mapSummonedHelper.find(dwHelperID) == m_mapSummonedHelper.end();
}

// IDA: ?CheckReleaseHelper@CGocHelper@@QEAA_NK@Z (0x140092DC0)
// 对齐 IDA: 检查是否可以释放指定助手
// 返回 true 如果助手存在且已召唤
bool CGocHelper::CheckReleaseHelper(std::uint32_t dwHelperID) {
    // 检查助手是否存在于 m_mapHelper
    if (m_mapHelper.find(dwHelperID) == m_mapHelper.end()) {
        return false;
    }
    // 检查是否已经召唤（如果已召唤则返回 true）
    return m_mapSummonedHelper.find(dwHelperID) != m_mapSummonedHelper.end();
}

// IDA: ?CheckHelperSummonDelay@CGocHelper@@QEAA_NXZ (0x140092CA0)
// 对齐 IDA: 检查召唤延迟时间
bool CGocHelper::CheckHelperSummonDelay() {
    // IDA: 比较当前时间与 m_tLastSummon
    // 默认延迟 10 秒
    std::time_t now = std::time(nullptr);
    return (now - m_tLastSummon) >= 10;
}

// IDA: ?CheckSummonHelperCount@CGocHelper@@QEAA_NXZ (0x140092E70)
// 对齐 IDA: 检查是否可以召唤更多助手
// 需要检查队伍人数和迷宫助手数量
bool CGocHelper::CheckSummonHelperCount() {
    // TODO: 汇编还原 - 需要访问 XMaze 和 CGocParty
    // 简化实现：检查已召唤数量
    // IDA: 如果在队伍中，检查队伍人数 + 已召唤数量 < 4
    //      如果不在队伍中，检查已召唤数量 + 1 < 4
    return m_mapSummonedHelper.size() < 3;
}

// IDA: ?AddMyHelper@CGocHelper@@QEAA_NAEAUST_HELPER_INFO@@_N@Z (0x140092900)
// 对齐 IDA: 添加助手到列表
bool CGocHelper::AddMyHelper(ST_HELPER_INFO& stInfo, bool bSend) {
    // Check TB_HELPER table
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        return false;
    }

    // IDA: 检查 TB_HELPER 表是否存在该助手
    // if (!XResourceMgr::GetTB_HELPER(&pGameServer->m_xResourceMgr, stInfo.dwHelperID)) {
    //     return false;
    // }

    // Check if helper already exists
    if (FindHelper(stInfo.dwHelperID)) {
        return false;
    }

    // Calculate helper stats
    CalcHelperStatsALL(&stInfo, 1);

    // IDA: key 是 dwHelperID 而不是 byOrder
    m_mapHelper[stInfo.dwHelperID] = stInfo;

    // Send packet if requested
    if (bSend) {
        // TODO: 汇编还原 - Send packet 0x27 sub 3
        // XSendPacket xSendPacket(0x27, 3);
        // xSendPacket << stInfo;
        // CGocNetwork::Send(pActor, &xSendPacket);
    }

    return true;
}

// IDA: ?HelperSummon@CGocHelper@@QEAA_NK@Z (0x140093410)
// 对齐 IDA: 召唤助手
bool CGocHelper::HelperSummon(std::uint32_t dwHelperID) {
    // TODO: 汇编还原 - Complex summon logic
    // Key steps from IDA:
    // 1. Check owner exists and is valid
    // 2. Check maze type is valid (not type 12)
    // 3. Get helper info from m_mapHelper
    // 4. Get TB_HELPER and TB_MONSTER from XResourceMgr
    // 5. Create monster at position near player
    // 6. Set owner, direction, AI
    // 7. Initialize stats via CGocNpcAttribute
    // 8. Add to XMaze helper list
    // 9. Send PS_HELPER_SUMMON_RES packet

    ST_HELPER_INFO stHelperInfo;
    if (!GetHelperInfo(dwHelperID, stHelperInfo)) {
        return false;
    }

    // TODO: 汇编还原 - Full summon logic
    return false;
}

// IDA: ?HelperRelease@CGocHelper@@QEAA_NK@Z (0x140094300)
// 对齐 IDA: 释放召唤的助手
bool CGocHelper::HelperRelease(std::uint32_t dwHelperID) {
    auto it = m_mapSummonedHelper.find(dwHelperID);
    if (it == m_mapSummonedHelper.end()) {
        return false;
    }

    CMonster* pMonster = it->second;
    if (!pMonster) {
        return false;
    }

    // TODO: 汇编还原 - Full release logic
    // 1. Disable helper warp in AI
    // 2. Update helper info state
    // 3. Call HelperSupportRelease
    // 4. Delete from XMaze
    // 5. Remove from m_mapSummonedHelper
    // 6. Send log to DB
    // 7. Send PS_HELPER_SUMMON_RES packet

    return false;
}

// IDA: ?HelperWarp@CGocHelper@@QEAA_NK@Z (0x140094130)
// 对齐 IDA: 传送助手到玩家位置
bool CGocHelper::HelperWarp(std::uint32_t dwHelperID) {
    // TODO: 汇编还原 - Warp summoned helper to player position
    return false;
}

// IDA: ?HelperProcess@CGocHelper@@QEAAXAEAUPS_HELPER_SUMMON_REQ@@@Z (0x140093080)
// 对齐 IDA: 处理助手召唤/释放请求
void CGocHelper::HelperProcess(struct PS_HELPER_SUMMON_REQ& psInfo) {
    // Get owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // Check if owner is dead
    // TODO: 汇编还原 - if (CMover::IsDie(pOwner)) return;

    if (psInfo.bSummon) {
        // Summon request
        if (!CheckHelperSummonDelay()) {
            // TODO: Send error message 0xE295
            return;
        }

        if (!CheckSummonHelper(psInfo.dwHelperID)) {
            // TODO: Send error message 0xE297
            return;
        }

        if (!CheckSummonHelperCount()) {
            // TODO: Send error message 0xE298
            return;
        }

        HelperSummon(psInfo.dwHelperID);
    } else {
        // Release request
        if (!CheckReleaseHelper(psInfo.dwHelperID)) {
            // TODO: Send error message 0xE297
            return;
        }

        HelperRelease(psInfo.dwHelperID);
    }
}

// IDA: ?CalcHelperStatsALL@CGocHelper@@QEAAXAEAUST_HELPER_INFO@@H@Z (0x140095280)
// 对齐 IDA: 计算助手所有属性
void CGocHelper::CalcHelperStatsALL(ST_HELPER_INFO* pstInfo, int nMode) {
    if (!pstInfo) {
        return;
    }

    // IDA: nMode 控制计算模式
    // nMode = 1: 重新计算所有属性
    // nMode = 0: 只更新 vecAddditional

    if (nMode == 1) {
        CalcOriginStats(*pstInfo);
        CalcEquipItemStats(*pstInfo);
        CalcFriendItemStats(*pstInfo);
    }

    // TODO: 汇编还原 - 处理 vecAddditional
}

// IDA: ?CalcOriginStats@CGocHelper@@QEAAXAEAUST_HELPER_INFO@@@Z (0x140095360)
// 对齐 IDA: 计算助手原始属性
void CGocHelper::CalcOriginStats(ST_HELPER_INFO& stInfo) {
    // TODO: 汇编还原 - Calculate origin stats from TB_HELPER
}

// IDA: ?CalcEquipItemStats@CGocHelper@@QEAAXAEAUST_HELPER_INFO@@@Z (0x140095500)
// 对齐 IDA: 计算装备物品属性
void CGocHelper::CalcEquipItemStats(ST_HELPER_INFO& stInfo) {
    // TODO: 汇编还原 - Calculate stats from equipped items (stItem[3])
}

// IDA: ?CalcFriendItemStats@CGocHelper@@QEAAXAEAUST_HELPER_INFO@@@Z (0x1400958B0)
// 对齐 IDA: 计算好友支援物品属性
void CGocHelper::CalcFriendItemStats(ST_HELPER_INFO& stInfo) {
    // TODO: 汇编还原 - Calculate stats from friend support item
}

// IDA: ?HelperSupportRelease@CGocHelper@@QEAAXXZ (0x1400966D0)
// 对齐 IDA: 释放所有助手支援
void CGocHelper::HelperSupportRelease() {
    // TODO: 汇编还原 - Release all helper support
}

// IDA: ?HelperSupportRelease@CGocHelper@@QEAAXK@Z (0x140096750)
// 对齐 IDA: 释放指定助手的支援
void CGocHelper::HelperSupportRelease(std::uint32_t dwHelperID) {
    // TODO: 汇编还原 - Release specific helper support
}

// IDA: ?SyncSummonedInfo@CGocHelper@@QEAAXXZ (0x140096060)
// 对齐 IDA: 同步已召唤助手信息
void CGocHelper::SyncSummonedInfo() {
    // TODO: 汇编还原 - Sync summoned helper info to client
}

// IDA: ?CheckAllHelperSummon@CGocHelper@@QEAAXXZ (0x1400948A0)
// 对齐 IDA: 检查并自动召唤所有助手
void CGocHelper::CheckAllHelperSummon() {
    // IDA: 检查自动召唤标志
    if (!IsAutoSummon()) {
        return;
    }

    // TODO: 汇编还原 - 检查队伍状态、迷宫类型等
    // 遍历助手列表，尝试召唤
    std::vector<ST_HELPER_INFO> vecHelper;
    GetHelperList(vecHelper, true);

    for (size_t i = 0; i < vecHelper.size() && CheckSummonHelperCount(); ++i) {
        HelperSummon(vecHelper[i].dwHelperID);
    }
}

// IDA: ?AllHelperRelease@CGocHelper@@QEAAXXZ (0x140094AF0)
// 对齐 IDA: 释放所有已召唤的助手
void CGocHelper::AllHelperRelease() {
    // IDA: 收集所有需要释放的助手ID
    std::vector<std::uint32_t> vecReleaseList;

    for (auto it = m_mapSummonedHelper.begin(); it != m_mapSummonedHelper.end(); ++it) {
        if (it->second) {
            vecReleaseList.push_back(it->first);
        }
    }

    // 释放所有助手
    for (size_t i = 0; i < vecReleaseList.size(); ++i) {
        HelperRelease(vecReleaseList[i]);
    }
}

// IDA: ?AllHelperWarp@CGocHelper@@QEAAXXZ (0x140094C10)
// 对齐 IDA: 传送所有助手到玩家位置
void CGocHelper::AllHelperWarp() {
    for (auto it = m_mapSummonedHelper.begin(); it != m_mapSummonedHelper.end(); ++it) {
        if (it->second) {
            HelperWarp(it->first);
        }
    }
}

// IDA: ?OtherHelperClear@CGocHelper@@QEAAXXZ (0x140094D30)
// 对齐 IDA: 清除其他助手
void CGocHelper::OtherHelperClear() {
    // TODO: 汇编还原 - Clear other helpers (not own helpers)
}

// Support info
// IDA: ?SetMySupportInfo@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_INFO_RES@@@Z (0x140096660)
// 对齐 IDA: 设置支援信息
void CGocHelper::SetMySupportInfo(const ST_HELPER_SUPPORT_INFO& stInfo) {
    m_stMySupport = stInfo;
}

// IDA: ?SetMySupportInfo@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_INFO_RES@@@Z (0x140096660)
// 对齐 IDA: 设置支援信息 (从数据库响应)
void CGocHelper::SetMySupportInfo(PS_HELPER_SUPPORT_INFO_RES& stMyInfo) {
    // IDA: qmemcpy(&this->m_stMySupport, v2, sizeof(this->m_stMySupport));
    // IDA: this->m_bMySupportResister = stMyInfo->bRegister;
    // IDA: this->m_byMySupportReward = stMyInfo->byRewardType;
    std::memcpy(&m_stMySupport, &stMyInfo, sizeof(m_stMySupport));
    m_bMySupportResister = stMyInfo.bRegister;
    m_byMySupportReward = stMyInfo.byRewardType;
}

// IDA: ?GetSupportTypeRate@CGocHelper@@QEAAME@Z (0x140096500)
// 对齐 IDA: 获取支援类型比率
float CGocHelper::GetSupportTypeRate(std::uint8_t byType) {
    // IDA: if (!byType) return 0.19999999; // 0.2
    // IDA: if (byType == 1) return 0.19999999; // 0.2
    // IDA: return 0.0;
    if (!byType) {
        return 0.2f;
    }
    if (byType == 1) {
        return 0.2f;
    }
    return 0.0f;
}

// IDA: ?GetSupportTypeValue@CGocHelper@@QEAAME@Z (0x140096540)
// 对齐 IDA: 获取支援类型数值
float CGocHelper::GetSupportTypeValue(std::uint8_t byType) {
    // IDA: fRate = CGocHelper::GetSupportTypeRate(this, byType);
    // IDA: if (fRate <= 0.0) return 0.0;
    float fRate = GetSupportTypeRate(byType);
    if (fRate <= 0.0f) {
        return 0.0f;
    }

    float fValue = 0.0f;

    // IDA: 根据类型获取玩家属性
    // byType == 0: 获取 STAT_INDEX 0x15 (攻击力)
    // byType == 1: 获取 STAT_INDEX 0x18 (防御力)
    // TODO: 汇编还原 - 需要访问 CMover owner 和 CGocAttribute 组件
    // CMover* pOwner = GetOwnerGO();
    // if (pOwner) {
    //     std::tr1::shared_ptr<CGocAttribute> pAttr = pOwner->GetGOC<CGocAttribute>();
    //     if (pAttr) {
    //         if (byType == 0) {
    //             fValue = pAttr->GetStat(0x15);
    //         } else if (byType == 1) {
    //             fValue = pAttr->GetStat(0x18);
    //         }
    //     }
    // }

    if (fValue > 0.0f) {
        return fValue * fRate;
    }
    return 0.0f;
}

// IDA: ?GetMyHelperStatsALL@CGocHelper@@QEAAXAEAUPS_HELPER_STAT_UPDATE@@@Z (0x140095170)
// 对齐 IDA: 获取所有助手属性
void CGocHelper::GetMyHelperStatsALL(PS_HELPER_STAT_UPDATE& psStat) {
    // IDA: 遍历 m_mapHelper，将每个助手的 vecAddditional 添加到 psStat.mapHelperStats
    for (auto it = m_mapHelper.begin(); it != m_mapHelper.end(); ++it) {
        std::uint32_t dwHelperID = it->first;
        const ST_HELPER_INFO& stInfo = it->second;

        // TODO: 汇编还原 - 需要完整的 StatInfoList 类型
        // StatInfoList vecList;
        // vecList = stInfo.vecAddditional;
        // psStat.mapHelperStats[dwHelperID] = vecList;
    }
}

// IDA: ?UnEquipHelperFriendItemStats@CGocHelper@@QEAAXK@Z (0x140095e80)
// 对齐 IDA: 卸下助手好友支援物品属性
void CGocHelper::UnEquipHelperFriendItemStats(std::uint32_t dwHelperID) {
    // IDA: 查找助手
    auto it = m_mapHelper.find(dwHelperID);
    if (it == m_mapHelper.end()) {
        return;
    }

    // IDA: 检查是否有好友支援物品 (stFriendSupport.fVal > 0)
    ST_HELPER_INFO& stInfo = it->second;
    if (stInfo.stFriendSupport.fVal <= 0.0f) {
        return;
    }

    // IDA: 获取已召唤的助手怪物
    CMonster* pHelper = GetSummonedHelper(dwHelperID);
    if (pHelper) {
        // TODO: 汇编还原 - 获取 CGocNpcAttribute 组件并更新属性
        // std::tr1::shared_ptr<CGocNpcAttribute> pAttr = pHelper->GetGOC<CGocNpcAttribute>();
        // if (pAttr) {
        //     if (stInfo.stFriendSupport.bySupportType == 1) {
        //         pAttr->RemoveStat(24); // 按类型移除属性
        //     } else {
        //         pAttr->RemoveStat(21);
        //         pAttr->RemoveStat(20);
        //     }
        // }
    }
}

// IDA: ?ReqHelperSupportInfo@CGocHelper@@QEAAXXZ (0x1400968f0)
// 对齐 IDA: 请求支援信息
void CGocHelper::ReqHelperSupportInfo() {
    // TODO: 汇编还原 - 发送请求到 RelayServer
    // CMover* pOwner = GetOwnerGO();
    // if (!pOwner) return;
    // CUser* pUser = dynamic_cast<CUser*>(pOwner);
    // if (!pUser) return;
    //
    // XSendPacket xSendPacket(0xF5, 0x27);
    // xSendPacket << pUser->GetUCID();
    // XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    // pGameServer->m_communitySocket.SendCmd(&xSendPacket, pUser, 0x27, 5);
}

// IDA: ?ReqHelperSupportRegister@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_REGISTER_REQ@@@Z (0x140096a10)
// 对齐 IDA: 请求注册支援
void CGocHelper::ReqHelperSupportRegister(PS_HELPER_SUPPORT_REGISTER_REQ& psSupport) {
    // IDA: fSupportValue = CGocHelper::GetSupportTypeValue(this, psSupport->bySupportType);
    float fSupportValue = GetSupportTypeValue(psSupport.bySupportType);
    if (fSupportValue < 0.0f) {
        return;
    }

    // TODO: 汇编还原 - 构造并发送 PS_SERVER_HELPER_SUPPORT_REGISTER
    // ATL::CTime tCurr = ATL::CTime::GetTickCount();
    // ATL::CTimeSpan tDuration(0, 24, 0, 0);
    // ATL::CTime tDate = tCurr + tDuration;
    //
    // PS_SERVER_HELPER_SUPPORT_REGISTER psSend;
    // psSend.stInfo.dwFriendUCID = pUser->GetUCID();
    // psSend.stInfo.bySupportType = psSupport.bySupportType;
    // psSend.stInfo.fVal = fSupportValue;
    // psSend.stInfo.nDate = tDate.GetTime();
    //
    // XSendPacket xSendPacket(0xF5, 0x28);
    // xSendPacket << psSend;
    // pGameServer->m_communitySocket.SendCmd(&xSendPacket, pUser, 0x27, 6);
}

// IDA: ?ReqHelperSupportReward@CGocHelper@@QEAAXXZ (0x140096be0)
// 对齐 IDA: 请求支援奖励
void CGocHelper::ReqHelperSupportReward() {
    // IDA: 检查是否可以领取奖励 (m_byMySupportReward == 1)
    if (m_byMySupportReward != 1) {
        return;
    }

    // TODO: 汇编还原 - 获取奖励配置并发送
    // XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    // TB_HELPER_REWARD* pTB_HELPER_REWARD = pGameServer->m_xResourceMgr.GetTB_HELPER_REWARD(1);
    // if (!pTB_HELPER_REWARD) return;
    //
    // PS_SERVER_HELPER_SUPPORT_REWARD psReward;
    // psReward.dwUCID = pUser->GetUCID();
    //
    // // 添加奖励物品
    // if (pTB_HELPER_REWARD->Reward_Item_ID_01 && pTB_HELPER_REWARD->Reward_Item_Count_01) {
    //     ST_CREATE_ITEM stItem;
    //     stItem.nItemID = pTB_HELPER_REWARD->Reward_Item_ID_01;
    //     stItem.shCount = pTB_HELPER_REWARD->Reward_Item_Count_01;
    //     psReward.stCreateItems.push_back(stItem);
    // }
    // // ... 更多奖励物品
    //
    // // 检查背包空间
    // std::tr1::shared_ptr<CGocInventory> pInven = pOwner->GetGOC<CGocInventory>();
    // int nError = pInven->CheckAddItems(psReward.stCreateItems);
    // if (nError) {
    //     CGocNetwork::SendErrorMessage(pOwner, 0x27, 7, nError == 5 ? 0xCB2A : 0xCB24);
    //     return;
    // }
    //
    // XSendPacket xSendPacket(0xF5, 0x29);
    // xSendPacket << psReward;
    // pGameServer->m_communitySocket.SendCmd(&xSendPacket, pUser, 0x27, 7);
}

// IDA: ?ReqHelperSupportList@CGocHelper@@QEAAXXZ (0x140096f70)
// 对齐 IDA: 请求支援列表
void CGocHelper::ReqHelperSupportList() {
    // TODO: 汇编还原 - 发送请求到 RelayServer
    // CMover* pOwner = GetOwnerGO();
    // if (!pOwner) return;
    // CUser* pUser = dynamic_cast<CUser*>(pOwner);
    // if (!pUser) return;
    //
    // XSendPacket xSendPacket(0xF5, 0x30);
    // xSendPacket << pUser->GetUCID();
    // XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    // pGameServer->m_communitySocket.SendCmd(&xSendPacket, pUser, 0x27, 8);
}

// IDA: ?GetLastOrderNumber@CGocHelper@@QEAAEXZ (0x14009a780)
// 对齐 IDA: 获取最后一个顺序号
std::uint8_t CGocHelper::GetLastOrderNumber() {
    std::uint8_t byOrder = 0;

    // IDA: 遍历 m_mapHelper，找到最大的 byOrder
    for (auto it = m_mapHelper.begin(); it != m_mapHelper.end(); ++it) {
        const ST_HELPER_INFO& stInfo = it->second;
        if (byOrder < stInfo.byOrder) {
            byOrder = stInfo.byOrder;
        }
    }

    return byOrder;
}

// IDA: ?ReqHelperEquip@CGocHelper@@QEAAXAEAUPS_HELPER_EQUIP_REQ@@@Z (0x140097200)
// 对齐 IDA: 请求装备助手物品
void CGocHelper::ReqHelperEquip(PS_HELPER_EQUIP_REQ& psEquip) {
    // TODO: 汇编还原 - 复杂的装备逻辑 (~700行)
    // 主要流程:
    // 1. 验证助手是否存在 (m_mapHelper.find)
    // 2. 获取 TB_HELPER 表
    // 3. 处理三种情况:
    //    a) xInvenSerial && xHelperSerial: 交换背包和助手物品
    //    b) xInvenSerial only: 从背包装备到助手空槽
    //    c) xHelperSerial only: 从助手卸下到背包空槽
    // 4. 验证物品类型和槽位匹配
    // 5. 发送 PS_DB_HELPER_EQUIP_REQ 到数据库
    // 6. 记录日志和统计信息

    // 简化实现 - 发送数据库请求
    // CMover* pOwner = GetOwnerGO();
    // if (!pOwner) return;
    // CUser* pUser = dynamic_cast<CUser*>(pOwner);
    // if (!pUser) return;
    //
    // PS_DB_HELPER_EQUIP_REQ psDB;
    // psDB.dwUCID = pUser->GetUCID();
    // psDB.psEquip = psEquip;
    //
    // XSendDBPacket xSendDBPacket(pUser, 0x26, 5);
    // xSendDBPacket << psDB;
    // XGameServer::Instance()->SendDBGame(&xSendDBPacket);
}

// IDA: ?ReqHelperChangeOrder@CGocHelper@@QEAAXAEAUPS_HELPER_CHANGE_ORDER@@@Z (0x140097eb0)
// 对齐 IDA: 请求更改助手顺序
void CGocHelper::ReqHelperChangeOrder(PS_HELPER_CHANGE_ORDER& psOrder) {
    // IDA: 查找两个助手
    auto it1 = m_mapHelper.find(psOrder.dwHelperID_1);
    auto it2 = m_mapHelper.find(psOrder.dwHelperID_2);

    // IDA: 如果两个助手都存在
    if (it1 != m_mapHelper.end() && it2 != m_mapHelper.end()) {
        // TODO: 汇编还原 - 发送数据库请求
        // CMover* pOwner = GetOwnerGO();
        // if (!pOwner) return;
        // CUser* pUser = dynamic_cast<CUser*>(pOwner);
        // if (!pUser) return;
        //
        // psOrder.dwUCID = pUser->GetUCID();
        // psOrder.byOrder_1 = it2->second.byOrder;
        // psOrder.byOrder_2 = it1->second.byOrder;
        //
        // XSendDBPacket xSendDBPacket(pUser, 0x26, 6);
        // xSendDBPacket << psOrder;
        // XGameServer::Instance()->SendDBGame(&xSendDBPacket);
    }
}

// IDA: ?ReqHelperChangeAutoSummon@CGocHelper@@QEAAXAEAUPS_HELPER_CHANGE_AUTO_SUMMON@@@Z (0x1400980c0)
// 对齐 IDA: 请求更改自动召唤设置
void CGocHelper::ReqHelperChangeAutoSummon(PS_HELPER_CHANGE_AUTO_SUMMON& psFlag) {
    // IDA: 检查是否与当前设置相同
    bool bNewFlag = (psFlag.bySelect != 0);
    if (IsAutoSummon() == bNewFlag) {
        // IDA: 如果相同，直接发送响应给客户端
        // XSendPacket xSendPacket(0x27, 0x15);
        // xSendPacket << psFlag;
        // CGocNetwork::Send(pOwner, &xSendPacket);
        return;
    }

    // TODO: 汇编还原 - 发送数据库请求
    // CMover* pOwner = GetOwnerGO();
    // if (!pOwner) return;
    //
    // XSendDBPacket xSendDBPacket(pOwner, 0x26, 7);
    // xSendDBPacket << psFlag;
    // XGameServer::Instance()->SendDBGame(&xSendDBPacket);
}

// IDA: ?ReqHelperSupportEquip@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_EQUIP_REQ@@@Z (0x140097080)
// 对齐 IDA: 请求支援装备
void CGocHelper::ReqHelperSupportEquip(PS_HELPER_SUPPORT_EQUIP_REQ& psEquip) {
    // IDA: 设置 UCID
    // CMover* pOwner = GetOwnerGO();
    // if (!pOwner) return;
    // CUser* pUser = dynamic_cast<CUser*>(pOwner);
    // if (!pUser) return;
    // psEquip.dwUCID = pUser->GetUCID();

    // IDA: 查找助手
    auto it = m_mapHelper.find(psEquip.nPoint);
    if (it == m_mapHelper.end()) {
        return;
    }

    // TODO: 汇编还原 - 发送请求到 RelayServer
    // XSendPacket xSendPacket(0xF5, 0x31);
    // xSendPacket << psEquip;
    // XGameServer::Instance()->m_communitySocket.SendCmd(&xSendPacket, pUser, 0x27, 9);
}

// IDA: ?SendDBAddHelper@CGocHelper@@QEAA_NK@Z (0x14009a290)
// 对齐 IDA: 发送添加助手到数据库
bool CGocHelper::SendDBAddHelper(std::uint32_t dwHelperID) {
    // IDA: 检查参数
    if (!dwHelperID) {
        return false;
    }

    // IDA: 检查是否已存在
    if (FindHelper(dwHelperID)) {
        return false;
    }

    // TODO: 汇编还原 - 发送数据库请求并记录日志
    // CMover* pOwner = GetOwnerGO();
    // if (!pOwner) return false;
    // CUser* pUser = dynamic_cast<CUser*>(pOwner);
    // if (!pUser) return false;
    //
    // PS_HELPER_ADD_REQ psHelper;
    // psHelper.dwUCID = pUser->GetUCID();
    // psHelper.dwHelperID = dwHelperID;
    // psHelper.byOrder = GetLastOrderNumber() + 1;
    //
    // XSendDBPacket xSendDBPacket(pUser, 0x26, 2);
    // xSendDBPacket << psHelper;
    // XGameServer::Instance()->SendDBGame(&xSendDBPacket);
    //
    // // 记录日志
    // ST_LOG_GAME stLog;
    // stLog._nUAID = pUser->GetUAID();
    // stLog._nUCID = pUser->GetUCID();
    // stLog._sMainType = 16;
    // stLog._sSubType = 1;
    // stLog.nParam0 = dwHelperID;
    // stLog.nParam2 = pUser->GetLevel();
    // XGameServer::Instance()->SendDBLog(&stLog);
    //
    // // 记录统计
    // ST_STATISTICS_HELPER stStatistics;
    // stStatistics.byFlag = 1;
    // stStatistics.dwUCID = pUser->GetUCID();
    // stStatistics.dwHelperID = dwHelperID;
    // XSendDBPacket xSendDBStatistics(pUser, 0xF0, 5);
    // xSendDBStatistics << stStatistics;
    // XGameServer::Instance()->SendDBStatistics(&xSendDBStatistics);

    return true;
}

// ============================================================================
// Response Handlers (from DB/Server)
// ============================================================================

// IDA: ?ResHelperSupportInfo@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_INFO_RES@@@Z (0x140098280)
// 对齐 IDA: 处理支援信息响应
void CGocHelper::ResHelperSupportInfo(PS_HELPER_SUPPORT_INFO_RES& psInfo) {
    // IDA: CGocHelper::SetMySupportInfo(this, psInfo);
    SetMySupportInfo(psInfo);

    // TODO: 汇编还原 - 发送响应给客户端
    // XSendPacket xSendPacket(0x27, 5);
    // xSendPacket << psInfo;
    // CGocNetwork::Send(pOwner, &xSendPacket);
}

// IDA: ?ResHelperSupportRegister@CGocHelper@@QEAAXAEAUPS_SERVER_HELPER_SUPPORT_REGISTER@@@Z (0x140098370)
// 对齐 IDA: 处理支援注册响应
void CGocHelper::ResHelperSupportRegister(PS_SERVER_HELPER_SUPPORT_REGISTER& psSupport) {
    // TODO: 汇编还原 - 完整逻辑
    // PS_HELPER_SUPPORT_REGISTER_RES psResult;
    // psResult.nResult = psSupport->nResult;
    //
    // if (!psSupport->nResult) {
    //     psResult.stMyInfo = psSupport->stInfo;
    //     psResult.stMyInfo.bRegister = 1;
    //     psResult.stMyInfo.byRewardType = 1;
    //     SetMySupportInfo(psResult.stMyInfo);
    //
    //     // 记录日志
    //     ST_LOG_GAME stLog;
    //     stLog._nUAID = pUser->GetUAID();
    //     stLog._nUCID = pUser->GetUCID();
    //     stLog._sMainType = 16;
    //     stLog._sSubType = 8;
    //     stLog.nParam2 = pUser->GetLevel();
    //     stLog.nParam3 = psResult.stMyInfo.stInfo.bySupportType;
    //     XGameServer::Instance()->SendDBLog(&stLog);
    // }
    //
    // XSendPacket xSendPacket(0x27, 6);
    // xSendPacket << psSupport;
    // CGocNetwork::Send(pOwner, &xSendPacket);
}

// IDA: ?ResHelperSupportReward@CGocHelper@@QEAAXAEAUPS_SERVER_HELPER_SUPPORT_REWARD@@@Z (0x140098600)
// 对齐 IDA: 处理支援奖励响应
void CGocHelper::ResHelperSupportReward(PS_SERVER_HELPER_SUPPORT_REWARD& psReward) {
    // IDA: if (psReward->nResult) { m_byMySupportReward = 0; }
    if (psReward.nResult) {
        m_byMySupportReward = 0;
    } else {
        // TODO: 汇编还原 - 创建物品并添加好友点数
        // std::tr1::shared_ptr<CGocInventory> pInven = pOwner->GetGOC<CGocInventory>();
        // if (pInven) {
        //     ST_LOG_GAME stLogData;
        //     if (!pInven->CreateItemReq(psReward->stCreateItems, 0, 15, &stLogData)) {
        //         CGocNetwork::SendErrorMessage(pOwner, 0x27, 7, 0xCB2A);
        //         return;
        //     }
        //     m_byMySupportReward = 2;
        //     pInven->AddTotalFriendPoint(psReward->wFriendPoint, 1);
        // }
    }

    // TODO: 汇编还原 - 发送响应给客户端
    // XSendPacket xSendPacket(0x27, 7);
    // xSendPacket << psResult;
    // CGocNetwork::Send(pOwner, &xSendPacket);
}

// IDA: ?ResHelperSupportList@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_LIST_RES@@@Z (0x140098850)
// 对齐 IDA: 处理支援列表响应
void CGocHelper::ResHelperSupportList(PS_HELPER_SUPPORT_LIST_RES& psList) {
    // TODO: 汇编还原 - 发送响应给客户端
    // XSendPacket xSendPacket(0x27, 8);
    // xSendPacket << psList;
    // CGocNetwork::Send(pOwner, &xSendPacket);
}

// IDA: ?ResHelperSupportEquip@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_EQUIP_RES@@@Z (0x140098920)
// 对齐 IDA: 处理支援装备响应
void CGocHelper::ResHelperSupportEquip(PS_HELPER_SUPPORT_EQUIP_RES& psEquip) {
    // IDA: 如果成功，更新助手的好友支援信息
    if (!psEquip.nResult) {
        auto it = m_mapHelper.find(psEquip.dwHelperID);
        if (it != m_mapHelper.end()) {
            ST_HELPER_INFO& stHelper = it->second;
            // IDA: qmemcpy(&stHelper->stFriendSupport, &psEquip->stHelper.stFriendSupport, sizeof(stHelper->stFriendSupport));
            std::memcpy(&stHelper.stFriendSupport, &psEquip.stHelper.stFriendSupport, sizeof(stHelper.stFriendSupport));
            CalcHelperStatsALL(&stHelper, 1);

            // TODO: 汇编还原 - 记录日志
            // ST_LOG_GAME stLog;
            // stLog._nUAID = pUser->GetUAID();
            // stLog._nUCID = pUser->GetUCID();
            // stLog._sMainType = 16;
            // stLog._sSubType = 9;
            // stLog.nParam0 = psEquip.dwHelperID;
            // stLog.nParam1 = psEquip.stHelper.stFriendSupport.dwFriendUCID;
            // stLog.nParam2 = pUser->GetLevel();
            // stLog.nParam3 = psEquip.stHelper.stFriendSupport.bySupportType;
            // XGameServer::Instance()->SendDBLog(&stLog);
        }
    }

    // TODO: 汇编还原 - 发送响应给客户端
    // PS_HELPER_EQUIP_RES psResult;
    // psResult.nResult = psEquip.nResult;
    // psResult.dwHelperID = psEquip.dwHelperID;
    // psResult.stHelper = stHelper;
    // XSendPacket xSendPacket(0x27, 9);
    // xSendPacket << psResult;
    // CGocNetwork::Send(pOwner, &xSendPacket);
}

// IDA: ?ResHelperSupportEquipReward@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_EQUIP_REWARD_RES@@@Z (0x140098ca0)
// 对齐 IDA: 处理支援装备奖励响应
void CGocHelper::ResHelperSupportEquipReward(PS_HELPER_SUPPORT_EQUIP_REWARD_RES& psReward) {
    // TODO: 汇编还原 - 添加好友点数
    // std::tr1::shared_ptr<CGocFriend> pFriend = pOwner->GetGOC<CGocFriend>();
    // if (pFriend) {
    //     pFriend->AddFriendPoint(psReward.dwFriendUCID, psReward.wFriendPointReward);
    // }
}

// IDA: ?ResHelperEquip@CGocHelper@@QEAAXAEAUPS_DB_HELPER_EQUIP_RES@@@Z (0x140098d20)
// 对齐 IDA: 处理装备响应 (复杂 ~4KB)
void CGocHelper::ResHelperEquip(PS_DB_HELPER_EQUIP_RES& psRes) {
    // IDA: 如果有错误，直接返回
    if (psRes.nError) {
        return;
    }

    // IDA: 查找助手
    auto it = m_mapHelper.find(psRes.psEquip.dwHelperID);
    if (it == m_mapHelper.end()) {
        return;
    }

    ST_HELPER_INFO& stHelper = it->second;

    // TODO: 汇编还原 - 复杂的装备交换逻辑 (~400行)
    // 主要流程:
    // 1. 如果 xInvenSerial && xHelperSerial: 交换背包和助手物品
    // 2. 如果 xInvenSerial only: 从背包装备到助手空槽
    // 3. 如果 xHelperSerial only: 从助手卸下到背包空槽
    // 4. 更新助手属性并发送响应

    // 简化实现 - 更新属性
    CalcHelperStatsALL(&stHelper, 1);

    // TODO: 汇编还原 - 发送响应给客户端
    // PS_HELPER_EQUIP_RES psResult;
    // psResult.nResult = psRes.nError;
    // psResult.stHelper = stHelper;
    // XSendPacket xSendPacket(0x27, 0x10);
    // xSendPacket << psResult;
    // CGocNetwork::Send(pOwner, &xSendPacket);
}

// IDA: ?ResHelperSupportRelease@CGocHelper@@QEAAXAEAUPS_DB_HELPER_SUPPORT_RELEASE@@@Z (0x140099d60)
// 对齐 IDA: 处理支援释放响应
void CGocHelper::ResHelperSupportRelease(PS_DB_HELPER_SUPPORT_RELEASE& psRelease) {
    // IDA: 如果成功，清除好友支援信息
    if (!psRelease.nResult) {
        auto it = m_mapHelper.find(psRelease.dwHelperID);
        if (it != m_mapHelper.end()) {
            ST_HELPER_INFO& stHelper = it->second;
            // IDA: 清除 stFriendSupport
            memset(&stHelper.stFriendSupport, 0, sizeof(stHelper.stFriendSupport));
            UnEquipHelperFriendItemStats(psRelease.dwHelperID);
            CalcHelperStatsALL(&stHelper, 1);

            // TODO: 汇编还原 - 发送响应给客户端
            // XSendPacket xSendPacket(0x27, 0x11);
            // xSendPacket << stHelper;
            // CGocNetwork::Send(pOwner, &xSendPacket);
        }
    }
}

// IDA: ?ResHelperChangeOrder@CGocHelper@@QEAAXAEAUPS_HELPER_CHANGE_ORDER@@@Z (0x140099f60)
// 对齐 IDA: 处理顺序变更响应
void CGocHelper::ResHelperChangeOrder(PS_HELPER_CHANGE_ORDER& psOrder) {
    // IDA: 如果成功，更新顺序
    if (!psOrder.nError) {
        auto it1 = m_mapHelper.find(psOrder.dwHelperID_1);
        auto it2 = m_mapHelper.find(psOrder.dwHelperID_2);

        if (it1 != m_mapHelper.end() && it2 != m_mapHelper.end()) {
            // IDA: 交换顺序
            it1->second.byOrder = psOrder.byOrder_1;
            it2->second.byOrder = psOrder.byOrder_2;
        }
    }

    // TODO: 汇编还原 - 发送响应给客户端
    // XSendPacket xSendPacket(0x27, 0x12);
    // xSendPacket << psOrder;
    // CGocNetwork::Send(pOwner, &xSendPacket);
}

// IDA: ?ResHelperChangeAutoSummon@CGocHelper@@QEAAXAEAUPS_HELPER_CHANGE_AUTO_SUMMON@@@Z (0x14009a140)
// 对齐 IDA: 处理自动召唤变更响应
void CGocHelper::ResHelperChangeAutoSummon(PS_HELPER_CHANGE_AUTO_SUMMON& psFlag) {
    // IDA: 如果 bySelect 是 0 或 1，更新标志
    if (!psFlag.bySelect || psFlag.bySelect == 1) {
        SetAutoSummonFlag(psFlag.bySelect);
    }

    // IDA: psFlag->bySelect = IsAutoSummon();
    psFlag.bySelect = IsAutoSummon() ? 1 : 0;

    // TODO: 汇编还原 - 发送响应给客户端
    // XSendPacket xSendPacket(0x27, 0x15);
    // xSendPacket << psFlag;
    // CGocNetwork::Send(pOwner, &xSendPacket);
}

// IDA: ?SendDBHelperList@CGocHelper@@QEAAXXZ (0x14009A670)
// 对齐 IDA: 发送助手列表请求到数据库
// IDA 反编译:
// void __fastcall CGocHelper::SendDBHelperList(CGocHelper *this)
// {
//   VBitmask *v1; // rax
//   int QuestID; // eax
//   XGameServer *v3; // rax
//   XSendDBPacket xSendDBPacket; // [rsp+30h] [rbp-10058h] BYREF
//   _BYTE v5[8]; // [rsp+10050h] [rbp-38h] BYREF
//   __int64 v6; // [rsp+10058h] [rbp-30h]
//   VChunkFile *v7; // [rsp+10060h] [rbp-28h]
//   IXObject *pObject; // [rsp+10068h] [rbp-20h]
//   DynArray_cl<int> *p_m_ChunkSizeTempMemOfs; // [rsp+10070h] [rbp-18h]
//
//   v6 = -2;
//   v7 = std::list<CBattleZone *>::size((VChunkLocker *)this);
//   if ( v7 )
//     pObject = (IXObject *)&v7[3].m_ChunkSizeTempMemOfs;
//   else
//     pObject = nullptr;
//   XSendDBPacket::XSendDBPacket(&xSendDBPacket, pObject, 0x26u, 1u);
//   p_m_ChunkSizeTempMemOfs = &std::list<CBattleZone *>::size((VChunkLocker *)this)[3].m_ChunkSizeTempMemOfs;
//   v1 = (VBitmask *)((__int64 (__fastcall *)(DynArray_cl<int> *, _BYTE *))p_m_ChunkSizeTempMemOfs->__vftable[7].dtr_DynArray_cl<int>)(p_m_ChunkSizeTempMemOfs, v5);
//   QuestID = CQuestCondition::GetQuestID(v1);
//   XParse::operator<<(&xSendDBPacket.XParse, QuestID);
//   v3 = TXSingleton<XGameServer>::Instance();
//   XGameServer::SendDBGame(v3, &xSendDBPacket);
//   XSendDBPacket::~XSendDBPacket((VGameProjectileObject *)&xSendDBPacket);
// }
void CGocHelper::SendDBHelperList()
{
    // TODO: 需要实现 - 获取 owner 对象
    // CMover* pOwner = GetOwnerGO();
    // if (!pOwner) return;

    // 获取 UCID
    // std::uint32_t dwUCID = CQuestCondition::GetQuestID(...);

    // 发送 DB 请求 (Main=0x26, Sub=1)
    // XSendDBPacket xSendDBPacket(pOwner, 0x26, 1);
    // xSendDBPacket << dwUCID;
    // XGameServer::Instance()->SendDBGame(&xSendDBPacket);
}
