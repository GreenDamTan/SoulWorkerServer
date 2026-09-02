// ForceProcess.cpp
// XForceProcess - Force/Guild 客户端请求处理器实现
// 对齐 IDA GameServer.exe；原始归属 process/ForceProcess.cpp
// (PDB cvdump MD5 0CE935F8D4EFBEDB2196DCD00B793D89)

#include "ForceProcess.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocInventory.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocQuest.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocRecode.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAttribute.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocEntity.h"
#include "Soulworker/GameServer/XGameServer/actor/component/XBaseInventory.h"
#include "Soulworker/GameServer/XGameServer/Item/CItem.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocParty.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocForce.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocFriend.h"
#include "Soulworker/GameServer/XGameServer/BattleZone.h"
#include "Soulworker/GameServer/XGameServer/ThreadLocalData.h"
#include "Soulworker/GameServer/XGameServer/XForceManager.h"
#include "Soulworker/GameServer/XGameServer/CForce.h"
#include "Soulworker/GameServer/XGameServer/CParty.h"
#include "Soulworker/GameServer/XCore/XArea/XDistrict.h"
#include "Soulworker/GameServer/XGameServer/SocialItemObject.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h"
#include "Soulworker/GameServer/XGameServer/XWorldResMgr.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"

// IDA: ??0XForceProcess@@QEAA@XZ @ 0x140430B00
XForceProcess::XForceProcess() {
}

// IDA: ??1XForceProcess@@UEAA@XZ @ 0x140430BC0
XForceProcess::~XForceProcess() {
}

// IDA: ?Parse@XForceProcess@@UEAA_NAEAVXPacket@@@Z @ 0x140430BF0
// subcmd 分发：1 邀请 / 2 接受 / 3 换队长 / 4 踢出 / 5 离开 /
// 8 取消 / 0x30 匹配进入 / 0x31 匹配退出 / 0x32 匹配检查，未知返回 1。
bool XForceProcess::Parse(XPacket& xPacket) {
    switch (xPacket.GetSubCmd()) {
        case 1: return ReqForceInvite(xPacket);
        case 2: return ReqForceAccept(xPacket);
        case 3: return ReqForceChangeMaster(xPacket);
        case 4: return ReqForceKickOut(xPacket);
        case 5: return ReqForceLeave();
        case 8: return ReqForceCancel(xPacket);
        case 0x30: return ReqForceMatchingEnter(xPacket);
        case 0x31: return ReqForceMatchingExit();
        case 0x32: return ReqForceMatchingCheck(xPacket);
        default: return true;
    }
}

// ============================================================================
// CheckForceMatchingEnterUser (0x140435020)
// 状态: 已精确还原 - 匹配进入前的单人条件校验。
// 校验链：迷宫表 -> 灵魂武器 -> 需求物品 -> 每日次数 -> 任务 ->
// 通关记录 -> 最低等级 -> 疲劳点 -> 交易状态 -> 社交物品。
// ============================================================================
bool XForceProcess::CheckForceMatchingEnterUser(CUser* pUser, std::uint16_t wMazeID,
                                                int& nError, int* nNeedItemID) {
    if (!pUser) {
        nError = 51001;
        return false;
    }

    TB_MAZE_INFO* pMazeData = XGameServer::Instance()->GetResourceMgr().GetTB_MAZE_INFO(wMazeID);
    if (!pMazeData) {
        nError = 55008;
        return false;
    }

    // Per IDA: 未装备灵魂武器 (dwItemID == -1)
    if (pUser->stMyCharInfoEx()->stSoulWeapon.dwItemID == -1) {
        nError = 53136;
        return false;
    }

    // Per IDA: 迷宫需求物品校验
    bool bCheckItem = false;
    if (pMazeData->NeedItem_ID) {
        TB_ITEM* pTBItem = XGameServer::Instance()->GetResourceMgr().GetTB_ITEM(pMazeData->NeedItem_ID);
        if (pTBItem) {
            TB_ITEM_CLASSIFY* pTBClassify = XGameServer::Instance()->GetResourceMgr()
                .GetTB_ITEM_CLASSIFY(pTBItem->Item_Classify_Index);
            if (pTBClassify) {
                CGocInventory* pGocInventory = pUser->GetGOC<CGocInventory>();
                if (pGocInventory) {
                    XBaseInventory* pInventory = pGocInventory->GetTBInvenPtr(pTBClassify->Item_Inven_Type);
                    if (pInventory) {
                        int nNeedCount = pMazeData->NeedItem_Value;
                        std::vector<std::shared_ptr<CItem>> vecItems;
                        pInventory->GetSameItems_2(static_cast<int>(pMazeData->NeedItem_ID), &vecItems, -1);
                        for (const auto& pItem : vecItems) {
                            if (!pItem) {
                                continue;
                            }
                            nNeedCount -= pItem->GetCount();
                            if (nNeedCount <= 0) {
                                bCheckItem = true;
                                pGocInventory->SetMazeNeedItemID(static_cast<int>(pMazeData->NeedItem_ID));
                                break;
                            }
                        }
                    }
                }
            }
            if (!bCheckItem) {
                nError = 55035;
                *nNeedItemID = static_cast<int>(pMazeData->NeedItem_ID);
                return false;
            }
        }
    }

    // Per IDA: 每日进入次数校验
    int nErrorID = 0;
    if (!pUser->CheckMazeEnterCount(pMazeData, nErrorID)) {
        nError = nErrorID;
        return false;
    }

    // Per IDA: 需求任务 - 进行中或已完成均可
    if (pMazeData->NeedQuest_ID) {
        CGocQuest* pQuest = pUser->GetGOC<CGocQuest>();
        if (!pQuest
            || (!pQuest->FindEpisode(pMazeData->NeedQuest_ID)
                && !pQuest->IsCompleteEpisode(pMazeData->NeedQuest_ID))) {
            nError = 55037;
            return false;
        }
    }

    // Per IDA: 前置迷宫通关记录
    CGocRecode* pRecode = pUser->GetGOC<CGocRecode>();
    if (!pRecode || !pRecode->IsClearMaze(static_cast<int>(pMazeData->Check_Clear_Maze))) {
        nError = 53127;
        return false;
    }

    // Per IDA: 最低等级
    if (pMazeData->Req_Min_Lv > pUser->GetLevel()) {
        nError = 53123;
        return false;
    }

    // Per IDA: 疲劳点
    if (pMazeData->Fatigue_Point) {
        CGocAttribute* pAttr = pUser->GetGOC<CGocAttribute>();
        if (!pAttr || !pAttr->CanUseFP(pMazeData->Fatigue_Point)) {
            nError = 55044;
            return false;
        }
    }

    // Per IDA: 交易中禁止进入
    CGocInventory* pGocInventory = pUser->GetGOC<CGocInventory>();
    if (!pGocInventory || pGocInventory->GetTradeState()) {
        nError = 55002;
        return false;
    }

    // Per IDA: 社交物品使用中（类型 3）禁止进入
    XArea* pArea = pUser->GetArea();
    if (pArea) {
        XDistrict* pDistrict = dynamic_cast<XDistrict*>(pArea);
        if (pDistrict) {
            const std::uint32_t dwSocialUseID = pUser->GetSocialUseID();
            if (dwSocialUseID) {
                CSocialItemObject* pSocialObject = pDistrict->FindSocialItemObject(dwSocialUseID);
                if (pSocialObject
                    && static_cast<std::uint8_t>(pSocialObject->GetSocialType()) == 3) {
                    nError = 55093;
                    return false;
                }
            }
        }
    }

    return true;
}

// ============================================================================
// ReqForceInvite (0x140430D40) - 已精确还原
// 邀请请求：解析 PS_REQ_PARTY_INVITE + bySelect；封包序号非法直接踢出
// (kickType 8)；lambda0 在玩家线程执行完整校验链（迷宫中/匹配中/已有队伍/
// 区域限制/已在 Force/非队长/满员(8)/Force 在迷宫中/邀请自己/被屏蔽），
// 通过则 (0xFA,0xB) 转发 CommunitySocket 邀请。
// ============================================================================
bool XForceProcess::ReqForceInvite(XPacket& xPacket) {
    PS_REQ_PARTY_INVITE stInvite;
    std::uint8_t bySelect = 0;

    xPacket >> stInvite;
    xPacket.XParse >> bySelect;

    CUser* pUser = GetClientPtr();

    // Per IDA: CWayPoint::GetCurID(&xPacket+5) 非零表示非法包序号 - 报错并踢出
    if (xPacket.GetCurID()) {
        SendErrorMessage(1, 0xC3B6);
        PS_KICK_USER_INFO psKick = {};
        psKick.dwUAID = pUser->GetUAID();
        psKick.byKickType = 8;
        pUser->Kickout(&psKick, false);
        return true;
    }

    // Per IDA lambda0 (0x1404310B0): 玩家线程执行邀请校验
    std::function<void()> func = [pUser, stInvite, bySelect]() {
        if (!pUser || !pUser->IsLive() || !pUser->GetArea()) {
            return;
        }

        // Per IDA: 迷宫中禁止邀请
        if (pUser->IsMaze()) {
            pUser->SendErrorMessage(0x2E, 1, 53102);
            return;
        }
        // Per IDA: 匹配中禁止邀请
        if (pUser->IsMatching()) {
            pUser->SendErrorMessage(0x2E, 1, 53131);
            return;
        }
        // Per IDA: 已在队伍中
        CGocParty* pGocParty = pUser->GetGOC<CGocParty>();
        if (pGocParty && pGocParty->IsParty() && pGocParty->GetPartyID() > 0) {
            pUser->SendErrorMessage(0x2E, 1, 53145);
            return;
        }
        // Per IDA: 区域禁止使用 Force（Force_Use==1）或战场区域
        if (pUser->GetArea()->GetWorldType() != 0) {
            pUser->SendErrorMessage(0x2E, 1, 53147);
            return;
        }
        TB_DISTRICT* pDistrict = XGameServer::Instance()->GetResourceMgr()
            .GetTB_DISTRICT(pUser->GetArea()->GetTBMapID());
        if (!pDistrict || pDistrict->Force_Use == 1) {
            pUser->SendErrorMessage(0x2E, 1, 53147);
            return;
        }

        std::uint32_t dwForceID = 0;
        // Per IDA: 已在 Force 中 - 需为队长且未满员且 Force 不在迷宫中
        if (ThreadLocalData::GetInstance()->GetForceMgr()->IsForceUser(pUser->GetActorID())) {
            if (!ThreadLocalData::GetInstance()->GetForceMgr()->IsMasterUser(pUser->GetActorID())) {
                pUser->SendErrorMessage(0x2E, 1, 53103);
                return;
            }
            const std::uint32_t dwMyForceID =
                ThreadLocalData::GetInstance()->GetForceMgr()->GetForceID(pUser->GetActorID());
            if (ThreadLocalData::GetInstance()->GetForceMgr()->GetUserCount(dwMyForceID) >= 8) {
                pUser->SendErrorMessage(0x2E, 1, 53110);
                return;
            }
            std::shared_ptr<CForce> pForce =
                ThreadLocalData::GetInstance()->GetForceMgr()->GetForce(pUser->GetActorID());
            if (!pForce || pForce->GetMazeID().nMapID > 0) {
                pUser->SendErrorMessage(0x2E, 1, 53116);
                return;
            }
        }

        // Per IDA: 不能邀请自己
        const std::wstring strMyName = pUser->GetName();
        const wchar_t* szMyName = strMyName.c_str();
        const wchar_t* szTargetName = stInvite.strReqName;
        bool bSelfInvite = true;
        size_t i = 0;
        while (true) {
            const wchar_t c1 = szMyName[i];
            const wchar_t c2 = szTargetName[i];
            if (c1 != c2) {
                bSelfInvite = false;
                break;
            }
            if (c1 == L'\0') {
                bSelfInvite = true;
                break;
            }
            ++i;
        }
        if (bSelfInvite) {
            pUser->SendErrorMessage(0x2E, 1, 53134);
            return;
        }

        // Per IDA: 目标玩家屏蔽了邀请人则拒绝
        CGocFriend* pFriend = pUser->GetGOC<CGocFriend>();
        if (pFriend && pFriend->IsBlockByName(stInvite.strReqName)) {
            pUser->SendErrorMessage(0x2E, 1, 53013);
            return;
        }

        // Per IDA: (0xFA,0xB) 转发邀请到 CommunitySocket
        PS_REQ_FORCE_INVITE stNewInvite = {};
        stNewInvite.dwReqActorID = pUser->GetActorID().dwActorID;
        const std::wstring strName = pUser->GetName();
        const wchar_t* szSrc = strName.c_str();
        wchar_t* szDst = stNewInvite.strReqName;
        do {
            *szDst = *szSrc;
            ++szSrc;
            ++szDst;
        } while (*szSrc);

        XSendPacket xSendPacket(0xFA, 0xB);
        xSendPacket << stNewInvite;
        xSendPacket.XParse << static_cast<std::uint32_t>(pUser->GetUAID());
        xSendPacket.XParse << static_cast<std::uint8_t>(pUser->GetLevel());
        xSendPacket.XParse << dwForceID;
        XGameServer::Instance()->GetCommunitySocket().SendCmd(
            &xSendPacket, pUser, 0x2E, 1);
    };

    if (pUser && pUser->GetArea()) {
        pUser->IncrementJobCount();
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);
        std::function<void()> funcDec = [pUser]() {
            pUser->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
        return true;
    }
    return false;
}

// ============================================================================
// ReqForceAccept (0x1404319F0) - 已精确还原
// 解析 dwMasterID；无 GetCurID 门控；pUser 空 / GetArea() 空返回 false；
// lambda2 (0x140431BD0) 在玩家线程执行完整接受校验链：
// IsMaze 53102 / IsMatching 53131 / 战场区域(WorldType==2)非安全区
// -> (0xFA,0xD) PS_FORCE_REJECT 53119 + 错误 53133 / 区域 Force_Use 53147 /
// 已在 Force (CGocForce IsParty) 53104 / 接受自己 53134 / 已有队伍
// (CGocParty IsParty) 53004 / 通过 -> PS_RES_FORCE_INVITE{dwAcceptID, 0}
// + (0xFA,0xC) + UAID + Level 经由 CCommunitySocket::SendCmd(0x2E,2)。
// ============================================================================
bool XForceProcess::ReqForceAccept(XPacket& xPacket) {
    int nMasterID = 0;
    xPacket.XParse >> nMasterID;

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }
    if (!pUser->GetArea()) {
        return false;
    }

    // Per IDA lambda2 (0x140431BD0): 玩家线程执行接受校验
    std::function<void()> func = [pUser, nMasterID]() {
        if (!pUser || !pUser->IsLive() || !pUser->GetArea()) {
            return;
        }

        // Per IDA: 迷宫中禁止接受
        if (pUser->IsMaze()) {
            pUser->SendErrorMessage(0x2E, 2, 53102);
            return;
        }
        // Per IDA: 匹配中禁止接受
        if (pUser->IsMatching()) {
            pUser->SendErrorMessage(0x2E, 2, 53131);
            return;
        }

        // Per IDA: 战场区域需处于安全区才能接受
        if (pUser->GetArea()->GetWorldType() == 2) {
            PS_FORCE_REJECT stReject = {};
            stReject.dwRejectID = static_cast<std::uint32_t>(pUser->GetActorID().dwActorID);
            stReject.dwReqActor = static_cast<std::uint32_t>(nMasterID);
            const std::wstring strName = pUser->GetName();
            const wchar_t* szSrc = strName.c_str();
            wchar_t* szDst = stReject.strRejectName;
            do {
                *szDst = *szSrc;
                ++szSrc;
                ++szDst;
            } while (*szSrc);
            stReject.dwErrorID = 53119;

            CBattleZone* pBattleZone = dynamic_cast<CBattleZone*>(pUser->GetArea());
            if (!pBattleZone || !pBattleZone->IsInSafetyZone(pUser)) {
                XSendPacket xSendPacket(0xFA, 0xD);
                xSendPacket << stReject;
                XGameServer::Instance()->GetCommunitySocket().SendCheck(&xSendPacket);
                pUser->SendErrorMessage(0x2E, 2, 53133);
                return;
            }
        }

        // Per IDA: 区域禁止使用 Force (Force_Use==1)
        TB_DISTRICT* pDistrict = XGameServer::Instance()->GetResourceMgr()
            .GetTB_DISTRICT(pUser->GetArea()->GetTBMapID());
        if (!pDistrict || pDistrict->Force_Use == 1) {
            pUser->SendErrorMessage(0x2E, 2, 53147);
            return;
        }

        // Per IDA: 已在 Force 中
        CGocForce* pGocForce = pUser->GetGOC<CGocForce>();
        if (!pGocForce || pGocForce->IsParty()) {
            pUser->SendErrorMessage(0x2E, 2, 53104);
            return;
        }

        // Per IDA: 不能接受自己的邀请
        if (static_cast<std::uint32_t>(nMasterID) == pUser->GetActorID().dwActorID) {
            pUser->SendErrorMessage(0x2E, 2, 53134);
            return;
        }

        // Per IDA: 已在队伍中
        CGocParty* pGocParty = pUser->GetGOC<CGocParty>();
        if (!pGocParty || pGocParty->IsParty()) {
            pUser->SendErrorMessage(0x2E, 2, 53004);
            return;
        }

        // Per IDA: (0xFA,0xC) 转发接受结果到 CommunitySocket
        PS_RES_FORCE_INVITE stAccept = {};
        stAccept.dwAcceptID = pUser->GetActorID().dwActorID;
        stAccept.nResult = 0;
        XSendPacket xSendPacket(0xFA, 0xC);
        xSendPacket << stAccept;
        xSendPacket.XParse << static_cast<std::uint32_t>(pUser->GetUAID());
        xSendPacket.XParse << static_cast<std::uint8_t>(pUser->GetLevel());
        XGameServer::Instance()->GetCommunitySocket().SendCmd(
            &xSendPacket, pUser, 0x2E, 2);
    };

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
    return true;
}

// ============================================================================
// ReqForceChangeMaster (0x140432790) - 已精确还原
// 解析 dwMasterID；lambda6 (0x140432980) 在玩家线程执行：
// GetGOC<CGocForce> 空跳过；否则 CGocForce::ChangeMaster(dwMasterID)。
// ============================================================================
bool XForceProcess::ReqForceChangeMaster(XPacket& xPacket) {
    std::uint32_t dwMasterID = 0;
    xPacket.XParse >> dwMasterID;

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }
    if (!pUser->GetArea()) {
        return false;
    }

    // Per IDA lambda6 (0x140432980): 玩家线程执行更换队长
    std::function<void()> func = [pUser, dwMasterID]() {
        if (!pUser || !pUser->IsLive()) {
            return;
        }
        CGocForce* pGocForce = pUser->GetGOC<CGocForce>();
        if (!pGocForce) {
            return;
        }
        pGocForce->ChangeMaster(dwMasterID);
    };

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
    return true;
}

// ============================================================================
// ReqForceKickOut (0x140432A20) - 已精确还原
// 解析 dwActorID；lambda8 (0x140432C10) 在玩家线程执行：
// GetGOC<CGocForce> 空跳过；IsMatchingDate -> SendErrorMessage(0x12,4,53152)；
// 否则 CGocForce::KickOut(dwActorID, pUser)。
// ============================================================================
bool XForceProcess::ReqForceKickOut(XPacket& xPacket) {
    std::uint32_t dwActorID = 0;
    xPacket.XParse >> dwActorID;

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }
    if (!pUser->GetArea()) {
        return false;
    }

    // Per IDA lambda8 (0x140432C10): 玩家线程执行踢出校验
    std::function<void()> func = [pUser, dwActorID]() {
        if (!pUser || !pUser->IsLive()) {
            return;
        }
        CGocForce* pGocForce = pUser->GetGOC<CGocForce>();
        if (!pGocForce) {
            return;
        }
        // Per IDA: 匹配中禁止踢出
        if (pGocForce->IsMatchingDate()) {
            pUser->SendErrorMessage(0x12, 4, 53152);
            return;
        }
        pGocForce->KickOut(dwActorID, pUser);
    };

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
    return true;
}

// ============================================================================
// ReqForceLeave (0x140432D40) - 已精确还原
// 无参数；lambda10 (0x140432EF0) 在玩家线程执行：
// GetGOC<CGocForce> 空跳过；IsMatchingDate -> SendErrorMessage(0x2E,5,53152)；
// 否则 CGocForce::Leave()。
// ============================================================================
bool XForceProcess::ReqForceLeave() {
    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }
    if (!pUser->GetArea()) {
        return false;
    }

    // Per IDA lambda10 (0x140432EF0): 玩家线程执行离开逻辑
    std::function<void()> func = [pUser]() {
        if (!pUser || !pUser->IsLive()) {
            return;
        }
        CGocForce* pGocForce = pUser->GetGOC<CGocForce>();
        if (!pGocForce) {
            return;
        }
        // Per IDA: 匹配中禁止离开
        if (pGocForce->IsMatchingDate()) {
            pUser->SendErrorMessage(0x2E, 5, 53152);
            return;
        }
        pGocForce->Leave();
    };

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
    return true;
}

// ============================================================================
// ReqForceCancel (0x140432370) - 已精确还原
// 解析 dwReqID + dwErrorID；lambda4 (0x140432580) 在玩家线程执行：
// 构造 PS_FORCE_REJECT{dwRejectID=自己, dwReqActor=dwReqID, 自己名字,
// dwErrorID} 经 (0xFA,0xD) 经由 CCommunitySocket::SendCheck 转发到
// RelayServer，通知邀请方取消。
// ============================================================================
bool XForceProcess::ReqForceCancel(XPacket& xPacket) {
    int nReqID = 0;
    std::uint32_t dwErrorID = 0;
    xPacket.XParse >> nReqID;
    xPacket.XParse >> dwErrorID;

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }
    if (!pUser->GetArea()) {
        return false;
    }

    // Per IDA lambda4 (0x140432580): 玩家线程转发取消通知
    std::function<void()> func = [pUser, nReqID, dwErrorID]() {
        if (!pUser || !pUser->IsLive() || !pUser->GetArea()) {
            return;
        }

        PS_FORCE_REJECT stReject = {};
        stReject.dwRejectID = static_cast<std::uint32_t>(pUser->GetActorID().dwActorID);
        stReject.dwReqActor = static_cast<std::uint32_t>(nReqID);
        const std::wstring strName = pUser->GetName();
        const wchar_t* szSrc = strName.c_str();
        wchar_t* szDst = stReject.strRejectName;
        do {
            *szDst = *szSrc;
            ++szSrc;
            ++szDst;
        } while (*szSrc);
        stReject.dwErrorID = dwErrorID;

        XSendPacket xSendPacket(0xFA, 0xD);
        xSendPacket << stReject;
        XGameServer::Instance()->GetCommunitySocket().SendCheck(&xSendPacket);
    };

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
    return true;
}

// ============================================================================
// ReqForceMatchingEnter (0x140432FD0) - 已精确还原
// 解析 dwMapID + dwPortalID；lambda12 (0x140433250) 在玩家线程执行完整
// 匹配进入链（捕获 CUser*/dwMapID/dwPortalID/this）：
// IsMatching 53131 -> 迷宫表空或无 Area 55003 -> GetStartPortalID 失败 55003
// -> CheckMazeOpenTime 55060 -> 非组队匹配迷宫(Maze_Type!=9/人数!=5) 55003
// -> CanEnterPortal 55007 -> Relay 未就绪 50003 -> CheckForceMatchingEnter 组级
// 校验 -> 构造 PS_SERVER_FORCE_MATCHING_ENTER + 队长 MEMBER 信息经
// (0xFA,0x13) SendCmd(0x2E,0x30)；失败则 LogError 514 + 需求物品错误
// (4,0x41,0xD6FB) 或 (0x2E,0x30,nErrorID)。
// ============================================================================
bool XForceProcess::ReqForceMatchingEnter(XPacket& xPacket) {
    std::uint32_t dwMapID = 0;
    std::uint32_t dwPortalID = 0;
    xPacket.XParse >> dwMapID;
    xPacket.XParse >> dwPortalID;

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }
    if (!pUser->GetArea()) {
        return false;
    }

    // Per IDA lambda12 (0x140433250): 玩家线程执行匹配进入
    std::function<void()> func = [this, pUser, dwMapID, dwPortalID]() {
        if (!pUser || !pUser->IsLive() || !pUser->GetArea()) {
            return;
        }

        // Per IDA: 匹配中禁止再次进入
        if (pUser->IsMatching()) {
            pUser->SendErrorMessage(0x2E, 0x30, 53131);
            return;
        }

        ST_CREATE_MAZE stCreateMaze;
        stCreateMaze.wReqMapID = static_cast<std::uint16_t>(dwMapID);

        TB_MAZE_INFO* pMazeData = XGameServer::Instance()->GetResourceMgr()
            .GetTB_MAZE_INFO(stCreateMaze.wReqMapID);
        if (!pMazeData || !pUser->GetArea()) {
            pUser->SendErrorMessage(0x2E, 0x30, 55003);
            return;
        }

        STPosInfo posInfo;
        int nJumpID = 0;
        if (!XGameServer::Instance()->GetWorldResMgr().GetStartPortalID(
                static_cast<int>(stCreateMaze.wReqMapID), &nJumpID, &posInfo)) {
            pUser->SendErrorMessage(0x2E, 0x30, 55003);
            return;
        }

        // Per IDA: 迷宫开放时间校验
        if (!XGameServer::Instance()->GetResourceMgr().CheckMazeOpenTime(dwMapID)) {
            pUser->SendErrorMessage(0x2E, 0x30, 55060);
            return;
        }

        // Per IDA: 仅组队匹配迷宫 (Maze_Type==9 且 Admission_Member==5)
        if (pMazeData->Maze_Type != 9 || pMazeData->Admission_Member != 5) {
            pUser->SendErrorMessage(0x2E, 0x30, 55003);
            return;
        }

        // Per IDA: 传送门进入校验
        if (!XGameServer::Instance()->GetWorldResMgr().CanEnterPortal(
                static_cast<int>(pUser->GetMapInsID().nMapID), dwPortalID, pUser)) {
            pUser->SendErrorMessage(0x2E, 0x30, 55007);
            return;
        }

        // Per IDA: Relay 未就绪
        if (!XGameServer::Instance()->GetCommunitySocket().IsReady()) {
            pUser->SendErrorMessage(0x2E, 0x30, 50003);
            return;
        }

        PS_SERVER_FORCE_MATCHING_ENTER psEnter;
        psEnter.stCreateMaze = stCreateMaze;

        int nErrorID = 0;
        int nNeedItemID = 0;
        if (!CheckForceMatchingEnter(pUser, stCreateMaze.wReqMapID, nErrorID,
                                     psEnter, &nNeedItemID)) {
            LogHelper::LogError("game.contents",
                "CheckMatchingEnterUser error - [ ActorID:%d, TBID:%d, Error:%d ] ( %d )",
                pUser->GetActorID().dwActorID, pMazeData->ID, nErrorID, 514);
            if (nNeedItemID) {
                CGocNetwork::SendErrorMessage(pUser, 4, 0x41, 0xD6FB, nNeedItemID);
            } else {
                CGocNetwork::SendErrorMessage(pUser, 0x2E, 0x30,
                                             static_cast<std::uint16_t>(nErrorID));
            }
            return;
        }

        // Per IDA: 构造队长成员信息并转发到 RelayServer
        PS_SERVER_FORCE_MATCHING_ENTER_MEMBER psMaster;
        psMaster.dwUAID = pUser->GetUAID();
        psMaster.dwUCID = pUser->GetActorID().dwActorID;
        psMaster.byClass = pUser->GetClass();
        psMaster.byLevel = static_cast<std::uint8_t>(pUser->GetLevel());
        CGocAttribute* pAttr = pUser->GetGOC<CGocAttribute>();
        psMaster.byAwaken = pAttr ? pAttr->GetAwaken() : 0;
        const std::wstring strName = pUser->GetName();
        const wchar_t* szSrc = strName.c_str();
        wchar_t* szDst = psMaster.strName;
        do {
            *szDst = *szSrc;
            ++szSrc;
            ++szDst;
        } while (*szSrc);
        psMaster.uxMapID = pUser->GetMapInsID();
        psMaster.nExp = pUser->GetExp();
        CGocEntity* pEntity = pUser->GetGOC<CGocEntity>();
        psMaster.nState = (pEntity && pEntity->GetNetCafe()) ? 1 : 0;
        psMaster.dwProfilePhotoID = pEntity ? pEntity->GetProfilePhotoID() : 0;

        XSendPacket xSendPacket(0xFA, 0x13);
        xSendPacket << psEnter;
        xSendPacket << psMaster;
        XGameServer::Instance()->GetCommunitySocket().SendCmd(
            &xSendPacket, pUser, 0x2E, 0x30);
    };

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
    return true;
}

// ============================================================================
// ReqForceMatchingExit (0x140433B90) - 已精确还原
// 无参数；lambda14 (0x140433D30) 在玩家线程执行：
// GetGOC<CGocForce> 空跳过；GetMatchingState != 1 ->
// (0xFA,0x14) + actorID + 0 + UAID + level 经 SendCmd(0x2E,0x31) 通知
// RelayServer 退出匹配。
// ============================================================================
bool XForceProcess::ReqForceMatchingExit() {
    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }
    if (!pUser->GetArea()) {
        return false;
    }

    // Per IDA lambda14 (0x140433D30): 玩家线程执行匹配退出
    std::function<void()> func = [pUser]() {
        if (!pUser || !pUser->IsLive()) {
            return;
        }
        CGocForce* pGocForce = pUser->GetGOC<CGocForce>();
        if (!pGocForce) {
            return;
        }
        // Per IDA: 仅在非匹配状态(1)时可退出
        if (pGocForce->GetMatchingState() != 1) {
            XSendPacket xSendPacket(0xFA, 0x14);
            xSendPacket.XParse << static_cast<std::uint32_t>(pUser->GetActorID().dwActorID);
            xSendPacket.XParse << static_cast<std::uint32_t>(0);
            xSendPacket.XParse << static_cast<std::uint32_t>(pUser->GetUAID());
            xSendPacket.XParse << static_cast<std::uint8_t>(pUser->GetLevel());
            XGameServer::Instance()->GetCommunitySocket().SendCmd(
                &xSendPacket, pUser, 0x2E, 0x31);
        }
    };

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
    return true;
}

// ============================================================================
// ReqForceMatchingCheck (0x140433F20) - 已精确还原
// 解析 byCheck；lambda16 (0x140434100) 在玩家线程执行：
// GetGOC<CGocForce> 空跳过；构造 PS_SERVER_FORCE_MATCHING_CHECK{UAID, UCID,
// byCheck, nError=0} 经 (0xFA,0x15) SendCmd(0x2E,0x32) 转发到 RelayServer。
// ============================================================================
bool XForceProcess::ReqForceMatchingCheck(XPacket& xPacket) {
    std::uint8_t byCheck = 0;
    xPacket.XParse >> byCheck;

    CUser* pUser = GetClientPtr();
    if (!pUser) {
        return false;
    }
    if (!pUser->GetArea()) {
        return false;
    }

    // Per IDA lambda16 (0x140434100): 玩家线程执行匹配检查
    std::function<void()> func = [pUser, byCheck]() {
        if (!pUser || !pUser->IsLive()) {
            return;
        }
        CGocForce* pGocForce = pUser->GetGOC<CGocForce>();
        if (!pGocForce) {
            return;
        }

        PS_SERVER_FORCE_MATCHING_CHECK psCheck;
        psCheck.dwUAID = pUser->GetUAID();
        psCheck.dwUCID = pUser->GetActorID().dwActorID;
        psCheck.byCheck = byCheck;
        psCheck.nError = 0;

        XSendPacket xSendPacket(0xFA, 0x15);
        xSendPacket << psCheck;
        XGameServer::Instance()->GetCommunitySocket().SendCmd(
            &xSendPacket, pUser, 0x2E, 0x32);
    };

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
    return true;
}

// ============================================================================
// CheckForceMatchingEnter (0x1404342A0) - 已精确还原
// 组级匹配进入校验：先对本人执行 CheckForceMatchingEnterUser，再分别遍历
// Party（byGroupType=1）与 Force（byGroupType=2）成员逐个校验并收集
// PS_SERVER_FORCE_MATCHING_ENTER_MEMBER 到 psEnter.vecMember。
// 错误链：本人空 51001 / 非队长 53002(Party) 53103(Force) /
// CheckForceMatchingEnter 失败 53149 / 成员离线 51001。
// ============================================================================
bool XForceProcess::CheckForceMatchingEnter(CUser* pUser, std::uint16_t wMazeID, int& nError,
                                            PS_SERVER_FORCE_MATCHING_ENTER& stEnter, int* pOut) {
    nError = 0;
    *pOut = 0;

    if (!pUser) {
        nError = 51001;
        return false;
    }

    if (!CheckForceMatchingEnterUser(pUser, wMazeID, nError, pOut)) {
        return false;
    }

    // Per IDA: Party 链 - 队长校验 + 成员遍历 (byGroupType=1)
    // 说明: IDA 反编译在 Party 链上也调用了 CGocForce::CheckForceMatchingEnter，
    // Hex-Rays 把 GetGOC<CGocParty> 的组件按 CGocForce 呈现；由于 CGocForce
    // 继承 CGocParty 且同一组件槽位，此处按 IDA 呈现统一取 CGocForce。
    CGocForce* pGocParty = pUser->GetGOC<CGocForce>();
    if (pGocParty && pGocParty->IsParty()) {
        if (!pGocParty->CGocParty::IsMaster()) {
            nError = 53002;
            return false;
        }
        if (!pGocParty->CheckForceMatchingEnter()) {
            nError = 53149;
            return false;
        }
        std::shared_ptr<CParty> pParty = pGocParty->GetParty();
        if (pParty) {
            for (const auto& pair : pParty->GetMemberMap()) {
                CPartyMember* pMemberInfo = pair.second;
                if (!pMemberInfo) {
                    return false;
                }
                CUser* pMember = XGameServer::Instance()->FindActorIDToUser(
                    pMemberInfo->GetMemberID());
                if (!pMember) {
                    nError = 51001;
                    return false;
                }
                if (!CheckForceMatchingEnterUser(pMember, wMazeID, nError, pOut)) {
                    return false;
                }

                PS_SERVER_FORCE_MATCHING_ENTER_MEMBER stMemberInfo;
                stMemberInfo.dwUAID = pMember->GetUAID();
                stMemberInfo.dwUCID = pMember->GetActorID().dwActorID;
                stMemberInfo.byClass = pMember->GetClass();
                stMemberInfo.byLevel = static_cast<std::uint8_t>(pMember->GetLevel());
                CGocAttribute* pAttr = pMember->GetGOC<CGocAttribute>();
                stMemberInfo.byAwaken = pAttr ? pAttr->GetAwaken() : 0;
                const std::wstring strName = pMember->GetName();
                const wchar_t* szSrc = strName.c_str();
                wchar_t* szDst = stMemberInfo.strName;
                do {
                    *szDst = *szSrc;
                    ++szSrc;
                    ++szDst;
                } while (*szSrc);
                stMemberInfo.uxMapID = pMember->GetMapInsID();
                stMemberInfo.nExp = pMember->GetExp();
                CGocEntity* pEntity = pMember->GetGOC<CGocEntity>();
                stMemberInfo.nState = (pEntity && pEntity->GetNetCafe()) ? 1 : 0;
                stMemberInfo.dwProfilePhotoID = pEntity ? pEntity->GetProfilePhotoID() : 0;
                stEnter.vecMember.push_back(stMemberInfo);
            }
        }
        stEnter.stPartyInfo.byGroupType = 1;
        stEnter.stPartyInfo.nID = static_cast<int>(pGocParty->GetPartyID());
    }

    // Per IDA: Force 链 - 队长校验 + 成员遍历 (byGroupType=2)
    CGocForce* pGocForce = pUser->GetGOC<CGocForce>();
    if (pGocForce && pGocForce->IsParty()) {
        if (!pGocForce->IsMaster(pUser->GetActorID().dwActorID)) {
            nError = 53103;
            return false;
        }
        if (!pGocForce->CheckForceMatchingEnter()) {
            nError = 53149;
            return false;
        }
        std::shared_ptr<CParty> pForceParty = pGocForce->GetParty();
        if (pForceParty) {
            for (const auto& pair : pForceParty->GetMemberMap()) {
                CPartyMember* pMemberInfo = pair.second;
                if (!pMemberInfo) {
                    return false;
                }
                CUser* pMember = XGameServer::Instance()->FindActorIDToUser(
                    pMemberInfo->GetMemberID());
                if (!pMember) {
                    nError = 51001;
                    return false;
                }
                if (!CheckForceMatchingEnterUser(pMember, wMazeID, nError, pOut)) {
                    return false;
                }

                PS_SERVER_FORCE_MATCHING_ENTER_MEMBER stMemberInfo;
                stMemberInfo.dwUAID = pMember->GetUAID();
                stMemberInfo.dwUCID = pMember->GetActorID().dwActorID;
                stMemberInfo.byClass = pMember->GetClass();
                stMemberInfo.byLevel = static_cast<std::uint8_t>(pMember->GetLevel());
                CGocAttribute* pAttr = pMember->GetGOC<CGocAttribute>();
                stMemberInfo.byAwaken = pAttr ? pAttr->GetAwaken() : 0;
                const std::wstring strName = pMember->GetName();
                const wchar_t* szSrc = strName.c_str();
                wchar_t* szDst = stMemberInfo.strName;
                do {
                    *szDst = *szSrc;
                    ++szSrc;
                    ++szDst;
                } while (*szSrc);
                stMemberInfo.uxMapID = pMember->GetMapInsID();
                stMemberInfo.nExp = pMember->GetExp();
                CGocEntity* pEntity = pMember->GetGOC<CGocEntity>();
                stMemberInfo.nState = (pEntity && pEntity->GetNetCafe()) ? 1 : 0;
                stMemberInfo.dwProfilePhotoID = pEntity ? pEntity->GetProfilePhotoID() : 0;
                stEnter.vecMember.push_back(stMemberInfo);
            }
        }
        stEnter.stPartyInfo.byGroupType = 2;
        stEnter.stPartyInfo.nID = static_cast<int>(pGocForce->GetPartyID());
    }

    return true;
}
