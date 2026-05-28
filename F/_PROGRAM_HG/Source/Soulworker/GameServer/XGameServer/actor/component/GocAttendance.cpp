// CGocAttendance - Game Object Component for Attendance System
// Restored from GameServer.exe IDA decompilation
// Address range: 0x140030270 - 0x140030316

#include "GocAttendance.h"
#include <cstring>

// Constructor (0x140030270)
CGocAttendance::CGocAttendance()
    : GOComponent()
{
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
void CGocAttendance::SetAttendancePlayTime(PS_ATTENDANCE_PLAY_TIME& stPlayTime)
{
    memcpy(&m_stAttendancePlayTime, &stPlayTime, sizeof(m_stAttendancePlayTime));
}
