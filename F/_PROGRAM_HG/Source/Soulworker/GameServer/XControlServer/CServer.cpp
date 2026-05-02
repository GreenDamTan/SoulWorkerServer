// CServer.cpp
// CServer 类实现 (对齐 IDA ControlServer.exe)

#include "CServer.h"
#include "ControlServer.h"
#include "MazeInfo.h"
#include "CMyRoom.h"
#include "ServerProcess.h"
#include "UserProcess.h"
#include "GMToolProcess.h"
#include "WorldModeProcess.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <cstdio>
#include <cstring>

// 对齐 IDA: CServer 构造函数
CServer::CServer() {
    // 初始化成员变量
    m_nSerial = 0;
    m_nMaxMazeCount = 0;
    m_nLogicThreadCount = 0;
    m_nLogicThreadSyncCount = 0;
    m_dwSyncLoad = 0;
}

// 对齐 IDA: CServer 析构函数
CServer::~CServer() {
    // 清理迷宫映射
    m_mapMazeInfo.clear();
    // 清理 MyRoom 映射
    m_mpMyRoomInfo.clear();
}

// 对齐 IDA 0x140041320: Init - 初始化服务器
bool CServer::Init() {
    // 对齐 IDA: 初始化成员变量 (顺序严格对齐)
    m_nMaxMazeCount = 0;
    m_stSyncInfo.SyncData = 0;  // 对齐 IDA: 使用 SyncData
    // 对齐 IDA: 设置同步标志位
    *reinterpret_cast<std::uint8_t*>(&m_stSyncInfo.SyncData) |= 2u;
    *reinterpret_cast<std::uint8_t*>(&m_stSyncInfo.SyncData) |= 1u;
    m_nSyncWorldForThread = 0;
    m_dwSyncLoad = 0;
    m_nLogicThreadCount = 0;
    m_nLogicThreadSyncCount = 0;  // 对齐 IDA: 初始化同步计数
    // 对齐 IDA: 清零服务器信息
    std::memset(&m_serverInfo, 0, sizeof(m_serverInfo));
    // 对齐 IDA: 清理映射
    m_mapMazeInfo.clear();
    m_mpMyRoomInfo.clear();
    // 对齐 IDA: 调用基类Init
    return XClient::Init();
}

// 对齐 IDA 0x140041590: SetServerInfo - 设置服务器信息
void CServer::SetServerInfo(const SS_SERVER_INFO& info) {
    // 对齐 IDA: 复制服务器信息 (1088 bytes)
    std::memcpy(&m_serverInfo, &info, sizeof(m_serverInfo));
    // 对齐 IDA: 重置同步数据
    m_stSyncInfo.SyncData = 0;
    // 对齐 IDA: 如果不是LoginServer类型，设置同步标志
    if (info.nType != 1) {
        *reinterpret_cast<std::uint8_t*>(&m_stSyncInfo.SyncData) |= 1u;
        *reinterpret_cast<std::uint8_t*>(&m_stSyncInfo.SyncData) |= 2u;
    }
    // 对齐 IDA: 设置同步线程数
    m_nSyncWorldForThread = info.sThreadCount;
}

// 对齐 IDA 0x140042AF0: SetSyncLoad - 设置同步加载状态
// IDA: if ((eServerSync & m_dwSyncLoad) == 0) m_dwSyncLoad |= eServerSync;
// IDA: if (m_dwSyncLoad == 3) XControlServer::SendCachingLoad()
void CServer::SetSyncLoad(E_SERVER_SYNC_LOAD eServerSync) {
    // 对齐 IDA: 按位或操作
    if ((static_cast<std::uint32_t>(eServerSync) & m_dwSyncLoad) == 0) {
        m_dwSyncLoad |= static_cast<std::uint32_t>(eServerSync);
    }
    LogHelper::LogInfo("game.system",
        "<CServer::SetSyncLoad> ID:%d, Server:%d, Req:%d",
        GetServerID(), m_dwSyncLoad, static_cast<int>(eServerSync));
    // 对齐 IDA: 当所有同步完成时发送缓存加载
    if (m_dwSyncLoad == 3) {
        auto pControlServer = XControlServer::Instance();
        if (pControlServer) {
            pControlServer->SendCachingLoad();
        }
    }
}

// 对齐 IDA 0x140041660: UpdateServerInfo - 更新服务器信息
void CServer::UpdateServerInfo(int nState, int nCount) {
    m_serverInfo.nState = nState;
    m_serverInfo.nCurUser = nCount;
    // 对齐 IDA: 如果是LoginServer类型，更新全局登录服用户数
    if (m_serverInfo.nType == 1) {
        auto pControlServer = XControlServer::Instance();
        auto pLoginServer = pControlServer->GetLoginServer();
        if (pLoginServer) {
            pLoginServer->SetUserCount(nCount);
        }
    }
}

// 对齐 IDA: FindMyRoom - 查找 MyRoom
std::shared_ptr<CMyRoom> CServer::FindMyRoom(std::uint32_t dwOwnerID) const {
    auto it = m_mpMyRoomInfo.find(dwOwnerID);
    if (it != m_mpMyRoomInfo.end()) {
        return it->second;
    }
    return nullptr;
}

// 对齐 IDA: DelMyRoom - 删除 MyRoom
void CServer::DelMyRoom(std::uint32_t dwOwnerID) {
    auto it = m_mpMyRoomInfo.find(dwOwnerID);
    if (it != m_mpMyRoomInfo.end()) {
        m_mpMyRoomInfo.erase(it);
        LogHelper::LogDebug("game.contents", "DelMyRoom: removed owner %u", dwOwnerID);
    }
}

// 对齐 IDA 0x140042340: SyncMaze - 同步迷宫信息
bool CServer::SyncMaze(PS_MAZE_UPDATE_INFO_SYNC* pMazeInfo) {
    if (!pMazeInfo) {
        return false;
    }

    // 对齐 IDA: 检查是否为最后一个同步包
    if (pMazeInfo->bLast) {
        // 对齐 IDA: 递增同步计数并检查是否完成
        ++m_nLogicThreadSyncCount;
        if (m_nLogicThreadSyncCount == m_nLogicThreadCount) {
            SetSyncLoad(E_SERVER_SYNC_LOAD_MAZE_INFO);
        }
        LogHelper::LogInfo("game.relay",
            "<SYNC> SyncMaze Finish [ID:%d] [%d / %d]",
            m_serverInfo.dwID,
            m_nLogicThreadSyncCount,
            m_nLogicThreadCount);
        return true;
    }

    // 对齐 IDA: 查找或创建迷宫信息
    UXMapID uxMapID = pMazeInfo->psMazeInfo.uxMapID;
    auto it = m_mapMazeInfo.find(uxMapID);
    if (it != m_mapMazeInfo.end()) {
        // 更新现有迷宫信息
        auto& pMaze = it->second;
        if (pMaze) {
            pMaze->SyncMazeInfo(pMazeInfo);
        }
    } else {
        // 对齐 IDA: 使用 XControlServer::Instance()->m_factoryMaze.Create() 创建 CMazeInfo
        auto pControlServer = XControlServer::Instance();
        std::tr1::shared_ptr<CMazeInfo> pMaze = pControlServer->GetMazeFactory().Create();  // 对齐 IDA: ClassFactory
        if (pMaze) {
            pMaze->SyncMazeInfo(pMazeInfo);
            m_mapMazeInfo[uxMapID] = pMaze;
        }
    }
    return true;
}

// 对齐 IDA 0x1400416D0: GetSerial - 获取并递增序列号
int CServer::GetSerial() {
    // 对齐 IDA: InterlockedIncrement(&m_nSerial)
#ifdef _WIN32
    return InterlockedIncrement(reinterpret_cast<LONG*>(&m_nSerial));
#else
    // Linux: use atomic increment
    return ++m_nSerial;
#endif
}

// 对齐 IDA 0x1400417C0: GetMapID - 生成地图实例ID
// IDA: mapID.nMapID = ((unsigned __int64)(unsigned __int16)this->m_serverInfo.nChannel << 48)
UXMapID CServer::GetMapID(WORD wMapID) {
    UXMapID uxMapID{};
    // 对齐 IDA: 使用 nChannel 作为 channel bits (bits 48-63)
    // 注意: IDA 明确使用 nChannel 而非 dwID
    uxMapID.parts.channel = static_cast<std::int64_t>(static_cast<unsigned __int16>(m_serverInfo.nChannel));
    uxMapID.parts.mapID = static_cast<std::int64_t>(wMapID);
    uxMapID.parts.seq = static_cast<std::int64_t>(GetSerial());
    return uxMapID;
}

// 对齐 IDA 0x140027CC0: RecvMapInfo - 接收地图信息
// IDA: *(_BYTE *)&this->m_stSyncInfo.SyncData &= ~2u;
void CServer::RecvMapInfo() {
    // 对齐 IDA: 清除 SyncData 的 bit 1 标志位
    m_stSyncInfo.SyncData &= ~2u;
}

// 对齐 IDA 0x140041930: RemoveMaze - 移除迷宫
void CServer::RemoveMaze(UXMapID uxMapID, bool bResult) {
    auto it = m_mapMazeInfo.find(uxMapID);
    if (it == m_mapMazeInfo.end()) {
        return;
    }

    auto& pMazeInfo = it->second;
    if (pMazeInfo) {
        if (bResult) {
            // 对齐 IDA: 重置父迷宫和子迷宫引用
            pMazeInfo->ResetParentMaze(this);
            pMazeInfo->ResetChildMaze();

            // 对齐 IDA: 处理队伍迷宫ID
            int nPartyID = pMazeInfo->GetPartyID();
            if (nPartyID > 0) {
                auto pControlServer = XControlServer::Instance();
                UXMapID uxEmptyMapID{};  // 设置为 0
                if (pControlServer->GetPartyManager().SetMazeID(nPartyID, uxEmptyMapID, uxMapID)) {
                    // 发送队伍移除包 (0xF2, 0x45)
                    XSendPacket xSendPacket(0xF2, 0x45);
                    xSendPacket.XParse << nPartyID;
                    xSendPacket.XParse << 0;  // bResult = false
                    xSendPacket.XParse << uxMapID.nMapID;
                    pControlServer->SendCommunity(xSendPacket);
                }
            }

            // 对齐 IDA: 处理Force迷宫ID
            int nForceID = pMazeInfo->GetForceID();
            if (nForceID > 0) {
                auto pControlServer = XControlServer::Instance();
                UXMapID uxEmptyMapID{};  // 设置为 0
                if (pControlServer->GetForceManager().SetMazeID(nForceID, uxEmptyMapID, uxMapID)) {
                    // 发送Force移除包 (0xF2, 0x46)
                    XSendPacket xSendPacket(0xF2, 0x46);
                    xSendPacket.XParse << nForceID;
                    xSendPacket.XParse << 0;  // bResult = false
                    xSendPacket.XParse << uxMapID.nMapID;
                    pControlServer->SendCommunity(xSendPacket);
                }
            }
        } else {
            // 对齐 IDA: 如果 bResult 为 false，设置状态为正常
            pMazeInfo->SetMazeState(1, 0);
        }
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "RemoveMaze complete");
}

// 对齐 IDA 0x1400421E0: UpdateMaze - 更新迷宫信息
// 注意: IDA 返回 0，此处对齐
bool CServer::UpdateMaze(PS_MAZE_UPDATE_INFO* pMazeInfo) {
    if (!pMazeInfo) {
        return false;
    }

    UXMapID uxMapID = pMazeInfo->uxMapID;
    auto it = m_mapMazeInfo.find(uxMapID);

    if (it != m_mapMazeInfo.end()) {
        // 更新现有迷宫
        auto& pMaze = it->second;
        if (pMaze) {
            pMaze->UpdateMazeInfo(pMazeInfo);  // 对齐 IDA: 传指针
        }
    } else {
        // 对齐 IDA: 使用 XControlServer::Instance()->m_factoryMaze.Create() 创建新迷宫
        auto pControlServer = XControlServer::Instance();
        std::tr1::shared_ptr<CMazeInfo> pMaze = pControlServer->GetMazeFactory().Create();  // 对齐 IDA: ClassFactory
        if (pMaze) {
            pMaze->UpdateMazeInfo(pMazeInfo);  // 对齐 IDA: 传指针
            m_mapMazeInfo[uxMapID] = pMaze;
        }
    }
    return false;  // 对齐 IDA: 返回 0
}

// 对齐 IDA 0x140042550: OnUpdate - 服务器更新循环
void CServer::OnUpdate(ULONGLONG dwTick) {
    // 对齐 IDA: 遍历迷宫映射
    auto it = m_mapMazeInfo.begin();
    while (it != m_mapMazeInfo.end()) {
        auto& pMazeInfo = it->second;
        if (pMazeInfo && pMazeInfo->IsDestroy()) {
            // 对齐 IDA: 发送移除迷宫包 (0xF2, 0x25)
            XSendPacket xSendPacket(0xF2, 0x25);
            ST_MAP_INFO stMazeInfo = pMazeInfo->GetMazeInfo();
            xSendPacket.XParse << stMazeInfo.uxMapID.nMapID;
            xSendPacket.XParse << pMazeInfo->GetMazeType();
            SendEx(xSendPacket);

            // 对齐 IDA: 调用 RemoveMaze
            RemoveMaze(stMazeInfo.uxMapID, true);

            // 对齐 IDA: 从映射中移除并继续
            it = m_mapMazeInfo.erase(it);
        } else {
            ++it;
        }
    }
}

// 对齐 IDA 0x140041180: RegisterProcess - 注册包处理类
bool CServer::RegisterProcess() {
    // 对齐 IDA: 创建 CServerProcess 并注册到 0xF2
    CServerProcess* pServerProcess = new CServerProcess();
    if (!Register(0xF2, pServerProcess)) {
        return false;
    }

    // 对齐 IDA: 创建 CUserProcess 并注册到 0xF3
    CUserProcess* pUserProcess = new CUserProcess();
    if (!Register(0xF3, pUserProcess)) {
        return false;
    }

    // 对齐 IDA: 创建 CGMToolProcess 并注册到 0xF8
    CGMToolProcess* pGMToolProcess = new CGMToolProcess();
    if (!Register(0xF8, pGMToolProcess)) {
        return false;
    }

    // 对齐 IDA: 创建 CWorldModeProcess 并注册到 0xFB
    CWorldModeProcess* pWorldModeProcess = new CWorldModeProcess();
    if (!Register(0xFB, pWorldModeProcess)) {
        return false;
    }

    return true;
}

// 对齐 IDA 0x1400427C0: CreateMyRoom - 创建 MyRoom
void CServer::CreateMyRoom(ST_MYROOM_OWNER_INFO stOwnerInfo, ST_MYROOM_USER stEnterUser, std::uint32_t dwOwnerUCID) {
    // 对齐 IDA: 获取地图ID (使用 dwMapID 作为 mapID)
    UXMapID uxMapID = GetMapID(static_cast<WORD>(stOwnerInfo.dwMapID));

    // 对齐 IDA: 创建 CMyRoom
    auto pMyRoom = std::make_shared<CMyRoom>();
    pMyRoom->CreateMyRoom(&stOwnerInfo, uxMapID, &stEnterUser, this);

    // 对齐 IDA: 插入映射
    m_mpMyRoomInfo[stOwnerInfo.dwOwnerUAID] = pMyRoom;

    // 对齐 IDA: 发送创建包 (0xF2, 0x52)
    XSendPacket xSendPacket(0xF2, 0x52);
    xSendPacket << stEnterUser;
    xSendPacket << stOwnerInfo;
    xSendPacket.XParse << uxMapID.nMapID;
    xSendPacket.XParse << dwOwnerUCID;
    SendEx(xSendPacket);

    GreenDamTan_log(__FILE__, __FUNCTION__, "CreateMyRoom: sent 0xF2,0x52");
}

// 对齐 IDA 0x140041C80: IsValidEnterPartyMemberInMaze
bool CServer::IsValidEnterPartyMemberInMaze(DWORD dwPartyID, UXMapID uxMapID, PS_ENTER_MAP_REQ& stEnterMap, PS_ENTER_MAP_RES& stEnterMapRes) {
    // 对齐 IDA 0x140041C80: IsValidEnterPartyMemberInMaze
    stEnterMapRes.dwUserID = stEnterMap.dwActorID;

    // 对齐 IDA: 查找迷宫信息
    auto it = m_mapMazeInfo.find(uxMapID);
    if (it == m_mapMazeInfo.end()) {
        stEnterMapRes.nResult = 1;  // 对齐 IDA: 迷宫不存在
        return false;
    }

    auto& pMazeInfo = it->second;
    if (!pMazeInfo) {
        stEnterMapRes.nResult = 1;
        return false;
    }

    // 对齐 IDA: 检查队伍ID和地图ID
    // IDA: GetPartyID == dwPartyID && GetMapID == stEnterMap.wMapID
    UXMapID mazeMapID = pMazeInfo->GetMapID();
    WORD wMazeMapID = static_cast<WORD>((mazeMapID.nMapID >> 32) & 0xFFFF);  // 提取 bits 32-47
    if (pMazeInfo->GetPartyID() != static_cast<int>(dwPartyID) ||
        wMazeMapID != stEnterMap.wMapID) {  // 对齐 IDA: 比较 wMapID (WORD)
        stEnterMapRes.nResult = 2;  // 对齐 IDA: 队伍/地图不匹配
        return false;
    }

    // 对齐 IDA: 检查成员列表
    auto pVecMember = pMazeInfo->GetMemberVector();
    if (pVecMember) {
        for (const auto& stMember : *pVecMember) {
            if (stEnterMap.dwActorID == stMember.dwMember) {  // 对齐 IDA: 比较 dwMember
                // 成员存在，填充响应
                pMazeInfo->GetMazeInfo(&stEnterMapRes);  // 对齐 IDA: qmemcpy 0x258 bytes
                stEnterMapRes.dwUserID = stEnterMap.dwActorID;
                stEnterMapRes.nResult = 0;
                stEnterMapRes.nJumpID = stEnterMap.nJumpID;
                stEnterMapRes.nPortalID = stEnterMap.nPortalID;
                return true;
            }
        }
    }

    stEnterMapRes.nResult = 3;  // 对齐 IDA: 成员不在列表中
    return false;
}

// 对齐 IDA 0x140041F30: IsValidEnterForceMemberInMaze
bool CServer::IsValidEnterForceMemberInMaze(DWORD dwForceID, UXMapID uxMapID, PS_ENTER_MAP_REQ& stEnterMap, PS_ENTER_MAP_RES& stEnterMapRes) {
    // 对齐 IDA 0x140041F30: IsValidEnterForceMemberInMaze
    stEnterMapRes.dwUserID = stEnterMap.dwActorID;

    // 对齐 IDA: 查找迷宫信息
    auto it = m_mapMazeInfo.find(uxMapID);
    if (it == m_mapMazeInfo.end()) {
        stEnterMapRes.nResult = 1;  // 对齐 IDA: 迷宫不存在
        return false;
    }

    auto& pMazeInfo = it->second;
    if (!pMazeInfo) {
        stEnterMapRes.nResult = 1;
        return false;
    }

    // 对齐 IDA: 检查ForceID和地图ID
    // IDA: GetForceID == dwForceID && GetMapID == stEnterMap.wMapID
    UXMapID mazeMapID = pMazeInfo->GetMapID();
    WORD wMazeMapID = static_cast<WORD>((mazeMapID.nMapID >> 32) & 0xFFFF);  // 提取 bits 32-47
    if (pMazeInfo->GetForceID() != static_cast<int>(dwForceID) ||
        wMazeMapID != stEnterMap.wMapID) {  // 对齐 IDA: 比较 wMapID (WORD)
        stEnterMapRes.nResult = 2;  // 对齐 IDA: Force/地图不匹配
        return false;
    }

    // 对齐 IDA: 检查成员列表
    auto pVecMember = pMazeInfo->GetMemberVector();
    if (pVecMember) {
        for (const auto& stMember : *pVecMember) {
            if (stEnterMap.dwActorID == stMember.dwMember) {  // 对齐 IDA: 比较 dwMember
                // 成员存在，填充响应
                pMazeInfo->GetMazeInfo(&stEnterMapRes);  // 对齐 IDA: qmemcpy 0x258 bytes
                stEnterMapRes.dwUserID = stEnterMap.dwActorID;
                stEnterMapRes.nResult = 0;
                stEnterMapRes.nJumpID = stEnterMap.nJumpID;
                stEnterMapRes.nPortalID = stEnterMap.nPortalID;
                return true;
            }
        }
    }

    stEnterMapRes.nResult = 3;  // 对齐 IDA: 成员不在列表中
    return false;
}
