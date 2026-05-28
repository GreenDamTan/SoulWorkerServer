#pragma once

#include "GOComponent.h"
#include <cstdint>

/**
 * @brief CGocMyroom - Game Object Component for personal room/housing system
 *
 * Handles player room customization, furniture, and room-related functionality.
 */
class CGocMyroom : public GOComponent {
public:
    CGocMyroom();
    virtual ~CGocMyroom();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Room info
    int GetRoomId() const;
    int GetRoomTheme() const;
    bool IsInRoom() const;

    // Room operations
    bool EnterRoom();
    bool LeaveRoom();
    bool SetRoomTheme(int nThemeId);

    // Furniture operations
    bool PlaceFurniture(int nFurnitureId, int nX, int nY);
    bool RemoveFurniture(int nSlotIndex);
    int GetFurnitureCount() const;
    int GetMaxFurniture() const;

protected:
    int m_nRoomId;
    int m_nRoomTheme;
    int m_nFurnitureCount;
};
