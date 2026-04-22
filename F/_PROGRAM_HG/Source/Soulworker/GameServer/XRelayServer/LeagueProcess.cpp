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
    // 对齐 IDA 0x140085a30: 先读 ST_LEAGUE_APPLICANT，然后设置 biApplicantDate 为当前时间
    ST_LEAGUE_APPLICANT stApplicant{};
    xPacket >> stApplicant;

    // 对齐 IDA: 设置申请时间为当前tick count
    stApplicant.biApplicantDate = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

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
    // 对齐 IDA 0x140088c60: 先读 PS_REQ_LEAGUE_DELEGATE，再读 dwUCID，再读 bGMDelegate
    PS_REQ_LEAGUE_DELEGATE stDelegate{};
    std::uint32_t dwUCID = 0;
    bool bGMDelegate = false;

    xPacket >> stDelegate;
    xPacket.XParse >> dwUCID;
    xPacket.XParse >> bGMDelegate;

    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    return DispatchLeagueJob([stDelegate, dwUCID, bGMDelegate, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueDelegate(pServer, dwUCID, stDelegate, bGMDelegate);
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
    // 对齐 IDA 0x140086ab0: 先读 ST_REQ_LEAGUE_INVITE_ACCEPT，再读 biJoinDate
    ST_REQ_LEAGUE_INVITE_ACCEPT stAccept{};
    std::int64_t biJoinDate = 0;

    xPacket >> stAccept;
    xPacket.XParse >> biJoinDate;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stAccept, biJoinDate, pServer]() {
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
        // 对齐 IDA lambda 0x140085480
        auto& leagueMgr = TXSingleton<XRelayServer>::Instance()->GetLeagueManager();

        // 获取邀请者用户（发送邀请的人）
        auto pReqUser = TXSingleton<XRelayServer>::Instance()->GetUser(stReject.dwReqUCID);

        // 删除被邀请者的邀请记录，获取联赛ID
        std::uint32_t nLeagueID = leagueMgr.DeleteInviteUser(stReject.dwTargetUCID);

        if (!pReqUser) {
            LogHelper::LogError("game.league",
                               "[LEAGUE] Failed ReqLeagueInviteReject - pReqUser == NULL",
                               stReject.dwReqUCID);
            // 对齐 IDA: 调用 SendErrorMessage 而非手动构造错误包
            if (pServer) {
                pServer->SendErrorMessage(0xF6, 0x10, 0xC73E);
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
    // 对齐 IDA 0x140086d90: 先读 ST_LEAGUE_BOARD，再读 dwActorID，再读 nLeagueID
    ST_LEAGUE_BOARD stBoard{};
    std::uint32_t dwActorID = 0;
    std::int32_t nLeagueID = 0;

    xPacket >> stBoard;
    xPacket.XParse >> dwActorID;
    xPacket.XParse >> nLeagueID;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stBoard, dwActorID, nLeagueID, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueBoard(pServer, dwActorID, stBoard, nLeagueID);
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
    // 对齐 IDA: 不传额外 dwActorID，操作者 ID 从 stReject.dwUCID 取
    return DispatchLeagueJob([stReject, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueApplicantReject(pServer, stReject);
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
    // 对齐 IDA 0x140084dd0: 先读 ST_LEAGUE_NOTICE，再读 dwActorID
    ST_LEAGUE_NOTICE stNotice{};
    std::uint32_t dwActorID = 0;

    xPacket >> stNotice;
    xPacket.XParse >> dwActorID;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stNotice, dwActorID, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueNoticeChange(pServer, dwActorID, stNotice);
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
    // 对齐 IDA 0x140087860: 先读 PS_REQ_LEAGUE_CARD，再读 dwUCID，再读 PS_RES_STORAGE_INFO
    PS_REQ_LEAGUE_CARD stCard{};
    std::uint32_t dwUCID = 0;
    PS_RES_STORAGE_INFO stStorage{};

    xPacket >> stCard;
    xPacket.XParse >> dwUCID;
    xPacket >> stStorage;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stCard, dwUCID, stStorage, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueCardChange(pServer, dwUCID, stCard, stStorage);
    });
}

// ============================================================================
// 0x27 - 联赛职位名称变更
// ============================================================================
bool CLeagueProcess::ReqLeaguePositionNameChange(XPacket& xPacket) {
    // 对齐 IDA 0x140087c70: 先读 ST_LEAGUE_POSITION_NAME_CHANGE，再读 dwActorID，再读 nLeagueID
    ST_LEAGUE_POSITION_NAME_CHANGE stChange{};
    std::uint32_t dwActorID = 0;
    std::int32_t nLeagueID = 0;

    xPacket >> stChange;
    xPacket.XParse >> dwActorID;
    xPacket.XParse >> nLeagueID;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([dwActorID, stChange, nLeagueID, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeaguePositionNameChange(pServer, nLeagueID, stChange, dwActorID);
    });
}

// ============================================================================
// 0x28 - 联赛权限变更
// ============================================================================
bool CLeagueProcess::ReqLeagueAuthChange(XPacket& xPacket) {
    // 对齐 IDA 0x140086490: 先读 ST_LEAGUE_AUTH_CHANGE，再读 nLeagueID，再读 dwActorID
    ST_LEAGUE_AUTH_CHANGE stAuth{};
    std::int32_t nLeagueID = 0;
    std::uint32_t dwActorID = 0;

    xPacket >> stAuth;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> dwActorID;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stAuth, nLeagueID, dwActorID, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueChangeAuth(pServer, nLeagueID, dwActorID, stAuth);
    });
}

// ============================================================================
// 0x36 - 联赛消息
// ============================================================================
bool CLeagueProcess::ReqLeagueMessage(XPacket& xPacket) {
    // 对齐 IDA 0x140087e50: 先读 PS_CHAT_LEAGUE，再读 PS_CHAT_ITEM_LINK_FOR_SERVER
    PS_CHAT_LEAGUE stChat{};
    PS_CHAT_ITEM_LINK_FOR_SERVER stItemLink{};

    xPacket >> stChat;
    xPacket >> stItemLink;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stChat, stItemLink, pServer]() mutable {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().SendLeagueMessage(stChat, stItemLink);
    });
}

// ============================================================================
// 0x37 - 联赛成员职位变更
// ============================================================================
bool CLeagueProcess::ReqLeagueMemberPositionChange(XPacket& xPacket) {
    // 对齐 IDA 0x140088310: 先读 ST_LEAGUE_MEMBER_POSITION，再读 nLeagueID，再读 dwActorID
    ST_LEAGUE_MEMBER_POSITION stPos{};
    std::int32_t nLeagueID = 0;
    std::uint32_t dwActorID = 0;

    xPacket >> stPos;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> dwActorID;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stPos, nLeagueID, dwActorID, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueMemberPositionChange(pServer, stPos, dwActorID, nLeagueID);
    });
}

// ============================================================================
// 0x39 - 联赛成员登出
// ============================================================================
bool CLeagueProcess::ReqLeagueMemberLogOut(XPacket& xPacket) {
    // 对齐 IDA 0x140088440: 先读 nLeagueID，再读 dwActorID，再读 biLogoutDate
    std::int32_t nLeagueID = 0;
    std::uint32_t dwActorID = 0;
    std::int64_t biLogoutDate = 0;

    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> dwActorID;
    xPacket.XParse >> biLogoutDate;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([nLeagueID, dwActorID, biLogoutDate, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().LogOutLeagueMember(dwActorID, nLeagueID, biLogoutDate);
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
        auto pUser = TXSingleton<XRelayServer>::Instance()->GetUser(dwUCID);
        if (!pUser) {
            LogHelper::LogError("game.relay", "[LEAUGE] ReqLeagueDeletePenalty - if( pUser )", dwUCID);
            return;
        }
        pUser->SetLeagueDeletePenalty(biPenalty);
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
        auto pUser = TXSingleton<XRelayServer>::Instance()->GetUser(dwUCID);
        if (!pUser) {
            LogHelper::LogError("game.relay", "[LEAUGE] ReqLeagueWithdrawPenalty - if( pUser )", dwUCID);
            return;
        }
        pUser->SetLeagueWithdrawPenalty(biPenalty);
    });
}

// ============================================================================
// 0x45 - 联赛开放状态
// ============================================================================
bool CLeagueProcess::ReqLeagueOpenOrNot(XPacket& xPacket) {
    // 对齐 IDA 0x140088930: 先读 ST_LEAGUE_OPEN，再读 dwUCID
    ST_LEAGUE_OPEN stOpen{};
    std::uint32_t dwActorID = 0;

    xPacket >> stOpen;
    xPacket.XParse >> dwActorID;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stOpen, dwActorID, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueOpenOrNot(pServer, stOpen, dwActorID);
    });
}

// ============================================================================
// 0x46 - 联赛招募公告
// ============================================================================
bool CLeagueProcess::ReqLeagueRecruitNotice(XPacket& xPacket) {
    // 对齐 IDA 0x140088a20: 先读 dwUCID，再读 ST_LEAGUE_RECRUIT_NOTICE
    std::uint32_t dwUCID = 0;
    ST_LEAGUE_RECRUIT_NOTICE stNotice{};

    xPacket.XParse >> dwUCID;
    xPacket >> stNotice;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([dwUCID, stNotice, pServer]() {
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
    // 对齐 IDA 0x140089060: 先读 PS_REQ_LEAGUE_SKILL + GetClientPtr(null检查)
    PS_REQ_LEAGUE_SKILL stSkill{};
    xPacket >> stSkill;

    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    return DispatchLeagueJob([stSkill, pServer]() {
        // 对齐 IDA lambda 0x140089110: 连接状态检查 + IsLockLeague防重复 + SetLockLeague(1)
        if (!pServer || !pServer->IsState(XClient::eStateConnect)) {
            return;
        }

        auto pUser = TXSingleton<XRelayServer>::Instance()->GetUser(stSkill.dwUCID);
        if (!pUser) {
            return;
        }

        if (pUser->IsLockLeague()) {
            LogHelper::LogError("game.relay", "ReqLEagueSKillLearn error - Several times Request");
            return;
        }

        pUser->SetLockLeague(1);
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
    // 对齐 IDA 0x140089680: 读取 nLeagueID -> dwUCID（非结构体）
    std::int32_t nLeagueID = 0;
    std::uint32_t dwUCID = 0;

    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> dwUCID;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([nLeagueID, dwUCID, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueMemberInitExp(nLeagueID, dwUCID);
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
    // 对齐 IDA 0x140089880: 先读 PS_REQ_LEAGUE_INVEN_INFO，再读 dwReqUCID
    PS_REQ_LEAGUE_INVEN_INFO stReq{};
    std::uint32_t dwReqUCID = 0;

    xPacket >> stReq;
    xPacket.XParse >> dwReqUCID;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([stReq, dwReqUCID, pServer]() {
        // 对齐 IDA lambda 0x1400899c0: 传 dwReqUCID 而非 stReq.dwNpcID
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueInevntoryInfo(dwReqUCID, stReq);
    });
}

// ============================================================================
// 0x61 - 联赛仓库移动
// ============================================================================
bool CLeagueProcess::ReqLeagueInventoryMove(XPacket& xPacket) {
    // 对齐 IDA 0x140089a10: 先读 dwReqUCID，再读 PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME
    std::uint32_t dwReqUCID = 0;
    PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME stMove{};

    xPacket.XParse >> dwReqUCID;
    xPacket >> stMove;

    CServer* pServer = GetClientPtr();
    return DispatchLeagueJob([dwReqUCID, stMove, pServer]() {
        TXSingleton<XRelayServer>::Instance()->GetLeagueManager().ReqLeagueInventoryMove(dwReqUCID, stMove);
    });
}