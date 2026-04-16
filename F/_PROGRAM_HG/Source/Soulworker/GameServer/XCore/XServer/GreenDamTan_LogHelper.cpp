#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace {
std::mutex g_logMutex;
std::filesystem::path g_logDirectory;
bool g_fileLoggingEnabled = false;
std::unordered_map<std::string, LogHelper::GreenDamTan_LogRoute> g_channelRoutes;

constexpr int kLogLevelDebug = 0;
constexpr int kLogLevelInfo = 1;
constexpr int kLogLevelError = 2;

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
    localtime_r(&localTime, &now);
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

std::string TrimAscii(std::string value) {
    auto notSpace = [](unsigned char ch) { return !std::isspace(ch); };
    value.erase(value.begin(), std::find_if(value.begin(), value.end(), notSpace));
    value.erase(std::find_if(value.rbegin(), value.rend(), notSpace).base(), value.end());
    return value;
}

std::string StripInlineComment(std::string value) {
    const std::size_t commentPos = value.find('#');
    if (commentPos != std::string::npos) {
        value.erase(commentPos);
    }
    return TrimAscii(std::move(value));
}

std::string ExtractFileName(const std::string& pathValue) {
    const std::string trimmed = TrimAscii(pathValue);
    if (trimmed.empty()) {
        return {};
    }
    return std::filesystem::path(trimmed).filename().string();
}

int ParseLogLevel(const std::string& levelValue) {
    std::string upper = levelValue;
    std::transform(upper.begin(), upper.end(), upper.begin(), [](unsigned char ch) {
        return static_cast<char>(std::toupper(ch));
    });
    if (upper == "ERROR") {
        return kLogLevelError;
    }
    if (upper == "INFO") {
        return kLogLevelInfo;
    }
    return kLogLevelDebug;
}

int LevelFromTag(const char* level) {
    const std::string upper = level ? level : "";
    if (upper == "ERROR") {
        return kLogLevelError;
    }
    if (upper == "INFO") {
        return kLogLevelInfo;
    }
    return kLogLevelDebug;
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

std::unordered_map<std::string, LogHelper::GreenDamTan_LogRoute> ParseConfiguredChannelRoutes(
    const std::filesystem::path& configPath) {
    std::unordered_map<std::string, std::string> appenderToFile;
    std::unordered_map<std::string, LogHelper::GreenDamTan_LogRoute> channelRoutes;

    std::ifstream stream(configPath, std::ios::binary);
    if (!stream) {
        return channelRoutes;
    }

    std::string line;
    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        const std::string trimmed = TrimAscii(line);
        if (trimmed.empty() || trimmed.front() == '#') {
            continue;
        }

        const std::size_t equalsPos = trimmed.find('=');
        if (equalsPos == std::string::npos) {
            continue;
        }

        const std::string key = TrimAscii(trimmed.substr(0, equalsPos));
        const std::string value = StripInlineComment(trimmed.substr(equalsPos + 1));
        if (key.empty() || value.empty()) {
            continue;
        }

        if (key.rfind("log4j.appender.", 0) == 0 && key.size() > 5 &&
            key.compare(key.size() - 5, 5, ".File") == 0) {
            const std::string appenderName = key.substr(15, key.size() - 20);
            const std::string fileName = ExtractFileName(value);
            if (!appenderName.empty() && !fileName.empty()) {
                appenderToFile[appenderName] = fileName;
            }
            continue;
        }

        if (key.rfind("log4j.logger.", 0) != 0) {
            continue;
        }

        const std::string channelName = key.substr(13);
        if (channelName.empty()) {
            continue;
        }

        LogHelper::GreenDamTan_LogRoute route{};
        std::size_t start = 0;
        bool firstToken = true;
        while (start <= value.size()) {
            const std::size_t commaPos = value.find(',', start);
            const std::string token = TrimAscii(
                value.substr(start, commaPos == std::string::npos ? std::string::npos : commaPos - start));
            if (!token.empty()) {
                if (firstToken) {
                    route.minLevel = ParseLogLevel(token);
                    firstToken = false;
                } else {
                    const auto appenderIt = appenderToFile.find(token);
                    if (appenderIt != appenderToFile.end()) {
                        route.fileName = appenderIt->second;
                        break;
                    }
                }
            }
            if (commaPos == std::string::npos) {
                break;
            }
            start = commaPos + 1;
        }

        if (!route.fileName.empty()) {
            channelRoutes[channelName] = route;
        }
    }

    return channelRoutes;
}

LogHelper::GreenDamTan_LogRoute FallbackRoute(const char* channel) {
    const std::string channelName = channel ? channel : "";
    if (channelName == "game.contents") {
        return {"Game.log", kLogLevelDebug};
    }
    return {"System.log", kLogLevelDebug};
}
}

void LogHelper::ConfigureFileLogging(const char* serverName) {
    std::lock_guard<std::mutex> lock(g_logMutex);
    const std::string folderName = NormalizeServerFolderName(serverName);
    std::error_code error;
    const std::filesystem::path currentPath = std::filesystem::current_path(error);
    if (error) {
        g_logDirectory.clear();
        g_fileLoggingEnabled = false;
        g_channelRoutes.clear();
        return;
    }

    std::filesystem::path configPath = currentPath / "config" / (folderName + "_LogSetting.properties");
    if (!std::filesystem::exists(configPath)) {
        configPath = currentPath / "Config" / (folderName + "_LogSetting.properties");
    }

    g_logDirectory = currentPath / "Log" / folderName;
    std::filesystem::create_directories(g_logDirectory, error);
    g_fileLoggingEnabled = !error;
    g_channelRoutes = ParseConfiguredChannelRoutes(configPath);
}

void LogHelper::ShutdownFileLogging() {
    std::lock_guard<std::mutex> lock(g_logMutex);
    g_fileLoggingEnabled = false;
    g_logDirectory.clear();
    g_channelRoutes.clear();
}

LogHelper::GreenDamTan_LogRoute LogHelper::GetConfiguredRoute(const char* channel) {
    std::lock_guard<std::mutex> lock(g_logMutex);
    const std::string channelName = channel ? channel : "";
    const auto it = g_channelRoutes.find(channelName);
    if (it != g_channelRoutes.end()) {
        return it->second;
    }
    return FallbackRoute(channel);
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

    LogHelper::GreenDamTan_LogRoute route = FallbackRoute(channel);
    const std::string channelName = channel ? channel : "";
    const auto it = g_channelRoutes.find(channelName);
    if (it != g_channelRoutes.end()) {
        route = it->second;
    }

    if (LevelFromTag(level) < route.minLevel) {
        return;
    }

    const std::tm localTime = GetLocalTimeSnapshot();
    std::string fileLine = FormatStructuredTimestamp(localTime);
    if (channelName == "game.contents") {
        fileLine += message;
    } else {
        fileLine += "[" + channelName + "][" + (level ? level : "") + "] " + message;
    }
    AppendLineToFile(g_logDirectory / route.fileName, fileLine);
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
