#pragma once

#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
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
namespace XMaze {
    extern VGameHelper* m_spGameHelper;
}

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

    // 用户管理
    void EnterUser(CUser* pUser);
    void ExitUser(CUser* pUser);
    CUser* FindNameToUser(wchar_t* pName);
    CUser* FindActorIDToUser(UXActorID uxActorID);
    CUser* FindUIDToUser(std::uint32_t dwUID);
    CUser* FindUser(std::uint32_t dwUID);  // Generic find user
    int GetOnlineCount();
    void BroadcastAll(void* pPacket, int nSize);

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

    // 日志
    void WriteLog(char* szFormat, ...);
    bool SendDBLog(ST_LOG_GAME& stLog);
    bool SendDBChatLog(ST_CHAT_LOG_GAME& stLog);
    bool SendDBStatLog(ST_STAT_LOG_GAME& stLog);
    bool SendDBTextLog(ST_LOG_TEXT& stLog);
    bool SendDBSystemLog(ST_LOG_SYSTEM& stLog);

    // 其他
    int nRand(int nMin, int nMax);
    float fRand(float fMin, float fMax);
    bool Shutdown(std::uint32_t dwTick) override;

    // 资源管理器访问
    XResourceMgr& GetResourceMgr() { return m_xResourceMgr; }
    const XResourceMgr& GetResourceMgr() const { return m_xResourceMgr; }

    // DB 发送接口
    bool SendDBGame(XGameServer* pServer, XSendDBPacket* pPacket);

private:
    // === 成员变量 - 对齐 IDA 反编译 ===

    // 基类 TXServer<CUser> 已包含 m_xOption, m_xObjectMgr 等

    // 随机数种子
    XSeed m_xSeed;

    // DB Agent 管理器 - 对齐 IDA: XGameDBSocketMgr m_xDBAgentMgr
    // 当前用指针简化，后续应对齐为直接成员
    XGameDBSocketMgr* m_xDBAgentMgr;

    // 资源管理器
    XResourceMgr m_xResourceMgr;

    // 物品工厂
    XItemFactory m_xItemFactory;

    // 动作资源管理器
    XActionResMgr m_xActionManager;

    // Akashic 资源管理器
    XAkashicResMgr m_xAkashicManager;

    // 世界资源管理器
    XWorldResMgr m_xWorldResMgr;

    // 社区 Socket (RelayServer 连接)
    CCommunitySocket m_communitySocket;

    // 控制 Socket (ControlServer 连接)
    CGameControlSocket m_controlSocket;

    // 观察 Socket
    CObserveSocket m_scObserveSocket;

    // Xigncode 反作弊
    CXigncode m_xignCode;

    // CURL 包装器
    CGameCurlWrapper m_curlWrapper;

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

    // 系统邮件表
    std::map<std::uint16_t, std::map<std::uint16_t, std::uint8_t>> m_mapSystemPostTalbe;

    // 读写锁
    CFSRWLock m_rwLock;

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
    std::uint64_t m_dwUpdateServerInfoTick = 0;
    std::uint64_t m_dwControlConnectTick = 0;
    std::uint64_t m_dwCommunityConnectTick = 0;
    std::uint64_t m_dw64MoneyTick = 0;
    std::uint64_t m_dw64CashshopTick = 0;

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
    void SendMoneySupply();
    void SendNoticeErrorControl_Community();
    void SendToObserve_LogicThreadState();
};
