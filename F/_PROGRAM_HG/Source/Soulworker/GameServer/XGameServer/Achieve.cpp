// CAchieve 实现文件
// 从 GameServer.exe IDA 反编译还原

#include "Soulworker/GameServer/XGameServer/Achieve.h"
#include <cstring>

// 构造函数 (0x140003280)
CAchieve::CAchieve()
    : m_pTBAchieve(nullptr)
    , m_bCollect(0)
    , m_biCount(0)
{
}

// Init (0x1400018a0)
void CAchieve::Init(TB_ACHIEVEMENT* pTBAchieve) {
    m_pTBAchieve = pTBAchieve;
    // 如果成就类型为32，则计数初始化为1
    // Note: Achievement_type access requires complete TB_ACHIEVEMENT definition
}

// SetAchieve (0x1400018e0)
void CAchieve::SetAchieve(TB_ACHIEVEMENT* pTBAchieve, std::int64_t biCount) {
    m_pTBAchieve = pTBAchieve;
    m_biCount = biCount;
}

// UpdateCount (0x140001910)
bool CAchieve::UpdateCount(int nCount, ST_ACHIEVE_UPDATE* stAchieveUpdate, ST_ACHIEVE_BIT* stAchieveBit, ST_ACHIEVE_CATEGORY* stCategory) {
    // TODO: Implement from IDA
    return false;
}

// UpdateCollectCount (0x140001c50)
bool CAchieve::UpdateCollectCount(int nCount, ST_ACHIEVE_UPDATE* stAchieveUpdate, ST_ACHIEVE_BIT* stAchieveBit, ST_ACHIEVE_CATEGORY* stCategory) {
    return UpdateCount(nCount, stAchieveUpdate, stAchieveBit, stCategory);
}

// EndCollect (0x140001cd0)
void CAchieve::EndCollect(ST_ACHIEVE_UPDATE_LIST* stUpdateList) {
    if (m_bCollect) {
        m_bCollect = 0;
    }
}

// GMAllClear (0x140001d30)
bool CAchieve::GMAllClear(ST_ACHIEVE_UPDATE_LIST* stUpdateList) {
    return false;
}
