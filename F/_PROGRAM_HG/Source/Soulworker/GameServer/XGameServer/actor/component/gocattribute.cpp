#include "gocattribute.h"

CGocAttribute::CGocAttribute()
    : GOComponent(E_GOC_TYPE_NONE) // Will need custom type for attribute
    , m_nHP(0)
    , m_nMaxHP(0)
    , m_nFP(0)
    , m_nMaxFP(0)
    , m_nLevel(1)
    , m_nExp(0)
    , m_nGameModeState(0)
    , m_nAttackPower(0)
    , m_nDefense(0)
    , m_nCriticalRate(0)
    , m_nCriticalDamage(0)
{
}

CGocAttribute::~CGocAttribute() {
    // TODO: Cleanup if needed
}

bool CGocAttribute::Initialize() {
    return GOComponent::Initialize();
}

void CGocAttribute::Shutdown() {
    GOComponent::Shutdown();
}

void CGocAttribute::Update(float fDeltaTime) {
    GOComponent::Update(fDeltaTime);
}

int CGocAttribute::GetHP() const {
    return m_nHP;
}

int CGocAttribute::GetMaxHP() const {
    return m_nMaxHP;
}

void CGocAttribute::SetHP(int nHP) {
    m_nHP = nHP;
}

int CGocAttribute::GetFP() const {
    return m_nFP;
}

int CGocAttribute::GetMaxFP() const {
    return m_nMaxFP;
}

void CGocAttribute::SetFP(int nFP) {
    m_nFP = nFP;
}

void CGocAttribute::FPRestore() {
    m_nFP = m_nMaxFP;
}

int CGocAttribute::GetLevel() const {
    return m_nLevel;
}

void CGocAttribute::SetLevel(int nLevel) {
    m_nLevel = nLevel;
}

int CGocAttribute::GetExp() const {
    return m_nExp;
}

void CGocAttribute::SetExp(int nExp) {
    m_nExp = nExp;
}

int CGocAttribute::GetGameModeState() const {
    return m_nGameModeState;
}

void CGocAttribute::SetGameModeState(int nState) {
    m_nGameModeState = nState;
}

void CGocAttribute::ApplyModifier(int nModifierType, int nValue) {
    // TODO: Implement modifier application
    (void)nModifierType;
    (void)nValue;
}

void CGocAttribute::RemoveModifier(int nModifierType, int nValue) {
    // TODO: Implement modifier removal
    (void)nModifierType;
    (void)nValue;
}

int CGocAttribute::GetAttackPower() const {
    return m_nAttackPower;
}

int CGocAttribute::GetDefense() const {
    return m_nDefense;
}

int CGocAttribute::GetCriticalRate() const {
    return m_nCriticalRate;
}

int CGocAttribute::GetCriticalDamage() const {
    return m_nCriticalDamage;
}
