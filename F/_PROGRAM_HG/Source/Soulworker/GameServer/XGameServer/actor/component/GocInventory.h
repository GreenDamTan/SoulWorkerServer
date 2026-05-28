#pragma once

#include "GOComponent.h"
#include <cstdint>

/**
 * @brief CGocInventory - Game Object Component for actor inventory
 *
 * Handles actor inventory, items, cash, money, etc.
 * Estimated ~100 functions based on PDB evidence.
 */
class CGocInventory : public GOComponent {
public:
    CGocInventory();
    virtual ~CGocInventory();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Currency functions
    int GetCash() const;
    void SetCash(int nCash);
    void AddCash(int nAmount);
    bool SubtractCash(int nAmount);

    int GetMoney() const;
    void SetMoney(int nMoney);
    void AddMoney(int nAmount);
    bool SubtractMoney(int nAmount);

    // Inventory management
    int GetInventorySize() const;
    int GetUsedSlots() const;
    int GetFreeSlots() const;

    // Item functions
    bool HasItem(int nItemId) const;
    int GetItemCount(int nItemId) const;
    bool AddItem(int nItemId, int nCount);
    bool RemoveItem(int nItemId, int nCount);
    bool MoveItem(int nFromSlot, int nToSlot);

    // Equipment functions
    int GetEquippedItem(int nEquipSlot) const;
    bool EquipItem(int nSlotIndex, int nEquipSlot);
    bool UnequipItem(int nEquipSlot);

    // Inventory operations
    void SortInventory();
    void ClearInventory();

    // Weight/encumbrance
    int GetCurrentWeight() const;
    int GetMaxWeight() const;
    bool IsOverencumbered() const;

    // Quest items
    bool IsQuestItem(int nItemId) const;
    int GetQuestItemCount() const;

protected:
    // Member variables (estimated layout)
    int m_nCash;
    int m_nMoney;
    int m_nInventorySize;
    int m_nUsedSlots;
    int m_nCurrentWeight;
    int m_nMaxWeight;
};
