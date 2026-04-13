#include "Soulworker/GameServer/XLoginServer/SystemProcess.h"

#include <array>
#include <cwchar>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace {
void CopyWideTokenToAnsi(const wchar_t* wideToken, char (&target)[1025]) {
    target[0] = '\0';
    if (!wideToken) {
        return;
    }

#ifdef _WIN32
    const int converted = ::WideCharToMultiByte(
        CP_ACP, 0, wideToken, -1, target, static_cast<int>(sizeof(target)), nullptr, nullptr);
    if (converted <= 0) {
        target[0] = '\0';
    }
#else
    std::mbstate_t state{};
    const wchar_t* source = wideToken;
    std::size_t converted = std::wcsrtombs(target, &source, sizeof(target) - 1, &state);
    if (converted == static_cast<std::size_t>(-1)) {
        target[0] = '\0';
        return;
    }

    target[converted] = '\0';
#endif
}
}

CSystemProcess::CSystemProcess() {
    SetCmd(eCMD_SYSTEM);
    SetName("CSystemProcess");
}

bool CSystemProcess::Parse(XPacket& xPacket) {
    switch (xPacket.GetSubCmd()) {
    case eSUB_CMD_OPTION_UPDATE:
        return ReqOptionUpdate(xPacket);
    case eSUB_CMD_SYSTEM_XIGNCODE:
        return ReqSystemXigncode(xPacket);
    case eSUB_CMD_SYSTEM_XIGNCODE_ERROR:
        return ReqSystemXigncodeError(xPacket);
    case eSUB_CMD_SYSTEM_KEEP_ALIVE:
        return ReqSystemKeepAlive(xPacket);
    case eSUB_CMD_SYSTEM_SG_TOKEN_UPDATE:
        return ReqSystemSGTokenUpdate(xPacket);
    case eSUB_CMD_SYSTEM_GAME_GUARD_AUTH:
        return ReqSystemGameGuardAuth(xPacket);
    case eSUB_CMD_SYSTEM_GAME_GUARD_ERROR:
        return ReqSystemGameGuardError(xPacket);
    default:
        return false;
    }
}

bool CSystemProcess::ReqOptionUpdate(XPacket& xPacket) {
    if (!GetClientPtr()) {
        return false;
    }

    ST_OPTION_BIT optionBits{};
    xPacket >> optionBits;
    return true;
}

bool CSystemProcess::ReqSystemXigncode(XPacket& xPacket) {
    CUser* user = GetClientPtr();
    if (!user) {
        return false;
    }

    PS_XIGNCODE_UPDATE update{};
    xPacket >> update;
    TXSingleton<XLoginServer>::Instance()->GetXigncode().RecvXigncode(user->GetSessionID(), update);
    return true;
}

bool CSystemProcess::ReqSystemXigncodeError(XPacket& xPacket) {
    if (!GetClientPtr()) {
        return false;
    }

    PS_XIGNCODE_ERROR error{};
    xPacket >> error;
    return true;
}

bool CSystemProcess::ReqSystemKeepAlive(XPacket& xPacket) {
    if (!GetClientPtr()) {
        return false;
    }

    std::uint64_t tickCount = 0;
    unsigned int aliveKey = 0;
    std::array<char, 32> aliveKeyResult{};
    xPacket.XParse >> tickCount;
    xPacket.XParse >> aliveKey;
    xPacket.XParse.GetBytes(aliveKeyResult.data(), aliveKeyResult.size());
    return true;
}

bool CSystemProcess::ReqSystemSGTokenUpdate(XPacket& xPacket) {
    CUser* user = GetClientPtr();
    if (!user) {
        return false;
    }

    PS_SG_TOKEN_UPDATE update{};
    xPacket >> update;

    ST_SG_AUTH_INFO authInfo{};
    authInfo.byAuthType = 1;
    CopyWideTokenToAnsi(update.szAcessToken, authInfo.szToken);

    if (user->GetSGAuthType() == 2) {
        return false;
    }

    user->SetSGAuthInfo(authInfo);
    return true;
}

bool CSystemProcess::ReqSystemGameGuardAuth(XPacket& xPacket) {
    if (!GetClientPtr()) {
        return false;
    }

    PS_GAME_GUARD_AUTH auth{};
    xPacket >> auth;
    return true;
}

bool CSystemProcess::ReqSystemGameGuardError(XPacket& xPacket) {
    if (!GetClientPtr()) {
        return false;
    }

    PS_GAME_GUARD_ERROR error{};
    xPacket >> error;
    return true;
}
