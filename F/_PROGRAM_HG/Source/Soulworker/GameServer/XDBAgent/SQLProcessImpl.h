#pragma once

#include "Soulworker/GameServer/XDBAgent/SQLProcess.h"
#include "Soulworker/GameServer/XCore/XServer/TXDBSocket.h"
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/GameServer/XRelayServer/UserObject.h"  // For ST_BLOCK_INFO
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_MyRoomStructs.h"  // For ST_MYROOM_ITEM
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"  // For ST_NETCAFE_MISSION_INFO

// SQL 处理器基类模板
// 所有 SQL 处理器继承自 XSQLProcess，结构相同：
// - 构造函数设置 MainCmd 和名称
// - DBParse 根据 SubCmd 路由到具体处理函数
template <std::uint8_t MainCmdValue>
class TXSQLProcessBase : public XSQLProcess {
public:
    TXSQLProcessBase(const char* name) {
        SetCmd(MainCmdValue);
        SetName(name);
    }

    virtual ~TXSQLProcessBase() = default;

    // DBParse - 子类需要实现此方法处理具体 SubCmd 路由
    virtual std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
        // 默认返回 -1 表示未处理
        return -1;
    }

    bool Clear() override {
        return true;
    }
};

// XSQLSystemPorcess - 注意保留原始拼写错误
// MainCmd = 0x01
class XSQLSystemPorcess : public TXSQLProcessBase<0x01> {
public:
    XSQLSystemPorcess();
    virtual ~XSQLSystemPorcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

private:
    // SubCmd handlers
    std::int32_t ReqConnectHanNetCafeList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqConnectHanNetCafeUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqWorldEventInfoSync(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqGetDBTime(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
};

// XSQLLoginProcess
// MainCmd = 0x02
class XSQLLoginProcess : public TXSQLProcessBase<0x02> {
public:
    XSQLLoginProcess();
    virtual ~XSQLLoginProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

private:
    // 主要 SubCmd handlers (完整列表有 50+ 个方法)
    std::int32_t ReqUserLogin(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqUserLogout(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqUserLoginForNHN(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqUserLoginForSG(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqUserLoginForGF(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqUserLoginForTWN(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqUserLoginForCHN(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqUserLoginForWM(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqEnterServer(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqClearUserState(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqUpdateUserState(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLoginCharacterCount(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLoadServerGroupInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAddServerGroupInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqUpdateServerGroupInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqDeleteServerGroupInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqUpdateCharacterCountForServer(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLoginUpdateUserState(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqOptionKeyUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqOptionLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqOptionUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqOptionLoad_UCID(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCreateSecondPW(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCheckSecondPW(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqSecondPWStateCheck(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqContinueSecondPW(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCreateTradePW(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCheckTradePW(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqTradePWStateCheck(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqTradePWContinue(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqUserLoadCash(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqUserAddCash(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqUserSetCash(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCheckServerGroupID(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqSGAuthInfoUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqSGAuthInfoLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqIndulgenceLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqIndulgenceUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAutoMailRecv(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAccountSessionIDUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAccountEventLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAccountEventUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAutoBlockAdd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqWMAuthInfoUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqWMAuthInfoLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqGFAuthInfoUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqGFAuthInfoLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqUseCoupon(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAccountDateInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAccountCashMileageInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAccountCashMileageUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqHanBillingOrderNo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqHanBillingUpdateInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqHanBillingOrderNo2(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqSGBillingOrderNo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqTWNBillingOrderNo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqTWNBillingUpdateInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqWMBillingOrderNo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqWMBillingUpdateInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    // TODO: 需人工审查 - 其他 Billing 平台方法待后续实现

    // 辅助方法
    std::int32_t LoadCashMileageInfo(XDBStmt* pDBStmt, std::uint32_t dwUAID, struct PS_DB_CASH_MILEAGE_INFO* psDBRes);

    // 辅助方法 - 用于 ReqUserLogin 的账户检查
    /// @brief 检查服务器状态
    bool CheckServerState(XDBStmt* pDBStmt, wchar_t* szID, int nIP, int* pServerState, wchar_t* szResultMsg);
    /// @brief 检查账户是否被封禁
    bool CheckBlockAccount(XDBStmt* pDBStmt, wchar_t* szID, unsigned int* pUAID, wchar_t* szResultMsg);
    /// @brief 检查 IP 是否被封禁
    bool CheckBlockIP(XDBStmt* pDBStmt, int nIP, unsigned char by1, unsigned char by2, unsigned char by3, wchar_t* szResultMsg);
    /// @brief 检查 MAC 地址是否被封禁（宽字符版本）
    bool CheckBlockMac(XDBStmt* pDBStmt, wchar_t* szMacAddress, wchar_t* szResultMsg);
    /// @brief 检查 MAC 地址是否被封禁（窄字符版本）
    bool CheckBlockMac(XDBStmt* pDBStmt, char* szMacAddress, wchar_t* szResultMsg);

    // 辅助方法 - 用于 ReqEnterServer 的密码状态检查
    /// @brief 获取二级密码状态（注意原始拼写 Seconde）
    std::uint8_t GetSecondePasswordState(XDBStmt* pDBStmt, int nUAID, int* nAccountState);
    /// @brief 获取交易密码状态
    std::uint8_t GetTradePasswordState(XDBStmt* pDBStmt, int nUAID, int* nAccountState);

    // 辅助方法 - 现金里程更新/使用
    /// @brief 更新现金里程
    std::int16_t UpdateCashMileage(XDBStmt* pDBStmt, struct PS_DB_CASH_MILEAGE_LIST* psDBMileageList);
    /// @brief 使用现金里程
    std::int16_t UseCashMileage(XDBStmt* pDBStmt, struct PS_DB_CASH_MILEAGE_LIST* psDBMileageList);
};

// XSQLCharacterProcess
// MainCmd = 0x03
class XSQLCharacterProcess : public TXSQLProcessBase<0x03> {
public:
    XSQLCharacterProcess();
    virtual ~XSQLCharacterProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

    // Public helper methods
    std::int32_t UpdateLastUCID(XDBStmt* pDBStmt, unsigned int dwUAID, unsigned int dwLastUCID);
    unsigned int SelectLastUCID(XDBStmt* pDBStmt, unsigned int dwUAID);

private:
    // SubCmd handlers - Character basic operations (0x01-0x08)
    std::int32_t ReqCharacterList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterCreate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterDelete(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterCheckName(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPacketStepCheck(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterChangeSlot(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterRepresentativeCheck(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterRepresentativeChange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // SubCmd handlers - Character load/save (0x11-0x12)
    std::int32_t ReqCharacterLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterSave(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // SubCmd handlers - Title operations (0x14-0x19)
    std::int32_t ReqLoadTitle(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAddTitle(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqSelectTitle(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqClearTitle(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqDeleteTitle(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqOpenTitle(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // SubCmd handlers - Character selection/Profile (0x21-0x28)
    std::int32_t ReqEnterGameServer(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqSelectCharacter(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterChangeServerNoReturn(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqFavoriteTitle(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterProfilePhotoLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterProfilePhotoAdd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterProfilePhotoUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterProfilePhotoChange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // Photo helper functions
    std::int32_t CharacterProfilePhotoUpdate(XDBStmt* pDBStmt, std::uint32_t dwUCID, ST_PROFILE_PHOTO_INFO& stInfo, std::int32_t& nError);

    // SubCmd handlers - Money/BP/Ether operations (0x31-0x39)
    std::int32_t ReqCharacterAddMoney(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterAddBP(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterDeleteConnection(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterAddEther(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterPvPKillCount(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterExpSave(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterLevelUp(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterUpdateMapID(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // SubCmd handlers - Server change/World state (0x42-0x4C)
    std::int32_t ReqCharacterChangeServer(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqExitServer(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterAddFriendPoint(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterResetWorldState(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterLoadWorldState(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterUpdateWorldState(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterLoadMazeEnterCount(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // SubCmd handlers - District/Maze state (0x50-0x57)
    std::int32_t ReqCharacterUpdateDistrictState(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterUpdateMazeState(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterUpdateInfiniteTowerLimitTime(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterUpdateInfiniteTowerClearChapter(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterUpdateKilledUser(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterClearKilledUser(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqEchelonUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // SubCmd handlers - Other character info (0x58-0x59)
    std::int32_t ReqOtherCharacterInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqUpdateUserInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // SubCmd handlers - Achievement/ClassScene (0x61-0x69)
    std::int32_t ReqAchieveSelect(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAchieveUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAchieveReward(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAchieveCollect(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAchieveReset(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqClassSceneUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterCommunitySelect(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterCommunityUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // ClassScene helper methods
    std::int32_t CreateClassScene(XDBStmt* pDBStmt, unsigned int dwUAID, std::uint8_t byClass);
    std::int32_t UpdateClassScene(XDBStmt* pDBStmt, unsigned int dwUAID, std::uint8_t byClass, PS_CLASS_SCENE& stClassScene);

    // Achievement helper methods
    std::int32_t AchieveClear(XDBStmt* pDBStmt, unsigned int dwUCID);
    std::int32_t AchieveCreateBit(XDBStmt* pDBStmt, unsigned int dwUCID);
    std::int32_t AchieveUpdate(XDBStmt* pDBStmt, unsigned int dwUCID, struct ST_ACHIEVE_UPDATE* pstUpdate);
    std::int32_t AchieveReward(XDBStmt* pDBStmt, unsigned int dwUCID, struct ST_ACHIEVE_BIT* pstBit);
    std::int32_t LoadAchieve(XDBStmt* pDBStmt, unsigned int dwUCID, struct ST_ACHIEVE_LIST* pstList);
    std::int32_t LoadAchieveBit(XDBStmt* pDBStmt, unsigned int dwUCID, struct ST_ACHIEVE_BIT* pstBit, struct ST_ACHIEVE_CATEGORY* pstCategory);
    std::int32_t LoadInfiniteTowerInfo(XDBStmt* pDBStmt, unsigned int dwUCID, struct PS_INFINITE_TOWER_INFO* stInfo, int* nPcLimitCount);
    std::int32_t LoadKilledUserInfo(XDBStmt* pDBStmt, unsigned int dwUCID, struct PS_KILLED_USER_INFOS* stKilledUserInfo);
    std::int32_t LoadRepresentativeInfo(XDBStmt* pDBStmt, unsigned int dwUAID, struct ST_REPRESENTATIVE_INFO* stInfo);
    std::int32_t LoadClassScene(XDBStmt* pDBStmt, unsigned int dwUAID, std::uint8_t byClass, struct PS_CLASS_SCENE* stClassScene);
    std::int32_t LoadCharacterMileage(XDBStmt* pDBStmt, unsigned int dwUCID, int* nDyePoint, int* nRenovatePoint, int* nRefinePoint);
    std::int32_t LoadCharacterEqualizerInfo(XDBStmt* pDBStmt, unsigned int dwUCID, int* nEqualizerID);
    std::int32_t LoadEnterWorldMode(XDBStmt* pDBStmt, unsigned int dwUCID, struct ST_ENTER_WORLD_MODE_INFO* stEnterList);
    std::int32_t LoadPrevMapID(XDBStmt* pDBStmt, unsigned int dwUCID, int* nPrevMapID);

    // Character detail info helper methods
    std::int32_t SelectOtherCharacterInfo(XDBStmt* pDBStmt, unsigned int dwUCID, struct PS_DB_CHARACTER_INFO_OTHER_RES* psRes);
    std::int32_t SelectCharacterItemInfoOther(XDBStmt* pDBStmt, unsigned int dwUCID, struct PS_DB_CHARACTER_INFO_OTHER_RES* psRes);
    std::int32_t SelectCharacterSocketInfoOther(XDBStmt* pDBStmt, unsigned int dwUCID, struct PS_DB_CHARACTER_INFO_OTHER_RES* psRes);
    std::int32_t SelectCharacterQuikSlotCard(XDBStmt* pDBStmt, unsigned int dwUCID, struct PS_DB_CHARACTER_INFO_OTHER_RES* psRes);
    std::int32_t SelectCharacterSkillInfo(XDBStmt* pDBStmt, unsigned int dwUCID, struct PS_DB_CHARACTER_INFO_OTHER_RES* psRes);

    // SubCmd handlers - Position/FP/State (0x70-0x78)
    std::int32_t ReqCharacterUpdatePos(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterFPUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterFPInit(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterAddState(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterRemoveState(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterFPBoosterAdd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterStatusUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // Helper methods for Position/FP/State
    std::int32_t AddCharacterState(XDBStmt* pDBStmt, unsigned int dwActorID, int nState);
    std::int32_t ReqCharacterLocationClearByServerID(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterCheckLocation(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // SubCmd handlers - League/Recycle/Awaken (0x80-0x89)
    std::int32_t ReqCharacterUpdateLeagueWithdrawPenalty(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterUpdateLeagueDeletePenalty(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterAddRecycle(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterUpdateAwaken(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterBroachActiveEffect(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterUpdateFreeReviveCount(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterCheatCountUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterEqualizerUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // SubCmd handlers - Title/Renovate (0x91-0x92)
    std::int32_t ReqAddTitleAll(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterRenovatePointUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
};

// XSQLPartyProcess
// MainCmd = 0x04
class XSQLPartyProcess : public TXSQLProcessBase<0x04> {
public:
    XSQLPartyProcess();
    virtual ~XSQLPartyProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

private:
    // SubCmd handlers
    std::int32_t ReqCreateParty(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqJoinParty(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeaveParty(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqUpdateMemberInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPartyChangeMaster(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPartyDelete(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPartyUpdateInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPartyLoginMember(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPartyLoadAll(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPartyMatchingCreate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPartyTypeUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

public:
    // Helper methods
    std::int32_t LoadPartyID(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::uint32_t& dwPartyID);

private:
    std::int32_t PartyDelete(XDBStmt* pDBStmt, int xReturnSessionID, PS_PARTY_LEAVE* stPartyLeave);
};

// XSQLFriendProcess
// MainCmd = 0x05
class XSQLFriendProcess : public TXSQLProcessBase<0x05> {
public:
    XSQLFriendProcess();
    virtual ~XSQLFriendProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

private:
    // SubCmd handlers
    std::int32_t ReqFriendLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqInviteFriend(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqInviteCheckFriend(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqDeleteFriend(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAcceptFriend(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAddBlockList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqDelBlockList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqRecruitList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqRecruitAdd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqRecruitDelete(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqFriendFind(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // Helper methods
    std::int32_t DeleteFriend(XDBStmt* pDBStmt, std::uint32_t dwID, std::uint32_t dwFriendID,
                              std::uint32_t& dwUAID, std::uint32_t& dwFriendUAID);
    std::int32_t DeleteFriend(XDBStmt* pDBStmt, std::uint32_t dwID, std::vector<PS_DB_FRIEND>& vecDelete);
    std::int32_t AddBlockList(XDBStmt* pDBStmt, PS_REQ_FRIEND_BLOCK_ADD* stBlock, DB_BLOCK_INFO* stBlockInfo);
    bool DeleteBlockList(XDBStmt* pDBStmt, PS_REQ_FRIEND_BLOCK_DELETE* stBlock, std::uint32_t& dwTargetUCID);
    std::int32_t LoadCharCommunity(XDBStmt* pDBStmt, std::uint32_t dwActorID, ST_CHAR_COMMUNITY& stCharCommunity);
    std::int32_t LoadFriend(XDBStmt* pDBStmt, std::uint32_t dwActorID, PS_DB_FRIEND_LIST& stFriendList,
                            std::vector<PS_DB_FRIEND>& vecDelete);
    std::int32_t LoadFriendBlock(XDBStmt* pDBStmt, std::uint32_t dwUAID, PS_BLOCKLIST_INFO& stBlockList);
    std::int32_t AddFriend(XDBStmt* pDBStmt, std::uint32_t dwReqID, std::uint32_t dwTargetID,
                           std::uint8_t byReqType, std::uint8_t byTargetType,
                           ST_FRIEND_INFO& stFriend, std::uint32_t& dwTargetUAID);
    bool CheckFriendName(XDBStmt* pDBStmt, wchar_t* strName, std::uint32_t& dwUCID);
    bool CheckFriendCount(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int16_t shType, int iMax);
    bool CheckAddFriend(XDBStmt* pDBStmt, std::uint32_t dwTargetID, std::uint32_t dwReqID, PS_DB_FRIEND_ACCEPT_RES& stResult);
    std::int32_t UpdateFriend(XDBStmt* pDBStmt, std::uint32_t dwID, std::uint32_t dwFriendID,
                              std::uint8_t byReqType, std::uint8_t byTargetType, std::uint32_t& dwFriendUAID);
    bool AddRecruit(XDBStmt* pDBStmt, std::uint32_t dwID, std::int64_t tAddTime);
    bool DeleteRecruit(XDBStmt* pDBStmt, std::uint32_t dwID);
};

// XSQLPostProcess
// MainCmd = 0x06
class XSQLPostProcess : public TXSQLProcessBase<0x06> {
public:
    XSQLPostProcess();
    virtual ~XSQLPostProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

private:
    // SubCmd handlers (25 handlers)
    std::int32_t ReqPostSendList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPostRecvList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPostSend(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPostRead(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPostReceipt(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPostSendDel(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPostRecvDel(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPostSendBack(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPostSendNameCheck(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPostSystemSend(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqGMTSendPostList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqGMTSendPostSend(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPostSave(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPostSaveList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPostAccountList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPostAccountRead(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPostAccountReceipt(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPostAccountDel(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPostAccountSend(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPostDeleteAll(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPostReceiptAll(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPostLevelUpEventLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPostLevelUpEventUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPostLevelUpEventReset(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPostRestoreLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // Helper functions
    std::int16_t UpdateRead(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial,
                           std::uint8_t byPostFlag, bool bDel, std::int64_t biRemainTime,
                           std::uint16_t* wPostCount, int* nErrorCode);
    std::int16_t UpdateReceipt(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial,
                              std::uint8_t byPostFlag, std::int64_t biRemainTime, int* nError);
    std::int16_t CreatePostItem(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_RES_STORAGE_INFO* stCreateItem);
    std::int16_t PostRecvItemUpdate(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_RES_STORAGE_INFO* stUpdateItem);
    std::int16_t UpdateSendItem(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_RES_STORAGE_INFO& stUpdateList);

    // Account Post helper functions
    std::int16_t UpdateAccountPostReceipt(XDBStmt* pDBStmt, std::uint32_t dwUAID, std::int64_t biSerial,
                                          std::uint8_t byFlag, std::int64_t* biRemainTime, int* nError);
    std::int16_t UpdateAccountPostReceiptAll(XDBStmt* pDBStmt, std::uint32_t dwUAID, std::int64_t biSerial,
                                             std::uint8_t byFlag, std::int64_t* biRemainTime, std::uint8_t bDecrease,
                                             std::uint16_t* wPostCount, int* nError);
    std::int16_t PostAccountSend(XDBStmt* pDBStmt, ST_ACCOUNT_POST_DATA* pstData, std::uint16_t* wAccountPostCount,
                                 std::int64_t biGMTNo, int* nPostErrorCode);
    std::int16_t UpdateReceiptAll(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial,
                                  std::uint8_t byPostFlag, std::int64_t biRemainTime, std::uint8_t bDecrease,
                                  std::uint16_t* wPostCount, int* nError);
    std::int16_t LoadPostRestoreItemSocket(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_ITEM_RESTORE_LIST* psRestoreItemList);

    // GMT Post helper functions
    std::int16_t SendPostSystemSend(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial,
                                    ST_SYSTEM_POST* stSystemPost, std::int64_t* biRegTime,
                                    std::uint16_t* wPostCount, std::int64_t biDelDate,
                                    std::int64_t biGMTNo, int* nPostErrorCode);
    std::int16_t SendPostItemRestore(XDBStmt* pDBStmt, std::uint32_t dwUCID, ST_POST_DATA* stPostData,
                                     std::uint16_t* wPostCount, std::int64_t biDelDate,
                                     std::int64_t biGMTNo, int* nPostErrorCode);
    std::int16_t UpdateGMTSendPost(XDBStmt* pDBStmt, std::int64_t biNo, std::uint32_t dwUCID);
};

// XSQLLeagueProcess
// MainCmd = 0x07
class XSQLLeagueProcess : public TXSQLProcessBase<0x07> {
public:
    XSQLLeagueProcess();
    virtual ~XSQLLeagueProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

private:
    // SubCmd handlers (33 handlers)
    std::int32_t ReqLeagueCreate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueDelete(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueBoard(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueWIthDraw(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueApplicant(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueKick(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueInviteAccept(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueApplicantAccept(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueApplicantReject(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueOverlapName(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueNoticeChange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueNameChange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueCardChange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeaguePositionNameChange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueAuthChange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueMemberPositionChange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueApplicantDelete(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueNoticeDateReset(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueOpenOrNot(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueRecruitNotice(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueSearch(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueRecordUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueDelegate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeaguGetWealth(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueLevelup(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueSkillLearn(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueMemberExpInit(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueInventoryMove(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueInventoryInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueInit(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeagueList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqGMTLeagueInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // Helper functions
    std::int16_t DelLeagueMember(XDBStmt* pDBStmt, std::uint32_t dwUCID, int nLeagueID, std::int64_t biPenalty);
    std::int16_t KickoutLeagueMember(XDBStmt* pDBStmt, std::uint32_t dwUCID, int nLeagueID);
    bool LeagueJoin(XDBStmt* pDBStmt, ST_REQ_LEAGUE_INVITE_ACCEPT& stAccept, std::int64_t biJoinDate);
    std::int16_t LeagueApplicant(XDBStmt* pDBStmt, ST_LEAGUE_APPLICANT& stApplicant);
    std::int16_t LeagueApplicantJoin(XDBStmt* pDBStmt, ST_REQ_LEAGUE_APPLICANT_ACCEPT* stAccept);
    std::int16_t LeagueApplicantReject(XDBStmt* pDBStmt, ST_REQ_LEAGUE_APPLICANT_REJECT* stReject);
    std::int16_t LoadLeagueMember(XDBStmt* pDBStmt, std::uint32_t dwUCID, ST_LEAGUE_MEMBER_EX& stMemberEx);

public:
    // League info load helpers (per IDA)
    std::int16_t LoadLeagueInfo(XDBStmt* pDBStmt, int nLeagueID, ST_LEAGUE_INFO& stLeagueInfo);

private:
    std::int16_t LoadLeagueMemberList(XDBStmt* pDBStmt, int nLeagueID, ST_LEAGUE_MEMBER_LIST& stMemberList);
    std::int16_t LoadLeagueBoard(XDBStmt* pDBStmt, int nLeagueID, ST_LEAGUE_BOARD_LIST& stBoardList);
    std::int16_t LoadLeagueApplicantList(XDBStmt* pDBStmt, int nLeagueID, ST_LEAGUE_APPLICANT_LIST& stApplicantList);
    std::int16_t LoadLeagueRecordList(XDBStmt* pDBStmt, int nLeagueID, ST_LEAGUE_RECORD_LIST& stRecordList);
    // League search helpers (per IDA)
    std::int16_t SearchLeagueToName(XDBStmt* pDBStmt, const ST_REQ_LEAGUE_SEARCH& stSearch, PS_LEAGUE_SUMMARY_LIST& psLeagueSummaryList);
    std::int16_t SearchLeagueToMaster(XDBStmt* pDBStmt, const ST_REQ_LEAGUE_SEARCH& stSearch, PS_LEAGUE_SUMMARY_LIST& psLeagueSummaryList);
    // League apply info helper (per IDA 0x1400727C0)
    bool GetApplyLeagueInfo(XDBStmt* pDBStmt, std::uint32_t dwUCID, ST_LEAGUE_APPLICANT_CHECK_LIST& stApplyLeagueList);
};

// XSQLForceProcess
// MainCmd = 0x08
class XSQLForceProcess : public TXSQLProcessBase<0x08> {
public:
    XSQLForceProcess();
    virtual ~XSQLForceProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

private:
    // SubCmd handlers (10 handlers)
    std::int32_t ReqCreateForce(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqJoinForce(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLeaveForce(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqUpdateMemberInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqForceChangeMaster(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqForceDelete(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqForceUpdateInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqForceLoadAll(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqForceMatchingCreate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqForceTypeUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // Force helper functions
    std::int16_t ForceDelete(XDBStmt* pDBStmt, int xReturnSessionID, PS_FORCE_LEAVE* stForceLeave);

public:
    std::int16_t LoadForceID(XDBStmt* pDBStmt, std::uint32_t dwMemberID, std::uint32_t* dwForceID);
};

// XSQLItemProcess
// MainCmd = 0x21
class XSQLItemProcess : public TXSQLProcessBase<0x21> {
public:
    XSQLItemProcess();
    virtual ~XSQLItemProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

    // Public helper methods for league inventory operations (per IDA)
    std::int16_t SelectItemSerial(XDBStmt* pDBStmt, STItem* pItemList);  // 加载物品详情
    std::int16_t SelectSocketItem(XDBStmt* pDBStmt, std::int64_t biSerial, PS_ITEM_SOCKET_LIST* pSocketList);
    std::int16_t SelectBroachItem(XDBStmt* pDBStmt, std::int64_t biSerial, PS_ITEM_BROACH_LIST* pBroachList);
    std::int16_t SelectPackageItem(XDBStmt* pDBStmt, std::int64_t biPackageSerial, PS_ITEM_PACKAGE_LIST* pPackageList);

    // Public helper methods for item count/deletion (per IDA)
    std::int16_t UpdateItemCount(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial, std::int16_t shCount);
    std::int16_t DeleteItem(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial, std::uint8_t byInvenType, std::int16_t shSlotPos);

    // Public helper methods for item selection (per IDA)
    std::int16_t SelectItem(XDBStmt* pDBStmt, std::uint32_t dwID, std::uint8_t byInvenType, PS_RES_STORAGE_INFO& psResStorage);
    std::int16_t SelectItem(XDBStmt* pDBStmt, std::uint32_t dwID, std::uint8_t byInvenType, std::uint8_t byFlag, PS_RES_STORAGE_INFO& psResStorage);

    // Public helper methods for item update (per IDA)
    std::int16_t UpdateItem(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial, std::uint8_t byInvenType, std::int16_t shSlotPos, STItem* pItem);
    std::int16_t UpdateItem(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial, std::uint8_t byInvenType, std::int16_t shSlotPos, std::int16_t shCount);
    std::int16_t MoveItem(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial, std::uint8_t byInvenType, std::int16_t shSlotPos, std::uint8_t byBindType, std::uint8_t byStoreType);

    // Public helper methods for repurchase (per IDA)
    std::int16_t RepurchaseItem(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial, std::uint8_t byInvenType, std::int16_t shSlotPos, STItem* pItem);
    std::int16_t AddRepurchaseList(XDBStmt* pDBStmt, std::int64_t biSrcSerial);
    std::int16_t DeleteRepurchaseList(XDBStmt* pDBStmt, std::int64_t biSrcSerial);

    // Public helper methods for extend slot (per IDA)
    std::int16_t SelectExtendSlotStep(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::uint8_t& byCommonStep, std::uint8_t& byConsumeStep, std::uint8_t& byCostumeStep, std::uint8_t& byCardStep, std::uint8_t& byBankCommon, std::uint8_t& byBankCostume);

    // Public helper methods for item upgrade/socket (per IDA)
    std::int16_t ItemUpgrade(XDBStmt* pDBStmt, std::uint32_t dwUCID, STItem* stItemInfo);
    std::int16_t SocketItemEquip(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial, STItem* stSocketItem, std::uint8_t bySocketPos);
    std::int16_t SocketItemDetach(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial, std::uint8_t byDetachPos);

    // Public helper methods for item reduce/endurance (per IDA)
    std::int16_t ReduceItem(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial, std::uint8_t byInvenType, std::int16_t shSlotPos, std::int16_t shCount);
    std::int16_t UpdateEndurance(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::uint8_t byInvenType, STItem* stItem);
    std::int16_t UpdateEndurance(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial, std::uint8_t byEndurance);

    // Public helper methods for item upgrade/exp (per IDA)
    std::int16_t UpgradeLimit(XDBStmt* pDBStmt, std::uint32_t dwActorID, std::int64_t biSerial, std::uint8_t byLimit);
    std::int16_t UpdateItemExp(XDBStmt* pDBStmt, std::uint32_t dwActorID, std::int64_t biSerial, std::int32_t nExp);

    // Public helper methods for item user change/move (per IDA)
    std::int16_t ItemUserChange(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial, std::uint8_t byInvenType, std::int16_t shSlotPos, STItem* stItemInfo);
    std::int16_t UpdateItemMove(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::uint32_t dwTargetUCID, std::int64_t biSerial, std::int16_t shSlotPos);

    // Public helper methods for appearance (per IDA)
    std::int16_t AppearanceUpdate(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::uint16_t wAppearanceID, std::int64_t biEndDate);

    // Public helper methods for quickslot/post/dye (per IDA)
    std::int16_t UpdateQuickSlotItem(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::uint32_t ItemID1, std::uint32_t ItemID2, std::uint32_t ItemID3, std::uint32_t ItemID4);
    std::int16_t SelectPostItemSerial(XDBStmt* pDBStmt, STItem* stItem);
    std::int16_t UpdateDyePoint(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int32_t nPoint);

    // Public helper methods for account bank/item bind/renovate (per IDA)
    std::int16_t SelectAccountBankSlotStep(XDBStmt* pDBStmt, std::uint32_t dwUAID, std::uint8_t& byAccountBankCommonStep, std::uint8_t& byAccountBankFashionStep);
    bool UpdateItemBindType(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial, std::uint8_t byBindType);
    std::int16_t UpdateRenovatePoint(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int32_t nPoint);
    std::int16_t UpdateRefinePoint(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int32_t nPoint);
    std::int16_t OpenCardDeck(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_QUICKSLOT_CARD* psInfo);

    // Public helper methods for error message (per IDA)
    void SendDBErrorMsg(int xReturnSessionID, std::uint8_t bySubCmd, int nErrorCode);

    // Public helper methods for shape load (per IDA)
    std::int16_t SelectShapeLoad(XDBStmt* pDBStmt, STMyCharInfoEx& stInfo, PS_BROACH_SHAPE_LIST& stBroachList);

    // Public helper methods for quickslot (per IDA)
    std::int16_t LoadQuickSlotItem(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_QUICKSLOT_ITEM* pQuickSlotInfo);
    std::int16_t LoadQuickSlotCard(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_QUICKSLOT_CARD_VEC* pQuickSlotCard);

    // Public helper methods for item creation (per IDA)
    std::int16_t CheckCreateItem(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::uint8_t byInvenType, std::int16_t shSlotPos, std::uint32_t dwItemID, std::int64_t biSerial, std::uint8_t byFlag, std::int64_t biItemSerial = 0);
    std::int16_t CreateItem(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::uint8_t byInvenType, std::int16_t shSlotPos, STItem* stItem, int* nErrorCode);

    // Public helper methods for item position update (per IDA)
    std::int16_t UpdateItemPos(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSerial, std::uint8_t byInvenType, std::int16_t shSlotPos);

    // Public helper methods for make limit (per IDA)
    std::int16_t UpdateItemMakeLimit(XDBStmt* pDBStmt, PS_DB_ITEM_MAKE_LIMIT_UPDATE* psUpdate);

    // Public helper methods for quickslot card (per IDA)
    bool UpdateQuickSlotCard(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_QUICKSLOT_UPDATE_CARD* psCard);

    // Public helper methods for item use info (per IDA)
    std::int16_t UseItemInfoSelect(XDBStmt* pDBStmt, std::uint32_t dwActorID, ST_USE_ITEM_INFO_LIST* stUseItemInfoList);
    std::int16_t UseItemInfoUpdate(XDBStmt* pDBStmt, std::uint32_t dwActorID, ST_USE_ITEM_INFO* stUseItemInfo);

    // Public helper methods for item move (per IDA)
    std::int16_t MoveItemEx(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int64_t biSrcSerial,
                            std::uint8_t byInvenType, std::int16_t shSlotPos,
                            std::uint8_t byBindType, std::int64_t biCashDate, std::uint8_t byStoreType);

private:
    // SubCmd handlers (52 handlers)
    std::int32_t ReqItemInventoryInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemMove(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemCombine(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemDivide(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemBreak(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemLoadQuickSlot(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemUpdateQuickSlot(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemCreate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemAddSlot(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemMoveMoney(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemBankInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemLineUp(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemReduce(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemUse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemMazeRewardItem(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemDelete(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemEquipSlotOpen(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int16_t EquipSlotOpen(XDBStmt* pDBStmt, std::uint32_t dwActorID, int nEquipPosBit);
    std::int32_t ReqItemUseInfoSelect(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemUseInfoUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemMoveEx(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemAppearanceLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemAppearanceUse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemAppearanceEnd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemAppearanceEquip(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemNameChange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemSocketLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemBroachLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemUseAkashicRecord(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLoadAkashicRecord(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAkashicRegisterAll(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAkashicReset(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemUpdateEx(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemPostRestoreCreate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemLimitUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemLimitLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemDeleteReserveAdd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemDeleteReserveDel(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemDeleteReserveLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemCooltimeLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemCooltimeUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemUseCountUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemNetCafeCostume(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemUpdatePos(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemMakeLimitLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemMakeLimitDelete(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemResealPackageInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemResealPackage(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemUseResealPackage(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqQuickslotCardDeckUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqQuickSlotCardDeckOpen(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
};

// XSQLShopProcess
// MainCmd = 0x22
class XSQLShopProcess : public TXSQLProcessBase<0x22> {
public:
    XSQLShopProcess();
    virtual ~XSQLShopProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

private:
    // SubCmd handlers (18 handlers)
    std::int32_t ReqItemBuy(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemSell(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemRepurchaser(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemDeleteRepurchase(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqNpcCreditGradeLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqNpcCreditGradeUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqNpcCreditGradeUpdateList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqShopItemLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqShopItemUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqShopCashItemBuy(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqShopCashItemSetLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqShopCashItemSet(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqShopCashItemSetDel(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqShopCashItemGift(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqShopCashItemGiftCheck(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCashItemBuyCountLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCashItemBuyCountUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqShopAccountItemUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

private:
    // Helper methods
    std::int32_t ItemBuy(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_RES_STORAGE_INFO& vecCreateItem, PS_RES_STORAGE_INFO& vecUpdateItem, std::uint8_t byFlag);
    std::int32_t CashBuyCount(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_CASH_BUY_COUNT& psCashBuyCount);
    std::int32_t CashBuyCountAccount(XDBStmt* pDBStmt, std::uint32_t dwUAID, PS_CASH_BUY_COUNT& psCashBuyCount);
    std::int32_t SelectShopItemLoad(XDBStmt* pDBStmt, std::uint32_t dwUCID, ST_SHOP_ITEM_LIST& stShopItemList);
    std::int32_t SelectShopAccountItemLoad(XDBStmt* pDBStmt, std::uint32_t dwUAID, ST_SHOP_ITEM_LIST& stShopAccountItemList);
    std::int32_t SelectCharacterUAID(XDBStmt* pDBStmt, std::uint32_t& dwUAID, std::uint32_t dwUCID, wchar_t* szName);
};

// XSQLTradeProcess
// MainCmd = 0x23
class XSQLTradeProcess : public TXSQLProcessBase<0x23> {
public:
    XSQLTradeProcess();
    virtual ~XSQLTradeProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

private:
    // SubCmd handlers (3 handlers)
    std::int32_t ReqTradeConfirm(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPrivateShopStart(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqPrivateShopBuy(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
};

// XSQLItemUpgradeProcess
// MainCmd = 0x24
class XSQLItemUpgradeProcess : public TXSQLProcessBase<0x24> {
public:
    XSQLItemUpgradeProcess();
    virtual ~XSQLItemUpgradeProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

private:
    // SubCmd handlers (2 handlers - ReqItemAddOptionResult handles both SubCmd 2 and 3)
    std::int32_t ReqItemUpgradeResult(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemAddOptionResult(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
};

// XSQLMyRoomProcess
// MainCmd = 0x25
class XSQLMyRoomProcess : public TXSQLProcessBase<0x25> {
public:
    XSQLMyRoomProcess();
    virtual ~XSQLMyRoomProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

    // Public helper methods for MyRoom items (per IDA)
    std::int16_t MyRoomItemAdd(XDBStmt* pDBStmt, std::uint32_t dwUAID, ST_MYROOM_ITEM& stInfo);
    std::int16_t MyRoomItemUpdate(XDBStmt* pDBStmt, ST_MYROOM_ITEM& stInfo);
    std::int16_t MyRoomItemDel(XDBStmt* pDBStmt, std::uint32_t dwUAID, std::uint32_t dwActorID, PS_STORAGE_INFO& stSelectItem);

private:
    // SubCmd handlers (28 handlers)
    std::int32_t ReqMyRoomCreate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyRoomCheck(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyRoomItemLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyRoomFurnitureLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyRoomIndexSelect(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqmyRoomFurnitureEdit(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyRoomItemAdd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyRoomItemDel(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyRoomSetup(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyRoomPollenAdd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyRoomPollenCultivation(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyRoomPollenHarvest(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyRoomPollenItemUse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyRoomPollenHelp(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMYRoomPollenCancel(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyRoomPollenLook(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyRoomPollenCheat(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyroomRecommend(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyroomFavorite(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyroomBoardList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyroomBoardWrite(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyroomCheckUAID(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyroomRankReward(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyroomUpdateData(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyroomInitData(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // MyRoom Favorite 辅助函数
    std::int16_t RegistMyroomFavorite(XDBStmt* pDBStmt, std::uint32_t dwUAID, ST_MYROOM_FAVORITE_INFO& stFavoriteInfo, int& nErrorCode);
    std::int16_t DeleteMyroomFavorite(XDBStmt* pDBStmt, std::uint32_t dwUAID, std::uint32_t dwOwnerUAID, int& nErrorCode);

    // MyRoom Rank 辅助函数
    std::int16_t MyroomRecommendLoad(XDBStmt* pDBStmt, std::uint32_t dwOwnerUAID, PS_MYROOM_RECOMMEND_LIST& psRecommendList);
    std::int16_t MyroomCurrentRankLoad(XDBStmt* pDBStmt, std::uint32_t dwOwnerUAID, PS_MYROOM_RANK_LIST& psRankList, PS_MYROOM_RANK_INFO& psRankInfo);
    std::int16_t MyroomPastRankLoad(XDBStmt* pDBStmt, std::uint32_t dwOwnerUAID, PS_MYROOM_RANK_LIST& psRankList, PS_MYROOM_RANK_INFO& psRankInfo);
    std::int16_t MyroomFavoriteLoad(XDBStmt* pDBStmt, std::uint32_t dwOwnerUAID, PS_MYROOM_FAVORITE_LIST& psFavoriteList);
    std::int16_t MyroomFunitureLoad(XDBStmt* pDBStmt, std::uint32_t dwOwnerUAID, PS_MYROOM_FUNITURE_LIST& stFunitureList);
    std::int16_t MyroomPollenLoad(XDBStmt* pDBStmt, std::uint32_t dwOwnerUAID, PS_MYROOM_POLLEN_LIST& psList);

    // MyRoom Board 辅助函数
    std::int16_t LoadMyBoardInfo(XDBStmt* pDBStmt, std::uint32_t dwUAID, ST_MYROOM_BOARD_INFO& stMyBoardInfo);
    std::int16_t LoadBoardList(XDBStmt* pDBStmt, PS_REQ_MYROOM_BOARD_INFO& psReqBoardInfo, PS_MYROOM_BOARD_LIST& psBoardList);

    std::int32_t ReqMyroomFavoriteInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyroomCommunityInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
};

// XSQLHelperProcess
// MainCmd = 0x26
class XSQLHelperProcess : public TXSQLProcessBase<0x26> {
public:
    XSQLHelperProcess();
    virtual ~XSQLHelperProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

    // Public helper for loading helper items
    std::int32_t ReqHelperItem(XDBStmt* pDBStmt, std::uint32_t dwUCID, ST_HELPER_INFO& stHelper);

private:
    // SubCmd handlers (7 handlers)
    std::int32_t ReqHelperListLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqHelperAdd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqHelperSupportEquip(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqHelperSupportRelease(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqHelperEquip(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqHelperChangeOrder(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqHelperChangeAutoSummon(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
};

// XSQLExchange
// MainCmd = 0x27
class XSQLExchange : public TXSQLProcessBase<0x27> {
public:
    XSQLExchange();
    virtual ~XSQLExchange();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

private:
    // SubCmd handlers (9 handlers)
    std::int32_t ReqExchangeSearch(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqExchangePriceHistoryList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqExchangeInterestList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqExchangeInterestItem(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqExchangeSellRegister(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqExchangeItemBuy(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqExchangeItemRecall(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqExchangeMyList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqExchangeItemInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // Helper functions
    std::int16_t ExchangeInterestItem_Add(XDBStmt* pDBStmt, PS_EXCHANGE_INTEREST_ITEM_RES& psInfo);
    std::int16_t ExchangeInterestItem_Del(XDBStmt* pDBStmt, PS_EXCHANGE_INTEREST_ITEM_RES& psInfo);
    std::int16_t ExchangeSellRecall(XDBStmt* pDBStmt, PS_DB_EXCHANGE_ITEM_RECALL_REQ& psRecall, ST_POST_DATA& stPost, std::uint16_t& wPostCount);
    std::int16_t GetExchangeMyList_Sell(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_EXCHANGE_MY_LIST_RES& psMyList);
    int GetExchangeSearchCount(XDBStmt* pDBStmt, PS_EXCHANGE_SEARCH_REQ& psSearch);
    std::int16_t SelectExchangeItemSocket(XDBStmt* pDBStmt, std::vector<ST_EXCHANGE_ITEM>& vecList);
    std::int16_t SelectExchangeItemSocket(XDBStmt* pDBStmt, std::vector<ST_MY_EXCHANGE_ITEM>& vecList);
    std::int16_t SelectExchangeItemPackage(XDBStmt* pDBStmt, std::vector<ST_EXCHANGE_ITEM>& vecList);
    std::int16_t SelectExchangeItemPackage(XDBStmt* pDBStmt, std::vector<ST_MY_EXCHANGE_ITEM>& vecList);
};

// XSQLRankingProcess
// MainCmd = 0x28
class XSQLRankingProcess : public TXSQLProcessBase<0x28> {
public:
    XSQLRankingProcess();
    virtual ~XSQLRankingProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

private:
    // SubCmd handlers (11 handlers)
    std::int32_t ReqRankingList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqRankingMyInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqRankingPointUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqLastRankingReward(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t UpdateRankingOperationPoint(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t SelectRankingOperationList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqRankingReset_Cheat(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqRankingUpdateData_Cheat(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqRankingInsertDummy_Cheat(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqRankingRefresh_Cheat(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqRankingOperation_Cheat(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // Helper methods for Ranking
    std::int16_t LoadTotalRankingList(XDBStmt* pDBStmt, int xReturnSessionID, PS_DB_RANKING_LIST_REQ& psReq);
    std::int16_t LoadTimeRankingList(XDBStmt* pDBStmt, int xReturnSessionID, PS_DB_RANKING_LIST_REQ& psReq);
    std::int16_t LoadClearCountRankingList(XDBStmt* pDBStmt, int xReturnSessionID, PS_DB_RANKING_LIST_REQ& psReq);
    std::int16_t LoadMonsterKillScoreRankingList(XDBStmt* pDBStmt, int xReturnSessionID, PS_DB_RANKING_LIST_REQ& psReq);
    std::int16_t LoadTotalRankMyInfo(XDBStmt* pDBStmt, PS_DB_MY_RANKING_INFO_RES& psRes);
    std::int16_t LoadTimeRankMyInfo(XDBStmt* pDBStmt, PS_DB_MY_RANKING_INFO_RES& psRes);
    std::int16_t LoadClearCountRankMyInfo(XDBStmt* pDBStmt, PS_DB_MY_RANKING_INFO_RES& psRes);
    std::int16_t LoadMonsterKillScoreRankMyInfo(XDBStmt* pDBStmt, PS_DB_MY_RANKING_INFO_RES& psRes);
    std::int16_t UpdatePointClearTime(XDBStmt* pDBStmt, PS_DB_RANKING_POINT_UPDATE& psReq);
    std::int16_t UpdatePointClearCount(XDBStmt* pDBStmt, PS_DB_RANKING_POINT_UPDATE& psReq);
    std::int16_t UpdatePointMonsterKillScore(XDBStmt* pDBStmt, PS_DB_RANKING_POINT_UPDATE& psReq);
    std::int16_t LoadTotalRankingList_Party(XDBStmt* pDBStmt, int xReturnSessionID, PS_DB_RANKING_LIST_REQ& psReq);
    std::int16_t LoadTimeRankingList_Party(XDBStmt* pDBStmt, int xReturnSessionID, PS_DB_RANKING_LIST_REQ& psReq);
    std::int16_t LoadClearCountRankingList_Party(XDBStmt* pDBStmt, int xReturnSessionID, PS_DB_RANKING_LIST_REQ& psReq);
    std::int16_t LoadMonsterKillScoreRankingList_Party(XDBStmt* pDBStmt, int xReturnSessionID, PS_DB_RANKING_LIST_REQ& psReq);
    std::int16_t LoadTotalRankMyInfo_Party(XDBStmt* pDBStmt, PS_DB_MY_RANKING_INFO_RES& psRes);
    std::int16_t LoadTimeRankMyInfo_Party(XDBStmt* pDBStmt, PS_DB_MY_RANKING_INFO_RES& psRes);
    std::int16_t LoadClearCountRankMyInfo_Party(XDBStmt* pDBStmt, PS_DB_MY_RANKING_INFO_RES& psRes);
    std::int16_t LoadMonsterKillScoreRankMyInfo_Party(XDBStmt* pDBStmt, PS_DB_MY_RANKING_INFO_RES& psRes);
    std::int16_t UpdatePointClearTime_Party(XDBStmt* pDBStmt, PS_DB_RANKING_POINT_UPDATE& psReq);
    std::int16_t UpdatePointClearCount_Party(XDBStmt* pDBStmt, PS_DB_RANKING_POINT_UPDATE& psReq);
    std::int16_t UpdatePointMonsterKillScore_Party(XDBStmt* pDBStmt, PS_DB_RANKING_POINT_UPDATE& psReq);
};

// XSQLQuestProcess
// MainCmd = 0x41
class XSQLQuestProcess : public TXSQLProcessBase<0x41> {
public:
    XSQLQuestProcess();
    virtual ~XSQLQuestProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

private:
    // SubCmd handlers (10 handlers)
    std::int32_t LoadQuest(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqQuestUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqQuestUpdateAll(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqQuestCompleteUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqDeleteEpisode(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqQuestReset(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqQuestRepeatAdd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqQuestRepeatUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqQuestFirstDropItemUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // Helper methods for LoadQuest
    std::int16_t LoadEpisode(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_QUEST_EPISODE_MAP& psInfo);
    std::int16_t LoadQuestComplete(XDBStmt* pDBStmt, std::uint32_t dwUCID, char* pCompleteData);
    std::int16_t LoadQuestComplete(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_QUEST_COMPLETE_EPISODE& stCompleteInfo);
    std::int16_t LoadRepeatQuest(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_REPEAT_QUEST_MAP& psRepeat);
    std::int16_t LoadQuestFirstDropItem(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_QUEST_FIRST_DROP_ITEM& psDrop);
    std::int16_t LoadQuestAddLoad(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_QUEST_COMPLETE_ADD_LIST& stInfo);
    std::int16_t LoadQuestDeleteLoad(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_QUEST_COMPLETE_ADD_LIST& stInfo);
    std::int16_t UpdateEpisode(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::uint32_t dwEpisodeID, ST_QUEST_EPISODE& stInfo);
    std::int16_t DeleteEpisode(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::uint32_t dwEpisodeID, std::uint8_t byContentsType, std::uint8_t byGiveUp);
    std::int16_t UpdateQuestRepeat(XDBStmt* pDBStmt, std::uint32_t dwUCID, ST_QUEST_REPEAT_INFO& stInfo);
};

// XSQLLogGameProcess
// MainCmd = 0x42
class XSQLLogGameProcess : public TXSQLProcessBase<0x42> {
public:
    XSQLLogGameProcess();
    virtual ~XSQLLogGameProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

private:
    // SubCmd handlers (22 handlers)
    std::int32_t WriteAuthLog(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t WriteMoneyLog(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ResItemCreate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ResItemUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ResTradeStart(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ResTradeResult(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ResItemRepair(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t WriteChatLog(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t WriteStatLog(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ResAchieve(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ResPostReceipt(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t WriteCashLog(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ResAccountPostReceipt(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ResClientLog(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t WriteConnectServerLog(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ResClassEventLog(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ResSGLog(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ResSGLogCharInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ResSGLogToken(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t WriteLogText(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t WriteSystemLog(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
};

// XSQLWorldProcess
// MainCmd = 0x43
class XSQLWorldProcess : public TXSQLProcessBase<0x43> {
public:
    XSQLWorldProcess();
    virtual ~XSQLWorldProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

private:
    // SubCmd handlers (7 handlers)
    std::int32_t ReqAddChannelMap(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqDeleteChannelMap(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqWorldUpdateMazeEnterLimitCount(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqWorldClearMazeEnterLimitCount(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqWorldGFBillingReloadList(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqWorldUpdateMazeEnterLimitCountGroup(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqWorldClearMazeEnterLimitCountGroup(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
};

// XSQLSkillProcess
// MainCmd = 0x44
class XSQLSkillProcess : public TXSQLProcessBase<0x44> {
public:
    XSQLSkillProcess();
    virtual ~XSQLSkillProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

    // Public helper for ReqCharacterLoad
    bool LoadSkill(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_SKILL_LOAD& stSkillInfo);

    // Public helper for ReqCharacterCreate
    bool AddSkill(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int32_t nSkill, std::int32_t nDivergence);
    bool UpdateSkillDeck(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int32_t nLine, std::int32_t* nRow);
    std::int16_t UpdateSkillPoinit(XDBStmt* pDBStmt, std::uint32_t dwActorID, std::uint16_t wTotalPoint, std::uint16_t wSkillPoint);
    std::int16_t OpenSkillDeck(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_SKILL_DECK_PAGE& stPage);

private:
    // Internal helpers for LoadSkill
    std::int16_t LoadHaveSkill(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_SKILL_LOAD& stSkillInfo);
    std::int16_t LoadSkillDeck(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_SKILL_LOAD& stSkillInfo);
    std::int16_t LoadDeckBonus(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_SKILL_LOAD& stSkillInfo);

    // SubCmd handlers (16 handlers)
    std::int32_t ReqSkillLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqSkillLearn(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqSkillUpdatePoint(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqSkillReset(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqSkillAddDeckSlot(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqSkillUpdateDeck(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqSkillResetDeck(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqSkillDivergence(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqSkillDeckBonus(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqSkillResetTaget(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqBoosterLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqBoosterAdd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqBoosterDel(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqDeckPageActive(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqDeckPageName(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqSkillDeckPageOpen(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
};

// XSQLOptionProcess
// MainCmd = 0x45
// 注意: IDA显示此处理器DBParse直接返回-1,无实际handler
class XSQLOptionProcess : public TXSQLProcessBase<0x45> {
public:
    XSQLOptionProcess();
    virtual ~XSQLOptionProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;
};

// XSQLSoulMetryProcess
// MainCmd = 0x46
class XSQLSoulMetryProcess : public TXSQLProcessBase<0x46> {
public:
    XSQLSoulMetryProcess();
    virtual ~XSQLSoulMetryProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

private:
    // SubCmd handlers (4 handlers)
    std::int32_t LoadSoulMetry(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqSoulMetryUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqSoulMetryComplete(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqSoulMetryReset(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // Helper functions
    std::int16_t LoadSoulMetryComplete(XDBStmt* pDBStmt, std::uint32_t dwUCID, char* pCompleteData);
    std::int16_t DeleteSoulMetry(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::int32_t nSoulMetryID);
};

// XSQLGestureProcess
// MainCmd = 0x47
class XSQLGestureProcess : public TXSQLProcessBase<0x47> {
public:
    XSQLGestureProcess();
    virtual ~XSQLGestureProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

    // Public helper for other modules
    std::int32_t UpdateGesture(XDBStmt* pDBStmt, std::int32_t dwUCID, PS_GESTURE_SLOT& stGesture);

private:
    // SubCmd handlers (2 handlers)
    std::int32_t ReqGestureLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqGestureUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
};

// XSQLDailyMissionProcess
// MainCmd = 0x48
class XSQLDailyMissionProcess : public TXSQLProcessBase<0x48> {
public:
    XSQLDailyMissionProcess();
    virtual ~XSQLDailyMissionProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

private:
    // SubCmd handlers (3 handlers)
    std::int32_t LoadDailyMission(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t AddAllDailyMission(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqDailyMissionUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    // Helper functions
    std::int32_t AddDailyMission(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::map<std::uint32_t, ST_DAILY_MISSION_INFO>& mapMission);
    std::int32_t DeleteAllDailyMission(XDBStmt* pDBStmt, std::uint32_t dwUCID);
    std::int32_t UpdateDailyMission(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_DAILY_MISSION_UPDATE& psMission);
};

// XSQLEvent
// MainCmd = 0x49
class XSQLEvent : public TXSQLProcessBase<0x49> {
public:
    XSQLEvent();
    virtual ~XSQLEvent();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

private:
    // SubCmd handlers (29 handlers)
    std::int32_t ReqWorldModeLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqWorldModeUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqWorldModeEnter(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqModeMazeEventLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqModeMazeEventRankUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqClassEventLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqClassEventUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqNetCafeMissionLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqNetCafeMissionUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    void SelectNetCafeMission(XDBStmt* pDBStmt, std::uint32_t dwUAID, ST_NETCAFE_MISSION_INFO& stMission);
    std::int32_t ReqPlayTimeByDay(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqWorldEventInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqWorldEventRegister(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqWorldEventReward(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqWorldEventDailyReward(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqRouletteEventInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqRouletteEventUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int16_t UpdateRouletteEvent(PS_DB_ROULETTE_EVENT_UPDATE* psDBUpdate, XDBStmt* pDBStmt);
    std::int32_t ReqRouletteRewardLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqRouletteInit(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqWorldEventReset_Cheat(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqEventAccountPlayTimeLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqEventAccountPlayTimeUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAttendanceLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAttendanceReward(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAttendanceContinueReward(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAttendancePlayTimeUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAttendancePlayTimeReward(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAttendanceReset(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAttendanceContinueReset(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAttendancePlayTimeReset(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // Attendance helper methods
    void AttendanceCharacterLoad(XDBStmt* pDBStmt, std::uint32_t dwUAID, std::uint32_t dwUCID, PS_ATTENDANCE_INFO* psAttendance);
    void AttendanceAccountLoad(XDBStmt* pDBStmt, std::uint32_t dwUAID, std::uint32_t dwUCID, PS_ATTENDANCE_INFO* psAttendance);
    void AttendanceCharacterReset(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::uint32_t dwAttendanceID);
    void AttendanceAccountReset(XDBStmt* pDBStmt, std::uint32_t dwUAID, std::uint32_t dwAttendanceID);
    void AttendanceContinueCharacterLoad(XDBStmt* pDBStmt, std::uint32_t dwUAID, std::uint32_t dwUCID, PS_ATTENDANCE_CONTINUE* psAttendance);
    void AttendanceContinueAccountLoad(XDBStmt* pDBStmt, std::uint32_t dwUAID, std::uint32_t dwUCID, PS_ATTENDANCE_CONTINUE* psAttendance);
    void AttendancePlayTimeCharacterLoad(XDBStmt* pDBStmt, std::uint32_t dwUAID, std::uint32_t dwUCID, PS_ATTENDANCE_PLAY_TIME* psAttendance);
    void AttendancePlayTimeAccountLoad(XDBStmt* pDBStmt, std::uint32_t dwUAID, std::uint32_t dwUCID, PS_ATTENDANCE_PLAY_TIME* psAttendance);
    void AttendanceCharacterReward(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_ATTENDANCE_INFO* psAttendance, std::uint32_t* dwError);
    void AttendanceAccountReward(XDBStmt* pDBStmt, std::uint32_t dwUAID, PS_ATTENDANCE_INFO* psAttendance, std::uint32_t* dwError);
    void AttendanceContinueCharacterReward(XDBStmt* pDBStmt, std::uint32_t dwUAID, std::uint32_t dwUCID, PS_ATTENDANCE_CONTINUE* psAttendance, std::uint32_t* dwError);
    void AttendanceContinueAccountReward(XDBStmt* pDBStmt, std::uint32_t dwUAID, std::uint32_t dwUCID, PS_ATTENDANCE_CONTINUE* psAttendance, std::uint32_t* dwError);
    void AttendancePlayTimeCharacterUpdate(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_ATTENDANCE_PLAY_TIME* psPlayTime);
    void AttendancePlayTimeAccountUpdate(XDBStmt* pDBStmt, std::uint32_t dwUAID, PS_ATTENDANCE_PLAY_TIME* psPlayTime);
    void AttendanceContinueCharacterReset(XDBStmt* pDBStmt, std::uint32_t dwUCID, PS_ATTENDANCE_CONTINUE* psAttendance);
    void AttendanceContinueAccountReset(XDBStmt* pDBStmt, std::uint32_t dwUAID, PS_ATTENDANCE_CONTINUE* psAttendance);
    std::uint32_t GetAttendanceID(std::int64_t biAttendanceDate);
};

// XSQLWeeklyMissionProcess
// MainCmd = 0x4A
class XSQLWeeklyMissionProcess : public TXSQLProcessBase<0x4A> {
public:
    XSQLWeeklyMissionProcess();
    virtual ~XSQLWeeklyMissionProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

private:
    // SubCmd handlers (5 handlers)
    std::int32_t ReqWeeklyMissionLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqWeeklyMissionUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqWeeklyMissionReward(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqWeeklyMissionRewardWeek(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqWeeklyMissionReset(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // Helper functions
    std::int32_t WeeklyMissionReset(XDBStmt* pDBStmt, std::uint32_t dwUCID, std::uint8_t byGroupID);
    std::int32_t WeeklyMissionResetAccount(XDBStmt* pDBStmt, std::uint32_t dwUAID, std::uint8_t byGroupID);
    std::int32_t WeeklyMissionUpdate(XDBStmt* pDBStmt, ST_DB_WEEKLY_MISSION_UPDATE& stMission);
    std::int32_t WeeklyMissionUpdateAccount(XDBStmt* pDBStmt, ST_DB_WEEKLY_MISSION_UPDATE& stMission);
};

// XSQLItemSetupProcess
// MainCmd = 0x81
class XSQLItemSetupProcess : public TXSQLProcessBase<0x81> {
public:
    XSQLItemSetupProcess();
    virtual ~XSQLItemSetupProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

private:
    // SubCmd handlers (35 handlers)
    std::int32_t ReqItemMake(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemUpgrade(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemExchange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemDisassemble(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemSocketEquip(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemSocketActive(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemSocketDetach(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemRepair(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemRepairNpc(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemRepairEquip(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemRepairAll(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemEndurance(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemEvolution(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemAkashicMake(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAkashicDisassemble(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemUpgradeLimit(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemExpUpdate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemDisassembleEx(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemBroachEquip(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemBroachActive(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemRestore(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemBroachCompose(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemUnSeal(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemUseEffect(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemRenovate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemBroachRemove(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemRefine(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemSocketExchange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemSocketUpgrade(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemSocketExtract(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemAkashicComposeEx(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemAkashicGetInfoAdd(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemAkashicGetInfoLoad(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemDye(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemTitleChange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
};

// XSQLStatisticsProcess
// MainCmd = 0xF0 (-16)
class XSQLStatisticsProcess : public TXSQLProcessBase<0xF0> {
public:
    XSQLStatisticsProcess();
    virtual ~XSQLStatisticsProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

private:
    // SubCmd handlers (12 handlers)
    std::int32_t ReqCharacterCreate(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqCharacterSave(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItemExchange(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqAkashic(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqHelper(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqQuest(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqDailyMission(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqSkill(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyRoom(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMyRoomItem(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqItem(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqMapSave(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);

    // Helper functions
    std::int32_t Exec_Skill(XDBStmt* pDBStmt, ST_STATISTICS_SKILL* stSkill);
};

// XSQLSGNetCafeProcess
// MainCmd = 0xF2 (-14)
class XSQLSGNetCafeProcess : public TXSQLProcessBase<0xF2> {
public:
    XSQLSGNetCafeProcess();
    virtual ~XSQLSGNetCafeProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

private:
    // SubCmd handlers (1 handler)
    std::int32_t ReqCharInfo(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
};

// XSQLCommonProcess
// MainCmd = 0xF3 (-13)
class XSQLCommonProcess : public TXSQLProcessBase<0xF3> {
public:
    XSQLCommonProcess();
    virtual ~XSQLCommonProcess();

    std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) override;

private:
    // SubCmd handlers (2 handlers)
    std::int32_t ReqKRRInsert(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
    std::int32_t ReqKRRDelete(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID);
};