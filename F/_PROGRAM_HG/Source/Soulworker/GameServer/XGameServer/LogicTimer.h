// LogicTimer.h
// LogicTimer - Logic timer class for maze time management
// IDA: ?IsFinishStep@LogicTimer@@QEAA_NXZ (0x140353500)
// IDA: ?SetFinishStep@LogicTimer@@QEAAX_N@Z (0x1403537d0)
// IDA: ?Update@LogicTimer@@QEAA?AW4TimerResult_e@@M@Z (0x1403536c0)

#pragma once

#include <cstdint>

// TimerResult_e - Timer result enumeration
enum class TimerResult_e : std::int64_t {
    TIMER_RESULT_DISABLED = -1,
    TIMER_RESULT_RUNNING = 0,
    TIMER_RESULT_FINISHED = 1
};

// LogicTimer - Timer class used in maze for step-based timing
class LogicTimer {
public:
    LogicTimer() : enable_(false), elapsedTimes_(0.0f), timer_(0.0f), m_bFinishStep(false) {}
    ~LogicTimer() = default;

    // IDA: ?IsFinishStep@LogicTimer@@QEAA_NXZ (0x140353500)
    // Returns true if the timer has finished its step
    bool IsFinishStep() const {
        return m_bFinishStep;
    }

    // IDA: ?SetFinishStep@LogicTimer@@QEAAX_N@Z (0x1403537d0)
    // Sets the finish step flag
    void SetFinishStep(bool bStep) {
        m_bFinishStep = bStep;
    }

    // IDA: ?Update@LogicTimer@@QEAA?AW4TimerResult_e@@M@Z (0x1403536c0)
    // Updates the timer with delta time, returns result
    TimerResult_e Update(float delta) {
        if (!enable_) {
            return TimerResult_e::TIMER_RESULT_DISABLED;
        }
        elapsedTimes_ += delta;
        if (elapsedTimes_ < timer_) {
            return TimerResult_e::TIMER_RESULT_RUNNING;
        }
        Disable(false);
        return TimerResult_e::TIMER_RESULT_FINISHED;
    }

    // Disable - Disables the timer
    void Disable(bool bFinish) {
        enable_ = false;
        if (bFinish) {
            m_bFinishStep = true;
        }
    }

    // Enable - Enables the timer
    void Enable(float fTargetTime) {
        enable_ = true;
        elapsedTimes_ = 0.0f;
        timer_ = fTargetTime;
        m_bFinishStep = false;
    }

    // Check if enabled
    bool IsEnabled() const { return enable_; }

    // Get elapsed time
    float GetElapsedTime() const { return elapsedTimes_; }

    // Get target time
    float GetTargetTime() const { return timer_; }

    // Reset timer
    void Reset() {
        enable_ = false;
        elapsedTimes_ = 0.0f;
        m_bFinishStep = false;
    }

private:
    bool enable_;           // Timer enabled flag
    float elapsedTimes_;    // Elapsed time
    float timer_;           // Target timer value
    bool m_bFinishStep;     // Finish step flag
};
