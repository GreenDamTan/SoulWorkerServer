// CForce.cpp
// CForce and CForceMember implementation
// 对齐 IDA GameServer.exe

#include "CForce.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMapMaze.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocQuest.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocRecode.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAttribute.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocEntity.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "User.h"
#include "GameServer.h"
#include "ThreadLocalData.h"

// Forward declarations
class XSendPacket;

// ============================================================================
// CForceMember Implementation
// ============================================================================

CForceMember::CForceMember()
    : CPartyMember() {
}

CForceMember::~CForceMember() {
}

// IDA: ?SetMapID@CForceMember@@QEAAXH@Z
void CForceMember::SetMapID(int nMapID) {
    // IDA: Set map ID in member info
    // m_partyMemberInfo.uxMapID related
    // TODO: Implement when ST_PARTY_MEMBER fields are available
}

// IDA: ?SetMaxHP@CForceMember@@QEAAXH@Z
void CForceMember::SetMaxHP(int nMaxHP) {
    // IDA: Set max HP in member info
    // TODO: Implement when ST_PARTY_MEMBER fields are available
}

// IDA: ?SetAwaken@CForceMember@@QEAAXE@Z (via CRespawnManager::SetPause)
void CForceMember::SetAwaken(std::uint8_t byAwaken) {
    // IDA: Maps to CRespawnManager::SetPause - awaken/pause state
    // TODO: Implement when awaken system is available
}

// ============================================================================
// CForce Implementation
// ============================================================================

CForce::CForce()
    : CParty()
    , m_byForceType(0) {
}

CForce::~CForce() {
}

// IDA: ?SetMemberMapID@CForce@@QEAAXKHHTUXMapID@@@Z @ 0x1401B7E80
// void __fastcall CForce::SetMemberMapID(CForce *this, unsigned int dwActorID, int nMapID, int nChannel, UXMapID uxMapID)
// {
//   // Find member in map
//   auto iter = m_mapForceMember.find(dwActorID);
//   if (iter != m_mapForceMember.end()) {
//     CForceMember* pMember = iter->second;
//     if (pMember) {
//       CPartyMember::Login(pMember);        // Mark as online
//       CForceMember::SetMapID(pMember, nMapID);
//       CPartyMember::SetChannel(pMember, nChannel);
//       UXMapID::UXMapID(&pMember->uxMapID, uxMapID);
//       CForce::SendUpdateMemberInfo(this, dwActorID);
//     }
//   }
// }
void CForce::SetMemberMapID(std::uint32_t dwActorID, int nMapID, int nChannel, const UXMapID& uxMapID) {
    // IDA verified: Find member, update map ID, channel, map instance, then broadcast
    auto iter = m_mapForceMember.find(dwActorID);
    if (iter != m_mapForceMember.end() && iter->second) {
        CForceMember* pMember = iter->second;
        
        // IDA: Call Login to mark as online
        pMember->Login();
        
        // IDA: Set map ID
        pMember->SetMapID(nMapID);
        
        // IDA: Set channel
        pMember->SetChannel(nChannel);
        
        // IDA: Set UXMapID (map instance ID)
        // TODO: pMember->SetMapInsID(uxMapID);
        
        // IDA: Broadcast update to all force members
        SendUpdateMemberInfo(dwActorID);
    }
}

// IDA: ?SetMemberHP@CForce@@QEAAXKTUXMapID@@H@Z @ 0x1401B7F50
// IDA verified: Set member HP and broadcast PS_FORCE_MEMEBER_HP to local map
void CForce::SetMemberHP(std::uint32_t dwActorID, const UXMapID& uxMapID, int nHP) {
    auto iter = m_mapForceMember.find(dwActorID);
    if (iter != m_mapForceMember.end() && iter->second) {
        CForceMember* pMember = iter->second;
        
        // IDA: Set HP using GameModeBase::SetModeState
        // TODO: pMember->SetHP(nHP);
        
        // IDA: Construct PS_FORCE_MEMEBER_HP packet
        // stMemeberHP.dwMemberID = dwActorID;
        // stMemeberHP.nMaxHP = pMember->GetMaxHP();
        // stMemeberHP.nHP = nHP;
        // XSendPacket(0x2E, 0x12) << stMemeberHP;
        // CParty::SendToLocal(this, &xSendPacket, uxMapID, dwActorID);
        
        // TODO: Implement packet sending when XSendPacket is available
    }
}

// IDA: ?SetMemberMaxHP@CForce@@QEAAXKTUXMapID@@H@Z @ 0x1401B80B0
// IDA verified: Set member max HP and broadcast PS_FORCE_MEMEBER_HP to local map
void CForce::SetMemberMaxHP(std::uint32_t dwActorID, const UXMapID& uxMapID, int nMaxHP) {
    auto iter = m_mapForceMember.find(dwActorID);
    if (iter != m_mapForceMember.end() && iter->second) {
        CForceMember* pMember = iter->second;
        
        // IDA: Set max HP using CForceMember::SetMaxHP
        pMember->SetMaxHP(nMaxHP);
        
        // IDA: Construct PS_FORCE_MEMEBER_HP packet
        // stMemeberHP.dwMemberID = dwActorID;
        // stMemeberHP.nMaxHP = nMaxHP;
        // stMemeberHP.nHP = pMember->GetHP();
        // XSendPacket(0x2E, 0x12) << stMemeberHP;
        // CParty::SendToLocal(this, &xSendPacket, uxMapID, dwActorID);
        
        // TODO: Implement packet sending when XSendPacket is available
    }
}

// IDA: ?SetMemberLevel@CForce@@QEAAXKH@Z @ 0x1401B8210
// IDA verified: Set member level and broadcast update
void CForce::SetMemberLevel(std::uint32_t dwActorID, int nLevel) {
    auto iter = m_mapForceMember.find(dwActorID);
    if (iter != m_mapForceMember.end() && iter->second) {
        CForceMember* pMember = iter->second;
        
        // IDA: Set level using CPartyMember::SetLevel
        pMember->SetLevel(static_cast<std::uint8_t>(nLevel));
        
        // IDA: Broadcast update to all force members
        SendUpdateMemberInfo(dwActorID);
    }
}

// IDA: ?SetMemberAwaken@CForce@@QEAAXKE@Z @ 0x1401B82B0
// IDA verified: Set member awaken status (maps to CRespawnManager::SetPause) and broadcast
void CForce::SetMemberAwaken(std::uint32_t dwActorID, std::uint8_t byAwaken) {
    auto iter = m_mapForceMember.find(dwActorID);
    if (iter != m_mapForceMember.end() && iter->second) {
        CForceMember* pMember = iter->second;
        
        // IDA: Set awaken using CRespawnManager::SetPause
        pMember->SetAwaken(byAwaken);
        
        // IDA: Broadcast update to all force members
        SendUpdateMemberInfo(dwActorID);
    }
}

// IDA: ?SetMemberProfilePhoto@CForce@@QEAAXKK@Z @ 0x1401B8350
// IDA verified: Set member profile photo and broadcast update
void CForce::SetMemberProfilePhoto(std::uint32_t dwActorID, std::uint32_t dwPhotoID) {
    auto iter = m_mapForceMember.find(dwActorID);
    if (iter != m_mapForceMember.end() && iter->second) {
        CForceMember* pMember = iter->second;
        
        // IDA: Set profile photo using CPartyMember::SetProfilePhoto
        pMember->SetProfilePhoto(dwPhotoID);
        
        // IDA: Broadcast update to all force members
        SendUpdateMemberInfo(dwActorID);
    }
}

// IDA: ?SendUpdateMemberInfo@CForce@@QEAAXK@Z
// Broadcast member info update to all force members
void CForce::SendUpdateMemberInfo(std::uint32_t dwActorID) {
    // IDA: Construct PS_FORCE_MEMBER_INFO packet
    // Send to all force members
    // TODO: Implement when packet structures are available
}

// IDA: ?SendToLocal@CParty@@QEAAXAEAVXSendPacket@@TUXMapID@@K@Z
// Send packet to members in same map instance
void CForce::SendToLocal(XSendPacket& packet, const UXMapID& uxMapID, std::uint32_t dwExcludeActorID) {
    // IDA: Iterate through all force members
    // Check if member's map instance matches uxMapID
    // Send packet to matching members (excluding dwExcludeActorID)
    // TODO: Implement when member iteration and packet sending are available
}

// IDA: ?GetForceType@CForce@@QEAAEXZ
std::uint8_t CForce::GetForceType() const {
    return m_byForceType;
}

// IDA: ?UpdateForceBooster@CForce@@QEAAXH@Z
void CForce::UpdateForceBooster(int nType) {
    // IDA: Update booster effects for all force members
    // nType parameter controls which booster to update
    // TODO: Implement when booster system is available
}

// IDA: ?GetMazeRecode@CForce@@QEAAXKPEAH@Z @ 0x1403A6D30
void CForce::GetMazeRecode(std::uint32_t dwActorID, int* pMazeRecode) {
    // IDA: iter = m_mapPartyMember.find(dwActorID);
    // if (iter != end) CPartyMember::GetRecode((CPartyMember*)iter->second, pMazeRecode);
    auto iter = m_mapPartyMember.find(dwActorID);
    if (iter != m_mapPartyMember.end()) {
        iter->second->GetRecode(pMazeRecode);
    }
}

// IDA: ?SendForceInfo@CForce@@QEAAXPEAVCUser@@E@Z @ 0x1401B7100
// 向单个成员发送 Force 信息：(0x2E,9) PS_FORCE_INFO，
// 遍历 m_mapForceMember 逐个拷贝成员数据（ST_PARTY_MEMBER 与
// ST_FORCE_MEMBER 线格式布局一致，CPartyMember 首字段即成员信息）。
void CForce::SendForceInfo(CUser* pMember, std::uint8_t byUpdateType) {
    if (!pMember) {
        return;
    }

    PS_FORCE_INFO psForceInfo = {};
    psForceInfo.dwForceID = m_dwForceID;
    psForceInfo.dwMaster = m_dwMasterID;
    psForceInfo.byUpdateType = byUpdateType;
    psForceInfo.byForceType = m_byForceType;

    // IDA: 遍历 m_mapForceMember 拷贝成员
    for (const auto& kv : m_mapForceMember) {
        CForceMember* pForceMember = kv.second;
        if (!pForceMember) {
            continue;
        }
        const ST_PARTY_MEMBER& stMember = pForceMember->GetMemberInfo();
        ST_FORCE_MEMBER stForceMember = {};
        stForceMember.dwMemberID = stMember.dwMemberID;
        std::memcpy(stForceMember.strName, stMember.strName, sizeof(stForceMember.strName));
        stForceMember.byLevel = stMember.byLevel;
        stForceMember.byClass = stMember.byClass;
        stForceMember.byAwaken = stMember.byAwaken;
        stForceMember.dwProfilePhotoID = stMember.dwProfilePhotoID;
        stForceMember.nMapID = stMember.nMapID;
        stForceMember.nChannel = stMember.nChannel;
        stForceMember.nMaxHP = stMember.nMaxHP;
        stForceMember.nHP = stMember.nHP;
        stForceMember.bLogin = stMember.bLogin;
        stForceMember.uxMapID = stMember.uxMapID;
        psForceInfo.vecForceMember.push_back(stForceMember);
    }

    XSendPacket xSendPacket(0x2E, 9);
    xSendPacket << psForceInfo;
    CGocNetwork::Send(pMember, xSendPacket);
}

// IDA: ?ApplyReward@CForce@@QEAAX... (dtor reference)
void CForce::ApplyReward(/* parameters TBD */) {
    // IDA: Apply rewards to force members
    // Called after completing content (maze, raid, etc.)
    // TODO: Implement when reward system is available
}

// IDA: GetMember - Get force member by ActorID
CForceMember* CForce::GetMember(std::uint32_t dwActorID) {
    auto iter = m_mapForceMember.find(dwActorID);
    if (iter != m_mapForceMember.end()) {
        return iter->second;
    }
    return nullptr;
}

// ============================================================================
// SendEnterMaze - IDA: ?SendEnterMaze@CForce@@QEAAXPEAVCUser@@AEAUPS_ENTER_MAP_RES@@@Z @ 0x1401B8F00
// 单人进迷宫：任务条件同步 -> 传送门定位 -> (3,0x42) DB Game 包 +
// (0xF0,0x12) 统计包 + ST_LOG_GAME(5,4) 日志；定位失败时清状态并发错误码。
// ============================================================================
void CForce::SendEnterMaze(CUser* pUser, PS_ENTER_MAP_RES* stEnterMapRes) {
    if (!pUser) {
        LogHelper::LogError("game.contents", "SendEnterMaze error - Invalid User( %d )", 1017);
        return;
    }

    // IDA: GetGOC<CGocQuest> 存在则 DBSyncQuestCondition
    {
        CGocQuest* pQuest = pUser->GetGOC<CGocQuest>();
        if (pQuest) {
            pQuest->DBSyncQuestCondition();
        }
    }

    // IDA: byChangeType 置 0，父实例 ID 非零时置 5
    stEnterMapRes->byChangeType = 0;
    if (stEnterMapRes->uxParentInstanceID.nMapID > 0) {
        stEnterMapRes->byChangeType = 5;
    }

    // IDA: nMapID = (int)(uxMapID.nMapID << 16 >> 48) 后查传送门
    const int nMapID = static_cast<int>(
        (stEnterMapRes->uxMapID.nMapID << 16) >> 48);
    if (!XGameServer::Instance()->GetWorldResMgr().GetPortalPos(
            nMapID, stEnterMapRes->nJumpID, &stEnterMapRes->stPosInfo)) {
        // IDA: 定位失败 - 清切换状态 + (0x11,0x41,0xD6DA) 错误 + LogError 1034
        pUser->ClearState(eStateChangeWorld);
        pUser->SendErrorMessage(0x11, 0x41, 0xD6DA);
        LogHelper::LogError("game.contents",
            "SendEnterMaze error - No World Data[ ActorID:%d, MapID:%d, JumpID:%d ] (%d )",
            stEnterMapRes->dwUserID,
            static_cast<int>((static_cast<std::uint64_t>(stEnterMapRes->uxMapID.nMapID) >> 16) >> 16),
            stEnterMapRes->nJumpID, 1034);
        return;
    }

    // IDA: 目标服务器与当前不同则置 bChangeServer
    if (stEnterMapRes->dwServerID != XGameServer::Instance()->GetOption().GetServerID()) {
        stEnterMapRes->bChangeServer = true;
    }

    LogHelper::LogDebug("game.contents",
        "<RecvCreateMazeRes> User ( %d ) ( %I64d, %d %d ) bChange ( %d ) ",
        stEnterMapRes->dwUserID,
        stEnterMapRes->uxMapID.nMapID,
        static_cast<int>((static_cast<std::uint64_t>(stEnterMapRes->uxMapID.nMapID) >> 16) >> 16),
        stEnterMapRes->nJumpID, stEnterMapRes->bChangeServer ? 1 : 0);

    // IDA: (3,0x42) DB Game 包 + 前图/复活点补零
    {
        XSendDBPacket xSendDBPacket(static_cast<XActor*>(pUser), 3, 0x42);
        xSendDBPacket << *stEnterMapRes;
        xSendDBPacket.XParse << 0;  // nPrevMapID
        xSendDBPacket.XParse << 0;  // nPrevRevivePoint
        XGameServer::Instance()->SendDBGame(xSendDBPacket);
    }

    // IDA: (0xF0,0x12) 统计包
    {
        ST_STATISTICS_MAP_SAVE stInfo = {};
        stInfo.dwUCID = stEnterMapRes->dwUserID;
        stInfo.dwMapID = static_cast<int>(
            (static_cast<std::uint64_t>(stEnterMapRes->uxMapID.nMapID) >> 16) >> 16);
        stInfo.dwServerID = XGameServer::Instance()->GetOption().GetServerID();
        XSendDBPacket xSendDBStatistics(static_cast<XActor*>(pUser), 0xF0, 0x12);
        xSendDBStatistics << stInfo;
        XGameServer::Instance()->SendDBStatistics(xSendDBStatistics);
    }

    // IDA: GetGOC<CGocForce> 存在才写 ST_LOG_GAME(5,4) 日志
    {
        CGocForce* pGocForce = pUser->GetGOC<CGocForce>();
        if (pGocForce) {
            ST_LOG_GAME stLog = {};
            stLog._nUAID = static_cast<int>(pUser->GetUAID());
            stLog._nUCID = static_cast<int>(pUser->GetActorID().dwActorID);
            stLog._sMainType = 5;
            stLog._sSubType = 4;
            stLog.nParam0 = static_cast<int>(
                (static_cast<std::uint64_t>(stEnterMapRes->uxMapID.nMapID) >> 16) >> 16);
            stLog.nParam1 = static_cast<int>(m_dwMasterID);
            stLog.nParam2 = 1;
            stLog.nParam3 = GetUserCount();
            stLog.nParam4 = static_cast<int>(m_dwForceID);
            stLog.nParam5 = static_cast<int>(pUser->GetLevel());
            stLog.nParam6 = static_cast<int>(stEnterMapRes->uxMapID.nMapID);
            std::wcscpy(stLog.szComment, L"메이즈 입장");
            XGameServer::Instance()->SendDBLog(stLog);
        }
    }
}

// ============================================================================
// SendEnterMaze - IDA: ?SendEnterMaze@CForce@@QEAAXAEAUPS_ENTER_MAP_RES@@@Z @ 0x1401BB420
// 全员广播进迷宫：先校验 TB_MAZE_INFO 存在，对已准备(IsReadyToMaze)且
// 本线程的成员逐人执行与单人版相同的同步/DB/日志流程。
// ============================================================================
void CForce::SendEnterMaze(PS_ENTER_MAP_RES* stEnterMapRes) {
    // IDA: 迷宫表查不到直接 LogError 1490 返回
    const std::uint16_t wMazeID = static_cast<std::uint16_t>(
        (stEnterMapRes->uxMapID.nMapID << 16) >> 48);
    if (!XGameServer::Instance()->GetResourceMgr().GetTB_MAZE_INFO(wMazeID)) {
        LogHelper::LogError("game.contents",
            "SendEnterMaze error - No have World Data[ ActorID:%d, MapID:%d, JumpID:%d ] ( %d )",
            stEnterMapRes->dwUserID,
            static_cast<int>((static_cast<std::uint64_t>(stEnterMapRes->uxMapID.nMapID) >> 16) >> 16),
            stEnterMapRes->nJumpID, 1490);
        return;
    }

    // IDA: m_uxMazeID = stEnterMapRes->uxMapID（记录当前迷宫）
    SetMazeID(stEnterMapRes->uxMapID);

    // IDA: 遍历 m_mapForceMember
    for (auto& kv : m_mapForceMember) {
        CForceMember* pMember = kv.second;
        if (!pMember) {
            continue;
        }
        if (!pMember->IsReadyToMaze()) {
            continue;
        }
        CUser* pUser = pMember->GetMember();
        if (!pUser) {
            continue;
        }
        UXMapID uxMapID = pUser->GetMapInsID();
        if (!pUser || !ThreadLocalData::GetInstance()->IsThreadArea(uxMapID)) {
            continue;
        }

        // IDA: 任务条件同步
        {
            CGocQuest* pQuest = pUser->GetGOC<CGocQuest>();
            if (pQuest) {
                pQuest->DBSyncQuestCondition();
            }
        }

        // IDA: byChangeType 门控
        stEnterMapRes->byChangeType = 0;
        if (stEnterMapRes->uxParentInstanceID.nMapID > 0) {
            stEnterMapRes->byChangeType = 5;
        }

        const int nMapID = static_cast<int>(
            (stEnterMapRes->uxMapID.nMapID << 16) >> 48);
        if (!XGameServer::Instance()->GetWorldResMgr().GetPortalPos(
                nMapID, stEnterMapRes->nJumpID, &stEnterMapRes->stPosInfo)) {
            // IDA: 定位失败清状态 + LogError 1525，整个函数返回
            pUser->ClearState(eStateChangeWorld);
            LogHelper::LogError("game.contents",
                "SendEnterMaze error - No have World Data[ ActorID:%d, MapID:%d, JumpID:%d ] ( %d )",
                stEnterMapRes->dwUserID,
                static_cast<int>((static_cast<std::uint64_t>(stEnterMapRes->uxMapID.nMapID) >> 16) >> 16),
                stEnterMapRes->nJumpID, 1525);
            return;
        }

        if (stEnterMapRes->dwServerID != XGameServer::Instance()->GetOption().GetServerID()) {
            stEnterMapRes->bChangeServer = true;
        }

        LogHelper::LogDebug("game.contents",
            "<RecvCreateMazeRes> User ( %d ) ( %I64d, %d %d ) bChange ( %d ) ",
            stEnterMapRes->dwUserID,
            stEnterMapRes->uxMapID.nMapID,
            static_cast<int>((static_cast<std::uint64_t>(stEnterMapRes->uxMapID.nMapID) >> 16) >> 16),
            stEnterMapRes->nJumpID, stEnterMapRes->bChangeServer ? 1 : 0);

        // IDA: 广播版把 dwUserID 覆写为当前成员
        stEnterMapRes->dwUserID = pUser->GetActorID().dwActorID;

        {
            XSendDBPacket xSendDBPacket(static_cast<XActor*>(pUser), 3, 0x42);
            xSendDBPacket << *stEnterMapRes;
            xSendDBPacket.XParse << 0;  // nPrevMapID
            xSendDBPacket.XParse << 0;  // nPrevRevivePoint
            XGameServer::Instance()->SendDBGame(xSendDBPacket);
        }

        {
            ST_STATISTICS_MAP_SAVE stInfo = {};
            stInfo.dwUCID = stEnterMapRes->dwUserID;
            stInfo.dwMapID = static_cast<int>(
                (static_cast<std::uint64_t>(stEnterMapRes->uxMapID.nMapID) >> 16) >> 16);
            stInfo.dwServerID = XGameServer::Instance()->GetOption().GetServerID();
            XSendDBPacket xSendDBStatistics(static_cast<XActor*>(pUser), 0xF0, 0x12);
            xSendDBStatistics << stInfo;
            XGameServer::Instance()->SendDBStatistics(xSendDBStatistics);
        }

        {
            ST_LOG_GAME stLog = {};
            stLog._nUAID = static_cast<int>(pUser->GetUAID());
            stLog._nUCID = static_cast<int>(pUser->GetActorID().dwActorID);
            stLog._sMainType = 5;
            stLog._sSubType = 4;
            stLog.nParam0 = static_cast<int>(
                (static_cast<std::uint64_t>(stEnterMapRes->uxMapID.nMapID) >> 16) >> 16);
            stLog.nParam1 = static_cast<int>(m_dwMasterID);
            stLog.nParam3 = GetUserCount();
            stLog.nParam4 = static_cast<int>(m_dwForceID);
            stLog.nParam5 = static_cast<int>(pUser->GetLevel());
            stLog.nParam6 = static_cast<int>(stEnterMapRes->uxMapID.nMapID);
            std::wcscpy(stLog.szComment, L"메이즈 입장");
            XGameServer::Instance()->SendDBLog(stLog);
        }
    }
}

// ============================================================================
// CForce::EnterMazeByForce - IDA @ 0x1401BC080
// 已精确还原 - ReqWorldEnterByForce lambda14 (0x14062DD90) Force 侧调用链。
// 与 CParty::EnterMazeByForce 同构但为独立函数体（publics RVA 0x1BB080）：
// 无效用户 1673 / 已有目标迷宫转发 (0xF3,0x20) / 无 MapID 1691 -> 0xD6DA /
// 表缺失或 Maze_Type 5/11 -> 1684/0xD6D9 / 非队长 1718 -> 0xCF97 /
// GetPortalPos 1736 -> 0xD6DB / 成员遍历（空成员 1746、登录态分 0xCF95/0xCF91、
// WorldID 不同 1757、等级 1764 -> 0xCF8F、灵武 1771 -> 0xCF90、
// 清迷宫 1779 -> 0xCF92、任务 1789 -> 0xCF93、疲劳 0xD704）。
// 注意: 与 CParty 版不同，此处无 CanEnterPortal 检查。
// ============================================================================
bool CForce::EnterMazeByForce(CUser* pReqUser, UXMapID uxMazeID,
                              PS_ENTER_MAP_REQ* stEnterMap) {
    if (!pReqUser) {
        LogHelper::LogError("game.contents",
            "EnterMazeByForce error - Invalid User[ MapID:%d ] ( %d )",
            static_cast<int>(stEnterMap->wMapID), 1673);
        return false;
    }

    UXMapID zeroID{};
    if (uxMazeID.nMapID != zeroID.nMapID) {
        // Per IDA: 目标表缺失或 Maze_Type 5/11 时禁止进入
        TB_MAZE_INFO* pTBMazeInfo = XGameServer::Instance()
            ->GetResourceMgr().GetTB_MAZE_INFO(stEnterMap->wMapID);
        if (!pTBMazeInfo || pTBMazeInfo->Maze_Type == 5
            || pTBMazeInfo->Maze_Type == 11) {
            LogHelper::LogError("game.contents",
                "EnterMazeByForce error - Impossible trespass in maze[ ActorID:%d, MapID:%d ] ( %d )",
                pReqUser->GetActorID().dwActorID,
                static_cast<int>(stEnterMap->wMapID), 1684);
            pReqUser->SendErrorMessage(0x11, 0x41, 0xD6D9);
            return false;
        }
        if (m_uxMazeID.nMapID) {
            // Per IDA: 已在迷宫中时转发控制服 (0xF3, 0x20)（Force 侧子命令）
            XSendPacket xSendPacket(0xF3, 0x20);
            xSendPacket.XParse << m_dwForceID;
            xSendPacket.XParse << m_uxMazeID.nMapID;
            xSendPacket << *stEnterMap;
            XGameServer::Instance()->GetControlSocket().SendCmd(
                &xSendPacket, pReqUser, 0x11, 0x41);
            return true;
        }
        LogHelper::LogError("game.contents",
            "EnterMazeByForce error - Impossible trespass in maze because no MapID[ ActorID:%d, MapID:%d ] ( %d )",
            pReqUser->GetActorID().dwActorID,
            static_cast<int>(stEnterMap->wMapID), 1691);
        pReqUser->SendErrorMessage(0x11, 0x41, 0xD6DA);
        return false;
    }

    // Per IDA: 清空准备列表
    m_vecReadyToMazeMember.clear();
    m_setAgreeToMazeMember.clear();

    TB_MAZE_INFO* pMazeData = XGameServer::Instance()
        ->GetResourceMgr().GetTB_MAZE_INFO(stEnterMap->wMapID);
    if (!pMazeData) {
        LogHelper::LogError("game.contents",
            "EnterMaze error - No Table TB_MAZE_INFO[ ActorID:%d, TBID:%d ] ( %d )",
            pReqUser->GetActorID().dwActorID,
            static_cast<int>(stEnterMap->wMapID), 1711);
        pReqUser->SendErrorMessage(0x11, 0x41, 0xD6D9);
        return false;
    }

    // Per IDA: 0x14002F250 (CWayPoint::GetCurID ICF) == 队长判定
    if (m_dwMasterID != CQuestCondition::GetQuestID(pReqUser->GetActorID())) {
        LogHelper::LogError("game.contents",
            "EnterMaze error - Create maze only party master[ ActorID:%d] ( %d )",
            pReqUser->GetActorID().dwActorID, 1718);
        pReqUser->SendErrorMessage(0x11, 0x41, 0xCF97);
        return false;
    }

    // Per IDA: GetPortalPos 直接以 &m_stNextMovePos 为出参
    if (!XGameServer::Instance()->GetWorldResMgr().GetPortalPos(
            stEnterMap->wMapID, stEnterMap->nJumpID, &m_stNextMovePos)) {
        LogHelper::LogError("game.contents",
            "EnterMaze error - Cant find portal area when create maze[ ActorID:%d , MapID:%d, JumpID:%d ] ( %d )",
            pReqUser->GetActorID().dwActorID,
            static_cast<int>(stEnterMap->wMapID), stEnterMap->nJumpID, 1736);
        pReqUser->SendErrorMessage(0x11, 0x41, 0xD6DB);
        return false;
    }

    // Per IDA: 成员准备登记 + 同意集合
    for (auto& kv : m_mapForceMember) {
        if (!kv.second) {
            LogHelper::LogError("game.contents",
                "EnterMaze error - No ForceMember In Force[ %d  ( %d )",
                m_dwForceID, 1746);
            pReqUser->SendErrorMessage(0x11, 0x41, 0xD6DB);
            return false;
        }
        CUser* pMember = XGameServer::Instance()->FindActorIDToUser(kv.first);
        if (!pMember) {
            if (kv.second->IsLogin()) {
                LogHelper::LogDebug("game.party",
                    "EnterMaze error - ERROR_MAZE_FAILED_ENTER_WRONG_AREA_FORCE_MEMBER");
                pReqUser->SendErrorMessage(0x11, 0x41, 0xCF91);
            } else {
                LogHelper::LogDebug("game.party",
                    "EnterMaze error - ERROR_MAZE_FAILED_ENTER_LOGOUT_FORCE_MEMBER");
                pReqUser->SendErrorMessage(0x11, 0x41, 0xCF95);
            }
            return false;
        }
        if (pMember->GetWorldID() != pReqUser->GetWorldID()) {
            LogHelper::LogDebug("game.party",
                "EnterMaze error - ERROR_MAZE_FAILED_ENTER_WRONG_AREA_FORCE_MEMBER[ ActorID:%d ] ( %d )",
                pReqUser->GetActorID().dwActorID, 1757);
            pReqUser->SendErrorMessage(0x11, 0x41, 0xCF91);
            return false;
        }
        if (pMember->GetLevel() < pMazeData->Req_Min_Lv) {
            LogHelper::LogError("game.contents",
                "EnterMaze error - Force member is low level ( %d )", 1764);
            pReqUser->SendErrorMessage(0x11, 0x41, 0xCF8F);
            return false;
        }
        if (pMember->stMyCharInfoEx()->stSoulWeapon.dwItemID == -1) {
            LogHelper::LogError("game.contents",
                "EnterMaze error - No equip weapon[ ActorID:%d ] ( %d )",
                pMember->GetActorID().dwActorID, 1771);
            pReqUser->SendErrorMessage(0x11, 0x41, 0xCF90);
            return false;
        }
        CGocRecode* pRecode = pMember->GetGOC<CGocRecode>();
        if (!pRecode || !pRecode->IsClearMaze(pMazeData->Check_Clear_Maze)) {
            LogHelper::LogError("game.contents",
                "EnterMaze error - Failed Maze Episode No[ ActorID:%d ] ( %d )",
                pMember->GetActorID().dwActorID, 1779);
            pReqUser->SendErrorMessage(0x11, 0x41, 0xCF92);
            return false;
        }
        if (pMazeData->NeedQuest_ID) {
            CGocQuest* pQuest = pMember->GetGOC<CGocQuest>();
            if (!pQuest
                || (!pQuest->FindEpisode(pMazeData->NeedQuest_ID)
                    && !pQuest->IsCompleteEpisode(pMazeData->NeedQuest_ID))) {
                LogHelper::LogError("game.contents",
                    "EnterMaze error - No Have Quest To Enter Maze[ ActorID:%d ] ( %d )",
                    pMember->GetActorID().dwActorID, 1789);
                pReqUser->SendErrorMessage(0x11, 0x41, 0xCF93);
                return false;
            }
        }
        if (pMazeData->Fatigue_Point) {
            CGocAttribute* pAttr = pMember->GetGOC<CGocAttribute>();
            if (!pAttr || !pAttr->CanUseFP(pMazeData->Fatigue_Point)) {
                pReqUser->SendErrorMessage(4, 1, 0xD704,
                    CQuestCondition::GetQuestID(pMember->GetActorID()));
                return false;
            }
        }

        // Per IDA: 成员准备登记 (nState = GetNetCafe ? 1 : 0)
        ST_ENTER_MAZE_MEMBER_INFO stMemberInfo{};
        stMemberInfo.dwMember = CQuestCondition::GetQuestID(
            pMember->GetActorID());
        CGocEntity* pEntity = pMember->GetGOC<CGocEntity>();
        stMemberInfo.nState = pEntity ? (pEntity->GetNetCafe() ? 1 : 0) : 0;
        m_vecReadyToMazeMember.push_back(stMemberInfo);
        if (m_dwMasterID != stMemberInfo.dwMember)
            m_setAgreeToMazeMember.insert(stMemberInfo.dwMember);
    }

    // Per IDA: CForce::SetEnterMazeRequst (0x1401BAEF0)
    SetEnterMazeRequst(*stEnterMap);

    // Per IDA: >1 成员时向所有成员逐个登记响应 (SetOutputState(2) ICF 标签)
    if (m_vecReadyToMazeMember.size() > 1) {
        for (auto& kv : m_mapForceMember) {
            auto it = m_mapForceMember.find(kv.first);
            if (it != m_mapForceMember.end() && it->second) {
                it->second->SetPartyMemberState(2);
                SetEnterMazeResponse(it->first);
            }
        }
    }
    return true;
}

// ============================================================================
// CForce::SetEnterMazeResponse - IDA @ 0x1401BB340
// 已精确还原 - 有进行中请求时从同意集合移除该成员；
// 移除后集合为空返回 true；仍有未同意成员时 AgreeEnterMaze 后返回 false。
// Force 侧独立函数体（publics RVA 0x1BA340，非 CParty COMDAT）。
// ============================================================================
bool CForce::SetEnterMazeResponse(std::uint32_t dwAgreeActor) {
    if (!m_stEnterMazeRequst.stMazeInfo.wMapID)
        return false;

    if (m_setAgreeToMazeMember.empty())
        return false;

    auto it = m_setAgreeToMazeMember.find(dwAgreeActor);
    if (it != m_setAgreeToMazeMember.end())
        m_setAgreeToMazeMember.erase(it);

    if (m_setAgreeToMazeMember.empty())
        return true;

    AgreeEnterMaze(dwAgreeActor);
    return false;
}

// ============================================================================
// CForce::AgreeEnterMaze - IDA @ 0x1401BB1D0
// 已精确还原 - 遍历 m_vecReadyToMazeMember，向每个在线成员
// 发送 (0x11, 0x4F) 包，负载为 dwAgreeActor（Force 侧子命令 0x4F，
// CParty 侧为 0x4A）。
// ============================================================================
void CForce::AgreeEnterMaze(std::uint32_t dwAgreeActor) {
    for (const auto& stMemberInfo : m_vecReadyToMazeMember) {
        CUser* pUser = XGameServer::Instance()->FindActorIDToUser(
            stMemberInfo.dwMember);
        if (pUser) {
            XSendPacket xSendPacket(0x11, 0x4F);
            xSendPacket.XParse << dwAgreeActor;
            CGocNetwork::Send(static_cast<XActor*>(pUser), xSendPacket);
        }
    }
}

// ============================================================================
// CForce::CreateMazeReq - IDA @ 0x1401BBC90
// 已精确还原 - 构造 ST_CREATE_MAZE（byGroupType=2, nID=m_dwForceID）并经
// XRelaySocket::SendCreateMazeReq (0xF2,0x21) 发往 ControlServer。
// ============================================================================
void CForce::CreateMazeReq() {
    ST_CREATE_MAZE stCreateMaze{};
    stCreateMaze.dwUserID = m_stEnterMazeRequst.stMazeInfo.dwActorID;
    stCreateMaze.dwUAID = m_stEnterMazeRequst.stMazeInfo.dwUAID;
    stCreateMaze.wReqMapID = m_stEnterMazeRequst.stMazeInfo.wMapID;
    stCreateMaze.nJumpID = m_stEnterMazeRequst.stMazeInfo.nJumpID;
    stCreateMaze.nPortalID = m_stEnterMazeRequst.stMazeInfo.nPortalID;
    stCreateMaze.dwUserID = m_dwMasterID;
    stCreateMaze.stPartyInfo.byGroupType = 2;
    stCreateMaze.stPartyInfo.nID = m_dwForceID;
    stCreateMaze.vecEnterMember.assign(
        m_vecReadyToMazeMember.begin(), m_vecReadyToMazeMember.end());
    XGameServer::Instance()->GetControlSocket().SendCreateMazeReq(stCreateMaze);
}

// ============================================================================
// CForce::CancelEnterMaze - IDA @ 0x1401BB030
// 已精确还原 - 清空进行中的进迷宫请求（wMapID=0），向所有已准备成员
// 广播 (0x11, 0x4D) 取消包（负载 dwCancelActor），最后清空准备列表。
// ============================================================================
void CForce::CancelEnterMaze(std::uint32_t dwCancelActor) {
    m_stEnterMazeRequst.stMazeInfo.wMapID = 0;
    for (const auto& stMemberInfo : m_vecReadyToMazeMember) {
        CUser* pUser = XGameServer::Instance()->FindActorIDToUser(
            stMemberInfo.dwMember);
        if (pUser) {
            XSendPacket xSendPacket(0x11, 0x4D);
            xSendPacket.XParse << dwCancelActor;
            CGocNetwork::Send(static_cast<XActor*>(pUser), xSendPacket);
        }
    }
    m_vecReadyToMazeMember.clear();
}
