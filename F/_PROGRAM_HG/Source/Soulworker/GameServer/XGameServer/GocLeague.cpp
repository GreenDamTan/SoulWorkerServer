// Guild/League System Implementation
// Decompiled from IDA GameServer.exe
// Target: GameServer.exe
// Port: 10004

#include "GocLeague.h"
#include "User.h"
#include "LeagueMember.h"
#include "..\XSCommon\Packet.h"
#include "..\XSCommon\XSendPacket.h"
#include "..\XSCommon\GocNetwork.h"
#include "..\XGameServer\XGameServer.h"
#include "..\XGameServer\CommunitySocket.h"
#include "..\XGameServer\ThreadLocalData.h"
#include "..\XGameServer\LogicThreadManager.h"
#include "..\XSCommon\STDef.h"
#include <cstring>

// ============================================================================
// CGocLeague Implementation
// ============================================================================

// Address: 0x1400FA870
CGocLeague::CGocLeague()
{
    // Constructor - Hidden C++ exception states
    // Initialize base GOComponent
}

// Address: 0x1400FA8B0
CGocLeague::~CGocLeague()
{
    // Destructor
}

// Address: 0x1400FA920
void CGocLeague::Init()
{
    // Initialize guild component
    // Clear member data and reset invite state
    if (m_stLeagueMember)
    {
        memset(m_stLeagueMember, 0, sizeof(CLeagueMember));
    }
    m_dwInviteActorID = 0;
}

// Address: 0x1402AC5A0
int CGocLeague::GetFamilyID()
{
    // Static getter for family ID (constant)
    return 0; // TODO: Determine actual constant value
}

// ============================================================================
// CLeagueMember Implementation
// ============================================================================

// Address: 0x14028A5C0
CLeagueMember::CLeagueMember()
{
    // Constructor - Hidden C++ exception states
    // Initialize member map
    Clear();
}

// Address: 0x14028A600
void CLeagueMember::Clear()
{
    // Clear all guild members
    m_mapMembers.clear();
    m_nSyncCount = 0;
    m_nInventorySyncCount = 0;
}

// Address: 0x14028A630
void CLeagueMember::AddLeagueMember(CUser* pUser)
{
    // Add a guild member to tracking map
    if (!pUser)
        return;
    
    // Get actor ID
    UXActorID actorID;
    // TODO: Call pUser->GetActorID(&actorID);
    
    // Insert into member map
    m_mapMembers[actorID.dwActorID] = pUser;
}

// Address: 0x14028A930
void CLeagueMember::JoinLeagueUser(
    ST_LEAGUE_MEMBER_EX stMemberEx,
    ST_LEAGUE_INFO_UPDATE stInfoUpdate,
    unsigned char byApplyState)
{
    // Broadcast new member joining to all guild members
    // Hidden C++ exception states: #wind=3
    
    for (auto it = m_mapMembers.begin(); it != m_mapMembers.end(); ++it)
    {
        CUser* pMember = it->second;
        if (!pMember)
            continue;
        
        // Skip if sync flag is set
        if (CUser::GetLeagueSyncFlag(pMember))
            continue;
        
        // Skip if same UCID as joining member
        if (stMemberEx.dwUCID == pMember->GetLeagueID())
            continue;
        
        // Update sync count
        CUser::UpdateLeagueSyncCount(pMember, m_nSyncCount);
        
        // Send join notification packet (0x22, 0x17)
        XSendPacket xSendPacket(0x22, 0x17);
        xSendPacket << stMemberEx;
        CGocNetwork::Send(&pMember->XActor, &xSendPacket);
        
        // Send info update packet (0x22, 0x43)
        XSendPacket packet(0x22, 0x43);
        packet << stInfoUpdate;
        CGocNetwork::Send(&pMember->XActor, &packet);
        
        // If apply state is 1, send additional applicant notification
        if (byApplyState == 1)
        {
            XSendPacket v20(0x22, 0x26);
            v20.XParse << stMemberEx.dwUCID;
            CGocNetwork::Send(&pMember->XActor, &v20);
        }
    }
}

// Address: 0x14028BF00
void CLeagueMember::KickoutLeagueMember(
    int dwReqUCID,
    unsigned int dwKickoutUCID,
    ST_LEAGUE_INFO_UPDATE stUpdateInfo)
{
    // Broadcast kick notification to all guild members
    // Hidden C++ exception states: #wind=2
    
    for (auto it = m_mapMembers.begin(); it != m_mapMembers.end(); ++it)
    {
        CUser* pMember = it->second;
        if (!pMember)
            continue;
        
        // Skip requester and kicked member
        if (dwReqUCID == pMember->GetLeagueID() || 
            dwKickoutUCID == pMember->GetLeagueID())
            continue;
        
        // Skip if sync flag set
        if (CUser::GetLeagueSyncFlag(pMember))
            continue;
        
        // Update sync count
        CUser::UpdateLeagueSyncCount(pMember, m_nSyncCount);
        
        // Send kick notification packet (0x22, 0x41)
        XSendPacket xSendPacket(0x22, 0x41);
        xSendPacket.XParse << dwReqUCID;
        xSendPacket.XParse << dwKickoutUCID;
        CGocNetwork::Send(&pMember->XActor, &xSendPacket);
        
        // Send info update packet (0x22, 0x43)
        XSendPacket xSendPacket2(0x22, 0x43);
        xSendPacket2 << stUpdateInfo;
        CGocNetwork::Send(&pMember->XActor, &xSendPacket2);
    }
    
    // Find and remove kicked user
    auto it = m_mapMembers.find(dwKickoutUCID);
    if (it != m_mapMembers.end())
    {
        CUser* pKickoutUser = it->second;
        if (pKickoutUser)
        {
            // Clear guild info from kicked user
            CUser::ClearLeagueInfo(pKickoutUser);
        }
        m_mapMembers.erase(it);
    }
}

// Address: 0x14028C6D0
void CLeagueMember::Levelup(
    int nLeagueID,
    unsigned char byLevel,
    char bySkillPoint,
    PS_AUTO_SKILL psAutoSkillInfo)
{
    // Broadcast guild level up to all members
    // Hidden C++ exception states: #wind=1
    
    for (auto it = m_mapMembers.begin(); it != m_mapMembers.end(); ++it)
    {
        CUser* pMember = it->second;
        if (!pMember || CUser::GetLeagueSyncFlag(pMember))
            continue;
        
        // Update member's guild level
        CUser::SetLeagueLevel(pMember, byLevel);
        
        // Update guild skills
        for (int i = 0; i < 8; ++i)
        {
            if (psAutoSkillInfo.bySkillInfo[i])
            {
                CUser::SetLeagueSkill(pMember, i, psAutoSkillInfo.bySkillInfo[i]);
            }
        }
        
        // Update sync count
        CUser::UpdateLeagueSyncCount(pMember, m_nSyncCount);
        
        // Send level up packet (0x22, 0x52)
        XSendPacket xSendPacket(0x22, 0x52);
        xSendPacket.XParse << nLeagueID;
        xSendPacket.XParse << byLevel;
        xSendPacket.XParse << bySkillPoint;
        xSendPacket << psAutoSkillInfo;
        CGocNetwork::Send(&pMember->XActor, &xSendPacket);
    }
}

// Address: 0x14028C590
void CLeagueMember::Wealth(ST_LEAGUE_INFO_UPDATE stUpdateInfo)
{
    // Broadcast guild wealth update to all members
    for (auto it = m_mapMembers.begin(); it != m_mapMembers.end(); ++it)
    {
        CUser* pMember = it->second;
        if (!pMember || CUser::GetLeagueSyncFlag(pMember))
            continue;
        
        CUser::UpdateLeagueSyncCount(pMember, m_nSyncCount);
        
        // Send wealth update packet (0x22, 0x55)
        XSendPacket xSendPacket(0x22, 0x55);
        xSendPacket << stUpdateInfo;
        CGocNetwork::Send(&pMember->XActor, &xSendPacket);
    }
}

// ============================================================================
// CLeagueProcess Implementation
// ============================================================================

// Address: 0x1404EBB20
CLeagueProcess::CLeagueProcess()
{
    // Constructor - Hidden C++ exception states
    // Initialize TXProcess base
    // Set command ID to 0x22
    // Set name to "CLeagueProcess"
}

// Address: 0x1404EBBE0
CLeagueProcess::~CLeagueProcess()
{
    // Destructor
}

// Address: 0x1404EBC10
bool CLeagueProcess::Parse(XPacket& xPacket)
{
    // Main packet routing for guild operations
    // Routes based on sub-command
    
    unsigned char bySubCmd = XPacket::GetSubCmd(&xPacket);
    
    switch (bySubCmd)
    {
        case 0x02: return ReqLeagueCreate(xPacket);
        case 0x03: return ReqLeagueDelete(xPacket);
        case 0x08: return ReqLeagueSearch(xPacket);
        case 0x10: return ReqLeagueOverlapName(xPacket);
        case 0x11: return ReqLeagueApplicant(xPacket);
        case 0x12: return ReqLeagueInfo(xPacket);
        case 0x13: return ReqLeagueWithDraw(xPacket);
        case 0x14: return ReqLeagueKick(xPacket);
        case 0x15: return ReqLeagueInvite(xPacket);
        case 0x16: return ReqLeagueInviteAccept(xPacket);
        case 0x17: return ReqLeagueInviteReject(xPacket);
        case 0x18: return ReqLeagueBoard(xPacket);
        case 0x19: return ReqLeagueApplicantAccept(xPacket);
        case 0x1A: return ReqLeagueApplicantReject(xPacket);
        case 0x1C: return ReqLeagueList(xPacket);
        case 0x1D: return ReqLeagueAuthChange(xPacket);
        case 0x1E: return ReqLeagueNoticeChange(xPacket);
        case 0x1F: return ReqLeagueNameChange(xPacket);
        case 0x20: return ReqLeagueCardChange(xPacket);
        case 0x21: return ReqLeaguePositionNameChange(xPacket);
        case 0x22: return ReqLeagueMemberPositionChange(xPacket);
        case 0x23: return ReqLeagueOpenOrNot(xPacket);
        case 0x24: return ReqLeagueRecruitNotice(xPacket);
        case 0x26: return ReqLeagueDelegate(xPacket);
        case 0x27: return ReqLeagueSkillLearn(xPacket);
        case 0x29: return ReqLeagueInventoryMove(xPacket);
        case 0x2A: return ReqLeagueInventoryInfo(xPacket);
        default: return true;
    }
}

// Address: 0x1404EBF40
bool CLeagueProcess::ReqLeagueSearch(XPacket& xPacket)
{
    // Handle guild search request
    // Hidden C++ exception states: #wind=2
    
    ST_REQ_LEAGUE_SEARCH stSearch;
    xPacket >> stSearch;
    
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;
    
    // TODO: Implement search logic using thread manager
    // Decompile shows lambda functions and job queue usage
    
    return true;
}

// Address: 0x1404EC720
bool CLeagueProcess::ReqLeagueCreate(XPacket& xPacket)
{
    // Handle guild creation request
    // Hidden C++ exception states: #wind=2
    
    ST_REQ_LEAGUE_CREATE stCreate;
    xPacket >> stCreate;
    
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // Increment job count
    XClient::IncrementJobCount(pUser);
    
    // TODO: Queue creation job to logic thread manager
    // Decompile shows lambda usage and DoJob calls
    
    return true;
}

// Address: 0x1404ED240
bool CLeagueProcess::ReqLeagueDelete(XPacket& xPacket)
{
    // Handle guild deletion request
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // TODO: Implement deletion logic
    return true;
}

// Address: 0x1404ED680
bool CLeagueProcess::ReqLeagueApplicant(XPacket& xPacket)
{
    // Handle guild application request
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // TODO: Implement application logic
    return true;
}

// Address: 0x1404EDC80
bool CLeagueProcess::ReqLeagueInfo(XPacket& xPacket)
{
    // Handle guild info request
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;
    
    // TODO: Implement info request logic
    return true;
}

// Address: 0x1404EDF90
bool CLeagueProcess::ReqLeagueWithDraw(XPacket& xPacket)
{
    // Handle guild withdrawal request
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // TODO: Implement withdrawal logic
    return true;
}

// Address: 0x1404EE340
bool CLeagueProcess::ReqLeagueKick(XPacket& xPacket)
{
    // Handle guild kick member request
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // TODO: Implement kick logic
    return true;
}

// Address: 0x1404EE790
bool CLeagueProcess::ReqLeagueInvite(XPacket& xPacket)
{
    // Handle guild invite request
    // Hidden C++ exception states: #wind=2
    
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // TODO: Implement invite logic
    return true;
}

// Address: 0x1404EECF0
bool CLeagueProcess::ReqLeagueInviteAccept(XPacket& xPacket)
{
    // Handle guild invite acceptance
    // Hidden C++ exception states: #wind=2
    
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // TODO: Implement invite accept logic
    return true;
}

// Address: 0x1404EF150
bool CLeagueProcess::ReqLeagueInviteReject(XPacket& xPacket)
{
    // Handle guild invite rejection
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // TODO: Implement invite reject logic
    return true;
}

// Address: 0x1404EF5B0
bool CLeagueProcess::ReqLeagueBoard(XPacket& xPacket)
{
    // Handle guild board request
    // Hidden C++ exception states: #wind=2
    
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // TODO: Implement board logic
    return true;
}

// Address: 0x1404EFB10
bool CLeagueProcess::ReqLeagueApplicantAccept(XPacket& xPacket)
{
    // Handle guild applicant acceptance
    // Hidden C++ exception states: #wind=2
    
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // TODO: Implement applicant accept logic
    return true;
}

// Address: 0x1404EFFD0
bool CLeagueProcess::ReqLeagueApplicantReject(XPacket& xPacket)
{
    // Handle guild applicant rejection
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // TODO: Implement applicant reject logic
    return true;
}

// Address: 0x1404F0380
bool CLeagueProcess::ResLeagueOverlapName(XPacket& xPacket)
{
    // Handle guild name overlap check response
    // Hidden C++ exception states: #wind=2
    
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;
    
    // TODO: Implement name check response logic
    return true;
}

// Address: 0x1404F0780
bool CLeagueProcess::ReqLeagueList(XPacket& xPacket)
{
    // Handle guild list request
    // Hidden C++ exception states: #wind=2
    
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // TODO: Implement list logic
    return true;
}

// Address: 0x1404F0B10
bool CLeagueProcess::ReqLeagueAuthChange(XPacket& xPacket)
{
    // Handle guild authority change request
    // Hidden C++ exception states: #wind=2
    
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // TODO: Implement auth change logic
    return true;
}

// Address: 0x1404F0F90
bool CLeagueProcess::ReqLeagueNoticeChange(XPacket& xPacket)
{
    // Handle guild notice change request
    // Hidden C++ exception states: #wind=2
    
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // TODO: Implement notice change logic
    return true;
}

// Address: 0x1404F1470
int CLeagueProcess::CheckNameChangeItem(
    PS_ITEM_SLOT_INFO psSlotInfo,
    PS_RES_STORAGE_INFO& psResInfo)
{
    // Check if item can be used for name change
    // Returns result code
    
    // TODO: Implement item check logic
    // Decompile shows complex item validation
    return 0;
}

// Address: 0x1404F1910
void CLeagueProcess::SendLeagueNameChangeMsg(
    int nResult,
    PS_REQ_LEAGUE_NAME_CHANGE psNameChange)
{
    // Send guild name change message to community server
    
    // TODO: Implement message sending logic
}

// Address: 0x1404F1AA0
bool CLeagueProcess::ReqLeagueNameChange(XPacket& xPacket)
{
    // Handle guild name change request
    // Hidden C++ exception states: #wind=2
    
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // TODO: Implement name change logic
    return true;
}

// Address: 0x1404F25E0
bool CLeagueProcess::ReqLeagueCardChange(XPacket& xPacket)
{
    // Handle guild card change request
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // TODO: Implement card change logic
    return true;
}

// Address: 0x1404F30C0
bool CLeagueProcess::ReqLeaguePositionNameChange(XPacket& xPacket)
{
    // Handle guild position name change request
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // TODO: Implement position name change logic
    return true;
}

// Address: 0x1404F36A0
bool CLeagueProcess::ReqLeagueMemberPositionChange(XPacket& xPacket)
{
    // Handle guild member position change request
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // TODO: Implement member position change logic
    return true;
}

// Address: 0x1404F3AC0
bool CLeagueProcess::ReqLeagueOpenOrNot(XPacket& xPacket)
{
    // Handle guild open/close toggle request
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // TODO: Implement open/close logic
    return true;
}

// Address: 0x1404F3E40
bool CLeagueProcess::ReqLeagueRecruitNotice(XPacket& xPacket)
{
    // Handle guild recruit notice request
    // Hidden C++ exception states: #wind=2
    
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // TODO: Implement recruit notice logic
    return true;
}

// Address: 0x1404F4380
bool CLeagueProcess::CheckNpc(unsigned int dwActorID, int nNpcID, int nDistance)
{
    // Check if NPC is within distance
    // TODO: Implement NPC distance check
    return true;
}

// Address: 0x1404F4570
bool CLeagueProcess::ReqLeagueDelegate(XPacket& xPacket)
{
    // Handle guild leader delegation request
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // TODO: Implement delegation logic
    return true;
}

// Address: 0x1404F4A40
bool CLeagueProcess::ReqLeagueSkillLearn(XPacket& xPacket)
{
    // Handle guild skill learn request
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // TODO: Implement skill learn logic
    return true;
}

// Address: 0x1404F5250
bool CLeagueProcess::ReqLeagueInventoryMove(XPacket& xPacket)
{
    // Handle guild inventory move request
    // Hidden C++ exception states: #wind=2
    
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // TODO: Implement inventory move logic
    return true;
}

// Address: 0x1404F62C0
bool CLeagueProcess::ReqLeagueInventoryInfo(XPacket& xPacket)
{
    // Handle guild inventory info request
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;
    
    // TODO: Implement inventory info logic
    return true;
}

// Address: 0x1404F6C40
int CLeagueProcess::CheckLeagueInventoryIn(
    PS_REQ_ITEM_MOVE_LEAGUE_INVEN psReq,
    STItem& stItem,
    PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME& psMove)
{
    // Check if item can be moved into guild inventory
    // TODO: Implement inventory in check
    return 0;
}

// Address: 0x1404F72A0
bool CLeagueProcess::CheckLeagueInventoryOut(PS_REQ_ITEM_MOVE_LEAGUE_INVEN psReq)
{
    // Check if item can be moved out of guild inventory
    // TODO: Implement inventory out check
    return true;
}

// Address: 0x1404F75E0
bool CLeagueProcess::CheckLeagueInventoryInMove(PS_REQ_ITEM_MOVE_LEAGUE_INVEN psReq)
{
    // Check if item can be moved within guild inventory
    // TODO: Implement inventory in move check
    return true;
}

// Address: 0x1404F0330
bool CLeagueProcess::DBLeagueParse(XPacket& xPacket)
{
    // Parse database response for guild operations
    // TODO: Implement DB response parsing
    return true;
}

// ============================================================================
// CGocRecode Extension for Guild Wealth
// ============================================================================

// Note: SendLeagueWealth is part of CGocRecode class
// Address: 0x1401588F0
// This function sends guild wealth contribution to community server
// See implementation in GocRecode.cpp

namespace CGocRecodeExtension
{
    void SendLeagueWealth(CMover* pMover, short shExp, int nGold)
    {
        // Hidden C++ exception states: #wind=1
        
        // Cast to CUser
        CUser* pUser = dynamic_cast<CUser*>(pMover);
        if (!pUser)
            return;
        
        // Check if user is in a guild
        int nLeagueID = CUser::GetLeagueID(pUser);
        if (nLeagueID <= 0)
            return;
        
        // Calculate guild contribution (1% of gold)
        int nGuildGold = nGold >= 0 ? (int)((double)nGold * 0.01) : 0;
        
        // Prepare guild wealth packet
        PS_LEAGUE_WEALTH_FOR_SERVER psLeagueWealthInfo;
        psLeagueWealthInfo.dwUCID = pUser->GetActorID().dwActorID;
        psLeagueWealthInfo.nLeagueID = nLeagueID;
        psLeagueWealthInfo.shExp = shExp;
        psLeagueWealthInfo.nGold = nGuildGold;
        
        // Send to community server (0xF6, 0x55)
        XSendPacket xSendPacket(0xF6, 0x55);
        xSendPacket << psLeagueWealthInfo;
        
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (pServer)
        {
            pServer->m_communitySocket.SendCheck(&xSendPacket);
        }
    }
}
