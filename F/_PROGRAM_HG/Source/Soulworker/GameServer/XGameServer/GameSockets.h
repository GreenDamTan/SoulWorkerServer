// GameSockets.h
// Game Server Socket Stubs

#pragma once

#include "Soulworker/GameServer/XCore/XServer/TXDBSocket.h"
#include "Soulworker/GameServer/XCore/XServer/Option.h"
#include <cstdint>

// 前置声明
class XRelaySocket;
class XPacket;

// XRelaySocket 存根 - 用于 GameServer 连接到 RelayServer
class XRelaySocket : public XIOCPClient {
public:
    XRelaySocket() = default;
    ~XRelaySocket() = default;

    static bool Connect(XRelaySocket* pSocket) {
        // TODO: 对齐 IDA 实现
        return true;
    }
    static void SendUpdateServerInfo(XRelaySocket* pSocket, int nType, int nCount) {
        // TODO: 对齐 IDA 实现
    }
    // AddUserCount: IDA 确认在 EnterUser/ExitUser 中调用
    // nCount: +1 for EnterUser, -1 for ExitUser
    static void AddUserCount(int nCount) {
        // TODO: 对齐 IDA 实现 - 通知 RelayServer 用户数变化
    }
};

// CCommunitySocket - Community Socket
class CCommunitySocket : public XRelaySocket {
public:
    CCommunitySocket() = default;
    ~CCommunitySocket() = default;

    void SetMyInfo(XOption* pOption) {}
    bool Init_2(E_POOL_ID ePoolID, const char* szIP, std::int16_t shPort) { return true; }
    static bool IsCachingLoadFinish(CCommunitySocket* pSocket) { return true; }
    static bool IsCanSend(CCommunitySocket* pSocket) { return true; }

    // Packet process dispatchers (implemented in GameSockets.cpp)
    bool PartyProcess(XPacket* xPacket);
    bool LeagueProcess(XPacket* xPacket);

    // Party packet handlers
    bool RecvPartyCreate(XPacket* xPacket);
    bool RecvPartyJoinMember(XPacket* xPacket);
    bool RecvPartyLeaveMember(XPacket* xPacket);
    bool RecvPartyChangeMaster(XPacket* xPacket);
    bool RecvUpdatePartyMember(XPacket* xPacket);
    bool RecvPartyDelete(XPacket* xPacket);
    bool RecvPartyEnterMaze(XPacket* xPacket);
    bool RecvPartyUpdateInfo(XPacket* xPacket);
    bool RecvPartyEnterServer(XPacket* xPacket);
    bool RecvPartyInvite(XPacket* xPacket);
    bool RecvPartyAccept(XPacket* xPacket);
    bool RecvPartyReject(XPacket* xPacket);
    bool RecvPartyMessage(XPacket* xPacket);
    bool RecvPartyMatchingEnter(XPacket* xPacket);
    bool RecvPartyMatchingExit(XPacket* xPacket);
    bool RecvPartyMatchingCheck(XPacket* xPacket);
    bool RecvPartyMatchingReset(XPacket* xPacket);
    bool RecvPartyMatchingWait(XPacket* xPacket);
    bool RecvPartyRecruitAdd(XPacket* xPacket);
    bool RecvPartyRecruitDel(XPacket* xPacket);
    bool RecvPartyRecruitApply(XPacket* xPacket);
    bool RecvPartyRecruitApplyAccept(XPacket* xPacket);
    bool RecvPartyRecruitApplyReject(XPacket* xPacket);
    bool RecvPartyRecruitApplyUpdate(XPacket* xPacket);
    bool RecvPartyRecruitList(XPacket* xPacket);
    bool RecvPartyRecruitMyApplyList(XPacket* xPacket);
    bool RecvPartyRecruitApplyList(XPacket* xPacket);
    bool RecvPartyRecruitInfo(XPacket* xPacket);
    bool RecvPartyRecruitApplyDel(XPacket* xPacket);
    bool RecvPartyRecruitApplyInfo(XPacket* xPacket);
    bool RecvPartyRecruitApplyNotice(XPacket* xPacket);
    bool RecvPartyRecruitApplyAcceptCheck(XPacket* xPacket);
    bool RecvPartyInfo(XPacket* xPacket);
    bool RecvPartyNameChange(XPacket* xPacket);
    bool RecvPartyMatchingMaze(XPacket* xPacket);
    bool RecvPartyMazeClear(XPacket* xPacket);

    // League packet handlers
    bool RecvCreateLeague(XPacket* xPacket);
    bool RecvLeagueDelete(XPacket* xPacket);
    bool RecvLeagueLogin(XPacket* xPacket);
    bool RecvLeagueInfo(XPacket* xPacket);
    bool RecvLeagueDelegate(XPacket* xPacket);
    bool RecvLeagueWithDraw(XPacket* xPacket);
    bool RecvLeagueMemberKick(XPacket* xPacket);
    bool RecvLeagueInfoChange(XPacket* xPacket);
    bool RecvLeagueInvite(XPacket* xPacket);
    bool RecvLeagueInviteAccept(XPacket* xPacket);
    bool RecvLeagueInviteReject(XPacket* xPacket);
    bool RecvLeagueBoard(XPacket* xPacket);
    bool RecvLeagueApplicantAcceptRes(XPacket* xPacket);
    bool RecvLeagueSearch(XPacket* xPacket);
    bool RecvLeagueApplicantRes(XPacket* xPacket);
    bool RecvLeagueApplicantAdd(XPacket* xPacket);
    bool RecvLeagueApplicantReject(XPacket* xPacket);
    bool RecvLeagueApplicantDelete(XPacket* xPacket);
    bool RecvLeagueNoticeChange(XPacket* xPacket);
    bool RecvLeagueList(XPacket* xPacket);
    bool RecvLeagueNameChange(XPacket* xPacket);
    bool RecvLeagueCardChange(XPacket* xPacket);
    bool RecvLeaguePositionNameChange(XPacket* xPacket);
    bool RecvLeagueAuthChange(XPacket* xPacket);
    bool RecvLeagueMessage(XPacket* xPacket);
    bool RecvLeagueMemberPositionChange(XPacket* xPacket);
    bool RecvLeagueApplicantUpdate(XPacket* xPacket);
    bool RecvLeagueMemberLogOut(XPacket* xPacket);
    bool RecvLeagueApplicantJoinUser(XPacket* xPacket);
    bool RecvLeagueInviteJoinUser(XPacket* xPacket);
    bool RecvLeagueMemberUpdate(XPacket* xPacket);
    bool RecvLeagueOpenOrNot(XPacket* xPacket);
    bool RecvLeagueRecruitNotice(XPacket* xPacket);
    bool RecvLeagueRecordUpdate(XPacket* xPacket);
    bool RecvLeagueCardChangeRes(XPacket* xPacket);
    bool RecvLeagueLevelup(XPacket* xPacket);
    bool RecvLeagueSkillLearn(XPacket* xPacket);
    bool RecvLeagueWealth(XPacket* xPacket);
    bool RecvLeagueSyncInfo(XPacket* xPacket);
    bool RecvLeagueSyncLoad(XPacket* xPacket);
    bool RecvLeagueInventoryInfo(XPacket* xPacket);
    bool RecvLeagueInventoryMove(XPacket* xPacket);
};

// CGameControlSocket - Game Control Socket
class CGameControlSocket : public XRelaySocket {
public:
    CGameControlSocket() = default;
    ~CGameControlSocket() = default;

    void SetMyInfo(XOption* pOption) {}
    bool Init_2(E_POOL_ID ePoolID, const char* szIP, std::int16_t shPort) { return true; }
    static bool IsCachingLoadFinish(CGameControlSocket* pSocket) { return true; }
    static bool IsCanSend(CGameControlSocket* pSocket) { return true; }

    // Packet process dispatchers (implemented in GameSockets.cpp)
    bool ServerProcessEx(XPacket* xPacket);
    bool PartyProcess(XPacket* xPacket);

    // Send methods
    void SendCheck(XSendPacket* pPacket) {
        // TODO: 对齐 IDA 实现 - 发送检查包到 Control
    }

    // Server control packet handlers
    bool RecvServerShutdown(XPacket* xPacket);
    bool RecvTimeEvent(XPacket* xPacket);
    bool RecvValueEvent(XPacket* xPacket);
    bool RecvCashShopBanner(XPacket* xPacket);
    bool RecvCreateMazeReq(XPacket* xPacket);
    bool RecvCreateMazeRes(XPacket* xPacket);
    bool RecvRemoveMaze(XPacket* xPacket);
    bool RecvGoBackMazeRes(XPacket* xPacket);
    bool RecvCreateMap(XPacket* xPacket);
    bool RecvEnterMap(XPacket* xPacket);
    bool RecvCheckPartyInMaze(XPacket* xPacket);
    bool RecvUsersInfo(XPacket* xPacket);
    bool ResCheckEnterMaze(XPacket* xPacket);
    bool RecvPartyMatching(XPacket* xPacket);
    bool RecvForceMatching(XPacket* xPacket);
    bool RecvMyRoomEnterReq(XPacket* xPacket);
    bool RecvMyRoomEnter(XPacket* xPacket);
    bool RecvMyRoomCreate(XPacket* xPacket);
    bool RecvMyRoomDeleteReq(XPacket* xPacket);
    bool RecvEnterMapToOther(XPacket* xPacket);
    bool RecvPostSend(XPacket* xPacket);
    bool RecvFindUser(XPacket* xPacket);
    bool RecvMyRoomDeleteRes(XPacket* xPacket);
    bool RecvEnterUserInfo(XPacket* xPacket);
    bool RecvServerOptionUpdate(XPacket* xPacket);
    bool RecvServerDayEventBoosterList(XPacket* xPacket);
    bool RecvCachingComplete(XPacket* xPacket);
    bool RecvServerCreateModeMazeReq(XPacket* xPacket);
    bool RecvServerRouletteEvent(XPacket* xPacket);
    bool RecvPartyEnterMaze(XPacket* xPacket);

    // IDA 0x1402DF510 引用 - 发送用户通知到 Control
    void RecvUserNotice(XSendPacket* pPacket) {
        // TODO: 对齐 IDA 实现 - 发送用户通知包到 Control Server
    }
};
