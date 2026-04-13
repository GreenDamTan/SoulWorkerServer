#pragma once

#include "Soulworker/GameServer/XLoginServer/LoginServer.h"

/**
 * @brief 登录服系统主命令处理器。
 *
 * 该类对应 `CSystemProcess`，负责承接 `main=1` 的系统层上行：
 * - 选项同步
 * - XignCode 校验
 * - KeepAlive
 * - SG Token 更新
 * - GameGuard 认证 / 错误
 */
class CSystemProcess : public TXProcess<CUser> {
public:
    CSystemProcess();

    bool Parse(XPacket& xPacket) override;

    bool ReqOptionUpdate(XPacket& xPacket);
    bool ReqSystemXigncode(XPacket& xPacket);
    bool ReqSystemXigncodeError(XPacket& xPacket);
    bool ReqSystemKeepAlive(XPacket& xPacket);
    bool ReqSystemSGTokenUpdate(XPacket& xPacket);
    bool ReqSystemGameGuardAuth(XPacket& xPacket);
    bool ReqSystemGameGuardError(XPacket& xPacket);
};
