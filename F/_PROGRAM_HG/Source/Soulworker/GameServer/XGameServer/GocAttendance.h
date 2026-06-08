// CGocAttendance - Attendance and Login Reward System
// Restored from GameServer.exe IDA decompilation
// Address range: 0x140030170 - 0x14003679B

#pragma once

#include "GOComponent.h"
#include <cstdint>
#include <cstring>

// Forward declarations
class CMover;
class CUser;
class XGameServer;
class XPacket;
class XSendPacket;
class XSendDBPacket;
struct PS_ATTENDANCE_INFO;
struct PS_ATTENDANCE_CONTINUE;
struct PS_ATTENDANCE_PLAY_TIME;
struct ST_CREATE_ITEM;
struct TB_CHECK_ATTENDANCE_INFO;
struct TB_CHECK_ATTENDANCE_REWARD;
struct TB_CHECK_ATTENDANCE_STREAK;
struct TB_CHECK_ACCESS_REWARD;
struct TB_ITEM;

/**
 * @brief PS_ATTENDANCE_INFO - Attendance information packet structure
 * 
 * Tracks daily attendance status for a user.
 * Size: 128 bytes (from IDA analysis)
 */
struct PS_ATTENDANCE_INFO {
    std::uint8_t byApplyAttendance = 0;        // Whether attendance is enabled
    std::uint32_t dwType = 0;                  // Attendance type (1=character, 2=account)
    std::uint32_t dwAttendanceID = 0;          // Current attendance ID (month-based)
    std::uint8_t byAttendanceCount = 0;        // Number of attendance days (max 14)
    std::int64_t nAttendance[14] = {};         // Attendance dates (time_t values)
    
    void Reset(std::uint32_t dwNewID);
};

/**
 * @brief PS_ATTENDANCE_CONTINUE - Consecutive attendance tracking structure
 * 
 * Tracks streak attendance for bonus rewards.
 * Size: 24 bytes (from IDA analysis)
 */
struct PS_ATTENDANCE_CONTINUE {
    std::uint8_t byApplyAttendance = 0;        // Whether streak tracking is enabled
    std::uint32_t dwType = 0;                  // Streak type
    std::uint8_t byAttendanceCount = 0;        // Current streak count (max 3)
    std::int64_t nLastAttendanceDate = 0;      // Last attendance date
    
    PS_ATTENDANCE_CONTINUE();
};

/**
 * @brief PS_ATTENDANCE_PLAY_TIME - Play time attendance tracking structure
 * 
 * Tracks cumulative play time for rewards.
 * Size: 32 bytes (from IDA analysis)
 */
struct PS_ATTENDANCE_PLAY_TIME {
    std::uint8_t byApplyAttendance = 0;        // Whether play time tracking is enabled
    std::uint32_t dwType = 0;                  // Play time type
    std::uint8_t byCurPos = 0;                 // Current reward position (0-3)
    std::int32_t nPlaySec = 0;                 // Accumulated play seconds
    std::int64_t nUpdateDate = 0;              // Last update date
    
    PS_ATTENDANCE_PLAY_TIME();
};

/**
 * @brief CGocAttendance - Game Object Component for attendance system
 * 
 * Handles daily attendance tracking, login rewards, cumulative bonuses,
 * streak tracking, and play time rewards.
 * 
 * Based on IDA decompilation of GameServer.exe (0x140030170 - 0x14003679B)
 */
class CGocAttendance : public GOComponent {
public:
    // Constructor (0x140030270)
    CGocAttendance();
    
    // Destructor (0x140030320)
    virtual ~CGocAttendance();
    
    // GOComponent interface
    bool Initialize() override { Init(); return true; }
    void Shutdown() override {}
    void Update(float fDeltaTime) override { (void)fDeltaTime; }
    
    // Static GetFamilyID (0x1403B22B0)
    static int GetFamilyID() { return 18; }
    
    // === Core initialization and lifecycle ===
    
    // Init (0x140030350) - Initialize attendance component
    void Init();
    
    // LogOut (0x140030420) - Handle player logout
    void LogOut();
    
    // OnUpdate (0x140030470) - Periodic update check
    void OnUpdate();
    
    // === Attendance data setters ===
    
    // SetAttendance (0x140030170) - Set attendance info data
    void SetAttendance(PS_ATTENDANCE_INFO& stAttendance);
    
    // SetAttendanceContinue (0x1400301C0) - Set streak data
    void SetAttendanceContinue(PS_ATTENDANCE_CONTINUE& stContinue);
    
    // SetAttendancePlayTime (0x140030210) - Set play time data
    void SetAttendancePlayTime(PS_ATTENDANCE_PLAY_TIME& stPlayTime);
    
    // === Attendance loading and validation ===
    
    // LoadAttendanceInfo (0x140031340) - Load attendance data from DB
    bool LoadAttendanceInfo(PS_ATTENDANCE_INFO& stAttendance,
                           PS_ATTENDANCE_CONTINUE& stContinue,
                           PS_ATTENDANCE_PLAY_TIME& stPlayTime);
    
    // AttendanceVailidityCheck (0x1400315B0) - Validate attendance data
    bool AttendanceVailidityCheck(PS_ATTENDANCE_INFO& stAttendance);
    
    // AttendanceContinueVailidityCheck (0x1400317F0) - Validate streak data
    bool AttendanceContinueVailidityCheck(PS_ATTENDANCE_CONTINUE& stAttendance);
    
    // AttendancePlayTimeVailidityCheck (0x140031940) - Validate play time data
    bool AttendancePlayTimeVailidityCheck(PS_ATTENDANCE_PLAY_TIME& stPlayTime);
    
    // === Attendance processing ===
    
    // GetAttendanceID (0x140031B80) - Calculate attendance ID from date
    std::uint32_t GetAttendanceID(std::int64_t biCurDate);
    
    // OnAttendance (0x140031C00) - Process daily attendance check-in
    void OnAttendance(std::int64_t biCurDate);
    
    // OnAttendancePlayTime (0x140032550) - Process play time attendance
    void OnAttendancePlayTime(std::int64_t biCurDate);
    
    // === Reward distribution ===
    
    // AttendanceReward (0x140032900) - Distribute attendance reward
    bool AttendanceReward(std::uint32_t dwRewardID);
    
    // AttendanceRewardRes (0x140032CC0) - Handle reward DB response
    bool AttendanceRewardRes(std::uint32_t dwError,
                            PS_ATTENDANCE_INFO& stAttendance,
                            ST_CREATE_ITEM& stItemInfo);
    
    // AttendanceContinueReward (0x1400333D0) - Distribute streak reward
    bool AttendanceContinueReward(std::uint32_t dwRewardItemID, std::int16_t shRewardCount);
    
    // AttendanceContinueRewardRes (0x1400336F0) - Handle streak reward DB response
    bool AttendanceContinueRewardRes(std::uint32_t dwError,
                                    PS_ATTENDANCE_CONTINUE& stContinue,
                                    ST_CREATE_ITEM& stItemInfo);
    
    // AttendancePlayTimeReward (0x140033DF0) - Distribute play time reward
    bool AttendancePlayTimeReward();
    
    // AttendancePlayTimeRewardRes (0x140034170) - Handle play time reward DB response
    bool AttendancePlayTimeRewardRes(PS_ATTENDANCE_PLAY_TIME& stPlayTime,
                                    ST_CREATE_ITEM& stItemInfo);
    
    // === Client communication ===
    
    // SendAttendanceInfo (0x140034980) - Send attendance info to client
    void SendAttendanceInfo();
    
    // SendAttendanceReward (0x140034A90) - Send reward packet to client
    void SendAttendanceReward(ST_CREATE_ITEM& stItemInfo);
    
    // SendAttendanceContinueReward (0x140034B80) - Send streak reward packet
    void SendAttendanceContinueReward(ST_CREATE_ITEM& stItemInfo);
    
    // SendAttendancePlayTimeReward (0x140034C70) - Send play time reward packet
    void SendAttendancePlayTimeReward(ST_CREATE_ITEM& stItemInfo);
    
    // === Database operations ===
    
    // SendDBAttendance (0x140034D60) - Send attendance data to DB
    void SendDBAttendance();
    
    // SendDBAttendanceLogOut (0x140035010) - Send logout attendance data
    void SendDBAttendanceLogOut();
    
    // SendDBAttendanceReset (0x140035260) - Reset attendance in DB
    void SendDBAttendanceReset(std::uint32_t dwAttendanceID);
    
    // === Play time by day tracking ===
    
    // InitPlayTimebyDay (0x140030540) - Initialize daily play time
    void InitPlayTimebyDay();
    
    // SendDBPlayTimeByDay (0x140030590) - Send daily play time to DB
    void SendDBPlayTimeByDay();
    
    // === Account play time events ===
    
    // LoadAccountPlayTimeEventReq (0x140030760) - Request account play time
    void LoadAccountPlayTimeEventReq();
    
    // LoadAccountPlayTimeEvent (0x1400308B0) - Load account play time data
    void LoadAccountPlayTimeEvent(struct PS_PLAY_TIME_BY_ACCOUNT& stData);
    
    // UpdateAccountPlayTimeEvent (0x140030B10) - Update play time event
    void UpdateAccountPlayTimeEvent();
    
    // SaveAccountPlayTimeEvent (0x140031050) - Save play time to DB
    void SaveAccountPlayTimeEvent();
    
    // ShowAccountPlayTimeEvent (0x140031200) - Show play time event info
    void ShowAccountPlayTimeEvent();
    
    // === GM/Cheat commands ===
    
    // Cheat_AttendanceReset (0x140035430) - GM: Reset attendance
    void Cheat_AttendanceReset();
    
    // Cheat_AttendanceContinueReset (0x140035490) - GM: Reset streak
    void Cheat_AttendanceContinueReset();
    
    // Cheat_AttendancePlayTimeReset (0x140035650) - GM: Reset play time
    void Cheat_AttendancePlayTimeReset();
    
    // Cheat_ShowAttendanceInfo (0x140035870) - GM: Show attendance info
    void Cheat_ShowAttendanceInfo();
    
    // Cheat_AttendancePlayTimeUpdate (0x140036740) - GM: Update play time
    void Cheat_AttendancePlayTimeUpdate(int nHour, int nMin);
    
private:
    // Member variables from IDA structure analysis
    
    // Attendance check tick (milliseconds)
    std::uint64_t m_dw64AttendanceCheckTick = 0;
    
    // Play time by day tracking
    std::uint64_t m_dw64PlayTimeByDay = 0;
    
    // Next attendance check time
    std::int64_t m_biNextAttendance = 0;
    
    // Attendance info data
    PS_ATTENDANCE_INFO m_stAttendanceInfo;
    
    // Streak attendance data
    PS_ATTENDANCE_CONTINUE m_stAttendanceContinue;
    
    // Play time attendance data
    PS_ATTENDANCE_PLAY_TIME m_stAttendancePlayTime;
    
    // Next play time attendance check
    std::int64_t m_biNextAttendancePlayTime = 0;
    
    // Account play time tracking
    std::int32_t m_nNextAccountPlayTime = 0;
    std::int32_t m_nAccountPlayTimeTick = 0;
    std::uint8_t m_byAccountPlayType = 0;
    std::int32_t m_nPrevAccountPlayTimeTick = 0;
    std::int32_t m_nAccountPlayTimeDBSaveTick = 0;
};
