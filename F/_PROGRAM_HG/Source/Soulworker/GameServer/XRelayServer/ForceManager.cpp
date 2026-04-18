#include "Soulworker/GameServer/XRelayServer/ForceManager.h"

#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

std::shared_ptr<CForce> CForceManager::GetOrCreateForce(std::uint32_t dwForceID) {
    auto& forceSlot = m_mapForce[dwForceID];
    if (!forceSlot) {
        forceSlot = std::make_shared<CForce>(dwForceID);
        forceSlot->SetMasterID(dwForceID);
    }
    return forceSlot;
}

std::shared_ptr<CForce> CForceManager::GetForce(std::uint32_t dwForceID) {
    const auto it = m_mapForce.find(dwForceID);
    return it == m_mapForce.end() ? std::shared_ptr<CForce>{} : it->second;
}

std::shared_ptr<CForce> CForceManager::GetForce(UXActorID uxActorID) {
    const auto forceUserIt = m_mapForceUser.find(uxActorID);
    if (forceUserIt == m_mapForceUser.end()) {
        return {};
    }
    return GetForce(forceUserIt->second);
}

void CForceManager::ResUpdateMemberInfo(ST_UPDATE_FORCE_MEMBER& stUpdateMember) {
    const std::shared_ptr<CForce> force = GetOrCreateForce(stUpdateMember.dwForceID);
    if (force) {
        force->SetMemberInfo(stUpdateMember.stForceMember);
        UXActorID uxActorID{};
        uxActorID.dwActorID = stUpdateMember.stForceMember.dwMemberID;
        m_mapForceUser[uxActorID] = stUpdateMember.dwForceID;
    }

    XSendPacket xSendPacket(0xFAu, 0x05u);
    xSendPacket << stUpdateMember;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

void CForceManager::SendForceMessage(PS_CHAT_PARTY& stChatForce, PS_CHAT_ITEM_LINK_FOR_SERVER& psChatItemLinkInfo) {
    XSendPacket xSendPacket(0xFAu, 0x10u);
    xSendPacket << stChatForce;
    xSendPacket << psChatItemLinkInfo;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

void CForceManager::EnterServer(CServer* pServer, PS_REQ_FORCE_ENTER_SERVER& stEnterServer) {
    if (stEnterServer.dwForceID == 0) {
        return;
    }

    const std::shared_ptr<CForce> force = GetForce(stEnterServer.dwForceID);
    if (!force) {
        LogHelper::LogError("game.contents",
                            "<FORCE> Error Enter Server ( PID %d / UCID %d ) ",
                            stEnterServer.dwForceID,
                            stEnterServer.dwMemberID);
        return;
    }

    force->SetMemberInfo(stEnterServer.dwMemberID, stEnterServer.uxMapID, stEnterServer.nMaxHP);
    UXActorID uxActorID{};
    uxActorID.dwActorID = stEnterServer.dwMemberID;
    m_mapForceUser[uxActorID] = stEnterServer.dwForceID;

    if (stEnterServer.bReqForceInfo) {
        PS_RES_FORCE_ENTER_SERVER stEnterServerRes{};
        stEnterServerRes.bLoadForce = true;
        force->GetForceInfo(stEnterServerRes.stForceInfo);
        stEnterServerRes.stForceInfo.dwForceID = stEnterServer.dwForceID;
        force->GetMemberInfo(stEnterServer.dwMemberID, stEnterServerRes.stEnterMember);

        XSendPacket sendPacket(0xFAu, 0x0Au);
        sendPacket << stEnterServerRes;
        pServer->SendEx(sendPacket);
    }

    if (force->GetMasterID() == stEnterServer.dwMemberID) {
        ST_PARTY_RECRUIT_INFO stRecruit{};
        if (TXSingleton<XRelayServer>::Instance()->GetPartyMatchingMgr().GetPartyRecruitInfo(stEnterServer.dwMemberID, stRecruit)) {
            XSendPacket xSendPacket(0xF4u, 0x2Eu);
            xSendPacket.XParse << stEnterServer.dwMemberID;
            xSendPacket << stRecruit.stRecruit;
            pServer->SendEx(xSendPacket);
        }
    }
}

bool CForceManager::CreateForceMatching(const PS_FORCE_INFO& stCreateForce) {
    const std::shared_ptr<CForce> force = GetOrCreateForce(stCreateForce.dwForceID);
    if (!force) {
        return false;
    }

    force->SetMasterID(stCreateForce.dwMaster);
    for (const ST_FORCE_MEMBER& member : stCreateForce.vecForceMember) {
        force->SetMemberInfo(member);
        UXActorID uxActorID{};
        uxActorID.dwActorID = member.dwMemberID;
        m_mapForceUser[uxActorID] = stCreateForce.dwForceID;
        TXSingleton<XRelayServer>::Instance()->GetPartyManager().AddPartyMember(stCreateForce.dwForceID, member.dwMemberID);
    }
    return true;
}

void CForceManager::DeleteForce(std::uint32_t dwForceID) {
    m_mapForce.erase(dwForceID);
}

void CForceManager::ReqForceInfo(CServer* pServer, std::uint32_t dwForceID, std::uint32_t dwActorID) {
    if (!pServer || !pServer->IsState(XClient::eStateConnect)) {
        return;
    }

    UXActorID uxActorID{};
    uxActorID.dwActorID = dwActorID;

    std::shared_ptr<CForce> force = GetForce(uxActorID);
    if (!force && dwForceID != 0) {
        force = GetForce(dwForceID);
    }
    if (!force) {
        return;
    }

    PS_FORCE_INFO stForceInfo{};
    force->GetForceInfo(stForceInfo);

    XSendPacket xSendPacket(0xFAu, 0x1Au);
    xSendPacket.XParse << dwActorID;
    xSendPacket << stForceInfo;
    pServer->SendEx(xSendPacket);
}
