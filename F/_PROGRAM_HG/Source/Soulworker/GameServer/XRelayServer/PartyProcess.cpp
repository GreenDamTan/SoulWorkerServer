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
            // 对齐 IDA: ReqPartyRecruitCreate 返回 void, shared_ptr 按值, 最后参数引用
            relayServer.GetPartyMatchingMgr().ReqPartyRecruitCreate(userParty,
                                                                     stRecruit,
                                                                     recruitRes.dwRecruitID);
            {
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

        relayServer.GetPartyMatchingMgr().ReqPartyRecruitDel(actorID);

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
    // 对齐 IDA 0x1400A35F0: sub=0x40 请求队伍信息
    std::uint32_t dwPartyID = 0;
    std::uint32_t dwActorID = 0;

    xPacket.XParse >> dwPartyID;
    xPacket.XParse >> dwActorID;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([dwPartyID, dwActorID, server]() {
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }

        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        std::shared_ptr<CParty> pParty = relayServer.GetPartyManager().GetParty(dwPartyID);

        if (pParty) {
            PS_PARTY_INFO stPartyInfo{};
            pParty->GetPartyInfo(stPartyInfo);

            XSendPacket xSendPacket(0xF4u, 0x40u);
            xSendPacket.XParse << dwActorID;
            xSendPacket << stPartyInfo;
            server->SendEx(xSendPacket);
        }
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
    // 对齐 IDA 0x1400A24A0: sub=0x11 邀请加入队伍
    PS_REQ_PARTY_INVITE stPartyInvite{};
    std::uint32_t dwUAID = 0;
    std::uint8_t byLevel = 0;
    std::uint32_t dwPartyID = 0;
    std::uint8_t bySelect = 0;  // 对齐 IDA KEKE: 第6个参数 E

    xPacket >> stPartyInvite;
    xPacket.XParse >> dwUAID;
    xPacket.XParse >> byLevel;
    xPacket.XParse >> dwPartyID;
    xPacket.XParse >> bySelect;  // 对齐 IDA: 读取选择标志字节

    CServer* server = GetClientPtr();
    return DispatchPartyJob([stPartyInvite, dwUAID, byLevel, dwPartyID, bySelect, server]() mutable {
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }

        // 对齐 IDA: 设置请求服务器 ID
        PS_REQ_PARTY_INVITE stNewPartyInvite = stPartyInvite;
        stNewPartyInvite.dwReqServerID = server->GetServerID();

        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetPartyManager().ReqInviteParty(
            server,
            stNewPartyInvite,
            dwUAID,
            byLevel,
            dwPartyID,
            bySelect);
    });
}

bool CPartyProcess::ReqPartyCreate(XPacket& xPacket) {
    // 对齐 IDA 0x1400A22B0: sub=0x01 创建队伍请求
    PS_REQ_PARTY_CREATE stPartyReq{};
    xPacket >> stPartyReq;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([stPartyReq, server]() mutable {
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }

        PS_REQ_PARTY_CREATE stNewPartyReq = stPartyReq;
        stNewPartyReq.dwReqServerID = server->GetServerID();

        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetPartyManager().ReqCreateParty(server, stNewPartyReq);
    });
}

bool CPartyProcess::ReqPartyLeaveMember(XPacket& xPacket) {
    // 对齐 IDA 0x1400A2C00: sub=0x03 成员离开队伍
    PS_PARTY_LEAVE stPartyLeave{};
    std::uint32_t dwActorID = 0;
    std::uint32_t dwUAID = 0;
    std::uint8_t byLevel = 0;
    std::uint8_t byLeaverLevel = 0;

    xPacket >> stPartyLeave;
    xPacket.XParse >> dwActorID;
    xPacket.XParse >> dwUAID;
    xPacket.XParse >> byLevel;
    xPacket.XParse >> byLeaverLevel;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([stPartyLeave, dwActorID, dwUAID, byLevel, byLeaverLevel, server]() mutable {
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }

        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetPartyManager().ReqPartyLeave(
            server, stPartyLeave, dwActorID, dwUAID, byLevel, byLeaverLevel);
    });
}

bool CPartyProcess::ReqPartyChangeMaster(XPacket& xPacket) {
    // 对齐 IDA 0x1400A2FD0: sub=0x04 变更队长
    PS_PARTY_CHANGE_MASTER stChangeMaster{};
    xPacket >> stChangeMaster;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([stChangeMaster, server]() mutable {
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }

        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

        // 对齐 IDA: 检查是否在招募中
        ST_PARTY_RECRUIT_INFO stTemp{};
        if (relayServer.GetPartyMatchingMgr().GetPartyRecruitInfo(
                stChangeMaster.dwReqActorID, stTemp)) {
            if (stTemp.stRecruit.dwMasterUCID == stChangeMaster.dwReqActorID) {
                // 已在招募列表中，返回错误码 55061
                PS_PARTY_CHANGE_MASTER stErrorRes = stChangeMaster;
                stErrorRes.nErrorCode = 55061;
                XSendPacket sendPacket(0xF4u, 0x04u);
                sendPacket << stErrorRes;
                server->SendEx(sendPacket);
                LogHelper::LogInfo(
                    "game.relay",
                    "<PARTY> ReqPartyChangeMaster - Added PartyRecruit [%u] ",
                    static_cast<unsigned int>(stChangeMaster.dwReqActorID));
                return;
            }
        }

        relayServer.GetPartyManager().ReqChangeMaster(server, stChangeMaster);
    });
}

bool CPartyProcess::ReqPartyAccept(XPacket& xPacket) {
    // 对齐 IDA 0x1400A28D0: sub=0x12 接受队伍邀请
    PS_RES_PARTY_INVITE stPartyAccept{};
    std::uint32_t dwUAID = 0;
    std::uint8_t byLevel = 0;

    xPacket >> stPartyAccept;
    xPacket.XParse >> dwUAID;
    xPacket.XParse >> byLevel;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([stPartyAccept, dwUAID, byLevel, server]() mutable {
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }

        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetPartyManager().ReqAcceptParty(
            server, stPartyAccept, dwUAID, byLevel);
    });
}

bool CPartyProcess::ReqPartyCancel(XPacket& xPacket) {
    // 对齐 IDA 0x1400A2A40: sub=0x13 取消队伍邀请
    PS_PARTY_REJECT stPartyReject{};
    xPacket >> stPartyReject;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([stPartyReject, server]() mutable {
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }

        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetPartyManager().ReqCancelParty(server, stPartyReject);
    });
}

bool CPartyProcess::SyncPartyMessage(XPacket& xPacket) {
    // 对齐 IDA 0x1400A2050: sub=0x14 同步队伍消息
    PS_CHAT_PARTY stChatParty{};
    PS_CHAT_ITEM_LINK_FOR_SERVER psChatLinkItemInfo{};

    xPacket >> stChatParty;
    xPacket >> psChatLinkItemInfo;

    return DispatchPartyJob([stChatParty, psChatLinkItemInfo]() mutable {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetPartyManager().SendPartyMessage(
            stChatParty,
            psChatLinkItemInfo);
    });
}

bool CPartyProcess::ReqPartyMatchingEnter(XPacket& xPacket) {
    // 对齐 IDA 0x1400A39A0: sub=0x20 进入队伍匹配
    ST_PARTY_MEMBER stMemberInfo{};
    std::int64_t nExp = 0;
    ST_CREATE_MAZE stCreateMaze{};
    std::uint32_t dwUAID = 0;
    int nState = 0;

    xPacket >> stMemberInfo;
    xPacket.XParse >> nExp;
    xPacket >> stCreateMaze;
    xPacket.XParse >> dwUAID;
    xPacket.XParse >> nState;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([stMemberInfo, stCreateMaze, nExp, nState, dwUAID, server]() {
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }

        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

        // 对齐 IDA: 检查用户是否存在
        const std::shared_ptr<CUserPartyInfo> pUserParty =
            relayServer.GetPartyUser(stMemberInfo.dwMemberID);
        if (!pUserParty) {
            // 用户不存在，返回错误
            ST_MATCHING_INFO stTempInfo{};
            XSendPacket xSendPacket(0xF4u, 0x20u);
            xSendPacket.XParse << stMemberInfo.dwMemberID;
            xSendPacket.XParse << 0;
            xSendPacket.XParse << stMemberInfo.dwMemberID;
            xSendPacket << stTempInfo;
            server->SendEx(xSendPacket);
            return;
        }

        // 对齐 IDA: 检查奖励状态
        if (pUserParty->GetRewardState()) {
            ST_MATCHING_INFO stTempInfo{};
            XSendPacket xSendPacket(0xF4u, 0x20u);
            xSendPacket.XParse << stMemberInfo.dwMemberID;
            xSendPacket.XParse << 0;
            xSendPacket.XParse << stMemberInfo.dwMemberID;
            xSendPacket << stTempInfo;
            server->SendEx(xSendPacket);
            return;
        }

        // 对齐 IDA: 检查申请招募数量
        if (pUserParty->GetApplyRecruitCount()) {
            ST_MATCHING_INFO stTempInfo{};
            XSendPacket xSendPacket(0xF4u, 0x20u);
            xSendPacket.XParse << stMemberInfo.dwMemberID;
            xSendPacket.XParse << 100;
            xSendPacket.XParse << stMemberInfo.dwMemberID;
            xSendPacket << stTempInfo;
            server->SendEx(xSendPacket);
            return;
        }

        // 对齐 IDA: 检查是否已在招募中
        if (relayServer.GetPartyMatchingMgr().FindRecruitID(stMemberInfo.dwMemberID) != 0) {
            ST_MATCHING_INFO stTempInfo{};
            XSendPacket xSendPacket(0xF4u, 0x20u);
            xSendPacket.XParse << stMemberInfo.dwMemberID;
            xSendPacket.XParse << 100;
            xSendPacket.XParse << stMemberInfo.dwMemberID;
            xSendPacket << stTempInfo;
            server->SendEx(xSendPacket);
            return;
        }

        // 对齐 IDA: 设置匹配状态
        pUserParty->SetMatchingState(1u);

        std::uint32_t dwMatchingID = 0;
        bool byCreate = false;

        // 对齐 IDA: 先尝试进入已有匹配, 最后参数为引用
        if (!relayServer.GetPartyMatchingMgr().EnterMatching(
                stMemberInfo, nExp, stCreateMaze.wReqMapID, nState, server, dwMatchingID)) {
            // 对齐 IDA: 进入失败则创建新匹配, portal/jump为uint32_t, 最后参数为引用
            relayServer.GetPartyMatchingMgr().CreateMatching(
                stMemberInfo, nExp, stCreateMaze.wReqMapID, nState,
                static_cast<std::uint32_t>(stCreateMaze.nPortalID),
                static_cast<std::uint32_t>(stCreateMaze.nJumpID), server, dwMatchingID);
            byCreate = true;
        }

        // 对齐 IDA: 设置匹配 ID
        pUserParty->SetMatchingID(dwMatchingID, 1u);

        // 对齐 IDA: 记录 DB 日志
        relayServer.SendDBLog(
            dwUAID, stMemberInfo.dwMemberID,
            22, 9,
            static_cast<int>(stMemberInfo.byLevel),
            static_cast<int>(byCreate),
            0, 0, 0,
            static_cast<std::int64_t>(dwMatchingID),
            0, L"");
    });
}

bool CPartyProcess::ReqPartyMatchingExit(XPacket& xPacket) {
    // 对齐 IDA 0x1400A43B0: sub=0x21 退出队伍匹配
    std::uint32_t dwActorID = 0;
    std::uint8_t byReason = 0;
    std::uint32_t dwUAID = 0;

    xPacket.XParse >> dwActorID;
    byReason = xPacket.XParse.GetBYTE();
    xPacket.XParse.GetBYTE();
    xPacket.XParse.GetBYTE();
    xPacket.XParse.GetBYTE();
    xPacket.XParse >> dwUAID;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([dwActorID, byReason, dwUAID, server]() {
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }

        // 对齐 IDA 0x1400A44D0: ExitMatching 需要 matchingID 从 GetPartyUser 获取
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        const std::shared_ptr<CUserPartyInfo> pUserParty = relayServer.GetPartyUser(dwActorID);
        if (pUserParty && pUserParty->GetMatchingID() != 0 && pUserParty->GetMatchingState() == 1) {
            const std::uint32_t dwMatchingID = pUserParty->GetMatchingID();
            relayServer.GetPartyMatchingMgr().ExitMatching(
                dwActorID, dwMatchingID, byReason, dwUAID);
            pUserParty->SetMatchingState(false);
            pUserParty->SetMatchingID(0, 0);
        }
    });
}

bool CPartyProcess::ReqPartyMatchingCheck(XPacket& xPacket) {
    // 对齐 IDA 0x1400A4600: sub=0x22 检查队伍匹配状态
    std::uint32_t dwActorID = 0;
    std::uint8_t byCheck = 0;
    std::uint32_t dwUAID = 0;

    xPacket.XParse >> dwActorID;
    byCheck = xPacket.XParse.GetBYTE();
    xPacket.XParse.GetBYTE();
    xPacket.XParse.GetBYTE();
    xPacket.XParse.GetBYTE();
    xPacket.XParse >> dwUAID;

    CServer* server = GetClientPtr();
    return DispatchPartyJob([dwActorID, byCheck, dwUAID, server]() {
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }

        // 对齐 IDA: 参数顺序 (dwActorID, byCheck, pServer, dwUAID)
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetPartyMatchingMgr().CheckMatching(
            dwActorID, byCheck, server, dwUAID);
    });
}

bool CPartyProcess::ReqPartyMazeClear(XPacket& xPacket) {
    // 对齐 IDA 0x1400A3810: sub=0x43 迷宫通关清除
    std::uint32_t dwPartyID = 0;

    xPacket.XParse >> dwPartyID;
    // 跳过额外的包数据（IDA显示这些字节被读取但未使用）
    xPacket.XParse.GetBYTE();
    xPacket.XParse.GetBYTE();
    xPacket.XParse.GetBYTE();
    xPacket.XParse.GetBYTE();

    CServer* server = GetClientPtr();
    return DispatchPartyJob([dwPartyID, server]() {
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }

        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetPartyManager().ReqPartyMazeClear(dwPartyID);
    });
}
