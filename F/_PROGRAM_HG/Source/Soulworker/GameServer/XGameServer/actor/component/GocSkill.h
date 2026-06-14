#pragma once

#include "GOComponent.h"
#include "Skill.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include <cstdint>
#include <vector>
#include <map>
#include <memory>
#include <cstring>

// Forward declarations
struct PS_SKILL_DECK_PAGE;
struct PS_DECK_ACTIVE;
struct PS_GESTURE_SLOT;
struct PS_UPDATE_DECK_BONUS_VEC;
struct SGroupID;
struct TB_DECK_BONUS;
class CUser;

/**
 * @brief ST_ROGUELIKE_SKILL_ACTIVE_COUNT - 模式技能激活计数结构
 * IDA 反编译确认: 用于 m_mapModeSkillActiveCount
 * 字段布局基于 IDA 0x1401745C0, 0x140174660, 0x140174780
 */
struct ST_ROGUELIKE_SKILL_ACTIVE_COUNT {
    int nTotalCount;      // 总计数 (HIDWORD of vftable in IDA)
    int nCurrentCount;    // 当前计数 (SHIDWORD check in IDA)
    bool bCanUse;         // 是否可用 (LOBYTE of m_eObjectFlags in IDA)

    ST_ROGUELIKE_SKILL_ACTIVE_COUNT() : nTotalCount(0), nCurrentCount(0), bCanUse(true) {}
};

/**
 * @brief CGocSkill - Game Object Component for actor skills
 *
 * Handles actor skills, passive skills, gesture slots, skill decks, etc.
 * IDA 反编译来源: GameServer.exe
 * 构造函数地址: 0x1401682A0
 * 析构函数地址: 0x140168500
 *
 * 内存布局 (IDA确认, 总大小: 1264字节):
 * - 偏移0:   GOComponent基类 (16字节)
 * - 偏移16:  m_HaveSkill (160字节)
 * - 偏移176: m_wTotalSkillPoint (2字节)
 * - 偏移178: m_wSkillPoint (2字节)
 * - 偏移180: m_wSkillDeckSlotCount (2字节)
 * - 偏移184: m_vPassiveSkill (32字节)
 * - 偏移216: m_byDeckCount (1字节)
 * - 偏移217: m_byActiveDeck (1字节)
 * - 偏移220: m_nSkillDeck (480字节, int[5][6][4])
 * - 偏移700: m_stSkillDeckPage (180字节, PS_SKILL_DECK_PAGE[5])
 * - 偏移880: m_nGestureSlot (24字节, int[6])
 * - 偏移904: m_mapSkillDivergence (32字节)
 * - 偏移936: m_bUseModeSkill (1字节)
 * - 偏移944: m_HaveModeSkill (160字节)
 * - 偏移1104: m_nModeSkillDeck (24字节, int[6])
 * - 偏移1128: m_vecModeDefaultSkillList (32字节)
 * - 偏移1160: m_vPassiveModeSkill (32字节)
 * - 偏移1192: m_ModeShopMyInfo (40字节)
 * - 偏移1232: m_mapModeSkillActiveCount (32字节)
 */
class CGocSkill : public GOComponent {
public:
    CGocSkill();
    virtual ~CGocSkill();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // 初始化
    void Init();

    // 技能检查
    bool IsHaveBaseSkill(unsigned int nSkillGroup) const;
    bool IsHaveSkillQuickSlot(unsigned int nSkillGroup) const;
    bool IsHaveSkill(int nSkillID) const;

    // 技能加载
    bool LoadSkill(void* stSkillLoad);

    // 技能学习
    bool LearnSkill(int nSkillID, bool bUseCheat = false, int nTicknum = 0);

    // 技能重置
    void ResetSkill(bool bFullReset, int nSkillID);

    // 技能删除
    void DeleteSkill(std::uint16_t wSkillID);

    // 获取技能
    std::shared_ptr<CSkill> GetHaveSkillGroup(int nSkillGroup) const;

    // 技能点管理
    // IDA 0x14016C050: ?AddSkillPoint@CGocSkill@@QEAAXHH_N@Z
    // 参数: nNowPoint - 当前技能点增量, nTotalPoint - 总技能点增量, bSyncDB - 是否同步数据库
    void AddSkillPoint(int nNowPoint, int nTotalPoint, bool bSyncDB = true);

    // 技能组
    static int GetFamilyID();
    bool IsModeState() const;
    int GetModeShopMoney() const;

    // 被动技能
    void SetPassiveSkillStat(std::uint16_t wBuffID);
    void ClearPassiveSkillStat(std::uint16_t wBuffID);
    void CheckPassiveSkill(std::uint8_t byType);

    // 技能卡组
    void ResetSkillDeck();
    bool LoadSkillDeck(void* stSkillLoad);
    bool UpdateSkillDeck(void* stSkillDeckVec);
    bool AddDeckSlot(void* stStorageInfo, int nIndex);
    void ChangeDeckNewSkill(int nOldSkillID, int nNewSkillID);
    bool FindSkillDeck(int nSkillIndex) const;
    void DefualtDeckCount();
    std::uint16_t GetDeckCount() const;
    std::uint8_t GetPageDeckCount() const;
    std::uint16_t GetDeckPos(std::uint16_t wPos) const;
    std::uint8_t GetDeckPage(std::uint16_t wPos) const;
    int ChangeActiveDeck(PS_DECK_ACTIVE psActive);
    int OpenSkillDeck();
    void SetDeckPageInfo(PS_SKILL_DECK_PAGE* psPage, std::uint8_t byCount);

    // 数据库/网络同步
    void SendDBLearnSkill(int nNewSkill, int nOldSkill, int nDivergenceID, int nUseSkillPoint);
    void SendPacketLearnSkill(int nSkillID, std::uint8_t byType, bool bResult, int nDivergenceID, int nTicknum);
    void SendDBUpdateSkillPoint();
    void SendPacketUpdateSkillPoint();
    void SendPacketLoadSkill();

    // 分歧系统
    bool LearnDivergence(int nSkillID, int nDivergenceID, int nReason);

    // 手势槽
    void SetGestureSlot(int* nGestureSlot);
    void SendGestureSlot();
    void GetGestureSlot(PS_GESTURE_SLOT* psGesture);

    // 模式技能
    void InitModeSkill();
    void ResetModeSkill();
    bool LearnModeSkill(int nSkillID);
    bool CanLearnModeSkill(int nSkillID);
    void ClearModeSkillDeck();
    void AddModeSkillDeck(int nSkillID);
    void GetModeDefaultSkillList(std::vector<unsigned long>& vecModeSkillList);
    void GetModeShopMyInfo(PS_ROGUELIKE_SHOP_MY_INFO& psMyInfo);
    void UpdateModeShopMoney(int nAddMoney);
    void AddModeShopBuyList(unsigned int dwUpgradeID);
    bool HaveModeSkillActiveCount(int nGroupID) const;
    void ChargeModeSkillActiveCount(int nGroupID);
    void AddModeSkillActiveCount(int nGroupID, int nTotalCount, int nCount);
    void UpdateModeSkillActiveState(int nGroupID, bool bCanUse);
    void ResetModeSkillActiveState();
    float GetRoguelikeSkillCoolTime(float fTotalTime, unsigned int dwSkillID);

    // 访问器
    int GetSkillPoints() const { return m_wSkillPoint; }
    int GetTotalSkillPoints() const { return m_wTotalSkillPoint; }
    std::uint8_t GetActiveDeck() const { return m_byActiveDeck; }
    std::uint16_t GetSkillDeckSlotCount() const { return m_wSkillDeckSlotCount; }

    // Helper methods
    CUser* GetOwnerUser() const;

    // 卡组加成
    bool DeckBonusAdd(PS_UPDATE_DECK_BONUS_VEC* psBonusList, int nTicknum);
    TB_DECK_BONUS* GetDeckBonus(std::uint8_t byDeckIndex, int nSkillID);

    // 技能点重置
    bool ResetSkillPoint(std::uint32_t dwSkillGroupID, std::uint32_t dwDivergence, bool bUseCheat, int nTicknum);

    // 被动技能应用
    void ApplySkillPassive();
    void ApplySkillPassive(int nSkillID);

    // 卡组名称
    void GetDeckName(void* psDeckNameVec);
    int ChangeDeckName(void* psDeckNameVec);

    // 特殊技能学习
    void AddSkillNoLearn();

protected:
    // 成员变量 - 基于 IDA 反编译的内存布局 (总大小: 1264字节)
    // 注意: 原始代码使用 boost::multi_index_container，这里用 std::map 替代

    // 偏移16: m_HaveSkill - 拥有的技能容器 (原始: boost::multi_index_container<std::shared_ptr<CSkill>, skill_indices>)
    std::map<int, std::shared_ptr<CSkill>> m_HaveSkill;

    // 偏移176: m_wTotalSkillPoint - 总技能点数
    std::uint16_t m_wTotalSkillPoint;

    // 偏移178: m_wSkillPoint - 当前技能点数
    std::uint16_t m_wSkillPoint;

    // 偏移180: m_wSkillDeckSlotCount - 技能卡组槽位数 (初始化为3)
    std::uint16_t m_wSkillDeckSlotCount;

    // 偏移184: m_vPassiveSkill - 被动技能列表
    std::vector<std::shared_ptr<CSkill>> m_vPassiveSkill;

    // 偏移216: m_byDeckCount - 卡组数量
    std::uint8_t m_byDeckCount;

    // 偏移217: m_byActiveDeck - 当前激活的卡组索引
    std::uint8_t m_byActiveDeck;

    // 偏移220: m_nSkillDeck - 技能卡组 (5页 x 6组 x 4槽 = 120个int, IDA显示int[5][6][4])
    int m_nSkillDeck[120];

    // 偏移700: m_stSkillDeckPage - 技能卡组页 (5页, 每页36字节)
    // 原始类型: PS_SKILL_DECK_PAGE[5]
    std::uint8_t m_stSkillDeckPage[5 * 36];

    // 偏移880: m_nGestureSlot - 手势槽 (6个)
    int m_nGestureSlot[6];

    // 偏移904: m_mapSkillDivergence - 技能分歧映射
    std::map<int, int> m_mapSkillDivergence;

    // 偏移936: m_bUseModeSkill - 是否使用模式技能
    bool m_bUseModeSkill;

    // 模式技能相关
    // 偏移944: m_HaveModeSkill - 拥有的模式技能容器
    std::map<int, std::shared_ptr<CSkill>> m_HaveModeSkill;

    // 偏移1104: m_nModeSkillDeck - 模式技能卡组 (6个)
    int m_nModeSkillDeck[6];

    // 偏移1128: m_vecModeDefaultSkillList - 模式默认技能列表
    std::vector<unsigned long> m_vecModeDefaultSkillList;

    // 偏移1160: m_vPassiveModeSkill - 模式被动技能列表
    std::vector<std::shared_ptr<CSkill>> m_vPassiveModeSkill;

    // 偏移1192: m_ModeShopMyInfo - 模式商店信息 (PS_ROGUELIKE_SHOP_MY_INFO, 40字节)
    PS_ROGUELIKE_SHOP_MY_INFO m_ModeShopMyInfo;

    // 偏移1232: m_mapModeSkillActiveCount - 模式技能激活计数
    // 原始类型: std::map<int, ST_ROGUELIKE_SKILL_ACTIVE_COUNT>
    std::map<int, ST_ROGUELIKE_SKILL_ACTIVE_COUNT> m_mapModeSkillActiveCount;
};
