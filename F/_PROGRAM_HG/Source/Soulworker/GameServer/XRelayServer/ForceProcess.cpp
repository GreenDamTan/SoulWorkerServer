#include "Soulworker/GameServer/XRelayServer/ForceProcess.h"

#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/ForceManager.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h"

#include <cstdlib>
#include <cstring>

bool CForceProcess::DispatchForceJob(const std::function<void()>& job) {
    if (!job) {
        return false;
    }
    return CLogicThreadManager::Instance().DoJob(0, job);
}

bool CForceProcess::Parse(XPacket& xPacket) {
    switch (static_cast<unsigned char>(xPacket.GetSubCmd())) {
    case 0x01:
        return ReqForceCreate(xPacket);
    case 0x09:
        // 对齐 IDA 0x140023526: case 9 调用 boost::multi_index::modify_ (0x1400CF1E0)
        // IDA 反编译显示 modify_ 函数主体仅为 return 1，是空实现
        return true;
    case 0x03:
        return ReqForceLeaveMember(xPacket);
    case 0x04:
        return ReqForceChangeMaster(xPacket);
    case 0x05:
        return ReqForceUpdateMember(xPacket);
    case 0x0A:
        return ReqForceEnterServer(xPacket);
    case 0x0B:
        return ReqForceInvite(xPacket);
    case 0x0C:
        return ReqForceAccept(xPacket);
    case 0x0D:
        return ReqForceCancel(xPacket);
    case 0x10:
        return SyncForceMessage(xPacket);
    case 0x13:
        return ReqForceMatchingEnter(xPacket);
    case 0x14:
        return ReqForceMatchingExit(xPacket);
    case 0x15:
        return ReqForceMatchingCheck(xPacket);
    case 0x19:
        return ReqForceMazeClear(xPacket);
    case 0x1A:
        return ReqForceInfo(xPacket);
    case 0x21:
        return ReqForceChangeMazeOpenCheck(xPacket);
    default:
        return true;
    }
}

bool CForceProcess::ReqForceUpdateMember(XPacket& xPacket) {
    ST_UPDATE_FORCE_MEMBER forceMember{};
    xPacket >> forceMember;

    CServer* server = GetClientPtr();
    CForceManager* forceManager = &TXSingleton<XRelayServer>::Instance()->GetForceManager();
    return DispatchForceJob([forceMember, server, forceManager]() mutable {
        IXObject* objectOwner = server ? static_cast<IXObject*>(server) : nullptr;
        XSendDBPacket sendPacket(objectOwner, 8u, 4u);
        sendPacket << forceMember;
        TXSingleton<XRelayServer>::Instance()->SendDBGame(sendPacket);
        if (forceManager) {
            forceManager->ResUpdateMemberInfo(forceMember);
        }
    });
}

bool CForceProcess::ReqForceEnterServer(XPacket& xPacket) {
    PS_REQ_FORCE_ENTER_SERVER enterServer{};
    xPacket >> enterServer;

    CServer* server = GetClientPtr();
    CForceManager* forceManager = &TXSingleton<XRelayServer>::Instance()->GetForceManager();
    return DispatchForceJob([enterServer, server, forceManager]() mutable {
        LogHelper::LogDebug(
            "game.relay",
            "GreenDamTan_log ForceProcess.cpp::CForceProcess::ReqForceEnterServer member=%u force=%u map=%lld maxHP=%d reqInfo=%u server=%p",
            static_cast<unsigned int>(enterServer.dwMemberID),
            static_cast<unsigned int>(enterServer.dwForceID),
            static_cast<long long>(enterServer.uxMapID.nMapID),
            enterServer.nMaxHP,
            static_cast<unsigned int>(enterServer.bReqForceInfo ? 1u : 0u),
            static_cast<void*>(server));
        if (forceManager) {
            forceManager->EnterServer(server, enterServer);
        }
    });
}

bool CForceProcess::ReqForceMatchingEnter(XPacket& xPacket) {
    PS_SERVER_FORCE_MATCHING_ENTER enterInfo{};
    PS_SERVER_FORCE_MATCHING_ENTER_MEMBER masterInfo{};
    xPacket >> enterInfo;
    xPacket >> masterInfo;

    CServer* server = GetClientPtr();
    return DispatchForceJob([enterInfo, masterInfo, server]() mutable {
        // 对齐 IDA lambda9_ 0x140024C50: 先检查 pServer 有效性
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }

        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        const std::shared_ptr<CUserPartyInfo> pUser = relayServer.GetPartyUser(masterInfo.dwUCID);
        if (!pUser) {
            // 对齐 IDA: 用户不存在，发送错误包 code=0
            ST_FORCE_MATCHING_INFO emptyInfo{};
            XSendPacket sendPacket(0xFAu, 0x13u);
            sendPacket.XParse << masterInfo.dwUCID;
            sendPacket.XParse << 0;
            sendPacket.XParse << masterInfo.dwUCID;
            sendPacket << emptyInfo;
            server->SendEx(sendPacket);
            return;
        }

        // 对齐 IDA: 检查奖励状态
        if (pUser->GetRewardState() != 0) {
            ST_FORCE_MATCHING_INFO emptyInfo{};
            XSendPacket sendPacket(0xFAu, 0x13u);
            sendPacket.XParse << masterInfo.dwUCID;
            sendPacket.XParse << 0;
            sendPacket.XParse << masterInfo.dwUCID;
            sendPacket << emptyInfo;
            server->SendEx(sendPacket);
            return;
        }

        // 对齐 IDA: 检查招募申请计数
        if (pUser->GetApplyRecruitCount() != 0) {
            ST_FORCE_MATCHING_INFO emptyInfo{};
            XSendPacket sendPacket(0xFAu, 0x13u);
            sendPacket.XParse << masterInfo.dwUCID;
            sendPacket.XParse << 100;
            sendPacket.XParse << masterInfo.dwUCID;
            sendPacket << emptyInfo;
            server->SendEx(sendPacket);
            return;
        }

        // 对齐 IDA: 检查是否已在招募匹配中
        if (relayServer.GetPartyMatchingMgr().FindRecruitID(masterInfo.dwUCID) != 0) {
            ST_FORCE_MATCHING_INFO emptyInfo{};
            XSendPacket sendPacket(0xFAu, 0x13u);
            sendPacket.XParse << masterInfo.dwUCID;
            sendPacket.XParse << 100;
            sendPacket.XParse << masterInfo.dwUCID;
            sendPacket << emptyInfo;
            server->SendEx(sendPacket);
            return;
        }

        // 对齐 IDA: 分三种情况处理 - solo/party/force matching
        const std::uint8_t byPartyGroupType = enterInfo.stPartyInfo.byGroupType;

        if (enterInfo.vecMember.empty()) {
            // === Solo matching (空成员列表) ===
            // 对齐 IDA: 先设置匹配状态为1
            pUser->SetMatchingState(1);

            std::uint32_t matchingID = 0;
            std::uint8_t byCreate = 0;

            // 对齐 IDA: solo 路径使用 EnterMatching(memberInfo, dwMazeID, pServer, matchingID)
            if (!relayServer.GetForceMatchingMgr().EnterMatching(
                    masterInfo,
                    static_cast<std::uint32_t>(enterInfo.stCreateMaze.wReqMapID),
                    server,
                    matchingID)) {
                // EnterMatching 失败，调用 CreateMatching
                // 对齐 IDA: CreateMatching(memberInfo, dwMazeID, dwUAID, dwUCID, pServer, matchingID)
                relayServer.GetForceMatchingMgr().CreateMatching(
                    masterInfo,
                    static_cast<std::uint32_t>(enterInfo.stCreateMaze.wReqMapID),
                    masterInfo.dwUAID,
                    masterInfo.dwUCID,
                    server,
                    matchingID);
                byCreate = 1;
            }

            // 设置匹配ID和状态
            pUser->SetMatchingID(matchingID, 2u);

            // 对齐 IDA: 发送 DB 日志
            relayServer.SendDBLog(masterInfo.dwUAID,
                                   masterInfo.dwUCID,
                                   23,
                                   9,
                                   static_cast<std::uint8_t>(enterInfo.stCreateMaze.wReqMapID >> 8),
                                   byCreate,
                                   0,
                                   0,
                                   0,
                                   matchingID,
                                   0,
                                   L"");

            LogHelper::LogDebug(
                "game.relay",
                "GreenDamTan_log ForceProcess.cpp::CForceProcess::ReqForceMatchingEnter solo master=%u maze=%u matching=%u create=%u",
                static_cast<unsigned int>(masterInfo.dwUCID),
                static_cast<unsigned int>(enterInfo.stCreateMaze.wReqMapID),
                static_cast<unsigned int>(matchingID),
                static_cast<unsigned int>(byCreate));
        }
        else if (byPartyGroupType == 1) {
            // === Party matching ===
            std::shared_ptr<CParty> pParty = relayServer.GetPartyManager().GetParty(static_cast<std::uint32_t>(enterInfo.stPartyInfo.nID));
            if (!pParty) {
                ST_FORCE_MATCHING_INFO emptyInfo{};
                XSendPacket sendPacket(0xFAu, 0x13u);
                sendPacket.XParse << masterInfo.dwUCID;
                sendPacket.XParse << 0;
                sendPacket.XParse << masterInfo.dwUCID;
                sendPacket << emptyInfo;
                server->SendEx(sendPacket);
                return;
            }

            std::uint32_t matchingID = 0;
            std::uint8_t byCreate = 0;

            // 对齐 IDA: party 路径使用 EnterMatching(enterInfo, memberInfo, pServer, 1, matchingID, byCreate)
            if (!relayServer.GetForceMatchingMgr().EnterMatching(
                    enterInfo,
                    masterInfo,
                    server,
                    1u,  // byPartyGroupType = 1 for Party
                    matchingID,
                    byCreate)) {
                ST_FORCE_MATCHING_INFO emptyInfo{};
                XSendPacket sendPacket(0xFAu, 0x13u);
                sendPacket.XParse << masterInfo.dwUCID;
                sendPacket.XParse << 0;
                sendPacket.XParse << masterInfo.dwUCID;
                sendPacket << emptyInfo;
                server->SendEx(sendPacket);
                return;
            }

            // 对齐 IDA: 为每个成员发送 DB 日志
            // PS_SERVER_FORCE_MATCHING_ENTER_MEMBER 没有 dwActorID，使用 dwUCID
            // 使用 uxMapID.nMapID 代替 wMapID
            for (const auto& member : enterInfo.vecMember) {
                relayServer.SendDBLog(member.dwUCID,
                                       member.dwUAID,
                                       23,
                                       9,
                                       static_cast<std::uint8_t>(member.uxMapID.nMapID >> 24),
                                       byCreate,
                                       0,
                                       0,
                                       0,
                                       matchingID,
                                       0,
                                       L"");
            }

            // 对齐 IDA: 更新所有成员的匹配状态
            for (const auto& member : enterInfo.vecMember) {
                const std::shared_ptr<CUserPartyInfo> pMemberUser = relayServer.GetPartyUser(member.dwUAID);
                if (pMemberUser) {
                    pMemberUser->SetMatchingState(1u);
                    pMemberUser->SetMatchingID(matchingID, 2u);
                }
            }
        }
        else if (byPartyGroupType == 2) {
            // === Force matching ===
            std::shared_ptr<CForce> pForce = relayServer.GetForceManager().GetForce(static_cast<std::uint32_t>(enterInfo.stPartyInfo.nID));
            if (!pForce) {
                ST_FORCE_MATCHING_INFO emptyInfo{};
                XSendPacket sendPacket(0xFAu, 0x13u);
                sendPacket.XParse << masterInfo.dwUCID;
                sendPacket.XParse << 0;
                sendPacket.XParse << masterInfo.dwUCID;
                sendPacket << emptyInfo;
                server->SendEx(sendPacket);
                return;
            }

            std::uint32_t matchingID = 0;
            std::uint8_t byCreate = 0;

            // 对齐 IDA: force 路径使用 EnterMatching(enterInfo, memberInfo, pServer, 2, matchingID, byCreate)
            if (!relayServer.GetForceMatchingMgr().EnterMatching(
                    enterInfo,
                    masterInfo,
                    server,
                    2u,  // byPartyGroupType = 2 for Force
                    matchingID,
                    byCreate)) {
                ST_FORCE_MATCHING_INFO emptyInfo{};
                XSendPacket sendPacket(0xFAu, 0x13u);
                sendPacket.XParse << masterInfo.dwUCID;
                sendPacket.XParse << 0;
                sendPacket.XParse << masterInfo.dwUCID;
                sendPacket << emptyInfo;
                server->SendEx(sendPacket);
                return;
            }

            // 对齐 IDA: 为每个成员发送 DB 日志
            // PS_SERVER_FORCE_MATCHING_ENTER_MEMBER 没有 dwActorID，使用 dwUCID
            // 使用 uxMapID.nMapID 代替 wMapID
            for (const auto& member : enterInfo.vecMember) {
                relayServer.SendDBLog(member.dwUCID,
                                       member.dwUAID,
                                       23,
                                       9,
                                       static_cast<std::uint8_t>(member.uxMapID.nMapID >> 24),
                                       byCreate,
                                       0,
                                       0,
                                       0,
                                       matchingID,
                                       0,
                                       L"");
            }

            // 对齐 IDA: 更新所有成员的匹配状态
            for (const auto& member : enterInfo.vecMember) {
                const std::shared_ptr<CUserPartyInfo> pMemberUser = relayServer.GetPartyUser(member.dwUAID);
                if (pMemberUser) {
                    pMemberUser->SetMatchingState(1u);
                    pMemberUser->SetMatchingID(matchingID, 2u);
                }
            }
        }
    });
}

bool CForceProcess::ReqForceMatchingExit(XPacket& xPacket) {
    std::uint32_t actorID = 0;
    std::uint8_t byReason = 0;
    std::uint32_t uaid = 0;
    std::uint8_t byLevel = 0;
    xPacket.XParse >> actorID;
    xPacket.XParse >> byReason;
    xPacket.XParse >> uaid;
    xPacket.XParse >> byLevel;

    CServer* server = GetClientPtr();
    return DispatchForceJob([actorID, byReason, uaid, byLevel, server]() {
        // 对齐 IDA lambda10_ 0x140026080: 先检查 pServer 有效性
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }

        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        const std::shared_ptr<CUserPartyInfo> partyUser = relayServer.GetPartyUser(actorID);
        if (!partyUser) {
            return;
        }

        // 对齐 IDA: 检查奖励状态和匹配状态
        if (partyUser->GetRewardState() != 0 && partyUser->GetMatchingState() == 2) {
            bool bPartyGroup = false;

            // 对齐 IDA: 遍历队伍成员
            std::shared_ptr<CParty> pParty = relayServer.GetPartyManager().GetParty(actorID);
            if (pParty) {
                bPartyGroup = true;
                pParty->ForEachMemberID([&relayServer, byReason, uaid](std::uint32_t dwMemberID) {
                    const std::shared_ptr<CUserPartyInfo> pMemberUser = relayServer.GetPartyUser(dwMemberID);
                    if (pMemberUser) {
                        const std::uint32_t dwMatchingID = pMemberUser->GetMatchingID();
                        relayServer.GetForceMatchingMgr().ExitMatching(dwMemberID, dwMatchingID, byReason, 0);
                        pMemberUser->SetMatchingState(false);
                        pMemberUser->SetMatchingID(0, 0);
                    }
                });
            }

            // 对齐 IDA: 遍历部队成员
            std::shared_ptr<CForce> pForce = relayServer.GetForceManager().GetForce(actorID);
            if (pForce) {
                bPartyGroup = true;
                pForce->ForEachMemberID([&relayServer, byReason, uaid](std::uint32_t dwMemberID) {
                    const std::shared_ptr<CUserPartyInfo> pMemberUser = relayServer.GetPartyUser(dwMemberID);
                    if (pMemberUser) {
                        const std::uint32_t dwMatchingID = pMemberUser->GetMatchingID();
                        relayServer.GetForceMatchingMgr().ExitMatching(dwMemberID, dwMatchingID, byReason, 0);
                        pMemberUser->SetMatchingState(false);
                        pMemberUser->SetMatchingID(0, 0);
                    }
                });
            }

            // 对齐 IDA: 如果不是队伍/部队成员，仅处理单人
            if (!bPartyGroup) {
                relayServer.GetForceMatchingMgr().ExitMatching(actorID, partyUser->GetMatchingID(), byReason, uaid);
                partyUser->SetMatchingState(false);
                partyUser->SetMatchingID(0, 0);
            }
        }

        LogHelper::LogDebug(
            "game.relay",
            "GreenDamTan_log ForceProcess.cpp::CForceProcess::ReqForceMatchingExit actor=%u reason=%u uaid=%u level=%u server=%p",
            static_cast<unsigned int>(actorID),
            static_cast<unsigned int>(byReason),
            static_cast<unsigned int>(uaid),
            static_cast<unsigned int>(byLevel),
            static_cast<void*>(server));
    });
}

bool CForceProcess::ReqForceMatchingCheck(XPacket& xPacket) {
    PS_SERVER_FORCE_MATCHING_CHECK checkInfo{};
    xPacket >> checkInfo;

    CServer* server = GetClientPtr();
    return DispatchForceJob([checkInfo, server]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        // 对齐 IDA: 添加 dwUAID 参数
        relayServer.GetForceMatchingMgr().CheckMatching(checkInfo.dwUCID, checkInfo.byCheck, server, checkInfo.dwUAID);
    });
}

bool CForceProcess::SyncForceMessage(XPacket& xPacket) {
    PS_CHAT_FORCE chatForce{};
    PS_CHAT_ITEM_LINK_FOR_SERVER itemLinkInfo{};
    xPacket >> chatForce;
    xPacket >> itemLinkInfo;

    CForceManager* forceManager = &TXSingleton<XRelayServer>::Instance()->GetForceManager();
    return DispatchForceJob([chatForce, itemLinkInfo, forceManager]() mutable {
        if (!forceManager) {
            XSendPacket sendPacket(0xFAu, 0x10u);
            sendPacket << chatForce;
            sendPacket << itemLinkInfo;
            TXSingleton<XRelayServer>::Instance()->SendPacketAll(sendPacket);
            return;
        }
        forceManager->SendForceMessage(chatForce, itemLinkInfo);
    });
}

bool CForceProcess::ReqForceInfo(XPacket& xPacket) {
    std::uint32_t forceID = 0;
    std::uint32_t actorID = 0;
    xPacket.XParse >> forceID;
    xPacket.XParse >> actorID;

    CServer* server = GetClientPtr();
    CForceManager* forceManager = &TXSingleton<XRelayServer>::Instance()->GetForceManager();
    return DispatchForceJob([forceID, actorID, server, forceManager]() {
        LogHelper::LogDebug(
            "game.relay",
            "GreenDamTan_log ForceProcess.cpp::CForceProcess::ReqForceInfo force=%u actor=%u server=%p",
            static_cast<unsigned int>(forceID),
            static_cast<unsigned int>(actorID),
            static_cast<void*>(server));
        if (forceManager) {
            forceManager->ReqForceInfo(server, forceID, actorID);
        }
    });
}

bool CForceProcess::ReqForceChangeMazeOpenCheck(XPacket& xPacket) {
    std::uint8_t byCheck = 1;
    xPacket.XParse >> byCheck;

    return DispatchForceJob([bOn = (byCheck != 0)]() {
        TXSingleton<XRelayServer>::Instance()->GetMazeOpenControl().SetCheckMazeOpenTime(bOn);
    });
}

bool CForceProcess::ReqForceInvite(XPacket& xPacket) {
    PS_REQ_FORCE_INVITE stForceInvite{};
    std::uint32_t dwUAID = 0;
    std::uint8_t byLevel = 0;
    std::uint32_t dwForceID = 0;

    xPacket >> stForceInvite;
    xPacket.XParse >> dwUAID;
    xPacket.XParse >> byLevel;
    xPacket.XParse >> dwForceID;

    CServer* server = GetClientPtr();
    CForceManager* forceManager = &TXSingleton<XRelayServer>::Instance()->GetForceManager();
    return DispatchForceJob([stForceInvite, dwUAID, byLevel, dwForceID, server, forceManager]() mutable {
        if (forceManager) {
            forceManager->ReqInviteForce(server, stForceInvite, dwUAID, byLevel, dwForceID);
        }
    });
}

bool CForceProcess::ReqForceAccept(XPacket& xPacket) {
    PS_RES_FORCE_INVITE stForceAccept{};
    std::uint32_t dwUAID = 0;
    std::uint8_t byLevel = 0;

    xPacket >> stForceAccept;
    xPacket.XParse >> dwUAID;
    xPacket.XParse >> byLevel;

    CServer* server = GetClientPtr();
    CForceManager* forceManager = &TXSingleton<XRelayServer>::Instance()->GetForceManager();
    return DispatchForceJob([stForceAccept, dwUAID, byLevel, server, forceManager]() mutable {
        if (forceManager) {
            forceManager->ReqAcceptForce(server, stForceAccept, dwUAID, byLevel);
        }
    });
}

bool CForceProcess::ReqForceCancel(XPacket& xPacket) {
    PS_FORCE_REJECT stForceReject{};
    xPacket >> stForceReject;

    CServer* server = GetClientPtr();
    CForceManager* forceManager = &TXSingleton<XRelayServer>::Instance()->GetForceManager();
    return DispatchForceJob([stForceReject, server, forceManager]() mutable {
        if (forceManager) {
            forceManager->ReqCancelForce(server, stForceReject);
        }
    });
}

bool CForceProcess::ReqForceCreate(XPacket& xPacket) {
    PS_REQ_FORCE_CREATE stForceReq{};
    xPacket >> stForceReq;

    CServer* server = GetClientPtr();
    CForceManager* forceManager = &TXSingleton<XRelayServer>::Instance()->GetForceManager();
    return DispatchForceJob([stForceReq, server, forceManager]() mutable {
        if (forceManager) {
            forceManager->ReqCreateForce(server, stForceReq);
        }
    });
}

bool CForceProcess::ReqForceLeaveMember(XPacket& xPacket) {
    PS_FORCE_LEAVE stForceLeave{};
    std::uint32_t dwActorID = 0;
    std::uint32_t dwUAID = 0;
    std::uint8_t byLevel = 0;
    std::uint8_t byLeaveLevel = 0;

    xPacket >> stForceLeave;
    xPacket.XParse >> dwActorID;
    xPacket.XParse >> dwUAID;
    xPacket.XParse >> byLevel;
    xPacket.XParse >> byLeaveLevel;

    CServer* server = GetClientPtr();
    CForceManager* forceManager = &TXSingleton<XRelayServer>::Instance()->GetForceManager();
    return DispatchForceJob([stForceLeave, dwActorID, dwUAID, byLevel, byLeaveLevel, server, forceManager]() mutable {
        // 对齐 IDA lambda5_ 0x140024460: 先检查 pServer 有效性
        if (!server || !server->IsState(XClient::eStateConnect)) {
            return;
        }
        if (forceManager) {
            forceManager->ReqForceLeave(server, stForceLeave, dwActorID, dwUAID, byLevel, byLeaveLevel);
        }
        // 对齐 IDA: 检查奖励状态，如果处于奖励状态则发送错误包
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        const std::shared_ptr<CUserPartyInfo> pUser = relayServer.GetPartyUser(dwActorID);
        if (pUser && pUser->GetRewardState() == 1) {
            // 奖励状态为1时无法离开队伍，发送错误码53152
            XSendPacket sendPacket(0xFAu, 0x03u);
            sendPacket << stForceLeave;
            sendPacket.XParse << dwActorID;
            sendPacket.XParse << static_cast<int>(53152);
            server->SendEx(sendPacket);
        }
    });
}

bool CForceProcess::ReqForceChangeMaster(XPacket& xPacket) {
    PS_FORCE_CHANGE_MASTER stChangeMaster{};
    xPacket >> stChangeMaster;

    CServer* server = GetClientPtr();
    CForceManager* forceManager = &TXSingleton<XRelayServer>::Instance()->GetForceManager();
    return DispatchForceJob([stChangeMaster, server, forceManager]() mutable {
        if (forceManager) {
            forceManager->ReqChangeMaster(server, stChangeMaster);
        }
    });
}

bool CForceProcess::ReqForceMazeClear(XPacket& xPacket) {
    std::uint32_t dwForceID = 0;
    std::uint8_t byClearFail = 0;

    xPacket.XParse >> dwForceID;
    xPacket.XParse >> byClearFail;

    CServer* server = GetClientPtr();
    CForceManager* forceManager = &TXSingleton<XRelayServer>::Instance()->GetForceManager();
    return DispatchForceJob([dwForceID, server, forceManager]() {
        if (forceManager) {
            forceManager->ReqForceMazeClear(dwForceID);
        }
    });
}
