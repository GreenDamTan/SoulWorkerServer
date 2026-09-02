// XPartyManager.h
// XPartyManager - Party Manager for GameServer
// IDA decompilation from GameServer.exe
//
// Functions:
// - XPartyManager::XPartyManager (0x1403ac640)
// - XPartyManager::~XPartyManager (0x1403ac710)
// - XPartyManager::CreateParty (0x1403ac790, 0x1403ac970)
// - XPartyManager::AddMember (0x1403ad4f0, 0x1403ad960)
// - XPartyManager::ChangeMaster (0x1403adb90)
// - XPartyManager::LeaveParty (0x1403add40)
// - XPartyManager::DeleteParty (0x1403ae1a0)
// - XPartyManager::GetPartyID (0x1403ae6d0)
// - XPartyManager::GetUserCount (0x1403ae840)
// - XPartyManager::IsPartyUser (0x1403ae8c0)
// - XPartyManager::IsMasterUser (0x1403ae930)
// - XPartyManager::ReqPartyEnterServer (0x1403aea20)
// - XPartyManager::ResEnterMaze (0x1403aeb80)
// - XPartyManager::ResPartyUpdateInfo (0x1403aecb0)
// - XPartyManager::UpdateMemberInfo (0x1403aed50)
// - XPartyManager::RecvPartyMazeClear (0x1403aeed0)

#pragma once

#include <cstdint>
#include <map>
#include <memory>

#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerParty.h"

// Forward declarations
class CParty;
class CUser;
class XSendPacket;
struct PS_REQ_PARTY_CREATE;
struct PS_PARTY_ADDMEMBER;
struct PS_PARTY_INFO;
struct PS_PARTY_LEAVE;
struct PS_ENTER_MAP_RES;
struct ST_UPDATE_PARTY_MEMBER;

// ============================================================================
// XPartyManager - Party manager for GameServer
// Manages party creation, member management, and party operations
// ============================================================================
class XPartyManager {
public:
    // === Constructor / Destructor ===

    // IDA: ??0XPartyManager@@QEAA@XZ @ 0x1403AC640
    XPartyManager();

    // IDA: ??1XPartyManager@@UEAA@XZ @ 0x1403AC710
    virtual ~XPartyManager();

    // === Party Creation ===

    // IDA: ?CreateParty@XPartyManager@@QEAAKAEAUPS_REQ_PARTY_CREATE@@@Z @ 0x1403AC790
    // Create party from request packet
    std::uint32_t CreateParty(PS_REQ_PARTY_CREATE& stCreateParty);

    // IDA: ?CreateParty@XPartyManager@@QEAA_NPEAVCUser@@K@Z @ 0x1403AC970
    // Create party with member user and party ID
    bool CreateParty(CUser* pMember, std::uint32_t dwPartyID);

    // === Member Management ===

    // IDA: ?AddMember@XPartyManager@@QEAA_NPEAVCUser@@AEAUPS_PARTY_ADDMEMBER@@AEAUPS_PARTY_INFO@@@Z @ 0x1403AD4F0
    // Add member to party with user context
    bool AddMember(CUser* pMember, PS_PARTY_ADDMEMBER& stAddMember, PS_PARTY_INFO& stPartyInfo);

    // IDA: ?AddMember@XPartyManager@@QEAA_NAEAUPS_PARTY_ADDMEMBER@@@Z @ 0x1403AD960
    // Add member to party without user context
    bool AddMember(PS_PARTY_ADDMEMBER& stAddMember);

    // IDA: ?ChangeMaster@XPartyManager@@QEAA_NKTUXActorID@@@Z @ 0x1403ADB90
    // Change party master
    bool ChangeMaster(std::uint32_t dwPartyID, UXActorID uNewMasterActorID);

    // IDA: ?LeaveParty@XPartyManager@@QEAA_NAEAUPS_PARTY_LEAVE@@K@Z @ 0x1403ADD40
    // Handle member leaving party
    bool LeaveParty(PS_PARTY_LEAVE& stPartyLeave, std::uint32_t dwNewMaster);

    // IDA: ?DeleteParty@XPartyManager@@QEAA_NAEAUPS_PARTY_LEAVE@@@Z @ 0x1403AE1A0
    // Delete party
    bool DeleteParty(PS_PARTY_LEAVE& stPartyLeave);

    // === Party Query ===

    // IDA: ?GetPartyID@XPartyManager@@QEAAKTUXActorID@@@Z @ 0x1403AE6D0
    // Get party ID by actor ID
    std::uint32_t GetPartyID(UXActorID uActorID);

    // IDA: ?GetParty@XPartyManager@@QEAA?AV?$shared_ptr@VCParty@@@tr1@std@@K@Z @ 0x1403AE740
    // Get party instance by party ID (empty shared_ptr if not found)
    std::shared_ptr<CParty> GetParty(std::uint32_t dwPartyID);

    // IDA: ?GetParty@XPartyManager@@QEAA?AV?$shared_ptr@VCParty@@@tr1@std@@TUXActorID@@@Z @ 0x1403AE7F0
    // Get party instance by actor ID (delegates to GetPartyID then by-ID overload)
    std::shared_ptr<CParty> GetParty(UXActorID uActorID);

    // IDA: ?GetUserCount@XPartyManager@@QEAAEK@Z @ 0x1403AE840
    // Get member count for party
    std::uint8_t GetUserCount(std::uint32_t dwPartyID);

    // IDA: ?IsPartyUser@XPartyManager@@QEAA_NTUXActorID@@@Z @ 0x1403AE8C0
    // Check if user is in any party
    bool IsPartyUser(UXActorID uActorID);

    // IDA: ?IsMasterUser@XPartyManager@@QEAA_NTUXActorID@@@Z @ 0x1403AE930
    // Check if user is party master
    bool IsMasterUser(UXActorID uActorID);

    // === Server Communication ===

    // IDA: ?ReqPartyEnterServer@XPartyManager@@QEAA_NPEAVCUser@@K@Z @ 0x1403AEA20
    // Request party enter server
    bool ReqPartyEnterServer(CUser* pReqUser, std::uint32_t dwPartyID);

    // IDA: ?ResEnterMaze@XPartyManager@@QEAAXPEAVCUser@@KAEAUPS_ENTER_MAP_RES@@@Z @ 0x1403AEB80
    // Response to maze enter
    void ResEnterMaze(CUser* pUser, std::uint32_t dwPartyID, PS_ENTER_MAP_RES& stMazeInfo);

    // IDA: ?ResPartyUpdateInfo@XPartyManager@@QEAAXKTUXMapID@@@Z @ 0x1403AECB0
    // Update party info with maze ID
    void ResPartyUpdateInfo(std::uint32_t dwPartyID, UXMapID uxMazeID);

    // IDA: ?UpdateMemberInfo@XPartyManager@@QEAAXAEAUST_UPDATE_PARTY_MEMBER@@@Z @ 0x1403AED50
    // Update member info
    void UpdateMemberInfo(ST_UPDATE_PARTY_MEMBER& stUpdateMember);

    // IDA: ?RecvPartyMazeClear@XPartyManager@@QEAAXK@Z @ 0x1403AEED0
    // Handle maze clear notification
    void RecvPartyMazeClear(std::uint32_t dwPartyID);

private:
    // === IDA confirmed member variables ===
    // m_mapPartyInfo: std::map<unsigned long, std::tr1::shared_ptr<CParty>>
    std::map<std::uint32_t, std::shared_ptr<CParty>> m_mapPartyInfo;

    // m_mapPartyUserInfo: std::map<UXActorID, unsigned long>
    std::map<UXActorID, std::uint32_t> m_mapPartyUserInfo;

    // m_PartyPool: ClassFactory<CParty, 64> - placeholder
    // Using simple allocation instead of object pool
    std::uint8_t m_PartyPool[64] = {};

    // m_dwSeqNo: sequence number for party operations
    std::uint32_t m_dwSeqNo = 0;
};
