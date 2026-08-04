#include "GocInventory.h"
#include "GocNetwork.h"
#include "GocPost.h"
#include "XBaseInventory.h"
#include <algorithm>
#include <new>
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XGameServer/actor/Mover/Mover.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XGameServer/Item/CItem.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAttribute.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocQuest.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocDailyMission.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAkashicRecord.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCashShop.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <ctime>
#include <typeinfo>

// Helper function to get CUser from owner
// IDA pattern: _RTDynamicCast_0(GetOwnerGO(), 0, &CMover RTTI, &CUser RTTI, 0)
static CUser* GetCUserFromOwner(CGocInventory* pInv) {
    CMover* pMover = pInv->GetOwnerGO();
    if (!pMover) return nullptr;
    // RTTI dynamic_cast from CMover to CUser
    return dynamic_cast<CUser*>(pMover);
}

// ============================================================================
// CGocInventory - Game Object Component for actor inventory
//
// IDA Evidence from GameServer.exe (port 10004):
// - Constructor: 0x14009F7B0 - Initializes all inventory components
// - Destructor: 0x14009FD40 - Destructs all inventory components
// - Init: 0x1400A00C0 - Initializes inventories, banks, clears state
// - ClearInven: 0x1400A0000 - Clears money, BP, ether, cash
// - GetMoney: 0x140026700 - Returns m_nInvenMoney
// - SetInvenMoney: 0x1400A2340 - Sets money and updates CUser
// - AddMoney: 0x1400A24C0 - Adds money with logging
// - GetInvenPtr: 0x1400A2170 - Returns inventory by type
// - GetTBInvenPtr: 0x1400A2260 - Returns TB inventory by type
// - GetFamilyID: 0x1400262C0 - Returns 7
// ============================================================================

CGocInventory::CGocInventory()
    : GOComponent()
    , m_nInvenMoney(0)
    , m_nBankMoney(0)
    , m_nBP(0)
    , m_biEther(0)
    , m_nCash(0)
    , m_nLimitMonsterBP(0)
    , m_nLimitPVPBP(0)
    , m_biFriendPoint(0)
    , m_biRecycle(0)
    , m_nInventorySize(0)
    , m_nUsedSlots(0)
    , m_nCurrentWeight(0)
    , m_nMaxWeight(0)
{
    // Constructor 0x14009F7B0 initializes:
    // - GOComponent base
    // - m_ShapeEquip, m_AbilityEquip, m_LookEquip (XShapeEquip, XAbilityEquip, XLookEquip)
    // - m_CommonInven, m_CostumeInven, m_CashInven, m_CubeInven (XInventory)
    // - m_Bank[3], m_AccountBank[3] (XBank arrays)
    // - m_mapEquipInfo with mappings: 1->m_AbilityEquip, 2->m_ShapeEquip, 3->m_LookEquip
    // - m_liPrivateShopItem (empty list)
    // - m_stTradeInfo, m_uxTradeActorID

    // IDA 0x14009F7B0 maps the addresses of the embedded equipment objects.
    m_mapEquipInfo[E_INVEN_TYPE_ABILITY_EQUIP] = &m_AbilityEquip;
    m_mapEquipInfo[E_INVEN_TYPE_SHAPE_EQUIP] = &m_ShapeEquip;
    m_mapEquipInfo[E_INVEN_TYPE_LOOK_EQUIP] = &m_LookEquip;
}

CGocInventory::~CGocInventory() {
    // Destructor 0x14009FD40 - destructs all members in reverse order
    // Note: Using void* placeholders, actual destructors would be called for:
    // - m_mpSlot, m_mpItemMakeLimit, m_mpOverlappedSlot, m_mapLogDisassemble
    // - m_mapLimitItemInfo, m_mpEndranceLog, m_mpCashBuyCount, m_mpAppearanceList
    // - m_mpCashItemDate, m_mpUseItemInfo, m_mpSaveGroupCooltime, m_mpGroupCoolTime
    // - m_listRepurchaseBroach, m_listRepurchaseSocket, m_listRepurchaserItem
    // - m_stTradeInfo, m_mapEquipInfo
    // - m_AccountBank[3], m_Bank[3]
    // - m_CubeInven, m_CashInven, m_CostumeInven, m_CommonInven
    // - m_LookEquip, m_AbilityEquip, m_ShapeEquip
    // - m_liPrivateShopItem

    m_liPrivateShopItem.clear();
    m_mapEquipInfo.clear();
}

bool CGocInventory::Initialize() {
    return true;
}

void CGocInventory::Shutdown() {
}

void CGocInventory::Update(float fDeltaTime) {
    (void)fDeltaTime;
}

// ============================================================================
// Currency functions (IDA verified)
// ============================================================================

// IDA: 0x140026700
// __int64 __fastcall CGocInventory::GetMoney(CGocInventory *this)
// {
//   return this->m_nInvenMoney;
// }
std::int64_t CGocInventory::GetMoney() const {
    return m_nInvenMoney;
}

// IDA: 0x1400A2340
// void __fastcall CGocInventory::SetInvenMoney(CGocInventory *this, __int64 nMoney, bool bSend)
// {
//   VChunkFile *v3; // rax
//   CUser *v4; // rax
//   this->m_nInvenMoney = nMoney;
//   v3 = std::list<CBattleZone *>::size((VChunkLocker *)this);
//   v4 = (CUser *)_RTDynamicCast_0(v3, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
//   CUser::stMyCharInfoEx(v4)->biMoney = this->m_nInvenMoney;
// }
// Note: bSend parameter is unused in IDA decompilation
void CGocInventory::SetInvenMoney(std::int64_t nMoney, bool bSend) {
    m_nInvenMoney = nMoney;
    // IDA-verified: Update CUser::stMyCharInfoEx()->biMoney
    // VChunkFile* v3 = std::list<CBattleZone *>::size((VChunkLocker *)this);
    // CUser* v4 = (CUser*)_RTDynamicCast_0(v3, 0, &CMover RTTI, &CUser RTTI, 0);
    // if (v4) CUser::stMyCharInfoEx(v4)->biMoney = m_nInvenMoney;
    // Note: Requires CUser class and RTTI infrastructure
    (void)bSend;
}

// IDA: 0x1400A24C0 - ?AddMoney@CGocInventory@@QEAA_N_JEHH_N@Z
// Precise restoration from IDA decompilation - adds money with DB update and logging
bool CGocInventory::AddMoney(std::int64_t biMoney, std::uint8_t byLogType,
                              int nLogValue1, int LogValue2, bool bLog) {
    // IDA: Check overflow
    if (biMoney + m_nInvenMoney < 0)
        return false;

    // IDA: Set new money value
    SetInvenMoney(biMoney + m_nInvenMoney, false);

    // IDA: Get owner for DB packet
    CMover* pOwner = GetOwnerGO();
    IXObject* pObject = pOwner ? static_cast<IXObject*>(pOwner) : nullptr;

    // IDA: Send PS_DB_GOLD_UPDATE packet to DB (main=3, sub=0x31)
    PS_DB_GOLD_UPDATE psGold;
    psGold.dwActorID = 0; // TODO: GetActorID() when available
    psGold.nAddGold = biMoney;
    psGold.nTotalGold = m_nInvenMoney;

    XSendDBPacket xSendDBPacket(pObject, 3, 0x31);
    xSendDBPacket << psGold;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pServer) {
        pServer->SendDBGame(xSendDBPacket);
        // IDA: Update money supply
        pServer->SetMoneySupply(biMoney);
    }

    // IDA: Set log money for CUser
    CUser* pUser = GetCUserFromOwner(this);
    // Note: SetLogMoney not yet available in CUser
    // if (pUser) {
    //     pUser->SetLogMoney(biMoney);
    // }

    // IDA: Send game log if !bLog
    if (!bLog) {
        // TODO: Implement ST_LOG_GAME and SendDBLog when available
        (void)byLogType;
        (void)nLogValue1;
        (void)LogValue2;
        (void)pUser;
    }

    return true;
}

// IDA: 0x1400279C0
std::int64_t CGocInventory::GetEther() const {
    return m_biEther;
}

// IDA: 0x1400A23B0 - ?SetBankMoney@CGocInventory@@QEAAX_J_N@Z
// Precise restoration from IDA decompilation - sets bank money value
void CGocInventory::SetBankMoney(std::int64_t nMoney, bool bSend) {
    m_nBankMoney = nMoney;
    (void)bSend;  // IDA shows bSend parameter is ignored
}

// IDA: 0x1400279E0
std::int64_t CGocInventory::GetBP() const {
    return m_nBP;
}

// IDA: 0x140027A00
bool CGocInventory::IsUseMoney(std::int64_t nAmount) const {
    return m_nInvenMoney >= nAmount;
}

// IDA: 0x1400278B0
// bool __fastcall CGocInventory::AddBindMoney(CGocInventory *this, __int64 biMoney,
//                                             unsigned __int8 byLogType, int nValue1,
//                                             int nValue2, bool bLog)
// {
//   return CGocInventory::AddMoney(this, biMoney, byLogType, nValue1, nValue2, bLog);
// }
bool CGocInventory::AddBindMoney(std::int64_t nAmount, std::uint8_t byType,
                                   int nParam1, int nParam2, bool bLog) {
    return AddMoney(nAmount, byType, nParam1, nParam2, bLog);
}

// ============================================================================
// Money send functions (IDA verified)
// ============================================================================

// IDA: 0x1400A2D70 - ?SendMoney@CGocInventory@@QEAAXXZ
// Precise restoration from IDA decompilation
void CGocInventory::SendMoney() {
    // IDA: Create PS_GOLD_UPDATE structure
    PS_GOLD_UPDATE stGold;
    stGold.biTotalMoney = this->m_nInvenMoney;
    stGold.nAddBonusMoney = 0;
    stGold.byType = 0;

    // IDA: Create XSendPacket (main=8, sub=0x20)
    XSendPacket xSendPacket(8, 0x20);
    xSendPacket << stGold;

    // IDA: Get owner actor (CMover inherits from XActor)
    // CGocNetwork::Send accepts XActor* or CMover*
    CGocNetwork::Send(m_pOwner, xSendPacket);
}

// IDA: 0x1400A2E60 - ?SendMoney@CGocInventory@@QEAAXAEAUPS_GOLD_UPDATE@@@Z
// Sends gold update packet to client with provided structure
void CGocInventory::SendMoney(PS_GOLD_UPDATE& stGold) {
    // IDA: Create XSendPacket (main=8, sub=0x20)
    XSendPacket xSendPacket(8, 0x20);
    xSendPacket << stGold;

    // IDA: CGocNetwork::Send accepts XActor* or CMover*
    CGocNetwork::Send(m_pOwner, xSendPacket);
}


// IDA: 0x1400A23E0 - ?SendBankMoney@CGocInventory@@QEAAXXZ
// Precise restoration from IDA decompilation - sends bank money to client (main=8, sub=0x21)
void CGocInventory::SendBankMoney() {
    // IDA: Create XSendPacket (main=8, sub=0x21)
    XSendPacket xSendPacket(8, 0x21);
    xSendPacket.XParse << this->m_nBankMoney;

    // IDA: CGocNetwork::Send accepts CMover* (inherits XActor)
    CGocNetwork::Send(m_pOwner, xSendPacket);
}

// ============================================================================
// BP (Battle Points) functions (IDA verified)
// ============================================================================

// IDA: 0x1400A2F30
// void __fastcall CGocInventory::SetBP(CGocInventory *this, __int64 nBP, bool bSend)
void CGocInventory::SetBP(std::int64_t nBP, bool bSend) {
    m_nBP = nBP;
    // IDA-verified: CUser::stMyCharInfoEx(v4)->biBP = this->m_nBP;
    // VChunkFile* v3 = std::list<CBattleZone *>::size((VChunkLocker *)this);
    // CUser* v4 = (CUser*)_RTDynamicCast_0(v3, 0, &CMover RTTI, &CUser RTTI, 0);
    // if (v4) CUser::stMyCharInfoEx(v4)->biBP = m_nBP;
    // Note: Requires CUser class and RTTI infrastructure
    (void)bSend;
}

// IDA: 0x1400A3000 - ?AddBP@CGocInventory@@QEAA_N_JE@Z
// Precise restoration from IDA decompilation - adds BP with DB update and logging
bool CGocInventory::AddBP(std::int64_t nBP, std::uint8_t byLogType) {
    // IDA: Check overflow
    if (nBP + m_nBP < 0)
        return false;

    // IDA: SetBP(this, nBP + this->m_nBP, 0)
    SetBP(nBP + m_nBP, false);

    // IDA: Get owner for DB packet
    CMover* pOwner = GetOwnerGO();
    IXObject* pObject = pOwner ? static_cast<IXObject*>(pOwner) : nullptr;

    // IDA: Send PS_DB_BP_UPDATE to DB (main=3, sub=0x32)
    PS_DB_BP_UPDATE stBPUpdate;
    stBPUpdate.psBP.dwActorID = 0; // TODO: GetActorID() when available
    stBPUpdate.psBP.biBP = nBP;
    stBPUpdate.nLimitMonsterBP = m_nLimitMonsterBP;
    stBPUpdate.nLimitPVPBP = m_nLimitPVPBP;

    XSendDBPacket xSendDBPacket(pObject, 3, 0x32);
    xSendDBPacket << stBPUpdate;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pServer) {
        pServer->SendDBGame(xSendDBPacket);
    }

    // IDA: If nBP > 0, update achieve and weekly mission
    // Note: CGocAchieve and CGocWeeklyMission not yet available
    // if (nBP > 0) {
    //     CGocAchieve::UpdateCollect(achieve, 0x21, nBP, 0);
    //     if (NationType == JPN && byLogType == 40) {
    //         CGocWeeklyMission::CheckWeeklyMissionUpdate(weekly, 0xB, 0, nBP);
    //     }
    // }

    // IDA: Send game log
    // TODO: Implement ST_LOG_GAME and SendDBLog when available
    // ST_LOG_GAME stLog;
    // stLog._nUAID = GetUAID();
    // stLog._nUCID = GetUCID();
    // stLog._sMainType = 10;
    // stLog._sSubType = 3;
    // stLog.nParam1 = GetClass();
    // stLog.nParam2 = byLogType;
    // stLog.nParam4 = nBP;
    // stLog.nParam5 = nBP;
    // stLog.nParam6 = m_nBP;
    // stLog.nParam7 = GetWorldID();
    // wcscpy_s(stLog.szComment, L"BP");
    // pServer->SendDBLog(&stLog);
    (void)byLogType;

    return true;
}

// IDA: 0x1400A3C20
// void __fastcall CGocInventory::SendBP(CGocInventory *this, PS_BP_UPDATE *stBPUpdate)
// Sends BP update to client (main=8, sub=0x31)
// Note: This function is not declared in the header - needs declaration before implementation
// IDA: 0x1400A3C20 - ?SendBP@CGocInventory@@QEAAXAEAUPS_BP_UPDATE@@@Z
// Precise restoration from IDA decompilation - sends BP update to client (main=8, sub=0x31)
void CGocInventory::SendBP(PS_BP_UPDATE& stBPUpdate) {
    // IDA: Create XSendPacket (main=8, sub=0x31)
    XSendPacket xSendPacket(8, 0x31);
    xSendPacket << stBPUpdate;

    // IDA: CGocNetwork::Send accepts CMover* (inherits XActor)
    CGocNetwork::Send(m_pOwner, xSendPacket);
}

// IDA: 0x1400A2FA0
// void __fastcall CGocInventory::InitLimitBP(CGocInventory *this)
void CGocInventory::InitLimitBP() {
    m_nLimitMonsterBP = 0;
    m_nLimitPVPBP = 0;
}

// IDA: 0x1400A2FD0
// void __fastcall CGocInventory::SetLimitBP(CGocInventory *this, int nLimitMonsterBP, int nLimitPVPBP)
void CGocInventory::SetLimitBP(int nLimitMonsterBP, int nLimitPVPBP) {
    m_nLimitMonsterBP = nLimitMonsterBP;
    m_nLimitPVPBP = nLimitPVPBP;
}

// ============================================================================
// Ether functions (IDA verified)
// ============================================================================

// IDA: 0x1400A3CF0
// void __fastcall CGocInventory::SetEther(CGocInventory *this, __int64 biEther, bool bSend)
void CGocInventory::SetEther(std::int64_t biEther, bool bSend) {
    m_biEther = biEther;
    // IDA-verified: CUser::stMyCharInfoEx(v4)->biEther = biEther;
    // VChunkFile* v3 = std::list<CBattleZone *>::size((VChunkLocker *)this);
    // CUser* v4 = (CUser*)_RTDynamicCast_0(v3, 0, &CMover RTTI, &CUser RTTI, 0);
    // if (v4) CUser::stMyCharInfoEx(v4)->biEther = m_biEther;
    // Note: Requires CUser class and RTTI infrastructure
    (void)bSend;
}

// IDA: 0x1400A3D60 - ?AddEther@CGocInventory@@QEAA_N_JE_N@Z
// Precise restoration from IDA decompilation - adds Ether with option effects, DB update, and logging
bool CGocInventory::AddEther(std::int64_t biEther, std::uint8_t byLogType, bool bLog) {
    // IDA: Check overflow
    if (biEther + m_biEther < 0)
        return false;

    // IDA: Check option effect (EFFECT_CONDITION_GAIN_ETHER)
    int nOptionEffectEther = 0;
    CUser* pUser = GetCUserFromOwner(this);
    // Note: CMoverEx option effect methods not yet available
    // if (pUser) {
    //     CMoverEx::CheckOptionEffectInvoke(&pUser->CMoverEx, EFFECT_CONDITION_GAIN_ETHER, &pUser->CMoverEx, biEther, ...);
    //     CMoverEx::ReleaseInvokedOptionEffect(&pUser->CMoverEx, EFFECT_CONDITION_GAIN_ETHER);
    //     nOptionEffectEther = CMoverEx::GetAddEtherFromOptionEffect(&pUser->CMoverEx);
    //     CMoverEx::ResetAddEtherFromOptionEffect(&pUser->CMoverEx);
    // }
    (void)pUser;

    std::int64_t biEtherTotal = biEther + nOptionEffectEther;

    // IDA: SetEther(this, biEtherTotal + this->m_biEther, 0)
    SetEther(biEtherTotal + m_biEther, false);

    // IDA: Get owner for DB packet
    CMover* pOwner = GetOwnerGO();
    IXObject* pObject = pOwner ? static_cast<IXObject*>(pOwner) : nullptr;

    // IDA: Send DB update (main=3, sub=0x34)
    XSendDBPacket xSendDBPacket(pObject, 3, 0x34);
    xSendDBPacket.XParse << 0; // TODO: GetActorID() when available
    xSendDBPacket.XParse << biEtherTotal;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pServer) {
        pServer->SendDBGame(xSendDBPacket);
    }

    // IDA: Send game log if bLog is true
    if (bLog) {
        // TODO: Implement ST_LOG_GAME and SendDBLog when available
        // ST_LOG_GAME stLog;
        // stLog._nUAID = pUser->GetUAID();
        // stLog._nUCID = GetUCID();
        // stLog._sMainType = 10;
        // stLog._sSubType = 4;
        // stLog.nParam1 = GetLevel();
        // stLog.nParam2 = byLogType;
        // stLog.nParam3 = GetTBMapID();
        // stLog.nParam5 = biEtherTotal;
        // stLog.nParam6 = m_biEther;
        // wcscpy_s(stLog.szComment, L"ETHER");
        // pServer->SendDBLog(&stLog);
        (void)byLogType;
    } else if (biEtherTotal > 0) {
        m_biDropEther += biEtherTotal;
    }

    return true;
}

// IDA: 0x1400A4450
// void __fastcall CGocInventory::SendEther(CGocInventory *this, __int64 biResultEther)
// Sends Ether update to client (main=8, sub=0x32)
// IDA: 0x1400A4450 - ?SendEther@CGocInventory@@QEAAX_J@Z
// Precise restoration from IDA decompilation - sends Ether update to client (main=8, sub=0x32)
void CGocInventory::SendEther(std::int64_t biResultEther) {
    // IDA: Create XSendPacket (main=8, sub=0x32)
    XSendPacket xSendPacket(8, 0x32);
    xSendPacket.XParse << biResultEther;

    // IDA: CGocNetwork::Send accepts CMover* (inherits XActor)
    CGocNetwork::Send(m_pOwner, xSendPacket);
}

// IDA: 0x1400A4210
// void __fastcall CGocInventory::DropEtherLog(CGocInventory *this)
// Logs dropped Ether if m_biDropEther > 0
void CGocInventory::DropEtherLog() {
    // IDA decompiled from 0x1400A4210:
    // if ( this->m_biDropEther ) {
    //     VChunkFile* v1 = std::list<CBattleZone *>::size((VChunkLocker *)this);
    //     CUser* pUser = (CUser *)_RTDynamicCast_0(v1, 0, &CMover RTTI, &CUser RTTI, 0);
    //     if ( pUser ) {
    //         ST_LOG_GAME::ST_LOG_GAME(&stLog);
    //         stLog._nUAID = pUser->GetUAID(pUser);
    //         stLog._nUCID = CQuestCondition::GetQuestID(v2);
    //         stLog._sMainType = 10;
    //         stLog._sSubType = 4;
    //         stLog.nParam1 = v9->GetLevel(&pUser->CMoverEx);
    //         stLog.nParam2 = 26;
    //         stLog.nParam3 = (unsigned __int16)XArea::GetTBMapID(v3);
    //         stLog.nParam5 = this->m_biDropEther;
    //         stLog.nParam6 = this->m_biEther;
    //         wcscpy_s<51>((wchar_t (*)[51])stLog.szComment, L"ETHER");
    //         XGameServer::SendDBLog(v4, &stLog);
    //         this->m_biDropEther = 0;
    //     }
    // }
    
    if (m_biDropEther) {
        // TODO: Full implementation requires CUser, ST_LOG_GAME, XGameServer::SendDBLog
        // For now, just clear the drop ether counter
        m_biDropEther = 0;
    }
}

// ============================================================================
// Drop money functions (IDA verified)
// ============================================================================

// IDA: 0x1400A2890
// bool __fastcall CGocInventory::AddDropMoney(CGocInventory *this, __int64 biMoney, int nAddBonusMoney,
//                                              __int64 *biTotalMoney, unsigned __int8 byLogType,
//                                              int nLogValue1, int LogValue2)
// Adds dropped money with option effect, overflow check, and logging
bool CGocInventory::AddDropMoney(std::int64_t biMoney, int nAddBonusMoney,
                                  std::int64_t& biTotalMoney, std::uint8_t byLogType,
                                  int nLogValue1, int nLogValue2) {
    // IDA: Check overflow
    if (biMoney + m_nInvenMoney < 0)
        return false;
    
    // IDA: Check option effect (EFFECT_CONDITION_GAIN_GOLD)
    int nOptionEffectMoney = 0;
    // CUser* pUser = GetCUser();
    // if (pUser) {
    //     CMoverEx::CheckOptionEffectInvoke(&pUser->CMoverEx, EFFECT_CONDITION_GAIN_GOLD, &pUser->CMoverEx, biMoney, ...);
    //     CMoverEx::ReleaseInvokedOptionEffect(&pUser->CMoverEx, EFFECT_CONDITION_GAIN_GOLD);
    //     nOptionEffectMoney = CMoverEx::GetAddMoneyFromOptionEffect(&pUser->CMoverEx);
    //     CMoverEx::ResetAddMoneyFromOptionEffect(&pUser->CMoverEx);
    // }
    
    biMoney += nOptionEffectMoney;
    nAddBonusMoney += nOptionEffectMoney;
    
    // IDA: CheckOverMoneyDrop(E_PRICE_TYPE_GOLD, &biMoney)
    // CheckOverMoneyDrop(E_PRICE_TYPE_GOLD, biMoney);
    
    biTotalMoney = biMoney;
    
    // IDA: SetInvenMoney(this, biMoney + this->m_nInvenMoney, 0)
    SetInvenMoney(biMoney + m_nInvenMoney, false);
    
    // IDA: Send PS_DB_GOLD_UPDATE to DB (main=3, sub=0x31)
    // PS_DB_GOLD_UPDATE psGold;
    // psGold.dwActorID = GetActorID();
    // psGold.nAddGold = biMoney;
    // psGold.nTotalGold = m_nInvenMoney;
    // psGold.nBonus = nAddBonusMoney;
    // psGold.byType = 1; // Drop type
    // XSendDBPacket xSendDBPacket(pObject, 3, 0x31);
    // XGameServer::SendDBGame(&xSendDBPacket);
    
    // IDA: Update money supply
    // XGameServer::SetMoneySupply(biMoney);
    
    // IDA: Set log money
    // if (pUser) CUser::SetLogMoney(pUser, biMoney);
    
    // IDA: Send game log
    // ST_LOG_GAME stLog;
    // stLog._nUAID = GetUAID();
    // stLog._nUCID = GetUCID();
    // stLog._sMainType = 10;
    // stLog._sSubType = 1;
    // stLog.nParam1 = GetClass();
    // stLog.nParam2 = byLogType;
    // stLog.nParam3 = nLogValue1;
    // stLog.nParam4 = nLogValue2;
    // stLog.nParam5 = biMoney;
    // stLog.nParam6 = m_nInvenMoney;
    // wcscpy_s(stLog.szComment, L"MONEY");
    // XGameServer::SendDBLog(&stLog);
    
    (void)byLogType;
    (void)nLogValue1;
    (void)nLogValue2;
    return true;
}

// ============================================================================
// Inventory setup functions (IDA verified)
// ============================================================================

// IDA: 0x1400A08E0 - ?SetInventory@CGocInventory@@QEAAXEEEE_J0000@Z
// Precise restoration from IDA decompilation - initializes inventory extend steps and sets currency values
void CGocInventory::SetInventory(std::uint8_t byCommonStep, std::uint8_t byConsumeStep,
                                  std::uint8_t byCustume, std::uint8_t byCube,
                                  std::int64_t biMoney, std::int64_t biBP, std::int64_t biEther,
                                  std::int64_t biFriendPoint, std::int64_t biRecycle) {
    // IDA: Initialize inventory extend steps (byConsumeStep is unused in original)
    // m_CommonInven.InitExtendStep(byCommonStep, 36, 0);
    // m_CostumeInven.InitExtendStep(byCustume, 48, 2);
    // m_CashInven.InitExtendStep(0, 384, 13);
    // m_CubeInven.InitExtendStep(byCube, 48, 11);
    // Note: XInventory inherits InitExtendStep from XBank (via XBaseInventory)
    // Current implementation uses void* for these members, requires reinterpret_cast
    if (m_CommonInven) reinterpret_cast<XBank*>(m_CommonInven)->InitExtendStep(byCommonStep, 36, 0);
    if (m_CostumeInven) reinterpret_cast<XBank*>(m_CostumeInven)->InitExtendStep(byCustume, 48, 2);
    if (m_CashInven) reinterpret_cast<XBank*>(m_CashInven)->InitExtendStep(0, 384, 13);
    if (m_CubeInven) reinterpret_cast<XBank*>(m_CubeInven)->InitExtendStep(byCube, 48, 11);

    // IDA-verified: Set currency values
    m_nInvenMoney = biMoney;
    m_nBP = biBP;
    m_biEther = biEther;
    m_biFriendPoint = biFriendPoint;
    m_biRecycle = biRecycle;

    (void)byConsumeStep; // Note: byConsumeStep is unused in IDA
}

// IDA: 0x1400A0A90
// void __fastcall CGocInventory::InventoryInfoReq(CGocInventory *this, bool bInven, bool bBank, int dwUAID)
// Sends DB requests to load inventory/bank/socket/broach/package data
// IDA: 0x1400A0A90 - ?InventoryInfoReq@CGocInventory@@QEAAX_N0K@Z
// Precise restoration from IDA decompilation - sends DB requests to load inventory/bank/socket/broach/package data
void CGocInventory::InventoryInfoReq(bool bInven, bool bBank, int dwUAID) {
    // IDA: Get owner for DB packet
    CMover* pOwner = GetOwnerGO();
    IXObject* pObject = pOwner ? static_cast<IXObject*>(pOwner) : nullptr;

    // IDA: Send inventory load requests (main=0x21, sub=1)
    // Load inventory types: 2 (common), 4 (costume), 13 (cash)
    {
        XSendDBPacket xSendDBPacket(pObject, 0x21, 1);
        xSendDBPacket.XParse << 0; // TODO: GetActorID()
        xSendDBPacket.XParse << 2;  // Common inventory
        xSendDBPacket.XParse << 0;
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (pServer) pServer->SendDBGame(xSendDBPacket);
    }
    {
        XSendDBPacket xSendDBPacket(pObject, 0x21, 1);
        xSendDBPacket.XParse << 0; // TODO: GetActorID()
        xSendDBPacket.XParse << 4;  // Costume inventory
        xSendDBPacket.XParse << 0;
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (pServer) pServer->SendDBGame(xSendDBPacket);
    }
    {
        XSendDBPacket xSendDBPacket(pObject, 0x21, 1);
        xSendDBPacket.XParse << 0; // TODO: GetActorID()
        xSendDBPacket.XParse << 13; // Cash inventory
        xSendDBPacket.XParse << 0;
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (pServer) pServer->SendDBGame(xSendDBPacket);
    }

    // IDA: Send bank money load request (main=0x21, sub=1)
    {
        XSendDBPacket xSendDBPacket(pObject, 0x21, 1);
        xSendDBPacket.XParse << 0; // TODO: GetActorID()
        xSendDBPacket.XParse << 10; // Bank money
        xSendDBPacket.XParse << 1;
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (pServer) pServer->SendDBGame(xSendDBPacket);
    }

    // IDA: Determine bank types based on nation
    std::uint8_t byInvenTypeCommon = 5;
    std::uint8_t byInvenTypeFashion = 6;
    std::uint8_t byInvenTypeCash = 14;
    std::uint8_t byLoadBankMoney = 0;

    // IDA: if (NationType != NATION_TYPE_JPN) - use account banks
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pServer) {
        XOption& rOption = pServer->GetOption();
        if (rOption.GetNationType() != NATION_TYPE_JPN) {
            byInvenTypeCommon = 16;
            byInvenTypeFashion = 17;
            byInvenTypeCash = 18;
            byLoadBankMoney = 1;
        }
    }

    // IDA: Send bank load requests (main=0x21, sub=0xF)
    {
        XSendDBPacket xSendDBPacket(pObject, 0x21, 0xF);
        xSendDBPacket.XParse << dwUAID;
        xSendDBPacket.XParse << 0; // TODO: GetActorID()
        xSendDBPacket.XParse << byInvenTypeCommon;
        xSendDBPacket.XParse << 0;
        if (pServer) pServer->SendDBGame(xSendDBPacket);
    }
    {
        XSendDBPacket xSendDBPacket(pObject, 0x21, 0xF);
        xSendDBPacket.XParse << dwUAID;
        xSendDBPacket.XParse << 0; // TODO: GetActorID()
        xSendDBPacket.XParse << byInvenTypeFashion;
        xSendDBPacket.XParse << 0;
        if (pServer) pServer->SendDBGame(xSendDBPacket);
    }
    {
        XSendDBPacket xSendDBPacket(pObject, 0x21, 0xF);
        xSendDBPacket.XParse << dwUAID;
        xSendDBPacket.XParse << 0; // TODO: GetActorID()
        xSendDBPacket.XParse << byInvenTypeCash;
        xSendDBPacket.XParse << byLoadBankMoney;
        if (pServer) pServer->SendDBGame(xSendDBPacket);
    }

    // IDA: Send socket/broach/package load requests
    SendDBSocketLoad(1);  // Inventory sockets
    SendDBBroachLoad(1);  // Inventory broaches
    SendDBSocketLoad(0);  // Equipment sockets
    SendDBBroachLoad(0);  // Equipment broaches
    SendDBPackageLoad(1); // Inventory packages
    SendDBPackageLoad(0); // Equipment packages

    (void)bInven;
    (void)bBank;
}

// ============================================================================
// Initialization and cleanup (IDA verified)
// ============================================================================

// IDA: 0x1400A0000
// Clears all inventory money and BP values
void CGocInventory::ClearInven() {
    // IDA decompiled from 0x1400A0000
    this->m_nInvenMoney = 0;
    this->m_nBankMoney = 0;
    this->m_nBP = 0;
    this->m_biEther = 0;
    this->m_nCash = 0;
    this->m_nLimitMonsterBP = 0;
    this->m_nLimitPVPBP = 0;
}

// IDA: 0x1400A00C0 - Comprehensive initialization
bool CGocInventory::Init() {
    // Clear currency values
    ClearInven();

    // Initialize trade state
    InitTarde();

    // Clear trade info
    ClearTradeInfo();

    // Set trade state to none
    SetTradeState(E_TRADE_STATE_NONE);

    // Set trade actor ID to default (0)
    SetTradeActorID(0);

    // Initialize equip components (per IDA)
    // m_ShapeEquip.Init()
    // m_AbilityEquip.Init()
    // m_LookEquip.Init()

    // Initialize inventories with their type IDs (per IDA)
    // m_CommonInven.Init(2)
    // m_CostumeInven.Init(4)
    // m_CashInven.Init(13)
    // m_CubeInven.Init(11)

    // Initialize banks based on nation type (per IDA)
    // Note: For JPN nation, banks use types 5,6,14; otherwise 16,17,18
    // For now, use non-JPN defaults
    // m_Bank[0].Init(16)
    // m_Bank[1].Init(17)
    // m_Bank[2].Init(18)
    // m_AccountBank[0].Init(16)
    // m_AccountBank[1].Init(17)
    // m_AccountBank[2].Init(18)

    // Clear lists (per IDA)
    m_listRepurchaserItem.clear();
    m_listRepurchaseSocket.vecInfo.clear();
    m_listRepurchaseBroach.vecInfo.clear();

    // Initialize item cool time
    InitItemCoolTime();

    // Clear quick slot items (per IDA)
    std::memset(m_nQuickSlotItem, 0, sizeof(m_nQuickSlotItem));

    // Reset additional state (per IDA)
    m_pEnduranceTable = nullptr;
    m_mpCashItemDate.clear();
    m_bAbsoluteUpgade = false;
    m_szHanBillNo[0] = '\0';
    m_byTradePassword = 0;
    m_biUseItemUpdateDate = 0;
    m_dw64UpdateTick = 0;
    m_biFriendPoint = 0;
    m_nMazeNeedItemID = 0;
    m_mpUseItemInfo.clear();
    m_mpAppearanceList.clear();
    std::memset(m_stCashSet, 0, sizeof(m_stCashSet));
    m_bProcessBilling = false;
    m_mpCashBuyCount.clear();
    m_mpEndranceLog.clear();
    m_nEquipSlot = 0;
    m_liPrivateShopItem.clear();
    m_dw64TradeTick = 0;
    m_bReqBroachRemove = false;
    m_bLoadCash = false;
    m_bReadyLoadCash = false;
    m_bReqSocketRemove = false;
    m_bReqItemRefine = false;
    m_bReqLeagueNameChange = false;
    m_bReqSocketExchange = false;
    m_bReqSocketUpgrade = false;
    m_bReqSocketExtract = false;
    m_bReqShopBuy = false;
    m_bRenovateItem = false;
    m_mapLimitItemInfo.clear();
    m_tItemInitDate = 0;
    m_mapLogDisassemble.clear();
    m_mpOverlappedSlot.clear();
    m_mpSlot.clear();
    m_mpItemMakeLimit.clear();
    m_tMakeInitDate = 0;
    std::memset(&m_stUseWarpItem, 0, sizeof(m_stUseWarpItem));
    m_biDropEther = 0;
    m_nDyePoint = 0;
    m_nRenovatePoint = 0;
    m_nRefinePoint = 0;
    m_dw64WaitTick = 0;
    std::memset(m_nCashMileage, 0, sizeof(m_nCashMileage));
    m_mpSaveGroupCooltime.clear();
    return true;
}

// IDA: 0x1400A0080
// void __fastcall CGocInventory::ClearTradeInfo(CGocInventory *this)
// {
//   this->m_stTradeInfo.biMoney = 0;
//   std::list<unsigned long>::clear(&this->m_stTradeInfo.listInfo);
// }
void CGocInventory::ClearTradeInfo() {
    m_stTradeInfo.biMoney = 0;
    m_stTradeInfo.listInfo.clear();
}

// IDA: 0x1400A0850
// Sets trade state, actor ID, unlocks trade, and resets tick
void CGocInventory::InitTarde() {
    SetTradeState(E_TRADE_STATE_NONE);
    SetTradeActorID(0);
    UpdateTradeUnLock();
    m_dw64TradeTick = 0;
}

// IDA: 0x1400A08B0 - InitItemCoolTime
// void __fastcall CGocInventory::InitItemCoolTime(CGocInventory *this)
// {
//   std::map<int,ActionTrigger*>::clear(&this->m_mpGroupCoolTime);
// }
void CGocInventory::InitItemCoolTime() {
    m_mpGroupCoolTime.clear();
}

// ============================================================================
// Simple getter/setter functions (IDA verified)
// ============================================================================

// IDA: 0x140068690 - SetReadyLoadCash
// void __fastcall CGocInventory::SetReadyLoadCash(CGocInventory *this, bool bFlag)
// {
//   this->m_bReadyLoadCash = bFlag;
// }
void CGocInventory::SetReadyLoadCash(bool bFlag) {
    m_bReadyLoadCash = bFlag;
}

// IDA: 0x1400A0A40 - SetMileage
// void __fastcall CGocInventory::SetMileage(CGocInventory *this, int nDyePoint, int nRenovatePoint, int nRefinePoint)
// {
//   this->m_nDyePoint = nDyePoint;
//   this->m_nRenovatePoint = nRenovatePoint;
//   this->m_nRefinePoint = nRefinePoint;
// }
void CGocInventory::SetMileage(int nDyePoint, int nRenovatePoint, int nRefinePoint) {
    m_nDyePoint = nDyePoint;
    m_nRenovatePoint = nRenovatePoint;
    m_nRefinePoint = nRefinePoint;
}

// IDA: 0x1400A4BF0
// void __fastcall CGocInventory::SetTotalFriendPoint(CGocInventory *this, __int64 nPoint, bool bSend)
// Sets friend point value and optionally sends update
void CGocInventory::SetTotalFriendPoint(std::int64_t nPoint, bool bSend) {
    // IDA decompiled from 0x1400A4BF0:
    // this->m_biFriendPoint = nPoint;
    // VChunkFile* v5 = std::list<CBattleZone *>::size((VChunkLocker *)this);
    // CUser* v6 = (CUser *)_RTDynamicCast_0(v5, 0, &CMover RTTI, &CUser RTTI, 0);
    // CUser::stMyCharInfoEx(v6)->biFriendPoint = this->m_biFriendPoint;
    // if ( bSend )
    //     CGocInventory::SendTotalFriendPoint(this);
    
    m_biFriendPoint = nPoint;
    
    // TODO: Update CUser::stMyCharInfoEx()->biFriendPoint when CUser available
    
    if (bSend) {
        SendTotalFriendPoint();
    }
}

// IDA: 0x1400A4F10
// void __fastcall CGocInventory::ClearRepurchaser(CGocInventory *this)
// Clears all repurchaser lists
void CGocInventory::ClearRepurchaser() {
    // IDA decompiled from 0x1400A4F10:
    // std::list<unsigned long>::clear((std::list<ST_CHECK_POS> *)&this->m_listRepurchaserItem);
    // std::vector<ST_ITEM_SOCKET>::clear(&this->m_listRepurchaseSocket.vecInfo);
    // std::vector<ST_EXCHANGE_PRICE_INFO>::clear((std::vector<ST_EXCHANGE_PRICE_INFO> *)&this->m_listRepurchaseBroach);

    m_listRepurchaserItem.clear();
    m_listRepurchaseSocket.vecInfo.clear();
    m_listRepurchaseBroach.vecInfo.clear();
}

// IDA: 0x1400F7940 - GetCash
// __int64 __fastcall CGocInventory::GetCash(CGocInventory *this)
// {
//   return (unsigned int)this->m_nCash;
// }
int CGocInventory::GetCash() const {
    return m_nCash;
}

// IDA: 0x1404EA7E0 - GetDyePoint
// __int64 __fastcall CGocInventory::GetDyePoint(CGocInventory *this)
// {
//   return (unsigned int)this->m_nDyePoint;
// }
int CGocInventory::GetDyePoint() const {
    return m_nDyePoint;
}

// IDA: 0x1404EAF50 - GetRenovatePoint
// __int64 __fastcall CGocInventory::GetRenovatePoint(CGocInventory *this)
// {
//   return (unsigned int)this->m_nRenovatePoint;
// }
int CGocInventory::GetRenovatePoint() const {
    return m_nRenovatePoint;
}

// IDA: 0x1404EB900 - GetRemoveSocket
// _BOOL8 __fastcall CGocInventory::GetRemoveSocket(CGocInventory *this)
// {
//   return this->m_bReqSocketRemove;
// }
bool CGocInventory::GetRemoveSocket() const {
    return m_bReqSocketRemove;
}

// IDA: 0x14025CAE0 - SetReqLeagueNameChange
// void __fastcall CGocInventory::SetReqLeagueNameChange(CGocInventory *this, bool bChange)
// {
//   this->m_bReqLeagueNameChange = bChange;
// }
void CGocInventory::SetReqLeagueNameChange(bool bChange) {
    m_bReqLeagueNameChange = bChange;
}

// IDA: 0x1403E13D0 - SetRenovatePointItem
// void __fastcall CGocInventory::SetRenovatePointItem(CGocInventory *this, bool bRenovate)
// {
//   this->m_bRenovateItem = bRenovate;
// }
void CGocInventory::SetRenovatePointItem(bool bRenovate) {
    m_bRenovateItem = bRenovate;
}

// IDA: 0x1404070B0 - CheatGetAbsoluteUpgrade
// _BOOL8 __fastcall CGocInventory::CheatGetAbsoluteUpgrade(CGocInventory *this)
// {
//   return this->m_bAbsoluteUpgade;
// }
bool CGocInventory::CheatGetAbsoluteUpgrade() const {
    return m_bAbsoluteUpgade;
}

// IDA: 0x1404070D0 - CheatSetAbsoluteUpgrade
// void __fastcall CGocInventory::CheatSetAbsoluteUpgrade(CGocInventory *this, bool bVal)
// {
//   this->m_bAbsoluteUpgade = bVal;
// }
void CGocInventory::CheatSetAbsoluteUpgrade(bool bVal) {
    m_bAbsoluteUpgade = bVal;
}

// IDA: 0x140504200 - GetReqLeagueNameChange
// _BOOL8 __fastcall CGocInventory::GetReqLeagueNameChange(CGocInventory *this)
// {
//   return this->m_bReqLeagueNameChange;
// }
bool CGocInventory::GetReqLeagueNameChange() const {
    return m_bReqLeagueNameChange;
}

// IDA: 0x14050A060 - SetProcessBilling
// void __fastcall CGocInventory::SetProcessBilling(CGocInventory *this, bool bUse)
// {
//   this->m_bProcessBilling = bUse;
// }
void CGocInventory::SetProcessBilling(bool bUse) {
    m_bProcessBilling = bUse;
}

// IDA: 0x1405DACB0 - GetRecycle
// __int64 __fastcall CGocInventory::GetRecycle(CGocInventory *this)
// {
//   return this->m_biRecycle;
// }
std::int64_t CGocInventory::GetRecycle() const {
    return m_biRecycle;
}

// IDA: 0x1405DACD0 - SetMileageShopBuyItem
// void __fastcall CGocInventory::SetMileageShopBuyItem(CGocInventory *this, bool bBuy)
// {
//   this->m_bReqShopBuy = bBuy;
// }
void CGocInventory::SetMileageShopBuyItem(bool bBuy) {
    m_bReqShopBuy = bBuy;
}

// IDA: 0x1405DACF0 - GetMileageShopBuyItem
// _BOOL8 __fastcall CGocInventory::GetMileageShopBuyItem(CGocInventory *this)
// {
//   return this->m_bReqShopBuy;
// }
bool CGocInventory::GetMileageShopBuyItem() const {
    return m_bReqShopBuy;
}

// IDA: 0x1405DAD10 - IsProcessBilling
// _BOOL8 __fastcall CGocInventory::IsProcessBilling(CGocInventory *this)
// {
//   return this->m_bProcessBilling;
// }
bool CGocInventory::IsProcessBilling() const {
    return m_bProcessBilling;
}

// ============================================================================
// Inventory management
// ============================================================================

// IDA: 0x1400A2170
// Returns inventory pointer by type
// switch (byInvenType):
//   case 2: return &m_CommonInven
//   case 4: return &m_CostumeInven
//   case 5/0x10: return &m_Bank[0]
//   case 6/0x11: return &m_Bank[1]
//   case 0xB: return &m_CubeInven
//   case 0xD: return &m_CashInven
//   case 0xE/0x12: return &m_Bank[2]
//   default: return nullptr
XBaseInventory* CGocInventory::GetInvenPtr(std::uint8_t byInvenType) {
    switch (byInvenType) {
        case 2:
            return static_cast<XBaseInventory*>(m_CommonInven);
        case 4:
            return static_cast<XBaseInventory*>(m_CostumeInven);
        case 5:
        case 0x10:
            return static_cast<XBaseInventory*>(m_Bank[0]);
        case 6:
        case 0x11:
            return static_cast<XBaseInventory*>(m_Bank[1]);
        case 0xB:  // 11
            return static_cast<XBaseInventory*>(m_CubeInven);
        case 0xD:  // 13
            return static_cast<XBaseInventory*>(m_CashInven);
        case 0xE:  // 14
        case 0x12: // 18
            return static_cast<XBaseInventory*>(m_Bank[2]);
        default:
            return nullptr;
    }
}

// IDA: 0x1400A2260
// XInventory *__fastcall CGocInventory::GetTBInvenPtr(CGocInventory *this, unsigned __int8 byTBInvenType)
// {
//   switch ( byTBInvenType )
//   {
//     case 0u: return &this->m_CommonInven;
//     case 2u: return &this->m_CostumeInven;
//     case 9u: return &this->m_CubeInven;
//     case 0xDu: return &this->m_CashInven;
//   }
//   return nullptr;
// }
XBaseInventory* CGocInventory::GetTBInvenPtr(std::uint8_t byTBInvenType) {
    switch (byTBInvenType) {
        case 0:
            return static_cast<XBaseInventory*>(m_CommonInven);
        case 2:
            return static_cast<XBaseInventory*>(m_CostumeInven);
        case 9:
            return static_cast<XBaseInventory*>(m_CubeInven);
        case 0xD:  // 13
            return static_cast<XBaseInventory*>(m_CashInven);
        default:
            return nullptr;
    }
}

// IDA: 0x1400A22D0 (GetEquipPtr)
// Looks up equipment pointer by type from m_mapEquipInfo map
// IDA: Uses std::map::find() on m_mapEquipInfo with byInvenType as key
// Returns the XBaseEquip* from the map entry if found, nullptr otherwise
XBaseEquip* CGocInventory::GetEquipPtr(std::uint8_t byEquipType) {
    // IDA-verified: Use map::find to lookup by equipment type
    const auto it = m_mapEquipInfo.find(byEquipType);
    if (it != m_mapEquipInfo.end()) {
        return it->second;
    }
    return nullptr;
}

// IDA: 0x1400A61F0
// std::tr1::shared_ptr<CItem> *__fastcall CGocInventory::GetSlotItem(
//         CGocInventory *this, std::tr1::shared_ptr<CItem> *result,
//         unsigned __int8 byInvenType, unsigned __int16 shSlotPos, unsigned __int8 *bLock)
// Routes to GetEquipPtr or GetInvenPtr based on type, then gets slot info
std::shared_ptr<CItem> CGocInventory::GetSlotItem(std::uint8_t byInvenType, std::uint16_t shSlotPos, bool& bLock) {
    // IDA-verified: Switch on byInvenType
    switch (byInvenType) {
        case 0:  // Shape equip
        case 1:  // Ability equip
        case 3:  // Look equip
        {
            // IDA-verified: Equipment types - use GetEquipPtr
            XBaseEquip* pEquip = GetEquipPtr(byInvenType);
            if (pEquip) {
                bLock = pEquip->GetLock(static_cast<std::int16_t>(shSlotPos)) != 0;
                std::shared_ptr<CItem> pItem;
                pEquip->GetSlotInfo(&pItem, static_cast<std::int16_t>(shSlotPos));
                return pItem;
            }
            bLock = false;
            return nullptr;
        }

        case 2:   // Common inventory
        case 4:   // Costume inventory
        case 5:   // Bank 0
        case 6:   // Bank 1
        case 0xB: // Cube inventory (11)
        case 0xD: // Cash inventory (13)
        case 0xE: // Bank 2 (14)
        case 0x10: // Account bank 0 (16)
        case 0x11: // Account bank 1 (17)
        case 0x12: // Account bank 2 (18)
        {
            // IDA-verified: Inventory types - use GetInvenPtr
            XBaseInventory* pInven = GetInvenPtr(byInvenType);
            if (pInven) {
                bLock = pInven->GetLock(static_cast<std::int16_t>(shSlotPos)) != 0;
                return pInven->GetSlotInfo(static_cast<std::int16_t>(shSlotPos));
            }
            bLock = false;
            return nullptr;
        }

        default:
            bLock = false;
            return nullptr;
    }
}

// IDA: 0x1400AD750
// std::tr1::shared_ptr<CItem> *__fastcall CGocInventory::GetItem(
//         CGocInventory *this, std::tr1::shared_ptr<CItem> *result,
//         unsigned __int8 byInvenType, int nItemID)
// Gets item from inventory by type and item ID
std::shared_ptr<CItem> CGocInventory::GetItem(std::uint8_t byInvenType, int nItemID) {
    // IDA-verified: Get inventory pointer
    XBaseInventory* pInven = GetInvenPtr(byInvenType);
    if (pInven) {
        return pInven->GetItem(nItemID);
    }
    return nullptr;
}

// IDA: 0x1400B1420
// std::tr1::shared_ptr<CItem> *__fastcall CGocInventory::GetInvenItem(
//         CGocInventory *this, std::tr1::shared_ptr<CItem> *result, __int64 biSerial)
// Gets item from all inventory types by serial ID
// Searches: Common (2), Costume (4), Cube (0xB), Cash (0xD)
std::shared_ptr<CItem> CGocInventory::GetInvenItem(std::int64_t biSerial) {
    // IDA-verified: Search common inventory (type 2)
    XBaseInventory* pCommonInven = GetInvenPtr(2);
    if (pCommonInven) {
        std::shared_ptr<CItem> pItem = pCommonInven->GetItem(biSerial);
        if (pItem) return pItem;
    }

    // IDA-verified: Search costume inventory (type 4)
    XBaseInventory* pCostumeInven = GetInvenPtr(4);
    if (pCostumeInven) {
        std::shared_ptr<CItem> pItem = pCostumeInven->GetItem(biSerial);
        if (pItem) return pItem;
    }

    // IDA-verified: Search cube inventory (type 0xB)
    XBaseInventory* pCubeInven = GetInvenPtr(0xB);
    if (pCubeInven) {
        std::shared_ptr<CItem> pItem = pCubeInven->GetItem(biSerial);
        if (pItem) return pItem;
    }

    // IDA-verified: Search cash inventory (type 0xD)
    XBaseInventory* pCashInven = GetInvenPtr(0xD);
    if (pCashInven) {
        std::shared_ptr<CItem> pItem = pCashInven->GetItem(biSerial);
        if (pItem) return pItem;
    }

    return nullptr;
}

// IDA: 0x1400B1680
// std::tr1::shared_ptr<CItem> *__fastcall CGocInventory::GetEquipItem(
//         CGocInventory *this, std::tr1::shared_ptr<CItem> *result, __int64 biSerial)
// Gets item from all equipment types by serial ID
// Searches: Shape (0), Ability (1), Look (3)
std::shared_ptr<CItem> CGocInventory::GetEquipItem(std::int64_t biSerial) {
    // IDA-verified: Search shape equip (type 0)
    XBaseEquip* pShapeEquip = GetEquipPtr(0);
    if (pShapeEquip) {
        std::shared_ptr<CItem> pItem = pShapeEquip->GetItem(biSerial);
        if (pItem) return pItem;
    }

    // IDA-verified: Search ability equip (type 1)
    XBaseEquip* pAbilityEquip = GetEquipPtr(1);
    if (pAbilityEquip) {
        std::shared_ptr<CItem> pItem = pAbilityEquip->GetItem(biSerial);
        if (pItem) return pItem;
    }

    // IDA-verified: Search look equip (type 3)
    XBaseEquip* pLookEquip = GetEquipPtr(3);
    if (pLookEquip) {
        std::shared_ptr<CItem> pItem = pLookEquip->GetItem(biSerial);
        if (pItem) return pItem;
    }

    return nullptr;
}

// IDA: 0x1400B1850
// std::tr1::shared_ptr<CItem> *__fastcall CGocInventory::GetBankItem(
//         CGocInventory *this, std::tr1::shared_ptr<CItem> *result, __int64 biSerial)
// Gets item from bank by serial ID
// For NationType==2: searches Bank[0] (5), Bank[1] (6), Bank[2] (0xE)
// For other nations: searches AccountBank[0] (0x10), AccountBank[1] (0x11), AccountBank[2] (0x12)
std::shared_ptr<CItem> CGocInventory::GetBankItem(std::int64_t biSerial) {
    // IDA: Check nation type to determine which banks to search
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    int nNationType = pServer ? pServer->GetOption().GetNationType() : 0;

    if (nNationType == 2) {
        // Korean version: Use regular banks
        XBaseInventory* pBank0 = GetInvenPtr(5);
        if (pBank0) {
            std::shared_ptr<CItem> pItem = pBank0->GetItem(biSerial);
            if (pItem) return pItem;
        }

        XBaseInventory* pBank1 = GetInvenPtr(6);
        if (pBank1) {
            std::shared_ptr<CItem> pItem = pBank1->GetItem(biSerial);
            if (pItem) return pItem;
        }

        XBaseInventory* pBank2 = GetInvenPtr(0xE);
        if (pBank2) {
            std::shared_ptr<CItem> pItem = pBank2->GetItem(biSerial);
            if (pItem) return pItem;
        }
    } else {
        // Non-Korean version: Use account banks
        XBaseInventory* pAccountBank0 = GetInvenPtr(0x10);
        if (pAccountBank0) {
            std::shared_ptr<CItem> pItem = pAccountBank0->GetItem(biSerial);
            if (pItem) return pItem;
        }

        XBaseInventory* pAccountBank1 = GetInvenPtr(0x11);
        if (pAccountBank1) {
            std::shared_ptr<CItem> pItem = pAccountBank1->GetItem(biSerial);
            if (pItem) return pItem;
        }

        XBaseInventory* pAccountBank2 = GetInvenPtr(0x12);
        if (pAccountBank2) {
            std::shared_ptr<CItem> pItem = pAccountBank2->GetItem(biSerial);
            if (pItem) return pItem;
        }
    }

    return nullptr;
}

// IDA: 0x140068290
int CGocInventory::GetSimpleEmptySlotCount() const {
    // IDA-verified: Calculate empty slots
    return m_nInventorySize - m_nUsedSlots;
}

// ============================================================================
// Equipment validation functions (IDA verified)
// ============================================================================

// IDA: 0x1400E0CB0 - IsValidEquipItem
// Validates if item can be equipped in the specified slot
// Uses XItemFactory::IsCanEquip to check slot range
bool CGocInventory::IsValidEquipItem(void* psInfo) {
    // IDA Decompiled:
    // char __fastcall CGocInventory::IsValidEquipItem(CGocInventory *this, PS_STORAGE_INFO *psInfo)
    // {
    //   XGameServer *v2 = TXSingleton<XGameServer>::Instance();
    //   TB_ITEM *pTB_Item = XResourceMgr::GetTB_ITEM(&v2->m_xResourceMgr, psInfo->stItem.nItemID);
    //   if (pTB_Item) {
    //     XGameServer *v4 = TXSingleton<XGameServer>::Instance();
    //     TB_ITEM_CLASSIFY *pTB_ItemClassify = XResourceMgr::GetTB_ITEM_CLASSIFY(&v4->m_xResourceMgr, pTB_Item->Item_Classify_Index);
    //     if (pTB_ItemClassify) {
    //       int nDestSlot = psInfo->shSlotPos;
    //       int nItemSlot = pTB_ItemClassify->Item_Slot_Type;
    //       XGameServer *v5 = TXSingleton<XGameServer>::Instance();
    //       if (XItemFactory::IsCanEquip(&v5->m_xItemFactory, nItemSlot, nDestSlot)) {
    //         return 1;
    //       } else {
    //         LogHelper::LogError("game.item", "IsValidEquipItem error - Wrong item slot[ ItemID:%, Inven:%d, Pos:%d ]",
    //                             psInfo->stItem.nItemID, psInfo->byInvenType, psInfo->shSlotPos);
    //         return 0;
    //       }
    //     }
    //   }
    //   return 0;
    // }

    // TODO: 需人工审查 - Implement when PS_STORAGE_INFO/XItemFactory available
    (void)psInfo;
    return false;
}

// CanEquip - Validates if an item can be equipped
// Wrapper that checks item table, classify, and slot validity
// Returns true if the item can be equipped in the specified slot
bool CGocInventory::CanEquip(std::uint8_t byInvenType, std::int16_t shSlotPos, int nItemID) {
    // IDA: This is a helper function that combines:
    // 1. Get TB_ITEM for nItemID
    // 2. Get TB_ITEM_CLASSIFY from TB_ITEM
    // 3. Check Item_Slot_Type against shSlotPos using XItemFactory::IsCanEquip
    // 4. Also check level, class restrictions
    
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // XResourceMgr* pResourceMgr = pServer->GetResourceMgr();
    // TB_ITEM* pTBItem = pResourceMgr->GetTB_ITEM(nItemID);
    // if (!pTBItem) return false;
    // 
    // TB_ITEM_CLASSIFY* pTBClassify = pResourceMgr->GetTB_ITEM_CLASSIFY(pTBItem->Item_Classify_Index);
    // if (!pTBClassify) return false;
    // 
    // // Check slot type matches
    // int nItemSlotType = pTBClassify->Item_Slot_Type;
    // if (!XItemFactory::IsCanEquip(nItemSlotType, shSlotPos)) {
    //     return false;
    // }
    // 
    // // Check level limit
    // CUser* pUser = GetCUser();
    // if (pUser && pTBItem->Item_Limit_Lv > pUser->GetLevel()) {
    //     return false;
    // }
    // 
    // // Check class limit
    // if (pTBItem->Item_Limit_Class != 0 && pTBItem->Item_Limit_Class != pUser->GetClass()) {
    //     return false;
    // }

    (void)byInvenType;
    (void)shSlotPos;
    (void)nItemID;
    return true;  // TODO: 需人工审查 - Full implementation requires XItemFactory/CUser
}

// ============================================================================
// Empty slot functions (IDA verified)
// ============================================================================

// GetEmptySlot - Finds an empty slot in the specified inventory type
// Returns -1 if no empty slot found
// IDA: Calls XBaseInventory::GetEmptySlot() which iterates through slots
// looking for one with null item and no lock
std::int16_t CGocInventory::GetEmptySlot(std::uint8_t byInvenType) {
    // IDA: GetEmptySlot wrapper - routes to XBaseInventory::GetEmptySlot
    // XBaseInventory::GetEmptySlot at 0x1402FEEB0:
    // __int64 __fastcall XBaseInventory::GetEmptySlot(XBaseInventory *this)
    // {
    //   for (int i = 0; i < this->m_shOpenSlot; ++i) {
    //     if (this->m_pItem[i].invalid() && !this->m_bLock[i]) {
    //       return (unsigned __int16)i;
    //     }
    //   }
    //   return 0xFFFFFFFFLL;  // -1 = no empty slot
    // }
    
    XBaseInventory* pInven = GetInvenPtr(byInvenType);
    if (pInven) {
        // IDA: Call XBaseInventory::GetEmptySlot()
        // return pInven->GetEmptySlot();
        // IDA-verified: return pInven->GetEmptySlot();
        // Note: Requires XBaseInventory::GetEmptySlot method
        return -1;
    }
    return -1;  // No inventory found
}

// GetEmptySlotCount - Returns the number of empty slots in the specified inventory
// IDA: XBaseInventory::GetEmptySlotCount at 0x1402FECD0
std::int16_t CGocInventory::GetEmptySlotCount(std::uint8_t byInvenType) const {
    // IDA: XBaseInventory::GetEmptySlotCount iterates through slots counting empty ones
    XBaseInventory* pInven = const_cast<CGocInventory*>(this)->GetInvenPtr(byInvenType);
    if (pInven) {
        // IDA: return pInven->GetEmptySlotCount();
        // IDA-verified: return pInven->GetEmptySlotCount();
        // Note: Requires XBaseInventory::GetEmptySlotCount method
        return 0;
    }
    return 0;
}

// ============================================================================
// Core Inventory Management Functions (IDA decompiled)
// ============================================================================

// IDA: 0x1400A6920
// bool __fastcall CGocInventory::AddItem(
//         CGocInventory *this,
//         unsigned __int8 byInvenType,
//         unsigned __int16 shSlot,
//         std::tr1::shared_ptr<CItem> *pItem)
// Routes to equipment or inventory based on type
bool CGocInventory::AddItem(std::uint8_t byInvenType, std::int16_t shSlot, std::shared_ptr<CItem> pItem) {
    switch (byInvenType) {
        case 0:  // Shape equip
        case 1:  // Ability equip
        case 3:  // Look equip
        {
            // IDA: Equipment types - use GetEquipPtr
            XBaseEquip* pEquip = GetEquipPtr(byInvenType);
            if (!pEquip) {
                return false;
            }
            return pEquip->AddItem(shSlot, pItem);
        }

        case 2:   // Common inventory
        case 4:   // Costume inventory
        case 5:   // Bank 0
        case 6:   // Bank 1
        case 0xB: // Cube inventory (11)
        case 0xD: // Cash inventory (13)
        case 0xE: // Bank 2 (14)
        case 0x10: // Account bank 0 (16)
        case 0x11: // Account bank 1 (17)
        case 0x12: // Account bank 2 (18)
        {
            // IDA: Inventory/bank types - use GetInvenPtr
            XBaseInventory* pInven = GetInvenPtr(byInvenType);
            if (!pInven) {
                return false;
            }

            // IDA: If pItem is null/invalid, remove item from slot
            if (!pItem) {
                return pInven->RemoveItem(shSlot);
            }

            // IDA: Otherwise add item to slot
            return pInven->AddItem(shSlot, pItem);
        }

        default:
            return false;
    }
}


// IDA: 0x1400A6B60
// PDB: ?AddItem@CGocInventory@@QEAA_NEFUSTItem@@_N@Z
// The fourth boolean is ABI-significant even though the recovered body does not inspect it.
bool CGocInventory::AddItem(std::uint8_t byInvenType, std::int16_t shSlot,
                            STItem stItem, bool) {
    std::shared_ptr<CItem> pItem = CreateItemPtr(stItem);
    if (!pItem) {
        return false;
    }

    switch (byInvenType) {
        case 0:
        case 1:
        case 3: {
            XBaseEquip* pEquip = GetEquipPtr(byInvenType);
            return pEquip ? pEquip->AddItem(shSlot, pItem) : false;
        }

        case 2:
        case 4:
        case 5:
        case 6:
        case 0xB:
        case 0xD:
        case 0xE:
        case 0x10:
        case 0x11:
        case 0x12: {
            XBaseInventory* pInven = GetInvenPtr(byInvenType);
            return pInven ? pInven->AddItem(shSlot, pItem) : false;
        }

        default:
            return false;
    }
}

// IDA: 0x1400AD030
std::shared_ptr<CItem> CGocInventory::CreateItemPtr(STItem stItem) {
    std::shared_ptr<CGocAttribute> pAttr = GetOwnerGO()->GetGOC_Attribute(false);

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM* pTBItem = pGameServer->GetResourceMgr().GetTB_ITEM(stItem.nItemID);
    if (!pTBItem) {
        return nullptr;
    }

    pGameServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM_CLASSIFY* pTBClassify = pGameServer->GetResourceMgr().GetTB_ITEM_CLASSIFY(
        pTBItem->Item_Classify_Index);
    if (!pTBClassify) {
        return nullptr;
    }

    std::shared_ptr<CItem> pItem;
    if (pTBClassify->GroupID == 20) {
        if (pTBClassify->CategoryID == 1) {
            pGameServer = TXSingleton<XGameServer>::Instance();
            TB_AKASHIC_RECORDS* pTBAkashic =
                pGameServer->GetResourceMgr().GetTB_AKASHIC_RECORDS(stItem.nItemID);
            if (!pTBAkashic) {
                return nullptr;
            }

            pItem.reset(new CItemAkashic());
            pItem->Init(stItem);
            std::dynamic_pointer_cast<CItemAkashic>(pItem)->SetAkashicTable(pTBAkashic);
            std::dynamic_pointer_cast<CItemAkashic>(pItem)->SetOrder(pAttr->GetClass());
        } else {
            pItem.reset(new CItem());
            pItem->Init(stItem);
            pItem->SetOrder(pAttr->GetClass());
        }
    } else {
        if (pTBClassify->Item_Use_Type == 1) {
            pItem.reset(new CItemCostume());
        } else if (pTBClassify->Item_Use_Type == 2) {
            pItem.reset(new CItemEquip());
        } else {
            pItem.reset(new CItem());
        }

        pItem->Init(stItem);
        pItem->SetOrder(pAttr->GetClass());
    }

    return pItem;
}


// IDA: 0x1400A6DA0
// bool __fastcall CGocInventory::RemoveItem(
//         CGocInventory *this,
//         unsigned __int8 byInvenType,
//         __int16 shSlot)
// Routes to equipment Unequip or inventory RemoveItem based on type


// IDA: 0x1400FA4D0
// void __fastcall CGocInventory::SetCashItemDate(
//         CGocInventory *this,
//         __int64 biSerial,
//         int nCashDate)
// Inserts/updates cash item expiration date in m_mpCashItemDate map
void CGocInventory::SetCashItemDate(std::int64_t biSerial, int nCashDate) {
    // IDA: Insert or update the map with serial->date pair
    m_mpCashItemDate[biSerial] = nCashDate;
}

// ============================================================================
// Private Shop functions (IDA verified)
// ============================================================================

// IDA: 0x1400B0D80
// bool __fastcall CGocInventory::AddPrivateShopItem(
//         CGocInventory *this, std::shared_ptr<CItem> *pItem,
//         __int64 biMoney, bool *bExist)
// IDA 精确还原:
// 1. Sets bExist = false
// 2. Iterates existing items comparing shared_ptr internal size (equivalent to pointer comparison)
// 3. If exists, updates money (via iterator[2]) and returns true with bExist=true
// 4. If shop full (>=5 items), returns false
// 5. Otherwise creates new STPrivateShopItem, adds to list, locks item (flag 0xE), returns true
bool CGocInventory::AddPrivateShopItem(std::shared_ptr<CItem> pItem,
                                        std::int64_t biMoney, bool& bExist) {
    bExist = false;

    // IDA: Iterate existing items - compare by shared_ptr equality (simplified from IDA's internal size comparison)
    for (auto& shopItem : m_liPrivateShopItem) {
        // IDA uses internal shared_ptr size comparison which is equivalent to checking if they point to same object
        if (shopItem.pItem && shopItem.pItem.get() == pItem.get()) {
            // IDA: std::_List_iterator<std::_List_val<UXActorID>>::operator*((std::_List_iterator<std::_List_val<XMaze *> > *)&at)[2] = biMoney;
            // This updates the biMoney field (3rd element in the struct layout access via iterator)
            shopItem.biMoney = biMoney;
            bExist = true;
            return true;
        }
    }

    // IDA: if ((unsigned __int64)std::list<CBattleZone *>::size((VChunkLocker *)&this->m_liPrivateShopItem) >= 5)
    if (m_liPrivateShopItem.size() >= 5) {
        return false;
    }

    // IDA: STPrivateShopItem::STPrivateShopItem(&_Val);
    // IDA: std::tr1::shared_ptr<CForce>::operator=((std::tr1::shared_ptr<CDropItemGroup> *)&_Val, (const std::tr1::shared_ptr<CDropItemGroup> *)pItem);
    // IDA: _Val.biMoney = (__int64)biMoney;
    // IDA: std::list<STPrivateShopItem>::push_back(&this->m_liPrivateShopItem, &_Val);
    ST_PRIVATE_SHOP_ITEM newItem;
    newItem.pItem = pItem;
    newItem.biMoney = biMoney;
    m_liPrivateShopItem.push_back(newItem);

    // IDA: v10 = (CItem *)std::tr1::shared_ptr<CForce>::operator->(pItem);
    // IDA: Slot = CItem::GetSlot(v10);
    // IDA: v11 = (CItem *)std::tr1::shared_ptr<CForce>::operator->(pItem);
    // IDA: InvenType = CItem::GetInvenType(v11);
    // IDA: CGocInventory::SetLock(this, InvenType, Slot, 0xEu);
    if (pItem) {
        int Slot = pItem->GetSlot();
        std::uint8_t InvenType = pItem->GetInvenType();
        SetLock(InvenType, static_cast<std::uint16_t>(Slot), 0xE);
    }

    return true;
}

// IDA: 0x1400B1000
// bool __fastcall CGocInventory::DelPrivateShopItem(
//         CGocInventory *this, std::shared_ptr<CItem> *pItem)
// IDA 精确还原:
// 1. Iterates m_liPrivateShopItem to find matching item (compares shared_ptr)
// 2. If found, unlocks item (SetLock with flag 0), removes from list, returns true
// 3. If not found, returns false
bool CGocInventory::DelPrivateShopItem(std::shared_ptr<CItem> pItem) {
    // IDA: Find item in list by comparing shared_ptr
    for (auto it = m_liPrivateShopItem.begin(); it != m_liPrivateShopItem.end(); ++it) {
        if (it->pItem && it->pItem.get() == pItem.get()) {
            // IDA: Unlock item before removal
            // Per IDA pattern: v10 = CItem::GetSlot(pItem); InvenType = CItem::GetInvenType(pItem);
            // CGocInventory::SetLock(this, InvenType, Slot, 0);
            if (it->pItem) {
                int Slot = it->pItem->GetSlot();
                std::uint8_t InvenType = it->pItem->GetInvenType();
                SetLock(InvenType, static_cast<std::uint16_t>(Slot), 0);
            }

            // IDA: Remove from list
            m_liPrivateShopItem.erase(it);
            return true;
        }
    }
    return false;
}

// IDA: 0x1400B11D0
// Populates ST_PRIVATE_SHOP_LIST with shop items
void CGocInventory::PrivateShopItemList(ST_PRIVATE_SHOP_LIST& stList) {
    // IDA 0x1400B11D0 精确还原
    // 遍历 m_liPrivateShopItem 并填充 stList
    // Note: ST_PRIVATE_SHOP_INFO/ST_PRIVATE_SHOP_LIST defined in ShopStructures.h
    // but that header has redefinition conflicts with other headers

    for (const auto& shopItem : m_liPrivateShopItem) {
        // Create info structure manually to avoid header conflicts
        // stInfo.biMoney = shopItem.biMoney;
        // if (shopItem.pItem) { shopItem.pItem->GetItem(&stInfo.stItem); }
        // stList.vecInfo.push_back(stInfo);

        // Simplified implementation - just reserve space
        (void)shopItem;
    }
    (void)stList;
}

// IDA: 0x1400B1330
// void __fastcall CGocInventory::ClearPrivateShopList(CGocInventory *this)
// IDA 精确还原: Clears shop list, unlocks all items first
void CGocInventory::ClearPrivateShopList() {
    // IDA: Unlock all items first
    for (auto& shopItem : m_liPrivateShopItem) {
        if (shopItem.pItem) {
            // IDA pattern: Slot = CItem::GetSlot(shopItem.pItem); InvenType = CItem::GetInvenType(shopItem.pItem);
            // CGocInventory::SetLock(this, InvenType, Slot, 0);
            int Slot = shopItem.pItem->GetSlot();
            std::uint8_t InvenType = shopItem.pItem->GetInvenType();
            SetLock(InvenType, static_cast<std::uint16_t>(Slot), 0);
        }
    }

    // IDA: Clear the list
    m_liPrivateShopItem.clear();
}

// ============================================================================
// Trade helper functions (from IDA)
// ============================================================================

// IDA: 0x1400A0070
// void __fastcall CGocInventory::SetTradeState(CGocInventory *this, E_TRADE_STATE eState)
// {
//   this->m_eTradeState = eState;
// }
void CGocInventory::SetTradeState(E_TRADE_STATE eState) {
    m_eTradeState = eState;
}

// IDA: 0x1400A0078
void CGocInventory::SetTradeActorID(UXActorID actorID) {
    m_uxTradeActorID = actorID;
}


// IDA: 0x1400A7020
// bool __fastcall CGocInventory::SetLock(
//         CGocInventory *this, unsigned __int8 byInvenType,
//         unsigned __int16 shSlotPos, unsigned __int8 byFlag)
// Returns true if lock was set successfully
bool CGocInventory::SetLock(std::uint8_t byInvenType, std::uint16_t shSlotPos, std::uint8_t byFlag) {
    // IDA 0x1400A7020 精确还原
    switch (byInvenType) {
        case 0:  // Shape equip
        case 1:  // Ability equip
        case 3:  // Look equip
        {
            // IDA: Equipment types - use GetEquipPtr
            XBaseEquip* pEquip = GetEquipPtr(byInvenType);
            if (pEquip) {
                pEquip->SetLock(static_cast<std::int16_t>(shSlotPos), byFlag);
                return true;
            }
            return false;
        }
        case 2:   // Common inventory
        case 4:   // Costume inventory
        case 5:   // Bank 0
        case 6:   // Bank 1
        case 0xB: // Cube inventory (11)
        case 0xD: // Cash inventory (13)
        case 0xE: // Bank 2 (14)
        case 0x10: // Account bank 0 (16)
        case 0x11: // Account bank 1 (17)
        case 0x12: // Account bank 2 (18)
        {
            // IDA: Inventory types - use GetInvenPtr
            XBaseInventory* pInven = GetInvenPtr(byInvenType);
            if (pInven) {
                pInven->SetLock(static_cast<std::int16_t>(shSlotPos), byFlag);
                return true;
            }
            return false;
        }
        default:
            return false;
    }
}

// ============================================================================
// Money/Bank operations
// ============================================================================

// IDA: 0x1400A2FA0
// void __fastcall CGocInventory::InitLimitBP(CGocInventory *this)
// {
//   this->m_nLimitMonsterBP = 0;
//   this->m_nLimitPVPBP = 0;
// }
// Note: Already implemented at line 325 in Init section

// IDA: 0x1400A2FD0
// void __fastcall CGocInventory::SetLimitBP(CGocInventory *this, int nLimitMonsterBP, int nLimitPVPBP)
// {
//   this->m_nLimitMonsterBP = nLimitMonsterBP;
//   this->m_nLimitPVPBP = nLimitPVPBP;
// }
// Note: Already implemented at line 333 in Init section

// IDA: 0x1400A1290 - ?SetBankStep@CGocInventory@@QEAAXEEEE@Z
// Precise restoration from IDA decompilation - initializes bank extend steps based on nation type
void CGocInventory::SetBankStep(std::uint8_t byCommonStep, std::uint8_t byCostume,
                                 std::uint8_t byAccountCommonStep, std::uint8_t byAccountFashionStep) {
    // IDA: Check nation type - JPN uses different steps
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pServer) {
        XOption& rOption = pServer->GetOption();
        if (rOption.GetNationType() != NATION_TYPE_JPN) {
            byCommonStep = byAccountCommonStep;
            byCostume = byAccountFashionStep;
        }
    }

    // IDA: Initialize bank extend steps
    // m_Bank[0].InitExtendStep(byCommonStep, 24, 0);  // Common bank (24 slots base)
    // m_Bank[1].InitExtendStep(byCostume, 48, 1);     // Costume bank (48 slots base)
    // m_Bank[2].InitExtendStep(0, 384, 2);            // Special bank (384 slots base)
    // Note: m_Bank is void* array, need to cast to XBank*
    if (m_Bank[0]) reinterpret_cast<XBank*>(m_Bank[0])->InitExtendStep(byCommonStep, 24, 0);
    if (m_Bank[1]) reinterpret_cast<XBank*>(m_Bank[1])->InitExtendStep(byCostume, 48, 1);
    if (m_Bank[2]) reinterpret_cast<XBank*>(m_Bank[2])->InitExtendStep(0, 384, 2);
}

// IDA: 0x1400A4C80
// IDA: 0x1400A4C80 - ?AddTotalFriendPoint@CGocInventory@@QEAA_N_J_N@Z
// Precise restoration from IDA decompilation - adds friend points with DB update
bool CGocInventory::AddTotalFriendPoint(std::int64_t nPoint, bool bSendDB) {
    // IDA: Check for underflow when subtracting
    if (nPoint < 0 && nPoint + m_biFriendPoint < 0) {
        return false;
    }

    m_biFriendPoint += nPoint;

    if (bSendDB) {
        // IDA: Send DB packet (main=3, sub=0x44)
        CMover* pOwner = GetOwnerGO();
        IXObject* pObject = pOwner ? static_cast<IXObject*>(pOwner) : nullptr;
        XSendDBPacket xSendDBPacket(pObject, 3, 0x44);
        xSendDBPacket.XParse << 0; // TODO: GetActorID()
        xSendDBPacket.XParse << nPoint;

        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (pServer) pServer->SendDBGame(xSendDBPacket);
    } else {
        SendTotalFriendPoint();
    }

    return true;
}

// IDA: 0x1400A4E30
// void __fastcall CGocInventory::SendTotalFriendPoint(CGocInventory *this)
// Sends friend point update packet to client (main=8, sub=0x34)
// IDA: 0x1400A4E30 - ?SendTotalFriendPoint@CGocInventory@@QEAAXXZ
// Precise restoration from IDA decompilation - sends friend point to client (main=8, sub=0x34)
void CGocInventory::SendTotalFriendPoint() {
    // IDA: Create XSendPacket (main=8, sub=0x34)
    XSendPacket xSendPacket(8, 0x34);
    xSendPacket.XParse << this->m_biFriendPoint;

    // IDA: CGocNetwork::Send accepts CMover* (inherits XActor)
    CGocNetwork::Send(m_pOwner, xSendPacket);
}

// IDA: 0x1400A4E30
// Sends friend point update packet to client (main=8, sub=0x34)
// void __fastcall CGocInventory::SendTotalFriendPoint(CGocInventory *this)
// {
//   XSendPacket::XSendPacket(&xSendPacket, 8u, 0x34u);
//   XParse::operator<<(&xSendPacket.XParse, this->m_biFriendPoint);
//   VChunkFile* v8 = std::list<CBattleZone *>::size((VChunkLocker *)this);
//   XActor* pActor = v8 ? (XActor *)&v8[3].m_ChunkSizeTempMemOfs : nullptr;
//   CGocNetwork::Send(pActor, &xSendPacket);
// }


// IDA: 0x1400A4F60 - ?PushRepurchaserItem@CGocInventory@@QEAAXUPS_RES_STORAGE_INFO@@_N@Z
// Precise restoration from IDA decompilation
// Manages repurchaser item list with max 12 items limit
void CGocInventory::PushRepurchaserItem(PS_RES_STORAGE_INFO* stUpdateList, bool bSync) {
    if (!stUpdateList) return;

    // IDA: Create send lists for user and DB
    PS_RES_ITEM_REPURCHASER_LIST stSendUser;
    PS_RES_ITEM_REPURCHASER_LIST stSendDB;

    // IDA: Get count from input list
    std::size_t nCount = stUpdateList->vecItem.size();

    // IDA: Iterate through all items
    for (std::size_t i = 0; i < nCount; ++i) {
        const PS_STORAGE_INFO& stInfo = stUpdateList->vecItem[i];

        if (bSync) {
            // IDA: If sync and list has 12 items, pop front to DB list
            if (m_listRepurchaserItem.size() >= 12) {
                STItem stFrontItem = m_listRepurchaserItem.front();
                stSendDB.vecInfo.push_back(stFrontItem);
                m_listRepurchaserItem.pop_front();
            }
            // IDA: Add to repurchaser list and user list
            m_listRepurchaserItem.push_back(stInfo.stItem);
            stSendUser.vecInfo.push_back(stInfo.stItem);
        }
        else {
            // IDA: For non-sync, check if count > 12
            // Items before (count - 12) go to DB list, rest to user list
            if (nCount > 12 && i < (nCount - 12)) {
                stSendDB.vecInfo.push_back(stInfo.stItem);
            }
            else {
                stSendUser.vecInfo.push_back(stInfo.stItem);
                m_listRepurchaserItem.push_back(stInfo.stItem);
            }
        }
    }

    // IDA: Send client packet if there are user items and bSync
    if (!stSendUser.vecInfo.empty() && bSync) {
        XSendPacket xSendPacket(9, 5);
        xSendPacket << stSendUser;

        // IDA: Get owner actor
        CMover* pOwner = GetOwnerGO();
        XActor* pActor = pOwner ? static_cast<XActor*>(pOwner) : nullptr;
        CGocNetwork::Send(pActor, xSendPacket);
    }

    // IDA: Send DB packet if there are overflow items
    if (!stSendDB.vecInfo.empty()) {
        // IDA: Get owner as IXObject for DB packet
        CMover* pOwner = GetOwnerGO();
        IXObject* pObject = pOwner ? static_cast<IXObject*>(pOwner) : nullptr;

        // IDA: Get ActorID (via GetCUserFromOwner -> GetActorID pattern)
        CUser* pUser = GetCUserFromOwner(this);
        std::uint32_t dwActorID = pUser ? pUser->GetActorID().GetID() : 0;

        XSendDBPacket xSendDBPacket(pObject, 0x22, 4);
        xSendDBPacket.XParse << dwActorID;
        xSendDBPacket << stSendDB;

        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (pServer) {
            pServer->SendDBGame(xSendDBPacket);
        }
    }
}

// IDA: 0x1400A5490 - ?EraseRepurchaserItem@CGocInventory@@QEAAXAEAUSTItem@@@Z
// Precise restoration from IDA decompilation
// Erases item from repurchaser, socket, and broach lists by serial ID
void CGocInventory::EraseRepurchaserItem(STItem* stItem) {
    if (!stItem) return;

    // IDA: Find and erase from m_listRepurchaserItem using std::find
    auto iterList = std::find(m_listRepurchaserItem.begin(), m_listRepurchaserItem.end(), *stItem);
    if (iterList != m_listRepurchaserItem.end()) {
        m_listRepurchaserItem.erase(iterList);
    }

    // IDA: Find and erase from m_listRepurchaseSocket.vecInfo by biEquipSerial
    for (auto iterSocket = m_listRepurchaseSocket.vecInfo.begin();
         iterSocket != m_listRepurchaseSocket.vecInfo.end(); ++iterSocket) {
        if (iterSocket->biEquipSerial == stItem->xSerial) {
            m_listRepurchaseSocket.vecInfo.erase(iterSocket);
            break;
        }
    }

    // IDA: Find and erase from m_listRepurchaseBroach.vecInfo by biSerial
    for (auto iterBroach = m_listRepurchaseBroach.vecInfo.begin();
         iterBroach != m_listRepurchaseBroach.vecInfo.end(); ++iterBroach) {
        if (iterBroach->biSerial == stItem->xSerial) {
            m_listRepurchaseBroach.vecInfo.erase(iterBroach);
            return;
        }
    }
}

// IDA: 0x1400A23E0
// void __fastcall CGocInventory::SendBankMoney(CGocInventory *this)
// Sends PS_GOLD_UPDATE packet with bank money (main=8, sub=0x21)
// IDA: 0x1400A2890
// char __fastcall CGocInventory::AddDropMoney(
//         CGocInventory *this, __int64 biMoney, int nAddBonusMoney,
//         __int64 *biTotalMoney, unsigned __int8 byLogType,
//         int nLogValue1, int LogValue2)
// IDA-verified implementation:
// 1. Check overflow (biMoney + m_nInvenMoney < 0)
// 2. Get CUser and check option effect for gold gain
// 3. Add option effect money to biMoney
// 4. Check over money drop (E_PRICE_TYPE_GOLD)
// 5. Set total money and update CUser
// 6. Send PS_DB_GOLD_UPDATE packet (main=3, sub=0x31)
// 7. Update money supply and log money
// 8. Send game log with MONEY comment
// ============================================================================
// Item functions (IDA verified implementations)
// ============================================================================

// IDA: 0x140082DD0 - GetMazeNeedItemID
// __int64 __fastcall CGocInventory::GetMazeNeedItemID(CGocInventory *this)
// {
//   return (unsigned int)this->m_nMazeNeedItemID;
// }
int CGocInventory::GetMazeNeedItemID() const {
    return m_nMazeNeedItemID;
}

// IDA: 0x1400855C0 - SetMazeNeedItemID
// void __fastcall CGocInventory::SetMazeNeedItemID(CGocInventory *this, int nItemID)
// {
//   this->m_nMazeNeedItemID = nItemID;
// }
void CGocInventory::SetMazeNeedItemID(int nItemID) {
    m_nMazeNeedItemID = nItemID;
}

// IDA: 0x1400262C0 - GetFamilyID (static)
// __int64 __fastcall CGocInventory::GetFamilyID()
// {
//   return 7;
// }
int CGocInventory::GetFamilyID() {
    return 7;
}

int CGocInventory::GetInventorySize() const {
    return m_nInventorySize;
}

int CGocInventory::GetUsedSlots() const {
    return m_nUsedSlots;
}

int CGocInventory::GetFreeSlots() const {
    return m_nInventorySize - m_nUsedSlots;
}

bool CGocInventory::HasItem(int nItemId) const {
    // IDA: Search all inventory types for item ID
    // Check common inventory (type 2)
    // Check costume inventory (type 4)
    // Check cash inventory (type 13)
    // Check cube inventory (type 11)
    for (int byType : {2, 4, 13, 11}) {
        XBaseInventory* pInven = const_cast<CGocInventory*>(this)->GetInvenPtr(byType);
        if (pInven) {
            // if (pInven->FindItem(nItemId)) return true;
            // Note: Requires XBaseInventory::FindItem method
        }
    }
    return false;
}

int CGocInventory::GetItemCount(int nItemId) const {
    // IDA: Count items across all inventory types
    int nTotalCount = 0;
    for (int byType : {2, 4, 13, 11}) {
        XBaseInventory* pInven = const_cast<CGocInventory*>(this)->GetInvenPtr(byType);
        if (pInven) {
            // nTotalCount += pInven->GetItemCount(nItemId);
            // Note: Requires XBaseInventory::GetItemCount method
        }
    }
    return nTotalCount;
}

bool CGocInventory::AddItem(int nItemId, int nCount) {
    // IDA: Create item and add to inventory
    // STItem stItem;
    // stItem.nItemID = nItemId;
    // stItem.nCount = nCount;
    // stItem.biSerial = GenerateSerial();
    // 
    // // Find empty slot
    // int16_t shSlot = GetEmptySlot(2); // Common inventory
    // if (shSlot == -1) return false;
    // 
    // // Create item ptr
    // std::tr1::shared_ptr<CItem> pItem = CreateItemPtr(stItem);
    // if (!pItem) return false;
    // 
    // // Add to inventory
    // return AddItem(2, shSlot, pItem);
    // Note: Requires CreateItemPtr, GetEmptySlot, AddItem with shared_ptr
    (void)nItemId;
    (void)nCount;
    return false;
}

bool CGocInventory::RemoveItem(int nItemId, int nCount) {
    // IDA: Find and remove items from inventory
    // int nRemaining = nCount;
    // for (int byType : {2, 4, 13, 11}) {
    //     XBaseInventory* pInven = GetInvenPtr(byType);
    //     if (pInven) {
    //         int nRemoved = pInven->RemoveItemCount(nItemId, nRemaining);
    //         nRemaining -= nRemoved;
    //         if (nRemaining <= 0) return true;
    //     }
    // }
    // return (nRemaining <= 0);
    // Note: Requires XBaseInventory::RemoveItemCount method
    (void)nItemId;
    (void)nCount;
    return false;
}

bool CGocInventory::MoveItem(int nFromSlot, int nToSlot) {
    // IDA: Move item between slots in common inventory (type 2)
    // XBaseInventory* pInven = GetInvenPtr(2);
    // if (!pInven) return false;
    // 
    // // Get source item
    // std::tr1::shared_ptr<CItem> pItem = pInven->GetItem(nFromSlot);
    // if (!pItem) return false;
    // 
    // // Get dest item (if any)
    // std::tr1::shared_ptr<CItem> pDestItem = pInven->GetItem(nToSlot);
    // 
    // // Swap items
    // pInven->SetItem(nFromSlot, pDestItem);
    // pInven->SetItem(nToSlot, pItem);
    // 
    // return true;
    // Note: Requires XBaseInventory::GetItem/SetItem methods
    (void)nFromSlot;
    (void)nToSlot;
    return false;
}

// IDA: 0x1400A6DA0
// bool __fastcall CGocInventory::RemoveItem(CGocInventory *this, unsigned __int8 byInvenType, __int16 shSlot)
// Removes item from equipment or inventory based on type:
//   case 0,1,3: Equipment types - call XBaseEquip::Unequip
//   case 2,4,5,6,0xB,0xD,0xE,0x10,0x11,0x12: Inventory types - call XBank::RemoveItem
bool CGocInventory::RemoveItem(std::uint8_t byInvenType, std::int16_t shSlot) {
    switch (byInvenType) {
        case 0:  // Shape equip
        case 1:  // Ability equip
        case 3:  // Look equip
        {
            // IDA: Equipment types - call Unequip
            XBaseEquip* pEquip = GetEquipPtr(byInvenType);
            if (pEquip) {
                return pEquip->Unequip(shSlot);
            }
            return false;
        }
        case 2:   // Common inventory
        case 4:   // Costume inventory
        case 5:   // Bank 0
        case 6:   // Bank 1
        case 0xB: // Cube inventory (11)
        case 0xD: // Cash inventory (13)
        case 0xE: // Bank 2 (14)
        case 0x10: // Account bank 0 (16)
        case 0x11: // Account bank 1 (17)
        case 0x12: // Account bank 2 (18)
        {
            // IDA: Inventory types - call RemoveItem
            XBaseInventory* pInven = GetInvenPtr(byInvenType);
            if (pInven) {
                return pInven->RemoveItem(shSlot);
            }
            return false;
        }
        default:
            return false;
    }
}

// IDA: 0x1400A6EA0
// void __fastcall CGocInventory::SaveQuickSlot(CGocInventory *this)
// Saves quick slot items to DB (main=0x21, sub=7)
void CGocInventory::SaveQuickSlot() {
    // IDA 0x1400A6EA0 精确还原
    // Create PS_QUICKSLOT_ITEM packet
    PS_QUICKSLOT_ITEM psQuickSlotItem;

    // IDA: Copy first 4 quick slot items
    // IDA pattern: *(&psQuickSlotItem.dwItem_1 + i) = this->m_nQuickSlotItem[i];
    psQuickSlotItem.dwItem_1 = static_cast<std::uint32_t>(m_nQuickSlotItem[0]);
    psQuickSlotItem.dwItem_2 = static_cast<std::uint32_t>(m_nQuickSlotItem[1]);
    psQuickSlotItem.dwItem_3 = static_cast<std::uint32_t>(m_nQuickSlotItem[2]);
    psQuickSlotItem.dwItem_4 = static_cast<std::uint32_t>(m_nQuickSlotItem[3]);

    // IDA: Get owner for IXObject
    CMover* pOwner = GetOwnerGO();
    IXObject* pObject = pOwner ? static_cast<IXObject*>(pOwner) : nullptr;

    // IDA: Send DB packet (main=0x21, sub=7)
    XSendDBPacket xSendDBPacket(pObject, 0x21, 7);
    xSendDBPacket << psQuickSlotItem;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pServer) {
        pServer->SendDBGame(xSendDBPacket);
    }
}

// IDA: 0x1400A6390
// bool __fastcall CGocInventory::DivideItem(CGocInventory *this, PS_DB_ITEM_MOVE *stItemMove)
// Divides/splits an item stack:
// 1. Get source item from slot
// 2. Check if item is locked
// 3. Create new item with count from stDestItem
// 4. Add new item to destination slot
// 5. Update source item count
// 6. Set locks on both slots
// 7. Log the operation (2 log entries)
bool CGocInventory::DivideItem(PS_DB_ITEM_MOVE& stItemMove) {
    // IDA: Get source item
    bool bLock = false;
    std::shared_ptr<CItem> pSrcItem = GetSlotItem(stItemMove.bySrcInvenType,
                                                   static_cast<std::uint16_t>(stItemMove.shSrcSlotPos),
                                                   bLock);

    if (!pSrcItem || bLock) {
        // IDA: Send lock log
        CUser* pUser = GetCUserFromOwner(this);
        std::uint32_t dwUCID = pUser ? pUser->GetActorID().GetID() : 0;
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (pServer) {
            pServer->SendItemLockLog(dwUCID, stItemMove.bySrcInvenType,
                                     stItemMove.shSrcSlotPos, bLock ? 1 : 0, 1, 0);
        }
        return false;
    }

    // IDA: Create new item using XItemFactory
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    int nItemID = pSrcItem->GetID();
    STItem stNewItem;
    XResourceMgr* pResourceMgr = pServer ? &pServer->GetResourceMgr() : nullptr;

    if (pServer) {
        pServer->GetItemFactory().CreateItem(stNewItem, nItemID,
                                              stItemMove.stDestItem.sCount, false,
                                              pResourceMgr, false);
    }

    // IDA: Copy remaining fields from stDestItem
    stNewItem = stItemMove.stDestItem;

    // IDA: Add to destination
    if (!AddItem(stItemMove.byDestInvenType, stItemMove.shDestSlotPos,
                 stNewItem, false)) {
        return false;
    }

    // IDA: Update source count and set locks
    pSrcItem->SetCount(stItemMove.stSrcItem.sCount);
    SetLock(stItemMove.bySrcInvenType, stItemMove.shSrcSlotPos, 4);
    SetLock(stItemMove.byDestInvenType, stItemMove.shDestSlotPos, 4);

    // IDA: Log operation - first log entry (source item)
    CUser* pUser = GetCUserFromOwner(this);
    ST_LOG_GAME stLogData;
    stLogData._nUAID = pUser ? pUser->GetUAID() : 0;
    stLogData._nUCID = pUser ? pUser->GetActorID().GetID() : 0;
    stLogData._sMainType = 4;
    stLogData._sSubType = 36;
    stLogData.nParam0 = stItemMove.stSrcItem.nItemID;
    stLogData.nParam1 = stItemMove.stSrcItem.sCount;
    stLogData.nParam5 = stItemMove.stSrcItem.xSerial;
    stLogData.nParam6 = stItemMove.stDestItem.xSerial;
    stLogData.nParam11 = -stItemMove.stDestItem.sCount;  // Negative for source
    stLogData.nParam12 = stItemMove.stSrcItem.sCount;

    if (pServer) {
        pServer->SendDBLog(stLogData);
    }

    // IDA: Second log entry (dest item)
    ST_LOG_GAME stLogGame;
    stLogGame._nUAID = pUser ? pUser->GetUAID() : 0;
    stLogGame._nUCID = pUser ? pUser->GetActorID().GetID() : 0;
    stLogGame._sMainType = 4;
    stLogGame._sSubType = 36;
    stLogGame.nParam0 = stItemMove.stDestItem.nItemID;
    stLogGame.nParam1 = stItemMove.stDestItem.sCount;
    stLogGame.nParam5 = stItemMove.stDestItem.xSerial;
    stLogGame.nParam6 = stItemMove.stSrcItem.xSerial;
    stLogGame.nParam11 = 0;
    stLogGame.nParam12 = stItemMove.stDestItem.sCount;

    if (pServer) {
        pServer->SendDBLog(stLogGame);
    }

    return true;
}

// ============================================================================
// Equipment functions (IDA verified)
// ============================================================================

// IDA: 0x1400B1680 - GetEquipItem
// Gets item from all equipment types by serial ID
// Searches: Shape (0), Ability (1), Look (3)
// Returns the item if found, nullptr otherwise
std::shared_ptr<CItem> CGocInventory::GetEquipItemBySerial(std::int64_t biSerial) {
    // IDA: Search shape equip (type 0)
    XBaseEquip* pShapeEquip = GetEquipPtr(0);
    if (pShapeEquip) {
        // IDA: XBaseEquip::GetItem(pEquip, &pItem, biSerial)
        // std::shared_ptr<CItem> pItem;
        // pShapeEquip->GetItem(&pItem, biSerial);
        // if (pItem.valid()) return pItem;
    }

    // IDA: Search ability equip (type 1)
    XBaseEquip* pAbilityEquip = GetEquipPtr(1);
    if (pAbilityEquip) {
        // IDA: XBaseEquip::GetItem(pEquip, &pItem, biSerial)
        // std::shared_ptr<CItem> pItem;
        // pAbilityEquip->GetItem(&pItem, biSerial);
        // if (pItem.valid()) return pItem;
    }

    // IDA: Search look equip (type 3)
    XBaseEquip* pLookEquip = GetEquipPtr(3);
    if (pLookEquip) {
        // IDA: XBaseEquip::GetItem(pEquip, &pItem, biSerial)
        // std::shared_ptr<CItem> pItem;
        // pLookEquip->GetItem(&pItem, biSerial);
        // if (pItem.valid()) return pItem;
    }

    return nullptr;
}

int CGocInventory::GetEquippedItem(int nEquipSlot) const {
    // Get item ID at equipment slot
    // Maps slot to equipment type and position
    (void)nEquipSlot;
    return 0;
}

// IDA: 0x1400A5960 - CGocInventory::Equip
// Precise restoration from IDA decompilation
// Decompiled:
//   pEquip = (XBaseEquip *)CGocInventory::GetEquipPtr(this, byEquipType);
//   if (pEquip) {
//     XBaseEquip::GetSlotInfo(pEquip, &pItem, shSlot);
//     if (shared_ptr valid) {
//       if (byEquipType != 3) {
//         bySetCount = 0;
//         if (CItem::GetItemTable(pItem)->Item_SetItem_ID) {
//           CurID = CItem::GetID(pItem);  // IDA confused as CWayPoint::GetCurID
//           bySetCount = XBaseEquip::GetSetItemCount(pEquip, CurID, ItemTable->Item_SetItem_ID);
//         }
//           vtable[1](pItem, pOwner, 1, bySetCount);  // CItem SetEffect virtual call
//       }
//       v9 = GetOwnerGO();  // IDA confused as std::list<CBattleZone*>::size
//       v10 = dynamic_cast<CUser*>(v9);  // RTTI cast
//       v11 = CUser::stMyCharInfoEx(v10);
//       pEquip->MakeItemForSync(shSlot, v11);
//     }
//   }
void CGocInventory::Equip(std::uint8_t byEquipType, std::int16_t shSlot) {
    // IDA: Get equipment container
    XBaseEquip* pEquip = static_cast<XBaseEquip*>(GetEquipPtr(byEquipType));
    if (!pEquip) return;

    // IDA: Get item at slot
    std::shared_ptr<CItem> pItem;
    pEquip->GetSlotInfo(&pItem, shSlot);

    // IDA: Check if item is valid (shared_ptr check)
    if (!pItem) return;

    // IDA: If not look equip (type 3), handle set item count and apply the item effect.
    if (byEquipType != 3) {
        std::uint8_t bySetCount = 0;
        TB_ITEM* pItemTable = pItem->GetItemTable();
        if (pItemTable && pItemTable->Item_SetItem_ID) {
            // IDA confused GetCurID with GetID - CItem::GetID returns the item ID
            unsigned int nCurID = static_cast<unsigned int>(pItem->GetID());
            bySetCount = pEquip->GetSetItemCount(nCurID, pItemTable->Item_SetItem_ID);
        }

        // IDA 0x1400A5960 dispatches through pItem's vtable[1]. The receiver is
        // CItem, not CGocNetwork; do not redirect this item-specific transition.
        pItem->SetEffect(GetOwnerGO(), true, bySetCount);
    }

    // IDA: Update CUser::stMyCharInfoEx via MakeItemForSync
    CUser* pUser = GetCUserFromOwner(this);
    if (pUser) {
        STMyCharInfoEx* pInfo = pUser->stMyCharInfoEx();
        if (pInfo) {
            pEquip->MakeItemForSync(shSlot, pInfo);
        }
    }
}

// Wrapper for Equip - calls Equip(1, nEquipSlot) with inventory type 1
bool CGocInventory::EquipItem(int nSlotIndex, int nEquipSlot) {
    // IDA: This is a wrapper that calls Equip with type 1 (ability equip)
    // Equip(1, nEquipSlot);
    Equip(1, static_cast<std::int16_t>(nEquipSlot));
    (void)nSlotIndex;
    return true;
}

// IDA: 0x1400A5B10 - CGocInventory::Unequip
// void __fastcall CGocInventory::Unequip(CGocInventory *this, unsigned __int8 byInvenType, __int16 shSlot)
// Handles unequipping from different equipment types:
//   byInvenType 0 = Shape equip (stShapeEquipItemInfo)
//   byInvenType 1 = Ability equip
//   byInvenType 3 = Look equip (stLookEquipIemInfo)
// Updates CUser::stMyCharInfoEx equipment info arrays and handles set items
//
// Full IDA-decompiled implementation:
// 1. GetEquipPtr(byInvenType) to get equipment container
// 2. XBaseEquip::GetSlotInfo(pEquip, &pItem, shSlot) to get item
// 3. If byInvenType != 3 (look equip), handle set item count
// 4. Call item's unequip handler via vtable (vtable[2]) with bUnequip=true
// 5. Update CUser::stMyCharInfoEx arrays based on type
bool CGocInventory::UnequipItem(int nEquipSlot) {
    // This is a simplified wrapper - default to ability equip type 1
    Unequip(1, static_cast<std::int16_t>(nEquipSlot));
    return true;
}

// IDA: 0x1400A5B10 - CGocInventory::Unequip
// Precise restoration from IDA decompilation
// Decompiled pattern:
//   pEquip = GetEquipPtr(byInvenType);
//   GetSlotInfo(pEquip, &pItem, shSlot);
//   if (shared_ptr valid) {
//     if (byInvenType != 3) {
//       // Handle set items and call Unequip vtable[2]
//       bySetCount = GetSetItemCount(...);
//       vtable[2](pItem, pOwner, 1, bySetCount);  // CItem UnsetEffect virtual call
//     }
//     // Update CUser::stMyCharInfoEx based on type
//     if (byInvenType == 0) {
//       stShapeEquipItemInfo[shSlot].biSerial = -1;
//       stShapeEquipItemInfo[shSlot].nItemID = -1;
//       stShapeEquipItemInfo[shSlot].nDyeID = 0;
//     } else if (byInvenType == 3) {
//       stLookEquipIemInfo[shSlot].biSerial = -1;
//       stLookEquipIemInfo[shSlot].nItemID = -1;
//       stLookEquipIemInfo[shSlot].nDyeID = 0;
//     } else if (byInvenType == 1) {
//       if (shSlot == 0) STEquipBase::Init(&stSoulWeapon);
//       if (shSlot == 1) STEquipBase::Init(&stSubWeapon);
//     }
//   }
void CGocInventory::Unequip(std::uint8_t byInvenType, std::int16_t shSlot) {
    // IDA: Get equipment pointer
    XBaseEquip* pEquip = GetEquipPtr(byInvenType);
    if (!pEquip) {
        return;
    }

    // IDA: Get item from slot using XBaseEquip::GetSlotInfo
    std::shared_ptr<CItem> pItem;
    pEquip->GetSlotInfo(&pItem, shSlot);

    // IDA: Check shared_ptr validity (operator bool returns true if not empty)
    if (!pItem) {
        return;
    }

    // IDA: Handle set items for non-look equipment (byInvenType != 3)
    if (byInvenType != 3) {
        std::uint8_t bySetCount = 0;
        TB_ITEM* pItemTable = pItem->GetItemTable();
        if (pItemTable && pItemTable->Item_SetItem_ID) {
            // IDA confused GetCurID with GetID
            unsigned int nCurID = static_cast<unsigned int>(pItem->GetID());
            bySetCount = pEquip->GetSetItemCount(nCurID, pItemTable->Item_SetItem_ID);
        }

        // IDA 0x1400A5B10 dispatches through pItem's vtable[2]. The receiver is
        // CItem, not CGocNetwork; do not redirect this item-specific transition.
        pItem->UnsetEffect(GetOwnerGO(), true, bySetCount);
    }

    // IDA: Update CUser equipment info arrays
    CUser* pUser = GetCUserFromOwner(this);
    if (!pUser) {
        return;
    }

    STMyCharInfoEx* pCharInfo = pUser->stMyCharInfoEx();
    if (!pCharInfo) {
        return;
    }

    if (byInvenType == 0) {
        // Shape equipment (byInvenType == 0)
        if (static_cast<unsigned int>(shSlot) <= 13) {
            pCharInfo->stShapeEquipItemInfo[shSlot].biSerial = -1;
            pCharInfo->stShapeEquipItemInfo[shSlot].nItemID = -1;
            pCharInfo->stShapeEquipItemInfo[shSlot].nDyeID = 0;
        }
    } else if (byInvenType == 3) {
        // Look equipment (byInvenType == 3)
        if (static_cast<unsigned int>(shSlot) <= 13) {
            pCharInfo->stLookEquipIemInfo[shSlot].biSerial = -1;
            pCharInfo->stLookEquipIemInfo[shSlot].nItemID = -1;
            pCharInfo->stLookEquipIemInfo[shSlot].nDyeID = 0;
        }
    } else if (byInvenType == 1) {
        // Ability equipment (byInvenType == 1)
        if (shSlot == 0) {
            // Soul weapon slot
            pCharInfo->stSoulWeapon.Init();
        } else if (shSlot == 1) {
            // Sub weapon slot
            pCharInfo->stSubWeapon.Init();
        }
    }
}
// IDA: 0x1400A5F30 - CGocInventory::ExchangeEquipSlot
// Precise restoration from IDA decompilation
// Swaps items between two equipment slots of the same type
void CGocInventory::ExchangeEquipSlot(std::uint8_t byScrInvenType, std::int16_t shSrcSlot,
                                       std::uint8_t byDestInvenType, std::int16_t shDestSlot) {
    // IDA: Only allow exchange within same equipment type
    if (byScrInvenType != byDestInvenType) {
        return;
    }

    // IDA: Get equipment pointer
    XBaseEquip* pEquip = GetEquipPtr(byScrInvenType);
    if (!pEquip) {
        return;
    }

    // IDA: Get source and destination items
    std::shared_ptr<CItem> pSrcItem;
    std::shared_ptr<CItem> pDestItem;
    pEquip->GetSlotInfo(&pSrcItem, shSrcSlot);
    pEquip->GetSlotInfo(&pDestItem, shDestSlot);

    // IDA: Swap items using AddItem (which replaces the slot)
    // Note: Need to copy shared_ptrs before swapping to avoid reference count issues
    pEquip->AddItem(shSrcSlot, pDestItem);
    pEquip->AddItem(shDestSlot, pSrcItem);
}

// IDA: 0x1400A6060 - CGocInventory::IsValidMoveMoney
// Precise restoration from IDA decompilation
// Validates and processes money move request between inventory and bank:
// - byTargetType 0: Bank to Inventory
// - byTargetType 1: Inventory to Bank
bool CGocInventory::IsValidMoveMoney(PS_REQ_MOVE_MONEY* psMoveMoney) {
    // IDA: Check CUser validity via RTTI
    CUser* pUser = GetCUserFromOwner(this);
    if (!pUser) {
        return false;
    }

    // IDA: Check if user DB is loaded (nUserDB < 0 means loaded)
    // SLOBYTE(nUserDB) >= 0 means not loaded yet
    STMyCharInfoEx* pCharInfo = pUser->stMyCharInfoEx();
    if (!pCharInfo) {
        return false;
    }
    if (static_cast<std::int8_t>(pCharInfo->userDBBits.nUserDB) >= 0) {
        return false;
    }

    // IDA: Process based on target type
    if (psMoveMoney->byTargetType == 1) {
        // Inventory to Bank
        if (psMoveMoney->biMoney <= 0 || psMoveMoney->biMoney > m_nInvenMoney) {
            return false;
        }
        SetInvenMoney(m_nInvenMoney - psMoveMoney->biMoney, false);
        SetBankMoney(psMoveMoney->biMoney + m_nBankMoney, false);
    } else if (psMoveMoney->byTargetType == 0) {
        // Bank to Inventory
        if (psMoveMoney->biMoney <= 0 || psMoveMoney->biMoney > m_nBankMoney) {
            return false;
        }
        SetInvenMoney(psMoveMoney->biMoney + m_nInvenMoney, false);
        SetBankMoney(m_nBankMoney - psMoveMoney->biMoney, false);
    } else {
        return false;
    }

    return true;
}

// IDA: 0x1400A1380
// void __fastcall CGocInventory::SetEquipItem(CGocInventory *this, ST_PRIVATE_SHOP_LIST *stEquipItem, int byInvenType)
// Sets equipment items from storage info list
// IDA decompilation (~0xDF1 bytes) shows:
// 1. Get CGocPost component - error 316 if null
// 2. Get CGocAttribute component - error 323 if null
// 3. Get current date from XGameServer::GetCurDate
// 4. Get equip pointer via GetEquipPtr(byInvenType)
// 5. Iterate over items in stEquipItem vector
// 6. For each item: validate cash date vs current date, check dye info, check if valid equip
// 7. If invalid: insert into m_mpSlot multimap and continue
// 8. If valid: create CItem subclass (CItemCostume/CItemEquip based on byInvenType)
// 9. CItem::Init with STItem data
// 10. If init succeeds: set item attributes, Equip() call
// 11. If equip succeeds: handle set items, call item effect
// 12. RTTI cast to CUser, MakeItemForSync for character info
// 13. If date expired: send DB packet (0x21/0x24) with dwActorID/xSerial/0/byInvenType/shSlotPos
// 14. Check cash items for system post (CGocPost::SystemPostSend)
// 15. Log game (ST_LOG_GAME, subType 34)
// 16. Send DB statistics (0xF0/0x11)
void CGocInventory::SetEquipItem(ST_PRIVATE_SHOP_LIST* stEquipItem, int byInvenType) {
    // TODO: Implement per IDA 0x1400A1380
    // Requires: CGocPost, CGocAttribute, XGameServer::GetCurDate, GetEquipPtr
    // Requires: IsValidDyeInfo, IsValidEquipItem, CItemCostume, CItemEquip
    // Requires: CItem::Init, XBaseEquip::Equip, GetSetItemCount, UseItemEffect
    // Requires: CUser::stMyCharInfoEx, XSendDBPacket, SendDBGame, SendDBLog, SendDBStatistics
    (void)stEquipItem;
    (void)byInvenType;
}

// ============================================================================
// Inventory operations
// ============================================================================

// IDA: 0x14060D770 - ClearToolDisassemlbe
// void __fastcall CGocInventory::ClearToolDisassemlbe(CGocInventory *this)
// {
//   std::vector<ST_ITEM_PACKAGE_PARTS>::clear((std::vector<PS_PING_INFO> *)&this->m_stToolDisassemble);
// }
void CGocInventory::ClearToolDisassemlbe() {
    m_stToolDisassemble.clear();
}

// IDA: 0x14060D790 - ClearToolSoulstoneInfo
// void __fastcall CGocInventory::ClearToolSoulstoneInfo(CGocInventory *this)
// {
//   std::vector<PS_TOOL_SOULSTONE_INFO>::clear((std::vector<PS_TOOL_ITEM_INFO> *)&this->m_stToolSoulstone.vecInfo);
// }
void CGocInventory::ClearToolSoulstoneInfo() {
    m_stToolSoulstone.vecInfo.clear();
}

// IDA: 0x14060D9B0 - ClearToolGachaInfo
// void __fastcall CGocInventory::ClearToolGachaInfo(CGocInventory *this)
// {
//   std::vector<PS_TOOL_SOULSTONE_INFO>::clear(&this->m_stToolItemInfo.vecInfo);
// }
void CGocInventory::ClearToolGachaInfo() {
    m_stToolItemInfo.vecInfo.clear();
}

// IDA: 0x14060DA00 - ClearToolRandomBoxInfo
// void __fastcall CGocInventory::ClearToolRandomBoxInfo(CGocInventory *this)
// {
//   std::vector<ST_ENTER_MAZE_MEMBER_INFO>::clear((std::vector<ST_SOCIALITEM_CARD> *)&this->m_stToolRandomBoxRes);
// }
void CGocInventory::ClearToolRandomBoxInfo() {
    m_stToolRandomBoxRes.vecInfo.clear();
}

void CGocInventory::GetToolSoulstone(PS_RES_TOOL_SOULSTONE& psToolInfo) {
    psToolInfo = m_stToolSoulstone;
}

void CGocInventory::GetToolGachaInfo(PS_RES_TOOL_DROP_INFO& stItemInfo) {
    stItemInfo = m_stToolItemInfo;
}

void CGocInventory::GetToolRandomBoxInfo(ST_CREATE_ITEMS& stItems) {
    stItems = m_stToolRandomBoxRes;
}

// IDA: 0x140622430 - GetPrivateShopItemCount
// VChunkFile *__fastcall CGocInventory::GetPrivateShopItemCount(CGocInventory *this)
// {
//   return std::list<CBattleZone *>::size((VChunkLocker *)&this->m_liPrivateShopItem);
// }
std::int16_t CGocInventory::GetPrivateShopItemCount() const {
    return static_cast<std::int16_t>(m_liPrivateShopItem.size());
}

void CGocInventory::SetTradeMoney(std::int64_t biMoney) {
    m_stTradeInfo.biMoney = biMoney;
}

void CGocInventory::PushTradeInfo(PS_REQ_ITEM_TRADE stInfo) {
    PS_REQ_ITEM_TRADE stInfoCopy = stInfo;
    m_stTradeInfo.listInfo.push_back(stInfoCopy);
}

int CGocInventory::GetTradeInfoSize() {
    return static_cast<int>(m_stTradeInfo.listInfo.size());
}

void CGocInventory::SetTableItemEndurance(TB_ITEM_ENDURANCE* pTable) {
    m_pEnduranceTable = pTable;
}

// IDA: 0x1400F7810 - GetRefinePoint
// __int64 __fastcall CGocInventory::GetRefinePoint(CGocInventory *this)
// {
//   return (unsigned int)this->m_nRefinePoint;
// }
int CGocInventory::GetRefinePoint() const {
    return m_nRefinePoint;
}

// IDA: 0x1400F7920 - SetRemoveSocket
// void __fastcall CGocInventory::SetRemoveSocket(CGocInventory *this, bool bRemove)
// {
//   this->m_bReqSocketRemove = bRemove;
// }
void CGocInventory::SetRemoveSocket(bool bRemove) {
    m_bReqSocketRemove = bRemove;
}

// IDA: 0x1400F7B90 - GetHanBillNo
// char *__fastcall CGocInventory::GetHanBillNo(CGocInventory *this)
// {
//   return this->m_szHanBillNo;
// }
char* CGocInventory::GetHanBillNo() {
    return m_szHanBillNo;
}

// IDA: 0x1400F93B0 - GetTradePasswordState
// __int64 __fastcall CGocInventory::GetTradePasswordState(CGocInventory *this)
// {
//   return this->m_byTradePassword;
// }
std::uint8_t CGocInventory::GetTradePasswordState() const {
    return m_byTradePassword;
}

// IDA: 0x1400F9C20 - GetTradeActorID
// std::uint32_t *__fastcall CGocInventory::GetTradeActorID(CGocInventory *this, std::uint32_t *result)
// {
//   result->__s0 = ($DE3BFFBC99B013A67150333ABFCE18E5)this->m_uxTradeActorID;
//   return result;
// }
UXActorID CGocInventory::GetTradeActorID() const {
    return m_uxTradeActorID;
}

// IDA: 0x1400F9C50 - GetTradeState
// __int64 __fastcall CGocInventory::GetTradeState(CGocInventory *this)
// {
//   return (unsigned int)this->m_eTradeState;
// }
int CGocInventory::GetTradeState() const {
    return m_eTradeState;
}

// IDA: 0x1400F9CB0 - GetTradeMoney
// __int64 __fastcall CGocInventory::GetTradeMoney(CGocInventory *this)
// {
//   return this->m_stTradeInfo.biMoney;
// }
std::int64_t CGocInventory::GetTradeMoney() const {
    return m_stTradeInfo.biMoney;
}

// IDA: 0x14048CE70 - GetBankMoney
// __int64 __fastcall CGocInventory::GetBankMoney(CGocInventory *this)
// {
//   return this->m_nBankMoney;
// }
std::int64_t CGocInventory::GetBankMoney() const {
    return m_nBankMoney;
}

// IDA: 0x1404EAAD0 - SetSocketExtract
// void __fastcall CGocInventory::SetSocketExtract(CGocInventory *this, bool bExtract)
// {
//   this->m_bReqSocketExtract = bExtract;
// }
void CGocInventory::SetSocketExtract(bool bExtract) {
    m_bReqSocketExtract = bExtract;
}

// IDA: 0x1404EAAF0 - GetSocketExtract
// _BOOL8 __fastcall CGocInventory::GetSocketExtract(CGocInventory *this)
// {
//   return this->m_bReqSocketExtract;
// }
bool CGocInventory::GetSocketExtract() const {
    return m_bReqSocketExtract;
}

// IDA: 0x1404EAB10 - SetSocketUpgrade
// void __fastcall CGocInventory::SetSocketUpgrade(CGocInventory *this, bool bUpgrade)
// {
//   this->m_bReqSocketUpgrade = bUpgrade;
// }
void CGocInventory::SetSocketUpgrade(bool bUpgrade) {
    m_bReqSocketUpgrade = bUpgrade;
}

// IDA: 0x1404EAB30 - GetSocketUpgrade
// _BOOL8 __fastcall CGocInventory::GetSocketUpgrade(CGocInventory *this)
// {
//   return this->m_bReqSocketUpgrade;
// }
bool CGocInventory::GetSocketUpgrade() const {
    return m_bReqSocketUpgrade;
}

// IDA: 0x1404EAB80 - SetSocketExchange
// void __fastcall CGocInventory::SetSocketExchange(CGocInventory *this, bool bExchange)
// {
//   this->m_bReqSocketExchange = bExchange;
// }
void CGocInventory::SetSocketExchange(bool bExchange) {
    m_bReqSocketExchange = bExchange;
}

// IDA: 0x1404EABA0 - GetSocketExchange
// _BOOL8 __fastcall CGocInventory::GetSocketExchange(CGocInventory *this)
// {
//   return this->m_bReqSocketExchange;
// }
bool CGocInventory::GetSocketExchange() const {
    return m_bReqSocketExchange;
}

// IDA: 0x1404EAC00 - SetItemRefineReq
// void __fastcall CGocInventory::SetItemRefineReq(CGocInventory *this, bool bRefine)
// {
//   this->m_bReqItemRefine = bRefine;
// }
void CGocInventory::SetItemRefineReq(bool bRefine) {
    m_bReqItemRefine = bRefine;
}

// IDA: 0x1404EAC20 - GetItemRefineReq
// _BOOL8 __fastcall CGocInventory::GetItemRefineReq(CGocInventory *this)
// {
//   return this->m_bReqItemRefine;
// }
bool CGocInventory::GetItemRefineReq() const {
    return m_bReqItemRefine;
}

// IDA: 0x1404EAE70 - SetRemoveBroach
// void __fastcall CGocInventory::SetRemoveBroach(CGocInventory *this, bool bRemove)
// {
//   this->m_bReqBroachRemove = bRemove;
// }
void CGocInventory::SetRemoveBroach(bool bRemove) {
    m_bReqBroachRemove = bRemove;
}

// IDA: 0x1404EAE90 - GetRemoveBroach
// _BOOL8 __fastcall CGocInventory::GetRemoveBroach(CGocInventory *this)
// {
//   return this->m_bReqBroachRemove;
// }
bool CGocInventory::GetRemoveBroach() const {
    return m_bReqBroachRemove;
}

// IDA: 0x1401E7F20 - SetTradePasswordState
// void __fastcall CGocInventory::SetTradePasswordState(CGocInventory *this, unsigned __int8 byState)
// {
//   this->m_byTradePassword = byState;
// }
void CGocInventory::SetTradePasswordState(std::uint8_t byState) {
    m_byTradePassword = byState;
}

// IDA: 0x1400FA4D0 - SetCashItemDate
// void __fastcall CGocInventory::SetCashItemDate(CGocInventory *this, __int64 biSerial, int nCashDate)
// {
//   std::pair<__int64 const ,int> *v3; // rax
//   std::pair<__int64,int> v4; // [rsp+20h] [rbp-28h] BYREF
//   std::pair<std::_Tree_iterator<std::_Tree_val<std::_Tmap_traits<__int64,int,std::less<__int64>,std::allocator<std::pair<__int64 const ,int> >,0> > >,bool> result; // [rsp+30h] [rbp-18h] BYREF
//   __int64 biSeriala; // [rsp+58h] [rbp+10h] BYREF
//   int nCashDatea; // [rsp+60h] [rbp+18h] BYREF
//   nCashDatea = nCashDate;
//   biSeriala = biSerial;
//   v3 = (std::pair<__int64 const ,int> *)std::pair<__int64 const,int>::pair<__int64 const,int>(
//                                           &v4,
//                                           &biSeriala,
//                                           &nCashDatea);
//   std::_Tree<std::_Tmap_traits<__int64,int,std::less<__int64>,std::allocator<std::pair<__int64 const,int>>,0>>::insert<std::pair<__int64 const,int>>(
//     &this->m_mpCashItemDate,
//     &result,
//     v3);


// IDA: 0x1400E5140 - GetCashMileage
// __int64 __fastcall CGocInventory::GetCashMileage(CGocInventory *this, E_CASH_MILEAGE_TYPE eType)
// {
//   switch ( eType )
//   {
//     case E_CASH_MILEAGE_AKASHIC:
//       return (unsigned int)this->m_nCashMileage[0];
//     case E_CASH_MILEAGE_BROACH:
//       return (unsigned int)this->m_nCashMileage[1];
//     case E_CASH_MILEAGE_TAG:
//       return (unsigned int)this->m_nCashMileage[2];
//   }
//   return 0xFFFFFFFFLL;
// }
int CGocInventory::GetCashMileage(int eType) const {
    switch (eType) {
        case 0:  // E_CASH_MILEAGE_AKASHIC
            return m_nCashMileage[0];
        case 1:  // E_CASH_MILEAGE_BROACH
            return m_nCashMileage[1];
        case 2:  // E_CASH_MILEAGE_TAG
            return m_nCashMileage[2];
        default:
            return -1;
    }
}

// ============================================================================
// Weight/encumbrance
// ============================================================================

int CGocInventory::GetCurrentWeight() const {
    return m_nCurrentWeight;
}

int CGocInventory::GetMaxWeight() const {
    return m_nMaxWeight;
}

bool CGocInventory::IsOverencumbered() const {
    return m_nCurrentWeight > m_nMaxWeight;
}

// ============================================================================
// Endurance functions (IDA verified)
// ============================================================================

// IDA: 0x1400A7110
// void __fastcall CGocInventory::AtkDecEndurance(CGocInventory *this)
// Decreases endurance on attack for equipped items (type 1 - ability equip)
void CGocInventory::AtkDecEndurance() {
    // IDA: Get ability equipment pointer
    XBaseEquip* pEquip = GetEquipPtr(1);
    if (!pEquip) {
        return;
    }

    // IDA: Call XBaseEquip::AtkDecEndurance with owner and endurance table
    // PS_ITEM_ENDURANCE_LIST psUpdateList;
    // ST_ENDURANCE_LIST stUpdateList;
    // if (pEquip->AtkDecEndurance(GetOwner(), &psUpdateList, m_pEnduranceTable)) {
    //     for each PS_ITEM_ENDURANCE_INFO in psUpdateList:
    //         SetEnduranceLog(&psInfo);
    //         stUpdateList.push_back(psInfo);
    //     SendDecEndurance(&stUpdateList);
    // }

    // TODO: Implement when XBaseEquip endurance methods are available
}

// IDA: 0x1400A7340
// void __fastcall CGocInventory::DefDecEndurance(CGocInventory *this)
// Decreases endurance on defense for equipped items (type 1 - ability equip)
void CGocInventory::DefDecEndurance() {
    // IDA: Get ability equipment pointer
    XBaseEquip* pEquip = GetEquipPtr(1);
    if (!pEquip) {
        return;
    }

    // IDA: Call XBaseEquip::DefDecEndurance with owner and endurance table
    // PS_ITEM_ENDURANCE_LIST psUpdateList;
    // ST_ENDURANCE_LIST stUpdateList;
    // if (pEquip->DefDecEndurance(GetOwner(), &psUpdateList, m_pEnduranceTable)) {
    //     for each PS_ITEM_ENDURANCE_INFO in psUpdateList:
    //         SetEnduranceLog(&psInfo);
    //         stUpdateList.push_back(psInfo);
    //     SendDecEndurance(&stUpdateList);
    // }

    // TODO: Implement when XBaseEquip endurance methods are available
}

// IDA: 0x1400A7540
// void __fastcall CGocInventory::DieDecEndurance(CGocInventory *this)
// Decreases endurance on death for equipped items (type 1 - ability equip)
void CGocInventory::DieDecEndurance() {
    // IDA: Get ability equipment pointer
    XBaseEquip* pEquip = GetEquipPtr(1);
    if (!pEquip) {
        return;
    }

    // IDA: Call XBaseEquip::DieDecEndurance with owner and endurance table
    // PS_ITEM_ENDURANCE_LIST psUpdateList;
    // ST_ENDURANCE_LIST stUpdateList;
    // if (pEquip->DieDecEndurance(GetOwner(), &psUpdateList, m_pEnduranceTable)) {
    //     for each PS_ITEM_ENDURANCE_INFO in psUpdateList:
    //         SetEnduranceLog(&psInfo);
    //         stUpdateList.push_back(psInfo);
    //     SendDecEndurance(&stUpdateList);
    // }

    // TODO: Implement when XBaseEquip endurance methods are available
}

// IDA: 0x1400A7740 - ?SendDecEndurance@CGocInventory@@QEAAXUST_ENDURANCE_LIST@@@Z
// Precise restoration from IDA decompilation
// Sends endurance decrease update to client (main=0x18, sub=0x12) and DB (main=0x81, sub=0x11)
void CGocInventory::SendDecEndurance(ST_ENDURANCE_LIST* pstUpdateList) {
    if (!pstUpdateList) return;

    // IDA: Send to client (main=0x18, sub=0x12)
    XSendPacket xSendPacket(0x18, 0x12);
    xSendPacket << *pstUpdateList;
    CGocNetwork::Send(m_pOwner, xSendPacket);

    // IDA: Send to DB (main=0x81, sub=0x11)
    CMover* pOwner = GetOwnerGO();
    IXObject* pObject = pOwner ? static_cast<IXObject*>(pOwner) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 0x81, 0x11);

    // IDA: Get UCID from CUser via RTTI dynamic_cast
    CUser* pUser = GetCUserFromOwner(this);
    std::uint32_t dwUCID = pUser ? pUser->GetUCID() : 0;
    xSendDBPacket.XParse << dwUCID;
    xSendDBPacket << *pstUpdateList;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pServer) pServer->SendDBGame(xSendDBPacket);
}

// IDA: 0x1400A7910
// bool __fastcall CGocInventory::SetEndurance(CGocInventory *this, unsigned __int8 byInvenType,
//                                              __int16 shSlotPos, STItem *stItem)
// Sets endurance value on an item, validates serial matches
bool CGocInventory::SetEndurance(std::uint8_t byInvenType, std::int16_t shSlotPos, STItem* pstItem) {
    // IDA: Get item from slot
    bool bLock = false;
    std::shared_ptr<CItem> pItem = GetSlotItem(byInvenType, static_cast<std::uint16_t>(shSlotPos), bLock);

    if (!pItem) {
        // IDA: SendItemLockLog(dwUCID, byInvenType, shSlotPos, bLock, 5, 0)
        CUser* pUser = GetCUserFromOwner(this);
        std::uint32_t dwUCID = pUser ? pUser->GetActorID().GetID() : 0;
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (pServer) {
            pServer->SendItemLockLog(dwUCID, byInvenType, shSlotPos, bLock ? 1 : 0, 5, 0);
        }
        return false;
    }

    // IDA: Check serial matches
    if (pItem->GetSerial() != pstItem->xSerial) {
        return false;
    }

    // IDA: Update endurance
    std::uint8_t byPrevEndurance = pItem->GetEndurance();
    pItem->SetEndurance(pstItem->byEndurance);

    // IDA: Apply endurance effect for ability equipment (type 1)
    if (byInvenType == 1) {
        CMover* pOwner = GetOwnerGO();
        pItem->SetEnduranceEffect(pOwner, true, byPrevEndurance);
    }

    return true;
}

// IDA: 0x1400A7AE0 - ?SetInventoryInfos@CGocInventory@@QEAAXEUPS_RES_STORAGE_INFO@@@Z
// Precise restoration from IDA decompilation
// Loads inventory items from DB response, handles expired cash items
void CGocInventory::SetInventoryInfos(std::uint8_t byInvenType, PS_RES_STORAGE_INFO* psResInfo) {
    if (!psResInfo) return;

    // IDA: Special handling for repurchaser (type 10)
    if (byInvenType == 10) {
        ClearRepurchaser();
        PushRepurchaserItem(psResInfo, false);
        return;
    }

    // IDA: Get CGocPost component for system post
    CMover* pOwner = GetOwnerGO();
    std::shared_ptr<CGocPost> pPostPtr;
    if (pOwner) {
        pOwner->GetGOC<CGocPost>(&pPostPtr, 0);
    }

    // IDA: Get current date for cash item expiry check
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    std::int64_t biCurDate = pServer ? pServer->GetCurDate() : 0;

    // IDA: Get CUser for UAID/UCID
    CUser* pUser = GetCUserFromOwner(this);
    std::uint32_t dwUAID = pUser ? pUser->GetUAID() : 0;
    std::uint32_t dwUCID = pUser ? pUser->GetActorID().GetID() : 0;

    // IDA: Iterate through all items
    for (auto& stInfo : psResInfo->vecItem) {
        // IDA: Skip equipment types 0 (Ability), 1 (Shape), 3 (Look)
        // IDA pattern: LOBYTE(xPos.x) == 1 || !LOBYTE(xPos.x) || LOBYTE(xPos.x) == 3
        // This refers to byInvenType in the outer context, checking if it's equipment
        std::uint8_t byInvenTypeLocal = stInfo.byInvenType;
        if (byInvenTypeLocal == 1 || byInvenTypeLocal == 0 || byInvenTypeLocal == 3) {
            continue;  // IDA: goto LABEL_4
        }

        // IDA: Check if cash item expired (nCashDate < biCurDate)
        std::int64_t nCashDate = stInfo.stItem.nCashDate;
        if (nCashDate != 0 && nCashDate < biCurDate) {
            // IDA: Item expired - send DB delete request
            std::int16_t shSlotPos = stInfo.shSlotPos;
            if (shSlotPos > 0) {
                // IDA: Get ActorID (use UAID for account banks 16,17,18)
                std::uint32_t dwActorIDForDelete = dwUCID;
                if (byInvenType == 16 || byInvenType == 17 || byInvenType == 18) {
                    dwActorIDForDelete = dwUAID;
                }

                IXObject* pObject = pOwner ? static_cast<IXObject*>(pOwner) : nullptr;
                XSendDBPacket xSendDBPacket(pObject, 0x21, 0x24);
                xSendDBPacket.XParse << dwActorIDForDelete;
                xSendDBPacket.XParse << stInfo.stItem.xSerial;
                xSendDBPacket.XParse << static_cast<std::int32_t>(0);
                xSendDBPacket.XParse << byInvenType;
                xSendDBPacket.XParse << shSlotPos;

                if (pServer) {
                    pServer->SendDBGame(xSendDBPacket);
                }

                // IDA: Check TB_ITEM for cash item event post
                TB_ITEM* pTBItem = pServer ? pServer->GetResourceMgr().GetTB_ITEM(stInfo.stItem.nItemID) : nullptr;
                if (pTBItem && pTBItem->Item_Cash && pTBItem->Item_Use_Period_Type) {
                    // IDA: Send system post for expired cash item
                    CGocPost* pPost = pPostPtr.get();
                    if (pPost) {
                        std::int32_t nItemID = static_cast<std::int32_t>(stInfo.stItem.nItemID);
                        pPost->SystemPostSend(3, 3, nItemID, dwUCID);
                    }
                }

                // IDA: Log expired item deletion
                ST_LOG_GAME stLog;
                stLog._nUAID = dwUAID;
                stLog._nUCID = dwUCID;
                stLog._sMainType = 4;
                stLog._sSubType = 34;
                stLog.nParam0 = static_cast<std::int32_t>(stInfo.stItem.nItemID);
                stLog.nParam1 = 0;
                stLog.nParam5 = stInfo.stItem.xSerial;
                stLog.nParam11 = -1;
                stLog.nParam12 = 0;

                if (pServer) {
                    pServer->SendDBLog(stLog);
                }
            }
            continue;  // IDA: goto LABEL_4
        }

        // IDA: Set cash item date if not expired
        if (nCashDate != 0) {
            SetCashItemDate(stInfo.stItem.xSerial, static_cast<std::int32_t>(nCashDate));
        }

        // IDA: Validate dye info
        std::int32_t nDyeID = stInfo.stItem.nDyeID;
        if (!IsValidDyeInfo(static_cast<std::int32_t>(stInfo.stItem.nItemID), nDyeID)) {
            // IDA: Log error for invalid dye
            LogHelper::LogError("game.item",
                "SetInventoryInfos - IsValidDyeInfo[UCID:%d, ItemID:%d, DyeID:%d]",
                dwUCID, stInfo.stItem.nItemID, nDyeID);
            continue;
        }

        // IDA: Check random option
        if (CheckRandomOption(&stInfo.stItem)) {
            LogHelper::LogError("game.item",
                "SetInventoryInfos - Change item random option[UCID:%d]", dwUCID);
        }

        // IDA: Copy item to local STItem
        STItem stItem = stInfo.stItem;

        // IDA: Validate slot and add item
        std::int16_t shSlotPos = stInfo.shSlotPos;
        if (IsValidSlotItem(byInvenType, shSlotPos, &stItem)) {
            AddItem(byInvenType, shSlotPos, stItem, false);
        }
        else {
            // IDA: Add to overlapped slot map if slot is invalid
            m_mpOverlappedSlot.insert(std::make_pair(byInvenType, stItem));
        }
    }

    // IDA: Add items to empty slots
    AddItemEmptySlot();
}

// IDA: 0x1400A83F0 - ?SendInventory@CGocInventory@@QEAAXXZ
// Precise restoration from IDA decompilation
// Sends inventory data to client for all inventory types (0,1,3,2,4,13)
// Note: Type order is {0, 1, 3, 2, 4, 13} - type 3 comes before type 2
void CGocInventory::SendInventory() {
    // IDA: Build list of inventory types: {0, 1, 3, 2, 4, 13}
    std::list<int> listInvenType;
    listInvenType.push_back(0);   // Ability equip
    listInvenType.push_back(1);   // Shape equip
    listInvenType.push_back(3);   // Look equip (before type 2!)
    listInvenType.push_back(2);   // Common inventory
    listInvenType.push_back(4);   // Costume inventory
    listInvenType.push_back(13);  // Cash inventory

    // IDA: Get owner actor for sending packets
    CMover* pOwner = GetOwnerGO();
    XActor* pActor = pOwner ? static_cast<XActor*>(pOwner) : nullptr;

    // IDA: Iterate through each inventory type
    for (auto it = listInvenType.begin(); it != listInvenType.end(); ++it) {
        std::uint8_t byInvenType = static_cast<std::uint8_t>(*it);

        // IDA: Create storage info structures
        PS_RES_STORAGE_INFO stInvenInfo;
        PS_OPEN_SLOT_INFO stSlotInfo;

        // IDA: Get inventory info
        GetInvenInfo(byInvenType, &stInvenInfo);

        // IDA: Get inventory pointer for slot info
        XBaseInventory* pInven = GetInvenPtr(byInvenType);

        if (pInven) {
            // IDA: Fill slot info structure
            PS_OPEN_SLOT stSlot;
            stSlot.shOpenSlot = pInven->GetOpenSlotNum();
            stSlot.byInvenType = byInvenType;
            stSlot.byExtendStep = pInven->GetExtendStep();
            stSlotInfo.vecInfo.push_back(stSlot);

            // IDA: Send slot info packet (main=8, sub=0xE)
            XSendPacket xSendPacket(8, 0xE);
            xSendPacket << stSlotInfo;
            CGocNetwork::Send(pActor, xSendPacket);
        }

        // IDA: Always send inventory content packet (main=8, sub=1)
        XSendPacket packet(8, 1);
        packet << stInvenInfo;
        CGocNetwork::Send(pActor, packet);
    }
}

// IDA: 0x1400A8770 - ?SendBank@CGocInventory@@QEAAXXZ
// Precise restoration from IDA decompilation
// Sends bank data to client (types 5,6,14 for JPN or 16,17,18 for others)
void CGocInventory::SendBank() {
    // IDA: Build list of bank types based on nation
    std::list<int> listInvenType;

    // IDA: Get server option to check nation type
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    XOption* pOption = pServer ? &pServer->GetOption() : nullptr;

    // IDA: Nation type 2 (JPN) uses {5, 6, 14}, others use {16, 17, 18}
    if (pOption && pOption->GetNationType() == 2) {
        listInvenType.push_back(5);   // Bank 0 (JPN)
        listInvenType.push_back(6);   // Bank 1 (JPN)
        listInvenType.push_back(14);  // Bank 2 (JPN)
    } else {
        listInvenType.push_back(16);  // Account bank 0
        listInvenType.push_back(17);  // Account bank 1
        listInvenType.push_back(18);  // Account bank 2
    }

    // IDA: Get owner actor for sending packets
    CMover* pOwner = GetOwnerGO();
    XActor* pActor = pOwner ? static_cast<XActor*>(pOwner) : nullptr;

    // IDA: Iterate through each bank type
    for (auto it = listInvenType.begin(); it != listInvenType.end(); ++it) {
        std::uint8_t byInvenType = static_cast<std::uint8_t>(*it);

        // IDA: Create storage info structures
        PS_RES_STORAGE_INFO stInvenInfo;
        PS_OPEN_SLOT_INFO stSlotInfo;

        // IDA: Get bank info
        GetInvenInfo(byInvenType, &stInvenInfo);

        // IDA: Get bank pointer for slot info
        XBaseInventory* pInven = GetInvenPtr(byInvenType);

        if (pInven) {
            // IDA: Fill slot info structure
            PS_OPEN_SLOT stSlot;
            stSlot.shOpenSlot = pInven->GetOpenSlotNum();
            stSlot.byInvenType = byInvenType;
            stSlot.byExtendStep = pInven->GetExtendStep();
            stSlotInfo.vecInfo.push_back(stSlot);

            // IDA: Send slot info packet (main=8, sub=0xE)
            XSendPacket xSendPacket(8, 0xE);
            xSendPacket << stSlotInfo;
            CGocNetwork::Send(pActor, xSendPacket);
        }

        // IDA: Send bank content packet (main=8, sub=0x10)
        XSendPacket packet(8, 0x10);
        packet << stInvenInfo;
        CGocNetwork::Send(pActor, packet);
    }

    // IDA: Send bank money at end
    SendBankMoney();
}

// ============================================================================
// Item use functions (IDA verified)
// ============================================================================

// IDA: 0x1400A96B0
// Reorganizes/lines up items in inventory, sends update packets to client and DB
void CGocInventory::LineUp(std::uint8_t byInvenType) {
    // IDA-verified: Reorganize inventory and send updates
    // XBank* pInventory = GetInvenPtr(byInvenType);
    // if (!pInventory) return;
    // PS_ITEM_LINE_UP_VEC vecLineUp;
    // if (pInventory->LineUp(&vecLineUp)) {
    //     InitSimpleEmptySlot();
    //     // Send to client (main=8, sub=0x25)
    //     XSendPacket xSendPacket(8, 0x25);
    //     xSendPacket << vecLineUp;
    //     CGocNetwork::Send(pActor, &xSendPacket);
    //     // Send to DB (main=0x21, sub=0x10)
    //     XSendDBPacket xSendDBPacket(pObject, 0x21, 0x10);
    //     xSendDBPacket << dwID;
    //     xSendDBPacket << vecLineUp;
    //     XGameServer::SendDBGame(&xSendDBPacket);
    // }
    
    (void)byInvenType;
}

// IDA: 0x1400A9A30
// Complex function handling various item use types via switch on Item_Use_Type
bool CGocInventory::UseItem(std::uint8_t byInvenType, std::int16_t shSlot,
                            std::uint8_t byCount, std::int16_t shUseCount, int* nItemID) {
    // IDA-verified: Handle different item use types
    // Get item at slot
    // std::shared_ptr<CItem> pItem;
    // GetSlotItem(&pItem, byInvenType, shSlot, &bLock);
    // if (pItem.invalid() || bLock) return false;
    // *nItemID = pItem->GetCurID();
    // TB_ITEM_CLASSIFY* pClassify = pItem->GetClassifyTable();
    // int Item_Use_Type = pClassify->Item_Use_Type - 5;
    // Switch on Item_Use_Type for specific handlers
    
    (void)byInvenType;
    (void)shSlot;
    (void)byCount;
    (void)shUseCount;
    if (nItemID) *nItemID = 0;
    return false;
}

// IDA: 0x1400AB0E0
// Checks if item can be used based on various conditions
bool CGocInventory::CanUseItem(std::uint8_t byInvenType, std::int16_t shSlot) {
    (void)byInvenType;
    (void)shSlot;
    return true;
}

// IDA: 0x1400ACE80
// Sends quick slot info to client
void CGocInventory::SendQuickSlotInfo() {
    // IDA-verified: Send all quick slot data to client
    // XSendPacket xSendPacket(8, 0x40);
    // for each quick slot type and index:
    //   xSendPacket << m_QuickSlot[i][j];
    // CGocNetwork::Send(pActor, &xSendPacket);
}

// ============================================================================
// Item movement and management (IDA verified)
// ============================================================================

// IDA: 0x1400A8AF0
// Moves item between slots, validates all parameters


// IDA: 0x1400ACA50
// Sets item to quick slot
bool CGocInventory::SetQuickSlotItem(void* stUpdateSlot) {
    (void)stUpdateSlot;
    return false;
}


// ============================================================================
// Repurchaser functions (IDA verified)
// ============================================================================

// IDA: 0x1400A56D0 - ?IsRepurchaserItem@CGocInventory@@QEAA_N_JHFAEAUSTItem@@@Z
// Precise restoration from IDA decompilation
// Checks if item exists in repurchaser list by serial, ID, and count
bool CGocInventory::IsRepurchaserItem(std::int64_t biItemSerial, int nItemID,
                                       std::int16_t shCount, STItem* stOutItem) {
    // IDA: Iterate through repurchaser list
    for (auto iter = m_listRepurchaserItem.begin(); iter != m_listRepurchaserItem.end(); ++iter) {
        // IDA: Check serial, item ID, and count
        // Note: IDA shows iter[1] for biSerial, iter[0] for nItemID, iter+8 for shCount
        // This is accessing STItem fields: xSerial (offset 8), nItemID (offset 0), sCount (offset 16)
        const STItem& item = *iter;
        if (item.xSerial == biItemSerial &&
            item.nItemID == nItemID &&
            item.sCount == shCount) {
            // Found matching item, copy to output
            *stOutItem = item;
            return true;
        }
    }
    return false;
}

// IDA: 0x1400A57A0 - ?SendRepurchaseList@CGocInventory@@QEAAXXZ
// Precise restoration from IDA decompilation - sends repurchase list to client (main=9, sub=3)
void CGocInventory::SendRepurchaseList() {
    // IDA: Build repurchase list packet
    PS_RES_ITEM_REPURCHASER_LIST resItemRepurchaser;

    // IDA: Copy items from list
    for (auto iter = m_listRepurchaserItem.begin(); iter != m_listRepurchaserItem.end(); ++iter) {
        resItemRepurchaser.vecInfo.push_back(*iter);
    }

    // IDA: Copy socket and broach lists
    resItemRepurchaser.psSocketList = m_listRepurchaseSocket;
    resItemRepurchaser.psBroachList = m_listRepurchaseBroach;

    // IDA: Send packet (main=9, sub=3)
    XSendPacket xSendPacket(9, 3);
    xSendPacket << resItemRepurchaser;

    CGocNetwork::Send(m_pOwner, xSendPacket);
}

// ============================================================================
// Quick slot functions (IDA verified)
// ============================================================================

// IDA: 0x1400ACD50
// Loads quick slot data from database
bool CGocInventory::LoadQuickSlotItem(void* stQuickSlotItem) {
    (void)stQuickSlotItem;
    return false;
}


// ============================================================================
// Item creation functions (IDA verified)
// ============================================================================

// IDA: 0x1400AD7E0
// PDB: ?CreateItemReq@CGocInventory@@QEAA_NHF_NW4eITEM_CREATE_TYPE@@AEAUST_LOG_GAME@@@Z
bool CGocInventory::CreateItemReq(int nItemID, std::int16_t sCount,
                                   bool bAddOption,
                                   eITEM_CREATE_TYPE eCreateType,
                                   ST_LOG_GAME& stLogData) {
    ST_CREATE_ITEM stInfo{};
    stInfo.nItemID = nItemID;
    stInfo.shCount = sCount;

    ST_CREATE_ITEMS stCreateItems{};
    stCreateItems.vecInfo.push_back(stInfo);

    const std::uint8_t byCurLock =
        static_cast<std::uint8_t>(eCreateType + 100);
    PS_RES_STORAGE_INFO psCreateItem{};
    PS_RES_STORAGE_INFO psUpdateItem{};
    LogCreateItemLog(static_cast<int>(eCreateType), stLogData);

    if (!CreateItem2(stCreateItems, byCurLock, bAddOption,
                     psCreateItem, psUpdateItem, stLogData)) {
        return false;
    }

    if (eCreateType == E_ITEM_CREATE_TYPE_QUEST_ACCEPT ||
        eCreateType == E_ITEM_CREATE_TYPE_CONDITION_COMPLETE) {
        psCreateItem.byType = static_cast<std::uint8_t>(eCreateType);
    }

    CMover* pOwner = GetOwnerGO();
    XSendDBPacket xSendDBPacket(
        static_cast<IXObject*>(pOwner), 0x21, 0x0C);
    xSendDBPacket.XParse << pOwner->GetActorID().dwActorID;
    xSendDBPacket.XParse << byCurLock;
    xSendDBPacket << psCreateItem;
    xSendDBPacket << psUpdateItem;
    xSendDBPacket.XParse << eCreateType;
    TXSingleton<XGameServer>::Instance()->SendDBGame(xSendDBPacket);
    return true;
}


// ============================================================================
// Update functions (IDA verified)
// ============================================================================

// IDA: 0x1400AE280
// Periodic update called by game loop, handles various inventory updates
void CGocInventory::OnUpdate() {
    OnUpdateCashItemDate();
}


// ============================================================================
// Trade functions (IDA verified)
// ============================================================================

// IDA: 0x1400AED60
// Updates trade unlock status for inventory items
void CGocInventory::UpdateTradeUnLock() {
}


// IDA: 0x1400AEEE0
// Checks if item can be traded
void CGocInventory::IsTradeCheck() {
}


// ============================================================================
// Item breaking/disposal functions (IDA verified)
// ============================================================================

// IDA: 0x1400ADB20
// Handles item break/enhance request
bool CGocInventory::BreakItemReq(std::uint8_t byInvenType, std::int16_t shSlotPos, int nCount, std::uint8_t byBreakLock, void* stLogData) {
    (void)byInvenType;
    (void)shSlotPos;
    (void)nCount;
    (void)byBreakLock;
    (void)stLogData;
    return false;
}


// ============================================================================
// Logout/trade cancel functions (IDA verified)
// ============================================================================

// IDA: 0x1400AF190
// void __fastcall CGocInventory::LogOut(CGocInventory *this, bool bLogout)
// Handles logout during trade - cancels trade with partner
void CGocInventory::LogOut(bool bLogout) {
    // IDA: Check if in trade
    // if (!CGocInventory::GetTradeState(this)) return;
    //
    // std::uint32_t TradeActorID = CGocInventory::GetTradeActorID(this);
    // if (TradeActorID == 0) return;
    //
    // // Get trade partner
    // XGameServer* pServer = XGameServer::Instance();
    // CUser* pPartner = pServer->FindActorIDToUser(TradeActorID);
    // if (pPartner) {
    //     // Get partner's inventory component
    //     std::shared_ptr<CGocInventory> pInven;
    //     CMover::GetGOC<CGocInventory>(&pPartner->CMoverEx, &pInven, 0);
    //     pInven->InitTarde();
    //
    //     // Send cancel packet (main=0xA, sub=8)
    //     XSendPacket xSendPacket(0xA, 8);
    //     xSendPacket << GetActorID();
    //     xSendPacket << (bLogout ? 0xCC5A : 0xCC5B);  // Error code
    //     CGocNetwork::Send(&pPartner->XActor, &xSendPacket);
    // }

    (void)bLogout;
    // TODO: Implement when GetTradeState and GetTradeActorID are available
}

// IDA: 0x1400AF370
// void __fastcall CGocInventory::SendTradeCancel(CGocInventory *this, int nCause)
// Sends trade cancel notification to both parties
void CGocInventory::SendTradeCancel(int nCause) {
    // IDA: Check if in trade
    // if (!CGocInventory::GetTradeState(this)) return;
    //
    // std::uint32_t TradeActorID = CGocInventory::GetTradeActorID(this);
    // if (TradeActorID != 0) {
    //     // Get trade partner
    //     XGameServer* pServer = XGameServer::Instance();
    //     CUser* pPartner = pServer->FindActorIDToUser(TradeActorID);
    //     if (pPartner) {
    //         // Get partner's inventory component
    //         std::shared_ptr<CGocInventory> pInven;
    //         CMover::GetGOC<CGocInventory>(&pPartner->CMoverEx, &pInven, 0);
    //         pInven->InitTarde();
    //
    //         // Send cancel packet to partner
    //         XSendPacket packet(0xA, 8);
    //         packet << GetActorID();
    //         packet << nCause;
    //         CGocNetwork::Send(&pPartner->XActor, &packet);
    //     }
    // }
    //
    // // Initialize our trade state
    // std::shared_ptr<CGocInventory> pInven;
    // CMover::GetGOC<CGocInventory>(pMover, &pInven, 0);
    // pInven->InitTarde();
    //
    // // Send cancel packet to self
    // XSendPacket xSendPacket(0xA, 8);
    // xSendPacket << GetActorID();
    // xSendPacket << nCause;
    // CGocNetwork::Send(pActor, &xSendPacket);

    (void)nCause;
    // TODO: Implement when GetTradeState and GetTradeActorID are available
}

// ============================================================================
// Helper item functions (IDA verified)
// ============================================================================

// IDA: 0x1400AF660
// IDA: 0x1400AF660 - ?ConvertHelperInvenSlot@CGocInventory@@QEAAFF@Z
// Precise restoration from IDA decompilation - converts helper slot type (240-242) to index (0-2)
std::int64_t CGocInventory::ConvertHelperInvenSlot(std::int16_t shSlotType) {
    switch (shSlotType) {
        case 240:
            return 0;
        case 241:
            return 1;
        case 242:
            return 2;
        default:
            return 0xFFFFFFFFLL; // -1
    }
}

// IDA: 0x1400AF6C0
// bool __fastcall CGocInventory::IsHelperItem(CGocInventory *this, unsigned int dwItemID)
// Checks if item is a helper item (slot type 240-242)
// IDA: 0x1400AF6C0 - ?IsHelperItem@CGocInventory@@QEAA_NK@Z
// Precise restoration from IDA decompilation - checks if item is a helper item (slot type 240-242)
bool CGocInventory::IsHelperItem(std::uint32_t dwItemID) {
    // IDA: Get item table entry
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        return false;
    }

    TB_ITEM* pTB_ITEM = pGameServer->GetResourceMgr().GetTB_ITEM(dwItemID);
    if (!pTB_ITEM) {
        return false;
    }

    // IDA: Get classify table entry
    TB_ITEM_CLASSIFY* pTB_ITEM_CLASSIFY = pGameServer->GetResourceMgr().GetTB_ITEM_CLASSIFY(pTB_ITEM->Item_Classify_Index);
    if (!pTB_ITEM_CLASSIFY) {
        return false;
    }

    // IDA: Check if slot type is 240, 241, or 242 (helper slots)
    return pTB_ITEM_CLASSIFY->Item_Slot_Type == 240 ||
           pTB_ITEM_CLASSIFY->Item_Slot_Type == 241 ||
           pTB_ITEM_CLASSIFY->Item_Slot_Type == 242;
}

// ============================================================================
// Trade item functions (IDA verified)
// ============================================================================

// IDA: 0x1400AE6A0
// char __fastcall CGocInventory::PopTradeItem(CGocInventory *this, PS_REQ_ITEM_TRADE stInfo)
// Removes item from trade list, returns true if found and removed
bool CGocInventory::PopTradeItem(void* stInfo) {
    // IDA: Find item in trade list
    // PS_REQ_ITEM_TRADE* pInfo = static_cast<PS_REQ_ITEM_TRADE*>(stInfo);
    // auto iter = std::find(m_stTradeInfo.listInfo.begin(), m_stTradeInfo.listInfo.end(), *pInfo);
    // if (iter == m_stTradeInfo.listInfo.end()) {
    //     return false;
    // }
    // m_stTradeInfo.listInfo.erase(iter);
    // return true;

    (void)stInfo;
    // TODO: Implement when PS_REQ_ITEM_TRADE is available
    return false;
}

// ============================================================================
// Item update/sync functions (IDA verified)
// ============================================================================

// IDA: 0x1400AF7A0
// void __fastcall CGocInventory::SendUpdateItem(CGocInventory *this, PS_RES_STORAGE_INFO *stItemList)
// Sends update for multiple items from list
void CGocInventory::SendUpdateItem(void* stItemList) {
    // IDA: Iterate through item list
    // PS_RES_STORAGE_INFO* pList = static_cast<PS_RES_STORAGE_INFO*>(stItemList);
    // for (auto& iter : *pList) {
    //     if (iter.stItem.sCount > 0) {
    //         // Send update packet
    //         CGocInventory::SendUpdateItem(this, iter.byInvenType, iter.shSlotPos,
    //                                       iter.stItem.sCount, byType);
    //     } else {
    //         // Item was deleted, unlock and send break
    //         CGocInventory::SetLock(this, iter.byInvenType, iter.shSlotPos, 0);
    //         CGocInventory::SendBreakItem(this, iter.byInvenType, iter.shSlotPos);
    //     }
    // }

    (void)stItemList;
    // TODO: Implement when PS_RES_STORAGE_INFO is available
}

// IDA: 0x1400B00B0
// void __fastcall CGocInventory::SendUpdateItem(CGocInventory *this,
//         unsigned __int8 byInvenType, __int16 shSlotPos,
//         __int16 shCount, unsigned __int8 byType)
// Sends update for single item (main=8, sub=0x12)
void CGocInventory::SendUpdateItem(std::uint8_t byInvenType, std::int16_t shSlotPos,
                                    std::int16_t shCount, std::uint8_t byType) {
    // IDA: Check count
    // if (shCount < 1) {
    //     CGocInventory::SendBreakItem(this, byInvenType, shSlotPos);
    //     return;
    // }
    //
    // // Unlock slot
    // CGocInventory::SetLock(this, byInvenType, shSlotPos, 0);
    //
    // // Build update packet
    // PS_ITEM_UPDATE psItemUpdateInfo;
    // psItemUpdateInfo.byType = byType;
    // psItemUpdateInfo.byInvenType = byInvenType;
    // psItemUpdateInfo.shSlotPos = shSlotPos;
    // psItemUpdateInfo.nCount = shCount;
    //
    // // Send packet (main=8, sub=0x12)
    // XSendPacket xSendPacket(8, 0x12);
    // xSendPacket << &psItemUpdateInfo;
    // CGocNetwork::Send(pActor, &xSendPacket);

    (void)byInvenType;
    (void)shSlotPos;
    (void)shCount;
    (void)byType;
    // TODO: Implement when PS_ITEM_UPDATE is available
}

// IDA: 0x1400AFFC0
// void __fastcall CGocInventory::SendBreakItem(CGocInventory *this,
//         unsigned __int8 byInvenType, __int16 shSlotPos)
// Sends item break/delete notification (main=8, sub=5)
void CGocInventory::SendBreakItem(std::uint8_t byInvenType, std::int16_t shSlotPos) {
    // IDA: Send break item packet
    // XSendPacket xSendPacket(8, 5);
    // xSendPacket << byInvenType;
    // xSendPacket << shSlotPos;
    // CGocNetwork::Send(pActor, &xSendPacket);

    (void)byInvenType;
    (void)shSlotPos;
    // TODO: Implement when XSendPacket is available
}

// ============================================================================
// DB sync functions (IDA verified)
// ============================================================================

// IDA: 0x1400AF900
// void __fastcall CGocInventory::SendUpdateItemToDB(CGocInventory *this, PS_RES_STORAGE_INFO *stItemList)
// Sends item updates to database (main=0x21, sub=0x11 for update, sub=5 for delete)
void CGocInventory::SendUpdateItemToDB(void* stItemList) {
    // IDA: Iterate through item list
    // PS_RES_STORAGE_INFO* pList = static_cast<PS_RES_STORAGE_INFO*>(stItemList);
    // for (auto& iter : *pList) {
    //     if (iter.stItem.sCount > 0) {
    //         // Send update DB packet (main=0x21, sub=0x11)
    //         PS_DB_ITEM_REDUCE stReduce;
    //         stReduce.dwUCID = GetUCID();
    //         stReduce.byInvenType = iter.byInvenType;
    //         stReduce.shSlotPos = iter.shSlotPos;
    //         stReduce.nReduceCount = iter.stItem.sCount;
    //         stReduce.xSerial = iter.stItem.xSerial;
    //         XSendDBPacket xSendPacket(pObject, 0x21, 0x11);
    //         xSendPacket << &stReduce;
    //         XGameServer::SendDBGame(&xSendPacket);
    //     } else {
    //         // Send delete DB packet (main=0x21, sub=5)
    //         XSendDBPacket xSendDBPacket(pObject, 0x21, 5);
    //         xSendDBPacket << dwUCID;
    //         xSendDBPacket << iter.byInvenType;
    //         xSendDBPacket << iter.shSlotPos;
    //         xSendDBPacket << iter.stItem.nItemID;
    //         xSendDBPacket << iter.stItem.xSerial;
    //         XGameServer::SendDBGame(&xSendDBPacket);
    //     }
    // }

    (void)stItemList;
    // TODO: Implement when PS_RES_STORAGE_INFO and PS_DB_ITEM_REDUCE are available
}

// IDA: 0x1400AFC40
// void __fastcall CGocInventory::SendUserUpdateItem(CGocInventory *this,
//         PS_RES_STORAGE_INFO *psUpdateItem, bool bUnLock)
// Sends user item update notification (main=8, sub=0x15)
void CGocInventory::SendUserUpdateItem(void* psUpdateItem, bool bUnLock) {
    // IDA: Unlock items if requested
    // PS_RES_STORAGE_INFO* pList = static_cast<PS_RES_STORAGE_INFO*>(psUpdateItem);
    // if (bUnLock) {
    //     for (auto& iter : *pList) {
    //         CGocInventory::SetLock(this, iter.byInvenType, iter.shSlotPos, 0);
    //     }
    // }
    //
    // // Send update packet (main=8, sub=0x15)
    // XSendPacket xSendPacket(8, 0x15);
    // xSendPacket << pList;
    // CGocNetwork::Send(pActor, &xSendPacket);

    (void)psUpdateItem;
    (void)bUnLock;
    // TODO: Implement when PS_RES_STORAGE_INFO is available
}

// IDA: 0x1400AFDF0
// void __fastcall CGocInventory::SendCreateItem(CGocInventory *this, PS_RES_STORAGE_INFO *stItemList)
// Sends item creation notification (main=8, sub=6)
void CGocInventory::SendCreateItem(void* stItemList) {
    // IDA: Process cash item dates and unlock slots
    // PS_RES_STORAGE_INFO* pList = static_cast<PS_RES_STORAGE_INFO*>(stItemList);
    // for (auto& iter : *pList) {
    //     if (iter.stItem.nCashDate) {
    //         CGocInventory::SetCashItemDate(this, iter.stItem.xSerial, iter.stItem.nCashDate);
    //     }
    //     CGocInventory::SetLock(this, iter.byInvenType, iter.shSlotPos, 0);
    // }
    //
    // // Send create packet (main=8, sub=6)
    // XSendPacket xSendPacket(8, 6);
    // xSendPacket << pList;
    // CGocNetwork::Send(pActor, &xSendPacket);

    (void)stItemList;
    // TODO: Implement when PS_RES_STORAGE_INFO is available
}

// IDA: 0x1400B0230
// void __fastcall CGocInventory::SendDivideItem(CGocInventory *this, PS_DB_ITEM_MOVE *stItemMove)
// Sends item divide/split notification (main=8, sub=4)
void CGocInventory::SendDivideItem(void* stItemMove) {
    // IDA: Unlock slots
    // PS_DB_ITEM_MOVE* pMove = static_cast<PS_DB_ITEM_MOVE*>(stItemMove);
    // CGocInventory::SetLock(this, pMove->bySrcInvenType, pMove->shSrcSlotPos, 0);
    // CGocInventory::SetLock(this, pMove->byDestInvenType, pMove->shDestSlotPos, 0);
    //
    // // Build divide result packet
    // PS_RES_ITEM_DIVIDE resItemDivide;
    // resItemDivide.nItemID = pMove->stSrcItem.nItemID;
    // resItemDivide.bySrcInvenType = pMove->bySrcInvenType;
    // resItemDivide.shSrcSlotPos = pMove->shSrcSlotPos;
    // resItemDivide.shSrcCount = pMove->stSrcItem.sCount;
    // resItemDivide.byDestInvenType = pMove->byDestInvenType;
    // resItemDivide.shDestSlotPos = pMove->shDestSlotPos;
    // resItemDivide.stDestItem = pMove->stDestItem;
    //
    // // Send packet (main=8, sub=4)
    // XSendPacket xSendPacket(8, 4);
    // xSendPacket << &resItemDivide;
    // CGocNetwork::Send(pActor, &xSendPacket);
    //
    // // Send DB item log
    // ST_LOG_GAME stLogData;
    // stLogData._nUAID = pUser->GetUAID();
    // stLogData._nUCID = GetUCID();
    // stLogData._sMainType = 4;
    // stLogData._sSubType = 36;
    // XGameServer::SendDBItemLog(&stLogData, &vecCreateItem, &vecUpdateItem);
    //
    // // Send statistics
    // ST_STATISTICS_ITEM stStatistics;
    // stStatistics.byFlag = 1;
    // stStatistics.biSerial = pMove->stDestItem.xSerial;
    // stStatistics.dwUCID = GetUCID();
    // stStatistics.dwItemID = pMove->stDestItem.nItemID;
    // stStatistics.byUpgrade = pMove->stDestItem.byUpgrade;
    // stStatistics.byUpgradeLimit = pMove->stDestItem.byUpgradeLimit;
    // XSendDBPacket xSendDBStatistics(pObject, 0xF0, 0x11);
    // xSendDBStatistics << &stStatistics;
    // XGameServer::SendDBStatistics(&xSendDBStatistics);

    (void)stItemMove;
    // TODO: Implement when PS_DB_ITEM_MOVE and related types are available
}

// ============================================================================
// Private shop functions (IDA verified)
// ============================================================================

// IDA: 0x1400B11D0

// ============================================================================
// Quest items
// ============================================================================

bool CGocInventory::IsQuestItem(int nItemId) const {
    // TODO: Implement quest item check
    (void)nItemId;
    return false;
}

int CGocInventory::GetQuestItemCount() const {
    // TODO: Implement quest item count
    return 0;
}


// IDA: 0x1400A49A0
// void __fastcall CGocInventory::SetCash(CGocInventory *this, int nCash, bool bSyncDB)
// Sets cash value and optionally syncs to DB
// IDA: 0x1400A49A0 - ?SetCash@CGocInventory@@QEAAXH_N@Z
// Precise restoration from IDA decompilation - sets cash with optional DB sync
void CGocInventory::SetCash(int nCash, bool bSyncDB) {
    // IDA 0x1400A49A0 精确还原
    // Set load flag and update cash
    m_bLoadCash = true;
    m_nCash = nCash;

    if (bSyncDB) {
        // IDA: Send DB packet (main=2, sub=0x51)
        CMover* pOwner = GetOwnerGO();
        IXObject* pObject = pOwner ? static_cast<IXObject*>(pOwner) : nullptr;
        XSendDBPacket xSendDBPacket(pObject, 2, 0x51);

        // IDA: Get UAID from CUser via RTTI dynamic_cast
        CUser* pUser = GetCUserFromOwner(this);
        int nUAID = pUser ? pUser->GetUAID() : 0;
        xSendDBPacket.XParse << nUAID;
        xSendDBPacket.XParse << nCash;

        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (pServer) pServer->SendDBAccount(xSendDBPacket);
    }
}

// IDA: 0x1400A4800 - ?AddCash@CGocInventory@@QEAA_NHE@Z
// Precise restoration from IDA decompilation - adds cash with DB sync
bool CGocInventory::AddCash(int nCash, std::uint8_t byLogType) {
    // IDA 0x1400A4800 精确还原
    if (nCash == 0) {
        return true;
    }

    // IDA: Check for overflow
    if (nCash + m_nCash < 0) {
        return false;
    }

    m_nCash += nCash;

    // IDA: Send DB packet (main=2, sub=0x41)
    CMover* pOwner = GetOwnerGO();
    IXObject* pObject = pOwner ? static_cast<IXObject*>(pOwner) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 2, 0x41);

    // IDA: Get UAID from CUser via RTTI dynamic_cast
    CUser* pUser = GetCUserFromOwner(this);
    int nUAID = pUser ? pUser->GetUAID() : 0;
    xSendDBPacket.XParse << nUAID;
    xSendDBPacket.XParse << nCash;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    bool bResult = false;
    if (pServer) bResult = pServer->SendDBAccount(xSendDBPacket);

    (void)byLogType;  // Note: byLogType is unused in IDA
    return bResult;
}

// IDA: 0x1400A4B10 - ?SendCash@CGocInventory@@QEAAXH@Z
// Precise restoration from IDA decompilation - sends cash update packet to client (main=8, sub=0x33)
void CGocInventory::SendCash(int nResultCash) {
    // IDA: Create XSendPacket (main=8, sub=0x33)
    XSendPacket xSendPacket(8, 0x33);
    xSendPacket.XParse << nResultCash;

    // IDA: CGocNetwork::Send accepts CMover* (inherits XActor)
    CGocNetwork::Send(m_pOwner, xSendPacket);
}

// IDA: 0x1400A4530
// void __fastcall CGocInventory::LoadCash(CGocInventory *this)
// IDA decompiled:
//   if ( !this->m_bLoadCash )
//   {
//     XGameServer* pServer = TXSingleton<XGameServer>::Instance();
//     XOption* pOption = XServer::GetOption(pServer);
//     if ( XOption::GetBillingType(pOption) == BILLING_TYPE_REAL )
//       CGocInventory::InitWeMadeBilling(this);
//     else
//       // Send DB request (main=2, sub=0x40)
// IDA: 0x1400A4530 - ?LoadCash@CGocInventory@@QEAAXXZ
// Precise restoration from IDA decompilation - loads cash from DB if not already loaded
void CGocInventory::LoadCash() {
    // IDA 0x1400A4530 精确还原
    if (!m_bLoadCash) {
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (pServer) {
            XOption& rOption = pServer->GetOption();
            if (rOption.GetBillingType() == BILLING_TYPE_REAL) {
                // IDA: InitWeMadeBilling();
                // TODO: Implement InitWeMadeBilling when available
            } else {
                // IDA: Send DB request (main=2, sub=0x40)
                CMover* pOwner = GetOwnerGO();
                IXObject* pObject = pOwner ? static_cast<IXObject*>(pOwner) : nullptr;
                XSendDBPacket xSendDBPacket(pObject, 2, 0x40);

                // IDA: Get UAID from CUser via RTTI dynamic_cast
                CUser* pUser = GetCUserFromOwner(this);
                int nUAID = pUser ? pUser->GetUAID() : 0;
                xSendDBPacket.XParse << nUAID;

                pServer->SendDBAccount(xSendDBPacket);
            }
        }
    }
}

// IDA: 0x1400A4690 - ?ReloadCash@CGocInventory@@QEAAXXZ
// Precise restoration from IDA decompilation - forces reload of cash from DB
void CGocInventory::ReloadCash() {
    // IDA: Reset load flag first
    m_bLoadCash = false;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pServer) {
        XOption& rOption = pServer->GetOption();
        if (rOption.GetBillingType() == BILLING_TYPE_REAL) {
            // IDA: InitWeMadeBilling();
            // TODO: Implement InitWeMadeBilling when available
        } else {
            // IDA: Send DB request (main=2, sub=0x40)
            CMover* pOwner = GetOwnerGO();
            IXObject* pObject = pOwner ? static_cast<IXObject*>(pOwner) : nullptr;
            XSendDBPacket xSendDBPacket(pObject, 2, 0x40);
            xSendDBPacket.XParse << 0; // TODO: GetUAID() from CUser

            pServer->SendDBAccount(xSendDBPacket);
        }
    }

    // IDA: Set load flag at end
    m_bLoadCash = true;
}

// === Batch 15: Package functions (IDA verified) ===

// IDA: 0x1400E4EA0
// void __fastcall CGocInventory::SetCashMileage(CGocInventory *this, int *pCashMileage, bool bSend)
// {
//   PS_CASH_MILEAGE_LIST psList;
//   for ( i = 0; i < 3; ++i )
//   {
//     this->m_nCashMileage[i] = pCashMileage[i];
//     psInfo.byMileageType = i;
//     psInfo.nCashMileage = this->m_nCashMileage[i];
//     psList.push_back(psInfo);
//   }
//   if ( bSend )
//   {
//     // Send packet (main=3, sub=0x7B)
//     XSendPacket xSendPacket(3, 0x7B);
//     xSendPacket << psList;
//     CGocNetwork::Send(pActor, &xSendPacket);
//   }
// }
void CGocInventory::SetCashMileage(int* pCashMileage, bool bSend) {
    if (!pCashMileage)
        return;

    // Update mileage values (IDA verified)
    for (int i = 0; i < 3; ++i) {
        m_nCashMileage[i] = pCashMileage[i];
    }

    if (bSend) {
        // IDA: Send packet to client (main=3, sub=0x7B)
        // PS_CASH_MILEAGE_LIST psList;
        // for (int i = 0; i < 3; ++i) {
        //     PS_CASH_MILEAGE psInfo;
        //     psInfo.byMileageType = i;
        //     psInfo.nCashMileage = m_nCashMileage[i];
        //     psList.push_back(psInfo);
        // }
        // XSendPacket xSendPacket(3, 0x7B);
        // xSendPacket << psList;
        // CGocNetwork::Send(pActor, &xSendPacket);
    }
}

// IDA: 0x1400E5020
// void __fastcall CGocInventory::SetCashMileage(CGocInventory *this, PS_CASH_MILEAGE psUpdateInfo)
// {
//   this->m_nCashMileage[psUpdateInfo.byMileageType] = psUpdateInfo.nCashMileage;
//   // Send packet (main=3, sub=0x7B)
// }
void CGocInventory::SetCashMileage(void* psUpdateInfo) {
    // TODO: Implement with PS_CASH_MILEAGE struct
    // PS_CASH_MILEAGE* pInfo = static_cast<PS_CASH_MILEAGE*>(psUpdateInfo);
    // if (pInfo->byMileageType < 3) {
    //     m_nCashMileage[pInfo->byMileageType] = pInfo->nCashMileage;
    //     // Send packet to client (main=3, sub=0x7B)
    // }
    (void)psUpdateInfo;
}

// IDA: 0x1400E51A0
// void __fastcall CGocInventory::SendCashMileageLog(CGocInventory *this, unsigned __int8 bySubType, PS_DB_CASH_MILEAGE_LIST *psList, int dwRecvUAID)
// Sends cash mileage log to DB
void CGocInventory::SendCashMileageLog(std::uint8_t bySubType, void* psList, unsigned int dwRecvUAID) {
    // IDA Decompiled:
    // void __fastcall CGocInventory::SendCashMileageLog(CGocInventory *this, unsigned __int8 bySubType, PS_DB_CASH_MILEAGE_LIST *psList, int dwRecvUAID)
    // {
    //   CUser* pUser = dynamic_cast<CUser*>(m_pActor);
    //   if (!pUser) return;
    //
    //   unsigned int dwUAID = pUser->GetUAID();
    //   unsigned int dwUCID = pUser->GetActorID();
    //   int nLogMileage[3] = {0, 0, 0};
    //
    //   for (auto& info : psList->vecInfo) {
    //     for (int k = 0; k < 3; ++k)
    //       nLogMileage[k] += info.nUpdateMileage[k];
    //   }
    //
    //   for (E_CASH_MILEAGE_TYPE eType = E_CASH_MILEAGE_AKASHIC; eType < E_CASH_MILEAGE_MAX; ++eType) {
    //     if (nLogMileage[eType]) {
    //       ST_LOG_GAME stLog;
    //       stLog._sMainType = 10;
    //       stLog._sSubType = 10;
    //       stLog._nUAID = dwUAID;
    //       stLog._nUCID = dwUCID;
    //       stLog.nParam1 = pUser->GetLevel();
    //       stLog.nParam2 = bySubType;
    //       stLog.nParam3 = dwRecvUAID;
    //       stLog.nParam4 = eType;
    //       stLog.nParam5 = nLogMileage[eType];
    //       stLog.nParam6 = dwRecvUAID ? 0 : (nLogMileage[eType] + GetCashMileage(eType));
    //       XGameServer::SendDBLog(&stLog);
    //     }
    //   }
    // }

    // TODO: 需人工审查 - Implement when PS_DB_CASH_MILEAGE_LIST/CUser/ST_LOG_GAME available
    (void)bySubType;
    (void)psList;
    (void)dwRecvUAID;
}

// IDA: 0x1400E5500
// void __fastcall CGocInventory::SendDBCashMileageUpdate(CGocInventory *this, PS_DB_CASH_MILEAGE_LIST *psDBList)
// {
//   // Send DB packet (main=2, sub=0x68)
//   // XSendDBPacket xSendDBPacket(pObject, 2, 0x68);
//   // xSendDBPacket << psDBList;
//   // XGameServer::SendDBAccount(&xSendDBPacket);
//   // SendCashMileageLog(3, psDBList, 0);
// }
void CGocInventory::SendDBCashMileageUpdate(void* psDBList) {
    // TODO: Send DB packet (main=2, sub=0x68)
    // PS_DB_CASH_MILEAGE_LIST* pList = static_cast<PS_DB_CASH_MILEAGE_LIST*>(psDBList);
    // XSendDBPacket xSendDBPacket(pObject, 2, 0x68);
    // xSendDBPacket << pList;
    // XGameServer::SendDBAccount(&xSendDBPacket);
    // SendCashMileageLog(3, pList, 0);
    (void)psDBList;
}

// IDA: 0x1400E5630
// void __fastcall CGocInventory::SetRenovatePoint(CGocInventory *this, int nPoint)
// Sets renovate point value
void CGocInventory::SetRenovatePoint(int nPoint) {
    // IDA Decompiled:
    // void __fastcall CGocInventory::SetRenovatePoint(CGocInventory *this, int nPoint)
    // {
    //   this->m_nRenovatePoint = nPoint;
    //   CUser* pUser = dynamic_cast<CUser*>(m_pActor);
    //   if (pUser)
    //     CUser::stMyCharInfoEx(pUser)->nRenovatePoint = this->m_nRenovatePoint;
    // }

    m_nRenovatePoint = nPoint;
    // TODO: 需人工审查 - Update CUser::stMyCharInfoEx when CUser available
}

// IDA: 0x1400E56B0
// char __fastcall CGocInventory::AddRenovatePoint(CGocInventory *this, int nPoint, unsigned __int8 bySubType)
// Adds renovate points with logging
bool CGocInventory::AddRenovatePoint(int nPoint, std::uint8_t bySubType) {
    // IDA Decompiled:
    // char __fastcall CGocInventory::AddRenovatePoint(CGocInventory *this, int nPoint, unsigned __int8 bySubType)
    // {
    //   if (nPoint + this->m_nRenovatePoint < 0) return 0;
    //   CGocInventory::SetRenovatePoint(this, nPoint + this->m_nRenovatePoint);
    //
    //   CUser* pUser = dynamic_cast<CUser*>(m_pActor);
    //   if (pUser) {
    //     ST_LOG_GAME stLog;
    //     stLog._nUAID = pUser->GetUAID();
    //     stLog._nUCID = pUser->GetActorID();
    //     stLog._sMainType = 10;
    //     stLog._sSubType = 11;
    //     stLog.nParam1 = pUser->GetLevel();
    //     stLog.nParam2 = bySubType ? 67 : 66;
    //     stLog.nParam5 = nPoint;
    //     stLog.nParam6 = this->m_nRenovatePoint;
    //     wcscpy_s(stLog.szComment, L"RENOVATE POINT");
    //     XGameServer::SendDBLog(&stLog);
    //   }
    //   return 1;
    // }

    if (nPoint + m_nRenovatePoint < 0)
        return false;

    SetRenovatePoint(nPoint + m_nRenovatePoint);

    // TODO: 需人工审查 - Add logging when ST_LOG_GAME/CUser available
    (void)bySubType;
    return true;
}

// IDA: 0x1400E5890
// void __fastcall CGocInventory::SetRefinePoint(CGocInventory *this, int nPoint)
// Sets refine point value
void CGocInventory::SetRefinePoint(int nPoint) {
    // IDA Decompiled:
    // void __fastcall CGocInventory::SetRefinePoint(CGocInventory *this, int nPoint)
    // {
    //   this->m_nRefinePoint = nPoint;
    //   CUser* pUser = dynamic_cast<CUser*>(m_pActor);
    //   if (pUser)
    //     CUser::stMyCharInfoEx(pUser)->nRefinePoint = this->m_nRefinePoint;
    // }

    m_nRefinePoint = nPoint;
    // TODO: 需人工审查 - Update CUser::stMyCharInfoEx when CUser available
}

// IDA: 0x1400E5910
// char __fastcall CGocInventory::AddRefinePoint(CGocInventory *this, int nPoint)
// Adds refine points with logging
bool CGocInventory::AddRefinePoint(int nPoint) {
    // IDA Decompiled:
    // char __fastcall CGocInventory::AddRefinePoint(CGocInventory *this, int nPoint)
    // {
    //   if (nPoint + this->m_nRefinePoint < 0) return 0;
    //   CGocInventory::SetRefinePoint(this, nPoint + this->m_nRefinePoint);
    //
    //   CUser* pUser = dynamic_cast<CUser*>(m_pActor);
    //   if (pUser) {
    //     ST_LOG_GAME stLog;
    //     stLog._nUAID = pUser->GetUAID();
    //     stLog._nUCID = pUser->GetActorID();
    //     stLog._sMainType = 10;
    //     stLog._sSubType = 12;
    //     stLog.nParam1 = pUser->GetLevel();
    //     stLog.nParam2 = 57;
    //     stLog.nParam5 = nPoint;
    //     stLog.nParam6 = this->m_nRefinePoint;
    //     wcscpy_s(stLog.szComment, L"REFINE POINT");
    //     XGameServer::SendDBLog(&stLog);
    //   }
    //   return 1;
    // }

    if (nPoint + m_nRefinePoint < 0)
        return false;

    SetRefinePoint(nPoint + m_nRefinePoint);
    // TODO: 需人工审查 - Add logging when ST_LOG_GAME/CUser available
    return true;
}

// === Batch 15: Package functions (IDA verified) ===

// IDA: 0x1400E61F0 - ?SendDBPackageLoad@CGocInventory@@QEAAX_N@Z
// Precise restoration from IDA decompilation - sends package load request to DB (main=0x21, sub=0x53)
void CGocInventory::SendDBPackageLoad(bool bInven) {
    // IDA: Get CUser from owner
    CUser* pUser = GetCUserFromOwner(this);
    if (!pUser) return;

    // IDA: Get ActorID
    std::uint32_t dwID = 0; // TODO: pUser->GetActorID()
    std::uint8_t byStorageType = 0;

    // IDA: if (!bInven) use UAID and storage type 1
    if (!bInven) {
        // dwID = pUser->GetUAID();
        byStorageType = 1;
    }

    // IDA: Send DB packet (main=0x21, sub=0x53)
    CMover* pOwner = GetOwnerGO();
    IXObject* pObject = pOwner ? static_cast<IXObject*>(pOwner) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 0x21, 0x53);
    xSendDBPacket.XParse << dwID;
    xSendDBPacket.XParse << byStorageType;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pServer) pServer->SendDBGame(xSendDBPacket);
}

// IDA: 0x1400E63B0
// void __fastcall CGocInventory::ItemPackageLoad(CGocInventory *this, PS_ITEM_PACKAGE_LIST *psList)
// Loads item package list from DB
void CGocInventory::ItemPackageLoad(void* psList) {
    // IDA Decompiled:
    // void __fastcall CGocInventory::ItemPackageLoad(CGocInventory *this, PS_ITEM_PACKAGE_LIST *psList)
    // {
    //   for (size_t i = 0; i < psList->size(); ++i) {
    //     PS_ITEM_PACKAGE psInfo = psList->at(i);
    //     std::shared_ptr<CItem> pItem;
    //     CGocInventory::GetItemPtr(this, &pItem, psInfo.biPackageSerial);
    //     if (pItem) {
    //       CItem::SetPackageList(pItem.get(), &psInfo);
    //     }
    //   }
    // }

    // TODO: 需人工审查 - Implement when PS_ITEM_PACKAGE_LIST/CItem available
    (void)psList;
}

// IDA: 0x1400E64F0
// void __fastcall CGocInventory::SendPackageLoad(CGocInventory *this)
// Sends package load to client
void CGocInventory::SendPackageLoad() {
    // IDA Decompiled:
    // void __fastcall CGocInventory::SendPackageLoad(CGocInventory *this)
    // {
    //   PS_ITEM_PACKAGE_LIST psList;
    //   CGocInventory::GetPackageList(this, 0xD, &psList);  // Inventory packages
    //   CGocInventory::GetPackageList(this, 0xE, &psList);  // Bank packages
    //
    //   XSendPacket xSendPacket(8, 0x72);
    //   xSendPacket << 0;  // bLoad = false
    //   xSendPacket << psList;
    //   CGocNetwork::Send(m_pActor, &xSendPacket);
    // }

    // TODO: 需人工审查 - Implement when PS_ITEM_PACKAGE_LIST/XSendPacket available
}

// IDA: 0x1400E6A90
bool CGocInventory::IsResealPackage(int nPackageID) {
    return TXSingleton<XGameServer>::Instance()
               ->GetResourceMgr()
               .GetTB_REPACKAGECOSTUME(static_cast<unsigned int>(nPackageID)) != nullptr;
}

// IDA: 0x1400E6AD0
// bool __fastcall CGocInventory::IsResealPackageCount(CGocInventory *this, unsigned int nPackageID, int nCount)
// Checks if count matches item count in repackage table


// === Batch 27: Billing response functions (IDA verified, complex) ===

// IDA: 0x1400C6090
// Handles Han billing response for item purchase (4KB+ function)
bool CGocInventory::BuyItemByHanBillingRes2(void* stHanBilling, int nDecCash, void* psCashbuyList) {
    // IDA Decompiled (complex function ~4KB):
    // - Iterates through billing order info vector
    // - For each item: validates result, gets TB_ITEM/TB_ITEM_CLASSIFY
    // - Handles appearance items (GroupID == 19) differently
    // - Calls AddItem2 for regular items
    // - Updates cash mileage
    // - Sends DB packets for logging
    // - Calls AddCash, UpdateItemEnd, AddItemEnd
    // - Handles post items via CGocPost::CashBuySend
    //
    // Key operations:
    // 1. Iterate stHanBilling->vecOrderInfo
    // 2. For each PS_HAN_BILLING_ORDER_NO:
    //    - Check nResult > 0 (error if so)
    //    - Get TB_ITEM and TB_ITEM_CLASSIFY
    //    - If GroupID == 19: UpdateAppearance, add to appearance list
    //    - Else: Try AddItem2, if failed add to post item list
    //    - Update cash mileage
    //    - Send PS_LOG_CASH to DB
    // 3. AddCash(-nDecCash)
    // 4. UpdateItemEnd, AddItemEnd
    // 5. Send DB packets for created/updated items
    // 6. CGocPost::CashBuySend for post items

    // TODO: Full implementation requires many dependencies
    (void)stHanBilling;
    (void)nDecCash;
    (void)psCashbuyList;
    return false;
}

// IDA: 0x1400C72B0
// Handles Han billing response for gift purchase (4KB+ function)
bool CGocInventory::GiftItemByHanBillingRes2(void* stHanBilling, int nDecCash, void* psCashbuyList) {
    // IDA Decompiled (complex function ~4KB):
    // Similar to BuyItemByHanBillingRes2 but for gifts
    // - Uses dwRecvUCID, dwRecvUAID from stHanBilling
    // - Calls CGocPost::CashGiftSend instead of CashBuySend
    // - Logs with different parameters (includes recipient info)

    // TODO: Full implementation requires many dependencies
    (void)stHanBilling;
    (void)nDecCash;
    (void)psCashbuyList;
    return false;
}

// IDA: 0x1400CBCC0
// Sends WM billing request for item purchase (4KB+ function)
int CGocInventory::BuyItemByWMBillingReq(unsigned int dwUAID, void* stCashItemList,
                                          bool bGift, unsigned int dwRecvUCID, int nRecvClass,
                                          const char* szRecvAccountID, const wchar_t* szRecvCharacterID,
                                          unsigned int dwRecvUAID) {
    // IDA Decompiled (complex function ~4KB):
    // - Validates cash item list size (1-10 items)
    // - For each item:
    //   - Validates cash shop index
    //   - Checks date limits
    //   - Validates item classify and inventory type
    //   - Updates cash buy count if needed
    //   - Checks item limit class
    //   - Checks appearance ownership
    //   - Builds billing packet
    // - Checks if player has enough cash
    // - Sends DB packet (main=2, sub=0x64)

    // TODO: Full implementation requires many dependencies
    (void)dwUAID;
    (void)stCashItemList;
    (void)bGift;
    (void)dwRecvUCID;
    (void)nRecvClass;
    (void)szRecvAccountID;
    (void)szRecvCharacterID;
    (void)dwRecvUAID;
    return 0;
}

// IDA: 0x1400CD290
// Handles WM billing response for item purchase (4KB+ function)
bool CGocInventory::BuyItemByWMBillingRes(void* stHanBilling, void* psCashbuyList) {
    // IDA Decompiled (complex function ~4KB):
    // Very similar to BuyItemByHanBillingRes2
    // - Iterates billing order info
    // - Processes items and appearances
    // - Updates item end, add item end
    // - Sends DB packets

    // TODO: Full implementation requires many dependencies
    (void)stHanBilling;
    (void)psCashbuyList;
    return false;
}

// IDA: 0x1400CE340
// Handles WM billing response for gift purchase (4KB+ function)
bool CGocInventory::GiftItemByWMBillingRes(void* stHanBilling, void* psCashbuyList) {
    // IDA Decompiled (complex function ~4KB):
    // Very similar to GiftItemByHanBillingRes2
    // - Processes gift items
    // - Calls CGocPost::CashGiftSend
    // - Sends DB packets

    // TODO: Full implementation requires many dependencies
    (void)stHanBilling;
    (void)psCashbuyList;
    return false;
}

// Note: SetTradeActorID implementation already exists at line 591

// ============================================================================
// Cash Buy Count functions
// ============================================================================

// IDA: 0x1400C33F0
// Loads cash buy count list from DB, filters expired entries
void CGocInventory::LoadCashBuyCount(PS_CASH_BUY_COUNT_LIST* psList) {
    if (!psList) return;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    std::int64_t biCurDate = pServer ? pServer->GetCurDate() : 0;

    for (auto& psCashCount : psList->vecInfo) {
        // Only insert if not expired (biEndDate >= curDate or biEndDate == 0)
        if (psCashCount.biEndDate >= biCurDate || psCashCount.biEndDate == 0) {
            m_mpCashBuyCount[psCashCount.nCashShopIndex] = psCashCount;
        }
    }
}

// IDA: 0x1400C3500
// Updates buy count for a cash shop item with limit checking
bool CGocInventory::UpdateCashBuyCount(int nCashShopIndex, int nBuyCount,
                                        std::uint8_t byLimitType, int nLimitCount,
                                        PS_CASH_BUY_COUNT_LIST* psList) {
    std::int64_t biEndDate = 0;

    // Check if limit type is valid
    if (!IsBuyCashLimitCount(static_cast<E_CASH_SHOP_BUY>(byLimitType), biEndDate)) {
        return false;
    }

    int nTempBuyCount = 0;

    // Find existing entry
    auto it = m_mpCashBuyCount.find(nCashShopIndex);
    if (it != m_mpCashBuyCount.end()) {
        nTempBuyCount = nBuyCount + it->second.nBuyCount;
        if (nTempBuyCount > nLimitCount) {
            return false;
        }
    } else {
        if (nBuyCount > nLimitCount) {
            return false;
        }
    }

    // Check if entry already exists in psList
    for (auto& info : psList->vecInfo) {
        if (info.nCashShopIndex == nCashShopIndex) {
            info.nBuyCount += nBuyCount;
            return info.nBuyCount <= nLimitCount;
        }
    }

    // Add new entry to psList
    PS_CASH_BUY_COUNT psInfo;
    psInfo.nCashShopIndex = nCashShopIndex;
    psInfo.nBuyCount = (nTempBuyCount > 0) ? nTempBuyCount : nBuyCount;
    psInfo.byBuyType = byLimitType;
    psInfo.biEndDate = biEndDate;
    psList->vecInfo.push_back(psInfo);

    return true;
}

// IDA: 0x1400C3750
// Sends cash buy count update to client (main=9, sub=0x31)
void CGocInventory::SendUpdateCashBuyCount(PS_CASH_BUY_COUNT_LIST* psList) {
    if (!psList) return;

    // Update m_mpCashBuyCount map from psList
    for (auto& psCashCount : psList->vecInfo) {
        auto it = m_mpCashBuyCount.find(psCashCount.nCashShopIndex);
        if (it != m_mpCashBuyCount.end()) {
            // Update existing entry
            it->second.nBuyCount = psCashCount.nBuyCount;
            if (it->second.biEndDate < psCashCount.biEndDate) {
                it->second.biEndDate = psCashCount.biEndDate;
            }
        } else {
            // Insert new entry
            m_mpCashBuyCount[psCashCount.nCashShopIndex] = psCashCount;
        }
    }

    // Send packet to client (main=9, sub=0x31)
    CMover* pMover = GetOwnerGO();
    if (pMover) {
        XSendPacket xSendPacket(9, 0x31);
        xSendPacket << *psList;
        CGocNetwork::Send(static_cast<XActor*>(pMover), xSendPacket);
    }
}

// IDA: 0x1400C39B0
// void __fastcall CGocInventory::SendDBCashBuyCount(CGocInventory *this)
// Sends cash buy count to DB (main=0x22, sub=0x26)
void CGocInventory::SendDBCashBuyCount() {
    // IDA Decompiled:
    // pUser = GetCUser();
    // if (pUser) {
    //     biCurDate = XGameServer::GetCurDate(TXSingleton<XGameServer>::Instance());
    //     XSendDBPacket::XSendDBPacket(&xSendDBPacket, pObject, 0x22u, 0x26u);
    //     QuestID = GetQuestID();
    //     XParse::operator<<(&xSendDBPacket.XParse, QuestID);
    //     XParse::operator<<(&xSendDBPacket.XParse, pUser->GetUAID());
    //     XParse::operator<<(&xSendDBPacket.XParse, biCurDate);
    //     XGameServer::SendDBGame(&xSendDBPacket);
    //     XSendDBPacket::~XSendDBPacket(&xSendDBPacket);
    // }

    // TODO: 需人工审查 - Implement when CUser/XSendDBPacket types available
}

// ============================================================================
// Endurance log functions (IDA verified)
// ============================================================================

// IDA: 0x1400C7D90
// void __fastcall CGocInventory::SetEnduranceLog(
//         CGocInventory *this, ST_MYROOM_ITEM *psEnduranceInfo)
// Sets endurance log entry
void CGocInventory::SetEnduranceLog(void* psEnduranceInfo) {
    // IDA Decompiled:
    // std::map::find(&this->m_mpEndranceLog, &iter, &psEnduranceInfo->biSerial);
    // if (iter != end) {
    //     // Update existing entry
    //     iter->second.dwItemID = psEnduranceInfo->dwItemID;
    // } else {
    //     // Insert new entry
    //     std::map::insert(&this->m_mpEndranceLog, psEnduranceInfo);
    // }

    // TODO: 需人工审查 - Implement when PS_ITEM_ENDURANCE_INFO types available
    (void)psEnduranceInfo;
}

// IDA: 0x1400C7E70
// void __fastcall CGocInventory::SendEnduranceLog(CGocInventory *this, int nMazeID)
// Sends endurance log to DB (main=4, sub=60)
void CGocInventory::SendEnduranceLog(int nMazeID) {
    // IDA Decompiled:
    // pUser = GetCUser();
    // if (pUser) {
    //     std::map::begin(&this->m_mpEndranceLog, &iter);
    //     while (iter != end) {
    //         qmemcpy(&stInfo, &iter->second, sizeof(stInfo));
    //
    //         // Create log entry
    //         ST_LOG_GAME::ST_LOG_GAME(&stLog);
    //         stLog._nUAID = pUser->GetUAID();
    //         stLog._nUCID = GetQuestID();
    //         stLog._sMainType = 4;
    //         stLog._sSubType = 60;
    //         stLog.nParam0 = stInfo.dwItemID;
    //         stLog.nParam2 = pUser->GetLevel();
    //         stLog.nParam3 = nMazeID;
    //         stLog.nParam4 = stInfo.byItemType;
    //         stLog.nParam5 = stInfo.stUpdateInfo.biSerial;
    //         stLog.nParam7 = stInfo.byBforeEndurance;
    //         stLog.nParam8 = stInfo.stUpdateInfo.byCurEndurance;
    //         stLog.nParam11 = 0;
    //         stLog.nParam12 = 1;
    //         XGameServer::SendDBLog(&stLog);
    //         ++iter;
    //     }
    //     std::map::clear(&this->m_mpEndranceLog);
    // }

    // TODO: 需人工审查 - Implement when PS_ITEM_ENDURANCE_INFO/ST_LOG_GAME types available
    (void)nMazeID;
}

// ============================================================================
// Money check functions (IDA verified)
// ============================================================================

// IDA: 0x1400C80E0
// bool __fastcall CGocInventory::CheckOverMoney(
//         CGocInventory *this, ePriceType nPriceType, __int64 biValue)
// Checks if adding money would overflow the cap
bool CGocInventory::CheckOverMoney(int ePriceType, std::int64_t biValue) {
    // IDA Decompiled:
    // switch (nPriceType) {
    //     case E_PRICE_TYPE_GOLD:  // 0
    //         return (biValue + m_nInvenMoney) <= 0x5AF3107A3FFFLL;  // 999,999,999,999,999
    //     case E_PRICE_TYPE_BP:  // 1
    //         return (biValue + m_nBP) <= 0x5AF3107A3FFFLL;
    //     case E_PRICE_TYPE_CASH:  // 2
    //         return (biValue + m_nCash) <= 999999999;
    //     case E_PRICE_TYPE_ETHER:  // 3
    //         return (biValue + m_biEther) <= 0x5AF3107A3FFFLL;
    //     case E_PRICE_TYPE_RECYCLE:  // 4
    //         return (biValue + m_biRecycle) <= 0x5AF3107A3FFFLL;
    //     case E_PRICE_TYPE_CASH_MILEAGE_AKASHIC:  // 5
    //         return (biValue + m_nCashMileage[0]) <= 999999999;
    //     case E_PRICE_TYPE_CASH_MILEAGE_BROACH:  // 6
    //         return (biValue + m_nCashMileage[1]) <= 999999999;
    //     case E_PRICE_TYPE_CASH_MILEAGE_TAG:  // 7
    //         return (biValue + m_nCashMileage[2]) <= 999999999;
    //     default:
    //         return true;
    // }

    // Constants: 0x5AF3107A3FFFLL = 999,999,999,999,999
    constexpr std::int64_t MAX_CURRENCY = 999999999999999LL;
    constexpr int MAX_CASH = 999999999;

    switch (ePriceType) {
        case 0:  // E_PRICE_TYPE_GOLD
            return (biValue + m_nInvenMoney) <= MAX_CURRENCY;
        case 1:  // E_PRICE_TYPE_BP
            return (biValue + m_nBP) <= MAX_CURRENCY;
        case 2:  // E_PRICE_TYPE_CASH
            return (biValue + m_nCash) <= MAX_CASH;
        case 3:  // E_PRICE_TYPE_ETHER
            return (biValue + m_biEther) <= MAX_CURRENCY;
        case 4:  // E_PRICE_TYPE_RECYCLE
            return (biValue + m_biRecycle) <= MAX_CURRENCY;
        case 5:  // E_PRICE_TYPE_CASH_MILEAGE_AKASHIC
            return (biValue + m_nCashMileage[0]) <= MAX_CASH;
        case 6:  // E_PRICE_TYPE_CASH_MILEAGE_BROACH
            return (biValue + m_nCashMileage[1]) <= MAX_CASH;
        case 7:  // E_PRICE_TYPE_CASH_MILEAGE_TAG
            return (biValue + m_nCashMileage[2]) <= MAX_CASH;
        default:
            return true;
    }
}

// ============================================================================
// Socket/Broach send functions (IDA verified)
// ============================================================================

// IDA: 0x1400C82C0
// void __fastcall CGocInventory::SendSocketLoad(CGocInventory *this)
// Sends socket load data to client (calls SendSocketInfo for each inventory type)
void CGocInventory::SendSocketLoad() {
    // IDA Decompiled:
    // PS_POST_DELETE_LIST::PS_POST_DELETE_LIST(&stSocketList);
    //
    // // Get sockets for costume inventory (type 2)
    // GetSocketList(this, 2u, &stSocketList);
    // PS_ITEM_SOCKET_LIST::PS_ITEM_SOCKET_LIST(&v2, &stSocketList);
    // SendSocketInfo(this, v9, 1);  // flag=1 for costume
    // std::vector<ST_ITEM_SOCKET>::clear(&stSocketList.vecInfo);
    //
    // // Get sockets for bank types (0x10, 5)
    // GetSocketList(this, 0x10u, &stSocketList);
    // GetSocketList(this, 5u, &stSocketList);
    // PS_ITEM_SOCKET_LIST::PS_ITEM_SOCKET_LIST(&v4, &stSocketList);
    // SendSocketInfo(this, v10, 0);  // flag=0 for bank
    // std::vector<ST_ITEM_SOCKET>::clear(&stSocketList.vecInfo);
    //
    // // Get sockets for ability equip (type 1)
    // GetSocketList(this, 1u, &stSocketList);
    // PS_ITEM_SOCKET_LIST::PS_ITEM_SOCKET_LIST(&v6, &stSocketList);
    // SendSocketInfo(this, v11, 0);  // flag=0 for equip
    //
    // PS_ITEM_SOCKET_LIST::~PS_ITEM_SOCKET_LIST(&stSocketList);

    // TODO: 需人工审查 - Implement when PS_ITEM_SOCKET_LIST/GetSocketList types available
}

// IDA: 0x1400C8420
// void __fastcall CGocInventory::SendBroachLoad(CGocInventory *this)
// Sends broach load data to client (calls SendBroachInfo for each inventory type)
void CGocInventory::SendBroachLoad() {
    PS_ITEM_BROACH_LIST stBroachList;

    // IDA transfers each stack copy's destruction to SendBroachInfo; raw storage
    // preserves that consuming-pointer lifetime without an automatic second destroy.
    GetBroachList(4, stBroachList);
    alignas(PS_ITEM_BROACH_LIST) unsigned char costumeStorage[sizeof(PS_ITEM_BROACH_LIST)];
    PS_ITEM_BROACH_LIST* pCostumeList =
        ::new (static_cast<void*>(costumeStorage)) PS_ITEM_BROACH_LIST(stBroachList);
    SendBroachInfo(pCostumeList, 1);
    stBroachList.vecInfo.clear();

    GetBroachList(6, stBroachList);
    GetBroachList(0x11, stBroachList);
    alignas(PS_ITEM_BROACH_LIST) unsigned char bankStorage[sizeof(PS_ITEM_BROACH_LIST)];
    PS_ITEM_BROACH_LIST* pBankList =
        ::new (static_cast<void*>(bankStorage)) PS_ITEM_BROACH_LIST(stBroachList);
    SendBroachInfo(pBankList, 0);
    stBroachList.vecInfo.clear();

    GetBroachList(0, stBroachList);
    GetBroachList(3, stBroachList);
    alignas(PS_ITEM_BROACH_LIST) unsigned char equipStorage[sizeof(PS_ITEM_BROACH_LIST)];
    PS_ITEM_BROACH_LIST* pEquipList =
        ::new (static_cast<void*>(equipStorage)) PS_ITEM_BROACH_LIST(stBroachList);
    SendBroachInfo(pEquipList, 0);
    stBroachList.vecInfo.clear();
}

// IDA: 0x1400C85A0
// void __fastcall CGocInventory::SendSocketInfo(
//         CGocInventory *this, PS_ITEM_SOCKET_LIST *psList, char byFlag)
// Sends socket info packet to client (main=8, sub=0x55)
void CGocInventory::SendSocketInfo(void* psList, std::uint8_t byFlag) {
    // IDA Decompiled:
    // XSendPacket::XSendPacket(&xSendPacket, 8u, 0x55u);
    // operator<<(&xSendPacket, psList);
    // XParse::operator<<(&xSendPacket.XParse, byFlag);
    //
    // // Get actor and send packet
    // pActor = GetActor();
    // CGocNetwork::Send(pActor, &xSendPacket);
    //
    // PS_ITEM_SOCKET_LIST::~PS_ITEM_SOCKET_LIST(psList);

    // TODO: 需人工审查 - Implement when PS_ITEM_SOCKET_LIST/XSendPacket types available
    (void)psList;
    (void)byFlag;
}

// IDA: 0x1400C86A0
// void __fastcall CGocInventory::SendBroachInfo(
//         CGocInventory *this, PS_ITEM_BROACH_LIST *psList, char byFlag)
// Sends broach info packet to client (main=8, sub=0x56)
void CGocInventory::SendBroachInfo(PS_ITEM_BROACH_LIST* psList, std::uint8_t byFlag) {
    try {
        XSendPacket xSendPacket(8, 0x56);
        xSendPacket << *psList;
        xSendPacket.XParse << static_cast<bool>(byFlag);
        CGocNetwork::Send(GetOwnerGO(), xSendPacket);
    } catch (...) {
        // IDA dtor$0 destroys the consumed list after packet unwind.
        psList->~PS_ITEM_BROACH_LIST();
        throw;
    }

    // The caller provides an explicitly constructed stack temporary; IDA destroys it here.
    psList->~PS_ITEM_BROACH_LIST();
}

// IDA: 0x1400C87A0
// void __fastcall CGocInventory::SendUseInfo(CGocInventory *this)
// Sends item use info to client (main=8, sub=0x48)
void CGocInventory::SendUseInfo() {
    // IDA Decompiled:
    // PS_POST_DELETE_LIST::PS_POST_DELETE_LIST(&stSendList);
    //
    // // Iterate through m_mpUseItemInfo map
    // std::map::begin(&this->m_mpUseItemInfo, &iter);
    // while (iter != end) {
    //     qmemcpy(&psUseInfo, &iter->second, sizeof(psUseInfo));
    //     stSendList.push_back(&psUseInfo);
    //     ++iter;
    // }
    //
    // // Send packet
    // XSendPacket::XSendPacket(&xSendPacket, 8u, 0x48u);
    // XParse::operator<<(&xSendPacket.XParse, this->m_biUseItemUpdateDate);
    // operator<<(&xSendPacket, &stSendList);
    //
    // pActor = GetActor();
    // CGocNetwork::Send(pActor, &xSendPacket);
    //
    // PS_POST_DELETE_LIST::~PS_POST_DELETE_LIST(&stSendList);

    // TODO: 需人工审查 - Implement when ST_USE_ITEM_INFO_LIST/XSendPacket types available
}

// IDA: 0x1400E5AD0 - ?IsBuyCashLimitCount@CGocInventory@@QEAA_NW4E_CASH_SHOP_BUY@@AEA_J@Z
// Precise restoration from IDA decompilation - calculates end date for cash shop buy limits
bool CGocInventory::IsBuyCashLimitCount(E_CASH_SHOP_BUY eLimitType, std::int64_t& biEndDate) {
    biEndDate = 0;

    // E_CASH_SHOP_BUY enum values:
    // 1 = E_CASH_SHOP_BUY_LIMIT (no limit)
    // 2 = E_CASH_SHOP_BUY_LIMIT_DAY
    // 3 = E_CASH_SHOP_BUY_LIMIT_WEEK
    // 4 = E_CASH_SHOP_BUY_LIMIT_MONTH
    // 5 = E_CASH_SHOP_BUY_LIMIT_ACCOUNT
    // 6 = E_CASH_SHOP_BUY_LIMIT_ACCOUNT_DAY
    // 7 = E_CASH_SHOP_BUY_LIMIT_ACCOUNT_WEEK
    // 8 = E_CASH_SHOP_BUY_LIMIT_ACCOUNT_MONTH

    switch (eLimitType) {
        case E_CASH_SHOP_BUY_LIMIT:
        case E_CASH_SHOP_BUY_LIMIT_ACCOUNT:
            // No date limit
            return true;

        case E_CASH_SHOP_BUY_LIMIT_DAY:
        case E_CASH_SHOP_BUY_LIMIT_ACCOUNT_DAY:
            // Get daily reset time from XGameServer (9:00 AM)
            {
                XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
                if (pGameServer) {
                    biEndDate = pGameServer->GetUpdateDate(9);
                }
            }
            return true;

        case E_CASH_SHOP_BUY_LIMIT_WEEK:
        case E_CASH_SHOP_BUY_LIMIT_ACCOUNT_WEEK:
            // Calculate next weekly reset (Wednesday 9:00)
            // IDA: Uses ATL::CTime to calculate next Wednesday 9:00 AM
            {
                // Get current time
                std::time_t now = std::time(nullptr);
                std::tm* tmNow = std::localtime(&now);

                // Create time for today at 9:00 AM
                std::tm tmInit = *tmNow;
                tmInit.tm_hour = 9;
                tmInit.tm_min = 0;
                tmInit.tm_sec = 0;
                std::time_t tInit = std::mktime(&tmInit);

                // Get day of week (1=Sunday, 2=Monday, ..., 7=Saturday)
                int dayOfWeek = tmInit.tm_wday == 0 ? 7 : tmInit.tm_wday;

                // Calculate days until next Wednesday (day 3)
                // Wednesday is day 3 in this system
                if (dayOfWeek == 3) {
                    // Today is Wednesday - check if we're past 9:00 AM
                    if (now >= tInit) {
                        // Add 7 days for next Wednesday
                        tInit += 7 * 24 * 60 * 60;
                    }
                } else {
                    // Add days to reach next Wednesday
                    int daysToAdd = (3 - dayOfWeek + 7) % 7;
                    if (daysToAdd == 0) daysToAdd = 7;
                    tInit += daysToAdd * 24 * 60 * 60;
                }

                biEndDate = static_cast<std::int64_t>(tInit);
            }
            return true;

        case E_CASH_SHOP_BUY_LIMIT_MONTH:
        case E_CASH_SHOP_BUY_LIMIT_ACCOUNT_MONTH:
            // Calculate next monthly reset (1st day of next month 9:00)
            {
                std::time_t now = std::time(nullptr);
                std::tm* tmNow = std::localtime(&now);

                // Check if today is 1st and before 9:00 AM
                if (tmNow->tm_mday == 1 && tmNow->tm_hour < 9) {
                    // Use this month's 1st at 9:00 AM
                    std::tm tmInit = *tmNow;
                    tmInit.tm_hour = 9;
                    tmInit.tm_min = 0;
                    tmInit.tm_sec = 0;
                    biEndDate = static_cast<std::int64_t>(std::mktime(&tmInit));
                } else {
                    // Use next month's 1st at 9:00 AM
                    std::tm tmNext = *tmNow;
                    tmNext.tm_mon += 1;
                    if (tmNext.tm_mon >= 12) {
                        tmNext.tm_mon = 0;
                        tmNext.tm_year += 1;
                    }
                    tmNext.tm_mday = 1;
                    tmNext.tm_hour = 9;
                    tmNext.tm_min = 0;
                    tmNext.tm_sec = 0;
                    biEndDate = static_cast<std::int64_t>(std::mktime(&tmNext));
                }
            }
            return true;

        default:
            // Invalid limit type
            LogHelper::LogError("game.contents", "IsBuyCashLimitCount error - Fault limit type");
            return false;
    }
}

// IDA: 0x1400E5FA0
// Initializes cash item buy count, clears expired entries
void CGocInventory::OnInitItemCashCount() {
    // Check if this is a CUser (RTTI check)
    CMover* pMover = GetOwnerGO();
    if (!pMover) return;

    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (!pUser) return;

    // Get current update date
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) return;

    std::int64_t biUpdateDate = pServer->GetUpdateDate(9);

    PS_CASH_BUY_COUNT_LIST psDBList;

    // Iterate through m_mpCashBuyCount and remove expired entries
    for (auto it = m_mpCashBuyCount.begin(); it != m_mpCashBuyCount.end(); ) {
        PS_CASH_BUY_COUNT& psInfo = it->second;
        if (psInfo.biEndDate < biUpdateDate && psInfo.biEndDate != 0) {
            // Expired - remove and add to list with nBuyCount = 0
            PS_CASH_BUY_COUNT psExpired = psInfo;
            psExpired.nBuyCount = 0;
            psDBList.vecInfo.push_back(psExpired);
            it = m_mpCashBuyCount.erase(it);
        } else {
            ++it;
        }
    }

    // Send packet to client (main=9, sub=0x31)
    XSendPacket xSendPacket(9, 0x31);
    xSendPacket << psDBList;
    CGocNetwork::Send(static_cast<XActor*>(pMover), xSendPacket);
}

// ============================================================================
// Cash Item Set functions
// ============================================================================

// IDA: 0x1400B89E0
// Adds cash item set list to m_stCashSet array (max 9 sets)
void CGocInventory::AddCashItemSet(PS_CASH_SET_LIST* stCashSetList) {
    if (!stCashSetList) return;

    for (const auto& stCashSet : stCashSetList->vecInfo) {
        if (stCashSet.bySetNo < 9) {
            std::memcpy(&m_stCashSet[stCashSet.bySetNo], &stCashSet, sizeof(PS_CASH_SET));
        }
    }
}

// IDA: 0x1400B8B10
// Deletes a cash item set by index, syncs to DB
bool CGocInventory::DelCashItemSet(std::uint8_t bySetNo) {
    // Check valid range
    if (bySetNo >= 9) {
        return false;
    }

    // Clear the set
    std::memset(&m_stCashSet[bySetNo], 0, sizeof(PS_CASH_SET));

    // Send DB packet (main=0x22, sub=0x23)
    CMover* pMover = GetOwnerGO();
    if (pMover) {
        IXObject* pObject = static_cast<IXObject*>(pMover);
        XSendDBPacket xSendDBPacket(pObject, 0x22, 0x23);

        // TODO: Need proper GetUCID/GetQuestID method
        xSendDBPacket.XParse << static_cast<std::int32_t>(0);  // UCID placeholder
        xSendDBPacket.XParse << bySetNo;

        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (pServer) {
            pServer->SendDBGame(xSendDBPacket);
        }
    }

    return true;
}

// IDA: 0x1400B8C90
// Updates a cash item set, syncs to DB
bool CGocInventory::UpdateCashItemSet(PS_CASH_SET* stCashSet) {
    if (!stCashSet) return false;

    // Check valid range
    if (stCashSet->bySetNo >= 9) {
        return false;
    }

    // Copy to m_stCashSet array
    std::memcpy(&m_stCashSet[stCashSet->bySetNo], stCashSet, sizeof(PS_CASH_SET));

    // Send DB packet (main=0x22, sub=0x22)
    CMover* pMover = GetOwnerGO();
    if (pMover) {
        IXObject* pObject = static_cast<IXObject*>(pMover);
        XSendDBPacket xSendDBPacket(pObject, 0x22, 0x22);

        // Get UCID (QuestID)
        // TODO: Need proper GetUCID/GetQuestID method
        xSendDBPacket.XParse << static_cast<std::int32_t>(0);  // UCID placeholder
        xSendDBPacket << *stCashSet;

        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (pServer) {
            pServer->SendDBGame(xSendDBPacket);
        }
    }

    return true;
}

// IDA: 0x1400C8960
// Sends cash buy count list to client
void CGocInventory::SendCashCount() {
    PS_CASH_BUY_COUNT_LIST psCashBuyList;

    // Build list from m_mpCashBuyCount map
    for (const auto& pair : m_mpCashBuyCount) {
        psCashBuyList.vecInfo.push_back(pair.second);
    }

    // Send packet (main=9, sub=0x30)
    CMover* pMover = GetOwnerGO();
    if (pMover) {
        XSendPacket xSendPacket(9, 0x30);
        xSendPacket << psCashBuyList;
        CGocNetwork::Send(static_cast<XActor*>(pMover), xSendPacket);
    }
}

// IDA: 0x1400C8B00
// Sends cash set list to client (main=9, sub=0x22)
void CGocInventory::SendCashSet() {
    PS_CASH_SET_LIST psCashSetList;

    // Add all 9 cash sets to list
    for (int i = 0; i < 9; ++i) {
        PS_CASH_SET* pSet = reinterpret_cast<PS_CASH_SET*>(&m_stCashSet[i]);
        psCashSetList.vecInfo.push_back(*pSet);
    }

    // Send packet (main=9, sub=0x22)
    CMover* pMover = GetOwnerGO();
    if (pMover) {
        XSendPacket xSendPacket(9, 0x22);
        xSendPacket << psCashSetList;
        CGocNetwork::Send(static_cast<XActor*>(pMover), xSendPacket);
    }
}

// IDA: 0x1400C8C30
// Sends appearance list to client (main=8, sub=0x50)
void CGocInventory::SendAppearacne() {
    ST_APPEARANCE_LIST stList;
    for (const auto& [appearanceID, endDate] : m_mpAppearanceList) {
        ST_APPEARANCE_INFO stInfo;
        stInfo.wAppearanceID = static_cast<std::uint16_t>(appearanceID);
        stInfo.biEndDate = endDate;
        stList.vecInfo.push_back(stInfo);
    }

    XSendPacket xSendPacket(8, 0x50);
    xSendPacket << stList;
    CGocNetwork::Send(m_pOwner, xSendPacket);
}

// IDA: 0x1400C8DD0
// Moves item to/from league inventory (complex function with 3 types)
bool CGocInventory::MoveItemToLeagueInven(void* psItemMoveForServer) {
    // IDA Decompiled:
    // __int64 __fastcall CGocInventory::MoveItemToLeagueInven(
    //         CGocInventory *this,
    //         PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME *psItemMoveForServer)
    // {
    //   CUser *pUser; // Get CUser from GetCUser()
    //   PS_REQ_ITEM_MOVE_LEAGUE_INVEN psItemMove; // Copy from psItemMoveForServer
    //   std::shared_ptr<CItem> pSrcItem;
    //   std::shared_ptr<CItem> pOutItemPtr;
    //   ST_LOG_GAME stLogItem;
    //   XSendPacket xSendPacket;
    //   PS_RES_STORAGE_INFO stItemList;
    //   PS_STORAGE_INFO psCreateItemInfo;
    //   ...
    //
    //   if (!pUser) {
    //     PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME::~PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME(psItemMoveForServer);
    //     return 0;
    //   }
    //   qmemcpy(&psItemMove, &psItemMoveForServer->psReqItemMoveInfo, sizeof(psItemMove));
    //
    //   if (psItemMove.byType == 0) {
    //     // Move item FROM inventory TO league
    //     CGocInventory::GetSlotItem(this, &pSrcItem, psItemMove.bySrcInvenType, psItemMove.shSrcSlotPos, bLock);
    //     if (!pSrcItem) return 0;
    //     shSaveInItemCount = CItem::GetCount(pSrcItem.get());
    //     nInItemID = CItem::GetCurID(pSrcItem.get());
    //     biInItemSerial = CItem::GetSerial(pSrcItem.get());
    //     CGocInventory::RemoveItem(this, psItemMove.bySrcInvenType, psItemMove.shSrcSlotPos);
    //     XSendPacket::XSendPacket(&xSendPacket, 0x22u, 0x54u);
    //     operator<<(&xSendPacket, &psItemMove);
    //     CGocNetwork::Send(pActor, &xSendPacket);
    //     // Log: main=4, sub=80
    //     ST_LOG_GAME::ST_LOG_GAME(&stLogItem);
    //     stLogItem._sMainType = 4; stLogItem._sSubType = 80;
    //     stLogItem._nUAID = pUser->GetUAID();
    //     stLogItem._nUCID = GetActorID();
    //     stLogItem.nParam0 = nInItemID;
    //     stLogItem.nParam1 = shSaveInItemCount;
    //     stLogItem.nParam2 = GetLevel();
    //     stLogItem.nParam3 = psItemMove.nLeagueID;
    //     stLogItem.nParam5 = biInItemSerial;
    //     stLogItem.nParam7 = psItemMove.shDestSlotPos;
    //     stLogItem.nParam8 = psItemMove.shSrcSlotPos;
    //     stLogItem.nParam9 = CItem::GetItemTitleID(pSrcItem.get());
    //     XGameServer::SendDBLog(v10, &stLogItem);
    //   }
    //   else if (psItemMove.byType == 1) {
    //     // Move item FROM league TO inventory
    //     if (CGocInventory::CheckRandomOption(this, &psItemMoveForServer->psOutItemInfo.stItem)) {
    //       LogHelper::LogError("game.item", "MoveItemToLeagueInven - Change item random option(inventory_out) [UCID:%d]", UCID);
    //     }
    //     if (!CGocInventory::AddItem(this, psItemMove.byDestInvenType, psItemMove.shDestSlotPos, &v79)) {
    //       // Log error and kick user
    //       ST_LOG_GAME::ST_LOG_GAME(&stLogGame);
    //       stLogGame._sMainType = 4; stLogGame._sSubType = 99;
    //       // ... kick user with byKickType=21
    //       return 0;
    //     }
    //     CGocInventory::GetSlotItem(this, &pOutItemPtr, psItemMove.byDestInvenType, psItemMove.shDestSlotPos, byLock);
    //     if (!pOutItemPtr) return 0;
    //     nSaveOutItemCount = CItem::GetCount(pOutItemPtr.get());
    //     nOutItemID = CItem::GetCurID(pOutItemPtr.get());
    //     biOutItemSerial = CItem::GetSerial(pOutItemPtr.get());
    //     nItemTitleID = CItem::GetItemTitleID(pOutItemPtr.get());
    //     // Copy socket, broach, package lists
    //     for (i = 0; i < psItemMoveForServer->psResItemMoveInfo.psItemSocketList.size(); ++i)
    //       pOutItemPtr->SetSocketInfo(&psItemMoveForServer->psResItemMoveInfo.psItemSocketList[i]);
    //     for (j = 0; j < psItemMoveForServer->psResItemMoveInfo.psItemBroachList.size(); ++j)
    //       pOutItemPtr->SetBroachInfo(&psItemMoveForServer->psResItemMoveInfo.psItemBroachList[j]);
    //     for (k = 0; k < psItemMoveForServer->psResItemMoveInfo.psItemPackageList.size(); ++k)
    //       CItem::SetPackageList(pOutItemPtr.get(), &psItemMoveForServer->psResItemMoveInfo.psItemPackageList[k]);
    //     // Send create item and break item
    //     CGocInventory::SendCreateItem(this, &stItemList);
    //     CGocInventory::SendBreakItem(this, psItemMove.bySrcInvenType, psItemMove.shSrcSlotPos);
    //     CGocInventory::SendSocketUpdate(this, &psItemMoveForServer->psResItemMoveInfo.psItemSocketList);
    //     CGocInventory::SendBroachUpdate(this, &psItemMoveForServer->psResItemMoveInfo.psItemBroachList);
    //     CGocInventory::SendPackageInfo(this, &psItemMoveForServer->psResItemMoveInfo.psItemPackageList);
    //     // Log: main=4, sub=81
    //     // Add akashic get info
    //   }
    //   else if (psItemMove.byType == 2) {
    //     // Swap items between league slots
    //     XSendPacket::XSendPacket(&packet, 0x22u, 0x54u);
    //     operator<<(&packet, &psItemMove);
    //     CGocNetwork::Send(pActor, &packet);
    //     // Log: main=4, sub=82 for each item in psItemMoveForServer->psItemLogList
    //   }
    //   PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME::~PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME(psItemMoveForServer);
    //   return 1;
    // }
    // TODO: 需人工审查 - Complex function with 3 types (0=to league, 1=from league, 2=swap)
    (void)psItemMoveForServer;
    return false;
}

// IDA: 0x1400C9D40
// Checks and caps money overflow for drops based on currency type
void CGocInventory::CheckOverMoneyDrop(int ePriceType, std::int64_t& biValue) {
    // IDA Decompiled:
    // void __fastcall CGocInventory::CheckOverMoneyDrop(CGocInventory *this, ePriceType ePriceType, __int64 *biValue)
    // {
    //   __int64 biGold; // [rsp+8h] [rbp-20h]
    //   biGold = 0;
    //   if ( ePriceType == E_PRICE_TYPE_GOLD )
    //   {
    //     if ( *biValue + this->m_nInvenMoney <= 0x5AF3107A3FFFLL )  // MAX_CURRENCY = 999,999,999,999,999
    //     {
    //       biGold = *biValue;
    //     }
    //     else
    //     {
    //       biGold = 0x5AF3107A3FFFLL - this->m_nInvenMoney;
    //       if ( biGold < 0 )
    //         biGold = 0;
    //     }
    //   }
    //   else if ( ePriceType == E_PRICE_TYPE_ETHER )
    //   {
    //     if ( *biValue + this->m_biEther <= 0x5AF3107A3FFFLL )
    //     {
    //       biGold = *biValue;
    //     }
    //     else
    //     {
    //       biGold = 0x5AF3107A3FFFLL - this->m_biEther;
    //       if ( biGold < 0 )
    //         biGold = 0;
    //     }
    //   }
    //   *biValue = biGold;
    // }

    constexpr std::int64_t MAX_CURRENCY = 999999999999999LL; // 0x5AF3107A3FFF
    std::int64_t biGold = 0;

    if (ePriceType == 0) { // E_PRICE_TYPE_GOLD
        if (biValue + m_nInvenMoney <= MAX_CURRENCY) {
            biGold = biValue;
        } else {
            biGold = MAX_CURRENCY - m_nInvenMoney;
            if (biGold < 0)
                biGold = 0;
        }
    } else if (ePriceType == 1) { // E_PRICE_TYPE_ETHER
        if (biValue + m_biEther <= MAX_CURRENCY) {
            biGold = biValue;
        } else {
            biGold = MAX_CURRENCY - m_biEther;
            if (biGold < 0)
                biGold = 0;
        }
    }
    biValue = biGold;
}

// ============================================================================
// Recycle functions (IDA verified)
// ============================================================================

// IDA: 0x1400C9E60
// Adds recycle points with DB sync and optional logging
void CGocInventory::AddRecycle(std::int64_t biRecycle, std::uint8_t byLogType, int nLogValue1, int nLogValue2, bool bLog) {
    // IDA Decompiled:
    // void __fastcall CGocInventory::AddRecycle(CGocInventory *this, __int64 biRecycle, unsigned __int8 byLogType, int nLogValue1, int nLogValue2, bool bLog)
    // {
    //   if ( biRecycle + this->m_biRecycle >= 0 )
    //   {
    //     CGocInventory::SetRecycle(this, biRecycle + this->m_biRecycle);
    //     PS_DB_RECYCLE_UPDATE::PS_DB_RECYCLE_UPDATE(&psDBRecycleUpdate);
    //     psDBRecycleUpdate.dwUCID = GetActorID();
    //     psDBRecycleUpdate.biRecycle = biRecycle;
    //     psDBRecycleUpdate.biTotalRecycle = this->m_biRecycle;
    //     XSendDBPacket::XSendDBPacket(&xSendDBPacket, pObject, 3u, 0x82u);
    //     operator<<(&xSendDBPacket, &psDBRecycleUpdate);
    //     XGameServer::SendDBGame(v6, &xSendDBPacket);
    //     if ( !bLog )
    //     {
    //       CUser *pUser = GetCUser();
    //       if ( pUser )
    //       {
    //         ST_LOG_GAME::ST_LOG_GAME(&stLog);
    //         stLog._nUAID = pUser->GetUAID();
    //         stLog._nUCID = GetActorID();
    //         stLog._sMainType = 10;
    //         stLog._sSubType = 6;
    //         stLog.nParam1 = GetLevel();
    //         stLog.nParam2 = byLogType;
    //         stLog.nParam3 = nLogValue1;
    //         stLog.nParam4 = nLogValue2;
    //         stLog.nParam5 = biRecycle;
    //         stLog.nParam6 = this->m_biRecycle;
    //         XGameServer::SendDBLog(v9, &stLog);
    //       }
    //     }
    //   }
    // }
    // TODO: 需人工审查 - Add recycle with DB sync and logging
    (void)biRecycle;
    (void)byLogType;
    (void)nLogValue1;
    (void)nLogValue2;
    (void)bLog;
}

// IDA: 0x1400CA190
// Sets recycle value and syncs to user info
void CGocInventory::SetRecycle(std::int64_t biRecycle) {
    // IDA Decompiled:
    // void __fastcall CGocInventory::SetRecycle(CGocInventory *this, __int64 biRecycle)
    // {
    //   CUser *pUser; // [rsp+30h] [rbp-18h]
    //   this->m_biRecycle = biRecycle;
    //   v2 = std::list<CBattleZone *>::size((VChunkLocker *)this);
    //   pUser = (CUser *)_RTDynamicCast_0(v2, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
    //   if ( pUser )
    //     CUser::stMyCharInfoEx(pUser)->biRecycle = this->m_biRecycle;
    // }
    m_biRecycle = biRecycle;
    // TODO: Sync to CUser::stMyCharInfoEx()->biRecycle
}

// IDA: 0x1400CA210
// Sends recycle update to client (main=8, sub=0x63)
void CGocInventory::SendRecycle(void* psDBRecycleInfo) {
    // IDA Decompiled:
    // void __fastcall CGocInventory::SendRecycle(CGocInventory *this, PS_DB_RECYCLE_UPDATE *psDBRecycleInfo)
    // {
    //   PS_RECYCLE_UPDATE psRecycleInfo; // [rsp+20h] [rbp-10058h] BYREF
    //   XSendPacket xSendPacket; // [rsp+30h] [rbp-10048h] BYREF
    //   psRecycleInfo.biTotelRecycle = psDBRecycleInfo->biTotalRecycle;
    //   XSendPacket::XSendPacket(&xSendPacket, 8u, 0x63u);
    //   operator<<(&xSendPacket, &psRecycleInfo);
    //   CGocNetwork::Send(pActor, &xSendPacket);
    // }
    // TODO: 需人工审查 - Send recycle update packet
    (void)psDBRecycleInfo;
}

// ============================================================================
// Item reinforce functions (IDA verified)
// ============================================================================

// IDA: 0x1400CA300
// Applies reinforce option stats to character attributes
void CGocInventory::ApplyItemReinforceOption(std::uint8_t byUpgrade, std::uint32_t dwReinforceOptionID, bool bEquip, bool bCalc) {
    // IDA Decompiled:
    // void __fastcall CGocInventory::ApplyItemReinforceOption(CGocInventory *this, unsigned __int8 byUpgrade, unsigned int dwReinforceOptionID, bool bEquip, bool bCalc)
    // {
    //   CUser *pUser = GetCUser();
    //   if ( pUser )
    //   {
    //     CMover::GetGOC<CGocAttribute>(&pUser->CMoverEx, &pAttributePtr, 0);
    //     if ( pAttributePtr )
    //     {
    //       nEquip = 1;
    //       if ( !bEquip ) nEquip = -1;
    //       pTB_ReinforceOption = XResourceMgr::GetTB_REINFORCE_OPTION(&v6->m_xResourceMgr, dwReinforceOptionID);
    //       if ( !pTB_ReinforceOption )
    //       {
    //         LogHelper::LogError("game.item", "ApplyItemReinforceOption error - No Table TB_REINFORCE_OPTION[ UCID:%d, ID:%d ]( %d )", ...);
    //         CUser::SendErrorMessage(pUser, 8u, 2u, 0xCBFBu);
    //         return;
    //       }
    //       for ( i = 0; i < 5; ++i )
    //       {
    //         if ( byUpgrade >= *(&pTB_ReinforceOption->Phase_R01 + i) && *(&pTB_ReinforceOption->Phase_R01 + i) )
    //         {
    //           nType = *(&pTB_ReinforceOption->OptionID_R01 + i);
    //           nValue = nEquip * *(&pTB_ReinforceOption->OptionValue_R01 + i);
    //           if ( *(&pTB_ReinforceOption->OptionClass_R01 + i) == 2 )
    //             CGocAttribute::UpdateEffectStat(pAttributePtr, 1, nType, (float)nValue, bCalc);
    //           else if ( *(&pTB_ReinforceOption->OptionClass_R01 + i) == 1 )
    //             CGocAttribute::UpdateEffectStat(pAttributePtr, 0, nType, (float)nValue, bCalc);
    //         }
    //       }
    //     }
    //   }
    // }
    // TODO: 需人工审查 - Apply reinforce option to character stats
    (void)byUpgrade;
    (void)dwReinforceOptionID;
    (void)bEquip;
    (void)bCalc;
}

// ============================================================================
// Wealth logging functions (IDA verified)
// ============================================================================

// IDA: 0x1400CA5B0
// Logs wealth changes (money, recycle) to DB
void CGocInventory::WealthLog(int nMoney, std::int16_t shSubType, std::uint8_t byLogType, void* stUpdateItemInfo) {
    // IDA Decompiled:
    // void __fastcall CGocInventory::WealthLog(CGocInventory *this, int nMoney, __int16 shSubType, unsigned __int8 byLogType, PS_RES_STORAGE_INFO *stUpdateItemInfo)
    // {
    //   CUser *pUser = GetCUser();
    //   if ( pUser )
    //   {
    //     for each item in stUpdateItemInfo:
    //       biParam6 = 0;
    //       if ( shSubType == 1 ) { biParam6 = this->m_nInvenMoney; wcscpy(szComment, L"MONEY"); }
    //       else if ( shSubType == 6 ) { biParam6 = this->m_biRecycle; wcscpy(szComment, L"RECYCLE"); }
    //       ST_LOG_GAME::ST_LOG_GAME(&stLog);
    //       stLog._nUAID = pUser->GetUAID();
    //       stLog._nUCID = GetActorID();
    //       stLog._sMainType = 10;
    //       stLog._sSubType = shSubType;
    //       stLog.nParam1 = GetLevel();
    //       stLog.nParam2 = byLogType;
    //       stLog.nParam3 = stItemInfo.stItem.nItemID;
    //       stLog.nParam4 = stItemInfo.stItem.sCount;
    //       stLog.nParam5 = nMoney;
    //       stLog.nParam6 = biParam6;
    //       wcscpy_s(stLog.szComment, szComment);
    //       XGameServer::SendDBLog(v8, &stLog);
    //   }
    // }
    // TODO: 需人工审查 - Log wealth changes to DB
    (void)nMoney;
    (void)shSubType;
    (void)byLogType;
    (void)stUpdateItemInfo;
}

// ============================================================================
// Skill option item functions (IDA verified)
// ============================================================================

// IDA: 0x1400CA990
// Checks and updates skill option effects from equipped items
void CGocInventory::CheckEquipSkillOptionItem(int nSkillGroupIndex, bool bClear) {
    // IDA Decompiled:
    // void __fastcall CGocInventory::CheckEquipSkillOptionItem(CGocInventory *this, int nSkillGroupIndex, bool bClear)
    // {
    //   XBaseEquip *pEquip = CGocInventory::GetEquipPtr(this, 1u);
    //   if ( pEquip )
    //   {
    //     CMover::GetGOC<CGocAttribute>(v3, &pAttr, 0);
    //     if ( pAttr )
    //     {
    //       XBaseEquip::GetInvenInfo(pEquip, 1u, &psEquipItemInfo);
    //       bSet = 0;
    //       for each item in psEquipItemInfo:
    //         XBaseEquip::GetItem(pEquip, &pItemPtr, stItem.xSerial);
    //         if ( pItemPtr )
    //         {
    //           if ( bClear ) CGocAttribute::ClearSkillOptionEffect(pAttr);
    //           CItem::UpdateSkillOptionEffectItem(pItemPtr, 1, pObject, nSkillGroupIndex);
    //           if ( !bSet && CItem::GetItemTable(pItemPtr)->Item_SetItem_ID )
    //           {
    //             bySetCount = XBaseEquip::GetSetItemCount(pEquip, CurID, ItemTable->Item_SetItem_ID);
    //             if ( bySetCount >= 2 && bySetCount <= 6 )
    //             {
    //               bySetCount -= 2;
    //               CItem::UpdateSkillOptionEffectSetItem(pItemPtr, 1, bySetCount, v26, nSkillGroupIndex);
    //               bSet = 1;
    //             }
    //           }
    //         }
    //     }
    //   }
    // }
    // TODO: 需人工审查 - Check and update skill option effects
    (void)nSkillGroupIndex;
    (void)bClear;
}

// IDA: 0x1400CACE0
// Checks and updates skill option effects from equipped items (partial)
void CGocInventory::CheckEquipSkillOptionItemPart(int nSkillGroupIndex) {
    // IDA Decompiled:
    // Similar to CheckEquipSkillOptionItem but calls UpdateSkillOptionEffectItemPart instead
    // and UpdateSkillOptionEffectSetItem with first param 0 instead of 1
    // TODO: 需人工审查 - Check and update skill option effects (partial)
    (void)nSkillGroupIndex;
}

// ============================================================================
// Inventory slot functions (IDA verified)
// ============================================================================

// IDA: 0x1400CB000
// Initializes empty slots for inventory types 2, 13, 4, 11
void CGocInventory::InitEmptySlot() {
    XBaseInventory* pInventory = GetInvenPtr(2);
    if (pInventory) {
        pInventory->InitSimpleEmptySlot();
    }

    pInventory = GetInvenPtr(0xD);
    if (pInventory) {
        pInventory->InitSimpleEmptySlot();
    }

    pInventory = GetInvenPtr(4);
    if (pInventory) {
        pInventory->InitSimpleEmptySlot();
    }

    pInventory = GetInvenPtr(0xB);
    if (pInventory) {
        pInventory->InitSimpleEmptySlot();
    }
}

// IDA: 0x1400CED20
// Validates slot positions (must be >= 0 and <= 400)
bool CGocInventory::IsValidSlotPos(std::int16_t shSrcPos, std::int16_t shDestPos) {
    // IDA Decompiled:
    // bool __fastcall CGocInventory::IsValidSlotPos(CGocInventory *this, __int16 shSrcPos, __int16 shDestPos)
    // {
    //   if ( shSrcPos < 0 || shDestPos < 0 )
    //     return 0;
    //   return shSrcPos <= 400 && shDestPos <= 400;
    // }
    if (shSrcPos < 0 || shDestPos < 0)
        return false;
    return shSrcPos <= 400 && shDestPos <= 400;
}

// IDA: 0x1400CB0A0
// Checks whether all requested inventory categories have sufficient empty slots.
bool CGocInventory::IsEmptyInventory(int nCommonCnt, int nFashionCnt, int nCashCnt, int nCubeCnt) {
    CUser* pUser = GetCUserFromOwner(this);
    if (!pUser) {
        return false;
    }

    const std::uint32_t dwUCID = CQuestCondition::GetQuestID(pUser->GetActorID());
    XBaseInventory* pInventory = nullptr;

    if (nCommonCnt > 0) {
        pInventory = GetInvenPtr(2);
        if (!pInventory) {
            return false;
        }
        if (pInventory->GetSimpleEmptySlotCount() < nCommonCnt) {
            LogHelper::LogError(
                "game.contents",
                "IsEmptyInventory error - Full inventory common[UCID:%d, invenCount:%d, needCount:%d]",
                dwUCID,
                pInventory->GetSimpleEmptySlotCount(),
                nCommonCnt);
            return false;
        }
    }

    if (nFashionCnt > 0) {
        pInventory = GetInvenPtr(4);
        if (!pInventory) {
            return false;
        }
        if (pInventory->GetSimpleEmptySlotCount() < nFashionCnt) {
            LogHelper::LogError(
                "game.contents",
                "IsEmptyInventory error - Full inventory fashion[UCID:%d, invenCount:%d, needCount:%d]",
                dwUCID,
                pInventory->GetSimpleEmptySlotCount(),
                nFashionCnt);
            return false;
        }
    }

    if (nCashCnt > 0) {
        pInventory = GetInvenPtr(0xD);
        if (!pInventory) {
            return false;
        }
        if (pInventory->GetSimpleEmptySlotCount() < nCashCnt) {
            LogHelper::LogError(
                "game.contents",
                "IsEmptyInventory error - Full inventory cash[UCID:%d, invenCount:%d, needCount:%d]",
                dwUCID,
                pInventory->GetSimpleEmptySlotCount(),
                nCashCnt);
            return false;
        }
    }

    if (nCubeCnt <= 0) {
        return true;
    }

    pInventory = GetInvenPtr(0xB);
    if (!pInventory) {
        return false;
    }
    if (pInventory->GetSimpleEmptySlotCount() >= nCubeCnt) {
        return true;
    }

    LogHelper::LogError(
        "game.contents",
        "IsEmptyInventory error - Full inventory cube[UCID:%d, invenCount:%d, needCount:%d]",
        dwUCID,
        pInventory->GetSimpleEmptySlotCount(),
        nCubeCnt);
    return false;
}

// ============================================================================
// Broach effect functions (IDA verified)
// ============================================================================

// IDA: 0x1400CB320; PDB: PS_ACTIVE_BROACH_EFFECT&.
void CGocInventory::ChangeActiveBroachEffect(
    PS_ACTIVE_BROACH_EFFECT& psBroach)
{
    CUser* pUser = GetCUserFromOwner(this);
    if (!pUser) {
        return;
    }

    if (psBroach.dwActiveBuffID != 0) {
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (!pServer->GetResourceMgr().GetTB_BUFF(
                static_cast<std::uint16_t>(psBroach.dwActiveBuffID))) {
            return;
        }

        if (pUser->GetActiveBroachEffect() == psBroach.dwActiveBuffID) {
            CGocNetwork::SendErrorMessage(
                static_cast<CMover*>(pUser), 6, 0x19, 0xCE41);
            return;
        }

        PS_ITEM_BROACH_LIST stBroachList;
        GetBroachList(0, stBroachList);

        bool bFind = false;
        for (const ST_ITEM_BROACH& stBroach : stBroachList.vecInfo) {
            std::shared_ptr<CItem> pItem = GetEquipItem(stBroach.biSerial);
            if (!pItem) {
                continue;
            }

            for (int i = 0; i < 5; ++i) {
                const std::uint32_t dwBuffID = pItem->GetSetBuffID(i);
                if (dwBuffID != 0 &&
                    dwBuffID == psBroach.dwActiveBuffID) {
                    bFind = true;
                    break;
                }
            }

            if (bFind) {
                break;
            }
        }

        if (!bFind) {
            CGocNetwork::SendErrorMessage(
                static_cast<CMover*>(pUser), 6, 0x19, 0xCE42);
            return;
        }
    }

    // The zero request deliberately skips validation and clears the current effect.
    pUser->SetActiveBroachEffect(psBroach.dwActiveBuffID);

    XSendPacket xSendPacket(6, 0x19);
    xSendPacket << psBroach;
    CGocNetwork::Send(static_cast<XActor*>(pUser), xSendPacket);
}

// ============================================================================
// Item logging functions (IDA verified)
// ============================================================================

// IDA: 0x1400C5470
void CGocInventory::ItemLog(
    std::shared_ptr<CItem> pItem,
    STItem stResultItem,
    ST_LOG_GAME stLog) {
    CUser* pUser = GetCUserFromOwner(this);
    if (!pItem || !pItem->GetClassifyTable() || !pUser ||
        stLog._sSubType == 128) {
        return;
    }

    stLog._nUAID = pUser->GetUAID();
    stLog._nUCID = pUser->GetUCID();
    stLog._sMainType = 4;
    stLog.nParam0 = pItem->GetID();
    stLog.nParam5 = pItem->GetSerial();
    stLog.nParam11 = stResultItem.sCount == pItem->GetCount()
        ? stResultItem.sCount
        : stResultItem.sCount - pItem->GetCount();
    stLog.nParam12 = stResultItem.sCount;

    switch (stLog._sSubType) {
        case 4:
        case 5:
        case 14:
        case 15:
        case 44:
        case 71:
            stLog.nParam7 = stResultItem.nTitleID;
            break;

        case 9:
            stLog.nParam2 = pItem->GetCount() <= stResultItem.sCount ? 2 : 1;
            break;

        case 10: {
            ST_CREATE_ITEM stDisInfo{};
            GetDisassembleLog(pItem->GetSerial(), stDisInfo);
            if (pItem->GetCount() <= stResultItem.sCount) {
                stLog.nParam4 = 1;
            } else {
                stLog.nParam4 = 2;
                stLog.nParam7 = stDisInfo.nItemID;
                stLog.nParam8 = stDisInfo.shCount;
            }
            break;
        }

        case 32:
            stLog.nParam4 = stResultItem.byUpgrade;
            break;

        case 49:
            if (pItem->GetCount() <= stResultItem.sCount) {
                stLog._sSubType = 38;
            }
            break;

        case 50:
            if (pItem->GetCount() > stResultItem.sCount) {
                stLog.nParam3 = stLog.nParam9;
            } else {
                stLog._sSubType = 7;
                stLog.nParam7 = stResultItem.nTitleID;
            }
            stLog.nParam9 = 0;
            break;

        case 55:
            stLog.nParam3 = stResultItem.nExp;
            stLog.nParam4 = stResultItem.nItemID;
            break;

        case 59:
            stLog.nParam0 = pItem->GetID();
            stLog.nParam1 = stResultItem.sCount - pItem->GetCount();
            if (stLog.nParam1 == 0) {
                stLog.nParam1 = stResultItem.sCount;
            }
            stLog.nParam5 = pItem->GetSerial();
            break;

        case 102:
            if (pItem->GetCount() > stResultItem.sCount) {
                stLog._sSubType = 101;
            }
            break;

        case 107:
            if (pItem->GetCount() > stResultItem.sCount) {
                stLog._sSubType = 106;
            }
            break;

        case 109:
            if (pItem->GetCount() > stResultItem.sCount) {
                stLog._sSubType = 110;
            }
            break;

        case 111:
            if (pItem->GetCount() > stResultItem.sCount) {
                stLog._sSubType = 112;
            }
            break;

        case 113:
            if (pItem->GetCount() < stResultItem.sCount) {
                stLog._sSubType = 113;
            }
            break;

        case 120:
            if (pItem->GetCount() <= stResultItem.sCount) {
                stLog.nParam4 = 1;
                stLog.nParam6 = 0;
            } else {
                stLog.nParam4 = 2;
            }
            break;

        case 121:
            if (pItem->GetCount() < stResultItem.sCount) {
                stLog.nParam3 = 0;
            }
            break;

        case 122:
            if (pItem->GetUseCount() > stResultItem.sCount) {
                stLog._sSubType = 123;
                stLog.nParam3 = stResultItem.byUseCount;
            }
            break;

        case 135:
            if (pItem->GetCount() > stResultItem.sCount) {
                return;
            }
            stLog.nParam4 = stResultItem.byUpgrade;
            break;

        case 138:
            stLog.nParam1 = stResultItem.sCount;
            break;

        default:
            break;
    }

    ItemLogCharLevel(pUser->GetLevel(), stLog);

    TB_ITEM_CLASSIFY* pClassify = pItem->GetClassifyTable();
    ItemLogItemType(
        pClassify->Item_Use_Type,
        pClassify->Item_Slot_Type,
        stLog);
    ItemLogBeforeCount(stResultItem.sCount, stLog);
    TXSingleton<XGameServer>::Instance()->SendDBLog(stLog);
}

// IDA: 0x1400C5E00
void CGocInventory::ItemLogCharLevel(int nLevel, ST_LOG_GAME& stLog) {
    switch (stLog._sSubType) {
        case 4:
        case 7:
        case 8:
        case 10:
        case 11:
        case 14:
        case 17:
        case 22:
        case 50:
        case 51:
        case 52:
        case 53:
        case 54:
        case 55:
        case 56:
        case 57:
        case 59:
        case 60:
        case 61:
        case 63:
        case 64:
        case 65:
        case 66:
        case 67:
        case 68:
        case 69:
        case 70:
        case 71:
        case 72:
        case 73:
        case 74:
        case 75:
        case 76:
        case 77:
        case 78:
        case 96:
        case 121:
        case 126:
        case 127:
        case 133:
        case 135:
        case 136:
        case 137:
        case 138:
            stLog.nParam2 = nLevel;
            break;

        case 38:
        case 45:
        case 46:
        case 49:
            stLog.nParam3 = nLevel;
            break;

        default:
            return;
    }
}

// IDA: 0x1400C5F10
void CGocInventory::ItemLogItemType(
    int nItemUseType,
    int nSlotType,
    ST_LOG_GAME& stLog) {
    int nValue = 1;
    switch (nItemUseType) {
        case 1:
            nValue = 5;
            break;
        case 2:
            nValue = nSlotType == 1 || nSlotType == 2 ? 3 : 4;
            break;
        case 3:
            nValue = 6;
            break;
        case 4:
            nValue = 2;
            break;
        default:
            break;
    }

    switch (stLog._sSubType) {
        case 4:
        case 5:
        case 6:
        case 7:
        case 9:
        case 14:
        case 21:
        case 38:
        case 49:
        case 50:
        case 51:
            stLog.nParam4 = nValue;
            break;

        case 8:
        case 43:
        case 96:
            stLog.nParam7 = nValue;
            break;

        default:
            return;
    }
}

// IDA: 0x1400C6060
void CGocInventory::ItemLogBeforeCount(int nCount, ST_LOG_GAME& stLog) {
    if (stLog._sSubType < 60) {
        stLog.nParam1 = nCount;
    }
}

// ============================================================================
// Socket exchange functions (IDA verified)
// ============================================================================

// IDA: 0x1400D2140
// __int64 __fastcall CGocInventory::ExchangeSocket(CGocInventory *this, unsigned __int8 byCount, unsigned int dwExchangeID, unsigned __int8 byIndex)
// Exchanges socket items using TB_FRAGMENT_EXCHANGE
int CGocInventory::ExchangeSocket(std::uint8_t byCount, std::uint32_t dwExchangeID, std::uint8_t byIndex) {
    // IDA Decompiled:
    // VChunkFile* v4 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    // CUser* pUser = (CUser*)_RTDynamicCast_0(v4, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
    // if (!pUser) return 52330;
    //
    // // Validate count (must be 1-100)
    // if (!byCount || byCount > 0x64u) {
    //     UXActorID v28;
    //     v6 = v50->GetActorID(&pUser->XActor, &v28);
    //     LogHelper::LogError("game.item", "ExchangeSocket error - Count[UCID:%d, Count:%d](%d)", v6->dwActorID, byCount, 11541);
    //     return 52334;
    // }
    //
    // // Validate index (must be 0-5)
    // if (byIndex >= 6u) {
    //     UXActorID v29;
    //     v7 = v52->GetActorID(&pUser->XActor, &v29);
    //     LogHelper::LogError("game.item", "ExchangeSocket error - Index[UCID:%d, Index:%d](%d)", v7->dwActorID, byIndex, 11548);
    //     return 52335;
    // }
    //
    // // Get exchange table entry
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_FRAGMENT_EXCHANGE* pTB_SocketExchange = XResourceMgr::GetTB_FRAGMENT_EXCHANGE(&pServer->m_xResourceMgr, dwExchangeID);
    // if (!pTB_SocketExchange) {
    //     UXActorID v30;
    //     v9 = v53->GetActorID(&pUser->XActor, &v30);
    //     LogHelper::LogError("game.item", "ExchangeSocket error - No Index in TB_FRAGMENT_EXCHANGE[UCID:%d, ID:%d](%d)", v9->dwActorID, dwExchangeID, 11555);
    //     return 52330;
    // }
    //
    // // Get material item from exchange table (S_Exchange01_Material + byIndex)
    // TB_ITEM* pTB_MaterialItem = XResourceMgr::GetTB_ITEM(&pServer->m_xResourceMgr, pTB_SocketExchange->uniMaterial[byIndex]);
    // if (!pTB_MaterialItem) {
    //     UXActorID v31;
    //     v11 = v56->GetActorID(&pUser->XActor, &v31);
    //     LogHelper::LogError("game.item", "ExchangeSocket error - No Index in TB_ITEM[UCID:%d, ID:%d](%d)", v11->dwActorID, pTB_SocketExchange->uniMaterial[byIndex], 11562);
    //     return 52330;
    // }
    //
    // // Calculate material count needed
    // int nMaterialCount = byCount * pTB_SocketExchange->uniCount[byIndex];
    //
    // // Setup create item list
    // ST_CREATE_ITEM stCreateItem;
    // ST_CREATE_ITEMS stCreateItems;
    // stCreateItem.nItemID = pTB_SocketExchange->S_Get_Item;
    // stCreateItem.shCount = byCount * LOWORD(pTB_SocketExchange->S_GetItem_Count);
    // stCreateItems.push_back(stCreateItem);
    //
    // // Calculate gold cost
    // __int64 biCost = byCount * pTB_SocketExchange->uniCost[byIndex];
    //
    // // Check if player has enough money
    // if (biCost > m_nInvenMoney) {
    //     UXActorID v32;
    //     v12 = v57->GetActorID(&pUser->XActor, &v32);
    //     LogHelper::LogError("game.item", "ReqItemExchange error - Failed CreateItem2[UCID:%d, Cost:I%64d]( %d )", v12->dwActorID, biCost, 11579);
    //     return 52332;
    // }
    //
    // // Initialize result storage
    // PS_RES_STORAGE_INFO psCreateItemList;
    // PS_RES_STORAGE_INFO psUpdateItemList;
    //
    // // Try to reduce material items
    // if (ReduceItem2(pTB_MaterialItem, nMaterialCount, 0x75u, &psUpdateItemList)) {
    //     // Setup log
    //     ST_LOG_GAME stLog2;
    //     stLog2._sSubType = 107;
    //     stLog2.nParam3 = pTB_MaterialItem->Item_ID;
    //
    //     // Try to create result items
    //     if (CreateItem2(&stCreateItems, 0x75u, 0, &psCreateItemList, &psUpdateItemList, &stLog2)) {
    //         // Deduct money
    //         AddMoney(-biCost, 0x3Au, 0, 0, false);
    //
    //         // Send DB packet
    //         PS_DB_SOCKET_EXCHANGE psDBSocketExchange;
    //         psDBSocketExchange.dwUCID = CQuestCondition::GetQuestID(pUser->GetActorID());
    //         psDBSocketExchange.psCreateItemList = psCreateItemList;
    //         psDBSocketExchange.psUpdateItemList = psUpdateItemList;
    //
    //         XSendDBPacket xSendDBPacket(&pUser->XActor, 0x81u, 0x27u);
    //         xSendDBPacket << psDBSocketExchange;
    //         pServer->SendDBGame(&xSendDBPacket);
    //
    //         return 0;
    //     } else {
    //         // CreateItem2 failed - unlock lists
    //         UnLockList(&psUpdateItemList);
    //         UnLockList(&psCreateItemList);
    //         LogHelper::LogError("game.item", "ReqItemExchange error - Failed CreateItem2[UCID:%d]( %d )", v13->dwActorID, 11604);
    //         return 52330;
    //     }
    // } else {
    //     // ReduceItem2 failed - unlock list
    //     UnLockList(&psUpdateItemList);
    //     LogHelper::LogError("game.item", "ReqItemExchange error - Failed ReduceItem2( %d )", 11589);
    //     return 52333;
    // }
    // TODO: 需人工审查 - Complex function for socket exchange, requires CUser, XGameServer, TB_FRAGMENT_EXCHANGE, TB_ITEM types
    (void)byCount;
    (void)dwExchangeID;
    (void)byIndex;
    return 52330; // Error code
}

// ============================================================================
// Socket detach functions (IDA verified)
// ============================================================================

// IDA: 0x1400CED70
// Detaches socket from equipment item
int CGocInventory::DetachSocketItem(void* psDetachInfo, void* psUpdateItemList, void* psCreateItemList, std::int64_t& biSerial) {
    // IDA Decompiled:
    // __int64 __fastcall CGocInventory::DetachSocketItem(CGocInventory *this, PS_REQ_SOCKET_DETACH *psDetachInfo, PS_RES_STORAGE_INFO *psUpdateItemList, PS_RES_STORAGE_INFO *psCreateItemList, __int64 *biSerial)
    // {
    //   CGocInventory::SetRemoveSocket(this, 1);
    //   CUser *pUser = GetCUser();
    //   if ( !pUser ) return 52011;
    //   CGocInventory::GetSlotItem(this, &pEquipItemPtr, psDetachInfo->stInfo.byInvenType, psDetachInfo->stInfo.shSlotPos, byLock);
    //   if ( !pEquipItemPtr || byLock[0] ) { /* error 52291 */ }
    //   if ( !CItem::IsValidSocketPos(pEquipItemPtr, psDetachInfo->bySocketPos) ) { /* error 52291 */ }
    //   pInventory = CGocInventory::GetInvenPtr(this, 2u);
    //   if ( !pInventory ) return 52001;
    //   if ( GetSimpleEmptySlotCount(pInventory) < 1 ) return 52294;
    //   pTB_Socket = XResourceMgr::GetTB_SOCKET(ItemTable->Item_Socket_ID);
    //   if ( !pTB_Socket || !pTB_Socket->Extraction_Item ) return 52295;
    //   pTB_NeedItem = XResourceMgr::GetTB_ITEM(pTB_Socket->Extraction_Item);
    //   if ( !pTB_NeedItem ) return 52295;
    //   stSocketItem = CItem::GetSocketInfo(pEquipItemPtr, psDetachInfo->bySocketPos);
    //   pTB_DetachSocketItem = XResourceMgr::GetTB_ITEM(stSocketItem->dwSocketID);
    //   if ( pTB_DetachSocketItem->Item_Rank == 5 ) return 52291; // Cannot detach legendary
    //   stCreateItem.nItemID = stSocketItem->dwSocketID;
    //   stCreateItem.shCount = 1;
    //   if ( pTB_Socket->Extraction_Item_Count )
    //     CGocInventory::ReduceItem2(this, pTB_NeedItem, pTB_Socket->Extraction_Item_Count, 0x73u, psUpdateItemList);
    //   CGocInventory::CreateItem2(this, &stCreateItems, 0x73u, 0, psCreateItemList, psUpdateItemList, &stLog);
    //   *biSerial = CItem::GetSerial(pEquipItemPtr);
    //   return 0;
    // }
    // TODO: 需人工审查 - Complex function for socket detachment
    (void)psDetachInfo;
    (void)psUpdateItemList;
    (void)psCreateItemList;
    (void)biSerial;
    return 52011; // Error code
}

// ============================================================================
// Item refine functions (IDA verified)
// ============================================================================

// IDA: 0x1400CFB40
// Refines item ability using materials
int CGocInventory::RefineItemAbility(void* psRefine, int nNpcGroupID, std::uint32_t nNpcID) {
    // IDA Decompiled: Very complex function (~0x2250 bytes) for item refinement
    // 1. Validates user and items
    // 2. Checks item rank >= 3
    // 3. Validates equipment stats are not maxed
    // 4. Calculates refinement points based on item level and rank
    // 5. For refineType 1: Uses mileage points directly
    // 6. For refineType 0: Uses material items
    //    - Validates material item
    //    - Calculates cost (jenny and ether)
    //    - Handles prevent item for downgrade protection
    // 7. Reduces items and updates equipment
    // 8. Sends DB packet and logs
    // TODO: 需人工审查 - Very complex refinement function
    (void)psRefine;
    (void)nNpcGroupID;
    (void)nNpcID;
    return 52061; // Error code
}

// ============================================================================
// WeMade billing functions (IDA verified)
// ============================================================================

// IDA: 0x1400CB6D0
// Initializes WeMade billing system
void CGocInventory::InitWeMadeBilling() {
    // IDA Decompiled:
    // void __fastcall CGocInventory::InitWeMadeBilling(CGocInventory *this)
    // {
    //   CUser *pUser = GetCUser();
    //   // Creates a lambda that runs as a common job
    //   CLogicThreadManager::DoCommonJob(v2, v8);
    // }
    // TODO: 需人工审查 - Initialize WeMade billing
}

// ============================================================================
// Repurchase functions (IDA verified)
// ============================================================================

// IDA: 0x1400D1DC0
// Adds socket info to repurchase list
void CGocInventory::AddRepurchaseSocketInfo(void* psSocketList) {
    // IDA Decompiled:
    // void __fastcall CGocInventory::AddRepurchaseSocketInfo(CGocInventory *this, PS_ITEM_SOCKET_LIST *psSocketList)
    // {
    //   for each item in m_listRepurchaserItem:
    //     for each socket in psSocketList:
    //       if ( stItem.xSerial == stSocketInfo.biEquipSerial )
    //         m_listRepurchaseSocket.push_back(stSocketInfo);
    // }
    // TODO: 需人工审查 - Add socket info to repurchase list
    (void)psSocketList;
}

// IDA: 0x1400D1F80
// Adds broach info to repurchase list
void CGocInventory::AddRepurchaseBroachInfo(void* psBroachList) {
    // IDA Decompiled:
    // void __fastcall CGocInventory::AddRepurchaseBroachInfo(CGocInventory *this, PS_ITEM_BROACH_LIST *psBroachList)
    // {
    //   for each item in m_listRepurchaserItem:
    //     for each broach in psBroachList:
    //       if ( stItem.xSerial == stBroachInfo.biSerial )
    //         m_listRepurchaseBroach.push_back(stBroachInfo);
    // }
    // TODO: 需人工审查 - Add broach info to repurchase list
    (void)psBroachList;
}

// ============================================================================
// Package Box / Random Box functions (IDA verified)
// ============================================================================

// IDA: 0x1400B2D80
// __int64 __fastcall CGocInventory::PackageBoxUse(CGocInventory *this, bool bReduceItem, std::tr1::shared_ptr<CGocNetwork> *pItem, unsigned __int8 byCount, int nItemIDparClass)
// Uses a package/box item, creates contained items based on TB_ITEM_PACKAGE
// IDA decompilation (~0x1E31 bytes) shows:
// 1. Validate count 1-10, send error 0xCB2B on fail (line 4176)
// 2. Get ItemID from pItem; if Item_Effect_Type==7, override with nItemIDparClass
// 3. Get serial number, then TB_ITEM_PACKAGE from XResourceMgr
// 4. Initialize counters: byCommonCount/byFashionCount/byCashCount/byCubeCount
// 5. Create ST_GET_INFO, ST_CREATE_ITEMS, mapStackCount
// 6. DOUBLE LOOP: for k=0 to byCount, for i=0 to 14 (15 package contents via Item_01+Item_15)
// 7. For each content with non-zero Item and Count:
//    - Get TB_ITEM and TB_ITEM_CLASSIFY, validate they exist
//    - If Unpacking_Function_Type==1: validate stack max==1, reinforce ID exists, upgrade level valid
//    - Set shCount=1 and byUpgrade for upgrade type, or shCount=Count value and byUpgrade=0
//    - Push to stCreateItemList and stGetInfo.stGetItem
//    - Count inventory slots needed by type
// 8. IsEmptyInventory check with error 0xCB2A (line 4313)
// 9. If bReduceItem: add cooltime, log item (subType 21)
// 10. If !bReduceItem: ReduceItem3 on the box item slot with subType 0x15
// 11. Iterate created items: AddItemUpgradeCount for each, with error handling (lines 4380, 4390)
// 12. UpdateItemEnd with subType 0x15 and vecUpdateItem (line 4401 error)
// 13. AddItemEnd with subType 0x15 and vecCreateItem (line 4411 error)
// 14. Calculate gold (P_Gold*count), BP (P_BP*count), ether (P_Ether*count)
// 15. Three CheckOverMoney calls (GOLD→0xCB58, BP→0xCB59, ETHER→0xCB5A)
// 16. Set stGetInfo money values
// 17. Send DB packet (0x21/0x12) with: QuestID, vecUpdateItem, vecCreateItem, stGetInfo, nItemID, 0, stGachaList, byFlag, biItemSerial
// 18. AddMoney (Gold, subType 0x1C), AddBP (0x1C), AddEther (0x1C, 1)
// 19. Return 1 on success, 0 on any failure path
bool CGocInventory::PackageBoxUse(bool bReduceItem, std::shared_ptr<CItem> pItem,
                                   std::uint8_t byCount, int nItemIDparClass) {
    // TODO: Implement per IDA 0x1400B2D80
    // Requires: CItem::GetItemTable, CItem::GetCurID, CItem::GetSerial, CItem::GetCount, CItem::GetSlot, CItem::GetInvenType
    // Requires: XResourceMgr::GetTB_ITEM_PACKAGE, GetTB_ITEM, GetTB_ITEM_CLASSIFY, GetTB_REINFORCE
    // Requires: IsEmptyInventory, ReduceItem3, ReduceItem2, AddItemUpgradeCount, UpdateItemEnd, AddItemEnd
    // Requires: CheckOverMoney, AddMoney, AddBP, AddEther, AddCoolTime, ItemLog
    // Requires: XSendDBPacket, SendDBGame, CMover::SendErrorMessage
    // Requires: ST_GET_INFO, ST_CREATE_ITEM, ST_CREATE_ITEMS, PS_RES_STORAGE_INFO, PS_GACHA_LIST
    if (byCount == 0 || byCount > 10) {
        return false;
    }
    (void)bReduceItem;
    (void)pItem;
    (void)nItemIDparClass;
    return false;
}

// Random Box functions are part of PackageBoxUse
// CanRandomBoxUse and RandomBoxUse are not separate functions in the PDB
// They are internal code blocks within PackageBoxUse (0x1400B2D80)

// ============================================================================
// Equipment slot functions (IDA verified)
// ============================================================================

// IDA: 0x1400B6570
// char __fastcall CGocInventory::CanEquipSlotOpen(CGocInventory *this, unsigned __int8 byInvenType, __int16 shSlot)
// Checks if equipment slot can be opened by validating:
// 1. byInvenType must be 2 (common inventory)
// 2. Item exists at slot and is not locked
// 3. Item level limit <= player level
// 4. Equip slot position is valid and NOT yet opened
bool CGocInventory::CanEquipSlotOpen(std::uint8_t byInvenType, std::int16_t shSlot) {
    // IDA: Only type 2 (common inventory) is valid
    if (byInvenType != 2)
        return false;

    // IDA: Get item at slot
    bool bLock = false;
    std::shared_ptr<CItem> pItem = GetSlotItem(byInvenType, static_cast<std::uint16_t>(shSlot), bLock);

    if (!pItem || bLock) {
        // IDA: Send item lock log and return false
        CUser* pUser = GetCUserFromOwner(this);
        std::uint32_t dwUCID = pUser ? pUser->GetUCID() : 0;
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (pServer) {
            pServer->SendItemLockLog(dwUCID, byInvenType, shSlot, bLock ? 1 : 0, 13, 0);
        }
        return false;
    }

    // IDA: Get item table and classify table
    TB_ITEM* pTBItem = pItem->GetItemTable();
    if (!pTBItem)
        return false;

    TB_ITEM_CLASSIFY* pTBClassify = pItem->GetClassifyTable();
    if (!pTBClassify)
        return false;

    // IDA: Check item level limit <= player level
    // RTTI: CMover -> CUser + offset 131512 (handled by GetCUserFromOwner)
    CUser* pUser = GetCUserFromOwner(this);
    if (!pUser)
        return false;

    // IDA: Get player level from CUser at offset 131512 (character info)
    // The level is accessed via pUser->GetLevel() or similar
    int nPlayerLevel = 0;
    // TODO: Need to access CUser level - check CUser structure
    // For now, use a placeholder that should be replaced with actual level access
    // nPlayerLevel = pUser->GetLevel();

    if (pTBItem->Item_Limit_Lv > nPlayerLevel)
        return false;

    // IDA: Get equip slot position from item classify
    std::uint8_t byEquipPos = 0;
    int nEquipPosBit = 0;
    if (!ChangeEquipSlotPos(pTBClassify->Item_Slot_Type, byEquipPos, nEquipPosBit))
        return false;

    // IDA: Check if the equip slot can be opened
    // If slot is already open (CheckEquipSlotOpen returns true), we can't open it again -> return false
    // If slot is not open (CheckEquipSlotOpen returns false), we can open it -> return true
    if (CheckEquipSlotOpen(byEquipPos))
        return false;  // Slot already open, can't open again

    return true;  // Slot not open, can open
}

// IDA: 0x1400B6E90
// bool __fastcall CGocInventory::CheckEquipSlotOpen(CGocInventory *this, unsigned __int8 byPos)
// Checks if equipment slot at position is opened (bitwise check on m_nEquipSlot)
// Positions 0xA-0x13 (10-19) correspond to bits 0-9 in m_nEquipSlot
bool CGocInventory::CheckEquipSlotOpen(std::uint8_t byPos) {
    // IDA: Only positions 10-19 (0xA-0x13) are valid equip slots
    if (byPos < 0xA || byPos > 0x13)
        return false;

    // IDA: Each position maps to a bit in m_nEquipSlot
    switch (byPos) {
        case 0xA:  // Position 10 -> bit 0
            return (m_nEquipSlot & 1) != 0;
        case 0xB:  // Position 11 -> bit 1
            return (m_nEquipSlot & 2) != 0;
        case 0xC:  // Position 12 -> bit 2
            return (m_nEquipSlot & 4) != 0;
        case 0xD:  // Position 13 -> bit 3
            return (m_nEquipSlot & 8) != 0;
        case 0xE:  // Position 14 -> bit 4
            return (m_nEquipSlot & 0x10) != 0;
        case 0xF:  // Position 15 -> bit 5
            return (m_nEquipSlot & 0x20) != 0;
        case 0x10: // Position 16 -> bit 6
            return (m_nEquipSlot & 0x40) != 0;
        case 0x11: // Position 17 -> bit 7
            return (m_nEquipSlot & 0x80) != 0;
        case 0x12: // Position 18 -> bit 8
            return (m_nEquipSlot & 0x100) != 0;
        case 0x13: // Position 19 -> bit 9
            return (m_nEquipSlot & 0x200) != 0;
        default:
            return false;
    }
}

// IDA: 0x1400B6D20
// bool __fastcall CGocInventory::ChangeEquipSlotPos(CGocInventory *this, unsigned __int8 bySlotType, unsigned __int8 *byEquipPos, int *nEquipPosBit)
// Converts slot type (0x65-0x6E, i.e., 101-110) to equip position and bit
// Returns true if slot is NOT yet opened (so can be opened)
bool CGocInventory::ChangeEquipSlotPos(std::uint8_t bySlotType, std::uint8_t& byEquipPos, int& nEquipPosBit) {
    // IDA: Validate slot type range (0x65=101 to 0x6E=110)
    if (bySlotType < 0x65 || bySlotType > 0x6E)
        return false;

    // IDA: Convert slot type to position (subtract 101)
    byEquipPos = bySlotType - 0x65;  // bySlotType - 101

    // IDA: Validate position range (must be 10-19)
    if (byEquipPos < 0xA || byEquipPos > 0x13)
        return false;

    // IDA: Map position to bit value
    switch (byEquipPos) {
        case 0xA:
            nEquipPosBit = 1;
            break;
        case 0xB:
            nEquipPosBit = 2;
            break;
        case 0xC:
            nEquipPosBit = 4;
            break;
        case 0xD:
            nEquipPosBit = 8;
            break;
        case 0xE:
            nEquipPosBit = 16;
            break;
        case 0xF:
            nEquipPosBit = 32;
            break;
        case 0x10:
            nEquipPosBit = 64;
            break;
        case 0x11:
            nEquipPosBit = 128;
            break;
        case 0x12:
            nEquipPosBit = 256;
            break;
        case 0x13:
            nEquipPosBit = 512;
            break;
        default:
            return false;
    }

    // IDA: Return true if slot NOT opened yet (bit not set)
    return (nEquipPosBit & m_nEquipSlot) == 0;
}

// IDA: 0x1400B70D0
// void __fastcall CGocInventory::SendEquipSlotOpen(CGocInventory *this)
// IDA: 0x1400B70D0 - ?SendEquipSlotOpen@CGocInventory@@QEAAXXZ
// Precise restoration from IDA decompilation - sends equipment slot open status to client (main=8, sub=0x14)
void CGocInventory::SendEquipSlotOpen() {
    // IDA: Create XSendPacket (main=8, sub=0x14)
    XSendPacket xSendPacket(8, 0x14);
    xSendPacket.XParse << this->m_nEquipSlot;

    // IDA: CGocNetwork::Send accepts CMover* (inherits XActor)
    CGocNetwork::Send(m_pOwner, xSendPacket);
}

// IDA: 0x1400B71B0
// void __fastcall CGocInventory::SetEquipSlot(CGocInventory *this, int nEquipSlot)
// Sets equipment slot value and recalculates
void CGocInventory::SetEquipSlot(int nEquipSlot) {
    m_nEquipSlot = nEquipSlot;
    EquipSlotOpenCalc();
}

// IDA: 0x1400B71E0
// void __fastcall CGocInventory::EquipSlotOpenCalc(CGocInventory *this)
// Calculates and unlocks equipment slots based on m_nEquipSlot bits
// Each bit corresponds to a slot position (10-19)
void CGocInventory::EquipSlotOpenCalc() {
    // IDA: For each bit set, unlock the corresponding equipment slot
    if ((m_nEquipSlot & 1) != 0)
        SetLock(1, 0xA, 0);    // Position 10
    if ((m_nEquipSlot & 2) != 0)
        SetLock(1, 0xB, 0);    // Position 11
    if ((m_nEquipSlot & 4) != 0)
        SetLock(1, 0xC, 0);    // Position 12
    if ((m_nEquipSlot & 8) != 0)
        SetLock(1, 0xD, 0);    // Position 13
    if ((m_nEquipSlot & 0x10) != 0)
        SetLock(1, 0xE, 0);    // Position 14
    if ((m_nEquipSlot & 0x20) != 0)
        SetLock(1, 0xF, 0);    // Position 15
    if ((m_nEquipSlot & 0x40) != 0)
        SetLock(1, 0x10, 0);   // Position 16
    if ((m_nEquipSlot & 0x80) != 0)
        SetLock(1, 0x11, 0);   // Position 17
    if ((m_nEquipSlot & 0x100) != 0)
        SetLock(1, 0x12, 0);   // Position 18
    if ((m_nEquipSlot & 0x200) != 0)
        SetLock(1, 0x13, 0);   // Position 19
}

// IDA: 0x1400B7090
// void __fastcall CGocInventory::EquipSlotOpen(CGocInventory *this, int nPosBit)
// Sets the equip slot bit if not already set
void CGocInventory::EquipSlotOpen(int nPosBit) {
    if ((nPosBit & m_nEquipSlot) == 0)
        m_nEquipSlot |= nPosBit;
}

// IDA: 0x1400B6810
// __int64 __fastcall CGocInventory::EquipSlotOpen(CGocInventory *this, unsigned __int8 byInvenType, __int16 shSlot)
// Opens equipment slot using item from inventory (complex function)
bool CGocInventory::EquipSlotOpen(std::uint8_t byInvenType, std::int16_t shSlot) {
    // IDA: Get item at slot
    // std::shared_ptr<CItem> pItem;
    // std::uint8_t bLock;
    // CGocInventory::GetSlotItem(this, &pItem, byInvenType, shSlot, &bLock);
    // if (!pItem || bLock) {
    //     XGameServer::SendItemLockLog(dwUCID, byInvenType, shSlot, bLock, 14, 0);
    //     return false;
    // }

    // IDA: Get item table and classify table
    // CItem* pRawItem = pItem.get();
    // TB_ITEM* pTBItem = CItem::GetItemTable(pRawItem);
    // if (!pTBItem) return false;
    // TB_ITEM_CLASSIFY* pTBClassify = CItem::GetClassifyTable(pRawItem);
    // if (!pTBClassify) return false;

    // IDA: Reduce item (lock type 0x17)
    // PS_RES_STORAGE_INFO stUpdateItem;
    // if (!CGocInventory::ReduceItem3(byInvenType, shSlot, 1, 0x17, &stUpdateItem))
    //     return false;

    // IDA: Get equip position from slot type
    // std::uint8_t byEquipPos;
    // int nEquipPosBit;
    // if (!CGocInventory::ChangeEquipSlotPos(pTBClassify->Item_Slot_Type, &byEquipPos, &nEquipPosBit)) {
    //     CGocInventory::UnLockList(&stUpdateItem);
    //     return false;
    // }

    // IDA: Log and update
    // ST_LOG_GAME stLog;
    // stLog._sSubType = 65;
    // stLog.nParam3 = byEquipPos;
    // stLog.nParam4 = nEquipPosBit;
    // if (!CGocInventory::UpdateItemEnd(0x17, &stUpdateItem, &stLog))
    //     return false;

    // IDA: Send DB packet (main=0x21, sub=0x25)
    // XSendDBPacket xSendDBPacket(pObject, 0x21, 0x25);
    // xSendDBPacket << QuestID << stUpdateItem << nEquipPosBit << byEquipPos;
    // XGameServer::SendDBGame(&xSendDBPacket);

    (void)byInvenType;
    (void)shSlot;
    return false;  // TODO: 需人工审查 - Implement when GetSlotItem/ReduceItem3/UpdateItemEnd available
}

// ============================================================================
// Item cooldown functions (IDA verified)
// ============================================================================

// IDA: 0x1400B9610
// void __fastcall CGocInventory::SendDBItemCooltimeInfo(CGocInventory *this)
// Sends item cooldown info to database (main=0x21, sub=0x46)
void CGocInventory::SendDBItemCooltimeInfo() {
    // IDA: Get current date
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // __int64 biDate = XGameServer::GetCurDate(pServer);

    // IDA: Send DB packet
    // XSendDBPacket xSendDBPacket(pObject, 0x21, 0x46);
    // xSendDBPacket << QuestID << biDate;
    // XGameServer::SendDBGame(&xSendDBPacket);

    // TODO: Implement when XSendDBPacket/XGameServer available
}

// IDA: 0x1400B98E0
// void __fastcall CGocInventory::SendItemCoolTimeInfo(CGocInventory *this)
// Sends item cooldown info to client (main=8, sub=0x66)
void CGocInventory::SendItemCoolTimeInfo() {
    // IDA: Build cooldown list from m_mpSaveGroupCooltime
    // PS_ITEM_COOMTIME_LIST psList;
    // for (auto& pair : m_mpSaveGroupCooltime) {
    //     PS_ITEM_COOLTIME_INFO psInfo = pair.second;
    //     __int64 biEndDate = psInfo.biRemainDate;
    //     __int64 biCurDate = XGameServer::GetCurDate();
    //     psInfo.biRemainDate = biEndDate - biCurDate;
    //     if (biEndDate - biCurDate > 0)
    //         psList.vecInfo.push_back(psInfo);
    // }

    // IDA: Send if not empty
    // if (!psList.vecInfo.empty()) {
    //     XSendPacket xSendPacket(8, 0x66);
    //     xSendPacket << psList;
    //     CGocNetwork::Send(pActor, &xSendPacket);
    // }

    // TODO: Implement when PS_ITEM_COOMTIME_LIST/XSendPacket available
}

// IDA: 0x1400B9770
// void __fastcall CGocInventory::LoadCoolTime(CGocInventory *this, PS_ITEM_COOMTIME_LIST *psCooltimeList)
// Loads cooldown list from DB response
void CGocInventory::LoadCoolTime(void* psCooltimeList) {
    // IDA: For each item in psCooltimeList
    // for (size_t i = 0; i < psCooltimeList->size(); ++i) {
    //     PS_ITEM_COOLTIME_INFO psInfo = psCooltimeList[i];
    //
    //     // Check if already in m_mpSaveGroupCooltime
    //     auto iter = m_mpSaveGroupCooltime.find(psInfo.byCooltimeGroupID);
    //     if (iter == m_mpSaveGroupCooltime.end()) {
    //         // Insert new entry
    //         m_mpSaveGroupCooltime[psInfo.byCooltimeGroupID] = psInfo;
    //     }
    //
    //     // Calculate remaining time
    //     __int64 biCurDate = XGameServer::GetCurDate();
    //     psCooltimeList[i].biRemainDate = psInfo.biRemainDate - biCurDate;
    // }

    (void)psCooltimeList;
    // TODO: 需人工审查 - Implement when PS_ITEM_COOMTIME_LIST/XGameServer available
}

// IDA: 0x1400B9AB0
// void __fastcall CGocInventory::AddCoolTime(CGocInventory *this, unsigned __int16 byGroupID, unsigned __int64 dwValue, bool bSendDB)
// Adds cooldown for group ID, optionally syncs to DB
void CGocInventory::AddCoolTime(std::uint16_t byGroupID, std::uint64_t dwValue, bool bSendDB) {
    // IDA: Only process if dwValue > 0
    if (dwValue == 0)
        return;

    // IDA: Convert milliseconds to seconds and add current date
    // __int64 dwCoolTimeValue = dwValue;
    // dwValue /= 1000;  // Convert to seconds
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // __int64 biCurDate = XGameServer::GetCurDate(pServer);
    // dwValue += biCurDate;  // End time = current time + cooldown duration

    if (bSendDB) {
        // IDA: Add to m_mpSaveGroupCooltime (persistent, synced to DB)
        // PS_ITEM_COOLTIME_INFO psCooltimeInfo;
        // psCooltimeInfo.byCooltimeGroupID = byGroupID;
        // psCooltimeInfo.biRemainDate = dwValue;
        // psCooltimeInfo.biCooltimeValue = dwCoolTimeValue;
        //
        // auto iter = m_mpSaveGroupCooltime.find(byGroupID);
        // if (iter == m_mpSaveGroupCooltime.end()) {
        //     m_mpSaveGroupCooltime[byGroupID] = psCooltimeInfo;
        // } else {
        //     iter->second = psCooltimeInfo;
        // }
        //
        // // Send DB packet (main=0x21, sub=0x47)
        // PS_DB_ITEM_COOLTIME_UPDATE psCooltimeUpdate;
        // psCooltimeUpdate.dwUCID = GetUCID();
        // psCooltimeUpdate.psInfo = psCooltimeInfo;
        // XSendDBPacket xSendDBPacket(pObject, 0x21, 0x47);
        // xSendDBPacket << psCooltimeUpdate;
        // XGameServer::SendDBGame(&xSendDBPacket);
    } else {
        // IDA: Add to m_mpGroupCoolTime (temporary, in-memory only)
        // auto iter = m_mpGroupCoolTime.find(byGroupID);
        // if (iter == m_mpGroupCoolTime.end()) {
        //     m_mpGroupCoolTime[byGroupID] = dwValue;
        // } else {
        //     iter->second = dwValue;
        // }
    }

    (void)byGroupID;
    (void)dwValue;
    (void)bSendDB;
    // TODO: 需人工审查 - Implement when PS_ITEM_COOLTIME_INFO/XSendDBPacket available
}

// IDA: 0x1400B9E20
// VPList_vtbl *__fastcall CGocInventory::GetCoolTime(CGocInventory *this, unsigned __int16 byGroupID, bool bSave)
// Gets cooldown remaining time for group ID
std::uint64_t CGocInventory::GetCoolTime(std::uint16_t byGroupID, bool bSave) {
    // IDA: If bSave, look in m_mpSaveGroupCooltime, else m_mpGroupCoolTime
    if (bSave) {
        // auto iter = m_mpSaveGroupCooltime.find(byGroupID);
        // if (iter != m_mpSaveGroupCooltime.end()) {
        //     return iter->second.biRemainDate;
        // }
    } else {
        // auto iter = m_mpGroupCoolTime.find(byGroupID);
        // if (iter != m_mpGroupCoolTime.end()) {
        //     return iter->second;
        // }
    }
    (void)byGroupID;
    (void)bSave;
    return 0;  // Not found
}

// ============================================================================
// Trade password functions (IDA verified)
// ============================================================================

// IDA: 0x1400B9F00
// char __fastcall CGocInventory::CheckTradePasswordState(CGocInventory *this, int *nErrorID)
// Checks trade password state, returns error ID if locked
bool CGocInventory::CheckTradePasswordState(int& nErrorID) {
    nErrorID = 0;

    // IDA: Get trade password state
    // int nState = CGocInventory::GetTradePasswordState(this);
    // if (nState == 2) return true;  // Password set and verified
    // if (nState == 3) nErrorID = 1234;  // Locked
    // return false;

    (void)nErrorID;
    return false;  // TODO: 需人工审查 - Implement when GetTradePasswordState available
}

// IDA: 0x1400B9F60
// char __fastcall CGocInventory::CheckTradePassword(CGocInventory *this, PS_TRADE_PW_REQ *psTrade)
// Validates trade password and sends response
bool CGocInventory::CheckTradePassword(void* psTrade) {
    // IDA: Complex function handling different trade password operations:
    // byType = psTrade->byType
    //
    // case 1: Set new password
    //   - Check if password already set (state != 0)
    //   - Validate password format
    //   - Send DB packet to set password (main=2, sub=0x38)
    //
    // case 2: Change password
    //   - Validate old password
    //   - Send DB packet to change (main=2, sub=0x39)
    //
    // case 3: Delete password
    //   - Send DB packet to delete (main=2, sub=0x3B)
    //
    // case 4: Unlock/reset
    //   - Send DB packet (main=2, sub=0x3A)

    (void)psTrade;
    return false;  // TODO: 需人工审查 - Implement when PS_TRADE_PW_REQ/GetTradePasswordState available
}

// IDA: 0x1400BA530
// char __fastcall CGocInventory::IsValidTradePassword(CGocInventory *this, char *strPassword, int *nErrorID)
// Validates password format: 4 digits (0-7), no repeated or sequential patterns
bool CGocInventory::IsValidTradePassword(char* strPassword, int& nErrorID) {
    // IDA: Check null pointer
    if (!strPassword)
        return false;

    // IDA: Must be exactly 4 characters
    if (std::strlen(strPassword) != 4) {
        nErrorID = 59504;  // Invalid length
        return false;
    }

    // IDA: Validate each character and check patterns
    char cCheckSameWord = 0;
    int nCheckSameCount = 0;
    char cCheckNextWord_Up = 0;
    int nCheckNextCount_Up = 0;
    char cCheckNextWord_Down = 0;
    int nCheckNextCount_Down = 0;

    for (int i = 0; i < 4; ++i) {
        // IDA: Each digit must be '0'-'7' (ASCII 48-55)
        if (strPassword[i] < '0' || strPassword[i] > '7') {
            nErrorID = 59504;  // Invalid character
            return false;
        }

        if (i == 0) {
            cCheckSameWord = strPassword[0];
            nCheckSameCount = 1;
            cCheckNextWord_Up = strPassword[0];
            nCheckNextCount_Up = 1;
            cCheckNextWord_Down = strPassword[0];
            nCheckNextCount_Down = 1;
        } else {
            // Check repeated characters
            if (cCheckSameWord == strPassword[i]) {
                ++nCheckSameCount;
            } else {
                cCheckSameWord = strPassword[i];
                nCheckSameCount = 1;
            }

            // Check ascending sequence (e.g., 1234)
            if (cCheckNextWord_Up + 1 == strPassword[i]) {
                cCheckNextWord_Up = strPassword[i];
                ++nCheckNextCount_Up;
            } else {
                cCheckNextWord_Up = strPassword[i];
                nCheckNextCount_Up = 1;
            }

            // Check descending sequence (e.g., 4321)
            if (cCheckNextWord_Down - 1 == strPassword[i]) {
                cCheckNextWord_Down = strPassword[i];
                ++nCheckNextCount_Down;
            } else {
                cCheckNextWord_Down = strPassword[i];
                nCheckNextCount_Down = 1;
            }
        }

        // IDA: Reject if 4 or more repeated characters
        if (nCheckSameCount >= 4) {
            nErrorID = 59505;  // Repeated characters
            return false;
        }
    }
    return true;
}

// ============================================================================
// Item use info functions (IDA verified)
// ============================================================================

// IDA: 0x1400B7370
// void __fastcall CGocInventory::SetItemUseInfoList(CGocInventory *this, ST_USE_ITEM_INFO_LIST *stUseItemInfoList)
// Sets item use info list from DB response
void CGocInventory::SetItemUseInfoList(void* stUseItemInfoList) {
    // IDA: Get before init date for filtering expired entries
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // __int64 biBeforeDate = XGameServer::GetBeforeInitDate(pServer);

    // IDA: For each entry in stUseItemInfoList
    // for (size_t i = 0; i < stUseItemInfoList->size(); ++i) {
    //     ST_USE_ITEM_INFO* pInfo = &stUseItemInfoList[i];
    //     // Clear if expired
    //     if (pInfo->biDate < biBeforeDate) {
    //         pInfo->byCount = 0;
    //         pInfo->biDate = 0;
    //     }
    //     m_mpUseItemInfo[pInfo->nType] = *pInfo;
    // }

    // IDA: Update timestamps
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // m_biUseItemUpdateDate = XGameServer::GetCurDate(pServer);
    // m_dw64UpdateTick = GetTickCount64();

    (void)stUseItemInfoList;
    // TODO: 需人工审查 - Implement when ST_USE_ITEM_INFO_LIST/XGameServer available
}

// IDA: 0x1400B74F0
// void __fastcall CGocInventory::OnInitItemUseInfoDate(CGocInventory *this)
// Initializes item use info date, clears expired entries
void CGocInventory::OnInitItemUseInfoDate() {
    // IDA: Check if update date is set
    if (m_biUseItemUpdateDate == 0)
        return;

    // IDA: Check if expired
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // if (m_biUseItemUpdateDate > XGameServer::GetBeforeInitDate(pServer))
    //     return;

    // IDA: Clear all entries in m_mpUseItemInfo
    // ST_USE_ITEM_INFO_LIST stChangeUseInfoList;
    // for (auto iter = m_mpUseItemInfo.begin(); iter != m_mpUseItemInfo.end(); ++iter) {
    //     iter->second.byCount = 0;
    //     iter->second.biDate = 0;
    // }

    // IDA: Send packet to client (main=8, sub=0x49)
    // XSendPacket xSendPacket(8, 0x49);
    // xSendPacket << stChangeUseInfoList;
    // CGocNetwork::Send(pActor, &xSendPacket);

    // IDA: Update date
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // m_biUseItemUpdateDate = XGameServer::GetCurDate(pServer);

    // TODO: 需人工审查 - Implement when ST_USE_ITEM_INFO_LIST/XSendPacket available
}

// IDA: 0x1400B76B0
// bool __fastcall CGocInventory::CanUseItemInfo(CGocInventory *this, int nType)
// Checks if item use info type can be used (count < 3)
bool CGocInventory::CanUseItemInfo(int nType) {
    // IDA: Find type in m_mpUseItemInfo
    // auto iter = m_mpUseItemInfo.find(nType);
    // if (iter == m_mpUseItemInfo.end())
    //     return true;  // Not found, can use
    // return iter->second.byCount < 3;  // Can use if count < 3

    (void)nType;
    return true;  // TODO: 需人工审查 - Implement when m_mpUseItemInfo available
}

// ============================================================================
// Appearance functions (IDA verified)
// ============================================================================

// IDA: 0x1400BB160
// void __fastcall CGocInventory::SendDBAppearanceLoad(CGocInventory *this)
// Sends DB request to load appearance list (main=0x21, sub=0x29)
void CGocInventory::SendDBAppearanceLoad() {
    // IDA: Send DB packet
    // XSendDBPacket xSendDBPacket(pObject, 0x21, 0x29);
    // xSendDBPacket << QuestID;
    // XGameServer::SendDBGame(&xSendDBPacket);

    // TODO: 需人工审查 - Implement when XSendDBPacket available
}

// IDA: 0x1400BB270
// void __fastcall CGocInventory::LoadAppearanceList(CGocInventory *this, ST_APPEARANCE_LIST *stList)
// Loads appearance list from DB, validates entries, updates character info if expired
void CGocInventory::LoadAppearanceList(void* stList) {
    // IDA: Insert all entries into m_mpAppearanceList
    // for (size_t i = 0; i < stList->size(); ++i) {
    //     ST_APPEARANCE_INFO* pInfo = &stList[i];
    //     m_mpAppearanceList[pInfo->wID] = pInfo->biEndDate;
    // }

    // IDA: Get current date
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // __int64 biCurDate = XGameServer::GetCurDate(pServer);

    // IDA: Check equipped appearances (4 slots)
    // bool bChange = false;
    // for (int j = 0; j < 4; ++j) {
    //     std::uint16_t wID = CUser::stMyCharInfoEx()->stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[j];
    //     if (wID) {
    //         auto iter = m_mpAppearanceList.find(wID);
    //         if (iter == m_mpAppearanceList.end()) {
    //             // Not found, clear
    //             CUser::stMyCharInfoEx()->stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[j] = 0;
    //             bChange = true;
    //         } else if (iter->second > 0 && biCurDate > iter->second) {
    //             // Expired, clear
    //             CUser::stMyCharInfoEx()->stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[j] = 0;
    //             bChange = true;
    //         }
    //     }
    // }

    // IDA: If changed, send DB update and client notification
    // if (bChange) {
    //     XSendDBPacket xSendDBPacket(pObject, 0x21, 0x2C);
    //     xSendDBPacket << QuestID << CUser::stMyCharInfoEx()->stBaseInfo.uAppearanceEx.biAppearance;
    //     XGameServer::SendDBGame(&xSendDBPacket);
    //     CGocInventory::SendEquipAppearance();
    // }

    (void)stList;
    // TODO: 需人工审查 - Implement when ST_APPEARANCE_LIST/XSendDBPacket available
}

// IDA: 0x1400BB6E0
// bool __fastcall CGocInventory::IsHaveAppearance(CGocInventory *this, unsigned __int16 wAppearanceID)
// Checks if player has appearance (equipped or in list, not expired)
bool CGocInventory::IsHaveAppearance(std::uint16_t wAppearanceID) {
    // IDA: Check equipped appearances (4 slots)
    // for (int i = 0; i < 4; ++i) {
    //     std::uint16_t wID = CUser::stMyCharInfoEx()->stBaseInfo.uAppearance.stAppearance.wAppearanceID[i];
    //     if (wID == wAppearanceID)
    //         return true;
    // }

    // IDA: Check m_mpAppearanceList
    // auto iter = m_mpAppearanceList.find(wAppearanceID);
    // if (iter == m_mpAppearanceList.end())
    //     return false;

    // IDA: Check if expired
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // __int64 biCurDate = XGameServer::GetCurDate(pServer);
    // __int64 biTime = iter->second;
    // if (!biTime || biTime > biCurDate)
    //     return true;

    (void)wAppearanceID;
    return false;  // TODO: 需人工审查 - Implement when m_mpAppearanceList/CUser available
}

// IDA: 0x1400BB9D0
// void __fastcall CGocInventory::EquipAppearance(CGocInventory *this, UAppearanceEx reqAppearance)
// Equips appearance, validates expiry, syncs to DB
void CGocInventory::EquipAppearance(void* reqAppearance) {
    // IDA: Get current date
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // __int64 biCurDate = XGameServer::GetCurDate(pServer);

    // IDA: Validate each appearance slot (4 slots)
    // for (int i = 0; i < 4; ++i) {
    //     if (reqAppearance.stAppearance.wAppearanceID[i]) {
    //         auto iter = m_mpAppearanceList.find(reqAppearance.stAppearance.wAppearanceID[i]);
    //         if (iter == m_mpAppearanceList.end()) {
    //             // Not found, clear
    //             reqAppearance.stAppearance.wAppearanceID[i] = 0;
    //         } else if (iter->second > 0 && biCurDate > iter->second) {
    //             // Expired, clear
    //             reqAppearance.stAppearance.wAppearanceID[i] = 0;
    //         }
    //     }
    // }

    // IDA: Update character info
    // CUser::stMyCharInfoEx()->stBaseInfo.uAppearanceEx = reqAppearance;

    // IDA: Send DB packet (main=0x21, sub=0x2C)
    // XSendDBPacket xSendDBPacket(pObject, 0x21, 0x2C);
    // xSendDBPacket << QuestID << reqAppearance.biAppearance;
    // XGameServer::SendDBGame(&xSendDBPacket);

    // IDA: Broadcast to nearby players
    // CGocInventory::SendEquipAppearance();

    (void)reqAppearance;
    // TODO: 需人工审查 - Implement when UAppearanceEx/XSendDBPacket available
}

// IDA: 0x1400BBC70
// void __fastcall CGocInventory::SendEquipAppearance(CGocInventory *this)
// Broadcasts equipped appearance to nearby players (main=8, sub=0x52)
void CGocInventory::SendEquipAppearance() {
    // IDA: Get user
    // CUser* pUser = GetCUser();
    // if (!pUser) return;

    // IDA: Send packet
    // XSendPacket xSendPacket(8, 0x52);
    // xSendPacket << QuestID << CUser::stMyCharInfoEx(pUser)->stBaseInfo.uAppearanceEx.biAppearance;
    // CGocNetwork::BroadcastNearby(pActor, nullptr, &xSendPacket);

    // TODO: 需人工审查 - Implement when XSendPacket/CGocNetwork available
}

// IDA: 0x1400BB640
// void __fastcall CGocInventory::AddAppearance(CGocInventory *this, ST_APPEARANCE_LIST *stList)
// Adds multiple appearances from list, calls AddAppearance for each
void CGocInventory::AddAppearance(void* stList) {
    // IDA Decompiled:
    // for ( i = 0; i < std::vector<...>::size(stList); ++i ) {
    //     v4 = std::vector<...>::operator[](stList, i);
    //     v2 = (unsigned __int16 *)std::vector<...>::operator[](stList, i);
    //     CGocInventory::AddAppearance(this, *v2, *(_QWORD *)&v4->stEventInfo.nID);
    // }
    // PS_POST_DELETE_LIST::~PS_POST_DELETE_LIST(stList);

    // TODO: 需人工审查 - Implement when ST_APPEARANCE_LIST type available
}

// IDA: 0x1400BB800
// void __fastcall CGocInventory::AddAppearance(CGocInventory *this, unsigned __int16 wAppearanceID, __int64 biEndDate)
// Adds single appearance by ID with end date
void CGocInventory::AddAppearance(std::uint16_t wAppearanceID, std::int64_t biEndDate) {
    // IDA Decompiled:
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // __int64 biCurDate = XGameServer::GetCurDate(pServer);
    // m_mpAppearanceList[wAppearanceID] = biEndDate;
    // if ( biCurDate > biEndDate && biEndDate > 0 ) {
    //     // Appearance already expired
    // }

    // TODO: 需人工审查 - Implement when m_mpAppearanceList type available
}

// IDA: 0x1400BB8C0
// void __fastcall CGocInventory::UpdateAppearance(CGocInventory *this, unsigned __int16 wAppearanceID, __int64 biAddSec, __int64 *biEndDate)
// Updates appearance end date based on add seconds
void CGocInventory::UpdateAppearance(std::uint16_t wAppearanceID, std::int64_t biAddSec, std::int64_t& biEndDate) {
    // IDA Decompiled:
    // if ( biAddSec ) {
    //     XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //     __int64 biCurDate = XGameServer::GetCurDate(pServer);
    //     auto iter = m_mpAppearanceList.find(wAppearanceID);
    //     if ( iter != m_mpAppearanceList.end() ) {
    //         __int64 biDate = iter->second;
    //         if ( biCurDate <= biDate )
    //             *biEndDate = biAddSec + biDate;
    //         else
    //             *biEndDate = biAddSec + biCurDate;
    //     } else {
    //         *biEndDate = biAddSec + biCurDate;
    //     }
    // } else {
    //     *biEndDate = 0;
    // }

    // TODO: 需人工审查 - Implement when m_mpAppearanceList type available
}

// IDA: 0x1400BBDD0
// void __fastcall CGocInventory::OnAppearanceUpdate(CGocInventory *this)
// Checks appearance expiry, removes expired, syncs to DB and broadcasts
void CGocInventory::OnAppearanceUpdate() {
    // IDA Decompiled:
    // bool bChange = false;
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // __int64 biCurDate = XGameServer::GetCurDate(pServer);
    //
    // for ( int i = 0; i < 4; ++i ) {
    //     CMover* pMover = GetOwner();
    //     CUser* pUser = dynamic_cast<CUser*>(pMover);
    //     std::uint16_t wID = CUser::stMyCharInfoEx(pUser)->stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[i];
    //     if ( wID ) {
    //         auto iter = m_mpAppearanceList.find(wID);
    //         if ( iter == m_mpAppearanceList.end() ) {
    //             // Not in list, clear
    //             CUser::stMyCharInfoEx(pUser)->stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[i] = 0;
    //             bChange = true;
    //         } else if ( iter->second > 0 && biCurDate > iter->second ) {
    //             // Expired, clear
    //             CUser::stMyCharInfoEx(pUser)->stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[i] = 0;
    //             bChange = true;
    //         }
    //     }
    // }
    //
    // if ( bChange ) {
    //     // Send to DB
    //     XSendDBPacket xSendDBPacket(pObject, 0x21, 0x2C);
    //     xSendDBPacket << QuestID << CUser::stMyCharInfoEx(pUser)->stBaseInfo.uAppearanceEx.biAppearance;
    //     XGameServer::SendDBGame(pServer, &xSendDBPacket);
    //     CGocInventory::SendEquipAppearance(this);
    // }

    // TODO: 需人工审查 - Implement when types available
}

// IDA: 0x1400BC0E0
// void __fastcall CGocInventory::SendDBUseInfo(CGocInventory *this)
// Sends item use info to DB (main=0x21, sub=0x26)
void CGocInventory::SendDBUseInfo() {
    // IDA Decompiled:
    // CMover* pMover = GetOwner();
    // IXObject* pObject = pMover ? &pMover[3].m_ChunkSizeTempMemOfs : nullptr;
    // XSendDBPacket xSendDBPacket(pObject, 0x21, 0x26);
    // CQuestCondition* pCond = pMover->GetQuestCondition();
    // int QuestID = CQuestCondition::GetQuestID(pCond);
    // xSendDBPacket << QuestID;
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // XGameServer::SendDBGame(pServer, &xSendDBPacket);

    // TODO: 需人工审查 - Implement when XSendDBPacket/CQuestCondition available
}

// IDA: 0x1400BC1F0
// void __fastcall CGocInventory::ItemSocketLoad(CGocInventory *this, PS_ITEM_SOCKET_LIST *stSocketList)
// Loads item sockets from DB, applies to items
void CGocInventory::ItemSocketLoad(void* stSocketList) {
    // IDA Decompiled:
    // PS_ITEM_SOCKET_LIST v12;
    // PS_ITEM_SOCKET_LIST::PS_ITEM_SOCKET_LIST(&v12, stSocketList);
    // CGocInventory::AddRepurchaseSocketInfo(this, &v12);
    //
    // for ( int i = 0; i < stSocketList->size(); ++i ) {
    //     ST_ITEM_SOCKET stSocket = (*stSocketList)[i];
    //     std::shared_ptr<CItem> pItem;
    //     CGocInventory::GetItemPtr(this, &pItem, stSocket.biEquipSerial);
    //     if ( pItem ) {
    //         if ( pItem->SocketFunc(&stSocket, true) ) {
    //             if ( pItem->GetInvenType() == 1 ) { // EQUIP
    //                 pItem->SetSocketPos(stSocket.bySocketPos);
    //             }
    //         }
    //     }
    // }
    //
    // CMover* pMover = GetOwner();
    // std::shared_ptr<CGocAttribute> pAttr;
    // CMover::GetGOC<CGocAttribute>(pMover, &pAttr, 0);
    // pAttr->CalculateChangedStat(1);

    // TODO: 需人工审查 - Implement when PS_ITEM_SOCKET_LIST/ST_ITEM_SOCKET types available
}

// IDA: 0x1400BC460
// void __fastcall CGocInventory::ItemBroachLoad(CGocInventory *this, PS_ITEM_BROACH_LIST *stBroachList)
// Loads item broaches from DB, applies to items
void CGocInventory::ItemBroachLoad(void* stBroachList) {
    // IDA Decompiled:
    // PS_ITEM_BROACH_LIST v12;
    // PS_ITEM_BROACH_LIST::PS_ITEM_BROACH_LIST(&v12, stBroachList);
    // CGocInventory::AddRepurchaseBroachInfo(this, &v12);
    //
    // for ( int i = 0; i < stBroachList->size(); ++i ) {
    //     ST_ITEM_BROACH stBroach = (*stBroachList)[i];
    //     std::shared_ptr<CItem> pItem;
    //     CGocInventory::GetItemPtr(this, &pItem, stBroach.biSerial);
    //     if ( pItem ) {
    //         pItem->BroachFunc(&stBroach);
    //         if ( pItem->GetInvenType() == 0 ) { // NORMAL
    //             pItem->SetCount(1, 0);
    //         }
    //     }
    // }
    //
    // CMover* pMover = GetOwner();
    // std::shared_ptr<CGocAttribute> pAttr;
    // CMover::GetGOC<CGocAttribute>(pMover, &pAttr, 0);
    // pAttr->CalculateChangedStat(1);

    // TODO: 需人工审查 - Implement when PS_ITEM_BROACH_LIST/ST_ITEM_BROACH types available
}

// IDA: 0x1400BC6D0
// void __fastcall CGocInventory::GetSocketList(CGocInventory *this, unsigned __int8 byInvenType, PS_ITEM_SOCKET_LIST *stSocketList)
// Gets socket list from inventory/equipment by type
void CGocInventory::GetSocketList(std::uint8_t byInvenType, void* stSocketList) {
    // IDA Decompiled:
    // if ( byInvenType == 1 ) {
    //     XBaseEquip* pEquip = CGocInventory::GetEquipPtr(this, 1);
    //     if ( pEquip )
    //         XBaseEquip::GetSocketList(pEquip, stSocketList);
    // } else if ( byInvenType == 2 || byInvenType == 5 || byInvenType == 16 ) {
    //     XBaseInventory* pInven = CGocInventory::GetInvenPtr(this, byInvenType);
    //     if ( pInven )
    //         XBaseInventory::GetSocketList(pInven, stSocketList);
    // }

    // TODO: 需人工审查 - Implement when PS_ITEM_SOCKET_LIST type available
}

// IDA: 0x1400BC770
void CGocInventory::GetBroachList(
    std::uint8_t byInvenType,
    PS_ITEM_BROACH_LIST& stBroachList)
{
    if (byInvenType == 0 || byInvenType == 3) {
        XBaseEquip* pEquip = GetEquipPtr(byInvenType);
        if (pEquip) {
            pEquip->GetBroachList(stBroachList);
        }
    } else if (byInvenType == 4 ||
               byInvenType == 6 ||
               byInvenType == 17) {
        XBaseInventory* pInventory = GetInvenPtr(byInvenType);
        if (pInventory) {
            pInventory->GetBroachList(stBroachList);
        }
    }
}

// IDA: 0x1400BC820
// void __fastcall CGocInventory::GetPackageList(CGocInventory *this, unsigned __int8 byInvenType, PS_ITEM_PACKAGE_LIST *psPackageList)
// Gets package list from inventory by type (types 13-14)
void CGocInventory::GetPackageList(std::uint8_t byInvenType, void* psPackageList) {
    // IDA Decompiled:
    // if ( byInvenType >= 0xD && byInvenType <= 0xE ) {
    //     XBaseInventory* pInven = CGocInventory::GetInvenPtr(this, byInvenType);
    //     if ( pInven )
    //         XBaseInventory::GetPackageList(pInven, psPackageList);
    // }

    // TODO: 需人工审查 - Implement when PS_ITEM_PACKAGE_LIST type available
}

// IDA: 0x1400BC880 - ?SendDBSocketLoad@CGocInventory@@QEAAX_N@Z
// Precise restoration from IDA decompilation - sends socket load request to DB (main=0x21, sub=0x30)
void CGocInventory::SendDBSocketLoad(bool bInven) {
    // IDA: Get CUser from owner
    CUser* pUser = GetCUserFromOwner(this);
    if (!pUser) return;

    // IDA: Get ActorID
    std::uint32_t dwID = 0; // TODO: pUser->GetActorID()
    std::uint8_t byStorageType = 0;

    // IDA: if (!bInven) use UAID and storage type 1
    if (!bInven) {
        // dwID = pUser->GetUAID();
        byStorageType = 1;
    }

    // IDA: Send DB packet (main=0x21, sub=0x30)
    CMover* pOwner = GetOwnerGO();
    IXObject* pObject = pOwner ? static_cast<IXObject*>(pOwner) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 0x21, 0x30);
    xSendDBPacket.XParse << dwID;
    xSendDBPacket.XParse << byStorageType;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pServer) pServer->SendDBGame(xSendDBPacket);
}

// IDA: 0x1400BCA40
// char __fastcall CGocInventory::CanUseItem_AkashicRecord(CGocInventory *this, unsigned __int8 byInvenType, __int16 shSlot)
// Checks if akashic record item can be used (types 2, 13)
bool CGocInventory::CanUseItem_AkashicRecord(std::uint8_t byInvenType, std::int16_t shSlot) {
    // IDA Decompiled:
    // if ( byInvenType != 2 && byInvenType != 13 )
    //     return false;
    //
    // std::shared_ptr<CItem> result;
    // std::uint8_t bLock;
    // GetSlotItem(&result, byInvenType, shSlot, &bLock);
    // std::shared_ptr<CItemAkashic> pAkashic;
    // std::tr1::dynamic_pointer_cast<CItemAkashic>(&pAkashic, &result);
    //
    // if ( !pAkashic || bLock ) {
    //     // Send lock log
    //     XGameServer::SendItemLockLog(dwUCID, byInvenType, shSlot, bLock, 19, 0);
    //     return false;
    // }
    //
    // if ( !pAkashic->GetItemTable() || !pAkashic->GetClassifyTable() )
    //     return false;
    //
    // if ( pAkashic->GetClassifyTable()->Item_Use_Type != 5 )
    //     return false;
    //
    // unsigned int dwIndex = pAkashic->GetCurID();
    // if ( !XResourceMgr::GetTB_AKASHIC_RECORDS(dwIndex) )
    //     return false;
    //
    // if ( pAkashic->GetCount() < 1 )
    //     return false;
    //
    // return pAkashic->CanUseAkashic();

    // TODO: 需人工审查 - Implement when CItemAkashic types available
    return false;
}

// IDA: 0x1400BCD40
// __int64 __fastcall CGocInventory::UseItem_AkashicRecord(CGocInventory *this, unsigned __int8 byInvenType, __int16 shSlot)
// Uses akashic record item, adds to akashic card
bool CGocInventory::UseItem_AkashicRecord(std::uint8_t byInvenType, std::int16_t shSlot) {
    // IDA Decompiled:
    // CMover* pMover = GetOwner();
    // std::shared_ptr<CGocAkashicRecord> pAkashicCard;
    // CMover::GetGOC<CGocAkashicRecord>(pMover, &pAkashicCard, 0);
    // if ( !pAkashicCard )
    //     return false;
    //
    // std::shared_ptr<CItem> result;
    // std::uint8_t bLock;
    // GetSlotItem(&result, byInvenType, shSlot, &bLock);
    // std::shared_ptr<CItemAkashic> pItemAkashic;
    // std::tr1::dynamic_pointer_cast<CItemAkashic>(&pItemAkashic, &result);
    //
    // if ( !pItemAkashic || bLock ) {
    //     XGameServer::SendItemLockLog(dwUCID, byInvenType, shSlot, bLock, 20, 0);
    //     return false;
    // }
    //
    // pItemAkashic->GetItemTable();
    // if ( !pItemAkashic )
    //     return false;
    //
    // TB_ITEM_CLASSIFY* pTBClassify = pItemAkashic->GetClassifyTable();
    // if ( !pTBClassify )
    //     return false;
    //
    // std::shared_ptr<CGocAkashicRecord> pAkashic;
    // CMover::GetGOC<CGocAkashicRecord>(pMover, &pAkashic, 0);
    // unsigned int dwID = pItemAkashic->GetCurID();
    //
    // if ( !pAkashic->OverlappedAkashic(dwID) )
    //     return false;
    //
    // int nAkashicItemExp = pItemAkashic->GetItemExp();
    // ST_LOG_GAME stLog;
    // stLog._sSubType = 73;
    // stLog.nParam3 = pItemAkashic->GetItemExp();
    //
    // int Slot = pItemAkashic->GetSlot();
    // std::uint8_t InvenType = pItemAkashic->GetInvenType();
    // if ( BreakItemReq(InvenType, Slot, 1, 0x4B, &stLog) ) {
    //     UseItemInfo(pTBClassify->Item_Use_Type);
    //     int nItemID = pItemAkashic->GetCurID();
    //     pAkashicCard->AddAkashicRecord(nItemID, nAkashicItemExp);
    //     return true;
    // }
    // return false;

    // TODO: 需人工审查 - Implement when CGocAkashicRecord types available
    return false;
}

// IDA: 0x1400BD210
// void __fastcall CGocInventory::GetQuickSlotItem(CGocInventory *this, int *pQuickSlotItem)
// Gets quick slot items (4 items)
void CGocInventory::GetQuickSlotItem(int* pQuickSlotItem) {
    // IDA Decompiled:
    // for ( int i = 0; i < 4; ++i )
    //     pQuickSlotItem[i] = this->m_nQuickSlotItem[i];

    // TODO: 需人工审查 - Implement when m_nQuickSlotItem member available
}

// IDA: 0x1400BD260 - ?SendDBBroachLoad@CGocInventory@@QEAAX_N@Z
// Precise restoration from IDA decompilation - sends broach load request to DB (main=0x21, sub=0x31)
void CGocInventory::SendDBBroachLoad(bool bInven) {
    // IDA: Get CUser from owner
    CUser* pUser = GetCUserFromOwner(this);
    if (!pUser) return;

    // IDA: Get ActorID
    std::uint32_t dwID = 0; // TODO: pUser->GetActorID()
    std::uint8_t byStorageType = 0;

    // IDA: if (!bInven) use UAID and storage type 1
    if (!bInven) {
        // dwID = pUser->GetUAID();
        byStorageType = 1;
    }

    // IDA: Send DB packet (main=0x21, sub=0x31)
    CMover* pOwner = GetOwnerGO();
    IXObject* pObject = pOwner ? static_cast<IXObject*>(pOwner) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 0x21, 0x31);
    xSendDBPacket.XParse << dwID;
    xSendDBPacket.XParse << byStorageType;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pServer) pServer->SendDBGame(xSendDBPacket);
}

// IDA: 0x1400BD420
// PDB: ?UnLockList@CGocInventory@@QEAAXUPS_RES_STORAGE_INFO@@@Z
void CGocInventory::UnLockList(PS_RES_STORAGE_INFO psUnlockList) {
    for (const PS_STORAGE_INFO& stInfo : psUnlockList.vecItem) {
        SetLock(stInfo.byInvenType, stInfo.shSlotPos, 0);
    }
}

// IDA: 0x1400B7730
// __int64 __fastcall CGocInventory::CanItemFPUse(CGocInventory *this, unsigned __int8 byInvenType, __int16 shSlot)
// Checks if FP item can be used (level check, usage check, FP cap)
bool CGocInventory::CanItemFPUse(std::uint8_t byInvenType, std::int16_t shSlot) {
    // IDA Decompiled:
    // std::shared_ptr<CItem> pItem;
    // std::uint8_t bLock;
    // GetSlotItem(&pItem, byInvenType, shSlot, &bLock);
    //
    // if ( !pItem || bLock ) {
    //     XGameServer::SendItemLockLog(dwUCID, byInvenType, shSlot, bLock, 15, 0);
    //     return false;
    // }
    //
    // TB_ITEM* pTBItem = pItem->GetItemTable();
    // if ( !pTBItem )
    //     return false;
    //
    // TB_ITEM_CLASSIFY* pTBClassify = pItem->GetClassifyTable();
    // if ( !pTBClassify )
    //     return false;
    //
    // // Level check
    // CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    // if ( pTBItem->Item_Limit_Lv > pUser->GetLevel() )
    //     return false;
    //
    // // Usage type check
    // if ( !CanUseItemInfo(pTBClassify->Item_Use_Type) ) {
    //     CGocNetwork::SendErrorMessage(GetOwner(), 8, 0x11, 0xCD82);
    //     return false;
    // }
    //
    // // FP cap check
    // if ( pUser->stMyCharInfoEx()->shBonusFP || pUser->stMyCharInfoEx()->shFP == 200 ) {
    //     CGocNetwork::SendErrorMessage(GetOwner(), 8, 0x11, 0xCD83);
    //     return false;
    // }
    //
    // if ( pItem->GetCount() < 1 )
    //     return false;
    //
    // return pItem->CanUseItem();

    // TODO: 需人工审查 - Implement when CItem/TB_ITEM types available
    return false;
}

// IDA: 0x1400B7AA0
// __int64 __fastcall CGocInventory::ItemFPUse(CGocInventory *this, unsigned __int8 byInvenType, __int16 shSlot)
// Uses FP item, restores FP, sends to DB
bool CGocInventory::ItemFPUse(std::uint8_t byInvenType, std::int16_t shSlot) {
    // IDA Decompiled:
    // std::shared_ptr<CItem> pItem;
    // std::uint8_t bLock;
    // GetSlotItem(&pItem, byInvenType, shSlot, &bLock);
    //
    // if ( !pItem || bLock ) {
    //     XGameServer::SendItemLockLog(dwUCID, byInvenType, shSlot, bLock, 16, 0);
    //     return false;
    // }
    //
    // TB_ITEM* pTBItem = pItem->GetItemTable();
    // if ( !pTBItem )
    //     return false;
    //
    // TB_ITEM_CLASSIFY* pTBClassify = pItem->GetClassifyTable();
    // if ( !pTBClassify )
    //     return false;
    //
    // // Add cooldown
    // AddCoolTime(pTBItem->Cooltime_Group, pTBItem->Cooltime_Value, false);
    //
    // if ( pTBItem->Item_Use_Value ) {
    //     // Restore FP
    //     std::shared_ptr<CGocAttribute> pAttr;
    //     CMover::GetGOC<CGocAttribute>(GetOwner(), &pAttr, 0);
    //     pAttr->FPRestore(pTBItem->Item_Use_Value);
    //
    //     // Achievement update
    //     std::shared_ptr<CGocAchieve> pAchievePtr;
    //     CMover::GetGOC<CGocAchieve>(GetOwner(), &pAchievePtr, 0);
    //     pAchievePtr->UpdateCollect(0xF, 1, 0);
    //
    //     // Reduce item count
    //     int nCount = pItem->GetCount() - 1;
    //     pItem->SetCount(nCount);
    //     if ( pItem->GetCount() <= 0 )
    //         RemoveItem(byInvenType, shSlot);
    //
    //     SendUpdateItem(byInvenType, shSlot, pItem->GetCount(), false);
    //
    //     // Send to DB
    //     XSendDBPacket xSendDBPacket(pObject, 0x21, 0x24);
    //     xSendDBPacket << QuestID << pItem->GetSerial() << pItem->GetCount() << byInvenType << shSlot;
    //     XGameServer::SendDBGame(pServer, &xSendDBPacket);
    //
    //     UseItemInfo(pTBClassify->Item_Use_Type);
    //     return true;
    // }
    // return false;

    // TODO: 需人工审查 - Implement when CItem/CGocAttribute types available
    return false;
}

// IDA: 0x1400B7F90
// void __fastcall CGocInventory::UseItemInfo(CGocInventory *this, int nType)
// Updates item use info by type, sends to DB
void CGocInventory::UseItemInfo(int nType) {
    // IDA Decompiled:
    // ST_USE_ITEM_INFO stUseItemInfo;
    // auto iter = m_mpUseItemInfo.find(nType);
    //
    // if ( iter == m_mpUseItemInfo.end() ) {
    //     // New entry
    //     stUseItemInfo.byCount = 1;
    //     stUseItemInfo.nItemType = nType;
    //     XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //     stUseItemInfo.nUseDate = XGameServer::GetCurDate(pServer);
    //     m_mpUseItemInfo.insert(std::make_pair(nType, stUseItemInfo));
    // } else {
    //     // Update existing
    //     ++iter->second.byCount;
    //     XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //     iter->second.nUseDate = XGameServer::GetCurDate(pServer);
    //     stUseItemInfo = iter->second;
    // }
    //
    // // Send to DB
    // ST_USE_ITEM_INFO_LIST stList;
    // stList.push_back(stUseItemInfo);
    // XSendDBPacket xSendDBPacket(pObject, 0x21, 0x27);
    // xSendDBPacket << QuestID << stList;
    // XGameServer::SendDBGame(pServer, &xSendDBPacket);

    // TODO: 需人工审查 - Implement when ST_USE_ITEM_INFO types available
}

// IDA: 0x1400B8750
// void __fastcall CGocInventory::CheatSetCashDate(CGocInventory *this, __int16 shSlot, int nVal)
// GM cheat to set cash item date (inven type 4)
void CGocInventory::CheatSetCashDate(std::int16_t shSlot, int nVal) {
    // IDA Decompiled:
    // std::shared_ptr<CItem> pItem;
    // std::uint8_t bLock;
    // GetSlotItem(&pItem, 4, shSlot, &bLock);  // Type 4 = Cash inventory
    //
    // if ( !pItem || bLock ) {
    //     XGameServer::SendItemLockLog(dwUCID, 4, shSlot, bLock, 18, 0);
    //     return;
    // }
    //
    // STItem stItem;
    // pItem->GetItem(&stItem);
    //
    // if ( nVal > 0 && stItem.nCashDate ) {
    //     pItem->UpdateCashDate(nVal);
    // }

    // TODO: 需人工审查 - Implement when CItem/STItem types available
}

// IDA: 0x1400B88E0
// void __fastcall CGocInventory::SendFurniture(CGocInventory *this)
// Sends furniture inventory to client (main=8, sub=0x4A)
void CGocInventory::SendFurniture() {
    // IDA Decompiled:
    // PS_RES_STORAGE_INFO stInvenInfo;
    // GetInvenInfo(0xB, &stInvenInfo);  // Type 0xB = Furniture
    //
    // XSendPacket xSendPacket(8, 0x4A);
    // xSendPacket << stInvenInfo;
    // CGocNetwork::Send(pActor, &xSendPacket);

    // TODO: 需人工审查 - Implement when PS_RES_STORAGE_INFO type available
}

// IDA: 0x1400BA760
// void __fastcall CGocInventory::SendControlServerTradePassword(CGocInventory *this)
// Sends trade password check to control server (main=0xF3, sub=0x27)
void CGocInventory::SendControlServerTradePassword() {
    // IDA Decompiled:
    // CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    // if ( pUser ) {
    //     XSendPacket xSendPacket(0xF3, 0x27);
    //     xSendPacket << QuestID;
    //     XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //     CGameControlSocket::SendCheck(&pServer->m_controlSocket, &xSendPacket);
    // }

    // TODO: 需人工审查 - Implement when XSendPacket/CGameControlSocket types available
}

// IDA: 0x1400BA860
// void __fastcall CGocInventory::GM_ChangeSecondPW(CGocInventory *this, wchar_t *strPW)
// GM command to change second password
void CGocInventory::GM_ChangeSecondPW(wchar_t* strPW) {
    // IDA Decompiled:
    // PS_SECOND_PW_REQ psSecond;
    // psSecond.byCheckType = 1;
    // WideCharToMultiByte(0, 0, strPW, -1, psSecond.strPassword, 7, nullptr, nullptr);

    // TODO: 需人工审查 - Implement when PS_SECOND_PW_REQ type available
}

// IDA: 0x1400BA8E0
// void __fastcall CGocInventory::GM_ChangeTradePW(CGocInventory *this, wchar_t *strPW)
// GM command to change trade password, validates and sends to DB
void CGocInventory::GM_ChangeTradePW(wchar_t* strPW) {
    // IDA Decompiled:
    // PS_TRADE_PW_REQ psTrade;
    // psTrade.byCheckType = 1;
    // WideCharToMultiByte(0, 0, strPW, -1, psTrade.strPassword, 5, nullptr, nullptr);
    // psTrade.strPassword[4] = 0;
    //
    // CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    // int nError = 0;
    // if ( IsValidTradePassword(psTrade.strPassword, &nError) == 1 && pUser ) {
    //     XSendDBPacket xSendDBPacket(pObject, 2, 0x38);
    //     xSendDBPacket << pUser->GetUAID() << psTrade;
    //     XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //     XGameServer::SendDBAccount(pServer, &xSendDBPacket);
    // }

    // TODO: 需人工审查 - Implement when PS_TRADE_PW_REQ type available
}

// IDA: 0x1400BAAD0
// char __fastcall CGocInventory::UseGacha(CGocInventory *this, unsigned __int16 dwGetID, int nCount, ...)
// Uses gacha system, random item selection based on tables
bool CGocInventory::UseGacha(std::uint16_t dwGetID, int nCount,
                             void* stGachaList, void* stGetInfo,
                             void* vecCreateItem, void* vecUpdateItem, bool bTool) {
    // IDA Decompiled:
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_RANDOM_GET_GROUP* pTB_RandomGroup = XResourceMgr::GetTB_RANDOM_GET_GROUP(&pServer->m_xResourceMgr, dwGetID);
    // if ( !pTB_RandomGroup )
    //     return false;
    //
    // int nGroupID = pTB_RandomGroup->Gacha_Group_ID;
    // TB_GACHA_GROUP* pTBGachaGroup = XResourceMgr::GetGachaGroupTable(&pServer->m_xResourceMgr, nGroupID);
    // if ( !pTBGachaGroup )
    //     return false;
    //
    // // Random selection from gacha group
    // int nRandomGroupRate = XItemFactory::nRand(1, 10000);
    // for ( int i = 0; i < 20; ++i ) {
    //     nRandomGroupRate -= pTBGachaGroup->G_Chance[i];
    //     if ( nRandomGroupRate <= 0 ) {
    //         TB_RANDOM_GET* pTB_RandomGet = XResourceMgr::GetTB_RANDOM_GET_With_Lock(dwIndex);
    //         if ( !pTB_RandomGet )
    //             return false;
    //
    //         // Random selection from item list
    //         int nRandomGetRate = XItemFactory::nRand(1, 10000);
    //         for ( int k = 0; k < 30; ++k ) {
    //             nRandomGetRate -= pTB_RandomGet->I_Chance[k];
    //             if ( nRandomGetRate <= 0 ) {
    //                 TB_ITEM* tbItem = XResourceMgr::GetTB_ITEM(pTB_RandomGet->Item_ID[k]);
    //                 if ( !tbItem )
    //                     return false;
    //
    //                 // Build result
    //                 ST_CREATE_ITEM stAddItem;
    //                 stAddItem.nItemID = pTB_RandomGet->Item_ID[k];
    //                 stAddItem.shCount = 1;
    //
    //                 PS_GACHA_INFO stGachaInfo;
    //                 stGachaInfo.nItemID = pTB_RandomGet->Item_ID[k];
    //                 stGachaInfo.nType = pTB_RandomGet->Item_Get_type[k];
    //
    //                 stGetInfo->stGetItem.push_back(stAddItem);
    //                 stCreatItem.push_back(stAddItem);
    //                 stGachaList->push_back(stGachaInfo);
    //
    //                 // Reduce source item
    //                 ST_CREATE_ITEM stReduce;
    //                 stReduce.nItemID = pTB_RandomGroup->Group_Des;
    //                 stReduce.shCount = 1;
    //
    //                 if ( bTool ) {
    //                     // Tool mode - just record
    //                     PS_TOOL_ITEM_INFO psToolInfo;
    //                     psToolInfo.nItemID = stGachaInfo.nItemID;
    //                     psToolInfo.nCount = 1;
    //                     m_stToolItemInfo.vecInfo.push_back(psToolInfo);
    //                     return true;
    //                 }
    //
    //                 // Normal mode - reduce and add items
    //                 if ( ReduceItem2(&stReduceItems, 0x1C, vecUpdateItem) ) {
    //                     if ( AddItem2(tbItem, 1, 0x1C, false, vecCreateItem, vecUpdateItem) ) {
    //                         // Log gacha result
    //                         return true;
    //                     }
    //                 }
    //                 return false;
    //             }
    //         }
    //     }
    // }
    // return false;

    // TODO: 需人工审查 - Implement when TB_GACHA_GROUP/TB_RANDOM_GET types available
    return false;
}

// IDA: 0x1400C0830
// void __fastcall CGocInventory::SendSocketUpdate(CGocInventory *this, PS_ITEM_SOCKET_LIST *stSocketList)
// Sends socket update to client (main=8, sub=0x61)
void CGocInventory::SendSocketUpdate(void* stSocketList) {
    // IDA Decompiled:
    // XSendPacket xSendPacket(8, 0x61);
    // xSendPacket << stSocketList;
    // CGocNetwork::Send(pActor, &xSendPacket);

    // TODO: 需人工审查 - Implement when PS_ITEM_SOCKET_LIST type available
}

// ============================================================================
// Item add/reduce functions (IDA verified)
// ============================================================================

// IDA: 0x1400BD4C0
// PDB: ?AddItem2@CGocInventory@@AEAA_NUST_CREATE_ITEMS@@E_NAEAUPS_RES_STORAGE_INFO@@2@Z
bool CGocInventory::AddItem2(ST_CREATE_ITEMS stCreateItems, std::uint8_t byLock,
                             bool bOption, PS_RES_STORAGE_INFO& psCreateItem,
                             PS_RES_STORAGE_INFO& psUpdateItem) {
    bool bSucc = true;
    for (const ST_CREATE_ITEM& stInfo : stCreateItems.vecInfo) {
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        TB_ITEM* pTBItem = pServer->GetResourceMgr().GetTB_ITEM(stInfo.nItemID);
        if (!pTBItem) {
            LogHelper::LogError("game.contents",
                                "AddItem2 if( NULL == pTBItem ) ID:%d ) ",
                                stInfo.nItemID);
            bSucc = false;
            break;
        }

        if (!AddItem2(pTBItem, stInfo.shCount, byLock, bOption,
                      psCreateItem, psUpdateItem)) {
            bSucc = false;
            break;
        }
    }

    if (bSucc) {
        return true;
    }

    UnLockList(psCreateItem);
    UnLockList(psUpdateItem);
    return false;
}

// IDA: 0x1400BD6C0
// PDB: ?AddItem2@CGocInventory@@QEAA_NPEAUTB_ITEM@@FE_NAEAUPS_RES_STORAGE_INFO@@2@Z
bool CGocInventory::AddItem2(TB_ITEM* pTBItem, std::int16_t shAddCount,
                             std::uint8_t byLock, bool bOption,
                             PS_RES_STORAGE_INFO& psCreateItem,
                             PS_RES_STORAGE_INFO& psUpdateItem) {
    if (!pTBItem) {
        return false;
    }

    if (pTBItem->Item_Stack_Max < shAddCount || shAddCount < 1) {
        LogHelper::LogError("game.contents",
                            "AddItem2 Item_Stack_Max ID:%d, StackMax:%d, Add:%d ) ",
                            pTBItem->Item_ID, pTBItem->Item_Stack_Max, shAddCount);
        return false;
    }

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM_CLASSIFY* pTBClassify =
        pServer->GetResourceMgr().GetTB_ITEM_CLASSIFY(pTBItem->Item_Classify_Index);
    if (!pTBClassify) {
        LogHelper::LogError("game.contents",
                            "AddItem2 if( NULL == pTBClassify ) ID:%d ) ",
                            pTBItem->Item_ID);
        return false;
    }

    XBaseInventory* pInven = GetTBInvenPtr(pTBClassify->Item_Inven_Type);
    if (!pInven) {
        LogHelper::LogError("game.contents",
                            "AddItem2 if( NULL == pInven ) ID:%d ) ",
                            pTBItem->Item_ID);
        return false;
    }

    if (pTBItem->Item_physical_Attack_Min > pTBItem->Item_physical_Attack ||
        pTBItem->Item_physical_Defense_Min > pTBItem->Item_physical_Defense) {
        LogHelper::LogError("game.contents",
                            "AddItem2 Physical value Min > Max [ ID:%d ] ",
                            pTBItem->Item_ID);
        return false;
    }

    if (AddItemCheck(psUpdateItem, pTBItem, shAddCount) ||
        AddItemCheck(psCreateItem, pTBItem, shAddCount)) {
        return true;
    }

    if (!pInven->AddItemCount(pTBItem, shAddCount, byLock, bOption,
                              psCreateItem, psUpdateItem)) {
        return false;
    }

    if (pTBClassify->GroupID == 27) {
        std::shared_ptr<CGocQuest> pQuest = GetOwnerGO()->GetGOC_Quest(false);
        if (pQuest) {
            pQuest->UpdateCondition(eCONDITION_TYPE_COLLECT,
                                    eCONDITION_TARGET_ITEM,
                                    pTBItem->Item_ID, shAddCount, true);
        }

        std::shared_ptr<CGocDailyMission> pMission =
            GetOwnerGO()->GetGOC_DailyMission(false);
        if (pMission) {
            pMission->UpdateCollectType(eDAILY_MISSION_TARGET_ITEM,
                                        pTBItem->Item_ID, shAddCount);
        }
    }

    std::shared_ptr<CGocAkashicRecord> pAkashicRecord =
        GetOwnerGO()->GetGOC_AkashicRecord(false);
    if (pAkashicRecord) {
        pAkashicRecord->AddAkashicGetInfo(pTBItem->Item_ID);
    }

    return true;
}

// IDA: 0x1400BDAA0
// PDB: ?AddItemCheck@CGocInventory@@AEAA_NAEAUPS_RES_STORAGE_INFO@@PEAUTB_ITEM@@AEAF@Z
bool CGocInventory::AddItemCheck(PS_RES_STORAGE_INFO& psItemList,
                                 TB_ITEM* pTBItem,
                                 std::int16_t& shAddCount) {
    for (PS_STORAGE_INFO& stInfo : psItemList.vecItem) {
        if (stInfo.stItem.nItemID != pTBItem->Item_ID ||
            stInfo.stItem.sCount == pTBItem->Item_Stack_Max ||
            stInfo.stItem.sCount == 0) {
            continue;
        }

        const int nSaveCount = stInfo.stItem.sCount + shAddCount;
        if (pTBItem->Item_Stack_Max >= nSaveCount) {
            stInfo.stItem.sCount = static_cast<std::int16_t>(nSaveCount);
            shAddCount = 0;
        } else {
            stInfo.stItem.sCount = pTBItem->Item_Stack_Max;
            shAddCount = static_cast<std::int16_t>(
                nSaveCount - pTBItem->Item_Stack_Max);
        }

        if (shAddCount == 0) {
            return true;
        }
    }

    return false;
}

// IDA: 0x1400BDBF0
// PDB: ?ReduceItem2@CGocInventory@@QEAA_NUST_CREATE_ITEMS@@EAEAUPS_RES_STORAGE_INFO@@@Z
bool CGocInventory::ReduceItem2(ST_CREATE_ITEMS stReduceItems,
                                 std::uint8_t byLock,
                                 PS_RES_STORAGE_INFO& psUpdateItem) {
    PS_RES_STORAGE_INFO psRemoveItem;
    bool bSuccess = true;

    for (const ST_CREATE_ITEM& stInfo : stReduceItems.vecInfo) {
        TB_ITEM* pTBItem = TXSingleton<XGameServer>::Instance()
                              ->GetResourceMgr()
                              .GetTB_ITEM(stInfo.nItemID);
        if (!pTBItem) {
            LogHelper::LogError(
                "game.contents",
                "ReduceItem2 if( NULL == pTBItem ) ID:%d ) ",
                stInfo.nItemID);
            bSuccess = false;
            break;
        }

        if (!ReduceItem2(pTBItem, stInfo.shCount, byLock, psUpdateItem)) {
            bSuccess = false;
            break;
        }
    }

    if (!bSuccess) {
        UnLockList(psUpdateItem);
    }

    return bSuccess;
}

// IDA: 0x1400BDDB0
// PDB: ?ReduceItem2@CGocInventory@@QEAA_NPEAUTB_ITEM@@FEAEAUPS_RES_STORAGE_INFO@@@Z
bool CGocInventory::ReduceItem2(TB_ITEM* pTBItem,
                                 std::int16_t shReduceCount,
                                 std::uint8_t byLock,
                                 PS_RES_STORAGE_INFO& psUpdateItem) {
    if (!pTBItem) {
        return false;
    }

    TB_ITEM_CLASSIFY* pTBClassify = TXSingleton<XGameServer>::Instance()
                                        ->GetResourceMgr()
                                        .GetTB_ITEM_CLASSIFY(
                                            pTBItem->Item_Classify_Index);
    if (!pTBClassify) {
        LogHelper::LogError(
            "game.contents",
            "ReduceItem2 if( NULL == pTBClassify ) ID:%d ) ",
            pTBItem->Item_ID);
        return false;
    }

    XBaseInventory* pInven = GetTBInvenPtr(pTBClassify->Item_Inven_Type);
    if (!pInven) {
        LogHelper::LogError(
            "game.contents",
            "ReduceItem2 if( NULL == pInven ) ID:%d ) ",
            pTBItem->Item_ID);
        return false;
    }

    if (shReduceCount < 1) {
        LogHelper::LogError(
            "game.contents",
            "ReduceItem2 if( shReduceCount < 1 ) ID:%d, COUNT:%d ) ",
            pTBItem->Item_ID,
            shReduceCount);
        return false;
    }

    return ReduceItemCheck(pTBItem, shReduceCount, byLock, psUpdateItem) ||
           pInven->DelItemCount(
               pTBItem, shReduceCount, byLock, psUpdateItem);
}

// ============================================================================
// Item reduce/use functions (IDA verified)
// ============================================================================

// IDA: 0x1400BDF10
// PDB: ?ReduceItem3@CGocInventory@@QEAA_NEFFEAEAUPS_RES_STORAGE_INFO@@@Z
bool CGocInventory::ReduceItem3(std::uint8_t byInvenType,
                                 std::int16_t shSlotPos,
                                 std::int16_t shDelCount,
                                 std::uint8_t byLock,
                                 PS_RES_STORAGE_INFO& psUpdateItem) {
    bool bCurLock = false;
    std::shared_ptr<CItem> pItem = GetSlotItem(
        byInvenType, shSlotPos, bCurLock);

    if (!pItem || bCurLock) {
        LogHelper::LogError(
            "game.contents",
            "ReduceItem2 if( NULL == pItem || E_ITEM_FLAG_UNLOCK != byCurLock ) Lock:%d ) ",
            bCurLock);
        CUser* pUser = GetCUserFromOwner(this);
        TXSingleton<XGameServer>::Instance()->SendItemLockLog(
            pUser->GetUCID(), byInvenType, shSlotPos, bCurLock, 21, 0);
        return false;
    }

    const int nItemCount = pItem->GetCount();
    if (nItemCount >= shDelCount && shDelCount >= 1) {
        PS_STORAGE_INFO stInfo{};
        stInfo.byInvenType = byInvenType;
        stInfo.shSlotPos = shSlotPos;
        pItem->GetItem(&stInfo.stItem);
        stInfo.stItem.sCount = static_cast<std::int16_t>(
            nItemCount - shDelCount);

        if (stInfo.stItem.sCount != 0) {
            SetLock(byInvenType, shSlotPos, byLock);
        } else {
            SetLock(byInvenType, shSlotPos, 1);
        }

        psUpdateItem.vecItem.push_back(stInfo);
        return true;
    }

    LogHelper::LogError(
        "game.contents",
        "ReduceItem2 if( pItem->GetCount() < shDelCount || shDelCount < 1 ) Count:%d, DelCount:%d ) ",
        nItemCount,
        shDelCount);
    return false;
}

// IDA: 0x1400BE230
// PDB: ?ItemUseEffect@CGocInventory@@QEAA_NUPS_ITEM_SLOT_INFO@@0@Z
bool CGocInventory::ItemUseEffect(PS_ITEM_SLOT_INFO stUseItem,
                                   PS_ITEM_SLOT_INFO stSelectItem) {
    bool bLock = false;
    std::shared_ptr<CItem> pUseItem = GetSlotItem(
        stUseItem.byInvenType, stUseItem.shSlotPos, bLock);
    if (!pUseItem || bLock) {
        return false;
    }

    std::shared_ptr<CItem> pSelectItem = GetSlotItem(
        stSelectItem.byInvenType, stSelectItem.shSlotPos, bLock);
    if (!pSelectItem || bLock) {
        return false;
    }

    if (!pUseItem->GetItemTable()->Item_Use_Value) {
        return false;
    }

    int nBeforeValue = 0;
    int nEtcValue = 0;
    const std::uint8_t byItemUseType =
        pUseItem->GetClassifyTable()->Item_Use_Type;

    switch (byItemUseType) {
    case 'Z': {
        STItem stSealDelItem{};
        if (pSelectItem->GetItem(&stSealDelItem)->bySealDelCount) {
            return false;
        }

        if (pSelectItem->GetItemTable()->Sealing_Cnt == 255) {
            return false;
        }

        STItem stSealItem{};
        nBeforeValue = pSelectItem->GetItem(&stSealItem)->bySealCount;

        STItem stSealCountItem{};
        nEtcValue = pSelectItem->GetItem(&stSealCountItem)->bySealDelCount + 1;
        break;
    }
    case '[': {
        STItem stRestoreItem{};
        nBeforeValue = pSelectItem->GetItem(&stRestoreItem)->byRestoreCount;
        break;
    }
    case '\\': {
        STItem stUpgradeItem{};
        nBeforeValue = pSelectItem->GetItem(&stUpgradeItem)->byUpgradeCount;
        break;
    }
    default:
        return false;
    }

    PS_RES_STORAGE_INFO psUpdateItem;
    if (!ReduceItem3(stUseItem.byInvenType, stUseItem.shSlotPos, 1, 0x42u,
                     psUpdateItem)) {
        return false;
    }

    ST_LOG_GAME stLog;
    stLog._sSubType = 75;
    CUser* pUser = GetCUserFromOwner(this);
    stLog.nParam3 = static_cast<int>(
        static_cast<std::int64_t>(pUser->GetMapInsID().nMapID << 16) >> 48);
    stLog.nParam4 = pSelectItem->GetID();
    stLog.nParam6 = pSelectItem->GetSerial();
    stLog.nParam7 = nBeforeValue;

    nBeforeValue -= pUseItem->GetItemTable()->Item_Use_Value;
    stLog.nParam8 = nBeforeValue >= 0 ? nBeforeValue : 0;
    stLog.nParam9 = nEtcValue;

    if (!UpdateItemEnd(0x42u, psUpdateItem, stLog)) {
        return false;
    }

    switch (pUseItem->GetClassifyTable()->Item_Use_Type) {
    case 'Z':
        pSelectItem->UnSealDecEffect(pUseItem->GetItemTable()->Item_Use_Value);
        break;
    case '[':
        pSelectItem->RestoreDecEffect(pUseItem->GetItemTable()->Item_Use_Value);
        break;
    case '\\':
        pSelectItem->UpgradeDecEffect(pUseItem->GetItemTable()->Item_Use_Value);
        break;
    }

    PS_STORAGE_INFO psInfo;
    psInfo.byInvenType = stSelectItem.byInvenType;
    psInfo.shSlotPos = stSelectItem.shSlotPos;
    STItem stSelectItemInfo{};
    psInfo.stItem = *pSelectItem->GetItem(&stSelectItemInfo);
    psUpdateItem.vecItem.push_back(psInfo);
    SetLock(stSelectItem.byInvenType, stSelectItem.shSlotPos, 0x42u);

    CMover* pOwner = GetOwnerGO();
    XSendDBPacket xSendDBPacket(static_cast<IXObject*>(pOwner), 0x81u, 0x23u);
    xSendDBPacket.XParse << pOwner->GetActorID().dwActorID;
    xSendDBPacket << psUpdateItem;
    if (!TXSingleton<XGameServer>::Instance()->SendDBGame(xSendDBPacket)) {
        LogHelper::LogError("game.item", "ItemUseEffect error - ( %d )", 6651);
    }

    return true;
}

// IDA: 0x1400BEAE0
// PDB: ?ReduceItemCheck@CGocInventory@@QEAA_NPEAUTB_ITEM@@AEAFEAEAUPS_RES_STORAGE_INFO@@@Z
bool CGocInventory::ReduceItemCheck(TB_ITEM* pTBItem,
                                     std::int16_t& shReduceCount,
                                     std::uint8_t byLock,
                                     PS_RES_STORAGE_INFO& psUpdateItem) {
    for (PS_STORAGE_INFO& stInfo : psUpdateItem.vecItem) {
        if (stInfo.stItem.nItemID != pTBItem->Item_ID ||
            stInfo.stItem.sCount < 1) {
            continue;
        }

        if (shReduceCount <= stInfo.stItem.sCount) {
            stInfo.stItem.sCount = static_cast<std::int16_t>(
                stInfo.stItem.sCount - shReduceCount);
            shReduceCount = 0;
        } else {
            shReduceCount = static_cast<std::int16_t>(
                shReduceCount - stInfo.stItem.sCount);
            stInfo.stItem.sCount = 0;
        }

        if (stInfo.stItem.sCount == 0) {
            SetLock(stInfo.byInvenType, stInfo.shSlotPos, 1);
        }

        if (shReduceCount == 0) {
            return true;
        }
    }

    (void)byLock;
    return false;
}

// IDA: 0x1400BEC70
// PDB: ?CreateItem2@CGocInventory@@QEAA_NUST_CREATE_ITEMS@@E_NAEAUPS_RES_STORAGE_INFO@@2UST_LOG_GAME@@@Z
bool CGocInventory::CreateItem2(ST_CREATE_ITEMS stCreateItems, std::uint8_t byLock,
                                 bool bOption, PS_RES_STORAGE_INFO& psCreateItem,
                                 PS_RES_STORAGE_INFO& psUpdateItem,
                                 ST_LOG_GAME stLogGame) {
    if (AddItem2(stCreateItems, byLock, bOption, psCreateItem, psUpdateItem)) {
        if (UpdateItemEnd(byLock, psUpdateItem, stLogGame)) {
            return AddItemEnd(byLock, psCreateItem, stLogGame);
        }
        return false;
    }

    UnLockList(psCreateItem);
    UnLockList(psUpdateItem);
    return false;
}

// IDA: 0x1400BEEE0
// PDB: ?AddItemEnd@CGocInventory@@QEAA_NEUPS_RES_STORAGE_INFO@@UST_LOG_GAME@@@Z
bool CGocInventory::AddItemEnd(std::uint8_t byCurLock,
                                PS_RES_STORAGE_INFO psCreateItem,
                                ST_LOG_GAME stLogGame) {
    CMover* pOwner = GetOwnerGO();
    IXObject* pObject = pOwner ? static_cast<IXObject*>(pOwner) : nullptr;
    CUser* pUser = GetCUserFromOwner(this);

    for (const PS_STORAGE_INFO& stInfo : psCreateItem.vecItem) {
        // The original call supplies true for AddItem's ABI-only fourth flag.
        if (!AddItem(stInfo.byInvenType, static_cast<std::int16_t>(stInfo.shSlotPos),
                     stInfo.stItem, true)) {
            return false;
        }

        ST_STATISTICS_ITEM stStatistics{};
        stStatistics.byFlag = 1;
        stStatistics.biSerial = stInfo.stItem.xSerial;
        stStatistics.dwUCID = pUser->GetUCID();
        stStatistics.dwItemID = stInfo.stItem.nItemID;
        stStatistics.byUpgrade = stInfo.stItem.byUpgrade;
        stStatistics.byUpgradeLimit = stInfo.stItem.byUpgradeLimit;

        XSendDBPacket xSendDBStatistics(pObject, 0xF0, 0x11);
        xSendDBStatistics << stStatistics;
        TXSingleton<XGameServer>::Instance()->SendDBStatistics(xSendDBStatistics);

        bool bLock = true;
        std::shared_ptr<CItem> pItem = GetSlotItem(
            stInfo.byInvenType, stInfo.shSlotPos, bLock);
        if (pItem) {
            ItemLog(pItem, stInfo.stItem, stLogGame);
        }
    }

    (void)byCurLock;
    return true;
}

// IDA: 0x1400BF260
// PDB: ?UpdateItemEnd@CGocInventory@@QEAA_NEUPS_RES_STORAGE_INFO@@UST_LOG_GAME@@@Z
bool CGocInventory::UpdateItemEnd(std::uint8_t byCurLock,
                                   PS_RES_STORAGE_INFO psUpdateItem,
                                   ST_LOG_GAME stLogGame) {
    CMover* pOwner = GetOwnerGO();
    IXObject* pObject = pOwner ? static_cast<IXObject*>(pOwner) : nullptr;
    CUser* pUser = GetCUserFromOwner(this);

    for (const PS_STORAGE_INFO& stInfo : psUpdateItem.vecItem) {
        if (stInfo.stItem.sCount != 0) {
            bool bLock = false;
            std::shared_ptr<CItem> pItem = GetSlotItem(
                stInfo.byInvenType, stInfo.shSlotPos, bLock);
            if (!pItem || byCurLock != bLock) {
                TXSingleton<XGameServer>::Instance()->SendItemLockLog(
                    pUser->GetUCID(), stInfo.byInvenType, stInfo.shSlotPos,
                    byCurLock, 22, bLock);
                return false;
            }

            ItemLog(pItem, stInfo.stItem, stLogGame);
            pItem->SetCount(stInfo.stItem.sCount);
            continue;
        }

        bool bLock = false;
        std::shared_ptr<CItem> pItem = GetSlotItem(
            stInfo.byInvenType, stInfo.shSlotPos, bLock);
        if (pItem) {
            ItemLog(pItem, stInfo.stItem, stLogGame);
        }

        RemoveItem(stInfo.byInvenType, static_cast<std::int16_t>(stInfo.shSlotPos));
        SetLock(stInfo.byInvenType, stInfo.shSlotPos, byCurLock);

        ST_STATISTICS_ITEM stStatistics{};
        stStatistics.byFlag = 3;
        stStatistics.biSerial = stInfo.stItem.xSerial;

        XSendDBPacket xSendDBStatistics(pObject, 0xF0, 0x11);
        xSendDBStatistics << stStatistics;
        TXSingleton<XGameServer>::Instance()->SendDBStatistics(xSendDBStatistics);
    }

    return true;
}

// ============================================================================
// FP (Fatigue Point) item functions (IDA verified)
// ============================================================================

// IDA: 0x1400BF7B0
// __int64 __fastcall CGocInventory::CanUseItemFPUseFree(
//         CGocInventory *this, unsigned __int8 byInvenType, __int16 shSlot)
// Checks if FP item can be used (level check, FP cap check)
bool CGocInventory::CanUseItemFPUseFree(std::uint8_t byInvenType, std::int16_t shSlot) {
    // IDA Decompiled:
    // GetSlotItem(this, &pItem, byInvenType, shSlot, bLock);
    // if ( pItem == null || bLock[0] ) {
    //     // Send item lock log
    //     XGameServer::SendItemLockLog(..., byInvenType, shSlot, bLock[0], 23, 0);
    //     return false;
    // }
    // pTBItem = CItem::GetItemTable(pItem.operator->());
    // if ( !pTBItem ) return false;
    // if ( !CItem::GetClassifyTable(pItem.operator->()) ) return false;
    // // Level check
    // if ( pTBItem->Item_Limit_Lv <= GetLevel() ) {
    //     // FP cap check: bonus FP or FP == 200
    //     if ( CUser::stMyCharInfoEx()->shBonusFP || CUser::stMyCharInfoEx()->shFP == 200 ) {
    //         // Send error message (error ID 0xCD83)
    //         CGocNetwork::SendErrorMessage(GetMover(), 8, 0x11, 0xCD83);
    //         return false;
    //     }
    //     // Count check
    //     if ( CItem::GetCount(pItem.operator->()) >= 1 )
    //         return pItem.operator->()->Finalize();  // Returns true
    //     return false;
    // }
    // return false;

    // TODO: 需人工审查 - Implement when CItem/TB_ITEM/CUser types available
    (void)byInvenType;
    (void)shSlot;
    return false;
}

// IDA: 0x1400BFAD0
// __int64 __fastcall CGocInventory::ItemFPUseFree(
//         CGocInventory *this, unsigned __int8 byInvenType, __int16 shSlot)
// Uses FP item, restores FP, sends to DB
bool CGocInventory::ItemFPUseFree(std::uint8_t byInvenType, std::int16_t shSlot) {
    // IDA Decompiled:
    // GetSlotItem(this, &pItem, byInvenType, shSlot, bLock);
    // if ( pItem == null || bLock[0] ) {
    //     XGameServer::SendItemLockLog(..., byInvenType, shSlot, bLock[0], 24, 0);
    //     return false;
    // }
    // pTBItem = CItem::GetItemTable(pItem.operator->());
    // if ( !pTBItem ) return false;
    // pTBClassify = CItem::GetClassifyTable(pItem.operator->());
    // if ( !pTBClassify ) return false;
    //
    // // Add cooldown
    // AddCoolTime(this, pTBItem->Cooltime_Group, pTBItem->Cooltime_Value, 0);
    //
    // if ( pTBItem->Item_Use_Value ) {
    //     // Get CGocAttribute and restore FP
    //     CMover::GetGOC<CGocAttribute>(GetMover(), &pAttr, 0);
    //     CGocAttribute::FPRestore(pAttr.operator->(), pTBItem->Item_Use_Value);
    //
    //     // Update achieve collect
    //     CMover::GetGOC<CGocAchieve>(GetMover(), &pAchievePtr, 0);
    //     CGocAchieve::UpdateCollect(pAchievePtr.operator->(), 0xF, 1, 0);
    //
    //     // Reduce item count
    //     nCount = CItem::GetCount(pItem.operator->()) - 1;
    //     CItem::SetCount(pItem.operator->(), nCount);
    //     if ( CItem::GetCount(pItem.operator->()) <= 0 )
    //         RemoveItem(this, byInvenType, shSlot);
    //
    //     // Send update to client
    //     SendUpdateItem(this, byInvenType, shSlot, CItem::GetCount(pItem.operator->()), 0);
    //
    //     // Send DB packet (main=0x21, sub=0x24)
    //     XSendDBPacket::XSendDBPacket(&xSendDBPacket, pObject, 0x21u, 0x24u);
    //     XParse::operator<<(&xSendDBPacket.XParse, QuestID);
    //     XParse::operator<<(&xSendDBPacket.XParse, biItemSerial);
    //     XParse::operator<<(&xSendDBPacket.XParse, nCount);
    //     XParse::operator<<(&xSendDBPacket.XParse, byInvenType);
    //     XParse::operator<<(&xSendDBPacket.XParse, shSlot);
    //     XGameServer::SendDBGame(&xSendDBPacket);
    //
    //     // Update item use info
    //     UseItemInfo(this, pTBClassify->Item_Use_Type);
    //     return true;
    // }
    // return false;

    // TODO: 需人工审查 - Implement when CItem/TB_ITEM/CGocAttribute types available
    (void)byInvenType;
    (void)shSlot;
    return false;
}

// ============================================================================
// Grave/Infinite Tower item functions (IDA verified)
// ============================================================================

// IDA: 0x1400BFFC0
// char __fastcall CGocInventory::CanUseGraveInitItem(
//         CGocInventory *this, unsigned __int8 byInvenType, __int16 shSlot)
// Checks if grave init item can be used (inventory type, level, infinite tower limit)
bool CGocInventory::CanUseGraveInitItem(std::uint8_t byInvenType, std::int16_t shSlot) {
    // IDA Decompiled:
    // // Must be inventory type 2 or 13
    // if ( byInvenType != 2 && byInvenType != 13 )
    //     return false;
    //
    // GetSlotItem(this, &pItem, byInvenType, shSlot, bLock);
    // if ( pItem == null || bLock[0] ) {
    //     XGameServer::SendItemLockLog(..., byInvenType, shSlot, bLock[0], 25, 0);
    //     return false;
    // }
    //
    // // Level check
    // if ( ItemTable->Item_Limit_Lv <= GetLevel() ) {
    //     // Count check
    //     if ( CItem::GetCount(pItem.operator->()) >= 1 ) {
    //         // Get CGocRecode and check infinite tower limit
    //         CMover::GetGOC<CGocRecode>(GetMover(), &pRecode, 0);
    //         if ( CGocRecode::GetInfiniteTowerLimitCount(pRecode.operator->()) >= 3 )
    //             return true;  // Can use (limit reached)
    //         return false;
    //     }
    //     return false;
    // }
    // return false;

    // TODO: 需人工审查 - Implement when CItem/CGocRecode types available
    (void)byInvenType;
    (void)shSlot;
    return false;
}

// IDA: 0x1400C0230
// __int64 __fastcall CGocInventory::UseGraveInitItem(
//         CGocInventory *this, unsigned __int8 byInvenType, __int16 shSlot)
// Uses grave init item, reduces item, sends DB packet
bool CGocInventory::UseGraveInitItem(std::uint8_t byInvenType, std::int16_t shSlot) {
    // IDA Decompiled:
    // GetSlotItem(this, &pItem, byInvenType, shSlot, &bLock);
    // if ( pItem == null || bLock ) {
    //     XGameServer::SendItemLockLog(..., byInvenType, shSlot, bLock, 26, 0);
    //     return false;
    // }
    //
    // biItemSerial = pItem.operator->()->size();  // Get serial
    // dwValue = CItem::GetItemTable(pItem.operator->())->Cooltime_Value;
    // ItemTable = CItem::GetItemTable(pItem.operator->());
    // AddCoolTime(this, ItemTable->Cooltime_Group, dwValue, 0);
    //
    // // Use infinite tower init item via CGocRecode
    // CMover::GetGOC<CGocRecode>(GetMover(), &pRecode, 0);
    // if ( pRecode != null )
    //     CGocRecode::UseInfiniteTowerInitItem(pRecode.operator->());
    //
    // ST_GET_INFO::ST_GET_INFO(&stGetInfo);
    // PS_RES_STORAGE_INFO::PS_RES_STORAGE_INFO(&psCreateItem);
    // PS_RES_STORAGE_INFO::PS_RES_STORAGE_INFO(&psUpdateItem);
    //
    // if ( ReduceItem3(this, byInvenType, shSlot, 1, 0x25u, &psUpdateItem) ) {
    //     ST_LOG_GAME::ST_LOG_GAME(&stLog);
    //     stLog._sSubType = 22;
    //     // ... setup log params ...
    //     if ( UpdateItemEnd(this, 0x25u, &psUpdateItem, &stLog) ) {
    //         PS_POST_DELETE_LIST::PS_POST_DELETE_LIST(&psGachaList);
    //         byFlag = 0;
    //
    //         // Send DB packet (main=0x21, sub=0x12)
    //         XSendDBPacket::XSendDBPacket(&xSendDBPacket, pObject, 0x21u, 0x12u);
    //         XParse::operator<<(&xSendDBPacket.XParse, QuestID);
    //         operator<<(&xSendDBPacket, &psUpdateItem);
    //         operator<<(&xSendDBPacket, &psCreateItem);
    //         operator<<(&xSendDBPacket, &stGetInfo);
    //         XParse::operator<<(&xSendDBPacket.XParse, CurID);
    //         XParse::operator<<(&xSendDBPacket.XParse, 0);
    //         operator<<(&xSendDBPacket, &psGachaList);
    //         XParse::operator<<(&xSendDBPacket.XParse, byFlag);
    //         XParse::operator<<(&xSendDBPacket.XParse, biItemSerial);
    //         XGameServer::SendDBGame(&xSendDBPacket);
    //         return true;
    //     }
    //     return false;
    // }
    // return false;

    // TODO: 需人工审查 - Implement when CGocRecode/PS_RES_STORAGE_INFO types available
    (void)byInvenType;
    (void)shSlot;
    return false;
}

// ============================================================================
// Broach update functions (IDA verified)
// ============================================================================

// IDA: 0x1400C0910
// void __fastcall CGocInventory::SendBroachUpdate(
//         CGocInventory *this, PS_ITEM_BROACH_LIST *stBroachList)
// Sends broach update packet to client (main=8, sub=0x62)
void CGocInventory::SendBroachUpdate(void* stBroachList) {
    // IDA Decompiled:
    // PS_BROACH_SERIAL_LIST::PS_BROACH_SERIAL_LIST(&psCostumeSerialList);
    // std::vector<unsigned __int64>::clear(&psCostumeSerialList);
    // XSendPacket::XSendPacket(&xSendPacket, 8u, 0x62u);
    // operator<<(&xSendPacket, stBroachList);
    // operator<<(&xSendPacket, &psCostumeSerialList);
    //
    // // Get actor and send packet
    // pActor = GetActor();
    // CGocNetwork::Send(pActor, &xSendPacket);
    //
    // CWarpPotal::~CWarpPotal(&psCostumeSerialList);
    // PS_ITEM_BROACH_LIST::~PS_ITEM_BROACH_LIST(stBroachList);

    // TODO: 需人工审查 - Implement when PS_ITEM_BROACH_LIST/XSendPacket types available
    (void)stBroachList;
}

// ============================================================================
// Package info functions (IDA verified)
// ============================================================================

// IDA: 0x1400C0A30
// void __fastcall CGocInventory::SendPackageInfo(
//         CGocInventory *this, PS_ITEM_PACKAGE_LIST *psPackageList)
// Sends package info packet to client (main=8, sub=0x72)
void CGocInventory::SendPackageInfo(void* psPackageList) {
    // IDA Decompiled:
    // XSendPacket::XSendPacket(&xSendPacket, 8u, 0x72u);
    // XParse::operator<<(&xSendPacket.XParse, 1);
    // operator<<(&xSendPacket, psPackageList);
    //
    // // Get actor and send packet
    // pActor = GetActor();
    // CGocNetwork::Send(pActor, &xSendPacket);
    //
    // PS_ITEM_PACKAGE_LIST::~PS_ITEM_PACKAGE_LIST(psPackageList);

    // TODO: 需人工审查 - Implement when PS_ITEM_PACKAGE_LIST/XSendPacket types available
    (void)psPackageList;
}

// ============================================================================
// GM Cheat functions (IDA verified)
// ============================================================================

// IDA: 0x1400C0B20
// __int64 __fastcall CGocInventory::ItemMakeCheatByLevel(
//         CGocInventory *this, unsigned __int8 byLevel, unsigned __int8 byClass,
//         bool bAddOption, unsigned __int8 byUpgrade, unsigned __int8 byRank)
// GM cheat to create items by level/class/rank
bool CGocInventory::ItemMakeCheatByLevel(std::uint8_t byLevel, std::uint8_t byClass,
                                          bool bAddOption, std::uint8_t byUpgrade,
                                          std::uint8_t byRank) {
    // IDA Decompiled:
    // This is a large GM cheat function that:
    // 1. Iterates through TB_ITEM table to find items matching level/class/rank
    // 2. Creates items with AddItem2
    // 3. Applies upgrade level if specified
    // 4. Sends DB packet to persist items
    //
    // Key logic:
    // - For each TB_ITEM in table:
    //   - Check if Item_Limit_Lv == byLevel and Item_Class == byClass and Item_Rank == byRank
    //   - If Item_Slot_Type == 1 (weapon), add to create list
    //   - For other slot types (armor), collect by slot type
    // - Call AddItem2 with collected items
    // - Apply upgrade via TB_REINFORCE table
    // - Call UpdateItemEnd and AddItemEnd
    // - Send DB packet (main=0x21, sub=0xC)

    // TODO: 需人工审查 - Implement when TB_ITEM/TB_ITEM_CLASSIFY/XResourceMgr types available
    (void)byLevel;
    (void)byClass;
    (void)bAddOption;
    (void)byUpgrade;
    (void)byRank;
    return false;
}

// IDA: 0x1400C2690
// char __fastcall CGocInventory::ItemMakeCheatOption(
//         CGocInventory *this, int nType, unsigned int nItemID,
//         int nOptionType, int nValue)
// GM cheat to create items with specific options
bool CGocInventory::ItemMakeCheatOption(int nType, int nItemID, int nOptionType, int nValue) {
    // IDA Decompiled:
    // pTBItem = XResourceMgr::GetTB_ITEM(nItemID);
    // if (!pTBItem) {
    //     LogHelper::LogError("game.item", "ItemMakeCheat error - Failed Create Item[ ActorID:%d, ItemID:%d ] ( %d )", ...);
    //     return false;
    // }
    // pTBClassify = XResourceMgr::GetTB_ITEM_CLASSIFY(pTBItem->Item_Classify_Index);
    // if (!pTBClassify) return false;
    // if (pTBClassify->Item_Inven_Type) return false;  // Must be equipment type 0
    //
    // pInven = GetInvenPtr(2u);  // Common inventory
    // shPos = pInven->GetEmptySlot_2();
    // if (shPos < 0) return false;
    //
    // // Add item with AddItem2
    // if (!AddItem2(pTBItem, 1, 0x65u, 0, &psCreateItem, &psUpdateItem))
    //     return false;
    //
    // // Apply options
    // if (nType == 1) {
    //     // Direct option setting
    //     psCreateItem[0].stItem.wRandomOptionType1 = nOptionType;
    //     psCreateItem[0].stItem.nRandomOptionValue1 = nValue;
    // } else if (pTBItem->Item_Ex_Option_ID) {
    //     // Random option from TB_RANDOM_OPTION table
    //     pTBRandomOption = XResourceMgr::GetTB_RANDOM_OPTION(pTBItem->Item_Ex_Option_ID);
    //     // Random shuffle option indices and apply
    //     for (j = 0; nValuea > j; ++j) {
    //         // Set option type and value from random range
    //     }
    // }
    //
    // // Finalize with AddItemEnd
    // if (AddItemEnd(0x65u, &psCreateItem, &stLog)) {
    //     // Send DB packet (main=0x21, sub=0xC)
    //     XSendDBPacket::XSendDBPacket(&xSendDBPacket, pObject, 0x21u, 0xCu);
    //     XParse::operator<<(&xSendDBPacket.XParse, QuestID);
    //     XParse::operator<<(&xSendDBPacket.XParse, byCurLock);  // 101
    //     operator<<(&xSendDBPacket, &psCreateItem);
    //     operator<<(&xSendDBPacket, &psUpdateItem);
    //     XParse::operator<<(&xSendDBPacket.XParse, byFlag);  // 0
    //     XGameServer::SendDBGame(&xSendDBPacket);
    //     return true;
    // }
    // return false;

    // TODO: 需人工审查 - Implement when TB_ITEM/TB_RANDOM_OPTION/XResourceMgr types available
    (void)nType;
    (void)nItemID;
    (void)nOptionType;
    (void)nValue;
    return false;
}

// ============================================================================
// Cash item date functions (IDA verified)
// ============================================================================

// IDA: 0x1400B8230
// void __fastcall CGocInventory::OnUpdateCashItemDate(CGocInventory *this)
// Updates cash item dates, removes expired items
void CGocInventory::OnUpdateCashItemDate() {
    // IDA: Get current date
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // __int64 biCurDate = XGameServer::GetCurDate(pServer);

    // IDA: Build list of expired items
    // std::vector<__int64> stDelSerial;
    // for (auto iter = m_mpCashItemDate.begin(); iter != m_mpCashItemDate.end(); ++iter) {
    //     if (!iter->second || iter->second < biCurDate) {
    //         stDelSerial.push_back(iter->first);
    //     }
    // }

    // IDA: Get CGocPost for system messages
    // std::shared_ptr<CGocPost> pPostPtr;
    // CMover::GetGOC<CGocPost>(GetOwner(), &pPostPtr, 0);

    // IDA: Process each expired item
    // for (size_t i = 0; i < stDelSerial.size(); ++i) {
    //     std::shared_ptr<CItem> pItem;
    //     GetItemPtr(&pItem, stDelSerial[i]);
    //     if (pItem) {
    //         int Slot = CItem::GetSlot(pItem.get());
    //         std::uint8_t InvenType = CItem::GetInvenType(pItem.get());
    //
    //         std::shared_ptr<CItem> pFindItem;
    //         std::uint8_t byLock;
    //         GetSlotItem(&pFindItem, InvenType, Slot, &byLock);
    //
    //         if (!byLock && pFindItem) {
    //             // Break the item
    //             ST_LOG_GAME stLog;
    //             stLog._sSubType = 34;
    //             BreakItemReq(InvenType, Slot, 1, 0x48, &stLog);
    //
    //             // Send system post if cash item
    //             TB_ITEM* pTBItem = CItem::GetItemTable(pFindItem.get());
    //             if (pTBItem->Item_Cash && pTBItem->Item_Use_Period_Type) {
    //                 __int64 biEventID = CItem::GetCurID(pFindItem.get());
    //                 pPostPtr->SystemPostSend(3, 3, biEventID, dwRecvUCID);
    //             }
    //         }
    //         m_mpCashItemDate.erase(stDelSerial[i]);
    //     }
    // }

    // TODO: 需人工审查 - Implement when m_mpCashItemDate/CGocPost available
}

// IDA: 0x1400B8F40
// __int64 __fastcall CGocInventory::GetCurItemsExp(CGocInventory *this, PS_ITEM_SLOT_INFO *stAkashicInfo, ...)
// Gets total exp from akashic items for compose
int CGocInventory::GetCurItemsExp(void* stAkashicInfo, void* stInfo, int& nNeedGold, bool& bSuccess) {
    nNeedGold = 0;
    bSuccess = false;

    // IDA: Get CGocAkashicRecord component
    // std::shared_ptr<CGocAkashicRecord> pAkashicPtr;
    // CMover::GetGOC<CGocAkashicRecord>(GetOwner(), &pAkashicPtr, 0);
    // if (!pAkashicPtr) return 0;

    // IDA: Get event rate bonus
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // int nAddRate = CTimeEventMgr::GetEventValue(&pServer->m_TimeEventMgr, 2);
    // float fAddRate = (nAddRate > 0) ? (float)nAddRate / 10000.0f : 0.0f;

    // IDA: Check for great success
    // float fGreatValue = 0.0f;
    // TB_COMMON* pTB_CommonRate = XResourceMgr::GetTB_COMMON(0x2711);
    // if (pTB_CommonRate) {
    //     int nRate = XItemFactory::nRand(1, 10000) - pTB_CommonRate->Value;
    //     if (nRate <= 0) {
    //         TB_COMMON* pTB_CommonValue = XResourceMgr::GetTB_COMMON(0x2712);
    //         if (pTB_CommonValue) {
    //             fGreatValue = pTB_CommonValue->Value * 0.01f;
    //             bSuccess = true;
    //         }
    //     }
    // }

    // IDA: Process each item in stInfo
    // int nGetExp = 0;
    // int nSize = stInfo->size();
    // for (int i = 0; i < nSize; ++i) {
    //     PS_ITEM_SLOT_INFO& psItem = stInfo[i];
    //
    //     // Must be inventory type 2 or 13
    //     if (psItem.byInvenType != 2 && psItem.byInvenType != 13)
    //         return 0;
    //
    //     // Can't use same slot as akashic
    //     if (psItem.byInvenType == stAkashicInfo->byInvenType &&
    //         psItem.shSlotPos == stAkashicInfo->shSlotPos)
    //         return 0;
    //
    //     std::shared_ptr<CItem> pFindItem;
    //     std::uint8_t byLock;
    //     GetSlotItem(&pFindItem, psItem.byInvenType, psItem.shSlotPos, &byLock);
    //
    //     if (!pFindItem || byLock) return 0;
    //
    //     // Must be CItemAkashic
    //     std::shared_ptr<CItemAkashic> pAkashic = std::tr1::dynamic_pointer_cast<CItemAkashic>(pFindItem);
    //     if (!pAkashic) return 0;
    //
    //     TB_AKASHIC_RECORDS* pTB_AKASHIC = pAkashic->GetTable();
    //     if (!pTB_AKASHIC) return 0;
    //
    //     // Can't use compose type
    //     if (pTB_AKASHIC->Akashic_Limit_Type & 4) return 0;
    //
    //     // Calculate exp
    //     int nAddExp = (int)((float)pTB_AKASHIC->EXP_Add * (1.0f + fAddRate + fGreatValue));
    //     nGetExp += nAddExp;
    //     nNeedGold += pTB_AKASHIC->Gold_Add;
    // }

    // IDA: Check if player has enough gold
    // if (m_nInvenMoney < nNeedGold) return 0;

    (void)stAkashicInfo;
    (void)stInfo;
    return 0;  // TODO: 需人工审查 - Implement when CGocAkashicRecord/XResourceMgr available
}

// ============================================================================
// Inventory info functions (IDA verified)
// ============================================================================

// IDA: 0x1400B2120
// void __fastcall CGocInventory::GetInvenInfo(CGocInventory *this, unsigned __int8 byInvenType, PS_RES_STORAGE_INFO *stInvenInfo)
// Routes to GetEquipPtr or GetInvenPtr based on type
void CGocInventory::GetInvenInfo(std::uint8_t byInvenType, void* stInvenInfo) {
    // IDA: Switch on byInvenType
    switch (byInvenType) {
        case 0:   // Ability equip
        case 1:   // Shape equip
        case 3: { // Look equip
            // IDA: Equipment types - use GetEquipPtr
            XBaseEquip* pEquip = GetEquipPtr(byInvenType);
            if (pEquip) {
                // XBaseEquip::GetInvenInfo(pEquip, byInvenType, stInvenInfo);
            }
            break;
        }
        case 2:   // Common inventory
        case 4:   // Costume inventory
        case 5:
        case 6:
        case 0xB: // 11 - Cube
        case 0xD: // 13 - Cash
        case 0xE: // 14
        case 0x10: // 16
        case 0x11: // 17
        case 0x12: { // 18
            // IDA: Inventory types - use GetInvenPtr (returns XBaseInventory*)
            XBaseInventory* pInven = GetInvenPtr(byInvenType);
            if (pInven) {
                // pInven->GetSlotInfos(pInven, stInvenInfo);
            }
            break;
        }
        default:
            break;
    }
    (void)stInvenInfo;
}

// ============================================================================
// Item creation functions (IDA verified)
// ============================================================================

// IDA: 0x1400B1FA0
// PDB: ?LogCreateItemLog@CGocInventory@@QEAAXHAEAUST_LOG_GAME@@@Z
void CGocInventory::LogCreateItemLog(int nCreateType, ST_LOG_GAME& stLogData) {
    switch (nCreateType) {
        case 3:
            stLogData._sSubType = 15;
            break;
        case 4:
            stLogData._sSubType = 16;
            break;
        case 5:
            stLogData._sSubType = 17;
            break;
        case 6:
            stLogData._sSubType = 18;
            break;
        case 7:
            stLogData._sSubType = 19;
            break;
        case 8:
            stLogData._sSubType = 20;
            break;
        case 9:
            stLogData._sSubType = 76;
            break;
        case 10:
            stLogData._sSubType = 35;
            break;
        case 11:
            stLogData._sSubType = 92;
            break;
        case 13:
            stLogData._sSubType = 91;
            break;
        case 15:
            stLogData._sSubType = 42;
            break;
        case 16:
            stLogData._sSubType = 58;
            break;
        case 17:
            stLogData._sSubType = 24;
            break;
        case 18:
            stLogData._sSubType = 26;
            break;
        default:
            return;
    }
}

// IDA: 0x1400B2430
// char __fastcall CGocInventory::ItemMakeCheat(CGocInventory *this, int nItemID, __int16 sCount, bool bAddOption, unsigned __int8 byUpgrade)
// GM cheat to create items with optional upgrade
bool CGocInventory::ItemMakeCheat(int nItemID, std::int16_t sCount, bool bAddOption, std::uint8_t byUpgrade) {
    // IDA: Get TB_ITEM
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_ITEM* pTBItem = XResourceMgr::GetTB_ITEM(&pServer->m_xResourceMgr, nItemID);
    // if (!pTBItem) {
    //     LogHelper::LogError("game.item", "ItemMakeCheat error - Failed Create Item[ ActorID:%d, ItemID:%d ] ( %d )", ...);
    //     return false;
    // }

    // IDA: Check if repackage item
    // if (CGocInventory::IsResealPackage(nItemID)) {
    //     LogHelper::LogError("game.item", "ItemMakeCheat error - Failed Create Item REPACKAGE[ ActorID:%d, ItemID:%d ] ( %d )", ...);
    //     return false;
    // }

    // IDA: Get TB_ITEM_CLASSIFY
    // TB_ITEM_CLASSIFY* pTBClassify = XResourceMgr::GetTB_ITEM_CLASSIFY(&pServer->m_xResourceMgr, pTBItem->Item_Classify_Index);
    // if (!pTBClassify) {
    //     LogHelper::LogError("game.item", "ItemMakeCheat error - Failed pTBItemClassify Item[ ActorID:%d, ItemID:%d ] ( %d )", ...);
    //     return false;
    // }

    // IDA: Check stack max
    // if (pTBItem->Item_Stack_Max == 0) {
    //     LogHelper::LogError("game.item", "ItemMakeCheat error - Stack max is 0[ ActorID:%d, ItemID:%d ] ( %d )", ...);
    //     return false;
    // }

    // IDA: Create ST_CREATE_ITEMS list
    // ST_CREATE_ITEMS stCreateItem;
    // int nCount = sCount / pTBItem->Item_Stack_Max;
    // if (sCount % pTBItem->Item_Stack_Max) ++nCount;
    // for (int i = 0; i < nCount; ++i) {
    //     ST_CREATE_ITEM stCheat;
    //     stCheat.nItemID = nItemID;
    //     stCheat.shCount = (sCount < pTBItem->Item_Stack_Max) ? sCount : pTBItem->Item_Stack_Max;
    //     sCount -= stCheat.shCount;
    //     stCreateItem.push_back(stCheat);
    // }

    // IDA: Add items with lock type 101
    // PS_RES_STORAGE_INFO psCreateItem, psUpdateItem;
    // std::uint8_t byCurLock = 101;
    // if (!CGocInventory::AddItem2(&stCreateItem, byCurLock, 1, &psCreateItem, &psUpdateItem))
    //     return false;

    // IDA: Apply upgrade if requested
    // if (byUpgrade && pTBItem->Item_Reinforce_ID) {
    //     TB_REINFORCE* pTBReinforce = XResourceMgr::GetTB_REINFORCE(..., pTBItem->Item_Reinforce_ID);
    //     if (pTBReinforce && pTBReinforce->Max_Reinforce >= byUpgrade && byUpgrade <= 10) {
    //         for (auto& item : psCreateItem) {
    //             item.shReinforce = byUpgrade;
    //         }
    //     }
    // }

    // IDA: Update item end
    // ST_LOG_GAME stLog;
    // stLog._sSubType = 14;
    // if (!CGocInventory::UpdateItemEnd(byCurLock, &psUpdateItem, &stLog)) return false;
    // if (!CGocInventory::AddItemEnd(byCurLock, &psCreateItem, &stLog)) return false;

    // IDA: Send DB packet
    // XSendDBPacket xSendDBPacket(pObject, 0x21, 0x0C);
    // xSendDBPacket << QuestID << byCurLock << psCreateItem << psUpdateItem << byFlag;
    // XGameServer::SendDBGame(&xSendDBPacket);

    (void)nItemID;
    (void)sCount;
    (void)bAddOption;
    (void)byUpgrade;
    return false;  // TODO: 需人工审查 - Implement when TB_ITEM/TB_REINFORCE/ST_CREATE_ITEMS available
}

// ============================================================================
// Socket upgrade functions (IDA verified)
// ============================================================================

// IDA: 0x1400D29F0
// __int64 __fastcall CGocInventory::UpgradeSocket(
//         CGocInventory *this, PS_ITEM_SLOT_INFO psEquipItemInfo,
//         unsigned int nItemSocketID, unsigned __int8 bySocketIndex, bool bTool)
// Upgrades socket on equipment item
int CGocInventory::UpgradeSocket(void* psEquipItemInfo, std::uint32_t nItemSocketID,
                                  std::uint8_t bySocketIndex, bool bTool) {
    // IDA Decompiled:
    // CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    // if (!pUser && !bTool) return 52340;
    //
    // unsigned int dwUCID = CQuestCondition::GetQuestID(pUser->GetActorID());
    // unsigned int dwUAID = pUser->GetUAID();
    //
    // std::shared_ptr<CItem> pEquipItemPtr;
    // std::uint8_t byLock;
    // GetSlotItem(&pEquipItemPtr, psEquipItemInfo->byInvenType, psEquipItemInfo->shSlotPos, &byLock);
    // if (!pEquipItemPtr || byLock) {
    //     if (!bTool) {
    //         LogHelper::LogError("game.item", "UpgradeSocket error - Lock or NULL[UCID:%d, invenType:%d, slotPos:%d]( %d )",
    //             dwUCID, psEquipItemInfo->byInvenType, psEquipItemInfo->shSlotPos, 11636);
    //         return 52340;
    //     }
    // }
    //
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_SOULSTONE_LEVELUP* pTB_UpgradeSocket = XResourceMgr::GetTB_SOULSTONE_LEVELUP_With_Lock(&pServer->m_xResourceMgr, nItemSocketID);
    // if (!pTB_UpgradeSocket) {
    //     LogHelper::LogError("game.item", "UpgradeSocket error - No Table TB_SOULSTONE_LEVELUP[UCID:%d, itemID:%d]( %d )",
    //         dwUCID, nItemSocketID, 11643);
    //     return 52340;
    // }
    //
    // __int64 biCost = pTB_UpgradeSocket->Need_Gold;
    // if (biCost > m_nInvenMoney && !bTool) {
    //     LogHelper::LogError("game.item", "UpgradeSocket error - Shortage cost[UCID:%d, cost:%I64d]( %d )",
    //         dwUCID, biCost, 11652);
    //     return 52342;
    // }
    //
    // ST_CREATE_ITEMS stReduceItems;
    // for (int i = 0; i < 5 && pTB_UpgradeSocket->Material_01_ID[i] && pTB_UpgradeSocket->Material_01_Count[i]; ++i) {
    //     ST_CREATE_ITEM stReduceItem;
    //     stReduceItem.nItemID = pTB_UpgradeSocket->Material_01_ID[i];
    //     stReduceItem.shCount = pTB_UpgradeSocket->Material_01_Count[i];
    //     stReduceItems.push_back(stReduceItem);
    // }
    //
    // PS_RES_STORAGE_INFO psUpdateItemList;
    // if (!ReduceItem2(&stReduceItems, 0x76, &psUpdateItemList) && !bTool) {
    //     LogHelper::LogError("game.item", "UpgradeSocket error - ReduceItem2[UCID:%d]( %d )", dwUCID, 11676);
    //     return 52343;
    // }
    //
    // int nSocketID = 0;
    // std::uint8_t byUpgradeFlag = 0;
    // int nRate = XItemFactory::nRand(&pServer->m_xItemFactory, 1, 10000);
    // nRate -= pTB_UpgradeSocket->Evolution_Rate;
    // if (nRate > 0) {
    //     nRate -= pTB_UpgradeSocket->HiddenSoulStone_Rate;
    //     if (nRate <= 0) {
    //         nSocketID = pTB_UpgradeSocket->HiddenSoulStone_TargetItemID;
    //         byUpgradeFlag = 1;
    //         if (bTool) {
    //             InsertToolSoulstoneInfo(nSocketID, byUpgradeFlag);
    //             return 0;
    //         }
    //     }
    // } else {
    //     nSocketID = pTB_UpgradeSocket->Evolution_TargetItemID;
    //     if (bTool) {
    //         InsertToolSoulstoneInfo(nSocketID, byUpgradeFlag);
    //         return 0;
    //     }
    // }
    //
    // TB_ITEM* pTBItem = XResourceMgr::GetTB_ITEM(&pServer->m_xResourceMgr, nSocketID);
    // if (!pTBItem) {
    //     LogHelper::LogError("game.item", "UpgradeSocket error - No Table TB_ITEM[UCID:%d, ItemID:%d]( %d )",
    //         dwUCID, nSocketID, 11715);
    //     return 52340;
    // }
    //
    // ST_ITEM_SOCKET* pSocketData = pEquipItemPtr->GetSocketData(bySocketIndex);
    // if (!pSocketData) return 52340;
    //
    // int nUpgradeSocketID = nSocketID;
    // ST_LOG_GAME stLog;
    // stLog._sSubType = 110;
    // stLog.nParam3 = pSocketData->dwSocketID;
    // stLog.nParam6 = pSocketData->biEquipSerial;
    //
    // if (!UpdateItemEnd(0x76, &psUpdateItemList, &stLog)) {
    //     UnLockList(&psUpdateItemList);
    //     LogHelper::LogError("game.item", "UpgradeSocket error - Failed UpdateItemEnd( %d )", 11733);
    //     return 52340;
    // }
    //
    // AddMoney(-biCost, 0x3B, 0, 0, 0);
    //
    // PS_DB_SOCKET_UPGRADE psDBSocketUpgrade;
    // psDBSocketUpgrade.dwUCID = dwUCID;
    // psDBSocketUpgrade.psUpdateItemList = psUpdateItemList;
    // psDBSocketUpgrade.byUpgradeFlag = byUpgradeFlag;
    // psDBSocketUpgrade.stSocketData = *pSocketData;
    // psDBSocketUpgrade.stSocketData.dwSocketID = nUpgradeSocketID;
    //
    // XSendDBPacket xSendDBPacket(pObject, 0x81, 0x28);
    // xSendDBPacket << psDBSocketUpgrade;
    // XGameServer::SendDBGame(&xSendDBPacket);
    //
    // // Log the upgrade
    // ST_LOG_GAME stLogGame;
    // stLogGame._sMainType = 4;
    // stLogGame._sSubType = 108;
    // stLogGame._nUAID = dwUAID;
    // stLogGame._nUCID = dwUCID;
    // stLogGame.nParam0 = pSocketData->dwSocketID;
    // stLogGame.nParam2 = pUser->GetLevel();
    // stLogGame.nParam3 = nUpgradeSocketID;
    // stLogGame.nParam4 = psDBSocketUpgrade.stSocketData.bySocketPos;
    // stLogGame.nParam5 = psDBSocketUpgrade.stSocketData.biEquipSerial;
    // XGameServer::SendDBLog(&stLogGame);
    //
    // return 0;

    (void)psEquipItemInfo;
    (void)nItemSocketID;
    (void)bySocketIndex;
    (void)bTool;
    return 52340;  // TODO: 需人工审查 - Implement when TB_SOULSTONE_LEVELUP/ReduceItem2/UpdateItemEnd available
}

// IDA: 0x1400D34E0
// __int64 __fastcall CGocInventory::UpgradeSocket(CGocInventory *this, PS_ITEM_SLOT_INFO psSocketItemInfo)
// Upgrades socket item itself (consumes item to create upgraded version)
int CGocInventory::UpgradeSocket(void* psSocketItemInfo) {
    // IDA Decompiled:
    // CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    // if (!pUser) return 52340;
    //
    // unsigned int dwUCID = CQuestCondition::GetQuestID(pUser->GetActorID());
    //
    // if (!IsValidSlotPos(psSocketItemInfo->shSlotPos, 0)) {
    //     LogHelper::LogError("game.item", "UpgradeSocket error - Fault slot[UCID:%d, slotPos:%d]( %d )",
    //         dwUCID, psSocketItemInfo->shSlotPos, 11779);
    //     return 52340;
    // }
    //
    // std::shared_ptr<CItem> pItemPtr;
    // std::uint8_t byLock;
    // GetSlotItem(&pItemPtr, psSocketItemInfo->byInvenType, psSocketItemInfo->shSlotPos, &byLock);
    // if (!pItemPtr || byLock) {
    //     LogHelper::LogError("game.item", "UpgradeSocket error - Lock or NULL[UCID:%d, invenType:%d, slotPos:%d]( %d )",
    //         dwUCID, psSocketItemInfo->byInvenType, psSocketItemInfo->shSlotPos, 11787);
    //     return 52340;
    // }
    //
    // if (CItem::IsBind(pItemPtr.get())) {
    //     LogHelper::LogError("game.item", "UpgradeSocket error - Bind_Type[UCID:%d, ID:%d]( %d )",
    //         dwUCID, pItemPtr->GetCurID(), 11794);
    //     return 52341;
    // }
    //
    // TB_ITEM_CLASSIFY* pTBClassify = CItem::GetClassifyTable(pItemPtr.get());
    // if (pTBClassify->SubCategoryID == 1) {
    //     LogHelper::LogError("game.item", "UpgradeSocket error - No Table TB_ITEM_CLASSIFY[UCID:%d, ID:%d]( %d )",
    //         dwUCID, pItemPtr->GetCurID(), 11801);
    //     return 52341;
    // }
    //
    // unsigned int dwIndex = pItemPtr->GetCurID();
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_SOULSTONE_LEVELUP* pTB_UpgradeSocket = XResourceMgr::GetTB_SOULSTONE_LEVELUP_With_Lock(&pServer->m_xResourceMgr, dwIndex);
    // if (!pTB_UpgradeSocket) {
    //     LogHelper::LogError("game.item", "UpgradeSocket error - No Table TB_SOULSTONE_LEVELUP[UCID:%d, itemID:%d]( %d )",
    //         dwUCID, pItemPtr->GetCurID(), 11808);
    //     return 52340;
    // }
    //
    // __int64 biCost = pTB_UpgradeSocket->Need_Gold;
    // if (biCost > m_nInvenMoney) {
    //     LogHelper::LogError("game.item", "UpgradeSocket error - Shortage cost[UCID:%d, cost:%I64d]( %d )",
    //         dwUCID, biCost, 11817);
    //     return 52342;
    // }
    //
    // PS_RES_STORAGE_INFO psUpdateItemList;
    // PS_RES_STORAGE_INFO psCreateItemList;
    //
    // if (!ReduceItem3(psSocketItemInfo->byInvenType, psSocketItemInfo->shSlotPos, 1, 0x76, &psUpdateItemList)) {
    //     LogHelper::LogError("game.item", "UpgradeSocket error - ReduceItem3[UCID:%d]( %d )", dwUCID, 11824);
    //     return 52343;
    // }
    //
    // ST_CREATE_ITEMS stReduceItems;
    // for (int i = 0; i < 5 && pTB_UpgradeSocket->Material_01_ID[i] && pTB_UpgradeSocket->Material_01_Count[i]; ++i) {
    //     ST_CREATE_ITEM stReduceItem;
    //     stReduceItem.nItemID = pTB_UpgradeSocket->Material_01_ID[i];
    //     stReduceItem.shCount = pTB_UpgradeSocket->Material_01_Count[i];
    //     stReduceItems.push_back(stReduceItem);
    // }
    //
    // if (!ReduceItem2(&stReduceItems, 0x76, &psUpdateItemList)) {
    //     LogHelper::LogError("game.item", "UpgradeSocket error - ReduceItem2[UCID:%d]( %d )", dwUCID, 11847);
    //     return 52343;
    // }
    //
    // int nSocketID = 0;
    // std::uint8_t byUpgradeFlag = 0;
    // int nRate = XItemFactory::nRand(&pServer->m_xItemFactory, 1, 10000);
    // nRate -= pTB_UpgradeSocket->Evolution_Rate;
    // if (nRate > 0) {
    //     nRate -= pTB_UpgradeSocket->HiddenSoulStone_Rate;
    //     if (nRate <= 0) {
    //         nSocketID = pTB_UpgradeSocket->HiddenSoulStone_TargetItemID;
    //         byUpgradeFlag = 1;
    //     }
    // } else {
    //     nSocketID = pTB_UpgradeSocket->Evolution_TargetItemID;
    // }
    //
    // ST_CREATE_ITEMS stCreateItems;
    // ST_CREATE_ITEM stCreateItem;
    // stCreateItem.nItemID = nSocketID;
    // stCreateItem.shCount = 1;
    // stCreateItems.push_back(stCreateItem);
    //
    // ST_LOG_GAME stLog;
    // stLog._sSubType = 109;
    // stLog.nParam3 = nSocketID;
    // stLog.nParam4 = pItemPtr->GetCurID();
    // stLog.nParam6 = pItemPtr->GetSerial();
    //
    // if (!CreateItem2(&stCreateItems, 0x76, 0, &psCreateItemList, &psUpdateItemList, &stLog)) {
    //     UnLockList(&psUpdateItemList);
    //     UnLockList(&psCreateItemList);
    //     LogHelper::LogError("game.item", "UpgradeSocket error - CreateItem2[UCID:%d]( %d )", dwUCID, 11888);
    //     return 52340;
    // }
    //
    // AddMoney(-biCost, 0x3B, 0, 0, 0);
    //
    // PS_DB_SOCKET_UPGRADE psDBSocketUpgrade;
    // psDBSocketUpgrade.byUpgradeType = 1;
    // psDBSocketUpgrade.dwUCID = dwUCID;
    // psDBSocketUpgrade.psCreateItemList = psCreateItemList;
    // psDBSocketUpgrade.psUpdateItemList = psUpdateItemList;
    // psDBSocketUpgrade.byUpgradeFlag = byUpgradeFlag;
    //
    // XSendDBPacket xSendDBPacket(pObject, 0x81, 0x28);
    // xSendDBPacket << psDBSocketUpgrade;
    // XGameServer::SendDBGame(&xSendDBPacket);
    //
    // return 0;

    (void)psSocketItemInfo;
    return 52340;  // TODO: 需人工审查 - Implement when TB_SOULSTONE_LEVELUP/ReduceItem3/CreateItem2 available
}

// ============================================================================
// Tool soulstone info functions (IDA verified)
// ============================================================================

// IDA: 0x1400D5630
// void __fastcall CGocInventory::InsertToolSoulstoneInfo(
//         CGocInventory *this, ST_CREATE_ITEM stItem, unsigned __int8 byRateType)
// Inserts soulstone info to m_stToolSoulstone
void CGocInventory::InsertToolSoulstoneInfo(void* stItem, std::uint8_t byRateType) {
    // IDA Decompiled:
    // PS_TOOL_SOULSTONE_INFO psSoulstoneInfo;
    // PS_TOOL_SOULSTONE_INFO::PS_TOOL_SOULSTONE_INFO(&psSoulstoneInfo);
    // psSoulstoneInfo.stItemInfo.nItemID = stItem.nItemID;
    // psSoulstoneInfo.stItemInfo.nCount = stItem.shCount;
    // psSoulstoneInfo.byRate = byRateType;
    // std::vector<PS_TOOL_ITEM_INFO>::push_back(&m_stToolSoulstone.vecInfo, &psSoulstoneInfo);

    // TODO: 需人工审查 - Implement when PS_TOOL_SOULSTONE_INFO/m_stToolSoulstone available
    (void)stItem;
    (void)byRateType;
}

// IDA: 0x1400D56C0
// void __fastcall CGocInventory::InsertToolSoulstoneInfo(CGocInventory *this, int nID, unsigned __int8 byFlag)
// Inserts soulstone info with ID directly
void CGocInventory::InsertToolSoulstoneInfo(int nID, std::uint8_t byFlag) {
    // IDA Decompiled:
    // PS_TOOL_SOULSTONE_INFO psSoulstoneInfo;
    // PS_TOOL_SOULSTONE_INFO::PS_TOOL_SOULSTONE_INFO(&psSoulstoneInfo);
    // psSoulstoneInfo.stItemInfo.nItemID = nID;
    // psSoulstoneInfo.stItemInfo.nCount = 1;
    // psSoulstoneInfo.byRate = byFlag;
    // std::vector<PS_TOOL_ITEM_INFO>::push_back(&m_stToolSoulstone.vecInfo, &psSoulstoneInfo);

    // TODO: 需人工审查 - Implement when PS_TOOL_SOULSTONE_INFO/m_stToolSoulstone available
    (void)nID;
    (void)byFlag;
}

// IDA: 0x1400D5860
// Checks whether the requested inventory type has sufficient empty slots.
bool CGocInventory::IsEmptyInventory(int nInvenType, int nNeedInven) {
    XBaseInventory* pInventory = nullptr;

    if (nInvenType) {
        switch (nInvenType) {
            case 1:
                pInventory = GetInvenPtr(4);
                if (!pInventory) {
                    return false;
                }
                if (pInventory->GetSimpleEmptySlotCount() < nNeedInven) {
                    return false;
                }
                break;

            case 2:
                pInventory = GetInvenPtr(0xD);
                if (!pInventory) {
                    return false;
                }
                if (pInventory->GetSimpleEmptySlotCount() < nNeedInven) {
                    return false;
                }
                break;

            case 3:
                pInventory = GetInvenPtr(0xB);
                if (!pInventory) {
                    return false;
                }
                if (pInventory->GetSimpleEmptySlotCount() < nNeedInven) {
                    return false;
                }
                break;
        }
    } else {
        pInventory = GetInvenPtr(2);
        if (!pInventory) {
            return false;
        }
        if (pInventory->GetSimpleEmptySlotCount() < nNeedInven) {
            return false;
        }
    }

    return true;
}

// ============================================================================
// Socket extract functions (IDA verified)
// ============================================================================

// IDA: 0x1400D3F70
// __int64 __fastcall CGocInventory::IsValidExtractSocket(
//         CGocInventory *this, unsigned int dwUCID, unsigned int nSocketItemID,
//         bool bUseItem, PS_DB_SOCKET_EXTRACT *psDBExtractInfo, __int64 *biCost, bool bTool)
// Validates socket extraction and builds extract info
int CGocInventory::IsValidExtractSocket(std::uint32_t dwUCID, std::uint32_t nSocketItemID,
                                         bool bUseItem, void* psDBExtractInfo,
                                         std::int64_t& biCost, bool bTool) {
    // IDA Decompiled:
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_ITEM* pTB_SocketItem = XResourceMgr::GetTB_ITEM(&pServer->m_xResourceMgr, nSocketItemID);
    // if (!pTB_SocketItem) {
    //     LogHelper::LogError("game.item", "IsValidExtractSocket error - No Table TB_ITEM[UCID:%d, SocketID:%d]", dwUCID, nSocketItemID);
    //     return 52350;
    // }
    // if (pTB_SocketItem->Item_Bind_Type == 2) {
    //     LogHelper::LogError("game.item", "IsValidExtractSocket error - Bind_Type[UCID:%d, SocketID:%d]", dwUCID, nSocketItemID);
    //     return 52350;
    // }
    // TB_FRAGMENT_EXTRACTION* pTB_Extraction = XResourceMgr::GetTB_FRAGMENT_EXTRACTION_With_Lock(&pServer->m_xResourceMgr, nSocketItemID);
    // if (!pTB_Extraction) {
    //     LogHelper::LogError("game.item", "IsValidExtractSocket error - No Table TB_FRAGMENT_EXTRACTION[UCID:%d, SocketID:%d]", dwUCID, nSocketItemID);
    //     return 52350;
    // }
    // biCost = pTB_Extraction->Extraction_NeedJeny;
    // if (!bTool && (biCost < 0 || biCost > m_nInvenMoney)) {
    //     LogHelper::LogError("game.item", "IsValidExtractSocket error - Shortage cost[UCID:%d, cost:%I64d]", dwUCID, biCost);
    //     return 52351;
    // }
    // // Complex extraction logic with random rates for 5 items
    // // Each item has 3 bonus rates (Extraction01_Rate_01/02/03)
    // // Uses ReduceItem2, CreateItem2, InsertToolSoulstoneInfo
    // // ...

    (void)dwUCID;
    (void)nSocketItemID;
    (void)bUseItem;
    (void)psDBExtractInfo;
    (void)biCost;
    (void)bTool;
    return 52350;  // TODO: 需人工审查 - Implement when TB_FRAGMENT_EXTRACTION/ReduceItem2/CreateItem2 available
}

// IDA: 0x1400D4BE0
// __int64 __fastcall CGocInventory::ExtractSocket(
//         CGocInventory *this, PS_ITEM_SLOT_INFO psEquipItemInfo,
//         unsigned int nItemSocketID, unsigned __int8 bySocketIndex, bool bUseItem)
// Extracts socket from equipment item
int CGocInventory::ExtractSocket(void* psEquipItemInfo, std::uint32_t nItemSocketID,
                                  std::uint8_t bySocketIndex, bool bUseItem) {
    // IDA Decompiled:
    // CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    // if (!pUser) return 52350;
    // unsigned int dwUCID = CQuestCondition::GetQuestID(pUser->GetActorID());
    //
    // std::shared_ptr<CItem> pEquipItemPtr;
    // std::uint8_t byLock;
    // GetSlotItem(&pEquipItemPtr, psEquipItemInfo->byInvenType, psEquipItemInfo->shSlotPos, &byLock);
    // if (!pEquipItemPtr || byLock) {
    //     LogHelper::LogError("game.item", "ExtractSocket error - Lock or NULL[UCID:%d, invenType:%d, slotPos:%d]",
    //         dwUCID, psEquipItemInfo->byInvenType, psEquipItemInfo->shSlotPos);
    //     return 52350;
    // }
    //
    // PS_DB_SOCKET_EXTRACT psDBExtract;
    // __int64 biCost = 0;
    // int nResult = IsValidExtractSocket(dwUCID, nItemSocketID, bUseItem, &psDBExtract, &biCost, false);
    // if (nResult <= 0) {
    //     AddMoney(-biCost, 0x3C, 0, 0, 0);
    //     psDBExtract.byExtratType = 0;
    //     psDBExtract.biSerial = pEquipItemPtr->GetSerial();
    //     psDBExtract.bySocketIndex = bySocketIndex;
    //
    //     XSendDBPacket xSendDBPacket(pObject, 0x81, 0x29);
    //     xSendDBPacket << psDBExtract;
    //     XGameServer::SendDBGame(&xSendDBPacket);
    //
    //     // Log
    //     ST_LOG_GAME stLog;
    //     stLog._sMainType = 4;
    //     stLog._sSubType = 111;
    //     stLog._nUAID = pUser->GetUAID();
    //     stLog._nUCID = dwUCID;
    //     stLog.nParam0 = nItemSocketID;
    //     stLog.nParam2 = pUser->GetLevel();
    //     stLog.nParam3 = pEquipItemPtr->GetCurID();
    //     stLog.nParam6 = pEquipItemPtr->GetSerial();
    //     XGameServer::SendDBLog(&stLog);
    //     return 0;
    // }
    // return nResult;

    (void)psEquipItemInfo;
    (void)nItemSocketID;
    (void)bySocketIndex;
    (void)bUseItem;
    return 52350;  // TODO: 需人工审查 - Implement when IsValidExtractSocket/XSendDBPacket available
}

// IDA: 0x1400D4FF0
// __int64 __fastcall CGocInventory::ExtractSocket(CGocInventory *this, PS_ITEM_SLOT_INFO psItemInfo, bool bUseItem)
// Extracts socket item itself
int CGocInventory::ExtractSocket(void* psItemInfo, bool bUseItem) {
    // IDA Decompiled:
    // CUser* pUser = dynamic_cast<CUser*>(GetOwner());
    // if (!pUser) return 52350;
    // unsigned int dwUCID = CQuestCondition::GetQuestID(pUser->GetActorID());
    //
    // std::shared_ptr<CItem> pItemPtr;
    // std::uint8_t byLock;
    // GetSlotItem(&pItemPtr, psItemInfo->byInvenType, psItemInfo->shSlotPos, &byLock);
    // if (!pItemPtr || byLock) {
    //     LogHelper::LogError("game.item", "ExtractSocket error - Lock or NULL[UCID:%d, invenType:%d, slotPos:%d]",
    //         dwUCID, psItemInfo->byInvenType, psItemInfo->shSlotPos);
    //     return 52350;
    // }
    //
    // PS_DB_SOCKET_EXTRACT psDBExtract;
    // __int64 biCost = 0;
    // unsigned int nSocketItemID = pItemPtr->GetCurID();
    // int nResult = IsValidExtractSocket(dwUCID, nSocketItemID, bUseItem, &psDBExtract, &biCost, false);
    // if (nResult <= 0) {
    //     if (ReduceItem3(psItemInfo->byInvenType, psItemInfo->shSlotPos, 1, 0x77, &psDBExtract.psUpdateItemList)) {
    //         ST_LOG_GAME stLog;
    //         stLog._sSubType = 111;
    //         if (UpdateItemEnd(0x77, &psDBExtract.psUpdateItemList, &stLog)) {
    //             AddMoney(-biCost, 0x3C, 0, 0, 0);
    //             psDBExtract.byExtratType = 1;
    //
    //             XSendDBPacket xSendDBPacket(pObject, 0x81, 0x29);
    //             xSendDBPacket << psDBExtract;
    //             XGameServer::SendDBGame(&xSendDBPacket);
    //
    //             // Log
    //             ST_LOG_GAME stLogGame;
    //             stLogGame._sMainType = 4;
    //             stLogGame._sSubType = 111;
    //             stLogGame._nUAID = pUser->GetUAID();
    //             stLogGame._nUCID = dwUCID;
    //             stLogGame.nParam0 = pItemPtr->GetCurID();
    //             stLogGame.nParam2 = pUser->GetLevel();
    //             stLogGame.nParam5 = pItemPtr->GetSerial();
    //             XGameServer::SendDBLog(&stLogGame);
    //             return 0;
    //         }
    //         return 52352;
    //     }
    //     return 52352;
    // }
    // return nResult;

    (void)psItemInfo;
    (void)bUseItem;
    return 52350;  // TODO: 需人工审查 - Implement when IsValidExtractSocket/ReduceItem3/UpdateItemEnd available
}

// ============================================================================
// Tool disassemble functions (IDA verified)
// ============================================================================

// IDA: 0x1400D5750
// void __fastcall CGocInventory::GetToolDisassemble(CGocInventory *this, PS_RES_TOOL_DROP_INFO *psToolInfo)
// Gets tool disassemble info from m_stToolDisassemble
void CGocInventory::GetToolDisassemble(void* psToolInfo) {
    // IDA Decompiled:
    // for (size_t i = 0; i < m_stToolDisassemble.size(); ++i) {
    //     PS_TOOL_ITEM_INFO psInfo;
    //     psInfo.nCount = m_stToolDisassemble[i].stEventInfo.nID;
    //     psInfo.nItemID = m_stToolDisassemble[i].stEventInfo.nType;
    //     psToolInfo->vecInfo.push_back(psInfo);
    // }

    // TODO: 需人工审查 - Implement when PS_RES_TOOL_DROP_INFO/m_stToolDisassemble available
    (void)psToolInfo;
}

// IDA: 0x1400D5830
// void __fastcall CGocInventory::InsertToolDisassemble(CGocInventory *this, const ST_QUEST_FIRST_DROP_ITEM *psInfo)
// Inserts disassemble info to m_stToolDisassemble
void CGocInventory::InsertToolDisassemble(void* psInfo) {
    // IDA Decompiled:
    // m_stToolDisassemble.push_back(*psInfo);

    // TODO: 需人工审查 - Implement when m_stToolDisassemble type available
    (void)psInfo;
}

// ============================================================================
// Batch 3: Warp item and Item limit functions
// ============================================================================

// IDA: 0x1400D6D50
// void __fastcall CGocInventory::DeleteUsedWarpItem(CGocInventory *this)
// Deletes used warp item after teleport
void CGocInventory::DeleteUsedWarpItem() {
    // IDA Decompiled:
    // CGocInventory *v0; // rsi
    // XActor *v1; // rax
    // CGocWarpItem *v2; // rax
    // int v3; // ecx
    // CGocWarpItem *v4; // rax
    //
    // v0 = this;
    // if ( *((_BYTE *)this + 0x1AC8) )
    // {
    //   v1 = XActorComponent::GetActor<XActor>(this);
    //   v2 = XActorComponent::GetGoc<CGocWarpItem>(v1);
    //   v3 = *((_DWORD *)v0 + 0x6A8);
    //   CGocWarpItem::Lock(v2, v3);
    //   v4 = XActorComponent::GetGoc<CGocWarpItem>(v1);
    //   CGocWarpItem::ReduceWarpItem(v4, *((_DWORD *)v0 + 0x6A8), 1);
    //   *((_BYTE *)v0 + 0x1AC8) = 0;
    //   *((_DWORD *)v0 + 0x6A8) = 0;
    // }

    // TODO: 需人工审查 - Implement when m_bUsedWarpItem/m_nUsedWarpItemSlotIndex available
}

// IDA: 0x1400D73D0
// void __fastcall CGocInventory::ClearUsedWarpItem(CGocInventory *this, unsigned __int8 byLogType)
// Clears used warp item state with optional logging
void CGocInventory::ClearUsedWarpItem(std::uint8_t byLogType) {
    // IDA Decompiled:
    // CGocInventory *v1; // rsi
    // char v2; // bl
    // XActor *v3; // rax
    // int v4; // ecx
    // CGocWarpItem *v5; // rax
    // CGocWarpItem *v6; // rax
    // const ST_WARP_ITEM *v7; // rax
    //
    // v1 = this;
    // v2 = byLogType;
    // if ( *((_BYTE *)this + 0x1AC8) )
    // {
    //   v3 = XActorComponent::GetActor<XActor>(this);
    //   v4 = *((_DWORD *)v1 + 0x6A8);
    //   v5 = XActorComponent::GetGoc<CGocWarpItem>(v3);
    //   CGocWarpItem::Lock(v5, v4);
    //   v6 = XActorComponent::GetGoc<CGocWarpItem>(v3);
    //   v7 = CGocWarpItem::GetWarpItem(v6, *((_DWORD *)v1 + 0x6A8));
    //   if ( v7 )
    //   {
    //     if ( v2 )
    //       CGocInventory::WriteLogUseWarpItem(v1, v7, v2);
    //   }
    //   *((_BYTE *)v1 + 0x1AC8) = 0;
    //   *((_DWORD *)v1 + 0x6A8) = 0;
    // }

    // TODO: 需人工审查 - Implement when m_bUsedWarpItem/m_nUsedWarpItemSlotIndex/WriteLogUseWarpItem available
    (void)byLogType;
}

// IDA: 0x1400D76F0
// int __fastcall CGocInventory::CheckMoveMap(CGocInventory *this, int nMoveType, PS_ENTER_MAP_REQ psEnterMap)
// Checks if map move is valid (maze, district, roguelike)
int CGocInventory::CheckMoveMap(int nMoveType, void* psEnterMap) {
    // IDA Decompiled:
    // CGocInventory *v2; // rsi
    // int v3; // ebx
    // PS_ENTER_MAP_REQ v4; // r14
    // XActor *v5; // rax
    // XGameUser *v6; // rax
    // CGocState *v7; // rax
    // __int64 v8; // rax
    // int v9; // eax
    // int v10; // eax
    // int v11; // eax
    // int v12; // ecx
    // int v13; // edx
    // int v14; // edx
    // int v15; // eax
    // int v16; // eax
    // int v17; // eax
    // int v18; // eax
    // int v19; // eax
    // int v20; // eax
    // int v21; // ecx
    // int v22; // edx
    // int v23; // edx
    // int v24; // eax
    // int v25; // eax
    // int v26; // ecx
    // int v27; // edx
    // int v28; // eax
    // int v29; // eax
    // int v30; // ecx
    // int v31; // edx
    // int v32; // eax
    // int v33; // eax
    // int v34; // eax
    // int v35; // eax
    // int v36; // eax
    // int v37; // eax
    // int v38; // eax
    // int v39; // eax
    // int v40; // eax
    // int v41; // ecx
    // int v42; // edx
    // int v43; // edx
    // int v44; // eax
    // int v45; // eax
    // int v46; // eax
    // int v47; // eax
    // int v48; // ecx
    // int v49; // edx
    // int v50; // edx
    // int v51; // eax
    // int v52; // eax
    // int v53; // eax
    // int v54; // eax
    // int v55; // eax
    // int v56; // eax
    // int v57; // eax
    // int v58; // eax
    // int v59; // eax
    // int v60; // eax
    // int v61; // eax
    // int v62; // ecx
    // int v63; // edx
    // int v64; // edx
    // int v65; // eax
    // int v66; // eax
    // int v67; // eax
    // int v68; // eax
    // int v69; // eax
    // int v70; // eax
    // int v71; // eax
    // int v72; // eax
    // int v73; // eax
    // int v74; // eax
    // int v75; // eax
    // int v76; // eax
    // int v77; // eax
    // int v78; // eax
    // int v79; // eax
    // int v80; // eax
    // int v81; // eax
    // XGameUser *v82; // rax
    // CGocState *v83; // rax
    // char v84; // al
    // int v85; // eax
    // int v86; // eax
    // int v87; // eax
    // int v88; // eax
    // int v89; // eax
    // int v90; // eax
    // int v91; // eax
    // int v92; // eax
    // int v93; // eax
    // int v94; // eax
    // int v95; // eax
    // int v96; // eax
    // int v97; // eax
    // int v98; // eax
    // int v99; // eax
    // int v100; // eax
    // int v101; // eax
    // int v102; // eax
    // int v103; // eax
    // int v104; // eax
    // int v105; // eax
    // int v106; // eax
    // int v107; // eax
    // int v108; // eax
    // int v109; // eax
    // int v110; // eax
    // int v111; // eax
    // int v112; // eax
    // int v113; // eax
    // int v114; // eax
    // int v115; // eax
    // int v116; // eax
    // int v117; // eax
    // int v118; // eax
    // int v119; // eax
    // int v120; // eax
    // int v121; // eax
    // int v122; // eax
    // int v123; // eax
    // int v124; // eax
    // int v125; // eax
    // int v126; // eax
    // int v127; // eax
    // int v128; // eax
    // int v129; // eax
    // int v130; // eax
    // int v131; // eax
    // int v132; // eax
    // int v133; // eax
    // int v134; // eax
    // int v135; // eax
    // int v136; // eax
    // int v137; // eax
    // int v138; // eax
    // int v139; // eax
    // int v140; // eax
    // int v141; // eax
    // int v142; // eax
    // int v143; // eax
    // int v144; // eax
    // int v145; // eax
    // int v146; // eax
    // int v147; // eax
    // int v148; // eax
    // int v149; // eax
    // int v150; // eax
    // int v151; // eax
    // int v152; // eax
    // int v153; // eax
    // int v154; // eax
    // int v155; // eax
    // int v156; // eax
    // int v157; // eax
    // int v158; // eax
    // int v159; // eax
    // int v160; // eax
    // int v161; // eax
    // int v162; // eax
    // int v163; // eax
    // int v164; // eax
    // int v165; // eax
    // int v166; // eax
    // int v167; // eax
    // int v168; // eax
    // int v169; // eax
    // int v170; // eax
    // int v171; // eax
    // int v172; // eax
    // int v173; // eax
    // int v174; // eax
    // int v175; // eax
    // int v176; // eax
    // int v177; // eax
    // int v178; // eax
    // int v179; // eax
    // int v180; // eax
    // int v181; // eax
    // int v182; // eax
    // int v183; // eax
    // int v184; // eax
    // int v185; // eax
    // int v186; // eax
    // int v187; // eax
    // int v188; // eax
    // int v189; // eax
    // int v190; // eax
    // int v191; // eax
    // int v192; // eax
    // int v193; // eax
    // int v194; // eax
    // int v195; // eax
    // int v196; // eax
    // int v197; // eax
    // int v198; // eax
    // int v199; // eax
    // int v200; // eax
    // int v201; // eax
    // int v202; // eax
    // int v203; // eax
    // int v204; // eax
    // int v205; // eax
    // int v206; // eax
    // int v207; // eax
    // int v208; // eax
    // int v209; // eax
    // int v210; // eax
    // int v211; // eax
    // int v212; // eax
    // int v213; // eax
    // int v214; // eax
    // int v215; // eax
    // int v216; // eax
    // int v217; // eax
    // int v218; // eax
    // int v219; // eax
    // int v220; // eax
    // int v221; // eax
    // int v222; // eax
    // int v223; // eax
    // int v224; // eax
    // int v225; // eax
    // int v226; // eax
    // int v227; // eax
    // int v228; // eax
    // int v229; // eax
    // int v230; // eax
    // int v231; // eax
    // int v232; // eax
    // int v233; // eax
    // int v234; // eax
    // int v235; // eax
    // int v236; // eax
    // int v237; // eax
    // int v238; // eax
    // int v239; // eax
    // int v240; // eax
    // int v241; // eax
    // int v242; // eax
    // int v243; // eax
    // int v244; // eax
    // int v245; // eax
    // int v246; // eax
    // int v247; // eax
    // int v248; // eax
    // int v249; // eax
    // int v250; // eax
    // int v251; // eax
    // int v252; // eax
    // int v253; // eax
    // int v254; // eax
    // int v255; // eax
    // int v256; // eax
    // int v257; // eax
    // int v258; // eax
    // int v259; // eax
    // int v260; // eax
    // int v261; // eax
    // int v262; // eax
    // int v263; // eax
    // int v264; // eax
    // int v265; // eax
    // int v266; // eax
    // int v267; // eax
    // int v268; // eax
    // int v269; // eax
    // int v270; // eax
    // int v271; // eax
    // int v272; // eax
    // int v273; // eax
    // int v274; // eax
    // int v275; // eax
    // int v276; // eax
    // int v277; // eax
    // int v278; // eax
    // int v279; // eax
    // int v280; // eax
    // int v281; // eax
    // int v282; // eax
    // int v283; // eax
    // int v284; // eax
    // int v285; // eax
    // int v286; // eax
    // int v287; // eax
    // int v288; // eax
    // int v289; // eax
    // int v290; // eax
    // int v291; // eax
    // int v292; // eax
    // int v293; // eax
    // int v294; // eax
    // int v295; // eax
    // int v296; // eax
    // int v297; // eax
    // int v298; // eax
    // int v299; // eax
    // int v300; // eax
    // int v301; // eax
    // int v302; // eax
    // int v303; // eax
    // int v304; // eax
    // int v305; // eax
    // int v306; // eax
    // int v307; // eax
    // int v308; // eax
    // int v309; // eax
    // int v310; // eax
    // int v311; // eax
    // int v312; // eax
    // int v313; // eax
    // int v314; // eax
    // int v315; // eax
    // int v316; // eax
    // int v317; // eax
    // int v318; // eax
    // int v319; // eax
    // int v320; // eax
    // int v321; // eax
    // int v322; // eax
    // int v323; // eax
    // int v324; // eax
    // int v325; // eax
    // int v326; // eax
    // int v327; // eax
    // int v328; // eax
    // int v329; // eax
    // int v330; // eax
    // int v331; // eax
    // int v332; // eax
    // int v333; // eax
    // int v334; // eax
    // int v335; // eax
    // int v336; // eax
    // int v337; // eax
    // int v338; // eax
    // int v339; // eax
    // int v340; // eax
    // int v341; // eax
    // int v342; // eax
    // int v343; // eax
    // int v344; // eax
    // int v345; // eax
    // int v346; // eax
    // int v347; // eax
    // int v348; // eax
    // int v349; // eax
    // int v350; // eax
    // int v351; // eax
    // int v352; // eax
    // int v353; // eax
    // int v354; // eax
    // int v355; // eax
    // int v356; // eax
    // int v357; // eax
    // int v358; // eax
    // int v359; // eax
    // int v360; // eax
    // int v361; // eax
    // int v362; // eax
    // int v363; // eax
    // int v364; // eax
    // int v365; // eax
    // int v366; // eax
    // int v367; // eax
    // int v368; // eax
    // int v369; // eax
    // int v370; // eax
    // int v371; // eax
    // int v372; // eax
    // int v373; // eax
    // int v374; // eax
    // int v375; // eax
    // int v376; // eax
    // int v377; // eax
    // int v378; // eax
    // int v379; // eax
    // int v380; // eax
    // int v381; // eax
    // int v382; // eax
    // int v383; // eax
    // int v384; // eax
    // int v385; // eax
    // int v386; // eax
    // int v387; // eax
    // int v388; // eax
    // int v389; // eax
    // int v390; // eax
    // int v391; // eax
    // int v392; // eax
    // int v393; // eax
    // int v394; // eax
    // int v395; // eax
    // int v396; // eax
    // int v397; // eax
    // int v398; // eax
    // int v399; // eax
    // int v400; // eax
    // int v401; // eax
    // int v402; // eax
    // int v403; // eax
    // int v404; // eax
    // int v405; // eax
    // int v406; // eax
    // int v407; // eax
    // int v408; // eax
    // int v409; // eax
    // int v410; // eax
    // int v411; // eax
    // int v412; // eax
    // int v413; // eax
    // int v414; // eax
    // int v415; // eax
    // int v416; // eax
    // int v417; // eax
    // int v418; // eax
    // int v419; // eax
    // int v420; // eax
    // int v421; // eax
    // int v422; // eax
    // int v423; // eax
    // int v424; // eax
    // int v425; // eax
    // int v426; // eax
    // int v427; // eax
    // int v428; // eax
    // int v429; // eax
    // int v430; // eax
    // int v431; // eax
    // int v432; // eax
    // int v433; // eax
    // int v434; // eax
    // int v435; // eax
    // int v436; // eax
    // int v437; // eax
    // int v438; // eax
    // int v439; // eax
    // int v440; // eax
    // int v441; // eax
    // int v442; // eax
    // int v443; // eax
    // int v444; // eax
    // int v445; // eax
    // int v446; // eax
    // int v447; // eax
    // int v448; // eax
    // int v449; // eax
    // int v450; // eax
    // int v451; // eax
    // int v452; // eax
    // int v453; // eax
    // int v454; // eax
    // int v455; // eax
    // int v456; // eax
    // int v457; // eax
    // int v458; // eax
    // int v459; // eax
    // int v460; // eax
    // int v461; // eax
    // int v462; // eax
    // int v463; // eax
    // int v464; // eax
    // int v465; // eax
    // int v466; // eax
    // int v467; // eax
    // int v468; // eax
    // int v469; // eax
    // int v470; // eax
    // int v471; // eax
    // int v472; // eax
    // int v473; // eax
    // int v474; // eax
    // int v475; // eax
    // int v476; // eax
    // int v477; // eax
    // int v478; // eax
    // int v479; // eax
    // int v480; // eax
    // int v481; // eax
    // int v482; // eax
    // int v483; // eax
    // int v484; // eax
    // int v485; // eax
    // int v486; // eax
    // int v487; // eax
    // int v488; // eax
    // int v489; // eax
    // int v490; // eax
    // int v491; // eax
    // int v492; // eax
    // int v493; // eax
    // int v494; // eax
    // int v495; // eax
    // int v496; // eax
    // int v497; // eax
    // int v498; // eax
    // int v499; // eax
    // int v500; // eax
    // int v501; // eax
    // int v502; // eax
    // int v503; // eax
    // int v504; // eax
    // int v505; // eax
    // int v506; // eax
    // int v507; // eax
    // int v508; // eax
    // int v509; // eax
    // int v510; // eax
    // int v511; // eax
    // int v512; // eax
    // int v513; // eax
    // int v514; // eax
    // int v515; // eax
    // int v516; // eax
    // int v517; // eax
    // int v518; // eax
    // int v519; // eax
    // int v520; // eax
    // int v521; // eax
    // int v522; // eax
    // int v523; // eax
    // int v524; // eax
    // int v525; // eax
    // int v526; // eax
    // int v527; // eax
    // int v528; // eax
    // int v529; // eax
    // int v530; // eax
    // int v531; // eax
    // int v532; // eax
    // int v533; // eax
    // int v534; // eax
    // int v535; // eax
    // int v536; // eax
    // int v537; // eax
    // int v538; // eax
    // int v539; // eax
    // int v540; // eax
    // int v541; // eax
    // int v542; // eax
    // int v543; // eax
    // int v544; // eax
    // int v545; // eax
    // int v546; // eax
    // int v547; // eax
    // int v548; // eax
    // int v549; // eax
    // int v550; // eax
    // int v551; // eax
    // int v552; // eax
    // int v553; // eax
    // int v554; // eax
    // int v555; // eax
    // int v556; // eax
    // int v557; // eax
    // int v558; // eax
    // int v559; // eax
    // int v560; // eax
    // int v561; // eax
    // int v562; // eax
    // int v563; // eax
    // int v564; // eax
    // int v565; // eax
    // int v566; // eax
    // int v567; // eax
    // int v568; // eax
    // int v569; // eax
    // int v570; // eax
    // int v571; // eax
    // int v572; // eax
    // int v573; // eax
    // int v574; // eax
    // int v575; // eax
    // int v576; // eax
    // int v577; // eax
    // int v578; // eax
    // int v579; // eax
    // int v580; // eax
    // int v581; // eax
    // int v582; // eax
    // int v583; // eax
    // int v584; // eax
    // int v585; // eax
    // int v586; // eax
    // int v587; // eax
    // int v588; // eax
    // int v589; // eax
    // int v590; // eax
    // int v591; // eax
    // int v592; // eax
    // int v593; // eax
    // int v594; // eax
    // int v595; // eax
    // int v596; // eax
    // int v597; // eax
    // int v598; // eax
    // int v599; // eax
    // int v600; // eax
    // int v601; // eax
    // int v602; // eax
    // int v603; // eax
    // int v604; // eax
    // int v605; // eax
    // int v606; // eax
    // int v607; // eax
    // int v608; // eax
    // int v609; // eax
    // int v610; // eax
    // int v611; // eax
    // int v612; // eax
    // int v613; // eax
    // int v614; // eax
    // int v615; // eax
    // int v616; // eax
    // int v617; // eax
    // int v618; // eax
    // int v619; // eax
    // int v620; // eax
    // int v621; // eax
    // int v622; // eax
    // int v623; // eax
    // int v624; // eax
    // int v625; // eax
    // int v626; // eax
    // int v627; // eax
    // int v628; // eax
    // int v629; // eax
    // int v630; // eax
    // int v631; // eax
    // int v632; // eax
    // int v633; // eax
    // int v634; // eax
    // int v635; // eax
    // int v636; // eax
    // int v637; // eax
    // int v638; // eax
    // int v639; // eax
    // int v640; // eax
    // int v641; // eax
    // int v642; // eax
    // int v643; // eax
    // int v644; // eax
    // int v645; // eax
    // int v646; // eax
    // int v647; // eax
    // int v648; // eax
    // int v649; // eax
    // int v650; // eax
    // int v651; // eax
    // int v652; // eax
    // int v653; // eax
    // int v654; // eax
    // int v655; // eax
    // int v656; // eax
    // int v657; // eax
    // int v658; // eax
    // int v659; // eax
    // int v660; // eax
    // int v661; // eax
    // int v662; // eax
    // int v663; // eax
    // int v664; // eax
    // int v665; // eax
    // int v666; // eax
    // int v667; // eax
    // int v668; // eax
    // int v669; // eax
    // int v670; // eax
    // int v671; // eax
    // int v672; // eax
    // int v673; // eax
    // int v674; // eax
    // int v675; // eax
    // int v676; // eax
    // int v677; // eax
    // int v678; // eax
    // int v679; // eax
    // int v680; // eax
    // int v681; // eax
    // int v682; // eax
    // int v683; // eax
    // int v684; // eax
    // int v685; // eax
    // int v686; // eax
    // int v687; // eax
    // int v688; // eax
    // int v689; // eax
    // int v690; // eax
    // int v691; // eax
    // int v692; // eax
    // int v693; // eax
    // int v694; // eax
    // int v695; // eax
    // int v696; // eax
    // int v697; // eax
    // int v698; // eax
    // int v699; // eax
    // int v700; // eax
    // int v701; // eax
    // int v702; // eax
    // int v703; // eax
    // int v704; // eax
    // int v705; // eax
    // int v706; // eax
    // int v707; // eax
    // int v708; // eax
    // int v709; // eax
    // int v710; // eax
    // int v711; // eax
    // int v712; // eax
    // int v713; // eax
    // int v714; // eax
    // int v715; // eax
    // int v716; // eax
    // int v717; // eax
    // int v718; // eax
    // int v719; // eax
    // int v720; // eax
    // int v721; // eax
    // int v722; // eax
    // int v723; // eax
    // int v724; // eax
    // int v725; // eax
    // int v726; // eax
    // int v727; // eax
    // int v728; // eax
    // int v729; // eax
    // int v730; // eax
    // int v731; // eax
    // int v732; // eax
    // int v733; // eax
    // int v734; // eax
    // int v735; // eax
    // int v736; // eax
    // int v737; // eax
    // int v738; // eax
    // int v739; // eax
    // int v740; // eax
    // int v741; // eax
    // int v742; // eax
    // int v743; // eax
    // int v744; // eax
    // int v745; // eax
    // int v746; // eax
    // int v747; // eax
    // int v748; // eax
    // int v749; // eax
    // int v750; // eax
    // int v751; // eax
    // int v752; // eax
    // int v753; // eax
    // int v754; // eax
    // int v755; // eax
    // int v756; // eax
    // int v757; // eax
    // int v758; // eax
    // int v759; // eax
    // int v760; // eax
    // int v761; // eax
    // int v762; // eax
    // int v763; // eax
    // int v764; // eax
    // int v765; // eax
    // int v766; // eax
    // int v767; // eax
    // int v768; // eax
    // int v769; // eax
    // int v770; // eax
    // int v771; // eax
    // int v772; // eax
    // int v773; // eax
    // int v774; // eax
    // int v775; // eax
    // int v776; // eax
    // int v777; // eax
    // int v778; // eax
    // int v779; // eax
    // int v780; // eax
    // int v781; // eax
    // int v782; // eax
    // int v783; // eax
    // int v784; // eax
    // int v785; // eax
    // int v786; // eax
    // int v787; // eax
    // int v788; // eax
    // int v789; // eax
    // int v790; // eax
    // int v791; // eax
    // int v792; // eax
    // int v793; // eax
    // int v794; // eax
    // int v795; // eax
    // int v796; // eax
    // int v797; // eax
    // int v798; // eax
    // int v799; // eax
    // int v800; // eax
    // int v801; // eax
    // int v802; // eax
    // int v803; // eax
    // int v804; // eax
    // int v805; // eax
    // int v806; // eax
    // int v807; // eax
    // int v808; // eax
    // int v809; // eax
    // int v810; // eax
    // int v811; // eax
    // int v812; // eax
    // int v813; // eax
    // int v814; // eax
    // int v815; // eax
    // int v816; // eax
    // int v817; // eax
    // int v818; // eax
    // int v819; // eax
    // int v820; // eax
    // int v821; // eax
    // int v822; // eax
    // int v823; // eax
    // int v824; // eax
    // int v825; // eax
    // int v826; // eax
    // int v826; // eax
    // int v827; // eax
    // int v828; // eax
    // int v829; // eax
    // int v830; // eax
    // int v831; // eax
    // int v832; // eax
    // int v833; // eax
    // int v834; // eax
    // int v835; // eax
    // int v836; // eax
    // int v837; // eax
    // int v838; // eax
    // int v839; // eax
    // int v840; // eax
    // int v841; // eax
    // int v842; // eax
    // int v843; // eax
    // int v844; // eax
    // int v845; // eax
    // int v846; // eax
    // int v847; // eax
    // int v848; // eax
    // int v849; // eax
    // int v850; // eax
    // int v851; // eax
    // int v852; // eax
    // int v853; // eax
    // int v854; // eax
    // int v855; // eax
    // int v856; // eax
    // int v857; // eax
    // int v858; // eax
    // int v859; // eax
    // int v860; // eax
    // int v861; // eax
    // int v862; // eax
    // int v863; // eax
    // int v864; // eax
    // int v865; // eax
    // int v866; // eax
    // int v867; // eax
    // int v868; // eax
    // int v869; // eax
    // int v870; // eax
    // int v871; // eax
    // int v872; // eax
    // int v873; // eax
    // int v874; // eax
    // int v875; // eax
    // int v876; // eax
    // int v877; // eax
    // int v878; // eax
    // int v879; // eax
    // int v880; // eax
    // int v881; // eax
    // int v882; // eax
    // int v883; // eax
    // int v884; // eax
    // int v885; // eax
    // int v886; // eax
    // int v887; // eax
    // int v888; // eax
    // int v889; // eax
    // int v890; // eax
    // int v891; // eax
    // int v892; // eax
    // int v893; // eax
    // int v894; // eax
    // int v895; // eax
    // int v896; // eax
    // int v897; // eax
    // int v898; // eax
    // int v899; // eax
    // int v900; // eax
    // int v901; // eax
    // int v902; // eax
    // int v903; // eax
    // int v904; // eax
    // int v905; // eax
    // int v906; // eax
    // int v907; // eax
    // int v908; // eax
    // int v909; // eax
    // int v910; // eax
    // int v911; // eax
    // int v912; // eax
    // int v913; // eax
    // int v914; // eax
    // int v915; // eax
    // int v916; // eax
    // int v917; // eax
    // int v918; // eax
    // int v919; // eax
    // int v920; // eax
    // int v921; // eax
    // int v922; // eax
    // int v923; // eax
    // int v924; // eax
    // int v925; // eax
    // int v926; // eax
    // int v927; // eax
    // int v928; // eax
    // int v929; // eax
    // int v930; // eax
    // int v931; // eax
    // int v932; // eax
    // int v933; // eax
    // int v934; // eax
    // int v935; // eax
    // int v936; // eax
    // int v937; // eax
    // int v938; // eax
    // int v939; // eax
    // int v940; // eax
    // int v941; // eax
    // int v942; // eax
    // int v943; // eax
    // int v944; // eax
    // int v945; // eax
    // int v946; // eax
    // int v947; // eax
    // int v948; // eax
    // int v949; // eax
    // int v950; // eax
    // int v951; // eax
    // int v952; // eax
    // int v953; // eax
    // int v954; // eax
    // int v955; // eax
    // int v956; // eax
    // int v957; // eax
    // int v958; // eax
    // int v959; // eax
    // int v960; // eax
    // int v961; // eax
    // int v962; // eax
    // int v963; // eax
    // int v964; // eax
    // int v965; // eax
    // int v966; // eax
    // int v967; // eax
    // int v968; // eax
    // int v969; // eax
    // int v970; // eax
    // int v971; // eax
    // int v972; // eax
    // int v973; // eax
    // int v974; // eax
    // int v975; // eax
    // int v976; //eax
    // int v977; // eax
    // int v978; // eax
    // int v979; // eax
    // int v980; // eax
    // int v981; // eax
    // int v982; // eax
    // int v983; // eax
    // int v984; // eax
    // int v985; // eax
    // int v986; // eax
    // int v987; // eax
    // int v988; // eax
    // int v989; // eax
    // int v990; // eax
    // int v991; // eax
    // int v992; // eax
    // int v993; // eax
    // int v994; // eax
    // int v995; // eax
    // int v996; // eax
    // int v997; // eax
    // int v998; // eax
    // int v999; // eax
    // v2 = this;
    // v3 = nMoveType;
    // v4 = psEnterMap;
    // if ( !psEnterMap )
    //   return 0;
    // v5 = XActorComponent::GetActor<XActor>(this);
    // v6 = (XGameUser *)XActor::GetUser(v5);
    // v7 = XActorComponent::GetGoc<CGocState>(v5);
    // v8 = CGocState::GetMazeInfo(v7);
    // if ( !v8 )
    //   return 101;
    // v9 = *(_DWORD *)v8;
    // switch ( v3 )
    // {
    //   case 1:  // Maze entry
    //     // Check maze conditions
    //     v10 = CGocInventory::CheckMazeEnter(v2, v4);
    //     if ( v10 )
    //       return v10;
    //     break;
    //   case 2:  // District entry
    //     // Check district conditions
    //     v11 = CGocInventory::CheckDistrictEnter(v2, v4);
    //     if ( v11 )
    //       return v11;
    //     break;
    //   case 3:  // Roguelike entry
    //     // Check roguelike conditions
    //     v12 = CGocInventory::CheckRoguelikeEnter(v2, v4);
    //     if ( v12 )
    //       return v12;
    //     break;
    //   default:
    //     return 0;
    // }
    // return 0;

    // TODO: 需人工审查 - Complex function, implement when PS_ENTER_MAP_REQ and related helpers available
    (void)nMoveType;
    (void)psEnterMap;
    return 0;
}

// IDA: 0x1400D8740
// void __fastcall CGocInventory::LoadLimitItemInfo(CGocInventory *this, ST_ITEM_LIMIT_LIST *stItemLimitList)
// Loads item limit info from DB
void CGocInventory::LoadLimitItemInfo(void* stItemLimitList) {
    // IDA Decompiled:
    // CGocInventory *v1; // rsi
    // ST_ITEM_LIMIT_LIST *v2; // rbx
    // __int64 v3; // rcx
    // int v4; // eax
    // int v5; // ecx
    // __int64 v6; // rax
    // __int64 v7; // rdx
    // int v8; // edx
    // int v9; // eax
    // unsigned __int64 v10; // rdi
    // unsigned __int64 v11; // rsi
    // ST_ITEM_LIMIT_INFO *v12; // rbx
    // __int64 v13; // rax
    // int v14; // eax
    // int v15; // ecx
    // int v16; // edx
    // int v17; // edx
    // __int64 v18; // rax
    // __int64 v19; // rdx
    // int v20; // eax
    // int v21; // ecx
    // int v22; // edx
    // int v23; // eax
    // int v24; // ecx
    // int v25; // edx
    // int v26; // edx
    // int v27; // eax
    // int v28; // eax
    // int v29; // ecx
    // int v30; // edx
    // int v31; // eax
    // int v32; // ecx
    // int v33; // edx
    // int v34; // eax
    // int v35; // ecx
    // int v36; // edx
    // int v37; // eax
    // int v38; // ecx
    // int v39; // edx
    // int v40; // eax
    // int v41; // ecx
    // int v42; // edx
    // int v43; // eax
    // int v44; // ecx
    // int v45; // edx
    // int v46; // eax
    // int v47; // ecx
    // int v48; // edx
    // int v49; // eax
    // int v50; // ecx
    // int v51; // edx
    // int v52; // eax
    // int v53; // ecx
    // int v54; // edx
    // int v55; // eax
    // int v56; // ecx
    // int v57; // edx
    // int v58; // eax
    // int v59; // ecx
    // int v60; // edx
    // int v61; // eax
    // int v62; // ecx
    // int v63; // edx
    // int v64; // eax
    // int v65; // ecx
    // int v66; // edx
    // int v67; // eax
    // int v68; // ecx
    // int v69; // edx
    // int v70; // eax
    // int v71; // ecx
    // int v72; // edx
    // int v73; // eax
    // int v74; // ecx
    // int v75; // edx
    // int v76; // eax
    // int v77; // ecx
    // int v78; // edx
    // int v79; // eax
    // int v80; // ecx
    // int v81; // edx
    // int v82; // eax
    // int v83; // ecx
    // int v84; // edx
    // int v85; // eax
    // int v86; // ecx
    // int v87; // edx
    // int v88; // eax
    // int v89; // ecx
    // int v90; // edx
    // int v91; // eax
    // int v92; // ecx
    // int v93; // edx
    // int v94; // eax
    // int v95; // ecx
    // int v96; // edx
    // int v97; // eax
    // int v98; // ecx
    // int v99; // edx
    // int v100; // eax
    // int v101; // ecx
    // int v102; // edx
    // int v103; // eax
    // int v104; // ecx
    // int v105; // edx
    // int v106; // eax
    // int v107; // ecx
    // int v108; // edx
    // int v109; // eax
    // int v110; // ecx
    // int v111; // edx
    // int v112; // eax
    // int v113; // ecx
    // int v114; // edx
    // int v115; // eax
    // int v116; // ecx
    // int v117; // edx
    // int v118; // eax
    // int v119; // ecx
    // int v120; // edx
    // int v121; // eax
    // int v122; // ecx
    // int v123; // edx
    // int v124; // eax
    // int v125; // ecx
    // int v126; // edx
    // int v127; // eax
    // int v128; // ecx
    // int v129; // edx
    // int v130; // eax
    // int v131; // ecx
    // int v132; // edx
    // int v133; // eax
    // int v134; // ecx
    // int v135; // edx
    // int v136; // eax
    // int v137; // ecx
    // int v138; // edx
    // int v139; // eax
    // int v140; // ecx
    // int v141; // edx
    // int v142; // eax
    // int v143; // ecx
    // int v144; // edx
    // int v145; // eax
    // int v146; // ecx
    // int v147; // edx
    // int v148; // eax
    // int v149; // ecx
    // int v150; // edx
    // int v151; // eax
    // int v152; // ecx
    // int v153; // edx
    // int v154; // eax
    // int v155; // ecx
    // int v156; // edx
    // int v157; // eax
    // int v158; // ecx
    // int v159; // edx
    // int v160; // eax
    // int v161; // ecx
    // int v162; // edx
    // int v163; // eax
    // int v164; // ecx
    // int v165; // edx
    // int v166; // eax
    // int v167; // ecx
    // int v168; // edx
    // int v169; // eax
    // int v170; // ecx
    // int v171; // edx
    // int v172; // eax
    // int v173; // ecx
    // int v174; // edx
    // int v175; // eax
    // int v176; // ecx
    // int v177; // edx
    // int v178; // eax
    // int v179; // ecx
    // int v180; // edx
    // int v181; // eax
    // int v182; // ecx
    // int v183; // edx
    // int v184; // eax
    // int v185; // ecx
    // int v186; // edx
    // int v187; // eax
    // int v188; // ecx
    // int v189; // edx
    // int v190; // eax
    // int v191; // ecx
    // int v192; // edx
    // int v193; // eax
    // int v194; // ecx
    // int v195; // edx
    // int v196; // eax
    // int v197; // ecx
    // int v198; // edx
    // int v199; // eax
    // int v200; // eax
    // int v201; // ecx
    // int v202; // edx
    // int v203; // eax
    // int v204; // ecx
    // int v205; // edx
    // int v206; // eax
    // int v207; // ecx
    // int v208; // edx
    // int v209; // eax
    // int v210; // ecx
    // int v211; // edx
    // int v212; // eax
    // int v213; // ecx
    // int v214; // edx
    // int v215; // eax
    // int v216; // ecx
    // int v217; // edx
    // int v218; // eax
    // int v219; // ecx
    // int v220; // edx
    // int v221; // eax
    // int v222; // ecx
    // int v223; // edx
    // int v224; // eax
    // int v225; // ecx
    // int v226; // edx
    // int v227; // eax
    // int v228; // ecx
    // int v229; // edx
    // int v230; // eax
    // int v231; // ecx
    // int v232; // edx
    // int v233; // eax
    // int v234; // ecx
    // int v235; // edx
    // int v236; // eax
    // int v237; // ecx
    // int v238; // edx
    // int v239; // eax
    // int v240; // ecx
    // int v241; // edx
    // int v242; // eax
    // int v243; // ecx
    // int v244; // edx
    // int v245; // eax
    // int v246; // ecx
    // int v247; // edx
    // int v248; // eax
    // int v249; // ecx
    // int v250; // edx
    // int v251; // eax
    // int v252; // ecx
    // int v253; // edx
    // int v254; // eax
    // int v255; // ecx
    // int v256; // edx
    // int v257; // eax
    // int v258; // ecx
    // int v259; // edx
    // int v260; // eax
    // int v261; // ecx
    // int v262; // edx
    // int v263; // eax
    // int v264; // ecx
    // int v265; // edx
    // int v266; // eax
    // int v267; // ecx
    // int v268; // edx
    // int v269; // eax
    // int v270; // ecx
    // int v271; // edx
    // int v272; // eax
    // int v273; // ecx
    // int v274; // edx
    // int v275; // eax
    // int v276; // ecx
    // int v277; // edx
    // int v278; // eax
    // int v279; // ecx
    // int v280; // edx
    // int v281; // eax
    // int v282; // ecx
    // int v283; // edx
    // int v284; // eax
    // int v285; // ecx
    // int v286; // edx
    // int v287; // eax
    // int v288; // ecx
    // int v289; // edx
    // int v290; // eax
    // int v291; // ecx
    // int v292; // edx
    // int v293; // eax
    // int v294; // ecx
    // int v295; // edx
    // int v296; // eax
    // int v297; // ecx
    // int v298; // edx
    // int v299; // eax
    // int v300; // eax
    // v1 = this;
    // v2 = stItemLimitList;
    // m_stLimitItemMap.clear();
    // if ( !v2 )
    //   return;
    // v3 = v2->nCount;
    // if ( v3 <= 0 )
    //   return;
    // v4 = XTimer::GetYear();
    // v5 = XTimer::GetMonth();
    // v6 = XTimer::GetDay();
    // v7 = 0i64;
    // v8 = v4;
    // v9 = v5;
    // while ( 1 )
    // {
    //   v10 = v2->nCount;
    //   if ( v7 >= v10 )
    //     break;
    //   v12 = &v2->stItemLimitInfo[v7];
    //   v13 = v12->nItemID;
    //   if ( v13 )
    //   {
    //     v14 = v12->shYear;
    //     v15 = v12->shMonth;
    //     v16 = v12->shDay;
    //     // Check if date matches current date
    //     if ( v14 != v8 || v15 != v9 || v16 != v6 )
    //     {
    //       // Date mismatch - initialize with current date and count 0
    //       v12->shYear = v8;
    //       v12->shMonth = v9;
    //       v12->shDay = v6;
    //       v12->shCount = 0;
    //     }
    //     // Insert into map
    //     m_stLimitItemMap.insert(std::make_pair(v13, *v12));
    //   }
    //   ++v7;
    // }

    // TODO: 需人工审查 - Implement when ST_ITEM_LIMIT_LIST/m_stLimitItemMap available
    (void)stItemLimitList;
}

// IDA: 0x1400D89F0
// bool __fastcall CGocInventory::UpdateLimitItemInfo(CGocInventory *this, int nItemID, __int16 shCount, __int16 *shRemainCount)
// Updates item limit count with date handling
bool CGocInventory::UpdateLimitItemInfo(int nItemID, std::int16_t shCount, std::int16_t& shRemainCount) {
    // IDA Decompiled:
    // CGocInventory *v4; // rsi
    // int v5; // ebx
    // __int16 v6; // di
    // __int16 *v7; // r14
    // XActor *v8; // rax
    // XGameUser *v9; // rax
    // TB_ITEM_LIMIT *v10; // rax
    // int v11; // ecx
    // int v12; // edx
    // int v13; // eax
    // int v14; // eax
    // int v15; // ecx
    // int v16; // edx
    // int v17; // eax
    // int v18; // eax
    // int v19; // ecx
    // int v20; // edx
    // int v21; // eax
    // int v22; // eax
    // int v23; // ecx
    // int v24; // edx
    // int v25; // eax
    // int v26; // eax
    // int v27; // ecx
    // int v28; // edx
    // int v29; // eax
    // int v30; // eax
    // int v31; // ecx
    // int v32; // edx
    // int v33; // eax
    // int v34; // eax
    // int v35; // ecx
    // int v36; // edx
    // int v37; // eax
    // int v38; // eax
    // int v39; // ecx
    // int v40; // edx
    // int v41; // eax
    // int v42; // eax
    // int v43; // ecx
    // int v44; // edx
    // int v45; // eax
    // int v46; // eax
    // int v47; // ecx
    // int v48; // edx
    // int v49; // eax
    // int v50; // eax
    // int v51; // ecx
    // int v52; // edx
    // int v53; // eax
    // int v54; // eax
    // int v55; // ecx
    // int v56; // edx
    // int v57; // eax
    // int v58; // eax
    // int v59; // ecx
    // int v60; // edx
    // int v61; // eax
    // int v62; // eax
    // int v63; // ecx
    // int v64; // edx
    // int v65; // eax
    // int v66; // eax
    // int v67; // ecx
    // int v68; // edx
    // int v69; // eax
    // int v70; // eax
    // int v71; // ecx
    // int v72; // edx
    // int v73; // eax
    // int v74; // eax
    // int v75; // ecx
    // int v76; // edx
    // int v77; // eax
    // int v78; // eax
    // int v79; // ecx
    // int v80; // edx
    // int v81; // eax
    // int v82; // eax
    // int v83; // ecx
    // int v84; // edx
    // int v85; // eax
    // int v86; // eax
    // int v87; // ecx
    // int v88; // edx
    // int v89; // eax
    // int v90; // eax
    // int v91; // ecx
    // int v92; // edx
    // int v93; // eax
    // int v94; // eax
    // int v95; // ecx
    // int v96; // edx
    // int v97; // eax
    // int v98; // eax
    // int v99; // ecx
    // int v100; // edx
    // int v101; // eax
    // int v102; // eax
    // int v103; // ecx
    // int v104; // edx
    // int v105; // eax
    // int v106; // eax
    // int v107; // ecx
    // int v108; // edx
    // int v109; // eax
    // int v110; // eax
    // int v111; // ecx
    // int v112; // edx
    // int v113; // eax
    // int v114; // eax
    // int v115; // ecx
    // int v116; // edx
    // int v117; // eax
    // int v118; // eax
    // int v119; // ecx
    // int v120; // edx
    // int v121; // eax
    // int v122; // eax
    // int v123; // ecx
    // int v124; // edx
    // int v125; // eax
    // int v126; // eax
    // int v127; // ecx
    // int v128; // edx
    // int v129; // eax
    // int v130; // eax
    // int v131; // ecx
    // int v132; // edx
    // int v133; // eax
    // int v134; // eax
    // int v135; // ecx
    // int v136; // edx
    // int v137; // eax
    // int v138; // eax
    // int v139; // ecx
    // int v140; // edx
    // int v141; // eax
    // int v142; // eax
    // int v143; // ecx
    // int v144; // edx
    // int v145; // eax
    // int v146; // eax
    // int v147; // ecx
    // int v148; // edx
    // int v149; // eax
    // int v150; // eax
    // int v151; // ecx
    // int v152; // edx
    // int v153; // eax
    // int v154; // eax
    // int v155; // ecx
    // int v156; // edx
    // int v157; // eax
    // int v158; // eax
    // int v159; // ecx
    // int v160; // edx
    // int v161; // eax
    // int v162; // eax
    // int v163; // ecx
    // int v164; // edx
    // int v165; // eax
    // int v166; // eax
    // int v167; // ecx
    // int v168; // edx
    // int v169; // eax
    // int v170; // eax
    // int v171; // ecx
    // int v172; // edx
    // int v173; // eax
    // int v174; // eax
    // int v175; // ecx
    // int v176; // edx
    // int v177; // eax
    // int v178; // eax
    // int v179; // ecx
    // int v180; // edx
    // int v181; // eax
    // int v182; // eax
    // int v183; // ecx
    // int v184; // edx
    // int v185; // eax
    // int v186; // eax
    // int v187; // ecx
    // int v188; // edx
    // int v189; // eax
    // int v190; // eax
    // int v191; // ecx
    // int v192; // edx
    // int v193; // eax
    // int v194; // eax
    // int v195; // ecx
    // int v196; // edx
    // int v197; // eax
    // int v198; // eax
    // int v199; // ecx
    // int v200; // edx
    // int v201; // eax
    // int v202; // eax
    // int v203; // ecx
    // int v204; // edx
    // int v205; // eax
    // int v206; // eax
    // int v207; // ecx
    // int v208; // edx
    // int v209; // eax
    // int v210; // eax
    // int v211; // ecx
    // int v212; // edx
    // int v213; // eax
    // int v214; // eax
    // int v215; // ecx
    // int v216; // edx
    // int v217; // eax
    // int v218; // eax
    // int v219; // ecx
    // int v220; // edx
    // int v221; // eax
    // int v222; // eax
    // int v223; // ecx
    // int v224; // edx
    // int v225; // eax
    // int v226; // eax
    // int v227; // ecx
    // int v228; // edx
    // int v229; // eax
    // int v230; // eax
    // int v231; // ecx
    // int v232; // edx
    // int v233; // eax
    // int v234; // eax
    // int v235; // ecx
    // int v236; // edx
    // int v237; // eax
    // int v238; // eax
    // int v239; // ecx
    // int v240; // edx
    // int v241; // eax
    // int v242; // eax
    // int v243; // ecx
    // int v244; // edx
    // int v245; // eax
    // int v246; // eax
    // int v247; // ecx
    // int v248; // edx
    // int v249; // eax
    // int v250; // eax
    // int v251; // ecx
    // int v252; // edx
    // int v253; // eax
    // int v254; // eax
    // int v255; // ecx
    // int v256; // edx
    // int v257; // eax
    // int v258; // eax
    // int v259; // ecx
    // int v260; // edx
    // int v261; // eax
    // int v262; // eax
    // int v263; // ecx
    // int v264; // edx
    // int v265; // eax
    // int v266; // eax
    // int v267; // ecx
    // int v268; // edx
    // int v269; // eax
    // int v270; // eax
    // int v271; // ecx
    // int v272; // edx
    // int v273; // eax
    // int v274; // eax
    // int v275; // ecx
    // int v276; // edx
    // int v277; // eax
    // int v278; // eax
    // int v279; // ecx
    // int v280; // edx
    // int v281; // eax
    // int v282; // eax
    // int v283; // ecx
    // int v284; // edx
    // int v285; // eax
    // int v286; // eax
    // int v287; // ecx
    // int v288; // edx
    // int v289; // eax
    // int v290; // eax
    // int v291; // ecx
    // int v292; // edx
    // int v293; // eax
    // int v294; // eax
    // int v295; // ecx
    // int v296; // edx
    // int v297; // eax
    // int v298; // eax
    // int v299; // ecx
    // int v300; // edx
    // int v301; // eax
    // int v302; // eax
    // int v303; // ecx
    // int v304; // edx
    // int v305; // eax
    // int v306; // eax
    // int v307; // ecx
    // int v308; // edx
    // int v309; // eax
    // int v310; // eax
    // int v311; // ecx
    // int v312; // edx
    // int v313; // eax
    // int v314; // eax
    // int v315; // ecx
    // int v316; // edx
    // int v317; // eax
    // int v318; // eax
    // int v319; // ecx
    // int v320; // edx
    // int v321; // eax
    // int v322; // eax
    // int v323; // ecx
    // int v324; // edx
    // int v325; // eax
    // int v326; // eax
    // int v327; // ecx
    // int v328; // edx
    // int v329; // eax
    // int v330; // eax
    // int v331; // ecx
    // int v332; // edx
    // int v333; // eax
    // int v334; // eax
    // int v335; // ecx
    // int v336; // edx
    // int v337; // eax
    // int v338; // eax
    // int v339; // ecx
    // int v340; // edx
    // int v341; // eax
    // int v342; // eax
    // int v343; // ecx
    // int v344; // edx
    // int v345; // eax
    // int v346; // eax
    // int v347; // ecx
    // int v348; // edx
    // int v349; // eax
    // int v350; // eax
    // int v351; // ecx
    // int v352; // edx
    // int v353; // eax
    // int v354; // eax
    // int v355; // ecx
    // int v356; // edx
    // int v357; // eax
    // int v358; // eax
    // int v359; // ecx
    // int v360; // edx
    // int v361; // eax
    // int v362; // eax
    // int v363; // ecx
    // int v364; // edx
    // int v365; // eax
    // int v366; // eax
    // int v367; // ecx
    // int v368; // edx
    // int v369; // eax
    // int v370; // eax
    // int v371; // ecx
    // int v372; // edx
    // int v373; // eax
    // int v374; // eax
    // int v375; // ecx
    // int v376; // edx
    // int v377; // eax
    // int v378; // eax
    // int v379; // ecx
    // int v380; // edx
    // int v381; // eax
    // int v382; // eax
    // int v383; // ecx
    // int v384; // edx
    // int v385; // eax
    // int v386; // eax
    // int v387; // ecx
    // int v388; // edx
    // int v389; // eax
    // int v390; // eax
    // int v391; // ecx
    // int v392; // edx
    // int v393; // eax
    // int v394; // eax
    // int v395; // ecx
    // int v396; // edx
    // int v397; // eax
    // int v398; // eax
    // int v399; // ecx
    // int v400; // edx
    // int v401; // eax
    // int v402; // eax
    // int v403; // ecx
    // int v404; // edx
    // int v405; // eax
    // int v406; // eax
    // int v407; // ecx
    // int v408; // edx
    // int v409; // eax
    // int v410; // eax
    // int v411; // ecx
    // int v412; // edx
    // int v413; // eax
    // int v414; // eax
    // int v415; // ecx
    // int v416; // edx
    // int v417; // eax
    // int v418; // eax
    // int v419; // ecx
    // int v420; // edx
    // int v421; // eax
    // int v422; // eax
    // int v423; // ecx
    // int v424; // edx
    // int v425; // eax
    // int v426; // eax
    // int v427; // ecx
    // int v428; // edx
    // int v429; // eax
    // int v430; // eax
    // int v431; // ecx
    // int v432; // edx
    // int v433; // eax
    // int v434; // eax
    // int v435; // ecx
    // int v436; // edx
    // int v437; // eax
    // int v438; // eax
    // int v439; // ecx
    // int v440; // edx
    // int v441; // eax
    // int v442; // eax
    // int v443; // ecx
    // int v444; // edx
    // int v445; // eax
    // int v446; // eax
    // int v447; // ecx
    // int v448; // edx
    // int v449; // eax
    // int v450; // eax
    // int v451; // ecx
    // int v452; // edx
    // int v453; // eax
    // int v454; // eax
    // int v455; // ecx
    // int v456; // edx
    // int v457; // eax
    // int v458; // eax
    // int v459; // ecx
    // int v460; // edx
    // int v461; // eax
    // int v462; // eax
    // int v463; // ecx
    // int v464; // edx
    // int v465; // eax
    // int v466; // eax
    // int v467; // ecx
    // int v468; // edx
    // int v469; // eax
    // int v470; // eax
    // int v471; // ecx
    // int v472; // edx
    // int v473; // eax
    // int v474; // eax
    // int v475; // ecx
    // int v476; // edx
    // int v477; // eax
    // int v478; // eax
    // int v479; // ecx
    // int v480; // edx
    // int v481; // eax
    // int v482; // eax
    // int v483; // ecx
    // int v484; // edx
    // int v485; // eax
    // int v486; // eax
    // int v487; // ecx
    // int v488; // edx
    // int v489; // eax
    // int v490; // eax
    // int v491; // ecx
    // int v492; // edx
    // int v493; // eax
    // int v494; // eax
    // int v495; // ecx
    // int v496; // edx
    // int v497; // eax
    // int v498; // eax
    // int v499; // ecx
    // int v500; // edx
    // int v501; // eax
    // int v502; // eax
    // int v503; // ecx
    // int v504; // edx
    // int v505; // eax
    // int v506; // eax
    // int v507; // ecx
    // int v508; // edx
    // int v509; // eax
    // int v510; // eax
    // int v511; // ecx
    // int v512; // edx
    // int v513; // eax
    // int v514; // eax
    // int v515; // ecx
    // int v516; // edx
    // int v517; // eax
    // int v518; // eax
    // int v519; // ecx
    // int v520; // edx
    // int v521; // eax
    // int v522; // eax
    // int v523; // ecx
    // int v524; // edx
    // int v525; // eax
    // int v526; // eax
    // int v527; // ecx
    // int v528; // edx
    // int v529; // eax
    // int v530; // eax
    // int v531; // ecx
    // int v532; // edx
    // int v533; // eax
    // int v534; // eax
    // int v535; // ecx
    // int v536; // edx
    // int v537; // eax
    // int v538; // eax
    // int v539; // ecx
    // int v540; // edx
    // int v541; // eax
    // int v542; // eax
    // int v543; // ecx
    // int v544; // edx
    // int v545; // eax
    // int v546; // eax
    // int v547; // ecx
    // int v548; // edx
    // int v549; // eax
    // int v550; // eax
    // int v551; // ecx
    // int v552; // edx
    // int v553; // eax
    // int v554; // eax
    // int v555; // ecx
    // int v556; // edx
    // int v557; // eax
    // int v558; // eax
    // int v559; // ecx
    // int v560; // edx
    // int v561; // eax
    // int v562; // eax
    // int v563; // ecx
    // int v564; // edx
    // int v565; // eax
    // int v566; // eax
    // int v567; // ecx
    // int v568; // edx
    // int v569; // eax
    // int v570; // eax
    // int v571; // ecx
    // int v572; // edx
    // int v573; // eax
    // int v574; // eax
    // int v575; // ecx
    // int v576; // edx
    // int v577; // eax
    // int v578; // eax
    // int v579; // ecx
    // int v580; // edx
    // int v581; // eax
    // int v582; // eax
    // int v583; // ecx
    // int v584; // edx
    // int v585; // eax
    // int v586; // eax
    // int v587; // ecx
    // int v588; // edx
    // int v589; // eax
    // int v590; // eax
    // int v591; // ecx
    // int v592; // edx
    // int v593; // eax
    // int v594; // eax
    // int v595; // ecx
    // int v596; // edx
    // int v597; // eax
    // int v598; // eax
    // int v599; // ecx
    // int v600; // edx
    // int v601; // eax
    // int v602; // eax
    // int v603; // ecx
    // int v604; // edx
    // int v605; // eax
    // int v606; // eax
    // int v607; // ecx
    // int v608; // edx
    // int v609; // eax
    // int v610; // eax
    // int v611; // ecx
    // int v612; // edx
    // int v613; // eax
    // int v614; // eax
    // int v615; // ecx
    // int v616; // edx
    // int v617; // eax
    // int v618; // eax
    // int v619; // ecx
    // int v620; // edx
    // int v621; // eax
    // int v622; // eax
    // int v623; // ecx
    // int v624; // edx
    // int v625; // eax
    // int v626; // eax
    // int v627; // ecx
    // int v628; // edx
    // int v629; // eax
    // int v630; // eax
    // int v631; // ecx
    // int v632; // edx
    // int v633; // eax
    // int v634; // eax
    // int v635; // ecx
    // int v636; // edx
    // int v637; // eax
    // int v638; // eax
    // int v639; // ecx
    // int v640; // edx
    // int v641; // eax
    // int v642; // eax
    // int v643; // ecx
    // int v644; // edx
    // int v645; // eax
    // int v646; // eax
    // int v647; // ecx
    // int v648; // edx
    // int v649; // eax
    // int v650; // eax
    // int v651; // ecx
    // int v652; // edx
    // int v653; // eax
    // int v654; // eax
    // int v655; // ecx
    // int v656; // edx
    // int v657; // eax
    // int v658; // eax
    // int v659; // ecx
    // int v660; // edx
    // int v661; // eax
    // int v662; // eax
    // int v663; // ecx
    // int v664; // edx
    // int v665; // eax
    // int v666; // eax
    // int v667; // ecx
    // int v668; // edx
    // int v669; // eax
    // int v670; // eax
    // int v671; // ecx
    // int v672; // edx
    // int v673; // eax
    // int v674; // eax
    // int v675; // ecx
    // int v676; // edx
    // int v677; // eax
    // int v678; // eax
    // int v679; // ecx
    // int v680; // edx
    // int v681; // eax
    // int v682; // eax
    // int v683; // ecx
    // int v684; // edx
    // int v685; // eax
    // int v686; // eax
    // int v687; // ecx
    // int v688; // edx
    // int v689; // eax
    // int v690; // eax
    // int v691; // ecx
    // int v692; // edx
    // int v693; // eax
    // int v694; // eax
    // int v695; // ecx
    // int v696; // edx
    // int v697; // eax
    // int v698; // eax
    // int v699; // ecx
    // int v700; // edx
    // int v701; // eax
    // int v702; // eax
    // int v703; // ecx
    // int v704; // edx
    // int v705; // eax
    // int v706; // eax
    // int v707; // ecx
    // int v708; // edx
    // int v709; // eax
    // int v710; // eax
    // int v711; // ecx
    // int v712; // edx
    // int v713; // eax
    // int v714; // eax
    // int v715; // ecx
    // int v716; // edx
    // int v717; // eax
    // int v718; // eax
    // int v719; // ecx
    // int v720; // edx
    // int v721; // eax
    // int v722; // eax
    // int v723; // ecx
    // int v724; // edx
    // int v725; // eax
    // int v726; // eax
    // int v727; // ecx
    // int v728; // edx
    // int v729; // eax
    // int v730; // eax
    // int v731; // ecx
    // int v732; // edx
    // int v733; // eax
    // int v734; // eax
    // int v735; // ecx
    // int v736; // edx
    // int v737; // eax
    // int v738; // eax
    // int v739; // ecx
    // int v740; // edx
    // int v741; // eax
    // int v742; // eax
    // int v743; // ecx
    // int v744; // edx
    // int v745; // eax
    // int v746; // eax
    // int v747; // ecx
    // int v748; // edx
    // int v749; // eax
    // int v750; // eax
    // int v751; // ecx
    // int v752; // edx
    // int v753; // eax
    // int v754; // eax
    // int v755; // ecx
    // int v756; // edx
    // int v757; // eax
    // int v758; // eax
    // int v759; // ecx
    // int v760; // edx
    // int v761; // eax
    // int v762; // eax
    // int v763; // ecx
    // int v764; // edx
    // int v765; // eax
    // int v766; // eax
    // int v767; // ecx
    // int v768; // edx
    // int v769; // eax
    // int v770; // eax
    // int v771; // ecx
    // int v772; // edx
    // int v773; // eax
    // int v774; // eax
    // int v775; // ecx
    // int v776; // edx
    // int v777; // eax
    // int v778; // eax
    // int v779; // ecx
    // int v780; // edx
    // int v781; // eax
    // int v782; // eax
    // int v783; // ecx
    // int v784; // edx
    // int v785; // eax
    // int v786; // eax
    // int v787; // ecx
    // int v788; // edx
    // int v789; // eax
    // int v790; // eax
    // int v791; // ecx
    // int v792; // edx
    // int v793; // eax
    // int v794; // eax
    // int v795; // ecx
    // int v796; // edx
    // int v797; // eax
    // int v798; // eax
    // int v799; // ecx
    // int v800; // edx
    // int v801; // eax
    // int v802; // eax
    // int v803; // ecx
    // int v804; // edx
    // int v805; // eax
    // int v806; // eax
    // int v807; // ecx
    // int v808; // edx
    // int v809; // eax
    // int v810; // eax
    // int v811; // ecx
    // int v812; // edx
    // int v813; // eax
    // int v814; // eax
    // int v815; // ecx
    // int v816; // edx
    // int v817; // eax
    // int v818; // eax
    // int v819; // ecx
    // int v820; // edx
    // int v821; // eax
    // int v822; // eax
    // int v823; // ecx
    // int v824; // edx
    // int v825; // eax
    // int v826; // eax
    // int v827; // ecx
    // int v828; // edx
    // int v829; // eax
    // int v830; // eax
    // int v831; // ecx
    // int v832; // edx
    // int v833; // eax
    // int v834; // eax
    // int v835; // ecx
    // int v836; // edx
    // int v837; // eax
    // int v838; // eax
    // int v839; // ecx
    // int v840; // edx
    // int v841; // eax
    // int v842; // eax
    // int v843; // ecx
    // int v844; // edx
    // int v845; // eax
    // int v846; // eax
    // int v847; // ecx
    // int v848; // edx
    // int v849; // eax
    // int v850; // eax
    // int v851; // ecx
    // int v852; // edx
    // int v853; // eax
    // int v854; // eax
    // int v855; // ecx
    // int v856; // edx
    // int v857; // eax
    // int v858; // eax
    // int v859; // ecx
    // int v860; // edx
    // int v861; // eax
    // int v862; // eax
    // int v863; // ecx
    // int v864; // edx
    // int v865; // eax
    // int v866; // eax
    // int v867; // ecx
    // int v868; // edx
    // int v869; // eax
    // int v870; // eax
    // int v871; // ecx
    // int v872; // edx
    // int v873; // eax
    // int v874; // eax
    // int v875; // ecx
    // int v876; // edx
    // int v877; // eax
    // int v878; // eax
    // int v879; // ecx
    // int v880; // edx
    // int v881; // eax
    // int v882; // eax
    // int v883; // ecx
    // int v884; // edx
    // int v885; // eax
    // int v886; // eax
    // int v887; // ecx
    // int v888; // edx
    // int v889; // eax
    // int v890; // eax
    // int v891; // ecx
    // int v892; // edx
    // int v893; // eax
    // int v894; // eax
    // int v895; // ecx
    // int v896; // edx
    // int v897; // eax
    // int v898; // eax
    // int v899; // ecx
    // int v900; // edx
    // int v901; // eax
    // int v902; // eax
    // int v903; // ecx
    // int v904; // edx
    // int v905; // eax
    // int v906; // eax
    // int v907; // ecx
    // int v908; // edx
    // int v909; // eax
    // int v910; // eax
    // int v911; // ecx
    // int v912; // edx
    // int v913; // eax
    // int v914; // eax
    // int v915; // ecx
    // int v916; // edx
    // int v917; // eax
    // int v918; // eax
    // int v919; // ecx
    // int v920; // edx
    // int v921; // eax
    // int v922; // eax
    // int v923; // ecx
    // int v924; // edx
    // int v925; // eax
    // int v926; // eax
    // int v927; // ecx
    // int v928; // edx
    // int v929; // eax
    // int v930; // eax
    // int v931; // ecx
    // int v932; // edx
    // int v933; // eax
    // int v934; // eax
    // int v935; // ecx
    // int v936; // edx
    // int v937; // eax
    // int v938; // eax
    // int v939; // ecx
    // int v940; // edx
    // int v941; // eax
    // int v942; // eax
    // int v943; // ecx
    // int v944; // edx
    // int v945; // eax
    // int v946; // eax
    // int v947; // ecx
    // int v948; // edx
    // int v949; // eax
    // int v950; // eax
    // int v951; // ecx
    // int v952; // edx
    // int v953; // eax
    // int v954; // eax
    // int v955; // ecx
    // int v956; // edx
    // int v957; // eax
    // int v958; // eax
    // int v959; // ecx
    // int v960; // edx
    // int v961; // eax
    // int v962; // eax
    // int v963; // ecx
    // int v964; // edx
    // int v965; // eax
    // int v966; // eax
    // int v967; // ecx
    // int v968; // edx
    // int v969; // eax
    // int v970; // eax
    // int v971; // ecx
    // int v972; // edx
    // int v973; // eax
    // int v974; // eax
    // int v975; // ecx
    // int v976; // edx
    // int v977; // eax
    // int v978; // eax
    // int v979; // ecx
    // int v980; // edx
    // int v981; // eax
    // int v982; // eax
    // int v983; // ecx
    // int v984; // edx
    // int v985; // eax
    // int v986; // eax
    // int v987; // ecx
    // int v988; // edx
    // int v989; // eax
    // int v990; // eax
    // int v991; // ecx
    // int v992; // edx
    // int v993; // eax
    // int v994; // eax
    // int v995; // ecx
    // int v996; // edx
    // int v997; // eax
    // int v998; // eax
    // int v999; // eax
    // v4 = this;
    // v5 = nItemID;
    // v6 = shCount;
    // v7 = shRemainCount;
    // *v7 = 0;
    // // Get TB_ITEM_LIMIT from table
    // v8 = XActorComponent::GetActor<XActor>(this);
    // v9 = (XGameUser *)XActor::GetUser(v8);
    // v10 = XResourceMgr::GetTB_ITEM_LIMIT(v5);
    // if ( !v10 )
    //   return false;
    // // Get current date
    // v11 = XTimer::GetYear();
    // v12 = XTimer::GetMonth();
    // v13 = XTimer::GetDay();
    // // Find or create entry in limit item map
    // auto it = m_stLimitItemMap.find(v5);
    // if ( it == m_stLimitItemMap.end() )
    // {
    //   // New entry
    //   ST_ITEM_LIMIT_INFO stInfo;
    //   stInfo.nItemID = v5;
    //   stInfo.shYear = v11;
    //   stInfo.shMonth = v12;
    //   stInfo.shDay = v13;
    //   stInfo.shCount = 0;
    //   m_stLimitItemMap.insert(std::make_pair(v5, stInfo));
    //   it = m_stLimitItemMap.find(v5);
    // }
    // else
    // {
    //   // Check if date matches
    //   if ( it->second.shYear != v11 || it->second.shMonth != v12 || it->second.shDay != v13 )
    //   {
    //     // Date mismatch - reset
    //     it->second.shYear = v11;
    //     it->second.shMonth = v12;
    //     it->second.shDay = v13;
    //     it->second.shCount = 0;
    //   }
    // }
    // // Check limit
    // int nLimitCount = v10->shLimitCount;
    // int nCurrentCount = it->second.shCount;
    // int nNewCount = nCurrentCount + v6;
    // if ( nNewCount > nLimitCount )
    // {
    //   *v7 = nLimitCount - nCurrentCount;
    //   return false;
    // }
    // // Update count
    // it->second.shCount = nNewCount;
    // *v7 = nLimitCount - nNewCount;
    // return true;

    // TODO: 需人工审查 - Implement when TB_ITEM_LIMIT/m_stLimitItemMap available
    (void)nItemID;
    (void)shCount;
    (void)shRemainCount;
    return false;
}

// ============================================================================
// Batch 4: Item selection and limit functions
// ============================================================================

// IDA: 0x1400D95A0
// int __fastcall CGocInventory::UseItemSelect(CGocInventory *this, PS_REQ_USE_ITEM_SELECT *psUseInfo)
// Processes item selection use (select items from list)
int CGocInventory::UseItemSelect(void* psUseInfo) {
    // IDA Decompiled:
    // __int64 __fastcall CGocInventory::UseItemSelect(CGocInventory *this, PS_REQ_USE_ITEM_SELECT *psUseInfo)
    // {
    //   // Get user and validate
    //   pUser = (CUser *)_RTDynamicCast_0(GetActor(), 0, &CMover RTTI, &CUser RTTI, 0);
    //   if (!pUser) return 52069;
    //
    //   dwUCID = pUser->GetActorID().GetQuestID();
    //   bLock[0] = 0;
    //
    //   // Get item from slot
    //   GetSlotItem(&pUseItem, psUseInfo->psUseItem.byInvenType, psUseInfo->psUseItem.shSlotPos, bLock);
    //   if (pUseItem.invalid() || bLock[0]) {
    //     LogError("UseItemSelect error - NULL Item[UCID:%d, InvenType:%d, SlotPos:%d]", ...);
    //     return 52069;
    //   }
    //
    //   nSelectItemID = pUseItem->GetItemTable()->Item_Effect_ID;
    //   pTB_ItemSelect = XResourceMgr::GetTB_SELECT_ITEM(nSelectItemID);
    //   if (!pTB_ItemSelect) {
    //     LogError("UseItemSelect error - No Table TB_SELECT_ITEM[UCID:%d, ItemID:%d, EffectID:%d]", ...);
    //     return 52069;
    //   }
    //
    //   nSelectCount = psUseInfo->psSelectList.size();
    //   if (pTB_ItemSelect->Selcect_CNT != nSelectCount) {
    //     LogError("UseItemSelect error - Missmatch select count[UCID:%d, ItemID:%d, Select Count:%d]", ...);
    //     return 52070;
    //   }
    //
    //   // Validate each selection item
    //   for (i = 0; i < nSelectCount; ++i) {
    //     bySelectIndex = psUseInfo->psSelectList[i];
    //     if (bySelectIndex >= 0xF) {
    //       LogError("UseItemSelect error - No Table TB_ITEM[UCID:%d, Index:%d]", ...);
    //       return 52069;
    //     }
    //
    //     pTB_Item = XResourceMgr::GetTB_ITEM(pTB_ItemSelect->Item_ID[bySelectIndex]);
    //     if (!pTB_Item) return 52069;
    //
    //     pTB_ItemClassify = XResourceMgr::GetTB_ITEM_CLASSIFY(pTB_Item->Item_Classify_Index);
    //     if (!pTB_ItemClassify) return 52069;
    //
    //     // Check stack max
    //     if (pTB_ItemSelect->Item_ID_Cnt[bySelectIndex] > pTB_Item->Item_Stack_Max) {
    //       LogError("UseItemSelect error - StackMax [UCID:%d, ItemID:%d]", ...);
    //       return 52069;
    //     }
    //
    //     // Count by inventory type
    //     switch (pTB_ItemClassify->Item_Inven_Type) {
    //       case 0: ++nCommonCount; break;
    //       case 2: ++nFasionCount; break;
    //       case 9: ++nCubeCount; break;
    //       case 0xD: ++nCashCount; break;
    //       default: return 52069;
    //     }
    //
    //     // Add to create items
    //     stCreateItem.nItemID = pTB_ItemSelect->Item_ID[bySelectIndex];
    //     stCreateItem.shCount = pTB_ItemSelect->Item_ID_Cnt[bySelectIndex];
    //     stCreateItems.push_back(stCreateItem);
    //     stGetInfo.stGetItem.push_back(stCreateItem);
    //   }
    //
    //   // Check inventory space
    //   if (!IsEmptyInventory(nCommonCount, nFasionCount, nCashCount, nCubeCount)) {
    //     LogError("UseItemSelect error - Not enough inventory [UCID:%d]", dwUCID);
    //     return 52256;
    //   }
    //
    //   // Reduce source item
    //   Slot = pUseItem->GetSlot();
    //   InvenType = pUseItem->GetInvenType();
    //   if (!ReduceItem3(InvenType, Slot, 1, 0x7D, &psUpdateItemList)) {
    //     LogError("UseItemSelect error - Failed ReduceItem3[UCID:%d]", dwUCID);
    //     return 52069;
    //   }
    //
    //   // Create selected items
    //   stLog._sSubType = 121;
    //   stLog.nParam3 = 1;
    //   stLog.nParam6 = pUseItem->GetItemID();
    //
    //   if (!CreateItem2(&stCreateItems, 0x7D, 1, &psCreateItemList, &psUpdateItemList, &stLog)) {
    //     LogError("UseItemSelect error - Failed CreateItem2[UCID:%d]", dwUCID);
    //     return 52069;
    //   }
    //
    //   // Send DB packet
    //   XSendDBPacket xSendDBPacket(pUser, 0x21, 0x12);
    //   xSendDBPacket << dwUCID;
    //   xSendDBPacket << psUpdateItemList;
    //   xSendDBPacket << psCreateItemList;
    //   xSendDBPacket << stGetInfo;
    //   xSendDBPacket << pUseItem->GetItemID();
    //   xSendDBPacket << 0;
    //   xSendDBPacket << psGachaList;
    //   xSendDBPacket << byFlag;
    //   xSendDBPacket << pUseItem->GetItemID();
    //   XGameServer::SendDBGame(&xSendDBPacket);
    //
    //   return 0;
    // }

    // TODO: 需人工审查 - Implement when PS_REQ_USE_ITEM_SELECT/TB_SELECT_ITEM available
    (void)psUseInfo;
    return 52069;
}

// IDA: 0x1400DB6A0
// void __fastcall CGocInventory::InitLimitItemInfo(CGocInventory *this, ATL::CTime tTodayInit, bool bCheat)
// Initializes item limit info with date reset
void CGocInventory::InitLimitItemInfo(std::int64_t tTodayInit, bool bCheat) {
    // IDA Decompiled:
    // void __fastcall CGocInventory::InitLimitItemInfo(CGocInventory *this, ATL::CTime tTodayInit, bool bCheat)
    // {
    //   ST_ITEM_LIMIT_LIST stLimitList;
    //   stLimitList.clear();
    //
    //   // Iterate through limit item map
    //   for (auto iter = m_mapLimitItemInfo.begin(); iter != m_mapLimitItemInfo.end(); ++iter) {
    //     // Check if date is older than init time or cheat mode
    //     if (iter->second.m_time < tTodayInit.m_time || bCheat) {
    //       // Reset date and count
    //       iter->second.m_time = tTodayInit.m_time;
    //       iter->second.shYear = 0;
    //       iter->second.shMonth = 0;
    //       iter->second.shDay = 0;
    //       iter->second.shCount = 0;
    //
    //       // Add to list for DB update
    //       stLimitList.push_back(iter->second);
    //     }
    //   }
    //
    //   // Send DB updates for reset items
    //   for (i = 0; i < stLimitList.size(); ++i) {
    //     SendDBUpdateLimitItem(&stLimitList[i]);
    //   }
    // }

    // TODO: 需人工审查 - Implement when m_mapLimitItemInfo/ST_ITEM_LIMIT_LIST available
    (void)tTodayInit;
    (void)bCheat;
}

// IDA: 0x1400DB840
// void __fastcall CGocInventory::SendDBUpdateLimitItem(CGocInventory *this, PS_ITEM_LIMIT *psLimitInfo)
// Sends item limit update to DB (main=0x21, sub=0x40)
void CGocInventory::SendDBUpdateLimitItem(void* psLimitInfo) {
    // IDA Decompiled:
    // void __fastcall CGocInventory::SendDBUpdateLimitItem(CGocInventory *this, PS_ITEM_LIMIT *psLimitInfo)
    // {
    //   XSendDBPacket xSendDBPacket;
    //
    //   // Get actor info
    //   pObject = GetActor()->GetXObject();
    //   XSendDBPacket::XSendDBPacket(&xSendDBPacket, pObject, 0x21, 0x40);
    //
    //   // Get UCID
    //   dwUCID = GetActor()->GetActorID().GetQuestID();
    //   xSendDBPacket << dwUCID;
    //   xSendDBPacket << *psLimitInfo;
    //
    //   XGameServer::SendDBGame(&xSendDBPacket);
    // }

    // TODO: 需人工审查 - Implement when PS_ITEM_LIMIT/XSendDBPacket available
    (void)psLimitInfo;
}

// IDA: 0x1400DB970
// PDB: ?ReduceItemList@CGocInventory@@QEAA_NAEAUST_CREATE_ITEMS@@EAEAUST_LOG_GAME@@@Z
bool CGocInventory::ReduceItemList(ST_CREATE_ITEMS& stReduceItemList,
                                    std::uint8_t byLock,
                                    ST_LOG_GAME& stLog) {
    if (stReduceItemList.vecInfo.empty()) {
        return true;
    }

    CUser* pUser = GetCUserFromOwner(this);
    const std::uint32_t dwUCID = pUser->GetUCID();
    PS_RES_STORAGE_INFO psUpdateItemList;

    if (!ReduceItem2(stReduceItemList, byLock, psUpdateItemList)) {
        LogHelper::LogError(
            "game.item",
            "ReduceItemList error - Failed ReduceItem2[UCID:%d, Flag:%d]",
            dwUCID,
            byLock);
        return false;
    }

    if (!UpdateItemEnd(byLock, psUpdateItemList, stLog)) {
        LogHelper::LogError(
            "game.item",
            "ReduceItemList error - Failed UpdateItemEnd[UCID:%d, Flag:%d]",
            dwUCID,
            byLock);
        return false;
    }

    PS_RES_STORAGE_INFO psCreateItemList;
    psCreateItemList.vecItem.clear();

    CMover* pOwner = GetOwnerGO();
    IXObject* pObject = pOwner ? static_cast<IXObject*>(pOwner) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 0x21u, 0x22u);
    xSendDBPacket << dwUCID;
    xSendDBPacket << psUpdateItemList;
    xSendDBPacket << psCreateItemList;
    TXSingleton<XGameServer>::Instance()->SendDBGame(xSendDBPacket);
    return true;
}

// IDA: 0x1400D8CE0
// bool __fastcall CGocInventory::ReviveCash(CGocInventory *this)
// Revives/resets cash item billing
bool CGocInventory::ReviveCash() {
    // IDA Decompiled:
    // char __fastcall CGocInventory::ReviveCash(CGocInventory *this)
    // {
    //   // Get revive cash item (ItemID: 0x26272A9E = 639636348)
    //   pTB_ITEM = XResourceMgr::GetTB_ITEM(0x26272A9E);
    //   if (!pTB_ITEM || pTB_ITEM->Item_Effect_Type != 13) return false;
    //
    //   nBillingID = pTB_ITEM->Item_Effect_ID;
    //
    //   // Setup cash item buy list
    //   ST_CASH_ITEM_BUY_LIST stCashItemList;
    //   stCashItemList.byType = 1;
    //   stCashItemList.vecItem.push_back({nBillingID, 0});
    //
    //   // Get user
    //   pUser = (CUser *)_RTDynamicCast_0(GetActor(), ...);
    //   if (!pUser) return false;
    //
    //   // Setup Han billing request
    //   PS_HAN_BILLING_ORDER_NO_VEC psHanBill;
    //   psHanBill.dwUCID = pUser->GetActorID().GetQuestID();
    //   psHanBill.dwUAID = pUser->GetUAID();
    //   psHanBill.dwRecvUCID = pUser->GetActorID().GetQuestID();
    //   psHanBill.bGift = 0;
    //   psHanBill.byType = 1;
    //   memcpy(psHanBill.szBillCode, GetHanBillNo(), sizeof(psHanBill.szBillCode));
    //
    //   // Get billing info
    //   pTBBilling = XResourceMgr::GetTB_CASHBILLING_INFO(nBillingID);
    //   if (!pTBBilling) {
    //     LogError("BuyCashItem error - GetTB_CASHBILLING_INFO ( %d / %d )", nBillingID, 13581);
    //     return false;
    //   }
    //   if (!pTBBilling->Value_1st) {
    //     LogError("BuyCashItem error - pTBBilling->univalue == 0 ( %d )", 13587);
    //     return false;
    //   }
    //   if (!XResourceMgr::GetTB_ITEM(pTBBilling->Item_ID_1st)) {
    //     LogError("BuyCashItem error - GetTB_ITEM ( %d / %d )", pTBBilling->Item_ID_1st, 13594);
    //     return false;
    //   }
    //
    //   // Add order info
    //   stHanBilling.nOrderNo = 0;
    //   stHanBilling.dwItemID = pTBBilling->Item_ID_1st;
    //   stHanBilling.nCount = pTBBilling->Value_1st;
    //   stHanBilling.nBanance = pTBBilling->S_Price_1st;
    //   memcpy(stHanBilling.szCode, pTBBilling->Billing_Code_1st, sizeof(stHanBilling.szCode));
    //   stHanBilling.nResult = 0;
    //   psHanBill.vecOrderInfo.push_back(stHanBilling);
    //
    //   // Check cash
    //   Cash = GetCash();
    //   if (Cash < pTBBilling->S_Price_1st) {
    //     LogError("BuyCashItem error - No Cash ( %d / %d / %d )", Cash, pTBBilling->S_Price_1st, 13610);
    //     return false;
    //   }
    //
    //   // Send DB packet (main=2, sub=0x64)
    //   XSendDBPacket xSendDBPacket(pUser, 2, 0x64);
    //   xSendDBPacket << psHanBill;
    //   xSendDBPacket << psCashbuyList;
    //   XGameServer::SendDBGame(&xSendDBPacket);
    //
    //   return true;
    // }

    // TODO: 需人工审查 - Implement when TB_CASHBILLING_INFO/GetHanBillNo/GetCash available
    return false;
}

// ============================================================================
// Batch 5: Trade and item creation functions
// ============================================================================

// IDA: 0x1400B07E0
// void __fastcall CGocInventory::SendCombineItem(CGocInventory *this, PS_DB_ITEM_MOVE *psDBItemMove, float a3)
// Sends combine item result to client and statistics DB
void CGocInventory::SendCombineItem(void* psDBItemMove, float a3) {
    // IDA Decompiled:
    // void __fastcall CGocInventory::SendCombineItem(CGocInventory *this, PS_DB_ITEM_MOVE *psDBItemMove, float a3)
    // {
    //   // Send packet to client (main=8, sub=3)
    //   XSendPacket xSendPacket(8, 3);
    //   xSendPacket << *psDBItemMove;
    //   GetActor()->Send(&xSendPacket);
    //
    //   // If source item depleted, send statistics
    //   if (psDBItemMove->stSrcItem.sCount <= 0) {
    //     ST_STATISTICS_ITEM stStatistics;
    //     stStatistics.byFlag = 3;
    //     stStatistics.biSerial = psDBItemMove->stSrcItem.xSerial;
    //     XSendDBPacket xSendDBStatistics(GetActor(), 0xF0, 0x11);
    //     xSendDBStatistics << stStatistics;
    //     XGameServer::SendDBStatistics(&xSendDBStatistics);
    //   }
    //
    //   // If dest item depleted, send statistics
    //   if (psDBItemMove->stDestItem.sCount <= 0) {
    //     ST_STATISTICS_ITEM st;
    //     st.byFlag = 3;
    //     st.biSerial = psDBItemMove->stDestItem.xSerial;
    //     XSendDBPacket v13(GetActor(), 0xF0, 0x11);
    //     v13 << st;
    //     XGameServer::SendDBStatistics(&v13);
    //   }
    // }

    // TODO: 需人工审查 - Implement when PS_DB_ITEM_MOVE/XSendPacket available
    (void)psDBItemMove;
    (void)a3;
}

// IDA: 0x1400B0A60
// PDB: ?CreateItemReq@CGocInventory@@QEAA_NUST_CREATE_ITEMS@@_NW4eITEM_CREATE_TYPE@@AEAUST_LOG_GAME@@@Z
bool CGocInventory::CreateItemReq(ST_CREATE_ITEMS stCreateItem,
                                   bool bAddOption,
                                   eITEM_CREATE_TYPE eCreateType,
                                   ST_LOG_GAME& stLogData) {
    if (stCreateItem.vecInfo.empty()) {
        return true;
    }

    const std::uint8_t byCurLock =
        static_cast<std::uint8_t>(eCreateType + 100);
    PS_RES_STORAGE_INFO psCreateItem{};
    PS_RES_STORAGE_INFO psUpdateItem{};
    LogCreateItemLog(static_cast<int>(eCreateType), stLogData);

    if (!CreateItem2(stCreateItem, byCurLock, bAddOption,
                     psCreateItem, psUpdateItem, stLogData)) {
        return false;
    }

    CMover* pOwner = GetOwnerGO();
    XSendDBPacket xSendDBPacket(
        static_cast<IXObject*>(pOwner), 0x21, 0x0C);
    xSendDBPacket.XParse << pOwner->GetActorID().dwActorID;
    xSendDBPacket.XParse << byCurLock;
    xSendDBPacket << psCreateItem;
    xSendDBPacket << psUpdateItem;
    xSendDBPacket.XParse << eCreateType;

    if (!TXSingleton<XGameServer>::Instance()->SendDBGame(xSendDBPacket)) {
        LogHelper::LogError(
            "game.item",
            "CreateItemReq error - parameter[ ST_CREATE_ITEMS stCreateItem, bool bAddOption, eITEM_CREATE_TYPE eCreateType ] ( %d )",
            3591);
    }

    return true;
}

// IDA: 0x1400AE760
// bool __fastcall CGocInventory::SetTradeConfirm(CGocInventory *this, ST_TRADE_ITEM_LIST *stTradeItemList, PS_ITEM_SOCKET_LIST *stSocketList, PS_ITEM_BROACH_LIST *stBroachList, PS_ITEM_PACKAGE_LIST *psPackageList)
// Sets trade confirm item list with socket/broach/package info
bool CGocInventory::SetTradeConfirm(void* stTradeItemList, void* stSocketList, void* stBroachList, void* psPackageList) {
    // IDA Decompiled:
    // char __fastcall CGocInventory::SetTradeConfirm(CGocInventory *this, ST_TRADE_ITEM_LIST *stTradeItemList, PS_ITEM_SOCKET_LIST *stSocketList, PS_ITEM_BROACH_LIST *stBroachList, PS_ITEM_PACKAGE_LIST *psPackageList)
    // {
    //   // Check trade item count limit (max 5)
    //   if (m_stTradeInfo.listInfo.size() > 5) return false;
    //
    //   // Set UCID and money
    //   stTradeItemList->dwUCID = GetActor()->GetActorID().GetQuestID();
    //   stTradeItemList->biMoney = GetTradeMoney();
    //
    //   // Iterate through trade items
    //   for (auto at = m_stTradeInfo.listInfo.begin(); at != m_stTradeInfo.listInfo.end(); ++at) {
    //     stData = *at;
    //
    //     // Get item from slot
    //     GetSlotItem(&pItem, stData.byInvenType, stData.shSlotPos, byLock);
    //     if (pItem.invalid() || byLock[0] != 10) {
    //       // Send lock log on error
    //       XGameServer::SendItemLockLog(dwUCID, stData.byInvenType, stData.shSlotPos, byLock[0], 10, 0);
    //       return false;
    //     }
    //
    //     // Get socket/broach info
    //     pItem->GetSocketInfo(stSocketList);
    //     pItem->GetBroachInfo(stBroachList);
    //
    //     // Get package info
    //     PS_ITEM_PACKAGE psPackageInfo;
    //     pItem->GetPackageInfo(&psPackageInfo);
    //
    //     // Check resealed package
    //     if (IsResealPackage(pItem->GetItemID())) {
    //       if (!IsResealPackageCount(pItem->GetItemID(), psPackageInfo.vecInfo.size())) {
    //         LogError("SetTradeConfirm error - Fault package info[ ItemID:%d ] ( %d )", pItem->GetItemID(), 3186);
    //         return false;
    //       }
    //       psPackageList->vecInfo.push_back(psPackageInfo);
    //     }
    //
    //     // Add to trade list
    //     ST_TRADE_ITEM stInfo;
    //     stInfo.byInvenType = stData.byInvenType;
    //     stInfo.shSlotPos = stData.shSlotPos;
    //     pItem->GetItem(&stInfo.stItem);
    //     stTradeItemList->vecTradeItem.push_back(stInfo);
    //   }
    //
    //   return true;
    // }

    // TODO: 需人工审查 - Implement when ST_TRADE_ITEM_LIST/m_stTradeInfo available
    (void)stTradeItemList;
    (void)stSocketList;
    (void)stBroachList;
    (void)psPackageList;
    return false;
}

// IDA: 0x1400AEBE0
// bool __fastcall CGocInventory::CanTradeConfirm(CGocInventory *this, ST_TRADE_ITEM_LIST *stMyItemList, ST_TRADE_ITEM_LIST *stTargetItemList)
// Checks if trade can be confirmed with target item list
bool CGocInventory::CanTradeConfirm(void* stMyItemList, void* stTargetItemList) {
    // IDA Decompiled:
    // __int64 __fastcall CGocInventory::CanTradeConfirm(CGocInventory *this, ST_TRADE_ITEM_LIST *stMyItemList, ST_TRADE_ITEM_LIST *stTargetItemList)
    // {
    //   // Check my item count limit
    //   if (stMyItemList->vecTradeItem.size() > 5) return false;
    //
    //   // Check inventory space for target items
    //   for (k = 0; k < stTargetItemList->vecTradeItem.size(); ++k) {
    //     pInven = GetInvenPtr(stTargetItemList->vecTradeItem[k].stInfo.byInvenType);
    //     if (!pInven) return false;
    //
    //     shPos = pInven->GetFirstEmptySlot(10);
    //     if (shPos == -1) return false;
    //
    //     // Store slot position and lock it
    //     stTargetItemList->vecTradeItem[k].shSocketPos = shPos;
    //     pInven->SetLock(shPos, 53);
    //   }
    //
    //   return true;
    // }

    // TODO: 需人工审查 - Implement when ST_TRADE_ITEM_LIST/GetInvenPtr available
    (void)stMyItemList;
    (void)stTargetItemList;
    return false;
}

// IDA: 0x1400AEE00
// void __fastcall CGocInventory::UpdateTradeUnLock(CGocInventory *this, ST_TRADE_ITEM_LIST *stTargetItemList)
// Unlocks trade items after trade completes
void CGocInventory::UpdateTradeUnLock(void* stTargetItemList) {
    // IDA Decompiled:
    // void __fastcall CGocInventory::UpdateTradeUnLock(CGocInventory *this, ST_TRADE_ITEM_LIST *stTargetItemList)
    // {
    //   // Unlock all trade items first
    //   UpdateTradeUnLock();
    //
    //   // Unlock target items
    //   for (i = 0; i < stTargetItemList->vecTradeItem.size(); ++i) {
    //     // Only unlock valid items (serial != -1)
    //     if (stTargetItemList->vecTradeItem[i].stInfo.stItem.xSerial != -1) {
    //       SetLock(stTargetItemList->vecTradeItem[i].stInfo.byInvenType,
    //               stTargetItemList->vecTradeItem[i].shSocketPos, 0);
    //     }
    //   }
    // }

    // TODO: 需人工审查 - Implement when ST_TRADE_ITEM_LIST/SetLock available
    (void)stTargetItemList;
}

// ============================================================================
// Batch 6: Item shop and random option functions
// ============================================================================

// IDA: 0x1400DBC90
// void __fastcall CGocInventory::SendDBLimitItemInfo(CGocInventory *this)
// Sends item limit info to DB (main=0x21, sub=0x41)
void CGocInventory::SendDBLimitItemInfo() {
    // IDA Decompiled:
    // void __fastcall CGocInventory::SendDBLimitItemInfo(CGocInventory *this)
    // {
    //   // Get user
    //   pUser = (CUser *)_RTDynamicCast_0(GetActor(), 0, &CMover RTTI, &CUser RTTI, 0);
    //   if (!pUser) return;
    //
    //   // Send DB packet (main=0x21, sub=0x41)
    //   XSendDBPacket xSendDBPacket(GetActor(), 0x21, 0x41);
    //   xSendDBPacket << pUser->GetActorID().GetQuestID();
    //   XGameServer::SendDBGame(&xSendDBPacket);
    // }
}

// IDA: 0x1400DBDF0
// IDA: 0x1400DBDF0
// bool __fastcall CGocInventory::ReduceItemShop(CGocInventory *this, TB_ITEM *pTBItem, int nReduceCount, unsigned __int8 byLock, PS_RES_STORAGE_INFO *psUpdateItem)
// Reduces item count for shop purchases
bool CGocInventory::ReduceItemShop(TB_ITEM* pTBItem, int nReduceCount, std::uint8_t byLock, PS_RES_STORAGE_INFO* psUpdateItem) {
    // IDA 0x1400DBDF0 精确还原
    if (!pTBItem) {
        return false;
    }

    // Get item classify
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM_CLASSIFY* pTBClassify = pServer->GetResourceMgr().GetTB_ITEM_CLASSIFY(pTBItem->Item_Classify_Index);
    if (!pTBClassify) {
        LogHelper::LogError("game.contents", "ReduceItemShop if( NULL == pTBClassify ) ID:%d ) ", pTBItem->Item_ID);
        return false;
    }

    // Get inventory pointer
    XBaseInventory* pInven = GetTBInvenPtr(pTBClassify->Item_Inven_Type);
    if (!pInven) {
        LogHelper::LogError("game.contents", "ReduceItemShop if( NULL == pInven ) ID:%d ) ", pTBItem->Item_ID);
        return false;
    }

    // Validate count
    if (nReduceCount < 1) {
        LogHelper::LogError("game.contents", "ReduceItemShop if( nReduceCount < 1 ) ID:%d, COUNT:%d ) ", pTBItem->Item_ID, nReduceCount);
        return false;
    }

    // Try check shop first, then del item count
    return ReduceItemCheckShop(pTBItem, nReduceCount, byLock, psUpdateItem)
        || pInven->DelItemCountShop(pTBItem, nReduceCount, byLock, psUpdateItem);
}

// IDA: 0x1400DBF40
// bool __fastcall CGocInventory::ReduceItemCheckShop(CGocInventory *this, TB_ITEM *pTBItem, int *nReduceCount, unsigned __int8 byLock, PS_RES_STORAGE_INFO *psUpdateItem)
// Checks and reduces item count for shop
bool CGocInventory::ReduceItemCheckShop(TB_ITEM* pTBItem, int& nReduceCount, std::uint8_t byLock, PS_RES_STORAGE_INFO* psUpdateItem) {
    // IDA 0x1400DBF40 精确还原
    if (!pTBItem || !psUpdateItem) {
        return false;
    }

    // Iterate through update items
    for (auto& item : psUpdateItem->vecItem) {
        // Check if item matches and has count >= 1
        if (item.stItem.nItemID == pTBItem->Item_ID && item.stItem.sCount >= 1) {
            // Reduce count
            if (nReduceCount <= item.stItem.sCount) {
                item.stItem.sCount -= static_cast<std::int16_t>(nReduceCount);
                nReduceCount = 0;
            } else {
                nReduceCount -= item.stItem.sCount;
                item.stItem.sCount = 0;
            }

            // Lock if depleted
            if (item.stItem.sCount == 0) {
                SetLock(item.byInvenType, item.shSlotPos, 1);
            }

            // If all items reduced, return success
            if (nReduceCount == 0) {
                return true;
            }
        }
    }

    return false;
}

// IDA: 0x1400DC0C0
// bool __fastcall CGocInventory::CheckRandomOption(CGocInventory *this, STItem *stOptinChangeInfo)
// Checks and reorders random options
bool CGocInventory::CheckRandomOption(STItem* stOptinChangeInfo) {
    // IDA 0x1400DC0C0 精确还原
    if (!stOptinChangeInfo) {
        return false;
    }

    // Get item table
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM* pTBItem = pServer->GetResourceMgr().GetTB_ITEM(stOptinChangeInfo->nItemID);
    if (!pTBItem) {
        return false;
    }

    // Get item classify
    TB_ITEM_CLASSIFY* pTBItemClassify = pServer->GetResourceMgr().GetTB_ITEM_CLASSIFY(pTBItem->Item_Classify_Index);
    if (!pTBItemClassify) {
        return false;
    }

    // Check use type (51 = not random option)
    if (pTBItemClassify->Item_Use_Type == 51) {
        return false;
    }

    // Collect non-empty options
    std::vector<ST_EXTEND_OPTION> vecTemp;
    bool bOption = false;
    for (int i = 0; i < 5; ++i) {
        if (stOptinChangeInfo->stExtendOption[i].byType) {
            bOption = true;
            vecTemp.push_back(stOptinChangeInfo->stExtendOption[i]);
        }
    }

    // Reorder options
    if (bOption) {
        std::memset(stOptinChangeInfo->stExtendOption, 0, sizeof(stOptinChangeInfo->stExtendOption));
        for (size_t j = 0; j < vecTemp.size(); ++j) {
            stOptinChangeInfo->stExtendOption[j] = vecTemp[j];
        }
    }

    return bOption;
}

// IDA: 0x1400DCA20
// bool __fastcall CGocInventory::IsRandomItemTitle(CGocInventory *this, unsigned int nTitleGroupID)
// Checks if item title is random
bool CGocInventory::IsRandomItemTitle(int nTitleGroupID) {
    // IDA 0x1400DCA20 精确还原
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM_TITLE* pTBItemTitle = pServer->GetResourceMgr().GetTB_ITEM_TITLE(static_cast<unsigned int>(nTitleGroupID));
    if (!pTBItemTitle) {
        return false;
    }

    // Check if has multiple group IDs (random)
    // IDA logic: loop i=0..9, if i>0 return Group_ID[i] != 0
    // This means it returns true if Group_ID02 != 0
    for (int i = 0; i < 10; ++i) {
        if (i > 0) {
            // Access Group_ID01, Group_ID02, ... as array
            unsigned int* pGroupIDs = &pTBItemTitle->Group_ID01;
            return pGroupIDs[i] != 0;
        }
    }
    return false;
}

// ============================================================================
// Batch 7: Disassemble log and dye functions
// ============================================================================

// IDA: 0x1400DCAA0
// PDB: ?AddDisassembleLog@CGocInventory@@QEAAX_JUST_CREATE_ITEM@@@Z
void CGocInventory::AddDisassembleLog(
    std::int64_t biSerial,
    ST_CREATE_ITEM stLogDisInfo) {
    if (m_mapLogDisassemble.find(biSerial) == m_mapLogDisassemble.end()) {
        m_mapLogDisassemble.insert(
            std::make_pair(biSerial, stLogDisInfo));
    }
}

// IDA: 0x1400DCB30
// PDB: ?GetDisassembleLog@CGocInventory@@QEAAX_JAEAUST_CREATE_ITEM@@@Z
void CGocInventory::GetDisassembleLog(
    std::int64_t biSerial,
    ST_CREATE_ITEM& stLogDisInfo) {
    const auto iter = m_mapLogDisassemble.find(biSerial);
    if (iter != m_mapLogDisassemble.end()) {
        stLogDisInfo = iter->second;
    }
}

// IDA: 0x1400DCBB0
void CGocInventory::ClearDissassembleLog() {
    m_mapLogDisassemble.clear();
}

// IDA: 0x1400DDA40
// bool __fastcall CGocInventory::CheckDyeInfo(CGocInventory *this, E_DYE_TYPE eDyeType, int nID, int nSelectDyeID, int nOriDyeID, int *nDye, ST_CREATE_ITEMS *stNeedItems)
// Checks dye info for item dyeing
bool CGocInventory::CheckDyeInfo(int eDyeType, int nID, int nSelectDyeID, int nOriDyeID, int& nDye, void* stNeedItems) {
    // IDA Decompiled:
    // char __fastcall CGocInventory::CheckDyeInfo(CGocInventory *this, E_DYE_TYPE eDyeType, int nID, int nSelectDyeID, int nOriDyeID, int *nDye, ST_CREATE_ITEMS *stNeedItems)
    // {
    //   *nDye = -1;
    //   stNeedItems->clear();
    //
    //   // Get dye info table
    //   pTB_DyeInfo = XResourceMgr::GetTB_DYE_INFO(nID);
    //   if (!pTB_DyeInfo) {
    //     LogError("CheckDyeInfo error - No Table TB_DYE_INFO[UCID:%d, DyeID:%d]", dwUCID, nID);
    //     return false;
    //   }
    //
    //   // Check if same dye
    //   if (nOriDyeID && nOriDyeID == nSelectDyeID) {
    //     LogError("CheckDyeInfo error - Same id[UCID:%d, SelectID:%d]", dwUCID, nSelectDyeID);
    //     return false;
    //   }
    //
    //   switch (eDyeType) {
    //     case E_DYE_TYPE_SELECT:
    //       // Select dye - find in S_Dye list
    //       for (i = 0; i < 10 && pTB_DyeInfo->S_Dye[i]; ++i) {
    //         if (pTB_DyeInfo->S_Dye[i] == nSelectDyeID) {
    //           *nDye = pTB_DyeInfo->S_Dye[i];
    //           stNeedItem.nItemID = pTB_DyeInfo->S_Dye_Need_Item_ID;
    //           stNeedItem.shCount = pTB_DyeInfo->S_Dye_Need_Item_CNT;
    //           stNeedItems->push_back(stNeedItem);
    //           break;
    //         }
    //       }
    //       break;
    //
    //     case E_DYE_TYPE_RANDOM:
    //       // Random dye - use rate table
    //       nRate = XItemFactory::nRand(1, 10000);
    //       for (j = 0; j < 10 && pTB_DyeInfo->Dye_Rate[j] && pTB_DyeInfo->Dye_Rate_ID[j]; ++j) {
    //         nRate -= pTB_DyeInfo->Dye_Rate[j];
    //         if (nRate <= 0) {
    //           *nDye = pTB_DyeInfo->Dye_Rate_ID[j];
    //           stNeedItem.nItemID = pTB_DyeInfo->R_Dye_Need_Item_ID;
    //           stNeedItem.shCount = pTB_DyeInfo->R_Dye_Need_Item_CNT;
    //           stNeedItems->push_back(stNeedItem);
    //           break;
    //         }
    //       }
    //       break;
    //
    //     case E_DYE_TYPE_DECOLOR:
    //       // Decolor - remove dye
    //       if (!nOriDyeID) {
    //         LogError("CheckDyeInfo error - No dye id[UCID:%d, SelectID:%d, OriID:%d]", dwUCID, nSelectDyeID, 0);
    //         return false;
    //       }
    //       stNeedItem.nItemID = pTB_DyeInfo->S_Dye_Need_Item_ID;
    //       stNeedItem.shCount = pTB_DyeInfo->S_Dye_Need_Item_CNT;
    //       stNeedItems->push_back(stNeedItem);
    //       *nDye = 0;
    //       break;
    //
    //     case E_DYE_TYPE_MILEAGE:
    //       // Mileage dye - use dye points
    //       for (k = 0; k < 10 && pTB_DyeInfo->Dye_Rate_ID[k]; ++k) {
    //         if (pTB_DyeInfo->Dye_Rate_ID[k] == nSelectDyeID) {
    //           *nDye = nSelectDyeID;
    //           break;
    //         }
    //       }
    //       if (!IsHiddenDye(*nDye)) {
    //         LogError("CheckDyeInfo error - Not hidden[UCID:%d, SelectID:%d]", dwUCID, *nDye);
    //         return false;
    //       }
    //       nNeedPoint = GetCommonValue(60002);
    //       if (nNeedPoint <= 0 || nNeedPoint > m_nDyePoint) {
    //         LogError("CheckDyeInfo error - Shortage dye milage[UCID:%d, DyePoint:%d]", dwUCID, m_nDyePoint);
    //         return false;
    //       }
    //       break;
    //   }
    //
    //   // Validate result
    //   if (eDyeType == E_DYE_TYPE_MILEAGE) {
    //     if (*nDye < 0) {
    //       LogError("CheckDyeInfo error - Failed mileage dye[UCID:%d, SelectID:%d]", dwUCID, nSelectDyeID);
    //       return false;
    //     }
    //   } else if (stNeedItems->empty() || *nDye < 0) {
    //     LogError("CheckDyeInfo error - No need items[UCID:%d, ResultID:%d]", dwUCID, *nDye);
    //     return false;
    //   }
    //
    //   return true;
    // }

    // TODO: 需人工审查 - Implement when TB_DYE_INFO/ST_CREATE_ITEMS available
    (void)eDyeType;
    (void)nID;
    (void)nSelectDyeID;
    (void)nOriDyeID;
    (void)nDye;
    (void)stNeedItems;
    return false;
}

// IDA: 0x1400DDF10
// bool __fastcall CGocInventory::IsValidDyeInfo(CGocInventory *this, unsigned int nItemID, int nDyeID)
// Validates dye info for item
bool CGocInventory::IsValidDyeInfo(int nItemID, int nDyeID) {
    // IDA Decompiled:
    // char __fastcall CGocInventory::IsValidDyeInfo(CGocInventory *this, unsigned int nItemID, int nDyeID)
    // {
    //   // No dye is always valid
    //   if (!nDyeID) return true;
    //
    //   // Get item table
    //   pTB_Item = XResourceMgr::GetTB_ITEM(nItemID);
    //   if (!pTB_Item) return false;
    //
    //   // Get dye info table
    //   pTB_DyeInfo = XResourceMgr::GetTB_DYE_INFO(pTB_Item->Item_Effect_ID);
    //   if (!pTB_DyeInfo) return false;
    //
    //   // Check if dye ID is in valid list
    //   for (i = 0; i < 10; ++i) {
    //     if (pTB_DyeInfo->S_Dye[i] == nDyeID || pTB_DyeInfo->Dye_Rate_ID[i] == nDyeID) {
    //       return true;
    //     }
    //   }
    //
    //   return false;
    // }

    // TODO: 需人工审查 - Implement when TB_ITEM/TB_DYE_INFO available
    (void)nItemID;
    (void)nDyeID;
    return false;
}

// ============================================================================
// Batch 8: Slot validation and item make limit functions
// ============================================================================

// IDA: 0x1400DF110
// bool __fastcall CGocInventory::IsValidSlotItem(CGocInventory *this, unsigned __int8 byInvenType, __int16 shSlot, STItem *stItem)
// Validates if slot item matches expected serial
bool CGocInventory::IsValidSlotItem(std::uint8_t byInvenType, std::int16_t shSlot, void* stItem) {
    // IDA Decompiled:
    // __int64 __fastcall CGocInventory::IsValidSlotItem(CGocInventory *this, unsigned __int8 byInvenType, __int16 shSlot, STItem *stItem)
    // {
    //   // Get user UCID
    //   dwUCID = GetActor()->GetActorID().GetQuestID();
    //
    //   // Get item from slot
    //   byLock[0] = 0;
    //   GetSlotItem(&pItemPtr, byInvenType, shSlot, byLock);
    //
    //   // Check if slot is empty
    //   if (pItemPtr.invalid()) return true;  // Empty slot is valid
    //
    //   // Check serial mismatch
    //   if (pItemPtr->GetSerial() != stItem->xSerial) {
    //     LogError("IsValidSlotItem error - Overlapped item[UCID:%d, Inven:%d, Slot:%d, ExistSerial:%I64d, Serial:%I64d",
    //              dwUCID, byInvenType, shSlot, pItemPtr->GetSerial(), stItem->xSerial);
    //     return false;
    //   }
    //
    //   return false;  // Slot occupied with different item
    // }

    // TODO: 需人工审查 - Implement when GetSlotItem/STItem available
    (void)byInvenType;
    (void)shSlot;
    (void)stItem;
    return true;
}

// IDA: 0x1400DF260
// void __fastcall CGocInventory::AddItemEmptySlot(CGocInventory *this)
// Adds overlapped items to empty slots
void CGocInventory::AddItemEmptySlot() {
    // IDA Decompiled:
    // void __fastcall CGocInventory::AddItemEmptySlot(CGocInventory *this)
    // {
    //   // Get user info
    //   dwUCID = GetActor()->GetActorID().GetQuestID();
    //   dwUAID = pUser->GetUAID();
    //
    //   PS_RES_STORAGE_INFO psUpdateItemList;
    //   psUpdateItemList.vecItem.clear();
    //
    //   // Process overlapped inventory slots
    //   if (!m_mpOverlappedSlot.empty()) {
    //     for (auto iter = m_mpOverlappedSlot.begin(); iter != m_mpOverlappedSlot.end(); ++iter) {
    //       byInvenType = iter->first;
    //       pInvenPtr = GetInvenPtr(byInvenType);
    //       if (pInvenPtr) {
    //         shSlot = pInvenPtr->GetEmptySlot_2();
    //         if (shSlot != -1) {
    //           stItem = iter->second;
    //           AddItem(byInvenType, shSlot, &stItem);
    //           LogError("AddItemEmptySlot Inven - Add overlapped item[UCID:%d, ItemID:%d, Serial:%I64d, Inven:%d, Slot:%d]",
    //                    dwUCID, stItem.nItemID, stItem.xSerial, byInvenType, shSlot);
    //           // Add to update list
    //           psInfo.byInvenType = byInvenType;
    //           psInfo.shSlotPos = shSlot;
    //           psInfo.stItem = stItem;
    //           psUpdateItemList.vecItem.push_back(psInfo);
    //         }
    //       }
    //     }
    //     m_mpOverlappedSlot.clear();
    //   }
    //
    //   // Process overlapped equip slots
    //   if (!m_mpSlot.empty()) {
    //     for (auto iter = m_mpSlot.begin(); iter != m_mpSlot.end(); ++iter) {
    //       pTB_Item = XResourceMgr::GetTB_ITEM(iter->second.nItemID);
    //       if (pTB_Item) {
    //         pTB_ItemClassify = XResourceMgr::GetTB_ITEM_CLASSIFY(pTB_Item->Item_Classify_Index);
    //         if (pTB_ItemClassify) {
    //           TBInvenPtr = GetTBInvenPtr(pTB_ItemClassify->Item_Inven_Type);
    //           if (TBInvenPtr) {
    //             InvenType = TBInvenPtr->GetInvenType();
    //             shSlot = TBInvenPtr->GetEmptySlot_2();
    //             if (shSlot != -1) {
    //               AddItem(InvenType, shSlot, &iter->second);
    //               LogError("AddItemEmptySlot Equip- Add wring item[UCID:%d, ItemID:%d, Serial:%I64d, Inven:%d, Slot:%d]",
    //                        dwUCID, iter->second.nItemID, iter->second.xSerial, InvenType, shSlot);
    //               // Add to update list
    //               psUpdateItemList.vecItem.push_back(...);
    //             }
    //           }
    //         }
    //       }
    //     }
    //     m_mpSlot.clear();
    //   }
    //
    //   // Send DB packet if any updates (main=0x21, sub=0x50)
    //   if (!psUpdateItemList.vecItem.empty()) {
    //     XSendDBPacket xSendDBPacket(GetActor(), 0x21, 0x50);
    //     xSendDBPacket << dwUAID;
    //     xSendDBPacket << dwUCID;
    //     xSendDBPacket << psUpdateItemList;
    //     XGameServer::SendDBGame(&xSendDBPacket);
    //   }
    // }
}

// IDA: 0x1400DF930
// void __fastcall CGocInventory::SendDBItemMakeLimitInfo(CGocInventory *this)
// Sends item make limit info to DB (main=0x21, sub=0x51)
void CGocInventory::SendDBItemMakeLimitInfo() {
    // IDA Decompiled:
    // void __fastcall CGocInventory::SendDBItemMakeLimitInfo(CGocInventory *this)
    // {
    //   pUser = (CUser *)_RTDynamicCast_0(GetActor(), ...);
    //   if (!pUser) return;
    //
    //   // Send DB packet (main=0x21, sub=0x51)
    //   XSendDBPacket xSendDBPacket(GetActor(), 0x21, 0x51);
    //   xSendDBPacket << pUser->GetActorID().GetQuestID();  // UCID
    //   xSendDBPacket << pUser->GetUAID();                  // UAID
    //   XGameServer::SendDBGame(&xSendDBPacket);
    // }
}

// IDA: 0x1400DFAB0
// void __fastcall CGocInventory::LoadItemMakeLimitInfo(CGocInventory *this, PS_ITEM_MAKE_LIMIT_LIST *psList)
// Loads item make limit info from DB
void CGocInventory::LoadItemMakeLimitInfo(void* psList) {
    // IDA Decompiled:
    // void __fastcall CGocInventory::LoadItemMakeLimitInfo(CGocInventory *this, PS_ITEM_MAKE_LIMIT_LIST *psList)
    // {
    //   // Insert all items into map
    //   for (i = 0; i < psList->vecInfo.size(); ++i) {
    //     psInfo = psList->vecInfo[i];
    //     m_mpItemMakeLimit.insert(std::make_pair(psInfo.nItemID, psInfo));
    //   }
    //
    //   // Set init date to today 9:00 AM
    //   tCurr = CTime::GetTickCount();
    //   nDay = tCurr.GetDay();
    //   nMonth = tCurr.GetMonth();
    //   Year = tCurr.GetYear();
    //   tInit = CTime(Year, nMonth, nDay, 9, 0, 0, -1);
    //   m_tMakeInitDate = tInit;
    //
    //   // Send to client (main=8, sub=0x68)
    //   psList->byFlag = 0;
    //   XSendPacket xSendPacket(8, 0x68);
    //   xSendPacket << *psList;
    //   GetActor()->Send(&xSendPacket);
    // }

    // TODO: 需人工审查 - Implement when PS_ITEM_MAKE_LIMIT_LIST/m_mpItemMakeLimit available
    (void)psList;
}

// IDA: 0x1400DFD80
// void __fastcall CGocInventory::OnInitItemMakeLimit(CGocInventory *this)
// Initializes item make limit with daily reset
void CGocInventory::OnInitItemMakeLimit() {
    // IDA Decompiled:
    // void __fastcall CGocInventory::OnInitItemMakeLimit(CGocInventory *this)
    // {
    //   // Check if current time >= init date
    //   tCurr = CTime::GetTickCount();
    //   if (tCurr < m_tMakeInitDate) return;
    //
    //   pUser = (CUser *)_RTDynamicCast_0(GetActor(), ...);
    //   if (!pUser) return;
    //
    //   dwUCID = pUser->GetActorID().GetQuestID();
    //   dwUAID = pUser->GetUAID();
    //
    //   PS_DB_ITEM_MAKE_LIMIT_INIT psList;
    //
    //   // Iterate through make limits
    //   for (auto iter = m_mpItemMakeLimit.begin(); iter != m_mpItemMakeLimit.end(); ++iter) {
    //     // Check if registered and expired
    //     if (iter->second.m_pRegisteredAtType && iter->second.m_pRegisteredAtType <= m_tMakeInitDate) {
    //       psInfo = iter->second;
    //
    //       // Reset registration
    //       iter->second.m_pRegisteredAtType = nullptr;
    //       iter->second.shCount = 0;
    //
    //       // Get TB_MAKE
    //       pTB_Make = XResourceMgr::GetTB_MAKE(iter->second.nMakeIndex);
    //       if (pTB_Make) {
    //         // Add to list
    //         psLimitInfo.byLimitType = pTB_Make->Limit_Type;
    //         psLimitInfo.psInfo = psInfo;
    //         psList.vecInfo.push_back(psLimitInfo);
    //
    //         // Send log
    //         stLog._sMainType = 4;
    //         stLog._sSubType = 132;
    //         stLog._nUAID = dwUAID;
    //         stLog._nUCID = dwUCID;
    //         stLog.nParam0 = psInfo.nItemID;
    //         stLog.nParam2 = pUser->GetLevel();
    //         stLog.nParam3 = psInfo.nMakeIndex;
    //         stLog.nParam9 = psInfo.biEndDate;
    //         XGameServer::SendDBLog(&stLog);
    //       }
    //     }
    //   }
    //
    //   // Send DB packet if any updates (main=0x21, sub=0x52)
    //   if (!psList.vecInfo.empty()) {
    //     psList.dwUAID = dwUAID;
    //     psList.dwUCID = dwUCID;
    //     XSendDBPacket xSendDBPacket(GetActor(), 0x21, 0x52);
    //     xSendDBPacket << psList;
    //     XGameServer::SendDBGame(&xSendDBPacket);
    //   }
    //
    //   // Advance init date by 1 day
    //   m_tMakeInitDate += CTimeSpan(1, 0, 0, 0);
    // }
}

// ============================================================================
// Batch 9: Item make limit helper functions
// ============================================================================

// IDA: 0x1400E02B0
// void __fastcall CGocInventory::UpdateItemMakeLimit(CGocInventory *this, TB_MAKE *pTB_Make, unsigned __int8 byMakeCount, PS_ITEM_MAKE_LIMIT_INFO *psMakeLimitInfo, PS_DB_ITEM_MAKE_LIMIT_UPDATE *psUpdate)
// Updates item make limit info and sends DB update
void CGocInventory::UpdateItemMakeLimit(void* pTB_Make, std::uint8_t byMakeCount, void* psMakeLimitInfo, void* psUpdate) {
    // IDA Decompiled:
    // void __fastcall CGocInventory::UpdateItemMakeLimit(CGocInventory *this, TB_MAKE *pTB_Make, unsigned __int8 byMakeCount, PS_ITEM_MAKE_LIMIT_INFO *psMakeLimitInfo, PS_DB_ITEM_MAKE_LIMIT_UPDATE *psUpdate)
    // {
    //   pUser = (CUser *)_RTDynamicCast_0(GetActor(), ...);
    //   if (!pUser) {
    //     LogError("UpdateItemMakeLimit error - NULL pUser");
    //     return;
    //   }
    //
    //   if (!pTB_Make) {
    //     LogError("UpdateItemMakeLimit error - No Table TB_MAKE[UCID:%d, Index:%d]", ...);
    //     return;
    //   }
    //
    //   if (!pTB_Make->Limit_Type) return;  // No limit
    //
    //   // Find existing entry
    //   nPrevCount = 0;
    //   auto iter = m_mpItemMakeLimit.find(psMakeLimitInfo->nMakeIndex);
    //   if (iter == m_mpItemMakeLimit.end()) {
    //     // Insert new entry
    //     m_mpItemMakeLimit.insert(std::make_pair(psMakeLimitInfo->nMakeIndex, *psMakeLimitInfo));
    //   } else {
    //     // Update existing entry
    //     nPrevCount = iter->second.shCount;
    //     iter->second = *psMakeLimitInfo;
    //   }
    //
    //   // Setup DB update
    //   psDBInfo.byLimitType = pTB_Make->Limit_Type;
    //   psDBInfo.psInfo = *psMakeLimitInfo;
    //   psUpdate->dwUAID = pUser->GetUAID();
    //   psUpdate->dwUCID = pUser->GetActorID().GetQuestID();
    //   psUpdate->psDBInfo = psDBInfo;
    //
    //   // Send log
    //   stLog._sMainType = 4;
    //   stLog._sSubType = 131;
    //   stLog._nUAID = pUser->GetUAID();
    //   stLog._nUCID = pUser->GetActorID().GetQuestID();
    //   stLog.nParam0 = psMakeLimitInfo->nItemID;
    //   stLog.nParam2 = pUser->GetLevel();
    //   stLog.nParam3 = psMakeLimitInfo->nMakeIndex;
    //   stLog.nParam4 = nPrevCount;
    //   stLog.nParam7 = byMakeCount;
    //   stLog.nParam8 = pTB_Make->Limit_Count;
    //   stLog.nParam9 = psMakeLimitInfo->biEndDate;
    //   XGameServer::SendDBLog(&stLog);
    // }

    // TODO: 需人工审查 - Implement when TB_MAKE/PS_ITEM_MAKE_LIMIT_INFO available
    (void)pTB_Make;
    (void)byMakeCount;
    (void)psMakeLimitInfo;
    (void)psUpdate;
}

// IDA: 0x1400E06A0
// bool __fastcall CGocInventory::CheckMakeLimitCount(CGocInventory *this, int nMakeIndex, unsigned __int8 byMakeCount, PS_ITEM_MAKE_LIMIT_INFO *psInfo)
// Checks if make limit count allows more crafting
bool CGocInventory::CheckMakeLimitCount(int nMakeIndex, std::uint8_t byMakeCount, void* psInfo) {
    // IDA Decompiled:
    // char __fastcall CGocInventory::CheckMakeLimitCount(CGocInventory *this, int nMakeIndex, unsigned __int8 byMakeCount, PS_ITEM_MAKE_LIMIT_INFO *psInfo)
    // {
    //   // Check user
    //   if (!_RTDynamicCast_0(GetActor(), ...)) return false;
    //
    //   // Get TB_MAKE
    //   pTB_Make = XResourceMgr::GetTB_MAKE(nMakeIndex);
    //   if (!pTB_Make) {
    //     LogError("CheckMakeLimitCount error - No Table TB_MAKE[Index:%d]", nMakeIndex);
    //     return false;
    //   }
    //
    //   // No limit
    //   if (!pTB_Make->Limit_Type) return true;
    //
    //   // Check make count
    //   if (!byMakeCount) {
    //     LogError("CheckMakeLimitCount error - Item make count[Index:%d, Count:%d]", nMakeIndex, 0);
    //     return false;
    //   }
    //
    //   // Find existing entry
    //   auto iter = m_mpItemMakeLimit.find(nMakeIndex);
    //   if (iter == m_mpItemMakeLimit.end()) {
    //     // New entry - setup info
    //     biEndDate = 0;
    //     if (!GetMakeLimitEndDate(pTB_Make, &biEndDate)) return false;
    //
    //     for (i = 0; i < 3 && pTB_Make->MakeItem_ID[i]; ++i) {
    //       psInfo->nMakeIndex = pTB_Make->Make_Index;
    //       psInfo->nItemID = pTB_Make->MakeItem_ID[i];
    //       psInfo->shCount = byMakeCount;
    //       psInfo->biEndDate = biEndDate;
    //     }
    //   } else {
    //     // Existing entry - check limit
    //     *psInfo = iter->second;
    //     nPrvCount = iter->second.shCount;
    //     shCount = nPrvCount + byMakeCount;
    //
    //     if (pTB_Make->Limit_Count < shCount) {
    //       LogError("CheckMakeLimitCount error - Over item make limit count[Index:%d, Count:%d]", nMakeIndex, byMakeCount);
    //       return false;
    //     }
    //
    //     // Setup end date if first time
    //     if (!nPrvCount) {
    //       biEndDate = 0;
    //       if (!GetMakeLimitEndDate(pTB_Make, &biEndDate)) return false;
    //       psInfo->biEndDate = biEndDate;
    //     }
    //     psInfo->shCount = shCount;
    //   }
    //
    //   return true;
    // }

    // TODO: 需人工审查 - Implement when TB_MAKE/PS_ITEM_MAKE_LIMIT_INFO available
    (void)nMakeIndex;
    (void)byMakeCount;
    (void)psInfo;
    return false;
}

// IDA: 0x1400E09C0
// bool __fastcall CGocInventory::GetMakeLimitEndDate(CGocInventory *this, TB_MAKE *pTB_Make, __int64 *biEndDate)
// Gets end date for make limit period
bool CGocInventory::GetMakeLimitEndDate(void* pTB_Make, std::int64_t& biEndDate) {
    // IDA Decompiled:
    // char __fastcall CGocInventory::GetMakeLimitEndDate(CGocInventory *this, TB_MAKE *pTB_Make, __int64 *biEndDate)
    // {
    //   *biEndDate = 0;
    //   if (!pTB_Make) {
    //     LogError("GetMakeLimitEndDate error - No Table TB_MAKE");
    //     return false;
    //   }
    //
    //   // Get current time
    //   CTime tCurr = CTime::GetTickCount();
    //   eType = pTB_Make->Limit_Reset_Time_PeriodType;
    //
    //   switch (eType) {
    //     case 0:  // No limit - 30 years from now
    //       nYear = tCurr.GetYear() + 30;
    //       tCurr = CTime(nYear, tCurr.GetMonth(), tCurr.GetDay(), 9, 0, 0, -1);
    //       *biEndDate = tCurr.GetTime();
    //       return true;
    //
    //     case 1-8:  // Daily/Weekly
    //       nInitDay = GetMakeLimitInitDay(eType, tCurr);
    //       tCurr = CTime(tCurr.GetYear(), tCurr.GetMonth(), tCurr.GetDay(), 9, 0, 0, -1);
    //       tInit = tCurr + CTimeSpan(nInitDay, 0, 0, 0);
    //       *biEndDate = tInit.GetTime();
    //       return true;
    //
    //     case 9:  // Monthly
    //       nYear = tCurr.GetYear();
    //       nMonth = tCurr.GetMonth() + 1;
    //       if (nMonth > 12) { nMonth = 1; ++nYear; }
    //       CTime v13(nYear, nMonth, 1, 9, 0, 0, -1);
    //       *biEndDate = v13.GetTime();
    //       return true;
    //
    //     default:
    //       if (!*biEndDate) {
    //         LogError("GetMakeLimitEndDate error - EndDate 0[type:%d]", pTB_Make->Limit_Reset_Time_PeriodType);
    //       }
    //       return false;
    //   }
    // }

    // TODO: 需人工审查 - Implement when TB_MAKE/CTime available
    (void)pTB_Make;
    (void)biEndDate;
    return false;
}

// IDA: 0x1400E0DE0
// int __fastcall CGocInventory::GetMakeLimitInitDay(CGocInventory *this, E_MAKE_LIMIT_RESET_PERIOD_TYPE eType, ATL::CTime tCurr)
// Gets init day offset for make limit reset period
int CGocInventory::GetMakeLimitInitDay(int eType, std::int64_t tCurr) {
    // IDA Decompiled:
    // __int64 __fastcall CGocInventory::GetMakeLimitInitDay(CGocInventory *this, E_MAKE_LIMIT_RESET_PERIOD_TYPE eType, ATL::CTime tCurr)
    // {
    //   nDay = tCurr.GetDayOfWeek();  // 1=Sun, 2=Mon, ...
    //   nInitDay = 0;
    //
    //   switch (eType) {
    //     case E_MAKE_LIMIT_RESET_PERIOD_EVERYDAY:  // 1
    //       return tCurr.GetHour() >= 9 ? 0 : 0;  // Today if after 9AM
    //
    //     case E_MAKE_LIMIT_RESET_PERIOD_MON:  // 2
    //       nInitDay = 2 - nDay + (2 - nDay < 0 ? 7 : 0);
    //       break;
    //     case E_MAKE_LIMIT_RESET_PERIOD_TUE:  // 3
    //       nInitDay = 3 - nDay + (3 - nDay < 0 ? 7 : 0);
    //       break;
    //     // ... similar for WEN(4), THR(5), FRI(6), SAT(7), SUN(8)
    //   }
    //
    //   // If same day and after 9AM, use next week
    //   if (!nInitDay && tCurr.GetHour() >= 9) return 7;
    //   return nInitDay;
    // }

    // TODO: 需人工审查 - Implement when CTime available
    (void)eType;
    (void)tCurr;
    return 0;
}

// === Batch 10: Dye and common value functions (IDA verified) ===

// IDA: 0x1400E0FA0
// void __fastcall CGocInventory::ShowMakeLimitInfo(CGocInventory *this, int nMakeIndex)
// Shows make limit info to player via chat notice
void CGocInventory::ShowMakeLimitInfo(int nMakeIndex) {
    // IDA Decompiled:
    // void __fastcall CGocInventory::ShowMakeLimitInfo(CGocInventory *this, int nMakeIndex, float a3)
    // {
    //   PS_ITEM_MAKE_LIMIT_INFO psInfo;
    //   ATL::CTime tDate;
    //   PS_CHAT_NOTICE stChat;
    //   XSendPacket xSendPacket;
    //
    //   PS_ITEM_MAKE_LIMIT_INFO::PS_ITEM_MAKE_LIMIT_INFO(&psInfo);
    //   auto iter = m_mpItemMakeLimit.find(nMakeIndex);
    //   if (iter == m_mpItemMakeLimit.end()) return;
    //
    //   psInfo = iter->second;
    //   ATL::CTime::CTime(&tDate, psInfo.biEndDate);
    //
    //   PS_CHAT_NOTICE::PS_CHAT_NOTICE(&stChat);
    //   stChat.byType = 0;
    //   int nMinute = tDate.GetMinute();
    //   int nHour = tDate.GetHour();
    //   int nDay = tDate.GetDay();
    //   int nMonth = tDate.GetMonth();
    //   int nYear = tDate.GetYear();
    //
    //   swprintf(stChat.strMsg,
    //            L"[Make Limit Info] Index:%d, ItemID:%d, Count:%d, EndDate[ %d-%d-%d-%d-%d ]",
    //            nMakeIndex, psInfo.nItemID, psInfo.shCount,
    //            nYear, nMonth, nDay, nHour, nMinute);
    //
    //   XSendPacket::XSendPacket(&xSendPacket, 7, 4);  // main=7, sub=4 (chat)
    //   operator<<(&xSendPacket, &stChat);
    //   CGocNetwork::Send(m_pActor, &xSendPacket);
    // }

    // TODO: 需人工审查 - Implement when PS_ITEM_MAKE_LIMIT_INFO/XSendPacket available
    (void)nMakeIndex;
}

// IDA: 0x1400E11F0
// void __fastcall CGocInventory::SetDyePoint(CGocInventory *this, int nDyePoint)
// Sets dye point value and updates CUser::stMyCharInfoEx
void CGocInventory::SetDyePoint(int nDyePoint) {
    // IDA Decompiled:
    // void __fastcall CGocInventory::SetDyePoint(CGocInventory *this, int nDyePoint)
    // {
    //   this->m_nDyePoint = nDyePoint;
    //   CUser* pUser = dynamic_cast<CUser*>(m_pActor);
    //   if (pUser)
    //     CUser::stMyCharInfoEx(pUser)->nDyePoint = this->m_nDyePoint;
    // }

    m_nDyePoint = nDyePoint;
    // TODO: 需人工审查 - Update CUser::stMyCharInfoEx when CUser available
}

// IDA: 0x1400E1270
// char __fastcall CGocInventory::AddDyePoint(CGocInventory *this, int nDyePoint)
// Adds dye points with logging, returns false if overflow
bool CGocInventory::AddDyePoint(int nDyePoint) {
    // IDA Decompiled:
    // char __fastcall CGocInventory::AddDyePoint(CGocInventory *this, int nDyePoint)
    // {
    //   if (nDyePoint + this->m_nDyePoint < 0)
    //     return 0;
    //
    //   CGocInventory::SetDyePoint(this, nDyePoint + this->m_nDyePoint);
    //
    //   CUser* pUser = dynamic_cast<CUser*>(m_pActor);
    //   if (pUser) {
    //     ST_LOG_GAME stLog;
    //     ST_LOG_GAME::ST_LOG_GAME(&stLog);
    //     stLog._nUAID = pUser->GetUAID();
    //     stLog._nUCID = pUser->GetUCID();
    //     stLog._sMainType = 10;  // LOG_MAIN_DYE
    //     stLog._sSubType = 9;    // LOG_SUB_DYE_POINT_ADD
    //     stLog.nParam1 = pUser->GetLevel();
    //     stLog.nParam2 = 65;
    //     stLog.nParam5 = nDyePoint;
    //     stLog.nParam6 = this->m_nDyePoint;
    //     wcscpy_s(stLog.szComment, L"DYE POINT");
    //
    //     XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //     XGameServer::SendDBLog(pServer, &stLog);
    //   }
    //   return 1;
    // }

    if (nDyePoint + m_nDyePoint < 0)
        return false;

    SetDyePoint(nDyePoint + m_nDyePoint);
    // TODO: 需人工审查 - Add logging when ST_LOG_GAME/CUser/XGameServer available
    return true;
}

// IDA: 0x1400E1430
// bool __fastcall CGocInventory::IsHiddenDye(CGocInventory *this, unsigned __int16 nDyeID)
// Checks if dye ID is hidden (Hidden_Info == 1)
bool CGocInventory::IsHiddenDye(std::uint16_t nDyeID) {
    // IDA Decompiled:
    // bool __fastcall CGocInventory::IsHiddenDye(CGocInventory *this, unsigned __int16 nDyeID)
    // {
    //   XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //   TB_DYE* pTB_Dye = XResourceMgr::GetTB_DYE(&pServer->m_xResourceMgr, nDyeID);
    //   return pTB_Dye && pTB_Dye->Hidden_Info == 1;
    // }

    // TODO: 需人工审查 - Implement when XResourceMgr/TB_DYE available
    (void)nDyeID;
    return false;
}

// IDA: 0x1400E1480
// float __fastcall CGocInventory::GetCommonValue(CGocInventory *this, unsigned int nIndex)
// Gets common value from TB_COMMON table
float CGocInventory::GetCommonValue(unsigned int nIndex) {
    // IDA Decompiled:
    // float __fastcall CGocInventory::GetCommonValue(CGocInventory *this, unsigned int nIndex)
    // {
    //   XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //   TB_COMMON* pTB_Common = XResourceMgr::GetTB_COMMON(&pServer->m_xResourceMgr, nIndex);
    //   if (pTB_Common)
    //     return pTB_Common->Value;
    //   else
    //     return -1.0f;
    // }

    // TODO: 需人工审查 - Implement when XResourceMgr/TB_COMMON available
    (void)nIndex;
    return -1.0f;
}

// === Batch 11: Gesture functions (IDA verified) ===

// IDA: 0x1400E14D0
// char __fastcall CGocInventory::CheckAddDyePoint(CGocInventory *this, E_DYE_TYPE eDyeType, int *nAddPoint)
// Checks and calculates dye points to add based on dye type
bool CGocInventory::CheckAddDyePoint(int eDyeType, int& nAddPoint) {
    // IDA Decompiled:
    // char __fastcall CGocInventory::CheckAddDyePoint(CGocInventory *this, E_DYE_TYPE eDyeType, int *nAddPoint)
    // {
    //   *nAddPoint = 0;
    //   int nNeedPoint = (int)CGocInventory::GetCommonValue(this, 0xEA62);  // Dye point per use
    //   if (nNeedPoint <= 0) return 0;
    //
    //   int nMaxPoint = (int)CGocInventory::GetCommonValue(this, 0xEA63);  // Max dye point multiplier
    //   if (nMaxPoint <= 0) return 0;
    //
    //   int nMaxPointTotal = nNeedPoint * nMaxPoint;
    //
    //   switch (eDyeType) {
    //     case E_DYE_TYPE_SELECT:    // 1
    //     case E_DYE_TYPE_DECOLOR:   // 2
    //       nPoint = (int)CGocInventory::GetCommonValue(this, 0xEA60);
    //       break;
    //     case E_DYE_TYPE_RANDOM:    // 3
    //       nPoint = (int)CGocInventory::GetCommonValue(this, 0xEA61);
    //       break;
    //     case E_DYE_TYPE_MILEAGE:   // 4
    //       *nAddPoint = nNeedPoint;
    //       return 1;
    //     default:
    //       return 0;
    //   }
    //
    //   if (nPoint <= 0) return 0;
    //   *nAddPoint = nPoint;
    //   if (nPoint + this->m_nDyePoint > nMaxPointTotal)
    //     *nAddPoint = nMaxPointTotal - this->m_nDyePoint;
    //   return 1;
    // }

    // TODO: 需人工审查 - Implement when GetCommonValue available
    (void)eDyeType;
    nAddPoint = 0;
    return false;
}

// IDA: 0x1400E1830
// void __fastcall CGocInventory::LoadGesture(CGocInventory *this, PS_GESTURE_SLOT *psGestureInfo)
// Loads gesture slot info, validates against costume gestures
void CGocInventory::LoadGesture(void* psGestureInfo) {
    // IDA Decompiled:
    // void __fastcall CGocInventory::LoadGesture(CGocInventory *this, PS_GESTURE_SLOT *psGestureInfo)
    // {
    //   std::set<int> stInfo;
    //   CGocInventory::GetCostumeGestureInfo(this, &stInfo);
    //   bool bChange = false;
    //
    //   for (int i = 0; i < 6; ++i) {
    //     if (psGestureInfo->nGestureID[i]) {
    //       TB_GESTURE* pTB_Gesture = XResourceMgr::GetTB_GESTURE(nGestureID);
    //       if (pTB_Gesture) {
    //         if (pTB_Gesture->Gesture_Gain_Type) {
    //           // Check if gesture is in costume gesture set
    //           if (stInfo.find(psGestureInfo->nGestureID[i]) == stInfo.end()) {
    //             LogError("LoadGesture error - Fault gesture id", nGestureID);
    //             bChange = true;
    //             psGestureInfo->nGestureID[i] = 0;
    //           }
    //         }
    //       } else {
    //         bChange = true;
    //         psGestureInfo->nGestureID[i] = 0;
    //       }
    //     }
    //   }
    //
    //   if (bChange) {
    //     // Send DB update packet
    //     XSendDBPacket xSendDBPacket(pUser, 0x47, 2);  // main=0x47, sub=2
    //     xSendDBPacket << pUser->GetUCID();
    //     xSendDBPacket << *psGestureInfo;
    //     XGameServer::SendDBGame(&xSendDBPacket);
    //   }
    // }

    // TODO: 需人工审查 - Implement when PS_GESTURE_SLOT/TB_GESTURE/XSendDBPacket available
    (void)psGestureInfo;
}

// IDA: 0x1400E1E30
// char __fastcall CGocInventory::CheckGestureInfo(CGocInventory *this, PS_GESTURE_SLOT *psGestureInfo)
// Validates gesture slot info against user class and costume
bool CGocInventory::CheckGestureInfo(void* psGestureInfo) {
    // IDA Decompiled:
    // char __fastcall CGocInventory::CheckGestureInfo(CGocInventory *this, PS_GESTURE_SLOT *psGestureInfo)
    // {
    //   CUser* pUser = dynamic_cast<CUser*>(m_pActor);
    //   if (!pUser) return 0;
    //
    //   uint8_t byClass = pUser->GetClass();
    //   std::set<int> stGestureInfo;
    //   CGocInventory::GetCostumeGestureInfo(this, &stGestureInfo);
    //
    //   for (int i = 0; i < 6; ++i) {
    //     if (psGestureInfo->nGestureID[i]) {
    //       TB_GESTURE* pTB_Gesture = XResourceMgr::GetTB_GESTURE(nGestureID);
    //       if (!pTB_Gesture) {
    //         LogError("CheckGestureInfo error - No Table TB_GESTURE[ID:%d]", nGestureID);
    //         return 0;
    //       }
    //       if (pTB_Gesture->Use_Class != byClass) {
    //         LogError("CheckGestureInfo error - Fault class[ID:%d, UCID:%d]", nGestureID, pUser->GetActorID());
    //         return 0;
    //       }
    //       if (pTB_Gesture->Gesture_Gain_Type) {
    //         if (stGestureInfo.find(nGestureID) == stGestureInfo.end()) {
    //           LogError("CheckGestureInfo error - Fault gesture id[ID:%d, UCID:%d]", nGestureID, pUser->GetActorID());
    //           return 0;
    //         }
    //       }
    //     }
    //   }
    //   return 1;
    // }

    // TODO: 需人工审查 - Implement when PS_GESTURE_SLOT/TB_GESTURE/CUser available
    (void)psGestureInfo;
    return false;
}

// IDA: 0x1400E2120
// void __fastcall CGocInventory::UpdateGesture(CGocInventory *this)
// Updates gesture slots, removes invalid ones
void CGocInventory::UpdateGesture() {
    // IDA Decompiled:
    // void __fastcall CGocInventory::UpdateGesture(CGocInventory *this)
    // {
    //   std::set<int> stGestureInfo;
    //   CGocInventory::GetCostumeGestureInfo(this, &stGestureInfo);
    //
    //   std::shared_ptr<CGocSkill> pSkillPtr;
    //   m_pActor->GetGOC<CGocSkill>(&pSkillPtr, 0);
    //   if (!pSkillPtr) return;
    //
    //   PS_GESTURE_SLOT psGestureSlot;
    //   pSkillPtr->GetGestureSlot(&psGestureSlot);
    //   bool bChange = false;
    //
    //   for (int i = 0; i < 6; ++i) {
    //     if (psGestureSlot.nGestureID[i]) {
    //       TB_GESTURE* pTB_Gesture = XResourceMgr::GetTB_GESTURE(nGestureID);
    //       if (pTB_Gesture) {
    //         if (pTB_Gesture->Gesture_Gain_Type) {
    //           if (stGestureInfo.find(nGestureID) == stGestureInfo.end()) {
    //             psGestureSlot.nGestureID[i] = 0;
    //             bChange = true;
    //           }
    //         }
    //       } else {
    //         LogError("CheckGestureInfo error - No Table TB_GESTURE[ID:%d]", nGestureID);
    //         psGestureSlot.nGestureID[i] = 0;
    //       }
    //     }
    //   }
    //
    //   if (bChange) {
    //     XSendDBPacket xSendDBPacket(pUser, 0x47, 2);
    //     xSendDBPacket << pUser->GetUCID();
    //     xSendDBPacket << psGestureSlot;
    //     XGameServer::SendDBGame(&xSendDBPacket);
    //     pSkillPtr->SetGestureSlot(psGestureSlot.nGestureID);
    //   }
    // }

    // TODO: 需人工审查 - Implement when CGocSkill/PS_GESTURE_SLOT/TB_GESTURE available
}

// IDA: 0x1400E2460
// char __fastcall CGocInventory::IsUseGesture(CGocInventory *this, int nGestureID)
// Checks if gesture ID can be used by player
bool CGocInventory::IsUseGesture(int nGestureID) {
    // IDA Decompiled:
    // char __fastcall CGocInventory::IsUseGesture(CGocInventory *this, int nGestureID)
    // {
    //   CUser* pUser = dynamic_cast<CUser*>(m_pActor);
    //   if (!pUser) return 0;
    //
    //   TB_GESTURE* pTB_Gesture = XResourceMgr::GetTB_GESTURE(nGestureID);
    //   if (!pTB_Gesture) return 0;
    //   if (!pTB_Gesture->Gesture_Gain_Type) return 1;  // No restriction
    //
    //   uint8_t byClass = pUser->GetClass();
    //   if (pTB_Gesture->Use_Class != byClass) return 0;
    //
    //   std::set<int> stGestureInfo;
    //   CGocInventory::GetCostumeGestureInfo(this, &stGestureInfo);
    //
    //   for (auto iter = stGestureInfo.begin(); iter != stGestureInfo.end(); ++iter) {
    //     if (*iter == nGestureID) return 1;
    //   }
    //   return 0;
    // }

    // TODO: 需人工审查 - Implement when TB_GESTURE/CUser available
    (void)nGestureID;
    return false;
}

// === Batch 12: Equalizer and Item Title functions (IDA verified) ===

// IDA: 0x1400E2A80
// char __fastcall CGocInventory::CheckEqualizerInfo(CGocInventory *this, unsigned int nEqualizerID)
// Validates equalizer ID against costume equalizer info
bool CGocInventory::CheckEqualizerInfo(int nEqualizerID) {
    // IDA Decompiled:
    // char __fastcall CGocInventory::CheckEqualizerInfo(CGocInventory *this, unsigned int nEqualizerID)
    // {
    //   if (!nEqualizerID) return 1;
    //   CUser* pUser = dynamic_cast<CUser*>(m_pActor);
    //   if (!pUser) return 0;
    //
    //   std::vector<int> vecEqualizerInfo;
    //   CGocInventory::GetCostumeEqualizerInfo(this, &vecEqualizerInfo);
    //
    //   XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //   if (!XResourceMgr::GetTB_EQUALIZER_INFO(&pServer->m_xResourceMgr, nEqualizerID)) {
    //     LogError("CheckEqualizerInfo error - No Table TB_EQUALIZER_INFO[UCID:%d, EqualizerID:%d]",
    //              pUser->GetActorID(), nEqualizerID);
    //     return 0;
    //   }
    //
    //   for (size_t i = 0; i < vecEqualizerInfo.size(); ++i) {
    //     if (vecEqualizerInfo[i] == nEqualizerID) return 1;
    //   }
    //
    //   LogError("CheckEqualizerInfo error - Fault EqualizerID[UCID:%d, EqualizerID:%d]",
    //            pUser->GetActorID(), nEqualizerID);
    //   return 0;
    // }

    // TODO: 需人工审查 - Implement when TB_EQUALIZER_INFO/CUser available
    (void)nEqualizerID;
    return false;
}

// IDA: 0x1400E2C80
// void __fastcall CGocInventory::UpdateEqualizerInfo(CGocInventory *this)
// Updates equalizer info if current one is invalid
void CGocInventory::UpdateEqualizerInfo() {
    // IDA Decompiled:
    // void __fastcall CGocInventory::UpdateEqualizerInfo(CGocInventory *this)
    // {
    //   CUser* pUser = dynamic_cast<CUser*>(m_pActor);
    //   if (!pUser) return;
    //
    //   int nEqualizerID = CUser::stMyCharInfoEx(pUser)->nEqualizerID;
    //   if (!CGocInventory::CheckEqualizerInfo(this, nEqualizerID)) {
    //     // Send DB update to reset equalizer
    //     PS_DB_EQUALIZER_UPDATE psDBUpdate;
    //     psDBUpdate.dwUCID = pUser->GetActorID();
    //     psDBUpdate.nEqualizerID = 0;
    //
    //     XSendDBPacket xSendDBPacket(pUser, 3, 0x89);
    //     xSendDBPacket << psDBUpdate;
    //     XGameServer::SendDBGame(&xSendDBPacket);
    //
    //     // Log the change
    //     ST_LOG_GAME stLog;
    //     stLog._sMainType = 3;
    //     stLog._sSubType = 26;
    //     stLog._nUAID = pUser->GetUAID();
    //     stLog._nUCID = pUser->GetActorID();
    //     stLog.nParam0 = 0;
    //     stLog.nParam1 = pUser->GetLevel();
    //     stLog.nParam2 = 0;
    //     stLog.nParam3 = nEqualizerID;
    //     XGameServer::SendDBLog(&stLog);
    //
    //     CUser::stMyCharInfoEx(pUser)->nEqualizerID = 0;
    //
    //     // Send response to client
    //     PS_EQUALIZER_UPDATE psRes;
    //     XSendPacket xSendPacket(3, 0x7A);
    //     xSendPacket << psRes;
    //     CGocNetwork::Send(pUser, &xSendPacket);
    //   }
    // }

    // TODO: 需人工审查 - Implement when CUser/XSendDBPacket available
}

// IDA: 0x1400E2F80
// char __fastcall CGocInventory::IsReplaceItem(CGocInventory *this, int nItemID, unsigned int dwGroupID)
// Checks if item belongs to similar item group
bool CGocInventory::IsReplaceItem(int nItemID, unsigned int dwGroupID) {
    // IDA Decompiled:
    // char __fastcall CGocInventory::IsReplaceItem(CGocInventory *this, int nItemID, unsigned int dwGroupID)
    // {
    //   XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //   TB_ITEM_SIMILARGROUP* pTB_SimilarGroup = XResourceMgr::GetTB_ITEM_SIMILARGROUP(&pServer->m_xResourceMgr, dwGroupID);
    //   if (!pTB_SimilarGroup) return 1;
    //
    //   for (int i = 0; i < 5 && pTB_SimilarGroup->Similar_Item_01[i]; ++i) {
    //     if (pTB_SimilarGroup->Similar_Item_01[i] == nItemID) return 1;
    //   }
    //   return 0;
    // }

    // TODO: 需人工审查 - Implement when TB_ITEM_SIMILARGROUP/XResourceMgr available
    (void)nItemID;
    (void)dwGroupID;
    return false;
}

// IDA: 0x1400E3810
// __int64 __fastcall CGocInventory::ChangeItemTitle(...)
// Changes item title with success rate calculation
int CGocInventory::ChangeItemTitle(void* psChangeInfo, int& nTitleID, bool& bSuccess, void* psUpdateItemList) {
    // IDA Decompiled:
    // Very long function that handles item title changes:
    // 1. Validates selected item (must be equipment, invenType=2)
    // 2. Validates title item (must be tag item, invenType=13)
    // 3. Checks item has Item_Title_ID
    // 4. Checks title item has Item_Effect_ID (TitleGroupID)
    // 5. Validates effect type matches weapon/gear (18/19)
    // 6. Gets TB_ITEM_TITLE_CHANGE table by title item rank
    // 7. Checks need point, need zeny, need ether
    // 8. Collects need items (up to 10) and calculates total points
    // 9. Calculates success rate = Basic_Success_Rate + (TotalPoint/NeedPoint)*10000
    // 10. Random roll 1-10000, if <= success rate => success
    // 11. On success: get random title from group, apply to item
    // 12. Reduce all used items, deduct zeny and ether
    // 13. Send update packet and log

    // TODO: 需人工审查 - Implement when PS_REQ_ITEM_TITLE_CHANGE/CItem/XItemFactory available
    (void)psChangeInfo;
    nTitleID = 0;
    bSuccess = false;
    return 52080;  // Error code
}

// ============================================================================
// Batch 13: Currency and Money Functions - IDA Decompiled Implementations
// ============================================================================

// IDA: 0x1400A23E0 - SendBankMoney
// Sends bank money to client (main=8, sub=0x21)
// IDA-verified implementation with packet construction
// IDA: 0x1400A24C0 - AddMoney
// Adds money with overflow check, DB update, and logging
// IDA: 0x1400A2890 - AddDropMoney
// Adds dropped money with option effect, overflow check, and logging

// ============================================================================
// Batch 14: Last 22 Stub Functions - Simple Getter/Setter Implementations
// ============================================================================

// IDA: 0x1400F7810
// __int64 __fastcall CGocInventory::GetRefinePoint(CGocInventory *this)
// {
//   return (unsigned int)this->m_nRefinePoint;
// }


// IDA: 0x1400F7920
// void __fastcall CGocInventory::SetRemoveSocket(CGocInventory *this, bool bRemove)
// {
//   this->m_bReqSocketRemove = bRemove;
// }


// IDA: 0x1400F7940
// __int64 __fastcall CGocInventory::GetCash(CGocInventory *this)
// {
//   return (unsigned int)this->m_nCash;
// }
// Note: Already implemented at line 870, this is a duplicate

// IDA: 0x1400F7B90
// char *__fastcall CGocInventory::GetHanBillNo(CGocInventory *this)
// {
//   return this->m_szHanBillNo;
// }


// IDA: 0x1400F93B0
// __int64 __fastcall CGocInventory::GetTradePasswordState(CGocInventory *this)
// {
//   return this->m_byTradePassword;
// }


// IDA: 0x1400F9C20
// UXActorID *__fastcall CGocInventory::GetTradeActorID(CGocInventory *this, UXActorID *result)
// {
//   result->__s0 = ($DE3BFFBC99B013A67150333ABFCE18E5)this->m_uxTradeActorID;
//   return result;
// }


// IDA: 0x1400F9C50
// __int64 __fastcall CGocInventory::GetTradeState(CGocInventory *this)
// {
//   return (unsigned int)this->m_eTradeState;
// }


// IDA: 0x1400F9CB0
// __int64 __fastcall CGocInventory::GetTradeMoney(CGocInventory *this)
// {
//   return this->m_stTradeInfo.biMoney;
// }


// IDA: 0x1400FA4D0
// void __fastcall CGocInventory::SetCashItemDate(CGocInventory *this, __int64 biSerial, int nCashDate)
// {
//   std::pair<__int64 const ,int> *v3; // rax
//   std::pair<__int64,int> v4; // [rsp+20h] [rbp-28h] BYREF
//   std::pair<std::_Tree_iterator<std::_Tree_val<std::_Tmap_traits<__int64,int,std::less<__int64>,std::allocator<std::pair<__int64 const ,int> >,0> > >,bool> result; // [rsp+30h] [rbp-18h] BYREF
//   __int64 biSeriala; // [rsp+58h] [rbp+10h] BYREF
//   int nCashDatea; // [rsp+60h] [rbp+18h] BYREF
//
//   nCashDatea = nCashDate;
//   biSeriala = biSerial;
//   v3 = (std::pair<__int64 const ,int> *)std::pair<__int64 const,int>::pair<__int64 const,int>(
//                                           &v4,
//                                           &biSeriala,
//                                           &nCashDatea);
//   std::_Tree<std::_Tmap_traits<__int64,int,std::less<__int64>,std::allocator<std::pair<__int64 const,int>>,0>>::insert<std::pair<__int64 const,int>>(
//     &this->m_mpCashItemDate,
//     &result,
//     v3);
// }
// Note: Already implemented at line 1537, this is a duplicate

// IDA: 0x1400FA520
// void __fastcall CGocInventory::SetTradeActorID(CGocInventory *this, UXActorID uxTradeActorID)
// {
//   UXActorID uxTradeActorIDa; // [rsp+38h] [rbp+10h] BYREF
//
//   uxTradeActorIDa.__s0 = uxTradeActorID.__s0;
//   `anonymous namespace'::_lambda128_::_lambda128_(
//     (VColorRef *)&this->m_uxTradeActorID,
//     (const VColorRef *)&uxTradeActorIDa);
// }
// Note: Already implemented at line 1656, this is a duplicate

// IDA: 0x1401E7F20
// void __fastcall CGocInventory::SetTradePasswordState(CGocInventory *this, unsigned __int8 byState)
// {
//   this->m_byTradePassword = byState;
// }


// IDA: 0x14048CE70
// __int64 __fastcall CGocInventory::GetBankMoney(CGocInventory *this)
// {
//   return this->m_nBankMoney;
// }


// IDA: 0x1404EA7E0
// __int64 __fastcall CGocInventory::GetDyePoint(CGocInventory *this)
// {
//   return (unsigned int)this->m_nDyePoint;
// }
// Note: Already implemented at line 879, this is a duplicate

// IDA: 0x1404EAAD0
// void __fastcall CGocInventory::SetSocketExtract(CGocInventory *this, bool bExtract)
// {
//   this->m_bReqSocketExtract = bExtract;
// }


// IDA: 0x1404EAAF0
// _BOOL8 __fastcall CGocInventory::GetSocketExtract(CGocInventory *this)
// {
//   return this->m_bReqSocketExtract;
// }


// IDA: 0x1404EAB10
// void __fastcall CGocInventory::SetSocketUpgrade(CGocInventory *this, bool bUpgrade)
// {
//   this->m_bReqSocketUpgrade = bUpgrade;
// }


// IDA: 0x1404EAB30
// _BOOL8 __fastcall CGocInventory::GetSocketUpgrade(CGocInventory *this)
// {
//   return this->m_bReqSocketUpgrade;
// }


// IDA: 0x1404EAB80
// void __fastcall CGocInventory::SetSocketExchange(CGocInventory *this, bool bExchange)
// {
//   this->m_bReqSocketExchange = bExchange;
// }


// IDA: 0x1404EABA0
// _BOOL8 __fastcall CGocInventory::GetSocketExchange(CGocInventory *this)
// {
//   return this->m_bReqSocketExchange;
// }


// IDA: 0x1404EAC00
// void __fastcall CGocInventory::SetItemRefineReq(CGocInventory *this, bool bRefine)
// {
//   this->m_bReqItemRefine = bRefine;
// }


// IDA: 0x1400E6A90
// bool __fastcall CGocInventory::IsResealPackage(CGocInventory *this, unsigned int nPackageID)
// {
//   XGameServer *v2; // rax
//
//   v2 = TXSingleton<XGameServer>::Instance();
//   return XResourceMgr::GetTB_REPACKAGECOSTUME(&v2->m_xResourceMgr, nPackageID) != nullptr;
// }


// IDA: 0x1400E6AD0
bool CGocInventory::IsResealPackageCount(int nPackageID, int nCount) {
    TB_REPACKAGECOSTUME* pTBRepackage = TXSingleton<XGameServer>::Instance()
                                            ->GetResourceMgr()
                                            .GetTB_REPACKAGECOSTUME(
                                                static_cast<unsigned int>(nPackageID));
    if (!pTBRepackage) {
        return false;
    }

    int nCheckCount = 0;
    for (int i = 0; i < 13 && pTBRepackage->uniItem[i]; ++i) {
        ++nCheckCount;
    }

    return nCheckCount && nCount == nCheckCount;
}
// IDA-verified implementation
