#include "GocHelper.h"
#include "User.h"
#include "Monster.h"
#include "BattleZone.h"
#include "XGameServer.h"
#include "XSendPacket.h"
#include "XPacket.h"
#include "ResourceMgr.h"
#include <algorithm>

// Static member
int CGocHelper::GetFamilyID()
{
    // IDA 0x140039020 - Static function returns family ID constant
    return 0;
}

// Constructor
CGocHelper::CGocHelper()
    : GOComponent()
    , m_byAutoSummonFlag(1)
    , m_dwLastSummonTime(0)
    , m_byMaxSummonCount(3)
    , m_byCurrentSummonCount(0)
    , m_byMySupportType(0)
    , m_fMySupportValue(0.0f)
    , m_dwMySupportTime(0)
{
    // IDA 0x140091E90 - Initialize member maps
    memset(&m_stMySupport, 0, sizeof(m_stMySupport));
}

// Destructor
CGocHelper::~CGocHelper()
{
    // IDA 0x140091F80 - Clear and destroy
    Clear();
}

// Initialize helper component
void CGocHelper::Init()
{
    // IDA 0x140091FE0 - Reset to initial state
    Clear();
}

// Clear all helper data
void CGocHelper::Clear()
{
    // IDA 0x140092000 - Clear all containers and reset state
    m_tLastSummon = 0;
    m_mapSummonedHelper.clear();
    m_mapHelperList.clear();
    m_byMySupportResister = 0;
    m_byMySupportReward = 0;
    m_byAutoSummonFlag = 1;
    memset(&m_stMySupport, 0, sizeof(m_stMySupport));
    m_byCurrentSummonCount = 0;
}

// Reset helper state (keep list but clear summoned)
void CGocHelper::Reset()
{
    // IDA 0x140092080 - Reset only summoned helpers
    m_tLastSummon = 0;
    m_mapSummonedHelper.clear();
    m_byCurrentSummonCount = 0;
}

// Set helper list from DB
bool CGocHelper::SetHelperList(PS_HELPER_LIST_RES& stRes)
{
    // IDA 0x1400920C0 - Load helper list from DB response
    SetAutoSummonFlag(stRes.byAutoSummon);
    
    for (size_t i = 0; i < stRes.vecHelper.size(); ++i)
    {
        ST_HELPER_INFO stAdd = stRes.vecHelper[i];
        AddMyHelper(stAdd, false);
    }
    
    SendHelperList();
    return true;
}

// Get helper list (all or summoned only)
bool CGocHelper::GetHelperList(std::vector<ST_HELPER_INFO>& vecList, bool bSummonedOnly)
{
    // IDA 0x1400922B0 - Retrieve helper list
    vecList.clear();
    
    for (auto& pair : m_mapHelperList)
    {
        if (!bSummonedOnly || pair.second.bySummonState == HELPER_STATE_SUMMONED)
        {
            vecList.push_back(pair.second);
        }
    }
    
    return true;
}

// Send helper list to client
void CGocHelper::SendHelperList()
{
    // IDA 0x140092560 - Send PS_HELPER_LIST_RES packet to client
    CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    if (!pUser)
        return;
    
    XSendPacket xSend;
    xSend << (unsigned short)0x7B20;  // SC_HELPER_LIST
    
    PS_HELPER_LIST_RES psHelper;
    psHelper.dwUCID = pUser->GetUCID();
    psHelper.byAutoSummon = IsAutoSummon() ? 1 : 0;
    GetHelperList(psHelper.vecHelper, false);
    
    xSend << psHelper;
    pUser->SendPacket(&xSend);
}

// Get helper info by ID
bool CGocHelper::GetHelperInfo(unsigned long dwHelperID, ST_HELPER_INFO& stInfo)
{
    // IDA 0x140092700 - Find helper in map
    auto it = m_mapHelperList.find(dwHelperID);
    if (it == m_mapHelperList.end())
        return false;
    
    stInfo = it->second;
    return true;
}

// Check if helper exists
bool CGocHelper::FindHelper(unsigned long dwHelperID)
{
    // IDA 0x1400928A0 - Check if helper ID exists
    return m_mapHelperList.find(dwHelperID) != m_mapHelperList.end();
}

// Add new helper
bool CGocHelper::AddMyHelper(ST_HELPER_INFO& stInfo, bool bSend)
{
    // IDA 0x140092900 - Add helper to list
    XGameServer* pServer = XGameServer::Instance();
    if (!pServer)
        return false;
    
    // Validate helper exists in TB_HELPER table
    TB_HELPER* pHelper = pServer->GetResourceMgr().GetTB_HELPER(stInfo.dwHelperID);
    if (!pHelper)
        return false;
    
    // Check if already exists
    if (FindHelper(stInfo.dwHelperID))
        return false;
    
    // Add to map
    m_mapHelperList[stInfo.dwHelperID] = stInfo;
    
    // Send to DB if requested
    if (bSend)
    {
        SendDBAddHelper(stInfo.dwHelperID);
    }
    
    return true;
}

// Process helper summon request
void CGocHelper::HelperProcess(PS_HELPER_SUMMON_REQ& stReq)
{
    // IDA 0x140093080 - Handle summon/release request
    CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    if (!pUser || pUser->IsDie())
        return;
    
    if (stReq.bySummon)
    {
        // Summon request
        if (!CheckHelperSummonDelay())
        {
            pUser->SendErrorMessage(0x27, 2, 0xE295);  // Summon delay error
            return;
        }
        
        if (!CheckSummonHelper(stReq.dwHelperID))
        {
            pUser->SendErrorMessage(0x27, 2, 0xE296);  // Cannot summon error
            return;
        }
        
        HelperSummon(stReq.dwHelperID);
    }
    else
    {
        // Release request
        HelperRelease(stReq.dwHelperID);
    }
}

// Summon helper
bool CGocHelper::HelperSummon(unsigned long dwHelperID)
{
    // IDA 0x140093410 - Summon helper monster
    CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    if (!pUser)
        return false;
    
    XGameServer* pServer = XGameServer::Instance();
    if (!pServer)
        return false;
    
    // Get helper info
    ST_HELPER_INFO stInfo;
    if (!GetHelperInfo(dwHelperID, stInfo))
        return false;
    
    // Get helper table data
    TB_HELPER* pHelper = pServer->GetResourceMgr().GetTB_HELPER(dwHelperID);
    if (!pHelper)
        return false;
    
    // Calculate spawn position near owner
    hkvVec3 vDestPos = pUser->GetPosition();
    float fAngle = pUser->GetOrientationYaw() + 3.141592f;  // Behind player
    float fDist = 2.0f;
    vDestPos.x += cosf(fAngle) * fDist;
    vDestPos.z += sinf(fAngle) * fDist;
    
    // Create helper monster
    CBattleZone* pZone = pUser->GetBattleZone();
    if (!pZone)
        return false;
    
    CMonster* pMonster = pZone->SpawnMonster(pHelper->Monster_ID, vDestPos, 0.0f);
    if (!pMonster)
        return false;
    
    // Set monster properties
    pMonster->SetOwnerID(pUser->GetActorID());
    pMonster->SetSummonType(1);  // Helper type
    pMonster->SetLevel(stInfo.nLevel);
    
    // Apply helper stats
    for (int i = 0; i < 32; ++i)
    {
        pMonster->SetStat(i, stInfo.fAbility[i]);
    }
    
    // Store summoned helper
    m_mapSummonedHelper[dwHelperID] = pMonster->GetActorID();
    m_byCurrentSummonCount++;
    SetHelperSummonState(dwHelperID, true);
    SetHelperSummonTime();
    
    // Send response to client
    PS_HELPER_SUMMON_RES psRes;
    psRes.dwHelperID = dwHelperID;
    psRes.byResult = 1;
    psRes.bySummonState = HELPER_STATE_SUMMONED;
    
    XSendPacket xSend;
    xSend << (unsigned short)0x7B22;  // SC_HELPER_SUMMON
    xSend << psRes;
    pUser->SendPacket(&xSend);
    
    // Log
    ST_LOG_GAME stLog;
    stLog.dwActorID = pUser->GetActorID();
    stLog.dwHelperID = dwHelperID;
    stLog.byAction = 1;  // Summon
    pServer->WriteLog(&stLog, LOG_HELPER);
    
    return true;
}

// Release helper
bool CGocHelper::HelperRelease(unsigned long dwHelperID)
{
    // IDA 0x140094300 - Release summoned helper
    CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    if (!pUser)
        return false;
    
    // Get summoned helper
    auto it = m_mapSummonedHelper.find(dwHelperID);
    if (it == m_mapSummonedHelper.end())
        return false;
    
    unsigned long dwActorID = it->second;
    m_mapSummonedHelper.erase(it);
    
    // Remove monster from zone
    CBattleZone* pZone = pUser->GetBattleZone();
    if (pZone)
    {
        CMonster* pMonster = dynamic_cast<CMonster*>(pZone->FindActor(dwActorID));
        if (pMonster)
        {
            pZone->DeleteMonster(pMonster);
        }
    }
    
    SetHelperSummonState(dwHelperID, false);
    m_byCurrentSummonCount--;
    
    // Send response to client
    PS_HELPER_SUMMON_RES psRes;
    psRes.dwHelperID = dwHelperID;
    psRes.byResult = 1;
    psRes.bySummonState = HELPER_STATE_NONE;
    
    XSendPacket xSend;
    xSend << (unsigned short)0x7B22;  // SC_HELPER_SUMMON
    xSend << psRes;
    pUser->SendPacket(&xSend);
    
    return true;
}

// Warp helper to owner position
bool CGocHelper::HelperWarp(unsigned long dwHelperID)
{
    // IDA 0x140094130 - Teleport helper to owner
    CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    if (!pUser)
        return false;
    
    CMonster* pMonster = GetSummonedHelper(dwHelperID);
    if (!pMonster)
        return false;
    
    // Set position to owner
    hkvVec3 vPos = pUser->GetPosition();
    pMonster->SetPositionXVec3(vPos);
    
    return true;
}

// Set helper summon state
void CGocHelper::SetHelperSummonState(unsigned long dwHelperID, bool bSummoned)
{
    // IDA 0x140092C20 - Update helper summon state in list
    auto it = m_mapHelperList.find(dwHelperID);
    if (it != m_mapHelperList.end())
    {
        it->second.bySummonState = bSummoned ? HELPER_STATE_SUMMONED : HELPER_STATE_NONE;
        it->second.dwSummonTime = bSummoned ? time(nullptr) : 0;
    }
}

// Check if can summon (delay check)
bool CGocHelper::CheckHelperSummonDelay()
{
    // IDA 0x140092CA0 - Check summon cooldown
    time_t tNow = time(nullptr);
    time_t tDiff = tNow - m_tLastSummon;
    
    return tDiff >= 2;  // 2 second cooldown
}

// Check if can summon specific helper
bool CGocHelper::CheckSummonHelper(unsigned long dwHelperID)
{
    // IDA 0x140092D10 - Validate helper can be summoned
    if (!FindHelper(dwHelperID))
        return false;
    
    if (m_byCurrentSummonCount >= m_byMaxSummonCount)
        return false;
    
    // Check if already summoned
    if (m_mapSummonedHelper.find(dwHelperID) != m_mapSummonedHelper.end())
        return false;
    
    return true;
}

// Check if can release helper
bool CGocHelper::CheckReleaseHelper(unsigned long dwHelperID)
{
    // IDA 0x140092DC0 - Validate helper can be released
    return m_mapSummonedHelper.find(dwHelperID) != m_mapSummonedHelper.end();
}

// Check summoned helper count
void CGocHelper::CheckSummonHelperCount()
{
    // IDA 0x140092E70 - Verify summoned count matches actual
    int nCount = 0;
    for (auto& pair : m_mapHelperList)
    {
        if (pair.second.bySummonState == HELPER_STATE_SUMMONED)
            nCount++;
    }
    
    m_byCurrentSummonCount = nCount;
}

// Summon all auto-summon helpers
void CGocHelper::CheckAllHelperSummon()
{
    // IDA 0x1400948A0 - Auto-summon helpers on login
    if (!IsAutoSummon())
        return;
    
    for (auto& pair : m_mapHelperList)
    {
        if (pair.second.byAutoSummon && 
            pair.second.bySummonState == HELPER_STATE_NONE &&
            m_byCurrentSummonCount < m_byMaxSummonCount)
        {
            HelperSummon(pair.first);
        }
    }
}

// Release all helpers
void CGocHelper::AllHelperRelease()
{
    // IDA 0x140094AF0 - Release all summoned helpers
    std::vector<unsigned long> vecIDs;
    for (auto& pair : m_mapSummonedHelper)
    {
        vecIDs.push_back(pair.first);
    }
    
    for (auto dwID : vecIDs)
    {
        HelperRelease(dwID);
    }
}

// Warp all helpers
void CGocHelper::AllHelperWarp()
{
    // IDA 0x140094C10 - Teleport all helpers to owner
    for (auto& pair : m_mapSummonedHelper)
    {
        HelperWarp(pair.first);
    }
}

// Clear helpers from other players
void CGocHelper::OtherHelperClear()
{
    // IDA 0x140094D30 - Clear other player's helpers (for party sync)
    AllHelperRelease();
}

// Get summoned helper instance
CMonster* CGocHelper::GetSummonedHelper(unsigned long dwHelperID)
{
    // IDA 0x140092AD0 - Get monster instance by helper ID
    auto it = m_mapSummonedHelper.find(dwHelperID);
    if (it == m_mapSummonedHelper.end())
        return nullptr;
    
    CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    if (!pUser)
        return nullptr;
    
    CBattleZone* pZone = pUser->GetBattleZone();
    if (!pZone)
        return nullptr;
    
    return dynamic_cast<CMonster*>(pZone->FindActor(it->second));
}

// Get all summoned helpers
void CGocHelper::GetSummonedHelperList(std::map<unsigned long, unsigned long>& mapList)
{
    // IDA 0x140092B40 - Get map of all summoned helpers
    mapList = m_mapSummonedHelper;
}

// Sync summoned helper info to client
void CGocHelper::SyncSummonedInfo()
{
    // IDA 0x140096060 - Send current summoned state to client
    CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    if (!pUser)
        return;
    
    for (auto& pair : m_mapSummonedHelper)
    {
        CMonster* pMonster = GetSummonedHelper(pair.first);
        if (pMonster)
        {
            XSendPacket xSend;
            xSend << (unsigned short)0x7B23;  // SC_HELPER_SYNC
            xSend << pair.first;  // Helper ID
            xSend << pMonster->GetActorID();
            xSend << pMonster->GetPosition();
            pUser->SendPacket(&xSend);
        }
    }
}

// Calculate all helper stats
void CGocHelper::CalcHelperStatsALL(ST_HELPER_INFO& stInfo, int nType)
{
    // IDA 0x140095280 - Calculate total stats
    CalcOriginStats(stInfo);
    CalcEquipItemStats(stInfo);
    CalcFriendItemStats(stInfo);
}

// Calculate origin stats from table
void CGocHelper::CalcOriginStats(ST_HELPER_INFO& stInfo)
{
    // IDA 0x140095360 - Get base stats from TB_HELPER
    XGameServer* pServer = XGameServer::Instance();
    if (!pServer)
        return;
    
    TB_HELPER* pHelper = pServer->GetResourceMgr().GetTB_HELPER(stInfo.dwHelperID);
    if (!pHelper)
        return;
    
    // Copy base stats
    for (int i = 0; i < 32; ++i)
    {
        stInfo.fAbility[i] = pHelper->fBaseStats[i];
    }
}

// Calculate equipped item stats
void CGocHelper::CalcEquipItemStats(ST_HELPER_INFO& stInfo)
{
    // IDA 0x140095500 - Add stats from equipped items
    // TODO: Implement item stat calculation
}

// Calculate friend support item stats
void CGocHelper::CalcFriendItemStats(ST_HELPER_INFO& stInfo)
{
    // IDA 0x1400958B0 - Add stats from friend support items
    // TODO: Implement friend item stat calculation
}

// Unequip item stats
void CGocHelper::UnEquipHelperItemStats(unsigned long dwHelperID, std::vector<unsigned long>& vecItems)
{
    // IDA 0x140095C80 - Remove item stats from helper
    ST_HELPER_INFO stInfo;
    if (!GetHelperInfo(dwHelperID, stInfo))
        return;
    
    // Recalculate stats
    CalcHelperStatsALL(stInfo, 0);
    
    // Update in list
    m_mapHelperList[dwHelperID] = stInfo;
}

// Unequip friend item stats
void CGocHelper::UnEquipHelperFriendItemStats(unsigned long dwHelperID)
{
    // IDA 0x140095E80 - Remove friend item stats
    ST_HELPER_INFO stInfo;
    if (!GetHelperInfo(dwHelperID, stInfo))
        return;
    
    CalcFriendItemStats(stInfo);
    m_mapHelperList[dwHelperID] = stInfo;
}

// Get support type rate
float CGocHelper::GetSupportTypeRate()
{
    // IDA 0x140096500 - Get support bonus rate
    XGameServer* pServer = XGameServer::Instance();
    if (!pServer)
        return 0.0f;
    
    return pServer->GetResourceMgr().GetHelperSupportRate();
}

// Get support type value
float CGocHelper::GetSupportTypeValue()
{
    // IDA 0x140096540 - Calculate support value
    return m_fMySupportValue * GetSupportTypeRate();
}

// Set support info
void CGocHelper::SetMySupportInfo(PS_HELPER_SUPPORT_INFO_RES& stRes)
{
    // IDA 0x140096660 - Update support info
    m_byMySupportType = stRes.bySupportType;
    m_fMySupportValue = stRes.fSupportValue;
    m_dwMySupportTime = stRes.dwSupportTime;
    m_stMySupport.bySupportType = stRes.bySupportType;
    m_stMySupport.fSupportValue = stRes.fSupportValue;
    m_stMySupport.dwSupportTime = stRes.dwSupportTime;
}

// Release support
void CGocHelper::HelperSupportRelease()
{
    // IDA 0x1400966D0 - Clear support
    m_byMySupportType = SUPPORT_TYPE_NONE;
    m_fMySupportValue = 0.0f;
    m_dwMySupportTime = 0;
    memset(&m_stMySupport, 0, sizeof(m_stMySupport));
}

// Request support info
void CGocHelper::ReqHelperSupportInfo()
{
    // IDA 0x1400968F0 - Request support info from DB
    CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    if (!pUser)
        return;
    
    XSendPacket xSend;
    xSend << (unsigned short)0x5B30;  // DB_HELPER_SUPPORT_INFO_REQ
    xSend << pUser->GetUCID();
    pUser->SendToDB(&xSend);
}

// Request support register
void CGocHelper::ReqHelperSupportRegister(PS_HELPER_SUPPORT_REGISTER_REQ& stReq)
{
    // IDA 0x140096A10 - Register for support
    CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    if (!pUser)
        return;
    
    XSendPacket xSend;
    xSend << (unsigned short)0x5B31;  // DB_HELPER_SUPPORT_REGISTER_REQ
    xSend << pUser->GetUCID();
    xSend << stReq.bySupportType;
    xSend << stReq.dwItemID;
    pUser->SendToDB(&xSend);
}

// Request support reward
void CGocHelper::ReqHelperSupportReward()
{
    // IDA 0x140096BE0 - Claim support reward
    CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    if (!pUser)
        return;
    
    XSendPacket xSend;
    xSend << (unsigned short)0x5B32;  // DB_HELPER_SUPPORT_REWARD_REQ
    xSend << pUser->GetUCID();
    pUser->SendToDB(&xSend);
}

// Request support list
void CGocHelper::ReqHelperSupportList()
{
    // IDA 0x140096F70 - Get support list
    CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    if (!pUser)
        return;
    
    XSendPacket xSend;
    xSend << (unsigned short)0x5B33;  // DB_HELPER_SUPPORT_LIST_REQ
    xSend << pUser->GetUCID();
    pUser->SendToDB(&xSend);
}

// Request support equip
void CGocHelper::ReqHelperSupportEquip(PS_HELPER_SUPPORT_EQUIP_REQ& stReq)
{
    // IDA 0x140097080 - Equip support item
    CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    if (!pUser)
        return;
    
    XSendPacket xSend;
    xSend << (unsigned short)0x5B34;  // DB_HELPER_SUPPORT_EQUIP_REQ
    xSend << pUser->GetUCID();
    xSend << stReq.dwHelperID;
    xSend << stReq.dwItemID;
    xSend << stReq.bySlotPos;
    pUser->SendToDB(&xSend);
}

// Request helper equip
void CGocHelper::ReqHelperEquip(PS_HELPER_EQUIP_REQ& stReq)
{
    // IDA 0x140097200 - Equip item to helper
    CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    if (!pUser)
        return;
    
    XSendPacket xSend;
    xSend << (unsigned short)0x5B35;  // DB_HELPER_EQUIP_REQ
    xSend << pUser->GetUCID();
    xSend << stReq.dwHelperID;
    xSend << stReq.dwItemID;
    xSend << stReq.bySlotPos;
    xSend << stReq.byEquip;
    pUser->SendToDB(&xSend);
}

// Request change order
void CGocHelper::ReqHelperChangeOrder(PS_HELPER_CHANGE_ORDER& stReq)
{
    // IDA 0x140097EB0 - Change helper display order
    CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    if (!pUser)
        return;
    
    // Update order locally
    auto it = m_mapHelperList.find(stReq.dwHelperID);
    if (it != m_mapHelperList.end())
    {
        it->second.byOrder = stReq.byNewOrder;
    }
    
    // Send to DB
    XSendPacket xSend;
    xSend << (unsigned short)0x5B36;  // DB_HELPER_CHANGE_ORDER
    xSend << pUser->GetUCID();
    xSend << stReq.dwHelperID;
    xSend << stReq.byNewOrder;
    pUser->SendToDB(&xSend);
}

// Request change auto summon
void CGocHelper::ReqHelperChangeAutoSummon(PS_HELPER_CHANGE_AUTO_SUMMON& stReq)
{
    // IDA 0x1400980C0 - Toggle auto-summon flag
    CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    if (!pUser)
        return;
    
    // Update locally
    auto it = m_mapHelperList.find(stReq.dwHelperID);
    if (it != m_mapHelperList.end())
    {
        it->second.byAutoSummon = stReq.byAutoSummon;
    }
    
    // Send to DB
    XSendPacket xSend;
    xSend << (unsigned short)0x5B37;  // DB_HELPER_CHANGE_AUTO_SUMMON
    xSend << pUser->GetUCID();
    xSend << stReq.dwHelperID;
    xSend << stReq.byAutoSummon;
    pUser->SendToDB(&xSend);
}

// Response handlers (DB callbacks)
void CGocHelper::ResHelperSupportInfo(PS_HELPER_SUPPORT_INFO_RES& stRes)
{
    // IDA 0x140098280 - Handle support info response
    SetMySupportInfo(stRes);
}

void CGocHelper::ResHelperSupportRegister(PS_SERVER_HELPER_SUPPORT_REGISTER& stRes)
{
    // IDA 0x140098370 - Handle register response
    CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    if (!pUser)
        return;
    
    XSendPacket xSend;
    xSend << (unsigned short)0x7B30;  // SC_HELPER_SUPPORT_REGISTER
    xSend << stRes.byResult;
    xSend << stRes.dwSupportID;
    pUser->SendPacket(&xSend);
}

void CGocHelper::ResHelperSupportReward(PS_SERVER_HELPER_SUPPORT_REWARD& stRes)
{
    // IDA 0x140098600 - Handle reward response
    CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    if (!pUser)
        return;
    
    XSendPacket xSend;
    xSend << (unsigned short)0x7B31;  // SC_HELPER_SUPPORT_REWARD
    xSend << stRes.byResult;
    xSend << stRes.dwRewardID;
    pUser->SendPacket(&xSend);
}

void CGocHelper::ResHelperSupportList(PS_HELPER_SUPPORT_LIST_RES& stRes)
{
    // IDA 0x140098850 - Handle support list response
    CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    if (!pUser)
        return;
    
    XSendPacket xSend;
    xSend << (unsigned short)0x7B32;  // SC_HELPER_SUPPORT_LIST
    xSend << stRes.byCount;
    pUser->SendPacket(&xSend);
}

void CGocHelper::ResHelperSupportEquip(PS_HELPER_SUPPORT_EQUIP_RES& stRes)
{
    // IDA 0x140098920 - Handle support equip response
    CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    if (!pUser)
        return;
    
    XSendPacket xSend;
    xSend << (unsigned short)0x7B33;  // SC_HELPER_SUPPORT_EQUIP
    xSend << stRes.byResult;
    xSend << stRes.dwHelperID;
    pUser->SendPacket(&xSend);
}

void CGocHelper::ResHelperSupportEquipReward(PS_HELPER_SUPPORT_EQUIP_RES& stRes)
{
    // IDA 0x140098CA0 - Handle support equip reward
    CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    if (!pUser)
        return;
    
    XSendPacket xSend;
    xSend << (unsigned short)0x7B34;  // SC_HELPER_SUPPORT_EQUIP_REWARD
    xSend << stRes.byResult;
    xSend << stRes.dwHelperID;
    pUser->SendPacket(&xSend);
}

void CGocHelper::ResHelperEquip(PS_DB_HELPER_EQUIP_RES& stRes)
{
    // IDA 0x140098D20 - Handle helper equip response
    CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    if (!pUser)
        return;
    
    // Update helper stats
    ST_HELPER_INFO stInfo;
    if (GetHelperInfo(stRes.dwHelperID, stInfo))
    {
        CalcHelperStatsALL(stInfo, 0);
        m_mapHelperList[stRes.dwHelperID] = stInfo;
    }
    
    XSendPacket xSend;
    xSend << (unsigned short)0x7B35;  // SC_HELPER_EQUIP
    xSend << stRes.byResult;
    xSend << stRes.dwHelperID;
    pUser->SendPacket(&xSend);
}

void CGocHelper::ResHelperSupportRelease(PS_DB_HELPER_SUPPORT_RELEASE& stRes)
{
    // IDA 0x140099D60 - Handle support release response
    HelperSupportRelease();
}

void CGocHelper::ResHelperChangeOrder(PS_HELPER_CHANGE_ORDER& stReq)
{
    // IDA 0x140099F60 - Handle order change confirmation
    SendHelperList();  // Refresh list
}

void CGocHelper::ResHelperChangeAutoSummon(PS_HELPER_CHANGE_AUTO_SUMMON& stReq)
{
    // IDA 0x14009A140 - Handle auto-summon change confirmation
    SendHelperList();  // Refresh list
}

// Database operations
bool CGocHelper::SendDBAddHelper(unsigned long dwHelperID)
{
    // IDA 0x14009A290 - Send add helper to DB
    CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    if (!pUser)
        return false;
    
    ST_HELPER_INFO stInfo;
    if (!GetHelperInfo(dwHelperID, stInfo))
        return false;
    
    XSendPacket xSend;
    xSend << (unsigned short)0x5B20;  // DB_HELPER_ADD
    xSend << pUser->GetUCID();
    xSend << stInfo;
    pUser->SendToDB(&xSend);
    
    return true;
}

void CGocHelper::SendDBHelperList()
{
    // IDA 0x14009A670 - Request helper list from DB
    CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    if (!pUser)
        return;
    
    XSendPacket xSend;
    xSend << (unsigned short)0x5B21;  // DB_HELPER_LIST_REQ
    xSend << pUser->GetUCID();
    pUser->SendToDB(&xSend);
}

// Auto summon management
void CGocHelper::SetAutoSummonFlag(unsigned char byFlag)
{
    // IDA 0x140091DA0 - Set auto-summon flag
    m_byAutoSummonFlag = byFlag;
}

bool CGocHelper::IsAutoSummon()
{
    // IDA 0x140091DC0 - Check auto-summon enabled
    return m_byAutoSummonFlag != 0;
}

void CGocHelper::SetHelperSummonTime()
{
    // IDA 0x140091DF0 - Set last summon time
    m_tLastSummon = time(nullptr);
}

unsigned char CGocHelper::GetLastOrderNumber()
{
    // IDA 0x14009A780 - Get highest order number
    unsigned char byMax = 0;
    for (auto& pair : m_mapHelperList)
    {
        if (pair.second.byOrder > byMax)
            byMax = pair.second.byOrder;
    }
    return byMax;
}

// Get all helper stats
void CGocHelper::GetMyHelperStatsALL(PS_HELPER_STAT_UPDATE& stUpdate)
{
    // IDA 0x140095170 - Calculate total stats from all summoned helpers
    memset(&stUpdate, 0, sizeof(stUpdate));
    
    for (auto& pair : m_mapSummonedHelper)
    {
        ST_HELPER_INFO stInfo;
        if (GetHelperInfo(pair.first, stInfo))
        {
            for (int i = 0; i < 32; ++i)
            {
                stUpdate.fAbility[i] += stInfo.fAbility[i];
            }
        }
    }
}
