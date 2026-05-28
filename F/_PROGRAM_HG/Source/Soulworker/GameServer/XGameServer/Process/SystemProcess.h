#pragma once

#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"

// Forward declarations
class CUser;
class XPacket;
class XSendPacket;

/**
 * @brief GameServer 系统阶段客户端请求处理器。
 *
 * 处理游戏服务器中的系统相关请求，包括：
 * - 选项更新
 * - Xigncode 验证
 * - 心跳保活
 * - Ping
 * - GameGuard 认证
 * 等
 *
 * 根据 IDA 反编译:
 * - 构造函数: 0x1406037C0
 * - Parse: 0x1406038B0
 * - cmd = 1 (eCMD_SYSTEM)
 */
class CSystemProcess : public TXProcess<CUser> {
public:
    CSystemProcess();
    virtual ~CSystemProcess() override;

    // === 核心虚函数 ===
    /** @brief 按系统阶段子命令分派客户端请求 (IDA: 0x1406038B0) */
    bool Parse(XPacket& xPacket) override;

    // === 客户端请求处理函数 ===

    /** @brief 按键选项更新 (IDA: 0x140603A30, sub=1) */
    bool ReqKeyOptionUpdate(XPacket& xPacket);

    /** @brief 选项更新 (IDA: 0x140603ED0, sub=2) */
    bool ReqOptionUpdate(XPacket& xPacket);

    /** @brief Xigncode 验证 (IDA: 0x140604410, sub=3) */
    bool ReqSystemXigncode(XPacket& xPacket);

    /** @brief Xigncode 错误 (IDA: 0x1406044E0, sub=4) */
    bool ReqSystemXigncodeError(XPacket& xPacket);

    /** @brief 心跳保活 (IDA: 0x140604520, sub=5) */
    bool ReqSystemKeepAlive(XPacket& xPacket);

    /** @brief Ping (IDA: 0x1406047A0, sub=6) */
    bool ReqSystemPing(XPacket& xPacket);

    /** @brief 客户端日志 (IDA: 0x140604AE0, sub=8) */
    bool ReqSystemClientLog(XPacket& xPacket);

    /** @brief SG令牌更新 (IDA: 0x140604F40, sub=0x11) */
    bool ReqSystemSGTokenUpdate(XPacket& xPacket);

    /** @brief GameGuard认证 (IDA: 0x140605440, sub=0x12) */
    bool ReqSystemGameGuardAuth(XPacket& xPacket);

    /** @brief GameGuard错误 (IDA: 0x1406056D0, sub=0x13) */
    bool ReqSystemGameGuardError(XPacket& xPacket);

    /** @brief Tick日志 (IDA: 0x1406058C0, sub=0x21) */
    bool ReqSystemTickLog(XPacket& xPacket);
};
