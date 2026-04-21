#include "Soulworker/GameServer/XRelayServer/PartyManager.h"

#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/Force.h"
#include "Soulworker/GameServer/XRelayServer/ForceManager.h"
#include "Soulworker/GameServer/XRelayServer/PartyRecruit.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"
#include "Soulworker/GameServer/XRelayServer/UserObject.h"
#include "Soulworker/GameServer/XRelayServer/UserPartyInfo.h"

#ifdef _WIN32
#include <windows.h>
#endif

std::shared_ptr<CParty> CPartyManager::GetOrCreateParty(std::uint32_t dwPartyID) {
    auto& partySlot = m_mapParty[dwPartyID];
    if (!partySlot) {
        partySlot = std::make_shared<CParty>(dwPartyID);
        partySlot->SetMasterID(dwPartyID);
    }
    return partySlot;
}

std::shared_ptr<CParty> CPartyManager::GetParty(std::uint32_t dwPartyID) {
    const auto it = m_mapParty.find(dwPartyID);
    return it == m_mapParty.end() ? std::shared_ptr<CParty>{} : it->second;
}

void CPartyManager::AddPartyMember(std::uint32_t dwPartyID, std::uint32_t dwMemberID) {
    UXActorID uxActorID{};
    uxActorID.dwActorID = dwMemberID;
    m_mapPartyUser[uxActorID] = dwPartyID;
}

void CPartyManager::DeleteParty(std::uint32_t dwPartyID) {
    m_mapParty.erase(dwPartyID);
}

void CPartyManager::ResRecruitAccept(CServer* pServer, PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK& psCheck) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    const std::shared_ptr<CPartyRecruit> pRecruit =
        relayServer.GetPartyMatchingMgr().FindRecruitPtr(psCheck.dwRecruitID);

    if (!pRecruit) {
        return;
    }

    if (psCheck.nErrorCode != 0) {
        XSendPacket xSendPacket(0xF4u, 0x28u);
        xSendPacket.XParse << pRecruit->GetMasterID();
        xSendPacket.XParse << psCheck.nErrorCode;
        pServer->SendEx(xSendPacket);
        return;
    }

    if (psCheck.byPartyGroupType == 1) {
        // Party path
        const std::uint32_t partyID = pRecruit->GetPartyID();
        const auto it = m_mapParty.find(partyID);

        if (it != m_mapParty.end()) {
            // Party exists, check if full
            std::shared_ptr<CParty> pParty = it->second;
            if (pParty->GetUserCount() >= 4) {
                XSendPacket xSendPacket(0xF4u, 0x28u);
                xSendPacket.XParse << pRecruit->GetMasterID();
                xSendPacket.XParse << 53020;
                pServer->SendEx(xSendPacket);
                return;
            }

            // Add member to existing party
            PS_PARTY_ADDMEMBER stAddMember{};
            stAddMember.stMember = psCheck.stMember.stMember;
            stAddMember.dwPartyID = partyID;
            ReqJoinMember(pServer, stAddMember, psCheck.dwUAID, psCheck.stMember.stMember.byLevel, psCheck.dwRecruitID);
        } else {
            // Create new party
            const std::uint32_t masterID = pRecruit->GetMasterID();
            const std::shared_ptr<CUserObject> pMaster = relayServer.GetUser(masterID);
            const std::shared_ptr<CUserObject> pMember = relayServer.GetUser(psCheck.stMember.stMember.dwMemberID);

            if (!pMaster || !pMember) {
                XSendPacket xSendPacket(0xF4u, 0x28u);
                xSendPacket.XParse << pRecruit->GetMasterID();
                xSendPacket.XParse << 53011;
                pServer->SendEx(xSendPacket);
                return;
            }

            PS_REQ_PARTY_CREATE stCreateParty{};
            stCreateParty.dwMasterUAID = pMaster->GetUAID();
            stCreateParty.dwRecruitID = psCheck.dwRecruitID;
            pMaster->GetPartyMemberInfo(stCreateParty.masterInfo);
            pMember->GetPartyMemberInfo(stCreateParty.memberInfo);

            const std::shared_ptr<CUserPartyInfo> pMemberParty = relayServer.GetPartyUser(psCheck.stMember.stMember.dwMemberID);
            if (pMemberParty) {
                pMemberParty->ClearRecruitParty(true);
            }
            relayServer.GetPartyMatchingMgr().ClearRecruitDate(psCheck.stMember.stMember.dwMemberID);

            ReqCreateParty(pServer, stCreateParty);
        }
    } else if (psCheck.byPartyGroupType == 2) {
        // Force path
        const std::uint32_t forceID = pRecruit->GetPartyID();
        const std::shared_ptr<CForce> pForce = relayServer.GetForceManager().GetForce(forceID);

        if (pForce) {
            // Force exists, check if full
            if (pForce->GetUserCount() >= 8) {
                XSendPacket xSendPacket(0xF4u, 0x28u);
                xSendPacket.XParse << pRecruit->GetMasterID();
                xSendPacket.XParse << 53110;
                pServer->SendEx(xSendPacket);
                return;
            }

            // Add member to existing force
            PS_FORCE_ADDMEMBER stAddMember{};
            stAddMember.stMember.dwMemberID = psCheck.stMember.stMember.dwMemberID;
            stAddMember.stMember.byLevel = psCheck.stMember.stMember.byLevel;
            stAddMember.stMember.byClass = psCheck.stMember.stMember.byClass;
            stAddMember.stMember.byAwaken = psCheck.stMember.stMember.byAwaken;
            stAddMember.stMember.dwProfilePhotoID = psCheck.stMember.stMember.dwProfilePhotoID;
            stAddMember.stMember.nMapID = psCheck.stMember.stMember.nMapID;
            stAddMember.stMember.nChannel = psCheck.stMember.stMember.nChannel;
            stAddMember.stMember.nMaxHP = psCheck.stMember.stMember.nMaxHP;
            stAddMember.stMember.nHP = psCheck.stMember.stMember.nHP;
            stAddMember.stMember.bLogin = psCheck.stMember.stMember.bLogin;
            stAddMember.stMember.uxMapID = psCheck.stMember.stMember.uxMapID;

            const std::wstring name = FixedWideArrayToWString(psCheck.stMember.stMember.strName);
            const std::size_t copyLen = std::min<std::size_t>(name.size(), std::size(stAddMember.stMember.strName) - 1);
            std::wmemcpy(stAddMember.stMember.strName, name.c_str(), copyLen);
            stAddMember.stMember.strName[copyLen] = L'\0';
            stAddMember.dwForceID = forceID;

            relayServer.GetForceManager().ReqJoinMember(pServer, stAddMember, psCheck.dwUAID, psCheck.stMember.stMember.byLevel, psCheck.dwRecruitID);
        } else {
            // Create new force
            const std::uint32_t masterID = pRecruit->GetMasterID();
            const std::shared_ptr<CUserObject> pMaster = relayServer.GetUser(masterID);
            const std::shared_ptr<CUserObject> pMember = relayServer.GetUser(psCheck.stMember.stMember.dwMemberID);

            if (!pMaster || !pMember) {
                XSendPacket xSendPacket(0xF4u, 0x28u);
                xSendPacket.XParse << pRecruit->GetMasterID();
                xSendPacket.XParse << 53106;
                pServer->SendEx(xSendPacket);
                return;
            }

            PS_REQ_FORCE_CREATE stCreateForce{};
            stCreateForce.dwMasterUAID = pMaster->GetUAID();
            stCreateForce.dwRecruitID = psCheck.dwRecruitID;
            pMaster->GetPartyMemberInfo(*reinterpret_cast<ST_PARTY_MEMBER*>(&stCreateForce.masterInfo));
            pMember->GetPartyMemberInfo(*reinterpret_cast<ST_PARTY_MEMBER*>(&stCreateForce.memberInfo));

            const std::shared_ptr<CUserPartyInfo> pMemberParty = relayServer.GetPartyUser(psCheck.stMember.stMember.dwMemberID);
            if (pMemberParty) {
                pMemberParty->ClearRecruitParty(true);
            }
            relayServer.GetPartyMatchingMgr().ClearRecruitDate(psCheck.stMember.stMember.dwMemberID);

            relayServer.GetForceManager().ReqCreateForce(pServer, stCreateForce);
        }
    } else {
        // Invalid party group type
        XSendPacket xSendPacket(0xF4u, 0x28u);
        xSendPacket.XParse << pRecruit->GetMasterID();
        xSendPacket.XParse << 53012;
        pServer->SendEx(xSendPacket);
    }
}

void CPartyManager::ReqJoinMember(CServer* pServer,
                                   PS_PARTY_ADDMEMBER& stAddMember,
                                   std::uint32_t dwUAID,
                                   std::uint8_t byLevel,
                                   std::uint32_t dwRecruitID) {
    const auto it = m_mapParty.find(stAddMember.dwPartyID);
    if (it == m_mapParty.end()) {
        LogHelper::LogError("game.relay",
                            "<PARTY> Failed ReqJoinMember ( PID %d / UCID %d ) ",
                            static_cast<int>(stAddMember.dwPartyID),
                            static_cast<int>(stAddMember.stMember.dwMemberID));
        return;
    }

    std::shared_ptr<CParty> pParty = it->second;
    pParty->AddMember(stAddMember.stMember);
    AddPartyMember(stAddMember.dwPartyID, stAddMember.stMember.dwMemberID);

    if (pParty->GetUserCount() == 4) {
        TXSingleton<XRelayServer>::Instance()->GetPartyMatchingMgr().ClearRecruitDate(pParty->GetMasterID());
    }

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    const std::shared_ptr<CUserPartyInfo> pUserParty = relayServer.GetPartyUser(stAddMember.stMember.dwMemberID);
    if (pUserParty) {
        relayServer.GetPartyMatchingMgr().ClearRecruitDate(pUserParty->GetActorID());
        pUserParty->ClearRecruitParty(true);
    }

    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 4u, 2u);
    xSendDBPacket << stAddMember;
    xSendDBPacket.XParse << dwRecruitID;
    relayServer.SendDBGame(xSendDBPacket);

    relayServer.SendDBLog(static_cast<int>(dwUAID),
                          static_cast<int>(stAddMember.stMember.dwMemberID),
                          22,
                          5,
                          static_cast<int>(byLevel),
                          0,
                          0,
                          static_cast<int>(pParty->GetUserCount()),
                          0,
                          0,
                          static_cast<std::int64_t>(stAddMember.dwPartyID),
                          L"");
}

void CPartyManager::ReqCreateParty(CServer* pServer, PS_REQ_PARTY_CREATE& stCreateParty) {
    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 4u, 1u);
    xSendDBPacket << stCreateParty;
    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
}

void CPartyManager::ResJoinMember(PS_PARTY_ADDMEMBER& stAddMember, std::uint32_t dwRecruitID) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    if (stAddMember.nErrorCode != 0) {
        LogHelper::LogError("game.relay",
                            "<PARTY> Failed ResJoinMember ( PID %d / UCID %d / %d ) ",
                            static_cast<int>(stAddMember.dwPartyID),
                            static_cast<int>(stAddMember.stMember.dwMemberID),
                            static_cast<int>(stAddMember.nErrorCode));
        return;
    }

    const auto it = m_mapParty.find(stAddMember.dwPartyID);
    if (it == m_mapParty.end() || !it->second) {
        return;
    }

    std::shared_ptr<CParty> pParty = it->second;
    PS_PARTY_INFO stPartyInfo{};
    pParty->GetPartyInfo(stPartyInfo);

    XSendPacket sendPacket(0xF4u, 2u);
    sendPacket << stAddMember;
    sendPacket << stPartyInfo;
    relayServer.SendPacketAll(sendPacket);

    if (dwRecruitID != 0) {
        relayServer.GetPartyMatchingMgr().AddRecruitMember(dwRecruitID, stAddMember.stMember.dwMemberID);
    }
}

void CPartyManager::CreateParty(PS_REQ_PARTY_CREATE& stPartyReq) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    XSendPacket sendPacket(0xF4u, 1u);
    sendPacket << stPartyReq;

    auto pParty = std::make_shared<CParty>(stPartyReq);
    m_mapParty[stPartyReq.dwPartyID] = pParty;

    AddPartyMember(stPartyReq.dwPartyID, stPartyReq.masterInfo.dwMemberID);
    AddPartyMember(stPartyReq.dwPartyID, stPartyReq.memberInfo.dwMemberID);

    relayServer.SendPacketAll(sendPacket);

    if (stPartyReq.dwRecruitID != 0) {
        relayServer.GetPartyMatchingMgr().CreateParty(stPartyReq);
    }

    relayServer.SendDBLog(static_cast<int>(stPartyReq.dwMasterUAID),
                          static_cast<int>(stPartyReq.masterInfo.dwMemberID),
                          22,
                          14,
                          static_cast<int>(stPartyReq.masterInfo.byLevel),
                          stPartyReq.masterInfo.nMapID,
                          static_cast<int>(stPartyReq.memberInfo.dwMemberID),
                          static_cast<int>(stPartyReq.memberInfo.byLevel),
                          0,
                          0,
                          static_cast<std::int64_t>(stPartyReq.dwPartyID),
                          L"");
}

void CPartyManager::ResPartyLeave(PS_PARTY_LEAVE& stPartyLeave, std::uint32_t dwNewMaster) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    XSendPacket sendPacket(0xF4u, 3u);
    sendPacket << stPartyLeave;
    sendPacket.XParse << dwNewMaster;
    sendPacket.XParse << 0u;
    relayServer.SendPacketAll(sendPacket);
}

void CPartyManager::ResUpdateMemberInfo(ST_UPDATE_PARTY_MEMBER& stUpdateMember) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    XSendPacket sendPacket(0xF4u, 5u);
    sendPacket << stUpdateMember;
    relayServer.SendPacketAll(sendPacket);
}

void CPartyManager::ResChangeMaster(PS_PARTY_CHANGE_MASTER& stChangeMaster) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    if (stChangeMaster.nErrorCode != 0) {
        LogHelper::LogInfo("game.relay",
                           "<PARTY> ResChangeMaster ( ErrorCode : %d )",
                           static_cast<int>(stChangeMaster.nErrorCode));
    }

    XSendPacket sendPacket(0xF4u, 4u);
    sendPacket << stChangeMaster;
    relayServer.SendPacketAll(sendPacket);
}

void CPartyManager::ResDeleteParty(PS_PARTY_LEAVE& stPartyLeave) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    // 广播 party 删除
    XSendPacket sendPacket(0xF4u, 6u);
    sendPacket << stPartyLeave;
    relayServer.SendPacketAll(sendPacket);

    // 清理 recruit 状态
    const std::uint32_t dwRecruitID = relayServer.GetPartyMatchingMgr().FindRecruitID(stPartyLeave.dwLeaveMember);
    const std::shared_ptr<CPartyRecruit> pRecruit = relayServer.GetPartyMatchingMgr().FindRecruitPtr(dwRecruitID);
    if (pRecruit) {
        if (pRecruit->GetMasterID() == stPartyLeave.dwLeaveMember) {
            // Master 退出：清除整个 recruit
            relayServer.GetPartyMatchingMgr().ClearRecruitDate(pRecruit->GetMasterID());
        } else {
            // 普通成员退出：清除 CID 并从 recruit 列表移除
            pRecruit->SetCID(0u);
            const std::uint32_t dwRecruitIDForDelete = relayServer.GetPartyMatchingMgr().FindRecruitID(pRecruit->GetMasterID());
            relayServer.GetPartyMatchingMgr().DeleteRecruitMember(dwRecruitIDForDelete, stPartyLeave.dwLeaveMember);
        }
    }

    // 从内存索引中删除 party
    DeleteParty(stPartyLeave.dwPartyID);
}

void CPartyManager::ResLoadPartyAll(PS_PARTY_INFO_ALL& stPartyInfoAll, std::uint8_t byEnd) {
    for (const auto& partyInfo : stPartyInfoAll.vecPartyInfo) {
        auto pParty = std::make_shared<CParty>();
        pParty->SetPartyInfo(partyInfo);

        m_mapParty[partyInfo.dwPartyID] = pParty;

        // 添加成员索引
        for (const auto& member : partyInfo.vecPartyMember) {
            AddPartyMember(partyInfo.dwPartyID, member.dwMemberID);
        }
    }

    if (byEnd) {
        m_bLoadParty = true;
        TXSingleton<XRelayServer>::Instance()->LoadForceDataReq();
    }

    LogHelper::LogInfo("game.system",
                       "<PARTY> LOAD PARTY INFO ALL ( COUNT : %d )",
                       static_cast<int>(stPartyInfoAll.vecPartyInfo.size()));
}

void CPartyManager::CreatePartyMatching(PS_PARTY_INFO& stCreateParty) {
    // 创建 CParty 对象并设置信息
    auto pParty = std::make_shared<CParty>();
    pParty->SetPartyInfo(stCreateParty);

    // 插入 party 索引
    m_mapParty[stCreateParty.dwPartyID] = pParty;

    // 遍历成员并添加索引
    for (const auto& member : stCreateParty.vecPartyMember) {
        AddPartyMember(stCreateParty.dwPartyID, member.dwMemberID);
    }
}

void CPartyManager::SetMaze(std::uint32_t dwPartyID, UXMapID uxMapID, UXMapID uxBeforeMapID) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    // 查找 party
    const auto it = m_mapParty.find(dwPartyID);
    if (it == m_mapParty.end() || !it->second) {
        return;
    }

    std::shared_ptr<CParty> pParty = it->second;

    // 条件：新的 mapID 非零，或者当前 MazeID 等于 beforeMapID
    UXMapID currentMazeID = pParty->GetMazeID();
    if (uxMapID.nMapID != 0 || currentMazeID.nMapID == uxBeforeMapID.nMapID) {
        // 更新 party 的 MazeID
        pParty->SetMazeID(uxMapID);

        // 发送 DB 更新 (0x04/0x08)
        IXObject* pObject = nullptr;
        XSendDBPacket xSendDBPacket(pObject, 4u, 8u);
        xSendDBPacket.XParse << dwPartyID;
        xSendDBPacket.XParse << uxMapID.nMapID;
        xSendDBPacket.XParse << 0;
        relayServer.SendDBGame(xSendDBPacket);

        // 广播 0xF4/0x09 (party maze info)
        XSendPacket sendPacket(0xF4u, 9u);
        sendPacket.XParse << dwPartyID;
        sendPacket.XParse << uxMapID.nMapID;
        relayServer.SendPacketAll(sendPacket);
    }
}

bool CPartyManager::IsParty(std::uint32_t dwActorID) {
    UXActorID uxActorID{};
    uxActorID.dwActorID = dwActorID;
    const auto it = m_mapPartyUser.find(uxActorID);
    return it != m_mapPartyUser.end();
}

void CPartyManager::SendPartyErrorInvite(CServer* pServer, PS_REQ_PARTY_INVITE& stPartyInvite, int nErrorCode) {
    stPartyInvite.nResult = nErrorCode;
    XSendPacket sendPacket(0xF4u, 0x11u);
    sendPacket << stPartyInvite;
    sendPacket.XParse << 0;
    sendPacket.XParse << 0;
    sendPacket.XParse << 0;
    pServer->SendEx(sendPacket);
}

void CPartyManager::ReqInviteParty(CServer* pServer, PS_REQ_PARTY_INVITE& stPartyInvite, int dwUAID, std::uint8_t byLevel, std::uint32_t dwPartyID) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    // Find invite target user by name
    const std::shared_ptr<CUserObject> pInviteUser = relayServer.GetUser(stPartyInvite.strName);
    if (!pInviteUser) {
        SendPartyErrorInvite(pServer, stPartyInvite, 53011);
        return;
    }

    // Set invite actor ID (GetCID returns the actor ID)
    stPartyInvite.dwInviteActorID = pInviteUser->GetCID();

    // Get invite user's party info
    const std::shared_ptr<CUserPartyInfo> pInvitePartyUser = relayServer.GetPartyUser(pInviteUser->GetCID());
    if (pInvitePartyUser) {
        // Check reward state
        if (pInvitePartyUser->GetRewardState()) {
            SendPartyErrorInvite(pServer, stPartyInvite, 53014);
            return;
        }
    }

    // Check if target is already in a party
    if (IsParty(pInviteUser->GetCID())) {
        SendPartyErrorInvite(pServer, stPartyInvite, 53004);
        return;
    }

    // Check block list
    if (relayServer.IsFriendBlock(stPartyInvite.dwInviteActorID, stPartyInvite.strReqName)) {
        SendPartyErrorInvite(pServer, stPartyInvite, 53014);
        return;
    }

    // Check recruit info - error code 53160 (0xCFA8) for type mismatch
    ST_PARTY_RECRUIT_INFO stPartyRecruit{};
    if (relayServer.GetPartyMatchingMgr().GetPartyRecruitInfo(stPartyInvite.dwReqActorID, stPartyRecruit)) {
        if (stPartyRecruit.stRecruit.byPartyGroupType != 1) {
            SendPartyErrorInvite(pServer, stPartyInvite, 53160);
            return;
        }
    }

    // Check invite cooldown using GetTickCount64
    const auto itInvite = m_mapPartyInvite.find(stPartyInvite.dwInviteActorID);
    if (itInvite != m_mapPartyInvite.end()) {
        std::uint64_t currentTick = static_cast<std::uint64_t>(GetTickCount64());
        if (currentTick < itInvite->second.dwLimitTime) {
            // Check if same inviter or different inviter
            if (itInvite->second.dwMasterID == stPartyInvite.dwReqActorID) {
                // Same inviter cooldown - error code 53015 (0xCF17)
                SendPartyErrorInvite(pServer, stPartyInvite, 53015);
            } else {
                // Different inviter cooldown - error code 53018 (0xCF1A)
                SendPartyErrorInvite(pServer, stPartyInvite, 53018);
            }
            return;
        }
        m_mapPartyInvite.erase(itInvite);
    }

    // Second party-membership check after cooldown - error code 53004
    if (IsParty(pInviteUser->GetCID())) {
        SendPartyErrorInvite(pServer, stPartyInvite, 53004);
        return;
    }

    // Check if target is in maze - error code 53001 (0xCF09)
    if (pInviteUser->IsMaze()) {
        SendPartyErrorInvite(pServer, stPartyInvite, 53001);
        return;
    }

    // Log invite
    LogHelper::LogDebug("game.relay", "<PARTY> Invite %d", static_cast<int>(stPartyInvite.dwInviteActorID));

    // Add to invite map using GetTickCount64 + 60000 (60 seconds)
    ST_INVITE_INFO stInviteInfo{};
    stInviteInfo.dwMasterID = stPartyInvite.dwReqActorID;
    stInviteInfo.dwLimitTime = static_cast<std::uint64_t>(GetTickCount64() + 60000);
    m_mapPartyInvite[stPartyInvite.dwInviteActorID] = stInviteInfo;

    // Send invite packet to target with trailing fields: dwUAID, byLevel, dwPartyID
    XSendPacket sendPacket(0xF4u, 0x11u);
    sendPacket << stPartyInvite;
    sendPacket.XParse << static_cast<std::uint32_t>(dwUAID);
    sendPacket.XParse << byLevel;
    sendPacket.XParse << dwPartyID;
    pInviteUser->SendPacket(sendPacket);
}
