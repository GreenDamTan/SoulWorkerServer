#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"

struct ST_FORCE_INVITE_INFO {
    std::uint32_t dwMasterID = 0;
    std::uint8_t _pad0[4] = {};
    std::uint64_t dwLimitTime = 0;
};

class CForceMember {
public:
    CForceMember() = default;
    explicit CForceMember(const ST_FORCE_MEMBER& forceMember) : m_stForceMember(forceMember), m_uxEnterMap(forceMember.uxMapID) {}

    void SetMemberInfo(const ST_FORCE_MEMBER& forceMember);
    void SetEnterMap(UXMapID uxMapID);
    void Login();
    void Logout();
    bool GetMemberInfo(ST_FORCE_MEMBER& forceMember) const;

private:
    ST_FORCE_MEMBER m_stForceMember{};
    std::uint64_t m_dwKickOutTime = 0;
    UXMapID m_uxEnterMap{};
};

class CForce {
public:
    CForce() = default;  // 对齐 IDA: ??0CForce@@QEAA@XZ = 无参数默认构造
    explicit CForce(PS_REQ_FORCE_CREATE& stCreateForce);

    void AddMember(ST_FORCE_MEMBER& stForceMember);  // 对齐 IDA: 非const引用
    void SetMemberInfo(ST_FORCE_MEMBER& forceMember);  // 对齐 IDA: 非const引用
    void SetMemberEnterMap(std::uint32_t dwMemberID, UXMapID uxMapID);  // 对齐 IDA 0x140094650
    void SetMemberInfo(std::uint32_t dwMemberID, UXMapID uxMapID, int nMaxHP);
    void SetForceInfo(PS_FORCE_INFO& forceInfo);  // 对齐 IDA: 非const引用
    bool GetMemberInfo(std::uint32_t dwMemberID, ST_FORCE_MEMBER& forceMember);  // 对齐 IDA: 非const方法
    void GetForceInfo(PS_FORCE_INFO& forceInfo);
    std::uint32_t GetMasterID() const { return m_dwMasterID; }
    void SetMasterID(std::uint32_t dwMasterID) { m_dwMasterID = dwMasterID; }
    std::uint32_t GetForceID() const { return m_dwForceID; }
    std::uint8_t GetUserCount() const { return static_cast<std::uint8_t>(m_mapForceMember.size()); }
    UXMapID GetMazeID() const { return m_uxMazeID; }
    void SetMazeID(UXMapID uxMazeID) { m_uxMazeID = uxMazeID; }
    std::uint8_t GetForceType() { return m_byForceType; }  // 对齐 IDA: 非const方法
    void SetForceType(std::uint8_t byForceType) { m_byForceType = byForceType; }
    // GreenDamTan_: IDA 中不存在，辅助方法用于支持现有调用
    void GreenDamTan_SetForceID(std::uint32_t dwForceID) { m_dwForceID = dwForceID; }
    bool ChangeMaster(std::uint32_t dwNewMasterID, bool bLeave);
    std::uint32_t FindNewMaster() const;
    void RemoveMember(std::uint32_t dwMemberID);
    void Kickout(std::uint32_t dwMemberID);
    void GetForceMemberList(ST_PARTY_MEMBER_LIST& stMemberList);  // 对齐 IDA: ST_PARTY_MEMBER_LIST 非const方法
    void SendNameChange(std::uint32_t dwActorID, const wchar_t* pChangeName);  // 对齐 IDA 0x140013BA0

private:
    std::shared_ptr<CForceMember> GetOrCreateMember(std::uint32_t dwMemberID);

    std::uint32_t m_dwForceID = 0;
    std::uint32_t m_dwMasterID = 0;
    UXMapID m_uxMazeID{};
    std::uint8_t m_byForceType = 0;
    std::map<std::uint32_t, std::shared_ptr<CForceMember>> m_mapForceMember;
};
