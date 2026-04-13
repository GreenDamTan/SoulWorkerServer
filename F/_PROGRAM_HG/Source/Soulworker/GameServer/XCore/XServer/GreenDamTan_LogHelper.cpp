#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <string>
#include <vector>

namespace {
std::mutex g_logMutex;
std::filesystem::path g_logDirectory;
bool g_fileLoggingEnabled = false;

std::string NormalizeServerFolderName(const char* serverName) {
    std::string folderName = serverName ? serverName : "";
    if (folderName.empty()) {
        return "Login";
    }

    std::transform(folderName.begin(), folderName.end(), folderName.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    folderName.front() = static_cast<char>(std::toupper(static_cast<unsigned char>(folderName.front())));
    return folderName;
}

std::string FormatVaList(const char* format, va_list args) {
    va_list argsCopy;
    va_copy(argsCopy, args);
#ifdef _WIN32
    const int required = _vscprintf(format, argsCopy);
#else
    const int required = std::vsnprintf(nullptr, 0, format, argsCopy);
#endif
    va_end(argsCopy);
    if (required <= 0) {
        return {};
    }

    std::vector<char> buffer(static_cast<std::size_t>(required) + 1, '\0');
    va_copy(argsCopy, args);
    std::vsnprintf(buffer.data(), buffer.size(), format, argsCopy);
    va_end(argsCopy);
    return std::string(buffer.data(), static_cast<std::size_t>(required));
}

std::tm GetLocalTimeSnapshot() {
    std::time_t now = std::time(nullptr);
    std::tm localTime{};
#ifdef _WIN32
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif
    return localTime;
}

std::string FormatStructuredTimestamp(const std::tm& localTime) {
    char buffer[32] = {};
    std::snprintf(buffer,
                  sizeof(buffer),
                  "[%02d-%02d-%02d %02d:%02d:%02d] ",
                  (localTime.tm_year + 1900) % 100,
                  localTime.tm_mon + 1,
                  localTime.tm_mday,
                  localTime.tm_hour,
                  localTime.tm_min,
                  localTime.tm_sec);
    return buffer;
}

std::string FormatLegacyTimestamp(const std::tm& localTime) {
    char buffer[24] = {};
    std::snprintf(buffer,
                  sizeof(buffer),
                  "<%02d/%02d %02d:%02d:%02d>:",
                  localTime.tm_mon + 1,
                  localTime.tm_mday,
                  localTime.tm_hour,
                  localTime.tm_min,
                  localTime.tm_sec);
    return buffer;
}

const char* SelectLogFileName(const char* channel) {
    const std::string channelName = channel ? channel : "";
    if (channelName == "game.contents") {
        return "Game.log";
    }
    return "System.log";
}

void AppendLineToFile(const std::filesystem::path& path, const std::string& line) {
    std::error_code error;
    std::filesystem::create_directories(path.parent_path(), error);
    std::ofstream stream(path, std::ios::binary | std::ios::app);
    if (!stream) {
        return;
    }
    stream.write(line.data(), static_cast<std::streamsize>(line.size()));
    stream.write("\r\n", 2);
}
}

void LogHelper::ConfigureFileLogging(const char* serverName) {
    std::lock_guard<std::mutex> lock(g_logMutex);
    const std::string folderName = NormalizeServerFolderName(serverName);
    std::error_code error;
    g_logDirectory = std::filesystem::current_path(error);
    if (error) {
        g_logDirectory.clear();
        g_fileLoggingEnabled = false;
        return;
    }

    g_logDirectory /= "Log";
    g_logDirectory /= folderName;
    std::filesystem::create_directories(g_logDirectory, error);
    g_fileLoggingEnabled = !error;
}

void LogHelper::ShutdownFileLogging() {
    std::lock_guard<std::mutex> lock(g_logMutex);
    g_fileLoggingEnabled = false;
    g_logDirectory.clear();
}

void LogHelper::LogDebug(const char* channel, const char* format, ...) {
    va_list args;
    va_start(args, format);
    Print("DEBUG", channel, format, args);
    va_end(args);
}

void LogHelper::LogInfo(const char* channel, const char* format, ...) {
    va_list args;
    va_start(args, format);
    Print("INFO", channel, format, args);
    va_end(args);
}

void LogHelper::LogError(const char* channel, const char* format, ...) {
    va_list args;
    va_start(args, format);
    Print("ERROR", channel, format, args);
    va_end(args);
}

void LogHelper::LogLegacy(const char* format, ...) {
    va_list args;
    va_start(args, format);
    PrintLegacy(format, args);
    va_end(args);
}

void LogHelper::Print(const char* level, const char* channel, const char* format, va_list args) {
    std::lock_guard<std::mutex> lock(g_logMutex);
    const std::string message = FormatVaList(format, args);
    std::printf("[%s][%s] %s\n", level, channel ? channel : "", message.c_str());
    std::fflush(stdout);

    if (!g_fileLoggingEnabled || g_logDirectory.empty()) {
        return;
    }

    const std::tm localTime = GetLocalTimeSnapshot();
    std::string fileLine = FormatStructuredTimestamp(localTime);
    const std::string channelName = channel ? channel : "";
    if (channelName == "game.contents") {
        fileLine += message;
    } else {
        fileLine += "[" + channelName + "][" + (level ? level : "") + "] " + message;
    }
    AppendLineToFile(g_logDirectory / SelectLogFileName(channel), fileLine);
}

void LogHelper::PrintLegacy(const char* format, va_list args) {
    std::lock_guard<std::mutex> lock(g_logMutex);
    const std::tm localTime = GetLocalTimeSnapshot();
    const std::string message = FormatVaList(format, args);
    const std::string prefix = FormatLegacyTimestamp(localTime);
    std::printf("%s%s\n", prefix.c_str(), message.c_str());
    std::fflush(stdout);

    if (!g_fileLoggingEnabled || g_logDirectory.empty()) {
        return;
    }

    AppendLineToFile(g_logDirectory / "System.log", prefix + message);
}
