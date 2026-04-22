#include "Soulworker/GameServer/XRelayServer/PartyProcess.h"

#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h"

bool CPartyProcess::DispatchPartyJob(const std::function<void()>& job) {
    if (!job) {
        return false;
    }
    return CLogicThreadManager::Instance().DoJob(0, job);
}

bool CPartyProcess::Parse(XPacket& xPacket) {
    // 对齐 IDA 0x1400A1D40: 完整 sub switch
    switch (static_cast<unsigned char>(xPacket.GetSubCmd())) {
    case 0x01:
        return ReqPartyCreate(xPacket);
    case 0x03:
        return ReqPartyLeaveMember(xPacket);
    case 0x04:
        return ReqPartyChangeMaster(xPacket);
    case 0x05:
        return ReqPartyUpdateMember(xPacket);
    case 0x10:
        return ReqPartyEnterServer(xPacket);
    case 0x11:
        return ReqPartyInvite(xPacket);
    case 0x12:
        return ReqPartyAccept(xPacket);
    case 0x13:
        return ReqPartyCancel(xPacket);
    case 0x14:
        return SyncPartyMessage(xPacket);
    case 0x20:
        return ReqPartyMatchingEnter(xPacket);
    case 0x21:
        return ReqPartyMatchingExit(xPacket);
    case 0x22:
        return ReqPartyMatchingCheck(xPacket);
    case 0x25:
        return ReqPartyRecruitAdd(xPacket);
    case 0x26:
        return ReqPartyRecruitDel(xPacket);
    case 0x27:
        return ReqPartyRecruitApply(xPacket);
    case 0x28:
        return ReqPartyRecruitApplyAccept(xPacket);
    case 0x29:
        return ReqPartyRecruitApplyReject(xPacket);
    case 0x2B:
        return ReqPartyRecruitList(xPacket);
    case 0x2C:
        return ReqPartyRecruitMyApplyList(xPacket);
    case 0x2D:
        return ReqPartyRecruitApplyList(xPacket);
    case 0x2F:
        return ReqPartyRecruitApplyDel(xPacket);
    case 0x30:
        return ReqPartyRecruitApplyInfo(xPacket);
    case 0x32:
        return ResPartyRecruitApplyAcceptCheck(xPacket);
    case 0x40:
        return ReqPartyInfo(xPacket);
    case 0x43:
        return ReqPartyMazeClear(xPacket);
    default:
        return true;
    }
}

bool CPartyProcess::ReqPartyUpdateMember(XPacket& xPacket) {
    ST_UPDATE_PARTY_MEMBER partyMember{};
    xPacket >> partyMember;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([partyMember, server]() mutable {
        IXObject* objectOwner = server ? static_cast<IXObject*>(server) : nullptr;
        XSendDBPacket sendPacket(objectOwner, 4u, 4u);
        sendPacket << partyMember;
        TXSingleton<XRelayServer>::Instance()->SendDBGame(sendPacket);
    });
}

bool CPartyProcess::ReqPartyEnterServer(XPacket& xPacket) {
    PS_REQ_PARTY_ENTER_SERVER enterServer{};
    xPacket >> enterServer;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([enterServer, server]() {
        LogHelper::LogDebug(
            "game.relay",
            "GreenDamTan_log PartyProcess.cpp::CPartyProcess::ReqPartyEnterServer member=%u party=%u map=%lld maxHP=%d reqInfo=%u server=%p",
            static_cast<unsigned int>(enterServer.dwMemberID),
            static_cast<unsigned int>(enterServer.dwPartyID),
            static_cast<long long>(enterServer.uxMapID.nMapID),
            enterServer.nMaxHP,
            static_cast<unsigned int>(enterServer.bReqPartyInfo ? 1u : 0u),
            static_cast<void*>(server));
    });
}

bool CPartyProcess::ReqPartyRecruitAdd(XPacket& xPacket) {
    PS_SERVER_PARTY_RECRUIT_ADD_REQ recruitAdd{};
    xPacket >> recruitAdd;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([recruitAdd, server]() {
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }

        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        const std::shared_ptr<CUserPartyInfo> userParty = relayServer.GetPartyUser(recruitAdd.dwUCID);
        if (!userParty) {
            return;
        }

        PS_SERVER_PARTY_RECRUIT_ADD_RES recruitRes{};
        recruitRes.stRecruitInfo.stRecruit = recruitAdd.stRecruit;
        recruitRes.stRecruitInfo.stRecruit.nRemainTime = 1800;
        recruitRes.dwUCID = recruitAdd.dwUCID;
        recruitRes.nResult = 1;

        if (userParty->GetRemainRecruitPenalty() > 0) {
            recruitRes.nResult = 53024;
            recruitRes.nRemainSec = static_cast<int>(userParty->GetRemainRecruitPenalty());
        } else if (relayServer.GetPartyMatchingMgr().GetPartyRecruitInfo(recruitAdd.dwUCID, recruitRes.stRecruitInfo)) {
            recruitRes.nResult = 53024;
        } else if (recruitAdd.stRecruit.byPartyGroupType != 1 && recruitAdd.stRecruit.byPartyGroupType != 2) {
            recruitRes.nResult = 53012;
        } else {
            relayServer.GetPartyMatchingMgr().ReqPartyRecruitDel(recruitAdd.dwUCID);
            ST_PARTY_RECRUIT stRecruit = recruitAdd.stRecruit;  // 复制以允许修改
            if (relayServer.GetPartyMatchingMgr().ReqPartyRecruitCreate(userParty,
                                                                        stRecruit,
                                                                        &recruitRes.dwRecruitID)) {
                relayServer.GetPartyMatchingMgr().GetPartyRecruitInfo(recruitAdd.dwUCID, recruitRes.stRecruitInfo);

                if (recruitRes.stRecruitInfo.stRecruit.byPartyGroupType == 1) {
                    relayServer.SendDBLog(recruitAdd.dwUAID,
                                          recruitAdd.dwUCID,
                                          22,
                                          1,
                                          recruitAdd.byLevel,
                                          0,
                                          0,
                                          0,
                                          0,
                                          0,
                                          static_cast<std::int64_t>(recruitRes.stRecruitInfo.stRecruit.dwPartyID),
                                          L"");
                } else if (recruitRes.stRecruitInfo.stRecruit.byPartyGroupType == 2) {
                    relayServer.SendDBLog(recruitAdd.dwUAID,
                                          recruitAdd.dwUCID,
                                          23,
                                          1,
                                          recruitAdd.byLevel,
                                          0,
                                          0,
                                          0,
                                          0,
                                          0,
                                          static_cast<std::int64_t>(recruitRes.stRecruitInfo.stRecruit.dwPartyID),
                                          L"");
                }
            }
        }

        XSendPacket sendPacket(0xF4u, 0x25u);
        sendPacket << recruitRes;
        server->SendEx(sendPacket);
    });
}

bool CPartyProcess::ReqPartyRecruitApply(XPacket& xPacket) {
    ST_PARTY_RECRUIT_APPLY applyInfo{};
    ST_APPLY_MEMBER applyMember{};
    xPacket >> applyInfo;
    xPacket >> applyMember;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([applyInfo, applyMember, server]() mutable {
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }

        const std::uint32_t actorID = applyMember.stMember.dwMemberID;
        const std::shared_ptr<CPartyRecruit> recruit =
            TXSingleton<XRelayServer>::Instance()->GetPartyMatchingMgr().FindRecruitPtr(applyInfo.dwRecruitID);
        if (!recruit) {
            XSendPacket packet(0xF4u, 0x27u);
            packet.XParse << actorID;
            packet.XParse << 4;
            packet.XParse << applyInfo.dwRecruitID;
            server->SendEx(packet);
            LogHelper::LogError("game.contents",
                                "[PARTY_RECRUIT] Failed ReqPartyRecruitApply -  NULL == Recruit %u ",
                                static_cast<unsigned int>(applyInfo.dwRecruitID));
            return;
        }

        std::uint8_t applyCount = 5;
        const std::shared_ptr<CUserPartyInfo> partyUser = TXSingleton<XRelayServer>::Instance()->GetPartyUser(actorID);
        if (!partyUser) {
            return;
        }

        if (TXSingleton<XRelayServer>::Instance()->GetPartyMatchingMgr().FindRecruitID(actorID) != 0 ||
            !partyUser->IsApplyRecruit(applyInfo.dwRecruitID)) {
            XSendPacket packet(0xF4u, 0x27u);
            packet.XParse << actorID;
            packet.XParse << 2;
            packet.XParse << applyInfo.dwRecruitID;
            server->SendEx(packet);
            LogHelper::LogError("game.contents",
                                "[PARTY_RECRUIT] Failed IsApplyParty -  Recruit %u ",
                                static_cast<unsigned int>(applyInfo.dwRecruitID));
            return;
        }

        if (!partyUser->CanApplyRecruit()) {
            XSendPacket packet(0xF4u, 0x27u);
            packet.XParse << actorID;
            packet.XParse << 5;
            packet.XParse << applyInfo.dwRecruitID;
            server->SendEx(packet);
            LogHelper::LogError("game.contents",
                                "[PARTY_RECRUIT] Failed Count -  Recruit %u ",
                                static_cast<unsigned int>(applyInfo.dwRecruitID));
            return;
        }

        const std::uint8_t result =
            TXSingleton<XRelayServer>::Instance()->GetPartyMatchingMgr().ReqPartyRecruitApply(applyInfo, applyMember);
        if (result == 0) {
            partyUser->SetApplyRecruitID(applyInfo.dwRecruitID);
            applyCount = partyUser->GetApplyRecruitCount();
        }

        XSendPacket packet(0xF4u, 0x27u);
        packet.XParse << actorID;
        packet.XParse << result;
        packet.XParse << applyInfo.dwRecruitID;
        packet.XParse << applyCount;
        server->SendEx(packet);
    });
}

bool CPartyProcess::ReqPartyRecruitDel(XPacket& xPacket) {
    std::uint32_t actorID = 0;
    std::uint32_t uaid = 0;
    std::uint8_t byLevel = 0;
    xPacket.XParse >> actorID;
    xPacket.XParse >> uaid;
    xPacket.XParse >> byLevel;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([actorID, uaid, byLevel, server]() {
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }

        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        ST_PARTY_RECRUIT_INFO recruitInfo{};
        if (!relayServer.GetPartyMatchingMgr().GetPartyRecruitInfo(actorID, recruitInfo) ||
            recruitInfo.stRecruit.dwMasterUCID != actorID) {
            return;
        }

        if (!relayServer.GetPartyMatchingMgr().ReqPartyRecruitDel(actorID)) {
            return;
        }

        if (recruitInfo.stRecruit.byPartyGroupType == 1) {
            relayServer.SendDBLog(uaid,
                                  actorID,
                                  22,
                                  2,
                                  byLevel,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  static_cast<std::int64_t>(recruitInfo.stRecruit.dwRecruitID),
                                  L"");
        } else if (recruitInfo.stRecruit.byPartyGroupType == 2) {
            relayServer.SendDBLog(uaid,
                                  actorID,
                                  23,
                                  2,
                                  byLevel,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  static_cast<std::int64_t>(recruitInfo.stRecruit.dwRecruitID),
                                  L"");
        }
    });
}

bool CPartyProcess::ReqPartyRecruitApplyAccept(XPacket& xPacket) {
    ST_PARTY_RECRUIT_APPLY_ACCEPT_REJECT acceptInfo{};
    xPacket >> acceptInfo;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([acceptInfo, server]() {
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }

        const std::uint32_t recruitID =
            TXSingleton<XRelayServer>::Instance()->GetPartyMatchingMgr().FindRecruitID(acceptInfo.dwMasterUCID);
        if (recruitID == 0) {
            XSendPacket packet(0xF4u, 0x28u);
            packet.XParse << acceptInfo.dwMasterUCID;
            packet.XParse << 53021;
            server->SendEx(packet);
            LogHelper::LogError("game.contents",
                                "[PARTY_RECRUIT] Falild ReqPartyRecruitApplyAccept - false == PARTY_MATCHING_MGR.FindRecruitID() %u ",
                                static_cast<unsigned int>(acceptInfo.dwMasterUCID));
            return;
        }

        if (!TXSingleton<XRelayServer>::Instance()->GetPartyMatchingMgr().ReqRecruitAccept(recruitID,
                                                                                            const_cast<ST_PARTY_RECRUIT_APPLY_ACCEPT_REJECT&>(acceptInfo),
                                                                                            server)) {
            XSendPacket packet(0xF4u, 0x28u);
            packet.XParse << acceptInfo.dwMasterUCID;
            packet.XParse << 53021;
            server->SendEx(packet);
            LogHelper::LogError("game.contents",
                                "[PARTY_RECRUIT] Failed ReqPartyRecruitApplyAccept - false == PARTY_MATCHING_MGR.ReqRecruitAccept %u ",
                                static_cast<unsigned int>(recruitID));
        }
    });
}

bool CPartyProcess::ReqPartyRecruitApplyReject(XPacket& xPacket) {
    ST_PARTY_RECRUIT_APPLY_ACCEPT_REJECT rejectInfo{};
    xPacket >> rejectInfo;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([rejectInfo, server]() {
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }

        const std::uint32_t recruitID =
            TXSingleton<XRelayServer>::Instance()->GetPartyMatchingMgr().FindRecruitID(rejectInfo.dwMasterUCID);
        if (recruitID == 0) {
            XSendPacket packet(0xF4u, 0x29u);
            packet.XParse << rejectInfo.dwMasterUCID;
            packet.XParse << 0u;
            server->SendEx(packet);
            LogHelper::LogError("game.contents",
                                "[PARTY_RECRUIT] Falild ReqPartyRecruitApplyReject - false == PARTY_RECRUIT_MGR.FindRecruitID() %u ",
                                static_cast<unsigned int>(rejectInfo.dwMasterUCID));
            return;
        }

        TXSingleton<XRelayServer>::Instance()->GetPartyMatchingMgr().ReqRecruitReject(recruitID,
                                                                                       rejectInfo.dwTargetUCID);
        XSendPacket packet(0xF4u, 0x29u);
        packet.XParse << rejectInfo.dwMasterUCID;
        packet.XParse << rejectInfo.dwTargetUCID;
        server->SendEx(packet);
    });
}

bool CPartyProcess::ReqPartyRecruitList(XPacket& xPacket) {
    std::uint32_t actorID = 0;
    xPacket.XParse >> actorID;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([actorID, server]() {
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }

        TXSingleton<XRelayServer>::Instance()->GetPartyMatchingMgr().SendPartyRecruitList(actorID, server);
    });
}

bool CPartyProcess::ReqPartyRecruitMyApplyList(XPacket& xPacket) {
    std::uint32_t actorID = 0;
    xPacket.XParse >> actorID;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([actorID, server]() {
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }

        TXSingleton<XRelayServer>::Instance()->GetPartyMatchingMgr().SendPartyRecruitMyApplyList(actorID, server);
    });
}

bool CPartyProcess::ReqPartyRecruitApplyList(XPacket& xPacket) {
    std::uint32_t actorID = 0;
    xPacket.XParse >> actorID;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([actorID, server]() {
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }

        TXSingleton<XRelayServer>::Instance()->GetPartyMatchingMgr().SendPartyRecruitApplyList(actorID, server);
    });
}

bool CPartyProcess::ReqPartyRecruitApplyDel(XPacket& xPacket) {
    std::uint32_t actorID = 0;
    std::uint32_t recruitID = 0;
    xPacket.XParse >> actorID;
    xPacket.XParse >> recruitID;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([actorID, recruitID, server]() {
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }

        if (const std::shared_ptr<CUserPartyInfo> partyUser =
                TXSingleton<XRelayServer>::Instance()->GetPartyUser(actorID)) {
            partyUser->DelPartyRecruit(recruitID, true);
        }
    });
}

bool CPartyProcess::ReqPartyRecruitApplyInfo(XPacket& xPacket) {
    std::uint32_t actorID = 0;
    std::uint32_t recruitID = 0;
    xPacket.XParse >> actorID;
    xPacket.XParse >> recruitID;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([actorID, recruitID, server]() {
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }

        TXSingleton<XRelayServer>::Instance()->GetPartyMatchingMgr().SendPartyRecruitApplyInfo(actorID,
                                                                                                recruitID,
                                                                                                server);
    });
}

bool CPartyProcess::ReqPartyInfo(XPacket& xPacket) {
    std::uint32_t partyID = 0;
    std::uint32_t actorID = 0;
    xPacket.XParse >> partyID;
    xPacket.XParse >> actorID;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([partyID, actorID, server]() {
        LogHelper::LogDebug(
            "game.relay",
            "GreenDamTan_log PartyProcess.cpp::CPartyProcess::ReqPartyInfo party=%u actor=%u server=%p",
            static_cast<unsigned int>(partyID),
            static_cast<unsigned int>(actorID),
            static_cast<void*>(server));
    });
}

bool CPartyProcess::ResPartyRecruitApplyAcceptCheck(XPacket& xPacket) {
    PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK psCheck{};
    xPacket >> psCheck;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([psCheck, server]() mutable {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetPartyManager().ResRecruitAccept(server, psCheck);
    });
}

bool CPartyProcess::ReqPartyInvite(XPacket& xPacket) {
    PS_REQ_PARTY_INVITE stPartyInvite{};
    xPacket >> stPartyInvite;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([stPartyInvite, server]() mutable {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

        // Get inviter's user object to get UAID and level
        const std::shared_ptr<CUserObject> pReqUser = relayServer.GetUser(stPartyInvite.dwReqActorID);
        std::uint32_t dwUAID = 0;
        std::uint8_t byLevel = 0;
        if (pReqUser) {
            dwUAID = pReqUser->GetUAID();
            byLevel = pReqUser->GetLevel();
        }

        // Get party ID from PartyManager
        UXActorID uxActorID{};
        uxActorID.dwActorID = stPartyInvite.dwReqActorID;
        const std::uint32_t dwPartyID = relayServer.GetPartyManager().GetPartyID(uxActorID);

        relayServer.GetPartyManager().ReqInviteParty(
            server,
            stPartyInvite,
            static_cast<int>(dwUAID),
            byLevel,
            dwPartyID);
    });
}

bool CPartyProcess::ReqPartyCreate(XPacket& xPacket) {
    // 对齐 IDA 0x1400A1DA4: sub=0x01
    // TODO: 对齐 IDA 反序列化
    static_cast<void>(xPacket);
    return true;
}

bool CPartyProcess::ReqPartyLeaveMember(XPacket& xPacket) {
    // 对齐 IDA: sub=0x03
    // TODO: 对齐 IDA 反序列化
    static_cast<void>(xPacket);
    return true;
}

bool CPartyProcess::ReqPartyChangeMaster(XPacket& xPacket) {
    // 对齐 IDA: sub=0x04
    // TODO: 对齐 IDA 反序列化
    static_cast<void>(xPacket);
    return true;
}

bool CPartyProcess::ReqPartyAccept(XPacket& xPacket) {
    // 对齐 IDA: sub=0x12
    // TODO: 对齐 IDA 反序列化
    static_cast<void>(xPacket);
    return true;
}

bool CPartyProcess::ReqPartyCancel(XPacket& xPacket) {
    // 对齐 IDA: sub=0x13
    // TODO: 对齐 IDA 反序列化
    static_cast<void>(xPacket);
    return true;
}

bool CPartyProcess::SyncPartyMessage(XPacket& xPacket) {
    // 对齐 IDA: sub=0x14
    // TODO: 对齐 IDA 反序列化
    static_cast<void>(xPacket);
    return true;
}

bool CPartyProcess::ReqPartyMatchingEnter(XPacket& xPacket) {
    // 对齐 IDA: sub=0x20
    // TODO: 对齐 IDA 反序列化
    static_cast<void>(xPacket);
    return true;
}

bool CPartyProcess::ReqPartyMatchingExit(XPacket& xPacket) {
    // 对齐 IDA: sub=0x21
    // TODO: 对齐 IDA 反序列化
    static_cast<void>(xPacket);
    return true;
}

bool CPartyProcess::ReqPartyMatchingCheck(XPacket& xPacket) {
    // 对齐 IDA: sub=0x22
    // TODO: 对齐 IDA 反序列化
    static_cast<void>(xPacket);
    return true;
}

bool CPartyProcess::ReqPartyMazeClear(XPacket& xPacket) {
    // 对齐 IDA: sub=0x43
    // TODO: 对齐 IDA 反序列化
    static_cast<void>(xPacket);
    return true;
}
