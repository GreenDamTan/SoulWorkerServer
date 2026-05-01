#pragma once

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_ClientBase.h"
#include "Soulworker/GameServer/XGameServer/MoverEx.h"
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>

// 前置声明
class ST_CHAR_COMMUNITY;
class ST_LEAGUE_INFO_FOR_GAME;
class STMyCharInfoEx;
class TB_CHARACTER;
class TB_ENDURANCE;
class CFSRWLock;
struct ST_CHECK_POS;
struct PS_TICKCOUNT_INFO;
struct TB_SKILL;
struct STPosInfo;
struct hkvVec3;
class XVec3;

// TODO: 推测结果 - 来自 IDA struct CUser + 构造函数 0x1406E2FA0
// CUser 继承自 XClient 和 CMoverEx
// IDA 段器显示多重继承: XClient::XSocket::__vftable 和多个 Vision Engine vtable 赋值
class CUser : public XClient, public CMoverEx {
public:
    // 构造/析构
    CUser();
    virtual ~CUser();

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
    // ...
};
