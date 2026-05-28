#include "GocSkill.h"

CGocSkill::CGocSkill()
    : GOComponent(E_GOC_TYPE_NONE) // Will need custom type for skill
    , m_nPassiveSkillCount(0)
    , m_nSkillCount(0)
    , m_nSkillPoints(0)
    , m_nCurrentComboCount(0)
{
}

CGocSkill::~CGocSkill() {
    // TODO: Cleanup if needed
}

bool CGocSkill::Initialize() {
    return GOComponent::Initialize();
}

void CGocSkill::Shutdown() {
    GOComponent::Shutdown();
}

void CGocSkill::Update(float fDeltaTime) {
    GOComponent::Update(fDeltaTime);
}

int CGocSkill::GetPassiveSkillCount() const {
    return m_nPassiveSkillCount;
}

void CGocSkill::SetPassiveSkillStat(int nSkillId, int nLevel) {
    // TODO: Implement passive skill stat setting
    (void)nSkillId;
    (void)nLevel;
}

int CGocSkill::GetPassiveSkillLevel(int nSkillId) const {
    // TODO: Implement passive skill level lookup
    (void)nSkillId;
    return 0;
}

int CGocSkill::GetSkillCount() const {
    return m_nSkillCount;
}

bool CGocSkill::HasSkill(int nSkillId) const {
    // TODO: Implement skill check
    (void)nSkillId;
    return false;
}

bool CGocSkill::LearnSkill(int nSkillId) {
    // TODO: Implement skill learning
    (void)nSkillId;
    return false;
}

bool CGocSkill::UpgradeSkill(int nSkillId) {
    // TODO: Implement skill upgrade
    (void)nSkillId;
    return false;
}

int CGocSkill::GetGestureSlot(int nSlotIndex) const {
    // TODO: Implement gesture slot lookup
    (void)nSlotIndex;
    return 0;
}

void CGocSkill::SetGestureSlot(int nSlotIndex, int nSkillId) {
    // TODO: Implement gesture slot setting
    (void)nSlotIndex;
    (void)nSkillId;
}

void CGocSkill::ClearGestureSlot(int nSlotIndex) {
    // TODO: Implement gesture slot clearing
    (void)nSlotIndex;
}

bool CGocSkill::IsSkillOnCooldown(int nSkillId) const {
    // TODO: Implement cooldown check
    (void)nSkillId;
    return false;
}

float CGocSkill::GetSkillCooldownTime(int nSkillId) const {
    // TODO: Implement cooldown time lookup
    (void)nSkillId;
    return 0.0f;
}

void CGocSkill::SetSkillCooldown(int nSkillId, float fTime) {
    // TODO: Implement cooldown setting
    (void)nSkillId;
    (void)fTime;
}

int CGocSkill::GetSkillPoints() const {
    return m_nSkillPoints;
}

void CGocSkill::SetSkillPoints(int nPoints) {
    m_nSkillPoints = nPoints;
}

void CGocSkill::AddSkillPoints(int nPoints) {
    m_nSkillPoints += nPoints;
}

int CGocSkill::GetCurrentComboCount() const {
    return m_nCurrentComboCount;
}

void CGocSkill::ResetCombo() {
    m_nCurrentComboCount = 0;
}

void CGocSkill::IncrementCombo() {
    m_nCurrentComboCount++;
}
