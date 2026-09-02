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
#include "Soulworker/GameServer/XGameServer/actor/component/GocFriend.h"
#include "Soulworker/GameServer/XGameServer/ThreadLocalData.h"
#include "Soulworker/GameServer/XGameServer/XForceManager.h"
#include "Soulworker/GameServer/XGameServer/CForce.h"
#include "Soulworker/GameServer/XGameServer/CParty.h"
#include "Soulworker/GameServer/XCore/XArea/XDistrict.h"
#include "Soulworker/GameServer/XGameServer/SocialItemObject.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h"

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

// IDA: ?ReqForceAccept@XForceProcess@@QEAA_NAEAVXPacket@@@Z (0x1404319F0)
// 状态: STUB
// TODO: 从 IDA 0x1404319F0 反编译还原 Force 邀请接受请求
bool XForceProcess::ReqForceAccept(XPacket& xPacket) {
    GreenDamTan_log(__FILE__, __FUNCTION__, "XForceProcess::ReqForceAccept stub - pending IDA restore");
    return true;
}

// IDA: ?ReqForceChangeMaster@XForceProcess@@QEAA_NAEAVXPacket@@@Z (0x140432790)
// 状态: STUB
// TODO: 从 IDA 0x140432790 反编译还原 Force 更换队长请求
bool XForceProcess::ReqForceChangeMaster(XPacket& xPacket) {
    GreenDamTan_log(__FILE__, __FUNCTION__, "XForceProcess::ReqForceChangeMaster stub - pending IDA restore");
    return true;
}

// IDA: ?ReqForceKickOut@XForceProcess@@QEAA_NAEAVXPacket@@@Z (0x140432A20)
// 状态: STUB
// TODO: 从 IDA 0x140432A20 反编译还原 Force 踢出成员请求
bool XForceProcess::ReqForceKickOut(XPacket& xPacket) {
    GreenDamTan_log(__FILE__, __FUNCTION__, "XForceProcess::ReqForceKickOut stub - pending IDA restore");
    return true;
}

// IDA: ?ReqForceLeave@XForceProcess@@QEAA_NXZ (0x140432D40)
// 状态: STUB
// TODO: 从 IDA 0x140432D40 反编译还原 Force 离开请求
bool XForceProcess::ReqForceLeave() {
    GreenDamTan_log(__FILE__, __FUNCTION__, "XForceProcess::ReqForceLeave stub - pending IDA restore");
    return true;
}

// IDA: ?ReqForceCancel@XForceProcess@@QEAA_NAEAVXPacket@@@Z (0x140432370)
// 状态: STUB
// TODO: 从 IDA 0x140432370 反编译还原 Force 取消请求
bool XForceProcess::ReqForceCancel(XPacket& xPacket) {
    GreenDamTan_log(__FILE__, __FUNCTION__, "XForceProcess::ReqForceCancel stub - pending IDA restore");
    return true;
}

// IDA: ?ReqForceMatchingEnter@XForceProcess@@QEAA_NAEAVXPacket@@@Z (0x140432FD0)
// 状态: STUB
// TODO: 从 IDA 0x140432FD0 反编译还原（lambda12 捕获 CUser*/dwID/ID/this）
bool XForceProcess::ReqForceMatchingEnter(XPacket& xPacket) {
    GreenDamTan_log(__FILE__, __FUNCTION__, "XForceProcess::ReqForceMatchingEnter stub - pending IDA restore");
    return true;
}

// IDA: ?ReqForceMatchingExit@XForceProcess@@QEAA_NXZ (0x140433B90)
// 状态: STUB
// TODO: 从 IDA 0x140433B90 反编译还原
bool XForceProcess::ReqForceMatchingExit() {
    GreenDamTan_log(__FILE__, __FUNCTION__, "XForceProcess::ReqForceMatchingExit stub - pending IDA restore");
    return true;
}

// IDA: ?ReqForceMatchingCheck@XForceProcess@@QEAA_NAEAVXPacket@@@Z (0x140433F20)
// 状态: STUB
// TODO: 从 IDA 0x140433F20 反编译还原
bool XForceProcess::ReqForceMatchingCheck(XPacket& xPacket) {
    GreenDamTan_log(__FILE__, __FUNCTION__, "XForceProcess::ReqForceMatchingCheck stub - pending IDA restore");
    return true;
}

// IDA: ?CheckForceMatchingEnter@XForceProcess@@QEAA_NPEAVCUser@@GAEAHAEAUPS_SERVER_FORCE_MATCHING_ENTER@@1@Z (0x1404342A0)
// 状态: STUB
// TODO: 从 IDA 0x1404342A0 反编译还原（组级匹配进入校验，遍历成员）
bool XForceProcess::CheckForceMatchingEnter(CUser* pUser, std::uint16_t wMazeID, int& nError,
                                            PS_SERVER_FORCE_MATCHING_ENTER& stEnter, int* pOut) {
    GreenDamTan_log(__FILE__, __FUNCTION__, "XForceProcess::CheckForceMatchingEnter stub - pending IDA restore");
    return true;
}
