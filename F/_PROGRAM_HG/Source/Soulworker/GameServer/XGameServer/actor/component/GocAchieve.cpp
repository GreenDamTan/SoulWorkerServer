// CGocAchieve - Game Object Component for Achievement System
// Restored from GameServer.exe IDA decompilation
// Address range: 0x140029030 - 0x14002EEB6

#include "GocAchieve.h"
#include "Soulworker/GameServer/XGameServer/AchieveType.h"
#include "Soulworker/GameServer/XGameServer/Achieve.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#endif

// Helper to get owner mover
CMover* CGocAchieve::GetOwnerMover() const
{
    return static_cast<CMover*>(m_pOwner);
}

// Constructor (0x140029030)
// IDA verified - Initializes base class and members
CGocAchieve::CGocAchieve()
    : GOComponent()
    , m_dw64LastUpdate(0)
{
    // IDA shows: GOComponent constructor called, then member initialization
    // ST_ACHIEVE_CATEGORY and ST_ACHIEVE_BIT have their own constructors
    // std::map default constructor for m_mpAchieveTypeList
}

// Destructor (0x1400290E0)
// IDA verified - Just destroys members naturally via vtable
CGocAchieve::~CGocAchieve()
{
    // IDA shows: sets vtable, then destroys map and calls GOComponent destructor
    // Does NOT call ClearAchieve - members are destroyed naturally
}

// Initialize (0x140029130)
bool CGocAchieve::Initialize()
{
    Init();
    return true;
}

void CGocAchieve::Shutdown()
{
    ClearAchieve();
}

void CGocAchieve::Update(float fDeltaTime)
{
    (void)fDeltaTime;
}

// GetFamilyID (0x1400487D0)
// IDA verified - Returns 14
int CGocAchieve::GetFamilyID()
{
    return 14;
}

// Init (0x140029130)
// IDA verified - Calls ClearAchieve
void CGocAchieve::Init()
{
    ClearAchieve();
}

// ClearAchieve (0x140029150)
// IDA verified - Clear all achievement data
void CGocAchieve::ClearAchieve()
{
    m_dw64LastUpdate = 0;
    std::memset(&m_stCategory, 0, sizeof(m_stCategory));
    std::memset(&m_stAchieveBit, 0, sizeof(m_stAchieveBit));

    // IDA shows: iterate and call CAchieveType::Clear() on each element before clearing
    for (auto it = m_mpAchieveTypeList.begin(); it != m_mpAchieveTypeList.end(); ++it) {
        CAchieveType* pAchieveType = it->second.get();
        if (pAchieveType) {
            pAchieveType->Clear();
        }
    }

    m_mpAchieveTypeList.clear();
}

// CheckAchieveReward (0x140029220)
// IDA verified - Check if achievement reward can be claimed
bool CGocAchieve::CheckAchieveReward(int nBit)
{
    if (nBit <= 0) {
        // TODO: Log error - LogHelper::LogError("game.contents", "CheckAchieveReward error - [ ActorID:%d, Bit:%d ] ( %d )", ...);
        return false;
    }

    int nIndex = nBit / 8;
    int nPos = nBit % 8;

    if (nIndex >= 128) {
        // TODO: Log error - index out of range
        return false;
    }

    // Check if reward bit is already set (already claimed)
    if ((m_stAchieveBit.szRewardBit[nIndex] & (1 << nPos)) != 0) {
        // TODO: Log error - reward already claimed
        return false;
    }

    return true;
}

// SetAchieveReward (0x1400293A0)
// IDA verified - Set achievement reward claimed
void CGocAchieve::SetAchieveReward(int nBit)
{
    if (nBit <= 0) {
        // TODO: Log error - LogHelper::LogError("game.contents", "SetAchieveReward error - [ ActorID:%d, Bit:%d ] ( %d )", ...);
        return;
    }

    int nIndex = nBit / 8;
    if (nIndex < 128) {
        m_stAchieveBit.szRewardBit[nIndex] |= (1 << (nBit % 8));
    }
}

// SendDBAchieveList (0x140029470)
// IDA verified - Send achievement list request to DB
void CGocAchieve::SendDBAchieveList()
{
    // IDA: Create XSendDBPacket with main=3, sub=0x61
    // Get QuestID from CQuestCondition::GetQuestID and serialize
    // Send via XGameServer::SendDBGame

    CMover* pMover = GetOwnerMover();
    if (!pMover) return;

    // TODO: Requires XSendDBPacket implementation
    // XSendDBPacket xSendDBPacket(pMover, 3, 0x61);
    // XParse::operator<<(&xSendDBPacket.XParse, QuestID);
    // XGameServer::Instance()->SendDBGame(&xSendDBPacket);
}

// SendDBUpdateList (0x140029580)
// IDA verified - Send update list to DB and user
void CGocAchieve::SendDBUpdateList(ST_ACHIEVE_UPDATE_LIST& stSendUser, ST_ACHIEVE_UPDATE_LIST& stSendDB)
{
    // IDA: Cast owner to CUser via RTTI
    // If stSendDB has entries, send to DB (main=3, sub=0x62)
    // Update open titles for each achievement via CGocEntity::UpdateOpenTitle
    // Send achievement log via XGameServer::SendDBAchieveLog
    // If stSendUser has entries, send to client (main=3, sub=0x71)

    CMover* pMover = GetOwnerMover();
    if (!pMover) {
        stSendUser.vecList.clear();
        stSendDB.vecList.clear();
        return;
    }

    // TODO: Requires RTTI cast to CUser and full packet implementation
    stSendUser.vecList.clear();
    stSendDB.vecList.clear();
}

// AchieveReward (0x140029B70)
// IDA verified - Process achievement reward claim
bool CGocAchieve::AchieveReward(int nIndex)
{
    // IDA: Complex flow:
    // 1. Get TB_ACHIEVEMENT from XResourceMgr::GetTB_ACHIEVEMENT
    // 2. Get CAchieve pointer via GetAchievePtr
    // 3. Check if achievement count meets requirement
    // 4. Check inventory capacity for rewards via CGocInventory::CheckOverMoney
    // 5. Check if reward can be claimed (CheckAchieveReward)
    // 6. Create reward items via CGocInventory::CreateItemReq
    // 7. Add titles via CGocEntity::AddTitleByClass
    // 8. Add gold/BP/Ether via CGocInventory::AddMoney/AddBP/AddEther
    // 9. Set reward claimed via SetAchieveReward
    // 10. Send DB update (main=3, sub=0x63)
    // 11. Log achievement claim

    // TODO: 需人工审查 - Requires full inventory and packet implementation
    (void)nIndex;
    return false;
}

// UpdateEnduranceAchieve (0x14002A8D0)
// IDA verified - Update endurance achievement
void CGocAchieve::UpdateEnduranceAchieve(std::uint8_t byEquipType)
{
    ST_ACHIEVE_UPDATE_LIST stSendUser;
    ST_ACHIEVE_UPDATE_LIST stSendDB;

    // IDA shows switch on byEquipType:
    // byEquipType == 0: normal equipment -> UpdateCollect(9,1), UpdateCollect(7,1)
    // byEquipType == 1: special equipment -> UpdateCollect(0xA,1), UpdateCollect(7,1)
    // Then UpdateCollect(6,1) for both

    if (byEquipType == 0) {
        UpdateCollect(9, 1, stSendUser, stSendDB, 0);
        UpdateCollect(7, 1, stSendUser, stSendDB, 0);
    } else if (byEquipType == 1) {
        UpdateCollect(0xA, 1, stSendUser, stSendDB, 0);
        UpdateCollect(7, 1, stSendUser, stSendDB, 0);
    } else {
        return;
    }

    UpdateCollect(6, 1, stSendUser, stSendDB, 0);
    SendDBUpdateList(stSendUser, stSendDB);
}

// UpdateQuestAchieve (0x14002AAD0)
// IDA verified - Update quest achievement
void CGocAchieve::UpdateQuestAchieve(std::uint8_t byQuestType)
{
    ST_ACHIEVE_UPDATE_LIST stSendUser;
    ST_ACHIEVE_UPDATE_LIST stSendDB;

    // IDA shows switch on byQuestType:
    // byQuestType == 0: UpdateCollect(0x1D, 1)
    // byQuestType == 1: UpdateCollect(0x1E, 1)
    // byQuestType == 2: (no specific type, falls through)
    // Then UpdateCollect(0x1C, 1) for all valid cases

    if (byQuestType == 0) {
        UpdateCollect(0x1D, 1, stSendUser, stSendDB, 0);
    } else if (byQuestType == 1) {
        UpdateCollect(0x1E, 1, stSendUser, stSendDB, 0);
    } else if (byQuestType != 2) {
        // Invalid quest type
        return;
    }

    UpdateCollect(0x1C, 1, stSendUser, stSendDB, 0);
    SendDBUpdateList(stSendUser, stSendDB);
}

// UpdateMonsterAchieve (0x14002AC80)
// IDA verified - Update monster kill achievement
void CGocAchieve::UpdateMonsterAchieve(TB_MONSTER* pTBMonster)
{
    if (!pTBMonster) return;

    ST_ACHIEVE_UPDATE_LIST stSendUser;
    ST_ACHIEVE_UPDATE_LIST stSendDB;

    // IDA shows switch on pTBMonster->Monster_Rank:
    // case 0: UpdateCollect(0x14, 1) - normal monster
    // case 1: UpdateCollect(0x15, 1) - elite
    // case 2: UpdateCollect(0x16, 1) - champion
    // case 3: UpdateCollect(0x2C, 1) - boss
    // case 4: UpdateCollect(0x17, 1) - raid boss
    // case 5: UpdateCollect(0x18, 1) - world boss

    switch (pTBMonster->Monster_Rank) {
        case 0:
            UpdateCollect(0x14, 1, stSendUser, stSendDB, 0);
            break;
        case 1:
            UpdateCollect(0x15, 1, stSendUser, stSendDB, 0);
            break;
        case 2:
            UpdateCollect(0x16, 1, stSendUser, stSendDB, 0);
            break;
        case 3:
            UpdateCollect(0x2C, 1, stSendUser, stSendDB, 0);
            break;
        case 4:
            UpdateCollect(0x17, 1, stSendUser, stSendDB, 0);
            break;
        case 5:
            UpdateCollect(0x18, 1, stSendUser, stSendDB, 0);
            break;
        default:
            break;
    }

    // Update monster-specific achievements
    UpdateCollect(0x13, 1, stSendUser, stSendDB, pTBMonster->ID);
    UpdateCollect(0x13, 1, stSendUser, stSendDB, pTBMonster->Monster_Type);
    UpdateCollect(0x19, 1, stSendUser, stSendDB, pTBMonster->Monster_Faction);

    SendDBUpdateList(stSendUser, stSendDB);
}

// UpdatemMazeClearAchieve (0x14002AF90)
// IDA verified - Update maze clear achievement
void CGocAchieve::UpdatemMazeClearAchieve(int nRank, int nClearTime, int nMazeID, std::uint8_t byMazeType)
{
    ST_ACHIEVE_UPDATE_LIST stSendUser;
    ST_ACHIEVE_UPDATE_LIST stSendDB;

    // IDA: If nClearTime <= 180 seconds, UpdateCollect(0x24, 1) - speed run
    if (nClearTime <= 180) {
        UpdateCollect(0x24, 1, stSendUser, stSendDB, 0);
    }

    // Switch on nRank:
    // case 0: UpdateCollect(0x2A, 1) - D rank
    // case 1: UpdateCollect(0x29, 1) - C rank
    // case 2: UpdateCollect(0x28, 1) - B rank
    // case 3: UpdateCollect(0x27, 1) - A rank
    // case 4: UpdateCollect(0x26, 1) - S rank
    // case 5: UpdateCollect(0x25, 1) - SS rank

    switch (nRank) {
        case 0:
            UpdateCollect(0x2A, 1, stSendUser, stSendDB, 0);
            break;
        case 1:
            UpdateCollect(0x29, 1, stSendUser, stSendDB, 0);
            break;
        case 2:
            UpdateCollect(0x28, 1, stSendUser, stSendDB, 0);
            break;
        case 3:
            UpdateCollect(0x27, 1, stSendUser, stSendDB, 0);
            break;
        case 4:
            UpdateCollect(0x26, 1, stSendUser, stSendDB, 0);
            break;
        case 5:
            UpdateCollect(0x25, 1, stSendUser, stSendDB, 0);
            // Then update maze-specific achievements
            UpdateCollect(0x23, 1, stSendUser, stSendDB, nMazeID);
            UpdateCollect(0x23, 1, stSendUser, stSendDB, 0);
            if (byMazeType) {
                UpdateCollect(0x2F, 1, stSendUser, stSendDB, byMazeType);
            }
            SendDBUpdateList(stSendUser, stSendDB);
            break;
        default:
            // No valid rank - just clean up
            break;
    }
}

// OnUpdatePlayTime (0x14002B2E0)
// IDA verified - Update play time achievement
void CGocAchieve::OnUpdatePlayTime()
{
    // IDA: Get CUser from owner, check if user has achievement flag set
    // Check if m_dw64LastUpdate is 0 or expired
    // If so, update m_dw64LastUpdate to GetTickCount64() + 60000
    // Call UpdateCollect(0x2B, 1, 0) - play time achievement type

#ifdef _WIN32
    std::uint64_t dwCurrentTime = ::GetTickCount64();
    if (m_dw64LastUpdate == 0 || m_dw64LastUpdate < dwCurrentTime) {
        m_dw64LastUpdate = dwCurrentTime + 60000; // 1 minute interval
        UpdateCollect(0x2B, 1, 0);
    }
#endif
}

// GMClearAchieve (0x14002B390)
// IDA verified - GM command to clear all achievements
void CGocAchieve::GMClearAchieve()
{
    ClearAchieve();

    // IDA: Send DB packet (main=3, sub=0x66) to notify database
    CMover* pMover = GetOwnerMover();
    if (!pMover) return;

    // TODO: Send DB clear packet
    // XSendDBPacket xSendDBPacket(pMover, 3, 0x66);
    // XParse::operator<<(&xSendDBPacket.XParse, QuestID);
    // XGameServer::Instance()->SendDBGame(&xSendDBPacket);
}

// GMAchieveComplete (0x14002B4B0)
// IDA verified - GM command to complete achievement
void CGocAchieve::GMAchieveComplete(int nGroupID, int nStep)
{
    if (nStep < 0) return;

    // IDA: Complex logic:
    // If nStep > 0: Complete specific achievement step
    // If nStep == 0: Complete all achievements in group
    // Uses XResourceMgr::GetTB_ACHIEVEMENT to get achievement data
    // Creates/updates achievements via CAchieveType::AddAchieve
    // Sends DB and client updates

    // TODO: 需人工审查 - Requires full implementation with XResourceMgr access
    (void)nGroupID;
    (void)nStep;
}

// LoadAchieve (0x14002BDD0)
// IDA verified - Load achievement data from DB response
void CGocAchieve::LoadAchieve(ST_ACHIEVE_BIT& stAchieveBit, ST_ACHIEVE_LIST& stAchieveList,
                               ST_ACHIEVE_CATEGORY& stCatagory, bool bFirst)
{
    // IDA: Copy bit and category data
    std::memcpy(&m_stAchieveBit, &stAchieveBit, sizeof(m_stAchieveBit));
    std::memcpy(&m_stCategory, &stCatagory, sizeof(m_stCategory));

    // TODO: Get owner level
    // std::uint8_t shLevel = ...;

    // IDA: Iterate TB_ACHIEVEMENT_BEGIN table via XGameServer singleton
    // For each entry where Achievement_Open_Lv <= shLevel:
    //   Get TB_ACHIEVEMENT and call InitAchieve

    // Process each achievement in the list
    // Handle level-type achievements (type 32)
    // Handle achievement progression to next level
    // Update category counts
    // Send DB update if needed

    // TODO: 需人工审查 - Requires XResourceMgr and XGameServer access
    (void)stAchieveList;
    (void)bFirst;
}

// CheckAchieveComplete (0x14002CA30)
// IDA verified - Check if achievement bit index is valid
bool CGocAchieve::CheckAchieveComplete(int nBit)
{
    if (nBit <= 0) {
        // TODO: Log error - LogHelper::LogError("game.contents", "CheckAchieveComplete error - [ ActorID:%d, Bit:%d ] ( %d )", ...);
        return false;
    }

    // IDA shows: returns true if nIndex <= 128
    return (nBit / 8) <= 128;
}

// InitAchieve (TB_ACHIEVEMENT*) (0x14002CAD0)
// IDA verified - Initialize achievement from table
void CGocAchieve::InitAchieve(TB_ACHIEVEMENT* pTBAchieve)
{
    if (!pTBAchieve) {
        // TODO: Log error - LogHelper::LogError("game.contents", "InitAchieve error - No Table TB_ACHIVEMENT[ ActorID:%d ] ( %d )", ...);
        return;
    }

    // Find existing achieve type
    auto it = m_mpAchieveTypeList.find(pTBAchieve->Achievement_type);
    if (it != m_mpAchieveTypeList.end()) {
        // Type exists, initialize achievement
        CAchieveType* pAchieveType = it->second.get();
        if (pAchieveType) {
            pAchieveType->Init(pTBAchieve);
        }
    } else {
        // Create new achieve type
        std::shared_ptr<CAchieveType> pAchieveType = std::make_shared<CAchieveType>();
        if (pAchieveType) {
            pAchieveType->Init(pTBAchieve);
            m_mpAchieveTypeList[pTBAchieve->Achievement_type] = pAchieveType;
        }
    }
}

// InitAchieve (ST_ACHIEVE_INFO&) (0x14002CCE0)
// IDA verified - Initialize achievement from info struct
void CGocAchieve::InitAchieve(ST_ACHIEVE_INFO& stAchieveInfo)
{
    // IDA: Get TB_ACHIEVEMENT from XResourceMgr::GetTB_ACHIEVEMENT
    // Find achieve type in map
    // Find specific achievement via CAchieveType::FindAchieve
    // Set achievement count via CAchieve::SetAchieve

    // TODO: Requires XResourceMgr access
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_ACHIEVEMENT* pTBAchieve = XResourceMgr::GetTB_ACHIEVEMENT(&pServer->m_xResourceMgr, stAchieveInfo.nIndex);
    // if (!pTBAchieve) return;

    // auto it = m_mpAchieveTypeList.find(pTBAchieve->Achievement_type);
    // if (it == m_mpAchieveTypeList.end()) return;

    // CAchieveType* pAchieveType = it->second.get();
    // std::shared_ptr<CAchieve> pAchieve = pAchieveType->FindAchieve(pTBAchieve->taget_ID);
    // if (pAchieve) {
    //     pAchieve->SetAchieve(pTBAchieve, stAchieveInfo.biCount);
    // }
    (void)stAchieveInfo;
}

// UpdateAchieve1 (0x14002CEB0)
// IDA verified - Update achievement count with immediate DB sync
void CGocAchieve::UpdateAchieve1(std::uint16_t wType, int nCount, int nTargetID)
{
    // IDA: Find achieve type, find achievement, call CAchieve::UpdateCount
    // Send to DB (main=3, sub=0x62), update open titles, send to client (main=3, sub=0x71)

    auto it = m_mpAchieveTypeList.find(wType);
    if (it == m_mpAchieveTypeList.end()) {
        return;
    }

    CAchieveType* pAchieveType = it->second.get();
    if (!pAchieveType) return;

    std::shared_ptr<CAchieve> pAchieve = pAchieveType->FindAchieve(nTargetID);
    if (!pAchieve) {
        return;
    }

    ST_ACHIEVE_UPDATE stAchieveUpdate;
    if (!pAchieve->UpdateCount(nCount, &stAchieveUpdate, &m_stAchieveBit, &m_stCategory)) {
        return;
    }

    ST_ACHIEVE_UPDATE_LIST stUpdateList;
    stUpdateList.vecList.push_back(stAchieveUpdate);

    // TODO: Send to DB, update titles via CGocEntity::UpdateOpenTitle, send to client
}

// UpdateCollect (immediate) (0x14002D590)
// IDA verified - Update collect achievement with immediate send
void CGocAchieve::UpdateCollect(std::uint16_t wType, int nCount, int nTargetID)
{
    auto it = m_mpAchieveTypeList.find(wType);
    if (it == m_mpAchieveTypeList.end()) {
        return;
    }

    CAchieveType* pAchieveType = it->second.get();
    if (!pAchieveType) return;

    std::shared_ptr<CAchieve> pAchieve = pAchieveType->FindAchieve(nTargetID);
    if (!pAchieve) {
        return;
    }

    ST_ACHIEVE_UPDATE stAchieveUpdate;
    if (!pAchieve->UpdateCollectCount(nCount, &stAchieveUpdate, &m_stAchieveBit, &m_stCategory)) {
        return;
    }

    if (stAchieveUpdate.stUpdateInfo.nIndex == 0) {
        return;
    }

    ST_ACHIEVE_UPDATE_LIST stUpdateList;
    stUpdateList.vecList.push_back(stAchieveUpdate);

    // TODO: Send to DB if nNextIndex != 0, update titles, send to client
}

// UpdateCollect (batch) (0x14002DD30)
// IDA verified - Update collect achievement with batch processing
void CGocAchieve::UpdateCollect(std::uint16_t wType, int nCount, ST_ACHIEVE_UPDATE_LIST& stSendUser,
                                 ST_ACHIEVE_UPDATE_LIST& stSendDB, int nTargetID)
{
    auto it = m_mpAchieveTypeList.find(wType);
    if (it == m_mpAchieveTypeList.end()) {
        return;
    }

    CAchieveType* pAchieveType = it->second.get();
    if (!pAchieveType) return;

    std::shared_ptr<CAchieve> pAchieve = pAchieveType->FindAchieve(nTargetID);
    if (!pAchieve) {
        return;
    }

    ST_ACHIEVE_UPDATE stAchieveUpdate;
    if (!pAchieve->UpdateCollectCount(nCount, &stAchieveUpdate, &m_stAchieveBit, &m_stCategory)) {
        return;
    }

    if (stAchieveUpdate.stUpdateInfo.nIndex == 0) {
        return;
    }

    // Add to send user list
    stSendUser.vecList.push_back(stAchieveUpdate);

    // Add to send DB list if there's a next index (achievement completed/progressed)
    if (stAchieveUpdate.nNextIndex != 0) {
        stSendDB.vecList.push_back(stAchieveUpdate);
    }
}

// LevelUp (0x14002DEC0)
// IDA verified - Handle level up achievements
void CGocAchieve::LevelUp()
{
    // IDA: Get owner level via vtable call
    // Call UpdateAchieve1(0x20, shLevel, 0) - level achievement type
    // Iterate TB_ACHIEVEMENT_BEGIN table, init achievements where Achievement_Open_Lv <= shLevel

    // TODO: Get owner level
    // std::uint8_t shLevel = ...;
    // UpdateAchieve1(0x20, shLevel, 0);

    // TODO: Iterate TB_ACHIEVEMENT_BEGIN via XGameServer singleton
}

// EndCollect (0x14002E000)
// IDA verified - End collection and send updates
void CGocAchieve::EndCollect()
{
    // IDA: Get CUser via RTTI, iterate all achieve types and call EndCollect
    // Send DB packet (main=3, sub=0x65)
    // Update open titles for each achievement

    ST_ACHIEVE_UPDATE_LIST stUpdateList;

    for (auto& pair : m_mpAchieveTypeList) {
        CAchieveType* pAchieveType = pair.second.get();
        if (pAchieveType) {
            pAchieveType->EndCollect(&stUpdateList);
        }
    }

    // TODO: Send DB packet and update titles
}

// GMAchieveCount (0x14002E510)
// IDA verified - GM command to set achievement count
void CGocAchieve::GMAchieveCount(int nGroupID, int nCount)
{
    // IDA: Calculate nAchieveIndex = 100 * nGroupID + 1
    // Get TB_ACHIEVEMENT from XResourceMgr::GetTB_ACHIEVEMENT
    // Find or create CAchieveType, add achievement if needed
    // Call UpdateAchieve1 with adjusted count

    int nAchieveIndex = 100 * nGroupID + 1;
    (void)nAchieveIndex;
    (void)nCount;

    // TODO: Requires XResourceMgr access
}

// SendAchieveList (0x14002EA80)
// IDA verified - Send achievement list to client
void CGocAchieve::SendAchieveList()
{
    ST_ACHIEVE_LIST stAchieveList;

    // Load all achievements from each type
    for (auto& pair : m_mpAchieveTypeList) {
        CAchieveType* pAchieveType = pair.second.get();
        if (pAchieveType) {
            pAchieveType->LoadAchieve(&stAchieveList);
        }
    }

    // TODO: Send packet to client (main=3, sub=0x70)
    // XSendPacket xSendPacket(3, 0x70);
    // operator<<(&xSendPacket, &m_stAchieveBit);
    // operator<<(&xSendPacket, &stAchieveList);
    // operator<<(&xSendPacket, &m_stCategory);
    // CGocNetwork::Send(pActor, &xSendPacket);

#ifdef _WIN32
    m_dw64LastUpdate = ::GetTickCount64();
#endif
}

// GetAchievePtr (0x14002EC60)
// IDA verified - Get achievement pointer by type and target ID
std::shared_ptr<CAchieve> CGocAchieve::GetAchievePtr(std::uint16_t wAchieveType, int nTagetID)
{
    auto it = m_mpAchieveTypeList.find(wAchieveType);
    if (it == m_mpAchieveTypeList.end()) {
        return nullptr;
    }

    CAchieveType* pAchieveType = it->second.get();
    if (!pAchieveType) {
        return nullptr;
    }

    return pAchieveType->FindAchieve(nTagetID);
}

// UpdateItemUpgradeAchieve (0x14002ED30)
// IDA verified - Update item upgrade achievement
void CGocAchieve::UpdateItemUpgradeAchieve(std::uint8_t byResult)
{
    // IDA: byResult == 1: success -> UpdateAchieve1(0x3A, 1, 0)
    // byResult != 1: failure -> UpdateAchieve1(0x32, 1, 0)
    // Always call UpdateAchieve1(0x31, 1, 0) - total upgrade attempts

    if (byResult == 1) {
        UpdateAchieve1(0x3A, 1, 0);  // Upgrade success
    } else {
        UpdateAchieve1(0x32, 1, 0);  // Upgrade failure
    }
    UpdateAchieve1(0x31, 1, 0);  // Total attempts
}

// UpdateHarvestAchieve (0x14002EDA0)
// IDA verified - Update harvest achievement
void CGocAchieve::UpdateHarvestAchieve(std::uint8_t byHarvestType)
{
    // IDA: Switch on byHarvestType:
    // case 0, 1: UpdateCollect(0x3D, 1, 0)
    // case 2: UpdateCollect(0x3E, 1, 0)
    // case 3: UpdateCollect(0x3F, 1, 0)

    switch (byHarvestType) {
        case 0:
        case 1:
            UpdateCollect(0x3D, 1, 0);
            break;
        case 2:
            UpdateCollect(0x3E, 1, 0);
            break;
        case 3:
            UpdateCollect(0x3F, 1, 0);
            break;
        default:
            break;
    }
}

// GetFirstAchieveID (0x14002EE30)
// IDA verified - Get first achievement ID in chain by walking backwards
std::uint32_t CGocAchieve::GetFirstAchieveID(TB_ACHIEVEMENT* pTBAchieve, std::uint16_t& wNowCategoryCount)
{
    std::uint32_t dwAchieveID = 0;

    // IDA: Walk backwards through achievement chain by looking up ID-1
    while (pTBAchieve) {
        dwAchieveID = pTBAchieve->ID;
        std::uint32_t dwIndex = pTBAchieve->ID - 1;

        // TODO: Get TB_ACHIEVEMENT from XResourceMgr
        // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        // pTBAchieve = XResourceMgr::GetTB_ACHIEVEMENT(&pServer->m_xResourceMgr, dwIndex);
        pTBAchieve = nullptr;  // Stub - break loop

        if (pTBAchieve) {
            ++wNowCategoryCount;
        }
    }

    return dwAchieveID;
}
