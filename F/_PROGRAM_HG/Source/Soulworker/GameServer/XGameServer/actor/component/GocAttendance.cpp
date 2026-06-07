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
// IDA: Checks against TB_CHECK_ACCESS_REWARD table, max position 3
bool CGocAttendance::AttendancePlayTimeVailidityCheck(PS_ATTENDANCE_PLAY_TIME& stPlayTime)
{
    // IDA: v2 = TXSingleton<XGameServer>::Instance()
    XGameServer* pGameServer = XGameServer::Instance();
    if (!pGameServer)
    {
        LogHelper::LogError("game.contents",
            "AttendancePlayTimeVailidityCheck Table Error [UCID:%d] (%d)", 0, 402);
        return false;
    }

    // IDA: pTBCheckAccess = XResourceMgr::GetTB_CHECK_ACCESS_REWARD(&v2->m_xResourceMgr, 1u)
    TB_CHECK_ACCESS_REWARD* pTBCheckAccess = pGameServer->GetResourceMgr().GetTB_CHECK_ACCESS_REWARD(1);
    if (!pTBCheckAccess)
    {
        CMover* pMover = GetOwnerMover();
        std::int32_t nUCID = pMover ? pMover->GetUCID() : 0;
        LogHelper::LogError("game.contents",
            "AttendancePlayTimeVailidityCheck Table Error [UCID:%d] (%d)", nUCID, 402);
        return false;
    }

    // IDA: if (stPlayTime->byCurPos <= 3u)
    if (stPlayTime.byCurPos > 3)
    {
        CMover* pMover = GetOwnerMover();
        std::int32_t nUCID = pMover ? pMover->GetUCID() : 0;
        LogHelper::LogError("game.contents",
            "AttendancePlayTimeVailidityCheck Count Error [UCID:%d / count:%d] (%d)",
            nUCID, stPlayTime.byCurPos, 408);
        stPlayTime.byCurPos = 3;
        return false;
    }

    // IDA: if (stPlayTime->byCurPos >= 3u || *(&pTBCheckAccess->Check_Attendance_Day_Time_1 + stPlayTime->byCurPos))
    // Check if current position < 3, validate time entry exists
    if (stPlayTime.byCurPos < 3)
    {
        // IDA: Check time entry from TB_CHECK_ACCESS_REWARD
        std::int32_t nCheckTime = 0;
        switch (stPlayTime.byCurPos)
        {
            case 0: nCheckTime = pTBCheckAccess->Check_Attendance_Day_Time_1; break;
            case 1: nCheckTime = pTBCheckAccess->Check_Attendance_Day_Time_2; break;
            case 2: nCheckTime = pTBCheckAccess->Check_Attendance_Day_Time_3; break;
        }

        if (!nCheckTime)
        {
            CMover* pMover = GetOwnerMover();
            std::int32_t nUCID = pMover ? pMover->GetUCID() : 0;
            LogHelper::LogError("game.contents",
                "AttendancePlayTimeVailidityCheck Time Error [UCID:%d / count:%d] (%d)",
                nUCID, stPlayTime.byCurPos, 417);
            return false;
        }
    }

    // IDA: ATL::CTime::CTime(&tCheckInitTime, 2000, 1, 1, 0, 0, 0, -1)
    // IDA: if (stPlayTime->nUpdateDate < (__int64)v2) stPlayTime->nUpdateDate = (__int64)v2
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

// Init (0x140030350)
// IDA-verified: Initialize attendance component
void CGocAttendance::Init()
{
    m_dw64AttendanceCheckTick = 0;
    m_dw64PlayTimeByDay = 0;
    m_biNextAttendance = 0;
    memset(&m_stAttendanceInfo, 0, sizeof(m_stAttendanceInfo));
    memset(&m_stAttendanceContinue, 0, sizeof(m_stAttendanceContinue));
    m_biNextAttendancePlayTime = 0;
    memset(&m_stAttendancePlayTime, 0, sizeof(m_stAttendancePlayTime));
    m_nNextAccountPlayTime = 0;
    m_nAccountPlayTimeTick = 0;
    m_byAccountPlayType = 0;
    m_nPrevAccountPlayTimeTick = 0;
    m_nAccountPlayTimeDBSaveTick = 0;
}

// LogOut (0x140030420)
// IDA-verified: Handle logout - send play time to DB
void CGocAttendance::LogOut()
{
    XGameServer* pGameServer = XGameServer::Instance();
    if (pGameServer && pGameServer->GetResourceMgr().GetServerContents(E_SERVER_OPTION_ATTENDANCE))
    {
        SendDBAttendanceLogOut();
    }
    SendDBPlayTimeByDay();
    SaveAccountPlayTimeEvent();
}

// OnUpdate (0x140030470)
// IDA-verified: Update attendance timers
void CGocAttendance::OnUpdate()
{
    // IDA: Check if play time needs to be saved (every 60 seconds)
    if (m_dw64PlayTimeByDay + 60000 <= GetTickCount64())
    {
        SendDBPlayTimeByDay();
    }

    UpdateAccountPlayTimeEvent();

    // IDA: Check attendance tick
    XGameServer* pGameServer = XGameServer::Instance();
    if (pGameServer && pGameServer->GetResourceMgr().GetServerContents(E_SERVER_OPTION_ATTENDANCE))
    {
        if (m_dw64AttendanceCheckTick && m_dw64AttendanceCheckTick < GetTickCount64())
        {
            m_dw64AttendanceCheckTick = GetTickCount64() + 5000;

            __int64 biCurDate = pGameServer->GetCurDate();
            OnAttendance(biCurDate);
            OnAttendancePlayTime(biCurDate);
        }
    }
}

// LoadAccountPlayTimeEventReq (0x140030760)
// IDA-verified: Request account play time event from DB
// IDA: main=0x49, sub=0x39
void CGocAttendance::LoadAccountPlayTimeEventReq()
{
    PS_PLAY_TIME_BY_ACCOUNT stInitInfo;
    memset(&stInitInfo, 0, sizeof(stInitInfo));

    // IDA: Get owner mover and cast to CUser to get UAID
    CMover* pMover = GetOwnerMover();
    if (!pMover)
        return;

    // IDA: RTTI dynamic_cast to CUser
    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (pUser)
    {
        stInitInfo.dwUAID = pUser->GetUAID();
    }

    // IDA: XSendDBPacket xSendDBPacket(pObject, 0x49u, 0x39u)
    XSendDBPacket xSendDBPacket(pMover, 0x49u, 0x39u);
    xSendDBPacket << stInitInfo;

    XGameServer* pGameServer = XGameServer::Instance();
    if (pGameServer)
    {
        pGameServer->SendDBGame(xSendDBPacket);
    }
}

// LoadAccountPlayTimeEvent (0x1400308B0)
// IDA-verified: Load account play time event from DB response
// IDA: main=0x49, sub=0x40 for save
void CGocAttendance::LoadAccountPlayTimeEvent(PS_PLAY_TIME_BY_ACCOUNT& stTime)
{
    // IDA: this->m_nNextAccountPlayTime = stTime->nInitTime
    m_nNextAccountPlayTime = stTime.nInitTime;
    // IDA: this->m_nAccountPlayTimeTick = 1000 * stTime->nSec
    m_nAccountPlayTimeTick = 1000 * stTime.nSec;
    // IDA: this->m_byAccountPlayType = stTime->byState
    m_byAccountPlayType = stTime.byState;
    // IDA: this->m_nPrevAccountPlayTimeTick = GetTickCount64()
    m_nPrevAccountPlayTimeTick = GetTickCount64();
    // IDA: this->m_nAccountPlayTimeDBSaveTick = GetTickCount64() + 60000
    m_nAccountPlayTimeDBSaveTick = GetTickCount64() + 60000;

    // IDA: v2 = TXSingleton<XGameServer>::Instance()
    // IDA: nTime = XGameServer::GetBeforeInitDate(v2)
    XGameServer* pGameServer = XGameServer::Instance();
    if (!pGameServer)
        return;

    __int64 nTime = pGameServer->GetBeforeInitDate();

    // IDA: if (this->m_nNextAccountPlayTime < nTime)
    if (m_nNextAccountPlayTime < nTime)
    {
        // IDA: PS_CASH_BUY_COUNT::PS_CASH_BUY_COUNT(&stInitInfo)
        PS_PLAY_TIME_BY_ACCOUNT stInitInfo;
        memset(&stInitInfo, 0, sizeof(stInitInfo));

        // IDA: Get UAID from owner
        CMover* pMover = GetOwnerMover();
        if (pMover)
        {
            CUser* pUser = dynamic_cast<CUser*>(pMover);
            if (pUser)
            {
                stInitInfo.dwUAID = pUser->GetUAID();
            }
        }

        // IDA: this->m_nNextAccountPlayTime = nTime
        m_nNextAccountPlayTime = nTime;
        stInitInfo.nInitTime = nTime;
        // IDA: this->m_byAccountPlayType = 0
        m_byAccountPlayType = 0;
        stInitInfo.byState = 0;
        // IDA: this->m_nAccountPlayTimeTick = 0
        m_nAccountPlayTimeTick = 0;
        stInitInfo.nSec = 0;

        // IDA: XSendDBPacket xSendDBPacket(pObject, 0x49u, 0x40u)
        if (pMover)
        {
            XSendDBPacket xSendDBPacket(pMover, 0x49u, 0x40u);
            xSendDBPacket << stInitInfo;
            pGameServer->SendDBGame(xSendDBPacket);
        }
    }
}

// UpdateAccountPlayTimeEvent (0x140030B10)
// IDA-verified: Update account play time event timer
// IDA: Checks tick every second, triggers auto-mail after 1 hour (3600000ms)
void CGocAttendance::UpdateAccountPlayTimeEvent()
{
    // IDA: if (!this->m_nPrevAccountPlayTimeTick) return
    if (!m_nPrevAccountPlayTimeTick)
        return;

    XGameServer* pGameServer = XGameServer::Instance();
    if (!pGameServer)
        return;

    // IDA: nTime = XGameServer::GetBeforeInitDate(v3)
    __int64 nTime = pGameServer->GetBeforeInitDate();

    // IDA: if (this->m_nNextAccountPlayTime >= (__int64)nTime)
    if (m_nNextAccountPlayTime >= nTime)
    {
        // IDA: nGap = GetTickCount64() - LODWORD(this->m_nPrevAccountPlayTimeTick)
        __int64 nGap = GetTickCount64() - m_nPrevAccountPlayTimeTick;

        // IDA: if (nGap >= 1000)
        if (nGap >= 1000)
        {
            // IDA: this->m_nAccountPlayTimeTick += nGap
            m_nAccountPlayTimeTick += nGap;

            // IDA: if (!this->m_byAccountPlayType)
            if (!m_byAccountPlayType)
            {
                // IDA: if (this->m_nAccountPlayTimeTick < 3600000)
                if (m_nAccountPlayTimeTick < 3600000)
                {
                    // IDA: if (GetTickCount64() >= this->m_nAccountPlayTimeDBSaveTick)
                    if (GetTickCount64() >= m_nAccountPlayTimeDBSaveTick)
                    {
                        SaveAccountPlayTimeEvent();
                        m_nAccountPlayTimeDBSaveTick = GetTickCount64() + 60000;
                    }
                }
                else
                {
                    // IDA: 1 hour reached - send auto mail
                    m_byAccountPlayType = 1;

                    CMover* pMover = GetOwnerMover();
                    if (pMover)
                    {
                        std::shared_ptr<CGocPost> pPostPtr;
                        pMover->GetGOC<CGocPost>(&pPostPtr, 0);

                        // IDA: if (pPostPtr)
                        if (pPostPtr && pPostPtr.get())
                        {
                            CGocPost* pPost = pPostPtr.get();
                            // IDA: if (CGocPost::SendAutoMail(v6, 4u))
                            if (pPost->SendAutoMail(4u))
                            {
                                // IDA: Send chat notice
                                PS_CHAT_NOTICE stChat;
                                memset(&stChat, 0, sizeof(stChat));
                                stChat.nMessageCode = 49241;

                                XSendPacket xSendPacket(7u, 4u);
                                xSendPacket << stChat;

                                // IDA: CGocNetwork::Send(pActor, &xSendPacket)
                                CGocNetwork::Send(pMover, &xSendPacket);

                                SaveAccountPlayTimeEvent();

                                // IDA: Send log
                                CUser* pUser = dynamic_cast<CUser*>(pMover);
                                if (pUser)
                                {
                                    ST_LOG_GAME stLog;
                                    memset(&stLog, 0, sizeof(stLog));
                                    stLog._nUAID = pUser->GetUAID();
                                    stLog._nUCID = pMover->GetUCID();
                                    stLog._sMainType = 3;
                                    stLog._sSubType = 20;
                                    stLog.nParam0 = 100;
                                    stLog.nParam1 = pMover->GetLevel();

                                    pGameServer->SendDBLog(stLog);
                                }
                            }
                        }
                    }
                }
            }
            // IDA: this->m_nPrevAccountPlayTimeTick = GetTickCount64()
            m_nPrevAccountPlayTimeTick = GetTickCount64();
        }
    }
    else
    {
        // IDA: Reset and save to DB
        PS_PLAY_TIME_BY_ACCOUNT stInitInfo;
        memset(&stInitInfo, 0, sizeof(stInitInfo));

        CMover* pMover = GetOwnerMover();
        if (pMover)
        {
            CUser* pUser = dynamic_cast<CUser*>(pMover);
            if (pUser)
            {
                stInitInfo.dwUAID = pUser->GetUAID();
            }
        }

        m_nNextAccountPlayTime = nTime;
        stInitInfo.nInitTime = nTime;
        m_byAccountPlayType = 0;
        stInitInfo.byState = 0;
        m_nAccountPlayTimeTick = 0;
        stInitInfo.nSec = 0;
        m_nPrevAccountPlayTimeTick = GetTickCount64();

        if (pMover)
        {
            XSendDBPacket xSendDBPacket(pMover, 0x49u, 0x40u);
            xSendDBPacket << stInitInfo;
            pGameServer->SendDBGame(xSendDBPacket);
        }
    }
}

// SaveAccountPlayTimeEvent (0x140031050)
// IDA-verified: Save account play time event to DB
// IDA: main=0x49, sub=0x40
void CGocAttendance::SaveAccountPlayTimeEvent()
{
    // IDA: if (!this->m_nPrevAccountPlayTimeTick) return
    if (!m_nPrevAccountPlayTimeTick)
        return;

    PS_PLAY_TIME_BY_ACCOUNT stInitInfo;
    memset(&stInitInfo, 0, sizeof(stInitInfo));

    // IDA: Get UAID from owner
    CMover* pMover = GetOwnerMover();
    if (pMover)
    {
        CUser* pUser = dynamic_cast<CUser*>(pMover);
        if (pUser)
        {
            stInitInfo.dwUAID = pUser->GetUAID();
        }
    }

    // IDA: stInitInfo.nInitTime = this->m_nNextAccountPlayTime
    stInitInfo.nInitTime = m_nNextAccountPlayTime;
    // IDA: stInitInfo.byState = this->m_byAccountPlayType
    stInitInfo.byState = m_byAccountPlayType;
    // IDA: stInitInfo.nSec = this->m_nAccountPlayTimeTick / 1000
    stInitInfo.nSec = m_nAccountPlayTimeTick / 1000;

    // IDA: XSendDBPacket xSendDBPacket(pObject, 0x49u, 0x40u)
    if (pMover)
    {
        XSendDBPacket xSendDBPacket(pMover, 0x49u, 0x40u);
        xSendDBPacket << stInitInfo;

        XGameServer* pGameServer = XGameServer::Instance();
        if (pGameServer)
        {
            pGameServer->SendDBGame(xSendDBPacket);
        }
    }
}

// ShowAccountPlayTimeEvent (0x140031200)
// IDA-verified: Show account play time event (debug)
void CGocAttendance::ShowAccountPlayTimeEvent()
{
    PS_CHAT_NOTICE stChat;
    stChat.byType = 0;

    swprintf(stChat.strMsg, L"TIME: %I64d %d %d", m_nAccountPlayTimeTick, m_nAccountPlayTimeTick / 1000);

    XSendPacket xSendPacket(7u, 4u);
    xSendPacket << stChat;

    VChunkFile* v11 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    XActor* pActor = v11 ? (XActor*)&v11[3].m_ChunkSizeTempMemOfs : nullptr;
    if (pActor)
    {
        CGocNetwork::Send(pActor, &xSendPacket);
    }
}

// OnAttendance (0x140031C00)
// IDA-verified: Handle attendance check
void CGocAttendance::OnAttendance(__int64 biCurDate)
{
    if (biCurDate < m_biNextAttendance)
        return;

    // IDA: CTime(2000, 1, 1, 0, 0, 0, -1)
    constexpr std::int64_t nMinValidDate = 946656000LL;

    XGameServer* pGameServer = XGameServer::Instance();

    // Process attendance info
    if (m_stAttendanceInfo.byApplyAttendance)
    {
        std::uint32_t dwAttendanceID = GetAttendanceID(biCurDate);
        std::uint32_t dwAttendanceRewardID = 0;

        if (pGameServer->GetResourceMgr().GetTB_CHECK_ATTENDANCE_INFO(dwAttendanceID))
        {
            bool bSendDBReset = false;
            bool bCheckAttendance = true;

            // Check existing attendance
            if (m_stAttendanceInfo.byAttendanceCount > 0 && m_stAttendanceInfo.nAttendance[0] > 0)
            {
                __int64 nLastDate = m_stAttendanceInfo.nAttendance[0];
                std::uint32_t dwLastAttendanceID = GetAttendanceID(nLastDate);
                if (dwAttendanceID != dwLastAttendanceID)
                {
                    bSendDBReset = true;
                    LogHelper::LogError("game.contents",
                        "OnAttendance Reset1 [UCID:%d] [%d!=%d] (%d)",
                        0, dwAttendanceID, dwLastAttendanceID, 475);
                }
            }

            // Check attendance ID match
            if (m_stAttendanceInfo.dwAttendanceID != dwAttendanceID)
            {
                bSendDBReset = true;
                LogHelper::LogError("game.contents",
                    "OnAttendance Reset2 [UCID:%d, %d,%d] (%d)",
                    0, m_stAttendanceInfo.dwAttendanceID, dwAttendanceID, 483);
            }

            // Check if can add attendance
            if (m_stAttendanceInfo.byAttendanceCount < 14)
            {
                if (m_stAttendanceInfo.byAttendanceCount > 0)
                {
                    __int64 nLastAttendance = m_stAttendanceInfo.nAttendance[m_stAttendanceInfo.byAttendanceCount - 1];
                    std::uint32_t dwLastID = GetAttendanceID(nLastAttendance);

                    if (dwAttendanceID == dwLastID)
                    {
                        __int64 nBeforeInit = pGameServer->GetBeforeInitDate();
                        if (nLastAttendance >= nBeforeInit)
                        {
                            bCheckAttendance = false;
                        }
                    }
                    else
                    {
                        bSendDBReset = true;
                        LogHelper::LogError("game.contents",
                            "OnAttendance Reset3 [UCID:%d] [%d!=%d] (%d)",
                            0, dwAttendanceID, dwLastID, 498);
                    }
                }
            }
            else
            {
                bCheckAttendance = false;
            }

            // Send DB reset if needed
            if (bSendDBReset)
            {
                SendDBAttendanceReset(dwAttendanceID);
                bCheckAttendance = true;
            }

            // Add attendance
            if (bCheckAttendance)
            {
                m_stAttendanceInfo.nAttendance[m_stAttendanceInfo.byAttendanceCount++] = biCurDate;
                dwAttendanceRewardID = m_stAttendanceInfo.byAttendanceCount + 100 * dwAttendanceID;
            }
        }
        else
        {
            LogHelper::LogError("game.contents",
                "OnAttendance TB_CHECK_ATTENDANCE_INFO Table Error [UCID:%d / TableID:%d] (%d)",
                0, dwAttendanceID, 536);
        }

        // Send reward
        if (dwAttendanceRewardID)
        {
            AttendanceReward(dwAttendanceRewardID);
        }
    }

    // Process attendance continue
    if (m_stAttendanceContinue.byApplyAttendance)
    {
        std::uint32_t dwRewardItemID = 0;
        std::int16_t shRewardCount = 0;

        TB_CHECK_ATTENDANCE_STREAK* pTBStreak = pGameServer->GetResourceMgr().GetTB_CHECK_ATTENDANCE_STREAK(1u);
        if (pTBStreak)
        {
            // Check continue attendance
            if (m_stAttendanceContinue.byAttendanceCount == 0)
            {
                // First attendance
                dwRewardItemID = (&pTBStreak->Attendance_Streak_Reward_1day)[m_stAttendanceContinue.byAttendanceCount];
                shRewardCount = (std::int16_t*)(&pTBStreak->Attendance_Streak_Reward_1day_Value)[m_stAttendanceContinue.byAttendanceCount * 2];
                m_stAttendanceContinue.nLastAttendanceDate = biCurDate;
                ++m_stAttendanceContinue.byAttendanceCount;
            }
            else if (m_stAttendanceContinue.nLastAttendanceDate <= nMinValidDate)
            {
                LogHelper::LogError("game.contents",
                    "OnAttendance Continue Last Date Error [UCID:%d / Pos:%d / Date:%d ] (%d)",
                    0, m_stAttendanceContinue.byAttendanceCount, m_stAttendanceContinue.nLastAttendanceDate, 598);
            }
            else
            {
                __int64 nLastDate = m_stAttendanceContinue.nLastAttendanceDate;
                __int64 nBeforeInit = pGameServer->GetBeforeInitDate();

                if (nLastDate < nBeforeInit)
                {
                    __int64 nYesterDay = nBeforeInit - 86400;

                    if (nYesterDay <= nLastDate)
                    {
                        // Continue streak
                        if (m_stAttendanceContinue.byAttendanceCount >= 3)
                        {
                            m_stAttendanceContinue.byAttendanceCount = 0;
                        }
                        dwRewardItemID = (&pTBStreak->Attendance_Streak_Reward_1day)[m_stAttendanceContinue.byAttendanceCount];
                        shRewardCount = (std::int16_t*)(&pTBStreak->Attendance_Streak_Reward_1day_Value)[m_stAttendanceContinue.byAttendanceCount * 2];
                        m_stAttendanceContinue.nLastAttendanceDate = biCurDate;
                        ++m_stAttendanceContinue.byAttendanceCount;
                    }
                    else
                    {
                        // Reset streak
                        m_stAttendanceContinue.byAttendanceCount = 0;
                        dwRewardItemID = (&pTBStreak->Attendance_Streak_Reward_1day)[m_stAttendanceContinue.byAttendanceCount];
                        shRewardCount = (std::int16_t*)(&pTBStreak->Attendance_Streak_Reward_1day_Value)[m_stAttendanceContinue.byAttendanceCount * 2];
                        m_stAttendanceContinue.nLastAttendanceDate = biCurDate;
                        ++m_stAttendanceContinue.byAttendanceCount;
                    }
                }
            }

            // Send continue reward
            if (dwRewardItemID && shRewardCount > 0)
            {
                AttendanceContinueReward(dwRewardItemID, shRewardCount);
            }
        }
        else
        {
            LogHelper::LogError("game.contents",
                "OnAttendance TB_CHECK_ATTENDANCE_STREAK Table Error [UCID:%d / TableID:%d] (%d)",
                0, 1, 614);
        }
    }

    // Update next attendance time
    m_biNextAttendance = pGameServer->GetUpdateDate(9);
}

// OnAttendancePlayTime (0x140032550)
// IDA-verified: Handle play time attendance
void CGocAttendance::OnAttendancePlayTime(__int64 biCurDate)
{
    if (!m_biNextAttendancePlayTime)
        return;

    XGameServer* pGameServer = XGameServer::Instance();
    TB_CHECK_ACCESS_REWARD* pTBCheckAccess = pGameServer->GetResourceMgr().GetTB_CHECK_ACCESS_REWARD(1u);

    if (!pTBCheckAccess)
    {
        m_biNextAttendancePlayTime = 0;
        return;
    }

    if (!m_stAttendancePlayTime.byApplyAttendance)
        return;

    bool bReset = m_stAttendancePlayTime.nUpdateDate < pGameServer->GetBeforeInitDate();
    if (m_biNextAttendancePlayTime < biCurDate)
        bReset = true;

    if (bReset)
    {
        LogHelper::LogError("game.contents",
            "OnAttendancePlayTime - Reset (UCID:%d, Type:%d, pos:%d) (%d)",
            0, m_stAttendancePlayTime.dwType, m_stAttendancePlayTime.byCurPos, 657);

        m_stAttendancePlayTime.byCurPos = 0;
        m_stAttendancePlayTime.nPlaySec = 0;
        m_stAttendancePlayTime.nUpdateDate = biCurDate;

        XSendPacket xSendPacket(0x2Au, 5u);

        VChunkFile* v19 = std::list<CBattleZone*>::size((VChunkLocker*)this);
        XActor* pActor = v19 ? (XActor*)&v19[3].m_ChunkSizeTempMemOfs : nullptr;
        if (pActor)
        {
            CGocNetwork::Send(pActor, &xSendPacket);
        }

        // Check if there's time entry for current position
        std::int32_t* pTimeEntry = &pTBCheckAccess->Check_Attendance_Day_Time_1 + m_stAttendancePlayTime.byCurPos;
        if (*pTimeEntry)
        {
            m_biNextAttendancePlayTime = pGameServer->GetUpdateDate(9);
        }
        else
        {
            m_biNextAttendancePlayTime = 0;
        }
    }
    else if (m_stAttendancePlayTime.byCurPos < 3)
    {
        std::int32_t* pTimeEntry = &pTBCheckAccess->Check_Attendance_Day_Time_1 + m_stAttendancePlayTime.byCurPos;
        if (*pTimeEntry)
        {
            m_stAttendancePlayTime.nUpdateDate = biCurDate;
            m_stAttendancePlayTime.nPlaySec += 5;

            if (*pTimeEntry < m_stAttendancePlayTime.nPlaySec)
            {
                m_stAttendancePlayTime.nPlaySec = *pTimeEntry;
            }

            if (*pTimeEntry == m_stAttendancePlayTime.nPlaySec)
            {
                AttendancePlayTimeReward();
            }
        }
        else
        {
            m_biNextAttendancePlayTime = 0;
        }
    }
}

// AttendanceReward (0x140032900)
// IDA-verified: Send attendance reward to DB
bool CGocAttendance::AttendanceReward(std::uint32_t dwRewardID)
{
    VChunkFile* v2 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    CUser* pUser = (CUser*)_RTDynamicCast_0(v2, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
    if (!pUser)
        return false;

    XGameServer* pGameServer = XGameServer::Instance();
    TB_CHECK_ATTENDANCE_REWARD* pTBCheckReward = pGameServer->GetResourceMgr().GetTB_CHECK_ATTENDANCE_REWARD(dwRewardID);

    if (!pTBCheckReward)
    {
        LogHelper::LogError("game.contents",
            "AttendanceReward error - No Table TB_CHECK_ATTENDANCE_REWARD (UCID:%d, Type:%d, ID:%d) (%d)",
            0, m_stAttendanceInfo.dwType, dwRewardID, 709);
        return false;
    }

    if (!pGameServer->GetResourceMgr().GetTB_ITEM(pTBCheckReward->Attendance_Item_Reward_ID_1))
    {
        LogHelper::LogError("game.contents",
            "AttendanceReward error - No Table pTB_ITEM (UCID:%d, Type:%d, ID:%d) (%d)",
            0, m_stAttendanceInfo.dwType, dwRewardID, 716);
        return false;
    }

    if (!pTBCheckReward->Attendance_Item_Reward_Num_1)
    {
        LogHelper::LogError("game.contents",
            "AttendanceReward error - Error Count (UCID:%d, Type:%d, ID:%d) (%d)",
            0, m_stAttendanceInfo.dwType, dwRewardID, 722);
        return false;
    }

    ST_CREATE_ITEM stItemInfo;
    stItemInfo.nItemID = pTBCheckReward->Attendance_Item_Reward_ID_1;
    stItemInfo.shCount = pTBCheckReward->Attendance_Item_Reward_Num_1;

    VChunkFile* v26 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    IXObject* pObject = v26 ? (IXObject*)&v26[3].m_ChunkSizeTempMemOfs : nullptr;

    XSendDBPacket xSendDBPacket(pObject, 0x49u, 0x42u);
    xSendDBPacket.XParse << pUser->GetUAID();

    UXActorID v20;
    pUser->GetActorID(&v20);
    xSendDBPacket.XParse << v20.dwActorID;

    xSendDBPacket << m_stAttendanceInfo;
    xSendDBPacket << stItemInfo;

    XGameServer::SendDBGame(pGameServer, &xSendDBPacket);

    return false;
}

// AttendanceContinueReward (0x1400333D0)
// IDA-verified: Send continue attendance reward to DB
bool CGocAttendance::AttendanceContinueReward(std::uint32_t dwRewardItemID, std::int16_t shRewardCount)
{
    VChunkFile* v3 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    CUser* pUser = (CUser*)_RTDynamicCast_0(v3, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
    if (!pUser)
        return false;

    XGameServer* pGameServer = XGameServer::Instance();

    if (!pGameServer->GetResourceMgr().GetTB_ITEM(dwRewardItemID))
    {
        LogHelper::LogError("game.contents",
            "AttendanceContinueReward error - No Table pTB_ITEM (UCID:%d, Type:%d, ItemID:%d) (%d)",
            0, m_stAttendanceContinue.dwType, dwRewardItemID, 842);
        return false;
    }

    if (shRewardCount <= 0)
    {
        LogHelper::LogError("game.contents",
            "AttendanceContinueReward error - Error Count (UCID:%d, Type:%d, Count:%d) (%d)",
            0, m_stAttendanceContinue.dwType, shRewardCount, 848);
        return false;
    }

    ST_CREATE_ITEM stItemInfo;
    stItemInfo.nItemID = dwRewardItemID;
    stItemInfo.shCount = shRewardCount;

    VChunkFile* v23 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    IXObject* pObject = v23 ? (IXObject*)&v23[3].m_ChunkSizeTempMemOfs : nullptr;

    XSendDBPacket xSendDBPacket(pObject, 0x49u, 0x43u);
    xSendDBPacket.XParse << pUser->GetUAID();

    UXActorID v17;
    pUser->GetActorID(&v17);
    xSendDBPacket.XParse << v17.dwActorID;

    xSendDBPacket << m_stAttendanceContinue;
    xSendDBPacket << stItemInfo;

    XGameServer::SendDBGame(pGameServer, &xSendDBPacket);

    return true;
}

// AttendancePlayTimeReward (0x140033DF0)
// IDA-verified: Send play time attendance reward to DB
bool CGocAttendance::AttendancePlayTimeReward()
{
    VChunkFile* v1 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    CUser* pUser = (CUser*)_RTDynamicCast_0(v1, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
    if (!pUser)
        return false;

    std::uint8_t byCurPos = m_stAttendancePlayTime.byCurPos;
    int nPlayTime = m_stAttendancePlayTime.nPlaySec;

    if (byCurPos >= 3)
        return false;

    XGameServer* pGameServer = XGameServer::Instance();
    TB_CHECK_ACCESS_REWARD* pTBCheckAccess = pGameServer->GetResourceMgr().GetTB_CHECK_ACCESS_REWARD(1u);
    if (!pTBCheckAccess)
        return false;

    std::int32_t* pTimeEntry = &pTBCheckAccess->Check_Attendance_Day_Time_1 + byCurPos;
    if (nPlayTime != *pTimeEntry)
        return false;

    std::uint32_t dwRewardItemID = *(&pTBCheckAccess->Check_Access_Reward_ID_1st + byCurPos);
    if (!pGameServer->GetResourceMgr().GetTB_ITEM(dwRewardItemID))
    {
        LogHelper::LogError("game.contents",
            "AttendancePlayTimeReward TB_ITEM NULL (UCID:%d, ItemID:%d, Pos:%d)",
            0, dwRewardItemID, byCurPos);
        return false;
    }

    ST_CREATE_ITEM stItemInfo;
    stItemInfo.nItemID = dwRewardItemID;
    stItemInfo.shCount = 1;

    ++m_stAttendancePlayTime.byCurPos;
    m_stAttendancePlayTime.nPlaySec = 0;

    VChunkFile* v24 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    IXObject* pObject = v24 ? (IXObject*)&v24[3].m_ChunkSizeTempMemOfs : nullptr;

    XSendDBPacket xSendDBPacket(pObject, 0x49u, 0x45u);
    xSendDBPacket.XParse << pUser->GetUAID();

    UXActorID v17;
    pUser->GetActorID(&v17);
    xSendDBPacket.XParse << v17.dwActorID;

    xSendDBPacket << m_stAttendancePlayTime;
    xSendDBPacket << stItemInfo;

    XGameServer::SendDBGame(pGameServer, &xSendDBPacket);

    return true;
}

/**
 * Cheat_ShowAttendanceInfo (0x140035870)
 * Debug function that broadcasts attendance info via chat messages
 */
void CGocAttendance::Cheat_ShowAttendanceInfo() {
    CUser* pUser = reinterpret_cast<CUser*>(GetOwnerMover());
    if (!pUser) {
        return;
    }

    XGameServer* pGameServer = XGameServer::GetInstance();
    if (!pGameServer) {
        return;
    }

    // Send attendance count
    {
        char szBuffer[256];
        sprintf_s(szBuffer, sizeof(szBuffer), "AttendanceCount: %d", m_stAttendanceInfo.byAttendanceCount);
        pUser->SendNoticeChatMessage(szBuffer);
    }

    // Send individual attendance dates
    for (int i = 0; i < 32; ++i) {
        if (m_stAttendanceInfo.biAttendance[i] != 0) {
            char szBuffer[256];
            sprintf_s(szBuffer, sizeof(szBuffer), "Attendance[%d]: %lld", i, m_stAttendanceInfo.biAttendance[i]);
            pUser->SendNoticeChatMessage(szBuffer);
        }
    }

    // Send continue attendance info
    {
        char szBuffer[256];
        sprintf_s(szBuffer, sizeof(szBuffer), "ContinueAttendance: Count=%d, CurCount=%d, Date=%lld",
                  m_stAttendanceContinue.byAttendanceContinueCount,
                  m_stAttendanceContinue.byCurAttendanceContinueCount,
                  m_stAttendanceContinue.biAttendanceContinueDate);
        pUser->SendNoticeChatMessage(szBuffer);
    }

    // Send play time info
    {
        char szBuffer[256];
        sprintf_s(szBuffer, sizeof(szBuffer), "PlayTime: CurPos=%d, PlaySec=%d, NextTime=%lld",
                  m_stAttendancePlayTime.byCurPos,
                  m_stAttendancePlayTime.nPlaySec,
                  m_biNextAttendancePlayTime);
        pUser->SendNoticeChatMessage(szBuffer);
    }
}

/**
 * Cheat_AttendancePlayTimeUpdate (0x140036740)
 * Simple setter for play time position and seconds
 */
void CGocAttendance::Cheat_AttendancePlayTimeUpdate(std::uint8_t byPos, int nPlaySec) {
    m_stAttendancePlayTime.byCurPos = byPos;
    m_stAttendancePlayTime.nPlaySec = nPlaySec;
}

/**
 * AttendanceRewardRes (0x140032cc0)
 * Handles DB response for attendance rewards
 */
bool CGocAttendance::AttendanceRewardRes(PS_DB_ATTENDANCE_REWARD& stReward) {
    CUser* pUser = reinterpret_cast<CUser*>(GetOwnerMover());
    if (!pUser) {
        return false;
    }

    XGameServer* pGameServer = XGameServer::GetInstance();
    if (!pGameServer) {
        return false;
    }

    // Update attendance info if reward was successful
    if (stReward.byState == 1) {
        // Mark this attendance as received
        if (stReward.byAttendanceIndex < 32) {
            // Get current date
            __int64 biCurDate = 0;
            time_t tNow = time(nullptr);
            struct tm* pTm = localtime(&tNow);
            if (pTm) {
                biCurDate = (pTm->tm_year + 1900) * 10000 + (pTm->tm_mon + 1) * 100 + pTm->tm_mday;
            }

            m_stAttendanceInfo.biAttendance[stReward.byAttendanceIndex] = biCurDate;
            ++m_stAttendanceInfo.byAttendanceCount;
        }
    }

    // Send reward items
    if (stReward.dwRewardItemID != 0) {
        // Create item and send to user
        PS_ITEM_SLOT_INFO stItemInfo = {};
        stItemInfo.nItemID = stReward.dwRewardItemID;
        stItemInfo.shCount = static_cast<std::int16_t>(stReward.nRewardCount);

        // Send item via post or direct inventory
        pUser->SendRewardItem(stItemInfo);
    }

    // Send response to client
    PS_ATTENDANCE_REWARD_RES stRes = {};
    stRes.byState = stReward.byState;
    stRes.byAttendanceIndex = stReward.byAttendanceIndex;
    stRes.dwRewardItemID = stReward.dwRewardItemID;
    stRes.nRewardCount = stReward.nRewardCount;

    XSendPacket xSendPacket(pUser);
    xSendPacket << stRes;
    xSendPacket.Send();

    return true;
}

/**
 * AttendanceContinueRewardRes (0x1400336f0)
 * Handles DB response for continue attendance rewards
 */
bool CGocAttendance::AttendanceContinueRewardRes(PS_DB_ATTENDANCE_CONTINUE_REWARD& stReward) {
    CUser* pUser = reinterpret_cast<CUser*>(GetOwnerMover());
    if (!pUser) {
        return false;
    }

    XGameServer* pGameServer = XGameServer::GetInstance();
    if (!pGameServer) {
        return false;
    }

    // Update continue attendance info if reward was successful
    if (stReward.byState == 1) {
        ++m_stAttendanceContinue.byCurAttendanceContinueCount;

        // Check if completed all rewards
        if (m_stAttendanceContinue.byCurAttendanceContinueCount >= m_stAttendanceContinue.byAttendanceContinueCount) {
            m_stAttendanceContinue.byCurAttendanceContinueCount = 0;
        }
    }

    // Send reward items
    if (stReward.dwRewardItemID != 0) {
        PS_ITEM_SLOT_INFO stItemInfo = {};
        stItemInfo.nItemID = stReward.dwRewardItemID;
        stItemInfo.shCount = static_cast<std::int16_t>(stReward.nRewardCount);

        pUser->SendRewardItem(stItemInfo);
    }

    // Send response to client
    PS_ATTENDANCE_CONTINUE_REWARD_RES stRes = {};
    stRes.byState = stReward.byState;
    stRes.byCurCount = m_stAttendanceContinue.byCurAttendanceContinueCount;
    stRes.dwRewardItemID = stReward.dwRewardItemID;
    stRes.nRewardCount = stReward.nRewardCount;

    XSendPacket xSendPacket(pUser);
    xSendPacket << stRes;
    xSendPacket.Send();

    return true;
}

/**
 * AttendancePlayTimeRewardRes (0x140034170)
 * Handles DB response for play time rewards
 */
bool CGocAttendance::AttendancePlayTimeRewardRes(PS_DB_ATTENDANCE_PLAYTIME_REWARD& stReward) {
    CUser* pUser = reinterpret_cast<CUser*>(GetOwnerMover());
    if (!pUser) {
        return false;
    }

    XGameServer* pGameServer = XGameServer::GetInstance();
    if (!pGameServer) {
        return false;
    }

    // Update play time info if reward was successful
    if (stReward.byState == 1) {
        ++m_stAttendancePlayTime.byCurPos;
        m_stAttendancePlayTime.nPlaySec = 0;
    }

    // Send reward items
    if (stReward.dwRewardItemID != 0) {
        PS_ITEM_SLOT_INFO stItemInfo = {};
        stItemInfo.nItemID = stReward.dwRewardItemID;
        stItemInfo.shCount = static_cast<std::int16_t>(stReward.nRewardCount);

        pUser->SendRewardItem(stItemInfo);
    }

    // Send response to client
    PS_ATTENDANCE_PLAYTIME_REWARD_RES stRes = {};
    stRes.byState = stReward.byState;
    stRes.byCurPos = m_stAttendancePlayTime.byCurPos;
    stRes.dwRewardItemID = stReward.dwRewardItemID;
    stRes.nRewardCount = stReward.nRewardCount;

    XSendPacket xSendPacket(pUser);
    xSendPacket << stRes;
    xSendPacket.Send();

    return true;
}

// SendDBAttendanceReset (0x140035260)
// IDA-verified: Send attendance reset request to database
// IDA: main=0x49, sub=0x46
void CGocAttendance::SendDBAttendanceReset(std::uint32_t dwAttendanceID)
{
    // IDA: Get owner mover and cast to CUser
    CMover* pMover = GetOwnerMover();
    if (!pMover)
        return;

    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (!pUser)
        return;

    // IDA: PS_ATTENDANCE_INFO::AttendanceReset(&this->m_stAttendanceInfo, dwAttendanceID)
    // Reset attendance info with new attendance ID
    m_stAttendanceInfo.byAttendanceCount = 0;
    for (int i = 0; i < 14; ++i)
    {
        m_stAttendanceInfo.nAttendance[i] = 0;
    }
    m_stAttendanceInfo.dwAttendanceID = dwAttendanceID;

    // IDA: Get IXObject from owner
    IXObject* pObject = pMover;

    // IDA: XSendDBPacket xSendDBPacket(pObject, 0x49u, 0x46u)
    XSendDBPacket xSendDBPacket(pObject, 0x49u, 0x46u);

    // IDA: XParse << GetUAID()
    xSendDBPacket.XParse << pUser->GetUAID();

    // IDA: GetActorID and send
    UXActorID actorID;
    pUser->GetActorID(&actorID);
    xSendDBPacket.XParse << actorID.dwActorID;

    // IDA: XParse << dwType << dwAttendanceID
    xSendDBPacket.XParse << m_stAttendanceInfo.dwType;
    xSendDBPacket.XParse << m_stAttendanceInfo.dwAttendanceID;

    // IDA: XGameServer::SendDBGame(v6, &xSendDBPacket)
    XGameServer* pGameServer = XGameServer::Instance();
    if (pGameServer)
    {
        pGameServer->SendDBGame(xSendDBPacket);
    }
}

// Cheat_AttendanceReset (0x140035430)
// IDA-verified: GM cheat command to reset attendance
void CGocAttendance::Cheat_AttendanceReset()
{
    // IDA: ATL::CTime::GetTickCount(&tCurr) - get current time
    // IDA: AttendanceID = CGocAttendance::GetAttendanceID(this, (__int64)v1)
    XGameServer* pGameServer = XGameServer::Instance();
    if (!pGameServer)
        return;

    __int64 biCurDate = pGameServer->GetCurDate();
    std::uint32_t dwAttendanceID = GetAttendanceID(biCurDate);

    // IDA: CGocAttendance::SendDBAttendanceReset(this, AttendanceID)
    SendDBAttendanceReset(dwAttendanceID);

    // IDA: this->m_biNextAttendance = XGameServer::GetBeforeInitDate(v3)
    m_biNextAttendance = pGameServer->GetBeforeInitDate();
}

// Cheat_AttendanceContinueReset (0x140035490)
// IDA-verified: GM cheat command to reset continue attendance
// IDA: main=0x49, sub=0x47
void CGocAttendance::Cheat_AttendanceContinueReset()
{
    // IDA: Get owner mover and cast to CUser
    CMover* pMover = GetOwnerMover();
    if (!pMover)
        return;

    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (!pUser)
        return;

    // IDA: Reset attendance continue data
    m_stAttendanceContinue.byAttendanceCount = 0;
    m_stAttendanceContinue.nLastAttendanceDate = 0;

    // IDA: this->m_biNextAttendance = XGameServer::GetBeforeInitDate(v2)
    XGameServer* pGameServer = XGameServer::Instance();
    if (pGameServer)
    {
        m_biNextAttendance = pGameServer->GetBeforeInitDate();
    }

    // IDA: Send DB packet
    IXObject* pObject = pMover;
    XSendDBPacket xSendDBPacket(pObject, 0x49u, 0x47u);

    // IDA: XParse << GetUAID()
    xSendDBPacket.XParse << pUser->GetUAID();

    // IDA: GetActorID and send
    UXActorID actorID;
    pUser->GetActorID(&actorID);
    xSendDBPacket.XParse << actorID.dwActorID;

    // IDA: operator<<(&xSendDBPacket, &this->m_stAttendanceContinue)
    xSendDBPacket << m_stAttendanceContinue;

    // IDA: XGameServer::SendDBGame(v6, &xSendDBPacket)
    if (pGameServer)
    {
        pGameServer->SendDBGame(xSendDBPacket);
    }
}

// Cheat_AttendancePlayTimeReset (0x140035650)
// IDA-verified: GM cheat command to reset play time attendance
// IDA: main=0x49, sub=0x48
void CGocAttendance::Cheat_AttendancePlayTimeReset()
{
    // IDA: Get owner mover and cast to CUser
    CMover* pMover = GetOwnerMover();
    if (!pMover)
        return;

    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (!pUser)
        return;

    // IDA: ATL::CTime::CTime(&tCurr, 2001, 1, 1, 0, 0, 0, -1) - use 2001-01-01 as base date
    // IDA: this->m_stAttendancePlayTime.byCurPos = 0
    // IDA: this->m_stAttendancePlayTime.nPlaySec = 0
    // IDA: this->m_stAttendancePlayTime.nUpdateDate = (__int64)v1 (converted CTime)
    m_stAttendancePlayTime.byCurPos = 0;
    m_stAttendancePlayTime.nPlaySec = 0;

    // IDA: Use CTime(2001, 1, 1, 0, 0, 0) converted to __int64
    // 2001-01-01 00:00:00 UTC = 978307200
    constexpr std::int64_t nBaseDate = 978307200LL;
    m_stAttendancePlayTime.nUpdateDate = nBaseDate;

    // IDA: this->m_biNextAttendancePlayTime = XGameServer::GetUpdateDate(v2, 9u)
    XGameServer* pGameServer = XGameServer::Instance();
    if (pGameServer)
    {
        m_biNextAttendancePlayTime = pGameServer->GetUpdateDate(9);
    }

    // IDA: Send DB packet
    IXObject* pObject = pMover;
    XSendDBPacket xSendDBPacket(pObject, 0x49u, 0x48u);

    // IDA: XParse << GetUAID()
    xSendDBPacket.XParse << pUser->GetUAID();

    // IDA: GetActorID and send
    UXActorID actorID;
    pUser->GetActorID(&actorID);
    xSendDBPacket.XParse << actorID.dwActorID;

    // IDA: operator<<(&xSendDBPacket, &this->m_stAttendancePlayTime)
    xSendDBPacket << m_stAttendancePlayTime;

    // IDA: XGameServer::SendDBGame(v6, &xSendDBPacket)
    if (pGameServer)
    {
        pGameServer->SendDBGame(xSendDBPacket);
    }
}
