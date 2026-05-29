// CGocAttendance - Game Object Component for Attendance System
// Restored from GameServer.exe IDA decompilation
// IDA-verified addresses:
// - SetAttendance: 0x140030170 (qmemcpy 128 bytes)
// - SetAttendanceContinue: 0x1400301C0 (qmemcpy 24 bytes)
// - SetAttendancePlayTime: 0x140030210 (qmemcpy 32 bytes)
// - CGocAttendance::CGocAttendance: 0x140030270
// - CGocAttendance::~CGocAttendance: 0x1400302E0 (scalar deleting destructor)
// - LoadAttendanceInfo: 0x140031340
// - AttendanceVailidityCheck: 0x1400315B0
// - AttendanceContinueVailidityCheck: 0x1400317F0
// - AttendancePlayTimeVailidityCheck: 0x140031940

#include "GocAttendance.h"
#include "Mover.h"
#include "XGameServer/XGameServer.h"
#include "XCore/XServer/LogHelper.h"
#include <cstring>

// Constructor (0x140030270)
// IDA-verified: GOComponent::GOComponent(this), sets vtable, calls default constructors for members
CGocAttendance::CGocAttendance()
    : GOComponent()
    , m_biNextAttendancePlayTime(0)
{
    // IDA: PS_ATTENDANCE_INFO::PS_ATTENDANCE_INFO(&this->m_stAttendanceInfo)
    // IDA: PS_ATTENDANCE_CONTINUE::PS_ATTENDANCE_CONTINUE(&this->m_stAttendanceContinue)
    // IDA: PS_ATTENDANCE_PLAY_TIME::PS_ATTENDANCE_PLAY_TIME(&this->m_stAttendancePlayTime)
    // Default constructors zero-initialize the structs
    memset(&m_stAttendanceInfo, 0, sizeof(m_stAttendanceInfo));
    memset(&m_stAttendanceContinue, 0, sizeof(m_stAttendanceContinue));
    memset(&m_stAttendancePlayTime, 0, sizeof(m_stAttendancePlayTime));
}

// Destructor (0x1400302E0)
// IDA-verified: scalar deleting destructor - calls destructor then operator delete if flag set
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
// IDA-verified: qmemcpy 128 bytes from input to member via temp buffer
void CGocAttendance::SetAttendance(PS_ATTENDANCE_INFO& stAttendance)
{
    // IDA uses intermediate buffer v2[128] then qmemcpy to member
    memcpy(&m_stAttendanceInfo, &stAttendance, sizeof(m_stAttendanceInfo));
}

// SetAttendanceContinue (0x1400301C0)
// IDA-verified: qmemcpy 24 bytes from input to member via temp buffer
void CGocAttendance::SetAttendanceContinue(PS_ATTENDANCE_CONTINUE& stContinue)
{
    // IDA uses intermediate buffer v2[24] then qmemcpy to member
    memcpy(&m_stAttendanceContinue, &stContinue, sizeof(m_stAttendanceContinue));
}

// SetAttendancePlayTime (0x140030210)
// IDA-verified: qmemcpy 32 bytes from input to member via temp buffer
void CGocAttendance::SetAttendancePlayTime(PS_ATTENDANCE_PLAY_TIME& stPlayTime)
{
    // IDA uses intermediate buffer v2[32] then qmemcpy to member
    memcpy(&m_stAttendancePlayTime, &stPlayTime, sizeof(m_stAttendancePlayTime));
}

// LoadAttendanceInfo (0x140031340)
// IDA-verified: Loads and validates all attendance data from database
// Returns true if successful, false on validation error
bool CGocAttendance::LoadAttendanceInfo(PS_ATTENDANCE_INFO& stAttendance,
                                        PS_ATTENDANCE_CONTINUE& stAttendanceContinue,
                                        PS_ATTENDANCE_PLAY_TIME& stAttendancePlayTime)
{
    // IDA: ATL::CTime tCurTime; ATL::CTime::GetTickCount(&tCurTime);
    // IDA: std::tr1::_Ptr_base<CParty>::_Get(...) - retrieves UCID from owner
    // Note: CTime::GetTickCount returns current time, used for logging context

    // Process attendance info if apply flag is set
    if (stAttendance.byApplyAttendance)
    {
        if (!AttendanceVailidityCheck(stAttendance))
        {
            // IDA: Log error with UCID and error code 300
            LogHelper::LogError("game.contents",
                "LoadAttendanceInfo - AttendanceVailidityCheck [UCID:%d] (%d)",
                0, 300); // TODO: 需人工审查 - Get UCID from owner mover
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
                0, 313); // TODO: 需人工审查 - Get UCID from owner mover

            // IDA: PS_ATTENDANCE_CONTINUE::PS_ATTENDANCE_CONTINUE(&psNewAttendanceContinue)
            // IDA: psNewAttendanceContinue.byApplyAttendance = stAttendanceContinue->byApplyAttendance
            // IDA: psNewAttendanceContinue.dwType = stAttendanceContinue->dwType
            // IDA: qmemcpy(stAttendanceContinue, &psNewAttendanceContinue, sizeof(PS_ATTENDANCE_CONTINUE))
            PS_ATTENDANCE_CONTINUE psNewAttendanceContinue;
            memset(&psNewAttendanceContinue, 0, sizeof(psNewAttendanceContinue));
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
            // IDA: v8 = TXSingleton<XGameServer>::Instance()
            // IDA: this->m_biNextAttendancePlayTime = XGameServer::GetUpdateDate(v8, 9u)
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
                0, 331); // TODO: 需人工审查 - Get UCID from owner mover
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
    // IDA: ATL::CTime::CTime(&tCheckInitTime, 2000, 1, 1, 0, 0, 0, -1)
    // CTime(2000, 1, 1, 0, 0, 0, -1) converts to __int64 value
    // Unix timestamp for 2000-01-01 00:00:00 UTC = 946656000 (local time may vary)
    constexpr std::int64_t nMinValidDate = 946656000LL;  // 2000-01-01 00:00:00

    std::uint8_t byCheckCount = 0;

    // IDA: for (sh = 0; sh < 14 && stAttendance->nAttendance[sh]; ++sh)
    for (int sh = 0; sh < 14 && stAttendance.nAttendance[sh]; ++sh)
    {
        if (stAttendance.nAttendance[sh] > 0)
        {
            // IDA: if (stAttendance->nAttendance[v11] < (__int64)v2)
            // where v2 is the CTime value converted to __int64
            if (stAttendance.nAttendance[sh] < nMinValidDate)
            {
                LogHelper::LogError("game.contents",
                    "AttendanceVailidityCheck Date Error [UCID:%d / Date:%d / count:%d] (%d)",
                    0, static_cast<int>(stAttendance.nAttendance[sh]), sh, 360);
                return false;
            }
            ++byCheckCount;
        }
    }

    // Verify count matches
    if (byCheckCount == stAttendance.byAttendanceCount)
    {
        return true;
    }

    // IDA: Log count mismatch error (code 370)
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
    // IDA: ATL::CTime::CTime(&tCheckInitTime, 2000, 1, 1, 0, 0, 0, -1)
    constexpr std::int64_t nMinValidDate = 946656000LL;  // 2000-01-01 00:00:00

    // IDA: if (stAttendance->nLastAttendanceDate >= (__int64)v2)
    // Check last attendance date is valid (>= year 2000)
    if (stAttendance.nLastAttendanceDate < nMinValidDate)
    {
        LogHelper::LogError("game.contents",
            "AttendanceContinueVailidityCheck Date Error [UCID:%d / date:%d] (%d)",
            0, static_cast<int>(stAttendance.nLastAttendanceDate), 384);
        return false;
    }

    // IDA: if (stAttendance->byAttendanceCount <= 3u)
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
    // IDA: v2 = TXSingleton<XGameServer>::Instance()
    // IDA: pTBCheckAccess = XResourceMgr::GetTB_CHECK_ACCESS_REWARD(&v2->m_xResourceMgr, 1u)
    XGameServer* pGameServer = XGameServer::Instance();
    if (!pGameServer)
    {
        LogHelper::LogError("game.contents",
            "AttendancePlayTimeVailidityCheck Table Error [UCID:%d] (%d)", 0, 402);
        return false;
    }

    // IDA: TB_CHECK_ACCESS_REWARD* pTBCheckAccess = XResourceMgr::GetTB_CHECK_ACCESS_REWARD(&pGameServer->m_xResourceMgr, 1u)
    // TODO: 需人工审查 - Get TB_CHECK_ACCESS_REWARD from resource manager
    // For now, we skip the table check and proceed with basic validation
    // TB_CHECK_ACCESS_REWARD* pTBCheckAccess = pGameServer->GetResourceMgr().GetTB_CHECK_ACCESS_REWARD(1);
    // if (!pTBCheckAccess)
    // {
    //     LogHelper::LogError("game.contents",
    //         "AttendancePlayTimeVailidityCheck Table Error [UCID:%d] (%d)", 0, 402);
    //     return false;
    // }

    // IDA: if (stPlayTime->byCurPos <= 3u)
    // Check current position is valid (max 3)
    if (stPlayTime.byCurPos > 3)
    {
        LogHelper::LogError("game.contents",
            "AttendancePlayTimeVailidityCheck Count Error [UCID:%d / count:%d] (%d)",
            0, stPlayTime.byCurPos, 408);
        stPlayTime.byCurPos = 3;
        return false;
    }

    // IDA: if (stPlayTime->byCurPos >= 3u || *(&pTBCheckAccess->Check_Attendance_Day_Time_1 + stPlayTime->byCurPos))
    // IDA shows: Check if current position < 3, validate time entry exists
    // TODO: 需人工审查 - Validate time entry from TB_CHECK_ACCESS_REWARD
    // if (stPlayTime.byCurPos < 3 && pTBCheckAccess && !pTBCheckAccess->Check_Attendance_Day_Time[stPlayTime.byCurPos])
    // {
    //     LogHelper::LogError("game.contents",
    //         "AttendancePlayTimeVailidityCheck Time Error [UCID:%d / count:%d] (%d)",
    //         0, stPlayTime.byCurPos, 417);
    //     return false;
    // }

    // IDA: ATL::CTime::CTime(&tCheckInitTime, 2000, 1, 1, 0, 0, 0, -1)
    // IDA: if (stPlayTime->nUpdateDate < (__int64)v2) stPlayTime->nUpdateDate = (__int64)v2
    // Validate update date (minimum year 2000)
    constexpr std::int64_t nMinValidDate = 946656000LL;  // 2000-01-01 00:00:00
    if (stPlayTime.nUpdateDate < nMinValidDate)
    {
        stPlayTime.nUpdateDate = nMinValidDate;
    }

    return true;
}

// GetOwnerMover - Get the owner mover object
// IDA: returns m_pOwner from GOComponent base class
CMover* CGocAttendance::GetOwnerMover() const
{
    return GetOwnerGO();
}

// InitPlayTimebyDay (0x140030540)
// IDA-verified: Initialize play time tracking for the day
void CGocAttendance::InitPlayTimebyDay()
{
    // IDA: this->m_dw64PlayTimeByDay = GetTickCount64()
    m_dw64PlayTimeByDay = GetTickCount64();

    // IDA: v1 = TXSingleton<XGameServer>::Instance()
    // IDA: Option = XServer::GetOption(v1)
    // IDA: if (XOption::GetNationType(Option) == NATION_TYPE_KOR)
    // IDA:     CGocAttendance::LoadAccountPlayTimeEventReq(this)
    XGameServer* pGameServer = XGameServer::Instance();
    if (pGameServer)
    {
        XOption& option = pGameServer->GetOption();
        if (option.GetNationType() == NATION_TYPE_KOR)
        {
            LoadAccountPlayTimeEventReq();
        }
    }
}

// SendDBPlayTimeByDay (0x140030590)
// IDA-verified: Send play time to database
void CGocAttendance::SendDBPlayTimeByDay()
{
    // IDA: if (!this->m_dw64PlayTimeByDay) return
    if (!m_dw64PlayTimeByDay)
    {
        return;
    }

    // IDA: Get owner as CUser via RTDynamicCast
    CMover* pMover = GetOwnerMover();
    if (!pMover)
    {
        return;
    }

    // TODO: 需人工审查 - Requires CUser RTTI cast and XSendDBPacket implementation
    // IDA: PS_PLAY_TIME_FOR_DAY psPlayTime
    // IDA: AccountID = CUser::GetAccountID(pUser)
    // IDA: psPlayTime.dwUCID = actorID.dwActorID
    // IDA: psPlayTime.nSec = (GetTickCount64() - this->m_dw64PlayTimeByDay) / 1000
    // IDA: XSendDBPacket xSendDBPacket(pObject, 0x49, 0x26)
    // IDA: XGameServer::SendDBGame(v4, &xSendDBPacket)
    // IDA: this->m_dw64PlayTimeByDay = GetTickCount64()

    // Reset timer after sending
    m_dw64PlayTimeByDay = GetTickCount64();
}

// LoadAccountPlayTimeEventReq (0x140030760)
// IDA-verified: Request account play time event from DB
void CGocAttendance::LoadAccountPlayTimeEventReq()
{
    // TODO: 需人工审查 - Requires XSendDBPacket implementation
    // IDA: Send DB request for account play time event
}
