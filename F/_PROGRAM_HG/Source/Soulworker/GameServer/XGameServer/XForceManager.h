// XForceManager.h
// Force/Guild Manager - Manages all Force (Guild) instances
// 对齐 IDA GameServer.exe

#pragma once

#include <cstdint>
#include <memory>
#include <map>

// 前置声明
class CForce;
class CUser;
class XActor;
struct PS_REQ_FORCE_CREATE;
struct PS_FORCE_LEAVE;
struct PS_FORCE_ADDMEMBER;
struct PS_FORCE_INFO;
struct UST_UPDATE_FORCE_MEMBER;
struct PS_ENTER_MAP_RES;
union UXActorID;
union UXMapID;

// ============================================================================
// XForceManager - Force/Guild Manager (Singleton)
// IDA确认的管理器类，负责管理所有Force实例
//
// Force = Guild (公会系统)
// 在SoulWorker中，Force即为Guild
// ============================================================================

class XForceManager {
public:
    // 单例访问
    static XForceManager* Instance();

    // === Force Creation & Deletion ===

    // CreateForce - 创建公会 (IDA: 0x1401C45C0)
    // 参数: PS_REQ_FORCE_CREATE 结构体
    // 返回: ForceID (失败返回0)
    std::uint32_t CreateForce(PS_REQ_FORCE_CREATE& stReq);

    // CreateForce - 创建公会 (IDA: 0x1401C47A0)
    // 参数: pUser 创建者, ForceID
    // 返回: 是否成功
    bool CreateForce(CUser* pUser, std::uint32_t dwForceID);

    // DeleteForce - 删除公会 (IDA: 0x1401C5ED0)
    // 参数: PS_FORCE_LEAVE 结构体
    void DeleteForce(PS_FORCE_LEAVE& stLeave);

    // === Force Membership ===

    // LeaveForce - 离开公会 (IDA: 0x1401C5A20)
    // 参数: PS_FORCE_LEAVE 结构体, ForceID
    // 返回: 是否成功
    bool LeaveForce(PS_FORCE_LEAVE& stLeave, std::uint32_t dwForceID);

    // AddMember - 添加成员 (IDA: 0x1401C5630)
    // 参数: PS_FORCE_ADDMEMBER 结构体
    void AddMember(PS_FORCE_ADDMEMBER& stAddMember);

    // AddMember - 添加成员 (IDA: 0x1401C51C0)
    // 参数: pUser, PS_FORCE_ADDMEMBER, PS_FORCE_INFO
    // 返回: 是否成功
    bool AddMember(CUser* pUser, PS_FORCE_ADDMEMBER& stAddMember, PS_FORCE_INFO& stForceInfo);

    // === Force Query ===

    // GetForce - 获取Force实例 (IDA: 0x1401C6460)
    // 参数: ForceID
    // 返回: shared_ptr<CForce>
    std::shared_ptr<CForce> GetForce(std::uint32_t dwForceID);

    // GetForce - 通过ActorID获取Force实例 (IDA: 0x1401C6510)
    // 参数: UXActorID
    // 返回: shared_ptr<CForce>
    std::shared_ptr<CForce> GetForce(const UXActorID& uxActorID);

    // GetForceID - 通过ActorID获取ForceID (IDA: 0x1401C63F0)
    // 参数: UXActorID
    // 返回: ForceID
    std::uint32_t GetForceID(const UXActorID& uxActorID);

    // IsForceUser - 检查用户是否在公会中 (IDA: 0x1401C65E0)
    // 参数: UXActorID
    // 返回: bool
    bool IsForceUser(const UXActorID& uxActorID);

    // IsMasterUser - 检查用户是否是队长 (IDA: 0x1401C6650)
    // 参数: UXActorID
    // 返回: bool
    bool IsMasterUser(const UXActorID& uxActorID);

    // GetUserCount - 获取公会成员数量 (IDA: 0x1401C6560)
    // 参数: ForceID
    // 返回: 成员数量
    std::uint32_t GetUserCount(std::uint32_t dwForceID);

    // === Force Management ===

    // ChangeMaster - 更改队长 (IDA: 0x1401C5860)
    // 参数: 新队长ActorID
    // 返回: 是否成功
    bool ChangeMaster(const UXActorID& uxNewMasterID);

    // ChangeMaster - 按 ForceID 更改队长 (IDA: 0x1401C6860)
    // 参数: ForceID, 新队长ActorID
    // 返回: 是否成功
    bool ChangeMaster(std::uint32_t dwForceID, UXActorID uNewMasterActorID);

    // RegisterForce - 注册Force (IDA: 0x1401C4E60)
    // 参数: ForceID, pMasterUser
    // 返回: shared_ptr<CForce>
    std::shared_ptr<CForce> RegisterForce(std::uint32_t dwForceID, CUser* pMasterUser);

    // AddForce - 添加Force到管理器 (IDA: 0x1401C4AC0)
    // 参数: PS_FORCE_INFO
    // 返回: shared_ptr<CForce>
    std::shared_ptr<CForce> AddForce(PS_FORCE_INFO& stForceInfo);

    // UpdateMemberInfo - 更新成员信息 (IDA: 0x1401C6A70)
    // 参数: UST_UPDATE_FORCE_MEMBER
    void UpdateMemberInfo(UST_UPDATE_FORCE_MEMBER& stUpdateMember);

    // ReqForceEnterServer - 请求进入服务器 (IDA: 0x1401C6740)
    // 参数: pUser, ForceID
    void ReqForceEnterServer(CUser* pUser, std::uint32_t dwForceID);

    // ResForceUpdateInfo - 响应Force更新信息 (IDA: 0x1401C69D0)
    // 参数: ForceID, UXMapID
    void ResForceUpdateInfo(std::uint32_t dwForceID, const UXMapID& uxMapID);

    // ResEnterMaze - 响应进入迷宫 (IDA: 0x1401C68A0)
    // 参数: pUser, ForceID, PS_ENTER_MAP_RES
    void ResEnterMaze(CUser* pUser, std::uint32_t dwForceID, PS_ENTER_MAP_RES& stRes);

    // RecvForceMazeClear - 接收迷宫清除消息 (IDA: 0x1401C6BF0)
    // 参数: ForceID
    void RecvForceMazeClear(std::uint32_t dwForceID);

protected:
    XForceManager() = default;
    ~XForceManager() = default;

    // PDB UDT 0x75c5e (Size 128): m_dwMaxForceID@8, m_mapForceInfo@16,
    // m_mapForceUserInfo@48, m_ForcePool@80
    std::uint32_t m_dwMaxForceID = 0;

    // Force map: ForceID -> shared_ptr<CForce>
    std::map<std::uint32_t, std::shared_ptr<CForce>> m_mapForceInfo;

    // ActorID -> ForceID mapping for quick lookup
    std::map<std::uint32_t, std::uint32_t> m_mapForceUserInfo;

private:
    static XForceManager* s_pInstance;
};
