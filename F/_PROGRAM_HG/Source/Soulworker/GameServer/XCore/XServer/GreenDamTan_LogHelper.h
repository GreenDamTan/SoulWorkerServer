#pragma once

#include <cstdarg>
#include <string>
#include <cstdio>

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

// 简单调试日志宏 (仅用于逆向恢复期调试)
// 支持格式化参数的版本
inline void GreenDamTan_log(const char* file, const char* func, const char* fmt, ...) {
    // TODO: 仅做测试用
    va_list args;
    va_start(args, fmt);
    printf("[GreenDamTan] %s:%s - ", file, func);
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}
