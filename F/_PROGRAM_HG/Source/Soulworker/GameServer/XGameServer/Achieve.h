// CAchieve - 成就系统类
// 从 GameServer.exe IDA 反编译还原
// 地址范围: 0x1400018a0 - 0x1400032ad

#pragma once

#include <cstdint>

// 前置声明
struct TB_ACHIEVEMENT;
struct ST_ACHIEVE_UPDATE;
struct ST_ACHIEVE_BIT;
struct ST_ACHIEVE_CATEGORY;
struct ST_ACHIEVE_UPDATE_LIST;
struct ST_ACHIEVE_INFO;
struct ST_ACHIEVE_LIST;

class CAchieve {
public:
    // 构造函数 (0x140003280)
    CAchieve();

    // 初始化成就 (0x1400018a0)
    // 如果成就类型为32，则计数初始化为1
    void Init(TB_ACHIEVEMENT* pTBAchieve);

    // 设置成就数据 (0x1400018e0)
    void SetAchieve(TB_ACHIEVEMENT* pTBAchieve, std::int64_t biCount);

    // 更新计数 (0x140001910)
    // 返回值: 是否成功更新
    // 处理不同成就类型(27, 32)，检查完成状态，处理下一级成就
    bool UpdateCount(int nCount, ST_ACHIEVE_UPDATE* stAchieveUpdate, ST_ACHIEVE_BIT* stAchieveBit, ST_ACHIEVE_CATEGORY* stCategory);

    // 更新收集计数 (0x140001c50)
    // 调用 UpdateCount 并管理收集标志
    bool UpdateCollectCount(int nCount, ST_ACHIEVE_UPDATE* stAchieveUpdate, ST_ACHIEVE_BIT* stAchieveBit, ST_ACHIEVE_CATEGORY* stCategory);

    // 结束收集 (0x140001cd0)
    // 如果正在收集，创建更新记录并清除收集标志
    void EndCollect(ST_ACHIEVE_UPDATE_LIST* stUpdateList);

    // GM命令全部清除 (0x140001d30)
    // 返回值: 是否成功
    bool GMAllClear(ST_ACHIEVE_UPDATE_LIST* stUpdateList);

    // 新增功能函数
    // 检查成就条件
    bool CheckAchieve(int nTargetID, int nConditionValue);

    // 更新成就进度
    bool UpdateAchieve(int nCount);

    // 添加成就到列表
    bool AddAchieve(ST_ACHIEVE_LIST* pstList);

    // 移除成就
    bool RemoveAchieve(ST_ACHIEVE_LIST* pstList);

    // 从数据库加载
    bool Load(ST_ACHIEVE_INFO* pstInfo);

    // 保存到数据库
    bool Save(ST_ACHIEVE_INFO* pstInfo);

    // 获取成就计数
    std::int64_t GetAchieveCount() const;

    // 检查成就是否完成
    bool IsAchieveComplete() const;

    // 获取成就奖励
    bool GetAchieveReward(unsigned int* pGold, unsigned int* pEther, unsigned int* pBP);

    // 清除成就
    void ClearAchieve();

    // === 辅助函数 (Round 6 Phase 5) ===

    // Check - Check achievement
    bool Check(int nTargetID, int nConditionValue);

    // Update - Update achievement progress
    bool Update(int nCount);

    // Save - Save to database
    bool Save(void* pDBContext);

    // Load - Load from database
    bool Load(void* pDBContext);

    // Complete - Complete achievement
    bool Complete();

    // GetProgress - Get progress percentage
    int GetProgress() const;

    // GetList - Get achievement list
    bool GetList(ST_ACHIEVE_LIST* pstList);

    // GetReward - Get reward
    bool GetReward(unsigned int* pGold, unsigned int* pEther, unsigned int* pBP);

    // ClaimReward - Claim reward
    bool ClaimReward(ST_ACHIEVE_BIT* pstBit);

    // Reset - Reset achievement
    void Reset();

    // 成员变量
    TB_ACHIEVEMENT* m_pTBAchieve;  // 成就表指针
    std::uint8_t m_bCollect;       // 收集标志
    std::int64_t m_biCount;        // 当前计数
};
