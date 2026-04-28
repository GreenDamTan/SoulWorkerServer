// ControlServer.cpp
// ControlServer 服务器主类实现 (Stub)

#include "ControlServer.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_CLogThreadManager.h"
#include <algorithm>
#include <cstdarg>

// XPRINT 宏定义 - 对齐 IDA
#define XPRINT(msg) LogHelper::LogError("game.system", "%s", msg)

// 单例实例
static XControlServer* g_pControlServerInstance = nullptr;

// ============================================================================
// 单例访问
// ============================================================================

XControlServer* XControlServer::Instance()
{
    if (!g_pControlServerInstance)
    {
        g_pControlServerInstance = new XControlServer();
    }
    return g_pControlServerInstance;
}

// ============================================================================
// 构造/析构
// ============================================================================

XControlServer::XControlServer()
    : BaseClass()
    , m_pLoginServer(nullptr)
    , m_pCommunityServer(nullptr)
    , m_pMonitor(nullptr)
    , m_nMoneySupply(0)
    , m_nMaxServerUserCount(0)
    , m_bAddLogin(false)
    , m_bRegisterAuth(false)
    , m_dwCachingLoad(0)
{
    memset(m_nSGAuthTypeCount, 0, sizeof(m_nSGAuthTypeCount));
}

XControlServer::~XControlServer()
{
    // 清理
}

// ============================================================================
// 初始化
// ============================================================================

bool XControlServer::InitServer()
{
    // 对齐 IDA 0x14000BD00 (XControlServer::InitServer)
    const char* szName = GetName();

    // 启动日志线程管理器
    CLogThreadManager* pLogMgr = TXSingleton<CLogThreadManager>::Instance();
    pLogMgr->Start(szName);
    LogHelper::LogInfo("game.contents", "Initialize server...");

    // 初始化随机种子
    XSeed::Init(&m_xSeed, 1);

    // 初始化成员变量
    m_pMonitor = nullptr;
    m_pLoginServer = nullptr;
    m_pCommunityServer = nullptr;
    m_bRegisterAuth = false;
    m_dwCachingLoad = 0;
    memset(&m_stServerGroupInfo, 0, sizeof(m_stServerGroupInfo));

    // 获取服务器配置
    DWORD dwServerID = m_xOption.GetServerID();
    const char* szGameDNS = m_xOption.GetDNS(1);
    const char* szDNS = m_xOption.GetDNS(2);

    // 初始化资源管理器
    if (!m_xResourceMgr.Init(szDNS, szGameDNS, dwServerID)) {
        XPRINT(" Error Table Load fail ");
        return false;
    }

    // 加载资源
    const char* szResFilePath = m_xOption.GetResFilePath();
    RES_LOAD_TYPE resLoadType = m_xOption.GetResLoadType();
    if (!m_xResourceMgr.Load(resLoadType, szResFilePath, 0, 0)) {
        XPRINT(" Error Table Load fail ");
        return false;
    }

    LogHelper::LogInfo("game.system", "[INIT] ResourceMgr - Load Complete!");

    // 设置服务器内容开关 (如果 ContentsOption.nOptionFlag == 2)
    XOption& xOption = this->GetOption();
    CONTENTS_OPTION_INFO* pContentsOption = xOption.GetContentsOption();
    if (pContentsOption->nOptionFlag == 2) {
        for (int i = E_SERVER_OPTION_ATTENDANCE; i < E_SERVER_OPTION_MAX; ++i) {
            m_xResourceMgr.SetServerContents(i, pContentsOption->bContents[i]);
        }
    }

    // 初始化物品工厂
    BYTE byChannel = m_xOption.GetChannel();
    BYTE byGroupID = m_xOption.GetGroupID();
    XItemFactory::Init(&m_xItemFactory, byGroupID, byChannel);
    int nType = m_xOption.GetType();
    LogHelper::LogInfo("game.system", "<ITEM_FACTORY> Factory Init ( %d, %d )", byGroupID, nType);

    // 初始化数据库代理管理器
    XGameDBSocketMgr::Init(&m_xDBAgentMgr);
    XGameDBSocketMgr::AutoConnect(&m_xDBAgentMgr);
    LogHelper::LogInfo("game.system", "[INIT] DBAgent Init ");

    // 启动观察套接字
    if (!CObserveSocket::StartUp(&m_scObserveSocket, &m_xOption)) {
        return false;
    }

    // 初始化世界模式管理器
    CWorldModeMgr::Init(&m_worldModeManager);
    LogHelper::LogInfo("game.system", "[INIT] WorldModeManager - Init ");

    // 设置缓存加载状态
    SetCachingLoad(E_SERVER_CACHING_LOAD_DB_COMPLETE);
    LogHelper::LogInfo("game.system", "[INIT] Complete Server Init");

    return true;
}

bool XControlServer::Clear()
{
    // 对齐 IDA 0x14000C0C0 (XControlServer::Clear)
    CFAutoSlimWriteLock lock(&m_rwLock);

    if (m_bClose)
    {
        return false;
    }

    // 停止日志线程管理器
    CLogThreadManager* pLogMgr = TXSingleton<CLogThreadManager>::Instance();
    pLogMgr->End();

    // 清理服务器映射
    {
        CFAutoSlimWriteLock serverLock(&m_rwServerLock);
        m_mapMazeServer.clear();
        m_mapGameServer.clear();
        m_mapMyRoomServer.clear();
    }

    // 清理资源管理器
    m_xResourceMgr.Clear();

    m_pMonitor = nullptr;
    m_pLoginServer = nullptr;

    // 断开数据库代理
    XGameDBSocketMgr::DisConnect(&m_xDBAgentMgr);

    m_bClose = true;
    return true;
}

// ============================================================================
// 用户管理
// ============================================================================

bool XControlServer::AddUser(CServer* pServer, STCharInfo& stInfo, UXMapID uxMapID,
                              DWORD dwIP, BYTE byTradePW, __int64 biSessionID, BYTE byBlockType)
{
    // 对齐 IDA 0x14000C220 (XControlServer::AddUser)
    CFAutoSlimWriteLock lock(&m_rwLock);

    // 检查是否已存在相同 UAID 的用户
    auto existingUser = m_UserInfos.FindByUAID(stInfo.dwUAID);
    if (existingUser)
    {
        // 如果已有用户的 AuthSessionID 更大，发送踢出包给新连接并返回 false
        if (existingUser->GetAuthSessionID() > biSessionID)
        {
            PS_KICK_USER_INFO psKick{};
            psKick.dwUAID = stInfo.dwUAID;
            psKick.byKickType = 1;

            XSendPacket xSendPacket(0xF3, 7);
            xSendPacket << psKick;
            pServer->SendEx(xSendPacket);
            return false;
        }

        // 否则删除旧用户并踢出
        PS_KICK_USER_INFO psKick{};
        psKick.dwUAID = stInfo.dwUAID;
        psKick.byKickType = 1;
        m_UserInfos.Erase(existingUser->GetCID());
        KickoutUser_NoLock(psKick, false);
    }

    // 创建新用户对象 (size: 0x378 = 888 bytes)
    CUserObject* pNewUser = new CUserObject(pServer, stInfo, uxMapID);
    std::tr1::shared_ptr<CUserObject> pUserInfo(pNewUser);

    pUserInfo->SetIP(dwIP);
    if (byTradePW)
    {
        pUserInfo->SetTradePWState(byTradePW);
    }
    pUserInfo->SetAuthSessionID(biSessionID);
    pUserInfo->SetBlockType(byBlockType);

    // 设置 GroupIDTick = 当前时间 + 300秒 (5分钟超时)
    ULONGLONG dw64Tick = GetTickCount64() + 300000;
    pUserInfo->SetGroupIDTick(dw64Tick);

    // 插入用户索引
    m_UserInfos.Insert(pUserInfo);

    // 设置连接时间戳 (对齐 IDA: v17->__vftable = TickCount64)
    pUserInfo->SetConnectTick(GetTickCount64());

    // 释放锁 (作用域结束自动释放)
    lock.~CFAutoSlimWriteLock();

    // 更新服务器用户状态
    CalculateServerUserStaus();

    // 更新用户地图信息
    UXMapID uxEmptyMapID;
    m_worldManager.UpdateUserMap(stInfo.uxActorID.dwActorID, uxEmptyMapID, uxMapID);
    return true;
}

void XControlServer::RemoveUser(DWORD dwActorID, int nAccountState, bool bKick_AlreadyLogin)
{
    // 对齐 IDA 0x14000C610 (XControlServer::RemoveUser)
    std::tr1::shared_ptr<CUserObject> pUserInfo = GetUser(dwActorID);
    if (!pUserInfo)
    {
        LogHelper::LogDebug("game.relay", "<REMOVE_USER> Failed < UID : %d > ", dwActorID);
        return;
    }

    // 计算最后服务器
    SHORT shLastServer = 0;
    if (nAccountState == 2 || bKick_AlreadyLogin)
    {
        XControlServer* pInstance = TXSingleton<XControlServer>::Instance();
        XOption& xOption = pInstance->GetOption();
        shLastServer = static_cast<SHORT>(xOption.GetGroupID());
    }

    // 对齐 IDA: 发送 AccountDB 用户状态更新包 (main=2, sub=2)
    XSendDBPacket xSendDBPacket(0, 2, 2);
    xSendDBPacket.XParse << static_cast<int>(pUserInfo->GetUAID());
    xSendDBPacket.XParse << shLastServer;
    xSendDBPacket.XParse << nAccountState;
    xSendDBPacket.XParse << static_cast<int>(pUserInfo->IsState());
    SendDBAccount(xSendDBPacket);

    // 计算游戏时间 (对齐 IDA: TickCount64 - m_dwConnectTick)
    ULONGLONG dwTick = GetTickCount64();
    int nPlayTime = static_cast<int>((dwTick - pUserInfo->GetConnectTick()) / 1000);

    // 对齐 IDA: 发送 DB 日志 (type=2, subtype=100)
    SendDBLog(pUserInfo->GetUAID(), pUserInfo->GetCID(), 2, 100, nPlayTime, 0, 0, 0, 0, 0, 0, nullptr);

    // 更新世界管理器中的用户地图 (从旧地图移到空地图)
    UXMapID uxOldMapID;
    UXMapID uxEmptyMapID;
    pUserInfo->GetMapIns(&uxOldMapID);
    m_worldManager.UpdateUserMap(dwActorID, uxOldMapID, uxEmptyMapID);

    // 处理队伍/Force 成员移除
    ST_PARTY_INFO stPartyInfo{};
    pUserInfo->GetPartyInfo(&stPartyInfo);
    if (stPartyInfo.byGroupType == 1)
    {
        // Party 成员移除
        m_partyManager.RemoveMember(stPartyInfo.nID, dwActorID);
    }
    else if (stPartyInfo.byGroupType == 2)
    {
        // Force 成员移除
        m_forceManager.RemoveMember(stPartyInfo.nID, dwActorID);
    }

    LogHelper::LogDebug("game.relay", "<REMOVE_USER> < UID : %d / State : %d > ", dwActorID, nAccountState);

    // 对齐 IDA: 更新认证类型计数 (m_nSGAuthTypeCount)
    BYTE byAuthType = pUserInfo->GetAuthType();
    int nAuthType = byAuthType;
    if (nAuthType > 0 && nAuthType < 4)
    {
        if (--m_nSGAuthTypeCount[nAuthType] < 0)
        {
            m_nSGAuthTypeCount[nAuthType] = 0;
            LogHelper::LogError("game.relay", "[AUTH_TYPE] Wrong Value - m_nSGAuthTypeCount[ %d ] < 0 ", nAuthType);
        }
    }

    // 从用户索引中删除 (使用写锁)
    {
        CFAutoSlimWriteLock lock(&m_rwLock);
        m_UserInfos.Erase(pUserInfo->GetCID());
    }

    // 对齐 IDA: 如果用户有 UAID 且状态正常，发送登出日志
    DWORD dwUAID = pUserInfo->GetUAID();
    if (dwUAID && !nAccountState)
    {
        __int64 biAuthSessionID = pUserInfo->GetAuthSessionID();
        int nIsState = static_cast<int>(pUserInfo->IsState());
        int nAuthTypeValue = static_cast<int>(byAuthType);
        SendDBLog(dwUAID, 0, 1, 2, 0, 0, nIsState, nAuthTypeValue, 0, 0, biAuthSessionID, nullptr);
    }

    // 重新计算服务器用户状态
    CalculateServerUserStaus();
}

std::tr1::shared_ptr<CUserObject> XControlServer::GetUser(DWORD dwActorID)
{
    CFAutoSlimReadLock lock(&m_rwLock);
    return m_UserInfos.FindByCID(dwActorID);
}

std::tr1::shared_ptr<CUserObject> XControlServer::GetUser(const wchar_t* szName)
{
    CFAutoSlimReadLock lock(&m_rwLock);
    return m_UserInfos.FindByName(std::wstring(szName));
}

// ============================================================================
// 发送包
// ============================================================================

bool XControlServer::SendPacketToLoginServer(XSendPacket& packet)
{
    if (m_pLoginServer)
    {
        m_pLoginServer->SendEx(packet);
        return true;
    }
    return false;
}

bool XControlServer::SendPacketToGameServer(XSendPacket& packet, CServer* pServer)
{
    if (pServer)
    {
        pServer->SendEx(packet);
        return true;
    }
    return false;
}

bool XControlServer::SendPacketAll(XSendPacket& packet, bool bExcludeLogin)
{
    for (auto& pair : m_mapGameServer)
    {
        if (pair.second)
        {
            pair.second->SendEx(packet);
        }
    }
    return true;
}

// ============================================================================
// 服务器信息管理
// ============================================================================

void XControlServer::AddLoginServerInfo(CServer* pServer)
{
    m_pLoginServer = pServer;
    m_bAddLogin = true;
}

void XControlServer::AddCommunityServerInfo(CServer* pServer)
{
    m_pCommunityServer = pServer;
}

void XControlServer::AddGameServerInfo(CServer* pServer)
{
    DWORD dwServerID = pServer->GetServerID();
    m_mapGameServer[dwServerID] = pServer;
}

void XControlServer::AddMazeServerInfo(CServer* pServer, int nIndex)
{
    DWORD dwServerID = pServer->GetServerID();
    m_mapMazeServer[dwServerID] = pServer;
}

void XControlServer::AddServerInfo(CServer* pServer)
{
    // 对齐 IDA 0x14000DF20 (XControlServer::AddServerInfo)
    if (!pServer) return;

    SS_SERVER_INFO* pInfo = pServer->GetServerInfo();
    int ServerType = pInfo->nType;
    switch (ServerType)
    {
        case 1:
            AddLoginServerInfo(pServer);
            break;
        case 2:
            AddGameServerInfo(pServer);
            AddMyRoomServerInfo(pServer);
            break;
        case 3:
            AddCommunityServerInfo(pServer);
            break;
    }

    int sPort = pInfo->sPort;
    char* szPublicIP = pInfo->szPublicIP;
    DWORD dwServerID = pServer->GetServerID();

    LogHelper::LogDebug("game.relay", "<ADD_SERVER> Add %d Server Info %s / %d ", dwServerID, szPublicIP, sPort);

    // 发送服务器添加确认包 (main=0xF2, sub=1)
    XSendPacket xSendPacket(0xF2, 1);
    xSendPacket.XParse << static_cast<int>(dwServerID);
    pServer->SendEx(xSendPacket);
}

void XControlServer::RemoveGameServerInfo(SS_SERVER_INFO& stInfo)
{
    // 对齐 IDA 0x14000E0B0 (XControlServer::RemoveGameServerInfo)
    DWORD dwServerID = stInfo.dwID;

    // 第一步：删除该服务器上的所有用户
    // 对齐 IDA: boost::multi_index::get<1>::equal_range(dwServerID) 遍历删除
    {
        CFAutoSlimWriteLock lock(&m_rwLock);
        m_UserInfos.EraseByServerID(dwServerID);
        GreenDamTan_log(__FILE__, __FUNCTION__, "RemoveGameServerInfo: removed users from server");
    }

    // 第二步：从服务器映射中删除
    {
        CFAutoSlimWriteLock lock(&m_rwServerLock);

        // 从游戏服务器映射删除
        auto itGame = m_mapGameServer.find(dwServerID);
        if (itGame != m_mapGameServer.end())
        {
            m_mapGameServer.erase(itGame);
        }

        // 从迷宫服务器映射删除
        auto itMaze = m_mapMazeServer.find(dwServerID);
        if (itMaze != m_mapMazeServer.end())
        {
            m_mapMazeServer.erase(itMaze);
        }
    }

    // 第三步：删除 MyRoom 服务器信息
    RemoveMyRoomServerInfo(dwServerID);
}

void XControlServer::RemoveMyRoomServerInfo(DWORD dwServerID)
{
    // 对齐 IDA 0x140011D50 (XControlServer::RemoveMyRoomServerInfo)
    CFAutoSlimWriteLock lock(&m_rwServerLock);

    auto it = m_mapMyRoomServer.find(dwServerID);
    if (it != m_mapMyRoomServer.end())
    {
        m_mapMyRoomServer.erase(it);
        GreenDamTan_log(__FILE__, __FUNCTION__, "RemoveMyRoomServerInfo: removed server");
    }
}

void XControlServer::AddMyRoomServerInfo(CServer* pServer)
{
    // 对齐 IDA 0x140011CB0 (XControlServer::AddMyRoomServerInfo)
    if (!pServer) return;

    CFAutoSlimWriteLock lock(&m_rwServerLock);
    DWORD dwServerID = pServer->GetServerID();
    m_mapMyRoomServer[dwServerID] = pServer;

    GreenDamTan_log(__FILE__, __FUNCTION__, "AddMyRoomServerInfo: added server");
}

void XControlServer::RemoveServerInfo(CServer* pServer, SS_SERVER_INFO& stInfo)
{
    // 对齐 IDA 0x14000E310 (XControlServer::RemoveServerInfo)
    if (stInfo.nType == 2)
    {
        RemoveGameServerInfo(stInfo);
        m_worldManager.DeleteMap(pServer);
        ClearUserState(stInfo.dwID);
    }

    LogHelper::LogInfo("game.system", "<REMOVE_SERVER> Remove Server ID: %d", stInfo.dwID);
}

// ============================================================================
// 迷宫创建
// ============================================================================

void XControlServer::ReqCreateMaze(CServer* pServer, ST_CREATE_MAZE& stCreate)
{
    // 对齐 IDA 0x14000E3A0 (XControlServer::ReqCreateMaze)
    CServer* pSelectServer = nullptr;

    // 如果有父迷宫，先尝试在该迷宫服务器创建
    if (stCreate.uxParentMazeID.nMapID > 0)
    {
        CServer* pParentServer = FindServerFromMaze(stCreate.uxParentMazeID);
        if (!pParentServer)
        {
            GreenDamTan_log(__FILE__, __FUNCTION__, "CreateMaze: parent maze server not found");
            return;
        }

        // 检查服务器状态和容量
        if (pParentServer->CanMakeMaze() && pParentServer->GetServerState() == 2)
        {
            float fMaxUser = pParentServer->GetMaxUserCount() * 0.7f;
            if (fMaxUser > pParentServer->GetUserCount())
            {
                pSelectServer = pParentServer;
            }
        }
    }

    // 如果 nCreateType == 2，强制使用请求服务器
    if (stCreate.nCreateType == 2)
    {
        pSelectServer = pServer;
    }

    // 如果没有选中服务器，尝试选择负载最低的迷宫服务器
    if (!pSelectServer)
    {
        // 先检查请求服务器本身
        if (pServer->GetServerState() == 2 && pServer->CanMakeMaze())
        {
            float fMaxUser = pServer->GetMaxUserCount() * 0.7f;
            if (fMaxUser >= pServer->GetUserCount())
            {
                pSelectServer = pServer;
            }
        }

        // 如果请求服务器不合适，遍历所有迷宫服务器
        if (!pSelectServer)
        {
            int nMinUser = 0;
            CFAutoSlimReadLock lock(&m_rwServerLock);

            for (auto& pair : m_mapMazeServer)
            {
                CServer* pMazeServer = pair.second;
                if (pMazeServer && pMazeServer->CanMakeMaze())
                {
                    float fMaxUser = pMazeServer->GetMaxUserCount() * 0.95f;
                    int nUserCount = pMazeServer->GetUserCount();
                    if (nUserCount < fMaxUser && pMazeServer->GetServerState() == 2)
                    {
                        if (nUserCount < nMinUser || !pSelectServer)
                        {
                            nMinUser = nUserCount;
                            pSelectServer = pMazeServer;
                        }
                    }
                }
            }
        }
    }

    // 执行迷宫创建
    if (pSelectServer)
    {
        // 填充服务器信息
        SS_SERVER_INFO* pServerInfo = pSelectServer->GetServerInfo();
        strcpy_s(stCreate.szIP, pServerInfo->szPublicIP);
        stCreate.sPort = pServerInfo->sPort;

        // 对齐 IDA: 生成迷宫地图ID
        stCreate.uxMapID = pSelectServer->GetMapID(stCreate.wReqMapID);
        stCreate.dwServerID = pSelectServer->GetServerID();

        // 存储创建请求以等待 Relay 响应
        ST_CREATE_MAZE_FOR_RELAY stCreateMazeReq{};
        stCreateMazeReq.pReqChannelServer = pServer;
        stCreateMazeReq.pReqMazeServer = pSelectServer;
        stCreateMazeReq.dwTime = GetTickCount64();
        stCreateMazeReq.vecEnterMember.assign(stCreate.vecEnterMember.begin(), stCreate.vecEnterMember.end());

        // 插入请求映射
        m_mapCreateMazeReq[stCreate.uxMapID] = stCreateMazeReq;

        // 发送创建请求到迷宫服务器 (0xF2, 0x21)
        XSendPacket packet(0xF2, 0x21);
        packet << stCreate;
        pSelectServer->SendEx(packet);

        LogHelper::LogDebug("game.relay", "<CREATE_MAZE> Maze Create Request Sent");
    }
    else
    {
        // 无法创建迷宫，返回错误响应
        PS_ENTER_MAP_RES stEnterMapRes{};
        memcpy(&stEnterMapRes, &stCreate, sizeof(PS_ENTER_MAP_RES));
        stEnterMapRes.nResult = 3;
        stEnterMapRes.stPartyInfo = stCreate.stPartyInfo;

        XSendPacket xSendPacket(0xF2, 0x22);
        xSendPacket << stEnterMapRes;
        pServer->SendEx(xSendPacket);

        GreenDamTan_log(__FILE__, __FUNCTION__, "CreateMaze: no available server");
    }
}

void XControlServer::ReqGoBackMaze(CServer* pServer, ST_GO_BACK_MAZE& stGoBack)
{
    // 对齐 IDA 0x14000EAC0 (XControlServer::ReqGoBackMaze)
    // 获取迷宫信息
    auto pMazeInfo = GetMazeInfo(stGoBack.uxMapID);
    if (!pMazeInfo)
    {
        // 迷宫不存在，发送失败响应
        XSendPacket xSendPacket(0xF2, 0x28);
        stGoBack.bResult = false;
        xSendPacket << stGoBack;
        pServer->SendEx(xSendPacket);
        return;
    }

    // 获取父迷宫
    auto pParentMaze = pMazeInfo->GetParentMaze();
    if (!pParentMaze)
    {
        // 父迷宫不存在，发送失败响应
        XSendPacket xSendPacket(0xF2, 0x28);
        stGoBack.bResult = false;
        xSendPacket << stGoBack;
        pServer->SendEx(xSendPacket);
        return;
    }

    // 获取父迷宫信息并填充响应
    ST_MAP_INFO stMapInfo = pParentMaze->GetMazeInfo();
    memcpy(&stGoBack, &stMapInfo, sizeof(ST_MAP_INFO));

    // 发送成功响应
    XSendPacket packet(0xF2, 0x28);
    stGoBack.bResult = true;
    packet << stGoBack;
    pServer->SendEx(packet);
}

void XControlServer::ResCreateMaze(ST_CREATE_MAZE& stCreate)
{
    // 对齐 IDA 0x14000ED10 (XControlServer::ResCreateMaze)
    // 查找创建请求
    auto it = m_mapCreateMazeReq.find(stCreate.uxMapID);
    if (it == m_mapCreateMazeReq.end())
    {
        GreenDamTan_log(__FILE__, __FUNCTION__, "ResCreateMaze: request not found");
        return;
    }

    ST_CREATE_MAZE_FOR_RELAY& stInfo = it->second;
    CServer* pReqChannelServer = stInfo.pReqChannelServer;
    CServer* pReqMazeServer = stInfo.pReqMazeServer;

    if (!pReqChannelServer)
    {
        m_mapCreateMazeReq.erase(it);
        return;
    }

    // 发送响应到请求服务器
    PS_ENTER_MAP_RES stEnterMapRes{};
    memcpy(&stEnterMapRes, &stCreate, sizeof(PS_ENTER_MAP_RES));
    stEnterMapRes.nResult = stCreate.nResult;
    stEnterMapRes.bChangeServer = false;
    stEnterMapRes.stPartyInfo = stCreate.stPartyInfo;

    XSendPacket packet(0xF2, 0x22);
    packet << stEnterMapRes;
    pReqChannelServer->SendEx(packet);

    GreenDamTan_log(__FILE__, __FUNCTION__, "ResCreateMaze: response sent");

    // 如果创建成功
    if (stEnterMapRes.nResult == 0)
    {
        // 设置 Party/Force 迷宫ID
        UXMapID uxBeforeMapID{};  // 初始化为空 UXMapID
        if (stCreate.stPartyInfo.byGroupType == 1 && stCreate.stPartyInfo.nID)
        {
            m_partyManager.SetMazeID(stCreate.stPartyInfo.nID, stCreate.uxMapID, uxBeforeMapID);
            // 发送迷宫ID更新包到 CommunityServer
            XSendPacket xSendPacket(0xF2, 0x45);
            xSendPacket.XParse << stCreate.stPartyInfo.nID;
            xSendPacket.XParse << stCreate.uxMapID.nMapID;
            xSendPacket.XParse << uxBeforeMapID.nMapID;
            if (m_pCommunityServer)
            {
                m_pCommunityServer->SendEx(xSendPacket);
            }
        }
        else if (stCreate.stPartyInfo.byGroupType == 2 && stCreate.stPartyInfo.nID)
        {
            m_forceManager.SetMazeID(stCreate.stPartyInfo.nID, stCreate.uxMapID, uxBeforeMapID);
            // 发送迷宫ID更新包到 CommunityServer
            XSendPacket xSendPacket(0xF2, 0x46);
            xSendPacket.XParse << stCreate.stPartyInfo.nID;
            xSendPacket.XParse << stCreate.uxMapID.nMapID;
            xSendPacket.XParse << uxBeforeMapID.nMapID;
            if (m_pCommunityServer)
            {
                m_pCommunityServer->SendEx(xSendPacket);
            }
        }

        // 创建迷宫信息对象并添加到服务器
        std::tr1::shared_ptr<CMazeInfo> pMazeInfo = m_factoryMaze.Create();
        if (pMazeInfo)
        {
            pMazeInfo->Init(stCreate);
            if (stInfo.pReqMazeServer)
            {
                stInfo.pReqMazeServer->AddMaze(stCreate.uxMapID, pMazeInfo);
            }
        }
        GreenDamTan_log(__FILE__, __FUNCTION__, "ResCreateMaze: maze info created");
    }

    // 删除请求记录
    m_mapCreateMazeReq.erase(it);
}

// ============================================================================
// 更新
// ============================================================================

// 静态定时器变量 (对齐 IDA)
static ULONGLONG dwDeleteTime = 0;         // 迷宫服务器更新定时器
static ULONGLONG m_nMoneyTick = 0;         // 金币统计定时器
static ULONGLONG dwServerGroupSync = 0;    // 服务器组同步定时器
static ULONGLONG dwSGUpdate = 0;           // SG更新定时器
static ULONGLONG dwCheckGroupIDTick = 0;   // GroupID检查定时器
static __int64 m_biMoneySum = 0;           // 金币累计总和
static DWORD _S5 = 0;                      // 初始化标志位

void XControlServer::OnUpdate(ULONGLONG dwTick)
{
    // 对齐 IDA 0x14000F240 (XControlServer::OnUpdate)
    BaseClass::OnUpdate(dwTick);

    // 初始化静态变量 (使用 _S5 标志位控制)
    if ((_S5 & 1) == 0) { _S5 |= 1; dwDeleteTime = GetTickCount64() + 5000; }
    if ((_S5 & 2) == 0) { _S5 |= 2; m_nMoneyTick = GetTickCount64() + 60000; }
    if ((_S5 & 4) == 0) { _S5 |= 4; dwServerGroupSync = GetTickCount64() + 5000; }
    if ((_S5 & 8) == 0) { _S5 |= 8; dwSGUpdate = dwTick + 60000; }
    if ((_S5 & 0x10) == 0) { _S5 |= 0x10; dwCheckGroupIDTick = dwTick + 600000; }

    // 迷宫服务器更新 (每 5 秒)
    if (dwDeleteTime < dwTick)
    {
        dwDeleteTime = dwTick + 5000;
        CFAutoSlimReadLock lock(&m_rwServerLock);
        for (auto& pair : m_mapMazeServer)
        {
            if (pair.second)
            {
                pair.second->OnUpdate(dwTick);
            }
        }
    }

    // 金币统计更新 (每 60 秒)
    if (m_nMoneyTick < dwTick)
    {
        int nServerUserCount = 0;
        int nLogin = 0;
        {
            CFAutoSlimReadLock lock(&m_rwLock);
            if (GetLoginServer())
            {
                nLogin = GetLoginServer()->GetUserCount();
            }
            nServerUserCount = nLogin + static_cast<int>(m_UserInfos.Size());
        }
        m_nMoneyTick = dwTick + 60000;
        m_biMoneySum += m_nMoneySupply;
        // 对齐 IDA: SendDBLog (type=100, subtype=2) - 金币供应
        SendDBLog(0, 0, 100, 2, 0, 0, 0, 0, 0, m_nMoneySupply, m_biMoneySum, nullptr);
        // 对齐 IDA: SendDBLog (type=100, subtype=3) - 用户统计
        SendDBLog(0, 0, 100, 3, nServerUserCount, m_nSGAuthTypeCount[1], m_nSGAuthTypeCount[2], m_nSGAuthTypeCount[3], 0, 0, 0, nullptr);
        m_nMoneySupply = 0;
    }

    // 服务器组同步 (每 5 秒)
    if (dwServerGroupSync < dwTick)
    {
        SendAccountDBLoginAddServerGroupInfo();
        if (m_bRegisterAuth)
        {
            int nUserCount = 0;
            {
                CFAutoSlimReadLock lock(&m_rwLock);
                nUserCount = static_cast<int>(m_UserInfos.Size());
            }
            m_stServerGroupInfo.nUserCount = nUserCount;
            // 对齐 IDA: 发送服务器组信息到 AccountDB (main=2, sub=0x22)
            XSendDBPacket xSendDBPacket(0, 2, 0x22);
            xSendDBPacket << m_stServerGroupInfo;
            SendDBAccount(xSendDBPacket);
        }
        UpdateServerState();
        dwServerGroupSync = dwTick + 5000;
    }

    // 对齐 IDA: 观察套接字更新
    // 对齐 IDA 0x140072020: OnUpdate(dw64CurrentTick, szIP, nPort, nUserCount, bControlConnect, bCommunityConnect, nMaxThreadCount, bNetCafe)
    int nMaxThreadCount = static_cast<int>(m_xOption.GetLogicThread());
    int nUserCount = static_cast<int>(m_UserInfos.Size());
    int nPort = static_cast<int>(m_xOption.GetPort());
    const char* szIP = m_xOption.GetIP();
    m_scObserveSocket.OnUpdate(dwTick, szIP, nPort, nUserCount, false, false, nMaxThreadCount, false);

    // 对齐 IDA: 世界模式管理器更新
    m_worldModeManager.Update();

    // 对齐 IDA 0x14002D9A0: 日常活动管理器更新 (dw64CurrentTick)
    m_dayEventManager.Update(dwTick);

    // 对齐 IDA 0x14003AC10: 轮盘活动管理器更新 (dw64Tick, nGroupID)
    m_rouletteEventManager.Update(dwTick, m_stServerGroupInfo.wID);

    // 关闭检查
    if (m_bClose)
    {
        m_bRunFlag = false;
    }
}

void XControlServer::UpdateServerState()
{
    // 对齐 IDA 0x140011A70 (XControlServer::UpdateServerState)
    CFAutoSlimReadLock lock(&m_rwServerLock);

    // 发送服务器状态更新给所有游戏服
    SS_UPDATE_SERVER_INFO stUpdateInfo{};
    stUpdateInfo.dwID = 0;

    for (auto& pair : m_mapGameServer) {
        CServer* pServer = pair.second;
        if (pServer) {
            // 判断状态: 如果已接收服务器信息且 PartyManager 已加载 -> state=2, 否则 state=1
            if (pServer->IsRecvServerInfo() && m_partyManager.IsLoad()) {
                stUpdateInfo.nState = 2;
            } else {
                stUpdateInfo.nState = 1;
            }

            XSendPacket xSendPacket(0xF2, 3);
            xSendPacket << stUpdateInfo;
            pServer->SendEx(xSendPacket);
        }
    }

    // 发送给登录服
    if (m_pLoginServer) {
        SS_UPDATE_SERVER_INFO st{};
        st.dwID = 0;
        if (m_pLoginServer->IsRecvServerInfo() && m_partyManager.IsLoad()) {
            st.nState = 2;
        } else {
            st.nState = 1;
        }

        XSendPacket xSendPacket(0xF2, 3);
        xSendPacket << st;
        m_pLoginServer->SendEx(xSendPacket);
    }
}

void XControlServer::UpdateAccountState(DWORD dwUAID, int nState)
{
    // 对齐 IDA 0x14000F8B0 (XControlServer::UpdateAccountState)
    XSendDBPacket xSendDBPacket(0, 2, 0x13);  // main=2, sub=0x13
    xSendDBPacket.XParse << static_cast<int>(dwUAID);
    xSendDBPacket.XParse << nState;
    SendDBAccount(xSendDBPacket);
}

void XControlServer::ClearUserState(DWORD dwServerID)
{
    // 对齐 IDA 0x14000F960 (XControlServer::ClearUserState)
    PS_REQ_CLEAR_USER_STATE stUserState{};

    CFAutoSlimReadLock lock(&m_rwLock);

    // 对齐 IDA: boost::multi_index::get<1>::equal_range(dwServerID)
    // 查找指定 ServerID 的所有用户并收集 UAID
    auto vecUsers = m_UserInfos.FindByServerID(dwServerID);
    for (const auto& pUser : vecUsers) {
        if (pUser) {
            stUserState.vecUserID.push_back(pUser->GetUAID());
        }
    }

    lock.~CFAutoSlimReadLock();

    // 如果有用户，发送清理包到AccountDB (main=2, sub=0x12)
    if (!stUserState.vecUserID.empty())
    {
        XSendDBPacket xSendDBPacket(0, 2, 0x12);
        xSendDBPacket << stUserState;
        SendDBAccount(xSendDBPacket);
    }
}

int XControlServer::CalculateServerUserStaus()
{
    // 对齐 IDA 0x1400151F0 (XControlServer::CalculateServerUserStaus)
    int nCalculateCount = m_nMaxServerUserCount / 6;

    CFAutoSlimReadLock lock(&m_rwLock);

    // 获取登录服用户数
    int nLogin = 0;
    if (GetLoginServer())
    {
        nLogin = GetLoginServer()->GetUserCount();
    }

    // 获取用户总数
    int nServerUserCount = nLogin + static_cast<int>(m_UserInfos.Size());
    lock.~CFAutoSlimReadLock();

    // 计算服务器状态
    if (nServerUserCount > nCalculateCount)
    {
        if (nServerUserCount > 3 * nCalculateCount)
        {
            if (nServerUserCount >= m_nMaxServerUserCount)
                m_stServerGroupInfo.nState = 4;  // 满员
            else
                m_stServerGroupInfo.nState = 3;  // 拥挤
        }
        else
        {
            m_stServerGroupInfo.nState = 2;  // 繁忙
        }
    }
    else
    {
        m_stServerGroupInfo.nState = 1;  // 良好
    }

    return nServerUserCount;
}

bool XControlServer::CheckUserCount()
{
    return m_stServerGroupInfo.nState != 4;
}

void XControlServer::UpdateMaxServerUserCount(int nMaxCount)
{
    m_nMaxServerUserCount = nMaxCount;
}

CServer* XControlServer::GetServer(DWORD dwServerID)
{
    CFAutoSlimReadLock lock(&m_rwServerLock);
    auto it = m_mapGameServer.find(dwServerID);
    if (it != m_mapGameServer.end())
        return it->second;
    return nullptr;
}

// ============================================================================
// 用户地图更新
// ============================================================================

void XControlServer::UpdateUserMap(CServer* pServer, PS_UPDATE_USER_MAP_INFO& stInfo)
{
    // 对齐 IDA 0x14000CCA0 (XControlServer::UpdateUserMap)
    UXMapID uxBeforeMap;
    UXMapID uxNewMap;
    UXMapID uxEmptyMap;

    std::tr1::shared_ptr<CUserObject> pUser = GetUser(stInfo.dwActorID);
    if (!pUser)
    {
        // 用户不存在
        if (stInfo.bLeaveParty)
        {
            if (stInfo.stPartyInfo.byGroupType == 1 && stInfo.stPartyInfo.nID > 0)
            {
                m_partyManager.RemoveMember(stInfo.stPartyInfo.nID, stInfo.dwActorID);
            }
            else if (stInfo.stPartyInfo.byGroupType == 2 && stInfo.stPartyInfo.nID > 0)
            {
                m_forceManager.RemoveMember(stInfo.stPartyInfo.nID, stInfo.dwActorID);
            }
        }

        PS_KICK_USER_INFO psKick{};
        psKick.dwUAID = stInfo.dwUAID;
        psKick.byKickType = 26;
        KickoutUser_UseLock(psKick, true);
        return;
    }

    // 检查 AuthSessionID 匹配
    if (pUser->GetAuthSessionID() != stInfo.biAuthSessionID)
    {
        PS_KICK_USER_INFO psKick{};
        psKick.dwUAID = stInfo.dwUAID;
        psKick.byKickType = 27;

        XSendPacket xSendPacket(0xF3, 7);
        xSendPacket << psKick;
        pServer->SendEx(xSendPacket);
        return;
    }

    // 获取旧地图
    pUser->GetMapIns(&uxBeforeMap);
    uxNewMap = stInfo.uxMapID;

    // 检查地图是否改变
    if (uxBeforeMap != uxNewMap)
    {
        pUser->SetMapIns(uxNewMap);
        m_worldManager.UpdateUserMap(stInfo.dwActorID, uxBeforeMap, uxNewMap);

        // 如果地图高字不同，需要更新服务器引用
        if (SHIWORD(uxBeforeMap.nMapID) != SHIWORD(uxNewMap.nMapID))
        {
            CFAutoSlimWriteLock lock(&m_rwLock);
            // 更新用户的服务器引用
            pUser->SetServer(pServer);
        }

        LogHelper::LogDebug("game.relay", "<UpdateUserMap> User : %d / Map : %d / Channel %d ",
            stInfo.dwActorID,
            SHIWORD(uxNewMap.nMapID),
            SBYTE3(uxNewMap.nMapID));
    }

    // 处理队伍/Force 成员
    if (stInfo.stPartyInfo.byGroupType == 1 && stInfo.stPartyInfo.nID > 0 && !stInfo.bLeaveParty)
    {
        m_partyManager.SetMember(stInfo.stPartyInfo.nID, stInfo.dwActorID, stInfo.uxMapID);
    }
    else if (stInfo.stPartyInfo.byGroupType == 2 && stInfo.stPartyInfo.nID > 0 && !stInfo.bLeaveParty)
    {
        m_forceManager.SetMember(stInfo.stPartyInfo.nID, stInfo.dwActorID, stInfo.uxMapID);
    }
    else if (stInfo.bLeaveParty)
    {
        if (stInfo.stPartyInfo.byGroupType == 1 && stInfo.stPartyInfo.nID > 0)
        {
            m_partyManager.RemoveMember(stInfo.stPartyInfo.nID, stInfo.dwActorID);
        }
        else if (stInfo.stPartyInfo.byGroupType == 2 && stInfo.stPartyInfo.nID > 0)
        {
            m_forceManager.RemoveMember(stInfo.stPartyInfo.nID, stInfo.dwActorID);
        }
    }

    // 处理 BeforeServerID
    int nBeforeServerID = pUser->GetBeforeServerID();
    if (nBeforeServerID > 0)
    {
        CServer* pBeforeServer = GetServer(nBeforeServerID);
        if (pBeforeServer)
        {
            PS_KICK_USER_INFO psKick{};
            psKick.dwUAID = stInfo.dwUAID;
            psKick.byKickType = 23;

            XSendPacket xSendPacket(0xF3, 7);
            xSendPacket << psKick;
            pBeforeServer->SendEx(xSendPacket);
        }
    }

    // 重置世界状态
    pUser->ResetWorldState(uxNewMap);

    // 处理事件迷宫逻辑 (Maze_Type 13 或 19)
    WORD wMapID = SWORD2(uxNewMap.nMapID);
    TB_MAZE_INFO* pTBMazeInfo = m_xResourceMgr.GetTB_MAZE_INFO(wMapID);
    if (pTBMazeInfo)
    {
        if (pTBMazeInfo->Maze_Type == 13 || pTBMazeInfo->Maze_Type == 19)
        {
            // 检查是否已有事件迷宫入口映射
            CFAutoSlimReadLock lockRead(&m_rwServerLock);
            auto it = m_mapEventMazeToEnter.find(pUser->GetCID());
            auto endIt = m_mapEventMazeToEnter.end();

            if (it != endIt)
            {
                // 已有入口，检查地图是否改变
                UXMapID uxOldMapID;
                uxOldMapID.nMapID = it->second;

                if (uxOldMapID != uxNewMap)
                {
                    // 地图改变，更新旧迷宫状态为即将销毁
                    lockRead.~CFAutoSlimReadLock();

                    auto pOldMazeInfo = GetMazeInfo(uxOldMapID);
                    if (pOldMazeInfo)
                    {
                        ULONGLONG dwTime = GetTickCount64() + 60000;  // 60秒后销毁
                        pOldMazeInfo->SetMazeState(3, dwTime);
                        GreenDamTan_log(__FILE__, __FUNCTION__, "EventMaze: set destroy state");
                    }

                    // 更新映射
                    {
                        CFAutoSlimWriteLock lockWrite(&m_rwServerLock);
                        m_mapEventMazeToEnter[pUser->GetCID()] = uxNewMap.nMapID;
                    }

                    LogHelper::LogInfo("game.system", "[REENTER] Update State %I64d %d",
                        uxNewMap.nMapID, SHIWORD(uxNewMap.nMapID));
                }
            }
            else
            {
                // 无入口，插入新映射
                lockRead.~CFAutoSlimReadLock();

                {
                    CFAutoSlimWriteLock lockWrite(&m_rwServerLock);
                    m_mapEventMazeToEnter[pUser->GetCID()] = uxNewMap.nMapID;
                }

                LogHelper::LogInfo("game.system", "[REENTER] Insert State %I64d %d",
                    uxNewMap.nMapID, SHIWORD(uxNewMap.nMapID));
            }
        }
        else
        {
            // 非事件迷宫类型，检查是否需要清理旧映射
            CFAutoSlimReadLock lockRead(&m_rwServerLock);
            auto it = m_mapEventMazeToEnter.find(pUser->GetCID());
            auto endIt = m_mapEventMazeToEnter.end();

            if (it != endIt)
            {
                // 有旧入口，更新迷宫状态并删除映射
                UXMapID uxOldMapID;
                uxOldMapID.nMapID = it->second;

                lockRead.~CFAutoSlimReadLock();

                auto pOldMazeInfo = GetMazeInfo(uxOldMapID);
                if (pOldMazeInfo)
                {
                    ULONGLONG dwTime = GetTickCount64() + 60000;
                    pOldMazeInfo->SetMazeState(3, dwTime);
                    GreenDamTan_log(__FILE__, __FUNCTION__, "EventMaze: cleanup old entry");
                }

                {
                    CFAutoSlimWriteLock lockWrite(&m_rwServerLock);
                    m_mapEventMazeToEnter.erase(pUser->GetCID());
                }

                LogHelper::LogInfo("game.system", "[REENTER] Delete State %I64d %d",
                    uxOldMapID.nMapID, SHIWORD(uxOldMapID.nMapID));
            }
        }
    }
}

// ============================================================================
// 踢出用户
// ============================================================================

void XControlServer::KickoutUser_UseLock(PS_KICK_USER_INFO& stInfo, bool bSend)
{
    CFAutoSlimWriteLock lock(&m_rwLock);
    KickoutUser_NoLock(stInfo, bSend);
}

void XControlServer::KickoutUser_NoLock(PS_KICK_USER_INFO& stInfo, bool bSend)
{
    // 对齐 IDA 0x14000D7E0 (XControlServer::KickoutUser_NoLock)
    // 按 UAID 索引查找用户
    auto pUser = m_UserInfos.FindByUAID(stInfo.dwUAID);
    if (pUser)
    {
        // 按 CID 索引删除
        m_UserInfos.Erase(pUser->GetCID());
    }

    // 对齐 IDA: 发送踢出包到所有服务器 (0xF3, 0x07)
    if (bSend)
    {
        XSendPacket xSendPacket(0xF3, 0x07);
        xSendPacket << stInfo;
        SendPacketAll(xSendPacket, bSend);
    }

    LogHelper::LogDebug("game.relay", "<KICKOUT> UAID : %d, Type : %d", stInfo.dwUAID, stInfo.byKickType);
}

// ============================================================================
// 聊天
// ============================================================================

void XControlServer::SendChatNotice(PS_CHAT_NOTICE& stNotice)
{
    // 对齐 IDA 0x14000FBC0 (XControlServer::SendChatNotice)
    XSendPacket xSendPacket(0xF3, 0x11);
    xSendPacket << stNotice;
    SendPacketAll(xSendPacket, false);
}

void XControlServer::SendChatMegaPhone(PS_CHAT_MEGAPHONE& stMega, PS_CHAT_ITEM_LINK_FOR_SERVER& stLink)
{
    // 对齐 IDA 0x14000FC50 (XControlServer::SendChatMegaPhone)
    XSendPacket xSendPacket(0xF3, 0x17);
    xSendPacket << stMega;
    xSendPacket << stLink;
    SendPacketAll(xSendPacket, false);
}

// ============================================================================
// 迷宫检查
// ============================================================================

void XControlServer::CheckPartyInMaze(CServer* pServer, PS_ENTER_MAP_REQ& stEnter)
{
    // 对齐 IDA 0x14000FD50 (XControlServer::CheckPartyInMaze)
    int nResult = 0;
    UXMapID uxMapID;
    uxMapID.nMapID = 0;

    // 获取队伍迷宫ID (IDA 显示使用 ForceManager::GetMazeID 在 PartyManager 上)
    if (m_partyManager.GetMazeID(stEnter.stPartyInfo.nID, stEnter.dwActorID, &uxMapID))
    {
        // 检查迷宫ID是否有效 (IDA: __s0 <= 0)
        if (uxMapID.nMapID <= 0)
        {
            // 队伍没有迷宫ID，检查 TB_MAZE_INFO
            TB_MAZE_INFO* pTBMazeInfo = m_xResourceMgr.GetTB_MAZE_INFO(stEnter.wMapID);
            if (pTBMazeInfo)
            {
                // 对齐 IDA: 使用 PartyManager::IsParty 和 ForceManager::IsFull
                bool bParty = m_partyManager.IsParty(stEnter.stPartyInfo.nID);
                bool bFull = m_partyManager.IsFull(stEnter.stPartyInfo.nID);
                nResult = CheckAdmissionMember(pTBMazeInfo->Admission_Member, false, bParty, false, bFull);
            }
            else
            {
                nResult = 55022;
                LogHelper::LogError("game.relay",
                    "<PARTY> TB_MAZE_INFO is NULL (PID : %d, M : %d)",
                    stEnter.stPartyInfo.nID, stEnter.wMapID);
            }
        }
        else
        {
            // 获取迷宫信息
            auto pMazeInfo = GetMazeInfo(uxMapID);
            if (!pMazeInfo)
            {
                nResult = 55022;
                LogHelper::LogError("game.relay",
                    "<PARTY> MazeInfo is NULL (PID : %d)",
                    stEnter.stPartyInfo.nID);

                // 清除无效迷宫ID (IDA 显示设置 __s0 = 0)
                UXMapID uxEmpty;
                uxEmpty.nMapID = 0;
                UXMapID uxBeforeMapID;
                m_partyManager.SetMazeID(stEnter.stPartyInfo.nID, uxEmpty, uxBeforeMapID);
            }
            else
            {
                // 对齐 IDA: CMazeInfo::IsValidEnterMaze
                nResult = pMazeInfo->IsValidEnterMaze(stEnter.dwActorID, &uxMapID);
            }
        }
    }
    else
    {
        nResult = 55022;
        LogHelper::LogError("game.relay",
            "<PARTY> GetMazeID Failed (PID : %d)",
            stEnter.stPartyInfo.nID);
    }

    // 日志记录 (对齐 IDA: aCheckMazeD)
    LogHelper::LogDebug("game.relay",
        "<CHECK_MAZE> Party Check (%d, %d, %I64d, %d)",
        stEnter.dwActorID, stEnter.stPartyInfo.nID, uxMapID.nMapID, nResult);

    // 发送响应包 (0xF2, 0x32)
    XSendPacket xSendPacket(0xF2, 0x32);
    xSendPacket.XParse << uxMapID.nMapID;
    xSendPacket << stEnter;
    xSendPacket.XParse << nResult;
    pServer->SendEx(xSendPacket);
}

void XControlServer::CheckForceInMaze(CServer* pServer, PS_ENTER_MAP_REQ& stEnter)
{
    // 对齐 IDA 0x1400100B0 (XControlServer::CheckForceInMaze)
    int nResult = 0;
    UXMapID uxMapID;
    uxMapID.nMapID = 0;

    // 获取 Force 迷宫ID (IDA 显示使用 ForceManager::GetMazeID)
    if (m_forceManager.GetMazeID(stEnter.stPartyInfo.nID, stEnter.dwActorID, &uxMapID))
    {
        // 检查迷宫ID是否有效 (IDA: __s0 <= 0)
        if (uxMapID.nMapID <= 0)
        {
            // Force 没有迷宫ID，检查 TB_MAZE_INFO
            TB_MAZE_INFO* pTBMazeInfo = m_xResourceMgr.GetTB_MAZE_INFO(stEnter.wMapID);
            if (pTBMazeInfo)
            {
                // 对齐 IDA: 使用 PartyManager::IsParty 和 ForceManager::IsFull 在 ForceManager 上
                bool bForce = m_forceManager.IsForce(stEnter.stPartyInfo.nID);
                bool bFull = m_forceManager.IsFull(stEnter.stPartyInfo.nID);
                nResult = CheckAdmissionMember(pTBMazeInfo->Admission_Member, false, false, bForce, bFull);
            }
            else
            {
                nResult = 53144;  // 对齐 IDA: Force 错误码
                LogHelper::LogError("game.relay",
                    "<FORCE> TB_MAZE_INFO is NULL (PID : %d, M : %d)",
                    stEnter.stPartyInfo.nID, stEnter.wMapID);
            }
        }
        else
        {
            // 获取迷宫信息
            auto pMazeInfo = GetMazeInfo(uxMapID);
            if (!pMazeInfo)
            {
                nResult = 53144;  // 对齐 IDA: Force 错误码
                LogHelper::LogError("game.relay",
                    "<FORCE> MazeInfo is NULL (PID : %d)",
                    stEnter.stPartyInfo.nID);

                // 清除无效迷宫ID (IDA 显示设置 __s0 = 0)
                UXMapID uxEmpty;
                uxEmpty.nMapID = 0;
                UXMapID uxBeforeMapID;
                m_forceManager.SetMazeID(stEnter.stPartyInfo.nID, uxEmpty, uxBeforeMapID);
            }
            else
            {
                // 对齐 IDA: CMazeInfo::IsValidEnterMaze
                nResult = pMazeInfo->IsValidEnterMaze(stEnter.dwActorID, &uxMapID);
            }
        }
    }
    else
    {
        nResult = 53144;  // 对齐 IDA: Force 错误码
        LogHelper::LogError("game.relay",
            "<FORCE> GetMazeID Failed (PID : %d)",
            stEnter.stPartyInfo.nID);
    }

    // 日志记录 (对齐 IDA: aCheckMazeD_0)
    LogHelper::LogDebug("game.relay",
        "<CHECK_MAZE> Force Check (%d, %d, %I64d, %d)",
        stEnter.dwActorID, stEnter.stPartyInfo.nID, uxMapID.nMapID, nResult);

    // 发送响应包 (0xF2, 0x32)
    XSendPacket xSendPacket(0xF2, 0x32);
    xSendPacket.XParse << uxMapID.nMapID;
    xSendPacket << stEnter;
    xSendPacket.XParse << nResult;
    pServer->SendEx(xSendPacket);
}

// ============================================================================
// 服务器切换
// ============================================================================

void XControlServer::ChangeServer(CServer* pServer, PS_REQ_CHANGE_SERVER& stChange)
{
    // 对齐 IDA 0x140010410 (XControlServer::ChangeServer)
    PS_RES_CHANGE_SERVER stChangeServerRes{};

    if (stChange.byType)
    {
        // byType != 0: 切换到登录服
        if (m_pLoginServer)
        {
            // 获取登录服 IP/Port
            SS_SERVER_INFO* pServerInfo = m_pLoginServer->GetServerInfo();
            strcpy_s(stChangeServerRes.szIP, pServerInfo->szPublicIP);
            stChangeServerRes.sPort = pServerInfo->sPort;
            stChangeServerRes.byType = stChange.byType;
            stChangeServerRes.bResult = true;
            UpdateAccountState(stChange.dwUAID, 2);
        }
        else
        {
            stChangeServerRes.bResult = false;
        }
    }
    else
    {
        // byType == 0: 切换到认证服
        if (m_xOption.IsUserAuth() && m_bRegisterAuth)
        {
            char szIP[256] = {};
            SHORT sPort = 0;
            if (m_xOption.GetServerPublicIPAndPort("AUTH", szIP, &sPort))
            {
                strcpy_s(stChangeServerRes.szIP, szIP);
                stChangeServerRes.sPort = sPort;
                stChangeServerRes.bResult = true;
                stChangeServerRes.byType = stChange.byType;
                UpdateAccountState(stChange.dwUAID, 1);
            }
            else if (m_pLoginServer)
            {
                // 回退到登录服
                SS_SERVER_INFO* pServerInfo = m_pLoginServer->GetServerInfo();
                strcpy_s(stChangeServerRes.szIP, pServerInfo->szPublicIP);
                stChangeServerRes.sPort = pServerInfo->sPort;
                stChangeServerRes.byType = stChange.byType;
                stChangeServerRes.bResult = true;
                UpdateAccountState(stChange.dwUAID, 1);
            }
            else
            {
                stChangeServerRes.bResult = false;
            }
        }
        else if (m_pLoginServer)
        {
            // 使用登录服
            SS_SERVER_INFO* pServerInfo = m_pLoginServer->GetServerInfo();
            strcpy_s(stChangeServerRes.szIP, pServerInfo->szPublicIP);
            stChangeServerRes.sPort = pServerInfo->sPort;
            stChangeServerRes.byType = stChange.byType;
            stChangeServerRes.bResult = true;
            UpdateAccountState(stChange.dwUAID, 1);
        }
        else
        {
            stChangeServerRes.bResult = false;
        }
    }

    stChangeServerRes.dwActorID = stChange.dwActorID;
    stChangeServerRes.dwUAID = stChange.dwUAID;

    // 发送响应包 (0xF3, 0x12)
    XSendPacket xSendPacket(0xF3, 0x12);
    xSendPacket << stChangeServerRes;
    pServer->SendEx(xSendPacket);

    // 对齐 IDA: 发送 DB 日志 (type=2, subtype=50)
    SendDBLog(stChange.dwUAID, stChange.dwActorID, 2, 50, 0, 0, 0, 0, 0, 0, 0, nullptr);
}

// ============================================================================
// 用户信息设置
// ============================================================================

void XControlServer::SetUsersInfo(CServer* pServer, PS_USERS_INFO& stUsers)
{
    // 对齐 IDA 0x140010740 (XControlServer::SetUsersInfo)
    // 遍历用户列表并添加用户
    for (const auto& userInfo : stUsers.vecUserInfo)
    {
        // 直接使用 stCharInfo
        STCharInfo stInfo = userInfo.stCharInfo;

        UXMapID uxMapID = userInfo.uxMapID;
        DWORD dwIP = userInfo.dwIP;
        BYTE byTradePW = userInfo.byTradePW;
        __int64 biAuthSessionID = userInfo.biAuthSessionID;

        // 添加用户
        AddUser(pServer, stInfo, uxMapID, dwIP, byTradePW, biAuthSessionID, 0);

        // 更新认证类型
        UpdateAuthType(stInfo.dwUAID, userInfo.byAuthType);
    }

    // 如果是最后一批用户
    if (stUsers.bFinish)
    {
        CFAutoSlimReadLock lock(&m_rwLock);
        size_t nUserCount = m_UserInfos.Size();
        lock.~CFAutoSlimReadLock();

        LogHelper::LogInfo("game.relay", "<SYNC> Users Info Finish : %d", nUserCount);

        // 通知服务器用户同步完成
        pServer->RecvUserInfo();
        pServer->SetSyncLoad(E_SERVER_SYNC_LOAD_USER);
        GreenDamTan_log(__FILE__, __FUNCTION__, "SetUsersInfo: finish flag set");
    }

    LogHelper::LogDebug("game.system", "<USERS> Sync Users Member : %d",
        stUsers.vecUserInfo.size());
}

// ============================================================================
// 日志
// ============================================================================

void XControlServer::SendDBLog(int nType, int nSubType, SHORT sMainType, SHORT sSubType,
                               int nParam0, int nParam1, int nParam2, int nParam3,
                               int nParam4, __int64 biParam5, __int64 biParam6, wchar_t* szString)
{
    // 对齐 IDA 0x140010DF0 (XControlServer::SendDBLog)
    ST_LOG_GAME stLog{};

    stLog._nUAID = nType;
    stLog._nUCID = nSubType;
    stLog._sMainType = sMainType;
    stLog._sSubType = sSubType;
    stLog.nParam0 = nParam0;
    stLog.nParam1 = nParam1;
    stLog.nParam2 = nParam2;
    stLog.nParam3 = nParam3;
    stLog.nParam4 = nParam4;
    stLog.nParam5 = biParam5;
    stLog.nParam6 = biParam6;

    if (szString)
    {
        wcscpy_s(stLog.szComment, 51, szString);
    }

    // 发送 LogDB 包 (main=0x42, sub=1)
    XSendDBPacket xSendDBPacket(0, 0x42, 1);
    xSendDBPacket << stLog;
    SendDBLog(xSendDBPacket);
}

void XControlServer::SendDBLog(ST_LOG_GAME& stLog)
{
    // 对齐 IDA 0x140010F50 (XControlServer::SendDBLog ST_LOG_GAME 重载)
    // 发送 LogDB 包 (main=0x42, sub=1) - 使用 SendDBGame 而非 SendDBLog
    XSendDBPacket xSendDBPacket(0, 0x42, 1);
    xSendDBPacket << stLog;
    SendDBGame(xSendDBPacket);
}

// ============================================================================
// 数据库发送方法 (对齐 IDA)
// ============================================================================

bool XControlServer::SendDBAccount(XSendDBPacket& packet)
{
    // 对齐 IDA 0x140011880 (XControlServer::SendDBAccount)
    int nOrderID = packet.GetOrderID();
    int iIndex = nOrderID % m_xDBAgentMgr.GetAccountDBAgentCount();
    if (m_xDBAgentMgr.SendAccountDBAgent(iIndex, packet))
    {
        return true;
    }
    LogHelper::LogError("game.system", "<Send AccountDB> Error Stat!");
    return false;
}

bool XControlServer::SendDBGame(XSendDBPacket& packet)
{
    // 对齐 IDA 0x140011910 (XControlServer::SendDBGame)
    int nOrderID = packet.GetOrderID();
    int iIndex = nOrderID % m_xDBAgentMgr.GetGameDBAgentCount();
    if (m_xDBAgentMgr.SendGameDBAgent(iIndex, packet))
    {
        return true;
    }
    LogHelper::LogError("game.system", "<Send GameDB> Error Stat!");
    return false;
}

bool XControlServer::SendDBLog(XSendDBPacket& packet)
{
    // 对齐 IDA 0x1400119A0 (XControlServer::SendDBLog)
    int nOrderID = packet.GetOrderID();
    int iIndex = nOrderID % m_xDBAgentMgr.GetLogDBAgentCount();
    if (m_xDBAgentMgr.SendLogDBAgent(iIndex, packet))
    {
        return true;
    }
    LogHelper::LogError("game.system", "<Send LogDB> Error Stat!");
    return false;
}

bool XControlServer::SendCommunity(XSendPacket& packet)
{
    // 对齐 IDA 0x140011A30 (XControlServer::SendCommunity)
    return m_pCommunityServer && m_pCommunityServer->SendEx(packet);
}

// ============================================================================
// 迷宫查找方法 (对齐 IDA)
// ============================================================================

CServer* XControlServer::FindServerFromMaze(UXMapID uxMapID)
{
    // 对齐 IDA 0x1400115F0 (XControlServer::FindServerFromMaze)
    CFAutoSlimReadLock lock(&m_rwServerLock);

    for (auto& pair : m_mapMazeServer)
    {
        CServer* pServer = pair.second;
        if (pServer)
        {
            if (pServer->IsMaze(uxMapID))
            {
                return pServer;
            }
        }
    }

    return nullptr;
}

std::tr1::shared_ptr<CMazeInfo> XControlServer::GetMazeInfo(UXMapID uxMazeID)
{
    // 对齐 IDA 0x1400116E0 (XControlServer::GetMazeInfo)
    CFAutoSlimReadLock lock(&m_rwServerLock);

    for (auto& pair : m_mapMazeServer)
    {
        CServer* pServer = pair.second;
        if (pServer)
        {
            auto pMazeInfo = pServer->GetMazeInfo(uxMazeID);
            if (pMazeInfo)
            {
                return pMazeInfo;
            }
        }
    }

    return std::tr1::shared_ptr<CMazeInfo>();
}

// ============================================================================
// GM命令
// ============================================================================

void XControlServer::GM_UserKick(CServer* pServer, ST_GM_USER_KICK_INFO& stInfo)
{
    // 对齐 IDA 0x140010FF0 (XControlServer::GM_UserKick)
    PS_KICK_USER_INFO psKick{};
    psKick.dwUAID = stInfo.dwUAID;
    psKick.byKickType = 6;  // GM踢人类型
    wcscpy_s(psKick.strMsg, stInfo.wszMsg);

    KickoutUser_UseLock(psKick, true);

    // 发送响应给GM (0xF8, 1)
    if (pServer)
    {
        XSendPacket xSendPacket(0xF8, 1);
        xSendPacket.XParse << stInfo.dwNo;
        pServer->SendEx(xSendPacket);
    }
}

void XControlServer::GM_Notice(CServer* pServer, ST_GM_NOTICE_INFO& stInfo)
{
    // 对齐 IDA 0x1400110F0 (XControlServer::GM_Notice)
    PS_CHAT_NOTICE stNotice{};
    stNotice.byType = static_cast<BYTE>(stInfo.shViewType);
    wcscpy_s(stNotice.strColor, stInfo.strColor);
    wcscpy_s(stNotice.strMsg, stInfo.strMsg);
    SendChatNotice(stNotice);

    // 发送响应给 GM
    if (pServer)
    {
        XSendPacket xSendPacket(0xF8, 2);
        xSendPacket.XParse << stInfo.dwNo;
        pServer->SendEx(xSendPacket);
    }
}

void XControlServer::GM_Shutdown(CServer* pServer, DWORD dwTime)
{
    // 对齐 IDA 0x140011200 (XControlServer::GM_Shutdown)
    // 发送关闭包给所有服务器 (排除登录服)
    XSendPacket xSendPacket(0xF2, 8);
    SendPacketAll(xSendPacket, true);

    // 发送响应给 GM
    if (pServer)
    {
        XSendPacket v4(0xF8, 3);
        v4.XParse << static_cast<int>(dwTime);
        pServer->SendEx(v4);
    }
}

void XControlServer::GM_TimeEvent(CServer* pServer, ST_GM_TIME_EVENT_INFO& stInfo)
{
    // 对齐 IDA 0x1400112E0 (XControlServer::GM_TimeEvent)
    // 发送时间事件包给所有服务器 (不排除登录服)
    XSendPacket xSendPacket(0xF2, 9);
    xSendPacket << stInfo;
    SendPacketAll(xSendPacket, false);
}

void XControlServer::GM_ValueEvent(CServer* pServer, PS_GM_VALUE_EVENT_LIST& stList)
{
    // 对齐 IDA 0x140011380 (XControlServer::GM_ValueEvent)
    // 发送值事件包给所有服务器 (不排除登录服) - 包ID: 0xF2, 0x14
    XSendPacket xSendPacket(0xF2, 0x14);
    xSendPacket << stList;
    SendPacketAll(xSendPacket, false);
}

void XControlServer::GM_ServerOption(CServer* pServer, PS_CONTENTS_INFO& stInfo)
{
    // 对齐 IDA 0x140011420 (XControlServer::GM_ServerOption)
    // 先更新本地资源管理器的服务器选项 (遍历 14 个选项)
    for (int i = 0; i < 14; ++i)
    {
        m_xResourceMgr.SetServerContents(static_cast<SERVER_OPTION>(i), stInfo.bContents[i]);
        LogHelper::LogInfo("game.system", "Server Option Info:%d", stInfo.bContents[i]);
    }

    // 发送服务器选项包给所有服务器 (排除登录服) - 包ID: 0xF2, 0x61
    XSendPacket xSendPacket(0xF2, 0x61);
    xSendPacket << stInfo;
    SendPacketAll(xSendPacket, true);
}

void XControlServer::GM_CashShopBanner(CServer* pServer, ST_BANNER_LIST& stList)
{
    // 对齐 IDA 0x140011550 (XControlServer::GM_CashShopBanner)
    // 发送商城横幅包给所有服务器 (排除登录服) - 包ID: 0xF2, 0x15
    XSendPacket xSendPacket(0xF2, 0x15);
    xSendPacket << stList;
    SendPacketAll(xSendPacket, true);
}

// ============================================================================
// 迷宫准入检查
// ============================================================================

int XControlServer::CheckAdmissionMember(int nType, bool bBreakInto, bool bParty, bool bForce, bool bFull)
{
    // 对齐 IDA 0x140015300 (XControlServer::CheckAdmissionMember)
    switch (nType)
    {
    case 1:  // 仅单人
        if (bParty || bForce)
            return 55032;  // 队伍/Force不允许单人入场
        break;
    case 2:  // 仅队伍
        if (!bParty)
            return 55033;  // 必须是队伍
        break;
    case 3:  // 队伍满员
        if (!bParty)
            return 55033;  // 必须是队伍
        if (!bFull && !bBreakInto)
            return 55051;  // 队伍未满员
        break;
    case 4:  // 禁止入场
        return 55052;  // 禁止入场
    case 5:  // Force 满员
        if (!bForce)
            return 53156;  // 必须是 Force
        if (!bFull)
            return 53126;  // Force 未满员
        break;
    }

    return 0;  // 允许入场
}

// ============================================================================
// 认证类型更新
// ============================================================================

bool XControlServer::UpdateAuthType(DWORD dwUAID, BYTE byAuthType)
{
    // 对齐 IDA 0x140015560 (XControlServer::UpdateAuthType)
    if (!byAuthType || byAuthType >= 4)
        return false;

    CFAutoSlimReadLock lock(&m_rwLock);

    // 按 UAID 查找用户
    auto pUser = GetUser(dwUAID);
    if (!pUser)
        return false;

    // 检查认证类型是否相同
    BYTE byOldAuthType = pUser->GetAuthType();

    if (byOldAuthType != byAuthType)
    {
        // 更新计数
        ++m_nSGAuthTypeCount[byAuthType];

        if (byOldAuthType)
        {
            --m_nSGAuthTypeCount[byOldAuthType];
            LogHelper::LogInfo("game.system",
                "[AUTH_TYPE] Wrong Value - Update Authtype ( %d / %d / %d )",
                dwUAID, byOldAuthType, byAuthType);
        }

        pUser->SetAuthType(byAuthType);
    }

    return true;
}

// ============================================================================
// 缓存加载状态
// ============================================================================

void XControlServer::SetCachingLoad(DWORD dwState)
{
    // 对齐 IDA 0x140014D40 (XControlServer::SetCachingLoad)
    if (!(dwState & m_dwCachingLoad))
    {
        m_dwCachingLoad |= dwState;
    }

    // 检查是否需要发送缓存加载完成
    if (dwState == E_SERVER_CACHING_LOAD_DB_COMPLETE)
    {
        // 检查所有必要的状态位
        bool bReady = (m_dwCachingLoad & 0x04) && (m_dwCachingLoad & 0x01) && (m_dwCachingLoad & 0x02);
        if (bReady)
        {
            SendCachingLoad();
        }
    }

    LogHelper::LogInfo("game.system", "<SetCachingLoad> Server:%d, Req:%d",
        m_dwCachingLoad, dwState);
}

void XControlServer::UnSetCachingLoad(DWORD dwState)
{
    // 对齐 IDA 0x140014E00 (XControlServer::UnSetCachingLoad)
    if (dwState & m_dwCachingLoad)
    {
        m_dwCachingLoad ^= dwState;
    }

    LogHelper::LogInfo("game.system", "<UnSetCachingLoad> Server:%d, Req:%d",
        m_dwCachingLoad, dwState);

    SendCachingLoad();
}

void XControlServer::SendCachingLoad()
{
    // 对齐 IDA 0x140014E80 (XControlServer::SendCachingLoad)
    bool bComplete = true;

    CFAutoSlimReadLock lock(&m_rwServerLock);

    // 检查所有游戏服务器是否同步完成
    for (const auto& pair : m_mapGameServer)
    {
        CServer* pServer = pair.second;
        if (pServer)
        {
            if (!pServer->IsSyncLoad(E_SERVER_SYNC_LOAD_USER))
                bComplete = false;
        }
    }

    lock.~CFAutoSlimReadLock();

    if (!m_mapGameServer.empty())
    {
        if (bComplete)
        {
            m_worldModeManager.SendModeSync();
            SetCachingLoad(E_SERVER_CACHING_LOAD_USER);
        }

        // 发送缓存加载状态包 (0xF2, 0x70)
        XSendPacket xSendPacket(0xF2, 0x70);
        xSendPacket.XParse << static_cast<int>(m_dwCachingLoad);
        SendPacketAll(xSendPacket, false);

        LogHelper::LogInfo("game.system", "<SendCachingLoad> CachingState : [%d]",
            m_dwCachingLoad);
    }
}

bool XControlServer::IsAccountDBConnection()
{
    return m_xDBAgentMgr.GetAccountDBAgentCount() > 0;
}

bool XControlServer::IsLogDBConnection()
{
    return m_xDBAgentMgr.GetLogDBAgentCount() > 0;
}

bool XControlServer::IsCompleteCachingLoad()
{
    return m_dwCachingLoad == (E_SERVER_CACHING_LOAD_DB_COMPLETE | E_SERVER_CACHING_LOAD_READY);
}

// ============================================================================
// MyRoom 服务器查找
// ============================================================================

CServer* XControlServer::GetMyRoomServer()
{
    // 对齐 IDA 0x140011E00 (XControlServer::GetMyRoomServer)
    CFAutoSlimReadLock lock(&m_rwServerLock);

    for (auto& pair : m_mapMyRoomServer)
    {
        CServer* pServer = pair.second;
        if (pServer)
        {
            return pServer;
        }
    }

    return nullptr;
}

std::tr1::shared_ptr<CMyRoom> XControlServer::FindMyRoom(DWORD dwUAID)
{
    // 对齐 IDA: 查找用户 MyRoom
    CFAutoSlimReadLock lock(&m_rwServerLock);

    auto it = m_mapMyRoomInfo.find(dwUAID);
    if (it != m_mapMyRoomInfo.end())
    {
        return it->second;
    }

    return std::tr1::shared_ptr<CMyRoom>();
}

// ============================================================================
// 服务器组信息同步
// ============================================================================

void XControlServer::SendAccountDBLoginAddServerGroupInfo()
{
    // 对齐 IDA 0x1400153D0 (XControlServer::SendAccountDBLoginAddServerGroupInfo)
    if (m_bAddLogin && IsAccountDBConnection())
    {
        if (m_xOption.IsUserAuth() && m_pLoginServer)
        {
            // 发送 AccountDB 包 (main=2, sub=0x21)
            // 注意: 服务器级操作无特定客户端连接，sessionID=0 是正确的简化实现
            XSendDBPacket xSendDBPacket(0, 2, 0x21);
            xSendDBPacket << m_stServerGroupInfo;
            SendDBAccount(xSendDBPacket);

            m_bAddLogin = false;

            LogHelper::LogInfo("game.system",
                "[INIT] Send Login Add ServerGroup Info ( %d )",
                m_stServerGroupInfo.wID);
        }
        else
        {
            m_bAddLogin = false;
        }
    }
}

// ============================================================================
// 模式迷宫创建 (对齐 IDA)
// ============================================================================

bool XControlServer::CreateMatchingModeMaze(ST_CREATE_MODE_MAZE& stCreateModeMaze)
{
    // 对齐 IDA 0x140013030 (XControlServer::CreateMatchingModeMaze)
    CServer* pSelectServer = nullptr;

    // 先尝试请求服务器
    CServer* pReqServer = GetServer(stCreateModeMaze.dwMasterServerID);
    if (pReqServer)
    {
        // 检查服务器状态和容量 (阈值 0.7)
        if (pReqServer->GetServerState() == 2 && pReqServer->CanMakeMaze())
        {
            float fMaxUser = pReqServer->GetMaxUserCount() * 0.7f;
            if (fMaxUser > pReqServer->GetUserCount())
            {
                pSelectServer = pReqServer;
            }
        }
    }

    // 如果请求服务器不合适，遍历迷宫服务器找最低负载 (阈值 0.95)
    if (!pSelectServer)
    {
        int nMinUser = 0;
        CFAutoSlimReadLock lock(&m_rwServerLock);

        for (auto& pair : m_mapMazeServer)
        {
            CServer* pCheckServer = pair.second;
            if (pCheckServer && pCheckServer->CanMakeMaze())
            {
                float fMaxUser = pCheckServer->GetMaxUserCount() * 0.95f;
                int nUserCount = pCheckServer->GetUserCount();
                if (nUserCount < fMaxUser && pCheckServer->GetServerState() == 2)
                {
                    if (nUserCount < nMinUser || !pSelectServer)
                    {
                        nMinUser = nUserCount;
                        pSelectServer = pCheckServer;
                    }
                }
            }
        }
    }

    if (pSelectServer)
    {
        // 填充服务器信息
        SS_SERVER_INFO* pServerInfo = pSelectServer->GetServerInfo();
        strcpy_s(stCreateModeMaze.szIP, pServerInfo->szPublicIP);
        stCreateModeMaze.sPort = pServerInfo->sPort;

        // 对齐 IDA: 生成迷宫地图ID
        stCreateModeMaze.uxMapID = pSelectServer->GetMapID(stCreateModeMaze.wReqMapID);
        stCreateModeMaze.dwServerID = pSelectServer->GetServerID();

        // 构建中继结构
        ST_CREATE_MODE_MAZE_FOR_RELAY stSaveCreate;
        stSaveCreate.pReqChannelServer = nullptr;
        stSaveCreate.pReqMazeServer = pSelectServer;
        stSaveCreate.dwTime = GetTickCount64();
        stSaveCreate.vecEnterMember.assign(stCreateModeMaze.vecEnterMember.begin(),
                                           stCreateModeMaze.vecEnterMember.end());

        // 插入请求映射
        m_mapCreateModeMazeReq[stCreateModeMaze.uxMapID] = stSaveCreate;

        // 发送创建请求到迷宫服务器 (0xF2, 0x72)
        XSendPacket xSendPacket(0xF2, 0x72);
        xSendPacket << stCreateModeMaze;
        pSelectServer->SendEx(xSendPacket);

        LogHelper::LogDebug("game.relay", "<CREATE_MODE> Mode Maze Create Request Sent");
        return true;
    }
    else
    {
        LogHelper::LogError("game.relay", "<CREATE_MODE> No server to Create Mode ( %d ).",
                           static_cast<int>(m_mapMazeServer.size()));
        return false;
    }
}

void XControlServer::ResCreateModeMaze(ST_CREATE_MODE_MAZE& stCreateModeMaze)
{
    // 对齐 IDA 0x140013560 (XControlServer::ResCreateModeMaze)
    CFAutoSlimReadLock lock(&m_rwServerLock);

    // 查找创建请求
    auto it = m_mapCreateModeMazeReq.find(stCreateModeMaze.uxMapID);
    if (it == m_mapCreateModeMazeReq.end())
    {
        LogHelper::LogDebug("game.relay", "<RES_CREATE_MODE_MAZE> Request not found");
        return;
    }

    ST_CREATE_MODE_MAZE_FOR_RELAY stInfo = it->second;

    // 对齐 IDA: 创建 CMazeInfo (使用 ClassFactory)
    std::tr1::shared_ptr<CMazeInfo> pMazeInfo = m_factoryMaze.Create();
    if (pMazeInfo)
    {
        // 对齐 IDA: CMazeInfo::Init
        pMazeInfo->Init(&stCreateModeMaze);

        // 对齐 IDA: 添加迷宫到服务器
        if (stInfo.pReqMazeServer)
        {
            stInfo.pReqMazeServer->AddMaze(stCreateModeMaze.uxMapID, pMazeInfo);
        }
    }

    // 对齐 IDA: 通知 CommunityServer (0xF2, 0x4A)
    if (m_pCommunityServer)
    {
        XSendPacket xSendPacket(0xF2, 0x4A);
        xSendPacket << stCreateModeMaze;
        m_pCommunityServer->SendEx(xSendPacket);
    }

    // 删除请求映射
    m_mapCreateModeMazeReq.erase(it);
}

// ============================================================================
// 匹配迷宫创建 (对齐 IDA)
// ============================================================================

bool XControlServer::CreateMatchingMaze(ST_CREATE_MAZE& stCreateMaze, PS_PARTY_INFO& stPartyInfo, DWORD dwMatchingID)
{
    // 对齐 IDA 0x140012460 (XControlServer::CreateMatchingMaze Party版)
    CServer* pSelectServer = nullptr;

    // 获取队伍主人的服务器
    auto pPartyMaster = GetUser(stPartyInfo.dwMaster);
    if (pPartyMaster)
    {
        DWORD dwServerID = pPartyMaster->GetServerID();
        CServer* pReqServer = GetServer(dwServerID);
        if (pReqServer)
        {
            // 检查服务器状态和容量 (阈值 0.7)
            if (pReqServer->GetServerState() == 2 && pReqServer->CanMakeMaze())
            {
                float fMaxUser = pReqServer->GetMaxUserCount() * 0.7f;
                if (fMaxUser > pReqServer->GetUserCount())
                {
                    pSelectServer = pReqServer;
                }
            }
        }
        stCreateMaze.dwUAID = pPartyMaster->GetUAID();
        stCreateMaze.dwUserID = stPartyInfo.dwMaster;
    }

    // 如果请求服务器不合适，遍历迷宫服务器找最低负载 (阈值 0.95)
    if (!pSelectServer)
    {
        int nMinUser = 0;
        CFAutoSlimReadLock lock(&m_rwServerLock);

        for (auto& pair : m_mapMazeServer)
        {
            CServer* pCheckServer = pair.second;
            if (pCheckServer && pCheckServer->CanMakeMaze())
            {
                float fMaxUser = pCheckServer->GetMaxUserCount() * 0.95f;
                int nUserCount = pCheckServer->GetUserCount();
                if (nUserCount < fMaxUser && pCheckServer->GetServerState() == 2)
                {
                    if (nUserCount < nMinUser || !pSelectServer)
                    {
                        nMinUser = nUserCount;
                        pSelectServer = pCheckServer;
                    }
                }
            }
        }
    }

    if (pSelectServer)
    {
        // 填充服务器信息
        SS_SERVER_INFO* pServerInfo = pSelectServer->GetServerInfo();
        strcpy_s(stCreateMaze.szIP, pServerInfo->szPublicIP);
        stCreateMaze.sPort = pServerInfo->sPort;

        // 对齐 IDA: 生成迷宫地图ID
        stCreateMaze.uxMapID = pSelectServer->GetMapID(stCreateMaze.wReqMapID);
        stCreateMaze.dwServerID = pSelectServer->GetServerID();

        // 设置队伍迷宫ID
        stPartyInfo.uxMazeID = stCreateMaze.uxMapID;

        // 构建中继结构
        ST_CREATE_MAZE_FOR_RELAY stSaveCreate;
        stSaveCreate.pReqChannelServer = nullptr;
        stSaveCreate.pReqMazeServer = pSelectServer;
        stSaveCreate.dwTime = GetTickCount64();
        stSaveCreate.stPartyInfo.byGroupType = 1;  // Party
        stSaveCreate.stPartyInfo.nID = stCreateMaze.stPartyInfo.nID;
        stSaveCreate.vecEnterMember.assign(stCreateMaze.vecEnterMember.begin(),
                                           stCreateMaze.vecEnterMember.end());

        // 插入请求映射
        m_mapCreateMazeReq[stCreateMaze.uxMapID] = stSaveCreate;

        // 发送创建请求到迷宫服务器 (0xF2, 0x41)
        XSendPacket xSendPacket(0xF2, 0x41);
        xSendPacket << stCreateMaze;
        xSendPacket << stPartyInfo;
        xSendPacket.XParse << static_cast<int>(dwMatchingID);
        pSelectServer->SendEx(xSendPacket);

        GreenDamTan_log(__FILE__, __FUNCTION__, "CreateMatchingMaze: request sent");
        return true;
    }
    else
    {
        LogHelper::LogError("game.relay", "<CREATE_MATCHING_MAZE> No server to Create Maze ( %d / %d ).",
                           static_cast<int>(m_mapMazeServer.size()),
                           SHIWORD(stCreateMaze.uxMapID.nMapID));
        return false;
    }
}

bool XControlServer::CreateMatchingMaze(ST_CREATE_MAZE& stCreateMaze, PS_FORCE_INFO& stForceInfo, DWORD dwMatchingID)
{
    // 对齐 IDA 0x140012A40 (XControlServer::CreateMatchingMaze Force版)
    CServer* pSelectServer = nullptr;

    // 获取 Force 主人的服务器
    auto pForceMaster = GetUser(stForceInfo.dwMaster);
    if (pForceMaster)
    {
        DWORD dwServerID = pForceMaster->GetServerID();
        CServer* pReqServer = GetServer(dwServerID);
        if (pReqServer)
        {
            if (pReqServer->GetServerState() == 2 && pReqServer->CanMakeMaze())
            {
                float fMaxUser = pReqServer->GetMaxUserCount() * 0.7f;
                if (fMaxUser > pReqServer->GetUserCount())
                {
                    pSelectServer = pReqServer;
                }
            }
        }
        stCreateMaze.dwUAID = pForceMaster->GetUAID();
        stCreateMaze.dwUserID = stForceInfo.dwMaster;
    }

    // 如果请求服务器不合适，遍历迷宫服务器找最低负载 (阈值 0.95)
    if (!pSelectServer)
    {
        int nMinUser = 0;
        CFAutoSlimReadLock lock(&m_rwServerLock);

        for (auto& pair : m_mapMazeServer)
        {
            CServer* pCheckServer = pair.second;
            if (pCheckServer && pCheckServer->CanMakeMaze())
            {
                float fMaxUser = pCheckServer->GetMaxUserCount() * 0.95f;
                int nUserCount = pCheckServer->GetUserCount();
                if (nUserCount < fMaxUser && pCheckServer->GetServerState() == 2)
                {
                    if (nUserCount < nMinUser || !pSelectServer)
                    {
                        nMinUser = nUserCount;
                        pSelectServer = pCheckServer;
                    }
                }
            }
        }
    }

    if (pSelectServer)
    {
        // 填充服务器信息
        SS_SERVER_INFO* pServerInfo = pSelectServer->GetServerInfo();
        strcpy_s(stCreateMaze.szIP, pServerInfo->szPublicIP);
        stCreateMaze.sPort = pServerInfo->sPort;

        // 对齐 IDA: 生成迷宫地图ID
        stCreateMaze.uxMapID = pSelectServer->GetMapID(stCreateMaze.wReqMapID);
        stCreateMaze.dwServerID = pSelectServer->GetServerID();

        // 设置 Force 迷宫ID
        stForceInfo.uxMazeID = stCreateMaze.uxMapID;

        // 构建中继结构
        ST_CREATE_MAZE_FOR_RELAY stSaveCreate;
        stSaveCreate.pReqChannelServer = nullptr;
        stSaveCreate.pReqMazeServer = pSelectServer;
        stSaveCreate.dwTime = GetTickCount64();
        stSaveCreate.stPartyInfo.byGroupType = 2;  // Force
        stSaveCreate.stPartyInfo.nID = stCreateMaze.stPartyInfo.nID;
        stSaveCreate.vecEnterMember.assign(stCreateMaze.vecEnterMember.begin(),
                                           stCreateMaze.vecEnterMember.end());

        // 插入请求映射
        m_mapCreateMazeReq[stCreateMaze.uxMapID] = stSaveCreate;

        // 发送创建请求到迷宫服务器 (0xF2, 0x41)
        XSendPacket xSendPacket(0xF2, 0x41);
        xSendPacket << stCreateMaze;
        xSendPacket << stForceInfo;
        xSendPacket.XParse << static_cast<int>(dwMatchingID);
        pSelectServer->SendEx(xSendPacket);

        LogHelper::LogDebug("game.relay", "<CREATE_MATCHING_MAZE> Force Maze Create Request Sent");
        return true;
    }
    else
    {
        LogHelper::LogError("game.relay", "<CREATE_MATCHING_MAZE> No server to Create Maze ( %d / %d ).",
                           static_cast<int>(m_mapMazeServer.size()),
                           SHIWORD(stCreateMaze.uxMapID.nMapID));
        return false;
    }
}

void XControlServer::ResCreateMatchingMaze(DWORD dwMatchingID, ST_CREATE_MAZE stCreateMaze, PS_PARTY_INFO stPartyInfo)
{
    // 对齐 IDA 0x140013BE0 (XControlServer::ResCreateMatchingMaze Party版)
    CFAutoSlimReadLock lock(&m_rwServerLock);

    // 查找创建请求
    auto it = m_mapCreateMazeReq.find(stCreateMaze.uxMapID);
    if (it == m_mapCreateMazeReq.end())
    {
        GreenDamTan_log(__FILE__, __FUNCTION__, "ResCreateMatchingMaze: request not found");
        return;
    }

    ST_CREATE_MAZE_FOR_RELAY stInfo = it->second;

    // 创建 CMazeInfo
    std::tr1::shared_ptr<CMazeInfo> pMazeInfo = m_factoryMaze.Create();
    if (pMazeInfo)
    {
        pMazeInfo->Init(stCreateMaze);
        GreenDamTan_log(__FILE__, __FUNCTION__, "ResCreateMatchingMaze: CMazeInfo created");

        // 添加迷宫到服务器
        if (stInfo.pReqMazeServer)
        {
            stInfo.pReqMazeServer->AddMaze(stCreateMaze.uxMapID, pMazeInfo);
            GreenDamTan_log(__FILE__, __FUNCTION__, "ResCreateMatchingMaze: AddMaze called");
        }
    }

    // 设置队伍成员迷宫ID
    for (auto& member : stInfo.vecEnterMember)
    {
        m_partyManager.SetMember(stInfo.stPartyInfo.nID, member.dwMember, stCreateMaze.uxMapID);
    }

    // 通知 CommunityServer (0xF2, 0x44)
    if (m_pCommunityServer)
    {
        // 如果是队伍，设置迷宫ID
        if (stCreateMaze.stPartyInfo.byGroupType == 1 && stCreateMaze.stPartyInfo.nID)
        {
            // 注意: IDA 显示调用的是 ForceManager::SetMazeID，但这应该是 PartyManager
            UXMapID uxBeforeMapID;  // 空 UXMapID
            m_partyManager.SetMazeID(stCreateMaze.stPartyInfo.nID, stCreateMaze.uxMapID, uxBeforeMapID);
        }

        PS_PARTY_INFO stForceInfo{};
        XSendPacket xSendPacket(0xF2, 0x44);
        xSendPacket.XParse << static_cast<int>(dwMatchingID);
        xSendPacket << stCreateMaze;
        xSendPacket << stPartyInfo;
        xSendPacket << stForceInfo;
        m_pCommunityServer->SendEx(xSendPacket);
    }

    // 删除请求映射
    m_mapCreateMazeReq.erase(it);
}

void XControlServer::ResCreateMatchingMaze(DWORD dwMatchingID, ST_CREATE_MAZE stCreateMaze, PS_FORCE_INFO stForceInfo)
{
    // 对齐 IDA 0x140013F70 (XControlServer::ResCreateMatchingMaze Force版)
    CFAutoSlimReadLock lock(&m_rwServerLock);

    auto it = m_mapCreateMazeReq.find(stCreateMaze.uxMapID);
    if (it == m_mapCreateMazeReq.end())
    {
        GreenDamTan_log(__FILE__, __FUNCTION__, "ResCreateMatchingMaze (Force): request not found");
        return;
    }

    ST_CREATE_MAZE_FOR_RELAY stInfo = it->second;

    // 创建 CMazeInfo 并初始化 (对齐 IDA)
    std::tr1::shared_ptr<CMazeInfo> pMazeInfo = m_factoryMaze.Create();
    if (pMazeInfo)
    {
        pMazeInfo->Init(stCreateMaze);
        GreenDamTan_log(__FILE__, __FUNCTION__, "ResCreateMatchingMaze (Force): CMazeInfo created");

        // 添加迷宫到服务器 (对齐 IDA: CServer::AddMaze)
        if (stInfo.pReqMazeServer)
        {
            stInfo.pReqMazeServer->AddMaze(stCreateMaze.uxMapID, pMazeInfo);
            GreenDamTan_log(__FILE__, __FUNCTION__, "ResCreateMatchingMaze (Force): AddMaze called");
        }
    }

    // 设置 Force 成员迷宫ID (对齐 IDA)
    for (auto& member : stInfo.vecEnterMember)
    {
        m_forceManager.SetMember(stInfo.stPartyInfo.nID, member.dwMember, stCreateMaze.uxMapID);
    }

    // 通知 CommunityServer (0xF2, 0x44)
    if (m_pCommunityServer)
    {
        // 如果是 Force (byGroupType=2)，设置迷宫ID
        if (stCreateMaze.stPartyInfo.byGroupType == 2 && stCreateMaze.stPartyInfo.nID)
        {
            // 对齐 IDA: CForceManager::SetMazeID 只接收2参数
            m_forceManager.SetMazeID(stCreateMaze.stPartyInfo.nID, stCreateMaze.uxMapID);
        }

        // 包顺序: dwMatchingID, stCreateMaze, stPartyInfo(空), stForceInfo
        PS_PARTY_INFO stPartyInfoOut{};
        XSendPacket xSendPacket(0xF2, 0x44);
        xSendPacket.XParse << static_cast<int>(dwMatchingID);
        xSendPacket << stCreateMaze;
        xSendPacket << stPartyInfoOut;
        xSendPacket << stForceInfo;
        m_pCommunityServer->SendEx(xSendPacket);
    }

    // 删除请求映射 (对齐 IDA)
    m_mapCreateMazeReq.erase(it);
}

// ============================================================================
// 会话检查 (对齐 IDA)
// ============================================================================

void XControlServer::CheckSessionID(CServer* pServer, DWORD dwUAID, __int64 biAuthSessionID)
{
    // 对齐 IDA 0x140014590 (XControlServer::CheckSessionID)
    BYTE byResult = 0;
    PS_KICK_USER_INFO psKick{};

    CFAutoSlimReadLock lock(&m_rwLock);

    // 按 UAID 查找用户
    auto pUser = m_UserInfos.FindByUAID(dwUAID);
    if (pUser)
    {
        __int64 nAuthSessionID = pUser->GetAuthSessionID();
        if (nAuthSessionID <= biAuthSessionID)
        {
            // 检查是否需要踢出
            if (biAuthSessionID > nAuthSessionID)
            {
                psKick.dwUAID = dwUAID;
                psKick.byKickType = 1;
            }
        }
        else
        {
            byResult = 1;  // 会话有效
        }
    }

    lock.~CFAutoSlimReadLock();

    // 如果需要踢出用户
    if (psKick.dwUAID)
    {
        KickoutUser_UseLock(psKick, false);
    }

    // 检查用户计数
    if (!byResult)
    {
        if (!m_xResourceMgr.GetServerContents(E_SERVER_OPTION_WAIT_SYSTEM) && !CheckUserCount())
        {
            byResult = 2;  // 服务器满员
        }
    }

    // 发送响应包 (0xF3, 0x32)
    XSendPacket packet(0xF3, 0x32);
    packet.XParse << dwUAID;
    packet.XParse << byResult;
    pServer->SendEx(packet);
}

// ============================================================================
// MyRoom 创建请求 (对齐 IDA)
// ============================================================================

void XControlServer::ReqCreateMyRoom(int nResult, DWORD dwOwnerUAID, ST_MYROOM_USER& stCreateUser, CServer* pMyRoomServer, DWORD dwOwnerUCID)
{
    // 对齐 IDA 0x1400147E0 (XControlServer::ReqCreateMyRoom)
    PS_ENTER_MAP_RES stEnterMapRes{};
    ST_MYROOM_OWNER_INFO stOwnerInfo{};

    // 获取请求服务器
    CServer* pSendServer = GetServer(stCreateUser.nServerID);
    if (!pSendServer)
    {
        LogHelper::LogError("game.contents", "<REQ_CREATE_MYROOM> Not found Server ( %d, %d )",
                           stCreateUser.nServerID, stCreateUser.dwUCID);
        return;
    }

    // 查找 MyRoom
    auto pMyRoom = FindMyRoom(dwOwnerUAID);
    if (!pMyRoom || nResult)
    {
        // 创建失败
        if (pMyRoomServer)
        {
            pMyRoomServer->DelMyRoom(dwOwnerUAID);
        }
        nResult = 58203;
    }
    else
    {
        // 创建成功
        pMyRoom->CreateSucc();
        stOwnerInfo = pMyRoom->GetOwnerInfo();

        stEnterMapRes.dwUserID = stCreateUser.dwUCID;
        SS_SERVER_INFO* pServerInfo = pMyRoomServer->GetServerInfo();
        strcpy_s(stEnterMapRes.szIP, pServerInfo->szPublicIP);
        stEnterMapRes.sPort = pServerInfo->sPort;
        stEnterMapRes.dwServerID = pMyRoomServer->GetServerID();
        stEnterMapRes.uxMapID = pMyRoom->GetUxMapID();
        stEnterMapRes.nResult = 0;
        stEnterMapRes.byChangeType = 0;
        stEnterMapRes.bChangeServer = (pMyRoomServer != pSendServer);
    }

    // 发送响应包 (0xF2, 0x51)
    XSendPacket xSendPacket(0xF2, 0x51);
    xSendPacket.XParse << nResult;
    xSendPacket << stEnterMapRes;
    xSendPacket << stOwnerInfo;
    xSendPacket.XParse << dwOwnerUCID;
    pSendServer->SendEx(xSendPacket);

    GreenDamTan_log(__FILE__, __FUNCTION__, "ReqCreateMyRoom: response sent");
}

// ============================================================================
// MyRoom 删除请求/响应 (对齐 IDA)
// ============================================================================

void XControlServer::DeleteMyRoomReq(DWORD dwOwnerUAID, UXMapID uxMapID, CServer* pServer)
{
    // 对齐 IDA 0x140014B10 (XControlServer::DeleteMyRoomReq)
    // 查找 MyRoom
    auto pMyRoom = FindMyRoom(dwOwnerUAID);

    if (pMyRoom)
    {
        // 检查状态，只有就绪(1)才能发送删除请求
        if (pMyRoom->GetMyRoomState() != E_MYROOM_STATE_READY)
        {
            LogHelper::LogError("game.contents", "DeleteMyRoomReq: invalid state=%u",
                               pMyRoom->GetMyRoomState());
            return;
        }

        // 调用删除请求
        pMyRoom->DeleteReq();
    }

    // 发送删除请求包 (0xF2, 0x53)
    XSendPacket xSendPacket(0xF2, 0x53);
    xSendPacket.XParse << static_cast<int>(dwOwnerUAID);
    xSendPacket.XParse << uxMapID.nMapID;
    pServer->SendEx(xSendPacket);

    GreenDamTan_log(__FILE__, __FUNCTION__, "DeleteMyRoomReq: sent");
}

void XControlServer::DeleteMyRoomRes(DWORD dwOwnerUAID, UXMapID uxMapID, CServer* pServer)
{
    // 对齐 IDA 0x140014C30 (XControlServer::DeleteMyRoomRes)
    // 查找 MyRoom
    auto pMyRoom = FindMyRoom(dwOwnerUAID);

    if (pMyRoom)
    {
        // 检查状态，只有删除中(2)才能确认删除
        if (pMyRoom->GetMyRoomState() != E_MYROOM_STATE_DELETE)
        {
            LogHelper::LogError("game.contents", "DeleteMyRoomRes: invalid state=%u",
                               pMyRoom->GetMyRoomState());
            return;
        }

        // 从管理器中移除 MyRoom
        {
            CFAutoSlimWriteLock lock(&m_rwServerLock);
            m_mapMyRoomInfo.erase(dwOwnerUAID);
        }
    }

    // 发送删除响应包 (0xF2, 0x59)
    XSendPacket xSendPacket(0xF2, 0x59);
    xSendPacket.XParse << uxMapID.nMapID;
    pServer->SendEx(xSendPacket);

    GreenDamTan_log(__FILE__, __FUNCTION__, "DeleteMyRoomRes: sent");
}

// ============================================================================
// MyRoom 进入请求 (对齐 IDA)
// ============================================================================

bool XControlServer::MyRoomEnterReq(ST_MYROOM_USER stEnterUser, ST_MYROOM_OWNER_INFO stOwnerInfo,
                                     CServer* pSendServer, DWORD dwOwnerUCID)
{
    // 对齐 IDA 0x140012050 (XControlServer::MyRoomEnterReq)
    // 查找 MyRoom
    auto pMyRoom = FindMyRoom(stOwnerInfo.dwUAID);

    if (!pMyRoom)
    {
        // MyRoom 不存在，需要创建
        CServer* pMyRoomServer = GetMyRoomServer();
        if (!pMyRoomServer)
        {
            LogHelper::LogError("game.contents", "<MYROOM_ENTER> Not found MyRoomServer ( UCID : %d, OwnerUAID : %d )",
                               stEnterUser.dwUCID, stOwnerInfo.dwUAID);
            return false;
        }

        // 调用服务器创建 MyRoom (会发送 0xF2, 0x52 包)
        pMyRoomServer->CreateMyRoom(&stOwnerInfo, &stEnterUser, dwOwnerUCID);

        GreenDamTan_log(__FILE__, __FUNCTION__, "MyRoomEnterReq: create new MyRoom");
        return true;
    }
    else
    {
        // MyRoom 已存在，用户进入
        pMyRoom->RoomEnterUser(&stEnterUser, pSendServer, dwOwnerUCID);

        GreenDamTan_log(__FILE__, __FUNCTION__, "MyRoomEnterReq: enter existing MyRoom");
        return true;
    }
}
// ============================================================================
// 迷宫成员进入 (对齐 IDA)
// ============================================================================

bool XControlServer::EnterMemberInMaze(CServer* pServer, DWORD dwPartyID, UXMapID uxMapID, PS_ENTER_MAP_REQ& stEnterMap)
{
    // 对齐 IDA 0x140012200 (XControlServer::EnterMemberInMaze)
    PS_ENTER_MAP_RES stEnterMapRes{};
    stEnterMapRes.nResult = 1;  // 默认失败

    // 查找迷宫服务器
    CServer* pMazeServer = FindServerFromMaze(uxMapID);
    if (pMazeServer)
    {
        // 对齐 IDA: CServer::IsValidEnterPartyMemberInMaze
        pMazeServer->IsValidEnterPartyMemberInMaze(dwPartyID, uxMapID, &stEnterMap, &stEnterMapRes);
    }

    // 发送响应包 (0xF4, 0x08)
    XSendPacket xSendPacket(0xF4, 0x08);
    xSendPacket.XParse << static_cast<int>(dwPartyID);
    xSendPacket << stEnterMapRes;
    pServer->SendEx(xSendPacket);

    GreenDamTan_log(__FILE__, __FUNCTION__, "EnterMemberInMaze: response sent");
    return true;
}

bool XControlServer::EnterMemberInMazeForce(CServer* pServer, DWORD dwForceID, UXMapID uxMapID, PS_ENTER_MAP_REQ& stEnterMap)
{
    // 对齐 IDA 0x140012330 (XControlServer::EnterMemberInMazeForce)
    PS_ENTER_MAP_RES stEnterMapRes{};
    stEnterMapRes.nResult = 1;  // 默认失败

    // 查找迷宫服务器
    CServer* pMazeServer = FindServerFromMaze(uxMapID);
    if (pMazeServer)
    {
        // 对齐 IDA: CServer::IsValidEnterForceMemberInMaze
        pMazeServer->IsValidEnterForceMemberInMaze(dwForceID, uxMapID, &stEnterMap, &stEnterMapRes);
    }

    // 发送响应包 (0xFA, 0x08)
    XSendPacket xSendPacket(0xFA, 0x08);
    xSendPacket.XParse << static_cast<int>(dwForceID);
    xSendPacket << stEnterMapRes;
    pServer->SendEx(xSendPacket);

    GreenDamTan_log(__FILE__, __FUNCTION__, "EnterMemberInMazeForce: response sent");
    return true;
}

// ============================================================================
// 迷宫进入检查 (对齐 IDA)
// ============================================================================

void XControlServer::ReqCheckEnterMaze(CServer* pServer, ST_SERVER_CHECK_ENTER_MAZE& stCheckEnter)
{
    // 对齐 IDA 0x1400137F0 (XControlServer::ReqCheckEnterMaze)
    if (stCheckEnter.nMapID)
    {
        // 有地图ID，检查迷宫状态
        UXMapID uxMapID;
        uxMapID.nMapID = stCheckEnter.nMapID;

        auto pMazeInfo = GetMazeInfo(uxMapID);
        if (pMazeInfo)
        {
            std::uint8_t byRealState = 0;
            std::uint8_t nResult = pMazeInfo->CheckDisconnecUsertState(stCheckEnter.dwUCID, byRealState);
            stCheckEnter.byState = byRealState;
            GreenDamTan_log(__FILE__, __FUNCTION__, "ReqCheckEnterMaze: check maze state");

            // 发送响应包 (0xF2, 0x38)
            XSendPacket xSendPacket(0xF2, 0x38);
            xSendPacket << stCheckEnter;
            pServer->SendEx(xSendPacket);
        }
    }
    else
    {
        // 无地图ID，查找事件迷宫入口映射
        CFAutoSlimReadLock lock(&m_rwServerLock);

        auto it = m_mapEventMazeToEnter.find(stCheckEnter.dwUCID);
        if (it != m_mapEventMazeToEnter.end())
        {
            __int64 nMazeInstance = it->second;
            UXMapID uxMapID;
            uxMapID.nMapID = nMazeInstance;

            auto pMazeInfo = GetMazeInfo(uxMapID);
            if (!pMazeInfo)
            {
                // 迷宫不存在，删除映射
                m_mapEventMazeToEnter.erase(it);
            }
            else
            {
                stCheckEnter.nMapID = uxMapID.nMapID;
                stCheckEnter.byState = 4;

                // 发送响应包 (0xF2, 0x38)
                XSendPacket xSendPacket(0xF2, 0x38);
                xSendPacket << stCheckEnter;
                pServer->SendEx(xSendPacket);
            }
        }
    }
}

void XControlServer::ReqDisconnectUserSync(__int64 nMapInstance, ST_MAZE_WAIT_ENTER_USER_INFO& stDisconnect)
{
    // 对齐 IDA 0x140013AA0 (XControlServer::ReqDisconnectUserSync)
    UXMapID uxMapID;
    uxMapID.nMapID = nMapInstance;

    auto pMazeInfo = GetMazeInfo(uxMapID);
    if (pMazeInfo)
    {
        pMazeInfo->SetDisconnectUserState(stDisconnect);
        GreenDamTan_log(__FILE__, __FUNCTION__, "ReqDisconnectUserSync: set disconnect state");
    }
}

// ============================================================================
// 服务器组ID检查响应 (对齐 IDA)
// ============================================================================

void XControlServer::ResCheckServerGroupID(DWORD dwUAID, int nServerGroupID, int nLastServerGroupID)
{
    // 对齐 IDA 0x140013B20 (XControlServer::ResCheckServerGroupID)
    if (nServerGroupID != nLastServerGroupID)
    {
        LogHelper::LogError("game.system",
            "<ResCheckServerGroupID> Already Login Server GroupID ( UAID:%d, Group:%d, Last:%d )",
            dwUAID, nServerGroupID, nLastServerGroupID);

        // 踢出用户
        PS_KICK_USER_INFO stKick{};
        stKick.dwUAID = dwUAID;
        stKick.byKickType = 28;
        KickoutUser_UseLock(stKick, true);
    }
}

// ============================================================================
// 交易密码状态 (对齐 IDA)
// ============================================================================

void XControlServer::SendUserTradePasswordStateSync(CServer* pReqServer, DWORD dwUCID, std::uint8_t byTradePWState)
{
    // 对齐 IDA 0x1400142E0 (XControlServer::SendUserTradePasswordStateSync)
    auto pUser = GetUser(dwUCID);
    if (pUser)
    {
        pUser->SetTradePWState(byTradePWState);
    }
}

void XControlServer::SendUserTradePasswordState(CServer* pReqServer, DWORD dwUCID)
{
    // 对齐 IDA 0x140014350 (XControlServer::SendUserTradePasswordState)
    auto pUser = GetUser(dwUCID);
    if (pUser)
    {
        std::uint8_t byTradePWState = pUser->GetTradePWState();

        // 发送响应包 (0xF3, 0x27)
        XSendPacket xSendPacket(0xF3, 0x27);
        xSendPacket.XParse << static_cast<int>(dwUCID);
        xSendPacket.XParse << byTradePWState;
        pReqServer->SendEx(xSendPacket);
    }
}

// ============================================================================
// 角色改名 (对齐 IDA)
// ============================================================================

void XControlServer::CharacterNameChange(DWORD dwActorID, wchar_t* pChangeName)
{
    // 对齐 IDA 0x140014440 (XControlServer::CharacterNameChange)
    if (!pChangeName) return;

    CFAutoSlimWriteLock lock(&m_rwLock);

    // 查找用户
    auto pUser = GetUser(dwActorID);
    if (pUser)
    {
        // 先从索引中移除
        m_UserInfos.Erase(dwActorID);

        // 更新名称
        pUser->SetName(pChangeName);

        // 重新插入索引
        m_UserInfos.Insert(pUser);
    }
}

// ============================================================================
// 轮盘事件更新 (对齐 IDA)
// ============================================================================

void XControlServer::UpdateRouletteEventInfo(PS_ROULETTE_EVENT_UPDATE_SERVER& stUpdateInfo)
{
    // 对齐 IDA 0x140015790 (XControlServer::UpdateRouletteEventInfo)
    // 直接调用 CRouletteEventMgr::UpdateRouletteItemInfo
    m_rouletteEventManager.UpdateRouletteItemInfo(stUpdateInfo);
}

// ============================================================================
// 重新进入地图请求 (对齐 IDA)
// ============================================================================

void XControlServer::ReqReEnterMap(CServer* pServer, PS_ENTER_MAP_REQ& stInfo)
{
    // 对齐 IDA 0x1400157C0 (XControlServer::ReqReEnterMap)
    UXMapID uxEnterMap;
    uxEnterMap.nMapID = stInfo.uxMapID.nMapID;

    // 检查是否是事件迷宫 (类型 13 或 19)
    TB_MAZE_INFO* pTBMazeInfo = m_xResourceMgr.GetTB_MAZE_INFO(SHIWORD(uxEnterMap.nMapID));
    if (pTBMazeInfo && (pTBMazeInfo->Maze_Type == 13 || pTBMazeInfo->Maze_Type == 19))
    {
        // 在事件迷宫入口映射中查找
        CFAutoSlimReadLock lock(&m_rwServerLock);

        auto it = m_mapEventMazeToEnter.find(stInfo.dwActorID);
        if (it != m_mapEventMazeToEnter.end())
        {
            __int64 nMazeInstance = it->second;
            UXMapID uxMapID;
            uxMapID.nMapID = static_cast<std::int64_t>(nMazeInstance);

            // 检查是否匹配
            if (uxMapID.nMapID > 0 && uxMapID.nMapID == uxEnterMap.nMapID)
            {
                auto pMazeInfo = GetMazeInfo(uxMapID);
                if (pMazeInfo)
                {
                    PS_ENTER_MAP_RES stEnterMap{};
                    stEnterMap.dwUAID = stInfo.dwUAID;
                    stEnterMap.dwUserID = stInfo.dwActorID;
                    stEnterMap.nPortalID = stInfo.nPortalID;

                    // 对齐 IDA: CMazeInfo::GetMazeInfo 填充迷宫信息
                    pMazeInfo->GetMazeInfo(&stEnterMap);

                    // 发送成功响应 (0xF2, 0x22)
                    XSendPacket xSendPacket(0xF2, 0x22);
                    xSendPacket << stEnterMap;
                    pServer->SendEx(xSendPacket);

                    LogHelper::LogDebug("game.contents", "[REENTER] Success %I64d %d %d",
                        uxMapID.nMapID, SHIWORD(uxMapID.nMapID), stEnterMap.dwUserID);
                    return;
                }
            }
        }
    }

    // 失败响应
    PS_ENTER_MAP_RES stEnterMapRes{};
    stEnterMapRes.nResult = 55066;

    XSendPacket xSendPacket(0xF2, 0x22);
    xSendPacket << stEnterMapRes;
    pServer->SendEx(xSendPacket);

    LogHelper::LogDebug("game.contents", "[REENTER] Failed %I64d %d %d",
        uxEnterMap.nMapID, SHIWORD(uxEnterMap.nMapID), stInfo.dwActorID);
}

// ============================================================================
// 同步事件迷宫 (对齐 IDA)
// ============================================================================

void XControlServer::SyncEventMaze(PS_MAZE_UPDATE_INFO_SYNC& stMazeInfo)
{
    // 对齐 IDA 0x140015B20 (XControlServer::SyncEventMaze)
    UXMapID uxMapID;
    uxMapID.nMapID = stMazeInfo.psMazeInfo.uxMapID.nMapID;

    // 遍历成员列表
    for (const auto& memberInfo : stMazeInfo.psMazeInfo.vecMemberInfo)
    {
        std::uint32_t dwUCID = memberInfo.dwMember;

        // 检查迷宫类型
        TB_MAZE_INFO* pTBMazeInfo = m_xResourceMgr.GetTB_MAZE_INFO(SHIWORD(uxMapID.nMapID));
        if (pTBMazeInfo)
        {
            // 插入事件迷宫入口映射
            CFAutoSlimWriteLock lock(&m_rwServerLock);
            m_mapEventMazeToEnter[dwUCID] = uxMapID.nMapID;
        }
    }
}

// ============================================================================
// WriteLog 虚函数 (对齐 IDA)
// ============================================================================

void XControlServer::WriteLog(const char* szLog, ...)
{
    // 对齐 IDA 0x140015090 (XControlServer::WriteLog)
    char szFormat[512];

    va_list va;
    va_start(va, szLog);
    vsprintf_s(szFormat, sizeof(szFormat), szLog, va);
    va_end(va);

    LogHelper::LogError("game.system", "%s", szFormat);
}

