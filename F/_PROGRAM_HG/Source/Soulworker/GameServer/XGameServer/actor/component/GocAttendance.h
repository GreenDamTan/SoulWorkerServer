#pragma once

#include "GOComponent.h"
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerAttendance.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerItem.h"  // For ST_CREATE_ITEM
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

    // LoadAttendanceInfo (0x140031340) - Load all attendance info with validation
    bool LoadAttendanceInfo(PS_ATTENDANCE_INFO& stAttendance,
                           PS_ATTENDANCE_CONTINUE& stAttendanceContinue,
                           PS_ATTENDANCE_PLAY_TIME& stAttendancePlayTime);

    // AttendanceVailidityCheck (0x1400315B0) - Validate attendance info
    bool AttendanceVailidityCheck(PS_ATTENDANCE_INFO& stAttendance);

    // AttendanceContinueVailidityCheck (0x1400317F0) - Validate continue attendance
    bool AttendanceContinueVailidityCheck(PS_ATTENDANCE_CONTINUE& stAttendance);

    // AttendancePlayTimeVailidityCheck (0x140031940) - Validate play time attendance
    bool AttendancePlayTimeVailidityCheck(PS_ATTENDANCE_PLAY_TIME& stPlayTime);

    // Init (0x140030350) - Initialize attendance component
    void Init();

    // LogOut (0x140030420) - Handle logout
    void LogOut();

    // OnUpdate (0x140030470) - Update attendance timers
    void OnUpdate();

    // InitPlayTimebyDay (0x140030540) - Initialize play time tracking
    void InitPlayTimebyDay();

    // SendDBPlayTimeByDay (0x140030590) - Send play time to DB
    void SendDBPlayTimeByDay();

    // LoadAccountPlayTimeEventReq (0x140030760) - Request account play time event
    void LoadAccountPlayTimeEventReq();

    // LoadAccountPlayTimeEvent (0x1400308B0) - Load account play time event
    void LoadAccountPlayTimeEvent(PS_PLAY_TIME_BY_ACCOUNT& stTime);

    // UpdateAccountPlayTimeEvent (0x140030B10) - Update account play time event
    void UpdateAccountPlayTimeEvent();

    // SaveAccountPlayTimeEvent (0x140031050) - Save account play time event
    void SaveAccountPlayTimeEvent();

    // ShowAccountPlayTimeEvent (0x140031200) - Show account play time event (debug)
    void ShowAccountPlayTimeEvent();

    // OnAttendance (0x140031C00) - Handle attendance check
    void OnAttendance(__int64 biCurDate);

    // OnAttendancePlayTime (0x140032550) - Handle play time attendance
    void OnAttendancePlayTime(__int64 biCurDate);

    // AttendanceReward (0x140032900) - Send attendance reward to DB
    bool AttendanceReward(std::uint32_t dwRewardID);

    // AttendanceContinueReward (0x1400333D0) - Send continue attendance reward to DB
    bool AttendanceContinueReward(std::uint32_t dwRewardItemID, std::int16_t shRewardCount);

    // AttendancePlayTimeReward (0x140033DF0) - Send play time attendance reward to DB
    bool AttendancePlayTimeReward();

    // Cheat_ShowAttendanceInfo (0x140035870) - Debug show attendance info
    void Cheat_ShowAttendanceInfo();

    // Cheat_AttendancePlayTimeUpdate (0x140036740) - Debug update play time
    void Cheat_AttendancePlayTimeUpdate(std::uint8_t byPos, int nPlaySec);

    // SendDBAttendance (0x140034d60) - Send attendance data to DB
    void SendDBAttendance();

    // SendDBAttendanceReset (0x140035260) - Send attendance reset to DB
    void SendDBAttendanceReset(std::uint32_t dwAttendanceID);

    // Cheat_AttendanceReset (0x140035430) - GM cheat reset attendance
    void Cheat_AttendanceReset();

    // Cheat_AttendanceContinueReset (0x140035490) - GM cheat reset continue attendance
    void Cheat_AttendanceContinueReset();

    // Cheat_AttendancePlayTimeReset (0x140035650) - GM cheat reset play time attendance
    void Cheat_AttendancePlayTimeReset();

    // AttendanceRewardRes (0x140032cc0) - Handle DB response for attendance reward
    // IDA: ?AttendanceRewardRes@CGocAttendance@@QEAA_NKAEAUPS_ATTENDANCE_INFO@@AEAUST_CREATE_ITEM@@@Z
    bool AttendanceRewardRes(unsigned int dwError, PS_ATTENDANCE_INFO& stAttendance, ST_CREATE_ITEM& stItemInfo);

    // AttendanceContinueRewardRes (0x1400336f0) - Handle DB response for continue reward
    // IDA: ?AttendanceContinueRewardRes@CGocAttendance@@QEAA_NKAEAUPS_ATTENDANCE_CONTINUE@@AEAUST_CREATE_ITEM@@@Z
    bool AttendanceContinueRewardRes(unsigned int dwError, PS_ATTENDANCE_CONTINUE& stAttendanceContinue, ST_CREATE_ITEM& stItemInfo);

    // AttendancePlayTimeRewardRes (0x140034170) - Handle DB response for play time reward
    // IDA: ?AttendancePlayTimeRewardRes@CGocAttendance@@QEAA_NAEAUPS_ATTENDANCE_PLAY_TIME@@AEAUST_CREATE_ITEM@@@Z
    bool AttendancePlayTimeRewardRes(PS_ATTENDANCE_PLAY_TIME& stAttendancePlayTime, ST_CREATE_ITEM& stItemInfo);

    // Accessors
    const PS_ATTENDANCE_INFO& GetAttendanceInfo() const { return m_stAttendanceInfo; }
    const PS_ATTENDANCE_CONTINUE& GetAttendanceContinue() const { return m_stAttendanceContinue; }
    const PS_ATTENDANCE_PLAY_TIME& GetAttendancePlayTime() const { return m_stAttendancePlayTime; }
    std::int64_t GetNextAttendancePlayTime() const { return m_biNextAttendancePlayTime; }

    // Get owner mover
    CMover* GetOwnerMover() const;

protected:
    // Member variables (from IDA structure at 0x140030270)
    PS_ATTENDANCE_INFO m_stAttendanceInfo;         // Attendance info (128 bytes)
    PS_ATTENDANCE_CONTINUE m_stAttendanceContinue; // Continue info (24 bytes)
    PS_ATTENDANCE_PLAY_TIME m_stAttendancePlayTime;// Play time info (32 bytes)
    std::int64_t m_biNextAttendancePlayTime = 0;   // Next attendance play time check
    std::int64_t m_biNextAttendance = 0;           // Next attendance check
    std::uint64_t m_dw64AttendanceCheckTick = 0;   // Attendance check tick
    std::uint64_t m_dw64PlayTimeByDay = 0;         // Play time by day
    std::int64_t m_nNextAccountPlayTime = 0;       // Next account play time
    std::int64_t m_nAccountPlayTimeTick = 0;       // Account play time tick
    std::uint8_t m_byAccountPlayType = 0;          // Account play type
    std::int64_t m_nPrevAccountPlayTimeTick = 0;   // Previous account play time tick
    std::int64_t m_nAccountPlayTimeDBSaveTick = 0; // Account play time DB save tick
};
