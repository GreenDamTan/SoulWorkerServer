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

} // namespace GreenDamTan

#endif // GREENDAMTAN_TIMECOMPAT_H
