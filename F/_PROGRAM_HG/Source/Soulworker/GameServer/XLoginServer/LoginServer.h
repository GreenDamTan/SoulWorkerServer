#pragma once

#include <array>
#include <atomic>
#include <deque>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "Soulworker/GameServer/XLoginServer/GreenDamTan_XItemFactory.h"
#include "Soulworker/GameServer/XLoginServer/User.h"

class XGameDBSocket;

/** @brief 登录服连接 CONTROL / Relay 的客户端骨架。 */
class XRelaySocket : public XIOCPClient {
public:
    struct RelayInfo {
        char szName[21] = {};
        char szPrivateIP[513] = {};
        std::int16_t sPort = 0;
        int nState = 0;
    };

    struct GreenDamTan_PendingCheckSessionReply {
        unsigned int uaid = 0;
        std::uint64_t authSessionID = 0;
        std::uint64_t queuedTick = 0;
        int sessionID = -1;
    };

    struct GreenDamTan_PendingChangeServerReply {
        PS_REQ_CHANGE_SERVER request{};
        std::uint64_t queuedTick = 0;
        int sessionID = -1;
    };

    ~XRelaySocket() override;

    /**
     * @brief 发送踢线请求到 Relay 层。
     * @param kickInfo 踢线信息。
     */
    void SendUserKickout(const PS_KICK_USER_INFO& kickInfo);

    /** @brief 记录一条创建迷宫请求。 */
    void SendCreateMazeReq(const ST_CREATE_MAZE& createMaze);

    /** @brief 记录一条“回角色选择/认证服”请求，供重建工程在缺少下游服务时兜底回灌。 */
    void GreenDamTan_RecordPendingChangeServer(const PS_REQ_CHANGE_SERVER& request, int sessionID);

    /** @brief 移除已完成的 change-server 兜底记录。 */
    void GreenDamTan_ClearPendingChangeServer(unsigned int uaid);

    /** @brief 记录一条等待 CONTROL 校验回包的角色列表请求，供缺少下游服务时本地兜底回灌。 */
    void GreenDamTan_RecordPendingCheckSession(unsigned int uaid, std::uint64_t authSessionID, int sessionID);

    /** @brief 移除已完成的 SessionID 校验兜底记录。 */
    void GreenDamTan_ClearPendingCheckSession(unsigned int uaid);

    /** @brief 初始化控制口底层 socket 与后台解析线程。 */
    bool Init(E_POOL_ID poolId, const char* ip, std::uint16_t port) override;

    /** @brief 缓存登录服自身配置，便于后续继续补控制口握手链。 */
    virtual void SetMyInfo(const XOption* option);

    /** @brief 连接到 `CONTROL` 私网地址。 */
    bool Connect() override;

    /** @brief 后台解析线程主体。 */
    void OnStartThread();
    /** @brief 后台解析线程收尾。 */
    void OnFinishThread();
    /** @brief 兼容 PDB 中的线程入口名称。 */
    static std::uint64_t _BackendThreadStartingPoint(void* ptr);

    /** @brief 真实连接建立后立刻向 CONTROL 上报自身服务器信息。 */
    void OnConnect() override;
    /** @brief 真实连接断开后的状态收口。 */
    void OnDisConnect() override;
    /** @brief 连接 CONTROL 失败时的日志收口。 */
    void OnNotConnect() override;
    /** @brief 分派 CONTROL/Relay 回包主命令。 */
    bool OnParse(XPacket& xPacket) override;

    bool ServerProcess(XPacket& xPacket);
    virtual bool ServerProcessEx(XPacket& xPacket);
    bool UserProcess(XPacket& xPacket);
    bool PartyProcess(XPacket& xPacket);
    bool FriendProcess(XPacket& xPacket);
    bool LeagueProcess(XPacket& xPacket);
    bool ForceProcess(XPacket& xPacket);
    bool WorldModeProcess(XPacket& xPacket);
    bool ModeMazeProcess(XPacket& xPacket);

    bool RecvServerUpdate(XPacket& xPacket);
    void RecvPacketFromRelay(XPacket& xPacket);
    bool RecvChangeChannelRes(XPacket& xPacket);
    bool RecvUpdateChannelAll(XPacket& xPacket);
    bool RecvUpdateChannel(XPacket& xPacket);

    virtual bool RecvUserKickout(XPacket& xPacket);
    bool RecvUserWhisperRes(XPacket& xPacket);
    bool RecvUserNotice(XPacket& xPacket);
    virtual bool RecvUserChangeServer(XPacket& xPacket);
    bool RecvUserEnterServer(XPacket& xPacket);
    bool RecvUserMegaPhone(XPacket& xPacket);
    bool RecvUserTradePasswordState(XPacket& xPacket);
    bool RecvExchangePriceHistory(XPacket& xPacket);
    bool RecvExchangePost(XPacket& xPacket);
    virtual bool RecvCheckSessionID(XPacket& xPacket);
    bool RecvGFBillingPostReload(XPacket& xPacket);

    /** @brief 对齐原版 `0xF2/0x01` 的 AddServer 上报。 */
    void SendAddServer();

    /** @brief 对齐 `XRelaySocket::SendUpdateServerInfo` 的最小语义。 */
    void SendUpdateServerInfo(std::int16_t nState, int nUserCount);

    /** @brief 当前控制/Relay 是否已完成可发业务包的就绪门槛。 */
    bool IsReady() const;

protected:
    SS_SERVER_INFO m_myInfo{};
    RelayInfo m_relayInfo{};
    int m_nSyncServerData = 0;

private:
    void GreenDamTan_ProcessPendingCheckSessionReplies();
    void GreenDamTan_ProcessPendingChangeServerReplies();

    std::thread backendThread_;
    std::atomic<bool> backendStop_{false};
    std::mutex pendingCheckSessionLock_;
    std::unordered_map<unsigned int, GreenDamTan_PendingCheckSessionReply> pendingCheckSessionReplies_;
    std::mutex pendingChangeServerLock_;
    std::unordered_map<unsigned int, GreenDamTan_PendingChangeServerReply> pendingChangeServerReplies_;
};

class CLoginControlSocket : public XRelaySocket {
public:
    void SetMyInfo(const XOption* option) override;
    bool ServerProcessEx(XPacket& xPacket) override;
    bool RecvUserKickout(XPacket& xPacket) override;
    bool RecvUserChangeServer(XPacket& xPacket) override;
    bool RecvCheckSessionID(XPacket& xPacket) override;

    bool RecvServerShutDown(XPacket& xPacket);
    bool RecvCreateMazeRes(XPacket& xPacket);
    bool RecvEnterServer(XPacket& xPacket);
    bool RecvServerOptionUpdate(XPacket& xPacket);
    bool RecvMaxServerUserCount(XPacket& xPacket);
};

class CLogThreadManager {
public:
    bool Start(const char* szName);
    void End();

private:
    bool m_isStart = false;
    std::string m_strConfigPath;
};

class CXigncode {
public:
    bool Init();
    void ConnectUser(int sessionID, unsigned int nAddr);
    void DisconnectUser(int sessionID);
    void RecvXigncode(int sessionID, const PS_XIGNCODE_UPDATE& stXigncode);
    void Release();

private:
    bool m_bInit = false;
};

class XSeed {
public:
    void Init(bool seedFromRandomDevice);

private:
    std::uint32_t m_dwSeed = 0;
};

class XGameDBSocketMgr {
public:
    XGameDBSocketMgr() = default;
    ~XGameDBSocketMgr();

    void Init();
    void AutoConnect();
    void DisConnect();
    bool SendAccountDBAgent(int iIndex, const XSendDBPacket& xSendPacket);
    bool SendGameDBAgent(int iIndex, const XSendDBPacket& xSendPacket);
    bool SendLogDBAgent(int iIndex, const XSendDBPacket& xSendPacket);
    bool SendStatisticsDBAgent(int iIndex, const XSendDBPacket& xSendPacket);
    int GetStatisticsDBAgentCount() const;
    int GetGameDBAgentCount() const;
    int GetAccountDBAgentCount() const;
    int GetLogDBAgentCount() const;

private:
    void InitAgentGroup(std::uint8_t byType, XGameDBSocket*& ppAgents, int& nAgentCount);
    void AutoConnectGroup(XGameDBSocket* pAgents, int nAgentCount);
    void DisconnectGroup(XGameDBSocket* pAgents, int nAgentCount);
    void Clear();

    XGameDBSocket* m_pGameDBAgent = nullptr;
    int m_nGameAgentCnt = 0;
    XGameDBSocket* m_pAccountDBAgent = nullptr;
    int m_nAccountAgentCnt = 0;
    XGameDBSocket* m_pLogDBAgent = nullptr;
    int m_nLogAgentCnt = 0;
    XGameDBSocket* m_pStatisticsDBAgent = nullptr;
    int m_nStatisticsAgentCnt = 0;
    XGameDBSocket* m_pSGLogDBAgent = nullptr;
    int m_nSGLogAgentCnt = 0;
};

class CObserveSocket : public XRelaySocket {
public:
    bool StartUp(XOption* pOption);
    bool OnUpdate(std::uint64_t currentTick,
                  const char* ip,
                  int port,
                  int userCount,
                  bool controlConnect,
                  bool communityConnect,
                  int maxThreadCount,
                  bool netCafe);

private:
    void OnConnect() override;
    void OnDisConnect() override;
    void OnNotConnect() override;
    void CalculateThreadStatus(wchar_t* szLogicThread, int nMaxThreadCount);
    void SendReportServerStatus(int nServerType,
                                int nUserCount,
                                bool bControlConnect,
                                bool bCommunityConnect,
                                int nMaxThreadCount,
                                char* szIP,
                                int nPort,
                                bool bNetCafe);
    void SetMyInfo(const XOption* option) override;

    SS_SERVER_INFO m_observeInfo{};
    SS_REPORT_POOL_INFO m_poolInfo{};
    std::uint32_t m_dwProcessID = 0xFFFFFFFFu;
    bool m_bActivate = false;
    CFSRWLock m_rwThreadTickLock{};
    std::array<std::uint64_t, 20> m_dwThreadFpsTick_Now{};
    std::array<std::uint64_t, 20> m_dwThreadFpsTick_Last{};
};

/**
 * @brief LoginServer 主状态容器。
 *
 * 该类承载登录服在本轮还原中最核心的数据：
 * - 全局配置与资源表
 * - 在线用户映射
 * - 等待队列及其票号计数器
 * - 发往 AccountDB / GameDB 的包暂存
 * - 登录审计日志
 *
 * 依赖关系：
 * - `XLoginProcess` 使用它查询配置并转发 AccountDB 请求。
 * - `XGameDBSocket` 使用它登记在线用户、写审计日志。
 * - `CLoginControlSocket` 使用它查找在线用户并转发 GameDB 请求。
 */
class XLoginServer : public XServer, public TXServer<CUser> {
public:
    XLoginServer();

    struct LoginAuditRecord {
        int uaid = 0;
        std::uint8_t loginType = 0;
        int errorCode = 0;
        std::uint32_t clientIpv4 = 0;
        std::int64_t authSessionId = 0;
        std::wstring macAddress;
        std::wstring authId;
    };

    struct WaitEntry {
        CUser* user = nullptr;
        int uaid = 0;
        std::uint64_t ticketToken = 0;
        std::uint16_t groupId = 0;
    };

    /** @brief 判断登录服是否处于拒绝受理状态。 */
    bool IsServerAcceptClosed() const { return acceptClosed_; }
    /** @brief 设置登录服是否拒绝受理。 */
    void SetServerAcceptClosed(bool value) { acceptClosed_ = value; }

    /** @brief 获取资源表管理器。 */
    XResourceMgr& GetResourceMgr() { return resourceMgr_; }
    /** @brief 获取物品工厂。 */
    XItemFactory& GetItemFactory() { return m_xItemFactory; }

    /**
     * @brief 记录一条发往 AccountDB 的包。
     * @param packet 已构造好的 DB 包。
     * @return 按 `XSendDBPacket::GetOrderID() % DBAgentCount` 选槽后的转发结果。
     */
    bool SendDBAccount(const XSendDBPacket& packet);

    /**
     * @brief 记录一条发往 GameDB 的包。
     * @param packet 已构造好的 DB 包。
     * @return 按 `XSendDBPacket::GetOrderID() % DBAgentCount` 选槽后的转发结果。
     */
    bool SendDBGame(const XSendDBPacket& packet);
    /**
     * @brief 记录一条发往 LogDB 的包。
     *
     * IDA 中该函数会在 `SYSTEM_TYPE_DEV` 时退化为 `SendDBGame`；
     * 非 DEV 时按 `XSendDBPacket::GetOrderID() % DBAgentCount` 选槽。
     */
    bool SendDBLog(const XSendDBPacket& packet);
    /** @brief 记录一条发往 StatisticsDB 的包，并按 `OrderID % Count` 选槽。 */
    bool SendDBStatistics(const XSendDBPacket& packet);

    /**
     * @brief 把用户登记到在线映射表。
     * @param user 待登记的玩家对象。
     * @return 若旧连接 `AuthSessionID` 更新于当前连接则返回 false，否则返回 true。
     */
    bool EnterUser(CUser* user);

    /**
     * @brief 根据 UAID 查找在线用户。
     * @param uaid 用户账号 ID。
     * @return 找到返回用户指针，否则返回空。
     */
    CUser* FindUIDToUser(int uaid);

    /**
     * @brief 根据角色 UCID 登记“角色 -> 当前登录连接”的映射。
     * @param ucid 角色 ID。
     * @param user 当前持有该角色的连接对象。
     */
    void AddActor(unsigned int ucid, CUser* user);

    /**
     * @brief 根据角色 UCID 查找当前登录连接。
     * @param ucid 角色 ID。
     * @return 找到则返回用户指针，否则返回空。
     */
    CUser* FindActor(unsigned int ucid);

    /** @brief 按角色 UCID 从在线角色映射中移除一条记录。 */
    void RemoveActor(unsigned int dwUCID);

    /** @brief 按 UAID 回收在线账号映射。 */
    void ExitUser(CUser* pUser);

    /** @brief 按踢线类型广播踢出当前所有在线用户。 */
    void KickoutAll(std::uint8_t byType);

    /**
     * @brief 按基础参数构造 `ST_LOG_GAME` 并发送到 LogDB。
     *
     * 对应 PDB 中 `XLoginServer::WriteLogDB(int, int, short, short, ...)`，
     * 当前保持 `main=0x42, sub=0` 的发包方向。
     */
    bool WriteLogDB(int uaid,
                    int ucid,
                    std::uint16_t mainType,
                    std::uint16_t subType,
                    int param0,
                    int param1,
                    int param2,
                    int param3,
                    int param4,
                    std::int64_t param5,
                    std::int64_t param6,
                    const wchar_t* comment,
                    const wchar_t* comment2);

    /** @brief 兼容当前骨架中仍使用 `std::wstring` 的调用点。 */
    bool WriteLogDB(int uaid,
                    int ucid,
                    std::uint16_t mainType,
                    std::uint16_t subType,
                    int param0,
                    int param1,
                    int param2,
                    int param3,
                    int param4,
                    std::int64_t param5,
                    std::int64_t param6,
                    const std::wstring& comment,
                    const std::wstring& comment2) {
        return WriteLogDB(uaid,
                          ucid,
                          mainType,
                          subType,
                          param0,
                          param1,
                          param2,
                          param3,
                          param4,
                          param5,
                          param6,
                          comment.c_str(),
                          comment2.c_str());
    }

    /**
     * @brief 直接发送完整的 `ST_LOG_GAME` 结构。
     *
     * 对应 PDB 中 `XLoginServer::WriteLogDB(ST_LOG_GAME)`，
     * 当前保持 `main=0x42, sub=1` 的细日志发包方向。
     */
    void WriteLogDB(ST_LOG_GAME stLog);

    /**
     * @brief 把用户压入等待队列。
     * @param user 用户对象。
     * @param uaid 账号 UAID。
     * @param ticketToken 入服请求中附带的 session/token。
     * @param groupId 当前登录服分组索引。
     */
    void PushWaitUser(CUser* user, int uaid, std::uint64_t ticketToken, std::uint16_t groupId);

    /**
     * @brief 判断当前用户是否应收到“继续等待”的提示包。
     * @param ticket 用户的等待票号。
     * @return 返回 true 表示应告知仍需等待，false 表示不必额外发提示。
     */
    bool CheckUserWaitCountSend(std::int64_t ticket) const;

    /** @brief 弹出等待队列头部用户，供后续继续处理。 */
    CUser* PopWaitUser();

    /** @brief 获取最近一位被放行进入下阶段的等待票号。 */
    std::int64_t GetLastEnterWaitTicket() const { return m_nLastEnterWaitTicket; }
    /** @brief 设置最近放行票号。 */
    void SetLastEnterWaitTicket(std::int64_t ticket) { m_nLastEnterWaitTicket = ticket; }

    /** @brief 增加/减少当前已送往 GameDB 的排队用户数。 */
    void AddSendGameDBUserCount(int delta);
    /** @brief 获取当前已送往 GameDB 的排队用户数。 */
    int GetSendGameDBUserCount() const { return m_nSendGameDBUserCount; }

    /** @brief 获取控制服统计到的在线人数。 */
    int GetControlServerUserCount() const { return m_nControlServerUserCount; }
    /** @brief 设置控制服统计到的在线人数。 */
    void SetControlServerUserCount(int count) { m_nControlServerUserCount = count; }

    /** @brief 获取服务器允许的最大在线人数。 */
    int GetMaxServerUserCount() const { return m_nMaxServerUserCount; }
    /** @brief 设置服务器允许的最大在线人数。 */
    void SetMaxServerUserCount(int count) { m_nMaxServerUserCount = count; }
    /** @brief 按原版 `UpdateMaxUserCount` 语义同时更新最大人数与当前人数。 */
    void UpdateMaxUserCount(int nMaxCount, int nServerUserCount);

    /**
     * @brief 生成当前世界日期结构。
     * @param outDate 输出的当前时间信息。
     */
    void GetCurDate(ST_WORLD_CUR_DATE& outDate) const;
    /** @brief 获取当前时间戳。 */
    std::int64_t GetCurDate() const;

    /**
     * @brief 向客户端下发服务器组列表以及该账号在各组的角色数。
     * @param pUser 目标客户端。
     * @param stInfos 数据库返回的服务器角色计数列表。
     */
    void SendServerGroupList(CUser* pUser, ST_USER_CHARACTER_COUNT_FOR_SERVER_VEC& stInfos);

    /**
     * @brief 处理等待队列，把一部分玩家推进到 `sub=0x11` 的入服 DB 阶段。
     * @param biTick 当前调度时刻，单位毫秒。
     *
     * 依赖关系：
     * - 使用 `CUser` 上的等待态字段。
     * - 向 AccountDB 发送入服请求。
     * - 周期性给仍在排队的用户重发排队序号。
     */
    void ProcessWaitUser(std::uint64_t biTick);

    /**
     * @brief 更新控制服下发的内容开关。
     * @param contentsInfo 控制服发送的内容开关结构。
     */
    void SendServerOption_SecondPW(const PS_CONTENTS_INFO* contentsInfo);

    /** @brief 获取 Relay 控制口对象。 */
    XRelaySocket& GetControlSocket() { return controlSocket_; }
    /** @brief 获取 XignCode 适配器。 */
    CXigncode& GetXigncode() { return m_xignCode; }

    /** @brief IDA 中的控制台控制处理器。 */
    static int ConsolCtrlHandler(unsigned int dwOPCode);

protected:
    bool OnAccect(XClient* pClient) override;
    void SetName() override;
    bool InitServer() override;
    bool Clear(std::uint32_t maxWait) override;
    void OnUpdate(std::uint64_t currentTick) override;
    int SetConsoleHandler(int add) override;

private:
    /** @brief 模拟原始代码中的随机批次选择。 */
    int nRand(int minValue, int maxValue) const;
    bool acceptClosed_ = false;
    XResourceMgr resourceMgr_;
    XItemFactory m_xItemFactory;
    XGameDBSocketMgr m_xDBAgentMgr;
    CLoginControlSocket controlSocket_;
    CObserveSocket m_scObserveSocket;
    XSeed m_xSeed;
    CXigncode m_xignCode;
    TXObjectMgr<CUser> m_xUserObjectMgr;
    mutable std::shared_mutex usersByUaidLock_;
    mutable std::shared_mutex usersByActorIdLock_;
    std::unordered_map<int, CUser*> usersByUaid_;
    std::unordered_map<unsigned int, CUser*> usersByActorId_;
    std::deque<CUser*> waitQueue_;
    std::vector<LoginAuditRecord> loginAuditRecords_;

    std::int64_t m_nLastEnterWaitTicket = 0;
    std::int64_t m_nWaitTicket = 1;
    int m_nSendGameDBUserCount = 0;
    int m_nControlServerUserCount = 0;
    int m_nMaxServerUserCount = 1000;
    std::uint64_t m_nNextWaitNotifyTick = 0;
    std::uint64_t m_nNextProcessWaitUserTick = 0;
    std::uint64_t m_nAutoShutdownTick = 0;
    std::uint64_t m_dw64FPSTick = 0;
    std::uint64_t m_dwConnectTick = 0;
    std::uint64_t m_dwUpdateServerInfoTick = 0;
    std::uint64_t m_dwSGUpdateTick = 0;
    std::uint64_t m_dwWaitUserTick = 0;
    std::uint32_t m_dwFrame = 0;
    int m_nCheckUserCount = 0;
    PS_CONTENTS_INFO m_stContentsInfo{};
};
