#pragma once

#include <cstddef>
#include <cstdint>

bool InstallService(const char* binaryPathName, const char* serviceName, std::uint32_t startType);
bool UninstallService(const char* serviceName);
std::size_t ErrorCode2String(void** msgBuf,
                             unsigned long lastError,
                             std::uint16_t priLangId,
                             std::uint16_t subLangId);
void SET_SERVICE_STATE(unsigned long dwState, unsigned long dwAccept);
std::uint64_t SERVICE_HANDLER(unsigned int fdwControl,
                              unsigned int dwEventType,
                              void* lpEventData,
                              void* lpContext);
void ServiceMain(unsigned int argc, const char** argv);
bool ServiceInit(unsigned int argc, char** argv);
