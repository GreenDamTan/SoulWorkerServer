#pragma once

#include <cstdint>
#include <map>
#include <memory>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/GameServer/XRelayServer/Force.h"

class CServer;

class CForceManager {
public:
    void Clear() {}
    void ResUpdateMemberInfo(ST_UPDATE_FORCE_MEMBER& stUpdateMember);
    void SendForceMessage(PS_CHAT_PARTY& stChatForce, PS_CHAT_ITEM_LINK_FOR_SERVER& psChatItemLinkInfo);
    void EnterServer(CServer* pServer, PS_REQ_FORCE_ENTER_SERVER& stEnterServer);
    bool CreateForceMatching(const PS_FORCE_INFO& stCreateForce);
    void DeleteForce(std::uint32_t dwForceID);
    std::shared_ptr<CForce> GetForce(std::uint32_t dwForceID);
    std::shared_ptr<CForce> GetForce(UXActorID uxActorID);
    void ReqForceInfo(CServer* pServer, std::uint32_t dwForceID, std::uint32_t dwActorID);

private:
    std::shared_ptr<CForce> GetOrCreateForce(std::uint32_t dwForceID);

    std::map<std::uint32_t, std::shared_ptr<CForce>> m_mapForce;
    std::map<UXActorID, std::uint32_t> m_mapForceUser;
    std::uint8_t m_factoryForce[48] = {}; // TODO: 需人工审查
    int m_nRequestNo = 0;
    bool m_bLoadForce = false;
    std::map<std::uint32_t, ST_FORCE_INVITE_INFO> m_mapForceInvite;
};

