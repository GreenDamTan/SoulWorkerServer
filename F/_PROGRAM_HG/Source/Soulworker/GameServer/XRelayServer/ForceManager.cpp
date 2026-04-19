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

void CForceManager::ReqJoinMember(CServer* pServer,
                                   PS_FORCE_ADDMEMBER& stAddMember,
                                   std::uint32_t dwUAID,
                                   std::uint8_t byLevel,
                                   std::uint32_t dwRecruitID) {
    const auto it = m_mapForce.find(stAddMember.dwForceID);
    if (it == m_mapForce.end()) {
        LogHelper::LogError("game.contents",
                            "<FORCE> Failed ReqJoinMember ( PID %d / UCID %d ) ",
                            static_cast<int>(stAddMember.dwForceID),
                            static_cast<int>(stAddMember.stMember.dwMemberID));
        return;
    }

    std::shared_ptr<CForce> pForce = it->second;
    pForce->AddMember(stAddMember.stMember);

    UXActorID uxActorID{};
    uxActorID.dwActorID = stAddMember.stMember.dwMemberID;
    m_mapForceUser[uxActorID] = stAddMember.dwForceID;

    TXSingleton<XRelayServer>::Instance()->GetPartyManager().AddPartyMember(stAddMember.dwForceID, stAddMember.stMember.dwMemberID);

    if (pForce->GetUserCount() == 8) {
        TXSingleton<XRelayServer>::Instance()->GetPartyMatchingMgr().ClearRecruitDate(pForce->GetMasterID());
    }

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    const std::shared_ptr<CUserPartyInfo> pUserParty = relayServer.GetPartyUser(stAddMember.stMember.dwMemberID);
    if (pUserParty) {
        relayServer.GetPartyMatchingMgr().ClearRecruitDate(pUserParty->GetActorID());
        pUserParty->ClearRecruitParty(true);
    }

    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 8u, 2u);
    xSendDBPacket << stAddMember;
    xSendDBPacket.XParse << dwRecruitID;
    relayServer.SendDBGame(xSendDBPacket);

    relayServer.SendDBLog(static_cast<int>(dwUAID),
                          static_cast<int>(stAddMember.stMember.dwMemberID),
                          23,
                          5,
                          static_cast<int>(byLevel),
                          0,
                          0,
                          static_cast<int>(pForce->GetUserCount()),
                          0,
                          0,
                          static_cast<std::int64_t>(stAddMember.dwForceID),
                          L"");
}

void CForceManager::ReqCreateForce(CServer* pServer, PS_REQ_FORCE_CREATE& stCreateForce) {
    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 8u, 1u);
    xSendDBPacket << stCreateForce;
    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
}

void CForceManager::ResJoinMember(PS_FORCE_ADDMEMBER& stAddMember, std::uint32_t dwRecruitID) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    if (stAddMember.nErrorCode != 0) {
        LogHelper::LogError("game.contents",
                            "<FORCE> Failed ResJoinMember ( PID %d / UCID %d / %d ) ",
                            static_cast<int>(stAddMember.dwForceID),
                            static_cast<int>(stAddMember.stMember.dwMemberID),
                            static_cast<int>(stAddMember.nErrorCode));
        return;
    }

    const auto it = m_mapForce.find(stAddMember.dwForceID);
    if (it == m_mapForce.end() || !it->second) {
        return;
    }

    std::shared_ptr<CForce> pForce = it->second;
    PS_FORCE_INFO stForceInfo{};
    pForce->GetForceInfo(stForceInfo);

    // Convert to party packet format for broadcast (uses same layout)
    PS_PARTY_ADDMEMBER st{};
    std::memcpy(&st, &stAddMember, sizeof(st));

    XSendPacket sendPacket(0xFAu, 2u);
    sendPacket << st;
    sendPacket << stForceInfo;
    relayServer.SendPacketAll(sendPacket);

    if (dwRecruitID != 0) {
        relayServer.GetPartyMatchingMgr().AddRecruitMember(dwRecruitID, stAddMember.stMember.dwMemberID);
    }
}

void CForceManager::CreateForce(PS_REQ_FORCE_CREATE& stForceReq) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    XSendPacket sendPacket(0xFAu, 1u);
    sendPacket << stForceReq;

    auto pForce = std::make_shared<CForce>(stForceReq);
    m_mapForce[stForceReq.dwForceID] = pForce;

    TXSingleton<XRelayServer>::Instance()->GetPartyManager().AddPartyMember(
        stForceReq.dwForceID, stForceReq.masterInfo.dwMemberID);
    TXSingleton<XRelayServer>::Instance()->GetPartyManager().AddPartyMember(
        stForceReq.dwForceID, stForceReq.memberInfo.dwMemberID);

    UXActorID uxActorID{};
    uxActorID.dwActorID = stForceReq.masterInfo.dwMemberID;
    m_mapForceUser[uxActorID] = stForceReq.dwForceID;

    uxActorID.dwActorID = stForceReq.memberInfo.dwMemberID;
    m_mapForceUser[uxActorID] = stForceReq.dwForceID;

    relayServer.SendPacketAll(sendPacket);

    if (stForceReq.dwRecruitID != 0) {
        relayServer.GetPartyMatchingMgr().CreateForce(stForceReq);
    }

    relayServer.SendDBLog(static_cast<int>(stForceReq.dwMasterUAID),
                          static_cast<int>(stForceReq.masterInfo.dwMemberID),
                          23,
                          14,
                          static_cast<int>(stForceReq.masterInfo.byLevel),
                          stForceReq.masterInfo.nMapID,
                          static_cast<int>(stForceReq.memberInfo.dwMemberID),
                          static_cast<int>(stForceReq.memberInfo.byLevel),
                          0,
                          0,
                          static_cast<std::int64_t>(stForceReq.dwForceID),
                          L"");
}

void CForceManager::ResChangeMaster(PS_FORCE_CHANGE_MASTER& stChangeMaster) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    if (stChangeMaster.nErrorCode != 0) {
        LogHelper::LogInfo("game.contents",
                           "<FORCE> ResChangeMaster ( ErrorCode : %d )",
                           static_cast<int>(stChangeMaster.nErrorCode));
    }

    XSendPacket sendPacket(0xFAu, 5u);
    sendPacket << stChangeMaster;
    relayServer.SendPacketAll(sendPacket);
}

void CForceManager::ResForceLeave(PS_FORCE_LEAVE& stForceLeave, std::uint32_t dwNewMaster) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    XSendPacket sendPacket(0xFAu, 3u);
    sendPacket << stForceLeave;
    sendPacket.XParse << dwNewMaster;
    sendPacket.XParse << 0u;
    relayServer.SendPacketAll(sendPacket);
}

void CForceManager::ResDeleteForce(PS_FORCE_LEAVE& stForceLeave) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    // 广播 force 删除
    XSendPacket sendPacket(0xFAu, 6u);
    sendPacket << stForceLeave;
    relayServer.SendPacketAll(sendPacket);

    // 清理 recruit 状态
    const std::uint32_t dwRecruitID = relayServer.GetPartyMatchingMgr().FindRecruitID(stForceLeave.dwLeaveMember);
    const std::shared_ptr<CPartyRecruit> pRecruit = relayServer.GetPartyMatchingMgr().FindRecruitPtr(dwRecruitID);
    if (pRecruit) {
        if (pRecruit->GetMasterID() == stForceLeave.dwLeaveMember) {
            // Master 退出：清除整个 recruit
            relayServer.GetPartyMatchingMgr().ClearRecruitDate(pRecruit->GetMasterID());
        } else {
            // 普通成员退出：清除 CID 并从 recruit 列表移除
            pRecruit->SetCID(0u);
            const std::uint32_t dwRecruitIDForDelete = relayServer.GetPartyMatchingMgr().FindRecruitID(pRecruit->GetMasterID());
            relayServer.GetPartyMatchingMgr().DeleteRecruitMember(dwRecruitIDForDelete, stForceLeave.dwLeaveMember);
        }
    }

    // 从内存索引中删除 force
    DeleteForce(stForceLeave.dwForceID);
}

void CForceManager::ResLoadForceAll(PS_FORCE_INFO_ALL& stForceInfoAll, std::uint8_t byEnd) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    for (const auto& forceInfo : stForceInfoAll.vecForceInfo) {
        auto pForce = std::make_shared<CForce>();
        pForce->SetForceInfo(forceInfo);

        m_mapForce[forceInfo.dwForceID] = pForce;

        // 添加成员索引
        for (const auto& member : forceInfo.vecForceMember) {
            UXActorID uxActorID{};
            uxActorID.dwActorID = member.dwMemberID;
            m_mapForceUser[uxActorID] = forceInfo.dwForceID;

            relayServer.GetPartyManager().AddPartyMember(forceInfo.dwForceID, member.dwMemberID);
        }
    }

    if (byEnd) {
        m_bLoadForce = true;
        relayServer.SetCachingLoad(1u);  // E_SERVER_CACHING_LOAD_PARTY
    }

    LogHelper::LogInfo("game.contents",
                       "<FORCE> LOAD FORCE INFO ALL ( COUNT : %d, END : %d )",
                       static_cast<int>(stForceInfoAll.vecForceInfo.size()),
                       static_cast<int>(byEnd));
}
