#include "CItem.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"

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
    , m_pItemTable(nullptr)
    , m_pClassifyTable(nullptr)
    , m_nSlot(-1)
    , m_nOrder(0)
    , m_byInvenType(0)
    , m_bEraseOnLineUp(true)
    , m_psPackageInfo(nullptr)
{
    // STItem constructor and Init are called via member initializer
    // m_stItem() calls STItem::STItem() which calls Init()
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

// ============================================================================
// Accessors (IDA verified)
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

// IDA: 0x1400F9E80
// __int64 __fastcall CItem::GetBind(CItem *this)
// { return this->m_stItem.bBindType; }
std::uint8_t CItem::GetBind() const {
    return m_stItem.bBindType;
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
    , m_stSocketData{}
{
    // Socket data array is initialized via vector constructor iterator
    // Each element is ST_ITEM_SOCKET (0x38 bytes), there are 4 elements
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
