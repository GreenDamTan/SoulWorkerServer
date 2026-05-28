#pragma once

#include "GOComponent.h"
#include <cstdint>

/**
 * @brief CGocExchange - Game Object Component for item exchange/trading
 *
 * Handles player-to-player trading and item exchange functionality.
 */
class CGocExchange : public GOComponent {
public:
    CGocExchange();
    virtual ~CGocExchange();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Exchange state
    bool IsInExchange() const;
    int GetExchangePartnerId() const;

    // Exchange operations
    bool StartExchange(int nPartnerId);
    bool CancelExchange();
    bool ConfirmExchange();
    bool LockExchange();

    // Item operations
    bool AddItemToExchange(int nSlotIndex, int nItemId, int nCount);
    bool RemoveItemFromExchange(int nSlotIndex);
    bool AddGoldToExchange(int nGold);
    int GetOfferedGold() const;

protected:
    int m_nPartnerId;
    int m_nOfferedGold;
    bool m_bExchangeActive;
};
