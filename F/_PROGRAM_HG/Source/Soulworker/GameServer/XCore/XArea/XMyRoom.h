#pragma once

#include "Soulworker/GameServer/XCore/XArea/XArea.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_MyRoomStructs.h"
#include "Soulworker/GameServer/XCore/XArea/Range2DScanner.h"
#include <cstdint>
#include <map>
#include <list>
#include <vector>

// 前置声明
class VEventObjectResource;
struct TB_MAZE_INFO;
struct TB_MYROOM_INFO;
struct ST_MYROOM_ITEM_LIST;
struct ST_MYROOM_USER;
class CMover;

// 前置声明 - 生成相关结构
struct VMonsterSpawnInfo;
enum E_SEND_INFO_TYPE;

// TODO: 推测结果 - 来自 IDA struct XMyRoom
class XMyRoom : public XArea {
public:
    XMyRoom();
    virtual ~XMyRoom();

    // IDA 0x1402AC560 - 获取世界类型 (返回 3)
    virtual int GetWorldType() override;

    // IDA 0x1402AC580 - 获取房间状态
    std::uint8_t GetMyRoomState();

    // IDA 0x1400FAB90 - 获取所有者 UAID
    std::uint32_t GetOwnerUAID();

    // IDA 0x1400FABB0 - 获取所有者信息
    void GetOwnerInfo(ST_MYROOM_OWNER_INFO& stOwnerInfo);

    // IDA 0x1400FAC00 - 检查是否是房间所有者
    bool IsMyRoomOwner(std::uint32_t dwUAID);

    // IDA 0x1402AC8A0 - 初始化
    virtual bool Init();

    // IDA 0x1402ACA30 - 清理
    virtual void Clear();

    // IDA 0x1402ACCE0 - 清除状态
    void ClearState();

    // IDA 0x1402ACD00 - 创建房间
    bool CreateRoom(void* stEnterUser, void* stOwnerInfo, TUXMapID uxMapID, std::uint32_t dwOwnerUCID);

    // IDA 0x1402ACE90 - 生成
    void SpawnGenerate();

    // IDA 0x1402AD380 - 获取生成位置
    void GetSpawnPos(const void* pEventObjectInfo, XVec3& vPos);

    // IDA 0x1402AD3F0 - 进入房间
    int EnterRoom(void* stEnterUser);

    // IDA 0x1402AD550 - 退出房间
    void ExitRoom(std::uint32_t dwUCID, std::uint32_t& dwBeforeMap);

    // IDA 0x1402AD750 - 加载房间成功
    void LoadMyRoomSucc(void* stMyRoomItemList);

    // IDA 0x1402AD8F0 - 进入Actor
    // IDA: ?EnterActor@XMyRoom@@UEAAGPEAVXActor@@@Z - returns unsigned short
    virtual std::uint16_t EnterActor(XActor* pActor) override;

    // IDA 0x1402AD040 - 执行生成盒
    void ExcuteSpawnBox(const VMonsterSpawnInfo* pMonsterSpawn, E_SEND_INFO_TYPE eType);

    // IDA 0x1402ADB E0 - 进入游戏对象
    virtual std::uint16_t EnterGameObject(XActor* pActor, E_SEND_INFO_TYPE eType);

    // IDA 0x1402ADEA0 - 退出游戏对象
    virtual std::uint16_t ExitGameObject(XActor* pActor, E_SEND_INFO_TYPE eType);

    // IDA 0x1402AE0D0 - 退出Actor
    virtual void ExitActor(XActor* pActor) override;

    // IDA 0x1402AE410 - 退出区域
    virtual void ExitArea(XActor* pActor);

    // IDA 0x1402AE6E0 - 更新
    virtual void OnUpdate(float fElapsed) override;

    // IDA 0x1402AE780 - 发送对象信息
    virtual bool SendObjectInfo(XActor* pActor);

    // IDA 0x1402AE8C0 - 发送广播
    virtual void SendBroadCast(XSendPacket& packet, XActor* pExceptActor, E_BROADCAST_TYPE eBroadCastType) override;

    // IDA 0x1402AE9C0 - 获取扫描器
    // Note: Using m_mapActor as simplified implementation (should use AREA_OBJECT)
    std::map<std::uint32_t, XActor*>* GetScanner(XActor* pActor);

    // IDA: ScanGridOrigin - inherited from XArea (base class stub)
    void ScanGridOrigin(float dx, float dy, unsigned char byNation, int sectorRange, unsigned int dwOptions, std::vector<CMover*>& vecOut) override;

    // IDA 0x1402AEA20 - 加载完成
    virtual void LoadComplete(XActor* pActor);

    // IDA 0x1402AEB50 - 发送房间加载
    void SendMyRoomLoad(class CUser* pUser);

    // IDA 0x1402AEF60 - 编辑家具
    bool EditFurniture(class CUser* pUser, void* stEditFurniture, std::uint8_t byState);

    // IDA 0x1402AF210 - 添加房间物品
    bool AddMyRoomItem(void* stItem);

    // IDA 0x1402AF2F0 - 删除房间物品
    bool DelMyRoomItem(std::int64_t biSerial, void* stItem);

    // IDA 0x1402AF490 - 添加房间使用用户
    std::uint8_t AddMyRoomUsedUser(std::uint32_t dwActorID, std::int64_t i64Serial, std::uint8_t byAniIndex);

    // IDA 0x1402AF530 - 删除房间使用用户
    bool DelMyRoomUsedUser(std::uint32_t dwActorID);

    // IDA 0x1402AF640 - 获取空索引
    std::uint8_t GetEmptyIndex(std::int64_t i64Serial);

    // IDA 0x1402AF900 - 检查是否可以更改门状态
    bool IsCanChangeDoorState(char cDoorIndex);

    // IDA 0x1402AF970 - 设置门状态
    void SetDoorState(char cDoorIndex, bool bOpen);

    // IDA 0x1402AF9C0 - 所有用户退出
    void AllUserOut(int nReason);

    // IDA 0x1402AFBA0 - 用户踢出
    void UserKickOut(std::uint32_t dwKickActorID);

    // IDA 0x1402AFD10 - 获取当前用户数
    int GetCurUserCount();

    // IDA 0x1402AFD40 - 设置房间设置
    void SetMyRoomSetup(class CUser* pOwnerUser, void* stMyRoomSetup);

    // IDA 0x1402AFE00 - 运行任务移动检查
    void RunQuestMoveCheck(int nBoxIndex, class CUser* pUser);

    // IDA 0x1402AFFE0 - 逃离Actor
    virtual bool EscapeActor(XActor* pActor);

    // IDA 0x1402B0140 - 获取出口区域ID
    virtual bool GetExitDistrictID(std::uint32_t dwActorID, std::uint16_t& wMapID, int& nJumpID, XVec3& vPos) override;

    // IDA 0x1402B0300 - 花粉加载
    void PollenLoad(void* psPollenList);

    // IDA 0x1402B0520 - 设置花粉信息
    void SetPollenInfo(void* psPollenInfo);

    // IDA 0x1402B0630 - 检查是否可以添加花粉
    bool CanPollenAdd(int nPollenIndex);

    // IDA 0x1402B06F0 - 检查是否可以培育
    std::uint32_t CanCultivation(int nPollenIndex);

    // IDA 0x1402B0D80 - 检查是否可以取消花粉
    bool CanPollenCancel(int nPollenIndex);

    // IDA 0x1402B0E10 - 获取花粉收获
    bool GetPollenHarvest(int nPollenIndex, std::uint32_t& dwItemID, std::int16_t& shCount, std::uint8_t& byResult);

    // IDA 0x1402B1100 - 清除花粉
    void ClearPollen(int nPollenIndex, std::uint8_t bySubcmd);

    // IDA 0x1402B1460 - 获取之前地图
    std::uint32_t GetBeforeMap(std::uint32_t dwActorID);

    // IDA 0x1402B1550 - 发送花粉添加
    void SendPollenAdd(int nPollenIndex);

    // IDA 0x1402B1660 - 发送花粉培育
    void SendPollenCultivation(void* psInfo);

    // IDA 0x1402B1870 - 发送花粉收获
    void SendPollenHarvest(int nPollenIndex);

    // IDA 0x1402B1930 - 发送花粉帮助
    void SendPollenHelp(int nPollenIndex, void* psHelpUser, std::int64_t biHarvestDate);

    // IDA 0x1402B1A60 - 发送花粉物品使用
    void SendPollenItemUse(void* psPollenInfo);

    // IDA 0x1402B1B50 - 发送花粉加载
    void SendPollenLoad(class CUser* pUser);

    // IDA 0x1402B0AC0 - 检查是否可以帮助用户
    std::uint32_t CanHelpUser(std::uint32_t dwHelpUCID, int nPollenIndex, std::uint8_t& byCount, std::int64_t& biHarvestDate);

    // IDA 0x1402B1250 - 添加花粉帮助用户
    void AddPollenHelpUser(int nPollenIndex, void* psHelpUser, std::int64_t biHarvestDate, void* psPollenInfo);

    // IDA 0x1402B1340 - 花粉物品使用
    void PollenItemUse(void* psPollenInfo);

    // IDA 0x1402B1D20 - 花粉使用
    void PollenUse(int nPollenIndex, std::uint8_t bySubCmd);

    // IDA 0x1402B1DB0 - 检查是否可以使用花粉
    bool CanUsePollen(int nPollenIndex);

    // IDA 0x1402B1E80 - 发送花粉取消
    void SendPollenCancel(int nPollenIndex);

    // IDA 0x1402B1F40 - 设置花粉锁定计数
    bool SetPollenLockCount(class CUser* pUser, int nPollenIndex, std::uint8_t byCount);

    // IDA 0x1402B20A0 - 作弊花粉
    void CheatPollen(class CUser* pUser, int nPollenIndex, int nDecSec);

    // IDA 0x1402B2630 - 更新推荐计数
    void UpdateRecommendCount(int nCount);

    // IDA 0x1402CBB40 - 发送运输信息
    virtual bool SendTransportationInfo(XActor* pActor);

    // IDA 0x14052A680 - 设置房间状态
    void SetMyRoomState(std::uint8_t byState);

    // IDA 0x14052A6A0 - 获取发送房间信息
    bool GetSendRoomInfo();

protected:
    // === IDA 确认的成员变量 ===

    // 对象扫描器 (用于 GetScanner 函数)
    AREA_OBJECT m_objectScanner;

    // 对象资源
    VEventObjectResource* m_pObjectResource;

    // 迷宫信息表
    TB_MAZE_INFO* m_pTBMazeInfo;

    // 我的房间信息表
    TB_MYROOM_INFO* m_pTBMyRoomInfo;

    // 房间创建标志
    bool m_bCreate;

    // 房间状态
    std::uint8_t m_byRoomState;

    // 进入用户数
    std::uint8_t m_byEnterUser;

    // 所有者信息
    ST_MYROOM_OWNER_INFO* m_stOwnerInfo;

    // 进入用户映射
    std::map<std::uint32_t, ST_MYROOM_USER> m_mpEnterUser;

    // 家具映射 (serial -> ST_MYROOM_ITEM)
    std::map<std::int64_t, ST_MYROOM_ITEM> m_mpMyRoomFurniture;

    // 我的房间物品列表
    std::vector<ST_MYROOM_ITEM> m_stMyRoomItemList;

    // 我的房间使用用户列表
    std::vector<ST_MYROOM_USED_USER> m_stMyRoomUsedUserList;

    // 任务移动盒映射
    std::map<int, void*> m_mapQuestMoveBox;

    // 花粉信息映射
    std::map<std::uint32_t, ST_POLLEN_INFO> m_mpPollenInfo;

    // 门开启状态数组 (10 个门)
    bool m_bDoorOpen[10] = {};

    // 发送我的房间信息标志
    bool m_bSendMyroomInfo;
};
