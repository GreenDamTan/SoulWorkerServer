#pragma once

#include <cstdint>

// Forward declarations
class XActor;

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
 * IDA struct GOComponent (16 bytes estimated):
 * - m_eGOCType: E_GOC_TYPE (4 bytes)
 * - m_pOwner: XActor* (8 bytes)
 * - padding (4 bytes for alignment)
 */
class GOComponent {
public:
    GOComponent()
        : m_eGOCType(E_GOC_TYPE_NONE)
        , m_pOwner(nullptr)
    {}

    explicit GOComponent(E_GOC_TYPE eType)
        : m_eGOCType(eType)
        , m_pOwner(nullptr)
    {}

    virtual ~GOComponent() = default;

    // Component type
    E_GOC_TYPE GetType() const { return m_eGOCType; }
    void SetType(E_GOC_TYPE eType) { m_eGOCType = eType; }

    // Owner actor
    XActor* GetOwner() const { return m_pOwner; }
    void SetOwner(XActor* pOwner) { m_pOwner = pOwner; }

    // Virtual interface for derived components
    virtual bool Initialize() { return true; }
    virtual void Shutdown() {}
    virtual void Update(float fDeltaTime) { (void)fDeltaTime; }

protected:
    E_GOC_TYPE m_eGOCType;
    XActor* m_pOwner;
};

static_assert(sizeof(GOComponent) >= 16, "GOComponent size check - at least 16 bytes expected");
