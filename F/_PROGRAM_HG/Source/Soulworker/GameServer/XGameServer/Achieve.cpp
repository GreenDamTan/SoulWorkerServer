// CAchieve 实现文件
// 从 GameServer.exe IDA 反编译还原

#include <cstdint>
#include <cstring>
#include <vector>

#include "Soulworker/GameServer/XGameServer/Achieve.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"

// 构造函数 (0x140003280)
// 状态: 已精确还原
CAchieve::CAchieve()
    : m_pTBAchieve(nullptr)
    , m_bCollect(0)
    , m_biCount(0)
{
}

// Init (0x1400018a0)
// 状态: 已精确还原
void CAchieve::Init(TB_ACHIEVEMENT* pTBAchieve) {
    m_pTBAchieve = pTBAchieve;
    if (pTBAchieve && pTBAchieve->Achievement_type == 32) {
        m_biCount = 1;
    }
}

// SetAchieve (0x1400018e0)
// 状态: 已精确还原
void CAchieve::SetAchieve(TB_ACHIEVEMENT* pTBAchieve, std::int64_t biCount) {
    m_pTBAchieve = pTBAchieve;
    m_biCount = biCount;
}

// UpdateCount (0x140001910)
// IDA 精确还原
bool CAchieve::UpdateCount(int nCount, ST_ACHIEVE_UPDATE* stAchieveUpdate, ST_ACHIEVE_BIT* stAchieveBit, ST_ACHIEVE_CATEGORY* stCategory) {
    // IDA: if ( this->m_pTBAchieve->Achievement_count == this->m_biCount ) return 0;
    if (m_pTBAchieve->Achievement_count == m_biCount) {
        return false;
    }

    // IDA: Achievement_type 检查
    int achievementType = m_pTBAchieve->Achievement_type;
    if (achievementType == 27 || achievementType == 32) {
        // IDA: if ( this->m_biCount >= nCount ) return 0; this->m_biCount = nCount;
        if (m_biCount >= nCount) {
            return false;
        }
        m_biCount = nCount;
    } else {
        // IDA: this->m_biCount += nCount;
        m_biCount += nCount;
    }

    // IDA: stAchieveUpdate->stUpdateInfo.nIndex = this->m_pTBAchieve->ID;
    stAchieveUpdate->stUpdateInfo.nIndex = m_pTBAchieve->ID;
    stAchieveUpdate->stUpdateInfo.biCount = m_biCount;

    // IDA: if ( this->m_pTBAchieve->Achievement_count > this->m_biCount ) return 1;
    if (m_pTBAchieve->Achievement_count > m_biCount) {
        return true;
    }

    // IDA: Achievement_Category 检查 < 7
    if (m_pTBAchieve->Achievement_Category < 7) {
        // IDA: ++stCategory->wCount[this->m_pTBAchieve->Achievement_Category];
        stCategory->wCount[m_pTBAchieve->Achievement_Category]++;
        stAchieveUpdate->nCurIndex = m_pTBAchieve->ID;

        // IDA: 循环查找下一级成就
        XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
        std::uint32_t dwIndex = m_pTBAchieve->ID + 1;
        TB_ACHIEVEMENT* pNextTBAchieve = pGameServer->GetResourceMgr().GetTB_ACHIEVEMENT(dwIndex);

        if (pNextTBAchieve) {
            while (pNextTBAchieve) {
                m_pTBAchieve = pNextTBAchieve;
                if (m_pTBAchieve->Achievement_Category >= 7) {
                    // IDA: LogHelper::LogError
                    return false;
                }
                stAchieveUpdate->nNextIndex = m_pTBAchieve->ID;
                stAchieveUpdate->byCategory = m_pTBAchieve->Achievement_Category;
                stAchieveUpdate->wCount = stCategory->wCount[m_pTBAchieve->Achievement_Category];

                if (m_pTBAchieve->Achievement_count > m_biCount) {
                    return true;
                }
                stCategory->wCount[m_pTBAchieve->Achievement_Category]++;
                dwIndex = m_pTBAchieve->ID + 1;
                pGameServer = TXSingleton<XGameServer>::Instance();
                pNextTBAchieve = pGameServer->GetResourceMgr().GetTB_ACHIEVEMENT(dwIndex);
            }
            stAchieveUpdate->wCount = stCategory->wCount[m_pTBAchieve->Achievement_Category];
            return true;
        } else {
            // IDA: 没有下一级成就时
            stAchieveUpdate->stUpdateInfo.biCount = m_pTBAchieve->Achievement_count;
            stAchieveUpdate->nNextIndex = m_pTBAchieve->ID;
            stAchieveUpdate->byCategory = m_pTBAchieve->Achievement_Category;
            stAchieveUpdate->wCount = stCategory->wCount[m_pTBAchieve->Achievement_Category];
            m_biCount = m_pTBAchieve->Achievement_count;
            return true;
        }
    } else {
        // IDA: LogHelper::LogError - Achievement_Category >= 7
        return false;
    }
}

// UpdateCollectCount (0x140001c50)
// IDA 精确还原
bool CAchieve::UpdateCollectCount(int nCount, ST_ACHIEVE_UPDATE* stAchieveUpdate, ST_ACHIEVE_BIT* stAchieveBit, ST_ACHIEVE_CATEGORY* stCategory) {
    // IDA: if ( !CAchieve::UpdateCount(this, nCount, stAchieveUpdate, stAchieveBit, stCategory) ) return 0;
    if (!UpdateCount(nCount, stAchieveUpdate, stAchieveBit, stCategory)) {
        return false;
    }
    // IDA: if ( !this->m_bCollect ) this->m_bCollect = 1;
    if (!m_bCollect) {
        m_bCollect = 1;
    }
    // IDA: if ( stAchieveUpdate->nNextIndex ) this->m_bCollect = 0;
    if (stAchieveUpdate->nNextIndex) {
        m_bCollect = 0;
    }
    return true;
}

// EndCollect (0x140001cd0)
// IDA 精确还原
void CAchieve::EndCollect(ST_ACHIEVE_UPDATE_LIST* stUpdateList) {
    // IDA: if ( this->m_bCollect )
    if (m_bCollect) {
        // IDA: ST_ACHIEVE_UPDATE::ST_ACHIEVE_UPDATE(&stInfo);
        ST_ACHIEVE_UPDATE stInfo;
        // IDA: stInfo.stUpdateInfo.nIndex = this->m_pTBAchieve->ID;
        stInfo.stUpdateInfo.nIndex = m_pTBAchieve->ID;
        // IDA: stInfo.stUpdateInfo.biCount = this->m_biCount;
        stInfo.stUpdateInfo.biCount = m_biCount;
        // IDA: std::vector<PS_DB_ITEM_MAKE_LIMIT_INFO>::push_back(stUpdateList, (const PS_LEAGUE_INVENTORY_FOR_LOG *)&stInfo);
        stUpdateList->vecList.push_back(stInfo);
        // IDA: this->m_bCollect = 0;
        m_bCollect = 0;
    }
}

// GMAllClear (0x140001d30)
// IDA 精确还原
bool CAchieve::GMAllClear(ST_ACHIEVE_UPDATE_LIST* stUpdateList) {
    // IDA: ST_ACHIEVE_UPDATE::ST_ACHIEVE_UPDATE(&stUpdate);
    ST_ACHIEVE_UPDATE stUpdate;
    // IDA: stUpdate.stUpdateInfo.nIndex = this->m_pTBAchieve->ID;
    stUpdate.stUpdateInfo.nIndex = m_pTBAchieve->ID;
    // IDA: stUpdate.stUpdateInfo.biCount = 0;
    stUpdate.stUpdateInfo.biCount = 0;

    // IDA: if ( this->m_pTBAchieve->Achievement_Order == 1 )
    if (m_pTBAchieve->Achievement_Order == 1) {
        // IDA: std::vector<PS_DB_ITEM_MAKE_LIMIT_INFO>::push_back(stUpdateList, (const PS_LEAGUE_INVENTORY_FOR_LOG *)&stUpdate);
        stUpdateList->vecList.push_back(stUpdate);
        // IDA: this->m_biCount = 0;
        m_biCount = 0;
        return true;
    } else {
        // IDA: nIndex = 100 * this->m_pTBAchieve->Achievement_Group + 1;
        int nIndex = 100 * m_pTBAchieve->Achievement_Group + 1;
        // IDA: v3 = TXSingleton<XGameServer>::Instance();
        XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
        // IDA: pNextAchieve = XResourceMgr::GetTB_ACHIEVEMENT(&v3->m_xResourceMgr, nIndex);
        TB_ACHIEVEMENT* pNextAchieve = pGameServer->GetResourceMgr().GetTB_ACHIEVEMENT(nIndex);

        if (pNextAchieve) {
            // IDA: this->m_pTBAchieve = pNextAchieve;
            m_pTBAchieve = pNextAchieve;
            // IDA: this->m_biCount = 0;
            m_biCount = 0;
            // IDA: stUpdate.nNextIndex = pNextAchieve->ID;
            stUpdate.nNextIndex = pNextAchieve->ID;
            // IDA: std::vector<PS_DB_ITEM_MAKE_LIMIT_INFO>::push_back(stUpdateList, (const PS_LEAGUE_INVENTORY_FOR_LOG *)&stUpdate);
            stUpdateList->vecList.push_back(stUpdate);
            return true;
        } else {
            return false;
        }
    }
}

// CheckAchieve - 检查成就条件
bool CAchieve::CheckAchieve(int nTargetID, int nConditionValue) {
    if (!m_pTBAchieve) {
        return false;
    }

    // 检查目标ID是否匹配
    if (m_pTBAchieve->taget_ID != nTargetID) {
        return false;
    }

    // 检查计数是否达到条件
    return m_biCount >= nConditionValue;
}

// UpdateAchieve - 更新成就进度
bool CAchieve::UpdateAchieve(int nCount) {
    if (!m_pTBAchieve) {
        return false;
    }

    m_biCount += nCount;
    return true;
}

// AddAchieve - 添加成就到列表
bool CAchieve::AddAchieve(ST_ACHIEVE_LIST* pstList) {
    if (!pstList || !m_pTBAchieve) {
        return false;
    }

    ST_ACHIEVE_INFO stInfo;
    stInfo.nIndex = m_pTBAchieve->ID;
    stInfo.biCount = m_biCount;

    pstList->vecList.push_back(stInfo);
    return true;
}

// RemoveAchieve - 移除成就
bool CAchieve::RemoveAchieve(ST_ACHIEVE_LIST* pstList) {
    if (!pstList || !m_pTBAchieve) {
        return false;
    }

    // 从列表中移除该成就
    auto& vec = pstList->vecList;
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        if (it->nIndex == m_pTBAchieve->ID) {
            vec.erase(it);
            return true;
        }
    }

    return false;
}

// Load - 从数据库加载
bool CAchieve::Load(ST_ACHIEVE_INFO* pstInfo) {
    if (!pstInfo || !m_pTBAchieve) {
        return false;
    }

    m_biCount = pstInfo->biCount;
    return true;
}

// Save - 保存到数据库
bool CAchieve::Save(ST_ACHIEVE_INFO* pstInfo) {
    if (!pstInfo || !m_pTBAchieve) {
        return false;
    }

    pstInfo->nIndex = m_pTBAchieve->ID;
    pstInfo->biCount = m_biCount;
    return true;
}

// GetAchieveCount - 获取成就计数
std::int64_t CAchieve::GetAchieveCount() const {
    return m_biCount;
}

// IsAchieveComplete - 检查成就是否完成
bool CAchieve::IsAchieveComplete() const {
    if (!m_pTBAchieve) {
        return false;
    }

    return m_pTBAchieve->Achievement_count > 0 && m_biCount >= m_pTBAchieve->Achievement_count;
}

// GetAchieveReward - 获取成就奖励
bool CAchieve::GetAchieveReward(unsigned int* pGold, unsigned int* pEther, unsigned int* pBP) {
    if (!m_pTBAchieve) {
        return false;
    }

    if (pGold) *pGold = m_pTBAchieve->achievement_Reward_gold;
    if (pEther) *pEther = m_pTBAchieve->achievement_Reward_Ether;
    if (pBP) *pBP = m_pTBAchieve->achievement_Reward_BP;

    return true;
}

// ClearAchieve - 清除成就
void CAchieve::ClearAchieve() {
    m_biCount = 0;
    if (m_pTBAchieve && m_pTBAchieve->Achievement_type == 32) {
        m_biCount = 1;
    }
    m_bCollect = 0;
}

// ============================================================================
// 辅助函数 (Round 6 Phase 5)
// ============================================================================

// Check - Check achievement (wrapper for CheckAchieve)
bool CAchieve::Check(int nTargetID, int nConditionValue) {
    return CheckAchieve(nTargetID, nConditionValue);
}

// Update - Update achievement progress (wrapper for UpdateAchieve)
bool CAchieve::Update(int nCount) {
    return UpdateAchieve(nCount);
}

// Save - Save to database (wrapper for existing Save)
bool CAchieve::Save(void* pDBContext) {
    // 使用现有的 Save 函数
    ST_ACHIEVE_INFO stInfo;
    return Save(&stInfo);
}

// Load - Load from database (wrapper for existing Load)
bool CAchieve::Load(void* pDBContext) {
    // 使用现有的 Load 函数
    ST_ACHIEVE_INFO stInfo;
    stInfo.biCount = 0;
    return Load(&stInfo);
}

// Complete - Complete achievement
bool CAchieve::Complete() {
    if (!m_pTBAchieve) {
        return false;
    }

    // 设置计数为完成条件
    m_biCount = m_pTBAchieve->Achievement_count;
    return true;
}

// GetProgress - Get progress percentage
int CAchieve::GetProgress() const {
    if (!m_pTBAchieve || m_pTBAchieve->Achievement_count <= 0) {
        return 0;
    }

    int nPercent = static_cast<int>((m_biCount * 100) / m_pTBAchieve->Achievement_count);
    if (nPercent > 100) {
        nPercent = 100;
    }

    return nPercent;
}

// GetList - Get achievement list (wrapper for AddAchieve)
bool CAchieve::GetList(ST_ACHIEVE_LIST* pstList) {
    return AddAchieve(pstList);
}

// GetReward - Get reward (wrapper for GetAchieveReward)
bool CAchieve::GetReward(unsigned int* pGold, unsigned int* pEther, unsigned int* pBP) {
    return GetAchieveReward(pGold, pEther, pBP);
}

// ClaimReward - Claim reward
bool CAchieve::ClaimReward(ST_ACHIEVE_BIT* pstBit) {
    if (!pstBit || !m_pTBAchieve) {
        return false;
    }

    // 设置奖励已领取标志
    int nIndex = m_pTBAchieve->ID;
    int nByteIndex = nIndex / 8;
    int nBitIndex = nIndex % 8;

    if (nByteIndex >= 0 && nByteIndex < 128) {
        pstBit->szRewardBit[nByteIndex] |= (1 << nBitIndex);
        return true;
    }

    return false;
}

// Reset - Reset achievement (wrapper for ClearAchieve)
void CAchieve::Reset() {
    ClearAchieve();
}
