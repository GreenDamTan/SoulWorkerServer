#include "gocentity.h"

CGocEntity::CGocEntity()
    : GOComponent(E_GOC_TYPE_NONE)
    , m_ullEntityId(0)
    , m_nEntityType(0)
    , m_bActive(false)
    , m_bVisible(true)
    , m_bSpawned(false)
{
}

CGocEntity::~CGocEntity() {
    // TODO: Cleanup if needed
}

bool CGocEntity::Initialize() {
    return GOComponent::Initialize();
}

void CGocEntity::Shutdown() {
    GOComponent::Shutdown();
}

void CGocEntity::Update(float fDeltaTime) {
    GOComponent::Update(fDeltaTime);
}

std::uint64_t CGocEntity::GetEntityId() const {
    return m_ullEntityId;
}

void CGocEntity::SetEntityId(std::uint64_t ullEntityId) {
    m_ullEntityId = ullEntityId;
}

bool CGocEntity::IsActive() const {
    return m_bActive;
}

void CGocEntity::SetActive(bool bActive) {
    m_bActive = bActive;
}

bool CGocEntity::IsVisible() const {
    return m_bVisible;
}

void CGocEntity::SetVisible(bool bVisible) {
    m_bVisible = bVisible;
}

void CGocEntity::Spawn() {
    // TODO: Implement spawn logic
    m_bSpawned = true;
}

void CGocEntity::Despawn() {
    // TODO: Implement despawn logic
    m_bSpawned = false;
}

bool CGocEntity::IsSpawned() const {
    return m_bSpawned;
}

int CGocEntity::GetEntityType() const {
    return m_nEntityType;
}

void CGocEntity::SetEntityType(int nEntityType) {
    m_nEntityType = nEntityType;
}
