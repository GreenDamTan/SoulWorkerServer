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

void XGameServer::EnterUser(CUser* pUser) {
    // TODO: 汇编还原 - IDA 0x1402D9BD0
}

void XGameServer::ExitUser(CUser* pUser) {
    // TODO: 汇编还原 - IDA 0x1402D9F30
}

CUser* XGameServer::FindNameToUser(wchar_t* pName) {
    // TODO: 汇编还原 - IDA 0x1402D9C50
    return nullptr;
}

CUser* XGameServer::FindActorIDToUser(UXActorID uxActorID) {
    // TODO: 汇编还原 - IDA 0x1402D9D90
    return nullptr;
}

CUser* XGameServer::FindUIDToUser(std::uint32_t dwUID) {
    // TODO: 汇编还原 - IDA 0x1402D9E60
    return nullptr;
}

bool XGameServer::OnAccect(XClient* pClient) {
    // TODO: 汇编还原 - IDA 0x1402D9B00
    return true;
}

void XGameServer::WriteLog(char* szFormat, ...) {
    // TODO: 汇编还原 - IDA 0x1402DAB80
}

bool XGameServer::SendDBLog(ST_LOG_GAME& stLog) {
    // TODO: 汇编还原 - IDA 0x1402DAC10
    return true;
}

bool XGameServer::SendDBChatLog(ST_CHAT_LOG_GAME& stLog) {
    // TODO: 汇编还原 - IDA 0x1402DACC0
    return true;
}

bool XGameServer::SendDBStatLog(ST_STAT_LOG_GAME& stLog) {
    // TODO: 汇编还原 - IDA 0x1402DAEB0
    return true;
}

bool XGameServer::SendDBTextLog(ST_LOG_TEXT& stLog) {
    // TODO: 汇编还原 - IDA 0x1402DAF60
    return true;
}

bool XGameServer::SendDBSystemLog(ST_LOG_SYSTEM& stLog) {
    // TODO: 汇编还原 - IDA 0x1402DB010
    return true;
}

int XGameServer::nRand(int nMin, int nMax) {
    // TODO: 汇编还原 - IDA 0x1402DAB00
    return 0;
}

float XGameServer::fRand(float fMin, float fMax) {
    // TODO: 汇编还原 - IDA 0x1402DAB40
    return 0.0f;
}

bool XGameServer::Shutdown(std::uint32_t dwTick) {
    // TODO: 汇编还原 - IDA 0x1402DB0C0
    return true;
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
