#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

// TODO: 汇编还原 - 构造函数 IDA 0x1406E2FA0
CUser::CUser()
    : XClient()
    , CMoverEx()
    , m_pCharTableRef(nullptr)
    , m_fContinousAttackCheckTime(0.0f)
    , m_nContinousAttackHit(0)
    , m_pEnduranceTable(nullptr)
    , m_pDefensiveWeapon(nullptr)
    , m_pControlMonster(nullptr)
    , m_dwDedicatedMonsterID(0)
    , m_nCombatType(-1)
    , m_fCombatChangeTime(0.0f)
    , m_byCombatChangeUseCount(0)
    , m_fTransportDelayTime(0.0f)
    , m_wTransportTableIndex(0)
    , m_tTransportTakeTime(0)
    , m_biLeagueDeletePenalty(0)
    , m_biLeagueWithdrawPenalty(0)
    , m_nKeepAliveSendCount(0)
    , m_nKeepAliveKeyCheckCount(0)
    , m_biAuthSessionID(0)
    , m_fMyroomBackupYaw(0.0f)
    , m_bMatchingState(false)
    , m_bTestMode(false)
    , m_dwUserInfoSync(0)
    , m_nLeagueSyncCount(0)
    , m_bLeagueSyncFlag(false)
    , m_nLeagueInventorySyncCount(0)
    , m_bSendLeagueInventoryCheck(false)
    , m_biLeagueInventoryTime(0)
    , m_fBattleStateTime(0.0f)
    , m_dwFirstUCID(0)
    , m_biLastLevelUpDate(0)
    , m_biLastComeBackDate(0)
    , m_bKick_AlreadyLogin(false)
    , m_nLastKeepAliveKey(0)
    , m_nKeepAliveKeyErrorCount(0)
    , m_fPassiveCheckTime(0.0f)
    , m_nPrevExp(0)
    , m_dwUAIDWhenEnter(0)
    , m_dwUCIDWhenEnter(0)
    , m_iWaitSuboInputSkillID(0)
    , m_pWaitSuboInputSkillTableRef(nullptr)
    , m_dwWaitSuboInputTime(0)
    , m_bSetDeathAttack(false)
    , m_dwCheckCharacterLocationTime(0)
    , m_biLastSendChattingLog(0)
    , m_biLogIncMoney(0)
    , m_biLogDescMoney(0)
    , m_dwLogMoneyTick(0)
    , m_bSentInvenInfo(false)
    , m_bWaitSuboInputActionProcess(false)
    , m_dwLogGapTick(0)
    , m_dwGap_min(0)
    , m_bChangeMap(true)
    , m_dwStatus(0)
    , m_byBlockType(0)
    , m_bReserveRevive(0)
    , m_dwSocialUseID(0)
    , m_bFirstEnter(false)
    , m_nCreateDate(0)
    , m_biAccountCreateDate(0)
    , m_biLastAccountComeBackDate(0)
    , m_nMaxContinousAttackHit(0)
{
    // IDA 构造函数序列:
    // 1. XClient::XClient(this)
    // 2. CMoverEx::CMoverEx(&this->CMoverEx)
    // 3-10. vtable assignments for Vision Engine interfaces
    // 11-13. ST_CHAR_COMMUNITY/ST_LEAGUE_INFO/STMyCharInfoEx construction
    // 14-25. member initializations
    // 26. InitComponant()
    // 27. ChangeBattlePose(1)
    // 35-37. SetInfo() / RegisterProcess()

    GreenDamTan_log(__FILE__, __FUNCTION__, "CUser constructed - stub");

    // TODO: 汇编还原 - vtable 赋值序列
    // this->XClient::XSocket::__vftable = (CUser_vtbl *)&CUser::`vftable'{for `XClient'}
    // this->CMoverEx::CMover::VisBaseEntity_cl::VisObject3D_cl::VisTypedEngineObject_cl::VTypedObject::__vftable = ...
    // ... (7 more vtable assignments)

    InitComponant();
    ChangeBattlePose(1);
    // TODO: SetInfo() 和 RegisterProcess() 调用
}

CUser::~CUser() {
    // TODO: 汇编还原 - 析构函数
}

void CUser::InitComponant() {
    // TODO: 汇编还原 - IDA
    GreenDamTan_log(__FILE__, __FUNCTION__, "InitComponant - stub");
}

void CUser::RegisterProcess() {
    // TODO: 汇编还原 - IDA
    GreenDamTan_log(__FILE__, __FUNCTION__, "RegisterProcess - stub");
}

void CUser::ChangeBattlePose(int nPose) {
    // TODO: 汇编还原 - IDA
}

void CUser::SetInfo() {
    // TODO: 汇编还原 - IDA
    GreenDamTan_log(__FILE__, __FUNCTION__, "SetInfo - stub");
}

void CUser::InitStoreSuboInputPacket() {
    // TODO: 汇编还原 - IDA
    GreenDamTan_log(__FILE__, __FUNCTION__, "InitStoreSuboInputPacket - stub");
}

bool CUser::IsStatus(std::uint32_t dwStatus) {
    // IDA 0x140026C30: return dwStatus & this->m_dwStatus
    return (dwStatus & m_dwStatus) != 0;
}

bool CUser::IsMatching() {
    // TODO: 汇编还原 - IDA 0x140082DF0
    return m_bMatchingState;
}

std::int64_t CUser::GetExp() {
    // IDA 0x1400F64A0: return this->m_stCharInfo.nExp
    return m_stCharInfo.nExp;
}

std::int64_t CUser::GetCreateDate() {
    // IDA 0x1401253E0: return this->m_nCreateDate
    return m_nCreateDate;
}

std::int64_t CUser::GetLastLevelupDate() {
    // IDA 0x140049310: return this->m_biLastLevelUpDate
    return m_biLastLevelUpDate;
}

std::int64_t CUser::GetLastComeBackDate() {
    // IDA 0x140187AE0: return this->m_biLastComeBackDate
    return m_biLastComeBackDate;
}

std::int64_t CUser::GetAccountCreateDate() {
    // IDA 0x140125B50: return this->m_biAccountCreateDate
    return m_biAccountCreateDate;
}

std::int64_t CUser::GetLastAccountComeBackDate() {
    // IDA 0x140187AC0: return this->m_biLastAccountComeBackDate
    return m_biLastAccountComeBackDate;
}

std::uint32_t CUser::GetSocialUseID() {
    // IDA 0x1400F72E0: return this->m_dwSocialUseID
    return m_dwSocialUseID;
}

std::uint32_t CUser::GetActiveBroachEffect() {
    // IDA 0x1400F7CE0: return this->m_stCharInfo.dwActiveBroachEffect
    return m_stCharInfo.dwActiveBroachEffect;
}

std::int32_t CUser::GetLeagueID() {
    // IDA 0x140165500: return this->m_stCharInfo.stLeagueInfo.nLeagueID
    return m_stCharInfo.stLeagueInfo.nLeagueID;
}

std::uint16_t CUser::GetMaxComboCount() {
    // IDA 0x140165270: return this->m_nMaxContinousAttackHit
    return static_cast<std::uint16_t>(m_nMaxContinousAttackHit);
}

std::wstring CUser::GetName() const {
    // TODO: 汇编还原 - IDA 0x140082D20
    return L"";
}

char* CUser::GetAccountID() {
    // IDA 0x140038710: return this->m_stCharInfo.szAccountID
    return m_stCharInfo.szAccountID;
}

std::uint32_t CUser::GetFirstUCID() {
    // IDA 0x140125400: return this->m_dwFirstUCID
    return m_dwFirstUCID;
}

void CUser::SetMatchingState(bool bState) {
    // TODO: 汇编还原 - IDA 0x1400855E0
    m_bMatchingState = bState;
}

void CUser::SetSocialUseID(std::uint32_t dwID) {
    // TODO: 汇编还原 - IDA 0x14018FC60
}

void CUser::SetLastLevelupDate(std::int64_t biDate) {
    // IDA 0x1400492F0
    m_biLastLevelUpDate = biDate;
}

void CUser::SetReserveRevive(int nType) {
    // IDA 0x140085DF0: this->m_bReserveRevive = bReserve
    m_bReserveRevive = nType;
}

float CUser::GetFP() {
    // IDA 0x140048FB0: return (uint16_t)this->m_stCharInfo.shFP
    return static_cast<float>(static_cast<std::uint16_t>(m_stCharInfo.shFP));
}

float CUser::GetBonusFP() {
    // IDA 0x140048F90: return (uint16_t)this->m_stCharInfo.shBonusFP
    return static_cast<float>(static_cast<std::uint16_t>(m_stCharInfo.shBonusFP));
}

std::uint8_t CUser::GetGMPower() {
    // IDA 0x140082DB0: return this->m_stCharInfo.byGMPower
    return m_stCharInfo.byGMPower;
}

std::uint8_t CUser::GetBlockType() {
    // IDA 0x140082D90: return this->m_byBlockType
    return m_byBlockType;
}

bool CUser::GetFirstEnter() {
    // IDA 0x140049600: return this->m_bFirstEnter
    return m_bFirstEnter;
}
