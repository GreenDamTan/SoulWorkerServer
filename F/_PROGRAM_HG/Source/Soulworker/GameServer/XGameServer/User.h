#pragma once

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_ClientBase.h"
#include "Soulworker/GameServer/XGameServer/MoverEx.h"
#include "Soulworker/GameServer/XGameServer/MySkillList.h"
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>

// 前置声明
class ST_CHAR_COMMUNITY;
class ST_LEAGUE_INFO_FOR_GAME;
class STMyCharInfoEx;
struct TB_CHARACTER;
class TB_ENDURANCE;
class CFSRWLock;
struct ST_CHECK_POS;
struct PS_TICKCOUNT_INFO;
struct TB_SKILL;
struct STPosInfo;
struct hkvVec3;
class XVec3;
struct PS_KICK_USER_INFO;  // IDA: 0x140001680

// TODO: 推测结果 - 来自 IDA struct CUser + 构造函数 0x1406E2FA0
// CUser 继承自 XClient 和 CMoverEx
// IDA 段器显示多重继承: XClient::XSocket::__vftable 和多个 Vision Engine vtable 赋值
class CUser : public XClient, public CMoverEx {
public:
    // 构造/析构
    CUser();
    virtual ~CUser() override;

    // === 核心虚函数重写 (IDA 反编译) ===
    // OnUpdate: IDA 0x1406ED290
    virtual void OnUpdate(float fDeltaTime) override;
    // Parse (数据包处理): 继承自 XClient，委托给 XProcessComposite::Parse
    // BridgeSend (发送数据包): IDA 0x1406E8B50
    virtual bool BridgeSend(XSendPacket& xSendPacket);
    // BridgeSend_AfterLoading: IDA 0x1406E8D00
    virtual bool BridgeSend_AfterLoading(XSendPacket& xSendPacket);

    // 核心方法 (来自构造函数调用序列)
    void InitComponant();
    void RegisterProcess();
    void ChangeBattlePose(int nPose);
    void SetInfo();
    void InitStoreSuboInputPacket();

    // 状态查询 (来自 IDA 符号)
    bool IsStatus(std::uint32_t dwStatus);
    bool IsMatching();
    std::int64_t GetExp();
    std::int64_t GetCreateDate();
    std::int64_t GetLastLevelupDate();
    std::int64_t GetLastComeBackDate();
    std::int64_t GetAccountCreateDate();
    std::int64_t GetLastAccountComeBackDate();
    std::uint32_t GetSocialUseID();
    std::uint32_t GetActiveBroachEffect();
    std::int32_t GetLeagueID();
    std::uint16_t GetMaxComboCount();

    // 名称
    std::wstring GetName() const;
    char* GetAccountID();
    std::uint32_t GetFirstUCID();

    // 表 ID
    virtual int GetTableID() override;

    // 状态设置
    void SetMatchingState(bool bState);
    void SetSocialUseID(std::uint32_t dwID);
    void SetLastLevelupDate(std::int64_t biDate);
    void SetReserveRevive(int nType);

    // 其他方法
    float GetFP();
    float GetBonusFP();
    std::uint8_t GetGMPower();
    std::uint8_t GetBlockType();
    bool GetFirstEnter();

    // === GetUAID: IDA 0x14070AF80 ===
    virtual std::uint32_t GetUAID() const;

    // === Kickout: IDA 0x1406EAA70 ===
    void Kickout(PS_KICK_USER_INFO* psKick, bool bDirect);

    // === 战斗相关方法 (IDA 反编译) ===
    // GetHP: IDA 0x14070AC50
    virtual int GetHP() override;
    // GetMaxHP: 继承自 CMoverEx (IDA 0x140189410)
    // SetHP: IDA 0x1406F4880
    virtual void SetHP(int nHP) override;
    // DamageProcessHP: IDA 0x1406F42C0
    // 注意: 基类签名是 (uint32, int, int, int, uint8, uint8) 而不是 (uint32, int, int, uint8, uint8)
    virtual int DamageProcessHP(std::uint32_t dwID, int nSkillID, int nDamage,
                                int nUnk1, std::uint8_t byUnk1, std::uint8_t byUnk2) override;
    // ApplySkillDamageFrame: IDA 0x1406F6140
    // 注意: 基类签名只有 3 个参数
    virtual void ApplySkillDamageFrame(int nSkillID, std::int16_t nTriggerIdx,
                                       std::uint8_t byAttackTargetCnt) override;
    // SetBattleStateTime: IDA referenced in DamageProcessHP
    void SetBattleStateTime(float fTime);

    // === 技能相关方法 (IDA 反编译) ===
    // ProcessChangeCombatAfterUseSkill: IDA 0x1406F67D0
    void ProcessChangeCombatAfterUseSkill();
    // GetCombatChangeTime - 获取战斗状态切换时间
    float GetCombatChangeTime();

    // CheckUseSkill: IDA 0x14037FBD0 (CMoverEx::CheckUseSkill)
    // byCheckVal: 检查类型 (1=总是允许, 2=动作状态检查, 3=倒地检查, 4=反击检查, 5=解锁检查)
    // byNormalVal: 普通检查标志位 (4=不能倒地, 8=不能反击命中, 16=需要解锁buff)
    int CheckUseSkill(std::uint8_t byCheckVal, std::uint8_t byNormalVal, TB_SKILL* pTBSkill);
    // CancelSkill: IDA 0x14037E9E0 (CMoverEx::CancelSkill)
    void CancelSkill();
    // GetSkillLevel: IDA 0x140189040 (CMoverEx::GetSkillLevel)
    std::uint8_t GetSkillLevel();
    // GetSkillCoolDownRate: IDA 0x1402C7240 (CMover::GetSkillCoolDownRate)
    float GetSkillCoolDownRate();
    // SetSkillCoolDownRate - 设置技能冷却速率修正
    void SetSkillCoolDownRate(float fRate);
    // CheckSkillSkipType: IDA 0x14037E490 (CMoverEx::CheckSkillSkipType)
    // Skill_Motion_Skip_Type: 1=检查状态1, 2=检查动作1或3-6, 3=总是跳过
    bool CheckSkillSkipType(std::uint32_t nSkillID);
    // IsCanSkill: IDA 0x14037FB80 (CMoverEx::IsCanSkill)
    // 检查是否可以使用技能 (不能有状态0x40000000或0x80000000)
    bool IsCanSkill();
    // PreSkillProcess: IDA 0x14037D790 (CMoverEx::PreSkillProcess)
    void PreSkillProcess(std::uint32_t nSkillID, int bNormalAttack);
    // SetSkillTable: IDA 0x140188F60 (CMoverEx::SetSkillTable)
    void SetSkillTable(TB_SKILL* pSkillRef);

    // CGocSkill 组件方法
    // IsHaveSkill - 检查是否拥有指定技能
    bool IsHaveSkill(int nSkillID);
    // LearnSkill - 学习新技能
    bool LearnSkill(int nSkillID, bool bUseCheat = false, int nTicknum = 0);
    // ResetSkill - 重置技能点
    void ResetSkill(bool bUseCheat = false, int nTicknum = 0);

    // CMySkillList 技能列表方法
    // UseSkill - 使用技能 (IDA 0x1402B75E0)
    // 返回值: 0=成功, 其他=错误码
    // Skill_Cost_Attribute: 1=HP, 2=SG, 3=Stamina, 4=其他
    int UseSkill(TB_SKILL* pSkillTable, TB_SKILL* pChangedSkillTable = nullptr, float fSkillCost = -1.0f);
    // SetSkillCooltime - 设置技能冷却 (IDA 0x1402C4AD0)
    void SetSkillCooltime(TB_SKILL* pSkillTable);
    // GetSkillCooltime - 获取技能剩余冷却时间 (IDA 0x1402C4940)
    float GetSkillCooltime(int nCooltimeGroup, std::uint16_t wGlobalCoolTime = 0, bool bCheckGlobalCool = true);
    // ReduceSkillCooltime - 减少技能冷却时间 (IDA 0x1402C5280)
    void ReduceSkillCooltime(float fReduceTime);
    // ResetCoolTime - 重置冷却时间 (IDA 0x1402C4870)
    void ResetCoolTime(int eType);

    // 被动技能方法
    // SetPassiveSkillStat - 设置被动技能属性
    void SetPassiveSkillStat(std::uint16_t wBuffID);
    // ClearPassiveSkillStat - 清除被动技能属性
    void ClearPassiveSkillStat(std::uint16_t wBuffID);
    // CheckPassiveSkill - 检查并触发被动技能 (IDA 0x140188FC0 - stub in CMoverEx)
    void CheckPassiveSkill(std::uint8_t byType, std::uint8_t byParam);

    // AI 技能条件检查
    // CheckSkillCondition: IDA 0x140269930 (CAi::CheckSkillCondition)
    bool CheckSkillCondition(int nSkillIndex, int nSkillGroup);

private:
    // === IDA 构造函数确认的成员变量 ===
    // CUser 构造函数初始化顺序 (0x1406E2FA0):

    // 1. 基类构造: XClient::XClient(this)
    // 2. 基类构造: CMoverEx::CMoverEx(&this->CMoverEx)
    // 3-10. 多个 vtable 赋值 (Vision Engine 多重继承)

    // 11. ST_CHAR_COMMUNITY::ST_CHAR_COMMUNITY(&m_stCharCommunity)
    ST_CHAR_COMMUNITY m_stCharCommunity;

    // 12. ST_LEAGUE_INFO_FOR_GAME::ST_LEAGUE_INFO_FOR_GAME(&m_stLeagueInfo)
    ST_LEAGUE_INFO_FOR_GAME m_stLeagueInfo;

    // 13. STMyCharInfoEx::STMyCharInfoEx(&m_stCharInfo)
    STMyCharInfoEx m_stCharInfo;

    // 14. m_pCharTableRef = nullptr
    TB_CHARACTER* m_pCharTableRef;

    // 15. m_fContinousAttackCheckTime = 0.0
    float m_fContinousAttackCheckTime;

    // 16. m_nContinousAttackHit = 0
    int m_nContinousAttackHit;

    // 17. STPosInfo::STPosInfo(&m_stEnterDistrictPos)
    STPosInfo m_stEnterDistrictPos;

    // 18. hkvVec3::hkvVec3(&m_xvMyroomBackupPos)
    hkvVec3 m_xvMyroomBackupPos;

    // 19. m_pEnduranceTable = nullptr
    TB_ENDURANCE* m_pEnduranceTable;

    // 20-23. 时间日志容器
    std::vector<void*> m_vecChattingTime;  // TODO: 需人工审查 - 类型待确认
    std::map<int, PS_TICKCOUNT_INFO> m_mpTickInfo;
    std::vector<void*> m_vecTickLog;
    std::vector<void*> m_vecPingLog;

    // 24. CFSRWLock::CFSRWLock(&m_rwAreaLock)
    CFSRWLock m_rwAreaLock;

    // 25. std::list<ST_CHECK_POS>::list(&m_listCheckPos)
    std::list<ST_CHECK_POS> m_listCheckPos;

    // 26. CUser::InitComponant(this) - 调用方法
    // 27. CUser::ChangeBattlePose(this, 1) - 调用方法

    // 28-34. 战斗状态
    void* m_pDefensiveWeapon;  // TODO: 需人工审查 - 类型待确认
    void* m_pControlMonster;   // TODO: 需人工审查 - 类型待确认
    std::uint32_t m_dwDedicatedMonsterID;
    int m_nCombatType;
    float m_fCombatChangeTime;
    std::uint8_t m_byCombatChangeUseCount;

    // 35-37. CUser::SetInfo / RegisterProcess 调用
    // 38-40. 传输/传送
    float m_fTransportDelayTime;
    std::uint16_t m_wTransportTableIndex;
    time_t m_tTransportTakeTime;

    // 41-43. 公会相关
    std::int64_t m_biLeagueDeletePenalty;
    std::int64_t m_biLeagueWithdrawPenalty;

    // 44-46. 保活
    int m_nKeepAliveSendCount;
    int m_nKeepAliveKeyCheckCount;
    std::int64_t m_biAuthSessionID;

    // 47-48. m_xvMyroomBackupPos 赋值 (hkvVec3(0,0,0))
    // 49. m_fMyroomBackupYaw = 0.0
    float m_fMyroomBackupYaw;

    // 50-56. 匹配/同步状态
    bool m_bMatchingState;
    bool m_bTestMode;
    std::uint32_t m_dwUserInfoSync;  // 注意: 构造函数中名为 dwUserInfoSync (非 m_dw)
    int m_nLeagueSyncCount;
    bool m_bLeagueSyncFlag;
    int m_nLeagueInventorySyncCount;
    bool m_bSendLeagueInventoryCheck;
    std::int64_t m_biLeagueInventoryTime;

    // 57-66. 更多成员
    float m_fBattleStateTime;
    std::uint32_t m_dwFirstUCID;
    std::int64_t m_biLastLevelUpDate;
    std::int64_t m_biLastComeBackDate;
    bool m_bKick_AlreadyLogin;
    int m_nLastKeepAliveKey;
    int m_nKeepAliveKeyErrorCount;
    float m_fPassiveCheckTime;
    int m_nPrevExp;
    std::uint32_t m_dwUAIDWhenEnter;
    std::uint32_t m_dwUCIDWhenEnter;

    // 67-72. Subo输入相关
    int m_iWaitSuboInputSkillID;
    TB_SKILL* m_pWaitSuboInputSkillTableRef;
    std::uint32_t m_dwWaitSuboInputTime;
    bool m_bSetDeathAttack;
    std::uint32_t m_dwCheckCharacterLocationTime;

    // 73-82. 日志相关 (cleared in constructor)
    std::int64_t m_biLastSendChattingLog;
    std::int64_t m_biLogIncMoney;
    std::int64_t m_biLogDescMoney;
    std::uint32_t m_dwLogMoneyTick;
    bool m_bSentInvenInfo;
    bool m_bWaitSuboInputActionProcess;
    std::uint32_t m_dwLogGapTick;
    std::uint32_t m_dwGap_min;
    bool m_bChangeMap;

    // 大量其他成员需要从 IDA 进一步验证
    // CMoverEx 基类部分约 60KB

    // === IDA 0x140026C30 CUser::IsStatus 使用 ===
    std::uint32_t m_dwStatus;

    // === IDA 0x140082D90 CUser::GetBlockType 使用 ===
    std::uint8_t m_byBlockType;

    // === IDA 0x140085DF0 CUser::SetReserveRevive 使用 ===
    int m_bReserveRevive;

    // === IDA 0x1400F72E0 CUser::GetSocialUseID 使用 ===
    std::uint32_t m_dwSocialUseID;

    // === IDA 0x140049600 CUser::GetFirstEnter 使用 ===
    bool m_bFirstEnter;

    // === IDA 0x1401253E0 CUser::GetCreateDate 使用 ===
    std::int64_t m_nCreateDate;

    // === IDA 0x140125B50 CUser::GetAccountCreateDate 使用 ===
    std::int64_t m_biAccountCreateDate;

    // === IDA 0x140187AC0 CUser::GetLastAccountComeBackDate 使用 ===
    std::int64_t m_biLastAccountComeBackDate;

    // === IDA 0x140165270 CUser::GetMaxComboCount 使用 ===
    int m_nMaxContinousAttackHit;

    // === IDA 0x14070AC50 CUser::GetHP 使用 ===
    // HP 存储在 szBuffer[60695] 偏移处
    int m_nHP;

    // === IDA 0x1406EAA70 CUser::Kickout 使用 ===
    // 踢出时间戳 (用于延迟踢出)
    std::uint64_t m_dwKickoutTime;
};
