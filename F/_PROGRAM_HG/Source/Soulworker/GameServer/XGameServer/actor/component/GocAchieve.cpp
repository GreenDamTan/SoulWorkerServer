// CGocAchieve - Game Object Component for Achievement System
// Restored from GameServer.exe IDA decompilation
// Address range: 0x140029030 - 0x14002EEB6

#include "GocAchieve.h"
#include "Soulworker/GameServer/XGameServer/AchieveType.h"
#include "Soulworker/GameServer/XGameServer/Achieve.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#endif

// Forward declarations for types not yet available
class XGameServer;
class CUser;

// Temporary stub for TXSingleton - TODO: Replace with proper implementation
namespace {
    XGameServer* GetGameServerInstance() {
        // TODO: Implement proper singleton access
        return nullptr;
    }
}

// Helper to get owner mover
CMover* CGocAchieve::GetOwnerMover() const
{
    // Owner is stored in GOComponent base class
    return static_cast<CMover*>(m_pOwner);
}

// Helper to get actor ID from owner
static int GetOwnerActorID(CGocAchieve* pThis)
{
    CMover* pMover = pThis->GetOwnerMover();
    if (!pMover) return 0;
    // TODO: Implement GetActorID when CMover interface is available
    return 0;
}

// Helper to get level from owner
static std::uint8_t GetOwnerLevel(CGocAchieve* pThis)
{
    CMover* pMover = pThis->GetOwnerMover();
    if (!pMover) return 0;
    // TODO: Implement GetLevel when CMover interface is available
    return 0;
}

// Constructor (0x140029030)
CGocAchieve::CGocAchieve()
    : GOComponent()
    , m_dw64LastUpdate(0)
{
    std::memset(&m_stAchieveBit, 0, sizeof(m_stAchieveBit));
    std::memset(&m_stCategory, 0, sizeof(m_stCategory));
}

// Destructor (0x1400290E0)
CGocAchieve::~CGocAchieve()
{
    ClearAchieve();
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
int CGocAchieve::GetFamilyID()
{
    return 14;
}

// Init (0x140029130)
void CGocAchieve::Init()
{
    ClearAchieve();
}

// ClearAchieve (0x140029150)
void CGocAchieve::ClearAchieve()
{
    m_dw64LastUpdate = 0;
    m_mpAchieveTypeList.clear();
    std::memset(&m_stAchieveBit, 0, sizeof(m_stAchieveBit));
    std::memset(&m_stCategory, 0, sizeof(m_stCategory));
}

// CheckAchieveReward (0x140029220)
// IDA verified - Check if achievement reward can be claimed
bool CGocAchieve::CheckAchieveReward(int nBit)
{
    if (nBit <= 0) {
        return false;
    }

    int nIndex = nBit / 8;
    int nPos = nBit % 8;

    if (nIndex >= 128) {
        return false;
    }

    // Check if reward bit is already set (already claimed)
    if ((m_stAchieveBit.szRewardBit[nIndex] & (1 << nPos)) != 0) {
        return false;
    }

    return true;
}

// SetAchieveReward (0x1400293A0)
// IDA verified - Set achievement reward claimed
void CGocAchieve::SetAchieveReward(int nBit)
{
    if (nBit <= 0) {
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
    // IDA decompilation shows:
    // 1. Get owner object
    // 2. Create XSendDBPacket with main=3, sub=0x61
    // 3. Get QuestID and serialize
    // 4. Send to DB via XGameServer::SendDBGame

    // TODO: 需人工审查 - Requires XSendDBPacket implementation
    CMover* pMover = GetOwnerMover();
    if (!pMover) return;

    // TODO: Implement when XSendDBPacket is available
    // XSendDBPacket xSendDBPacket(pMover, 3, 0x61);
    // int nQuestID = ...;
    // xSendDBPacket.XParse << nQuestID;
    // XGameServer::Instance()->SendDBGame(&xSendDBPacket);
}

// SendDBUpdateList (0x140029580)
// IDA verified - Send update list to DB and user
void CGocAchieve::SendDBUpdateList(ST_ACHIEVE_UPDATE_LIST& stSendUser, ST_ACHIEVE_UPDATE_LIST& stSendDB)
{
    // IDA decompilation shows:
    // 1. Cast owner to CUser
    // 2. If stSendDB has entries, send to DB (main=3, sub=0x62)
    // 3. Update open titles for each achievement
    // 4. Send achievement log
    // 5. If stSendUser has entries, send to client (main=3, sub=0x71)
    // 6. Cleanup lists

    CMover* pMover = GetOwnerMover();
    if (!pMover) {
        stSendUser.vecList.clear();
        stSendDB.vecList.clear();
        return;
    }

    // TODO: 需人工审查 - Requires XSendDBPacket and XSendPacket implementation
    // The IDA code shows complex logic for DB sync and client notification

    // Cleanup
    stSendUser.vecList.clear();
    stSendDB.vecList.clear();
}

// AchieveReward (0x140029B70)
// IDA verified - Process achievement reward claim
bool CGocAchieve::AchieveReward(int nIndex)
{
    // IDA decompilation shows complex flow:
    // 1. Get TB_ACHIEVEMENT from resource manager
    // 2. Get CAchieve pointer for the achievement type/target
    // 3. Check if achievement count meets requirement
    // 4. Check inventory capacity for rewards
    // 5. Check if reward can be claimed (CheckAchieveReward)
    // 6. Create reward items
    // 7. Add titles, gold, BP, Ether
    // 8. Set reward claimed
    // 9. Send DB update
    // 10. Log achievement claim

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

    // IDA shows:
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
void CGocAchieve::UpdateQuestAchieve(std::uint8_t byQuestType)
{
    // TODO: 需人工审查 - Need to decompile and determine quest type mapping
    (void)byQuestType;
}

// UpdateMonsterAchieve (0x14002AC80)
void CGocAchieve::UpdateMonsterAchieve(TB_MONSTER* pTBMonster)
{
    if (!pTBMonster) return;

    ST_ACHIEVE_UPDATE_LIST stSendUser;
    ST_ACHIEVE_UPDATE_LIST stSendDB;

    // TODO: 需人工审查 - Need TB_MONSTER structure details
    // IDA shows logic based on monster rank/type

    SendDBUpdateList(stSendUser, stSendDB);
}

// UpdatemMazeClearAchieve (0x14002AF90)
void CGocAchieve::UpdatemMazeClearAchieve(int nRank, int nClearTime, int nMazeID, std::uint8_t byMazeType)
{
    // TODO: 需人工审查 - Need to decompile maze clear achievement logic
    (void)nRank;
    (void)nClearTime;
    (void)nMazeID;
    (void)byMazeType;
}

// OnUpdatePlayTime (0x14002B2E0)
// IDA verified - Update play time achievement
void CGocAchieve::OnUpdatePlayTime()
{
#ifdef _WIN32
    std::uint64_t dwCurrentTime = ::GetTickCount64();
    if (m_dw64LastUpdate == 0 || m_dw64LastUpdate < dwCurrentTime) {
        m_dw64LastUpdate = dwCurrentTime + 60000; // 1 minute interval
        UpdateCollect(0x2B, 1, 0); // Update play time achievement (type 0x2B)
    }
#endif
}

// GMClearAchieve (0x14002B390)
void CGocAchieve::GMClearAchieve()
{
    ClearAchieve();
}

// GMAchieveComplete (0x14002B4B0)
void CGocAchieve::GMAchieveComplete(int nGroupID, int nStep)
{
    // TODO: 需人工审查 - Need to decompile GM achievement complete
    (void)nGroupID;
    (void)nStep;
}

// LoadAchieve (0x14002BDD0)
// IDA verified - Load achievement data from DB response
void CGocAchieve::LoadAchieve(ST_ACHIEVE_BIT& stAchieveBit, ST_ACHIEVE_LIST& stAchieveList,
                               ST_ACHIEVE_CATEGORY& stCatagory, bool bFirst)
{
    // IDA shows complex initialization flow:
    // 1. Copy bit and category data
    // 2. Get owner level
    // 3. Initialize achievements from TB_ACHIEVEMENT_BEGIN table based on level
    // 4. Process each achievement in the list
    // 5. Handle level-type achievements (type 32)
    // 6. Handle achievement progression to next level
    // 7. Update category counts
    // 8. Send DB update if needed

    // Copy input data
    m_stAchieveBit = stAchieveBit;
    m_stCategory = stCatagory;

    std::uint8_t shLevel = GetOwnerLevel(this);

    // Initialize from TB_ACHIEVEMENT_BEGIN table
    // TODO: Need XResourceMgr access to iterate TB_ACHIEVEMENT_BEGIN
    XGameServer* pServer = GetGameServerInstance();
    if (pServer) {
        // TODO: Iterate m_xResourceMgr.m_mapTB_ACHIEVEMENT_BEGIN
        // For each entry where Achievement_Open_Lv <= shLevel:
        //   Get TB_ACHIEVEMENT and call InitAchieve
    }

    ST_ACHIEVE_UPDATE_LIST stUpdateList;
    bool bSendDB = false;
    std::map<std::uint8_t, ST_ACHIEVE_UPDATE> mapCategory;

    // Process each achievement in list
    for (size_t i = 0; i < stAchieveList.vecList.size(); ++i) {
        ST_ACHIEVE_INFO& stInfo = stAchieveList.vecList[i];

        // TODO: Get TB_ACHIEVEMENT from resource manager
        // TB_ACHIEVEMENT* pTBAchieve = XResourceMgr::GetTB_ACHIEVEMENT(stInfo.nIndex);
        // if (!pTBAchieve) continue;

        bool bChange = false;
        ST_ACHIEVE_UPDATE stAchieveUpdate;
        // TODO: Initialize stAchieveUpdate from pTBAchieve and stInfo

        // Handle level type achievements (type 32)
        // if (pTBAchieve->Achievement_type == 32 && stInfo.biCount != shLevel) ...

        // Handle achievement progression
        // if (pTBAchieve->Achievement_count <= stInfo.biCount) ...

        // Initialize achievement with loaded data
        InitAchieve(stInfo);

        if (bChange) {
            bSendDB = true;
            stUpdateList.vecList.push_back(stAchieveUpdate);
        }
    }

    // Handle first load category tracking
    if (bFirst) {
        for (int cat = 0; cat < 7; ++cat) {
            auto itCat = mapCategory.find(cat);
            if (itCat != mapCategory.end()) {
                if (m_stCategory.wCount[cat] != itCat->second.wCount) {
                    bSendDB = true;
                    m_stCategory.wCount[cat] = itCat->second.wCount;
                    stUpdateList.vecList.push_back(itCat->second);
                }
            }
        }
    }

    // Send DB update if needed
    if (bSendDB) {
        // TODO: Send DB update packet (main=3, sub=0x62)
    }
}

// CheckAchieveComplete (0x14002CA30)
// IDA verified - Check if achievement is complete
bool CGocAchieve::CheckAchieveComplete(int nBit)
{
    if (nBit <= 0) return false;

    int nIndex = nBit / 8;
    int nPos = nBit % 8;

    if (nIndex >= 128) return false;

    // Return true if the bit is set (achievement completed)
    return (m_stAchieveBit.szRewardBit[nIndex] & (1 << nPos)) != 0;
}

// InitAchieve (TB_ACHIEVEMENT*) (0x14002CAD0)
// IDA verified - Initialize achievement from table
void CGocAchieve::InitAchieve(TB_ACHIEVEMENT* pTBAchieve)
{
    // IDA shows:
    // 1. Check pTBAchieve is valid
    // 2. Find existing achieve type in map
    // 3. If found, call CAchieveType::Init
    // 4. If not found, create new CAchieveType, init, and insert

    if (!pTBAchieve) {
        // LogHelper::LogError("game.contents", "InitAchieve error - No Table TB_ACHIVEMENT[ ActorID:%d ] ( %d )", ...);
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
    // IDA shows:
    // 1. Get TB_ACHIEVEMENT from resource manager
    // 2. Find achieve type
    // 3. Find specific achievement
    // 4. Set achievement count

    // TODO: Need XResourceMgr access
    // TB_ACHIEVEMENT* pTBAchieve = XResourceMgr::GetTB_ACHIEVEMENT(stAchieveInfo.nIndex);
    // if (!pTBAchieve) return;

    // Find achieve type
    // auto it = m_mpAchieveTypeList.find(pTBAchieve->Achievement_type);
    // if (it == m_mpAchieveTypeList.end()) return;

    // CAchieveType* pAchieveType = it->second.get();
    // std::shared_ptr<CAchieve> pAchieve = pAchieveType->FindAchieve(pTBAchieve->taget_ID);
    // if (!pAchieve) return;

    // pAchieve->SetAchieve(pTBAchieve, stAchieveInfo.biCount);
}

// UpdateAchieve1 (0x14002CEB0)
// IDA verified - Update achievement count with immediate DB sync
void CGocAchieve::UpdateAchieve1(std::uint16_t wType, int nCount, int nTargetID)
{
    // IDA shows:
    // 1. Find achieve type
    // 2. Find specific achievement
    // 3. Call CAchieve::UpdateCount
    // 4. Send to DB (main=3, sub=0x62)
    // 5. Send achievement log
    // 6. Update open titles
    // 7. Send to client (main=3, sub=0x71)

    // Find achieve type
    auto it = m_mpAchieveTypeList.find(wType);
    if (it == m_mpAchieveTypeList.end()) {
        return;
    }

    CAchieveType* pAchieveType = it->second.get();
    if (!pAchieveType) return;

    // Find specific achievement
    std::shared_ptr<CAchieve> pAchieve = pAchieveType->FindAchieve(nTargetID);
    if (!pAchieve) {
        return;
    }

    // Update count
    ST_ACHIEVE_UPDATE stAchieveUpdate;
    if (!pAchieve->UpdateCount(nCount, &stAchieveUpdate, &m_stAchieveBit, &m_stCategory)) {
        return;
    }

    // Create update list and send
    ST_ACHIEVE_UPDATE_LIST stUpdateList;
    stUpdateList.vecList.push_back(stAchieveUpdate);

    // TODO: Send to DB, update titles, send to client
}

// UpdateCollect (immediate) (0x14002D590)
// IDA verified - Update collect achievement with immediate send
void CGocAchieve::UpdateCollect(std::uint16_t wType, int nCount, int nTargetID)
{
    // IDA shows similar flow to UpdateAchieve1 but uses UpdateCollectCount

    // Find achieve type
    auto it = m_mpAchieveTypeList.find(wType);
    if (it == m_mpAchieveTypeList.end()) {
        return;
    }

    CAchieveType* pAchieveType = it->second.get();
    if (!pAchieveType) return;

    // Find specific achievement
    std::shared_ptr<CAchieve> pAchieve = pAchieveType->FindAchieve(nTargetID);
    if (!pAchieve) {
        return;
    }

    // Update collect count
    ST_ACHIEVE_UPDATE stAchieveUpdate;
    if (!pAchieve->UpdateCollectCount(nCount, &stAchieveUpdate, &m_stAchieveBit, &m_stCategory)) {
        return;
    }

    if (stAchieveUpdate.stUpdateInfo.nIndex == 0) {
        return;
    }

    // Create update list
    ST_ACHIEVE_UPDATE_LIST stUpdateList;
    stUpdateList.vecList.push_back(stAchieveUpdate);

    // TODO: Send to DB if nNextIndex != 0, update titles, send to client
}

// UpdateCollect (batch) (0x14002DD30)
// IDA verified - Update collect achievement with batch processing
void CGocAchieve::UpdateCollect(std::uint16_t wType, int nCount, ST_ACHIEVE_UPDATE_LIST& stSendUser,
                                 ST_ACHIEVE_UPDATE_LIST& stSendDB, int nTargetID)
{
    // IDA shows:
    // 1. Find achieve type and achievement
    // 2. Call UpdateCollectCount
    // 3. Add to stSendUser
    // 4. If nNextIndex != 0, also add to stSendDB

    // Find achieve type
    auto it = m_mpAchieveTypeList.find(wType);
    if (it == m_mpAchieveTypeList.end()) {
        return;
    }

    CAchieveType* pAchieveType = it->second.get();
    if (!pAchieveType) return;

    // Find specific achievement
    std::shared_ptr<CAchieve> pAchieve = pAchieveType->FindAchieve(nTargetID);
    if (!pAchieve) {
        return;
    }

    // Update collect count
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
    std::uint8_t shLevel = GetOwnerLevel(this);

    // Update level achievement (type 0x20)
    UpdateAchieve1(0x20, shLevel, 0);

    // TODO: Process TB_ACHIEVEMENT_BEGIN table entries
    // IDA shows iteration over achievement begin table for level-based unlocks
}

// EndCollect (0x14002E000)
// IDA verified - End collection and send updates
void CGocAchieve::EndCollect()
{
    ST_ACHIEVE_UPDATE_LIST stUpdateList;

    // End collect for all achievement types
    for (auto& pair : m_mpAchieveTypeList) {
        CAchieveType* pAchieveType = pair.second.get();
        if (pAchieveType) {
            pAchieveType->EndCollect(&stUpdateList);
        }
    }

    // Send updates if any
    if (!stUpdateList.vecList.empty()) {
        // TODO: Send to client (main=3, sub=0x71)
    }
}

// GMAchieveCount (0x14002E510)
void CGocAchieve::GMAchieveCount(int nGroupID, int nCount)
{
    // TODO: 需人工审查 - Need to decompile GM achievement count
    (void)nGroupID;
    (void)nCount;
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
    // Packet contains: m_stAchieveBit, stAchieveList, m_stCategory

#ifdef _WIN32
    m_dw64LastUpdate = ::GetTickCount64();
#endif
}

// GetAchievePtr (0x14002EC60)
// IDA verified - Get achievement pointer by type and target ID
std::shared_ptr<CAchieve> CGocAchieve::GetAchievePtr(std::uint16_t wAchieveType, int nTagetID)
{
    // Find achieve type
    auto it = m_mpAchieveTypeList.find(wAchieveType);
    if (it == m_mpAchieveTypeList.end()) {
        return nullptr;
    }

    CAchieveType* pAchieveType = it->second.get();
    if (!pAchieveType) {
        return nullptr;
    }

    // Find and return achievement
    return pAchieveType->FindAchieve(nTagetID);
}

// UpdateItemUpgradeAchieve (0x14002ED30)
void CGocAchieve::UpdateItemUpgradeAchieve(std::uint8_t byResult)
{
    // TODO: 需人工审查 - Need to decompile item upgrade achievement
    (void)byResult;
}

// UpdateHarvestAchieve (0x14002EDA0)
void CGocAchieve::UpdateHarvestAchieve(std::uint8_t byHarvestType)
{
    // TODO: 需人工审查 - Need to decompile harvest achievement
    (void)byHarvestType;
}

// GetFirstAchieveID (0x14002EE30)
// IDA verified - Get first achievement ID in chain
std::uint32_t CGocAchieve::GetFirstAchieveID(TB_ACHIEVEMENT* pTBAchieve, std::uint16_t& wNowCategoryCount)
{
    if (!pTBAchieve) {
        return 0;
    }

    // Find achieve type
    auto it = m_mpAchieveTypeList.find(pTBAchieve->Achievement_type);
    if (it == m_mpAchieveTypeList.end()) {
        return 0;
    }

    CAchieveType* pAchieveType = it->second.get();
    if (!pAchieveType) {
        return 0;
    }

    // Find achievement
    std::shared_ptr<CAchieve> pAchieve = pAchieveType->FindAchieve(pTBAchieve->taget_ID);
    if (!pAchieve) {
        return 0;
    }

    // Check completion status
    if (pAchieve->GetAchieveCount() >= pTBAchieve->Achievement_count) {
        wNowCategoryCount = 1;
    } else {
        wNowCategoryCount = 0;
    }

    return pTBAchieve->ID;
}
