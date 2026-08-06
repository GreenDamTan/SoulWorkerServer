#pragma once

#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"

// Forward declarations
class CUser;
class XPacket;
class XSendPacket;

/**
 * @brief GameServer 登录阶段客户端请求处理器。
 *
 * 处理游戏服务器中的登录相关请求，包括：
 * - 计费订单处理
 * - 账户事件
 * - 账户日期信息
 * 等
 *
 * 根据 IDA 反编译:
 * - 构造函数: 0x1405043E0
 * - DBParse: 0x1405044D0
 * - cmd = 2 (eCMD_LOGIN)
 */
class CLoginProcess : public TXProcess<CUser> {
public:
    CLoginProcess();
    virtual ~CLoginProcess() override;

    // IDA vtable slot 0x1405052E0; PDB symbol is ?Parse@CLoginProcess@@UEAA_NAEAVXPacket@@@Z.
    bool Parse(XPacket& xPacket) override;

    // === 数据库响应处理 ===
    /** @brief 处理数据库响应 (IDA: 0x1405044D0) */
    bool DBParse(XPacket& xPacket);

    // === 数据库响应处理函数 ===

    /** @brief 计费订单号响应2 (IDA: 0x140504670, sub='D') */
    bool ResHanBillingOrderNo2(XPacket& xPacket);

    /** @brief 特赦加载响应 (IDA: 0x140505010, sub='F') */
    bool ResIndulgenceLoad(XPacket& xPacket);

    /** @brief 账户事件响应 (IDA: 0x1405052F0, sub='U') */
    bool ResAccountEvent(XPacket& xPacket);

    /** @brief WM认证信息加载响应 (IDA: 0x140505700, sub='c') */
    bool ResWMAuthInfoLoad(XPacket& xPacket);

    /** @brief WM计费订单号响应 (IDA: 0x140505A50, sub='d') */
    bool ResWMBillingOrderNo(XPacket& xPacket);

    /** @brief 账户日期信息响应 (IDA: 0x140506710, sub='f') */
    bool ResAccountDateInfo(XPacket& xPacket);
};
