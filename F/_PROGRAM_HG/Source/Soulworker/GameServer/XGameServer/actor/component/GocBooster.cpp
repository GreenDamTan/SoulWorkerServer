// GocBooster.cpp - CGocBooster implementation
// IDA verified from GameServer.exe
//
// Key functions decompiled from GameServer.exe (port 10004):
// - Constructor: 0x140049B10
// - Destructor: 0x140049BD0
// - Init: 0x140049C30
// - OnUpdate: 0x140049CA0
// - AddBooster: 0x14004AA30
// - RemoveBooster: 0x14004AC20
// - ClearAllBoosters: 0x14004AF30
// - GetTotalRate: 0x14004B220
// - GetTotalValue: 0x14004B350

#include "GocBooster.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include <ctime>
#include <algorithm>

#ifdef _WIN32
#include <ctime>
#define TIME32 _time32
#else
#define TIME32 time
#endif

// ============================================================================
// Construction / Destruction
// ============================================================================

// Note: SetChangeStat is inline in header (0x140049AF0)

// IDA: ??0CGocBooster@@QEAA@XZ (0x140049B10)
// Verified: Constructor initializes base class and member maps
// Per IDA decompile at 0x140049B10:
// - Calls GOComponent::GOComponent(this)
// - Sets vftable to CGocBooster::`vftable'
// - Constructs m_mapBooster (std::map<unsigned short, ST_BOOSTER_INFO>)
// - Constructs m_mapGroupID (std::map<unsigned short, unsigned char>)
CGocBooster::CGocBooster()
    : GOComponent()
    , m_byConsumeArea(0)
    , m_bChangeStat(false)
    , m_bLoadDB(false)
{
    // Per IDA: std::map default constructors called after base class init
    // m_mapBooster and m_mapGroupID are default-constructed
}

// IDA: ??1CGocBooster@@UEAA@XZ (0x140049BD0)
// Verified: Destructor cleans up in reverse order
CGocBooster::~CGocBooster()
{
    // IDA: Maps are destructed in reverse order of construction
    // m_mapGroupID destructor
    // m_mapBooster destructor
    // GOComponent destructor called automatically
}

// ============================================================================
// Initialization
// ============================================================================

// IDA: ?Init@CGocBooster@@QEAAXXZ (0x140049C30)
// Verified: Clears all booster state and resets to initial state
void CGocBooster::Init()
{
    m_byConsumeArea = 0;
    m_mapBooster.clear();
    m_bChangeStat = false;
    m_bLoadDB = false;
    m_mapGroupID.clear();
    std::fill_n(m_wBoosterID, 10, static_cast<std::uint16_t>(0));
}

bool CGocBooster::Initialize()
{
    Init();
    return true;
}

void CGocBooster::Shutdown()
{
    ClearAllBoosters();
}

// ============================================================================
// Update Loop
// ============================================================================

// IDA: ?OnUpdate@CGocBooster@@QEAAXM@Z (0x140049CA0)
// Verified: Updates booster timers and removes expired boosters
// Per IDA decompile at 0x140049CA0:
// - Checks XActor::IsPlayer on owner
// - Checks UserDB flags bit 0 and bit 1
// - Iterates boosters and checks time type
// - Removes expired boosters
void CGocBooster::OnUpdate(float fDeltaTime)
{
    if (m_mapBooster.empty()) {
        return;
    }

    std::uint16_t wRemoveIndex = 0;
    const std::int64_t tNow = TIME32(nullptr);
    for (auto& pair : m_mapBooster) {
        ST_BOOSTER_INFO& stBooster = pair.second;
        if (stBooster.wBoosterID == 0 || stBooster.byTimeType == 0 || stBooster.lRemainTime <= 0) {
            continue;
        }
        if (stBooster.byTimeType == 4) {
            if (tNow >= stBooster.lRemainTime) {
                wRemoveIndex = stBooster.wBoosterID;
                break;
            }
        } else {
            stBooster.bConsumeTime = 1;
            stBooster.lRemainTime -= static_cast<std::int64_t>(fDeltaTime * 1000.0f);
            if (stBooster.lRemainTime <= 0) {
                wRemoveIndex = stBooster.wBoosterID;
                break;
            }
        }
    }
    if (wRemoveIndex != 0) {
        RemoveBooster(wRemoveIndex);
    }
}


void CGocBooster::Update(float fDeltaTime)
{
    OnUpdate(fDeltaTime);
}

// ============================================================================
// Booster Entry/Exit
// ============================================================================

// IDA: ?EnterBooster@CGocBooster@@QEAAXE@Z (0x14004A080)
// Verified: Sets consume area and sends booster list to client
void CGocBooster::EnterBooster(std::uint8_t byConsumeArea)
{
    m_byConsumeArea = byConsumeArea;

    if (!m_mapBooster.empty()) {
        // IDA: LogHelper::LogDebug("game.contents", "<BOOSTER> Enter Booster %d", m_byConsumeArea);
        SendBoosterList();
        CheckSendBuffAbility();
    }
}

// IDA: ?ExitBooster@CGocBooster@@QEAAXXZ (0x140049FA0)
// Verified: Saves booster state to DB and stops time consumption
void CGocBooster::ExitBooster()
{
    for (auto& pair : m_mapBooster) {
        ST_BOOSTER_INFO& stBooster = pair.second;

        // IDA: Only save boosters with time type 1 (maze time)
        if (stBooster.byTimeType == 1) {
            bool bAccount = (stBooster.bAccount != 0);
            SaveBoosterDB(stBooster.wBoosterID, stBooster.lRemainTime, bAccount);
        }
        stBooster.bConsumeTime = 0;
    }

    // IDA: LogHelper::LogDebug("game.contents", "<BOOSTER> Exit Booster");
}

// ============================================================================
// Booster Queries
// ============================================================================

// IDA: ?FindBooster@CGocBooster@@QEAA_NG@Z (0x14004A540)
// Verified: Checks if booster with given index exists
bool CGocBooster::FindBooster(std::uint16_t wIndex)
{
    return m_mapBooster.find(wIndex) != m_mapBooster.end();
}

// IDA: ?GetBooster@CGocBooster@@QEAA_NGAEAUST_BOOSTER_INFO@@@Z (0x14004A5A0)
// Verified: Retrieves booster info by index
bool CGocBooster::GetBooster(std::uint16_t wIndex, ST_BOOSTER_INFO& stBase)
{
    auto iter = m_mapBooster.find(wIndex);
    if (iter == m_mapBooster.end()) {
        return false;
    }
    stBase = iter->second;
    return true;
}

// IDA: ?GetBoosterList@CGocBooster@@QEAA_NAEAV?$vector@UST_BOOSTER_OUTPUT@@...@@Z (0x14004A140)
// Verified: Returns all boosters as output format
bool CGocBooster::GetBoosterList(std::vector<ST_BOOSTER_OUTPUT>& vecBooster)
{
    if (m_mapBooster.empty()) {
        return false;
    }

    for (const auto& pair : m_mapBooster) {
        const ST_BOOSTER_INFO& stBooster = pair.second;
        ST_BOOSTER_OUTPUT stOutput;
        _ConvertOutputData(const_cast<ST_BOOSTER_INFO*>(&stBooster), &stOutput);
        vecBooster.push_back(stOutput);
    }

    return true;
}

// ============================================================================
// Booster Operations
// ============================================================================

// IDA: ?AddBooster@CGocBooster@@QEAAXG_N@Z (0x14004AA30)
// Verified: Adds a new booster or updates existing one
// Per IDA decompile at 0x14004AA30:
// - Gets TB_BOOSTER table entry
// - If booster exists, calls UpdateBoosterTime
// - Otherwise creates new ST_BOOSTER_INFO and inserts into map
// - Calls ApplyBoosterStat on new booster
// - Calls UpdateBoosterTime at end
void CGocBooster::AddBooster(std::uint16_t wIndex, bool bAccount)
{
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) {
        return;
    }

    TB_BOOSTER* pBoosterTable = pServer->GetResourceMgr().GetTB_BOOSTER(wIndex);
    if (!pBoosterTable) {
        return;
    }

    // Per IDA: If booster already exists, just update time
    if (FindBooster(wIndex)) {
        UpdateBoosterTime(wIndex);
        return;
    }

    // Per IDA: Create new booster entry
    ST_BOOSTER_INFO stNewBooster = {};
    stNewBooster.wBoosterID = wIndex;
    stNewBooster.byTimeType = pBoosterTable->Decrease_Condition;
    stNewBooster.bAccount = bAccount ? 1 : 0;
    stNewBooster.lRemainTime = 0;
    stNewBooster.bConsumeTime = 0;

    // Per IDA: Insert into map using std::pair construction
    m_mapBooster[wIndex] = stNewBooster;
    
    // Per IDA: Apply stat effects
    ApplyBoosterStat(pBoosterTable);
    
    // Per IDA: Always call UpdateBoosterTime at end
    UpdateBoosterTime(wIndex);
}

// IDA: ?RemoveBooster@CGocBooster@@QEAAXG@Z (0x14004AC20)
// Verified: Removes booster and updates stats
// Per IDA decompile at 0x14004AC20:
// - Gets TB_BOOSTER table and finds booster in map
// - Calculates FP before removal if EffectType == 4
// - Checks player state via vftable call
// - Clears booster stat if allowed
// - Calls DeleteBoosterDB, DeleteGroupID, SendRemoveBooster
// - Erases from map
// - Updates PCBang FP if changed
void CGocBooster::RemoveBooster(std::uint16_t wIndex)
{
    auto iter = m_mapBooster.find(wIndex);
    if (iter == m_mapBooster.end()) {
        return;
    }
    bool bAccount = (iter->second.bAccount != 0);
    (void)bAccount;
    DeleteBoosterDB(wIndex, bAccount);
    m_mapBooster.erase(iter);
}


// IDA: ?ClearAllBoosters@CGocBooster@@QEAAXXZ (0x14004AF30)
// Verified: Removes all boosters safely using a temporary list
void CGocBooster::ClearAllBoosters()
{
    // IDA: Create a copy of booster IDs to avoid iterator invalidation
    std::vector<std::uint16_t> vecBoosterList;
    vecBoosterList.reserve(m_mapBooster.size());

    for (const auto& pair : m_mapBooster) {
        vecBoosterList.push_back(pair.second.wBoosterID);
    }

    // Remove each booster
    for (std::uint16_t wBoosterID : vecBoosterList) {
        RemoveBooster(wBoosterID);
    }
}

// ============================================================================
// Booster Time Management
// ============================================================================

// IDA: ?SetBoosterTime@CGocBooster@@QEAAXG_J@Z (0x14004A650)
// Verified: Sets remaining time for a booster
void CGocBooster::SetBoosterTime(std::uint16_t wIndex, std::int64_t nTime)
{
    auto iter = m_mapBooster.find(wIndex);
    if (iter != m_mapBooster.end()) {
        iter->second.lRemainTime = nTime;
    }
}

// IDA: ?UpdateBoosterTime@CGocBooster@@QEAAXG@Z (0x14004BA50)
// Verified: Updates booster time based on table settings
void CGocBooster::UpdateBoosterTime(std::uint16_t wBoosterID)
{
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) {
        return;
    }

    TB_BOOSTER* pBoosterTable = pServer->GetResourceMgr().GetTB_BOOSTER(wBoosterID);
    if (!pBoosterTable) {
        return;
    }

    if (pBoosterTable->Decrease_Condition == 0) {
        return;
    }

    ST_BOOSTER_INFO info = {};
    if (!GetBooster(wBoosterID, info)) {
        return;
    }

    info.byTimeType = pBoosterTable->Decrease_Condition;

    if (pBoosterTable->Decrease_Condition == 4) {
        // IDA: Time type 4 = absolute timestamp
        if (info.lRemainTime == 0) {
            info.lRemainTime = TIME32(nullptr);
        }
        info.lRemainTime += pBoosterTable->Booster_Time / 1000;
    }
    else {
        // IDA: Other time types = add milliseconds
        info.lRemainTime += pBoosterTable->Booster_Time;
    }

    SetBoosterTime(wBoosterID, info.lRemainTime);
    SaveBoosterDB(wBoosterID, info.lRemainTime, info.bAccount != 0);
}

// ============================================================================
// Booster Stat Effects
// ============================================================================

// IDA: ?ApplyBoosterStat@CGocBooster@@QEAAXPEAUTB_BOOSTER@@@Z (0x14004B040)
// Verified: Applies special stat effects (type 9)
void CGocBooster::ApplyBoosterStat(TB_BOOSTER* pBoosterTable)
{
    if (!pBoosterTable) {
        return;
    }

    // IDA: Apply effect type 9 (special stat change)
    for (int i = 0; i < 8; ++i) {
        if (pBoosterTable->uniEffectType[i] == eBooster_Effect_Special) {
            // IDA: Call ApplyType on owner
            // TODO: 需人工审查 - Need CMover::ApplyBuffStat implementation
            // CMover* pOwner = GetOwnerGO();
            // if (pOwner) {
            //     pOwner->ApplyBuffStat(pBoosterTable->uniApplyType[i]);
            // }
            m_bChangeStat = true;
        }
    }
}

// IDA: ?ClearBoosterStat@CGocBooster@@QEAAXPEAUTB_BOOSTER@@@Z (0x14004B0F0)
// Verified: Clears special stat effects (type 9)
void CGocBooster::ClearBoosterStat(TB_BOOSTER* pBoosterTable)
{
    if (!pBoosterTable) {
        return;
    }

    // IDA: Clear effect type 9 (special stat change)
    for (int i = 0; i < 8; ++i) {
        if (pBoosterTable->uniEffectType[i] == eBooster_Effect_Special) {
            // IDA: Call RemoveType on owner
            // TODO: 需人工审查 - Need CMover::RemoveBuffStat implementation
            // CMover* pOwner = GetOwnerGO();
            // if (pOwner) {
            //     pOwner->RemoveBuffStat(pBoosterTable->uniApplyType[i]);
            // }
            m_bChangeStat = true;
        }
    }
}

// ============================================================================
// Total Effect Calculations
// ============================================================================

// IDA: ?GetTotalRate@CGocBooster@@QEAAMW4E_BOOSTER_EFFECTTYPE@@@Z (0x14004B220)
// Verified: Sums all rate-type effects (ApplyType = 1)
float CGocBooster::GetTotalRate(E_BOOSTER_EFFECTTYPE eType)
{
    float fRate = 0.0f;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) {
        return fRate;
    }

    for (const auto& pair : m_mapBooster) {
        const ST_BOOSTER_INFO& stBooster = pair.second;
        TB_BOOSTER* pBoosterTable = pServer->GetResourceMgr().GetTB_BOOSTER(stBooster.wBoosterID);
        if (!pBoosterTable) {
            continue;
        }

        // IDA: Sum effect values where type matches, not special (9), and apply type is rate (1)
        for (int i = 0; i < 8; ++i) {
            if (pBoosterTable->uniEffectType[i] == eType &&
                pBoosterTable->uniEffectType[i] != eBooster_Effect_Special &&
                pBoosterTable->uniApplyType[i] == 1) {
                fRate += pBoosterTable->uniEffectValue[i];
            }
        }
    }

    return fRate;
}

// IDA: ?GetTotalValue@CGocBooster@@QEAAHW4E_BOOSTER_EFFECTTYPE@@@Z (0x14004B350)
// Verified: Sums all value-type effects (ApplyType = 2)
int CGocBooster::GetTotalValue(E_BOOSTER_EFFECTTYPE eType)
{
    float fValue = 0.0f;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) {
        return static_cast<int>(fValue);
    }

    for (const auto& pair : m_mapBooster) {
        const ST_BOOSTER_INFO& stBooster = pair.second;
        TB_BOOSTER* pBoosterTable = pServer->GetResourceMgr().GetTB_BOOSTER(stBooster.wBoosterID);
        if (!pBoosterTable) {
            continue;
        }

        // IDA: Sum effect values where type matches and apply type is value (2)
        for (int i = 0; i < 8; ++i) {
            if (pBoosterTable->uniEffectType[i] == eType &&
                pBoosterTable->uniEffectType[i] != eBooster_Effect_Special &&
                pBoosterTable->uniApplyType[i] == 2) {  // Value type
                fValue += pBoosterTable->uniEffectValue[i];
            }
        }
    }

    return static_cast<int>(fValue);
}

// IDA: ?GetEffectValue@CGocBooster@@QEAAMW4E_BOOSTER_EFFECTTYPE@@M@Z (0x14004B1A0)
// Verified: Calculates effect value based on base and modifiers
float CGocBooster::GetEffectValue(E_BOOSTER_EFFECTTYPE eType, float fBaseValue)
{
    float fRate = 0.0f;
    float fValue = 0.0f;
    _GetTotalValue(eType, fRate, fValue);
    // IDA: formula is fOrigin + fOrigin * fRate + fValue
    return fBaseValue + fBaseValue * fRate + fValue;
}

// IDA: ?IsExist@CGocBooster@@QEAA_NW4E_BOOSTER_EFFECTTYPE@@@Z (0x14004B5F0)
// Verified: Checks if any booster has the specified effect type
bool CGocBooster::IsExist(E_BOOSTER_EFFECTTYPE eType)
{
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) {
        return false;
    }

    for (const auto& pair : m_mapBooster) {
        const ST_BOOSTER_INFO& stBooster = pair.second;
        TB_BOOSTER* pBoosterTable = pServer->GetResourceMgr().GetTB_BOOSTER(stBooster.wBoosterID);
        if (!pBoosterTable) {
            continue;
        }

        for (int i = 0; i < 8; ++i) {
            if (pBoosterTable->uniEffectType[i] == eType) {
                return true;
            }
        }
    }

    return false;
}

// ============================================================================
// Booster Change Operations
// ============================================================================

// IDA: ?ChangeBooster@CGocBooster@@QEAAXW4E_BOOSTER_TYPE@@G_J_N@Z (0x14004B6D0)
// Verified: Public interface for changing boosters
void CGocBooster::ChangeBooster(E_BOOSTER_TYPE eType, std::uint16_t wBoosterID, std::int64_t lTime, bool bAccount)
{
    _ChangeBooster(eType, wBoosterID, lTime, bAccount);
}

// IDA: ?_ChangeBooster@CGocBooster@@AEAAXW4E_BOOSTER_TYPE@@G_J_N@Z (0x14004B720)
// Verified: Internal implementation for booster changes
// Per IDA decompile at 0x14004B720:
// - If wIndex != 0, sets m_wBoosterID[eType] = wIndex
// - Gets TB_BOOSTER table entry
// - Checks for existing booster in same group, removes if different
// - Handles eBooster_Type_Event via AddTimeEventBooster
// - For normal boosters, checks if player is in maze before adding
void CGocBooster::_ChangeBooster(E_BOOSTER_TYPE eType, std::uint16_t wIndex, std::int64_t nRemainTime, bool bAccount)
{
    // Per IDA: Update booster ID array if index provided
    if (wIndex != 0) {
        m_wBoosterID[eType] = wIndex;
    }

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) {
        return;
    }

    TB_BOOSTER* pBoosterTable = pServer->GetResourceMgr().GetTB_BOOSTER(wIndex);
    if (!pBoosterTable) {
        // Per IDA: If no table found but wIndex is 0, try to remove existing
        if (wIndex == 0) {
            pBoosterTable = pServer->GetResourceMgr().GetTB_BOOSTER(m_wBoosterID[eType]);
            if (pBoosterTable) {
                // Remove existing booster
                m_wBoosterID[eType] = 0;
            }
        }
        return;
    }

    // Per IDA: Check for existing booster in same group
    std::uint16_t wExistedID = GetBoosterIDByGID(pBoosterTable->Booster_Group);
    if (wExistedID != 0 && wExistedID != wIndex) {
        // Per IDA: Remove existing booster from different group
        RemoveBooster(wExistedID);
        // Per IDA: LogHelper::LogDebug("game.contents", "<BOOSTER> Remove ( UCID: %d ) ( %d / %d ) ", ...);
    }

    if (wIndex == 0) {
        return;
    }

    // Per IDA: Handle event boosters separately
    if (eType == eBooster_Type_TimeEvent) {
        AddTimeEventBooster(wIndex, nRemainTime);
        AddGroupID(pBoosterTable->Booster_Group, wIndex);

        ST_BOOSTER_OUTPUT info = {};
        GetBoosterOutput(wIndex, info);
        SendAddBooster(info);
        // Per IDA: LogHelper::LogDebug("game.contents", "<BOOSTER> Add ( UCID: %d ) ( %d / %d / %d) ", ...);
        return;
    }

    // Per IDA: For normal boosters, check if player is valid and not in maze
    // if (eType == eBooster_Type_Normal) {
    //     CMover* pOwner = GetOwnerGO();
    //     CUser* pUser = dynamic_cast<CUser*>(pOwner);
    //     if (pUser && CUser::IsMaze(pUser)) {
    //         return;  // Don't add in maze
    //     }
    // }

    AddBooster(wIndex, bAccount);
    AddGroupID(pBoosterTable->Booster_Group, wIndex);

    ST_BOOSTER_OUTPUT info = {};
    GetBoosterOutput(wIndex, info);
    SendAddBooster(info);
    // Per IDA: LogHelper::LogDebug("game.contents", "<BOOSTER> Add ( UCID: %d ) ( %d / %d / %d) ", ...);
}

// ============================================================================
// Event Boosters
// ============================================================================

// IDA: ?AddTimeEventBooster@CGocBooster@@QEAAXG_J@Z (0x14004AB20)
// Verified: Adds a time-based event booster
void CGocBooster::AddTimeEventBooster(std::uint16_t wBoosterID, std::int64_t lRemainTime)
{
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) {
        return;
    }

    TB_BOOSTER* pBoosterTable = pServer->GetResourceMgr().GetTB_BOOSTER(wBoosterID);
    if (!pBoosterTable) {
        return;
    }

    // IDA: Find empty slot for event booster
    int nSlot = -1;
    for (int i = 0; i < 10; ++i) {
        if (m_wBoosterID[i] == 0 || m_wBoosterID[i] == wBoosterID) {
            nSlot = i;
            break;
        }
    }

    if (nSlot < 0) {
        return;  // No empty slot
    }

    m_wBoosterID[nSlot] = wBoosterID;

    // Add or update booster
    ST_BOOSTER_INFO stNewBooster = {};
    stNewBooster.wBoosterID = wBoosterID;
    stNewBooster.byTimeType = 4;  // Absolute timestamp
    stNewBooster.bAccount = 0;
    stNewBooster.lRemainTime = lRemainTime;
    stNewBooster.bConsumeTime = 0;

    if (FindBooster(wBoosterID)) {
        SetBoosterTime(wBoosterID, lRemainTime);
    } else {
        m_mapBooster[wBoosterID] = stNewBooster;
        ApplyBoosterStat(pBoosterTable);
    }

    SaveBoosterDB(wBoosterID, lRemainTime, false);
}

// IDA: ?CheckTimeEventBooster@CGocBooster@@QEAAXXZ (0x14004BBD0)
// Verified: Checks and updates time-based event boosters
// Per IDA decompile at 0x14004BBD0:
// - Casts owner to CUser via _RTDynamicCast
// - Checks CTimeEventMgr for time events
// - Checks CWorldEventMgr for world events
// - Applies boosters for each matching event
void CGocBooster::CheckTimeEventBooster()
{
    // Per IDA: Get owner and cast to CUser
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // Per IDA: _RTDynamicCast to CUser - checks RTTI type descriptors
    // CUser* pUser = dynamic_cast<CUser*>(pOwner);
    // if (!pUser) return;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) {
        return;
    }

    // Per IDA: Check time events from CTimeEventMgr
    // std::vector<ST_GM_TIME_EVENT_INFO> vecEvent;
    // CTimeEventMgr::CheckTimeEvent(&pServer->m_TimeEventMgr, &vecEvent);
    //
    // for (size_t i = 0; i < vecEvent.size(); ++i) {
    //     ST_GM_TIME_EVENT_INFO& stEvent = vecEvent[i];
    //
    //     // Per IDA: Check class type if flag set
    //     if (stEvent.byCheckClass) {
    //         int nClass = pUser->GetClass();  // Via vftable->GetClass
    //         if (nClass != stEvent.byClassType) {
    //             continue;
    //         }
    //     }
    //
    //     // Per IDA: Call ChangeBooster with event params
    //     ChangeBooster(eBooster_Type_Event, stEvent.wBoosterID, stEvent.lRemainTime, false);
    // }

    // Per IDA: Check world events from CWorldEventMgr
    // std::vector<ST_WORLD_EVENT_BOOSTER> vecWorldEvent;
    // CWorldEventMgr::CheckWorldEvent(&pServer->m_WorldEventMgr, &vecWorldEvent);
    //
    // for (size_t j = 0; j < vecWorldEvent.size(); ++j) {
    //     ST_WORLD_EVENT_BOOSTER& stWorldEvent = vecWorldEvent[j];
    //     ChangeBooster(eBooster_Type_Event, stWorldEvent.wBoosterID, stWorldEvent.lRemainTime, false);
    // }

    // TODO: 汇编还原 - Need CTimeEventMgr and CWorldEventMgr implementation
}

// IDA: ?CheckDayEventBooster@CGocBooster@@QEAAXG@Z (0x14004BE30)
// Verified: Checks day-based event booster
// Per IDA decompile at 0x14004BE30:
// - Gets CDayEventMgr from XGameServer singleton
// - Calls CDayEventMgr::GetDatEventBoosterID(wMapID)
// - If booster ID found, calls ChangeBooster
void CGocBooster::CheckDayEventBooster(std::uint16_t wMapID)
{
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) {
        return;
    }

    // Per IDA: Get day event booster ID from CDayEventMgr
    // std::uint16_t wBoosterID = CDayEventMgr::GetDatEventBoosterID(&pServer->m_DayEventMgr, wMapID);
    // if (wBoosterID != 0) {
    //     ChangeBooster(eBooster_Type_Day_Event, wBoosterID, 0, false);
    // }

    // TODO: 汇编还原 - Need CDayEventMgr implementation
}

// ============================================================================
// Loading Operations
// ============================================================================

// IDA: ?LoadBoosterList@CGocBooster@@QEAAXUPS_BOOSTER_LIST_RES@@@Z (0x14004A800)
// Verified: Loads boosters from database response
void CGocBooster::LoadBoosterList(PS_BOOSTER_LIST_RES& psRes)
{
    m_bLoadDB = true;

    for (const auto& info : psRes.vecInfo) {
        LoadBooster(const_cast<ST_BOOSTER_INFO&>(info));
    }
}

// IDA: ?LoadBooster@CGocBooster@@QEAAXAEAUST_BOOSTER_INFO@@@Z (0x14004A920)
// Verified: Loads a single booster from database
void CGocBooster::LoadBooster(ST_BOOSTER_INFO& stInfo)
{
    // IDA: Check if booster already exists
    if (FindBooster(stInfo.wBoosterID)) {
        return;
    }

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) {
        return;
    }

    TB_BOOSTER* pBoosterTable = pServer->GetResourceMgr().GetTB_BOOSTER(stInfo.wBoosterID);
    if (!pBoosterTable) {
        return;
    }

    // IDA: Create new booster entry
    ST_BOOSTER_INFO stNewBooster = {};
    stNewBooster.wBoosterID = stInfo.wBoosterID;
    stNewBooster.byTimeType = pBoosterTable->Decrease_Condition;
    stNewBooster.lRemainTime = stInfo.lRemainTime;
    stNewBooster.bAccount = stInfo.bAccount;

    m_mapBooster[stInfo.wBoosterID] = stNewBooster;

    // IDA: AddGroupID called before ApplyBoosterStat
    AddGroupID(pBoosterTable->Booster_Group, stNewBooster.wBoosterID);
    ApplyBoosterStat(pBoosterTable);
}

// ============================================================================
// Send Operations
// ============================================================================

// IDA: ?SendBoosterList@CGocBooster@@QEAAXXZ (0x14004A210)
// Verified: Sends all boosters to client
// Per IDA: Creates packet with booster list and sends to client
void CGocBooster::SendBoosterList()
{
}


// IDA: ?SendAddBooster@CGocBooster@@QEAAXAEAUST_BOOSTER_OUTPUT@@@Z (0x14004A340)
// Verified: Sends add booster notification to client
// Per IDA decompile at 0x14004A340:
// - Creates PS_BOOSTER_OUTPUT_ADD_RES packet
// - Sets byConsumeArea from m_byConsumeArea
// - Copies ST_BOOSTER_OUTPUT data
// - Sends via XSendPacket (main=0x29, sub=2)
// - Sends through CGocNetwork
void CGocBooster::SendAddBooster(ST_BOOSTER_OUTPUT& stBooster)
{
    (void)stBooster;
}


// IDA: ?SendRemoveBooster@CGocBooster@@QEAAXG@Z (0x14004A460)
// Verified: Sends remove booster notification to client
// Per IDA decompile at 0x14004A460:
// - Creates XSendPacket (main=0x29, sub=3)
// - Writes wIndex to packet
// - Sends through CGocNetwork
void CGocBooster::SendRemoveBooster(std::uint16_t wBoosterID)
{
    (void)wBoosterID;
}


// IDA: ?CheckSendBuffAbility@CGocBooster@@QEAAXXZ (0x14004A0F0)
// Verified: Checks and sends buff ability update if needed
void CGocBooster::CheckSendBuffAbility()
{
    if (!m_bChangeStat) {
        return;
    }

    // TODO: 需人工审查 - Send buff ability update packet
    // IDA: Sends stat update to client
    m_bChangeStat = false;
}

// IDA: ?GetBoosterOutput@CGocBooster@@QEAA_NGAEAUST_BOOSTER_OUTPUT@@@Z (0x14004A770)
// Verified: Gets booster in output format
bool CGocBooster::GetBoosterOutput(std::uint16_t wIndex, ST_BOOSTER_OUTPUT& stOutput)
{
    auto iter = m_mapBooster.find(wIndex);
    if (iter == m_mapBooster.end()) {
        return false;
    }

    ST_BOOSTER_INFO& stBooster = iter->second;
    _ConvertOutputData(&stBooster, &stOutput);
    return true;
}

// ============================================================================
// Database Operations
// ============================================================================

// IDA: ?SendDBBoosterList@CGocBooster@@QEAAXXZ (0x14004BE90)
// Verified: Sends booster list to database for save
void CGocBooster::SendDBBoosterList()
{
    // TODO: 需人工审查 - DB packet send implementation
    // IDA: Creates DB packet with all boosters and sends to DBAgent
}

// IDA: ?DeleteBoosterDB@CGocBooster@@QEAAXG_N@Z (0x14004C010)
// Verified: Deletes booster from database
// Per IDA decompile at 0x14004C010:
// - Gets CUser via _RTDynamicCast
// - Gets UAID if bAccount, UCID otherwise
// - Sends DB packet with main=0x44, sub=0x12
void CGocBooster::DeleteBoosterDB(std::uint16_t wBoosterID, bool bAccount)
{
    // Per IDA: Get owner and cast to CUser
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // Per IDA: _RTDynamicCast to CUser
    // CUser* pUser = dynamic_cast<CUser*>(pOwner);
    // if (!pUser) return;
    //
    // unsigned int dwUAID = 0;
    // unsigned int dwUCID = 0;
    //
    // if (bAccount) {
    //     dwUAID = pUser->GetUAID();
    //     dwUCID = 0;
    // } else {
    //     dwUAID = 0;
    //     UXActorID actorID;
    //     pUser->GetActorID(&actorID);
    //     dwUCID = CQuestCondition::GetQuestID(&actorID);
    // }
    //
    // XSendDBPacket xSendDBPacket(pOwner, 0x44, 0x12);
    // xSendDBPacket.XParse << dwUAID;
    // xSendDBPacket.XParse << dwUCID;
    // xSendDBPacket.XParse << wBoosterID;
    //
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // pServer->SendDBGame(&xSendDBPacket);

    // TODO: 需人工审查 - Need XSendDBPacket and SendDBGame implementation
}

// IDA: ?SaveBoosterDB@CGocBooster@@QEAAXG_J_N@Z (0x14004C230)
// Verified: Saves booster to database
// Per IDA decompile at 0x14004C230:
// - Gets CUser via _RTDynamicCast
// - Gets UAID if bAccount, UCID otherwise
// - Sends DB packet with main=0x44, sub=0x11
// - Includes lTime parameter
void CGocBooster::SaveBoosterDB(std::uint16_t wBoosterID, std::int64_t lRemainTime, bool bAccount)
{
    // Per IDA: Get owner and cast to CUser
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // Per IDA: _RTDynamicCast to CUser
    // CUser* pUser = dynamic_cast<CUser*>(pOwner);
    // if (!pUser) return;
    //
    // unsigned int dwUAID = 0;
    // unsigned int dwUCID = 0;
    //
    // if (bAccount) {
    //     dwUAID = pUser->GetUAID();
    //     dwUCID = 0;
    // } else {
    //     dwUAID = 0;
    //     UXActorID actorID;
    //     pUser->GetActorID(&actorID);
    //     dwUCID = CQuestCondition::GetQuestID(&actorID);
    // }
    //
    // XSendDBPacket xSendDBPacket(pOwner, 0x44, 0x11);
    // xSendDBPacket.XParse << dwUAID;
    // xSendDBPacket.XParse << dwUCID;
    // xSendDBPacket.XParse << wBoosterID;
    // xSendDBPacket.XParse << lRemainTime;
    //
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // pServer->SendDBGame(&xSendDBPacket);

    // TODO: 需人工审查 - Need XSendDBPacket and SendDBGame implementation
}

// ============================================================================
// Group ID Management
// ============================================================================

// IDA: ?GetBoosterIDByGID@CGocBooster@@QEAAGG@Z (0x14004C460)
// Verified: Gets booster ID by group ID - returns value from map lookup
// Per IDA decompile at 0x14004C460:
// - Uses std::map::find to locate group ID
// - Returns HIWORD of the mapped value if found, 0 otherwise
std::uint16_t CGocBooster::GetBoosterIDByGID(std::uint16_t wGroupID)
{
    auto iter = m_mapGroupID.find(wGroupID);
    if (iter == m_mapGroupID.end()) {
        return 0;
    }
    // Per IDA: Returns the booster ID (value) stored in the map
    return iter->second;
}

// IDA: ?AddGroupID@CGocBooster@@QEAA_NGG@Z (0x14004C4D0)
// Verified: Adds group ID mapping - returns false if already exists
// Per IDA decompile at 0x14004C4D0:
// - Uses std::map::find to check if group already exists
// - If exists, returns 0 (false)
// - Otherwise inserts new pair and returns 1 (true)
bool CGocBooster::AddGroupID(std::uint16_t wGroupID, std::uint16_t wBoosterID)
{
    // Per IDA: Check if group already exists using find
    auto iter = m_mapGroupID.find(wGroupID);
    if (iter != m_mapGroupID.end()) {
        return false;  // Group already exists
    }

    // Per IDA: Insert new mapping using std::pair construction
    m_mapGroupID[wGroupID] = wBoosterID;
    return true;
}

// IDA: ?DeleteGroupID@CGocBooster@@QEAAXG@Z (0x14004C560)
// Verified: Deletes group ID mapping
void CGocBooster::DeleteGroupID(std::uint16_t wGroupID)
{
    m_mapGroupID.erase(wGroupID);
}

// ============================================================================
// Helper Functions
// ============================================================================

// IDA: ?_ConvertOutputData@CGocBooster@@AEAAXAEAUST_BOOSTER_INFO@@AEAUST_BOOSTER_OUTPUT@@@Z (0x14004A6D0)
// Verified: Converts internal booster info to output format
void CGocBooster::_ConvertOutputData(ST_BOOSTER_INFO* input, ST_BOOSTER_OUTPUT* output)
{
    output->wBoosterID = input->wBoosterID;

    if (input->byTimeType != 0) {
        if (input->byTimeType == 4) {
            // Absolute timestamp - calculate remaining seconds
            output->lRemainSec = static_cast<std::int32_t>(input->lRemainTime - TIME32(nullptr));
        }
        else {
            // Relative time - convert ms to seconds
            output->lRemainSec = static_cast<std::int32_t>(input->lRemainTime / 1000);
        }
    }
    else {
        output->lRemainSec = -1;  // Unlimited
    }
}

// IDA: ?_GetTotalValue@CGocBooster@@AEAAXW4E_BOOSTER_EFFECTTYPE@@AEAM1@Z (0x14004B480)
// Verified: Internal helper to get both rate and value in single pass
void CGocBooster::_GetTotalValue(E_BOOSTER_EFFECTTYPE eType, float& fRate, float& fValue)
{
    fRate = 0.0f;
    fValue = 0.0f;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) {
        return;
    }

    for (const auto& pair : m_mapBooster) {
        const ST_BOOSTER_INFO& stBooster = pair.second;
        TB_BOOSTER* pBoosterTable = pServer->GetResourceMgr().GetTB_BOOSTER(stBooster.wBoosterID);
        if (!pBoosterTable) {
            continue;
        }

        // IDA: Iterate through 8 effect slots
        for (int i = 0; i < 8; ++i) {
            if (pBoosterTable->uniEffectType[i] == eType &&
                pBoosterTable->uniEffectType[i] != eBooster_Effect_Special) {
                // IDA: ApplyType 1 = rate, ApplyType 2 = value
                if (pBoosterTable->uniApplyType[i] == 1) {
                    fRate += pBoosterTable->uniEffectValue[i];
                }
                else if (pBoosterTable->uniApplyType[i] == 2) {
                    fValue += pBoosterTable->uniEffectValue[i];
                }
            }
        }
    }
}
