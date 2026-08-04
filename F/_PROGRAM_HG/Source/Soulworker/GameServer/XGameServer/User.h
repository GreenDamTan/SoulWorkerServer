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

// GOC 组件前置声明 (forward declarations for component types)
class CGocSkill;
class CGocNetwork;
class CGocAttribute;
class CGocBooster;
class CGocQuest;
class CGocAchieve;
class CGocParty;
class CGocForce;
class XMaze;

// GOC 组件索引常量
static constexpr int GOC_SKILL = 0;
static constexpr int GOC_NETWORK = 1;
static constexpr int GOC_ATTRIBUTE = 2;
static constexpr int GOC_BOOSTER = 3;
static constexpr int GOC_QUEST = 4;
static constexpr int GOC_ACHIEVE = 5;

// TODO: 推测结果 - 来自 IDA struct CUser + 构造函数 0x1406E2FA0
// CUser 继承自 XClient 和 CMoverEx
// IDA 段器显示多重继承: XClient::XSocket::__vftable 和多个 Vision Engine vtable 赋值
class CUser : public XClient, public CMoverEx {
public:
    // 构造/析构
    CUser();
    virtual ~CUser() override;

    // === 核心虚函数重写 (IDA 反编译) ===
    // OnUpdate: IDA 0x1406ED290 (float deltaTime, different from XClient::OnUpdate(uint64))
    virtual void OnUpdate(float fDeltaTime);
    // Parse (数据包处理): 继承自 XClient，委托给 XProcessComposite::Parse
    // BridgeSend (发送数据包): IDA 0x1406E8B50
    virtual bool BridgeSend(XSendPacket& xSendPacket);
    // BridgeSend_AfterLoading: IDA 0x1406E8D00
    virtual bool BridgeSend_AfterLoading(XSendPacket& xSendPacket);

    void CheckDBLoad_All();
    void SendSyncDBLoad();
    void OnPassiveCheck(float fDeltaTime);

    // 核心方法 (来自构造函数调用序列)
    void InitComponant();
    void RegisterProcess();
    void ChangeBattlePose(int nPose);
    void SetInfo();
    void InitStoreSuboInputPacket();

    // GOC 组件管理模板方法
    template<typename T>
    void CreateComponent(int nIndex);

    template<typename T>
    T* GetGOC();

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
    void SetActiveBroachEffect(std::uint32_t dwEffect);
    std::int32_t GetLeagueID();
    std::uint16_t GetMaxComboCount();
    
    // === Combo System Functions (IDA) ===
    // GetComboCount: IDA 0x14070A470 - Returns current combo count
    virtual int GetComboCount() override;
    
    // CheckContinousAttack: IDA 0x1406F1110 - Check and update continuous attack combo
    std::uint16_t CheckContinousAttack(std::uint8_t byHitCount);
    
    // ApplyComboBuff: IDA 0x1406F15F0 - Apply combo buff effects
    void ApplyComboBuff(const struct TB_COMBO_BUFF* pCombo);

    // 名称
    std::wstring GetName() const;
    char* GetAccountID();
    std::uint32_t GetFirstUCID();

    // 表 ID (not virtual in base class)
    virtual int GetTableID();

    // GetMaze: returns current maze area when the user is inside a maze.
    XMaze* GetMaze() const;

    // 状态设置
    void SetMatchingState(bool bState);
    void SetSocialUseID(std::uint32_t dwID);
    void SetLastLevelupDate(std::int64_t biDate);
    void SetReserveRevive(int nType);

    // 其他方法
    // GetFP: IDA 0x140048FB0 - 返回 unsigned short 扩展到 int64
    std::int64_t GetFP();
    // GetBonusFP: IDA 0x140048F90 - 返回 unsigned short 扩展到 int64
    std::int64_t GetBonusFP();
    // GetPCBangFP: IDA 0x1406F9EE0 - 获取网吧FP (bCheckUse: 是否检查可用状态)
    std::int16_t GetPCBangFP(bool bCheckUse);

    // AddFP: IDA 0x1406F9B20 - 添加FP值，返回是否成功
    bool AddFP(std::int16_t shPoint);
    // AddBonusFP: IDA 0x1406F9BA0 - 添加奖励FP值，返回总是true
    bool AddBonusFP(std::int16_t shPoint);
    // AddPCBangFP: IDA 0x1406F9C20 - 添加网吧FP值，并发送DB更新
    bool AddPCBangFP(std::int16_t shPoint, std::int16_t shPointOther, bool bSendDB);

    // GetGMPower: IDA 0x140082DB0
    std::uint8_t GetGMPower();
    // GetBlockType: IDA 0x140082D90
    std::uint8_t GetBlockType();
    bool GetFirstEnter();

    // === IsGM: IDA 0x1402A4C60 ===
    // IDA: return this->m_stCharInfo.byGMPower != 0
    bool IsGM() const;

    // === IsStatus: IDA 0x140026C30 ===
    // IDA: return dwStatus & this->m_dwStatus
    bool IsStatus(std::uint32_t dwStatus) const;

    // === GetUAID: IDA 0x14070AF80 ===
    // IDA: return this->m_stCharInfo.dwUAID
    virtual std::uint32_t GetUAID() const;

    // === GetUCID: User Character ID ===
    // Returns character ID (same as UAID for this implementation)
    // IDA: Used throughout codebase for logging and DB operations
    virtual std::uint32_t GetUCID() const { return m_stCharInfo.dwUAID; }

    // === GetActorID: IDA 0x1406E8A30 ===
    // IDA: return UXActorID from szBuffer[59743]
    // 返回用户的 ActorID (角色唯一标识)
    virtual UXActorID GetActorID() const;

    // === GetAuthSessionID: IDA 0x1401C9EE0 ===
    std::int64_t GetAuthSessionID() const;

    // === IsPrivateShop: IDA 0x1402D3700 ===
    bool IsPrivateShop() const;

    // === IsPVPPenalty: IDA 0x1401ADC50 ===
    bool IsPVPPenalty() const;

    // === SetFullStat: IDA 0x1401ADC70 ===
    void SetFullStat(bool bFirstEnter);

    // === IsFullStat: IDA 0x1401ADCA0 ===
    bool IsFullStat() const;

    // === SetClientLoadComplete: IDA 0x1401ADCC0 ===
    void SetClientLoadComplete(bool bComplete);

    // === Kickout: IDA 0x1406EAA70 ===
    void Kickout(PS_KICK_USER_INFO* psKick, bool bDirect);

    // === Public Transport Methods ===
    // IsPlayingPublicTransport: IDA 0x1402A5110
    bool IsPlayingPublicTransport() const { return m_bPublicTransportRiding; }
    // GetPublicTransportIndex: IDA 0x1402A50F0
    std::uint16_t GetPublicTransportIndex() const { return m_wTransportTableIndex; }
    // GetPublicTransportTime: IDA 0x1402A50C0
    float GetPublicTransportTime() const;

    // === 派系判断方法 (IDA 反编译) ===
    // IsEnemy: IDA 0x1406F3BA0 - 判断是否为敌人
    virtual bool IsEnemy(CMover* pMover) const override;
    // IsFriend: IDA 0x1406F3C30 - 判断是否为友方
    virtual int IsFriend(CMover* pMover) override;
    // IsLeague: IDA 0x1406F3CC0 - 判断是否为联盟成员
    int IsLeague(CMover* pMover);
    // GetGameOption: IDA 0x1402F6A00 - 获取游戏选项
    void GetGameOption(struct ST_GAME_OPTION& stGameOption);

    // === 战斗相关方法 (IDA 反编译) ===
    // GetHP: IDA 0x14070AC50 (not virtual in base class)
    virtual int GetHP() const;
    // GetMaxHP: 继承自 CMoverEx (IDA 0x140189410)
    // SetHP: IDA 0x1406F4880
    virtual void SetHP(int nHP) override;
    // DamageProcessHP: IDA 0x1406F42C0
    // 注意: 5参数版本 - 重载而非override (基类CMover只有3参数版本)
    virtual int DamageProcessHP(std::uint32_t dwID, int nSkillID, int nDamage,
                                std::uint8_t byDamageFlag, std::uint8_t byHitParts);

    // === 专用怪物相关方法 (IDA 精确还原) ===
    // GetDedicatedMonster: IDA 0x1406FEF70
    CMoverEx* GetDedicatedMonster();
    // CheckDedicatedMonster: IDA 0x1406F41C0
    int CheckDedicatedMonster(std::uint32_t dwID, std::uint32_t nSkillID,
                              std::uint32_t nDamage, std::uint8_t byDamageFlag,
                              std::uint8_t byHitParts);

    // ApplySkillDamageFrame: IDA 0x1406F6140
    // 注意: CUser version with full signature (not override, different from CMover)
    virtual void ApplySkillDamageFrame(int nSkillID, std::int16_t nTriggerIdx,
                                       std::uint8_t byAttackTargetCnt, hkvVec3* vPos,
                                       float fAttackRot, int nContinueAttack,
                                       std::uint8_t byDamageType, bool bPenetrate);
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
    void SetSkillTable(TB_SKILL* pSkillRef) override;

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
    // CheckPassiveSkillByHit - 检查击中时的被动技能 (IDA 0x1406F0480)
    void CheckPassiveSkillByHit(CMoverEx* pMover, TB_SKILL* pSkillTable, std::uint8_t byResult) override;

    // AI 技能条件检查
    // CheckSkillCondition: IDA 0x140269930 (CAi::CheckSkillCondition)
    bool CheckSkillCondition(int nSkillIndex, int nSkillGroup);

    // === Player Actions (Round 8 Phase 4) ===
    // Kickout - Kick player from server (simple overload)
    void Kickout();
    // DamageProcess - Player damage processing
    void DamageProcess(CMover* pAttacker, int nDamage, int nSkillID = 0, int nDamageFlag = 0);
    // OnDie - Player death handler
    void OnDie(CMover* pKiller);
    // Respawn - Respawn player at spawn point
    void Respawn();
    // Revive - Revive player with HP percent
    void Revive(int nHPPercent);
    // Warp - Teleport player to position (IDA decompilation shows this is called)
    void Warp(XVec3* pPos);
    // Exit - Exit current area/maze (IDA decompilation shows this is called)
    void Exit();

    // === Inventory Functions ===
    // AddItem - Add item to inventory, check space, stack
    // Returns: item count added, or -1 on error
    int AddItem(std::uint32_t dwItemID, int nCount, bool bBind = false, int nExpireTime = 0);
    // AddItem - Simple overload with item ID and count only
    BOOL AddItem(int nItemID, int nCount);
    // RemoveItem - Remove item from inventory
    // Returns: item count removed, or -1 on error
    int RemoveItem(std::uint32_t dwItemID, int nCount);
    // RemoveItem - Simple overload
    BOOL RemoveItem(int nItemID, int nCount);
    // UseItem - Use consumable item, apply effects
    // Returns: true on success
    bool UseItem(std::uint32_t dwItemID, int nSlotIndex);
    // UseItem - Use item by slot index
    BOOL UseItem(int nSlotIndex);

    // === Equipment Functions ===
    // EquipItem - Equip item to slot
    // Returns: true on success
    bool EquipItem(int nSlotIndex, int nEquipSlot);
    // EquipItem - Equip item from inventory slot
    // Returns: true on success
    BOOL EquipItem(int nSlotIndex);
    // UnequipItem - Remove item from slot
    // Returns: true on success
    bool UnequipItem(int nEquipSlot);
    // GetEquipSlot - Get item at equipment slot
    // Returns: item ID at slot, or 0 if empty
    std::uint32_t GetEquipSlot(int nEquipSlot) const;

    // === Party Functions ===
    // JoinParty - Join existing party
    // Returns: true on success
    bool JoinParty(std::uint32_t dwPartyID);
    // JoinParty - Join party by ID (simple overload)
    BOOL JoinParty(unsigned long dwPartyID);
    // LeaveParty - Leave current party
    void LeaveParty();
    // CreateParty - Create new party
    // Returns: true on success
    BOOL CreateParty();

    // === Guild Functions ===
    // JoinGuild - Join guild
    // Returns: true on success
    bool JoinGuild(std::uint32_t dwGuildID);
    // JoinGuild - Join guild by ID (simple overload)
    BOOL JoinGuild(unsigned long dwGuildID);
    // LeaveGuild - Leave guild
    // Returns: true on success
    bool LeaveGuild();
    // CreateGuild - Create new guild
    // Returns: true on success
    BOOL CreateGuild(const char* szGuildName);

    // === Trade Functions ===
    // StartTrade - Initiate trade with player
    // Returns: true on success
    bool StartTrade(std::uint32_t dwTargetID);
    // EndTrade - End trade session
    // Returns: true on success
    bool EndTrade();
    // AcceptTrade - Accept trade offer
    // Returns: true on success
    bool AcceptTrade();

    // === Status Functions (IDA) ===
    // SetMP - Set MP/SG value
    // IDA 0x1406F48C0 (estimated)
    void SetMP(int nMP);
    // GetMP - Get current MP/SG
    // IDA 0x14070AC60 (estimated)
    int GetMP();
    // GetMaxHP - Get max HP (override from CMover)
    // IDA 0x140189410 (CMoverEx::GetMaxHP)
    int GetMaxHP() override;
    // GetMaxMP - Get max MP/SG
    // IDA 0x140189450 (estimated)
    int GetMaxMP();
    
    // === Base Class Overrides (IDA) ===
    // GetLevel - IDA 0x140366CB0 (CMover::GetLevel)
    std::uint8_t GetLevel();
    // GetClass - IDA 0x140366C30 (CMover::GetClass)
    std::uint8_t GetClass();
    // GetStat - IDA 0x140166360 (CMover::GetStat)
    float GetStat(int iIndex);
    
    // === Combat Type Functions (IDA) ===
    // ChangeCombatType - IDA 0x1406F66D0 (CUser::ChangeCombatType)
    void ChangeCombatType(int nValue, float fChangeTime, std::uint8_t byUseCount);
    
    // === Skill Animation Functions (IDA) ===
    // ChargeSkillStart - IDA 0x14037EA30 (CMoverEx::ChargeSkillStart)
    void ChargeSkillStart();
    // UpdateSkillAnimInfo - IDA 0x14037EE30 (CMoverEx::UpdateSkillAnimInfo)
    void UpdateSkillAnimInfo(TB_SKILL* pSkillTableRef);
    // GetSkillAnimName - IDA 0x14037EF50 (CMoverEx::GetSkillAnimName)
    const char* GetSkillAnimName(TB_SKILL* pSkillTableRef, std::uint8_t byStep);
    // GetControlType - IDA 0x140398C30 (CMoverEx::GetControlType)
    std::uint8_t GetControlType(TB_SKILL* pSkillTable);

    // === Packet Functions (IDA) ===
    // SendPacket - Send packet to client
    // IDA 0x1406E8B50 (BridgeSend wrapper)
    bool SendPacket(XSendPacket& xSendPacket);
    // BroadcastPacket - Broadcast to nearby players
    // IDA 0x1406E8F00 (estimated)
    void BroadcastPacket(XSendPacket& xSendPacket, float fRadius = 0.0f);
    // SendToParty - Send to party members
    // IDA 0x1406E9000 (estimated)
    void SendToParty(XSendPacket& xSendPacket);
    // SendToGuild - Send to guild members
    // IDA 0x1406E9100 (estimated)
    void SendToGuild(XSendPacket& xSendPacket);

    // SendErrorMessage - Send error message packet to client
    // Used by CGocNetwork::SendErrorMessage
    void SendErrorMessage(std::uint8_t ucMainCmd, std::uint8_t ucSubCmd, std::uint16_t xErrorCode);

    // SendBannerInfo - Send banner info to client
    // IDA 0x1406FEFB0
    void SendBannerInfo();

    // IDA 0x1406E9E60: writes this user's STCharInfoEx portion into an info packet.
    virtual void SetInfoPacket(XSendPacket& xSendPacket);
    // IDA 0x1406E9EC0: refreshes and returns the full character info block.
    STMyCharInfoEx& GetMyCharInfoEx();
    // IDA 0x1406E8A10: returns pointer to character info block (for compatibility with existing code)
    STMyCharInfoEx* stMyCharInfoEx();

    // IDA 0x1406E9F20 - Build buff info from buff state array
    void BuildBuffInfo();

    // === Anti-Cheat Functions (IDA) ===
    
    // CheckSpeedHackAttack - Detect speed hacks in attack timing
    // IDA 0x1406EBA30 - Returns true if speed hack NOT detected
    bool CheckSpeedHackAttack();
    
    // GetSpeedHackAttackLimitTime - Get minimum time between attacks (ms)
    // IDA 0x1406EBA20 - Returns 100ms
    std::uint32_t GetSpeedHackAttackLimitTime();
    
    // CheckInvalidPos - Validate player position for speed/teleport hacks
    // IDA 0x1406EBDE0 - Returns true if position is INVALID (cheat detected)
    // iCallFuncId: 1=move, other=skill/warp
    // fMoveSpeed: client reported speed
    // byRunBit: running flag
    bool CheckInvalidPos(XVec3* vPos, int iCallFuncId, XVec3* vTargetPos, 
                         float fMoveSpeed, std::uint8_t byRunBit);
    
    // CheckValidBot - Check if player name contains "Bot"
    // IDA 0x1406EAA00
    void CheckValidBot();
    
    // SendCheckSpeedLog - Log speed hack detection to DB
    // IDA 0x1407017A0
    void SendCheckSpeedLog();
    
    // CheckKickoutNow - Check if player should be kicked now
    // IDA 0x140701680
    bool CheckKickoutNow();
    
    // SetKick_AlreadyLogin - Mark player for kick due to duplicate login
    // IDA 0x14070AD80
    void SetKick_AlreadyLogin();
    
    // IsKick_AlreadyLogin - Check if player is marked for kick
    // IDA 0x14070AFA0
    bool IsKick_AlreadyLogin();

    // === Data Functions (IDA) ===
    // SaveData - Save player data to database
    // IDA 0x1406E9200 (estimated)
    bool SaveData();
    // LoadData - Load player data from database
    // IDA 0x1406E9300 (estimated)
    bool LoadData();

    // === Inventory Helper Functions ===
    // GetItemCount - Get count of specific item in inventory
    // Returns: item count, or 0 if not found
    int GetItemCount(std::uint32_t dwItemID);

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

    // Public transport state
    bool m_bPublicTransportRiding = false;
    // VPublicTransportPath m_sPublicTransportPath;  // TODO: Add when type is available

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

    // === IDA 0x1401ADC50 CUser::IsPVPPenalty 使用 ===
    bool m_bPVPPenalty;

    // === IDA 0x1401ADC70 CUser::SetFullStat / 0x1401ADCA0 CUser::IsFullStat 使用 ===
    bool m_bFirstWorldEnter;

    // === IDA 0x1401ADCC0 CUser::SetClientLoadComplete 使用 ===
    bool m_bClientLoadComplete;

    // === IDA 0x1402F6A00 CUser::GetGameOption 使用 ===
    ST_GAME_OPTION m_stGameOption{};

    // === Anti-Cheat Member Variables (IDA) ===
    // Speed hack detection - IDA 0x1406EBA30
    std::uint64_t m_dwCheckSpeedHackAttack;         // Last attack time for speed check
    std::uint64_t m_dwCheckSpeedHackAttackForMin;   // 1-second window timer
    int m_nCheatCountForMin;                        // Cheat count per minute
    std::uint32_t m_dwCheatCount[4];                // Total cheat counts by type [3]=speed
    int m_nCurSkillTableIdx;                        // Current skill table index
    
    // Position validation - IDA 0x1406EBDE0
    int m_nCheckWrongSpeedCount;                    // Wrong speed count
    int m_nCheckWrongTargetCount;                   // Wrong target count
    int m_nCheckWrongPosCount;                      // Wrong position count
    float m_fCheckMaxSpeed;                         // Max detected speed
    
    // Bot detection - IDA 0x1406EAA00
    bool m_bIsBot;                                  // Is bot flag

    // GOC 组件表 (继承自 CMover, 在 m_GOComponentTable)
};

// ============================================================================
// GOC 组件模板方法实现
// ============================================================================

template<typename T>
void CUser::CreateComponent(int nIndex) {
    // TODO: 当 CGoc 类型完整定义后取消注释:
    // m_GOComponentTable[nIndex] = std::make_shared<T>();
    // 当前仅保持占位
    if (nIndex < static_cast<int>(m_GOComponentTable.size()) && !m_GOComponentTable[nIndex]) {
        // Leave empty - component should be created by derived class
    }
}

template<typename T>
T* CUser::GetGOC() {
    // TODO: 当组件创建后返回实际指针
    // 需要类型到索引的映射才能正确查找
    // 当前简化实现
    return nullptr;
}
