// GameSockets.h
// Game Server Socket Stubs

#pragma once

#include "Soulworker/GameServer/XCore/XServer/TXDBSocket.h"
#include "Soulworker/GameServer/XCore/XServer/Option.h"
#include <cstdint>

// 前置声明
class XRelaySocket;
class XPacket;
class CUser;

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
// Per IDA: 继承自 XRelaySocket，用于与 ControlServer 通信
class CGameControlSocket : public XRelaySocket {
public:
    CGameControlSocket();
    virtual ~CGameControlSocket();

    // Per IDA 0x1401ca350: 设置服务器信息
    virtual void SetMyInfo(XOption* pOption);

    // Per IDA 0x1401ca500: 服务器包处理
    virtual bool ServerProcessEx(XPacket* xPacket);

    // Per IDA 0x1401cb1e0: 组队包处理
    virtual bool PartyProcess(XPacket* xPacket);

    // Per IDA 0x1401cb230: 世界模式包处理
    virtual bool WorldModeProcess(XPacket* xPacket);

    // Per IDA 0x1401cb320: 军团包处理
    virtual bool ForceProcess(XPacket* xPacket);

    // 静态方法
    static bool IsCachingLoadFinish(CGameControlSocket* pSocket) { return pSocket->m_bCachingLoadFinish; }
    static bool IsCanSend(CGameControlSocket* pSocket) { return true; }

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
    bool RecvPartyMatching(XPacket* xPacket);
    bool RecvForceMatching(XPacket* xPacket);
    bool RecvForceEnterMaze(XPacket* xPacket);
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
    bool RecvUserEnterServer(XPacket* xPacket);
    bool RecvUserChangeServer(XPacket* xPacket);
    bool RecvUserKickout(XPacket* xPacket);
    bool RecvUpdateChannelAll(XPacket* xPacket);
    bool RecvUpdateChannel(XPacket* xPacket);
    bool RecvChangeChannelRes(XPacket* xPacket);
    bool ResCheckEnterMaze(XPacket* xPacket);

    // User chat/notification packet handlers (IDA addresses)
    // Per IDA 0x1401d14c0: 用户公告通知
    bool RecvUserNotice(XPacket* xPacket);
    // Per IDA 0x1401d1680: 喇叭消息
    bool RecvUserMegaPhone(XPacket* xPacket);
    // Per IDA 0x1401d1ab0: 交易密码状态
    bool RecvUserTradePasswordState(XPacket* xPacket);
    // Per IDA 0x1401d1da0: 私聊响应
    bool RecvUserWhisperRes(XPacket* xPacket);

    // WorldMode packet handlers (IDA addresses)
    // Per IDA 0x1401d3a10: WorldMode开始
    bool RecvWorldModeStart(XPacket* xPacket);
    // Per IDA 0x1401d3c00: WorldMode更新
    bool RecvWorldModeUpdate(XPacket* xPacket);
    // Per IDA 0x1401d3d00: WorldMode结束
    bool RecvWorldModeFinish(XPacket* xPacket);
    // Per IDA 0x1401d3fc0: WorldMode同步
    bool RecvWorldModeSync(XPacket* xPacket);
    // Per IDA 0x1401d42c0: WorldMode命令
    bool RecvWorldModeCommand(XPacket* xPacket);
    // Per IDA 0x1401d4920: WorldMode清除
    bool RecvWorldModeClear(XPacket* xPacket);
    // Per IDA 0x1401d5a40: WorldMode完成
    bool RecvWorldModeComplete(XPacket* xPacket);

    // Send methods (IDA addresses)
    // Per IDA 0x1401d3890: 发送检查包
    void SendCheck(XSendPacket* pPacket);
    // Per IDA 0x1401d3900: 发送命令包
    bool SendCmd(XSendPacket* pPacket, CUser* pUser, std::uint8_t byMainCmd, std::uint8_t bySubCmd);
    // Per IDA 0x1401d39d0: 检查是否可以发送
    bool IsCanSend();

    // IDA 0x1401d2320: 连接回调
    virtual void OnConnect();

    // IDA 0x1401d22f0: 断开连接回调
    virtual void OnDisConnect();

    // IDA 0x1401d2180: 线程启动回调
    virtual void OnStartThread();

    // IDA 0x1401d2280: 线程结束回调
    virtual void OnFinishThread();

private:
    // === IDA 确认的成员变量 ===
    // 服务器信息
    struct {
        unsigned int dwID;
        int nGroup;
        int nType;
        int nChannel;
        int nState;
        std::int16_t sPort;
        int nCurUser;
        int nMaxUser;
        std::int16_t sThreadCount;
        char szPrivateIP[513];
        char szPublicIP[513];
        char szName[21];
    } m_myInfo;

    // Relay 信息
    struct {
        char szName[21];
        char szPrivateIP[513];
        std::int16_t sPort;
    } m_relayInfo;

    // 缓存加载状态
    unsigned int m_dwCachingLoad;
    bool m_bSyncUserInfoReq;
    bool m_bCachingLoadFinish;

    // Memory router (从 IDA 反编译确认)
    char memoryRouter[8]; // hkMemoryRouter placeholder
};
