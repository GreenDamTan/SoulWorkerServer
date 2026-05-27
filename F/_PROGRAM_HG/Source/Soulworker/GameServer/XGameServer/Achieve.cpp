// CAchieve 实现文件
// 从 GameServer.exe IDA 反编译还原

#include <cstdint>
#include <cstring>
#include <vector>

// 首先定义结构体宏并包含 TB_ACHIEVEMENT 结构体定义
#define GREENDAMTAN_TB_STRUCT_SECTION
#include "Soulworker/GameServer/XSCommon/Table/TB_ACHIEVEMENT.h"
#undef GREENDAMTAN_TB_STRUCT_SECTION

#include "Soulworker/GameServer/XGameServer/Achieve.h"

// 前向声明所需结构体 (避免 PSServerDB.h 中的编译问题)
struct ST_ACHIEVE_INFO {
    int nIndex = 0;
    std::uint64_t biCount = 0;
};

struct ST_ACHIEVE_LIST {
    std::vector<ST_ACHIEVE_INFO> vecList;
};

struct ST_ACHIEVE_UPDATE {
    ST_ACHIEVE_INFO stUpdateInfo{};
    int nNextIndex = 0;
    std::uint8_t byCategory = 0;
    std::uint16_t wCount = 0;
    int nCurIndex = 0;
    ST_ACHIEVE_UPDATE() : stUpdateInfo{}, nNextIndex(0), byCategory(0), wCount(0), nCurIndex(0) {}
};

struct ST_ACHIEVE_UPDATE_LIST {
    std::vector<ST_ACHIEVE_UPDATE> vecList;
};

struct ST_ACHIEVE_BIT {
    std::uint8_t szRewardBit[128] = {};
};

struct ST_ACHIEVE_CATEGORY {
    std::uint16_t wCount[7] = {};
};

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
// 状态: 已实现
bool CAchieve::UpdateCount(int nCount, ST_ACHIEVE_UPDATE* stAchieveUpdate, ST_ACHIEVE_BIT* stAchieveBit, ST_ACHIEVE_CATEGORY* stCategory) {
    if (!m_pTBAchieve || !stAchieveUpdate || !stAchieveBit || !stCategory) {
        return false;
    }

    // 处理不同成就类型的累加逻辑
    std::int64_t newCount = m_biCount;
    if (m_pTBAchieve->Achievement_type == 27 || m_pTBAchieve->Achievement_type == 32) {
        newCount += nCount;
    } else {
        newCount = nCount;
    }

    m_biCount = newCount;

    // 检查成就完成状态
    bool bComplete = false;
    if (m_pTBAchieve->Achievement_count > 0 && m_biCount >= m_pTBAchieve->Achievement_count) {
        bComplete = true;
    }

    // 设置更新信息
    stAchieveUpdate->stUpdateInfo.nIndex = m_pTBAchieve->ID;
    stAchieveUpdate->stUpdateInfo.biCount = m_biCount;
    stAchieveUpdate->nCurIndex = m_pTBAchieve->ID;

    if (bComplete) {
        // 自动跳转到下一级成就 (ID+1)
        stAchieveUpdate->nNextIndex = m_pTBAchieve->ID + 1;

        // 更新类别计数 (类别索引检查 < 7)
        std::uint8_t categoryIndex = m_pTBAchieve->Achievement_Category;
        if (categoryIndex < 7) {
            stCategory->wCount[categoryIndex]++;
            stAchieveUpdate->byCategory = categoryIndex;
            stAchieveUpdate->wCount = stCategory->wCount[categoryIndex];
        }
    } else {
        stAchieveUpdate->nNextIndex = 0;
    }

    return true;
}

// UpdateCollectCount (0x140001c50)
// 状态: 已实现
bool CAchieve::UpdateCollectCount(int nCount, ST_ACHIEVE_UPDATE* stAchieveUpdate, ST_ACHIEVE_BIT* stAchieveBit, ST_ACHIEVE_CATEGORY* stCategory) {
    // 设置收集标志
    m_bCollect = 1;
    return UpdateCount(nCount, stAchieveUpdate, stAchieveBit, stCategory);
}

// EndCollect (0x140001cd0)
// 状态: 已实现
void CAchieve::EndCollect(ST_ACHIEVE_UPDATE_LIST* stUpdateList) {
    if (m_bCollect && stUpdateList && m_pTBAchieve) {
        // 创建 ST_ACHIEVE_UPDATE 记录
        ST_ACHIEVE_UPDATE stUpdate;
        stUpdate.stUpdateInfo.nIndex = m_pTBAchieve->ID;
        stUpdate.stUpdateInfo.biCount = m_biCount;
        stUpdate.nCurIndex = m_pTBAchieve->ID;
        stUpdate.nNextIndex = 0;

        // 添加到 ST_ACHIEVE_UPDATE_LIST
        stUpdateList->vecList.push_back(stUpdate);

        // 清除 m_bCollect 标志
        m_bCollect = 0;
    }
}

// GMAllClear (0x140001d30)
// 状态: 已实现
bool CAchieve::GMAllClear(ST_ACHIEVE_UPDATE_LIST* stUpdateList) {
    if (!stUpdateList || !m_pTBAchieve) {
        return false;
    }

    // 使用公式 nIndex = 100 * Achievement_Group + 1 找到组内第一级成就
    int nFirstIndex = 100 * m_pTBAchieve->Achievement_Group + 1;

    // 重置成就到第一级
    m_biCount = 0;
    if (m_pTBAchieve->Achievement_type == 32) {
        m_biCount = 1;
    }

    // 创建更新记录
    ST_ACHIEVE_UPDATE stUpdate;
    stUpdate.stUpdateInfo.nIndex = m_pTBAchieve->ID;
    stUpdate.stUpdateInfo.biCount = m_biCount;
    stUpdate.nCurIndex = m_pTBAchieve->ID;
    stUpdate.nNextIndex = nFirstIndex;

    stUpdateList->vecList.push_back(stUpdate);

    return true;
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
