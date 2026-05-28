#pragma once

#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"

// Forward declarations
class CUser;
class XPacket;
class XSendPacket;

/**
 * @brief GameServer 角色阶段客户端请求处理器。
 *
 * 处理游戏服务器中的角色相关请求，包括：
 * - 进入游戏服务器
 * - 角色信息请求
 * - 角色保存
 * - 角色复活
 * - 切换服务器
 * - 成就奖励
 * - 交易密码
 * 等
 *
 * 根据 IDA 反编译:
 * - 构造函数: 0x1403B22C0
 * - Parse: 0x1403B23B0
 * - DBParse: 0x1403B6600
 */
class CCharacterProcess : public TXProcess<CUser> {
public:
    CCharacterProcess();
    virtual ~CCharacterProcess() override;

    // === 核心虚函数 ===
    /** @brief 按角色阶段子命令分派客户端请求 (IDA: 0x1403B23B0) */
    bool Parse(XPacket& xPacket) override;

    /** @brief 处理数据库响应 (IDA: 0x1403B6600) */
    bool DBParse(XPacket& xPacket);

    // === 客户端请求处理函数 ===

    /** @brief 进入游戏服务器请求 (IDA: 0x1403B2690, sub=0x21) */
    bool ReqEnterGameServer(XPacket& xPacket);

    /** @brief 角色信息请求 (IDA: 0x1403B2930, sub=0x31) */
    bool ReqCharacterInfo(XPacket& xPacket);

    /** @brief 角色保存请求 (IDA: 0x1403B31A0, sub=0x33) */
    bool ReqCharacterSave();

    /** @brief 角色复活请求 (IDA: 0x1403B3820, sub=0x42) */
    bool ReqCharacterRevive(XPacket& xPacket);

    /** @brief 角色预留复活请求 (IDA: 0x1403B3B10, sub=0x49) */
    bool ReqCharacterReserveRevive(XPacket& xPacket);

    /** @brief 角色改变动作请求 (IDA: 0x1403B3D30, sub=0x45) */
    bool ReqCharacterChangeMotion(XPacket& xPacket);

    /** @brief 角色检查进入迷宫 (IDA: 0x1403B5960, sub=0x40) */
    bool ReqCharacterCheckEnterMaze();

    /** @brief 交易密码请求 (IDA: 0x1403B5330, sub=0x18) */
    bool ReqTradePassword(XPacket& xPacket);

    /** @brief 角色加载标题 (IDA: 0x1403B4200, sub=0x23) */
    bool ReqCharacterLoadTitle();

    /** @brief 角色选择标题 (IDA: 0x1403B4410, sub=0x25) */
    bool ReqCharacterSelectTitle(XPacket& xPacket);

    /** @brief 角色收藏标题 (IDA: 0x1403B4730, sub=0x2A) */
    bool ReqCharacterFavoriteTitle(XPacket& xPacket);

    /** @brief 角色更新过场动画 (IDA: 0x1403B5FE0, sub=0x19) */
    bool ReqCharacterUpdateCutscene(XPacket& xPacket);

    /** @brief 角色更新特殊选项列表 (IDA: 0x1403B6390, sub=0x47) */
    bool ReqCharacterUpdateSpecialOptionList(XPacket& xPacket);

    /** @brief 角色切服请求 (IDA: 0x1403BF080, sub=0x60) */
    bool ReqCharacterChangeServer(XPacket& xPacket);

    /** @brief 角色获取奖励分享点 (IDA: 0x1403BF960, sub=0x63) */
    bool ReqCharacterGetRewardSharePoint(XPacket& xPacket);

    /** @brief 成就奖励请求 (IDA: 0x1403B4CD0, sub=0x74) */
    bool ReqAchieveReward(XPacket& xPacket);

    /** @brief 其他角色信息请求 (IDA: 0x1403BF520, sub=0x75) */
    bool ReqOtherCharacterInfo(XPacket& xPacket);

    /** @brief 角色社区请求 (IDA: 0x1403B4FE0, sub=0x77) */
    bool ReqCharacterCommnuity(XPacket& xPacket);

    /** @brief 包步骤检查请求 (IDA: 0x1403B5650, sub=0x05) */
    bool ReqPacketStepCheck(XPacket& xPacket);

    // === 数据库响应处理函数 ===

    /** @brief 包步骤检查响应 (IDA, sub=0x05) */
    bool ResPacketStepCheck(XPacket& xPacket);

    /** @brief 角色加载响应 (IDA: 0x1403B7060, sub=0x11) */
    bool ResCharacterLoad(XPacket& xPacket);

    /** @brief 角色保存响应 (IDA: 0x1403BC110, sub=0x12) */
    bool ResCharacterSave(XPacket& xPacket);

    /** @brief 角色加载现金响应 (IDA: 0x1403BAF10, sub=0x13) */
    bool ResCharacterLoadCash(XPacket& xPacket);

    /** @brief 角色标题响应 (IDA: 0x1403C01B0, sub=0x14) */
    bool ResCharacterTitle(XPacket& xPacket);

    /** @brief 进入迷宫响应 (IDA: 0x1403BC150, sub=0x41) */
    bool ResEnterMaze(XPacket& xPacket);

    /** @brief 角色切服响应 (IDA: 0x1403BC180, sub=0x42) */
    bool ResCharacterChangeServer(XPacket& xPacket);

    /** @brief 角色添加金币响应 (IDA: 0x1403BB1A0, sub=0x31) */
    bool ResCharacterAddMoney(XPacket& xPacket);

    /** @brief 角色添加BP响应 (IDA: 0x1403BB600, sub=0x32) */
    bool ResCharacterAddBP(XPacket& xPacket);

    /** @brief 角色添加以太响应 (IDA: 0x1403BBBA0, sub=0x34) */
    bool ResCharacterAddEther(XPacket& xPacket);

    /** @brief 角色添加现金响应 (IDA: 0x1403BBE60, sub=0x36) */
    bool ResCharacterAddCash(XPacket& xPacket);

    /** @brief 成就选择响应 (IDA: 0x1403BDA90, sub=0x61) */
    bool ResAchieveSelect(XPacket& xPacket);

    /** @brief 成就更新响应 (IDA: 0x1403BE090, sub=0x62) */
    bool ResAchieveUpdate(XPacket& xPacket);

    /** @brief 成就奖励响应 (IDA: 0x1403BE630, sub=0x63) */
    bool ResAchieveReward(XPacket& xPacket);

    /** @brief 成就重置响应 (IDA: 0x1403BEA60, sub=0x66) */
    bool ResAchieveReset();

    /** @brief 角色社区响应 (IDA: 0x1403BED80, sub=0x68) */
    bool ResCharacterCommnuity(XPacket& xPacket);

    /** @brief 创建交易密码响应 (IDA: 0x1403C06C0, sub=0x45) */
    bool ResCreateTradePassword(XPacket& xPacket);

    /** @brief 检查交易密码响应 (IDA: 0x1403C0B90, sub=0x46) */
    bool ResCheckTradePassword(XPacket& xPacket);

    /** @brief 交易密码状态检查响应 (IDA: 0x1403C0F80, sub=0x47) */
    bool ResTradePasswordStateCheck(XPacket& xPacket);

    /** @brief 继续交易密码响应 (IDA: 0x1403C12F0, sub=0x4B) */
    bool ResContinueTradePassword(XPacket& xPacket);

    /** @brief 其他角色信息响应 (IDA, sub=0x58) */
    bool ResOtherCharacterInfo(XPacket& xPacket);
};
