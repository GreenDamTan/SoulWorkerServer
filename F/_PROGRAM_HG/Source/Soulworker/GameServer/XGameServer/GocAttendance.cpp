// CGocAttendance Implementation
// Restored from GameServer.exe IDA decompilation
// Port 10004 - Addresses: 0x140030170 - 0x14003679B

#include "GocAttendance.h"
#include "Mover.h"
#include "User.h"
#include "XGameServer.h"
#include "XPacket.h"
#include "XSendPacket.h"
#include "XSendDBPacket.h"
#include "TimeHelper.h"
#include "ItemHelper.h"
#include "StringHelper.h"
#include <ctime>
#include <algorithm>

// External table references
extern std::vector<TB_CHECK_ATTENDANCE_INFO> g_vecCheckAttendanceInfo;
extern std::vector<TB_CHECK_ATTENDANCE_REWARD> g_vecCheckAttendanceReward;
extern std::vector<TB_CHECK_ATTENDANCE_STREAK> g_vecCheckAttendanceStreak;
extern std::vector<TB_CHECK_ACCESS_REWARD> g_vecCheckAccessReward;
extern std::vector<TB_ITEM> g_vecItem;

// ============================================================================
// PS_ATTENDANCE_INFO Implementation
// ============================================================================

void PS_ATTENDANCE_INFO::Reset(std::uint32_t dwNewID) {
    byApplyAttendance = 0;
    dwType = 0;
    dwAttendanceID = dwNewID;
    byAttendanceCount = 0;
    std::memset(nAttendance, 0, sizeof(nAttendance));
}

// ============================================================================
// PS_ATTENDANCE_CONTINUE Implementation
// ============================================================================

PS_ATTENDANCE_CONTINUE::PS_ATTENDANCE_CONTINUE() {
    byApplyAttendance = 0;
    dwType = 0;
    byAttendanceCount = 0;
    nLastAttendanceDate = 0;
}

// ============================================================================
// PS_ATTENDANCE_PLAY_TIME Implementation
// ============================================================================

PS_ATTENDANCE_PLAY_TIME::PS_ATTENDANCE_PLAY_TIME() {
    byApplyAttendance = 0;
    dwType = 0;
    byCurPos = 0;
    nPlaySec = 0;
    nUpdateDate = 0;
}

// ============================================================================
// CGocAttendance Implementation
// ============================================================================

// Constructor (0x140030270)
CGocAttendance::CGocAttendance() {
    m_dw64AttendanceCheckTick = 0;
    m_dw64PlayTimeByDay = 0;
    m_biNextAttendance = 0;
    m_biNextAttendancePlayTime = 0;
    m_nNextAccountPlayTime = 0;
    m_nAccountPlayTimeTick = 0;
    m_byAccountPlayType = 0;
    m_nPrevAccountPlayTimeTick = 0;
    m_nAccountPlayTimeDBSaveTick = 0;
}

// Destructor (0x140030320)
CGocAttendance::~CGocAttendance() {
    // Save play time data on destruction
    SendDBPlayTimeByDay();
}

// Init (0x140030350)
void CGocAttendance::Init() {
    // Initialize attendance check tick
    m_dw64AttendanceCheckTick = 0;
    m_dw64PlayTimeByDay = 0;
    m_biNextAttendance = 0;
    m_biNextAttendancePlayTime = 0;
    
    // Reset attendance data structures
    std::memset(&m_stAttendanceInfo, 0, sizeof(m_stAttendanceInfo));
    std::memset(&m_stAttendanceContinue, 0, sizeof(m_stAttendanceContinue));
    std::memset(&m_stAttendancePlayTime, 0, sizeof(m_stAttendancePlayTime));
    
    // Initialize play time by day tracking
    InitPlayTimebyDay();
    
    // Initialize account play time event
    m_nNextAccountPlayTime = 0;
    m_nAccountPlayTimeTick = 0;
    m_byAccountPlayType = 0;
    m_nPrevAccountPlayTimeTick = 0;
    m_nAccountPlayTimeDBSaveTick = 0;
}

// LogOut (0x140030420)
void CGocAttendance::LogOut() {
    // Send attendance data to database on logout
    SendDBAttendanceLogOut();
    
    // Send play time by day data
    SendDBPlayTimeByDay();
    
    // Save account play time event if active
    if (m_byAccountPlayType != 0) {
        SaveAccountPlayTimeEvent();
    }
}

// OnUpdate (0x140030470)
void CGocAttendance::OnUpdate() {
    // Get current time
    std::int64_t biCurTime = TimeHelper::GetTime();
    
    // Check attendance periodically (every 60 seconds)
    if (biCurTime >= m_biNextAttendance) {
        // Perform attendance check
        OnAttendance(biCurTime);
        
        // Set next check time (60 seconds later)
        m_biNextAttendance = biCurTime + 60;
    }
    
    // Check play time attendance periodically
    if (biCurTime >= m_biNextAttendancePlayTime) {
        // Perform play time attendance check
        OnAttendancePlayTime(biCurTime);
        
        // Set next check time (60 seconds later)
        m_biNextAttendancePlayTime = biCurTime + 60;
    }
    
    // Update account play time event if active
    if (m_byAccountPlayType != 0) {
        UpdateAccountPlayTimeEvent();
    }
}

// SetAttendance (0x140030170)
void CGocAttendance::SetAttendance(PS_ATTENDANCE_INFO& stAttendance) {
    // Copy attendance info data
    std::memcpy(&m_stAttendanceInfo, &stAttendance, sizeof(PS_ATTENDANCE_INFO));
    
    // Validate attendance data
    AttendanceVailidityCheck(m_stAttendanceInfo);
}

// SetAttendanceContinue (0x1400301C0)
void CGocAttendance::SetAttendanceContinue(PS_ATTENDANCE_CONTINUE& stContinue) {
    // Copy streak data
    std::memcpy(&m_stAttendanceContinue, &stContinue, sizeof(PS_ATTENDANCE_CONTINUE));
    
    // Validate streak data
    AttendanceContinueVailidityCheck(m_stAttendanceContinue);
}

// SetAttendancePlayTime (0x140030210)
void CGocAttendance::SetAttendancePlayTime(PS_ATTENDANCE_PLAY_TIME& stPlayTime) {
    // Copy play time data
    std::memcpy(&m_stAttendancePlayTime, &stPlayTime, sizeof(PS_ATTENDANCE_PLAY_TIME));
    
    // Validate play time data
    AttendancePlayTimeVailidityCheck(m_stAttendancePlayTime);
}

// LoadAttendanceInfo (0x140031340)
bool CGocAttendance::LoadAttendanceInfo(PS_ATTENDANCE_INFO& stAttendance,
                                       PS_ATTENDANCE_CONTINUE& stContinue,
                                       PS_ATTENDANCE_PLAY_TIME& stPlayTime) {
    // Set attendance data
    SetAttendance(stAttendance);
    SetAttendanceContinue(stContinue);
    SetAttendancePlayTime(stPlayTime);
    
    // Send attendance info to client
    SendAttendanceInfo();
    
    return true;
}

// AttendanceVailidityCheck (0x1400315B0)
bool CGocAttendance::AttendanceVailidityCheck(PS_ATTENDANCE_INFO& stAttendance) {
    // Check if attendance is enabled
    if (stAttendance.byApplyAttendance == 0) {
        return false;
    }
    
    // Get current date
    std::int64_t biCurDate = TimeHelper::GetTime();
    std::uint32_t dwCurAttendanceID = GetAttendanceID(biCurDate);
    
    // Check if attendance ID changed (new month)
    if (stAttendance.dwAttendanceID != dwCurAttendanceID) {
        // Reset attendance for new month
        stAttendance.Reset(dwCurAttendanceID);
        
        // Send DB reset request
        SendDBAttendanceReset(dwCurAttendanceID);
        
        return true;
    }
    
    // Validate attendance count (max 14 days)
    if (stAttendance.byAttendanceCount > 14) {
        stAttendance.byAttendanceCount = 14;
    }
    
    return true;
}

// AttendanceContinueVailidityCheck (0x1400317F0)
bool CGocAttendance::AttendanceContinueVailidityCheck(PS_ATTENDANCE_CONTINUE& stAttendance) {
    // Check if streak tracking is enabled
    if (stAttendance.byApplyAttendance == 0) {
        return false;
    }
    
    // Get current date
    std::int64_t biCurDate = TimeHelper::GetTime();
    std::int64_t biLastDate = stAttendance.nLastAttendanceDate;
    
    // Check if streak should be reset (more than 1 day gap)
    if (biLastDate != 0) {
        std::int64_t biDiffDays = (biCurDate - biLastDate) / 86400;
        
        if (biDiffDays > 1) {
            // Reset streak
            stAttendance.byAttendanceCount = 0;
            stAttendance.nLastAttendanceDate = 0;
        }
    }
    
    // Validate streak count (max 3)
    if (stAttendance.byAttendanceCount > 3) {
        stAttendance.byAttendanceCount = 3;
    }
    
    return true;
}

// AttendancePlayTimeVailidityCheck (0x140031940)
bool CGocAttendance::AttendancePlayTimeVailidityCheck(PS_ATTENDANCE_PLAY_TIME& stPlayTime) {
    // Check if play time tracking is enabled
    if (stPlayTime.byApplyAttendance == 0) {
        return false;
    }
    
    // Get current date
    std::int64_t biCurDate = TimeHelper::GetTime();
    
    // Check if date changed (new day)
    if (stPlayTime.nUpdateDate != 0) {
        std::int64_t biDiffDays = (biCurDate - stPlayTime.nUpdateDate) / 86400;
        
        if (biDiffDays > 0) {
            // Reset play time for new day
            stPlayTime.byCurPos = 0;
            stPlayTime.nPlaySec = 0;
            stPlayTime.nUpdateDate = biCurDate;
        }
    }
    
    // Validate position (max 3 rewards)
    if (stPlayTime.byCurPos > 3) {
        stPlayTime.byCurPos = 3;
    }
    
    return true;
}

// GetAttendanceID (0x140031B80)
std::uint32_t CGocAttendance::GetAttendanceID(std::int64_t biCurDate) {
    // Convert time to struct tm
    std::time_t time = static_cast<std::time_t>(biCurDate);
    std::tm* pTm = std::localtime(&time);
    
    if (pTm == nullptr) {
        return 0;
    }
    
    // Calculate attendance ID: Year * 100 + Month
    // Example: 2024-06 -> 202406
    std::uint32_t dwAttendanceID = (pTm->tm_year + 1900) * 100 + (pTm->tm_mon + 1);
    
    return dwAttendanceID;
}

// OnAttendance (0x140031C00)
void CGocAttendance::OnAttendance(std::int64_t biCurDate) {
    // Check if attendance is enabled
    if (m_stAttendanceInfo.byApplyAttendance == 0) {
        return;
    }
    
    // Validate attendance data
    AttendanceVailidityCheck(m_stAttendanceInfo);
    
    // Get today's date (midnight)
    std::time_t time = static_cast<std::time_t>(biCurDate);
    std::tm* pTm = std::localtime(&time);
    if (pTm == nullptr) {
        return;
    }
    
    pTm->tm_hour = 0;
    pTm->tm_min = 0;
    pTm->tm_sec = 0;
    std::int64_t biToday = std::mktime(pTm);
    
    // Check if already attended today
    for (int i = 0; i < m_stAttendanceInfo.byAttendanceCount; ++i) {
        if (m_stAttendanceInfo.nAttendance[i] == biToday) {
            // Already attended today
            return;
        }
    }
    
    // Check if we can add attendance (max 14 days)
    if (m_stAttendanceInfo.byAttendanceCount >= 14) {
        return;
    }
    
    // Add attendance for today
    m_stAttendanceInfo.nAttendance[m_stAttendanceInfo.byAttendanceCount] = biToday;
    m_stAttendanceInfo.byAttendanceCount++;
    
    // Update streak attendance
    if (m_stAttendanceContinue.byApplyAttendance != 0) {
        // Check streak continuity
        std::int64_t biLastDate = m_stAttendanceContinue.nLastAttendanceDate;
        
        if (biLastDate == 0 || (biToday - biLastDate) == 86400) {
            // Continue streak or start new streak
            if (m_stAttendanceContinue.byAttendanceCount < 3) {
                m_stAttendanceContinue.byAttendanceCount++;
            }
        } else if ((biToday - biLastDate) > 86400) {
            // Reset streak
            m_stAttendanceContinue.byAttendanceCount = 1;
        }
        
        m_stAttendanceContinue.nLastAttendanceDate = biToday;
    }
    
    // Send attendance data to database
    SendDBAttendance();
}

// OnAttendancePlayTime (0x140032550)
void CGocAttendance::OnAttendancePlayTime(std::int64_t biCurDate) {
    // Check if play time tracking is enabled
    if (m_stAttendancePlayTime.byApplyAttendance == 0) {
        return;
    }
    
    // Validate play time data
    AttendancePlayTimeVailidityCheck(m_stAttendancePlayTime);
    
    // Update date if needed
    if (m_stAttendancePlayTime.nUpdateDate == 0) {
        m_stAttendancePlayTime.nUpdateDate = biCurDate;
    }
}

// AttendanceReward (0x140032900)
bool CGocAttendance::AttendanceReward(std::uint32_t dwRewardID) {
    // Check if attendance is enabled
    if (m_stAttendanceInfo.byApplyAttendance == 0) {
        return false;
    }
    
    // Validate reward ID
    if (dwRewardID == 0 || dwRewardID > g_vecCheckAttendanceReward.size()) {
        return false;
    }
    
    // Get reward info
    const TB_CHECK_ATTENDANCE_REWARD& stReward = g_vecCheckAttendanceReward[dwRewardID - 1];
    
    // Check if attendance count matches reward requirement
    if (m_stAttendanceInfo.byAttendanceCount < stReward.byNeedCount) {
        return false;
    }
    
    // Create item info for reward
    ST_CREATE_ITEM stItemInfo;
    std::memset(&stItemInfo, 0, sizeof(stItemInfo));
    
    stItemInfo.dwItemID = stReward.dwRewardItemID;
    stItemInfo.shCount = stReward.shRewardCount;
    stItemInfo.byGrade = 0;
    stItemInfo.byEnhance = 0;
    
    // Send reward request to database
    XSendDBPacket sendPacket;
    sendPacket.SetID(0x5A20); // DB_ATTENDANCE_REWARD_REQ
    sendPacket << dwRewardID;
    sendPacket << stItemInfo.dwItemID;
    sendPacket << stItemInfo.shCount;
    sendPacket.Send();
    
    return true;
}

// AttendanceRewardRes (0x140032CC0)
bool CGocAttendance::AttendanceRewardRes(std::uint32_t dwError,
                                        PS_ATTENDANCE_INFO& stAttendance,
                                        ST_CREATE_ITEM& stItemInfo) {
    // Check for errors
    if (dwError != 0) {
        // Send error to client
        XSendPacket sendPacket;
        sendPacket.SetID(0x7A21); // SC_ATTENDANCE_REWARD_ERROR
        sendPacket << dwError;
        sendPacket.Send();
        
        return false;
    }
    
    // Update attendance info
    std::memcpy(&m_stAttendanceInfo, &stAttendance, sizeof(PS_ATTENDANCE_INFO));
    
    // Send reward to client
    SendAttendanceReward(stItemInfo);
    
    // Send updated attendance info
    SendAttendanceInfo();
    
    return true;
}

// AttendanceContinueReward (0x1400333D0)
bool CGocAttendance::AttendanceContinueReward(std::uint32_t dwRewardItemID, std::int16_t shRewardCount) {
    // Check if streak tracking is enabled
    if (m_stAttendanceContinue.byApplyAttendance == 0) {
        return false;
    }
    
    // Check if streak count is sufficient (need at least 1)
    if (m_stAttendanceContinue.byAttendanceCount == 0) {
        return false;
    }
    
    // Create item info for reward
    ST_CREATE_ITEM stItemInfo;
    std::memset(&stItemInfo, 0, sizeof(stItemInfo));
    
    stItemInfo.dwItemID = dwRewardItemID;
    stItemInfo.shCount = shRewardCount;
    stItemInfo.byGrade = 0;
    stItemInfo.byEnhance = 0;
    
    // Send streak reward request to database
    XSendDBPacket sendPacket;
    sendPacket.SetID(0x5A22); // DB_ATTENDANCE_CONTINUE_REWARD_REQ
    sendPacket << m_stAttendanceContinue.byAttendanceCount;
    sendPacket << stItemInfo.dwItemID;
    sendPacket << stItemInfo.shCount;
    sendPacket.Send();
    
    return true;
}

// AttendanceContinueRewardRes (0x1400336F0)
bool CGocAttendance::AttendanceContinueRewardRes(std::uint32_t dwError,
                                                PS_ATTENDANCE_CONTINUE& stContinue,
                                                ST_CREATE_ITEM& stItemInfo) {
    // Check for errors
    if (dwError != 0) {
        // Send error to client
        XSendPacket sendPacket;
        sendPacket.SetID(0x7A23); // SC_ATTENDANCE_CONTINUE_REWARD_ERROR
        sendPacket << dwError;
        sendPacket.Send();
        
        return false;
    }
    
    // Update streak info
    std::memcpy(&m_stAttendanceContinue, &stContinue, sizeof(PS_ATTENDANCE_CONTINUE));
    
    // Send reward to client
    SendAttendanceContinueReward(stItemInfo);
    
    // Send updated attendance info
    SendAttendanceInfo();
    
    return true;
}

// AttendancePlayTimeReward (0x140033DF0)
bool CGocAttendance::AttendancePlayTimeReward() {
    // Check if play time tracking is enabled
    if (m_stAttendancePlayTime.byApplyAttendance == 0) {
        return false;
    }
    
    // Check if already claimed all rewards
    if (m_stAttendancePlayTime.byCurPos >= 3) {
        return false;
    }
    
    // Get play time reward threshold (in seconds)
    // Thresholds: 1800 (30 min), 3600 (1 hour), 7200 (2 hours)
    const std::int32_t nThresholds[] = {1800, 3600, 7200};
    std::int32_t nThreshold = nThresholds[m_stAttendancePlayTime.byCurPos];
    
    // Check if play time meets threshold
    if (m_stAttendancePlayTime.nPlaySec < nThreshold) {
        return false;
    }
    
    // Send play time reward request to database
    XSendDBPacket sendPacket;
    sendPacket.SetID(0x5A24); // DB_ATTENDANCE_PLAYTIME_REWARD_REQ
    sendPacket << m_stAttendancePlayTime.byCurPos;
    sendPacket << m_stAttendancePlayTime.nPlaySec;
    sendPacket.Send();
    
    return true;
}

// AttendancePlayTimeRewardRes (0x140034170)
bool CGocAttendance::AttendancePlayTimeRewardRes(PS_ATTENDANCE_PLAY_TIME& stPlayTime,
                                                ST_CREATE_ITEM& stItemInfo) {
    // Update play time info
    std::memcpy(&m_stAttendancePlayTime, &stPlayTime, sizeof(PS_ATTENDANCE_PLAY_TIME));
    
    // Send reward to client
    SendAttendancePlayTimeReward(stItemInfo);
    
    // Send updated attendance info
    SendAttendanceInfo();
    
    return true;
}

// SendAttendanceInfo (0x140034980)
void CGocAttendance::SendAttendanceInfo() {
    XSendPacket sendPacket;
    sendPacket.SetID(0x7A20); // SC_ATTENDANCE_INFO
    
    // Send attendance info
    sendPacket << m_stAttendanceInfo.byApplyAttendance;
    sendPacket << m_stAttendanceInfo.dwType;
    sendPacket << m_stAttendanceInfo.dwAttendanceID;
    sendPacket << m_stAttendanceInfo.byAttendanceCount;
    
    for (int i = 0; i < 14; ++i) {
        sendPacket << m_stAttendanceInfo.nAttendance[i];
    }
    
    // Send streak info
    sendPacket << m_stAttendanceContinue.byApplyAttendance;
    sendPacket << m_stAttendanceContinue.dwType;
    sendPacket << m_stAttendanceContinue.byAttendanceCount;
    sendPacket << m_stAttendanceContinue.nLastAttendanceDate;
    
    // Send play time info
    sendPacket << m_stAttendancePlayTime.byApplyAttendance;
    sendPacket << m_stAttendancePlayTime.dwType;
    sendPacket << m_stAttendancePlayTime.byCurPos;
    sendPacket << m_stAttendancePlayTime.nPlaySec;
    sendPacket << m_stAttendancePlayTime.nUpdateDate;
    
    sendPacket.Send();
}

// SendAttendanceReward (0x140034A90)
void CGocAttendance::SendAttendanceReward(ST_CREATE_ITEM& stItemInfo) {
    XSendPacket sendPacket;
    sendPacket.SetID(0x7A22); // SC_ATTENDANCE_REWARD
    
    sendPacket << stItemInfo.dwItemID;
    sendPacket << stItemInfo.shCount;
    sendPacket << stItemInfo.byGrade;
    sendPacket << stItemInfo.byEnhance;
    
    sendPacket.Send();
}

// SendAttendanceContinueReward (0x140034B80)
void CGocAttendance::SendAttendanceContinueReward(ST_CREATE_ITEM& stItemInfo) {
    XSendPacket sendPacket;
    sendPacket.SetID(0x7A24); // SC_ATTENDANCE_CONTINUE_REWARD
    
    sendPacket << stItemInfo.dwItemID;
    sendPacket << stItemInfo.shCount;
    sendPacket << stItemInfo.byGrade;
    sendPacket << stItemInfo.byEnhance;
    
    sendPacket.Send();
}

// SendAttendancePlayTimeReward (0x140034C70)
void CGocAttendance::SendAttendancePlayTimeReward(ST_CREATE_ITEM& stItemInfo) {
    XSendPacket sendPacket;
    sendPacket.SetID(0x7A26); // SC_ATTENDANCE_PLAYTIME_REWARD
    
    sendPacket << stItemInfo.dwItemID;
    sendPacket << stItemInfo.shCount;
    sendPacket << stItemInfo.byGrade;
    sendPacket << stItemInfo.byEnhance;
    
    sendPacket.Send();
}

// SendDBAttendance (0x140034D60)
void CGocAttendance::SendDBAttendance() {
    XSendDBPacket sendPacket;
    sendPacket.SetID(0x5A21); // DB_ATTENDANCE_UPDATE
    
    sendPacket << m_stAttendanceInfo.byApplyAttendance;
    sendPacket << m_stAttendanceInfo.dwType;
    sendPacket << m_stAttendanceInfo.dwAttendanceID;
    sendPacket << m_stAttendanceInfo.byAttendanceCount;
    
    for (int i = 0; i < 14; ++i) {
        sendPacket << m_stAttendanceInfo.nAttendance[i];
    }
    
    sendPacket << m_stAttendanceContinue.byApplyAttendance;
    sendPacket << m_stAttendanceContinue.dwType;
    sendPacket << m_stAttendanceContinue.byAttendanceCount;
    sendPacket << m_stAttendanceContinue.nLastAttendanceDate;
    
    sendPacket.Send();
}

// SendDBAttendanceLogOut (0x140035010)
void CGocAttendance::SendDBAttendanceLogOut() {
    // Send attendance data on logout
    SendDBAttendance();
    
    // Send play time data
    XSendDBPacket sendPacket;
    sendPacket.SetID(0x5A25); // DB_ATTENDANCE_PLAYTIME_UPDATE
    
    sendPacket << m_stAttendancePlayTime.byApplyAttendance;
    sendPacket << m_stAttendancePlayTime.dwType;
    sendPacket << m_stAttendancePlayTime.byCurPos;
    sendPacket << m_stAttendancePlayTime.nPlaySec;
    sendPacket << m_stAttendancePlayTime.nUpdateDate;
    
    sendPacket.Send();
}

// SendDBAttendanceReset (0x140035260)
void CGocAttendance::SendDBAttendanceReset(std::uint32_t dwAttendanceID) {
    XSendDBPacket sendPacket;
    sendPacket.SetID(0x5A23); // DB_ATTENDANCE_RESET
    
    sendPacket << dwAttendanceID;
    
    sendPacket.Send();
}

// InitPlayTimebyDay (0x140030540)
void CGocAttendance::InitPlayTimebyDay() {
    m_dw64PlayTimeByDay = 0;
}

// SendDBPlayTimeByDay (0x140030590)
void CGocAttendance::SendDBPlayTimeByDay() {
    if (m_dw64PlayTimeByDay == 0) {
        return;
    }
    
    XSendDBPacket sendPacket;
    sendPacket.SetID(0x5A26); // DB_PLAYTIME_BY_DAY
    
    sendPacket << m_dw64PlayTimeByDay;
    
    sendPacket.Send();
    
    // Reset after sending
    m_dw64PlayTimeByDay = 0;
}

// LoadAccountPlayTimeEventReq (0x140030760)
void CGocAttendance::LoadAccountPlayTimeEventReq() {
    XSendDBPacket sendPacket;
    sendPacket.SetID(0x5A27); // DB_ACCOUNT_PLAYTIME_EVENT_LOAD_REQ
    
    sendPacket.Send();
}

// LoadAccountPlayTimeEvent (0x1400308B0)
void CGocAttendance::LoadAccountPlayTimeEvent(struct PS_PLAY_TIME_BY_ACCOUNT& stData) {
    m_nAccountPlayTimeTick = stData.nPlayTimeTick;
    m_byAccountPlayType = stData.byPlayType;
    
    // Show event info if active
    if (m_byAccountPlayType != 0) {
        ShowAccountPlayTimeEvent();
    }
}

// UpdateAccountPlayTimeEvent (0x140030B10)
void CGocAttendance::UpdateAccountPlayTimeEvent() {
    // Get current tick
    std::int32_t nCurTick = static_cast<std::int32_t>(TimeHelper::GetTick());
    
    // Calculate elapsed time
    std::int32_t nElapsed = nCurTick - m_nPrevAccountPlayTimeTick;
    
    // Update play time (convert milliseconds to seconds)
    if (nElapsed > 0) {
        m_nAccountPlayTimeTick += nElapsed / 1000;
    }
    
    // Update previous tick
    m_nPrevAccountPlayTimeTick = nCurTick;
    
    // Save to database periodically (every 300 seconds)
    if (m_nAccountPlayTimeTick - m_nAccountPlayTimeDBSaveTick >= 300) {
        SaveAccountPlayTimeEvent();
        m_nAccountPlayTimeDBSaveTick = m_nAccountPlayTimeTick;
    }
}

// SaveAccountPlayTimeEvent (0x140031050)
void CGocAttendance::SaveAccountPlayTimeEvent() {
    XSendDBPacket sendPacket;
    sendPacket.SetID(0x5A28); // DB_ACCOUNT_PLAYTIME_EVENT_SAVE
    
    sendPacket << m_nAccountPlayTimeTick;
    sendPacket << m_byAccountPlayType;
    
    sendPacket.Send();
}

// ShowAccountPlayTimeEvent (0x140031200)
void CGocAttendance::ShowAccountPlayTimeEvent() {
    XSendPacket sendPacket;
    sendPacket.SetID(0x7A28); // SC_ACCOUNT_PLAYTIME_EVENT_INFO
    
    sendPacket << m_nAccountPlayTimeTick;
    sendPacket << m_byAccountPlayType;
    
    sendPacket.Send();
}

// Cheat_AttendanceReset (0x140035430)
void CGocAttendance::Cheat_AttendanceReset() {
    // Reset attendance info
    std::int64_t biCurDate = TimeHelper::GetTime();
    std::uint32_t dwAttendanceID = GetAttendanceID(biCurDate);
    
    m_stAttendanceInfo.Reset(dwAttendanceID);
    
    // Send to database
    SendDBAttendanceReset(dwAttendanceID);
    
    // Send to client
    SendAttendanceInfo();
}

// Cheat_AttendanceContinueReset (0x140035490)
void CGocAttendance::Cheat_AttendanceContinueReset() {
    // Reset streak info
    m_stAttendanceContinue.byAttendanceCount = 0;
    m_stAttendanceContinue.nLastAttendanceDate = 0;
    
    // Send to database
    SendDBAttendance();
    
    // Send to client
    SendAttendanceInfo();
}

// Cheat_AttendancePlayTimeReset (0x140035650)
void CGocAttendance::Cheat_AttendancePlayTimeReset() {
    // Reset play time info
    m_stAttendancePlayTime.byCurPos = 0;
    m_stAttendancePlayTime.nPlaySec = 0;
    m_stAttendancePlayTime.nUpdateDate = TimeHelper::GetTime();
    
    // Send to database
    XSendDBPacket sendPacket;
    sendPacket.SetID(0x5A25); // DB_ATTENDANCE_PLAYTIME_UPDATE
    
    sendPacket << m_stAttendancePlayTime.byApplyAttendance;
    sendPacket << m_stAttendancePlayTime.dwType;
    sendPacket << m_stAttendancePlayTime.byCurPos;
    sendPacket << m_stAttendancePlayTime.nPlaySec;
    sendPacket << m_stAttendancePlayTime.nUpdateDate;
    
    sendPacket.Send();
    
    // Send to client
    SendAttendanceInfo();
}

// Cheat_ShowAttendanceInfo (0x140035870)
void CGocAttendance::Cheat_ShowAttendanceInfo() {
    // Send attendance info to client
    SendAttendanceInfo();
}

// Cheat_AttendancePlayTimeUpdate (0x140036740)
void CGocAttendance::Cheat_AttendancePlayTimeUpdate(int nHour, int nMin) {
    // Update play time manually (GM command)
    std::int32_t nSeconds = nHour * 3600 + nMin * 60;
    
    m_stAttendancePlayTime.nPlaySec = nSeconds;
    
    // Send to client
    SendAttendanceInfo();
}
