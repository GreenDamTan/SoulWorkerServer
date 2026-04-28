// ControlServer.h
// ControlServer 服务器主类定义

#pragma once

#include "Soulworker/GameServer/XCore/XServer/TXMultiPoolServer.h"
#include "Soulworker/GameServer/XCore/XServer/XGameDBSocketMgr.h"
#include "Soulworker/GameServer/XCore/XServer/CObserveSocket.h"
#include "Soulworker/GameServer/XCore/XServer/XSeed.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XSCommon/Table/XItemFactory.h"
#include "Soulworker/GameServer/XLoginServer/PartyManager.h"
#include "Soulworker/GameServer/XLoginServer/WorldModeMgr.h"
#include "Soulworker/GameServer/XLoginServer/DayEventManager.h"
#include "Soulworker/GameServer/XLoginServer/RouletteEventManager.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"
#include "WorldManager.h"
#include "UserObject.h"
#include "MazeInfo.h"
#include <list>
#include <map>
#include <unordered_map>
#include <memory>

// 前向声明
class CServer;
struct ST_CREATE_MAZE;
struct ST_GO_BACK_MAZE;
struct ST_CREATE_MAZE_FOR_RELAY;
struct ST_CREATE_MODE_MAZE_FOR_RELAY;
struct ST_LOG_GAME;
class CForceManager;
class CMyRoom;

// 注意: ST_MYROOM_USER 和 ST_MYROOM_OWNER_INFO 已在 ServerProcess.h 中定义

// GM 相关结构体前向声明 (定义在 PSServer.h)
struct ST_GM_USER_KICK_INFO;
struct ST_GM_NOTICE_INFO;
struct ST_GM_TIME_EVENT_INFO;
struct ST_BANNER_LIST;
struct ST_GM_VALUE_EVENT_INFO;
struct PS_GM_VALUE_EVENT_LIST;

// ============================================================================
// GreenDamTan_ClassFactory - 对象工厂模板 (替代 ClassFactory)
// 对齐 IDA: ClassFactory<T, PoolSize> 使用 boost::object_pool 进行内存池管理
// 原始实现: create() 使用 boost::object_pool::construct + shared_ptr(destroy bind)
// 简化实现: 当前使用 std::make_shared，后续可替换为 boost::object_pool
// ============================================================================
template<typename T, size_t PoolSize>
class GreenDamTan_ClassFactory {
public:
    GreenDamTan_ClassFactory() = default;
    ~GreenDamTan_ClassFactory() = default;

    // 对齐 IDA: ?create@?$ClassFactory@VCMazeInfo@@$0EA@@@QEAA?AV?$shared_ptr@VCMazeInfo@@@tr1@std@@XZ
    // 原始使用 boost::object_pool::construct，简化为 std::make_shared
    std::shared_ptr<T> Create() {
        return std::make_shared<T>();
    }

    // 对齐 IDA: ?destroy@?$ClassFactory@VCMazeInfo@@$0EA@@@QEAAXPEAVCMazeInfo@@@Z
    // 原始使用 boost::object_pool::destroy，简化为空实现(shared_ptr 自动管理)
    void Destroy(std::shared_ptr<T> pObj) {
        // shared_ptr 析构自动释放，无需显式调用
    }

private:
    // 原始 IDA 成员布局: boost::object_pool<T, boost::default_user_allocator_new_delete> allocator
    // 简化实现不保留 allocator，后续需要 boost::object_pool 时可添加
};

// 别名，用于兼容原代码
template<typename T, size_t PoolSize>
using ClassFactory = GreenDamTan_ClassFactory<T, PoolSize>;

// ST_WAIT_ENTER_SERVER 结构体定义
struct ST_WAIT_ENTER_SERVER {
    DWORD dwActorID = 0;
    DWORD dwServerID = 0;
    ULONGLONG dwTick = 0;
};

// 使用 std::tr1 命名空间 (VS2010 兼容)
namespace std { namespace tr1 = std; }

// ============================================================================
// GreenDamTan_UserIndexContainer - 用户索引容器 (替代 boost::multi_index)
// 对齐 IDA: boost::multi_index_container<std::tr1::shared_ptr<CUserObject>, indices>
// 索引: CID (hashed_unique), UAID (hashed_unique), Name (hashed_unique), ServerID (ordered_non_unique)
// ============================================================================
class GreenDamTan_UserIndexContainer
{
public:
    // 按 ActorID 查找
    std::tr1::shared_ptr<CUserObject> FindByCID(DWORD dwActorID) const {
        auto it = m_mapByCID.find(dwActorID);
        return it != m_mapByCID.end() ? it->second : nullptr;
    }

    // 按名称查找
    std::tr1::shared_ptr<CUserObject> FindByName(const std::wstring& strName) const {
        auto it = m_mapByName.find(strName);
        return it != m_mapByName.end() ? it->second : nullptr;
    }

    // 按 C 风格字符串名称查找
    std::tr1::shared_ptr<CUserObject> FindByName(const wchar_t* pName) const {
        if (!pName) return nullptr;
        return FindByName(std::wstring(pName));
    }

    // 按 UAID 查找
    std::tr1::shared_ptr<CUserObject> FindByUAID(DWORD dwUAID) const {
        auto it = m_mapByUAID.find(dwUAID);
        return it != m_mapByUAID.end() ? it->second : nullptr;
    }

    // 对齐 IDA boost::multi_index::get<1>::equal_range(ServerID)
    // 返回指定 ServerID 的所有用户
    std::vector<std::tr1::shared_ptr<CUserObject>> FindByServerID(DWORD dwServerID) const {
        std::vector<std::tr1::shared_ptr<CUserObject>> vecResult;
        auto range = m_mapByServerID.equal_range(dwServerID);
        for (auto it = range.first; it != range.second; ++it) {
            vecResult.push_back(it->second);
        }
        return vecResult;
    }

    // 插入用户
    bool Insert(std::tr1::shared_ptr<CUserObject> pUser) {
        if (!pUser) return false;

        DWORD dwCID = pUser->GetCID();
        DWORD dwUAID = pUser->GetUAID();
        DWORD dwServerID = pUser->GetServerID();
        const std::wstring& strName = pUser->GetName();

        // 检查重复
        if (m_mapByCID.count(dwCID)) return false;
        if (m_mapByUAID.count(dwUAID)) return false;
        if (!strName.empty() && m_mapByName.count(strName)) return false;

        // 插入各索引
        m_mapByCID[dwCID] = pUser;
        m_mapByUAID[dwUAID] = pUser;
        m_mapByServerID.emplace(dwServerID, pUser);  // ServerID 允许重复
        if (!strName.empty()) {
            m_mapByName[strName] = pUser;
        }
        return true;
    }

    // 删除单个用户 (按 ActorID)
    void Erase(DWORD dwActorID) {
        auto it = m_mapByCID.find(dwActorID);
        if (it == m_mapByCID.end()) return;

        auto pUser = it->second;
        m_mapByCID.erase(it);

        if (pUser) {
            m_mapByUAID.erase(pUser->GetUAID());
            const std::wstring& strName = pUser->GetName();
            if (!strName.empty()) {
                m_mapByName.erase(strName);
            }
            // 从 ServerID 索引删除 (需要遍历找到对应项)
            EraseFromServerIDIndex(pUser);
        }
    }

    // 对齐 IDA RemoveGameServerInfo: 删除指定 ServerID 的所有用户
    // 返回被删除的用户数量
    size_t EraseByServerID(DWORD dwServerID) {
        auto range = m_mapByServerID.equal_range(dwServerID);
        if (range.first == range.second) return 0;

        // 收集要删除的用户 ActorID
        std::vector<DWORD> vecActorIDs;
        for (auto it = range.first; it != range.second; ++it) {
            if (it->second) {
                vecActorIDs.push_back(it->second->GetCID());
            }
        }

        // 从其他索引删除
        for (DWORD dwActorID : vecActorIDs) {
            Erase(dwActorID);
        }

        return vecActorIDs.size();
    }

    // 获取用户数量
    size_t size() const { return m_mapByCID.size(); }
    size_t Size() const { return size(); }

    // 对齐 IDA boost::multi_index: 更新用户服务器引用
    // 用于 UpdateUserMap 中当 map 高字 (server/channel) 变化时更新索引
    void UpdateUserServer(std::tr1::shared_ptr<CUserObject> pUser, CServer* pNewServer) {
        if (!pUser || !pNewServer) return;

        DWORD dwOldServerID = pUser->GetServerID();
        DWORD dwNewServerID = pNewServer->GetServerID();

        if (dwOldServerID == dwNewServerID) {
            // ServerID 相同，只更新指针
            pUser->SetServer(pNewServer);
            return;
        }

        // 从旧 ServerID 索引删除
        EraseFromServerIDIndex(pUser);

        // 更新用户的服务器引用 (同时更新 ServerID)
        pUser->SetServer(pNewServer);

        // 添加到新 ServerID 索引
        m_mapByServerID.emplace(dwNewServerID, pUser);
    }

private:
    // 从 ServerID 索引中删除指定用户 (辅助函数)
    void EraseFromServerIDIndex(std::tr1::shared_ptr<CUserObject> pUser) {
        if (!pUser) return;
        DWORD dwServerID = pUser->GetServerID();
        auto range = m_mapByServerID.equal_range(dwServerID);
        for (auto it = range.first; it != range.second; ++it) {
            if (it->second && it->second->GetCID() == pUser->GetCID()) {
                m_mapByServerID.erase(it);
                return;
            }
        }
    }

    std::unordered_map<DWORD, std::tr1::shared_ptr<CUserObject>> m_mapByCID;      // ActorID -> User
    std::unordered_map<DWORD, std::tr1::shared_ptr<CUserObject>> m_mapByUAID;     // UAID -> User
    std::unordered_map<std::wstring, std::tr1::shared_ptr<CUserObject>> m_mapByName; // Name -> User
    // 对齐 IDA: ordered_non_unique<GetServerID> - 使用 multimap 模拟
    std::unordered_multimap<DWORD, std::tr1::shared_ptr<CUserObject>> m_mapByServerID; // ServerID -> User (non-unique)
};

// ============================================================================
// XControlServer - 控制服务器主类
// ============================================================================
class XControlServer : public TXMultiPoolServer<CServer>
{
    typedef TXMultiPoolServer<CServer> BaseClass;

public:
    // 单例访问
    static XControlServer* Instance();

    XControlServer();
    virtual ~XControlServer();

    // 初始化
    virtual bool InitServer() override;
    virtual bool Clear() override;

    // 用户管理
    bool AddUser(CServer* pServer, STCharInfo& stInfo, UXMapID uxMapID,
                 DWORD dwIP, BYTE byTradePW, __int64 biSessionID, BYTE byBlockType);
    void RemoveUser(DWORD dwActorID, int nAccountState, bool bKick_AlreadyLogin);
    std::tr1::shared_ptr<CUserObject> GetUser(DWORD dwActorID);
    std::tr1::shared_ptr<CUserObject> GetUser(const wchar_t* szName);

    // 世界管理器访问
    CWorldManager& GetWorldManager() { return m_worldManager; }

    // Party/Force管理器访问
    CPartyManager& GetPartyManager() { return m_partyManager; }
    CForceManager& GetForceManager() { return m_forceManager; }

    // 迷宫工厂访问
    ClassFactory<CMazeInfo, 64>& GetMazeFactory() { return m_factoryMaze; }

    // 世界模式管理器访问
    CWorldModeMgr& GetWorldModeManager() { return m_worldModeManager; }

    // 用户地图更新
    void UpdateUserMap(CServer* pServer, PS_UPDATE_USER_MAP_INFO& stInfo);

    // 踢出用户
    void KickoutUser_UseLock(PS_KICK_USER_INFO& stInfo, bool bSend);
    void KickoutUser_NoLock(PS_KICK_USER_INFO& stInfo, bool bSend);

    // 发送包
    bool SendPacketToLoginServer(XSendPacket& packet);
    bool SendPacketToGameServer(XSendPacket& packet, CServer* pServer);
    bool SendPacketAll(XSendPacket& packet, bool bExcludeLogin);

    // 服务器信息管理
    void AddLoginServerInfo(CServer* pServer);
    void AddCommunityServerInfo(CServer* pServer);
    void AddGameServerInfo(CServer* pServer);
    void AddMazeServerInfo(CServer* pServer, int nIndex);
    void AddServerInfo(CServer* pServer);
    void RemoveGameServerInfo(SS_SERVER_INFO& stInfo);
    void RemoveServerInfo(CServer* pServer, SS_SERVER_INFO& stInfo);

    // 迷宫创建
    void ReqCreateMaze(CServer* pServer, ST_CREATE_MAZE& stCreate);
    void ReqGoBackMaze(CServer* pServer, ST_GO_BACK_MAZE& stGoBack);
    void ResCreateMaze(ST_CREATE_MAZE& stCreate);

    // 匹配迷宫创建 (对齐 IDA)
    bool CreateMatchingMaze(ST_CREATE_MAZE& stCreateMaze, PS_PARTY_INFO& stPartyInfo, DWORD dwMatchingID);
    bool CreateMatchingMaze(ST_CREATE_MAZE& stCreateMaze, PS_FORCE_INFO& stForceInfo, DWORD dwMatchingID);
    void ResCreateMatchingMaze(DWORD dwMatchingID, ST_CREATE_MAZE stCreateMaze, PS_PARTY_INFO stPartyInfo);
    void ResCreateMatchingMaze(DWORD dwMatchingID, ST_CREATE_MAZE stCreateMaze, PS_FORCE_INFO stForceInfo);

    // 模式迷宫创建 (对齐 IDA)
    bool CreateMatchingModeMaze(ST_CREATE_MODE_MAZE& stCreateModeMaze);
    void ResCreateModeMaze(ST_CREATE_MODE_MAZE& stCreateModeMaze);

    // 更新
    virtual void OnUpdate(ULONGLONG dwTick) override;
    void UpdateServerState();

    // 状态更新
    void UpdateAccountState(DWORD dwUAID, int nState);
    void ClearUserState(DWORD dwServerID);
    int CalculateServerUserStaus();
    bool CheckUserCount();
    void UpdateMaxServerUserCount(int nMaxCount);

    // 对齐 IDA: GetLoginServer 返回 m_pLoginServer
    CServer* GetLoginServer() { return m_pLoginServer; }

    // 服务器获取
    CServer* GetServer(DWORD dwServerID);

    // 聊天
    void SendChatNotice(PS_CHAT_NOTICE& stNotice);
    void SendChatMegaPhone(PS_CHAT_MEGAPHONE& stMega, PS_CHAT_ITEM_LINK_FOR_SERVER& stLink);

    // 迷宫检查
    void CheckPartyInMaze(CServer* pServer, PS_ENTER_MAP_REQ& stEnter);
    void CheckForceInMaze(CServer* pServer, PS_ENTER_MAP_REQ& stEnter);

    // 服务器切换
    void ChangeServer(CServer* pServer, PS_REQ_CHANGE_SERVER& stChange);

    // 用户信息设置
    void SetUsersInfo(CServer* pServer, PS_USERS_INFO& stUsers);

    // 日志
    void SendDBLog(int nType, int nSubType, SHORT sMainType, SHORT sSubType,
                   int nParam0, int nParam1, int nParam2, int nParam3,
                   int nParam4, __int64 biParam5, __int64 biParam6, wchar_t* szString);
    void SendDBLog(ST_LOG_GAME& stLog);

    // GM命令
    void GM_UserKick(CServer* pServer, ST_GM_USER_KICK_INFO& stInfo);
    void GM_Notice(CServer* pServer, ST_GM_NOTICE_INFO& stInfo);
    void GM_Shutdown(CServer* pServer, DWORD dwTime);
    void GM_TimeEvent(CServer* pServer, ST_GM_TIME_EVENT_INFO& stInfo);
    void GM_ValueEvent(CServer* pServer, PS_GM_VALUE_EVENT_LIST& stList);
    void GM_ServerOption(CServer* pServer, PS_CONTENTS_INFO& stInfo);
    void GM_CashShopBanner(CServer* pServer, ST_BANNER_LIST& stList);

    // 迷宫查找
    CServer* FindServerFromMaze(UXMapID uxMapID);
    std::tr1::shared_ptr<CMazeInfo> GetMazeInfo(UXMapID uxMapID);

    // 数据库发送
    bool SendDBAccount(XSendDBPacket& packet);
    bool SendDBGame(XSendDBPacket& packet);
    bool SendDBLog(XSendDBPacket& packet);
    bool SendCommunity(XSendPacket& packet);

    // 迷宫准入检查
    int CheckAdmissionMember(int nType, bool bBreakInto, bool bParty, bool bForce, bool bFull);

    // 认证类型更新
    bool UpdateAuthType(DWORD dwUAID, BYTE byAuthType);

    // 缓存加载状态
    void SetCachingLoad(DWORD dwState);
    void UnSetCachingLoad(DWORD dwState);
    void SendCachingLoad();
    bool IsAccountDBConnection();
    bool IsLogDBConnection();
    bool IsCompleteCachingLoad();
    DWORD GetCachingLoad() const { return m_dwCachingLoad; }

    // 金币供应 (对齐 IDA 0x140046690)
    void SetMoneySupply(__int64 biMoney) { m_nMoneySupply += biMoney; }
    __int64 GetMoneySupply() const { return m_nMoneySupply; }

    // 服务器组信息同步
    void SendAccountDBLoginAddServerGroupInfo();

    // 资源管理器访问
    XResourceMgr& GetResourceMgr() { return m_xResourceMgr; }

    // 物品工厂访问
    XItemFactory& GetItemFactory() { return m_xItemFactory; }

    // MyRoom 服务器查找
    CServer* GetMyRoomServer();
    std::tr1::shared_ptr<CMyRoom> FindMyRoom(DWORD dwUAID);
    void AddMyRoomServerInfo(CServer* pServer);
    void RemoveMyRoomServerInfo(DWORD dwServerID);

    // 会话检查 (对齐 IDA)
    void CheckSessionID(CServer* pServer, DWORD dwUAID, __int64 biAuthSessionID);

    // MyRoom 创建请求 (对齐 IDA)
    void ReqCreateMyRoom(int nResult, DWORD dwOwnerUAID, ST_MYROOM_USER& stCreateUser, CServer* pMyRoomServer, DWORD dwOwnerUCID);

    // MyRoom 删除请求/响应 (对齐 IDA)
    void DeleteMyRoomReq(DWORD dwOwnerUAID, UXMapID uxMapID, CServer* pServer);
    void DeleteMyRoomRes(DWORD dwOwnerUAID, UXMapID uxMapID, CServer* pServer);

    // MyRoom 进入请求 (对齐 IDA)
    bool MyRoomEnterReq(ST_MYROOM_USER stEnterUser, ST_MYROOM_OWNER_INFO stOwnerInfo, CServer* pSendServer, DWORD dwOwnerUCID);

    // 迷宫成员进入 (对齐 IDA)
    bool EnterMemberInMaze(CServer* pServer, DWORD dwPartyID, UXMapID uxMapID, PS_ENTER_MAP_REQ& stEnterMap);
    bool EnterMemberInMazeForce(CServer* pServer, DWORD dwForceID, UXMapID uxMapID, PS_ENTER_MAP_REQ& stEnterMap);

    // 迷宫进入检查 (对齐 IDA)
    void ReqCheckEnterMaze(CServer* pServer, ST_SERVER_CHECK_ENTER_MAZE& stCheckEnter);
    void ReqDisconnectUserSync(__int64 nMapInstance, ST_MAZE_WAIT_ENTER_USER_INFO& stDisconnect);

    // 服务器组ID检查响应 (对齐 IDA)
    void ResCheckServerGroupID(DWORD dwUAID, int nServerGroupID, int nLastServerGroupID);

    // 交易密码状态 (对齐 IDA)
    void SendUserTradePasswordStateSync(CServer* pReqServer, DWORD dwUCID, std::uint8_t byTradePWState);
    void SendUserTradePasswordState(CServer* pReqServer, DWORD dwUCID);

    // 角色改名 (对齐 IDA)
    void CharacterNameChange(DWORD dwActorID, wchar_t* pChangeName);

    // 轮盘事件更新 (对齐 IDA)
    void UpdateRouletteEventInfo(PS_ROULETTE_EVENT_UPDATE_SERVER& stUpdateInfo);

    // 重新进入地图请求 (对齐 IDA)
    void ReqReEnterMap(CServer* pServer, PS_ENTER_MAP_REQ& stInfo);

    // 同步事件迷宫 (对齐 IDA)
    void SyncEventMaze(PS_MAZE_UPDATE_INFO_SYNC& stMazeInfo);

protected:
    // 对齐 IDA 0x140015090 (XControlServer::WriteLog)
    virtual void WriteLog(const char* szLog, ...);

private:
    // 成员变量
    GreenDamTan_UserIndexContainer m_UserInfos;                     // 用户信息容器 (+0xB4C0)
    ClassFactory<CMazeInfo, 64> m_factoryMaze;                      // 迷宫工厂 (+0xB600, 48 bytes)
    std::map<DWORD, CServer*> m_mapGameServer;                      // 游戏服务器映射 (+0xB630, 32 bytes)
    std::map<DWORD, CServer*> m_mapMazeServer;                      // 迷宫服务器映射 (+0xB650, 32 bytes)
    std::map<DWORD, CServer*> m_mapMyRoomServer;                    // MyRoom服务器映射 (+0xB670, 32 bytes)
    CServer* m_pLoginServer;                                        // 登录服务器 (+0xB690, 8 bytes)
    CServer* m_pCommunityServer;                                    // 社区服务器 (+0xB698, 8 bytes)
    CServer* m_pMonitor;                                            // 监控服务器 (+0xB6A0, 8 bytes)
    bool m_bAddLogin;                                               // 添加登录标志 (+0xB6A8, 1 byte)
    std::map<UXMapID, ST_CREATE_MAZE_FOR_RELAY> m_mapCreateMazeReq; // 创建迷宫请求 (+0xB6B0, 32 bytes)
    std::map<UXMapID, ST_CREATE_MODE_MAZE_FOR_RELAY> m_mapCreateModeMazeReq; // 模式迷宫请求 (+0xB6D0, 32 bytes)
    std::map<UXMapID, std::tr1::shared_ptr<CMazeInfo>> m_mapBattleZoneInfo; // 战场信息 (+0xB6F0, 32 bytes)
    ST_SERVER_GROUP_INFO m_stServerGroupInfo;                       // 服务器组信息 (+0xB710, 548 bytes)
    int m_nMaxServerUserCount;                                      // 最大用户数 (+0xB934, 4 bytes)
    XSeed m_xSeed;                                                  // 随机种子 (+0xB938, 168 bytes)
    bool m_bRegisterAuth;                                           // 注册认证标志 (+0xB9E0, 1 byte)
    XGameDBSocketMgr m_xDBAgentMgr;                                 // DB代理管理 (+0xB9E8, 72 bytes)
    CObserveSocket m_scObserveSocket;                               // 观察socket (+0xBA30, 69720 bytes)
    CFSRWLock m_rwLock;                                             // 读写锁 (+0x1CAD8, 8 bytes)
    CFSRWLock m_rwServerLock;                                       // 服务器锁 (+0x1CAE0, 8 bytes)
    __int64 m_nMoneySupply;                                         // 金币供应 (+0x1CAE8, 8 bytes)
    DWORD m_dwCachingLoad;                                          // 缓存加载状态 (+0x1CAF0, 4 bytes)
    CWorldManager m_worldManager;                                   // 世界管理器 (+0x1CAF8, 72 bytes)
    XResourceMgr m_xResourceMgr;                                    // 资源管理 (+0x1CB40, 16216 bytes)
    CPartyManager m_partyManager;                                   // 组队管理 (+0x20AA8, 40 bytes)
    CForceManager m_forceManager;                                   // Force管理 (+0x20AD0, 40 bytes)
    CWorldModeMgr m_worldModeManager;                               // 世界模式管理 (+0x20AF8, 72 bytes)
    CDayEventMgr m_dayEventManager;                                 // 日事件管理 (+0x20B40, 56 bytes)
    CRouletteEventMgr m_rouletteEventManager;                       // 轮盘事件管理 (+0x20B78, 224 bytes)
    XItemFactory m_xItemFactory;                                    // 物品工厂 (+0x20C58, 272 bytes)
    std::list<ST_WAIT_ENTER_SERVER> m_listWaitEnterServer;          // 等待进入服务器列表 (+0x20D68, 24 bytes)
    int m_nSGAuthTypeCount[4];                                      // 认证类型计数 (+0x20D80, 16 bytes)
    std::map<DWORD, __int64> m_mapEventMazeToEnter;                 // 事件迷宫入口 (+0x20D90, 32 bytes)
    std::map<DWORD, std::tr1::shared_ptr<CMyRoom>> m_mapMyRoomInfo;  // MyRoom信息 (+0x20DB0, 32 bytes)
};

// 认证类型枚举
enum E_SG_AUTH_TYPE
{
    E_SG_AUTH_TYPE_NORMAL  = 0,
    E_SG_AUTH_TYPE_PANGYA = 1,
    E_SG_AUTH_TYPE_STEAM  = 2,
};

// 服务器缓存加载状态
enum E_SERVER_CACHING_LOAD
{
    E_SERVER_CACHING_LOAD_DB_COMPLETE = 0x01,
    E_SERVER_CACHING_LOAD_READY       = 0x02,
    E_SERVER_CACHING_LOAD_USER        = 0x04,
};
// 注意: E_SERVER_SYNC_LOAD 已在 ServerProcess.h 中定义

// ============================================================================
// GM 结构体序列化函数 (对齐 IDA)
// ============================================================================
// 注意: 序列化函数已在前面定义，此处仅声明
