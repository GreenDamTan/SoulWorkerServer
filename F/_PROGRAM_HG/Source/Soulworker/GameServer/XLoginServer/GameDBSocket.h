#pragma once

#include "Soulworker/GameServer/XLoginServer/LoginServer.h"

/**
 * @brief LoginServer 与 GameDB / AccountDB 之间的回包处理器。
 *
 * 当前已还原的关键函数：
 * - `DBLoginParse`：分派登录相关 DB 回包。
 * - `ResLogin`：处理登录验证结果。
 * - `ResEnterServer`：处理“进入服务器/选择世界”阶段的回包。
 * - `ResLoginCharacterCount`：处理服务器组角色计数回包。
 * - `ResOptionLoad`：处理选项与服务器内容开关回包。
 * - `ResSecondPW*`：处理二级密码的继续/创建/校验/状态查询回包。
 * - `DBCharacterParse` / `ResCharacterList`：处理角色列表阶段已确认的角色 DB 回包。
 * - `ResCharacterCreate`：处理建角回包并刷新本地角色列表骨架。
 * - `ResCharacterDelete` / `ResCharacterCheckName` / `ResCharacterChangeSlot`：
 *   处理删角、角色名检测与换槽回包。
 * - `ResSelectCharacter` / `ResCharacterChangeServer` / `ResCharacterUpdateMap`：
 *   处理选择角色后至切服状态同步阶段的已确认回包。
 * - `ResCharacterRepresentative*`：处理代表角色校验与切换回包。
 */
class XGameDBSocket : public TXDBSocketT<CUser> {
public:
    /** @brief 按 DB 包里的会话号回查当前登录连接。 */
    // 对齐 IDA: 参数类型 H = int
    CUser* FindUser(int xSessionID) override;
    /** @brief 按主命令分派 DBAgent 回包。 */
    bool DBParse(CUser* pUser, XPacket& xPacket) override;
    /** @brief 分派登录阶段的 DB 回包。 */
    bool DBLoginParse(CUser* pUser, XPacket& xPacket);
    /** @brief 分派角色阶段的 DB 回包。 */
    bool DBCharacterParse(CUser* pUser, XPacket& xPacket);
    /** @brief DBAgent 断线后的登录服收口。 */
    void OnDisConnect() override;
    /** @brief DBAgent 未连接成功时的登录服收口。 */
    void OnNotConnect() override;
    /** @brief 处理 AccountDB 返回的登录结果。 */
    bool ResLogin(CUser* pUser, XPacket& xPacket);
    /** @brief 处理 AccountDB 返回的入服结果。 */
    bool ResEnterServer(CUser* pUser, XPacket& xPacket);
    /** @brief 处理服务器组角色计数回包。 */
    bool ResLoginCharacterCount(CUser* pUser, XPacket& xPacket);
    /** @brief 处理客户端选项/按键配置回包。 */
    bool ResOptionLoad(CUser* pUser, XPacket& xPacket);
    /** @brief 处理二级密码继续流程回包。 */
    bool ResSecondPWContinue(CUser* pUser, XPacket& xPacket);
    /** @brief 处理二级密码创建回包。 */
    bool ResSecondPWCreate(CUser* pUser, XPacket& xPacket);
    /** @brief 处理二级密码校验回包。 */
    bool ResSecondPWCheck(CUser* pUser, XPacket& xPacket);
    /** @brief 处理二级密码状态检查回包。 */
    bool ResSecondPWStateCheck(CUser* pUser, XPacket& xPacket);
    /** @brief 处理 SG / 外部认证信息加载回包。 */
    bool ResSGAuthInfoLoad(CUser* pUser, XPacket& xPacket);
    /** @brief 处理角色列表回包。 */
    bool ResCharacterList(CUser* pUser, XPacket& xPacket);
    /** @brief 处理建角回包。 */
    bool ResCharacterCreate(CUser* pUser, XPacket& xPacket);
    /** @brief 处理删角回包。 */
    bool ResCharacterDelete(CUser* pUser, XPacket& xPacket);
    /** @brief 处理角色名检测回包。 */
    bool ResCharacterCheckName(CUser* pUser, XPacket& xPacket);
    /** @brief 处理角色换槽回包。 */
    bool ResCharacterChangeSlot(CUser* pUser, XPacket& xPacket);
    /** @brief 处理选择角色后的切服/入图回包。 */
    bool ResSelectCharacter(CUser* pUser, XPacket& xPacket);
    /** @brief 处理角色切服结果回包。 */
    bool ResCharacterChangeServer(CUser* pUser, XPacket& xPacket);
    /** @brief 处理角色地图更新结果回包。 */
    bool ResCharacterUpdateMap(CUser* pUser, XPacket& xPacket);
    /** @brief 处理代表角色校验回包。 */
    bool ResCharacterRepresentativeCheck(CUser* pUser, XPacket& xPacket);
    /** @brief 处理代表角色切换回包。 */
    bool ResCharacterRepresentativeChange(CUser* pUser, XPacket& xPacket);

private:
    /**
     * @brief 处理单个布罗奇外观列表项对角色显示信息的影响。
     *
     * 当前已按 IDA 核实逻辑接入 `TB_ITEM / TB_BROACH_SET` 推导链，
     * 用于把三件套布罗奇组合转成角色列表展示时的 `STBuffInfo`。
     */
    void BroachEffect(const PS_BROACH_SHAPE& broachShape, STMyCharInfoEx* characterInfo);
    /** @brief 发送单个物品统计包。 */
    void SendStatisticsDB_Item(CUser* pUser,
                               std::uint8_t byFlag,
                               std::int64_t biSerial,
                               unsigned int dwUCID,
                               unsigned int dwItemID,
                               std::uint8_t byUpgrade,
                               std::uint8_t byUpgradeLimit);
};
