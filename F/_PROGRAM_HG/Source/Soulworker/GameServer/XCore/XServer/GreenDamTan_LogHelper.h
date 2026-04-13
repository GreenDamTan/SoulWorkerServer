#pragma once

#include <cstdarg>

class LogHelper {
public:
    static void ConfigureFileLogging(const char* serverName);
    static void ShutdownFileLogging();
    static void LogDebug(const char* channel, const char* format, ...);
    static void LogInfo(const char* channel, const char* format, ...);
    static void LogError(const char* channel, const char* format, ...);
    static void LogLegacy(const char* format, ...);

private:
    static void Print(const char* level, const char* channel, const char* format, va_list args);
    static void PrintLegacy(const char* format, va_list args);
};

class CLogThreadProc {
public:
    static void OnFinalizeThread(void*) {}
};
