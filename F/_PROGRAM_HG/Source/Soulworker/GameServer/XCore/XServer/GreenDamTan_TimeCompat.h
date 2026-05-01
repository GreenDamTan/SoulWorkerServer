// GreenDamTan_TimeCompat.h
// Cross-platform time compatibility helpers for ControlServer reconstruction
// This is a reconstruction-only helper file

#ifndef GREENDAMTAN_TIMECOMPAT_H
#define GREENDAMTAN_TIMECOMPAT_H

#include <cstdint>

#ifdef _WIN32
#include <windows.h>
#else
#include <ctime>
#include <sys/time.h>
#endif

namespace GreenDamTan {

// Forward declarations - standalone functions defined later
inline std::int64_t GetCurrentTime();
inline int GetDayOfWeek(std::int64_t nTime);

// GetTickCount64 - milliseconds since system start
inline std::uint64_t GetTickCount64Compat() {
#ifdef _WIN32
    return ::GetTickCount64();
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return static_cast<std::uint64_t>(ts.tv_sec) * 1000ULL +
           static_cast<std::uint64_t>(ts.tv_nsec) / 1000000ULL;
#endif
}

// Get current Unix timestamp in seconds
inline std::int64_t GetCurrentTime() {
#ifdef _WIN32
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    std::uint64_t ull = static_cast<std::uint64_t>(ft.dwLowDateTime) |
                        (static_cast<std::uint64_t>(ft.dwHighDateTime) << 32);
    // Convert from 100-nanosecond intervals since Jan 1, 1601 to seconds since Jan 1, 1970
    return static_cast<std::int64_t>((ull - 116444736000000000ULL) / 10000000ULL);
#else
    return static_cast<std::int64_t>(std::time(nullptr));
#endif
}

// Get current time in milliseconds (Unix timestamp)
inline std::int64_t GetCurrentTimeMs() {
#ifdef _WIN32
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    std::uint64_t ull = static_cast<std::uint64_t>(ft.dwLowDateTime) |
                        (static_cast<std::uint64_t>(ft.dwHighDateTime) << 32);
    return static_cast<std::int64_t>((ull - 116444736000000000ULL) / 10000ULL);
#else
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return static_cast<std::int64_t>(tv.tv_sec) * 1000LL +
           static_cast<std::int64_t>(tv.tv_usec / 1000);
#endif
}

// Get day of week (1=Sunday, 2=Monday, ..., 7=Saturday, matching ATL::CTime behavior)
inline int GetDayOfWeek(std::int64_t nTime) {
#ifdef _WIN32
    SYSTEMTIME st;
    FILETIME ft;
    std::int64_t ull = static_cast<std::uint64_t>(nTime) * 10000000ULL + 116444736000000000ULL;
    ft.dwLowDateTime = static_cast<DWORD>(ull & 0xFFFFFFFF);
    ft.dwHighDateTime = static_cast<DWORD>(ull >> 32);
    FileTimeToSystemTime(&ft, &st);
    return static_cast<int>(st.wDayOfWeek) + 1;  // ATL uses 1-based, Sunday=1
#else
    struct tm tm_result;
    time_t t = static_cast<time_t>(nTime);
    localtime_r(&t, &tm_result);
    return tm_result.tm_wday + 1;  // Convert 0-based to 1-based
#endif
}

// Get day of week for current time
inline int GetCurrentDayOfWeek() {
    return GetDayOfWeek(GetCurrentTime());
}

// Get hour and minute from time value
inline void GetTimeHM(std::int64_t nTime, int* pHour, int* pMin) {
    if (!pHour || !pMin) return;

#ifdef _WIN32
    SYSTEMTIME st;
    FILETIME ft;
    std::int64_t ull = static_cast<std::uint64_t>(nTime) * 10000000ULL + 116444736000000000ULL;
    ft.dwLowDateTime = static_cast<DWORD>(ull & 0xFFFFFFFF);
    ft.dwHighDateTime = static_cast<DWORD>(ull >> 32);
    FileTimeToSystemTime(&ft, &st);
    *pHour = static_cast<int>(st.wHour);
    *pMin = static_cast<int>(st.wMinute);
#else
    struct tm tm_result;
    time_t t = static_cast<time_t>(nTime);
    localtime_r(&t, &tm_result);
    *pHour = tm_result.tm_hour;
    *pMin = tm_result.tm_min;
#endif
}

// Sleep for specified milliseconds
inline void SleepMs(std::uint32_t dwMs) {
#ifdef _WIN32
    ::Sleep(dwMs);
#else
    struct timespec ts;
    ts.tv_sec = static_cast<time_t>(dwMs / 1000);
    ts.tv_nsec = static_cast<long>((dwMs % 1000) * 1000000);
    nanosleep(&ts, nullptr);
#endif
}

// CTimeCompat - simple wrapper for time value (similar to ATL::CTime)
class CTimeCompat {
public:
    CTimeCompat() : m_nTime(0) {}
    explicit CTimeCompat(std::int64_t nTime) : m_nTime(nTime) {}
    CTimeCompat(const CTimeCompat& other) : m_nTime(other.m_nTime) {}

    CTimeCompat& operator=(const CTimeCompat& other) {
        m_nTime = other.m_nTime;
        return *this;
    }

    CTimeCompat& operator=(std::int64_t nTime) {
        m_nTime = nTime;
        return *this;
    }

    std::int64_t GetTime() const { return m_nTime; }
    operator std::int64_t() const { return m_nTime; }

    int GetYear() const {
        return GetTm()->tm_year + 1900;
    }

    int GetMonth() const {
        return GetTm()->tm_mon + 1;
    }

    int GetDay() const {
        return GetTm()->tm_mday;
    }

    int GetHour() const {
        return GetTm()->tm_hour;
    }

    int GetMinute() const {
        return GetTm()->tm_min;
    }

    int GetSecond() const {
        return GetTm()->tm_sec;
    }

    int GetDayOfWeek() const {
        return GreenDamTan::GetDayOfWeek(m_nTime);
    }

private:
    std::int64_t m_nTime;

#ifdef _WIN32
    mutable SYSTEMTIME m_stCache;
    mutable bool m_bCached = false;

    const SYSTEMTIME* GetSystemTime() const {
        if (!m_bCached) {
            FILETIME ft;
            std::uint64_t ull = static_cast<std::uint64_t>(m_nTime) * 10000000ULL + 116444736000000000ULL;
            ft.dwLowDateTime = static_cast<DWORD>(ull & 0xFFFFFFFF);
            ft.dwHighDateTime = static_cast<DWORD>(ull >> 32);
            FileTimeToSystemTime(&ft, &m_stCache);
            m_bCached = true;
        }
        return &m_stCache;
    }

    const tm* GetTm() const {
        static thread_local tm result;
        auto* st = GetSystemTime();
        result.tm_year = st->wYear - 1900;
        result.tm_mon = st->wMonth - 1;
        result.tm_mday = st->wDay;
        result.tm_hour = st->wHour;
        result.tm_min = st->wMinute;
        result.tm_sec = st->wSecond;
        result.tm_wday = st->wDayOfWeek;
        return &result;
    }
#else
    mutable struct tm m_tmCache;
    mutable bool m_bCached = false;

    const struct tm* GetTm() const {
        if (!m_bCached) {
            time_t t = static_cast<time_t>(m_nTime);
            localtime_r(&t, &m_tmCache);
            m_bCached = true;
        }
        return &m_tmCache;
    }
#endif
};

// GetTimeAsInt64 - extract time value from CTimeCompat
inline std::int64_t GetTimeAsInt64(const CTimeCompat& time) {
    return time.GetTime();
}

// GetCurrentTimeCompat - 返回当前时间的 CTimeCompat (类似 ATL::CTime::GetCurrentTime)
inline CTimeCompat GetCurrentTimeCompat() {
    return CTimeCompat(GetCurrentTime());
}

} // namespace GreenDamTan

#endif // GREENDAMTAN_TIMECOMPAT_H