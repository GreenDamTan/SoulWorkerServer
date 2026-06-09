#pragma once

#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include "Soulworker/Common/XNet/XCommon/PSWorld.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/GameServer/XCore/XServer/XSeed.h"
#include "Soulworker/GameServer/XCore/XServer/Option.h"
#include "Soulworker/GameServer/XCore/XServer/CFSRWLock.h"
#include "Soulworker/GameServer/XCore/XServer/CObserveSocket.h"
#include "Soulworker/GameServer/XCore/XServer/XGameDBSocketMgr.h"
#include "Soulworker/GameServer/XCore/XServer/TXDBSocket.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XGameServer/Xigncode.h"
#include "Soulworker/GameServer/XGameServer/XWorldResMgr.h"
#include "Soulworker/GameServer/XGameServer/XItemFactory.h"
#include "Soulworker/GameServer/XGameServer/ActionResMgr.h"
#include "Soulworker/GameServer/XGameServer/XAkashicResMgr.h"
#include "Soulworker/GameServer/XGameServer/ManagerStubs.h"
#include "Soulworker/GameServer/XGameServer/CGameCurlWrapper.h"
#include "Soulworker/GameServer/XGameServer/GameSockets.h"
#include "Soulworker/GameServer/XLoginServer/DayEventManager.h"
#include <cstdint>
#include <string>
#include <map>
#include <unordered_map>

// 前置声明
class CUser;
struct ST_LOG_GAME;
struct ST_CHAT_LOG_GAME;
struct ST_STAT_LOG_GAME;
struct ST_LOG_TEXT;
struct ST_LOG_SYSTEM;

// 全局变量声明 (来自 Vision 引擎)
extern std::string g_strCurPath_10;

// 前置声明 - Vision 引擎类型
class VGameHelper;
struct VisCallback_cl;
namespace Vision {
    struct Callbacks {
        static VisCallback_cl OnEngineInit;
    };
}

// XSendDBPacket 前置声明
class XSendDBPacket;

// XMaze 前置声明 (用于 m_spGameHelper)
// 注意: 这里使用 extern 声明全局变量，不是 namespace
extern VGameHelper* g_XMaze_GameHelper;

// 对齐 IDA 0x1402D8DE0 XGameServer::InitServer 分析
// XGameServer 实例大小约 0x42438 (271416 字节)
// 关键成员布局从反编译推断
class XGameServer : public TXServer<CUser> {
public:
    // 构造/析构
    XGameServer();
    virtual ~XGameServer();

    // 单例访问
    static XGameServer* Instance();

    // 核心生命周期
    bool InitServer() override;
    bool Clear() override;
    void OnUpdate(std::uint64_t dwTick) override;
    int SetConsoleHandler(int add) override;
    void SetName() override;

    // 用户管理
    void EnterUser(CUser* pUser);
    void ExitUser(CUser* pUser);
    CUser* FindNameToUser(wchar_t* pName);
    CUser* FindActorIDToUser(UXActorID uxActorID);
    CUser* FindUIDToUser(std::uint32_t dwUID);
    CUser* FindUser(std::uint32_t dwUID);  // Generic find user
    int GetOnlineCount();
    void BroadcastAll(void* pPacket, int nSize);
    void KickoutUserAll(std::uint8_t byType);

    // Packet Handlers
    void RecvChat(CUser* pUser, void* pPacket);
    void RecvMove(CUser* pUser, void* pPacket);
    void RecvAttack(CUser* pUser, void* pPacket);
    void RecvSkill(CUser* pUser, void* pPacket);
    void RecvItem(CUser* pUser, void* pPacket);

    // Database Operations
    bool SaveUser(CUser* pUser);
    bool LoadUser(std::uint32_t dwUID);
    void SaveAllUsers();
    bool BackupDatabase(const char* szPath);
    bool RestoreDatabase(const char* szPath);

    // 网络
    bool OnAccect(XClient* pClient) override;
    bool IsServerAcceptClosed();
    void SetServerAcceptClosed(bool bClose);

    // 日志
    void WriteLog(char* szFormat, ...);
    bool SendDBLog(ST_LOG_GAME& stLog);
    bool SendDBChatLog(ST_CHAT_LOG_GAME& stLog);
    bool SendDBStatLog(ST_STAT_LOG_GAME& stLog);
    bool SendDBTextLog(ST_LOG_TEXT& stLog);
    bool SendDBSystemLog(ST_LOG_SYSTEM& stLog);
    bool SendDBGame(XSendDBPacket& xSendPacket);
    bool SendDBLogPacket(XSendDBPacket& xSendPacket);
    bool SendDBAccount(XSendDBPacket& xSendPacket);

    // 数据库日志扩展
    void SendDBItemLog(ST_LOG_GAME& stLog, PS_RES_STORAGE_INFO& vecCreateItem, PS_RES_STORAGE_INFO& vecUpdateItem);
    void SendDBTradeLog(std::uint32_t dwReqUAID, std::uint32_t dwReqUCID, std::uint32_t dwResUAID, std::uint32_t dwResUCID);
    void SendDBTradeLog(std::uint32_t dwUAID_1, std::uint32_t dwUCID_1, std::uint32_t dwUAID_2, std::uint32_t dwUCID_2,
                        ST_TRADE_ITEM_LIST& vecItem_1, ST_TRADE_ITEM_LIST& vecItem_2);
    void SendDBItemRepairLog(std::uint32_t dwUAID, std::uint32_t dwUCID, std::int16_t MainType, std::int16_t SubType,
                              PS_RES_STORAGE_INFO& vecTagetItem, int nParam2, int nParam3, int nParam6, wchar_t* Comment);
    void SendDBAchieveLog(std::uint32_t dwUAID, std::uint32_t dwUCID, std::int16_t shSub, std::int8_t byLevel,
                           ST_ACHIEVE_UPDATE_LIST& stUpdateList, wchar_t* Comment);

    // 其他
    int nRand(int nMin, int nMax);
    float fRand(float fMin, float fMax);
    bool Shutdown(std::uint32_t dwTick) override;

    // 资源管理器访问
    XResourceMgr& GetResourceMgr() { return m_xResourceMgr; }
    const XResourceMgr& GetResourceMgr() const { return m_xResourceMgr; }

    // 世界资源管理器访问
    XWorldResMgr& GetWorldResMgr() { return m_xWorldResMgr; }
    const XWorldResMgr& GetWorldResMgr() const { return m_xWorldResMgr; }

    // 日期相关
    std::int64_t GetCurDate();
    void GetCurDate(ST_WORLD_CUR_DATE& stDate);
    std::int64_t GetUpdateDate(std::uint8_t byType);
    std::int64_t GetBeforeInitDate();
    std::uint64_t GetInitTick();
    void SetMoneySupply(std::int64_t biMoney);
    std::uint8_t GetSystemPostTableIndex(std::uint8_t bySubType, std::uint16_t wType);
    void SetAllUserInfoSync(int nServerType, bool bAdd);
    void SyncUsersInfo(int nType, int nCount);

    // 商店相关
    TB_SHOP* GetShopItem(std::uint32_t dwGroupID, std::uint32_t dwIndex);
    bool IsCashShopBuy(int nIndex);

    // 性能监控
    void SetPerformanceState(bool bState);
    void AddPerformanceCount();

    // 物品锁定日志
    void SendItemLockLog(std::uint32_t dwUCID, std::uint8_t byInvenType, std::int16_t shPos,
                          std::uint8_t byLock, int nCheckPos, int nEtcValue);

    // 管理器访问
    CDailyMissionMgr* GetDailyMissionMgr();
    CTimeEventMgr& GetTimeEventMgr() { return m_TimeEventMgr; }
    CDayEventMgr& GetDayEventMgr() { return m_DayEventMgr; }
    CWorldEventMgr& GetWorldEventMgr() { return m_WorldEventMgr; }

private:
    // === 成员变量 - 对齐 IDA 反编译 ===

    // 基类 TXServer<CUser> 已包含 m_xOption, m_xObjectMgr 等

    // 随机数种子
    XSeed m_xSeed;

    // DB Agent 管理器 - 对齐 IDA: XGameDBSocketMgr m_xDBAgentMgr
    XGameDBSocketMgr m_xDBAgentMgr;

    // 资源管理器
    XResourceMgr m_xResourceMgr;

    // 世界资源管理器
    XWorldResMgr m_xWorldResMgr;

    // 物品工厂
    XItemFactory m_xItemFactory;

    // 动作资源管理器
    XActionResMgr m_xActionManager;

    // Akashic 资源管理器
    XAkashicResMgr m_xAkashicManager;

    // 每日任务管理器
    CDailyMissionMgr m_DailyMissionMgr;

    // 时间事件管理器
    CTimeEventMgr m_TimeEventMgr;

    // 每日事件管理器
    CDayEventMgr m_DayEventMgr;

    // 世界事件管理器
    CWorldEventMgr m_WorldEventMgr;

    // 排行榜管理器
    CRankingMgr m_RankingMgr;

    // CURL 包装器
    CGameCurlWrapper m_curlWrapper;

    // 社区 Socket (RelayServer 连接)
    CCommunitySocket m_communitySocket;

    // 控制 Socket (ControlServer 连接)
    CGameControlSocket m_controlSocket;

    // 观察 Socket
    CObserveSocket m_scObserveSocket;

    // Xigncode 反作弊
    CXigncode m_xignCode;

    // 系统邮件表
    std::map<std::uint16_t, std::map<std::uint16_t, std::uint8_t>> m_mapSystemPostTalbe;

    // 商店信息
    std::map<std::uint32_t, std::map<std::uint32_t, TB_SHOP>*> m_mapShopInfo;

    // 商城列表
    std::map<std::uint32_t, STCashItem> m_mapCashshopList;

    // 日志映射
    std::map<std::int32_t, PS_UPDATE_MAZE_ENTER_LIMIT_COUNT> m_mapLog;

    // 读写锁
    CFSRWLock m_rwLock;
    CFSRWLock m_rwMapLock;
    CFSRWLock m_rwCinderellaLock;
    CFSRWLock m_rwCashshopLock;

    // === 在线用户表 (对齐 IDA: boost::multi_index_container) ===
    // m_UserInfos 是受 m_rwLock 保护的在线表
    // 同时具备 actor 索引与 UAID 索引
    // 当前简化实现：使用两个 unordered_map 模拟多索引
    std::unordered_map<UXActorID, CUser*> m_mapActorToUser;      // ActorID -> User
    std::unordered_map<std::uint32_t, CUser*> m_mapUIDToUser;    // UAID -> User
    std::unordered_map<std::wstring, CUser*> m_mapNameToUser;    // Name -> User

    // === 状态标志和时间戳 ===
    bool m_bClose = false;
    bool m_bNeedHavokInit = true;
    bool m_bResetUserConnectInfo = false;
    bool m_bAcceptClose = false;
    bool m_bSGKeepAlive = false;
    bool m_bPerformanceState = false;  // 性能监控状态
    std::int64_t m_biInitDateBefore = 0;
    std::int64_t m_biInitDateAfter = 0;
    std::int64_t m_biMoneySupply = 0;
    std::uint64_t m_dw64WaitTick = 0;
    std::uint64_t m_dwUpdateServerInfoTick = 0;
    std::uint64_t m_dwControlConnectTick = 0;
    std::uint64_t m_dwCommunityConnectTick = 0;
    std::uint64_t m_dw64MoneyTick = 0;
    std::uint64_t m_dw64CashshopTick = 0;
    std::uint64_t m_dw64PerformanceTick = 0;  // 性能监控时间戳
    std::uint32_t m_dwWriteTime = 0;
    std::uint32_t m_dwCheckPerformance = 0;   // 性能检查计数
    std::int32_t m_nReserveUser = 0;
    std::int32_t m_nRoomIndex = 0;

    // Vision 引擎事件句柄
#ifdef _WIN32
    void* m_hVisionEvent = nullptr;
#else
    int m_hVisionEvent = -1;
#endif

    // === 辅助方法 ===
    void LoadDailyMissionTable();
    void LoadSystemPostTable();
    void InitShop();
    void ClearShop();
    void InitDate();
    void UpdateInitDate();
    void OverlappedCashshop();
    void LoadCashShop();
    void SendCashShopItemUpdate();
    void SendCashShopTabUpdate();
    void SendCashShop(CUser* pUser);  // IDA 0x1402DE190
    void NameChange(CUser* pUser, const wchar_t* pChangeName);  // IDA 0x1402DE750
    void SendMoneySupply();
    void SendNoticeErrorControl_Community();
    void SendToObserve_LogicThreadState();
    void SGStoveLogin(CUser* pUser);  // IDA 0x1402DF600
    void AddSystemPostTableIndex(std::uint16_t wSubType, std::uint16_t wType, std::uint8_t byIndex);

    // 静态控制台处理函数
#ifdef _WIN32
    static BOOL WINAPI ConsolCtrlHandler(DWORD dwOPCode);
#endif
};
