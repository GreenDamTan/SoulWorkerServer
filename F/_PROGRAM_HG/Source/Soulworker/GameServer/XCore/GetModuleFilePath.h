// GetModuleFilePath.h
// GetModuleFilePath - Get module file path utility
// IDA decompilation from GameServer.exe
//
// Functions:
// - GetModuleFilePath (0x1400185d0)

#pragma once

#ifdef _WIN32
#include <windows.h>
#endif
#include <string>

// ============================================================================
// GetModuleFilePath - Get the directory path of the current module
// IDA: GetModuleFilePath @ 0x1400185D0
// ============================================================================
inline std::string GetModuleFilePath()
{
    // IDA code:
    // std::string *__fastcall GetModuleFilePath(std::string *result)
    // {
    //   char szPath[272]; // [rsp+30h] [rbp-168h] BYREF
    //   std::string strPath; // [rsp+140h] [rbp-58h] BYREF
    //
    //   memset(szPath, 0, 260);
    //   GetModuleFileNameA(nullptr, szPath, 0x104u);
    //   std::string::string(&strPath, szPath);
    //   last_of = std::string::find_last_of(&strPath, "\\", std::string::npos);
    //   std::string::erase(&strPath, last_of, _Count);
    //   std::string::string(result, &strPath);
    //   std::string::~string(&strPath);
    //   return result;
    // }

#ifdef _WIN32
    char szPath[MAX_PATH] = {0};
    GetModuleFileNameA(nullptr, szPath, MAX_PATH);

    std::string strPath(szPath);
    size_t lastSlash = strPath.find_last_of('\\');
    if (lastSlash != std::string::npos)
    {
        strPath = strPath.substr(0, lastSlash);
    }
    return strPath;
#else
    return "./";
#endif
}
