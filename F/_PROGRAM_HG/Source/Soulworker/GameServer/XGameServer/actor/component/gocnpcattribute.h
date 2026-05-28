#pragma once

#include "gocbase.h"
#include <cstdint>

/**
 * @brief CGocNpcAttribute - Game Object Component for NPC attribute system
 *
 * Handles NPC-specific attributes, stats, and NPC-related functionality.
 */
class CGocNpcAttribute : public GOComponent {
public:
    CGocNpcAttribute();
    virtual ~CGocNpcAttribute();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // NPC info
    int GetNpcId() const;
    int GetNpcType() const;
    int GetNpcLevel() const;
    bool IsNpc() const;

    // NPC stats
    int GetNpcHp() const;
    int GetNpcMaxHp() const;
    int GetNpcAttack() const;
    int GetNpcDefense() const;

    // NPC behavior
    void SetNpcAggroTarget(int nTargetId);
    int GetNpcAggroTarget() const;
    bool HasAggro() const;

protected:
    int m_nNpcId;
    int m_nNpcType;
    int m_nNpcLevel;
    int m_nAggroTargetId;
};
