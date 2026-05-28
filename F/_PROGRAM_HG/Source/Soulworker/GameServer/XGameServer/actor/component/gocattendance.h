#pragma once

#include "gocbase.h"
#include <cstdint>

/**
 * @brief CGocAttendance - Game Object Component for attendance check-in system
 *
 * Handles daily attendance rewards and cumulative attendance tracking.
 */
class CGocAttendance : public GOComponent {
public:
    CGocAttendance();
    virtual ~CGocAttendance();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Attendance info
    int GetAttendanceCount() const;
    int GetConsecutiveDays() const;
    bool HasCheckedInToday() const;
    int GetLastCheckInDate() const;

    // Attendance operations
    bool CheckIn();
    bool ClaimReward(int nRewardIndex);
    bool CanClaimReward(int nRewardIndex) const;

    // Rewards
    int GetTotalRewardCount() const;
    int GetClaimedRewardCount() const;

protected:
    int m_nAttendanceCount;
    int m_nConsecutiveDays;
    int m_nLastCheckInDate;
    int m_nClaimedRewardCount;
};
