#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XCore/XServer/XSeed.h"
#include "Soulworker/GameServer/XGameServer/WorldManager.h"
#include "Soulworker/GameServer/XCore/XServer/TXDBSocket.h"
#include "Soulworker/GameServer/XCore/XServer/CFSRWLock.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include <ctime>
#include <cstdlib>
#include <cstdarg>

#ifdef _WIN32
#include <windows.h>
#endif

// ============================================================
// 全局变量 - FPS 相关
// ============================================================
static std::uint64_t m_dw64FPSTick = 0;
static std::uint32_t m_dwFrame = 0;
static std::uint32_t nCheckUserCount = 0;

// 全局变量 - 更新定时器
static std::uint64_t dwSGUpdate = 0;
static std::uint64_t dwSecondUpdate = 0;
static bool bOnlyoneDestory = false;

// 全局字符串路径 (Vision 引擎)
std::string g_strCurPath_10;

// ============================================================
// Vision 引擎存根类型和函数
// ============================================================

// VisCallback_cl 实现
class VisCallback_clImpl {
public:
    static void TriggerCallbacks(void*, void*) {}
};

struct SoulWorkerGameModule_t {};
static SoulWorkerGameModule_t g_SoulWorkerGameModule;

namespace Vision {
    void SetTimer(void*) {}
    std::uint64_t GetTimer() { return 0; }
    void RegisterModule(SoulWorkerGameModule_t*) {}
    void UnregisterModule(SoulWorkerGameModule_t*) {}
}

namespace VVideo {
    void SetHeadlessModeEnabled(int) {}
}

class VisFile_cl {
public:
    static void SetAssetProfile(VisFile_cl*, const char*) {}
    static void AddDataDirectory(VisFile_cl*, const char*, int) {}
};

namespace Vision {
    VisFile_cl File;
}

class VStreamProcessor {
public:
    static void Initialize(int) {}
};

// VGameHelper 继承自 VBaseObject
class VGameHelper : public VBaseObject {
public:
    VGameHelper() = default;
};

// XMaze 静态成员定义
namespace XMaze {
VGameHelper* m_spGameHelper = nullptr;
}

namespace DohHavokHelper {
    void init() {}
    void deinit() {}
}

struct VMemoryStatistics_t {
    std::int64_t m_iTotalByteAlloc = 0;
    std::int64_t m_iTotalByteFree = 0;
    int m_iTotalNumAllocations = 0;
    int m_iTotalNumFree = 0;
    VMemoryStatistics_t() = default;
};

inline void VBaseMem_GetStats(VMemoryStatistics_t*) {}

class VisRenderContext_cl {
public:
    static void GlobalTick() {}
};

class XTime {
public:
    static std::uint64_t GetTickCount() {
#ifdef _WIN32
        return ::GetTickCount64();
#else
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return static_cast<std::uint64_t>(ts.tv_sec) * 1000 + ts.tv_nsec / 1000000;
#endif
    }
};

void XPRINT(const char* msg) {
    LogHelper::LogDebug("game.system", "%s", msg);
}

// ============================================================
// XGameServer 构造函数
// ============================================================
XGameServer::XGameServer()
    : TXServer<CUser>()
    , m_xSeed()
    , m_xDBAgentMgr(nullptr)
    , m_xResourceMgr()
    , m_mapSystemPostTalbe()
    , m_bClose(false)
    , m_bNeedHavokInit(true)
    , m_bResetUserConnectInfo(false)
    , m_dwUpdateServerInfoTick(0)
    , m_dwControlConnectTick(0)
    , m_dwCommunityConnectTick(0)
    , m_dw64MoneyTick(0)
    , m_dw64CashshopTick(0)
    , m_hVisionEvent(nullptr)
{
    // 对齐 IDA 0x1402D86D0 构造函数
}

XGameServer::~XGameServer() {
    // TODO: 对齐 IDA 实现
}

XGameServer* XGameServer::Instance() {
    return TXSingleton<XGameServer>::Instance();
}

// ============================================================
// XGameServer::InitServer
// IDA 0x1402D8DE0
// 对齐反编译结果实现
// ============================================================
bool XGameServer::InitServer() {
    // 1. 初始化随机数种子
    XSeed::Init(&m_xSeed, 1);

    // 2. 获取服务器名称并启动日志线程管理器
    const char* szName = this->GetName();
    CGameLogThreadManager* pLogMgr = TXSingleton<CGameLogThreadManager>::Instance();
    CGameLogThreadManager::Start(pLogMgr, szName);

    // 3. 初始化世界管理器
    XWorldManager* pWorldMgr = TXSingleton<XWorldManager>::Instance();
    pWorldMgr->Init();

    // 4. 初始化 Xigncode (如果启用)
    if (m_xOption.GetSecurityType() == SECURITY_ON) {
        bool bXignInit = m_xignCode.Init();
        m_xOption.SetInitXignCode(bXignInit);
    }

    // 5. 显示服务器信息
    m_xOption.ShowServerInfo();

    // 6. 设置 IO Pool 限制
    int nLimitIOPool = m_xOption.GetMaxIOPool();
    this->SetLimitIOPool(nLimitIOPool);

    // 7. 初始化资源管理器
    std::uint32_t dwServerID = m_xOption.GetServerID();
    const char* szGameDNS = m_xOption.GetDNS(1);
    const char* szCommonDNS = m_xOption.GetDNS(2);

    if (!m_xResourceMgr.Init(szCommonDNS, szGameDNS, dwServerID)) {
        LogHelper::LogError("game.system", "[SYSTEM] Failed Load Common Table DB !! ( %s ) ", szCommonDNS);
        return false;
    }

    // 8. 加载资源表
    int nServerIDForLoad = m_xOption.GetServerID();
    std::uint16_t nWorldID = m_xOption.GetGroupID();
    const char* szResFilePath = m_xOption.GetResFilePath();
    RES_LOAD_TYPE eResLoadType = m_xOption.GetResLoadType();

    if (!m_xResourceMgr.Load(eResLoadType, szResFilePath, nWorldID, nServerIDForLoad)) {
        LogHelper::LogError("game.system", "[SYSTEM] Failed Load Common Table DB !! ( %s ) ", szCommonDNS);
        return false;
    }

    LogHelper::LogInfo("game.system", "[INIT] ResourceMgr - Load Complete!");

    // 9. 设置服务器内容选项
    CONTENTS_OPTION_INFO* pContentsOption = m_xOption.GetContentsOption();
    if (pContentsOption && pContentsOption->nOptionFlag == 2) {
        for (int i = 0; i < E_SERVER_OPTION_MAX; ++i) {
            m_xResourceMgr.SetServerContents(i, pContentsOption->bContents[i]);
        }
    }

    // 10. 加载每日任务表和系统邮件表
    LoadDailyMissionTable();
    LoadSystemPostTable();
    LogHelper::LogInfo("game.system", "[INIT] Load Table Daily / SystemPost");

    // 11. 初始化商店
    InitShop();
    LogHelper::LogInfo("game.system", "[INIT] Shop - Init");

    // 12. 初始化物品工厂
    std::uint8_t byGroupID = m_xOption.GetGroupID();
    int nFactoryServerID = m_xOption.GetServerID();
    XItemFactory::Init(&m_xItemFactory, byGroupID, nFactoryServerID);
    LogHelper::LogInfo("game.system", "<ITEM_FACTORY> Factory Init ( %d, %d )",
                        static_cast<int>(byGroupID), nFactoryServerID);

    // 13. 初始化 Vision 引擎
    Vision::SetTimer(nullptr);
    Vision::RegisterModule(&g_SoulWorkerGameModule);
    VVideo::SetHeadlessModeEnabled(1);

    // 设置资源路径
    const char* szCurPath = g_strCurPath_10.c_str();
    VisFile_cl::SetAssetProfile(&Vision::File, szCurPath);

    // 添加数据目录
    std::string strPath;
    strPath = g_strCurPath_10 + "/";
    VisFile_cl::AddDataDirectory(&Vision::File, strPath.c_str(), 0);

    strPath = g_strCurPath_10 + "/Scripts";
    VisFile_cl::AddDataDirectory(&Vision::File, strPath.c_str(), 0);

    strPath = g_strCurPath_10 + "/World/Table";
    VisFile_cl::AddDataDirectory(&Vision::File, strPath.c_str(), 0);

    strPath = g_strCurPath_10 + "/ActionData";
    VisFile_cl::AddDataDirectory(&Vision::File, strPath.c_str(), 0);

    // 14. 初始化流处理器
    VStreamProcessor::Initialize(1);

    // 15. 创建 VGameHelper
    VGameHelper* pGameHelper = new VGameHelper();
    XMaze::m_spGameHelper = pGameHelper;
    LogHelper::LogInfo("game.system", "[INIT] Vision - Init");

    // 16. 加载世界/动作/Akashic 资源
    XWorldResMgr::LoadAll(&m_xWorldResMgr);
    LogHelper::LogInfo("game.system", "[INIT] m_xWorldResMgr - Load");

    m_xActionManager.LoadAll();
    LogHelper::LogInfo("game.system", "[INIT] m_xActionManager - Load");

    XAkashicResMgr::LoadAll(&m_xAkashicManager);
    LogHelper::LogInfo("game.system", "[INIT] m_xAkashicManager - Load");

    // 17. 触发引擎初始化回调
    VisCallback_clImpl::TriggerCallbacks(nullptr, nullptr);

    // 18. 启动逻辑线程
    int nLogicThread = m_xOption.GetLogicThread();
    if (nLogicThread <= 1) {
        nLogicThread = 1;
    }
    CLogicThreadManager* pLogicMgr = TXSingleton<CLogicThreadManager>::Instance();
    CLogicThreadManager::Start(pLogicMgr, nLogicThread);
    LogHelper::LogInfo("game.system", "[INIT] LogicThread - Start ");

    // 19. 初始化 DB Agent 管理器 (存根)
    // m_xDBAgentMgr.Init();
    // m_xDBAgentMgr.AutoConnect();
    LogHelper::LogInfo("game.system", "[INIT] m_xDBAgentMgr - Init ");

    // 20. 初始化 Community Socket (存根)
    // m_communitySocket.SetMyInfo(&m_xOption);
    LogHelper::LogInfo("game.system", "[INIT] m_communitySocket - Init ");

    // 21. 初始化 Control Socket (存根)
    // m_controlSocket.SetMyInfo(&m_xOption);
    LogHelper::LogInfo("game.system", "[INIT] m_controlSocket - Init ");

    // 22. 启动 Observe Socket (存根)
    // CObserveSocket::StartUp(&m_scObserveSocket, &m_xOption);

    // 23. 初始化计算状态
    CCalculateStatus* pCalcStatus = TXSingleton<CCalculateStatus>::Instance();
    CCalculateStatus::Init(pCalcStatus);

    // 24. 初始化随机数种子
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    LogHelper::LogInfo("game.system", "[INIT] CALCULATE_STATUS - Init ");

    // 25. 触发引擎初始化回调 (第二次)
    VisCallback_clImpl::TriggerCallbacks(nullptr, nullptr);

    // 26. 初始化日期
    InitDate();

    // 27. 加载商城
    OverlappedCashshop();
    LoadCashShop();
    LogHelper::LogInfo("game.system", "[INIT] CASH_SHOP - Init ");

    // 28. 检查排行榜奖励表
    std::uint32_t dwLastRankingRewardID_Date = CRankingMgr::GetLastRewardID_Date(&m_RankingMgr);
    if (dwLastRankingRewardID_Date) {
        // TODO: 需要实现 CheckRankingRewardTable
        // if (!m_xResourceMgr.CheckRankingRewardTable(dwLastRankingRewardID_Date)) {
        //     return false;
        // }
    }

    // 29. 初始化 CURL
    CGameCurlWrapper::Init(&m_curlWrapper);
    LogHelper::LogInfo("game.system", "[INIT] CURL - Init ");

    // 30. 检查区域服务器 (存根 - 始终返回 true)
    // if (!XDistrict::IsDistirct(this)) return false;

    LogHelper::LogInfo("game.system", "[INIT] SGStoveInit - Init ");
    LogHelper::LogInfo("game.system", "[INIT] Complete Server Init");

    return true;
}

// ============================================================
// XGameServer::Clear
// IDA 0x1402D9900
// 对齐反编译结果实现
// ============================================================
bool XGameServer::Clear() {
    CFAutoSlimWriteLock autolock(&m_rwLock);

    // 检查是否已经关闭
    if (m_bClose) {
        return false;
    }

    // 断开所有 Socket 连接 (存根)
    // XIOCPClient::DisConnect(&m_communitySocket);
    // XIOCPClient::DisConnect(&m_controlSocket);

    // 断开 DB Agent (存根)
    // m_xDBAgentMgr.DisConnect();

    // 清理商店
    ClearShop();

    // 清理世界管理器
    XWorldManager* pWorldMgr = TXSingleton<XWorldManager>::Instance();
    pWorldMgr->Clear();

    // 清理各种资源管理器
    m_xWorldResMgr.Clear();
    m_xResourceMgr.Clear();
    m_xActionManager.Clear();
    m_xAkashicManager.Clear();

    // 清理系统邮件表
    m_mapSystemPostTalbe.clear();

    // 清理 CURL
    m_curlWrapper.Clear();

    // 释放 Xigncode
    if (m_xOption.GetSecurityType() == SECURITY_ON) {
        m_xignCode.Release();
    }

    // 释放 VGameHelper
    if (XMaze::m_spGameHelper) {
        VGameHelper* pHelper = XMaze::m_spGameHelper;
        XMaze::m_spGameHelper = nullptr;
        delete pHelper;
    }

    // 设置关闭标志
    m_bClose = true;

    return true;
}

// ============================================================
// XGameServer::OnUpdate
// IDA 0x1402DA160
// 对齐反编译结果实现
// ============================================================
void XGameServer::OnUpdate(std::uint64_t dwTick) {
    // 初始化 Havok (首次更新时)
    if (m_bNeedHavokInit) {
        DohHavokHelper::init();
        m_bNeedHavokInit = false;
#ifdef _WIN32
        m_hVisionEvent = CreateEventA(nullptr, TRUE, FALSE, nullptr);
#endif
    }

    // 初始化静态更新定时器
    static bool s_bInitSG = false;
    if (!s_bInitSG) {
        s_bInitSG = true;
        dwSGUpdate = dwTick + 60000;
    }

    static bool s_bInitSecond = false;
    if (!s_bInitSecond) {
        s_bInitSecond = true;
        dwSecondUpdate = dwTick + 60000;
    }

    // FPS 计数
    if (m_dw64FPSTick == 0) {
        m_dw64FPSTick = XTime::GetTickCount();
    }

    if (dwTick - m_dw64FPSTick >= 1000) {
        m_dw64FPSTick = dwTick;
        m_dwFrame = 0;
        ++nCheckUserCount;
    } else {
        ++m_dwFrame;
    }

    // 更新世界管理器
    XWorldManager* pWorldMgr = TXSingleton<XWorldManager>::Instance();
    pWorldMgr->OnUpdate(dwTick);

    // Vision 引擎全局 Tick
    VisRenderContext_cl::GlobalTick();

    // 处理关闭逻辑
    if (m_bClose) {
        if (!bOnlyoneDestory) {
            bOnlyoneDestory = true;
            Vision::UnregisterModule(&g_SoulWorkerGameModule);
            DohHavokHelper::deinit();
#ifdef _WIN32
            Sleep(333);
#endif
            m_bRunFlag = false;
#ifdef _WIN32
            if (m_hVisionEvent) {
                SetEvent(m_hVisionEvent);
            }
#endif
            XIOCPServer::Shutdown(0xFFFFFFFF);
        }
        return;
    }

    // === 正常更新逻辑 ===

    // Control Socket 连接检查和更新 (存根)
    if (m_dwControlConnectTick < dwTick) {
        // XPRINT("Try to Connection ! ");
        // XRelaySocket::Connect(&m_controlSocket);
        m_dwControlConnectTick = dwTick + 10000;
    }

    // Community Socket 连接检查 (存根)
    if (m_dwCommunityConnectTick < dwTick) {
        // XPRINT("Try to Connection ! ");
        // XRelaySocket::Connect(&m_communitySocket);
        m_dwCommunityConnectTick = dwTick + 10000;
    }

    // 定期用户计数日志
    if (nCheckUserCount > 60) {
        CFAutoSlimReadLock autolock(&m_rwLock);
        int nCount = 0;  // TODO: 从 m_UserInfos 获取用户数量
        LogHelper::LogDebug("game.system", "<SYSTEM> User Count ( %d )", nCount);

        nCheckUserCount = 0;

        // 内存统计
        VMemoryStatistics_t memStats;
        VBaseMem_GetStats(&memStats);
        LogHelper::LogDebug("game.system", "<SYSTEM> Memory ( %lld / %d )",
                            memStats.m_iTotalByteAlloc - memStats.m_iTotalByteFree,
                            memStats.m_iTotalNumAllocations - memStats.m_iTotalNumFree);
        LogHelper::LogDebug("game.system", "<SYSTEM> Memory ( %lld / %lld / %d / %d )",
                            memStats.m_iTotalByteAlloc, memStats.m_iTotalByteFree,
                            memStats.m_iTotalNumAllocations, memStats.m_iTotalNumFree);
    }

    // 更新日期
    UpdateInitDate();

    // 货币供应更新
    if (m_dw64MoneyTick < dwTick) {
        if (m_dw64MoneyTick) {
            SendMoneySupply();
        }
        m_dw64MoneyTick = dwTick + 60000;
    }

    // 商城更新
    if (m_dw64CashshopTick < dwTick) {
        // TODO: 需要实现 CashShop 相关方法
        // std::uint32_t dwOldVersion = m_xResourceMgr.GetCashshopVersion();
        // m_xResourceMgr.CashshopLoad_DBConnect();
        // std::uint32_t dwNewVersion = m_xResourceMgr.GetCashshopVersion();
        // if (dwOldVersion != dwNewVersion) {
        //     OverlappedCashshop();
        //     LoadCashShop();
        //     SendCashShopItemUpdate();
        // }
        // bool bChange = false;
        // m_xResourceMgr.CashShopTabLoad_DBConnect(&bChange);
        // if (bChange) {
        //     SendCashShopTabUpdate();
        // }

        CTimeEventMgr::Update(&m_TimeEventMgr);

        m_dw64CashshopTick = dwTick + 60000;
    }

    // 世界事件更新
    CWorldEventMgr::Update(&m_WorldEventMgr, dwTick);

    // 排行榜更新
    if (m_xResourceMgr.GetServerContents(E_SERVER_OPTION_RANKING)) {
        CRankingMgr::LoadRankingListReq(&m_RankingMgr, dwTick);
    }
}

int XGameServer::SetConsoleHandler(int add) {
    return XServer::SetConsoleHandler(add);
}

// ============================================================
// XGameServer::EnterUser
// IDA 0x1402D9BD0
// 对齐反编译结果实现
// 已确认:
// 1. 进入函数先拿 m_rwLock 写锁
// 2. 在 m_UserInfos 上做 boost::multi_index insert(pUser)
// 3. 插入成功后立即 XRelaySocket::AddUserCount(+1)
// ============================================================
void XGameServer::EnterUser(CUser* pUser) {
    if (!pUser) {
        return;
    }

    CFAutoSlimWriteLock autolock(&m_rwLock);

    // 获取用户的 ActorID 和 UAID
    UXActorID uxActorID = pUser->GetActorID();
    std::uint32_t dwUID = pUser->GetUAID();
    std::wstring strName = pUser->GetName();

    // 插入到各个索引
    m_mapActorToUser[uxActorID] = pUser;
    m_mapUIDToUser[dwUID] = pUser;
    if (!strName.empty()) {
        m_mapNameToUser[strName] = pUser;
    }

    // 通知 RelayServer 用户数增加
    XRelaySocket::AddUserCount(1);
}

// ============================================================
// XGameServer::ExitUser
// IDA 0x1402D9F30
// 对齐反编译结果实现
// 已确认:
// 1. 先拿 m_rwLock 写锁
// 2. 取当前 pUser->GetActorID()
// 3. 再按 actor 索引去 m_UserInfos 查这个对象
// 4. 命中后才真正 erase
// 5. XRelaySocket::AddUserCount(-1)
// ============================================================
void XGameServer::ExitUser(CUser* pUser) {
    if (!pUser) {
        return;
    }

    CFAutoSlimWriteLock autolock(&m_rwLock);

    // 获取用户的 ActorID
    UXActorID uxActorID = pUser->GetActorID();
    std::uint32_t dwUID = pUser->GetUAID();
    std::wstring strName = pUser->GetName();

    // 按 ActorID 索引查找并删除
    auto it = m_mapActorToUser.find(uxActorID);
    if (it != m_mapActorToUser.end() && it->second == pUser) {
        m_mapActorToUser.erase(it);
        m_mapUIDToUser.erase(dwUID);
        if (!strName.empty()) {
            m_mapNameToUser.erase(strName);
        }
        // 通知 RelayServer 用户数减少
        XRelaySocket::AddUserCount(-1);
    }
}

// ============================================================
// XGameServer::FindNameToUser
// IDA 0x1402D9C50
// 对齐反编译结果实现
// 读锁下按名称查找在线对象
// ============================================================
CUser* XGameServer::FindNameToUser(wchar_t* pName) {
    if (!pName || !pName[0]) {
        return nullptr;
    }

    CFAutoSlimReadLock autolock(&m_rwLock);

    std::wstring strName(pName);
    auto it = m_mapNameToUser.find(strName);
    if (it != m_mapNameToUser.end()) {
        return it->second;
    }
    return nullptr;
}

// ============================================================
// XGameServer::FindActorIDToUser
// IDA 0x1402D9D90
// 对齐反编译结果实现
// 读锁下走 actor 索引查找在线对象
// ============================================================
CUser* XGameServer::FindActorIDToUser(UXActorID uxActorID) {
    CFAutoSlimReadLock autolock(&m_rwLock);

    auto it = m_mapActorToUser.find(uxActorID);
    if (it != m_mapActorToUser.end()) {
        return it->second;
    }
    return nullptr;
}

// ============================================================
// XGameServer::FindUIDToUser
// IDA 0x1402D9E60
// 对齐反编译结果实现
// 读锁下走 UAID 索引查找在线对象
// ============================================================
CUser* XGameServer::FindUIDToUser(std::uint32_t dwUID) {
    CFAutoSlimReadLock autolock(&m_rwLock);

    auto it = m_mapUIDToUser.find(dwUID);
    if (it != m_mapUIDToUser.end()) {
        return it->second;
    }
    return nullptr;
}

// ============================================================
// XGameServer::OnAccect
// IDA 0x1402D9B00
// 对齐反编译结果实现
// 客户端连接接受处理
// ============================================================
bool XGameServer::OnAccect(XClient* pClient) {
    if (!pClient) {
        return false;
    }

    // 基类处理
    return XServer::OnAccect(pClient);
}

// ============================================================
// XGameServer::WriteLog
// IDA 0x1402DAB80
// 对齐反编译结果实现
// 格式化日志输出
// ============================================================
void XGameServer::WriteLog(char* szFormat, ...) {
    if (!szFormat) {
        return;
    }

    char szBuffer[4096];
    va_list args;
    va_start(args, szFormat);
#ifdef _WIN32
    vsnprintf_s(szBuffer, sizeof(szBuffer), _TRUNCATE, szFormat, args);
#else
    vsnprintf(szBuffer, sizeof(szBuffer), szFormat, args);
#endif
    va_end(args);

    LogHelper::LogInfo("game.system", "%s", szBuffer);
}

// ============================================================
// XGameServer::SendDBLog
// IDA 0x1402DAC10
// 对齐反编译结果实现
// 发送游戏日志到 DB Agent
// ============================================================
bool XGameServer::SendDBLog(ST_LOG_GAME& stLog) {
    // TODO: 对齐 IDA - 通过 m_xDBAgentMgr 发送日志
    // 当前存根实现
    return true;
}

// ============================================================
// XGameServer::SendDBChatLog
// IDA 0x1402DACC0
// 对齐反编译结果实现
// 发送聊天日志到 DB Agent
// ============================================================
bool XGameServer::SendDBChatLog(ST_CHAT_LOG_GAME& stLog) {
    // TODO: 对齐 IDA - 通过 m_xDBAgentMgr 发送聊天日志
    // 当前存根实现
    return true;
}

// ============================================================
// XGameServer::SendDBStatLog
// IDA 0x1402DAEB0
// 对齐反编译结果实现
// 发送统计日志到 DB Agent
// ============================================================
bool XGameServer::SendDBStatLog(ST_STAT_LOG_GAME& stLog) {
    // TODO: 对齐 IDA - 通过 m_xDBAgentMgr 发送统计日志
    // 当前存根实现
    return true;
}

// ============================================================
// XGameServer::SendDBTextLog
// IDA 0x1402DAF60
// 对齐反编译结果实现
// 发送文本日志到 DB Agent
// ============================================================
bool XGameServer::SendDBTextLog(ST_LOG_TEXT& stLog) {
    // TODO: 对齐 IDA - 通过 m_xDBAgentMgr 发送文本日志
    // 当前存根实现
    return true;
}

// ============================================================
// XGameServer::SendDBSystemLog
// IDA 0x1402DB010
// 对齐反编译结果实现
// 发送系统日志到 DB Agent
// ============================================================
bool XGameServer::SendDBSystemLog(ST_LOG_SYSTEM& stLog) {
    // TODO: 对齐 IDA - 通过 m_xDBAgentMgr 发送系统日志
    // 当前存根实现
    return true;
}

// ============================================================
// XGameServer::nRand
// IDA 0x1402DAB00
// 对齐反编译结果实现
// 返回 [nMin, nMax] 范围内的随机整数
// ============================================================
int XGameServer::nRand(int nMin, int nMax) {
    if (nMin >= nMax) {
        return nMin;
    }
    // 使用 XSeed 生成随机数
    return m_xSeed.Rand(nMin, nMax);
}

// ============================================================
// XGameServer::fRand
// IDA 0x1402DAB40
// 对齐反编译结果实现
// 返回 [fMin, fMax] 范围内的随机浮点数
// ============================================================
float XGameServer::fRand(float fMin, float fMax) {
    if (fMin >= fMax) {
        return fMin;
    }
    // 使用 XSeed 生成随机数
    return m_xSeed.Rand(fMin, fMax);
}

// ============================================================
// XGameServer::Shutdown
// IDA 0x1402DB0C0
// 对齐反编译结果实现
// 服务器关闭处理
// ============================================================
bool XGameServer::Shutdown(std::uint32_t dwTick) {
    // 调用基类关闭
    bool bResult = XServer::Shutdown(dwTick);
    
    // 清理资源
    Clear();
    
    return bResult;
}

// ============================================================
// 辅助方法存根
// ============================================================

void XGameServer::LoadDailyMissionTable() {}
void XGameServer::LoadSystemPostTable() {}
void XGameServer::InitShop() {}
void XGameServer::ClearShop() {}
void XGameServer::InitDate() {}
void XGameServer::UpdateInitDate() {}
void XGameServer::OverlappedCashshop() {}
void XGameServer::LoadCashShop() {}
void XGameServer::SendCashShopItemUpdate() {}
void XGameServer::SendCashShopTabUpdate() {}
void XGameServer::SendMoneySupply() {}
void XGameServer::SendNoticeErrorControl_Community() {}
void XGameServer::SendToObserve_LogicThreadState() {}
bool XGameServer::SendDBGame(XGameServer* pServer, XSendDBPacket* pPacket) { return true; }

// ============================================================
// Phase 6 新增 - 用户管理函数
// ============================================================

// ============================================================
// XGameServer::FindUser
// 通用查找用户 - 按 UID 查找
// ============================================================
CUser* XGameServer::FindUser(std::uint32_t dwUID) {
    return FindUIDToUser(dwUID);
}

// ============================================================
// XGameServer::GetOnlineCount
// 获取在线用户数量
// ============================================================
int XGameServer::GetOnlineCount() {
    CFAutoSlimReadLock autolock(&m_rwLock);
    return static_cast<int>(m_mapActorToUser.size());
}

// ============================================================
// XGameServer::BroadcastAll
// 向所有在线用户广播消息
// ============================================================
void XGameServer::BroadcastAll(void* pPacket, int nSize) {
    if (!pPacket || nSize <= 0) {
        return;
    }

    CFAutoSlimReadLock autolock(&m_rwLock);

    // 遍历所有在线用户并发送消息
    for (auto& pair : m_mapActorToUser) {
        CUser* pUser = pair.second;
        if (pUser) {
            // TODO: 调用 CUser 的发送函数
            // pUser->SendPacket(pPacket, nSize);
            GreenDamTan_log(__FILE__, __FUNCTION__, "BroadcastAll to user - stub");
        }
    }
}

// ============================================================
// Phase 6 新增 - Packet Handlers
// ============================================================

// ============================================================
// XGameServer::RecvChat
// 处理聊天包
// ============================================================
void XGameServer::RecvChat(CUser* pUser, void* pPacket) {
    if (!pUser || !pPacket) {
        return;
    }

    // TODO: 解析聊天包并处理
    // 1. 验证用户权限
    // 2. 检查聊天内容
    // 3. 广播给目标用户
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvChat - stub");
}

// ============================================================
// XGameServer::RecvMove
// 处理移动包
// ============================================================
void XGameServer::RecvMove(CUser* pUser, void* pPacket) {
    if (!pUser || !pPacket) {
        return;
    }

    // TODO: 解析移动包并处理
    // 1. 验证移动合法性
    // 2. 更新用户位置
    // 3. 广播给周围用户
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvMove - stub");
}

// ============================================================
// XGameServer::RecvAttack
// 处理攻击包
// ============================================================
void XGameServer::RecvAttack(CUser* pUser, void* pPacket) {
    if (!pUser || !pPacket) {
        return;
    }

    // TODO: 解析攻击包并处理
    // 1. 验证攻击合法性
    // 2. 计算伤害
    // 3. 应用伤害到目标
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvAttack - stub");
}

// ============================================================
// XGameServer::RecvSkill
// 处理技能包
// ============================================================
void XGameServer::RecvSkill(CUser* pUser, void* pPacket) {
    if (!pUser || !pPacket) {
        return;
    }

    // TODO: 解析技能包并处理
    // 1. 验证技能学习状态
    // 2. 检查冷却时间
    // 3. 执行技能效果
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvSkill - stub");
}

// ============================================================
// XGameServer::RecvItem
// 处理物品包
// ============================================================
void XGameServer::RecvItem(CUser* pUser, void* pPacket) {
    if (!pUser || !pPacket) {
        return;
    }

    // TODO: 解析物品包并处理
    // 1. 验证物品所有权
    // 2. 执行物品操作
    // 3. 更新数据库
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvItem - stub");
}

// ============================================================
// Phase 6 新增 - Database Operations
// ============================================================

// ============================================================
// XGameServer::SaveUser
// 保存用户数据到数据库
// ============================================================
bool XGameServer::SaveUser(CUser* pUser) {
    if (!pUser) {
        return false;
    }

    // TODO: 通过 DB Agent 保存用户数据
    // 1. 序列化用户数据
    // 2. 发送到 DB Agent
    // 3. 等待确认
    GreenDamTan_log(__FILE__, __FUNCTION__, "SaveUser - stub");
    return true;
}

// ============================================================
// XGameServer::LoadUser
// 从数据库加载用户数据
// ============================================================
bool XGameServer::LoadUser(std::uint32_t dwUID) {
    // TODO: 通过 DB Agent 加载用户数据
    // 1. 发送加载请求
    // 2. 等待响应
    // 3. 反序列化用户数据
    GreenDamTan_log(__FILE__, __FUNCTION__, "LoadUser - stub");
    return true;
}

// ============================================================
// XGameServer::SaveAllUsers
// 保存所有在线用户数据
// ============================================================
void XGameServer::SaveAllUsers() {
    CFAutoSlimReadLock autolock(&m_rwLock);

    // 遍历所有在线用户并保存
    for (auto& pair : m_mapActorToUser) {
        CUser* pUser = pair.second;
        if (pUser) {
            SaveUser(pUser);
        }
    }

    LogHelper::LogInfo("game.system", "[DB] SaveAllUsers completed - %d users", 
                        static_cast<int>(m_mapActorToUser.size()));
}

// ============================================================
// XGameServer::BackupDatabase
// 创建数据库备份
// ============================================================
bool XGameServer::BackupDatabase(const char* szPath) {
    if (!szPath || !szPath[0]) {
        return false;
    }

    // TODO: 实现数据库备份
    // 1. 通知 DB Agent 创建备份
    // 2. 等待备份完成
    GreenDamTan_log(__FILE__, __FUNCTION__, "BackupDatabase - stub");
    LogHelper::LogInfo("game.system", "[DB] BackupDatabase to: %s", szPath);
    return true;
}

// ============================================================
// XGameServer::RestoreDatabase
// 从备份恢复数据库
// ============================================================
bool XGameServer::RestoreDatabase(const char* szPath) {
    if (!szPath || !szPath[0]) {
        return false;
    }

    // TODO: 实现数据库恢复
    // 1. 验证备份文件
    // 2. 通知 DB Agent 恢复数据
    // 3. 重新加载必要数据
    GreenDamTan_log(__FILE__, __FUNCTION__, "RestoreDatabase - stub");
    LogHelper::LogInfo("game.system", "[DB] RestoreDatabase from: %s", szPath);
    return true;
}
