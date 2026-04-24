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
        partySlot = std::make_shared<CParty>();  // 对齐 IDA: 使用默认构造
        partySlot->GreenDamTan_SetPartyID(dwPartyID);
        partySlot->SetMasterID(dwPartyID);
    }
    return partySlot;
}

std::shared_ptr<CParty> CPartyManager::GetParty(std::uint32_t dwPartyID) {
    const auto it = m_mapParty.find(dwPartyID);
    return it == m_mapParty.end() ? std::shared_ptr<CParty>{} : it->second;
}

// 对齐 IDA 0x1400955D0: 通过 UXActorID 查找队伍
std::shared_ptr<CParty> CPartyManager::GetParty(UXActorID uxActorID) {
    const auto it = m_mapPartyUser.find(uxActorID);
    if (it == m_mapPartyUser.end()) {
        return std::shared_ptr<CParty>{};
    }
    return GetParty(it->second);
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
            pMaster->GetPartyMemberInfo(&stCreateParty.masterInfo);
            pMember->GetPartyMemberInfo(&stCreateParty.memberInfo);

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
            pMaster->GetPartyMemberInfo(reinterpret_cast<ST_PARTY_MEMBER*>(&stCreateForce.masterInfo));
            pMember->GetPartyMemberInfo(reinterpret_cast<ST_PARTY_MEMBER*>(&stCreateForce.memberInfo));

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
    for (auto& partyInfo : stPartyInfoAll.vecPartyInfo) {
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

bool CPartyManager::CreatePartyMatching(PS_PARTY_INFO stCreateParty) {
    // 创建 CParty 对象并设置信息
    auto pParty = std::make_shared<CParty>();
    pParty->SetPartyInfo(stCreateParty);

    // 插入 party 索引
    m_mapParty[stCreateParty.dwPartyID] = pParty;

    // 遍历成员并添加索引
    for (const auto& member : stCreateParty.vecPartyMember) {
        AddPartyMember(stCreateParty.dwPartyID, member.dwMemberID);
    }

    return true;  // 对齐 IDA 0x140098DD0: 返回 bool
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

void CPartyManager::ReqInviteParty(CServer* pServer, PS_REQ_PARTY_INVITE& stPartyInvite, std::uint32_t dwUAID, std::uint8_t byLevel, std::uint32_t dwPartyID, std::uint8_t bySelect) {
    // 对齐 IDA KEKE: bySelect 参数暂未使用，保留以匹配签名
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
    // 对齐 IDA KEKE: bySelect 参数暂未发送，保留以备后续使用
    XSendPacket sendPacket(0xF4u, 0x11u);
    sendPacket << stPartyInvite;
    sendPacket.XParse << dwUAID;
    sendPacket.XParse << byLevel;
    sendPacket.XParse << dwPartyID;
    pInviteUser->SendPacket(sendPacket);
}

// ============================================================================
// 对齐 IDA: 新增队伍管理方法
// ============================================================================

void CPartyManager::ReqPartyLeave(CServer* pServer, PS_PARTY_LEAVE& stLeave,
                                    std::uint32_t dwActorID, std::uint32_t dwUAID,
                                    std::uint8_t byLevel, std::uint8_t byLeaverLevel) {
    // 对齐 IDA 0x140097830: 成员离开队伍 (完整实现)
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    // 查找队伍
    auto itParty = m_mapParty.find(stLeave.dwPartyID);
    if (itParty == m_mapParty.end() || !itParty->second) {
        return;
    }

    std::shared_ptr<CParty> pParty = itParty->second;

    // 检查队伍人数：如果 <= 2 人，解散队伍
    if (pParty->GetUserCount() <= 2) {
        ReqDeleteParty(pServer, stLeave, dwActorID, dwUAID, byLevel, byLeaverLevel);
        return;
    }

    std::uint32_t dwNewMaster = pParty->GetMasterID();

    // 检查离开的是否是队长
    if (pParty->GetMasterID() == stLeave.dwLeaveMember) {
        // 队长离开，需要选择新队长
        dwNewMaster = pParty->FindNewMaster();
        if (dwNewMaster == 0) {
            LogHelper::LogError("game.relay", "<PARTY> Cant Find New Master ( PID %d )", stLeave.dwPartyID);
            return;
        }

        // 变更队长
        pParty->SetMasterID(dwNewMaster);

        // 清除原队长的招募状态
        relayServer.GetPartyMatchingMgr().ClearRecruitDate(stLeave.dwLeaveMember);
    }

    // 从队伍用户索引中移除
    UXActorID uxLeaveMember{};
    uxLeaveMember.dwActorID = stLeave.dwLeaveMember;
    m_mapPartyUser.erase(uxLeaveMember);

    // 从队伍成员列表中移除 (Kickout 会执行 erase)
    // GreenDamTan: 原代码有 RemoveMember 调用，但 IDA 中 Kickout 已包含 erase 逻辑

    // 从招募列表中移除
    const std::uint32_t dwMasterID = pParty->GetMasterID();
    const std::uint32_t dwRecruitID = relayServer.GetPartyMatchingMgr().FindRecruitID(dwMasterID);
    relayServer.GetPartyMatchingMgr().DeleteRecruitMember(dwRecruitID, stLeave.dwLeaveMember);

    // 踢出成员
    pParty->Kickout(stLeave.dwLeaveMember);

    // 发送 DB 请求
    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 4u, 3u);
    xSendDBPacket << stLeave;
    xSendDBPacket.XParse << dwNewMaster;
    relayServer.SendDBGame(xSendDBPacket);

    // 发送 DB 日志
    relayServer.SendDBLog(
        static_cast<int>(dwUAID),
        static_cast<int>(dwActorID),
        22,
        7,
        static_cast<int>(byLevel),
        static_cast<int>(stLeave.dwLeaveMember),
        static_cast<int>(byLeaverLevel),
        static_cast<int>(pParty->GetUserCount()),
        stLeave.bKickout ? 1 : 0,
        static_cast<int>(dwNewMaster),
        static_cast<int>(stLeave.dwPartyID),
        L"");

    // 如果是踢出操作，发送额外的日志给被踢出者
    if (stLeave.bKickout) {
        const std::shared_ptr<CUserObject> pUser = relayServer.GetUser(stLeave.dwLeaveMember);
        if (pUser) {
            relayServer.SendDBLog(
                static_cast<int>(pUser->GetUAID()),
                static_cast<int>(stLeave.dwLeaveMember),
                22,
                13,
                static_cast<int>(byLevel),
                static_cast<int>(dwActorID),
                0,
                0,
                0,
                0,
                static_cast<int>(stLeave.dwPartyID),
                L"");
        }
    }
}

void CPartyManager::ReqDeleteParty(CServer* pServer, PS_PARTY_LEAVE& stLeave,
                                    std::uint32_t dwActorID, std::uint32_t dwUAID,
                                    std::uint8_t byLevel, std::uint8_t byLeaverLevel) {
    // 对齐 IDA 0x140098280: 解散队伍
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    // 删除队伍
    DeleteParty(stLeave.dwPartyID);

    // 发送 DB 请求
    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 4u, 6u);
    xSendDBPacket << stLeave;
    relayServer.SendDBGame(xSendDBPacket);

    // 判断是否为踢出操作
    const bool bKickout = (dwActorID != stLeave.dwLeaveMember);

    // 发送 DB 日志 (22/7)
    relayServer.SendDBLog(
        static_cast<int>(dwUAID),
        static_cast<int>(dwActorID),
        22,
        7,
        static_cast<int>(byLevel),
        static_cast<int>(stLeave.dwLeaveMember),
        static_cast<int>(byLeaverLevel),
        0,
        bKickout ? 1 : 0,
        0,
        static_cast<int>(stLeave.dwPartyID),
        L"");

    // 如果是踢出操作，发送额外的日志给被踢出者
    if (bKickout) {
        const std::shared_ptr<CUserObject> pUser = relayServer.GetUser(stLeave.dwLeaveMember);
        if (pUser) {
            relayServer.SendDBLog(
                static_cast<int>(pUser->GetUAID()),
                static_cast<int>(stLeave.dwLeaveMember),
                22,
                13,
                static_cast<int>(byLevel),
                static_cast<int>(dwActorID),
                0,
                0,
                0,
                0,
                static_cast<int>(stLeave.dwPartyID),
                L"");
        }
    }

    // 发送 DB 日志 (22/8) - 队伍解散
    relayServer.SendDBLog(
        static_cast<int>(dwUAID),
        static_cast<int>(dwActorID),
        22,
        8,
        static_cast<int>(byLevel),
        0,
        0,
        0,
        0,
        0,
        static_cast<int>(stLeave.dwPartyID),
        L"");
}

void CPartyManager::ReqChangeMaster(CServer* pServer, PS_PARTY_CHANGE_MASTER& stChangeMaster) {
    // 对齐 IDA 0x140097E50: 变更队长 (完整实现)
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    int nErrorCode = 0;

    // 查找队伍
    auto itParty = m_mapParty.find(stChangeMaster.dwPartyID);
    if (itParty != m_mapParty.end() && itParty->second) {
        std::shared_ptr<CParty> pParty = itParty->second;

        // 尝试变更队长
        // GreenDamTan: 原调用 ChangeMaster 但 IDA 中无此方法，直接设置 MasterID
        pParty->SetMasterID(stChangeMaster.dwNewMasterID);

        // 成功，发送 DB 请求
        IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
        XSendDBPacket xSendDBPacket(pObject, 4u, 5u);
        xSendDBPacket << stChangeMaster;
        relayServer.SendDBGame(xSendDBPacket);
    } else {
        nErrorCode = 2;  // 队伍不存在
    }

    // 如果出错，发送错误包
    if (nErrorCode != 0) {
        PS_PARTY_CHANGE_MASTER stChangeMasterRes = stChangeMaster;
        stChangeMasterRes.nErrorCode = nErrorCode;

        XSendPacket sendPacket(0xF4u, 4u);
        sendPacket << stChangeMasterRes;
        if (pServer) {
            pServer->SendEx(sendPacket);
        }
        LogHelper::LogInfo("game.relay", "<PARTY> ReqChangeMaster ( ErrorCode : %d )", nErrorCode);
    }
}

void CPartyManager::SendPartyErrorAccept(CServer* pServer, std::uint32_t dwActorID, int nErrorCode) {
    // 对齐 IDA 0x140098D20: 发送队伍接受错误
    PS_RES_PARTY_ACCEPT stPartyAccept{};
    stPartyAccept.dwAcceptID = dwActorID;
    stPartyAccept.nResult = nErrorCode;

    XSendPacket sendPacket(0xF4u, 0x12u);
    sendPacket << stPartyAccept;
    if (pServer) {
        pServer->SendEx(sendPacket);
    }
}

void CPartyManager::ReqAcceptParty(CServer* pServer, PS_RES_PARTY_INVITE& stAccept,
                                    std::uint32_t dwUAID, std::uint8_t byLevel) {
    // 对齐 IDA 0x140096130: 接受队伍邀请 (完整实现)
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    // 查找邀请记录
    auto itInvite = m_mapPartyInvite.find(stAccept.dwAcceptID);
    if (itInvite == m_mapPartyInvite.end()) {
        return;
    }

    // 仅在 nResult == 0 时处理接受逻辑
    if (stAccept.nResult == 0) {
        // 复制邀请信息
        ST_INVITE_INFO stInviteInfo = itInvite->second;

        // 检查邀请是否过期
        if (stInviteInfo.dwLimitTime >= static_cast<std::uint64_t>(GetTickCount64())) {
            // 获取邀请者（队长）信息
            const std::shared_ptr<CUserPartyInfo> pMasterPartyInfo = relayServer.GetPartyUser(stInviteInfo.dwMasterID);
            const std::shared_ptr<CUserObject> pMaster = relayServer.GetUser(stInviteInfo.dwMasterID);

            if (!pMasterPartyInfo || !pMaster) {
                SendPartyErrorAccept(pServer, stAccept.dwAcceptID, 53011);
                LogHelper::LogError("game.relay", "<PARTY> ReqAcceptParty::pMaster = NULL %d", stInviteInfo.dwMasterID);
            } else {
                // 检查招募类型
                ST_PARTY_RECRUIT_INFO stPartyRecruit{};
                if (relayServer.GetPartyMatchingMgr().GetPartyRecruitInfo(stInviteInfo.dwMasterID, stPartyRecruit)
                    && stPartyRecruit.stRecruit.byPartyGroupType != 1) {
                    // 非队伍类型，返回错误
                    SendPartyErrorAccept(pServer, stAccept.dwAcceptID, 53016);
                } else if (pMasterPartyInfo->GetRewardState()) {
                    // 邀请者正在匹配中，移除邀请并返回错误
                    m_mapPartyInvite.erase(stAccept.dwAcceptID);
                    SendPartyErrorAccept(pServer, stAccept.dwAcceptID, 53028);
                    LogHelper::LogError("game.relay",
                                        "<PARTY> ReqAcceptParty::Requestor is matching on party = NULL %d",
                                        stInviteInfo.dwMasterID);
                    return;
                } else {
                    // 检查邀请者是否已在 Force 中
                    const std::uint32_t dwMasterMatchingID = pMaster->GetMatchingID();
                    if (IsParty(dwMasterMatchingID)) {
                        SendPartyErrorAccept(pServer, stAccept.dwAcceptID, 53016);
                        LogHelper::LogError("game.relay", "<PARTY> ReqAcceptParty::In Force %d", stInviteInfo.dwMasterID);
                    } else {
                        // 获取接受者信息
                        const std::shared_ptr<CUserPartyInfo> pMemberPartyInfo = relayServer.GetPartyUser(stAccept.dwAcceptID);
                        const std::shared_ptr<CUserObject> pMember = relayServer.GetUser(stAccept.dwAcceptID);

                        if (!pMemberPartyInfo || !pMember) {
                            SendPartyErrorAccept(pServer, stAccept.dwAcceptID, 53011);
                            LogHelper::LogError("game.relay", "<PARTY> ReqAcceptParty::dwAcceptID = NULL %d", stAccept.dwAcceptID);
                        } else {
                            // 检查接受者是否已在队伍中
                            const std::uint32_t dwMemberMatchingID = pMember->GetMatchingID();
                            if (IsParty(dwMemberMatchingID)) {
                                SendPartyErrorAccept(pServer, stAccept.dwAcceptID, 53016);
                                LogHelper::LogError("game.relay", "<PARTY> ReqAcceptParty::In Party %d", stAccept.dwAcceptID);
                            } else if (IsParty(pMemberPartyInfo->GetMatchingID())) {
                                // 检查接受者是否在 Force 中
                                SendPartyErrorAccept(pServer, stAccept.dwAcceptID, 53016);
                                LogHelper::LogError("game.relay", "<PARTY> ReqAcceptParty::In Force %d", stAccept.dwAcceptID);
                            } else if (pMaster->IsMaze() || pMember->IsMaze()) {
                                // 检查是否在迷宫中
                                SendPartyErrorAccept(pServer, stAccept.dwAcceptID, 53016);
                                LogHelper::LogError("game.relay", "<PARTY> ReqAcceptParty::In Maze %d", stAccept.dwAcceptID);
                            } else {
                                // 检查邀请者是否已有队伍
                                UXActorID uxMasterID{};
                                uxMasterID.dwActorID = stInviteInfo.dwMasterID;
                                const auto itUser = m_mapPartyUser.find(uxMasterID);

                                if (itUser != m_mapPartyUser.end()) {
                                    // 邀请者已有队伍 - 加入现有队伍
                                    const std::uint32_t dwPartyID = itUser->second;
                                    const auto itParty = m_mapParty.find(dwPartyID);

                                    if (itParty == m_mapParty.end() || !itParty->second) {
                                        SendPartyErrorAccept(pServer, stAccept.dwAcceptID, 53016);
                                        LogHelper::LogError("game.relay", "<PARTY> ReqAcceptParty::No Party Info ");
                                    } else {
                                        const std::shared_ptr<CParty> pParty = itParty->second;

                                        // 验证队长身份
                                        if (pParty->GetMasterID() != stInviteInfo.dwMasterID) {
                                            SendPartyErrorAccept(pServer, stAccept.dwAcceptID, 53016);
                                            LogHelper::LogError("game.relay", "<PARTY> ReqAcceptParty::Already Make Party!");
                                        } else if (pParty->GetUserCount() >= 4) {
                                            SendPartyErrorAccept(pServer, stAccept.dwAcceptID, 53010);
                                            LogHelper::LogError("game.relay", "<PARTY> ReqAcceptParty::Many party ");
                                        } else {
                                            // 获取成员信息并加入队伍
                                            ST_PARTY_MEMBER stPartyMemberInfo{};
                                            pMember->GetPartyMemberInfo(&stPartyMemberInfo);

                                            PS_PARTY_ADDMEMBER stAddMember{};
                                            std::memcpy(&stAddMember.stMember, &stPartyMemberInfo, sizeof(ST_PARTY_MEMBER));
                                            stAddMember.dwPartyID = dwPartyID;
                                            stAddMember.stMember.uxMapID = stPartyMemberInfo.uxMapID;

                                            const std::uint32_t dwMasterRecruitID =
                                                relayServer.GetPartyMatchingMgr().FindRecruitID(stInviteInfo.dwMasterID);
                                            ReqJoinMember(pServer, stAddMember, dwUAID, byLevel, dwMasterRecruitID);
                                        }
                                    }
                                } else {
                                    // 邀请者没有队伍 - 创建新队伍
                                    PS_REQ_PARTY_CREATE stCreateParty{};
                                    pMaster->GetPartyMemberInfo(&stCreateParty.masterInfo);
                                    pMember->GetPartyMemberInfo(&stCreateParty.memberInfo);
                                    stCreateParty.dwMasterUAID = pMaster->GetUAID();

                                    const std::uint32_t dwMasterUCID = pMasterPartyInfo->GetActorID();
                                    stCreateParty.dwRecruitID =
                                        relayServer.GetPartyMatchingMgr().FindRecruitID(dwMasterUCID);

                                    const std::uint32_t dwMemberCID = pMemberPartyInfo->GetActorID();
                                    relayServer.GetPartyMatchingMgr().ClearRecruitDate(dwMemberCID);
                                    pMemberPartyInfo->ClearRecruitParty(true);
                                    pMasterPartyInfo->ClearRecruitParty(true);

                                    ReqCreateParty(pServer, stCreateParty);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // 无论接受或拒绝，移除邀请记录
    m_mapPartyInvite.erase(stAccept.dwAcceptID);
}

void CPartyManager::ReqCancelParty(CServer* pServer, PS_PARTY_REJECT& stReject) {
    // 对齐 IDA 0x140096E00: 取消/拒绝队伍邀请

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    // 查找邀请记录
    auto itInvite = m_mapPartyInvite.find(stReject.dwRejectID);
    if (itInvite == m_mapPartyInvite.end()) {
        return;
    }

    // 复制邀请信息
    ST_INVITE_INFO stInviteInfo = itInvite->second;

    // 通知邀请者（队长）
    const std::shared_ptr<CUserObject> pMaster = relayServer.GetUser(stInviteInfo.dwMasterID);
    if (pMaster) {
        XSendPacket sendPacket(0xF4u, 0x13u);
        sendPacket << stReject;
        pMaster->SendPacket(sendPacket);
    }

    // 移除邀请记录
    m_mapPartyInvite.erase(itInvite);
}

void CPartyManager::SendPartyMessage(PS_CHAT_PARTY& stChatParty, PS_CHAT_ITEM_LINK_FOR_SERVER stItemLink) {
    // 对齐 IDA 0x140098B80: 第一参数引用，第二参数按值传递
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    XSendPacket sendPacket(0xF4u, 0x14u);
    sendPacket << stChatParty;
    sendPacket << stItemLink;
    relayServer.SendPacketAll(sendPacket);
}

bool CPartyManager::ReqPartyMazeClear(std::uint32_t dwPartyID) {
    // 对齐 IDA 0x140098FC0: 迷宫通关清除，返回 bool，单参数
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    // 查找队伍
    const auto it = m_mapParty.find(dwPartyID);
    if (it == m_mapParty.end() || !it->second) {
        return false;
    }

    std::shared_ptr<CParty> pParty = it->second;

    // 清除队伍的迷宫状态
    pParty->SetMazeID(UXMapID{});

    // 广播迷宫清除通知
    XSendPacket sendPacket(0xF4u, 0x43u);
    sendPacket.XParse << dwPartyID;
    relayServer.SendPacketAll(sendPacket);

    return true;
}

void CPartyManager::EnterServer(CServer* pServer, PS_REQ_PARTY_ENTER_SERVER& stEnterServer) {
    // 对齐 IDA 0x140096FB0: 进入服务器时更新成员信息
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    // 如果队伍 ID 为 0，直接返回
    if (stEnterServer.dwPartyID == 0) {
        return;
    }

    // 查找队伍
    const auto it = m_mapParty.find(stEnterServer.dwPartyID);
    if (it == m_mapParty.end() || !it->second) {
        LogHelper::LogError("game.relay",
                            "<PARTY> Error Enter Server ( PID %d / UCID %d ) ",
                            static_cast<int>(stEnterServer.dwPartyID),
                            static_cast<int>(stEnterServer.dwMemberID));
        return;
    }

    std::shared_ptr<CParty> pParty = it->second;

    // 更新成员信息
    pParty->SetMemberInfo(stEnterServer.dwMemberID, stEnterServer.uxMapID, stEnterServer.nMaxHP);

    // 构建响应
    PS_RES_PARTY_ENTER_SERVER stEnterServerRes{};
    stEnterServerRes.bLoadParty = false;

    // 如果需要队伍信息，获取队伍详情
    if (stEnterServer.bReqPartyInfo) {
        pParty->GetPartyInfo(stEnterServerRes.stPartyInfo);
        stEnterServerRes.bLoadParty = true;
    }

    stEnterServerRes.stPartyInfo.dwPartyID = stEnterServer.dwPartyID;

    // 获取成员信息
    pParty->GreenDamTan_GetMemberInfo(stEnterServer.dwMemberID, &stEnterServerRes.stEnterMember);

    // 如果需要队伍信息，发送给客户端
    if (stEnterServer.bReqPartyInfo) {
        XSendPacket sendPacket(0xF4u, 0x10u);
        sendPacket << stEnterServerRes;
        if (pServer) {
            pServer->SendEx(sendPacket);
        }
    }

    // 如果成员是队长，发送招募信息
    if (pParty->GetMasterID() == stEnterServer.dwMemberID) {
        ST_PARTY_RECRUIT_INFO stRecruit{};
        relayServer.GetPartyMatchingMgr().GetPartyRecruitInfo(pParty->GetMasterID(), stRecruit);

        XSendPacket xSendPacket(0xF4u, 0x2Eu);
        xSendPacket.XParse << stEnterServer.dwMemberID;
        xSendPacket << stRecruit.stRecruit;
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
    }
}

// 对齐 IDA 0x140098990
void CPartyManager::ReqUpdateMemberInfo(CServer* pServer, ST_UPDATE_PARTY_MEMBER& stPartyMember) {
    // 查找 party
    const auto it = m_mapParty.find(stPartyMember.dwPartyID);
    if (it != m_mapParty.end() && it->second) {
        it->second->GreenDamTan_SetMemberInfo(stPartyMember.stPartyMember);
    }

    // 发送 DB 更新 (0x04/0x04)
    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 4u, 4u);
    xSendDBPacket << stPartyMember;
    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
}

// 对齐 IDA 0x140099500
void CPartyManager::SendPartyNameChange(std::uint32_t dwPartyID, std::uint32_t dwActorID, const wchar_t* pChangeName) {
    const auto it = m_mapParty.find(dwPartyID);
    if (it != m_mapParty.end() && it->second) {
        it->second->SendNameChange(dwActorID, pChangeName);
    }
}
