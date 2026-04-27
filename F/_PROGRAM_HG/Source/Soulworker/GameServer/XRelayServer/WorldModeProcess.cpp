#include "Soulworker/GameServer/XRelayServer/WorldModeProcess.h"

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

// ============================================================================
// DEPRECATED: 此处理器对应的主命令 0xFB 在 RelayServer 中从未被注册
// ============================================================================
// 参考 WorldModeProcess.h 中的详细说明
// 若此 stub 被调用，说明有客户端发送了不存在的 0xFB 主命令
// ============================================================================
bool CServerWorldModeProcess::Parse(XPacket& xPacket) {
    // WorldMode process handles sub-commands related to world-mode maze features
    // Currently implemented as stub - packet accepted but no action taken
    LogHelper::LogDebug("game.worldmode", "CServerWorldModeProcess::Parse [DEPRECATED] main=0xFB sub=%u (主命令 0xFB 在 RelayServer 中未注册)", xPacket.GetSubCmd());
    return true;
}
