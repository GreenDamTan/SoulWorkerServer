// WorldManager.cpp
// ControlServer.exe 世界管理器实现 (Stub)

#include "WorldManager.h"
#include "ControlServer.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include <algorithm>

// ============================================================================
// CWorldManager 实现
// ============================================================================

CWorldManager::CWorldManager()
{
    // 对齐 IDA 0x140002710 (CWorldManager::CWorldManager)
    // 成员变量自动初始化
}

CWorldManager::~CWorldManager()
{
    // 对齐 IDA 0x140002760 (CWorldManager::~CWorldManager)
    // 成员析构自动调用
}

void CWorldManager::AddMap(CServer* pServer, PS_CREATE_MAP_LIST& stMapList)
{
    CFAutoSlimWriteLock lock(&m_rwLock);

    for (auto& stMap : stMapList.vecCreateMap)
    {
        // 迷宫地图 (MapID / 10000 == 2)
        // 对齐 IDA: SWORD2(stMap.uxMapID.nMapID) / 10000LL == 2
        // SWORD2 extracts signed short from high word (nMapID >> 16)
        SHORT shMapIDHigh = (SHORT)(stMap.uxMapID.nMapID >> 16);
        if (shMapIDHigh / 10000 == 2)
        {
            // 对齐 IDA 0x1400027A0: 迷宫地图创建
            XControlServer* pControlServer = XControlServer::Instance();
            std::tr1::shared_ptr<CMazeInfo> pMazeInfo = pControlServer->GetMazeFactory().Create();
            if (pMazeInfo)
            {
                pMazeInfo->Init(stMap);
                pServer->AddMaze(stMap.uxMapID, pMazeInfo);
            }
        }
        else
        {
            // 普通地图频道
            // 对齐 IDA: _Keyval = (int)((unsigned __int64)stMap.uxMapID.nMapID >> 16) >> 16
            int nTableID = (int)(stMap.uxMapID.nMapID >> 16);

            auto it = m_mapMapInfo.find(nTableID);
            if (it != m_mapMapInfo.end())
            {
                // 已存在该地图，添加频道
                it->second->AddChannel(pServer, stMap.uxMapID, stMap.nMaxUserCount, stMap.nCurUserCount);
            }
            else
            {
                // 创建新的地图频道管理器
                // 对齐 IDA: CMapWithChannel::CMapWithChannel(v23, (unsigned __int64)(stMap.uxMapID.nMapID << 16) >> 48)
                auto pNewMap = std::tr1::make_shared<CMapWithChannel>(nTableID);
                pNewMap->AddChannel(pServer, stMap.uxMapID, stMap.nMaxUserCount, stMap.nCurUserCount);
                m_mapMapInfo[nTableID] = pNewMap;
            }
        }
    }
}

void CWorldManager::DeleteMap(CServer* pServer)
{
    // 对齐 IDA 0x140002B70 (CWorldManager::DeleteMap)
    CFAutoSlimWriteLock lock(&m_rwLock);

    // 遍历所有地图
    for (auto it = m_mapMapInfo.begin(); it != m_mapMapInfo.end(); )
    {
        auto& pMapInfo = it->second;
        if (!pMapInfo)
        {
            ++it;
            continue;
        }

        // 删除该服务器的所有频道
        pMapInfo->DeleteChannel(pServer);

        // 如果地图频道已空，删除该地图
        if (pMapInfo->GetCount() == 0)
        {
            it = m_mapMapInfo.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

bool CWorldManager::ReqEnterMap(CServer* pServer, PS_ENTER_MAP_REQ& stEnterReq)
{
    // 对齐 IDA 0x140002CA0 (CWorldManager::ReqEnterMap)
    CFAutoSlimReadLock lock(&m_rwLock);

    PS_ENTER_MAP_RES stEnterRes{};
    stEnterRes.dwUserID = stEnterReq.dwActorID;
    stEnterRes.nResult = 50004;  // 默认错误码

    // 获取用户信息
    XControlServer* pControlServer = XControlServer::Instance();
    auto pUserInfo = pControlServer->GetUser(stEnterReq.dwActorID);
    if (!pUserInfo)
    {
        GreenDamTan_log(__FILE__, __FUNCTION__, "NULL == pUserInfo");
        return false;
    }

    // 验证服务器ID
    if (pServer->GetServerID() != pUserInfo->GetServerID())
    {
        GreenDamTan_log(__FILE__, __FUNCTION__, "ServerID mismatch");
        return false;
    }

    // 检查是否可以切换世界
    if (!pUserInfo->CanChangeWorld())
    {
        stEnterRes.nResult = 50023;
        XSendPacket packet(0xF2, 0x31);
        packet << stEnterRes;
        pServer->SendEx(packet);
        return false;
    }

    // Raid模式判断 (MapID == 30031)
    bool bRaid = (stEnterReq.byChangeType != 6 && stEnterReq.wMapID == 30031);
    int nActiveModeID = 0;
    if (bRaid)
    {
        // 对齐 IDA: CWorldModeMgr::IsEnterMode 检查
        ST_ENTER_WORLD_MODE_INFO stEnterInfo{};
        pUserInfo->GetEnterWorldModeInfo(stEnterInfo);
        int nErrorCode = pControlServer->GetWorldModeManager().IsEnterMode(&stEnterInfo, &nActiveModeID);
        if (nErrorCode)
        {
            LogHelper::LogError("game.relay", "<CHANGE_MAP> IsEnterMode - UCID( %d ) Error ( %d )",
                stEnterRes.dwUserID, nErrorCode);
            stEnterRes.nResult = nErrorCode;
            XSendPacket errorPacket(0xF2, 0x31);
            errorPacket << stEnterRes;
            pServer->SendEx(errorPacket);
            return false;
        }
    }

    CServer* pSelectServer = nullptr;

    // 查找目标地图频道
    int nMapID = stEnterReq.wMapID;
    auto it = m_mapMapInfo.find(nMapID);
    if (it != m_mapMapInfo.end())
    {
        // 找到地图，查找频道
        auto pChannel = it->second->FindChannel(
            pServer->GetServerID(), &stEnterReq.stPartyInfo, stEnterReq.dwActorID, bRaid);
        if (pChannel)
        {
            pChannel->AddSelectUser(stEnterReq.dwActorID);
            pSelectServer = pChannel->GetServer();

            // 设置用户世界状态
            UXMapID uxMapID;
            pChannel->GetMapID(&uxMapID);
            ULONGLONG dwWaitTick = GetTickCount64() + 3000;
            pUserInfo->SetWorldState(1, dwWaitTick, uxMapID);

            // 填充响应
            stEnterRes.dwServerID = pSelectServer->GetServerID();
            stEnterRes.nJumpID = stEnterReq.nJumpID;
            stEnterRes.uxMapID = uxMapID;
            stEnterRes.bChangeServer = false;
            stEnterRes.byType = (stEnterReq.wMapID > 20000);
            stEnterRes.nResult = 0;
            stEnterRes.byChangeType = (stEnterReq.byChangeType != 6) ? 4 : stEnterReq.byChangeType;

            // 对齐 IDA: 填充IP和端口
            SS_SERVER_INFO* pInfo = pSelectServer->GetServerInfo();
            if (pInfo)
            {
                strcpy_s(stEnterRes.szIP, pInfo->szPublicIP);
                stEnterRes.sPort = pInfo->sPort;
            }
        }
        else
        {
            if (bRaid)
                stEnterRes.nResult = 55065;
        }
    }
    else if (nMapID / 10000 == 2)
    {
        // 迷宫地图 - 使用逃脱位置
        int nDistrict = 10003;  // 默认逃脱地图
        int nStartBoxID = 1000301;
        // 对齐 IDA: 从 TB_MAZE_INFO 获取 Escape_District_ID 和 Escape_StartBox_ID
        TB_MAZE_INFO* pTBMazeInfo = pControlServer->GetResourceMgr().GetTB_MAZE_INFO(nMapID);
        if (pTBMazeInfo && pTBMazeInfo->Escape_District_ID)
        {
            nDistrict = pTBMazeInfo->Escape_District_ID;
            nStartBoxID = pTBMazeInfo->Escape_StartBox_ID;
        }

        auto itEscape = m_mapMapInfo.find(nDistrict);
        if (itEscape != m_mapMapInfo.end())
        {
            auto pChannel = itEscape->second->FindChannel(
                pServer->GetServerID(), &stEnterReq.stPartyInfo, stEnterReq.dwActorID, false);
            if (pChannel)
            {
                pChannel->AddSelectUser(stEnterReq.dwActorID);
                pSelectServer = pChannel->GetServer();

                stEnterRes.dwServerID = pSelectServer->GetServerID();
                stEnterRes.nJumpID = nStartBoxID;
                pChannel->GetMapID(&stEnterRes.uxMapID);
                stEnterRes.bChangeServer = false;
                stEnterRes.byChangeType = 4;
                stEnterRes.nResult = 0;
            }
        }
        else
        {
            GreenDamTan_log(__FILE__, __FUNCTION__, "Cant Find Escape pos");
        }
    }
    else
    {
        // 默认地图 10003
        int nDefaultMap = 10003;
        auto itDefault = m_mapMapInfo.find(nDefaultMap);
        if (itDefault != m_mapMapInfo.end())
        {
            auto pChannel = itDefault->second->FindChannel(
                pServer->GetServerID(), &stEnterReq.stPartyInfo, stEnterReq.dwActorID, false);
            if (pChannel)
            {
                pChannel->AddSelectUser(stEnterReq.dwActorID);
                pSelectServer = pChannel->GetServer();

                stEnterRes.dwServerID = pSelectServer->GetServerID();
                stEnterRes.nJumpID = 1000301;
                pChannel->GetMapID(&stEnterRes.uxMapID);
                stEnterRes.bChangeServer = false;
                stEnterRes.byChangeType = 4;
                stEnterRes.nResult = 0;
            }
        }
        else
        {
            GreenDamTan_log(__FILE__, __FUNCTION__, "Cant Find Default pos");
        }
    }

    // 如果找到目标服务器，发送通知
    if (pSelectServer)
    {
        // Party/Force 成员设置
        if (stEnterReq.stPartyInfo.byGroupType == 1 && stEnterReq.stPartyInfo.nID > 0)
        {
            // 对齐 IDA: CPartyManager::SetMember
            pControlServer->GetPartyManager().SetMember(
                stEnterReq.stPartyInfo.nID, stEnterRes.dwUserID, stEnterRes.uxMapID);
            pUserInfo->SetPartyInfo(stEnterReq.stPartyInfo);
        }
        else if (stEnterReq.stPartyInfo.byGroupType == 2 && stEnterReq.stPartyInfo.nID > 0)
        {
            // 对齐 IDA: CForceManager::SetMember
            pControlServer->GetForceManager().SetMember(
                stEnterReq.stPartyInfo.nID, stEnterRes.dwUserID, stEnterRes.uxMapID);
            pUserInfo->SetPartyInfo(stEnterReq.stPartyInfo);
        }

        // 发送给目标游戏服
        XSendPacket notifyPacket(0xF3, 0x14);
        notifyPacket.XParse << stEnterReq.dwActorID;
        notifyPacket << stEnterRes.uxMapID;
        notifyPacket << stEnterReq.stPartyInfo;
        pSelectServer->SendEx(notifyPacket);
    }

    // 发送响应给请求服务器
    XSendPacket sendPacket(0xF2, 0x31);
    sendPacket << stEnterRes;
    pServer->SendEx(sendPacket);

    // Raid DB日志
    if (stEnterRes.nResult == 0 && bRaid)
    {
        // 对齐 IDA: 发送 Raid DB 包 (main=0x49, sub=8)
        ST_WORLD_MODE stInfo{};
        stInfo.nModeDateID = nActiveModeID;
        stInfo.biEnterDate = GetTickCount64();

        XSendDBPacket xSendDBPacket(pServer, 0x49, 8);
        xSendDBPacket.XParse << stEnterRes.dwUserID;
        xSendDBPacket << stInfo;
        pControlServer->SendDBGame(xSendDBPacket);
    }

    return true;
}

bool CWorldManager::ReqEnterMapToOther(CServer* pServer, PS_ENTER_MAP_REQ& stEnterReq,
                                        STPosInfo stPosInfo, DWORD dwTargetID)
{
    // 对齐 IDA 0x140003D30 (CWorldManager::ReqEnterMapToOther)
    CFAutoSlimReadLock lock(&m_rwLock);

    PS_ENTER_MAP_RES stEnterRes{};
    stEnterRes.dwUserID = stEnterReq.dwActorID;
    stEnterRes.nResult = 50004;
    stEnterRes.stPosInfo.vPos = stEnterReq.vNextPos;

    // 获取用户信息
    XControlServer* pControlServer = XControlServer::Instance();
    auto pUserInfo = pControlServer->GetUser(stEnterRes.dwUserID);
    if (!pUserInfo)
    {
        return false;
    }

    CServer* pSelectServer = nullptr;
    int nMapID = stEnterReq.wMapID;

    // 查找目标地图
    auto it = m_mapMapInfo.find(nMapID);
    if (it != m_mapMapInfo.end())
    {
        auto& pMapInfo = it->second;
        if (pMapInfo)
        {
            // 提取频道号: SBYTE3(stPosInfo.uxMapID.nMapID)
            int nChannel = (signed char)((stPosInfo.uxMapID.nMapID >> 24) & 0xFF);
            auto pChannel = pMapInfo->FindChannel(nChannel);

            if (pChannel)
            {
                pChannel->AddSelectUser(stEnterReq.dwActorID);
                pSelectServer = pChannel->GetServer();

                stEnterRes.dwServerID = pSelectServer->GetServerID();
                stEnterRes.nJumpID = stEnterReq.nJumpID;
                pChannel->GetMapID(&stEnterRes.uxMapID);
                stEnterRes.bChangeServer = false;
                stEnterRes.byType = (stEnterReq.wMapID > 20000);
                stEnterRes.nResult = 0;
                stEnterRes.byChangeType = (stEnterReq.byChangeType != 6) ? 4 : stEnterReq.byChangeType;

                // 填充IP和端口
                SS_SERVER_INFO* pInfo = pSelectServer->GetServerInfo();
                if (pInfo)
                {
                    strcpy_s(stEnterRes.szIP, pInfo->szPublicIP);
                    stEnterRes.sPort = pInfo->sPort;
                }
            }
        }
    }
    else if (nMapID / 10000 == 2)
    {
        // 迷宫地图 - 使用逃脱位置
        int nDistrict = 10003;
        int nStartBoxID = 1000301;

        // 从 TB_MAZE_INFO 获取逃脱位置
        TB_MAZE_INFO* pTBMazeInfo = pControlServer->GetResourceMgr().GetTB_MAZE_INFO(nMapID);
        if (pTBMazeInfo && pTBMazeInfo->Escape_District_ID)
        {
            nDistrict = pTBMazeInfo->Escape_District_ID;
            nStartBoxID = pTBMazeInfo->Escape_StartBox_ID;
        }

        auto itEscape = m_mapMapInfo.find(nDistrict);
        if (itEscape != m_mapMapInfo.end())
        {
            pSelectServer = pServer;
            stEnterRes.dwServerID = pServer->GetServerID();
            stEnterRes.nJumpID = stEnterReq.nJumpID;
            stEnterRes.uxMapID = stPosInfo.uxMapID;
            stEnterRes.bChangeServer = false;
            stEnterRes.byChangeType = 4;
            stEnterRes.nResult = 0;

            SS_SERVER_INFO* pInfo = pSelectServer->GetServerInfo();
            if (pInfo)
            {
                strcpy_s(stEnterRes.szIP, pInfo->szPublicIP);
                stEnterRes.sPort = pInfo->sPort;
            }
        }
        else
        {
            GreenDamTan_log(__FILE__, __FUNCTION__, "<FIND_MAP> Cant Find Escape pos");
        }
    }
    else
    {
        // 默认地图 10003
        int nDefaultMap = 10003;
        int nDefaultJumpID = 1000301;

        auto itDefault = m_mapMapInfo.find(nDefaultMap);
        if (itDefault != m_mapMapInfo.end())
        {
            auto& pMapInfo = itDefault->second;
            if (pMapInfo)
            {
                auto pChannel = pMapInfo->FindChannel(
                    pServer->GetServerID(), &stEnterReq.stPartyInfo, stEnterReq.dwActorID, false);

                if (pChannel)
                {
                    pChannel->AddSelectUser(stEnterReq.dwActorID);
                    pSelectServer = pChannel->GetServer();

                    stEnterRes.dwServerID = pSelectServer->GetServerID();
                    stEnterRes.nJumpID = nDefaultJumpID;
                    pChannel->GetMapID(&stEnterRes.uxMapID);
                    stEnterRes.bChangeServer = false;
                    stEnterRes.byChangeType = 4;
                    stEnterRes.nResult = 0;

                    SS_SERVER_INFO* pInfo = pSelectServer->GetServerInfo();
                    if (pInfo)
                    {
                        strcpy_s(stEnterRes.szIP, pInfo->szPublicIP);
                        stEnterRes.sPort = pInfo->sPort;
                    }
                }
            }
        }
        else
        {
            GreenDamTan_log(__FILE__, __FUNCTION__, "<FIND_MAP> Cant Find Default pos");
        }
    }

    // 如果找到目标服务器，发送通知
    if (pSelectServer)
    {
        // Party/Force 成员设置
        if (stEnterReq.stPartyInfo.byGroupType == 1 && stEnterReq.stPartyInfo.nID > 0)
        {
            pControlServer->GetPartyManager().SetMember(
                stEnterReq.stPartyInfo.nID, stEnterRes.dwUserID, stEnterRes.uxMapID);
            pUserInfo->SetPartyInfo(stEnterReq.stPartyInfo);
        }
        else if (stEnterReq.stPartyInfo.byGroupType == 2 && stEnterReq.stPartyInfo.nID > 0)
        {
            pControlServer->GetForceManager().SetMember(
                stEnterReq.stPartyInfo.nID, stEnterRes.dwUserID, stEnterRes.uxMapID);
            pUserInfo->SetPartyInfo(stEnterReq.stPartyInfo);
        }

        // 发送给目标游戏服
        XSendPacket notifyPacket(0xF3, 0x14);
        notifyPacket.XParse << stEnterReq.dwActorID;
        notifyPacket << stEnterRes.uxMapID;
        notifyPacket << stEnterReq.stPartyInfo;
        pSelectServer->SendEx(notifyPacket);
    }

    // 发送响应给请求服务器
    XSendPacket sendPacket(0xF2, 0x54);
    sendPacket << stEnterRes;
    sendPacket << stPosInfo;
    sendPacket.XParse << (int)dwTargetID;
    pServer->SendEx(sendPacket);

    return true;
}

void CWorldManager::ReqChangeChannel(CServer* pServer, PS_ENTER_MAP_REQ& stEnterReq)
{
    // 对齐 IDA 0x140004720 (CWorldManager::ReqChangeChannel)
    XControlServer* pControlServer = XControlServer::Instance();
    auto pUser = pControlServer->GetUser(stEnterReq.dwActorID);

    if (!pUser)
    {
        GreenDamTan_log(__FILE__, __FUNCTION__, "[CHANGE_CHANNEL] pUser == NULL");
        return;
    }

    // 验证服务器ID
    int nUserServerID = pUser->GetServerID();
    int nServerID = pServer->GetServerID();
    if (nUserServerID != nServerID)
    {
        GreenDamTan_log(__FILE__, __FUNCTION__, "[CHANGE_CHANNEL] ServerID mismatch");
        return;
    }

    // 检查是否可以切换世界
    if (!pUser->CanChangeWorld())
    {
        PS_ENTER_MAP_RES st{};
        st.dwUserID = stEnterReq.dwActorID;
        st.nResult = 50023;

        XSendPacket packet(0xF2, 0x10);
        packet << st;
        pServer->SendEx(packet);
        return;
    }

    CFAutoSlimReadLock lock(&m_rwLock);

    PS_ENTER_MAP_RES stEnterRes{};
    stEnterRes.dwUserID = stEnterReq.dwActorID;
    stEnterRes.nResult = 0;

    // 查找目标地图
    int nMapID = stEnterReq.wMapID;
    auto it = m_mapMapInfo.find(nMapID);

    if (it != m_mapMapInfo.end())
    {
        auto& pMapInfo = it->second;
        if (pMapInfo)
        {
            // 查找指定频道
            int nChannel = stEnterReq.wChannel;
            auto pChannel = pMapInfo->FindChannel(nChannel);

            if (!pChannel)
            {
                stEnterRes.nResult = 50004;
                GreenDamTan_log(__FILE__, __FUNCTION__, "<CHANGE_CHANNEL> Failed Find Channel");
            }
            else
            {
                // 检查频道状态
                if (!pChannel->IsOKToEnter() || pChannel->GetServerState() != 2)
                {
                    stEnterRes.nResult = 51008;
                    GreenDamTan_log(__FILE__, __FUNCTION__, "<CHANGE_CHANNEL> Channel not OK");

                    XSendPacket failPacket(0xF2, 0x10);
                    failPacket << stEnterRes;
                    pServer->SendEx(failPacket);
                    return;
                }

                // 设置用户世界状态
                int nChannelServerID = pChannel->GetServer()->GetServerID();
                if (nUserServerID != nChannelServerID)
                {
                    pUser->SetBeforeServerID(nUserServerID);
                }

                UXMapID uxMapID;
                pChannel->GetMapID(&uxMapID);
                ULONGLONG dwWaitTick = GetTickCount64() + 3000;
                pUser->SetWorldState(1, dwWaitTick, uxMapID);

                pChannel->AddSelectUser(stEnterReq.dwActorID);

                stEnterRes.dwServerID = nChannelServerID;
                stEnterRes.nJumpID = stEnterReq.nJumpID;
                stEnterRes.uxMapID = uxMapID;
                stEnterRes.bChangeServer = false;
                stEnterRes.byType = (stEnterReq.wMapID > 20000);

                // 填充IP和端口
                SS_SERVER_INFO* pInfo = pChannel->GetServer()->GetServerInfo();
                if (pInfo)
                {
                    strcpy_s(stEnterRes.szIP, pInfo->szPublicIP);
                    stEnterRes.sPort = pInfo->sPort;
                }

                // Party/Force 成员设置
                if (stEnterReq.byChangeType == 1 && stEnterReq.stPartyInfo.nID > 0)
                {
                    pControlServer->GetPartyManager().SetMember(
                        stEnterReq.stPartyInfo.nID, stEnterRes.dwUserID, stEnterRes.uxMapID);
                }
                else if (stEnterReq.byChangeType == 2 && stEnterReq.stPartyInfo.nID > 0)
                {
                    pControlServer->GetForceManager().SetMember(
                        stEnterReq.stPartyInfo.nID, stEnterRes.dwUserID, stEnterRes.uxMapID);
                }
            }
        }
    }
    else
    {
        stEnterRes.nResult = 50004;
        GreenDamTan_log(__FILE__, __FUNCTION__, "<CHANGE_CHANNEL> Failed Find Map");
    }

    // 发送响应
    XSendPacket packet(0xF2, 0x10);
    packet << stEnterRes;
    pServer->SendEx(packet);
}

void CWorldManager::SendChannelInfoAll()
{
    // 对齐 IDA 0x140004F70 (CWorldManager::SendChannelInfoAll)
    CFAutoSlimReadLock lock(&m_rwLock);

    for (auto& pair : m_mapMapInfo)
    {
        auto& pMap = pair.second;
        if (!pMap)
            continue;

        PS_CHANNEL_INFO stChannel;
        pMap->GetChannel(stChannel);

        XSendPacket packet(0xF2, 0x11);
        packet << stChannel;
        XControlServer::Instance()->SendPacketAll(packet, false);
    }
}

void CWorldManager::UpdateUserMap(int dwActorID, UXMapID uxOldMapID, UXMapID uxNewMapID)
{
    // 对齐 IDA 0x1400050E0 (CWorldManager::UpdateUserMap)

    // 从旧地图频道减少用户计数
    auto itBefore = m_mapChannelOfMap.find(uxOldMapID);
    if (itBefore != m_mapChannelOfMap.end())
    {
        auto& pChannelBefore = itBefore->second;
        if (pChannelBefore)
        {
            pChannelBefore->AddUserCount(dwActorID, -1);
        }
    }

    // 向新地图频道增加用户计数
    auto itAfter = m_mapChannelOfMap.find(uxNewMapID);
    if (itAfter != m_mapChannelOfMap.end())
    {
        auto& pChannelAfter = itAfter->second;
        if (pChannelAfter)
        {
            pChannelAfter->AddUserCount(dwActorID, 1);
        }
    }
}

void CWorldManager::AddChannelOfMap(UXMapID uxMapID, std::tr1::shared_ptr<CChannelOfMap> pChannel)
{
    // 对齐 IDA 0x140005230: 先检查是否存在，不存在才插入
    CFAutoSlimWriteLock lock(&m_rwLock);
    auto it = m_mapChannelOfMap.find(uxMapID);
    if (it == m_mapChannelOfMap.end())
    {
        m_mapChannelOfMap.insert(std::make_pair(uxMapID, pChannel));
    }
}

void CWorldManager::RemoveChannelOfMap(UXMapID uxMapID)
{
    // 对齐 IDA 0x140005300: 先查找，存在才删除
    CFAutoSlimWriteLock lock(&m_rwLock);
    auto it = m_mapChannelOfMap.find(uxMapID);
    if (it != m_mapChannelOfMap.end())
    {
        m_mapChannelOfMap.erase(it);
    }
}

// ============================================================================
// CMapWithChannel 实现
// ============================================================================

CMapWithChannel::CMapWithChannel(WORD wMapID)
    : m_nTableID(wMapID)
    , m_nMaxChannelCount(999)
    , m_nChannel_Index(0)
    , m_nStartChannel(999)
{
}

CMapWithChannel::~CMapWithChannel()
{
    for (auto& pair : m_mapChannelForServer)
    {
        delete pair.second;
    }
    m_mapChannelForServer.clear();
    m_mapChannel.clear();
}

bool CMapWithChannel::AddChannel(CServer* pServer, UXMapID uxMapID, int nMaxUserCount, int nCurUserCount)
{
    // 对齐 IDA 0x1400018B0 (CMapWithChannel::AddChannel)
    // 提取频道号: SBYTE3(uxMapID.nMapID) = (uxMapID.nMapID >> 24) & 0xFF as signed byte
    int nChannel = (signed char)((uxMapID.nMapID >> 24) & 0xFF);

    // 检查频道是否已存在
    auto it = m_mapChannel.find(nChannel);
    if (it != m_mapChannel.end())
    {
        GreenDamTan_log(__FILE__, __FUNCTION__, "<ADD_MAP> Failed Add Map - channel exists");
        SendServerAddChannelMap(pServer, uxMapID, false);
        return false;
    }

    // 创建新的频道对象
    auto pChannel = std::tr1::make_shared<CChannelOfMap>(pServer, uxMapID, nMaxUserCount, nCurUserCount);

    // 插入到频道映射
    m_mapChannel[nChannel] = pChannel;

    // 日志
    GreenDamTan_log(__FILE__, __FUNCTION__, "<ADD_CHANNEL> channel added");

    // 获取服务器ID并添加到服务器映射
    DWORD dwServerID = pServer->GetServerID();
    auto itServer = m_mapChannelForServer.find((int)dwServerID);
    if (itServer == m_mapChannelForServer.end())
    {
        // 创建新的服务器频道信息
        ST_CHANNEL_MAP_INFO* pInfo = new ST_CHANNEL_MAP_INFO();
        pInfo->nCount = 0;
        pInfo->mapUXMapID[0] = uxMapID;
        m_mapChannelForServer[(int)dwServerID] = pInfo;
    }
    else
    {
        // 添加到已有服务器信息
        int nIndex = (int)itServer->second->mapUXMapID.size();
        itServer->second->mapUXMapID[nIndex] = uxMapID;
    }

    // 更新起始频道
    if (m_nStartChannel >= nChannel)
    {
        m_nStartChannel = nChannel;
    }
    m_nChannel_Index = m_nStartChannel;

    // 添加到 WorldManager
    XControlServer::Instance()->GetWorldManager().AddChannelOfMap(uxMapID, pChannel);

    // 发送 DB 和服务器响应
    SendDBAddChannelMap(pChannel);
    SendServerAddChannelMap(pServer, uxMapID, true);

    return true;
}

void CMapWithChannel::DeleteChannel(CServer* pServer)
{
    // 对齐 IDA 0x1400020A0 (CMapWithChannel::DeleteChannel)
    // 遍历所有频道，删除属于该服务器的频道
    for (auto it = m_mapChannel.begin(); it != m_mapChannel.end(); )
    {
        auto& pChannel = it->second;
        if (pChannel && pChannel->GetServer() == pServer)
        {
            // 从 WorldManager 移除
            UXMapID uxMapID;
            pChannel->GetMapID(&uxMapID);
            XControlServer::Instance()->GetWorldManager().RemoveChannelOfMap(uxMapID);

            // 从映射中删除
            it = m_mapChannel.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // 从服务器映射中删除
    int nServerID = (int)pServer->GetServerID();
    auto itServer = m_mapChannelForServer.find(nServerID);
    if (itServer != m_mapChannelForServer.end())
    {
        ST_CHANNEL_MAP_INFO* pInfo = itServer->second;
        if (pInfo)
        {
            delete pInfo;
        }
        m_mapChannelForServer.erase(itServer);
    }

    // 如果是系统类型2，发送DB包
    XOption& pOption = XControlServer::Instance()->GetOption();
    if (pOption.GetSystemType() == SYSTEM_TYPE_TEST)
    {
        XSendDBPacket dbPacket(pServer, 0x43, 1);
        dbPacket.XParse << (int)pServer->GetServerID();
        XControlServer::Instance()->SendDBGame(dbPacket);
    }
}

std::tr1::shared_ptr<CChannelOfMap> CMapWithChannel::FindChannel(
    DWORD dwServerID, ST_PARTY_INFO* stPartyInfo, DWORD dwActorID, bool bRaid)
{
    // 对齐 IDA 0x140001E10 (CMapWithChannel::FindChannel)
    // 优先级: Party > ServerID > UserCount

    // 1. 如果有Party/Force信息，先按Party查找
    if (stPartyInfo && stPartyInfo->byGroupType != 0 && stPartyInfo->nID > 0)
    {
        auto pChannel = FindChannelByParty(stPartyInfo, dwActorID, bRaid);
        if (pChannel)
        {
            return pChannel;
        }
    }

    // 2. 按服务器ID查找
    auto pChannel = FindChannelByServerID(dwServerID, dwActorID, bRaid);
    if (pChannel)
    {
        return pChannel;
    }

    // 3. 按用户数查找
    return FindChannelByUserCount(dwActorID, bRaid);
}

std::tr1::shared_ptr<CChannelOfMap> CMapWithChannel::FindChannel(int nChannel)
{
    auto it = m_mapChannel.find(nChannel);
    if (it != m_mapChannel.end())
        return it->second;
    return std::tr1::shared_ptr<CChannelOfMap>();
}

std::tr1::shared_ptr<CChannelOfMap> CMapWithChannel::FindChannelByParty(
    ST_PARTY_INFO* stPartyInfo, DWORD dwActorID, bool bRaid)
{
    // 对齐 IDA 0x140005380 (CMapWithChannel::FindChannelByParty)
    if (!stPartyInfo || stPartyInfo->byGroupType == 0 || stPartyInfo->nID <= 0)
    {
        return std::tr1::shared_ptr<CChannelOfMap>();
    }

    // 获取Party/Force成员所在的MapID
    UXMapID uxPartyMapID;
    uxPartyMapID.nMapID = 0;

    XControlServer* pControlServer = XControlServer::Instance();
    if (stPartyInfo->byGroupType == 1)
    {
        // Party
        uxPartyMapID = pControlServer->GetPartyManager().FindSamePlace(
            stPartyInfo->nID, dwActorID, m_nTableID);
    }
    else if (stPartyInfo->byGroupType == 2)
    {
        // Force
        uxPartyMapID = pControlServer->GetForceManager().FindSamePlace(
            stPartyInfo->nID, dwActorID, m_nTableID);
    }

    // 检查MapID是否匹配当前地图
    WORD wPartyMapID = (WORD)((uxPartyMapID.nMapID >> 16) & 0xFFFF);
    if (m_nTableID != wPartyMapID)
    {
        return std::tr1::shared_ptr<CChannelOfMap>();
    }

    // 遍历频道查找匹配的
    for (auto& pair : m_mapChannel)
    {
        auto& pChannel = pair.second;
        if (!pChannel)
            continue;

        // 检查服务器状态 (状态2表示良好)
        if (pChannel->GetServerState() == 2 && uxPartyMapID.nMapID > 0)
        {
            UXMapID channelMapID;
            pChannel->GetMapID(&channelMapID);

            if (channelMapID.nMapID == uxPartyMapID.nMapID)
            {
                // Raid模式下检查用户数限制
                if (!bRaid || pChannel->GetTotalCount() < pChannel->GetMaxUserCount())
                {
                    return pChannel;
                }
            }
        }
    }

    return std::tr1::shared_ptr<CChannelOfMap>();
}

std::tr1::shared_ptr<CChannelOfMap> CMapWithChannel::FindChannelByServerID(
    DWORD dwServerID, DWORD dwActorID, bool bRaid)
{
    // 对齐 IDA 0x140005630 (CMapWithChannel::FindChannelByServerID)
    int nKey = (int)dwServerID;
    auto itServer = m_mapChannelForServer.find(nKey);
    if (itServer == m_mapChannelForServer.end())
    {
        return std::tr1::shared_ptr<CChannelOfMap>();
    }

    ST_CHANNEL_MAP_INFO* pInfo = itServer->second;
    if (!pInfo)
    {
        return std::tr1::shared_ptr<CChannelOfMap>();
    }

    // 检查索引范围
    int nCount = pInfo->nCount;
    if (nCount >= (int)pInfo->mapUXMapID.size())
    {
        pInfo->nCount = 0;
    }

    // 查找当前索引对应的 UXMapID
    auto it = pInfo->mapUXMapID.find(pInfo->nCount);
    if (it == pInfo->mapUXMapID.end())
    {
        return std::tr1::shared_ptr<CChannelOfMap>();
    }

    // 提取频道号: SBYTE3(uxMapID.nMapID) = (nMapID >> 24) & 0xFF
    int nChannel = (signed char)((it->second.nMapID >> 24) & 0xFF);

    // 调用 CheckChannelUserCount 检查用户数
    auto pChannel = CheckChannelUserCount(nChannel, pInfo, bRaid);

    if (!pChannel)
    {
        GreenDamTan_log(__FILE__, __FUNCTION__, "<CHANNEL> Failed Find Map 1");
    }

    return pChannel;
}

std::tr1::shared_ptr<CChannelOfMap> CMapWithChannel::FindChannelByUserCount(
    DWORD dwActorID, bool bRaid)
{
    // 对齐 IDA 0x140005820 (CMapWithChannel::FindChannelByUserCount)
    auto pFindChannel = FindChannelUserLow(bRaid);

    if (!pFindChannel)
    {
        GreenDamTan_log(__FILE__, __FUNCTION__, "<CHANNEL> Failed FindChannelByUserCount");
    }

    return pFindChannel;
}

std::tr1::shared_ptr<CChannelOfMap> CMapWithChannel::CheckChannelUserCount(
    int nChannel, ST_CHANNEL_MAP_INFO* pInfo, bool bRaid)
{
    // 对齐 IDA 0x1400058E0 (CMapWithChannel::CheckChannelUserCount)
    auto pFindChannel = FindChannel(nChannel);

    // 如果没找到或者频道状态不正常，查找低负载频道
    if (!pFindChannel || pFindChannel->IsState())
    {
        return FindChannelUserLow(pInfo, nChannel, bRaid);
    }

    return pFindChannel;
}

std::tr1::shared_ptr<CChannelOfMap> CMapWithChannel::FindChannelUserLow(
    ST_CHANNEL_MAP_INFO* pInfo, int nNowChannel, bool bRaid)
{
    // 对齐 IDA 0x140005A40 (CMapWithChannel::FindChannelUserLow)
    auto pNowChannel = FindChannel(nNowChannel);
    std::tr1::shared_ptr<CChannelOfMap> pFindChannel;

    if (pInfo)
    {
        // 遍历服务器拥有的所有频道
        for (auto& pair : pInfo->mapUXMapID)
        {
            // 提取频道号
            int nChannel = (signed char)((pair.second.nMapID >> 24) & 0xFF);
            auto pChannel = FindChannel(nChannel);

            if (!pChannel)
                continue;

            // 检查服务器状态 (状态2表示良好)
            if (pChannel->GetServerState() != 2)
                continue;

            // 选择用户数最低的频道
            if (!pFindChannel || pFindChannel->GetTotalCount() > pChannel->GetTotalCount())
            {
                pFindChannel = pChannel;
                pInfo->nCount = pair.first;
            }

            // 如果找到正常状态频道，停止搜索
            if (pFindChannel && !pFindChannel->IsState())
            {
                break;
            }
        }
    }

    // 如果当前频道和找到的频道都存在，比较状态
    if (pNowChannel && pFindChannel)
    {
        if (pNowChannel->IsState() <= pFindChannel->IsState())
        {
            pFindChannel = pNowChannel;
        }
    }

    // Raid模式下检查是否满员
    if (!bRaid || !pFindChannel || pFindChannel->GetTotalCount() < pFindChannel->GetMaxUserCount())
    {
        return pFindChannel;
    }

    return std::tr1::shared_ptr<CChannelOfMap>();
}

std::tr1::shared_ptr<CChannelOfMap> CMapWithChannel::FindChannelUserLow(bool bRaid)
{
    // 对齐 IDA 0x140005D70 (CMapWithChannel::FindChannelUserLow)
    std::tr1::shared_ptr<CChannelOfMap> pFindChannel;

    // 遍历所有频道
    for (auto& pair : m_mapChannel)
    {
        auto& pChannel = pair.second;
        if (!pChannel)
            continue;

        // 检查服务器状态 (状态2表示良好)
        if (pChannel->GetServerState() != 2)
            continue;

        // 选择用户数最低的频道
        if (!pFindChannel || pFindChannel->GetTotalCount() > pChannel->GetTotalCount())
        {
            pFindChannel = pChannel;
        }

        // 如果找到正常状态频道，停止搜索
        if (pFindChannel && !pFindChannel->IsState())
        {
            break;
        }
    }

    // Raid模式下检查是否满员
    if (!bRaid || !pFindChannel || pFindChannel->GetTotalCount() < pFindChannel->GetMaxUserCount())
    {
        return pFindChannel;
    }

    return std::tr1::shared_ptr<CChannelOfMap>();
}

void CMapWithChannel::GetChannel(PS_CHANNEL_INFO& stChannel)
{
    // 对齐 IDA 0x140002610 (CMapWithChannel::GetChannel)
    stChannel.wMapID = m_nTableID;
    stChannel.vecChannel.clear();

    // 遍历所有频道，填充频道信息
    for (auto& pair : m_mapChannel)
    {
        auto& pChannel = pair.second;
        if (!pChannel)
            continue;

        ST_CHANNEL_INFO stInfo;
        // 频道号: SBYTE3(uxMapID.nMapID) = (nMapID >> 24) & 0xFF as signed
        UXMapID uxMapID;
        pChannel->GetMapID(&uxMapID);
        stInfo.wChannel = (std::uint16_t)((signed char)((uxMapID.nMapID >> 24) & 0xFF));
        stInfo.byChannelState = pChannel->IsState() ? 1 : 0;
        stChannel.vecChannel.push_back(stInfo);
    }
}

void CMapWithChannel::SendDBAddChannelMap(std::tr1::shared_ptr<CChannelOfMap> pChannel)
{
    // 对齐 IDA 0x1400023C0 (CMapWithChannel::SendDBAddChannelMap)
    if (!pChannel)
        return;

    // 只有测试系统类型才发送
    XOption& option = XControlServer::Instance()->GetOption();
    if (option.GetSystemType() != SYSTEM_TYPE_TEST)
        return;

    ST_DB_CHANNEL_MAP stChannelMap;
    pChannel->GetMapID(&stChannelMap.uxMapID);

    // 频道号: SBYTE3(uxMapID.nMapID) = (nMapID >> 24) & 0xFF as signed
    stChannelMap.sChannel = (signed char)((stChannelMap.uxMapID.nMapID >> 24) & 0xFF);
    stChannelMap.wTableID = (std::uint16_t)m_nTableID;
    stChannelMap.dwServerID = pChannel->GetServer()->GetServerID();

    XSendDBPacket dbPacket(pChannel->GetServer(), 0x43, 0);
    dbPacket.XParse << stChannelMap.uxMapID.nMapID;
    dbPacket.XParse << stChannelMap.sChannel;
    dbPacket.XParse << stChannelMap.wTableID;
    dbPacket.XParse << stChannelMap.dwServerID;
    XControlServer::Instance()->SendDBGame(dbPacket);
}

void CMapWithChannel::SendServerAddChannelMap(CServer* pServer, UXMapID uxMapID, bool bResult)
{
    // 对齐 IDA 0x140002550 (CMapWithChannel::SendServerAddChannelMap)
    if (!pServer)
        return;

    PS_CREATE_MAP_RES stCreateMap;
    stCreateMap.uxMapID = uxMapID;
    stCreateMap.nResult = bResult ? 1 : 0;

    XSendPacket packet(0xF2, 0x30);
    packet << stCreateMap.uxMapID;
    packet.XParse << stCreateMap.nResult;
    pServer->SendEx(packet);
}

int CMapWithChannel::GetCount() const
{
    return (int)m_mapChannel.size();
}

// ============================================================================
// CChannelOfMap 实现
// ============================================================================

CChannelOfMap::CChannelOfMap()
    : m_pServer(nullptr)
    , m_nState(0)
    , m_nUserCount(0)
    , m_nMaxUserCount(0)
{
    m_uxMapID.nMapID = 0;
}

CChannelOfMap::CChannelOfMap(CServer* pServer, UXMapID uxMapID, int nMaxUserCount, int nCurUserCount)
{
    Init(pServer, uxMapID, nMaxUserCount, nCurUserCount);
}

void CChannelOfMap::Init(CServer* pServer, UXMapID uxMapID, int nMaxUserCount, int nCurUserCount)
{
    m_uxMapID = uxMapID;
    m_pServer = pServer;
    m_nMaxUserCount = nMaxUserCount;
    m_nUserCount = nCurUserCount;
    m_nState = 0;
}

CChannelOfMap::~CChannelOfMap()
{
    m_mapSelectUser.clear();
}

void CChannelOfMap::AddUserCount(int nUserID, int nAdd)
{
    // 对齐 IDA 0x140001000 (CChannelOfMap::AddUserCount)
    m_nUserCount += nAdd;

    // 从选择用户映射中移除该用户
    auto it = m_mapSelectUser.find(nUserID);
    if (it != m_mapSelectUser.end()) {
        m_mapSelectUser.erase(it);
    } else {
        LogHelper::LogError("game.relay",
            "<MAP %d / %d > CChannelOfMap::AddUserCount ( %d ) ",
            (int)((unsigned __int64)m_uxMapID.nMapID >> 16) >> 16,
            (int)(m_uxMapID.nMapID & 0xFFFFFFFF) >> 24,
            nUserID);
    }

    // 清理过期的选择用户
    ULONGLONG dwCurrentTime = GetTickCount64();
    auto iter = m_mapSelectUser.begin();
    while (iter != m_mapSelectUser.end()) {
        if (iter->second > dwCurrentTime) {
            ++iter;
        } else {
            iter = m_mapSelectUser.erase(iter);
        }
    }

    // 更新频道状态
    ChangeChannelState();
}

void CChannelOfMap::AddSelectUser(int nUserID)
{
    // 对齐 IDA 0x1400011C0 (CChannelOfMap::AddSelectUser)
    ULONGLONG dwWaitTime = GetTickCount64() + 300000;
    auto result = m_mapSelectUser.insert(std::make_pair(nUserID, dwWaitTime));

    if (!result.second) {
        // 插入失败，用户已存在
        LogHelper::LogError("game.relay",
            "<MAP %d / %d > CChannelOfMap::AddSelectUser ( %d ) ",
            (int)((unsigned __int64)m_uxMapID.nMapID >> 16) >> 16,
            (int)(m_uxMapID.nMapID & 0xFFFFFFFF) >> 24,
            nUserID);
    }

    // 更新频道状态
    ChangeChannelState();
}

void CChannelOfMap::ChangeChannelState()
{
    // 对齐 IDA 0x140001270 (CChannelOfMap::ChangeChannelState)
    int nBeforeState = m_nState;
    bool bChange = false;

    m_nState = CalculateChannelState(nBeforeState, &bChange);

    if (bChange)
    {
        // 广播频道状态变化
        WORD wMapID = (WORD)((unsigned __int64)m_uxMapID.nMapID >> 16) >> 16;
        ST_CHANNEL_INFO stChannel;
        stChannel.wChannel = (std::uint16_t)((signed char)((m_uxMapID.nMapID >> 24) & 0xFF));
        stChannel.byChannelState = IsState() ? 1 : 0;

        XSendPacket packet(0xF2, 0x12);
        packet.XParse << wMapID;
        packet << stChannel;
        XControlServer::Instance()->SendPacketAll(packet, false);
    }

    if (nBeforeState != m_nState)
    {
        GreenDamTan_log(__FILE__, __FUNCTION__, "<CHANGE_CHANNEL_STATE> state changed");
    }
}

int CChannelOfMap::CalculateChannelState(int nBeforeState, bool* pbChange)
{
    // 对齐 IDA 0x140001410 (CChannelOfMap::CalculateChannelState)
    int nState = 0;
    *pbChange = false;

    // 计算各阈值
    int nThreshold95 = (int)((float)m_nMaxUserCount * 0.94999999f);
    int nThreshold70 = (int)((float)m_nMaxUserCount * 0.69999999f);
    int nThreshold40 = (int)((float)m_nMaxUserCount * 0.40000001f);

    int nTotalCount = GetTotalCount();

    if (nTotalCount < nThreshold95)
    {
        if (nTotalCount < nThreshold70)
        {
            if (nTotalCount < nThreshold40)
            {
                // < 40%
                if (nBeforeState < 1)
                {
                    nState = 0;
                    if (nBeforeState != 0)
                        *pbChange = true;
                }
                else if (nTotalCount <= (int)((float)m_nMaxUserCount * 0.34999999f))
                {
                    nState = 0;
                    *pbChange = true;
                }
            }
            else
            {
                // 40% - 70%
                if (nBeforeState < 2)
                {
                    nState = 1;
                    if (nBeforeState != 1)
                        *pbChange = true;
                }
                else if (nTotalCount <= (int)((float)m_nMaxUserCount * 0.64999998f))
                {
                    nState = 1;
                    *pbChange = true;
                }
            }
        }
        else
        {
            // 70% - 95%
            if (nBeforeState < 3)
            {
                nState = 2;
                if (nBeforeState != 2)
                    *pbChange = true;
            }
            else if (nTotalCount <= (int)((float)m_nMaxUserCount * 0.89999998f))
            {
                nState = 2;
                *pbChange = true;
            }
        }
    }
    else
    {
        // >= 95% (满员)
        if (nBeforeState < 3)
            *pbChange = true;
        return 3;
    }

    return nState;
}

int CChannelOfMap::GetTotalCount()
{
    return m_nUserCount + (int)m_mapSelectUser.size();
}

bool CChannelOfMap::IsOKToEnter() const
{
    // 对齐 IDA 0x140001600 (CChannelOfMap::IsOKToEnter)
    return m_nMaxUserCount > m_nUserCount;
}

int CChannelOfMap::GetServerState() const
{
    if (m_pServer)
        return m_pServer->GetServerState();
    return 0;
}

bool CChannelOfMap::IsState() const
{
    return m_nState == 1;
}

UXMapID* CChannelOfMap::GetMapID(UXMapID* pMapID) const
{
    if (pMapID)
        *pMapID = m_uxMapID;
    return pMapID;
}

// ============================================================================
// ST_CHANNEL_MAP_INFO 实现
// ============================================================================

ST_CHANNEL_MAP_INFO::ST_CHANNEL_MAP_INFO()
    : nCount(0)
{
}

ST_CHANNEL_MAP_INFO::~ST_CHANNEL_MAP_INFO()
{
    mapUXMapID.clear();
}