// CGocAchieve - Game Object Component for Achievement System
// Restored from GameServer.exe IDA decompilation
// Address range: 0x140029030 - 0x14002EEB6

#include "GocAchieve.h"
#include "Soulworker/GameServer/XGameServer/AchieveType.h"
#include "Soulworker/GameServer/XGameServer/Achieve.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
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
    CMover* pMover = GetOwnerMover();
    if (!pMover) return;

    // IDA: Create XSendDBPacket and send quest ID
    XSendDBPacket xSendDBPacket(pMover, 3, 0x61);
    int QuestID = pMover->GetQuestCondition()->GetQuestID();
    xSendDBPacket.XParse << QuestID;
    XGameServer::Instance()->SendDBGame(&xSendDBPacket);
}

// SendDBUpdateList (0x140029580)
// IDA verified - Send update list to DB and user
void CGocAchieve::SendDBUpdateList(ST_ACHIEVE_UPDATE_LIST& stSendUser, ST_ACHIEVE_UPDATE_LIST& stSendDB)
{
    // IDA: Get CUser via RTTI dynamic cast
    VChunkFile* v5 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    CUser* pUser = (CUser*)_RTDynamicCast_0(v5, 0, &CMover::RTTI_Type_Descriptor, &CUser::RTTI_Type_Descriptor, 0);
    
    if (!pUser) {
        // Log error if user not found
        LogHelper::LogError("game.contents", "SendDBUpdateList error - [ ActorID:%d ] ( %d )", 
                           pUser->GetActorID(), 88);
        stSendUser.vecList.clear();
        stSendDB.vecList.clear();
        return;
    }

    // IDA: Send to DB if there are entries
    if (!stSendDB.vecList.empty()) {
        CMover* pMover = GetOwnerMover();
        XSendDBPacket xSendDBPacket(pMover, 3, 0x62);
        int QuestID = pMover->GetQuestCondition()->GetQuestID();
        xSendDBPacket.XParse << QuestID;
        xSendDBPacket << stSendDB;
        xSendDBPacket << m_stAchieveBit;
        xSendDBPacket << m_stCategory;
        XGameServer::Instance()->SendDBGame(&xSendDBPacket);
        
        // IDA: Send achievement log
        XGameServer::Instance()->SendDBAchieveLog(
            pUser->GetUAID(), QuestID, 1, 
            pUser->GetLevel(), &stSendDB, 0);
        
        // IDA: Update open titles for each achievement
        for (const auto& stUpdate : stSendDB.vecList) {
            if (stUpdate.nNextIndex != 0) {
                auto pEntity = pMover->GetGOC<CGocEntity>();
                if (stUpdate.nCurIndex == stUpdate.nNextIndex) {
                    pEntity->UpdateOpenTitle(2, stUpdate.nNextIndex);
                } else {
                    for (int i = stUpdate.nCurIndex; i < stUpdate.nNextIndex; ++i) {
                        pEntity->UpdateOpenTitle(2, i);
                    }
                }
            }
        }
    }

    // IDA: Send to client if there are entries
    if (!stSendUser.vecList.empty()) {
        XSendPacket xSendPacket(3, 0x71);
        xSendPacket << stSendUser;
        xSendPacket << m_stAchieveBit;
        xSendPacket << m_stCategory;
        CGocNetwork::Send(pUser, &xSendPacket);
    }

    stSendUser.vecList.clear();
    stSendDB.vecList.clear();
}

// AchieveReward (0x140029B70)
// IDA decompiled - Process achievement reward claim
// Full implementation from IDA decompilation
bool CGocAchieve::AchieveReward(int nIndex)
{
    // IDA: Get TB_ACHIEVEMENT from XResourceMgr
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ACHIEVEMENT* pTBAchieve = XResourceMgr::GetTB_ACHIEVEMENT(&pServer->m_xResourceMgr, nIndex);
    if (!pTBAchieve) {
        LogHelper::LogError("game.contents", "AchieveReward error - [ ActorID:%d, Index:%d ] ( %d )", 
                           GetOwnerMover()->GetActorID(), nIndex, 140);
        return false;
    }

    // IDA: Get CAchieve pointer
    std::shared_ptr<CAchieve> pAchieve = GetAchievePtr(pTBAchieve->Achievement_type, pTBAchieve->taget_ID);
    if (!pAchieve) {
        return false;
    }

    // IDA: Check if achievement count meets requirement
    if (pAchieve->GetCount() < pTBAchieve->Achievement_count) {
        return false;
    }

    // IDA: Get inventory component
    CMover* pMover = GetOwnerMover();
    auto pInvenPtr = pMover->GetGOC<CGocInventory>();
    if (!pInvenPtr) {
        LogHelper::LogError("game.contents", "AchieveReward error pInvenPtr is NULL [ ActorID:%d ] ( %d )", 
                           pMover->GetActorID(), 184);
        return false;
    }

    // IDA: Check inventory capacity for all rewards
    if (!pInvenPtr->CheckOverMoney(E_PRICE_TYPE_GOLD, pTBAchieve->achievement_Reward_gold)) {
        CGocNetwork::SendErrorMessage(pMover, 3, 0x74, 0xCB58);
        return false;
    }
    if (!pInvenPtr->CheckOverMoney(E_PRICE_TYPE_BP, pTBAchieve->achievement_Reward_BP)) {
        CGocNetwork::SendErrorMessage(pMover, 3, 0x74, 0xCB59);
        return false;
    }
    if (!pInvenPtr->CheckOverMoney(E_PRICE_TYPE_ETHER, pTBAchieve->achievement_Reward_Ether)) {
        CGocNetwork::SendErrorMessage(pMover, 3, 0x74, 0xCB5A);
        return false;
    }

    // IDA: Check if reward can be claimed
    if (!CheckAchieveReward(pTBAchieve->Complete_Bit)) {
        LogHelper::LogError("game.contents", "AchieveReward error [ ActorID:%d, Bit:%d ] ( %d )", 
                           pMover->GetActorID(), pTBAchieve->Complete_Bit, 175);
        return false;
    }

    // IDA: Get entity component
    auto pEntity = pMover->GetGOC<CGocEntity>();
    if (!pEntity) {
        LogHelper::LogError("game.contents", "AchieveReward error pEntity is NULL [ ActorID:%d ] ( %d )", 
                           pMover->GetActorID(), 190);
        return false;
    }

    // IDA: Create reward items
    ST_CREATE_ITEMS stCreateItems;
    for (int i = 0; i < 2; ++i) {
        int nItemID = *(&pTBAchieve->achievement_Reward_item_1 + i);
        if (nItemID > 0) {
            if (*(&pTBAchieve->achievement_Reward_item_type_1 + i) == 1) {
                // Direct item
                ST_CREATE_ITEM stItem;
                stItem.nItemID = *(&pTBAchieve->achievement_Reward_item_1 + i);
                stItem.shCount = *(&pTBAchieve->achievement_Reward_item_count_1 + i);
                stCreateItems.vecItems.push_back(stItem);
            } else if (*(&pTBAchieve->achievement_Reward_item_type_1 + i) == 2) {
                // Quest reward
                TB_QUEST_REWARD* pTBReward = XResourceMgr::GetTB_QUEST_REWARD(&pServer->m_xResourceMgr, nItemID);
                if (pTBReward) {
                    unsigned __int8 byClass = pMover->GetClass() - 1;
                    if (byClass < 8) {
                        unsigned int dwIndex = *(&pTBReward->Item_PC_A + byClass);
                        TB_ITEM* pTB_ITEM = XResourceMgr::GetTB_ITEM(&pServer->m_xResourceMgr, dwIndex);
                        if (pTB_ITEM) {
                            ST_CREATE_ITEM stItem;
                            stItem.nItemID = pTB_ITEM->Item_ID;
                            stItem.shCount = *(&pTBAchieve->achievement_Reward_item_count_1 + i);
                            stCreateItems.vecItems.push_back(stItem);
                        }
                    }
                }
            }
        }
    }

    // IDA: Create items in inventory
    ST_LOG_GAME stLogItem;
    stLogItem.nParam3 = nIndex;
    if (!pInvenPtr->CreateItemReq(&stCreateItems, 1, E_ITEM_CREATE_TYPE_ACHIEVE, &stLogItem)) {
        LogHelper::LogError("game.contents", "AchieveReward error - No Empty Slot[ ActorID:%d ] ( %d )", 
                           pMover->GetActorID(), 234);
        return false;
    }

    // IDA: Add titles
    for (int j = 0; j < 2; ++j) {
        if (*(&pTBAchieve->achievement_Reward_title_1 + j)) {
            pEntity->AddTitleByClass(*(&pTBAchieve->achievement_Reward_title_1 + j), 1);
        }
    }

    // IDA: Set reward claimed
    SetAchieveReward(pTBAchieve->Complete_Bit);

    // IDA: Add gold
    if (pTBAchieve->achievement_Reward_gold) {
        pInvenPtr->AddMoney(pTBAchieve->achievement_Reward_gold, 0x15, nIndex, 0, 0);
    }

    // IDA: Add BP
    if (pTBAchieve->achievement_Reward_BP) {
        pInvenPtr->AddBP(pTBAchieve->achievement_Reward_BP, 0x15);
    }

    // IDA: Add Ether
    if (pTBAchieve->achievement_Reward_Ether) {
        pInvenPtr->AddEther(pTBAchieve->achievement_Reward_Ether, 0x15, 1);
    }

    // IDA: Send DB update
    ST_ACHIEVE_INFO stAchieveInfo;
    stAchieveInfo.nIndex = nIndex;
    XSendDBPacket xSendDBPacket(pMover, 3, 0x63);
    int QuestID = pMover->GetQuestCondition()->GetQuestID();
    xSendDBPacket.XParse << QuestID;
    xSendDBPacket << m_stAchieveBit;
    xSendDBPacket << stAchieveInfo;
    
    if (XGameServer::Instance()->SendDBGame(&xSendDBPacket)) {
        // IDA: Send achievement log
        CUser* pUser = dynamic_cast<CUser*>(pMover);
        if (pUser) {
            ST_LOG_GAME stLog;
            stLog._nUAID = pUser->GetUAID();
            stLog._nUCID = pUser->GetUCID();
            stLog._sMainType = 8;
            stLog._sSubType = 2;
            stLog.nParam0 = nIndex;
            stLog.nParam4 = pUser->GetLevel();
            wcscpy_s(stLog.szComment, L"Achievement");
            XGameServer::Instance()->SendDBLog(&stLog);
        }
        return true;
    }

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
// IDA decompiled - GM command to clear all achievements
void CGocAchieve::GMClearAchieve()
{
    ClearAchieve();

    CMover* pMover = GetOwnerMover();
    if (!pMover) return;

    // IDA: Send DB packet (main=3, sub=0x66) to notify database
    XSendDBPacket xSendDBPacket(pMover, 3, 0x66);
    int QuestID = pMover->GetQuestCondition()->GetQuestID();
    xSendDBPacket.XParse << QuestID;
    XGameServer::Instance()->SendDBGame(&xSendDBPacket);
}

// GMAchieveComplete (0x14002B4B0)
// IDA decompiled - GM command to complete achievement
void CGocAchieve::GMAchieveComplete(int nGroupID, int nStep)
{
    if (nStep < 0) return;
    
    CMover* pMover = GetOwnerMover();
    if (!pMover) return;
    
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    
    if (nStep) {
        // Complete specific achievement step
        int nAchieveIndex = nStep + 100 * nGroupID;
        TB_ACHIEVEMENT* pTBAchieve = XResourceMgr::GetTB_ACHIEVEMENT(&pServer->m_xResourceMgr, nAchieveIndex);
        if (!pTBAchieve) return;
        
        int nIndex = pTBAchieve->Complete_Bit / 8;
        int nPos = pTBAchieve->Complete_Bit & 7;
        if (nIndex > 128) return;
        
        auto it = m_mpAchieveTypeList.find(pTBAchieve->Achievement_type);
        if (it != m_mpAchieveTypeList.end()) {
            std::shared_ptr<CAchieve> pAchieve = it->second->FindAchieve(pTBAchieve->taget_ID);
            if (pAchieve) {
                if (pTBAchieve->Achievement_type == 32 || pTBAchieve->Achievement_type == 27) {
                    UpdateAchieve1(pTBAchieve->Achievement_type, pTBAchieve->Achievement_count, pTBAchieve->taget_ID);
                } else {
                    int nCurCount = pAchieve->GetCount();
                    UpdateAchieve1(pTBAchieve->Achievement_type, pTBAchieve->Achievement_count - nCurCount, pTBAchieve->taget_ID);
                }
            }
        }
    } else {
        // Complete all achievements in group
        ST_ACHIEVE_UPDATE_LIST stUpdateList;
        
        int nStartIndex = 100 * nGroupID + 1;
        TB_ACHIEVEMENT* pTBAchieve = XResourceMgr::GetTB_ACHIEVEMENT(&pServer->m_xResourceMgr, nStartIndex);
        if (!pTBAchieve) return;
        
        // Find or create achieve type
        auto it = m_mpAchieveTypeList.find(pTBAchieve->Achievement_type);
        if (it == m_mpAchieveTypeList.end()) {
            auto pAchieveType = std::make_shared<CAchieveType>();
            pAchieveType->AddAchieve(pTBAchieve, nullptr);
            m_mpAchieveTypeList[pTBAchieve->Achievement_type] = pAchieveType;
            
            ST_ACHIEVE_UPDATE stUpdate;
            stUpdate.stUpdateInfo.nIndex = pTBAchieve->ID;
            stUpdate.stUpdateInfo.biCount = 0;
            stUpdateList.vecList.push_back(stUpdate);
        } else {
            std::shared_ptr<CAchieve> pAchieve = it->second->FindAchieve(pTBAchieve->taget_ID);
            if (!pAchieve) {
                ST_ACHIEVE_UPDATE stUpdate;
                stUpdate.stUpdateInfo.nIndex = pTBAchieve->ID;
                stUpdate.stUpdateInfo.biCount = 0;
                stUpdateList.vecList.push_back(stUpdate);
                it->second->AddAchieve(pTBAchieve, nullptr);
            } else {
                pAchieve->GMAllClear(&stUpdateList);
            }
        }
        
        // Clear reward bits for 10 steps
        for (int i = 0; i < 10; ++i) {
            int nAchieveIndex = 100 * nGroupID + i + 1;
            TB_ACHIEVEMENT* pTBTemp = XResourceMgr::GetTB_ACHIEVEMENT(&pServer->m_xResourceMgr, nAchieveIndex);
            if (!pTBTemp) break;
            
            int nIndex = pTBTemp->Complete_Bit / 8;
            int nPos = pTBTemp->Complete_Bit & 7;
            if (nIndex >= 128) break;
            
            if ((m_stAchieveBit.szRewardBit[nIndex] & (1 << nPos)) != 0) {
                m_stAchieveBit.szRewardBit[nIndex] &= ~(1 << nPos);
            }
        }
        
        // Send DB update
        XSendDBPacket xSendDBPacket(pMover, 3, 0x62);
        int QuestID = pMover->GetQuestCondition()->GetQuestID();
        xSendDBPacket.XParse << QuestID;
        xSendDBPacket << stUpdateList;
        xSendDBPacket << m_stAchieveBit;
        xSendDBPacket << m_stCategory;
        pServer->SendDBGame(&xSendDBPacket);
        
        // Send client update
        XSendPacket xSendPacket(3, 0x71);
        xSendPacket << stUpdateList;
        xSendPacket << m_stAchieveBit;
        xSendPacket << m_stCategory;
        CGocNetwork::Send(pMover, &xSendPacket);
    }
}

// LoadAchieve (0x14002BDD0)
// IDA decompiled - Load achievement data from DB response
void CGocAchieve::LoadAchieve(ST_ACHIEVE_BIT& stAchieveBit, ST_ACHIEVE_LIST& stAchieveList,
                               ST_ACHIEVE_CATEGORY& stCatagory, bool bFirst)
{
    // Copy bit and category data
    std::memcpy(&m_stAchieveBit, &stAchieveBit, sizeof(m_stAchieveBit));
    std::memcpy(&m_stCategory, &stCatagory, sizeof(m_stCategory));
    
    CMover* pMover = GetOwnerMover();
    if (!pMover) return;
    
    std::uint8_t shLevel = pMover->GetLevel();
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    
    // Initialize achievements from TB_ACHIEVEMENT_BEGIN table
    auto& mapBegin = pServer->m_xResourceMgr.m_mapTB_ACHIEVEMENT_BEGIN;
    for (auto it = mapBegin.begin(); it != mapBegin.end(); ++it) {
        TB_ACHIEVEMENT_BEGIN& tbBegin = it->second;
        if (tbBegin.Achievement_Open_Lv <= shLevel) {
            TB_ACHIEVEMENT* pTBAchieve = XResourceMgr::GetTB_ACHIEVEMENT(&pServer->m_xResourceMgr, tbBegin.Achievement_ID);
            if (pTBAchieve) {
                InitAchieve(pTBAchieve);
            }
        }
    }
    
    ST_ACHIEVE_UPDATE_LIST stUpdateList;
    bool bSendDB = false;
    std::map<unsigned char, ST_ACHIEVE_UPDATE> mapCategory;
    
    // Process each achievement in the list
    for (size_t i = 0; i < stAchieveList.vecList.size(); ++i) {
        ST_ACHIEVE_INFO& stInfo = stAchieveList.vecList[i];
        TB_ACHIEVEMENT* pTBAchieve = XResourceMgr::GetTB_ACHIEVEMENT(&pServer->m_xResourceMgr, stInfo.nIndex);
        if (!pTBAchieve) continue;
        
        bool bChange = false;
        ST_ACHIEVE_UPDATE stUpdate;
        stUpdate.stUpdateInfo.nIndex = pTBAchieve->ID;
        stUpdate.stUpdateInfo.biCount = stInfo.biCount;
        stUpdate.nCurIndex = pTBAchieve->ID;
        stUpdate.byCategory = pTBAchieve->Achievement_Category;
        stUpdate.wCount = m_stCategory.wCount[pTBAchieve->Achievement_Category];
        
        // Handle level-type achievements
        if (pTBAchieve->Achievement_type == 32) {
            if (stInfo.biCount != shLevel) {
                stUpdate.stUpdateInfo.biCount = shLevel;
                stInfo.biCount = shLevel;
                bChange = true;
            }
        }
        
        // Check for achievement progression
        if (pTBAchieve->Achievement_count <= stInfo.biCount) {
            unsigned int dwNextIndex = pTBAchieve->ID + 1;
            TB_ACHIEVEMENT* pNextTBAchieve = XResourceMgr::GetTB_ACHIEVEMENT(&pServer->m_xResourceMgr, dwNextIndex);
            
            while (pNextTBAchieve) {
                pTBAchieve = pNextTBAchieve;
                if (pTBAchieve->Achievement_Category >= 7) {
                    break;
                }
                
                stInfo.nIndex = pTBAchieve->ID;
                stUpdate.nNextIndex = pTBAchieve->ID;
                stUpdate.byCategory = pTBAchieve->Achievement_Category;
                stUpdate.wCount = m_stCategory.wCount[pTBAchieve->Achievement_Category];
                bChange = true;
                
                if (pTBAchieve->Achievement_type == 32) {
                    stUpdate.stUpdateInfo.biCount = shLevel;
                    stInfo.biCount = shLevel;
                }
                
                if (pTBAchieve->Achievement_count > stInfo.biCount) {
                    break;
                }
                
                m_stCategory.wCount[pTBAchieve->Achievement_Category]++;
                stUpdate.wCount = m_stCategory.wCount[pTBAchieve->Achievement_Category];
                dwNextIndex = pTBAchieve->ID + 1;
                pNextTBAchieve = XResourceMgr::GetTB_ACHIEVEMENT(&pServer->m_xResourceMgr, dwNextIndex);
            }
        }
        
        // Initialize achievement
        InitAchieve(stInfo);
        
        // Clear reward bit if achievement incomplete
        if (pTBAchieve->Achievement_count > stInfo.biCount) {
            int nIndex = pTBAchieve->Complete_Bit / 8;
            int nPos = pTBAchieve->Complete_Bit & 7;
            if (nIndex < 128 && (m_stAchieveBit.szRewardBit[nIndex] & (1 << nPos)) != 0) {
                m_stAchieveBit.szRewardBit[nIndex] &= ~(1 << nPos);
                bSendDB = true;
            }
        }
        
        if (bChange) {
            bSendDB = true;
            stUpdateList.vecList.push_back(stUpdate);
        }
        
        // Handle category counts on first load
        if (bFirst) {
            bool bComplete = pTBAchieve->Achievement_count <= stInfo.biCount;
            std::uint16_t wNowCategoryCount = bComplete ? 1 : 0;
            
            if (GetFirstAchieveID(pTBAchieve, wNowCategoryCount)) {
                auto itCat = mapCategory.find(pTBAchieve->Achievement_Category);
                if (itCat != mapCategory.end()) {
                    itCat->second.wCount += wNowCategoryCount;
                } else {
                    stUpdate.nNextIndex = pTBAchieve->ID;
                    stUpdate.wCount = wNowCategoryCount;
                    mapCategory[pTBAchieve->Achievement_Category] = stUpdate;
                }
            }
        }
    }
    
    // Update category counts on first load
    if (bFirst) {
        for (int i = 0; i < 7; ++i) {
            auto itCat = mapCategory.find(i);
            if (itCat != mapCategory.end()) {
                if (m_stCategory.wCount[i] != itCat->second.wCount) {
                    bSendDB = true;
                    m_stCategory.wCount[i] = itCat->second.wCount;
                    stUpdateList.vecList.push_back(itCat->second);
                }
            }
        }
    }
    
    // Send DB update if needed
    if (bSendDB) {
        XSendDBPacket xSendDBPacket(pMover, 3, 0x62);
        int QuestID = pMover->GetQuestCondition()->GetQuestID();
        xSendDBPacket.XParse << QuestID;
        xSendDBPacket << stUpdateList;
        xSendDBPacket << m_stAchieveBit;
        xSendDBPacket << m_stCategory;
        pServer->SendDBGame(&xSendDBPacket);
    }
    
    // Set achievement flag in user data
    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (pUser) {
        pUser->GetCharInfo()->UserDB.byAchieveFlag |= 1;
    }
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
// IDA decompiled - Initialize achievement from info struct
void CGocAchieve::InitAchieve(ST_ACHIEVE_INFO& stAchieveInfo)
{
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ACHIEVEMENT* pTBAchieve = XResourceMgr::GetTB_ACHIEVEMENT(&pServer->m_xResourceMgr, stAchieveInfo.nIndex);
    if (!pTBAchieve) {
        LogHelper::LogError("game.contents", "InitAchieve error - No Table TB_ACHIVEMENT[ ActorID:%d, Index:%d ] ( %d )",
                           GetOwnerMover()->GetActorID(), stAchieveInfo.nIndex, 790);
        return;
    }
    
    auto it = m_mpAchieveTypeList.find(pTBAchieve->Achievement_type);
    if (it == m_mpAchieveTypeList.end()) {
        LogHelper::LogError("game.contents", "InitAchieve error - No Exist First Table[ ActorID:%d, Index:%d ] ( %d )",
                           GetOwnerMover()->GetActorID(), stAchieveInfo.nIndex, 800);
        return;
    }
    
    std::shared_ptr<CAchieve> pAchieve = it->second->FindAchieve(pTBAchieve->taget_ID);
    if (!pAchieve) {
        LogHelper::LogError("game.contents", "InitAchieve error - No Exist First Table[ ActorID:%d, Index:%d ] ( %d )",
                           GetOwnerMover()->GetActorID(), stAchieveInfo.nIndex, 800);
        return;
    }
    
    pAchieve->SetAchieve(pTBAchieve, stAchieveInfo.biCount);
}

// UpdateAchieve1 (0x14002CEB0)
// IDA decompiled - Update achievement count with immediate DB sync
void CGocAchieve::UpdateAchieve1(std::uint16_t wType, int nCount, int nTargetID)
{
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerMover());
    if (!pUser) {
        LogHelper::LogError("game.contents", "UpdateAchieve1 error - [ ActorID:%d ] ( %d )",
                           GetOwnerMover()->GetActorID(), 813);
        return;
    }
    
    auto it = m_mpAchieveTypeList.find(wType);
    if (it == m_mpAchieveTypeList.end()) return;
    
    std::shared_ptr<CAchieve> pAchieve = it->second->FindAchieve(nTargetID);
    if (!pAchieve) return;
    
    ST_ACHIEVE_UPDATE stAchieveUpdate;
    if (!pAchieve->UpdateCount(nCount, &stAchieveUpdate, &m_stAchieveBit, &m_stCategory)) {
        return;
    }
    
    ST_ACHIEVE_UPDATE_LIST stUpdateList;
    stUpdateList.vecList.push_back(stAchieveUpdate);
    
    // Send to DB
    CMover* pMover = GetOwnerMover();
    XSendDBPacket xSendDBPacket(pMover, 3, 0x62);
    int QuestID = pMover->GetQuestCondition()->GetQuestID();
    xSendDBPacket.XParse << QuestID;
    xSendDBPacket << stUpdateList;
    xSendDBPacket << m_stAchieveBit;
    xSendDBPacket << m_stCategory;
    XGameServer::Instance()->SendDBGame(&xSendDBPacket);
    
    // Send achievement log
    std::uint8_t shLevel = pMover->GetLevel();
    XGameServer::Instance()->SendDBAchieveLog(pUser->GetUAID(), QuestID, 3, shLevel, &stUpdateList, 0);
    
    // Update open titles
    for (const auto& stUpdate : stUpdateList.vecList) {
        if (stUpdate.nNextIndex) {
            auto pEntity = pMover->GetGOC<CGocEntity>();
            if (stUpdate.nCurIndex == stUpdate.nNextIndex) {
                pEntity->UpdateOpenTitle(2, stUpdate.nNextIndex);
            } else {
                for (int i = stUpdate.nCurIndex; i < stUpdate.nNextIndex; ++i) {
                    pEntity->UpdateOpenTitle(2, i);
                }
            }
        }
    }
    
    // Send to client
    XSendPacket xSendPacket(3, 0x71);
    xSendPacket << stUpdateList;
    xSendPacket << m_stAchieveBit;
    xSendPacket << m_stCategory;
    CGocNetwork::Send(pMover, &xSendPacket);
}

// UpdateCollect (immediate) (0x14002D590)
// IDA decompiled - Update collect achievement with immediate send
void CGocAchieve::UpdateCollect(std::uint16_t wType, int nCount, int nTargetID)
{
    auto it = m_mpAchieveTypeList.find(wType);
    if (it == m_mpAchieveTypeList.end()) return;
    
    std::shared_ptr<CAchieve> pAchieve = it->second->FindAchieve(nTargetID);
    if (!pAchieve) return;
    
    ST_ACHIEVE_UPDATE stAchieveUpdate;
    if (!pAchieve->UpdateCollectCount(nCount, &stAchieveUpdate, &m_stAchieveBit, &m_stCategory)) {
        return;
    }
    
    if (stAchieveUpdate.stUpdateInfo.nIndex == 0) return;
    
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerMover());
    if (!pUser) {
        LogHelper::LogError("game.contents", "UpdateCollect error - [ ActorID:%d ] ( %d )",
                           GetOwnerMover()->GetActorID(), 889);
        return;
    }
    
    ST_ACHIEVE_UPDATE_LIST stUpdateList;
    stUpdateList.vecList.push_back(stAchieveUpdate);
    
    // Send to DB if achievement progressed
    if (stAchieveUpdate.nNextIndex) {
        CMover* pMover = GetOwnerMover();
        XSendDBPacket xSendDBPacket(pMover, 3, 0x62);
        int QuestID = pMover->GetQuestCondition()->GetQuestID();
        xSendDBPacket.XParse << QuestID;
        xSendDBPacket << stUpdateList;
        xSendDBPacket << m_stAchieveBit;
        xSendDBPacket << m_stCategory;
        XGameServer::Instance()->SendDBGame(&xSendDBPacket);
        
        // Send achievement log
        std::uint8_t shLevel = pMover->GetLevel();
        XGameServer::Instance()->SendDBAchieveLog(pUser->GetUAID(), QuestID, 1, shLevel, &stUpdateList, 0);
        
        // Update open titles
        for (const auto& stUpdate : stUpdateList.vecList) {
            if (stUpdate.nNextIndex) {
                auto pEntity = pMover->GetGOC<CGocEntity>();
                if (stUpdate.nCurIndex == stUpdate.nNextIndex) {
                    pEntity->UpdateOpenTitle(2, stUpdate.nNextIndex);
                } else {
                    for (int i = stUpdate.nCurIndex; i < stUpdate.nNextIndex; ++i) {
                        pEntity->UpdateOpenTitle(2, i);
                    }
                }
            }
        }
    }
    
    // Send to client
    XSendPacket xSendPacket(3, 0x71);
    xSendPacket << stUpdateList;
    xSendPacket << m_stAchieveBit;
    xSendPacket << m_stCategory;
    CGocNetwork::Send(GetOwnerMover(), &xSendPacket);
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
// IDA decompiled - Handle level up achievements
void CGocAchieve::LevelUp()
{
    CMover* pMover = GetOwnerMover();
    if (!pMover) return;
    
    std::uint8_t shLevel = pMover->GetLevel();
    UpdateAchieve1(0x20, shLevel, 0);
    
    // Initialize achievements for new level
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    auto& mapBegin = pServer->m_xResourceMgr.m_mapTB_ACHIEVEMENT_BEGIN;
    
    for (auto it = mapBegin.begin(); it != mapBegin.end(); ++it) {
        TB_ACHIEVEMENT_BEGIN& tbBegin = it->second;
        if (tbBegin.Achievement_Open_Lv <= shLevel) {
            TB_ACHIEVEMENT* pTBAchieve = XResourceMgr::GetTB_ACHIEVEMENT(&pServer->m_xResourceMgr, tbBegin.Achievement_ID);
            if (pTBAchieve) {
                InitAchieve(pTBAchieve);
            }
        }
    }
}

// EndCollect (0x14002E000)
// IDA decompiled - End collection and send updates
void CGocAchieve::EndCollect()
{
    ST_ACHIEVE_UPDATE_LIST stUpdateList;
    
    for (auto& pair : m_mpAchieveTypeList) {
        CAchieveType* pAchieveType = pair.second.get();
        if (pAchieveType) {
            pAchieveType->EndCollect(&stUpdateList);
        }
    }
    
    if (stUpdateList.vecList.empty()) return;
    
    CMover* pMover = GetOwnerMover();
    if (!pMover) return;
    
    // Send to DB
    XSendDBPacket xSendDBPacket(pMover, 3, 0x65);
    int QuestID = pMover->GetQuestCondition()->GetQuestID();
    xSendDBPacket.XParse << QuestID;
    xSendDBPacket << stUpdateList;
    xSendDBPacket << m_stAchieveBit;
    xSendDBPacket << m_stCategory;
    XGameServer::Instance()->SendDBGame(&xSendDBPacket);
    
    // Update open titles
    CUser* pUser = dynamic_cast<CUser*>(pMover);
    for (const auto& stUpdate : stUpdateList.vecList) {
        if (stUpdate.nNextIndex && pUser) {
            auto pEntity = pMover->GetGOC<CGocEntity>();
            if (stUpdate.nCurIndex == stUpdate.nNextIndex) {
                pEntity->UpdateOpenTitle(2, stUpdate.nNextIndex);
            } else {
                for (int i = stUpdate.nCurIndex; i < stUpdate.nNextIndex; ++i) {
                    pEntity->UpdateOpenTitle(2, i);
                }
            }
        }
    }
}

// GMAchieveCount (0x14002E510)
// IDA decompiled - GM command to set achievement count
void CGocAchieve::GMAchieveCount(int nGroupID, int nCount)
{
    int nAchieveIndex = 100 * nGroupID + 1;
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ACHIEVEMENT* pTBAchieve = XResourceMgr::GetTB_ACHIEVEMENT(&pServer->m_xResourceMgr, nAchieveIndex);
    if (!pTBAchieve) return;
    
    // Find or create achieve type
    auto it = m_mpAchieveTypeList.find(pTBAchieve->Achievement_type);
    if (it == m_mpAchieveTypeList.end()) {
        auto pAchieveType = std::make_shared<CAchieveType>();
        pAchieveType->AddAchieve(pTBAchieve, nullptr);
        m_mpAchieveTypeList[pTBAchieve->Achievement_type] = pAchieveType;
    }
    
    // Call UpdateAchieve1 with adjusted count
    UpdateAchieve1(pTBAchieve->Achievement_type, nCount, pTBAchieve->taget_ID);
}

// SendAchieveList (0x14002EA80)
// IDA decompiled - Send achievement list to client
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
    
    // Send packet to client
    CMover* pMover = GetOwnerMover();
    if (!pMover) return;
    
    XSendPacket xSendPacket(3, 0x70);
    xSendPacket << m_stAchieveBit;
    xSendPacket << stAchieveList;
    xSendPacket << m_stCategory;
    CGocNetwork::Send(pMover, &xSendPacket);
    
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
// IDA decompiled - Get first achievement ID in chain by walking backwards
std::uint32_t CGocAchieve::GetFirstAchieveID(TB_ACHIEVEMENT* pTBAchieve, std::uint16_t& wNowCategoryCount)
{
    std::uint32_t dwAchieveID = 0;
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    
    // Walk backwards through achievement chain by looking up ID-1
    while (pTBAchieve) {
        dwAchieveID = pTBAchieve->ID;
        std::uint32_t dwIndex = pTBAchieve->ID - 1;
        
        pTBAchieve = XResourceMgr::GetTB_ACHIEVEMENT(&pServer->m_xResourceMgr, dwIndex);
        
        if (pTBAchieve) {
            ++wNowCategoryCount;
        }
    }
    
    return dwAchieveID;
}
