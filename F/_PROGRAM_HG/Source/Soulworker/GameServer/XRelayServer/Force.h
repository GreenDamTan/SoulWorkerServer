#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#undef NOMINMAX
#undef WIN32_LEAN_AND_MEAN
#endif

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <vector>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/GameServer/XRelayServer/Party.h"  // 对齐 IDA: CForce 继承自 CParty

// 前置声明 - RelayServer 不直接使用 CUser
class CUser;

struct ST_FORCE_INVITE_INFO {
    std::uint32_t dwMasterID = 0;
    std::uint8_t _pad0[4] = {};
    std::uint64_t dwLimitTime = 0;
};

class CForceMember {
public:
    CForceMember() = default;
    explicit CForceMember(const ST_FORCE_MEMBER& forceMember) : m_stForceMember(forceMember), m_uxEnterMap(forceMember.uxMapID) {}

    void Init(ST_FORCE_MEMBER& stMember);  // 对齐 IDA 0x1401C9FD0
    void SetMemberInfo(const ST_FORCE_MEMBER& forceMember);
    void SetEnterMap(UXMapID uxMapID);
    void Login();
    void Logout();
    bool GetMemberInfo(ST_FORCE_MEMBER& forceMember) const;
    void Clear();  // 对齐 IDA 0x1401CA0A0
    void SetMapID(int nMapID);  // 对齐 IDA 0x1401CA1C0
    void SetMaxHP(int nHP);  // 对齐 IDA 0x1401CA1E0

    // IDA 0x1401C51C0: IsLogin - 检查成员是否登录
    bool IsLogin() const { return m_stForceMember.bLogin; }

    // IDA: SetMember - 设置成员指针
    void SetMember(CUser* pMember) { m_pMember = pMember; }
    CUser* GetMember() const { return m_pMember; }

private:
    ST_FORCE_MEMBER m_stForceMember{};
    std::uint64_t m_dwKickOutTime = 0;
    UXMapID m_uxEnterMap{};
    CUser* m_pMember = nullptr;  // IDA: offset 0, 指向 CUser 对象
};

// 对齐 IDA 0x1400132B0: CForce 继承自 CParty（构造函数调用 CParty::Clear）
class CForce : public CParty {
public:
    CForce() = default;  // 对齐 IDA: ??0CForce@@QEAA@XZ = 调用 CParty::Clear
    explicit CForce(PS_REQ_FORCE_CREATE& stCreateForce);

    // IDA 0x1401B63E0: Create - 设置Force ID和Master ID
    void Create(std::uint32_t dwPartyID, std::uint32_t dwMasterID);

    void Clear();  // 对齐 IDA 0x1401B6460
    void AddMember(ST_FORCE_MEMBER& stForceMember);  // 对齐 IDA: 非const引用
    void SetMemberInfo(ST_FORCE_MEMBER& forceMember);  // 对齐 IDA: 非const引用
    void SetMemberEnterMap(std::uint32_t dwMemberID, UXMapID uxMapID);  // 对齐 IDA 0x140094650
    void SetMemberInfo(std::uint32_t dwMemberID, UXMapID uxMapID, int nMaxHP);
    void SetForceInfo(PS_FORCE_INFO& forceInfo);  // 对齐 IDA: 非const引用
    bool GetMemberInfo(std::uint32_t dwMemberID, ST_FORCE_MEMBER& forceMember);  // 对齐 IDA: 非const方法
    void GetForceInfo(PS_FORCE_INFO& forceInfo);

    // IDA 0x1401B7D80: 设置指定成员的Force信息
    void SetForceMemberInfo(std::uint32_t dwActorID, ST_FORCE_MEMBER& stMemberInfo);
    // IDA 0x1401B7E80: 设置成员地图ID
    void SetMemberMapID(std::uint32_t dwActorID, int nMapID, int nChannel, UXMapID uxMapID);
    // IDA 0x1401B7F50: 设置成员当前HP
    void SetMemberHP(std::uint32_t dwActorID, UXMapID uxMapID, int nHP);
    // IDA 0x1401B80B0: 设置成员最大HP
    void SetMemberMaxHP(std::uint32_t dwActorID, UXMapID uxMapID, int nMaxHP);
    // IDA 0x1401B82B0: 设置成员觉醒状态
    void SetMemberAwaken(std::uint32_t dwActorID, std::uint8_t byAwaken);
    // IDA 0x1401B8350: 设置成员头像照片
    void SetMemberProfilePhoto(std::uint32_t dwActorID, std::uint32_t dwProfilePhotoID);
    // IDA 0x1401B83F0: 发送成员更新信息
    void SendUpdateMemberInfo(std::uint32_t dwActorID);
    // IDA 0x1401BB340: 设置进入迷宫响应
    bool SetEnterMazeResponse(std::uint32_t dwAgreeActor);
    // IDA 0x1403A6CB0: 设置迷宫记录
    void SetMazeRecode(std::uint32_t dwActorID, int* pMazeRecode);
    // IDA 0x1403A6D30: 获取迷宫记录
    void GetMazeRecode(std::uint32_t dwActorID, int* pMazeRecode);

    std::uint32_t GetMasterID() const { return m_dwMasterID; }
    void SetMasterID(std::uint32_t dwMasterID) { m_dwMasterID = dwMasterID; }
    std::uint32_t GetForceID() const { return m_dwForceID; }
    std::uint8_t GetUserCount() const { return static_cast<std::uint8_t>(m_mapForceMember.size()); }
    UXMapID GetMazeID() const { return m_uxMazeID; }
    void SetMazeID(UXMapID uxMazeID) { m_uxMazeID = uxMazeID; }
    std::uint8_t GetForceType() { return m_byForceType; }  // 对齐 IDA: 非const方法，使用 m_byForceType
    // IDA 0x1403AABF0: SetForceType - 设置Force类型
    void SetForceType(std::uint8_t byForceType);
    // GreenDamTan_: IDA 中不存在，辅助方法用于支持现有调用
    void GreenDamTan_SetForceID(std::uint32_t dwForceID) { m_dwForceID = dwForceID; }
    bool ChangeMaster(std::uint32_t dwNewMasterID, bool bLeave);

    // 对齐 IDA ReqForceMatchingExit lambda10_: 遍历成员ID
    void ForEachMemberID(const std::function<void(std::uint32_t)>& callback) {
        for (const auto& pair : m_mapForceMember) {
            callback(pair.first);
        }
    }

    // IDA: ?Send@CForce@@QEAAXAEAVXSendPacket@@PEAVCUser@@@Z @ 0x1403A5E10
    // 发送数据包给所有Force成员（排除指定用户）
    void Send(XSendPacket& xSendPacket, CUser* pExceptUser);

    std::uint32_t FindNewMaster() const;
    void RemoveMember(std::uint32_t dwMemberID);
    void Kickout(std::uint32_t dwMemberID);
    void GetForceMemberList(ST_PARTY_MEMBER_LIST& stMemberList);  // 对齐 IDA: ST_PARTY_MEMBER_LIST 非const方法
    void SendNameChange(std::uint32_t dwActorID, const wchar_t* pChangeName);  // 对齐 IDA 0x140013BA0

    // IDA 0x1401B72B0: OrderPlayPoint - 排序游戏点数
    void OrderPlayPoint();
    // IDA 0x1401B76A0: MazeReward - 迷宫奖励发放
    int MazeReward(std::uint32_t dwPlayTime);
    // IDA 0x1401B7A30: ApplyReward - 应用奖励
    void ApplyReward();

private:
    std::shared_ptr<CForceMember> GetOrCreateMember(std::uint32_t dwMemberID);

    std::uint32_t m_dwForceID = 0;
    std::uint32_t m_dwMasterID = 0;
    UXMapID m_uxMazeID{};
    std::uint8_t m_byForceType = 0;
    std::map<std::uint32_t, std::shared_ptr<CForceMember>> m_mapForceMember;

    // IDA: 进入迷宫请求相关成员 (继承自CParty)
    // 简化实现：RelayServer不处理迷宫进入逻辑
    // ST_EnterMazeRequst m_stEnterMazeRequst{};
    // std::set<std::uint32_t> m_setAgreeToMazeMember;
};
