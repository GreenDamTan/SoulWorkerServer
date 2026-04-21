#pragma once

#include <cstdint>
#include <map>
#include <memory>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"

class CPartyMember {
public:
    CPartyMember() = default;
    explicit CPartyMember(const ST_PARTY_MEMBER& partyMember) : m_stPartyMember(partyMember) {}

    std::uint32_t GetMemberID() const { return m_stPartyMember.dwMemberID; }
    void SetMemberInfo(const ST_PARTY_MEMBER& partyMember) { m_stPartyMember = partyMember; }
    bool GetMemberInfo(ST_PARTY_MEMBER& partyMember) const {
        partyMember = m_stPartyMember;
        return true;
    }

private:
    ST_PARTY_MEMBER m_stPartyMember{};
};

class CParty {
public:
    explicit CParty(std::uint32_t dwPartyID = 0) : m_dwPartyID(dwPartyID) {}
    explicit CParty(PS_REQ_PARTY_CREATE& stCreateParty);

    std::uint32_t GetPartyID() const { return m_dwPartyID; }
    std::uint32_t GetMasterID() const { return m_dwMasterID; }
    void SetMasterID(std::uint32_t dwMasterID) { m_dwMasterID = dwMasterID; }
    std::uint8_t GetUserCount() const { return static_cast<std::uint8_t>(m_mapPartyMember.size()); }

    void AddMember(const ST_PARTY_MEMBER& stPartyMember);
    void SetMemberInfo(const ST_PARTY_MEMBER& partyMember);
    void SetPartyInfo(const PS_PARTY_INFO& partyInfo);
    bool GetMemberInfo(std::uint32_t dwMemberID, ST_PARTY_MEMBER& partyMember) const;
    void GetPartyInfo(PS_PARTY_INFO& partyInfo) const;

    UXMapID GetMazeID() const { return m_uxMazeID; }
    void SetMazeID(UXMapID uxMazeID) { m_uxMazeID = uxMazeID; }

private:
    std::shared_ptr<CPartyMember> GetOrCreateMember(std::uint32_t dwMemberID);

    std::uint32_t m_dwPartyID = 0;
    std::uint32_t m_dwMasterID = 0;
    UXMapID m_uxMazeID{};
    std::map<std::uint32_t, std::shared_ptr<CPartyMember>> m_mapPartyMember;
};
