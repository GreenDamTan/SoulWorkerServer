#pragma once

#include "gocbase.h"
#include <cstdint>

/**
 * @brief CGocEntity - Game Object Component for entity management
 *
 * Handles entity state, lifecycle, and core entity properties.
 */
class CGocEntity : public GOComponent {
public:
    CGocEntity();
    virtual ~CGocEntity();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Entity identification
    std::uint64_t GetEntityId() const;
    void SetEntityId(std::uint64_t ullEntityId);

    // Entity state
    bool IsActive() const;
    void SetActive(bool bActive);

    bool IsVisible() const;
    void SetVisible(bool bVisible);

    // Entity lifecycle
    void Spawn();
    void Despawn();
    bool IsSpawned() const;

    // Entity type
    int GetEntityType() const;
    void SetEntityType(int nEntityType);

protected:
    std::uint64_t m_ullEntityId;
    int m_nEntityType;
    bool m_bActive;
    bool m_bVisible;
    bool m_bSpawned;
};
