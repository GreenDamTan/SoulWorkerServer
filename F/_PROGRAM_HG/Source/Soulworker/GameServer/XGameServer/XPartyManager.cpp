// XPartyManager.cpp
// XPartyManager - Party Manager for GameServer
// IDA decompilation from GameServer.exe

#include "XPartyManager.h"
#include "CParty.h"
#include "User.h"
#include "GameServer.h"
#include "Soulworker/GameServer/XCore/XServer/XSendPacket.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocParty.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocForce.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerParty.h"
#include <memory>

// IDA: ??0XPartyManager@@QEAA@XZ @ 0x1403AC640
XPartyManager::XPartyManager() {
    m_mapPartyInfo.clear();
    m_mapPartyUserInfo.clear();
    m_dwSeqNo = 0;
}

// IDA: ??1XPartyManager@@UEAA@XZ @ 0x1403AC710
XPartyManager::~XPartyManager() {
    m_mapPartyUserInfo.clear();
    m_mapPartyInfo.clear();
}

// IDA: ?CreateParty@XPartyManager@@QEAAKAEAUPS_REQ_PARTY_CREATE@@@Z @ 0x1403AC790
std::uint32_t XPartyManager::CreateParty(PS_REQ_PARTY_CREATE& stCreateParty) {
    std::uint32_t dwPartyID = stCreateParty.dwPartyID;
    if (!dwPartyID) {
        return 0;
    }

    // Check if party already exists
    auto it = m_mapPartyInfo.find(dwPartyID);
    if (it != m_mapPartyInfo.end()) {
        // Party already exists
        return dwPartyID;
    }

    // Create new party
    auto pParty = std::make_shared<CParty>();
    if (!pParty) {
        return 0;
    }

    // Initialize party with master info
    pParty->Create(dwPartyID, stCreateParty.masterInfo);

    // Add first member
    pParty->AddMember(stCreateParty.memberInfo, nullptr);

    // Store party in map
    m_mapPartyInfo[dwPartyID] = pParty;

    // Update user-party mapping
    m_mapPartyUserInfo[stCreateParty.masterInfo.dwMemberID] = dwPartyID;
    m_mapPartyUserInfo[stCreateParty.memberInfo.dwMemberID] = dwPartyID;

    return dwPartyID;
}

// IDA: ?CreateParty@XPartyManager@@QEAA_NPEAVCUser@@K@Z @ 0x1403AC970
bool XPartyManager::CreateParty(CUser* pMember, std::uint32_t dwPartyID) {
    if (!pMember || !dwPartyID) {
        return false;
    }

    // Find party
    auto it = m_mapPartyInfo.find(dwPartyID);
    if (it == m_mapPartyInfo.end()) {
        return false;
    }

    auto& pParty = it->second;
    if (!pParty) {
        return false;
    }

    // Get user's party component
    auto pGocParty = pMember->GetGOC<CGocParty>();
    if (!pGocParty) {
        return false;
    }

    // Set party reference in user's component
    pGocParty->SetForce(pParty);
    pGocParty->SendPartyInfo(1);

    // TODO: Check if in maze and sync HP/update member info
    // This requires XArea::IsMaze() and other maze-related APIs

    // TODO: Send PS_UPDATE_USER_MAP_INFO to control server
    // This requires XGameServer and control socket access

    return true;
}

// IDA: ?AddMember@XPartyManager@@QEAA_NPEAVCUser@@AEAUPS_PARTY_ADDMEMBER@@AEAUPS_PARTY_INFO@@@Z @ 0x1403AD4F0
bool XPartyManager::AddMember(CUser* pMember, PS_PARTY_ADDMEMBER& stAddMember, PS_PARTY_INFO& stPartyInfo) {
    // Find party
    auto it = m_mapPartyInfo.find(stAddMember.dwPartyID);

    if (it != m_mapPartyInfo.end()) {
        // Party exists, add member to existing party
        auto& pParty = it->second;
        if (!pParty) {
            return false;
        }

        // Add member to party
        pParty->AddMember(stAddMember, pMember);

        // Update user's party component
        auto pGocParty = pMember->GetGOC<CGocParty>();
        if (pGocParty) {
            pGocParty->SetForce(pParty);
        }

        // Update user-party mapping
        // Remove old mapping if exists
        auto userIt = m_mapPartyUserInfo.find(stAddMember.dwMemberID);
        if (userIt != m_mapPartyUserInfo.end()) {
            m_mapPartyUserInfo.erase(userIt);
        }

        // Add new mapping
        m_mapPartyUserInfo[stAddMember.dwMemberID] = stAddMember.dwPartyID;

        // Send party info to member
        pParty->SendPartyInfo(pMember, 2);
    } else {
        // Party doesn't exist, create new party from stPartyInfo
        auto pParty = std::make_shared<CParty>();
        if (!pParty) {
            return false;
        }

        // Create party with info
        pParty->Create(stPartyInfo.dwPartyID, stPartyInfo.vecPartyMember.empty() ?
            ST_PARTY_MEMBER{} : stPartyInfo.vecPartyMember[0]);

        // Store party
        m_mapPartyInfo[stPartyInfo.dwPartyID] = pParty;

        // Add all members from party info to user mapping
        for (const auto& member : stPartyInfo.vecPartyMember) {
            m_mapPartyUserInfo[member.dwMemberID] = stPartyInfo.dwPartyID;
        }

        // Update user's party component
        auto pGocParty = pMember->GetGOC<CGocParty>();
        if (pGocParty) {
            pGocParty->SetForce(pParty);
        }
    }

    return true;
}

// IDA: ?AddMember@XPartyManager@@QEAA_NAEAUPS_PARTY_ADDMEMBER@@@Z @ 0x1403AD960
bool XPartyManager::AddMember(PS_PARTY_ADDMEMBER& stAddMember) {
    // Find party
    auto it = m_mapPartyInfo.find(stAddMember.dwPartyID);
    if (it == m_mapPartyInfo.end()) {
        return false;
    }

    auto& pParty = it->second;
    if (!pParty) {
        return false;
    }

    // Add member to party
    pParty->AddMember(stAddMember, nullptr);

    // Update user-party mapping
    // Remove old mapping if exists
    auto userIt = m_mapPartyUserInfo.find(stAddMember.dwMemberID);
    if (userIt != m_mapPartyUserInfo.end()) {
        m_mapPartyUserInfo.erase(userIt);
    }

    // Add new mapping
    m_mapPartyUserInfo[stAddMember.dwMemberID] = stAddMember.dwPartyID;

    // Send packet to party members
    XSendPacket xSendPacket(0x12, 0x10);
    xSendPacket << stAddMember;
    pParty->Send(xSendPacket, stAddMember.dwMemberID);

    return true;
}

// IDA: ?ChangeMaster@XPartyManager@@QEAA_NKTUXActorID@@@Z @ 0x1403ADB90
bool XPartyManager::ChangeMaster(std::uint32_t dwPartyID, UXActorID uNewMasterActorID) {
    // Validate new master actor ID
    if (uNewMasterActorID.dwActorID == 0xFFFFFFFF) {
        return false;
    }

    // Find party
    auto it = m_mapPartyInfo.find(dwPartyID);
    if (it == m_mapPartyInfo.end()) {
        return false;
    }

    auto& pParty = it->second;
    if (!pParty) {
        return false;
    }

    // Change master in party
    if (!pParty->ChangeMaster(uNewMasterActorID.dwActorID)) {
        return false;
    }

    // Send change master packet
    XSendPacket xSendPacket(0x12, 0x03);
    xSendPacket << uNewMasterActorID.dwActorID;
    pParty->Send(xSendPacket, 0);

    return true;
}

// IDA: ?LeaveParty@XPartyManager@@QEAA_NAEAUPS_PARTY_LEAVE@@K@Z @ 0x1403ADD40
bool XPartyManager::LeaveParty(PS_PARTY_LEAVE& stPartyLeave, std::uint32_t dwNewMaster) {
    // Find party
    auto it = m_mapPartyInfo.find(stPartyLeave.dwExitUCID);
    if (it == m_mapPartyInfo.end()) {
        return false;
    }

    auto& pParty = it->second;
    if (!pParty) {
        return false;
    }

    // Get member that is leaving
    auto pMember = pParty->GetMember(stPartyLeave.dwExitUAID);
    if (pMember) {
        auto pUser = pMember->GetMember();
        if (pUser) {
            // TODO: Check if in maze and handle maze-related logic
            // Remove party booster
            pUser->ChangeBooster(eBooster_Type_Party, 0);
        }
    }

    // Change master if needed
    if (dwNewMaster) {
        if (pParty->ChangeMaster(dwNewMaster)) {
            XSendPacket xSendPacket(0x12, 0x03);
            xSendPacket << dwNewMaster;
            pParty->Send(xSendPacket, 0);
        }
    }

    // Send leave packet
    XSendPacket xSendPacket(0x12, 0x05);
    xSendPacket << stPartyLeave;
    pParty->Send(xSendPacket, 0);

    // Kick user from party
    pParty->UserKickOut(stPartyLeave.dwExitUAID);

    // Remove from user-party mapping
    auto userIt = m_mapPartyUserInfo.find(stPartyLeave.dwExitUAID);
    if (userIt != m_mapPartyUserInfo.end()) {
        m_mapPartyUserInfo.erase(userIt);
    }

    // Update monster level stat
    pParty->ChangeMonsterLevelStat(false);

    return true;
}

// IDA: ?DeleteParty@XPartyManager@@QEAA_NAEAUPS_PARTY_LEAVE@@@Z @ 0x1403AE1A0
bool XPartyManager::DeleteParty(PS_PARTY_LEAVE& stPartyLeave) {
    // Find party
    auto it = m_mapPartyInfo.find(stPartyLeave.dwExitUCID);
    if (it == m_mapPartyInfo.end()) {
        return false;
    }

    auto& pParty = it->second;
    if (!pParty) {
        return false;
    }

    // Send leave packet if exit UAID is valid
    if (stPartyLeave.dwExitUAID != 0xFFFFFFFF) {
        XSendPacket xSendPacket(0x12, 0x05);
        xSendPacket << stPartyLeave;
        pParty->Send(xSendPacket, 0);

        // Get leaving member
        auto pMember = pParty->GetMember(stPartyLeave.dwExitUAID);
        if (pMember) {
            auto pUser = pMember->GetMember();
            if (pUser) {
                // TODO: Check if in maze and handle appropriately
            }
        }
    }

    // Remove force booster
    pParty->RemoveForceBooster();

    // Change monster level stat
    pParty->ChangeMonsterLevelStat(true);

    // Send delete party packet
    XSendPacket xSendPacket(0x12, 0x07);
    xSendPacket << stPartyLeave.dwExitUCID;
    pParty->Send(xSendPacket, 0);

    // Get all member IDs
    std::vector<std::uint32_t> vecMember;
    pParty->Enumerate(vecMember);

    // Remove all members from user-party mapping
    for (std::uint32_t dwMemberID : vecMember) {
        m_mapPartyUserInfo.erase(dwMemberID);
    }

    // Get maze ID and clear party from maze
    // TODO: XMaze::ClearParty() when maze system is implemented
    // UXMapID uxMazeID = pParty->GetMazeID();

    // Clear party
    pParty->Clear();

    // Remove party from map
    m_mapPartyInfo.erase(it);

    return true;
}

// IDA: ?GetPartyID@XPartyManager@@QEAAKTUXActorID@@@Z @ 0x1403AE6D0
std::uint32_t XPartyManager::GetPartyID(UXActorID uActorID) {
    auto it = m_mapPartyUserInfo.find(uActorID);
    if (it == m_mapPartyUserInfo.end()) {
        return 0;
    }
    return it->second;
}

// IDA: ?GetUserCount@XPartyManager@@QEAAEK@Z @ 0x1403AE840
std::uint8_t XPartyManager::GetUserCount(std::uint32_t dwPartyID) {
    auto it = m_mapPartyInfo.find(dwPartyID);
    if (it == m_mapPartyInfo.end()) {
        return 1; // Return 1 if party not found (per IDA behavior)
    }

    auto& pParty = it->second;
    if (!pParty) {
        return 1;
    }

    return pParty->GetUserCount();
}

// IDA: ?IsPartyUser@XPartyManager@@QEAA_NTUXActorID@@@Z @ 0x1403AE8C0
bool XPartyManager::IsPartyUser(UXActorID uActorID) {
    return m_mapPartyUserInfo.find(uActorID) != m_mapPartyUserInfo.end();
}

// IDA: ?IsMasterUser@XPartyManager@@QEAA_NTUXActorID@@@Z @ 0x1403AE930
bool XPartyManager::IsMasterUser(UXActorID uActorID) {
    // Find user's party ID
    auto userIt = m_mapPartyUserInfo.find(uActorID);
    if (userIt == m_mapPartyUserInfo.end()) {
        return false;
    }

    // Find party
    auto partyIt = m_mapPartyInfo.find(userIt->second);
    if (partyIt == m_mapPartyInfo.end()) {
        return false;
    }

    auto& pParty = partyIt->second;
    if (!pParty) {
        return false;
    }

    // Check if actor ID matches master ID
    return pParty->GetMasterID() == uActorID.dwActorID;
}

// IDA: ?ReqPartyEnterServer@XPartyManager@@QEAA_NPEAVCUser@@K@Z @ 0x1403AEA20
bool XPartyManager::ReqPartyEnterServer(CUser* pReqUser, std::uint32_t dwPartyID) {
    if (!pReqUser) {
        return false;
    }

    // Build enter server request
    PS_REQ_PARTY_ENTER_SERVER stEnterServer{};
    stEnterServer.dwMemberID = pReqUser->GetActorID().dwActorID;
    stEnterServer.dwPartyID = dwPartyID;
    stEnterServer.uxMapID = pReqUser->GetMapInsID();
    stEnterServer.nMaxHP = pReqUser->GetMaxHP();
    stEnterServer.bReqPartyInfo = 1;

    // Send to control server
    XSendPacket xSendPacket(0xF4, 0x10);
    xSendPacket << stEnterServer;

    auto pGameServer = TXSingleton<XGameServer>::Instance();
    if (pGameServer) {
        pGameServer->GetControlSocket().SendCheck(xSendPacket);
    }

    return true;
}

// IDA: ?ResEnterMaze@XPartyManager@@QEAAXPEAVCUser@@KAEAUPS_ENTER_MAP_RES@@@Z @ 0x1403AEB80
void XPartyManager::ResEnterMaze(CUser* pUser, std::uint32_t dwPartyID, PS_ENTER_MAP_RES& stMazeInfo) {
    if (!pUser) {
        GreenDamTan_log("game.contents", "<%d PARTY> ResEnterMaze Invalid User (%lld)",
            dwPartyID, stMazeInfo.uxMapID.nMapID);
        return;
    }

    // Find party
    auto it = m_mapPartyInfo.find(dwPartyID);
    if (it == m_mapPartyInfo.end()) {
        return;
    }

    auto& pParty = it->second;
    if (!pParty) {
        return;
    }

    // Handle maze enter response
    pParty->ResEnterMaze(pUser, &stMazeInfo);

    GreenDamTan_log("game.contents", "<%d PARTY> ResEnterMaze (UCID : %d / %lld)",
        dwPartyID, pUser->GetActorID().dwActorID, stMazeInfo.uxMapID.nMapID);
}

// IDA: ?ResPartyUpdateInfo@XPartyManager@@QEAAXKTUXMapID@@@Z @ 0x1403AECB0
void XPartyManager::ResPartyUpdateInfo(std::uint32_t dwPartyID, UXMapID uxMazeID) {
    // Find party
    auto it = m_mapPartyInfo.find(dwPartyID);
    if (it == m_mapPartyInfo.end()) {
        return;
    }

    auto& pParty = it->second;
    if (!pParty) {
        return;
    }

    // Set maze ID
    pParty->SetMazeID(uxMazeID);
}

// IDA: ?UpdateMemberInfo@XPartyManager@@QEAAXAEAUST_UPDATE_PARTY_MEMBER@@@Z @ 0x1403AED50
void XPartyManager::UpdateMemberInfo(ST_UPDATE_PARTY_MEMBER& stUpdateMember) {
    // Find party
    auto it = m_mapPartyInfo.find(stUpdateMember.dwPartyID);
    if (it == m_mapPartyInfo.end()) {
        return;
    }

    auto& pParty = it->second;
    if (!pParty) {
        return;
    }

    // Update member info in party
    pParty->UpdateMemberInfo(stUpdateMember);

    // Send update packet to party members
    XSendPacket xSendPacket(0x12, 0x06);
    xSendPacket << stUpdateMember;
    pParty->Send(xSendPacket, 0);

    GreenDamTan_log("game.contents", "<%d PARTY> UpdateMember (UCID : %d / %d)",
        stUpdateMember.dwPartyID, stUpdateMember.stPartyMember.dwMemberID,
        stUpdateMember.stPartyMember.nMapID);
}

// IDA: ?RecvPartyMazeClear@XPartyManager@@QEAAXK@Z @ 0x1403AEED0
void XPartyManager::RecvPartyMazeClear(std::uint32_t dwPartyID) {
    // Find party
    auto it = m_mapPartyInfo.find(dwPartyID);
    if (it == m_mapPartyInfo.end()) {
        return;
    }

    auto& pParty = it->second;
    if (!pParty) {
        return;
    }

    // Set force type to 0 (normal)
    pParty->SetForceType(0);

    // Send maze clear packet
    XSendPacket xSendPacket(0x12, 0x35);
    pParty->Send(xSendPacket, 0);
}
