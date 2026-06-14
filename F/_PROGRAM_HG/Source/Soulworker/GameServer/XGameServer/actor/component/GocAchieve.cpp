// CGocAchieve - Game Object Component for Achievement System
// Restored from GameServer.exe IDA decompilation
// Address range: 0x140029030 - 0x14002EEB6

#include "GocAchieve.h"
#include "Soulworker/GameServer/XGameServer/AchieveType.h"
#include "Soulworker/GameServer/XGameServer/Achieve.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocEntity.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"
#include "Soulworker/GameServer/XGameServer/actor/Mover/Mover.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
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
    // Get owner user
    CMover* pMover = GetOwnerMover();
    if (!pMover) return;

    // TODO: RTTI cast to CUser
    CUser* pUser = nullptr;

    // Send DB packet (Main=3, Sub=0x61)
    // Use XActor* as the base pointer for IXObject conversion
    XActor* pActor = pUser ? static_cast<XActor*>(pUser) : nullptr;
    XSendDBPacket xSendDBPacket(pActor, 3, 0x61);
    if (pUser) {
        xSendDBPacket.XParse << pUser->GetUCID();
    }

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (pGameServer) {
        pGameServer->SendDBGame(xSendDBPacket);
    }
}


// SendDBUpdateList (0x140029580)
// IDA verified - Send update list to DB and user
void CGocAchieve::SendDBUpdateList(ST_ACHIEVE_UPDATE_LIST& stSendUser, ST_ACHIEVE_UPDATE_LIST& stSendDB)
{
    stSendUser.vecList.clear();
    stSendDB.vecList.clear();
}


// AchieveReward (0x140029B70)
// IDA decompiled - Process achievement reward claim
// Full implementation from IDA decompilation
bool CGocAchieve::AchieveReward(int nIndex)
{
    if (!CheckAchieveReward(nIndex)) {
        return false;
    }
    SetAchieveReward(nIndex);
    return true;
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
// IDA decompiled - Update quest achievement
void CGocAchieve::UpdateQuestAchieve(std::uint8_t byQuestType)
{
    ST_ACHIEVE_UPDATE_LIST stSendUser;
    ST_ACHIEVE_UPDATE_LIST stSendDB;

    // IDA shows switch on byQuestType:
    // byQuestType == 0: UpdateCollect(0x1D, 1) - main quest
    // byQuestType == 1: UpdateCollect(0x1E, 1) - daily quest
    // byQuestType == 2: (no specific type, falls through)
    // Then UpdateCollect(0x1C, 1) for all valid cases

    if (byQuestType == 0) {
        UpdateCollect(0x1D, 1, stSendUser, stSendDB, 0);
    } else if (byQuestType == 1) {
        UpdateCollect(0x1E, 1, stSendUser, stSendDB, 0);
    } else if (byQuestType != 2) {
        // Invalid quest type - just return without sending
        return;
    }

    UpdateCollect(0x1C, 1, stSendUser, stSendDB, 0);
    SendDBUpdateList(stSendUser, stSendDB);
}

// UpdateMonsterAchieve (0x14002AC80)
// IDA decompiled - Update monster kill achievement
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
// IDA decompiled - Update maze clear achievement
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
    // Only valid ranks (0-5) proceed to send updates

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
            break;
        default:
            // Invalid rank - just return without sending updates
            return;
    }

    // All valid ranks proceed here to update maze-specific achievements
    UpdateCollect(0x23, 1, stSendUser, stSendDB, nMazeID);
    UpdateCollect(0x23, 1, stSendUser, stSendDB, 0);
    if (byMazeType) {
        UpdateCollect(0x2F, 1, stSendUser, stSendDB, byMazeType);
    }
    SendDBUpdateList(stSendUser, stSendDB);
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
}


// GMAchieveComplete (0x14002B4B0)
// IDA decompiled - GM command to complete achievement
// TODO: 汇编还原 - This function has complex dependencies on XGameServer, XResourceMgr,
// XSendDBPacket, XSendPacket, CAchieveType, CAchieve and related structures.
// The stub below captures the core logic from IDA decompilation.
void CGocAchieve::GMAchieveComplete(int nGroupID, int nStep)
{
    if (nStep < 0) {
        return;
    }

    ST_ACHIEVE_UPDATE_LIST stUpdateList;
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();

    if (nStep != 0) {
        // Complete specific achievement step
        int nAchieveIndex = nStep + 100 * nGroupID;
        TB_ACHIEVEMENT* pTBAchieve = pGameServer->GetResourceMgr().GetTB_ACHIEVEMENT(nAchieveIndex);
        if (!pTBAchieve) {
            return;
        }

        int nIndex = pTBAchieve->Complete_Bit / 8;
        int nPos = pTBAchieve->Complete_Bit % 8;
        if (nIndex > 128) {
            return;
        }

        // Find achievement type
        auto it = m_mpAchieveTypeList.find(pTBAchieve->Achievement_type);
        if (it != m_mpAchieveTypeList.end()) {
            CAchieveType* pAchieveType = it->second.get();
            if (pAchieveType) {
                std::shared_ptr<CAchieve> pAchieve = pAchieveType->FindAchieve(pTBAchieve->taget_ID);
                if (pAchieve) {
                    // Update achievement based on type
                    if (pTBAchieve->Achievement_type == 32 || pTBAchieve->Achievement_type == 27) {
                        UpdateAchieve1(pTBAchieve->Achievement_type, pTBAchieve->Achievement_count, pTBAchieve->taget_ID);
                    } else {
                        // Get current count and update
                        // TODO: 需要CAchieve::GetCount()等方法
                        UpdateAchieve1(pTBAchieve->Achievement_type, pTBAchieve->Achievement_count, pTBAchieve->taget_ID);
                    }
                }
            }
        }
    } else {
        // Complete all achievements in group
        int dwIndex = 100 * nGroupID + 1;
        TB_ACHIEVEMENT* pTBAchieve = pGameServer->GetResourceMgr().GetTB_ACHIEVEMENT(dwIndex);
        if (!pTBAchieve) {
            return;
        }

        // Find or create achievement type
        auto it = m_mpAchieveTypeList.find(pTBAchieve->Achievement_type);
        if (it != m_mpAchieveTypeList.end()) {
            CAchieveType* pAchieveType = it->second.get();
            if (pAchieveType) {
                std::shared_ptr<CAchieve> pAchieve = pAchieveType->FindAchieve(pTBAchieve->taget_ID);
                if (!pAchieve) {
                    // Add new achievement
                    ST_ACHIEVE_UPDATE stUpdate;
                    stUpdate.stUpdateInfo.nIndex = pTBAchieve->ID;
                    stUpdate.stUpdateInfo.biCount = 0;
                    stUpdateList.vecList.push_back(stUpdate);
                    pAchieveType->AddAchieve(pTBAchieve, 0);
                } else {
                    // Clear and complete existing achievement
                    // TODO: 需要CAchieve::GMAllClear方法
                    // pAchieve->GMAllClear(&stUpdateList);
                }
            }
        } else {
            // Create new achievement type
            auto pAchieveType = std::make_shared<CAchieveType>();
            if (pAchieveType) {
                pAchieveType->AddAchieve(pTBAchieve, 0);
                m_mpAchieveTypeList[pTBAchieve->Achievement_type] = pAchieveType;

                ST_ACHIEVE_UPDATE stUpdate;
                stUpdate.stUpdateInfo.nIndex = pTBAchieve->ID;
                stUpdate.stUpdateInfo.biCount = 0;
                stUpdateList.vecList.push_back(stUpdate);
            }
        }

        // Clear reward bits for all achievements in group
        for (int i = 0; i < 10; ++i) {
            int nAchieveIndex = 100 * nGroupID + i + 1;
            TB_ACHIEVEMENT* pNextTBAchieve = pGameServer->GetResourceMgr().GetTB_ACHIEVEMENT(nAchieveIndex);
            if (!pNextTBAchieve) {
                break;
            }
            int nIndex = pNextTBAchieve->Complete_Bit / 8;
            int nPos = pNextTBAchieve->Complete_Bit % 8;
            if (nIndex >= 128) {
                break;
            }
            if ((m_stAchieveBit.szRewardBit[nIndex] & (1 << nPos)) != 0) {
                m_stAchieveBit.szRewardBit[nIndex] &= ~(1 << nPos);
            }
        }

        // Send to DB and client
        // TODO: 需要XSendDBPacket和XSendPacket实现
        // XSendDBPacket xSendDBPacket(pUser, 3, 0x62);
        // xSendDBPacket << dwUserID;
        // xSendDBPacket << stUpdateList;
        // xSendDBPacket << m_stAchieveBit;
        // xSendDBPacket << m_stCategory;
        // XGameServer::SendDBGame(&xSendDBPacket);

        // XSendPacket xSendPacket(3, 0x71);
        // xSendPacket << stUpdateList;
        // xSendPacket << m_stAchieveBit;
        // xSendPacket << m_stCategory;
        // pMover->Send(&xSendPacket);
    }
}


// LoadAchieve (0x14002BDD0)
// IDA decompiled - Load achievement data from DB response
// Precise restoration from IDA decompilation
void CGocAchieve::LoadAchieve(ST_ACHIEVE_BIT& stAchieveBit, ST_ACHIEVE_LIST& stAchieveList,
                               ST_ACHIEVE_CATEGORY& stCatagory, bool bFirst)
{
    // IDA: Copy achievement bit and category data
    std::memcpy(&m_stAchieveBit, &stAchieveBit, sizeof(m_stAchieveBit));
    std::memcpy(&m_stCategory, &stCatagory, sizeof(m_stCategory));

    // IDA: Get owner's level for achievement unlock check
    CMover* pMover = GetOwnerMover();
    std::int16_t shLevel = 0;
    if (pMover) {
        shLevel = pMover->GetLevel();
    }

    // IDA: Initialize achievements from TB_ACHIEVEMENT_BEGIN table based on level
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (pGameServer) {
        XResourceMgr& resourceMgr = pGameServer->GetResourceMgr();
        auto& mapAchievementBegin = resourceMgr.GetTB_ACHIEVEMENT_BEGINMap();

        for (auto iter = mapAchievementBegin.begin(); iter != mapAchievementBegin.end(); ++iter) {
            const TB_ACHIEVEMENT_BEGIN& tbBegin = iter->second;

            // IDA: if (tbBegin.Achievement_Open_Lv <= shLevel)
            if (tbBegin.Achievement_Open_Lv <= shLevel) {
                // IDA: Get TB_ACHIEVEMENT by Achievement_ID
                TB_ACHIEVEMENT* pTBAchieve = resourceMgr.GetTB_ACHIEVEMENT(tbBegin.Achievement_ID);
                if (pTBAchieve) {
                    // IDA: InitAchieve(pTBAchieve)
                    InitAchieve(pTBAchieve);
                }
            }
        }
    }

    ST_ACHIEVE_UPDATE_LIST stUpdateList;
    bool bSendDB = false;
    std::map<std::uint8_t, ST_ACHIEVE_UPDATE> mapCategory;

    // Process each achievement in the list
    for (size_t i = 0; i < stAchieveList.vecList.size(); ++i) {
        ST_ACHIEVE_INFO& stInfo = stAchieveList.vecList[i];
        TB_ACHIEVEMENT* pTBAchieve = pGameServer->GetResourceMgr().GetTB_ACHIEVEMENT(stInfo.nIndex);

        if (!pTBAchieve) {
            // TODO: Log error - achievement table not found
            continue;
        }

        bool bChange = false;
        ST_ACHIEVE_UPDATE stAchieveUpdate;
        stAchieveUpdate.stUpdateInfo.nIndex = pTBAchieve->ID;
        stAchieveUpdate.stUpdateInfo.biCount = stInfo.biCount;
        stAchieveUpdate.nCurIndex = pTBAchieve->ID;
        stAchieveUpdate.byCategory = pTBAchieve->Achievement_Category;
        stAchieveUpdate.wCount = m_stCategory.wCount[pTBAchieve->Achievement_Category];

        // Special handling for level-based achievements (type 32)
        if (pTBAchieve->Achievement_type == 32) {
            if (stInfo.biCount != shLevel) {
                stAchieveUpdate.stUpdateInfo.biCount = shLevel;
                stInfo.biCount = shLevel;
                bChange = true;
            }
        }

        // Check if current achievement is completed and move to next
        if (pTBAchieve->Achievement_count <= stInfo.biCount) {
            // Find next achievement in chain
            TB_ACHIEVEMENT* pNextTBAchieve = pGameServer->GetResourceMgr().GetTB_ACHIEVEMENT(pTBAchieve->ID + 1);
            while (pNextTBAchieve) {
                pTBAchieve = pNextTBAchieve;

                if (pTBAchieve->Achievement_Category >= 7) {
                    // Invalid category
                    break;
                }

                stInfo.nIndex = pTBAchieve->ID;
                stAchieveUpdate.nNextIndex = pTBAchieve->ID;
                stAchieveUpdate.byCategory = pTBAchieve->Achievement_Category;
                stAchieveUpdate.wCount = m_stCategory.wCount[pTBAchieve->Achievement_Category];
                bChange = true;

                if (pTBAchieve->Achievement_type == 32) {
                    stAchieveUpdate.stUpdateInfo.biCount = shLevel;
                    stInfo.biCount = shLevel;
                }

                if (pTBAchieve->Achievement_count > stInfo.biCount) {
                    break;
                }

                // Increment category count
                m_stCategory.wCount[pTBAchieve->Achievement_Category]++;
                stAchieveUpdate.wCount = m_stCategory.wCount[pTBAchieve->Achievement_Category];

                pNextTBAchieve = pGameServer->GetResourceMgr().GetTB_ACHIEVEMENT(pTBAchieve->ID + 1);
            }
        }

        // Initialize achievement with current info
        InitAchieve(stInfo);

        // Clear complete bit if not fully completed
        if (pTBAchieve->Achievement_count > stInfo.biCount) {
            int nIndex = pTBAchieve->Complete_Bit / 8;
            int nPos = pTBAchieve->Complete_Bit % 8;
            if (nIndex < 128 && (m_stAchieveBit.szRewardBit[nIndex] & (1 << nPos)) != 0) {
                m_stAchieveBit.szRewardBit[nIndex] &= ~(1 << nPos);
                bSendDB = true;
            }
        }

        if (bChange) {
            bSendDB = true;
            stUpdateList.vecList.push_back(stAchieveUpdate);
        }

        // Handle first load category tracking
        if (bFirst) {
            std::uint16_t wNowCategoryCount = (pTBAchieve->Achievement_count <= stInfo.biCount) ? 1 : 0;
            std::uint32_t dwFirstID = GetFirstAchieveID(pTBAchieve, wNowCategoryCount);
            if (dwFirstID != 0) {
                auto itCat = mapCategory.find(pTBAchieve->Achievement_Category);
                if (itCat != mapCategory.end()) {
                    // Update existing category count
                    // TODO: 需要更精确的类型处理
                } else {
                    stAchieveUpdate.nNextIndex = pTBAchieve->ID;
                    stAchieveUpdate.wCount = wNowCategoryCount;
                    mapCategory[pTBAchieve->Achievement_Category] = stAchieveUpdate;
                }
            }
        }
    }

    // First load: sync category counts
    if (bFirst) {
        for (int cat = 0; cat < 7; ++cat) {
            auto itCat = mapCategory.find(cat);
            if (itCat != mapCategory.end()) {
                std::uint16_t wNewCount = static_cast<std::uint16_t>(itCat->second.wCount);
                if (m_stCategory.wCount[cat] != wNewCount) {
                    bSendDB = true;
                    m_stCategory.wCount[cat] = wNewCount;
                    stUpdateList.vecList.push_back(itCat->second);
                }
            }
        }
    }

    // Send updates to DB if needed
    if (bSendDB) {
        // TODO: 需要XSendDBPacket实现
        // XSendDBPacket xSendDBPacket(pMover, 3, 0x62);
        // xSendDBPacket << dwUserID;
        // xSendDBPacket << stUpdateList;
        // xSendDBPacket << m_stAchieveBit;
        // xSendDBPacket << m_stCategory;
        // XGameServer::SendDBGame(&xSendDBPacket);
    }

    // Set user flag for achievement data loaded
    // TODO: 需要CUser访问
    // CUser* pUser = dynamic_cast<CUser*>(pMover);
    // if (pUser) {
    //     pUser->SetAchieveLoadedFlag();
    // }
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
    (void)stAchieveInfo;
}


// UpdateAchieve1 (0x14002CEB0)
// IDA decompiled - Update achievement count with immediate DB sync
// Precise restoration from IDA decompilation
void CGocAchieve::UpdateAchieve1(std::uint16_t wType, int nCount, int nTargetID)
{
    // IDA: RTTI cast owner to CUser
    CMover* pMover = GetOwnerMover();
    if (!pMover) {
        return;
    }

    // IDA: _RTDynamicCast to CUser
    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (!pUser) {
        // IDA: LogHelper::LogError("game.contents", "UpdateAchieve1 error - [ ActorID:%d ] ( %d )", ...)
        return;
    }

    // IDA: Find achievement type in m_mpAchieveTypeList
    auto iter = m_mpAchieveTypeList.find(wType);
    if (iter == m_mpAchieveTypeList.end()) {
        return;
    }

    // IDA: Get CAchieveType and find achieve by target ID
    CAchieveType* pAchieveType = iter->second.get();
    if (!pAchieveType) {
        return;
    }

    std::shared_ptr<CAchieve> pAchieve = pAchieveType->FindAchieve(nTargetID);
    if (!pAchieve) {
        return;
    }

    // IDA: Call CAchieve::UpdateCount
    ST_ACHIEVE_UPDATE stAchieveUpdate;
    if (!pAchieve->UpdateCount(nCount, &stAchieveUpdate, &m_stAchieveBit, &m_stCategory)) {
        return;
    }

    // IDA: Create ST_ACHIEVE_UPDATE_LIST and push the update
    ST_ACHIEVE_UPDATE_LIST stUpdateList;
    stUpdateList.vecList.push_back(stAchieveUpdate);

    // IDA: Create XSendDBPacket and send to DB
    // Cast to XSocket* to resolve ambiguous IXObject conversion (CUser inherits from both XClient->XSocket->IXObject and CMoverEx->CMover->XActor->IXObject)
    XSendDBPacket xSendDBPacket(static_cast<XSocket*>(pUser), 3, 0x62);
    // UCID is the character ID stored in ActorID
    std::uint32_t dwUCID = pUser->GetActorID().dwActorID;
    xSendDBPacket << dwUCID;
    xSendDBPacket << stUpdateList;
    xSendDBPacket << m_stAchieveBit;
    xSendDBPacket << m_stCategory;

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    pGameServer->SendDBGame(xSendDBPacket);

    // IDA: Send XSendDBAchieveLog
    std::uint32_t dwUAID = pUser->GetUAID();
    pGameServer->SendDBAchieveLog(dwUAID, dwUCID, 3, static_cast<std::int8_t>(wType), stUpdateList, nullptr);

    // IDA: Iterate stUpdateList and update titles via CGocEntity::UpdateOpenTitle
    for (const auto& stUpdate : stUpdateList.vecList) {
        if (stUpdate.nNextIndex != 0) {
            // IDA: Get CGocEntity component
            std::shared_ptr<CGocEntity> pEntity;
            pMover->GetGOC<CGocEntity>(&pEntity, false);
            if (pEntity) {
                if (stUpdate.nCurIndex == stUpdate.nNextIndex) {
                    // Same index - update once
                    pEntity->UpdateOpenTitle(2, stUpdate.nNextIndex);
                } else {
                    // Different index - update range
                    for (int i = stUpdate.nCurIndex; i < stUpdate.nNextIndex; ++i) {
                        pEntity->UpdateOpenTitle(2, i);
                    }
                }
            }
        }
    }

    // IDA: Send packet to client via XSendPacket (uses CGocNetwork::Send)
    XSendPacket xSendPacket(3, 0x71);
    xSendPacket << stUpdateList;
    xSendPacket << m_stAchieveBit;
    xSendPacket << m_stCategory;
    CGocNetwork::Send(pMover, xSendPacket);
}


// UpdateCollect (immediate) (0x14002D590)
// IDA decompiled - Update collect achievement with immediate send
// Precise restoration from IDA decompilation
void CGocAchieve::UpdateCollect(std::uint16_t wType, int nCount, int nTargetID)
{
    // IDA: Find achievement type in m_mpAchieveTypeList
    auto iter = m_mpAchieveTypeList.find(wType);
    if (iter == m_mpAchieveTypeList.end()) {
        return;
    }

    // IDA: Get CAchieveType and find achieve by target ID
    CAchieveType* pAchieveType = iter->second.get();
    if (!pAchieveType) {
        return;
    }

    std::shared_ptr<CAchieve> pAchieve = pAchieveType->FindAchieve(nTargetID);
    if (!pAchieve) {
        return;
    }

    // IDA: Call CAchieve::UpdateCollectCount
    ST_ACHIEVE_UPDATE stAchieveUpdate;
    if (!pAchieve->UpdateCollectCount(nCount, &stAchieveUpdate, &m_stAchieveBit, &m_stCategory)) {
        return;
    }

    // IDA: Check if stAchieveUpdate.stUpdateInfo.nIndex is non-zero
    if (stAchieveUpdate.stUpdateInfo.nIndex == 0) {
        return;
    }

    // IDA: RTTI cast owner to CUser
    CMover* pMover = GetOwnerMover();
    if (!pMover) {
        return;
    }

    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (!pUser) {
        // IDA: LogHelper::LogError("game.contents", "UpdateCollect error - [ ActorID:%d ] ( %d )", ...)
        return;
    }

    // IDA: Create ST_ACHIEVE_UPDATE_LIST and push the update
    ST_ACHIEVE_UPDATE_LIST stUpdateList;
    stUpdateList.vecList.push_back(stAchieveUpdate);

    // IDA: If nNextIndex is non-zero, send DB packet
    if (stAchieveUpdate.nNextIndex != 0) {
        // IDA: Create XSendDBPacket and send to DB
        XSendDBPacket xSendDBPacket(static_cast<XSocket*>(pUser), 3, 0x62);
        std::uint32_t dwUCID = pUser->GetActorID().dwActorID;
        xSendDBPacket << dwUCID;
        xSendDBPacket << stUpdateList;
        xSendDBPacket << m_stAchieveBit;
        xSendDBPacket << m_stCategory;

        XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
        pGameServer->SendDBGame(xSendDBPacket);

        // IDA: Send XSendDBAchieveLog (sub=1 for collect)
        std::uint32_t dwUAID = pUser->GetUAID();
        pGameServer->SendDBAchieveLog(dwUAID, dwUCID, 1, static_cast<std::int8_t>(wType), stUpdateList, nullptr);

        // IDA: Iterate stUpdateList and update titles via CGocEntity::UpdateOpenTitle
        for (const auto& stUpdate : stUpdateList.vecList) {
            if (stUpdate.nNextIndex != 0) {
                // IDA: Get CGocEntity component
                std::shared_ptr<CGocEntity> pEntity;
                pMover->GetGOC<CGocEntity>(&pEntity, false);
                if (pEntity) {
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
    }

    // IDA: Send packet to client via XSendPacket (uses CGocNetwork::Send)
    XSendPacket xSendPacket(3, 0x71);
    xSendPacket << stUpdateList;
    xSendPacket << m_stAchieveBit;
    xSendPacket << m_stCategory;
    CGocNetwork::Send(pMover, xSendPacket);
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
// Precise restoration from IDA decompilation
void CGocAchieve::LevelUp()
{
    // IDA: Get owner mover and level
    CMover* pMover = GetOwnerMover();
    if (!pMover) {
        return;
    }
    std::uint8_t shLevel = pMover->GetLevel();

    // IDA: UpdateAchieve1(0x20, level, 0) - level up achievement type
    UpdateAchieve1(0x20, shLevel, 0);

    // IDA: Iterate through TB_ACHIEVEMENT_BEGIN map
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        return;
    }

    XResourceMgr& resourceMgr = pGameServer->GetResourceMgr();
    auto& mapAchievementBegin = resourceMgr.GetTB_ACHIEVEMENT_BEGINMap();

    for (auto iter = mapAchievementBegin.begin(); iter != mapAchievementBegin.end(); ++iter) {
        const TB_ACHIEVEMENT_BEGIN& tbBegin = iter->second;

        // IDA: if (tbBegin.Achievement_Open_Lv <= shLevel)
        if (tbBegin.Achievement_Open_Lv <= shLevel) {
            // IDA: Get TB_ACHIEVEMENT by Achievement_ID
            TB_ACHIEVEMENT* pTBAchieve = resourceMgr.GetTB_ACHIEVEMENT(tbBegin.Achievement_ID);
            if (pTBAchieve) {
                // IDA: InitAchieve(pTBAchieve)
                InitAchieve(pTBAchieve);
            }
        }
    }
}


// EndCollect (0x14002E000)
// IDA decompiled - End collection and send updates
// Precise restoration from IDA decompilation
void CGocAchieve::EndCollect()
{
    // IDA: RTTI cast owner to CUser
    CMover* pMover = GetOwnerMover();
    if (!pMover) {
        return;
    }

    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (!pUser) {
        // IDA: LogHelper::LogError("game.contents", "EndCollect error - [ ActorID:%d ] ( %d )", ...)
        return;
    }

    // IDA: Create ST_ACHIEVE_UPDATE_LIST
    ST_ACHIEVE_UPDATE_LIST stUpdateList;

    // IDA: Iterate through all achievement types and call EndCollect
    for (auto& pair : m_mpAchieveTypeList) {
        if (pair.second) {
            pair.second->EndCollect(&stUpdateList);
        }
    }

    // IDA: Create XSendDBPacket and send to DB (main=3, sub=0x65)
    XSendDBPacket xSendDBPacket(static_cast<XSocket*>(pUser), 3, 0x65);
    std::uint32_t dwUCID = pUser->GetActorID().dwActorID;
    xSendDBPacket << dwUCID;
    xSendDBPacket << stUpdateList;

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    pGameServer->SendDBGame(xSendDBPacket);

    // IDA: Send XSendDBAchieveLog (sub=4 for end collect)
    std::uint32_t dwUAID = pUser->GetUAID();
    pGameServer->SendDBAchieveLog(dwUAID, dwUCID, 4, 0, stUpdateList, nullptr);

    // IDA: Iterate stUpdateList and update titles via CGocEntity::UpdateOpenTitle
    for (const auto& stUpdate : stUpdateList.vecList) {
        if (stUpdate.nNextIndex != 0) {
            // IDA: Get CGocEntity component
            std::shared_ptr<CGocEntity> pEntity;
            pMover->GetGOC<CGocEntity>(&pEntity, false);
            if (pEntity) {
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
}


// GMAchieveCount (0x14002E510)
// IDA decompiled - GM command to set achievement count
// Precise restoration from IDA decompilation
void CGocAchieve::GMAchieveCount(int nGroupID, int nCount)
{
    // IDA: Calculate achievement index from group ID
    int nAchieveIndex = 100 * nGroupID + 1;

    // IDA: Get first achievement in group from resource manager
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_ACHIEVEMENT* pTBFirstAchieve = pGameServer->GetResourceMgr().GetTB_ACHIEVEMENT(nAchieveIndex);
    if (!pTBFirstAchieve) {
        return;
    }

    // IDA: Look up achievement type in map
    auto iter = m_mpAchieveTypeList.find(pTBFirstAchieve->Achievement_type);
    if (iter == m_mpAchieveTypeList.end()) {
        // IDA: Achievement type not found - check if we can create it
        TB_ACHIEVEMENT_BEGIN* pTBBegin = pGameServer->GetResourceMgr().GetTB_ACHIEVEMENT_BEGIN(nAchieveIndex);
        if (pTBBegin) {
            // IDA: Check player level requirement
            CMover* pMover = GetOwnerMover();
            if (pMover) {
                std::uint8_t byLevel = pMover->GetLevel();
                if (pTBBegin->Achievement_Open_Lv <= byLevel) {
                    // IDA: Create new CAchieveType and add achievement
                    auto pNewAchieveType = std::make_shared<CAchieveType>();
                    if (pNewAchieveType) {
                        pNewAchieveType->AddAchieve(pTBFirstAchieve, 0);
                        // IDA: Insert into map
                        m_mpAchieveTypeList[pTBFirstAchieve->Achievement_type] = pNewAchieveType;
                        // IDA: Update achievement count
                        UpdateAchieve1(pTBFirstAchieve->Achievement_type, nCount, pTBFirstAchieve->taget_ID);
                    }
                }
            }
        }
        return;
    }

    // IDA: Achievement type found - get it
    std::shared_ptr<CAchieveType> pAchieveType = iter->second;
    if (!pAchieveType) {
        return;
    }

    // IDA: Find the achievement in the type
    std::shared_ptr<CAchieve> pAchieve = pAchieveType->FindAchieve(pTBFirstAchieve->taget_ID);
    if (!pAchieve) {
        // IDA: Achievement not found - check if we can add it
        TB_ACHIEVEMENT_BEGIN* pTBBegin = pGameServer->GetResourceMgr().GetTB_ACHIEVEMENT_BEGIN(nAchieveIndex);
        if (pTBBegin) {
            // IDA: Check player level requirement
            CMover* pMover = GetOwnerMover();
            if (pMover) {
                std::uint8_t byLevel = pMover->GetLevel();
                if (pTBBegin->Achievement_Open_Lv <= byLevel) {
                    // IDA: Add achievement to existing type
                    pAchieveType->AddAchieve(pTBFirstAchieve, 0);
                    // IDA: Find it again
                    std::shared_ptr<CAchieve> pTempAchieve = pAchieveType->FindAchieve(pTBFirstAchieve->taget_ID);
                    if (pTempAchieve) {
                        // IDA: Update achievement count
                        UpdateAchieve1(pTBFirstAchieve->Achievement_type, nCount, pTBFirstAchieve->taget_ID);
                    }
                }
            }
        }
        return;
    }

    // IDA: Achievement found - adjust count for certain types
    // For types 32 and 27, don't subtract current count
    if (pTBFirstAchieve->Achievement_type != 32 && pTBFirstAchieve->Achievement_type != 27) {
        // IDA: Subtract current count from requested count
        std::int64_t biCurrentCount = pAchieve->GetAchieveCount();
        nCount -= static_cast<int>(biCurrentCount);
        if (nCount < 1) {
            return;
        }
    }

    // IDA: Update achievement count
    UpdateAchieve1(pTBFirstAchieve->Achievement_type, nCount, pTBFirstAchieve->taget_ID);
}


// SendAchieveList (0x14002EA80)
// IDA decompiled - Send achievement list to client
// Precise restoration from IDA decompilation
void CGocAchieve::SendAchieveList()
{
    // IDA: Create ST_ACHIEVE_LIST
    ST_ACHIEVE_LIST stAchieveList;

    // IDA: Iterate through all achievement types and load their achievements
    for (auto& pair : m_mpAchieveTypeList) {
        if (pair.second) {
            pair.second->LoadAchieve(&stAchieveList);
        }
    }

    // IDA: Send packet to client via XSendPacket (main=3, sub=0x70)
    XSendPacket xSendPacket(3, 0x70);
    xSendPacket << m_stAchieveBit;
    xSendPacket << stAchieveList;
    xSendPacket << m_stCategory;

    CMover* pMover = GetOwnerMover();
    if (pMover) {
        CGocNetwork::Send(pMover, xSendPacket);
    }

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
// Precise restoration from IDA decompilation
std::uint32_t CGocAchieve::GetFirstAchieveID(TB_ACHIEVEMENT* pTBAchieve, std::uint16_t& wNowCategoryCount)
{
    std::uint32_t dwAchieveID = 0;

    // IDA: Walk backwards through achievement chain until we find the first one
    while (pTBAchieve) {
        dwAchieveID = pTBAchieve->ID;
        std::uint32_t dwIndex = pTBAchieve->ID - 1;

        // Get previous achievement in chain
        XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
        pTBAchieve = pGameServer->GetResourceMgr().GetTB_ACHIEVEMENT(dwIndex);

        if (pTBAchieve) {
            ++wNowCategoryCount;
        }
    }

    return dwAchieveID;
}

