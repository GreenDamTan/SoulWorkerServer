#include "Soulworker/GameServer/XRelayServer/ForceManager.h"

#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/PartyManager.h"
#include "Soulworker/GameServer/XRelayServer/PartyMatchingMgr.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"
#include "Soulworker/GameServer/XRelayServer/UserObject.h"
#include "Soulworker/GameServer/XRelayServer/UserPartyInfo.h"

#ifdef _WIN32
#include <windows.h>
#endif

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

    // Broadcast force delete
    XSendPacket sendPacket(0xFAu, 6u);
    sendPacket << stForceLeave;
    relayServer.SendPacketAll(sendPacket);

    // Clean up recruit state
    const std::uint32_t dwRecruitID = relayServer.GetPartyMatchingMgr().FindRecruitID(stForceLeave.dwLeaveMember);
    const std::shared_ptr<CPartyRecruit> pRecruit = relayServer.GetPartyMatchingMgr().FindRecruitPtr(dwRecruitID);
    if (pRecruit) {
        if (pRecruit->GetMasterID() == stForceLeave.dwLeaveMember) {
            // Master left: clear entire recruit
            relayServer.GetPartyMatchingMgr().ClearRecruitDate(pRecruit->GetMasterID());
        } else {
            // Non-master left: clear CID and remove from recruit member list
            pRecruit->SetCID(0u);
            // Original binary: uses pRecruit->GetUAID() which returns m_stPartyRecruit.dwRecruitID
            const std::uint32_t dwRecruitIDFromObject = pRecruit->m_stPartyRecruit.dwRecruitID;
            relayServer.GetPartyMatchingMgr().DeleteRecruitMember(dwRecruitIDFromObject, stForceLeave.dwLeaveMember);
        }
    }

    // Remove force from memory (force was already deleted in ReqDeleteForce, but
    // ResDeleteForce may also be called from DB response path where force still exists)
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

void CForceManager::SetMaze(std::uint32_t dwForceID, UXMapID uxMapID, UXMapID uxBeforeMapID) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    // 查找 force
    const auto it = m_mapForce.find(dwForceID);
    if (it == m_mapForce.end() || !it->second) {
        return;
    }

    std::shared_ptr<CForce> pForce = it->second;

    // 条件：新的 mapID 非零，或者当前 MazeID 等于 beforeMapID
    UXMapID currentMazeID = pForce->GetMazeID();
    if (uxMapID.nMapID != 0 || currentMazeID.nMapID == uxBeforeMapID.nMapID) {
        // 更新 force 的 MazeID
        pForce->SetMazeID(uxMapID);

        // 发送 DB 更新 (0x08/0x08)
        IXObject* pObject = nullptr;
        XSendDBPacket xSendDBPacket(pObject, 8u, 8u);
        xSendDBPacket.XParse << dwForceID;
        xSendDBPacket.XParse << uxMapID.nMapID;
        xSendDBPacket.XParse << 0;
        relayServer.SendDBGame(xSendDBPacket);

        // 广播 0xFA/0x09 (force maze info)
        XSendPacket sendPacket(0xFAu, 9u);
        sendPacket.XParse << dwForceID;
        sendPacket.XParse << uxMapID.nMapID;
        relayServer.SendPacketAll(sendPacket);
    }
}

void CForceManager::SendForceErrorInvite(CServer* pServer, PS_REQ_FORCE_INVITE& stForceInvite, int nErrorCode) {
    stForceInvite.nResult = nErrorCode;

    XSendPacket sendPacket(0xFAu, 0x0Bu);
    sendPacket << stForceInvite;
    sendPacket.XParse << static_cast<std::uint32_t>(0);
    sendPacket.XParse << static_cast<std::uint8_t>(0);
    sendPacket.XParse << static_cast<std::uint32_t>(0);
    pServer->SendEx(sendPacket);
}

void CForceManager::ReqInviteForce(CServer* pServer,
                                    PS_REQ_FORCE_INVITE& stForceInvite,
                                    int dwUAID,
                                    std::uint8_t byLevel,
                                    std::uint32_t dwForceID) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    // Look up the invited user by name
    const std::shared_ptr<CUserObject> pInviteUser = relayServer.GetUser(stForceInvite.strName);
    if (!pInviteUser) {
        SendForceErrorInvite(pServer, stForceInvite, 53111);
        return;
    }

    // Set invite actor ID from the user's MatchingID
    stForceInvite.dwInviteActorID = pInviteUser->GetCID();

    // Check if invitee has reward state
    const std::shared_ptr<CUserPartyInfo> pInvitePartyUser = relayServer.GetPartyUser(pInviteUser->GetCID());
    if (pInvitePartyUser) {
        if (pInvitePartyUser->GetRewardState()) {
            SendForceErrorInvite(pServer, stForceInvite, 53113);
            return;
        }
    }

    // Check if invitee is already in a party - error code 53145
    if (relayServer.GetPartyManager().IsParty(pInviteUser->GetCID())) {
        SendForceErrorInvite(pServer, stForceInvite, 53145);
        return;
    }

    // Check friend block - error code 53113
    if (relayServer.IsFriendBlock(stForceInvite.dwInviteActorID, stForceInvite.dwReqActorID)) {
        SendForceErrorInvite(pServer, stForceInvite, 53113);
        return;
    }

    // Check recruit info - force type should be 2
    ST_PARTY_RECRUIT_INFO stPartyRecruit{};
    if (relayServer.GetPartyMatchingMgr().GetPartyRecruitInfo(stForceInvite.dwReqActorID, stPartyRecruit)) {
        if (stPartyRecruit.stRecruit.byPartyGroupType != 2) {
            SendForceErrorInvite(pServer, stForceInvite, 53034);
            return;
        }
    }

    // Check invite cooldown
    const auto itInvite = m_mapForceInvite.find(stForceInvite.dwInviteActorID);
    if (itInvite != m_mapForceInvite.end()) {
        std::uint64_t currentTick = static_cast<std::uint64_t>(GetTickCount64());
        if (currentTick < itInvite->second.dwLimitTime) {
            if (itInvite->second.dwMasterID == stForceInvite.dwReqActorID) {
                // Same inviter cooldown - error code 53114
                SendForceErrorInvite(pServer, stForceInvite, 53114);
            } else {
                // Different inviter cooldown - error code 53117
                SendForceErrorInvite(pServer, stForceInvite, 53117);
            }
            return;
        }
        m_mapForceInvite.erase(itInvite);
    }

    // Check if invitee is already in a force - error code 53104
    UXActorID uxActorID{};
    uxActorID.dwActorID = pInviteUser->GetCID();
    const auto itForceUser = m_mapForceUser.find(uxActorID);
    if (itForceUser != m_mapForceUser.end()) {
        SendForceErrorInvite(pServer, stForceInvite, 53104);
        return;
    }

    // Check if invitee is in maze - error code 53102
    if (pInviteUser->IsMaze()) {
        SendForceErrorInvite(pServer, stForceInvite, 53102);
        return;
    }

    // Check district Force_Use - error code 53159
    const std::int16_t mapID = static_cast<std::int16_t>(pInviteUser->GetMapID());
    TB_DISTRICT* pDistrict = relayServer.GetDistrictControl().GetTB_DISTRICT(mapID);
    if (!pDistrict || pDistrict->Force_Use == 1) {
        SendForceErrorInvite(pServer, stForceInvite, 53159);
        return;
    }

    // Log invite
    LogHelper::LogDebug("game.contents", "<FORCE> Invite %d", static_cast<int>(stForceInvite.dwInviteActorID));

    // Add to invite map
    ST_FORCE_INVITE_INFO stInviteInfo{};
    stInviteInfo.dwMasterID = stForceInvite.dwReqActorID;
    stInviteInfo.dwLimitTime = static_cast<std::uint64_t>(GetTickCount64() + 60000);
    m_mapForceInvite[stForceInvite.dwInviteActorID] = stInviteInfo;

    // Send invite packet 0xFA/0x0B
    XSendPacket sendPacket(0xFAu, 0x0Bu);
    sendPacket << stForceInvite;
    sendPacket.XParse << static_cast<std::uint32_t>(dwUAID);
    sendPacket.XParse << byLevel;
    sendPacket.XParse << dwForceID;
    pInviteUser->SendPacket(sendPacket);
}

void CForceManager::SendForceErrorAccept(CServer* pServer, std::uint32_t dwActorID, int nErrorCode) {
    PS_RES_FORCE_ACCEPT stForceAccept{};
    stForceAccept.dwAcceptID = dwActorID;
    stForceAccept.nResult = nErrorCode;

    XSendPacket sendPacket(0xFAu, 0x0Cu);
    sendPacket << stForceAccept;
    pServer->SendEx(sendPacket);
}

void CForceManager::ReqAcceptForce(CServer* pServer,
                                    PS_RES_FORCE_INVITE& stAcceptForce,
                                    std::uint32_t dwUAID,
                                    std::uint8_t byLevel) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    // Look up invite by acceptID
    const auto itInvite = m_mapForceInvite.find(stAcceptForce.dwAcceptID);
    if (itInvite == m_mapForceInvite.end()) {
        return;
    }

    if (stAcceptForce.nResult == 0) {
        // Accepted: extract invite info
        ST_FORCE_INVITE_INFO stInviteInfo = itInvite->second;

        if (stInviteInfo.dwLimitTime < static_cast<std::uint64_t>(GetTickCount64())) {
            // Cooldown expired - fall through to erase below
        } else {
            // Look up master user and party info
            const std::shared_ptr<CUserPartyInfo> pMasterForceInfo = relayServer.GetPartyUser(stInviteInfo.dwMasterID);
            const std::shared_ptr<CUserObject> pMaster = relayServer.GetUser(stInviteInfo.dwMasterID);

            if (!pMasterForceInfo || !pMaster) {
                SendForceErrorAccept(pServer, stAcceptForce.dwAcceptID, 53111);
                LogHelper::LogError("game.system", "<FORCE> ReqAcceptForce::pMaster = NULL %d", stInviteInfo.dwMasterID);
                return;
            }

            // Check recruit party group type
            ST_PARTY_RECRUIT_INFO stPartyRecruit{};
            if (relayServer.GetPartyMatchingMgr().GetPartyRecruitInfo(stInviteInfo.dwMasterID, stPartyRecruit)) {
                if (stPartyRecruit.stRecruit.byPartyGroupType != 2) {
                    SendForceErrorAccept(pServer, stAcceptForce.dwAcceptID, 53115);
                    return;
                }
            }

            // Check if master has reward state (matching on party)
            if (pMasterForceInfo->GetRewardState()) {
                // Erase invite entry
                m_mapForceInvite.erase(stAcceptForce.dwAcceptID);
                SendForceErrorAccept(pServer, stAcceptForce.dwAcceptID, 53131);
                LogHelper::LogError("game.contents",
                                    "<FORCE> ReqAcceptForce::Requestor is matching on party = NULL %d",
                                    stInviteInfo.dwMasterID);
                return;
            }

            // Check if master is already in a party
            const std::uint32_t dwMasterMatchingID = pMaster->GetCID();
            if (relayServer.GetPartyManager().IsParty(dwMasterMatchingID)) {
                SendForceErrorAccept(pServer, stAcceptForce.dwAcceptID, 53115);
                LogHelper::LogError("game.relay", "<FORCE> ReqAcceptForce::In Party %d", stInviteInfo.dwMasterID);
                return;
            }

            // Look up member (acceptor) user and party info
            const std::shared_ptr<CUserPartyInfo> pMemberForceInfo = relayServer.GetPartyUser(stAcceptForce.dwAcceptID);
            const std::shared_ptr<CUserObject> pMember = relayServer.GetUser(stAcceptForce.dwAcceptID);

            if (!pMember || !pMemberForceInfo) {
                SendForceErrorAccept(pServer, stAcceptForce.dwAcceptID, 53111);
                LogHelper::LogError("game.contents", "<FORCE> ReqAcceptForce::dwAcceptID = NULL %d", stAcceptForce.dwAcceptID);
                return;
            }

            // Check if member is already in a party
            const std::uint32_t dwMemberMatchingID = pMember->GetCID();
            if (relayServer.GetPartyManager().IsParty(dwMemberMatchingID)) {
                SendForceErrorAccept(pServer, stAcceptForce.dwAcceptID, 53115);
                LogHelper::LogError("game.relay", "<FORCE> ReqAcceptForce::In Party %d", stAcceptForce.dwAcceptID);
                return;
            }

            // Check if member is already in a force
            const std::uint32_t dwMemberCID = pMemberForceInfo->GetMatchingID();
            if (relayServer.GetPartyManager().IsParty(dwMemberCID)) {
                SendForceErrorAccept(pServer, stAcceptForce.dwAcceptID, 53115);
                LogHelper::LogError("game.relay", "<FORCE> ReqAcceptForce::In Force %d", stAcceptForce.dwAcceptID);
                return;
            }

            // Check if either is in maze
            if (pMaster->IsMaze() || pMember->IsMaze()) {
                SendForceErrorAccept(pServer, stAcceptForce.dwAcceptID, 53115);
                LogHelper::LogError("game.contents", "<FORCE> ReqAcceptForce::In Maze %d", stAcceptForce.dwAcceptID);
                return;
            }

            // Check if master already has a force
            UXActorID uxMasterActorID{};
            uxMasterActorID.dwActorID = stInviteInfo.dwMasterID;
            const auto itForceUser = m_mapForceUser.find(uxMasterActorID);

            if (itForceUser != m_mapForceUser.end()) {
                // Master already in a force - join existing force
                const std::uint32_t dwForceID = itForceUser->second;

                const auto itForce = m_mapForce.find(dwForceID);
                if (itForce == m_mapForce.end()) {
                    SendForceErrorAccept(pServer, stAcceptForce.dwAcceptID, 53115);
                    LogHelper::LogError("game.contents", "<FORCE> ReqAcceptForce::No Force Info ");
                    return;
                }

                std::shared_ptr<CForce> pForce = itForce->second;
                if (pForce->GetMasterID() != stInviteInfo.dwMasterID) {
                    SendForceErrorAccept(pServer, stAcceptForce.dwAcceptID, 53115);
                    LogHelper::LogError("game.contents", "<FORCE> ReqAcceptForce::Already Make Force!");
                    return;
                }

                if (pForce->GetUserCount() >= 8) {
                    SendForceErrorAccept(pServer, stAcceptForce.dwAcceptID, 53110);
                    LogHelper::LogError("game.contents", "<FORCE> ReqAcceptForce::Many party ");
                    return;
                }

                // Build add-member request
                ST_PARTY_MEMBER stMemberInfo{};
                pMember->GetPartyMemberInfo(stMemberInfo);

                ST_FORCE_MEMBER stForceMemberInfo{};
                std::memcpy(&stForceMemberInfo, &stMemberInfo, sizeof(ST_FORCE_MEMBER));

                PS_FORCE_ADDMEMBER stAddMember{};
                stAddMember.stMember = stForceMemberInfo;
                stAddMember.dwForceID = dwForceID;

                const std::uint32_t dwMasterRecruitID = relayServer.GetPartyMatchingMgr().FindRecruitID(stInviteInfo.dwMasterID);
                ReqJoinMember(pServer, stAddMember, dwUAID, byLevel, dwMasterRecruitID);
            } else {
                // Master not in a force yet - create new force
                PS_REQ_FORCE_CREATE stCreateForce{};

                ST_PARTY_MEMBER stMasterMemberInfo{};
                ST_PARTY_MEMBER stMemberMemberInfo{};
                pMaster->GetPartyMemberInfo(stMasterMemberInfo);
                pMember->GetPartyMemberInfo(stMemberMemberInfo);
                std::memcpy(&stCreateForce.masterInfo, &stMasterMemberInfo, sizeof(ST_FORCE_MEMBER));
                std::memcpy(&stCreateForce.memberInfo, &stMemberMemberInfo, sizeof(ST_FORCE_MEMBER));

                stCreateForce.dwMasterUAID = pMaster->GetUAID();

                const std::uint32_t dwUCID = pMasterForceInfo->GetActorID();
                stCreateForce.dwRecruitID = relayServer.GetPartyMatchingMgr().FindRecruitID(dwUCID);

                const std::uint32_t dwPartyID = pMemberForceInfo->GetActorID();
                relayServer.GetPartyMatchingMgr().ClearRecruitDate(dwPartyID);
                pMemberForceInfo->ClearRecruitParty(true);
                pMasterForceInfo->ClearRecruitParty(true);

                ReqCreateForce(pServer, stCreateForce);
            }
        }
    }

    // Always erase invite entry at the end
    const auto itErase = m_mapForceInvite.find(stAcceptForce.dwAcceptID);
    if (itErase != m_mapForceInvite.end()) {
        m_mapForceInvite.erase(itErase);
    }
}

void CForceManager::ReqCancelForce(CServer* pServer, PS_PARTY_REJECT& stForceReject) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    // Look up invite by rejectID
    const auto itInvite = m_mapForceInvite.find(stForceReject.dwRejectID);
    if (itInvite == m_mapForceInvite.end()) {
        return;
    }

    const ST_FORCE_INVITE_INFO& stInviteInfo = itInvite->second;

    // Look up the master user
    const std::shared_ptr<CUserObject> pMaster = relayServer.GetUser(stInviteInfo.dwMasterID);
    if (pMaster) {
        // Send reject packet to master 0xFA/0x0D
        XSendPacket xSendPacket(0xFAu, 0x0Du);
        xSendPacket << stForceReject;
        pMaster->SendPacket(xSendPacket);
    }

    // Erase invite entry
    m_mapForceInvite.erase(itInvite);
}

void CForceManager::ReqForceLeave(CServer* pServer,
                                    PS_FORCE_LEAVE& stForceLeave,
                                    std::uint32_t dwActorID,
                                    std::uint32_t dwUAID,
                                    std::uint8_t byLevel,
                                    std::uint8_t byLeaveLevel) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    // Look up force by forceID
    const auto it = m_mapForce.find(stForceLeave.dwForceID);
    if (it == m_mapForce.end() || !it->second) {
        return;
    }

    std::shared_ptr<CForce> pForce = it->second;

    // If only 2 members or less, delete entire force
    if (pForce->GetUserCount() <= 2) {
        ReqDeleteForce(pServer, stForceLeave, dwActorID, dwUAID, byLevel);
        return;
    }

    // Check if leaver is master
    std::uint32_t dwNewMaster = pForce->GetMasterID();
    if (pForce->GetMasterID() == stForceLeave.dwLeaveMember) {
        // Find new master
        dwNewMaster = pForce->FindNewMaster();
        if (dwNewMaster == 0) {
            LogHelper::LogError("game.contents", "<FORCE> Cant Find New Master ( PID %d )", stForceLeave.dwForceID);
            return;
        }
        pForce->ChangeMaster(dwNewMaster, false);
        relayServer.GetPartyMatchingMgr().ClearRecruitDate(stForceLeave.dwLeaveMember);
    }

    // Remove member from force user lookup (only this, not from force member map)
    RemoveForceMember(stForceLeave.dwLeaveMember);

    // Get master for recruit operations
    const std::uint32_t dwUCID = pForce->GetMasterID();
    const std::uint32_t dwRecruitID = relayServer.GetPartyMatchingMgr().FindRecruitID(dwUCID);
    relayServer.GetPartyMatchingMgr().DeleteRecruitMember(dwRecruitID, stForceLeave.dwLeaveMember);

    // Kick user from force
    pForce->Kickout(stForceLeave.dwLeaveMember);

    // Send DB leave request
    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 8u, 3u);
    xSendDBPacket << stForceLeave;
    xSendDBPacket.XParse << dwNewMaster;
    relayServer.SendDBGame(xSendDBPacket);

    // Send DB log
    relayServer.SendDBLog(static_cast<int>(dwUAID),
                          static_cast<int>(dwActorID),
                          23, 7,
                          static_cast<int>(byLevel),
                          static_cast<int>(stForceLeave.dwLeaveMember),
                          static_cast<int>(byLeaveLevel),
                          static_cast<int>(pForce->GetUserCount()),
                          stForceLeave.bKickout ? 1 : 0,
                          static_cast<int>(dwNewMaster),
                          static_cast<std::int64_t>(stForceLeave.dwForceID),
                          L"");
}

void CForceManager::ReqChangeMaster(CServer* pServer, PS_FORCE_CHANGE_MASTER& stChangeMaster) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    int nErrorCode = 0;

    // Look up force by forceID
    const auto it = m_mapForce.find(stChangeMaster.dwForceID);
    if (it == m_mapForce.end() || !it->second) {
        nErrorCode = 2;
    } else {
        std::shared_ptr<CForce> pForce = it->second;
        if (!pForce->ChangeMaster(stChangeMaster.dwNewMasterID, true)) {
            nErrorCode = 1;
        } else {
            // Send DB change master request
            IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
            XSendDBPacket xSendDBPacket(pObject, 8u, 5u);
            xSendDBPacket << stChangeMaster;
            relayServer.SendDBGame(xSendDBPacket);
        }
    }

    if (nErrorCode != 0) {
        PS_FORCE_CHANGE_MASTER stChangeMasterRes = stChangeMaster;
        stChangeMasterRes.nErrorCode = nErrorCode;

        XSendPacket sendPacket(0xFAu, 4u);
        sendPacket << stChangeMasterRes;
        pServer->SendEx(sendPacket);

        LogHelper::LogInfo("game.contents", "<FORCE> ReqChangeMaster ( ErrorCode : %d )", nErrorCode);
    }
}

bool CForceManager::ReqForceMazeClear(std::uint32_t dwForceID) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    const auto it = m_mapForce.find(dwForceID);
    if (it == m_mapForce.end() || !it->second) {
        return false;
    }

    std::shared_ptr<CForce> pForce = it->second;

    // If force type is 1 (maze), reset to 0
    if (pForce->GetForceType() == 1) {
        pForce->SetForceType(0);

        XSendDBPacket xSendDBPacket(nullptr, 8u, 0xEu);
        xSendDBPacket.XParse << dwForceID;
        relayServer.SendDBGame(xSendDBPacket);
    }

    // Broadcast 0xFA/0x19 to all servers
    XSendPacket xSendPacket(0xFAu, 0x19u);
    xSendPacket.XParse << dwForceID;
    relayServer.SendPacketAll(xSendPacket);

    return true;
}

void CForceManager::ReqDeleteForce(CServer* pServer,
                                     PS_FORCE_LEAVE& stForceLeave,
                                     std::uint32_t dwActorID,
                                     std::uint32_t dwUAID,
                                     std::uint8_t byLevel) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    // Delete force from map immediately (original does this first)
    DeleteForce(stForceLeave.dwForceID);

    // Send DB delete request
    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 8u, 6u);
    xSendDBPacket << stForceLeave;
    relayServer.SendDBGame(xSendDBPacket);

    // Send DB log
    relayServer.SendDBLog(static_cast<int>(dwUAID),
                          static_cast<int>(dwActorID),
                          23, 8,
                          static_cast<int>(byLevel),
                          0,
                          0,
                          0,
                          0,
                          0,
                          static_cast<std::int64_t>(stForceLeave.dwForceID),
                          L"");
}

void CForceManager::RemoveForceMember(std::uint32_t dwMemberID) {
    // Only erase from the user-to-force lookup table
    // (original does NOT remove from force object's member map here;
    //  that's handled by Kickout/RemoveMember separately)
    UXActorID uxActorID{};
    uxActorID.dwActorID = dwMemberID;

    const auto itForceUser = m_mapForceUser.find(uxActorID);
    if (itForceUser != m_mapForceUser.end()) {
        m_mapForceUser.erase(itForceUser);
    }
}
