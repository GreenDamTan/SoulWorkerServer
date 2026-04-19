#pragma once

#include <cstdint>
#include <map>
#include <memory>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/GameServer/XRelayServer/Party.h"

class CServer;

class CPartyManager {
public:
    void Clear() {}

    std::shared_ptr<CParty> GetParty(std::uint32_t dwPartyID);
    std::uint32_t GetPartyID(UXActorID uxActorID) {
        const auto it = m_mapPartyUser.find(uxActorID);
        return it == m_mapPartyUser.end() ? 0u : it->second;
    }
    void AddPartyMember(std::uint32_t dwPartyID, std::uint32_t dwMemberID);
    void DeleteParty(std::uint32_t dwPartyID);
    void ResRecruitAccept(CServer* pServer, PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK& psCheck);
    void ReqJoinMember(CServer* pServer, PS_PARTY_ADDMEMBER& stAddMember, std::uint32_t dwUAID, std::uint8_t byLevel, std::uint32_t dwRecruitID);
    void ReqCreateParty(CServer* pServer, PS_REQ_PARTY_CREATE& stCreateParty);
    void ResJoinMember(PS_PARTY_ADDMEMBER& stAddMember, std::uint32_t dwRecruitID);
    void CreateParty(PS_REQ_PARTY_CREATE& stPartyReq);
    void ResPartyLeave(PS_PARTY_LEAVE& stPartyLeave, std::uint32_t dwNewMaster);
    void ResDeleteParty(PS_PARTY_LEAVE& stPartyLeave);
    void ResUpdateMemberInfo(ST_UPDATE_PARTY_MEMBER& stUpdateMember);
    void ResChangeMaster(PS_PARTY_CHANGE_MASTER& stChangeMaster);
    void ResLoadPartyAll(PS_PARTY_INFO_ALL& stPartyInfoAll, std::uint8_t byEnd);

private:
    std::shared_ptr<CParty> GetOrCreateParty(std::uint32_t dwPartyID);

    std::map<std::uint32_t, std::shared_ptr<CParty>> m_mapParty;
    std::map<UXActorID, std::uint32_t> m_mapPartyUser;
    std::uint8_t m_factoryParty[48] = {}; // TODO: 需人工审查
    bool m_bLoadParty = false;
};
