#include "GocInventory.h"

CGocInventory::CGocInventory()
    : GOComponent(E_GOC_TYPE_NONE) // Will need custom type for inventory
    , m_nCash(0)
    , m_nMoney(0)
    , m_nInventorySize(0)
    , m_nUsedSlots(0)
    , m_nCurrentWeight(0)
    , m_nMaxWeight(0)
{
}

CGocInventory::~CGocInventory() {
    // TODO: Cleanup if needed
}

bool CGocInventory::Initialize() {
    return GOComponent::Initialize();
}

void CGocInventory::Shutdown() {
    GOComponent::Shutdown();
}

void CGocInventory::Update(float fDeltaTime) {
    GOComponent::Update(fDeltaTime);
}

int CGocInventory::GetCash() const {
    return m_nCash;
}

void CGocInventory::SetCash(int nCash) {
    m_nCash = nCash;
}

void CGocInventory::AddCash(int nAmount) {
    m_nCash += nAmount;
}

bool CGocInventory::SubtractCash(int nAmount) {
    if (m_nCash >= nAmount) {
        m_nCash -= nAmount;
        return true;
    }
    return false;
}

int CGocInventory::GetMoney() const {
    return m_nMoney;
}

void CGocInventory::SetMoney(int nMoney) {
    m_nMoney = nMoney;
}

void CGocInventory::AddMoney(int nAmount) {
    m_nMoney += nAmount;
}

bool CGocInventory::SubtractMoney(int nAmount) {
    if (m_nMoney >= nAmount) {
        m_nMoney -= nAmount;
        return true;
    }
    return false;
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
    // TODO: Implement item lookup
    (void)nItemId;
    return false;
}

int CGocInventory::GetItemCount(int nItemId) const {
    // TODO: Implement item count lookup
    (void)nItemId;
    return 0;
}

bool CGocInventory::AddItem(int nItemId, int nCount) {
    // TODO: Implement item addition
    (void)nItemId;
    (void)nCount;
    return false;
}

bool CGocInventory::RemoveItem(int nItemId, int nCount) {
    // TODO: Implement item removal
    (void)nItemId;
    (void)nCount;
    return false;
}

bool CGocInventory::MoveItem(int nFromSlot, int nToSlot) {
    // TODO: Implement item movement
    (void)nFromSlot;
    (void)nToSlot;
    return false;
}

int CGocInventory::GetEquippedItem(int nEquipSlot) const {
    // TODO: Implement equipped item lookup
    (void)nEquipSlot;
    return 0;
}

bool CGocInventory::EquipItem(int nSlotIndex, int nEquipSlot) {
    // TODO: Implement item equipping
    (void)nSlotIndex;
    (void)nEquipSlot;
    return false;
}

bool CGocInventory::UnequipItem(int nEquipSlot) {
    // TODO: Implement item unequipping
    (void)nEquipSlot;
    return false;
}

void CGocInventory::SortInventory() {
    // TODO: Implement inventory sorting
}

void CGocInventory::ClearInventory() {
    // TODO: Implement inventory clearing
}

int CGocInventory::GetCurrentWeight() const {
    return m_nCurrentWeight;
}

int CGocInventory::GetMaxWeight() const {
    return m_nMaxWeight;
}

bool CGocInventory::IsOverencumbered() const {
    return m_nCurrentWeight > m_nMaxWeight;
}

bool CGocInventory::IsQuestItem(int nItemId) const {
    // TODO: Implement quest item check
    (void)nItemId;
    return false;
}

int CGocInventory::GetQuestItemCount() const {
    // TODO: Implement quest item count
    return 0;
}
