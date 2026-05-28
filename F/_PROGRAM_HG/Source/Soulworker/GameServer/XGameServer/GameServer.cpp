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
// IDA 0x1402D86D0
// ============================================================
XGameServer::XGameServer()
    : TXServer<CUser>()
    , m_xSeed(0)
    , m_xDBAgentMgr()
    , m_xResourceMgr()
    , m_xWorldResMgr()
    , m_xItemFactory()
    , m_communitySocket()
    , m_controlSocket()
    , m_scObserveSocket()
    , m_xActionManager()
    , m_xAkashicManager()
    , m_DailyMissionMgr()
    , m_TimeEventMgr()
    , m_DayEventMgr()
    , m_WorldEventMgr()
    , m_RankingMgr()
    , m_curlWrapper()
{
    // 初始化时间戳
    m_dwControlConnectTick = 0;
    m_dwUpdateServerInfoTick = 0;
    m_dwCommunityConnectTick = 0;
    m_bClose = false;
    m_nReserveUser = 0;
    m_nRoomIndex = 0;
    // m_rwLock and m_rwCinderellaLock are default constructed

    // 初始化货币供应
    m_biMoneySupply = 0;
    m_dw64MoneyTick = 0;
    m_dw64WaitTick = 0;

    // 初始化商城列表
    m_dw64CashshopTick = 0;

    // 初始化状态标志
    m_bResetUserConnectInfo = false;
    m_bNeedHavokInit = true;
    m_dwWriteTime = 0;
    m_hVisionEvent = nullptr;
    m_bAcceptClose = false;
    m_bSGKeepAlive = false;
}

// ============================================================
// XGameServer 析构函数
// IDA 0x1402D8B50
// ============================================================
XGameServer::~XGameServer() {
    // 成员析构由编译器自动处理
}

XGameServer* XGameServer::Instance() {
    return TXSingleton<XGameServer>::Instance();
}

// ============================================================
// XGameServer::SetName
// IDA 0x1402D8DB0
// 对齐反编译结果实现
// ============================================================
void XGameServer::SetName() {
    sprintf_s(m_szName, sizeof(m_szName), "MAZE");
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
#ifdef _WIN32
    return ::SetConsoleCtrlHandler(ConsolCtrlHandler, add);
#else
    return XServer::SetConsoleHandler(add);
#endif
}

// ============================================================
// XGameServer::ConsolCtrlHandler
// IDA 0x1402D8D50
// 对齐反编译结果实现
// 控制台控制处理函数
// ============================================================
#ifdef _WIN32
BOOL WINAPI XGameServer::ConsolCtrlHandler(DWORD dwOPCode) {
    // 处理 CTRL_C, CTRL_BREAK, CTRL_CLOSE, CTRL_LOGOFF, CTRL_SHUTDOWN
    if (dwOPCode > 2 && (dwOPCode > 4 || dwOPCode > 6)) {
        return FALSE;
    }

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pServer) {
        pServer->Shutdown(0xFFFFFFFF);
    }
    return TRUE;
}
#endif

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
// 发送游戏日志到 DB Agent (main=0x42, sub=1)
// ============================================================
bool XGameServer::SendDBLog(ST_LOG_GAME& stLogGame) {
    XSendDBPacket xSendPacket(0, 0x42, 1);
    xSendPacket << stLogGame;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    bool bResult = pServer->SendDBLogPacket(xSendPacket);
    return bResult;
}

// ============================================================
// XGameServer::SendDBChatLog
// IDA 0x1402DACC0
// 对齐反编译结果实现
// 发送聊天日志到 DB Agent (main=0x42, sub=9)
// ============================================================
bool XGameServer::SendDBChatLog(ST_CHAT_LOG_GAME& stChatLog) {
    ST_CHAT_LOG_GAME stLog;
    stLog.nUAID = stChatLog.nUAID;
    stLog.nUCID = stChatLog.nUCID;
    stLog.sType = stChatLog.sType;
    stLog.nParam0 = stChatLog.nParam0;
    stLog.nParam1 = stChatLog.nParam1;
    stLog.nParam2 = stChatLog.nParam2;
    stLog.nParam3 = stChatLog.nParam3;
    stLog.nParam4 = stChatLog.nParam4;
    stLog.nParam5 = stChatLog.nParam5;
    stLog.nParam6 = stChatLog.nParam6;

    // 复制注释字符串
    for (int i = 0; i < 257 && stChatLog.szComment[i]; ++i) {
        stLog.szComment[i] = stChatLog.szComment[i];
    }

    XSendDBPacket xSendPacket(0, 0x42, 9);
    xSendPacket << stLog;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    bool bResult = pServer->SendDBLogPacket(xSendPacket);
    return bResult;
}

// ============================================================
// XGameServer::SendDBStatLog
// IDA 0x1402DAEB0
// 对齐反编译结果实现
// 发送统计日志到 DB Agent (main=0x42, sub=0x10)
// ============================================================
bool XGameServer::SendDBStatLog(ST_STAT_LOG_GAME& stStatLog) {
    XSendDBPacket xSendPacket(0, 0x42, 0x10);
    xSendPacket << stStatLog;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    bool bResult = pServer->SendDBLogPacket(xSendPacket);
    return bResult;
}

// ============================================================
// XGameServer::SendDBTextLog
// IDA 0x1402DAF60
// 对齐反编译结果实现
// 发送文本日志到 DB Agent (main=0x42, sub=0x21)
// ============================================================
bool XGameServer::SendDBTextLog(ST_LOG_TEXT& stLogGame) {
    XSendDBPacket xSendPacket(0, 0x42, 0x21);
    xSendPacket << stLogGame;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    bool bResult = pServer->SendDBLogPacket(xSendPacket);
    return bResult;
}

// ============================================================
// XGameServer::SendDBSystemLog
// IDA 0x1402DB010
// 对齐反编译结果实现
// 发送系统日志到 DB Agent (main=0x42, sub=0x25)
// ============================================================
bool XGameServer::SendDBSystemLog(ST_LOG_SYSTEM& stLogSystem) {
    XSendDBPacket xSendPacket(0, 0x42, 0x25);
    xSendPacket << stLogSystem;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    bool bResult = pServer->SendDBLogPacket(xSendPacket);
    return bResult;
}

// ============================================================
// XGameServer::nRand
// IDA 0x1402DAB00
// 对齐反编译结果实现: return XRand<int>(&this->m_xSeed, nMin, nMax)
// ============================================================
int XGameServer::nRand(int nMin, int nMax) {
    return XRand<int>(&m_xSeed, nMin, nMax);
}

// ============================================================
// XGameServer::fRand
// IDA 0x1402DAB40
// 对齐反编译结果实现: return XRand<float>(&this->m_xSeed, fMin, fMax)
// ============================================================
float XGameServer::fRand(float fMin, float fMax) {
    return XRand<float>(&m_xSeed, fMin, fMax);
}

// ============================================================
// XGameServer::Shutdown
// IDA 0x1402DB0C0
// 对齐反编译结果实现
// 服务器关闭处理
// ============================================================
bool XGameServer::Shutdown(std::uint32_t dwMaxWait) {
    // 结束逻辑线程管理器
    CLogicThreadManager* pLogicMgr = TXSingleton<CLogicThreadManager>::Instance();
    CLogicThreadManager::End(pLogicMgr);

    // 结束日志线程管理器
    CGameLogThreadManager* pLogMgr = TXSingleton<CGameLogThreadManager>::Instance();
    CGameLogThreadManager::End(pLogMgr);

    // 清理资源
    Clear();

    return true;
}

// ============================================================
// XGameServer::SendDBGame
// IDA 0x1402DB110
// 对齐反编译结果实现
// 发送数据包到 GameDB
// ============================================================
bool XGameServer::SendDBGame(XSendDBPacket& xSendPacket) {
    int nOrderID = xSendPacket.GetOrderID();
    int iIndex = nOrderID % m_xDBAgentMgr.GetGameDBAgentCount();

    if (m_xDBAgentMgr.SendGameDBAgent(iIndex, xSendPacket)) {
        return true;
    }

    LogHelper::LogError("game.system", "<Send GameDB> Error Stat!");
    return false;
}

// ============================================================
// XGameServer::SendDBLogPacket
// IDA 0x1402DB1A0
// 对齐反编译结果实现
// 发送日志数据包到 LogDB
// ============================================================
bool XGameServer::SendDBLogPacket(XSendDBPacket& xSendPacket) {
    XOption& pOption = this->GetOption();

    if (pOption.GetSystemType() == SYSTEM_TYPE_DEV) {
        return SendDBGame(xSendPacket);
    }

    int nOrderID = xSendPacket.GetOrderID();
    int iIndex = nOrderID % m_xDBAgentMgr.GetLogDBAgentCount();

    if (m_xDBAgentMgr.SendLogDBAgent(iIndex, xSendPacket)) {
        return true;
    }

    LogHelper::LogError("game.system", "<Send LogDB> Error Stat!");
    return false;
}

// ============================================================
// XGameServer::SendDBAccount
// IDA 0x1402DB250
// 对齐反编译结果实现
// 发送数据包到 AccountDB
// ============================================================
bool XGameServer::SendDBAccount(XSendDBPacket& xSendPacket) {
    XOption& pOption = this->GetOption();

    if (pOption.GetSystemType() == SYSTEM_TYPE_DEV) {
        return SendDBGame(xSendPacket);
    }

    int nOrderID = xSendPacket.GetOrderID();
    int iIndex = nOrderID % m_xDBAgentMgr.GetAccountDBAgentCount();

    if (m_xDBAgentMgr.SendAccountDBAgent(iIndex, xSendPacket)) {
        return true;
    }

    LogHelper::LogError("game.system", "<Send AccountDB> Error Stat!");
    return false;
}

// ============================================================
// 辅助方法存根
// ============================================================

void XGameServer::LoadDailyMissionTable() {
    // IDA 0x1402DCEF0
    // TODO: 需要实现 XResourceMgr::m_mapTB_DAILY_MISSION 访问
    // 遍历 m_xResourceMgr.m_mapTB_DAILY_MISSION 并插入到 m_DailyMissionMgr
}

void XGameServer::LoadSystemPostTable() {
    // IDA 0x1402DCF90
    // TODO: 需要实现 XResourceMgr::m_mapTB_SYSTEMMAIL 访问
    // 遍历 m_xResourceMgr.m_mapTB_SYSTEMMAIL 并添加到索引
}

void XGameServer::InitShop() {
    // IDA 0x1402DCA50
    // 初始化商店信息
    // 遍历 m_xResourceMgr.m_mapTB_SHOP 并插入到 m_mapShopInfo
}

void XGameServer::ClearShop() {
    // IDA 0x1402DCCD0
    // 清理商店信息
    for (auto it = m_mapShopInfo.begin(); it != m_mapShopInfo.end(); ++it) {
        if (it->second) {
            it->second->clear();
            delete it->second;
        }
    }
    m_mapShopInfo.clear();
}

void XGameServer::InitDate() {
    // IDA 0x1402DD4B0
    // 初始化日期
    ATL::CTime tTime = ATL::CTime::GetCurrentTime();
    int nDay = tTime.GetDay();
    int nMonth = tTime.GetMonth();
    int nYear = tTime.GetYear();

    ATL::CTime tUpdate(nYear, nMonth, nDay, 9, 0, 0, -1);

    if (tTime.GetHour() >= 9) {
        m_biInitDateBefore = tUpdate.GetTime();
        m_biInitDateAfter = (tUpdate + ATL::CTimeSpan(1, 0, 0, 0)).GetTime();
    } else {
        m_biInitDateBefore = (tUpdate - ATL::CTimeSpan(1, 0, 0, 0)).GetTime();
        m_biInitDateAfter = tUpdate.GetTime();
    }

    ULONGLONG dwTick = GetTickCount64();
    m_dw64WaitTick = 1000 * (m_biInitDateAfter - tTime.GetTime()) + dwTick;
}

void XGameServer::UpdateInitDate() {
    // IDA 0x1402DD5F0
    if (m_dw64WaitTick <= GetTickCount64() && GetCurDate() > m_biInitDateAfter) {
        m_biInitDateBefore = m_biInitDateAfter;
        m_biInitDateAfter += 86400;
        m_dw64WaitTick = GetTickCount64() + 86400000;
    }
}

void XGameServer::OverlappedCashshop() {
    // IDA 0x1402DEB40
    // 重叠商城数据
}

void XGameServer::LoadCashShop() {
    // IDA 0x1402DD690
    // 加载商城数据
}

void XGameServer::SendCashShopItemUpdate() {
    // TODO: 实现
}

void XGameServer::SendCashShopTabUpdate() {
    // TODO: 实现
}

void XGameServer::SendMoneySupply() {
    // IDA 0x1402DE560
    std::int64_t biMoney = m_biMoneySupply;
    m_biMoneySupply = 0;

    XSendPacket xSendPacket(0xF3, 0x13);
    xSendPacket << biMoney;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pServer) {
        pServer->m_controlSocket.SendCheck(&xSendPacket);
    }
}

void XGameServer::SendNoticeErrorControl_Community() {
    // TODO: 实现
}

void XGameServer::SendToObserve_LogicThreadState() {
    // TODO: 实现
}

// ============================================================
// XGameServer::GetShopItem
// IDA 0x1402DCE20
// 对齐反编译结果实现
// ============================================================
TB_SHOP* XGameServer::GetShopItem(std::uint32_t nGroup, std::uint32_t dwItemID) {
    auto it = m_mapShopInfo.find(nGroup);
    if (it == m_mapShopInfo.end()) {
        return nullptr;
    }

    auto* pItemMap = it->second;
    if (!pItemMap) {
        return nullptr;
    }

    auto itemIt = pItemMap->find(dwItemID);
    if (itemIt == pItemMap->end()) {
        return nullptr;
    }

    return &itemIt->second;
}

// ============================================================
// XGameServer::GetCurDate
// IDA 0x1402DD2E0
// 对齐反编译结果实现
// ============================================================
std::int64_t XGameServer::GetCurDate() {
    ATL::CTime tTime = ATL::CTime::GetCurrentTime();
    return tTime.GetTime();
}

// ============================================================
// XGameServer::GetBeforeInitDate
// IDA 0x1400386F0
// 对齐反编译结果实现
// ============================================================
std::int64_t XGameServer::GetBeforeInitDate() {
    return m_biInitDateBefore;
}

// ============================================================
// XGameServer::GetInitTick
// IDA 0x140048E10
// 返回类型: unsigned __int64 (对齐 IDA)
// ============================================================
std::uint64_t XGameServer::GetInitTick() {
    return m_dw64WaitTick;
}

// ============================================================
// XGameServer::SetMoneySupply
// IDA 0x1400FA020
// 对齐反编译结果实现
// ============================================================
void XGameServer::SetMoneySupply(std::int64_t biMoney) {
    m_biMoneySupply += biMoney;
}

// ============================================================
// XGameServer::GetDailyMissionMgr
// IDA 0x14005AC30
// 对齐反编译结果实现
// ============================================================
CDailyMissionMgr* XGameServer::GetDailyMissionMgr() {
    return &m_DailyMissionMgr;
}

// ============================================================
// XGameServer::IsServerAcceptClosed
// IDA 0x1402F6D30
// 对齐反编译结果实现
// ============================================================
bool XGameServer::IsServerAcceptClosed() {
    return m_bAcceptClose;
}

// ============================================================
// XGameServer::KickoutUserAll
// IDA 0x1402DE620
// 对齐反编译结果实现
// ============================================================
void XGameServer::KickoutUserAll(std::uint8_t byType) {
    CFAutoSlimReadLock autolock(&m_rwLock);

    for (auto it = m_mapActorToUser.begin(); it != m_mapActorToUser.end(); ++it) {
        CUser* pUser = it->second;
        if (pUser) {
            PS_KICK_USER_INFO psKick;
            psKick.dwUAID = pUser->GetUAID();
            psKick.byKickType = byType;
            pUser->Kickout(&psKick, false);
        }
    }
}

// ============================================================
// XGameServer::AddSystemPostTableIndex
// IDA 0x1402DD050
// 对齐反编译结果实现
// ============================================================
void XGameServer::AddSystemPostTableIndex(std::uint16_t wSubType, std::uint16_t wType, std::uint8_t byIndex) {
    auto it = m_mapSystemPostTalbe.find(wSubType);
    if (it != m_mapSystemPostTalbe.end()) {
        it->second[wType] = byIndex;
    } else {
        std::map<std::uint16_t, std::uint8_t> mapNew;
        mapNew[wType] = byIndex;
        m_mapSystemPostTalbe[wSubType] = mapNew;
    }
}

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
