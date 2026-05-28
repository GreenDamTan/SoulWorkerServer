#pragma once

#include "GOComponent.h"
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include <cstdint>

// Forward declarations
class CMover;

/**
 * @brief CGocAttendance - Game Object Component for attendance check-in system
 *
 * Handles daily attendance rewards and cumulative attendance tracking.
 * Restored from GameServer.exe IDA decompilation.
 * Address range: 0x140030270 - 0x140030316
 */
class CGocAttendance : public GOComponent {
public:
    // Constructor (0x140030270)
    CGocAttendance();

    // Destructor (0x1400302E0)
    virtual ~CGocAttendance();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Static method - Get family ID
    static int GetFamilyID() { return 15; }

    // SetAttendance (0x140030170) - Set attendance info
    void SetAttendance(PS_ATTENDANCE_INFO& stAttendance);

    // SetAttendanceContinue (0x1400301C0) - Set continue info
    void SetAttendanceContinue(PS_ATTENDANCE_CONTINUE& stContinue);

    // SetAttendancePlayTime (0x140030210) - Set play time info
    void SetAttendancePlayTime(PS_ATTENDANCE_PLAY_TIME& stPlayTime);

    // Accessors
    const PS_ATTENDANCE_INFO& GetAttendanceInfo() const { return m_stAttendanceInfo; }
    const PS_ATTENDANCE_CONTINUE& GetAttendanceContinue() const { return m_stAttendanceContinue; }
    const PS_ATTENDANCE_PLAY_TIME& GetAttendancePlayTime() const { return m_stAttendancePlayTime; }

    // Get owner mover
    CMover* GetOwnerMover() const;

protected:
    // Member variables (from IDA structure at 0x140030270)
    PS_ATTENDANCE_INFO m_stAttendanceInfo;         // Attendance info (128 bytes)
    PS_ATTENDANCE_CONTINUE m_stAttendanceContinue; // Continue info (24 bytes)
    PS_ATTENDANCE_PLAY_TIME m_stAttendancePlayTime;// Play time info (32 bytes)
};
