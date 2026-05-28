#pragma once

#include <cstdint>

// Forward declarations
class XActor;
class CMover;

/**
 * @brief E_GOC_TYPE - Game Object Component type enumeration
 *
 * Component types for Vision Engine's component-based architecture.
 */
enum E_GOC_TYPE : std::int32_t {
    E_GOC_TYPE_NONE = 0,
    E_GOC_TYPE_TRANSFORM = 1,
    E_GOC_TYPE_RENDER = 2,
    E_GOC_TYPE_PHYSICS = 3,
    E_GOC_TYPE_COLLISION = 4,
    E_GOC_TYPE_AI = 5,
    E_GOC_TYPE_ANIMATION = 6,
    E_GOC_TYPE_SOUND = 7,
    E_GOC_TYPE_PARTICLE = 8,
};

/**
 * @brief GOComponent - Base class for Game Object Components
 *
 * Vision Engine uses a component-based architecture where actors
 * are composed of various components (GOC) that provide specific
 * functionality.
 *
 * IDA struct GOComponent (16 bytes):
 * - __vftable: GOComponent_vtbl* (8 bytes) - offset 0
 * - m_OwnerGO: CMover* (8 bytes) - offset 8
 */
class GOComponent {
public:
    // IDA: ??0GOComponent@@QEAA@XZ
    GOComponent()
        : m_pOwner(nullptr)
    {}

    // IDA: virtual destructor
    virtual ~GOComponent() = default;

    // Virtual interface functions
    virtual bool Initialize() { return true; }
    virtual void Shutdown() {}
    virtual void Update(float fDeltaTime) { (void)fDeltaTime; }

    // Owner actor (CMover in GameServer)
    CMover* GetOwnerGO() const { return m_pOwner; }
    void SetOwnerGO(CMover* pOwner) { m_pOwner = pOwner; }

protected:
    // IDA: offset 8, size 8
    CMover* m_pOwner;
};

static_assert(sizeof(GOComponent) >= 16, "GOComponent size check - at least 16 bytes expected");
