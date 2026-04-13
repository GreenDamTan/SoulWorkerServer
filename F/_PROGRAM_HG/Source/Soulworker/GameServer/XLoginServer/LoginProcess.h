#pragma once

#include "Soulworker/GameServer/XLoginServer/LoginServer.h"

/**
 * @brief 登录服客户端请求处理器。
 *
 * 该类对应 `XLoginProcess`，负责解析客户端发来的登录主命令，
 * 并把请求分发到登录、服务器列表、入服排队、选项同步等子流程。
 *
 * 依赖关系：
 * - 继承 `TXProcess<CUser>`，因此直接绑定一个客户端对象。
 * - 依赖 `XLoginServer` 查询全局配置、转发 DB 请求。
 */
class XLoginProcess : public TXProcess<CUser> {
public:
    XLoginProcess() {
        SetCmd(eCMD_LOGIN);
        SetName("XLoginProcess");
    }

    /**
     * @brief 按子命令分派客户端请求。
     * @param xPacket 客户端发送的协议包。
     * @return 返回 true 表示包已处理；返回 false 表示当前连接无效等严重错误。
     */
    bool Parse(XPacket& xPacket);

    /** @brief 处理账号登录请求。 */
    bool ReqUserLogin(XPacket& xPacket);
    /** @brief 请求服务器/世界列表；该函数在 PDB 中为 `void`。 */
    void SendServerList(XPacket& xPacket);
    /** @brief 请求切服连接信息。 */
    bool ReqServerConnect(XPacket& xPacket);
    /** @brief 请求进入服务器或等待队列。 */
    bool ReqEnterServer(XPacket& xPacket);
    /** @brief 同步客户端选项位到 AccountDB。 */
    bool ReqOptionUpdate(XPacket& xPacket);
    /** @brief 主动查询当前等待队列位置。 */
    bool ReqEnterWaitCheck(XPacket& xPacket);
    /** @brief 取消当前等待队列请求。 */
    bool ReqEnterWaitCancel(XPacket& xPacket);
};
