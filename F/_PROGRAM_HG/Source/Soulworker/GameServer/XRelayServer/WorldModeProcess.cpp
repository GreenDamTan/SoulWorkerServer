#include "Soulworker/GameServer/XRelayServer/WorldModeProcess.h"

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

bool CServerWorldModeProcess::Parse(XPacket& xPacket) {
    // WorldMode process handles sub-commands related to world-mode maze features
    // Currently implemented as stub - packet accepted but no action taken
    LogHelper::LogDebug("game.worldmode", "CServerWorldModeProcess::Parse sub=%u", xPacket.GetSubCmd());
    return true;
}
