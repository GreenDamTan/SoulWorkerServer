#include "Soulworker/GameServer/XRelayServer/LeagueProcess.h"

#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h"

namespace {

// 联赛任务分发到逻辑线程1
bool DispatchLeagueJob(const std::function<void()>& job) {
    if (!job) {
        return false;
    }
    return CLogicThreadManager::Instance().DoJob(1, job);
}

} // namespace

bool CLeagueProcess::Parse(XPacket& xPacket) {
    switch (static_cast<unsigned char>(xPacket.GetSubCmd())) {
    case 0x01:
        return ReqLeagueCreate(xPacket);
    case 0x02:
        return ReqLeagueDelete(xPacket);
    case 0x04:
        return ReqLeagueApplicant(xPacket);
    case 0x06:
        return ReqLeagueInfo(xPacket);
    case 0x07:
        return ReqLeagueDelegate(xPacket);
    case 0x08:
        return ReqLeagueWithDraw(xPacket);
    case 0x09:
        return ReqLeagueKick(xPacket);
    case 0x0C:
        return ReqLeagueInvite(xPacket);
    case 0x0D:
        return ReqLeagueInviteAccept(xPacket);
    case 0x10:
        return ReqLeagueInviteReject(xPacket);
    case 0x14:
        return ReqLeagueBoard(xPacket);
    case 0x16:
        return ReqLeagueApplicantAccept(xPacket);
    case 0x17:
        return ReqLeagueApplicantReject(xPacket);
    case 0x18:
        return ReqLeagueSearch(xPacket);
    case 0x23:
        return ReqLeagueNoticeChange(xPacket);
    case 0x24:
        return ReqLeagueList(xPacket);
    case 0x25:
        return ReqLeagueNameChange(xPacket);
    case 0x26:
        return ReqLeagueCardChange(xPacket);
    case 0x27:
        return ReqLeaguePositionNameChange(xPacket);
    case 0x28:
        return ReqLeagueAuthChange(xPacket);
    case 0x36:
        return ReqLeagueMessage(xPacket);
    case 0x37:
        return ReqLeagueMemberPositionChange(xPacket);
    case 0x39:
        return ReqLeagueMemberLogOut(xPacket);
    case 0x43:
        return ReqLeagueDeletePenalty(xPacket);
    case 0x44:
        return ReqLeagueWithdrawPenalty(xPacket);
    case 0x45:
        return ReqLeagueOpenOrNot(xPacket);
    case 0x46:
        return ReqLeagueRecruitNotice(xPacket);
    case 0x47:
        return ReqLeagueRecordUpdate(xPacket);
    case 0x51:
        return ReqLeagueLevelup(xPacket);
    case 0x53:
        return ReqLeagueSkillLearn(xPacket);
    case 0x55:
        return ReqLeagueWealth(xPacket);
    case 0x56:
        return ReqLeagueMemberInitExp(xPacket);
    case 0x57:
        return ReqLeagueSkillPointUpdate(xPacket);
    case 0x58:
        return ReqSyncLeagueInfo(xPacket);
    case 0x60:
        return ReqLeagueInventoryInfo(xPacket);
    case 0x61:
        return ReqLeagueInventoryMove(xPacket);
    default:
        return true;
    }
}

// ============================================================================
// 0x01 - 联赛创建
// ============================================================================
bool CLeagueProcess::ReqLeagueCreate(XPacket& xPacket) {
    PS_LEAGUE_CREATE_FOR_SERVER stCreate{};
    xPacket >> stCreate;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stCreate, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueCreate(pServer, stCreate);
    });
}

// ============================================================================
// 0x02 - 联赛删除
// ============================================================================
bool CLeagueProcess::ReqLeagueDelete(XPacket& xPacket) {
    std::uint32_t dwActorID = 0;
    std::int32_t nLeagueID = 0;
    std::int64_t biPenalty = 0;
    xPacket.XParse >> dwActorID;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> biPenalty;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([dwActorID, nLeagueID, biPenalty, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueDel(pServer, dwActorID, nLeagueID, biPenalty);
    });
}

// ============================================================================
// 0x04 - 联赛申请者
// ============================================================================
bool CLeagueProcess::ReqLeagueApplicant(XPacket& xPacket) {
    ST_LEAGUE_APPLICANT stApplicant{};
    xPacket >> stApplicant;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stApplicant, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueApplicant(stApplicant, pServer);
    });
}

// ============================================================================
// 0x06 - 联赛信息
// ============================================================================
bool CLeagueProcess::ReqLeagueInfo(XPacket& xPacket) {
    std::uint32_t dwActorID = 0;
    std::int32_t nLeagueID = 0;
    xPacket.XParse >> dwActorID;
    xPacket.XParse >> nLeagueID;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([dwActorID, nLeagueID, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueInfo(pServer, dwActorID, nLeagueID);
    });
}

// ============================================================================
// 0x07 - 联赛转让
// ============================================================================
bool CLeagueProcess::ReqLeagueDelegate(XPacket& xPacket) {
    PS_REQ_LEAGUE_DELEGATE stDelegate{};
    xPacket >> stDelegate;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stDelegate, pServer]() {
        bool bGMDelegate = false;
        // dwNpcID 用作请求者UCID
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueDelegate(pServer, stDelegate.dwNpcID, stDelegate, bGMDelegate);
    });
}

// ============================================================================
// 0x08 - 联赛退出
// ============================================================================
bool CLeagueProcess::ReqLeagueWithDraw(XPacket& xPacket) {
    UXActorID uxActorID{};
    std::int32_t nLeagueID = 0;
    std::int64_t biPenalty = 0;
    xPacket >> uxActorID;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> biPenalty;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([uxActorID, nLeagueID, biPenalty, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueWithDraw(pServer, uxActorID, nLeagueID, biPenalty);
    });
}

// ============================================================================
// 0x09 - 联赛踢人
// ============================================================================
bool CLeagueProcess::ReqLeagueKick(XPacket& xPacket) {
    std::uint32_t dwActorID = 0;
    std::uint32_t dwTargetID = 0;
    std::int32_t nLeagueID = 0;
    xPacket.XParse >> dwActorID;
    xPacket.XParse >> dwTargetID;
    xPacket.XParse >> nLeagueID;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([dwActorID, dwTargetID, nLeagueID, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueKick(pServer, dwActorID, dwTargetID, nLeagueID);
    });
}

// ============================================================================
// 0x0C - 联赛邀请
// ============================================================================
bool CLeagueProcess::ReqLeagueInvite(XPacket& xPacket) {
    ST_REQ_LEAGUE_INVITE stInvite{};
    xPacket >> stInvite;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stInvite, pServer]() {
        auto pUser = TXSingleton<XRelayServer>::Instance()->GetUser(stInvite.dwActorID);
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueInvite(pServer, stInvite, pUser);
    });
}

// ============================================================================
// 0x0D - 联赛邀请接受
// ============================================================================
bool CLeagueProcess::ReqLeagueInviteAccept(XPacket& xPacket) {
    ST_REQ_LEAGUE_INVITE_ACCEPT stAccept{};
    xPacket >> stAccept;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stAccept, pServer]() {
        std::int64_t biJoinDate = 0;
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqInviteAccept(pServer, stAccept, biJoinDate);
    });
}

// ============================================================================
// 0x10 - 联赛邀请拒绝
// ============================================================================
bool CLeagueProcess::ReqLeagueInviteReject(XPacket& xPacket) {
    ST_REQ_LEAGUE_INVITE_REJECT stReject{};
    xPacket >> stReject;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stReject, pServer]() {
        auto& leagueMgr = TXSingleton<XRelayServer>::Instance()->GetLeagueManager();

        // 获取邀请者用户（发送邀请的人）
        auto pReqUser = TXSingleton<XRelayServer>::Instance()->GetUser(stReject.dwReqUCID);

        // 删除被邀请者的邀请记录，获取联赛ID
        std::uint32_t nLeagueID = leagueMgr.DeleteInviteUser(stReject.dwTargetUCID);

        if (!pReqUser) {
            LogHelper::LogError("game.league",
                               "[LEAGUE] Failed ReqLeagueInviteReject - pReqUser == NULL",
                               stReject.dwReqUCID);
            // 发送错误消息（主命令0xF6，子命令0x10，错误码0xC73E=57022）
            XSendPacket xSendPacket(0xF6, 0x10);
            xSendPacket.XParse << static_cast<std::int32_t>(0xC73E);
            if (pServer) {
                pServer->SendEx(xSendPacket);
            }
            return;
        }

        // 发送拒绝包给邀请者（主命令0xF6，子命令0x10）
        XSendPacket xSendPacket(0xF6, 0x10);
        xSendPacket << stReject;
        xSendPacket.XParse << static_cast<std::int32_t>(nLeagueID);
        pReqUser->SendPacket(xSendPacket);
    });
}

// ============================================================================
// 0x14 - 联赛公告板
// ============================================================================
bool CLeagueProcess::ReqLeagueBoard(XPacket& xPacket) {
    ST_LEAGUE_BOARD stBoard{};
    xPacket >> stBoard;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stBoard, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueBoard(pServer, 0, stBoard, stBoard.nLeagueID);
    });
}

// ============================================================================
// 0x16 - 联赛申请者接受
// ============================================================================
bool CLeagueProcess::ReqLeagueApplicantAccept(XPacket& xPacket) {
    ST_REQ_LEAGUE_APPLICANT_ACCEPT stAccept{};
    std::uint32_t dwActorID = 0;

    // 包格式：先读结构体，再读操作者UCID
    xPacket >> stAccept;
    xPacket.XParse >> dwActorID;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stAccept, dwActorID, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueApplicantAccept(pServer, stAccept, dwActorID);
    });
}

// ============================================================================
// 0x17 - 联赛申请者拒绝
// ============================================================================
bool CLeagueProcess::ReqLeagueApplicantReject(XPacket& xPacket) {
    ST_REQ_LEAGUE_APPLICANT_REJECT stReject{};
    xPacket >> stReject;

    CServer* pServer = GetClientPtr();
    // stReject.dwUCID 是操作者的UCID
    return DispatchLeagueJob([stReject, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueApplicantReject(pServer, stReject, stReject.dwUCID);
    });
}

// ============================================================================
// 0x18 - 联赛搜索
// ============================================================================
bool CLeagueProcess::ReqLeagueSearch(XPacket& xPacket) {
    ST_REQ_LEAGUE_SEARCH stSearch{};
    std::uint32_t dwActorID = 0;
    xPacket >> stSearch;
    xPacket.XParse >> dwActorID;

    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    return DispatchLeagueJob([stSearch, dwActorID, pServer]() {
        // 检查服务器连接状态
        if (!pServer->IsState(XClient::eStateConnect)) {
            return;
        }

        // 发送到DB处理联赛搜索（main=7, sub=0x29）
        IXObject* pObject = static_cast<IXObject*>(pServer);
        XSendDBPacket xSendDBPacket(pObject, 7, 0x29);
        xSendDBPacket << stSearch;
        xSendDBPacket.XParse << dwActorID;
        xSendDBPacket.XParse << pServer->GetServerID();

        TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
    });
}

// ============================================================================
// 0x23 - 联赛公告变更
// ============================================================================
bool CLeagueProcess::ReqLeagueNoticeChange(XPacket& xPacket) {
    ST_LEAGUE_NOTICE stNotice{};
    xPacket >> stNotice;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stNotice, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueNoticeChange(pServer, stNotice.nLeagueID, stNotice);
    });
}

// ============================================================================
// 0x24 - 联赛列表
// ============================================================================
bool CLeagueProcess::ReqLeagueList(XPacket& xPacket) {
    std::uint32_t dwUCID = 0;
    std::uint8_t byType = 0;
    xPacket.XParse >> dwUCID;
    xPacket.XParse >> byType;

    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    return DispatchLeagueJob([dwUCID, byType, pServer]() {
        // 发送到DB处理联赛列表（main=7, sub=0x41）
        IXObject* pObject = static_cast<IXObject*>(pServer);
        XSendDBPacket xSendDBPacket(pObject, 7, 0x41);
        xSendDBPacket.XParse << dwUCID;
        xSendDBPacket.XParse << byType;
        xSendDBPacket.XParse << pServer->GetServerID();

        TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
    });
}

// ============================================================================
// 0x25 - 联赛名称变更
// ============================================================================
bool CLeagueProcess::ReqLeagueNameChange(XPacket& xPacket) {
    PS_LEAGUE_NAME_CHANGE_SERVER stChange{};
    xPacket >> stChange;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stChange, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueNameChange(stChange);
    });
}

// ============================================================================
// 0x26 - 联赛卡片变更
// ============================================================================
bool CLeagueProcess::ReqLeagueCardChange(XPacket& xPacket) {
    PS_REQ_LEAGUE_CARD stCard{};
    xPacket >> stCard;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stCard, pServer]() {
        PS_RES_STORAGE_INFO stStorage{};
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueCardChange(pServer, stCard.nLeagueID, stCard, stStorage);
    });
}

// ============================================================================
// 0x27 - 联赛职位名称变更
// ============================================================================
bool CLeagueProcess::ReqLeaguePositionNameChange(XPacket& xPacket) {
    std::int32_t nLeagueID = 0;
    ST_LEAGUE_POSITION_NAME_CHANGE stChange{};
    xPacket.XParse >> nLeagueID;
    xPacket >> stChange;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([nLeagueID, stChange, pServer]() {
        std::uint32_t dwSomething = 0;
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeaguePositionNameChange(pServer, nLeagueID, stChange, dwSomething);
    });
}

// ============================================================================
// 0x28 - 联赛权限变更
// ============================================================================
bool CLeagueProcess::ReqLeagueAuthChange(XPacket& xPacket) {
    std::int32_t nLeagueID = 0;
    ST_LEAGUE_AUTH_CHANGE stAuth{};

    // 从包中读取联赛ID和权限变更结构
    xPacket.XParse >> nLeagueID;
    xPacket >> stAuth;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([nLeagueID, stAuth, pServer]() {
        // dwActorID需要从会话中获取，这里暂时用0
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueChangeAuth(pServer, nLeagueID, 0, stAuth);
    });
}

// ============================================================================
// 0x36 - 联赛消息
// ============================================================================
bool CLeagueProcess::ReqLeagueMessage(XPacket& xPacket) {
    PS_CHAT_LEAGUE stChat{};
    xPacket >> stChat;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stChat, pServer]() mutable {
        PS_CHAT_ITEM_LINK_FOR_SERVER stItemLink{};
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().SendLeagueMessage(stChat, stItemLink);
    });
}

// ============================================================================
// 0x37 - 联赛成员职位变更
// ============================================================================
bool CLeagueProcess::ReqLeagueMemberPositionChange(XPacket& xPacket) {
    std::uint32_t dwOperatorID = 0;
    ST_LEAGUE_MEMBER_POSITION stPos{};
    std::int32_t nLeagueID = 0;

    // 包格式：操作者ID -> 结构体 -> 联赛ID
    xPacket.XParse >> dwOperatorID;
    xPacket >> stPos;
    xPacket.XParse >> nLeagueID;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([dwOperatorID, stPos, nLeagueID, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueMemberPositionChange(pServer, stPos, dwOperatorID, nLeagueID);
    });
}

// ============================================================================
// 0x39 - 联赛成员登出
// ============================================================================
bool CLeagueProcess::ReqLeagueMemberLogOut(XPacket& xPacket) {
    std::uint32_t dwUCID = 0;
    std::int32_t nLeagueID = 0;
    std::int64_t biPenalty = 0;
    xPacket.XParse >> dwUCID;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> biPenalty;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([dwUCID, nLeagueID, biPenalty, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().LogOutLeagueMember(dwUCID, nLeagueID, biPenalty);
    });
}

// ============================================================================
// 0x43 - 联赛删除惩罚
// ============================================================================
bool CLeagueProcess::ReqLeagueDeletePenalty(XPacket& xPacket) {
    std::uint32_t dwUCID = 0;
    std::int64_t biPenalty = 0;
    xPacket.XParse >> dwUCID;
    xPacket.XParse >> biPenalty;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([dwUCID, biPenalty, pServer]() {
        LogHelper::LogDebug("game.league", "ReqLeagueDeletePenalty ucid=%u penalty=%lld", dwUCID, static_cast<long long>(biPenalty));
    });
}

// ============================================================================
// 0x44 - 联赛退出惩罚
// ============================================================================
bool CLeagueProcess::ReqLeagueWithdrawPenalty(XPacket& xPacket) {
    std::uint32_t dwUCID = 0;
    std::int64_t biPenalty = 0;
    xPacket.XParse >> dwUCID;
    xPacket.XParse >> biPenalty;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([dwUCID, biPenalty, pServer]() {
        LogHelper::LogDebug("game.league", "ReqLeagueWithdrawPenalty ucid=%u penalty=%lld", dwUCID, static_cast<long long>(biPenalty));
    });
}

// ============================================================================
// 0x45 - 联赛开放状态
// ============================================================================
bool CLeagueProcess::ReqLeagueOpenOrNot(XPacket& xPacket) {
    ST_LEAGUE_OPEN stOpen{};
    xPacket >> stOpen;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stOpen, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueOpenOrNot(pServer, stOpen, stOpen.nLeagueID);
    });
}

// ============================================================================
// 0x46 - 联赛招募公告
// ============================================================================
bool CLeagueProcess::ReqLeagueRecruitNotice(XPacket& xPacket) {
    ST_LEAGUE_RECRUIT_NOTICE stNotice{};
    xPacket >> stNotice;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stNotice, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueRecruitNotice(pServer, stNotice.nLeagueID, stNotice);
    });
}

// ============================================================================
// 0x47 - 联赛记录更新
// ============================================================================
bool CLeagueProcess::ReqLeagueRecordUpdate(XPacket& xPacket) {
    ST_LEAGUE_RECORD stRecord{};
    xPacket >> stRecord;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stRecord, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueRecordUpdate(stRecord);
    });
}

// ============================================================================
// 0x51 - 联赛升级
// ============================================================================
bool CLeagueProcess::ReqLeagueLevelup(XPacket& xPacket) {
    std::int32_t nLeagueID = 0;
    std::uint8_t byType = 0;
    std::uint32_t dwActorID = 0;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> byType;
    xPacket.XParse >> dwActorID;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([nLeagueID, byType, dwActorID, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueLevelup(nLeagueID, byType, dwActorID);
    });
}

// ============================================================================
// 0x53 - 联赛技能学习
// ============================================================================
bool CLeagueProcess::ReqLeagueSkillLearn(XPacket& xPacket) {
    PS_REQ_LEAGUE_SKILL stSkill{};
    xPacket >> stSkill;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stSkill, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueSkillLearn(pServer, stSkill);
    });
}

// ============================================================================
// 0x55 - 联赛财富
// ============================================================================
bool CLeagueProcess::ReqLeagueWealth(XPacket& xPacket) {
    PS_LEAGUE_WEALTH_FOR_SERVER stWealth{};
    xPacket >> stWealth;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stWealth, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqApplyLeagueExp(stWealth);
    });
}

// ============================================================================
// 0x56 - 联赛成员经验初始化
// ============================================================================
bool CLeagueProcess::ReqLeagueMemberInitExp(XPacket& xPacket) {
    PS_REQ_LEAGUE_INVEN_INFO stReq{};
    xPacket >> stReq;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stReq, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueMemberInitExp(stReq);
    });
}

// ============================================================================
// 0x57 - 联赛技能点更新
// ============================================================================
bool CLeagueProcess::ReqLeagueSkillPointUpdate(XPacket& xPacket) {
    std::int32_t nLeagueID = 0;
    std::uint8_t byType = 0;
    std::uint32_t dwSomething = 0;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> byType;
    xPacket.XParse >> dwSomething;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([nLeagueID, byType, dwSomething, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueSkillPointUpdate(nLeagueID, byType, dwSomething);
    });
}

// ============================================================================
// 0x58 - 同步联赛信息
// ============================================================================
bool CLeagueProcess::ReqSyncLeagueInfo(XPacket& xPacket) {
    PS_SYNC_LEAGUE_INFO stSync{};
    xPacket >> stSync;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stSync, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().SyncLeagueInfo(stSync);
    });
}

// ============================================================================
// 0x60 - 联赛仓库信息
// ============================================================================
bool CLeagueProcess::ReqLeagueInventoryInfo(XPacket& xPacket) {
    PS_REQ_LEAGUE_INVEN_INFO stReq{};
    xPacket >> stReq;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stReq, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueInevntoryInfo(stReq.dwNpcID, stReq);
    });
}

// ============================================================================
// 0x61 - 联赛仓库移动
// ============================================================================
bool CLeagueProcess::ReqLeagueInventoryMove(XPacket& xPacket) {
    PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME stMove{};
    xPacket >> stMove;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stMove, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueInventoryMove(0, stMove);
    });
}