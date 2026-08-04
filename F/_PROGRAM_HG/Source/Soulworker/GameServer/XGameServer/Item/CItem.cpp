#include "CItem.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAttribute.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocInventory.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocQuest.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include <cmath>

// ============================================================================
// CItem - Item base class for GameServer
//
// IDA Evidence from GameServer.exe (port 10004):
// - Constructor: 0x140281950 - Initializes vtable, STItem, m_nSlot=-1, m_nOrder=0,
//                               m_bEraseOnLineUp=1, PS_ITEM_PACKAGE, then calls STItem::Init
// - Destructor: 0x1400FA350 - Sets vtable, calls PS_ITEM_PACKAGE destructor
// - GetSlot: 0x1400264F0 - Returns m_nSlot
// - GetCount: 0x140074910 - Returns m_stItem.sCount
// - GetItem: 0x140074920 - Returns STItem copy
// - GetInvenType: 0x1400266E0 - Returns m_byInvenType
// - SetCount: 0x140082D00 - Sets m_stItem.sCount
// - GetItemTable: 0x14009EF20 - Returns m_pItemTable
// - GetClassifyTable: 0x14009EF40 - Returns m_pClassifyTable
// - CanUse: 0x1400FA1F0 - Virtual, checks m_pClassifyTable->GroupID == 17
// ============================================================================

// ============================================================================
// CItem Implementation
// ============================================================================

// IDA: 0x140281950
// CItem *__fastcall CItem::CItem(CItem *this)
// {
//   this->__vftable = (CItem_vtbl *)&CItem::`vftable';
//   STItem::STItem(&this->m_stItem);
//   this->m_nSlot = -1;
//   this->m_nOrder = 0;
//   this->m_bEraseOnLineUp = 1;
//   PS_ITEM_PACKAGE::PS_ITEM_PACKAGE(&this->m_psPackageInfo);
//   STItem::Init(&this->m_stItem);
//   return this;
// }
CItem::CItem()
    : m_stItem()
    , m_nSlot(-1)
    , m_nOrder(0)
    , m_bEraseOnLineUp(true)
    , m_psPackageInfo()
{
    m_stItem.Init();
}

// IDA: 0x1400FA350
// void __fastcall CItem::~CItem(CItem *this)
// {
//   this->__vftable = (CItem_vtbl *)&CItem::`vftable';
//   PS_PROFILE_PHOTO_LOAD::~PS_PROFILE_PHOTO_LOAD((STMonterGroupMonsterInfo *)&this->m_psPackageInfo);
// }
CItem::~CItem() {
    // Note: The IDA decompiler shows a confusing cast, but the actual destructor
    // being called is for PS_ITEM_PACKAGE (or a related structure)
    // The m_psPackageInfo appears to be embedded, not a pointer
    // For now, we just let the members destruct naturally
}

// ============================================================================
// Virtual functions
// ============================================================================

// IDA: 0x1400FA1F0
// _BOOL8 __fastcall CItem::CanUse(CItem *this)
// {
//   return this->m_pClassifyTable->GroupID == 17;
// }
bool CItem::CanUse() {
    if (m_pClassifyTable) {
        return m_pClassifyTable->GroupID == 17;
    }
    return false;
}

// IDA: 0x1402819E0
// char __fastcall CItem::Init(CItem *this, STItem *stItem)
// Initializes table pointers and resets per-instance package/title state.
bool CItem::Init(STItem stItem) {
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    m_pItemTable = pGameServer->GetResourceMgr().GetTB_ITEM(stItem.nItemID);
    if (!m_pItemTable) {
        LogHelper::LogError("game.item",
                            "Init error - No Table TB_ITEM[ ItemID:%d ] ( %d ) ",
                            stItem.nItemID,
                            18);
        return false;
    }

    pGameServer = TXSingleton<XGameServer>::Instance();
    m_pClassifyTable = pGameServer->GetResourceMgr().GetTB_ITEM_CLASSIFY(
        m_pItemTable->Item_Classify_Index);
    if (!m_pClassifyTable) {
        LogHelper::LogError("game.item",
                            "Init error - No Table TB_ITEM_CLASSIFY[ Index:%d ] ( %d )",
                            m_pItemTable->Item_Classify_Index,
                            25);
        return false;
    }

    m_stItem = stItem;
    m_fCurEnduranceRate = 0.0f;
    m_bEraseOnLineUp = false;
    for (int i = 0; i < 2; ++i) {
        m_nTitleValue[i] = 0;
    }
    m_psPackageInfo.biPackageSerial = 0;
    m_psPackageInfo.vecInfo.clear();
    return true;
}

// IDA: 0x140281B50
// void __fastcall CItem::SetOrder(CItem *this, int nMyClass)
// Sets item order for sorting based on item properties and player class
void CItem::SetOrder(int nMyClass) {
    if (m_pClassifyTable && m_pItemTable) {
        int nClass = m_pItemTable->Item_Limit_Class;
        if (nMyClass == nClass) {
            nClass = 0;
        }
        // Calculate order value based on item properties
        std::int64_t v3 = 100000000000LL * (9 - nClass)
                        + 10000000000000LL * m_pItemTable->Item_Lv
                        + 1000000000000000LL * m_pItemTable->Item_Rank
                        + 100000000000000000LL * m_pClassifyTable->Classify_Align_Type;
        m_nOrder = m_pItemTable->Item_ID + 1000000000 * m_pClassifyTable->Item_Slot_Type + v3;
    }
}

// IDA: 0x140281C90
// PDB folds this empty base body with UnsetEffect; both virtual slots are required.
void CItem::SetEffect(CMover* pObject, bool bSend, std::uint8_t bySetCount) {
}

// IDA: 0x140281C90
// void __fastcall CItem::UnsetEffect(CItem *this, CMover *pObject, bool bSend, unsigned __int8 bySetCount)
// Virtual stub - removes item effects from mover
void CItem::UnsetEffect(CMover* pObject, bool bSend, std::uint8_t bySetCount) {
    // Base class stub - do nothing
    // Derived classes (CItemEquip) override this
}

// IDA: 0x1400FA230
// bool __fastcall CItem::SetSocketItem(CItem *this, ST_ITEM_SOCKET *stItemSocket, bool bLoad)
// Virtual stub - sets socket item data
bool CItem::SetSocketItem(ST_ITEM_SOCKET stItemSocket, bool bLoad) {
    // Base class stub - return false
    return false;
}

// IDA: 0x1400FA250
// void __fastcall CItem::SetSocketEffect(CItem *this, CMover *pObject, bool bSend)
// Virtual stub - applies socket effects to mover
void CItem::SetSocketEffect(CMover* pObject, std::uint8_t bySocketPos) {
    // Base class stub - do nothing
}

// IDA: 0x1400FA260
// ST_ITEM_SOCKET *__fastcall CItem::GetSocketItem(CItem *this, unsigned __int8 bySocketPos)
// Virtual stub - gets socket item data
ST_ITEM_SOCKET* CItem::GetSocketItem(std::uint8_t bySocketPos) {
    // Base class stub - return nullptr
    return nullptr;
}

// IDA: 0x1400FA270
// void __fastcall CItem::CompareEquipedSocket(CItem *this, int nSocketType, int nPropertyType, unsigned __int8 bySocketPos)
// Virtual stub - compares equipped socket
void CItem::CompareEquipedSocket(int nSocketType, int nPropertyType, std::uint8_t& bySocketPos) {
    // Base class stub - do nothing
}

// IDA: 0x1400FA290
// bool __fastcall CItem::IsOpposite(CItem *this, int nType, int nCompareType)
// Virtual stub - checks if types are opposite
bool CItem::IsOpposite(int nType, int nCompareType) {
    // Base class stub - return false
    return false;
}

// IDA: 0x1400FA2B0
// void __fastcall CItem::SetBroach(CItem *this, ST_ITEM_BROACH *stBroach)
// Virtual stub - sets broach data
void CItem::SetBroach(ST_ITEM_BROACH stBroach) {
    // Base class stub - do nothing
}

// IDA: 0x1400FA2D0
// bool __fastcall CItem::CanBroachActive(CItem *this, unsigned __int8 byCheckRank)
// Virtual stub - checks if broach can be activated
bool CItem::CanBroachActive(std::uint8_t byCheckRank) {
    // Base class stub - return false
    return false;
}

// IDA: 0x1400FA2E0
// bool __fastcall CItem::CanBroachEquip(CItem *this, unsigned __int8 byEquipPos, unsigned int dwClassifyIndex)
// Virtual stub - checks if broach can be equipped
bool CItem::CanBroachEquip(std::uint8_t byEquipPos, std::uint32_t dwClassifyIndex) {
    // Base class stub - return false
    return false;
}

// IDA: 0x1400FA300
// void __fastcall CItem::GetBroachInfo(CItem *this, ST_ITEM_BROACH *stBroach, int nIndex, int *nResult)
// Virtual stub - gets broach info
void CItem::GetBroachInfo(ST_ITEM_BROACH& stBroach, int nIndex, int& nResult) {
    // Base class stub - do nothing
}

// CItem vtable +0xC0 (0x140B71488) points at the shared no-op COMDAT
// 0x14018F110; the PDB GetBroachList symbol folds into this body.
void CItem::GetBroachList(PS_ITEM_BROACH_LIST&) {
}

// CItem vtable +0x110 (0x140B714D8) points at the shared zero-return COMDAT
// 0x1400FA340; the PDB GetSetBuffID symbol folds into this body.
std::uint32_t CItem::GetSetBuffID(int) {
    return 0;
}

// IDA: 0x140281D50
// void __fastcall CItem::SetEnduranceEffect(CItem *this, CMover *pObject, bool bSend, unsigned __int8 byPrevEndurance)
// Sets endurance effect on mover (complex function with stat calculations)
void CItem::SetEnduranceEffect(CMover* pObject, bool bSend, std::uint8_t byPrevEndurance) {
    // TODO: 需要实现完整的耐久度效果计算
    // 这是一个复杂的函数，涉及：
    // 1. 检查物品槽位类型（武器、防具等）
    // 2. 获取 TB_ITEM_REPAIR 表
    // 3. 计算耐久度惩罚对属性的影响
    // 4. 更新攻击力、防御力、魔法攻击、魔法防御等属性
    // 5. 更新选项效果
    // 6. 更新强化效果
    // 7. 更新 Socket 效果
    // 简化实现：暂不处理
}

// IDA: 0x1402825C0
// void __fastcall CItem::SetEffectSetItem(CItem *this, CMover *pObject, unsigned __int8 bySetCount, bool bChange)
// Applies set item effects when wearing multiple pieces of a set
void CItem::SetEffectSetItem(CMover* pObject, std::uint8_t bySetCount, bool bChange) {
    // TODO: 需要实现套装效果
    // 简化实现：暂不处理
}

// IDA: 0x140282930
// void __fastcall CItem::UnSetEffectSetItem(CItem *this, CMover *pObject, unsigned __int8 bySetCount, bool bChange)
// Removes set item effects
void CItem::UnSetEffectSetItem(CMover* pObject, std::uint8_t bySetCount, bool bChange) {
    // TODO: 需要实现移除套装效果
    // 简化实现：暂不处理
}

// IDA: 0x140282CF0
// void __fastcall CItem::UnSealDecEffect(CItem *this, unsigned __int8 byValue)
// Decreases seal count
void CItem::UnSealDecEffect(std::uint8_t byValue) {
    if (m_stItem.bySealCount >= static_cast<int>(byValue)) {
        m_stItem.bySealCount -= byValue;
    } else {
        m_stItem.bySealCount = 0;
    }
    ++m_stItem.bySealDelCount;
}

// IDA: 0x140282D50
// void __fastcall CItem::RestoreDecEffect(CItem *this, unsigned __int8 byValue)
// Decreases restore count
void CItem::RestoreDecEffect(std::uint8_t byValue) {
    if (m_stItem.byRestoreCount >= static_cast<int>(byValue)) {
        m_stItem.byRestoreCount -= byValue;
    } else {
        m_stItem.byRestoreCount = 0;
    }
}

// IDA: 0x140282DA0
// void __fastcall CItem::UpgradeDecEffect(CItem *this, unsigned __int8 byValue)
// Decreases upgrade count
void CItem::UpgradeDecEffect(std::uint8_t byValue) {
    if (m_stItem.byUpgradeCount >= static_cast<int>(byValue)) {
        m_stItem.byUpgradeCount -= byValue;
    } else {
        m_stItem.byUpgradeCount = 0;
    }
}

// IDA: 0x140282DF0
// void __fastcall CItem::UpdateSkillOptionEffectItem(CItem *this, bool bSet, CMover *pObject, int nIndex)
// Updates skill option effects on item
void CItem::UpdateSkillOptionEffectItem(bool bSet, CMover* pObject, int nIndex) {
    // TODO: 需要实现技能选项效果更新
    // 简化实现：暂不处理
}

// IDA: 0x1402830F0
// void __fastcall CItem::UpdateSkillOptionEffectSetItem(CItem *this, bool bSet, unsigned __int8 bySetCount, CMover *pObject, int nIndex)
// Updates skill option effects for set items
void CItem::UpdateSkillOptionEffectSetItem(bool bSet, std::uint8_t bySetCount, CMover* pObject, int nIndex) {
    // TODO: 需要实现套装技能选项效果更新
    // 简化实现：暂不处理
}

// IDA: 0x140283430
// void __fastcall CItem::UpdateSkillOptionEffectItemPart(CItem *this, int nSkillGroupID, CMover *pObject)
// Updates skill option effect for specific skill group
void CItem::UpdateSkillOptionEffectItemPart(int nSkillGroupID, CMover* pObject) {
    // TODO: 需要实现技能选项效果部分更新
    // 需要 CGocAttribute::ClearSkillOptionEffectPart
    // 简化实现：暂不处理
}

// IDA: 0x140283560
// void __fastcall CItem::SetEffectTitleItemValue(CItem *this, bool bWeapon, float fValue, bool bAdd)
// Sets title item value effect
void CItem::SetEffectTitleItemValue(bool bWeapon, float fValue, bool bAdd) {
    if (bAdd) {
        if (bWeapon) {
            m_nTitleValue[0] = static_cast<int>(static_cast<float>(m_nTitleValue[0]) + fValue);
        } else {
            m_nTitleValue[1] = static_cast<int>(static_cast<float>(m_nTitleValue[1]) + fValue);
        }
    } else if (bWeapon) {
        m_nTitleValue[0] = static_cast<int>(static_cast<float>(m_nTitleValue[0])
            + floor(static_cast<float>(static_cast<float>(m_stItem.nAttack) * static_cast<float>(fValue * 0.01f)) + 0.5f));
    } else {
        m_nTitleValue[1] = static_cast<int>(static_cast<float>(m_nTitleValue[1])
            + floor(static_cast<float>(static_cast<float>(m_stItem.nDefense) * static_cast<float>(fValue * 0.01f)) + 0.5f));
    }
    // Clamp values to non-negative
    for (int i = 0; i < 2; ++i) {
        if (m_nTitleValue[i] < 0) {
            m_nTitleValue[i] = 0;
        }
    }
}

// IDA: 0x140283700
// void __fastcall CItem::SetEffectTitleItem(CItem *this, CMover *pObjectPtr, bool bEquip)
// Sets title item effects on mover
void CItem::SetEffectTitleItem(CMover* pObject, bool bEquip) {
    // TODO: 需要实现完整的称号效果设置
    // 需要 TB_ITEM_TITLE_VALUE 表和 CGocAttribute::UpdateEffectStat
    // 简化实现：暂不处理
}

// IDA: 0x140283D20
// void __fastcall CItem::SetSocketOption(CItem *this, ST_EXTEND_OPTION *stOption)
// Sets socket option data (copies 5 ST_EXTEND_OPTION structures)
void CItem::SetSocketOption(ST_EXTEND_OPTION* stOption) {
    for (int i = 0; i < 5; ++i) {
        m_stItem.stExtendOption[i] = stOption[i];
    }
}

// IDA: 0x140283D80
// char __fastcall CItem::RefineItemAbility(CItem *this, unsigned int nEffectID, bool bUsePrevent, int *nPrevValue, int *nChangedValue, bool *bPrevent)
// Refines item ability with random value generation
bool CItem::RefineItemAbility(std::uint32_t nEffectID, bool bUsePrevent, int* nPrevValue, int* nChangedValue, bool* bPrevent) {
    // TODO: 需要实现完整的精炼逻辑
    // 需要 TB_RANK_CHANGE 表和 XItemFactory::nRand
    // 简化实现：暂不处理
    *nPrevValue = 0;
    *nChangedValue = 0;
    *bPrevent = false;
    return false;
}

// IDA: 0x140284380
// void __fastcall CItem::SetPackageList(CItem *this, PS_ITEM_PACKAGE *psInfo)
// Sets package list info
void CItem::SetPackageList(PS_ITEM_PACKAGE& psInfo) {
    // TODO: 需要实现 PS_ITEM_PACKAGE 结构复制
    // 简化实现：暂不处理
}

// IDA: 0x1402843C0
// void __fastcall CItem::GetPackageInfo(CItem *this, PS_ITEM_PACKAGE *psInfo)
// Gets package info
void CItem::GetPackageInfo(PS_ITEM_PACKAGE& psInfo) {
    // TODO: 需要实现 PS_ITEM_PACKAGE 结构获取
    // 简化实现：暂不处理
}

// IDA: 0x140284460
// char __fastcall CItem::GetRevertBind(CItem *this, unsigned __int8 *byBindType, bool bEquip)
// Gets revert bind type based on current bind and equip state
bool CItem::GetRevertBind(std::uint8_t& byBindType, bool bEquip) {
    byBindType = 0;
    if (bEquip) {
        if (m_stItem.bBindType) {
            if (m_stItem.bBindType == 1) {
                byBindType = 3;
                return true;
            }
            return false;
        } else {
            byBindType = 2;
            return true;
        }
    } else if (m_stItem.bBindType == 2) {
        byBindType = 0;
        return true;
    } else if (m_stItem.bBindType == 3) {
        byBindType = 1;
        return true;
    }
    return false;
}

// IDA: 0x1404EB7E0
// __int64 __fastcall CItem::GetUpgradeLimit(CItem *this)
// { return this->m_stItem.byUpgradeLimit; }
std::uint8_t CItem::GetUpgradeLimit() const {
    return m_stItem.byUpgradeLimit;
}

// IDA: 0x1404EB7F0
// void __fastcall CItem::UpgradeLimitInc(CItem *this, unsigned __int8 byCount)
// { this->m_stItem.byUpgradeLimit += byCount; }
void CItem::UpgradeLimitInc(std::uint8_t byCount) {
    m_stItem.byUpgradeLimit += byCount;
}

// IDA: 0x140564300
// void __fastcall CItem::SetEraseLineUp(CItem *this, bool bFlag)
// { this->m_bEraseOnLineUp = bFlag; }
void CItem::SetEraseLineUp(bool bFlag) {
    m_bEraseOnLineUp = bFlag;
}

// IDA: 0x1400F9E80
// __int64 __fastcall CItem::GetBind(CItem *this)
// { return this->m_stItem.bBindType; }
std::uint8_t CItem::GetBind() const {
    return m_stItem.bBindType;
}

// IDA: 0x140284410
// _BOOL8 __fastcall CItem::IsBind(CItem *this)
// { return this->m_stItem.bBindType != 0; }
bool CItem::IsBind() const {
    return m_stItem.bBindType != 0;
}

// IDA: 0x140284440
// void __fastcall CItem::SetBind(CItem *this, unsigned __int8 eBindType)
// { this->m_stItem.bBindType = eBindType; }
void CItem::SetBind(std::uint8_t eBindType) {
    m_stItem.bBindType = eBindType;
}

// IDA: 0x140284500
// bool __fastcall CItem::IsAkashicRecordStack(CItem *this)
// {
//   if ( !this->m_pClassifyTable )
//     return 0;
//   if ( this->m_pClassifyTable->GroupID == 20 )
//     return this->m_stItem.nExp == 0;
//   return 1;
// }
bool CItem::IsAkashicRecordStack() const {
    if (!m_pClassifyTable) {
        return false;
    }
    if (m_pClassifyTable->GroupID == 20) {
        return m_stItem.nExp == 0;
    }
    return true;
}

// IDA: 0x140307550
// __int64 __fastcall CItem::GetOrder(CItem *this)
// { return this->m_nOrder; }
std::int64_t CItem::GetOrder() const {
    return m_nOrder;
}

// IDA: 0x140284350
// void __fastcall CItem::GetPackageList(CItem *this, PS_ITEM_PACKAGE_LIST *psList)
// { std::vector<PS_ITEM_PACKAGE>::push_back(&psList->vecInfo, &this->m_psPackageInfo); }
void CItem::GetPackageList(PS_ITEM_PACKAGE_LIST& psList) {
    // TODO: 需要实现 PS_ITEM_PACKAGE_LIST 结构
    // 简化实现：暂不处理
}

// IDA: 0x1403086C0
// __int64 __fastcall CItem::GetUpgrade(CItem *this)
// { return this->m_stItem.byUpgrade; }
std::uint8_t CItem::GetUpgrade() const {
    return m_stItem.byUpgrade;
}

// IDA: 0x1403086D0
// __int64 __fastcall CItem::GetDyeID(CItem *this)
// { return (unsigned int)this->m_stItem.nDyeID; }
int CItem::GetDyeID() const {
    return m_stItem.nDyeID;
}

// IDA: 0x140407130
// void __fastcall CItem::SetUpgrade(CItem *this, unsigned __int8 byUp)
// { this->m_stItem.byUpgrade = byUp; }
void CItem::SetUpgrade(std::uint8_t byUp) {
    m_stItem.byUpgrade = byUp;
}

// IDA: 0x1404EA7C0
// void __fastcall CItem::SetDyeID(CItem *this, int nDyeID)
// { this->m_stItem.nDyeID = nDyeID; }
void CItem::SetDyeID(int nDyeID) {
    m_stItem.nDyeID = nDyeID;
}

// IDA: 0x1404EA890
// void __fastcall CItem::SetItemTitle(CItem *this, int nTitleID)
// { this->m_stItem.nTitleID = nTitleID; }
void CItem::SetItemTitle(int nTitleID) {
    m_stItem.nTitleID = nTitleID;
}

// IDA: 0x1404EB050
// void __fastcall CItem::SetRestoreCount(CItem *this, unsigned __int8 byCount)
// { this->m_stItem.byRestoreCount = byCount; }
void CItem::SetRestoreCount(std::uint8_t byCount) {
    m_stItem.byRestoreCount = byCount;
}

// IDA: 0x1404EB070
// void __fastcall CItem::SetFlag(CItem *this, unsigned __int8 byFlag)
// { this->m_stItem.eFlag = byFlag; }
void CItem::SetFlag(std::uint8_t byFlag) {
    m_stItem.eFlag = byFlag;
}

// IDA: 0x1404EB090
// void __fastcall CItem::UpgradeCountInc(CItem *this)
// { ++this->m_stItem.byUpgradeCount; }
void CItem::UpgradeCountInc() {
    ++m_stItem.byUpgradeCount;
}

// IDA: 0x1404EB5C0
// __int64 __fastcall CItem::GetRestoreCount(CItem *this)
// { return this->m_stItem.byRestoreCount; }
std::uint8_t CItem::GetRestoreCount() const {
    return m_stItem.byRestoreCount;
}

// ============================================================================
// Mutators
// ============================================================================

// IDA: 0x1400264F0
// __int64 __fastcall CItem::GetSlot(CItem *this)
// { return (unsigned int)this->m_nSlot; }
int CItem::GetSlot() const {
    return m_nSlot;
}

// IDA: 0x140074910
// __int64 __fastcall CItem::GetCount(CItem *this)
// { return (unsigned int)this->m_stItem.sCount; }
int CItem::GetCount() const {
    return static_cast<int>(m_stItem.sCount);
}

// IDA: 0x140082D00
// void __fastcall CItem::SetCount(CItem *this, __int16 nCount)
// { this->m_stItem.sCount = nCount; }
void CItem::SetCount(std::int16_t nCount) {
    m_stItem.sCount = nCount;
}

// IDA: 0x1400266E0
// __int64 __fastcall CItem::GetInvenType(CItem *this)
// { return this->m_byInvenType; }
std::uint8_t CItem::GetInvenType() const {
    return m_byInvenType;
}

// IDA: 0x140074920
// STItem *__fastcall CItem::GetItem(CItem *this, STItem *result)
// {
//   _BYTE v3[120]; // [rsp+0h] [rbp-98h] BYREF
//   qmemcpy(v3, &this->m_stItem, sizeof(v3));
//   qmemcpy(result, v3, sizeof(STItem));
//   return result;
// }
STItem* CItem::GetItem(STItem* pOutItem) const {
    if (pOutItem) {
        *pOutItem = m_stItem;
    }
    return pOutItem;
}

// IDA: 0x14009EF20
// TB_ITEM *__fastcall CItem::GetItemTable(CItem *this)
// { return this->m_pItemTable; }
TB_ITEM* CItem::GetItemTable() const {
    return m_pItemTable;
}

// IDA: 0x14009EF40
// TB_ITEM_CLASSIFY *__fastcall CItem::GetClassifyTable(CItem *this)
// { return this->m_pClassifyTable; }
TB_ITEM_CLASSIFY* CItem::GetClassifyTable() const {
    return m_pClassifyTable;
}

// IDA: 0x140082D70
// __int64 __fastcall CItem::GetFlag(CItem *this)
// { return this->m_stItem.eFlag; }
std::uint8_t CItem::GetFlag() const {
    return m_stItem.eFlag;
}

// IDA: 0x140082D80
// __int64 __fastcall CItem::GetEndurance(CItem *this)
// { return this->m_stItem.byEndurance; }
std::uint8_t CItem::GetEndurance() const {
    return m_stItem.byEndurance;
}

int CItem::GetID() const {
    return m_stItem.nItemID;
}

std::int64_t CItem::GetSerial() const {
    return m_stItem.xSerial;
}

// IDA: 0x1400F8120
// __int64 __fastcall CItem::GetItemTitleID(CItem *this)
// { return (unsigned int)this->m_stItem.nTitleID; }
int CItem::GetItemTitleID() const {
    return m_stItem.nTitleID;
}

// IDA: 0x1400F8990
// __int64 __fastcall CItem::GetUseCount(CItem *this)
// { return this->m_stItem.byUseCount; }
std::uint8_t CItem::GetUseCount() const {
    return m_stItem.byUseCount;
}

// IDA: 0x1400F90A0
// __int64 __fastcall CItem::GetItemExp(CItem *this)
// { return (unsigned int)this->m_stItem.nExp; }
int CItem::GetItemExp() const {
    return m_stItem.nExp;
}

// ============================================================================
// Mutators
// ============================================================================

void CItem::SetSlot(int nSlot) {
    m_nSlot = nSlot;
}

void CItem::SetInvenType(std::uint8_t byType) {
    m_byInvenType = byType;
}

// IDA: 0x1400F77E0
// void __fastcall CItem::SetItem(CItem *this, STItem *stItem)
// { STItem::operator=(&this->m_stItem, stItem); }
void CItem::SetItem(STItem& stItem) {
    m_stItem = stItem;
}

// IDA: 0x1400F9E90
// void __fastcall CItem::SetEndurance(CItem *this, unsigned __int8 byEndurance)
// { this->m_stItem.byEndurance = byEndurance; }
void CItem::SetEndurance(std::uint8_t byEndurance) {
    m_stItem.byEndurance = byEndurance;
}

// IDA: 0x1400F9630
// void __fastcall CItem::UpdateCashDate(CItem *this, int nDate)
// { this->m_stItem.nCashDate = nDate; }
void CItem::UpdateCashDate(int nDate) {
    m_stItem.nCashDate = nDate;
}

// IDA: 0x14019D1D0
// void __fastcall CItem::SetUseCount(CItem *this, unsigned __int8 byUseCount)
// { this->m_stItem.byUseCount = byUseCount; }
void CItem::SetUseCount(std::uint8_t byUseCount) {
    m_stItem.byUseCount = byUseCount;
}

void CItem::SetItemTable(TB_ITEM* pTable) {
    m_pItemTable = pTable;
}

void CItem::SetClassifyTable(TB_ITEM_CLASSIFY* pTable) {
    m_pClassifyTable = pTable;
}

// ============================================================================
// CItemEquip Implementation
// ============================================================================

// IDA: 0x1400FA0C0
// CItemEquip *__fastcall CItemEquip::CItemEquip(CItemEquip *this)
// {
//   CItem::CItem(this);
//   this->__vftable = (CItemEquip_vtbl *)&CItemEquip::`vftable';
//   `vector constructor iterator'(this->m_stSocketData, 0x38u, 4,
//     (void *(__fastcall *)(void *))ST_ITEM_SOCKET::ST_ITEM_SOCKET);
//   return this;
// }
CItemEquip::CItemEquip()
    : CItem()
{
}

// IDA: 0x1400F5000
// void __fastcall CItemEquip::~CItemEquip(CItemEquip *this)
// {
//   this->__vftable = (CItemEquip_vtbl *)&CItemEquip::`vftable';
//   CItem::~CItem(this);
// }
CItemEquip::~CItemEquip() {
    // Base class destructor called automatically
}

// IDA: 0x1402850E0
void CItemEquip::SetEffect(CMover* pObject, bool bSend, std::uint8_t bySetCount) {
    if (!m_pItemTable || GetClassifyTable()->Item_Use_Type != 2) {
        return;
    }

    float fResult = 1.0f;
    if (m_pItemTable->Item_Endurance_Max) {
        TB_ITEM_REPAIR* pTBItemRepair = TXSingleton<XGameServer>::Instance()
            ->GetResourceMgr().GetTB_ITEM_REPAIR(m_stItem.byEndurance);
        if (pTBItemRepair && m_pItemTable->Item_Endurance_Max != m_stItem.byEndurance
            && pTBItemRepair->Dur_Penalty > m_fCurEnduranceRate) {
            fResult = (100.0f - pTBItemRepair->Dur_Penalty - m_fCurEnduranceRate) / 100.0f;
            m_fCurEnduranceRate = pTBItemRepair->Dur_Penalty;
        }
    }

    const std::shared_ptr<CGocAttribute> pAttr = pObject->GetGOC_Attribute(false);
    const float nPATK = static_cast<float>(m_stItem.nAttack) * fResult;
    pAttr->UpdateAddStat(0x15, nPATK, false);
    pAttr->UpdateAddStat(0x14, nPATK * 0.8f, false);

    const float nMATK = static_cast<float>(m_pItemTable->Item_Magic_Attack) * fResult;
    pAttr->UpdateAddStat(0x17, nMATK, false);
    pAttr->UpdateAddStat(0x16, nMATK * 0.8f, false);

    const float nPDEF = static_cast<float>(m_stItem.nDefense) * fResult;
    pAttr->UpdateAddStat(0x18, nPDEF, false);

    const float nMDEF = static_cast<float>(m_pItemTable->Item_Magic_Defense) * fResult;
    pAttr->UpdateAddStat(0x19, nMDEF, false);
    pAttr->CalculateChangedStat(bSend);

    pAttr->SetItemRateInfo(m_pClassifyTable->Item_Slot_Type,
                           nPATK,
                           nPDEF,
                           m_pItemTable->Item_Lv,
                           m_pItemTable->Item_Rank);

    for (int i = 0; i < 5; ++i) {
        const int nStat = *(&m_pItemTable->S_Option_Type_1 + i);
        if (!nStat) {
            break;
        }

        const int nClass = *(&m_pItemTable->Option_Class_1 + i);
        const float fValue = static_cast<float>(*(&m_pItemTable->S_Option_Value_1 + i)) * fResult;
        if (nClass) {
            pAttr->UpdateEffectStat(
                nClass, nStat, static_cast<float>(*(&m_pItemTable->S_Option_Value_1 + i)), false);
        } else {
            pAttr->UpdateEffectStat(nClass, nStat, fValue, false);
        }
    }

    for (int i = 0; i < 5; ++i) {
        const ST_EXTEND_OPTION stOption = m_stItem.stExtendOption[i];
        if (stOption.byType) {
            pAttr->UpdateEffectStat(
                0, stOption.byType, static_cast<float>(stOption.nOption) * fResult, false);
        }
    }

    SetEffectTitleItem(pObject, true);
    pAttr->UpdateAddStat(0x15, static_cast<float>(m_nTitleValue[0]), false);
    pAttr->UpdateAddStat(0x14, static_cast<float>(m_nTitleValue[0]), false);
    pAttr->UpdateAddStat(0x18, static_cast<float>(m_nTitleValue[1]), false);

    if (m_stItem.byUpgrade && m_stItem.byUpgrade <= 10) {
        TB_REINFORCE* pTBReinforce = TXSingleton<XGameServer>::Instance()
            ->GetResourceMgr().GetTB_REINFORCE(m_pItemTable->Item_Reinforce_ID);
        if (pTBReinforce) {
            // IDA's untyped float load aliases uniBreak[2 * byUpgrade + 8];
            // the PDB table layout resolves that address to uniStat[byUpgrade - 1].
            const float fStatRate = pTBReinforce->uniStat[m_stItem.byUpgrade - 1];
            float fUpgradeValue = (static_cast<float>(m_nTitleValue[0] + m_stItem.nAttack)
                                   * fStatRate / 100.0f) * fResult;
            pAttr->UpdateAddStat(0x15, fUpgradeValue, false);
            pAttr->UpdateAddStat(0x14, fUpgradeValue * 0.8f, false);
            pAttr->AddItemRateInfo(m_pClassifyTable->Item_Slot_Type, fUpgradeValue);

            fUpgradeValue = (static_cast<float>(m_nTitleValue[1] + m_stItem.nDefense)
                             * fStatRate / 100.0f) * fResult;
            pAttr->UpdateAddStat(0x18, fUpgradeValue, false);
            pAttr->AddItemRateInfo(m_pClassifyTable->Item_Slot_Type, fUpgradeValue);
        } else {
            LogHelper::LogError("game.contents",
                                "SetEffect error - Not Exist ReinforceID[ ReinforceID:%d ] ( %d )",
                                m_pItemTable->Item_Reinforce_ID,
                                1216);
        }

        const std::shared_ptr<CGocInventory> pInventory = pObject->GetGOC_Inventory(false);
        if (pInventory) {
            pInventory->ApplyItemReinforceOption(
                m_stItem.byUpgrade, m_pItemTable->Item_Reinforce_Option_ID, true, false);
        }
    }

    for (int i = 0; i < m_stItem.bySocketActiveCount; ++i) {
        const ST_ITEM_SOCKET& stSocket = m_stSocketData[i];
        if (stSocket.dwSocketID == -1) {
            continue;
        }

        TB_ITEM* pTBItem = TXSingleton<XGameServer>::Instance()
            ->GetResourceMgr().GetTB_ITEM(stSocket.dwSocketID);
        if (!pTBItem) {
            LogHelper::LogError("game.item",
                                "SetEffect error - Socket Option Item[ SocketID:%d ] ( %d )",
                                stSocket.dwSocketID,
                                1250);
            continue;
        }

        TB_ITEM_CLASSIFY* pTBItemClassify = TXSingleton<XGameServer>::Instance()
            ->GetResourceMgr().GetTB_ITEM_CLASSIFY(pTBItem->Item_Classify_Index);
        if (!pTBItemClassify) {
            LogHelper::LogError("game.item",
                                "SetEffect error - Socket Option Item[ SocketID:%d ] ( %d )",
                                stSocket.dwSocketID,
                                1257);
            continue;
        }

        for (int k = 0; k < 5; ++k) {
            const int nStat = *(&pTBItem->S_Option_Type_1 + k);
            if (!nStat) {
                break;
            }

            if (pTBItemClassify->SubCategoryID != 1) {
                if (m_pClassifyTable->Item_Slot_Type == 1) {
                    if (nStat < 86 || nStat > 91) {
                        continue;
                    }
                } else if (nStat < 80 || nStat > 85) {
                    continue;
                }
            }

            const int nClass = *(&pTBItem->Option_Class_1 + k);
            const float fValue = static_cast<float>(*(&pTBItem->S_Option_Value_1 + k)) * fResult;
            if (nClass) {
                pAttr->UpdateEffectStat(
                    nClass, nStat, static_cast<float>(*(&pTBItem->S_Option_Value_1 + k)), false);
            } else {
                pAttr->UpdateEffectStat(nClass, nStat, fValue, false);
            }
        }

        for (int k = 0; k < 5 && stSocket.stExtendOption[k].byType; ++k) {
            pAttr->UpdateEffectStat(0,
                                    stSocket.stExtendOption[k].byType,
                                    static_cast<float>(stSocket.stExtendOption[k].nOption) * fResult,
                                    false);
        }
    }

    if (bySetCount) {
        SetEffectSetItem(pObject, bySetCount, false);
    }
    UpdateSkillOptionEffectItem(true, pObject, 0);
    pAttr->CalculateChangedStat(bSend);
    pAttr->CalculateChangedEffect(bSend);
}

// IDA: 0x140285EE0
void CItemEquip::UnsetEffect(CMover* pObject, bool bSend, std::uint8_t bySetCount) {
    if (!m_pItemTable || GetClassifyTable()->Item_Use_Type != 2) {
        return;
    }

    float fResult = 1.0f;
    if (m_pItemTable->Item_Endurance_Max) {
        TB_ITEM_REPAIR* pTBItemRepair = TXSingleton<XGameServer>::Instance()
            ->GetResourceMgr().GetTB_ITEM_REPAIR(m_stItem.byEndurance);
        if (pTBItemRepair && m_pItemTable->Item_Endurance_Max != m_stItem.byEndurance) {
            fResult = (100.0f - pTBItemRepair->Dur_Penalty) / 100.0f;
            m_fCurEnduranceRate = 0.0f;
        }
    }

    const std::shared_ptr<CGocAttribute> pAttr = pObject->GetGOC_Attribute(false);
    const float nPATK = -static_cast<float>(m_stItem.nAttack) * fResult;
    pAttr->UpdateAddStat(0x15, nPATK, false);
    pAttr->UpdateAddStat(0x14, nPATK * 0.8f, false);

    const float nMATK = -static_cast<float>(m_pItemTable->Item_Magic_Attack) * fResult;
    pAttr->UpdateAddStat(0x17, nMATK, false);
    pAttr->UpdateAddStat(0x16, nMATK * 0.8f, false);

    const float nPDEF = -static_cast<float>(m_stItem.nDefense) * fResult;
    pAttr->UpdateAddStat(0x18, nPDEF, false);

    const float nMDEF = -static_cast<float>(m_pItemTable->Item_Magic_Defense) * fResult;
    pAttr->UpdateAddStat(0x19, nMDEF, false);
    pAttr->CalculateChangedStat(bSend);
    pAttr->UnsetItemRateInfo(m_pClassifyTable->Item_Slot_Type);

    for (int i = 0; i < 5; ++i) {
        const int nStat = *(&m_pItemTable->S_Option_Type_1 + i);
        if (!nStat) {
            break;
        }

        const int nClass = *(&m_pItemTable->Option_Class_1 + i);
        const float fValue = static_cast<float>(*(&m_pItemTable->S_Option_Value_1 + i));
        if (nClass) {
            pAttr->UpdateEffectStat(nClass, nStat, -fValue, false);
        } else {
            pAttr->UpdateEffectStat(nClass, nStat, -(fValue * fResult), false);
        }
    }

    for (int i = 0; i < 5; ++i) {
        const ST_EXTEND_OPTION stOption = m_stItem.stExtendOption[i];
        if (stOption.byType) {
            pAttr->UpdateEffectStat(
                0, stOption.byType, -static_cast<float>(stOption.nOption) * fResult, false);
        }
    }

    pAttr->UpdateAddStat(0x15, -static_cast<float>(m_nTitleValue[0]), false);
    pAttr->UpdateAddStat(0x14, -static_cast<float>(m_nTitleValue[0]), false);
    pAttr->UpdateAddStat(0x18, -static_cast<float>(m_nTitleValue[1]), false);

    if (m_stItem.byUpgrade && m_stItem.byUpgrade <= 10) {
        TB_REINFORCE* pTBReinforce = TXSingleton<XGameServer>::Instance()
            ->GetResourceMgr().GetTB_REINFORCE(m_pItemTable->Item_Reinforce_ID);
        if (pTBReinforce) {
            const float fStatRate = pTBReinforce->uniStat[m_stItem.byUpgrade - 1];
            float fUpgradeValue = -((static_cast<float>(m_nTitleValue[0] + m_stItem.nAttack)
                                     * fStatRate / 100.0f) * fResult);
            pAttr->UpdateAddStat(0x15, fUpgradeValue, false);
            pAttr->UpdateAddStat(0x14, fUpgradeValue * 0.8f, false);

            fUpgradeValue = -((static_cast<float>(m_nTitleValue[1] + m_stItem.nDefense)
                               * fStatRate / 100.0f) * fResult);
            pAttr->UpdateAddStat(0x18, fUpgradeValue, false);
        } else {
            LogHelper::LogError("game.contents",
                                "UnsetEffect error - Not Exist ReinforceID[ ReinforceID:%d ] ( %d )",
                                m_pItemTable->Item_Reinforce_ID,
                                1404);
        }

        const std::shared_ptr<CGocInventory> pInventory = pObject->GetGOC_Inventory(false);
        if (pInventory) {
            pInventory->ApplyItemReinforceOption(
                m_stItem.byUpgrade, m_pItemTable->Item_Reinforce_Option_ID, false, false);
        }
    }

    SetEffectTitleItem(pObject, false);

    for (int i = 0; i < m_stItem.bySocketActiveCount; ++i) {
        const ST_ITEM_SOCKET& stSocket = m_stSocketData[i];
        if (stSocket.dwSocketID == -1) {
            continue;
        }

        TB_ITEM* pTBItem = TXSingleton<XGameServer>::Instance()
            ->GetResourceMgr().GetTB_ITEM(stSocket.dwSocketID);
        if (!pTBItem) {
            LogHelper::LogError("game.item",
                                "UnsetEffect error - Socket Option Item[ SocketID:%d ]",
                                stSocket.dwSocketID);
            continue;
        }

        TB_ITEM_CLASSIFY* pTBItemClassify = TXSingleton<XGameServer>::Instance()
            ->GetResourceMgr().GetTB_ITEM_CLASSIFY(pTBItem->Item_Classify_Index);
        if (!pTBItemClassify) {
            LogHelper::LogError("game.item",
                                "SetEffect error - Socket Option Item[ SocketID:%d ] ( %d )",
                                stSocket.dwSocketID,
                                1444);
            continue;
        }

        for (int k = 0; k < 5; ++k) {
            const int nStat = *(&pTBItem->S_Option_Type_1 + k);
            if (!nStat) {
                break;
            }

            if (pTBItemClassify->SubCategoryID != 1) {
                if (m_pClassifyTable->Item_Slot_Type == 1) {
                    if (nStat < 86 || nStat > 91) {
                        continue;
                    }
                } else if (nStat < 80 || nStat > 85) {
                    continue;
                }
            }

            const int nClass = *(&pTBItem->Option_Class_1 + k);
            const float fValue = static_cast<float>(*(&pTBItem->S_Option_Value_1 + k)) * fResult;
            if (nClass) {
                pAttr->UpdateEffectStat(
                    nClass, nStat, -static_cast<float>(*(&pTBItem->S_Option_Value_1 + k)), false);
            } else {
                pAttr->UpdateEffectStat(nClass, nStat, -fValue, false);
            }
        }

        for (int k = 0; k < 5 && stSocket.stExtendOption[k].byType; ++k) {
            pAttr->UpdateEffectStat(0,
                                    stSocket.stExtendOption[k].byType,
                                    -static_cast<float>(stSocket.stExtendOption[k].nOption) * fResult,
                                    false);
        }
    }

    if (bySetCount) {
        UnSetEffectSetItem(pObject, bySetCount, false);
    }
    UpdateSkillOptionEffectItem(false, pObject, 0);
    pAttr->CalculateChangedStat(bSend);
    pAttr->CalculateChangedEffect(bSend);
}

// IDA: 0x1400FA050
// void __fastcall CItemEquip::SetSocketActive(CItemEquip *this, unsigned __int8 bySocketPos)
// { this->m_stItem.bySocketActiveCount = bySocketPos; }
void CItemEquip::SetSocketActive(std::uint8_t bySocketPos) {
    m_stItem.bySocketActiveCount = bySocketPos;
}

// ============================================================================
// CItemAkashic Implementation
// ============================================================================

// IDA: 0x1400F9D30
// CItemAkashic *__fastcall CItemAkashic::CItemAkashic(CItemAkashic *this)
// {
//   CItem::CItem(this);
//   this->__vftable = (CItemAkashic_vtbl *)&CItemAkashic::`vftable';
//   return this;
// }
CItemAkashic::CItemAkashic()
    : CItem()
    , m_pAkashicTable(nullptr)
{
}

// IDA: 0x1400F4F50
// void __fastcall CItemAkashic::~CItemAkashic(CItemAkashic *this)
// {
//   this->__vftable = (CItemAkashic_vtbl *)&CItemAkashic::`vftable';
//   CItem::~CItem(this);
// }
CItemAkashic::~CItemAkashic() {
    // Base class destructor called automatically
}

// IDA: 0x1400F9CF0
// _BOOL8 __fastcall CItemAkashic::CanUse(CItemAkashic *this)
// { return this->m_pClassifyTable->GroupID == 20; }
bool CItemAkashic::CanUse() {
    if (m_pClassifyTable) {
        return m_pClassifyTable->GroupID == 20;
    }
    return false;
}

// IDA: 0x1400F9CD0
// void __fastcall CItemAkashic::SetAkashicTable(CItemAkashic *this, TB_AKASHIC_RECORDS *pTBItem)
// { this->m_pAkashicTable = pTBItem; }
void CItemAkashic::SetAkashicTable(TB_AKASHIC_RECORDS* pTable) {
    m_pAkashicTable = pTable;
}

TB_AKASHIC_RECORDS* CItemAkashic::GetAkashicTable() const {
    return m_pAkashicTable;
}

// ============================================================================
// CItemCostume Implementation
// ============================================================================

// IDA: 0x140287F20
CItemCostume::CItemCostume()
    : CItem()
    , m_stItemBroach()
    , m_dwSetBuffID{}
{
}

// IDA: 0x1402883A0
void CItemCostume::SetEffect(CMover* pObject, bool bSend, std::uint8_t bySetCount) {
    if (!m_pItemTable || !m_pClassifyTable->Item_Slot_Type
        || GetClassifyTable()->Item_Use_Type != 1) {
        return;
    }

    const std::shared_ptr<CGocAttribute> pAttr = pObject->GetGOC_Attribute(false);
    for (int i = 0; i < 15; ++i) {
        if (m_stItemBroach.dwItemID[i] == -1) {
            continue;
        }

        TB_ITEM* pTBItem = TXSingleton<XGameServer>::Instance()
            ->GetResourceMgr().GetTB_ITEM(m_stItemBroach.dwItemID[i]);
        if (!pTBItem) {
            continue;
        }

        for (int k = 0; k < 5; ++k) {
            const int nStat = *(&pTBItem->S_Option_Type_1 + k);
            if (!nStat) {
                break;
            }

            const int nClass = *(&pTBItem->Option_Class_1 + k);
            const float fValue = static_cast<float>(*(&pTBItem->S_Option_Value_1 + k));
            pAttr->UpdateEffectStat(nClass, nStat, fValue, false);
        }
    }

    BroachSetEffect(pObject);
    pAttr->CalculateChangedStat(bSend);
    pAttr->CalculateChangedEffect(bSend);
}

// IDA: 0x140288600
void CItemCostume::UnsetEffect(CMover* pObject, bool bSend, std::uint8_t bySetCount) {
    if (!m_pItemTable || !m_pClassifyTable->Item_Slot_Type
        || GetClassifyTable()->Item_Use_Type != 1) {
        return;
    }

    const std::shared_ptr<CGocAttribute> pAttr = pObject->GetGOC_Attribute(false);
    for (int i = 0; i < 15; ++i) {
        if (m_stItemBroach.dwItemID[i] == -1) {
            continue;
        }

        TB_ITEM* pTBItem = TXSingleton<XGameServer>::Instance()
            ->GetResourceMgr().GetTB_ITEM(m_stItemBroach.dwItemID[i]);
        if (!pTBItem) {
            continue;
        }

        for (int k = 0; k < 5; ++k) {
            const int nStat = *(&pTBItem->S_Option_Type_1 + k);
            if (!nStat) {
                break;
            }

            const int nClass = *(&pTBItem->Option_Class_1 + k);
            const float fValue = -static_cast<float>(*(&pTBItem->S_Option_Value_1 + k));
            pAttr->UpdateEffectStat(nClass, nStat, fValue, false);
        }
    }

    ClearBroachSet(pObject);
    pAttr->CalculateChangedStat(bSend);
    pAttr->CalculateChangedEffect(bSend);
}

// IDA: 0x140288880
void CItemCostume::BroachSetEffect(CMover* pObject) {
    const std::shared_ptr<CGocAttribute> pAttr = pObject->GetGOC_Attribute(false);
    static_cast<void>(pAttr);

    for (int i = 0; i < 5; ++i) {
        int nEffectID = 0;
        int nSetCode = 0;
        int nCount = 0;
        for (; nCount < 3; ++nCount) {
            const int nItemID = m_stItemBroach.dwItemID[3 * i + nCount];
            if (nItemID < 1) {
                break;
            }

            TB_ITEM* pTBItem = TXSingleton<XGameServer>::Instance()
                ->GetResourceMgr().GetTB_ITEM(nItemID);
            if (!pTBItem || pTBItem->Item_Effect_Type != 6) {
                break;
            }

            if (nEffectID) {
                if (nEffectID != pTBItem->Item_Effect_ID) {
                    break;
                }
            } else {
                nEffectID = pTBItem->Item_Effect_ID;
            }

            nSetCode += static_cast<int>(static_cast<float>(pTBItem->Item_Rank)
                                         * std::pow(10.0f, static_cast<float>(2 - nCount)));
        }

        if (nCount == 3 && nEffectID) {
            nSetCode += 10000 * nEffectID;
            nSetCode += 1000 * (i + 1);
            TB_BROACH_SET* pTBBroachSet = TXSingleton<XGameServer>::Instance()
                ->GetResourceMgr().GetTB_BROACH_SET(static_cast<std::uint16_t>(nSetCode));
            if (pTBBroachSet) {
                if (m_dwSetBuffID[i] != pTBBroachSet->Broach_Set_Buff) {
                    if (m_dwSetBuffID[i] && m_dwSetBuffID[i] != pTBBroachSet->Broach_Set_Buff) {
                        pObject->ClearBuffStatus(m_dwSetBuffID[i], true, 0);
                        m_dwSetBuffID[i] = 0;
                    }

                    bool bShowBuff = false;
                    CUser* pUser = dynamic_cast<CUser*>(pObject);
                    if (pUser && pUser->GetActiveBroachEffect() == pTBBroachSet->Broach_Set_Buff) {
                        bShowBuff = true;
                    }

                    if (pObject->SetBuffStatus(pTBBroachSet->Broach_Set_Buff,
                                               CQuestCondition::GetQuestID(pObject->GetActorID()),
                                               bShowBuff)) {
                        m_dwSetBuffID[i] = pTBBroachSet->Broach_Set_Buff;
                    }
                }
            } else if (m_dwSetBuffID[i]) {
                pObject->ClearBuffStatus(m_dwSetBuffID[i], true, 0);
                m_dwSetBuffID[i] = 0;
            }
        } else if (m_dwSetBuffID[i]) {
            pObject->ClearBuffStatus(m_dwSetBuffID[i], true, 0);
            m_dwSetBuffID[i] = 0;
        }
    }
}

// IDA: 0x140288D20
void CItemCostume::ClearBroachSet(CMover* pObject) {
    const std::shared_ptr<CGocAttribute> pAttr = pObject->GetGOC_Attribute(false);
    static_cast<void>(pAttr);

    for (int i = 0; i < 5; ++i) {
        if (!m_dwSetBuffID[i]) {
            continue;
        }

        CUser* pUser = dynamic_cast<CUser*>(pObject);
        if (pUser && pUser->GetActiveBroachEffect() == m_dwSetBuffID[i]) {
            pUser->SetActiveBroachEffect(0);
        }
        pObject->ClearBuffStatus(m_dwSetBuffID[i], true, 0);
        m_dwSetBuffID[i] = 0;
    }
}

// IDA: 0x140287FA0
void CItemCostume::SetBroach(ST_ITEM_BROACH stBroach) {
    m_stItemBroach = stBroach;
}

// IDA: 0x140287FF0. The -1 serial is the only empty-broach sentinel.
void CItemCostume::GetBroachList(PS_ITEM_BROACH_LIST& stBroachList) {
    if (m_stItemBroach.biSerial != -1) {
        stBroachList.vecInfo.push_back(m_stItemBroach);
    }
}

// IDA: 0x140289A90. IDA treats the parameter as unsigned; reject negative
// signed callers before indexing the recovered five-entry array.
std::uint32_t CItemCostume::GetSetBuffID(int nIndex) {
    if (nIndex < 0 || nIndex > 4) {
        return 0;
    }
    return m_dwSetBuffID[nIndex];
}
