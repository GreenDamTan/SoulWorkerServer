#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XCommon/XSWCommand.h"
#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

// ============================================================================
// DEPRECATED: 此类型为重构占位，无原始符号对应
// ============================================================================
// IDA 调查结论 (2026-04-26):
// - CServer::RegisterProcess(0x1400D1B40) 注册主命令: 0xF2/0xF3/0xF4/0xF5/0xF6/0xF7/0xFA/0xFD
// - 主命令 0xFB (251) 从未被注册
// - 与 WorldMode 协议相关的正确处理器是 CServerModeMazeProcess (0xFD)
// - GameServer CGameControlSocket::WorldModeProcess 处理子命令 1-7，但那是不同的协议栈
//
// 建议：
// - 此类应删除或改名为 GreenDamTan_WorldModeProcessPlaceholder
// - 若未来发现真实 0xFB 处理器，应建立新类而非沿用此 stub
// ============================================================================
class CServerWorldModeProcess : public TXProcess<CServer> {
public:
    // 注意：此类不存在于 IDA，为重构辅助类
    // 主命令 0xFB 在 RelayServer 中从未被注册
    CServerWorldModeProcess() {
        SetCmd(0xFB);
        SetName("CServerWorldModeProcess");
    }

    bool Parse(XPacket& xPacket) override;
};
