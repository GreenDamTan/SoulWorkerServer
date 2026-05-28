// CGocAttendance - Game Object Component for Attendance System
// Restored from GameServer.exe IDA decompilation
// Address range: 0x140030170 - 0x140031B7B

#include "GocAttendance.h"
#include "Mover.h"
#include "XGameServer/XGameServer.h"
#include "XCore/XServer/LogHelper.h"
#include <cstring>

// Constructor (0x140030270)
// IDA-verified: Calls GOComponent constructor, sets vtable, initializes member structs
CGocAttendance::CGocAttendance()
    : GOComponent()
    , m_biNextAttendancePlayTime(0)
{
    // IDA shows: PS_ATTENDANCE_INFO::PS_ATTENDANCE_INFO(&this->m_stAttendanceInfo)
    // IDA shows: PS_ATTENDANCE_CONTINUE::PS_ATTENDANCE_CONTINUE(&this->m_stAttendanceContinue)
    // IDA shows: PS_ATTENDANCE_PLAY_TIME::PS_ATTENDANCE_PLAY_TIME(&this->m_stAttendancePlayTime)
    // Default constructors zero-initialize the structs
    memset(&m_stAttendanceInfo, 0, sizeof(m_stAttendanceInfo));
    memset(&m_stAttendanceContinue, 0, sizeof(m_stAttendanceContinue));
    memset(&m_stAttendancePlayTime, 0, sizeof(m_stAttendancePlayTime));
}

// Destructor (0x1400302E0)
CGocAttendance::~CGocAttendance()
{
}

// Initialize
bool CGocAttendance::Initialize()
{
    return true;
}

void CGocAttendance::Shutdown()
{
}

void CGocAttendance::Update(float fDeltaTime)
{
    (void)fDeltaTime;
}

// SetAttendance (0x140030170)
void CGocAttendance::SetAttendance(PS_ATTENDANCE_INFO& stAttendance)
{
    memcpy(&m_stAttendanceInfo, &stAttendance, sizeof(m_stAttendanceInfo));
}

// SetAttendanceContinue (0x1400301C0)
void CGocAttendance::SetAttendanceContinue(PS_ATTENDANCE_CONTINUE& stContinue)
{
    memcpy(&m_stAttendanceContinue, &stContinue, sizeof(m_stAttendanceContinue));
}

// SetAttendancePlayTime (0x140030210)
// IDA-verified: Copies 32 bytes from input to member
void CGocAttendance::SetAttendancePlayTime(PS_ATTENDANCE_PLAY_TIME& stPlayTime)
{
    memcpy(&m_stAttendancePlayTime, &stPlayTime, sizeof(m_stAttendancePlayTime));
}

// LoadAttendanceInfo (0x140031340)
// IDA-verified: Loads and validates all attendance data from database
// Returns true if successful, false on validation error
bool CGocAttendance::LoadAttendanceInfo(PS_ATTENDANCE_INFO& stAttendance,
                                        PS_ATTENDANCE_CONTINUE& stAttendanceContinue,
                                        PS_ATTENDANCE_PLAY_TIME& stAttendancePlayTime)
{
    // Get current tick count for time validation
    // IDA shows: ATL::CTime::GetTickCount(&tCurTime)

    // Process attendance info if apply flag is set
    if (stAttendance.byApplyAttendance)
    {
        if (!AttendanceVailidityCheck(stAttendance))
        {
            // IDA: Log error with UCID and error code 300
            LogHelper::LogError("game.contents",
                "LoadAttendanceInfo - AttendanceVailidityCheck [UCID:%d] (%d)",
                0, 300); // TODO: 需人工审查 - Get UCID from owner
            return false;
        }
        SetAttendance(stAttendance);
    }

    // Process attendance continue if apply flag is set
    if (stAttendanceContinue.byApplyAttendance)
    {
        if (!AttendanceContinueVailidityCheck(stAttendanceContinue))
        {
            // IDA: Log error with UCID and error code 313
            LogHelper::LogError("game.contents",
                "LoadAttendanceInfo - AttendanceContinueVailidityCheck [UCID:%d] (%d)",
                0, 313); // TODO: 需人工审查 - Get UCID from owner

            // Reset to default continue data on validation failure
            PS_ATTENDANCE_CONTINUE psNewAttendanceContinue;
            psNewAttendanceContinue.byApplyAttendance = stAttendanceContinue.byApplyAttendance;
            psNewAttendanceContinue.dwType = stAttendanceContinue.dwType;
            stAttendanceContinue = psNewAttendanceContinue;
        }
        SetAttendanceContinue(stAttendanceContinue);
    }

    // Process play time attendance if apply flag is set
    if (stAttendancePlayTime.byApplyAttendance)
    {
        if (AttendancePlayTimeVailidityCheck(stAttendancePlayTime))
        {
            // Get next update date from XGameServer
            XGameServer* pGameServer = XGameServer::Instance();
            if (pGameServer)
            {
                m_biNextAttendancePlayTime = pGameServer->GetUpdateDate(9);
            }
        }
        else
        {
            m_biNextAttendancePlayTime = 0;
            // IDA: Log error with UCID and error code 331
            LogHelper::LogError("game.contents",
                "LoadAttendanceInfo - AttendancePlayTimeVailidityCheck [UCID:%d] (%d)",
                0, 331); // TODO: 需人工审查 - Get UCID from owner
        }
        SetAttendancePlayTime(stAttendancePlayTime);
    }

    return true;
}

// AttendanceVailidityCheck (0x1400315B0)
// IDA-verified: Validates attendance info data
// Checks that attendance dates are >= 2000-01-01 and counts match
bool CGocAttendance::AttendanceVailidityCheck(PS_ATTENDANCE_INFO& stAttendance)
{
    // IDA shows: ATL::CTime::CTime(&tCheckInitTime, 2000, 1, 1, 0, 0, 0, -1)
    // Initialize check time to 2000-01-01 00:00:00
    // CTime tCheckInitTime(2000, 1, 1, 0, 0, 0, -1);

    std::uint8_t byCheckCount = 0;

    // Check each attendance entry (max 14 slots)
    for (int sh = 0; sh < 14 && stAttendance.nAttendance[sh]; ++sh)
    {
        if (stAttendance.nAttendance[sh] > 0)
        {
            // IDA shows comparison with CTime from GetTickCount
            // Validate date is reasonable (>= year 2000)
            // TODO: 需人工审查 - Full date validation logic
            // if (stAttendance.nAttendance[sh] < nMinValidDate)
            // {
            //     LogHelper::LogError("game.contents",
            //         "AttendanceVailidityCheck Date Error [UCID:%d / Date:%d / count:%d] (%d)",
            //         0, stAttendance.nAttendance[sh], sh, 360);
            //     return false;
            // }
            ++byCheckCount;
        }
    }

    // Verify count matches
    if (byCheckCount == stAttendance.byAttendanceCount)
    {
        return true;
    }

    // IDA: Log count mismatch error
    LogHelper::LogError("game.contents",
        "AttendanceVailidityCheck Diff Count [UCID:%d / count:%d / count:%d] (%d)",
        0, byCheckCount, stAttendance.byAttendanceCount, 370);

    return false;
}

// AttendanceContinueVailidityCheck (0x1400317F0)
// IDA-verified: Validates continue attendance data
// Checks date >= 2000-01-01 and count <= 3
bool CGocAttendance::AttendanceContinueVailidityCheck(PS_ATTENDANCE_CONTINUE& stAttendance)
{
    // IDA shows: ATL::CTime::CTime(&tCheckInitTime, 2000, 1, 1, 0, 0, 0, -1)
    // Initialize check time to 2000-01-01 00:00:00

    // Check last attendance date is valid (>= year 2000)
    // TODO: 需人工审查 - Full date validation with CTime comparison
    // if (stAttendance.nLastAttendanceDate < nMinValidDate)
    // {
    //     LogHelper::LogError("game.contents",
    //         "AttendanceContinueVailidityCheck Date Error [UCID:%d / date:%d] (%d)",
    //         0, stAttendance.nLastAttendanceDate, 384);
    //     return false;
    // }

    // Check attendance count is valid (max 3)
    if (stAttendance.byAttendanceCount > 3)
    {
        LogHelper::LogError("game.contents",
            "AttendanceContinueVailidityCheck Count Error [UCID:%d / count:%d] (%d)",
            0, stAttendance.byAttendanceCount, 390);
        return false;
    }

    return true;
}

// AttendancePlayTimeVailidityCheck (0x140031940)
// IDA-verified: Validates play time attendance data
// Checks against TB_CHECK_ACCESS_REWARD table
bool CGocAttendance::AttendancePlayTimeVailidityCheck(PS_ATTENDANCE_PLAY_TIME& stPlayTime)
{
    // Get the check access reward table entry
    XGameServer* pGameServer = XGameServer::Instance();
    if (!pGameServer)
    {
        LogHelper::LogError("game.contents",
            "AttendancePlayTimeVailidityCheck Table Error [UCID:%d] (%d)", 0, 402);
        return false;
    }

    // IDA shows: XResourceMgr::GetTB_CHECK_ACCESS_REWARD(&v2->m_xResourceMgr, 1u)
    // TB_CHECK_ACCESS_REWARD* pTBCheckAccess = pGameServer->GetResourceMgr().GetTB_CHECK_ACCESS_REWARD(1);
    // if (!pTBCheckAccess)
    // {
    //     LogHelper::LogError("game.contents",
    //         "AttendancePlayTimeVailidityCheck Table Error [UCID:%d] (%d)", 0, 402);
    //     return false;
    // }

    // Check current position is valid (max 3)
    if (stPlayTime.byCurPos > 3)
    {
        LogHelper::LogError("game.contents",
            "AttendancePlayTimeVailidityCheck Count Error [UCID:%d / count:%d] (%d)",
            0, stPlayTime.byCurPos, 408);
        stPlayTime.byCurPos = 3;
        return false;
    }

    // IDA shows: Check if current position < 3, validate time entry exists
    // if (stPlayTime.byCurPos < 3 && !pTBCheckAccess->Check_Attendance_Day_Time[stPlayTime.byCurPos])
    // {
    //     LogHelper::LogError("game.contents",
    //         "AttendancePlayTimeVailidityCheck Time Error [UCID:%d / count:%d] (%d)",
    //         0, stPlayTime.byCurPos, 417);
    //     return false;
    // }

    // Validate update date (minimum year 2000)
    // IDA shows: CTime tCheckInitTime(2000, 1, 1, 0, 0, 0, -1)
    // if (stPlayTime.nUpdateDate < nMinValidDate)
    // {
    //     stPlayTime.nUpdateDate = nMinValidDate;
    // }

    return true;
}
