// CForce.cpp
// CForce and CForceMember implementation
// 对齐 IDA GameServer.exe

#include "CForce.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMapMaze.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocQuest.h"
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
