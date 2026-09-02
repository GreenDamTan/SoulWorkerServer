#pragma once

#include <cstdint>
#include <memory>

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

    // PDB fieldlist 0x3F9A list[6]: Finalize INTRODUCING VIRTUAL (vfptr offset 8,
    // publics ?Finalize@GOComponent@@UEAAXXZ RVA 0x1AB740), PDB vtable 槽 1。
    // 原始 vtable: slot0=~GOComponent(vecDelDtor), slot1=Finalize。
    // 注意: 活跃工程此前的 Initialize/Shutdown/Update 臆造虚槽被 26 个子类
    // override 依赖, 全量 vtable 槽位对齐留待 GOComponent ABI 批次统一处理,
    // 此处追加声明保证 CreateMonster/GocHelper 链的 Finalize 调用可编译。
    virtual void Finalize() {}

    // Owner actor (CMover in GameServer)
    CMover* GetOwnerGO() const { return m_pOwner; }
    void SetOwnerGO(CMover* pOwner) { m_pOwner = pOwner; }

    // Template functions for component registration
    // IDA pattern: ??$Register@V{ComponentType}@@@GOComponent@@SAXPEAVCMover@@V?$shared_ptr@V{ComponentType}@@@tr1@std@@@Z
    template<typename T>
    static void Register(CMover* pOwner, std::shared_ptr<T> pComponent);

    // IDA pattern: ??$CreateAndRegister@V{ComponentType}@@@GOComponent@@SA?AV?$shared_ptr@V{ComponentType}@@@tr1@std@@PEAVCMover@@@Z
    template<typename T>
    static std::shared_ptr<T> CreateAndRegister(CMover* pOwner);

protected:
    // IDA: offset 8, size 8
    CMover* m_pOwner;
};

static_assert(sizeof(GOComponent) >= 16, "GOComponent size check - at least 16 bytes expected");

// Forward declaration for CMover::SetGOC
class CMover;
