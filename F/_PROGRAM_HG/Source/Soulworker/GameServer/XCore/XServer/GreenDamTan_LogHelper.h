#pragma once

#include <cstdarg>
#include <string>

class CLogThreadProc;

class LogHelper {
public:
    struct GreenDamTan_LogRoute {
        std::string fileName;
        int minLevel = 0;
    };

    static void ConfigureFileLogging(const char* serverName);
    static void ShutdownFileLogging();
    static GreenDamTan_LogRoute GetConfiguredRoute(const char* channel);
    static void LogDebug(const char* channel, const char* format, ...);
    static void LogInfo(const char* channel, const char* format, ...);
    static void LogError(const char* channel, const char* format, ...);
    static void LogLegacy(const char* format, ...);

private:
    static void Print(const char* level, const char* channel, const char* format, va_list args);
    static void PrintLegacy(const char* format, va_list args);
};
