#pragma once

#include "Soulworker/GameServer/XLoginServer/LoginServer.h"

/**
 * @brief 角色阶段客户端请求处理器。
 *
 * 根据 `LoginServer.pdb` 中的模块信息，登录服存在独立的
 * `CCharacterProcess` 编译单元。本轮继续沿着已重新在 IDA 中核实过的
 * 角色上行链补骨架：
 * - `Parse`
 * - `ReqCharacterDelete`
 * - `ReqCharacterCheckName`
 * - `ReqCharacterChangeSlot`
 * - `ReqSelectCharacter`
 * - `ReqCharacterChangeServer`
 * - `ReqSecondPassword`
 *
 * 其余角色阶段上行请求仍保持“已确认存在但尚未还原”的状态，
 * 避免在证据不足时补入臆测逻辑。
 */
class CCharacterProcess : public TXProcess<CUser> {
public:
    CCharacterProcess() {
        SetCmd(eCMD_CHARACTER);
        SetName("CCharacterProcess");
    }

    /** @brief 按角色阶段子命令分派客户端请求。 */
    bool Parse(XPacket& xPacket);

    /** @brief 处理客户端角色选择请求。 */
    bool ReqSelectCharacter(XPacket& xPacket);

    /** @brief 处理客户端角色列表请求。 */
    bool ReqCharacterList(XPacket& xPacket);

    /** @brief 处理客户端删角请求。 */
    bool ReqCharacterDelete(XPacket& xPacket);

    /** @brief 处理客户端改名检测请求。 */
    bool ReqCharacterCheckName(XPacket& xPacket);

    /** @brief 处理客户端角色换槽请求。 */
    bool ReqCharacterChangeSlot(XPacket& xPacket);

    /** @brief 处理客户端建角请求。 */
    bool ReqCharacterCreate(XPacket& xPacket);

    /** @brief 处理客户端继续切服请求。 */
    bool ReqCharacterChangeServer(XPacket& xPacket);

    /** @brief 处理客户端代表角色检查请求。 */
    bool ReqCharacterRepresentativeCheck(XPacket& xPacket);

    /** @brief 处理客户端代表角色变更请求。 */
    bool ReqCharacterRepresentativeChange(XPacket& xPacket);

    /** @brief 处理客户端二级密码请求。 */
    bool ReqSecondPassword(XPacket& xPacket);

    /** @brief 校验二级密码格式是否合法。 */
    bool IsValidSecondPassword(char* strPassword, int& nErrorID);
};
