#include "ItemCostume.h"
#include "Soulworker/GameServer/XGameServer/XGameServer.h"
#include "Soulworker/GameServer/XCore/XServer/XResourceMgr.h"
#include "Soulworker/GameServer/XCore/Util/LogHelper.h"

// =====================================================
// Constructor / Destructor
// =====================================================

// IDA: 0x140287F20
CItemCostume::CItemCostume()
    : CItem()
{
    // Initialize broach slots
    for (int i = 0; i < 5; ++i)
    {
        m_stItemBroach[i].dwBroachID = 0;
        m_stItemBroach[i].biSerial = 0;
        m_stItemBroach[i].bySlot = i;
        m_dwSetBuffID[i] = 0;
    }
    
    m_dwCostumeSetID = 0;
}

CItemCostume::~CItemCostume()
{
}

// =====================================================
// IDA: 0x140288080 - CanBroachActive
// =====================================================
bool CItemCostume::CanBroachActive(unsigned char bySlot)
{
    // IDA decompiled logic:
    // 1. Validate slot index (0-4)
    // 2. Check if broach exists in slot
    // 3. Check if broach can be activated based on item properties

    if (bySlot >= 5)
    {
        LogHelper::LogError("game.item", "CanBroachActive error - Invalid slot index %d", bySlot);
        return false;
    }

    // Check if broach exists
    if (m_stItemBroach[bySlot].dwBroachID == 0)
    {
        return false;
    }

    // Get broach item table
    XResourceMgr* pResourceMgr = TXSingleton<XResourceMgr>::Instance();
    if (!pResourceMgr)
        return false;

    TB_ITEM* pTBItem = pResourceMgr->GetTBItem(m_stItemBroach[bySlot].dwBroachID);
    if (!pTBItem)
    {
        LogHelper::LogError("game.item", 
            "CanBroachActive error - No TB_ITEM for broach %d", 
            m_stItemBroach[bySlot].dwBroachID);
        return false;
    }

    // Check broach type and activation conditions
    // Based on IDA: check item type, level requirements, etc.
    // For now, return true if broach exists
    return true;
}

// =====================================================
// IDA: 0x1402883A0 - SetEffect
// =====================================================
void CItemCostume::SetEffect(CUser* pUser)
{
    // IDA decompiled logic:
    // 1. Validate user
    // 2. Get costume item table
    // 3. Apply costume visual effect
    // 4. Apply costume buffs
    // 5. Apply costume set bonuses if applicable

    if (!pUser)
    {
        LogHelper::LogError("game.item", "SetEffect error - Invalid user");
        return;
    }

    TB_ITEM* pTBItem = GetItemTable();
    if (!pTBItem)
    {
        LogHelper::LogError("game.item", "SetEffect error - No item table");
        return;
    }

    // Apply costume effect
    // This would typically:
    // - Set visual appearance ID on user
    // - Apply stat bonuses
    // - Apply special effects (gesture, equalizer)

    // Check for costume set
    if (IsCostumeSet())
    {
        // Apply set bonus buffs
        for (int i = 0; i < 5; ++i)
        {
            if (m_dwSetBuffID[i] != 0)
            {
                // Apply buff to user
                // pUser->AddBuff(m_dwSetBuffID[i]);
            }
        }
    }

    // Activate broaches
    for (int i = 0; i < 5; ++i)
    {
        if (CanBroachActive(i))
        {
            // Apply broach effect
            // This would add broach stats/buffs to user
        }
    }
}

// =====================================================
// IDA: 0x140288600 - UnsetEffect
// =====================================================
void CItemCostume::UnsetEffect(CUser* pUser)
{
    // IDA decompiled logic:
    // 1. Validate user
    // 2. Remove costume visual effect
    // 3. Remove costume buffs
    // 4. Remove costume set bonuses

    if (!pUser)
    {
        LogHelper::LogError("game.item", "UnsetEffect error - Invalid user");
        return;
    }

    // Remove costume set buffs
    if (IsCostumeSet())
    {
        for (int i = 0; i < 5; ++i)
        {
            if (m_dwSetBuffID[i] != 0)
            {
                // Remove buff from user
                // pUser->RemoveBuff(m_dwSetBuffID[i]);
            }
        }
    }

    // Deactivate broaches
    for (int i = 0; i < 5; ++i)
    {
        if (m_stItemBroach[i].dwBroachID != 0)
        {
            // Remove broach effect
        }
    }
}

// =====================================================
// Broach Management
// =====================================================

const ST_ITEM_BROACH* CItemCostume::GetBroach(unsigned char bySlot) const
{
    if (bySlot >= 5)
        return nullptr;

    if (m_stItemBroach[bySlot].dwBroachID == 0)
        return nullptr;

    return &m_stItemBroach[bySlot];
}

bool CItemCostume::SetBroach(unsigned char bySlot, unsigned int dwBroachID, __int64 biSerial)
{
    if (bySlot >= 5)
    {
        LogHelper::LogError("game.item", "SetBroach error - Invalid slot %d", bySlot);
        return false;
    }

    // Check if slot is empty
    if (m_stItemBroach[bySlot].dwBroachID != 0)
    {
        LogHelper::LogError("game.item", "SetBroach error - Slot %d already occupied", bySlot);
        return false;
    }

    m_stItemBroach[bySlot].dwBroachID = dwBroachID;
    m_stItemBroach[bySlot].biSerial = biSerial;
    m_stItemBroach[bySlot].bySlot = bySlot;

    return true;
}

bool CItemCostume::RemoveBroach(unsigned char bySlot)
{
    if (bySlot >= 5)
    {
        LogHelper::LogError("game.item", "RemoveBroach error - Invalid slot %d", bySlot);
        return false;
    }

    m_stItemBroach[bySlot].dwBroachID = 0;
    m_stItemBroach[bySlot].biSerial = 0;

    return true;
}

// =====================================================
// Costume Set System
// =====================================================

bool CItemCostume::IsCostumeSet() const
{
    return m_dwCostumeSetID != 0;
}

unsigned int CItemCostume::GetCostumeSetID() const
{
    return m_dwCostumeSetID;
}

unsigned int CItemCostume::GetSetBuffID(unsigned char bySlot) const
{
    if (bySlot >= 5)
        return 0;

    return m_dwSetBuffID[bySlot];
}

// =====================================================
// IDA: 0x1400E1AE0 - GetCostumeGestureInfo (static)
// =====================================================
bool CItemCostume::GetCostumeGestureInfo(unsigned int dwItemID, unsigned int& dwGestureID)
{
    // IDA decompiled logic:
    // 1. Get TB_ITEM for costume
    // 2. Check if costume has gesture
    // 3. Return gesture ID

    XResourceMgr* pResourceMgr = TXSingleton<XResourceMgr>::Instance();
    if (!pResourceMgr)
        return false;

    TB_ITEM* pTBItem = pResourceMgr->GetTBItem(dwItemID);
    if (!pTBItem)
        return false;

    // Check item type is costume
    if (pTBItem->Item_Type != ITEM_TYPE_COSTUME)
        return false;

    // Get gesture ID from item table
    // This would be a specific field in TB_ITEM
    // For now, return false as placeholder
    dwGestureID = 0;
    return false;
}

// =====================================================
// IDA: 0x1400E2600 - GetCostumeEqualizerInfo (static)
// =====================================================
bool CItemCostume::GetCostumeEqualizerInfo(unsigned int dwItemID, unsigned int& dwEqualizerID)
{
    // IDA decompiled logic:
    // 1. Get TB_ITEM for costume
    // 2. Check if costume has equalizer
    // 3. Return equalizer ID

    XResourceMgr* pResourceMgr = TXSingleton<XResourceMgr>::Instance();
    if (!pResourceMgr)
        return false;

    TB_ITEM* pTBItem = pResourceMgr->GetTBItem(dwItemID);
    if (!pTBItem)
        return false;

    // Check item type is costume
    if (pTBItem->Item_Type != ITEM_TYPE_COSTUME)
        return false;

    // Get equalizer ID from item table
    // This would be a specific field in TB_ITEM
    // For now, return false as placeholder
    dwEqualizerID = 0;
    return false;
}
