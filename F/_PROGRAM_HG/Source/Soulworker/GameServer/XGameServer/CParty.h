// CParty.h
// CParty and CPartyMember classes for GameServer
// 对齐 IDA GameServer.exe

#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMapMaze.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerParty.h"
#include <cstdint>
#include <map>
#include <set>
#include <vector>
#include <memory>

// 前置声明
class CUser;
class CMover;
class XSendPacket;
class XArea;

// ============================================================================
// CPartyMember - 队伍成员类
// IDA 确认大小: 0x90 (144 bytes)
//
// 功能: 管理单个队伍成员的状态和信息
// ============================================================================
class CPartyMember {
public:
    // === 构造函数 ===
    // IDA: ??0CPartyMember@@QEAA@XZ @ 0x1401C9F50
    CPartyMember();

    // === 析构函数 ===
    // IDA: ??1CPartyMember@@UEAA@XZ
    ~CPartyMember();

    // === Member Info Management ===

    // Init - 初始化成员信息
    // IDA: ?Init@CPartyMember@@QEAAXAEAUST_PARTY_MEMBER@@@Z
    void Init(ST_PARTY_MEMBER& stPartyMember);

    // Clear - 清除成员信息
    // IDA: ?Clear@CPartyMember@@QEAAXXZ
    void Clear();

    // Logout - 登出处理
    // IDA: ?Logout@CPartyMember@@QEAAXAEAUST_UPDATE_PARTY_MEMBER@@@Z @ 0x1401CA160
    void Logout(ST_UPDATE_PARTY_MEMBER& psUpdateMember);

    // Login - 登录处理
    // IDA: ?Login@CPartyMember@@QEAAXXZ
    void Login();

    // === Setters ===

    // SetLevel - 设置等级
    // IDA: ?SetLevel@CPartyMember@@QEAAXH@Z @ 0x1401CA200
    void SetLevel(std::uint8_t nLevel);

    // SetChannel - 设置频道
    // IDA: ?SetChannel@CPartyMember@@QEAAXH@Z @ 0x1401C53B0
    void SetChannel(int nChannel);

    // ChangeName - 更改名称
    // IDA: ?ChangeName@CPartyMember@@QEAAXPEA_W@Z @ 0x1401CA240
    void ChangeName(wchar_t* szName);

    // SetMember - 设置成员用户指针
    // IDA: ?SetMember@CPartyMember@@QEAAXPEAVCUser@@@Z @ 0x1401C5430
    void SetMember(CUser* pMember);

    // SetProfilePhoto - 设置头像
    // IDA: ?SetProfilePhoto@CPartyMember@@QEAAXK@Z
    void SetProfilePhoto(std::uint32_t dwPhotoID);

    // SetHP - 设置当前HP
    // IDA: ?SetHP@CPartyMember@@QEAAXH@Z
    void SetHP(int nHP);

    // SetMaxHP - 设置最大HP
    // IDA: ?SetMaxHP@CPartyMember@@QEAAXH@Z
    void SetMaxHP(int nMaxHP);

    // === Getters ===

    // GetRecode - 获取迷宫记录
    // IDA: ?GetRecode@CPartyMember@@QEAAXPEAH@Z @ 0x1401C53D0
    void GetRecode(int* pMazeRecode) const;

    // SetRecode - 设置迷宫记录
    // IDA: ?SetRecode@CPartyMember@@QEAAXPEAH@Z @ 0x1403AC5E0
    void SetRecode(const int* pMazeRecode);

    // ClearRecode - 清除迷宫记录
    // IDA: ?ClearRecode@CPartyMember@@QEAAXXZ @ 0x1403B0240
    void ClearRecode();

    // GetMember - 获取成员用户指针
    CUser* GetMember() const { return m_pMember; }

    // GetMemberInfo - 获取成员信息
    const ST_PARTY_MEMBER& GetMemberInfo() const { return m_partyMemberInfo; }

    // GetMemberID - 获取成员ID
    std::uint32_t GetMemberID() const { return m_partyMemberInfo.dwMemberID; }

    // IsLogin - 检查是否在线
    bool IsLogin() const { return m_partyMemberInfo.bLogin != 0; }

    // SetPartyMemberState - 设置成员状态
    // IDA 0x14025DED0: m_nPartyMemberState = nState（被 CForce::SetForceType 内联调用，
    // IDA 误标为 CFsmTransition::SetOutputState）
    void SetPartyMemberState(int nState) { m_nPartyMemberState = nState; }

    // GetPartyMemberState - 读取成员状态（CForceMember::IsReadyToMaze == 2 判定用）
    int GetPartyMemberState() const { return m_nPartyMemberState; }

private:
    // === IDA 确认的成员变量 ===
    // offset 0x00: m_partyMemberInfo (88 bytes - ST_PARTY_MEMBER)
    ST_PARTY_MEMBER m_partyMemberInfo;

    // offset 0x58: m_nPartyID (4 bytes)
    int m_nPartyID = 0;

    // offset 0x5C: m_nPartyMemberState (4 bytes)
    int m_nPartyMemberState = 0;

    // offset 0x60: m_pMember (8 bytes - CUser pointer)
    CUser* m_pMember = nullptr;

    // offset 0x68: m_nMazeRecode (40 bytes - 10 int values)
    int m_nMazeRecode[10] = {};

    // Total: 0x90 (144 bytes)
};
static_assert(sizeof(CPartyMember) == 0x90, "CPartyMember size must be 0x90 bytes per IDA");

// ============================================================================
// ST_EnterMazeRequst - 进入迷宫请求结构
// PDB UDT 0x730ab (CForce::STEnterMazeRequst, CParty/CForce 共享):
//   stMazeInfo (PS_ENTER_MAP_REQ, +0) + dwEndTime (uint64, +88), Size 96
// ============================================================================
struct ST_EnterMazeRequst {
    PS_ENTER_MAP_REQ stMazeInfo{};
    std::uint64_t dwEndTime = 0;
};
static_assert(sizeof(ST_EnterMazeRequst) == 96,
              "ST_EnterMazeRequst size must match PDB UDT 0x730ab");

// ============================================================================
// CParty - 队伍类
// IDA 确认大小: 动态大小 (包含 std::map)
//
// 功能: 管理队伍状态、成员列表、迷宫进入等
// ============================================================================
class CParty {
public:
    // === 构造函数 ===
    // IDA: ??0CParty@@QEAA@XZ @ 0x1403A4A80
    CParty();

    // === 析构函数 ===
    // IDA: ??1CParty@@UEAA@XZ @ 0x1403A4BB0
    virtual ~CParty();

    // === Party Management ===

    // Create - 创建队伍
    // IDA: ?Create@CParty@@QEAAXKAEAUST_PARTY_MEMBER@@@Z @ 0x1403A4C10
    void Create(std::uint32_t dwPartyID, ST_PARTY_MEMBER& psMasterInfo);

    // Clear - 清除队伍
    // IDA: ?Clear@CParty@@QEAAXXZ @ 0x1403A4DB0
    void Clear();

    // AddMember - 添加成员
    // IDA: ?AddMember@CParty@@QEAA_NAEAUST_PARTY_MEMBER@@PEAVCUser@@@Z @ 0x1403A4EF0
    bool AddMember(ST_PARTY_MEMBER& stPartyMember, CUser* pMember);

    // GetMember - 获取成员
    // IDA: ?GetMember@CParty@@QEAAPEAVCPartyMember@@K@Z @ 0x1403A5050
    CPartyMember* GetMember(std::uint32_t dwActorID);

    // IsMember - 检查成员是否在队伍中
    // IDA: ?IsMember@CParty@@QEAA_NK@Z @ 0x1401B8640
    bool IsMember(std::uint32_t dwActorID) const;

    // GetUserCount - 获取成员数量
    // IDA: ?GetUserCount@CParty@@QEAAAEXZ
    std::uint8_t GetUserCount() const;

    // 供 CGocForce/CGocParty 遍历成员（原代码通过 friend 或内部访问 m_mapPartyMember）
    const std::map<std::uint32_t, CPartyMember*>& GetMemberMap() const { return m_mapPartyMember; }

    // GetPartyInfo - 填充队伍信息
    // IDA: ?GetPartyInfo@CParty@@QEAAXAEAUPS_PARTY_INFO@@@Z @ 0x1401B9740
    void GetPartyInfo(PS_PARTY_INFO& stPartyInfo) const;

    // CheckPassiveSkill - 检查队伍成员的被动技能
    // IDA: ?CheckPassiveSkill@CParty@@QEAAXPEAVCUser@@EE@Z @ 0x1401BBE00
    void CheckPassiveSkill(CUser* pOwner, std::uint8_t byTargetType, std::uint8_t byCondition);

    // === Member Info Update ===

    // UpdateMemberInfo - 更新成员信息
    // IDA: ?UpdateMemberInfo@CParty@@QEAAXAEAUST_UPDATE_PARTY_MEMBER@@@Z @ 0x1403A50C0
    void UpdateMemberInfo(ST_UPDATE_PARTY_MEMBER& stPartyMember);

    // UpdateMemberLevelOrder - 更新成员等级顺序
    // IDA: ?UpdateMemberLevelOrder@CParty@@QEAAXXZ @ 0x1403A5150
    void UpdateMemberLevelOrder();

    // UpdatePartyBooster - 更新队伍增益
    // IDA: ?UpdatePartyBooster@CParty@@QEAAXK@Z @ 0x1403A56D0
    void UpdatePartyBooster(std::uint32_t dwLeaveMemberID = 0);

    // ChangeMonsterLevelStat - 改变怪物等级统计
    // IDA: ?ChangeMonsterLevelStat@CParty@@QEAAX_N@Z @ 0x1403A5B20
    void ChangeMonsterLevelStat(bool bDeleteParty);

    // === Send Functions ===

    // SendPartyInfo - 发送队伍信息
    // IDA: ?SendPartyInfo@CParty@@QEAAXPEAVCUser@@E@Z @ 0x1403A5C60
    void SendPartyInfo(CUser* pMember, std::uint8_t updateType);

    // Send - 发送数据包给所有成员
    // IDA: ?Send@CParty@@QEAAXAEAVXSendPacket@@K@Z @ 0x1403A5FB0
    void Send(XSendPacket& xSendPacket, std::uint32_t dwExceptID = 0);

    // SendToLocal - 发送数据包给本地成员
    // IDA: ?SendToLocal@CParty@@QEAAXAEAVXSendPacket@@TUXMapID@@K@Z @ 0x1403A6110
    void SendToLocal(XSendPacket& xSendPacket, UXMapID uxMapID, std::uint32_t dwExceptID = 0);

    // SendUpdateMemberInfo - 发送成员更新信息
    // IDA: ?SendUpdateMemberInfo@CParty@@QEAAXK@Z @ 0x1403A7320
    void SendUpdateMemberInfo(std::uint32_t dwActorID);

    // SendPartyUpdateMemberInfo - 发送队伍成员更新信息
    // IDA: ?SendPartyUpdateMemberInfo@CParty@@QEAAXPEAVCUser@@@Z @ 0x1403A8720
    void SendPartyUpdateMemberInfo(CUser* pUser);

    // ShowPartyInfo - 显示队伍信息
    // IDA: ?ShowPartyInfo@CParty@@QEAAXXZ @ 0x1403A7BE0
    void ShowPartyInfo();

    // === Member State Setters ===

    // SetMemberMapID - 设置成员地图ID
    // IDA: ?SetMemberMapID@CParty@@QEAAXKHHTUXMapID@@@Z @ 0x1403A6DB0
    void SetMemberMapID(std::uint32_t dwActorID, int nMapID, int nChannel, UXMapID uxMapID);

    // SetMemberHP - 设置成员HP
    // IDA: ?SetMemberHP@CParty@@QEAAXKTUXMapID@@H@Z @ 0x1403A6E80
    void SetMemberHP(std::uint32_t dwActorID, UXMapID uxMapID, int nHP);

    // SetMemberMaxHP - 设置成员最大HP
    // IDA: ?SetMemberMaxHP@CParty@@QEAAXKTUXMapID@@H@Z @ 0x1403A6FE0
    void SetMemberMaxHP(std::uint32_t dwActorID, UXMapID uxMapID, int nMaxHP);

    // SetMemberLevel - 设置成员等级
    // IDA: ?SetMemberLevel@CParty@@QEAAXKH@Z @ 0x1403A7140
    void SetMemberLevel(std::uint32_t dwActorID, int nLevel);

    // SetMemberAwaken - 设置成员觉醒状态
    // IDA: ?SetMemberAwaken@CParty@@QEAAXKE@Z @ 0x1403A71E0
    void SetMemberAwaken(std::uint32_t dwActorID, std::uint8_t byAwaken);

    // SetMemberProfilePhoto - 设置成员头像
    // IDA: ?SetMemberProfilePhoto@CParty@@QEAAXKK@Z @ 0x1403A7280
    void SetMemberProfilePhoto(std::uint32_t dwActorID, std::uint32_t dwPhotoID);

    // === Party Operations ===

    // DropItem - 掉落物品
    // IDA: ?DropItem@CParty@@QEAAXHHAEAUXVec3@@_NHH@Z @ 0x1403A7460
    void DropItem(int nDropID, int nMonsterLv, struct XVec3* vPos, bool bBoss, int nMonsterID, int nMapID);

    // UserKickOut - 踢出用户
    // IDA: ?UserKickOut@CParty@@QEAAHK@Z @ 0x1403A7D90
    std::uint32_t UserKickOut(std::uint32_t dwMember);

    // ChangeMaster - 更换队长
    // IDA: ?ChangeMaster@CForce@@QEAA_NK@Z @ 0x1403A5580 (CParty 与 CForce 同布局共享实现)
    bool ChangeMaster(std::uint32_t dwPartyMaster);

    // Enumerate - 枚举全部成员 ID
    // IDA: ?Enumerate@CForce@@QEAAXAEAV?$vector@KV?$allocator@K@std@@@std@@@Z @ 0x1401B8C80
    void Enumerate(std::vector<std::uint32_t>& vecMember);

    // SyncMemberHP - 同步成员 HP（SetMaxHP + SetHP）
    // IDA: ?SyncMemberHP@CParty@@QEAAXHHH@Z @ 0x1403ABEE0
    void SyncMemberHP(std::uint32_t nMemberID, int nMaxHP, int nHP);

    // RegisterPartyMember - 注册成员用户指针（SetMember）
    // IDA: ?RegisterPartyMember@CParty@@QEAAXKPEAVCUser@@@Z @ 0x1401B7E00
    void RegisterPartyMember(std::uint32_t dwActorID, CUser* pMember);

    // RemoveForceBooster - 移除所有本线程成员的队伍增益
    // IDA: ?RemoveForceBooster@CForce@@QEAAXXZ @ 0x1403A5600（CParty/CForce 同 272 布局）
    void RemoveForceBooster();

    // SetForceType - 设置类型；type==1 时对所有成员设置成员状态 2
    // IDA: ?SetForceType@CForce@@QEAAXE@Z @ 0x1403AABF0
    void SetForceType(std::uint8_t byType);

    // === Maze Functions ===

    // ResEnterMaze - 进入迷宫响应
    // IDA: ?ResEnterMaze@CParty@@QEAAXPEAVCUser@@AEAUPS_ENTER_MAP_RES@@@Z @ 0x1403A7E90
    void ResEnterMaze(CUser* pUser, struct PS_ENTER_MAP_RES* stEnterMap);

    // SendEnterMaze - 发送进入迷宫
    // IDA: ?SendEnterMaze@CParty@@QEAAXPEAVCUser@@AEAUPS_ENTER_MAP_RES@@@Z @ 0x1403A8090
    void SendEnterMaze(CUser* pUser, struct PS_ENTER_MAP_RES* stEnterMapRes);

    // EnterMaze - 进入迷宫
    // IDA: ?EnterMaze@CParty@@QEAA_NPEAVCUser@@TUXMapID@@AEAUPS_ENTER_MAP_REQ@@@Z @ 0x1403A8940
    bool EnterMaze(CUser* pReqUser, UXMapID uxMazeID, struct PS_ENTER_MAP_REQ* stEnterMap);

    // EnterMazeByForce - 强制进入迷宫 (ReqWorldEnterByForce lambda14 调用链)
    // IDA: ?EnterMazeByForce@CParty@@QEAA_NPEAVCUser@@TUXMapID@@AEAUPS_ENTER_MAP_REQ@@@Z @ 0x1403AAED0
    bool EnterMazeByForce(CUser* pReqUser, UXMapID uxMazeID, struct PS_ENTER_MAP_REQ* stEnterMap);

    // === Reward Functions ===

    // OrderPlayPoint - 排序游戏点数
    // IDA: ?OrderPlayPoint@CParty@@QEAAXXZ @ 0x1403A6250
    void OrderPlayPoint();

    // MazeReward - 迷宫奖励
    // IDA: ?MazeReward@CParty@@QEAAHK@Z @ 0x1403A6600
    int MazeReward(std::uint32_t dwPlayTime);

    // ApplyReward - 应用奖励
    // IDA: ?ApplyReward@CParty@@QEAAXXZ @ 0x1403A6990
    void ApplyReward();

    // SendMazeClear - 发送迷宫清除
    // IDA: ?SendMazeClear@CParty@@QEAAXH@Z
    void SendMazeClear(int nParam);

    // === Getters ===

    // GetPartyID - 获取队伍ID
    std::uint32_t GetPartyID() const { return m_dwPartyID; }

    // GetMasterID - 获取队长ID
    std::uint32_t GetMasterID() const { return m_dwMasterID; }

    // GetMazeID - 获取迷宫ID
    // IDA: ?GetMazeID@CParty@@QEAA?ATUXMapID@@XZ @ 0x1402F69D0
    UXMapID GetMazeID() const;

    // GetMinLevel - 获取最小等级
    int GetMinLevel() const { return m_nMinLevel; }

    // GetMaxLevel - 获取最大等级
    int GetMaxLevel() const { return m_nMaxLevel; }

    // GetPartyType - 获取队伍类型
    std::uint8_t GetPartyType() const { return m_byPartyType; }

    // IsLoad - 检查是否已加载
    bool IsLoad() const { return m_bLoad != 0; }

    // === Setters ===

    // SetPartyID - 设置队伍ID
    void SetPartyID(std::uint32_t dwPartyID) { m_dwPartyID = dwPartyID; }

    // SetMasterID - 设置队长ID
    void SetMasterID(std::uint32_t dwMasterID) { m_dwMasterID = dwMasterID; }

    // SetMazeID - 设置迷宫ID
    void SetMazeID(const UXMapID& uxMazeID) { m_uxMazeID = uxMazeID; }

    // SetPartyType - 设置队伍类型
    void SetPartyType(std::uint8_t byPartyType) { m_byPartyType = byPartyType; }

    // SetLoad - 设置加载标志
    void SetLoad(bool bLoad) { m_bLoad = bLoad ? 1 : 0; }

    // SetEnterMazeRequst - 设置进入迷宫请求
    // IDA: ?SetEnterMazeRequst@CParty@@QEAAXAEAUPS_ENTER_MAP_REQ@@@Z @ 0x1401B9EF0
    //      (CParty/CForce 共享同一 COMDAT 体)
    void SetEnterMazeRequst(PS_ENTER_MAP_REQ& stEnterMap);

    // GetEnterMazeRequest - 获取进行中的进迷宫请求
    // IDA: ?GetEnterMazeRequest@CParty@@QEAAPEAUSTEnterMazeRequst@1@XZ
    //      (publics RVA 0x2E250; CParty/CForce 共享 COMDAT；
    //       ReqWorldEnterByForce lambda14 以 stMazeInfo.wMapID (+16) 读取)
    ST_EnterMazeRequst* GetEnterMazeRequest() { return &m_stEnterMazeRequst; }

    // SetEnterMazeResponse - 登记进入迷宫响应，全员同意后广播
    // IDA: ?SetEnterMazeResponse@CParty@@QEAA_NK@Z @ 0x1403AA360
    bool SetEnterMazeResponse(std::uint32_t dwAgreeActor);

    // CreateMazeReq - 构造 ST_CREATE_MAZE 并经控制服转发创建请求
    // IDA: ?CreateMazeReq@CParty@@QEAAXXZ @ 0x1403AAD60
    void CreateMazeReq();

    // AgreeEnterMaze - 广播同意进入迷宫 (0x11, 0x4A) 给全部已准备成员
    // IDA: ?AgreeEnterMaze@CParty@@QEAAXK@Z @ 0x1403AA1F0
    void AgreeEnterMaze(std::uint32_t dwAgreeActor);

protected:
    // === IDA 确认的成员变量 ===
    // offset 0x00: vtable (8 bytes)
    // offset 0x08: m_dwPartyID (4 bytes)
    std::uint32_t m_dwPartyID = 0;

    // offset 0x0C: m_dwMasterID (4 bytes)
    std::uint32_t m_dwMasterID = 0;

    // offset 0x10: m_uxMazeID (8 bytes)
    UXMapID m_uxMazeID{};

    // offset 0x18: m_mapPartyMember (std::map<unsigned long, CPartyMember*>)
    std::map<std::uint32_t, CPartyMember*> m_mapPartyMember;

    // offset after map: m_bLoad (1 byte)
    std::uint8_t m_bLoad = 0;

    // offset: m_stEnterMazeRequst
    ST_EnterMazeRequst m_stEnterMazeRequst{};

    // offset: m_vecReadyToMazeMember
    std::vector<ST_ENTER_MAZE_MEMBER_INFO> m_vecReadyToMazeMember;

    // offset: m_setAgreeToMazeMember
    std::set<std::uint32_t> m_setAgreeToMazeMember;

    // offset: m_stNextMovePos
    STPosInfo m_stNextMovePos{};

    // offset: m_nMaxLevel (4 bytes)
    int m_nMaxLevel = 0;

    // offset: m_nMinLevel (4 bytes)
    int m_nMinLevel = 0;

    // offset: m_byPartyType (1 byte)
    std::uint8_t m_byPartyType = 0;
};
