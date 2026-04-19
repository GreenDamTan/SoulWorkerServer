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
    explicit CForce(std::uint32_t dwForceID = 0) : m_dwForceID(dwForceID) {}
    explicit CForce(PS_REQ_FORCE_CREATE& stCreateForce);

    void AddMember(const ST_FORCE_MEMBER& stForceMember);
    void SetMemberInfo(const ST_FORCE_MEMBER& forceMember);
    void SetMemberInfo(std::uint32_t dwMemberID, UXMapID uxMapID, int nMaxHP);
    void SetForceInfo(const PS_FORCE_INFO& forceInfo);
    bool GetMemberInfo(std::uint32_t dwMemberID, ST_FORCE_MEMBER& forceMember) const;
    void GetForceInfo(PS_FORCE_INFO& forceInfo) const;
    std::uint32_t GetMasterID() const { return m_dwMasterID; }
    void SetMasterID(std::uint32_t dwMasterID) { m_dwMasterID = dwMasterID; }
    std::uint32_t GetForceID() const { return m_dwForceID; }
    std::uint8_t GetUserCount() const { return static_cast<std::uint8_t>(m_mapForceMember.size()); }

private:
    std::shared_ptr<CForceMember> GetOrCreateMember(std::uint32_t dwMemberID);

    std::uint32_t m_dwForceID = 0;
    std::uint32_t m_dwMasterID = 0;
    UXMapID m_uxMazeID{};
    std::uint8_t m_byForceType = 0;
    std::map<std::uint32_t, std::shared_ptr<CForceMember>> m_mapForceMember;
};
