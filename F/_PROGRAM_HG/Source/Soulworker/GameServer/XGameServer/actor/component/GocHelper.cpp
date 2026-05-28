#include "GocHelper.h"
#include "GocNetwork.h"
#include "GocAttribute.h"
#include "GocNpcAttribute.h"
#include "GocParty.h"
#include "../User.h"
#include "../Monster.h"
#include "../Ai.h"
#include "../GameServer.h"
#include "../../XCore/XArea/XMaze.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include <ctime>

// ============================================================================
// CGocHelper - Game Object Component for helper/assistant system
// ============================================================================

// IDA: ??0CGocHelper@@QEAA@XZ (0x140091E90)
CGocHelper::CGocHelper()
    : GOComponent()
    , m_tLastSummon(0)
    , m_mapSummonedHelper()
    , m_mapHelper()
    , m_stMySupport()
    , m_bMySupportResister(0)
    , m_byMySupportReward(0)
    , m_byAutoSummonFlag(1)
{
    Clear();
}

// IDA: ??1CGocHelper@@UEAA@XZ (0x140091F80)
CGocHelper::~CGocHelper() {
    Clear();
    m_mapHelper.clear();
    m_mapSummonedHelper.clear();
}

// GOComponent interface
bool CGocHelper::Initialize() {
    return true;
}

void CGocHelper::Shutdown() {
    Clear();
}

void CGocHelper::Update(float fDeltaTime) {
    (void)fDeltaTime;
    // TODO: 汇编还原 - Update logic if needed
}

// IDA: ?Init@CGocHelper@@QEAAXXZ (0x140091FE0)
void CGocHelper::Init() {
    Clear();
}

// IDA: ?Clear@CGocHelper@@QEAAXXZ (0x140092000)
void CGocHelper::Clear() {
    m_tLastSummon = 0;
    m_mapSummonedHelper.clear();
    m_mapHelper.clear();
    m_bMySupportResister = 0;
    m_byMySupportReward = 0;
    m_byAutoSummonFlag = 1;
    memset(&m_stMySupport, 0, sizeof(m_stMySupport));
}

// IDA: ?Reset@CGocHelper@@QEAAXXZ (0x140092080)
void CGocHelper::Reset() {
    m_tLastSummon = 0;
    m_mapSummonedHelper.clear();
}

// IDA: ?GetFamilyID@CGocHelper@@SAHXZ (0x140039020)
int CGocHelper::GetFamilyID() {
    return 16;
}

// IDA: ?SetHelperList@CGocHelper@@QEAA_NAEAUPS_HELPER_LIST_RES@@@Z (0x1400920C0)
bool CGocHelper::SetHelperList(PS_HELPER_LIST_RES& psList) {
    SetAutoSummonFlag(psList.byAutoSummon);

    for (size_t i = 0; i < psList.vecHelper.size(); ++i) {
        ST_HELPER_INFO stAdd = psList.vecHelper[i];
        AddMyHelper(stAdd, false);
    }

    SendHelperList();
    return true;
}

// IDA: ?GetHelperList@CGocHelper@@QEAA_NAEAV?$vector@UST_HELPER_INFO@@V?$allocator@UST_HELPER_INFO@@@std@@@std@@_N@Z (0x1400922B0)
bool CGocHelper::GetHelperList(std::vector<ST_HELPER_INFO>& vecHelper, bool isOrder) {
    vecHelper.clear();

    if (isOrder) {
        // Sort by order
        std::map<std::uint8_t, ST_HELPER_INFO> mapHelperOrder;

        for (auto it = m_mapHelper.begin(); it != m_mapHelper.end(); ++it) {
            mapHelperOrder[it->second.byOrder] = it->second;
        }

        for (auto it = mapHelperOrder.begin(); it != mapHelperOrder.end(); ++it) {
            vecHelper.push_back(it->second);
        }
    } else {
        // No ordering, just iterate
        for (auto it = m_mapHelper.begin(); it != m_mapHelper.end(); ++it) {
            vecHelper.push_back(it->second);
        }
    }

    return true;
}

// IDA: ?SendHelperList@CGocHelper@@QEAAXXZ (0x140092560)
void CGocHelper::SendHelperList() {
    // TODO: 汇编还原 - Need to get owner actor and send packet
    // PS_HELPER_LIST_RES psHelper;
    // psHelper.dwUCID = ...;
    // psHelper.byAutoSummon = IsAutoSummon();
    // GetHelperList(psHelper.vecHelper, false);
    // XSendPacket xSendPacket(0x27, 1);
    // xSendPacket << psHelper;
    // CGocNetwork::Send(pActor, &xSendPacket);
}

// IDA: ?HelperSummon@CGocHelper@@QEAA_NK@Z (0x140093410)
bool CGocHelper::HelperSummon(std::uint32_t dwHelperID) {
    // TODO: 汇编还原 - Complex summon logic
    // Key steps from IDA:
    // 1. Check owner exists and is valid
    // 2. Check maze type is valid (not type 12)
    // 3. Get helper info from m_mapHelper
    // 4. Get TB_HELPER and TB_MONSTER from XResourceMgr
    // 5. Create monster at position near player
    // 6. Set owner, direction, AI
    // 7. Initialize stats via CGocNpcAttribute
    // 8. Add to XMaze helper list
    // 9. Send PS_HELPER_SUMMON_RES packet

    // Placeholder implementation
    ST_HELPER_INFO stHelperInfo;
    if (!GetHelperInfo(dwHelperID, stHelperInfo)) {
        return false;
    }

    // TODO: 汇编还原 - Full summon logic
    // Need access to:
    // - XGameServer singleton
    // - XResourceMgr for table data
    // - Owner CUser
    // - XMaze
    // - CMonster creation

    return false; // TODO: Implement
}

// IDA: ?HelperRelease@CGocHelper@@QEAA_NK@Z (0x140094300)
bool CGocHelper::HelperRelease(std::uint32_t dwHelperID) {
    // TODO: 汇编还原 - Release summoned helper
    // Key steps from IDA:
    // 1. Check owner and maze exist
    // 2. Get summoned helper CMonster
    // 3. Disable helper warp in AI
    // 4. Update helper info state
    // 5. Call HelperSupportRelease
    // 6. Delete from XMaze
    // 7. Remove from m_mapSummonedHelper
    // 8. Send log to DB
    // 9. Send PS_HELPER_SUMMON_RES packet

    auto it = m_mapSummonedHelper.find(dwHelperID);
    if (it == m_mapSummonedHelper.end()) {
        return false;
    }

    CMonster* pMonster = it->second;
    if (!pMonster) {
        return false;
    }

    // TODO: 汇编还原 - Full release logic

    return false; // TODO: Implement
}

// Helper info access
bool CGocHelper::GetHelperInfo(std::uint32_t dwHelperID, ST_HELPER_INFO& stInfo) {
    for (auto it = m_mapHelper.begin(); it != m_mapHelper.end(); ++it) {
        if (it->second.dwHelperID == dwHelperID) {
            stInfo = it->second;
            return true;
        }
    }
    return false;
}

// IDA: ?SetHelperSummonState@CGocHelper@@QEAAXK_N@Z (0x140092C20)
void CGocHelper::SetHelperSummonState(std::uint32_t dwHelperID, bool bSummon) {
    auto it = m_mapHelper.find(dwHelperID);
    if (it != m_mapHelper.end()) {
        it->second.bSummon = bSummon;
    }
}

void CGocHelper::SetHelperSummonTime() {
    m_tLastSummon = std::time(nullptr);
}

CMonster* CGocHelper::GetSummonedHelper(std::uint32_t dwHelperID) {
    auto it = m_mapSummonedHelper.find(dwHelperID);
    if (it != m_mapSummonedHelper.end()) {
        return it->second;
    }
    return nullptr;
}

// IDA: ?GetSummonedHelperList@CGocHelper@@QEAAXAEAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@@Z (0x140092B40)
void CGocHelper::GetSummonedHelperList(std::map<std::uint32_t, std::uint32_t>& mapSummonHelperList) {
    mapSummonHelperList.clear();

    for (auto it = m_mapSummonedHelper.begin(); it != m_mapSummonedHelper.end(); ++it) {
        std::uint32_t dwHelperID = it->first;
        CMonster* pMonster = it->second;

        if (pMonster) {
            // Get actor ID from monster
            // TODO: 汇编还原 - Get actor ID from CMonster
            // mapSummonHelperList[dwHelperID] = pMonster->GetActorID();
        }
    }
}

// IDA: ?CheckSummonHelper@CGocHelper@@QEAA_NK@Z (0x140092D10)
bool CGocHelper::CheckSummonHelper(std::uint32_t dwHelperID) {
    // Check if helper exists in m_mapHelper and NOT in m_mapSummonedHelper
    bool bInHelperList = false;
    for (auto it = m_mapHelper.begin(); it != m_mapHelper.end(); ++it) {
        if (it->second.dwHelperID == dwHelperID) {
            bInHelperList = true;
            break;
        }
    }

    if (!bInHelperList) {
        return false;
    }

    // Check if already summoned
    auto itSummoned = m_mapSummonedHelper.find(dwHelperID);
    return (itSummoned == m_mapSummonedHelper.end());
}

// IDA: ?FindHelper@CGocHelper@@QEAA_NK@Z (0x140092AE0)
bool CGocHelper::FindHelper(std::uint32_t dwHelperID) {
    for (auto it = m_mapHelper.begin(); it != m_mapHelper.end(); ++it) {
        if (it->second.dwHelperID == dwHelperID) {
            return true;
        }
    }
    return false;
}

// IDA: ?CheckReleaseHelper@CGocHelper@@QEAA_NK@Z
bool CGocHelper::CheckReleaseHelper(std::uint32_t dwHelperID) {
    // Check if helper is currently summoned
    auto it = m_mapSummonedHelper.find(dwHelperID);
    return (it != m_mapSummonedHelper.end());
}

// IDA: ?CheckHelperSummonDelay@CGocHelper@@QEAA_NXZ
bool CGocHelper::CheckHelperSummonDelay() {
    // TODO: 汇编还原 - Check summon delay time
    // Compare current time with m_tLastSummon
    std::time_t now = std::time(nullptr);
    // Return true if enough time has passed since last summon
    return true; // TODO: Implement actual delay check
}

// IDA: ?CheckSummonHelperCount@CGocHelper@@QEAA_NXZ
bool CGocHelper::CheckSummonHelperCount() {
    // TODO: 汇编还原 - Check if we can summon more helpers
    // Return true if count is below limit
    return m_mapSummonedHelper.size() < 3; // Default limit is 3
}

// Auto summon
void CGocHelper::SetAutoSummonFlag(std::uint8_t byFlag) {
    m_byAutoSummonFlag = byFlag;
}

bool CGocHelper::IsAutoSummon() const {
    return m_byAutoSummonFlag != 0;
}

// Helper operations
// IDA: ?AddMyHelper@CGocHelper@@QEAA_NAEAUST_HELPER_INFO@@_N@Z (0x140092900)
bool CGocHelper::AddMyHelper(ST_HELPER_INFO& stInfo, bool bSend) {
    // Check TB_HELPER table
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        return false;
    }

    // TODO: 汇编还原 - Check TB_HELPER table
    // if (!XResourceMgr::GetTB_HELPER(&pGameServer->m_xResourceMgr, stInfo.dwHelperID)) {
    //     return false;
    // }

    // Check if helper already exists
    if (FindHelper(stInfo.dwHelperID)) {
        return false;
    }

    // Calculate helper stats
    CalcHelperStatsALL(&stInfo, true);

    // Add to m_mapHelper (key is byOrder)
    m_mapHelper[stInfo.byOrder] = stInfo;

    // Send packet if requested
    if (bSend) {
        // TODO: 汇编还原 - Send packet 0x27 sub 3
        // XSendPacket xSendPacket(0x27, 3);
        // xSendPacket << stInfo;
        // CGocNetwork::Send(pActor, &xSendPacket);
    }

    return true;
}

void CGocHelper::CalcHelperStatsALL(ST_HELPER_INFO* pstInfo, bool bRecalc) {
    // TODO: 汇编还原 - Calculate all helper stats
    // From IDA: processes vecAddditional (stat updates)
    // Applies via CGocNpcAttribute
    if (!pstInfo) {
        return;
    }

    // Process vecAddditional vector
    // for (size_t i = 0; i < pstInfo->vecAddditional.size(); ++i) {
    //     Apply stat updates
    // }
}

void CGocHelper::HelperSupportRelease(std::uint32_t dwHelperID) {
    // TODO: 汇编还原 - Release helper support
    // Called from HelperRelease
}

// IDA: ?HelperProcess@CGocHelper@@QEAAXAEAUPS_HELPER_SUMMON_REQ@@@Z (0x140093080)
void CGocHelper::HelperProcess(struct PS_HELPER_SUMMON_REQ& psInfo) {
    // TODO: 汇编还原 - Process helper summon/release request
    // Key logic from IDA:
    // 1. Check if owner is dead
    // 2. If bSummon:
    //    - CheckHelperSummonDelay
    //    - Check maze conditions
    //    - CheckSummonHelperCount
    //    - Call HelperSummon
    // 3. If release:
    //    - CheckReleaseHelper
    //    - Call HelperRelease

    // Get owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }

    // Check if owner is dead
    // TODO: 汇编还原 - if (CMover::IsDie(pOwner)) return;

    if (psInfo.bSummon) {
        // Summon request
        if (!CheckHelperSummonDelay()) {
            // TODO: Send error message 0xE295
            return;
        }

        if (!CheckSummonHelper(psInfo.dwHelperID)) {
            // TODO: Send error message 0xE297
            return;
        }

        if (!CheckSummonHelperCount()) {
            // TODO: Send error message 0xE298
            return;
        }

        HelperSummon(psInfo.dwHelperID);
    } else {
        // Release request
        if (!CheckReleaseHelper(psInfo.dwHelperID)) {
            // TODO: Send error message 0xE297
            return;
        }

        HelperRelease(psInfo.dwHelperID);
    }
}

// Support info
void CGocHelper::SetMySupportInfo(const ST_HELPER_SUPPORT_INFO& stInfo) {
    m_stMySupport = stInfo;
}
