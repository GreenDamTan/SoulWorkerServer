#pragma once

#include "GOComponent.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include <cstdint>
#include <map>

/**
 * @brief ST_BOOSTER_INFO - Booster information structure
 *
 * Used for class event missions and booster tracking.
 * Defined in PSServerDB.h
 */

/**
 * @brief CGocClassEvent - Game Object Component for class event system
 *
 * Handles class-specific events, missions, and booster bonuses.
 * This component is attached to CMover (player characters) to track
 * their class event progress and booster states.
 *
 * IDA struct CGocClassEvent (64 bytes):
 * - inherits GOComponent (16 bytes) - offset 0
 * - m_mapMission: std::map<unsigned short, ST_BOOSTER_INFO> - offset 16 (24-32 bytes)
 * - m_bLoad: bool - offset ~48
 * - m_fPlayTimeUpdate: float - offset ~52
 */
class CGocClassEvent : public GOComponent {
public:
    // IDA: ??0CGocClassEvent@@QEAA@XZ (0x14004E040)
    CGocClassEvent();

    // IDA: ??1CGocClassEvent@@UEAA@XZ (0x14004E0D0)
    virtual ~CGocClassEvent();

    // IDA: ?Init@CGocClassEvent@@QEAAXXZ (0x14004E120)
    void Init();

    // IDA: ?GetFamilyID@CGocClassEvent@@SAHXZ (0x140039060)
    static int GetFamilyID();

    // IDA: ?OnUpdate@CGocClassEvent@@QEAAXM@Z (0x140187DB0)
    void OnUpdate(float fDeltaTime);

    // IDA: ?Load@CGocClassEvent@@QEAAXAEAUST_CLASS_EVENT_LIST@@@Z (0x14018E110)
    void Load(ST_CLASS_EVENT_LIST& stList);

    // IDA: ?SendLoadGameDB@CGocClassEvent@@QEAAXXZ (0x1401AB740)
    void SendLoadGameDB();

    // IDA: ?CheckClassEventMission@CGocClassEvent@@QEAA_NEH@Z (0x1400F92E0)
    bool CheckClassEventMission(unsigned char byType, int nValue);

    // IDA: ?CheckClassEventMission_other@CGocClassEvent@@QEAA_NEHHK@Z (0x14004E160)
    bool CheckClassEventMission_other(unsigned char byType, int nValue, int nReward);

    // GOComponent interface
    bool Initialize() override { return true; }
    void Shutdown() override {}
    void Update(float fDeltaTime) override { OnUpdate(fDeltaTime); }

    // Accessors
    bool IsLoaded() const { return m_bLoad; }
    float GetPlayTimeUpdate() const { return m_fPlayTimeUpdate; }
    const std::map<unsigned short, ST_BOOSTER_INFO>& GetMissionMap() const { return m_mapMission; }

protected:
    // IDA: offset 16, std::map<unsigned short, ST_BOOSTER_INFO>
    std::map<unsigned short, ST_BOOSTER_INFO> m_mapMission;

    // IDA: offset ~48
    bool m_bLoad;

    // IDA: offset ~52
    float m_fPlayTimeUpdate;
};

// PDB shows CGocClassEvent size = 64 bytes
static_assert(sizeof(CGocClassEvent) >= 48, "CGocClassEvent size check - at least 48 bytes expected");
