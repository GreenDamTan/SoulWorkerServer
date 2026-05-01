// WorldModeMgr.cpp
// CWorldModeMgr 世界模式管理类实现
// 对齐 IDA ControlServer.exe

#include "WorldModeMgr.h"
#include "ControlServer.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_TimeCompat.h"
#include <cstdio>

// 对齐 IDA 0x1400497C0: CWorldModeMgr 构造函数
// IDA 显示构造函数只初始化成员，不调用 Init()
CWorldModeMgr::CWorldModeMgr()
{
    // 对齐 IDA: CFSRWLock::CFSRWLock(&this->m_rwLock)
    // 对齐 IDA: this->m_bLoadReq = 0
    m_bLoadReq = false;
    // 对齐 IDA: this->m_bLoadDB = 0
    m_bLoadDB = false;
    // 对齐 IDA: std::map<...>::map(&this->m_mapWorldMode) - 默认构造
    // 对齐 IDA: ATL::CTime::CTime(&this->m_tInit) - 默认构造
    // 注意: IDA 中没有调用 Init()，Init() 是单独调用的
}

// 对齐 IDA: CWorldModeMgr 析构函数
CWorldModeMgr::~CWorldModeMgr()
{
    m_mapWorldMode.clear();
}

// 对齐 IDA 0x140049840: Init 初始化
void CWorldModeMgr::Init()
{
    // 对齐 IDA: 清空 m_mapWorldMode
    m_mapWorldMode.clear();

    // 对齐 IDA: m_dwUpdateTime = GetTickCount64()
    m_dwUpdateTime = GreenDamTan::GetTickCount64Compat();

    // 对齐 IDA: 从 XControlServer 获取 XResourceMgr 并设置 m_bModeOn
    auto pControlServer = XControlServer::Instance();
    if (pControlServer) {
        // TODO: m_bModeOn = XResourceMgr::GetServerContents(&pControlServer->GetResourceMgr(), E_SERVER_OPTION_D6_MODE);
        // 需要 XResourceMgr 完整实现后替换
    }

    m_bSendMode = false;
    m_nActiveModeDateID = 0;
    m_nActiveModeID = 0;
    m_bLoadDB = false;
    m_bLoadReq = false;
}

// 对齐 IDA 0x1400498A0: LoadMode 加载模式
void CWorldModeMgr::LoadMode(ST_WORLD_MODE_INFO_VEC& stModeVec)
{
    m_bLoadDB = true;

    // TODO: 需人工审查 - 完整实现需要:
    // 1. 从 XResourceMgr 获取 WorldMode 列表
    // 2. 匹配 DB 中的模式信息
    // 3. 创建 CWorldMode 对象并存入 m_mapWorldMode

    CFAutoSlimWriteLock lock(&m_rwLock);

    for (auto& stInfo : stModeVec.vecInfo) {
        if (stInfo.nModeID > 0 && stInfo.nModeDateID > 0) {
            auto pMode = std::tr1::make_shared<CWorldMode>();
            if (pMode) {
                pMode->Init(stInfo);
                std::pair<int, int> key(stInfo.nModeDateID, stInfo.nModeID);
                m_mapWorldMode[key] = pMode;
            }
        }
    }

    // 设置初始化时间
    m_tInit = GreenDamTan::GetCurrentTime();

    char buf[128];
    snprintf(buf, sizeof(buf), "LoadMode completed, %zu modes loaded", m_mapWorldMode.size());
    GreenDamTan_log("WorldModeMgr.cpp", __FUNCTION__, buf);
}

// 对齐 IDA 0x14004A050: Update 更新
// IDA: 复杂的周期更新逻辑，包含加载请求、缓存检查、星期变更检测、模式启动
void CWorldModeMgr::Update()
{
    // 对齐 IDA: if (!m_bLoadReq && m_bModeOn)
    // 发送 DB 请求加载模式 (0x49/0x06)
    if (!m_bLoadReq && m_bModeOn) {
        auto pControlServer = XControlServer::Instance();
        if (pControlServer) {
            // 对齐 IDA: GetOption 返回引用，直接调用
            int nGroupID = pControlServer->GetOption().GetGroupID();
            // TODO: XSendDBPacket xSendDBPacket(0, 0x49, 0x06);
            // xSendDBPacket.XParse << nGroupID;
            // m_bLoadReq = pControlServer->SendDBGame(&xSendDBPacket);
            (void)nGroupID;  // TODO: 需人工审查 - 实际发送逻辑
        }
    }

    // 对齐 IDA: if (!m_bLoadDB) return
    if (!m_bLoadDB) {
        return;
    }

    // 对齐 IDA: 检查缓存加载完成
    auto pControlServer = XControlServer::Instance();
    if (pControlServer && pControlServer->IsCompleteCachingLoad()) {
        // 对齐 IDA: 检查 E_SERVER_OPTION_D6_MODE
        // if (!XResourceMgr::GetServerContents(..., E_SERVER_OPTION_D6_MODE)) {
        //     m_bModeOn = 0;
        //     if (!m_bSendMode) {
        //         发送清空模式包 (0xFB, 0x05)
        //         m_bSendMode = 1;
        //     }
        // }

        // 对齐 IDA: if (GetTickCount64() >= m_dwUpdateTime)
        std::int64_t dwCurrentTime = GreenDamTan::GetTickCount64Compat();
        if (dwCurrentTime >= m_dwUpdateTime) {
            m_dwUpdateTime = dwCurrentTime + 333;  // 对齐 IDA: 333ms 更新周期

            // 对齐 IDA: 获取当前时间并检查星期变更
            GreenDamTan::CTimeCompat tCurr = GreenDamTan::GetCurrentTimeCompat();

            // 对齐 IDA: 检查星期变更
            // if (m_tInit._Get() != nullptr) {
            //     int DayOfWeek = m_tInit.GetDayOfWeek();
            //     int v7 = tCurr.GetDayOfWeek();
            //     if (DayOfWeek != v7) {
            //         InitMode(tCurr);
            //         SendModeList();
            //     }
            // }

            CFAutoSlimWriteLock lock(&m_rwLock);

            // 对齐 IDA: 获取今日模式列表并遍历更新
            std::list<ST_WORLD_MODE> stModeList;
            int nDayOfWeek = GreenDamTan::GetDayOfWeek(m_tInit);
            // TODO: XResourceMgr::GetWorldMode(..., nDayOfWeek, 0, &stModeList);

            // 对齐 IDA: 后续启动的模式列表 (存储 ModeDateID 和 NextModeID 对)
            std::list<std::pair<int, int>> listStartMode;

            for (auto& stMode : stModeList) {
                // 对齐 IDA: 查找模式
                std::pair<int, int> key(stMode.nModeDateID, stMode.nModeID);
                auto it = m_mapWorldMode.find(key);
                if (it != m_mapWorldMode.end() && it->second) {
                    auto& pMode = it->second;

                    // 对齐 IDA: CWorldMode::Update 返回值检查
                    if (pMode->Update(m_bModeOn) == 1) {
                        // 对齐 IDA: if (IsFinish && IsSuccess)
                        if (pMode->IsFinish() && pMode->IsSuccess()) {
                            int nNextID = pMode->GetNextModeID();
                            if (nNextID > 0) {
                                // 对齐 IDA: push_back (ModeDateID, NextModeID) 到启动列表
                                listStartMode.push_back(std::make_pair(stMode.nModeDateID, nNextID));
                            }
                        }
                    }
                }
            }

            // 对齐 IDA: 遍历启动列表，启动后续模式
            for (auto& pairNext : listStartMode) {
                // 对齐 IDA: 查找后续模式
                std::pair<int, int> key2(pairNext.first, pairNext.second);  // 使用存储的 ModeDateID
                auto it2 = m_mapWorldMode.find(key2);
                if (it2 != m_mapWorldMode.end() && it2->second) {
                    it2->second->StartMode();
                }
            }
        }
    }
}

// 对齐 IDA 0x14004A630: SendModeSync 发送模式同步
void CWorldModeMgr::SendModeSync()
{
    if (!m_bLoadDB) {
        return;
    }

    // 对齐 IDA: 获取今日模式列表
    std::list<ST_WORLD_MODE> stModeList;
    int nDayOfWeek = GreenDamTan::GetDayOfWeek(m_tInit);

    auto pControlServer = XControlServer::Instance();
    if (pControlServer) {
        // TODO: XResourceMgr::GetWorldMode(&pControlServer->GetResourceMgr(), nDayOfWeek, 0, &stModeList);
    }

    // 对齐 IDA: 构建模式信息向量
    ST_WORLD_MODE_INFO_VEC stInfoVec;
    for (auto& stMode : stModeList) {
        int nMonsterCount = 0;

        // 对齐 IDA: 如果有后置模式ID, 查找其怪物计数
        // auto pTBMode = XResourceMgr::GetTB_MODE_DISTRICT6(pControlServer->GetResourceMgr(), stMode.nModeID);
        // if (pTBMode && pTBMode->After_Mode_ID) {
        //     查找后置模式获取怪物计数
        // }

        // 对齐 IDA: 查找当前模式
        std::pair<int, int> key(stMode.nModeDateID, stMode.nModeID);
        auto it = m_mapWorldMode.find(key);
        if (it != m_mapWorldMode.end() && it->second) {
            ST_WORLD_MODE_INFO stInfo;
            it->second->GetModeInfo(stInfo);
            stInfo.nMonsterClearCount = nMonsterCount;
            stInfoVec.vecInfo.push_back(stInfo);
        }
    }

    // 对齐 IDA: 发送到所有服务器 (0xFB/0x05)
    if (!stInfoVec.vecInfo.empty()) {
        // XSendPacket xSendPacket(0xFB, 0x05);
        // xSendPacket << stInfoVec;
        // pControlServer->SendPacketAll(&xSendPacket, 0);
    }

    GreenDamTan_log("WorldModeMgr.cpp", __FUNCTION__, "Mode sync sent");
}

// 对齐 IDA 0x14004A9A0: SetModeStartTime 设置模式开始时间
void CWorldModeMgr::SetModeStartTime(int nModeDateID, std::vector<int>& vecModeID, std::int64_t nTime)
{
    for (int nModeID : vecModeID) {
        std::pair<int, int> key(nModeDateID, nModeID);
        auto it = m_mapWorldMode.find(key);
        if (it != m_mapWorldMode.end() && it->second) {
            it->second->SetStartTime(nTime);
        }
    }
}

// 对齐 IDA 0x14004AAB0: SetModeFinishTime 设置模式完成时间
void CWorldModeMgr::SetModeFinishTime(int nModeDateID, std::vector<int>& vecModeID, std::int64_t nTime)
{
    for (int nModeID : vecModeID) {
        std::pair<int, int> key(nModeDateID, nModeID);
        auto it = m_mapWorldMode.find(key);
        if (it != m_mapWorldMode.end() && it->second) {
            it->second->SetFinishTime(nTime);
        }
    }
}

// 对齐 IDA 0x14004ABC0: ReqModeComplete 请求完成
// IDA: 第二个参数为 bClear，当 bClear=true 时调用 SetClearTime
bool CWorldModeMgr::ReqModeComplete(PS_WORLD_MODE_COMPLETE& stComplete, bool bClear)
{
    CFAutoSlimWriteLock lock(&m_rwLock);

    bool bComplete = false;
    std::pair<int, int> key(stComplete.nModeDateID, stComplete.nModeID);
    auto it = m_mapWorldMode.find(key);
    if (it != m_mapWorldMode.end() && it->second) {
        bComplete = it->second->ReqModeComplete(stComplete);
        if (bClear) {
            it->second->SetClearTime();
        }
    }
    return bComplete;
}

// 对齐 IDA 0x14004ACC0: IsActiveMode 是否激活模式
// 遍历 vecModeID，检查 m_mapWorldMode 中是否有 state == 1 的模式
bool CWorldModeMgr::IsActiveMode(int nModeDateID, std::vector<int>& vecModeID)
{
    CFAutoSlimReadLock lock(&m_rwLock);

    for (size_t i = 0; i < vecModeID.size(); ++i) {
        int nModeID = vecModeID[i];
        std::pair<int, int> key(nModeDateID, nModeID);
        auto it = m_mapWorldMode.find(key);
        if (it != m_mapWorldMode.end() && it->second) {
            if (it->second->GetState() == 1) {  // state == 1 表示激活
                return true;
            }
        }
    }
    return false;
}

// 对齐 IDA 0x14004AE00: GetModeList 获取模式列表
// IDA: 使用 XResourceMgr::GetWorldMode 获取今日模式列表，再从 m_mapWorldMode 查找并获取详细信息
void CWorldModeMgr::GetModeList(ST_WORLD_MODE_INFO_VEC& stModeVec)
{
    CFAutoSlimReadLock lock(&m_rwLock);

    // 对齐 IDA: 获取今日模式列表
    std::list<ST_WORLD_MODE> stModeList;
    int nDayOfWeek = GreenDamTan::GetDayOfWeek(m_tInit);

    auto pControlServer = XControlServer::Instance();
    if (pControlServer) {
        // TODO: XResourceMgr::GetWorldMode(&pControlServer->m_xResourceMgr, nDayOfWeek, 0, &stModeList);
    }

    stModeVec.vecInfo.clear();

    // 对齐 IDA: 遍历 stModeList，从 m_mapWorldMode 获取详细信息
    for (auto& stModeInfo : stModeList) {
        std::pair<int, int> key(stModeInfo.nModeDateID, stModeInfo.nModeID);
        auto it = m_mapWorldMode.find(key);
        if (it != m_mapWorldMode.end() && it->second) {
            ST_WORLD_MODE_INFO stInfo;
            it->second->GetModeInfo(stInfo);
            stModeVec.vecInfo.push_back(stInfo);
        }
    }
}

// 对齐 IDA 0x14004AFC0: IsTodayModeList 是否今日模式列表
// IDA: 使用 XResourceMgr::GetWorldMode 获取今日模式列表，再匹配 nModeDateID
bool CWorldModeMgr::IsTodayModeList(int nModeDateID, std::vector<int>& vecModeID)
{
    CFAutoSlimReadLock lock(&m_rwLock);

    vecModeID.clear();

    // 对齐 IDA: 获取今日模式列表
    std::list<ST_WORLD_MODE> stModeList;
    int nDayOfWeek = GreenDamTan::GetDayOfWeek(m_tInit);

    auto pControlServer = XControlServer::Instance();
    if (pControlServer) {
        // TODO: XResourceMgr::GetWorldMode(&pControlServer->m_xResourceMgr, nDayOfWeek, 0, &stModeList);
    }

    // 对齐 IDA: 遍历 stModeList，匹配 nModeDateID
    for (auto& stInfo : stModeList) {
        if (stInfo.nModeDateID == nModeDateID) {
            vecModeID.push_back(stInfo.nModeID);
        }
    }

    return !vecModeID.empty();
}

// 对齐 IDA 0x14004B140: InitMode 初始化模式
// IDA 显示这个函数非常复杂，需要:
// 1. 检查 m_bLoadDB 是否为 true
// 2. 获取写锁
// 3. 从 XResourceMgr::GetWorldMode 获取模式列表
// 4. 遍历模式列表，对已存在的模式调用 InitMode，对新模式调用 Init
void CWorldModeMgr::InitMode(std::int64_t nTime)
{
    // 对齐 IDA: if (!m_bLoadDB) return;
    if (!m_bLoadDB) {
        return;
    }

    CFAutoSlimWriteLock lock(&m_rwLock);

    // 对齐 IDA: 获取当前星期几
    int nDayOfWeek = GreenDamTan::GetDayOfWeek(m_tInit);

    // TODO: XResourceMgr 集成 - 需要调用 GetWorldMode
    std::list<ST_WORLD_MODE> stModeList;
    auto pControlServer = XControlServer::Instance();
    if (pControlServer) {
        // XResourceMgr::GetWorldMode(&pControlServer->m_xResourceMgr, nDayOfWeek, 0, &stModeList);
    }

    // 对齐 IDA: 遍历第一遍 - 对已存在的模式调用 InitMode
    for (auto& stModeInfo : stModeList) {
        std::pair<int, int> key(stModeInfo.nModeDateID, stModeInfo.nModeID);
        auto it = m_mapWorldMode.find(key);
        if (it != m_mapWorldMode.end() && it->second) {
            // 对齐 IDA: CWorldMode::InitMode(pMode, m_tInit)
            it->second->InitMode(m_tInit);
        }
    }

    // 对齐 IDA: 更新 m_tInit
    m_tInit = nTime;

    // 对齐 IDA: 获取新时间的星期几
    int nNewDayOfWeek = GreenDamTan::GetDayOfWeek(m_tInit);

    // TODO: XResourceMgr 集成 - 再次获取模式列表
    stModeList.clear();
    if (pControlServer) {
        // XResourceMgr::GetWorldMode(&pControlServer->m_xResourceMgr, nNewDayOfWeek, 0, &stModeList);
    }

    // 对齐 IDA: 遍历第二遍 - 对新模式调用 Init
    for (auto& stModeInfo : stModeList) {
        std::pair<int, int> key(stModeInfo.nModeDateID, stModeInfo.nModeID);
        auto it = m_mapWorldMode.find(key);
        if (it != m_mapWorldMode.end() && it->second) {
            ST_WORLD_MODE_INFO stInfo{};
            stInfo.nModeID = stModeInfo.nModeID;
            stInfo.nModeDateID = stModeInfo.nModeDateID;
            it->second->Init(stInfo);
        }
    }

    GreenDamTan_log("WorldModeMgr.cpp", __FUNCTION__, "Mode initialized");
}

// 对齐 IDA 0x14004B490: SendModeList 发送模式列表
// IDA: 独立实现，不调用 SendModeSync
void CWorldModeMgr::SendModeList()
{
    // 对齐 IDA: if (!m_bLoadDB) return;
    if (!m_bLoadDB) {
        LogHelper::LogError("game.contents", "WORLD_MODE LOAD ERROR - SendModeList");
        return;
    }

    // 对齐 IDA: 获取今日模式列表
    std::list<ST_WORLD_MODE> stModeList;
    int nDayOfWeek = GreenDamTan::GetDayOfWeek(m_tInit);

    auto pControlServer = XControlServer::Instance();
    if (pControlServer) {
        // TODO: XResourceMgr::GetWorldMode(&pControlServer->m_xResourceMgr, nDayOfWeek, 0, &stModeList);
    }

    // 对齐 IDA: 构建模式信息向量
    ST_WORLD_MODE_INFO_VEC stInfoVec;
    for (auto& stMode : stModeList) {
        // 对齐 IDA: 查找当前模式
        std::pair<int, int> key(stMode.nModeDateID, stMode.nModeID);
        auto it = m_mapWorldMode.find(key);
        if (it != m_mapWorldMode.end() && it->second) {
            ST_WORLD_MODE_INFO stInfo;
            it->second->GetModeInfo(stInfo);
            stInfoVec.vecInfo.push_back(stInfo);
        }
    }

    // 对齐 IDA: 发送到所有服务器 (0xFB/0x05)
    if (!stInfoVec.vecInfo.empty()) {
        // XSendPacket xSendPacket(0xFB, 0x05);
        // xSendPacket << stInfoVec;
        // pControlServer->SendPacketAll(&xSendPacket, 0);
    }

    // 对齐 IDA: 日志输出
    LogHelper::LogError("game.contents", "WORLD_MODE LOAD - SendModeList[modeCnt:%d, Sync:%d]",
        static_cast<int>(stInfoVec.vecInfo.size()), 0);
}

// 对齐 IDA 0x14004B730: UpdateMonsterCount 更新怪物计数
// 查找模式，调用 AddMonsterCount，如果返回 1 则更新计数并发送 0xFB/0x02 包
void CWorldModeMgr::UpdateMonsterCount(PS_WORLD_MODE_UPDATE& stUpdate)
{
    CFAutoSlimWriteLock lock(&m_rwLock);

    std::pair<int, int> key(stUpdate.stInfo.nModeDateID, stUpdate.stInfo.nModeID);
    auto it = m_mapWorldMode.find(key);
    if (it != m_mapWorldMode.end() && it->second) {
        if (it->second->AddMonsterCount() == 1) {
            // 更新计数成功，设置标志
            ++stUpdate.nMonsterClearCount;
            stUpdate.bUpdate = true;
        }
    }

    // 发送 0xFB/0x02 包到所有服务器
    auto pControlServer = XControlServer::Instance();
    if (pControlServer) {
        // TODO: XSendPacket xSendPacket(0xFB, 0x02);
        // xSendPacket << stUpdate;
        // pControlServer->SendPacketAll(&xSendPacket, 0);
    }
}

// 对齐 IDA 0x14004B8A0: IsEnterMode 是否进入模式
// 检查玩家是否可以进入指定模式，返回错误码或 0 表示成功
// IDA 显示参数实际是 std::vector<ST_WORLD_MODE>
int CWorldModeMgr::IsEnterMode(std::vector<ST_WORLD_MODE>& stList, int& nActiveModeID)
{
    CFAutoSlimReadLock lock(&m_rwLock);

    int nActiveModeDateID = 0;
    nActiveModeID = 0;

    // 获取当前激活模式
    GetActiveMode(nActiveModeDateID, nActiveModeID);

    if (!nActiveModeDateID || !nActiveModeID) {
        // 没有激活的模式
        LogHelper::LogError("game.contents", "<DISTRICT6_MODE> Faild IsEnterMode Active(%d, %d,)",
            nActiveModeDateID, nActiveModeID);
        return 55064;  // 错误码：无激活模式
    }

    // 对齐 IDA: 遍历 stList 查找匹配 nActiveModeDateID 的元素
    std::int64_t biEnterDate = 0;
    for (size_t i = 0; i < stList.size(); ++i) {
        // 对齐 IDA: nActiveModeDateID 与 stList[i].nModeDateID 比较
        if (nActiveModeDateID == stList[i].nModeDateID) {
            biEnterDate = stList[i].biEnterDate;
            break;
        }
    }

    if (!biEnterDate) {
        return 0;  // 没有进入日期，允许进入
    }

    // 检查模式开始时间
    std::pair<int, int> key(nActiveModeDateID, nActiveModeID);
    auto it = m_mapWorldMode.find(key);
    if (it != m_mapWorldMode.end() && it->second) {
        std::int64_t biModeStartTime = 0;
        it->second->GetModeStartTime(biModeStartTime);

        if (biEnterDate >= biModeStartTime) {
            // 进入时间在模式开始时间之后，不允许
            LogHelper::LogError("game.contents", "<DISTRICT6_MODE> Faild IsEnterMode Time(%d, %d,)",
                nActiveModeDateID, nActiveModeID);
            return 55090;  // 错误码：时间检查失败
        }
    }

    return 0;  // 成功
}

// 对齐 IDA 0x14004BB40: GetActiveMode 获取激活模式
// 从 XResourceMgr 获取今日模式列表，遍历查找 state == 1 的模式
void CWorldModeMgr::GetActiveMode(int& nActiveModeDateID, int& nModeID)
{
    CFAutoSlimReadLock lock(&m_rwLock);

    nActiveModeDateID = 0;
    nModeID = 0;

    // 获取今日模式列表
    std::list<ST_WORLD_MODE> stModeList;
    int nDayOfWeek = GreenDamTan::GetDayOfWeek(m_tInit);

    auto pControlServer = XControlServer::Instance();
    if (pControlServer) {
        // TODO: XResourceMgr::GetWorldMode(&pControlServer->m_xResourceMgr, nDayOfWeek, 0, &stModeList);
    }

    // 遍历查找激活模式
    for (auto it_list = stModeList.begin(); it_list != stModeList.end(); ++it_list) {
        std::pair<int, int> key(it_list->nModeDateID, it_list->nModeID);
        auto it = m_mapWorldMode.find(key);
        if (it != m_mapWorldMode.end() && it->second) {
            if (it->second->GetState() == 1) {  // state == 1 表示激活
                nActiveModeDateID = it_list->nModeDateID;
                nModeID = it_list->nModeID;
                return;
            }
        }
    }
}
