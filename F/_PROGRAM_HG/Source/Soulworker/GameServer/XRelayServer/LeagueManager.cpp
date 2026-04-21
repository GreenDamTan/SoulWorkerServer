#include "Soulworker/GameServer/XRelayServer/LeagueManager.h"

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/League.h"
#include "Soulworker/GameServer/XRelayServer/LeagueMember.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"

// ============================================================================
// 联赛生命周期
// ============================================================================

void CLeagueManager::ReqLeagueCreate(CServer* pServer, const PS_LEAGUE_CREATE_FOR_SERVER& stCreate) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueCreate leagueID=%d", stCreate.stCreateInfo.nLeagueID);

    // 创建可修改的副本以设置服务器ID和默认权限
    PS_LEAGUE_CREATE_FOR_SERVER stSendCreate = stCreate;

    if (pServer) {
        stSendCreate.nServerID = pServer->GetServerID();
    }

    // 根据IDA分析设置默认权限
    stSendCreate.nAuth_Elder |= 1;
    stSendCreate.nAuth_Manager |= 1;
    stSendCreate.nAuth_Manager |= 0x10;
    stSendCreate.nAuth_SubMaster |= 1;
    stSendCreate.nAuth_SubMaster |= 2;
    stSendCreate.nAuth_SubMaster |= 0x10;

    // 发送到DB (main=7, sub=0 用于联赛创建)
    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 7, 0);
    xSendDBPacket << stSendCreate;

    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
}

void CLeagueManager::ResCreateLeague(CServer* pServer, PS_LEAGUE_CREATE_FOR_SERVER& stCreate) {
    // 对齐 IDA 0x140079a50: 联赛创建 DB 响应处理
    // 获取用户对象
    auto pUser = TXSingleton<XRelayServer>::Instance()->GetUser(stCreate.dwActorID);
    if (!pUser) {
        LogHelper::LogError("game.contents", "ResCreateLeague error - pUser == NULL %d ", stCreate.dwActorID);
        return;
    }

    // 获取频道信息
    std::uint8_t byChannel = pUser->GetChannel();

    // 创建联赛信息和成员信息
    ST_LEAGUE_INFO stLeagueInfo{};
    ST_LEAGUE_MEMBER_EX stMemberInfo{};

    // 调用 CreateLeague 创建联赛
    CreateLeague(stCreate, stLeagueInfo, stMemberInfo, byChannel);

    // 删除申请列表
    DeleteApplicantList(pServer, stMemberInfo.dwUCID);

    // 构建响应包 (0xF6, 1)
    ST_LEAGUE_INFO_EX stLeagueInfoEx{};
    ST_LEAGUE_INFO_FOR_GAME stLeagueInfoForGame{};

    // 复制联赛名称
    wcscpy_s(stLeagueInfoEx.szLeagueName, stLeagueInfo.szLeagueName);
    stLeagueInfoEx.nLeagueID = stLeagueInfo.nLeagueID;
    stLeagueInfoEx.dwUCID = stLeagueInfo.dwMasterUCID;
    stLeagueInfoEx.nMemberCount = 1;

    // 设置联赛游戏信息
    stLeagueInfoForGame.byLeagueLevel = stLeagueInfo.byRating;
    stLeagueInfoForGame.dwMasterUCID = stLeagueInfo.dwMasterUCID;
    stLeagueInfoForGame.byPosition = stMemberInfo.stMember.byPosition;
    for (int i = 0; i < 8; ++i) {
        stLeagueInfoForGame.bySkillInfo[i] = stLeagueInfo.bySkill[i];
    }
    for (int j = 0; j < 9; ++j) {
        stLeagueInfoForGame.nAuth[j] = stLeagueInfo.nAuth[j];
    }

    // 发送创建成功响应 (0xF6, 1)
    XSendPacket xSendPacket(0xF6, 1);
    xSendPacket << stLeagueInfo;
    xSendPacket << stMemberInfo;
    xSendPacket.XParse << stCreate.dwActorID;
    xSendPacket << stLeagueInfoEx;
    xSendPacket << stLeagueInfoForGame;
    if (pServer) {
        pServer->SendEx(xSendPacket);
    }
}

void CLeagueManager::CreateLeague(PS_LEAGUE_CREATE_FOR_SERVER& stCreateInfo, ST_LEAGUE_INFO& stLeagueInfo, ST_LEAGUE_MEMBER_EX& stMemberInfo, std::uint8_t byChannel) {
    // 对齐 IDA 0x1400797c0: 创建联赛信息结构体
    // 设置联赛信息
    wcscpy_s(stLeagueInfo.szLeagueName, stCreateInfo.stCreateInfo.szName);
    wcscpy_s(stLeagueInfo.szMasterName, stCreateInfo.szMasterName);
    stLeagueInfo.nLeagueID = stCreateInfo.stCreateInfo.nLeagueID;
    stLeagueInfo.shMemberCount = 1;
    stLeagueInfo.dwMasterUCID = stCreateInfo.dwActorID;
    stLeagueInfo.nCreateDate = stCreateInfo.nCreateDate;
    stLeagueInfo.bySkillPoint = 1;
    stLeagueInfo.nAuth[4] = stCreateInfo.nAuth_Elder;      // Elder (长老)
    stLeagueInfo.nAuth[5] = stCreateInfo.nAuth_Manager;    // Manager (管理员)
    stLeagueInfo.nAuth[7] = stCreateInfo.nAuth_SubMaster;  // SubMaster (副盟主)
    stLeagueInfo.bOpen = true;
    stLeagueInfo.biInitDate = stCreateInfo.nCreateDate;

    // 设置成员信息
    wcscpy_s(stMemberInfo.szName, stCreateInfo.szMasterName);
    stMemberInfo.stMember.nLeagueID = stLeagueInfo.nLeagueID;
    stMemberInfo.dwUCID = stCreateInfo.dwActorID;
    stMemberInfo.stMember.biJoinDate = stCreateInfo.nCreateDate;
    stMemberInfo.stMember.byPosition = 100;  // Master position
    stMemberInfo.sWorldID = stCreateInfo.sWorldID;
    stMemberInfo.shLevel = stCreateInfo.byLevel;
    stMemberInfo.byClass = stCreateInfo.byClass;
    stMemberInfo.byAwaken = stCreateInfo.byAwaken;
    stMemberInfo.dwProfilePhotoID = stCreateInfo.dwProfilePhotoID;
    stMemberInfo.byChannel = byChannel;
    stMemberInfo.bLogin = true;

    // 添加联赛到管理器
    AddLeague(stLeagueInfo, stMemberInfo);
}

void CLeagueManager::ReqLeagueDel(CServer* pServer, std::uint32_t dwActorID, std::int32_t nLeagueID, std::int64_t biPenalty) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueDel leagueID=%d actorID=%u penalty=%lld", nLeagueID, dwActorID, biPenalty);

    // 查找联赛
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogDebug("game.league", "Not Exist League [LeagueID:%d]", nLeagueID);
        // 发送错误包（主命令0xF6，子命令2，错误码57016）
        std::int32_t nErrorCode = 57016;
        XSendPacket xSendPacket(0xF6, 2);
        xSendPacket.XParse << dwActorID;
        xSendPacket.XParse << nLeagueID;
        xSendPacket.XParse << biPenalty;
        xSendPacket.XParse << nErrorCode;
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "ReqLeagueDel League NULL(%d)", 394);
        return;
    }

    // 检查是否是会长
    if (!pLeague->IsMaster(dwActorID)) {
        LogHelper::LogDebug("game.league", "No Master [UCID:%d]", dwActorID);
        // 发送错误包（主命令0xF6，子命令2，错误码57015）
        std::int32_t nErrorCode = 57015;
        XSendPacket xSendPacket(0xF6, 2);
        xSendPacket.XParse << dwActorID;
        xSendPacket.XParse << nLeagueID;
        xSendPacket.XParse << biPenalty;
        xSendPacket.XParse << nErrorCode;
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
        return;
    }

    // 检查成员数量（只有会长一人时才能解散）
    std::int32_t nMemberCount = pLeague->GetMemberCount();
    if (nMemberCount > 1) {
        LogHelper::LogDebug("game.league", "Not Exist League [LeagueID:%d]", nLeagueID);
        // 发送错误包（主命令0xF6，子命令2，错误码57018）
        std::int32_t nErrorCode = 57018;
        XSendPacket xSendPacket(0xF6, 2);
        xSendPacket.XParse << dwActorID;
        xSendPacket.XParse << nLeagueID;
        xSendPacket.XParse << biPenalty;
        xSendPacket.XParse << nErrorCode;
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
        return;
    }

    // 发送到DB处理联赛删除（main=7, sub=1）
    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 7, 1);
    xSendDBPacket.XParse << pServer->GetServerID();
    xSendDBPacket.XParse << dwActorID;
    xSendDBPacket.XParse << nLeagueID;
    xSendDBPacket.XParse << biPenalty;

    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
}

void CLeagueManager::ResLeagueDel(CServer* pServer, std::uint32_t dwUCID, std::int32_t nLeagueID, std::int64_t biPenalty, std::int32_t nErrorCode) {
    // 联赛删除 DB 响应处理
    if (nErrorCode != 0) {
        // DB 返回错误，发送错误通知给客户端（0xF6, 2）
        XSendPacket xSendPacket(0xF6, 2);
        xSendPacket.XParse << dwUCID;
        xSendPacket.XParse << nLeagueID;
        xSendPacket.XParse << biPenalty;
        xSendPacket.XParse << nErrorCode;
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
        return;
    }

    // 成功删除：从本地列表移除联赛
    DeleteLeague(nLeagueID);

    // 发送删除成功通知给客户端（0xF6, 2）
    XSendPacket xSendPacket(0xF6, 2);
    xSendPacket.XParse << dwUCID;
    xSendPacket.XParse << nLeagueID;
    xSendPacket.XParse << biPenalty;
    xSendPacket.XParse << static_cast<std::int32_t>(0);  // nErrorCode = 0 (success)
    if (pServer) {
        pServer->SendEx(xSendPacket);
    }

    // 通知所有 Relay 服务器联赛已删除
    XSendPacket xSendPacket2(0xF6, 0x3F);
    xSendPacket2.XParse << nLeagueID;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket2);
}

void CLeagueManager::DeleteLeague(std::int32_t nLeagueID) {
    m_mpLeagueList.erase(nLeagueID);

    for (auto it = m_vecLeagueList.begin(); it != m_vecLeagueList.end(); ++it) {
        if (*it) {
            ST_LEAGUE_INFO info;
            (*it)->GetLeagueInfo(info);
            if (info.nLeagueID == nLeagueID) {
                m_vecLeagueList.erase(it);
                return;
            }
        }
    }
}

void CLeagueManager::Clear() {
    m_mpLeagueList.clear();
    m_vecLeagueList.clear();
    m_mpLeagueInvite.clear();
}

// ============================================================================
// 联赛信息
// ============================================================================

void CLeagueManager::ReqLeagueInfo(CServer* pServer, std::uint32_t dwActorID, std::int32_t nLeagueID) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueInfo actorID=%u leagueID=%d", dwActorID, nLeagueID);

    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "ReqLeagueInfo League NULL(%d)", 178);
        return;
    }

    // Get member pointer for the requesting actor
    auto pMember = pLeague->GetLeagueMemberPtr(dwActorID);
    if (!pMember) {
        return;
    }

    // Collect all league data
    ST_LEAGUE_INFO stLeagueInfo{};
    ST_LEAGUE_MEMBER_LIST stMemberList{};
    ST_LEAGUE_APPLICANT_LIST stApplicantList{};
    ST_LEAGUE_BOARD_LIST stBoardList{};
    ST_LEAGUE_RECORD_LIST stRecordList{};
    ST_LEAGUE_INFO_FOR_GAME stInfoForGame{};

    std::uint8_t byPosition = 0;
    pMember->GetPosition(byPosition);

    pLeague->GetLeagueInfo(stLeagueInfo);
    pLeague->GetMemberList(stMemberList);
    pLeague->GetApplicantList(stApplicantList);
    pLeague->GetBoardList(stBoardList);
    pLeague->GetRecordList(stRecordList);
    pLeague->SetLeagueInfoForGame(byPosition != 0, stInfoForGame);

    // 发送联赛信息给请求成员
    SendLeagueInfo(dwActorID, stLeagueInfo, stMemberList, stApplicantList, stBoardList, static_cast<std::uint8_t>(0), stRecordList, stInfoForGame);
}

bool CLeagueManager::GetMemberInfo(std::int32_t nLeagueID, std::uint32_t dwActorID, ST_LEAGUE_MEMBER_EX& stMember) {
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        return false;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "GetMemberInfo NULL(%d)", 64);
        return false;
    }

    auto pMember = pLeague->GetLeagueMemberPtr(dwActorID);
    if (!pMember) {
        return false;
    }

    pMember->GetLeagueMember(stMember);
    return true;
}

void CLeagueManager::AddLeague(ST_LEAGUE_INFO stInfo, ST_LEAGUE_MEMBER_EX stMember) {
    auto pLeague = std::make_shared<CLeague>();
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "AddLeague NULL(%d)", 86);
        return;
    }

    pLeague->SetLeagueInfo(stInfo);
    pLeague->AddMember(stMember);

    auto result = m_mpLeagueList.insert({stInfo.nLeagueID, pLeague});
    if (result.second) {
        m_vecLeagueList.push_back(pLeague);
    }
}

// ============================================================================
// 联赛登录/登出
// ============================================================================

bool CLeagueManager::ReqLeagueLogin(std::uint32_t dwUCID, std::int32_t nLeagueID) {
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogDebug("game.league", "Failed Load LeagueInfo [nLeagueID:%d]", nLeagueID);
        return false;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "ReqLeagueLogin Leauge NULL(%d)", 127);
        return false;
    }

    if (pLeague->LoginMember(dwUCID)) {
        pLeague->SendLeagueInfo(dwUCID);
        return true;
    }

    return false;
}

void CLeagueManager::LogOutLeagueMember(std::uint32_t dwUCID, std::int32_t nLeagueID, std::int64_t biPenalty) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::LogOutLeagueMember ucid=%u leagueID=%d", dwUCID, nLeagueID);

    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogDebug("game.league", "LogOutLeagueMember: League not found [leagueID:%d]", nLeagueID);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "LogOutLeagueMember: League NULL(%d)", 1870);
        return;
    }

    // 登出成员
    pLeague->LogOutMember(dwUCID, biPenalty);

    // 获取成员信息并更新
    auto pMember = pLeague->GetLeagueMemberPtr(dwUCID);
    if (!pMember) {
        LogHelper::LogDebug("game.league", "LogOutLeagueMember: Member NULL(%d)", 1882);
        return;
    }

    ST_LEAGUE_MEMBER_EX stMemberEx{};
    ST_LEAGUE_MEMBER_UPDATE stUpdate{};
    pMember->GetLeagueMember(stMemberEx);
    pLeague->LeagueMemberUpdate(stMemberEx, stUpdate);

    // 广播成员更新包（主命令0xF6，子命令0x39）
    XSendPacket xSendPacket(0xF6, 0x39);
    xSendPacket << stUpdate;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

void CLeagueManager::SendFailLeagueLogin(std::uint32_t dwUCID) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::SendFailLeagueLogin ucid=%u", dwUCID);

    // 发送联赛登录失败包（主命令0xF6，子命令3）
    // 所有结构都是空的，告诉用户联赛不存在或无法登录
    XSendPacket xSendPacket(0xF6, 3);

    // 第一个字段是0（错误标志）
    xSendPacket.XParse << 0;

    // 空的成员更新结构
    ST_LEAGUE_MEMBER_UPDATE stUpdate{};
    xSendPacket << stUpdate;

    // 空的联赛信息
    ST_LEAGUE_INFO stInfo{};
    xSendPacket << stInfo;

    // 空的成员列表
    ST_LEAGUE_MEMBER_LIST stMemberList{};
    xSendPacket << stMemberList;

    // 空的申请者列表
    ST_LEAGUE_APPLICANT_LIST stApplicantList{};
    xSendPacket << stApplicantList;

    // 空的公告板列表
    ST_LEAGUE_BOARD_LIST stBoardList{};
    xSendPacket << stBoardList;

    // 状态为0
    std::uint8_t byState = 0;
    xSendPacket.XParse << byState;

    // 空的联赛信息扩展
    ST_LEAGUE_INFO_EX stInfoEx{};
    xSendPacket << stInfoEx;

    // 空的记录列表
    ST_LEAGUE_RECORD_LIST stRecordList{};
    xSendPacket << stRecordList;

    // 空的游戏联赛信息
    ST_LEAGUE_INFO_FOR_GAME stInfoForGame{};
    xSendPacket << stInfoForGame;

    // 最后一个字段是0
    xSendPacket.XParse << 0;

    // 广播给所有服务器
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

// ============================================================================
// 邀请管理
// ============================================================================

void CLeagueManager::ReqLeagueInvite(CServer* pServer, const ST_REQ_LEAGUE_INVITE& stInvite, std::shared_ptr<CUserObject> pUser) {
    // 对齐 IDA 0x1400786d0: 联赛邀请处理
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueInvite leagueID=%d actorID=%u targetID=%u",
                       stInvite.nLeagueID, stInvite.dwActorID, stInvite.dwTargetActorID);

    // 查找联赛
    auto it = m_mpLeagueList.find(stInvite.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogDebug("game.league", "Failed Invite League [ LeagueID:%d ]", stInvite.nLeagueID);
        SendLeagueErrorMsg(pServer, const_cast<ST_REQ_LEAGUE_INVITE&>(stInvite), 57016);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "ReqLeagueInvite pLeague NULL(%d)", 1282);
        return;
    }

    // 获取联赛信息
    ST_LEAGUE_INFO stInfo{};
    pLeague->GetLeagueInfo(stInfo);

    // 获取邀请者成员信息
    auto pMember = pLeague->GetLeagueMemberPtr(stInvite.dwActorID);
    if (!pMember) {
        LogHelper::LogDebug("game.league", "ReqLeagueInvite Member NULL(%d)", 1290);
        return;
    }

    ST_LEAGUE_MEMBER_EX stMemberEx{};
    pMember->GetLeagueMember(stMemberEx);

    // 对齐 IDA: 查 TB_LEAGUE_INFO 表检查成员数量
    auto pTBLeague = TXSingleton<XRelayServer>::Instance()->GetResourceMgr().GetTB_LEAGUE_INFO(stInfo.byRating);
    if (pTBLeague) {
        std::int32_t nMemberCount = pLeague->GetMemberCount();
        if (nMemberCount >= static_cast<std::int32_t>(pTBLeague->League_Member)) {
            LogHelper::LogDebug("game.league", "Failed Invite League [ MemberCount:%d ]", nMemberCount);
            SendLeagueErrorMsg(pServer, const_cast<ST_REQ_LEAGUE_INVITE&>(stInvite), 57018);
            return;
        }
    }

    // 检查目标用户是否有效
    if (!pUser) {
        SendLeagueErrorMsg(pServer, const_cast<ST_REQ_LEAGUE_INVITE&>(stInvite), 57016);
        return;
    }

    // 检查目标是否在迷宫中
    if (pUser->IsMaze()) {
        LogHelper::LogDebug("game.league", "Failed Invite League [ In Maze ]");
        SendLeagueErrorMsg(pServer, const_cast<ST_REQ_LEAGUE_INVITE&>(stInvite), 57021);
        return;
    }

    // 检查目标是否已有联赛
    if (pUser->GetLeagueID() != 0) {
        LogHelper::LogDebug("game.league", "Failed Invite League [Exist League:%d]", pUser->GetLeagueID());
        SendLeagueErrorMsg(pServer, const_cast<ST_REQ_LEAGUE_INVITE&>(stInvite), 57008);
        return;
    }

    // 对齐 IDA: 检查退会惩罚时间
    std::int64_t biPenalty = pUser->GetLeagueWithdrawPenalty();
    std::int64_t biCurTime = static_cast<std::int64_t>(std::time(nullptr));
    if (biPenalty - biCurTime > 0) {
        // 惩罚时间未过
        ST_REQ_LEAGUE_INVITE stErrorInvite = stInvite;
        stErrorInvite.nResult = 57037;
        XSendPacket xSendPacket(0xF6, 0xC);
        xSendPacket << stErrorInvite;
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
        return;
    }

    // 检查邀请者权限
    if (stInvite.dwActorID == stInfo.dwMasterUCID) {
        // 会长直接邀请
        AddInviteUser(stInvite.dwTargetActorID, stInfo.nLeagueID);

        // 对齐 IDA: 检查是否已邀请过（重复邀请检查）
        if (CheckInviteUser(stInvite.dwTargetActorID)) {
            LogHelper::LogDebug("game.league", "Failed Invite League [ TargetUCID:%u ]", stInvite.dwTargetActorID);
            SendLeagueErrorMsg(pServer, const_cast<ST_REQ_LEAGUE_INVITE&>(stInvite), 57009);
            return;
        }

        // 发送邀请包给目标用户（主命令0xF6，子命令0xC）
        ST_REQ_LEAGUE_INVITE stSendInvite = stInvite;
        // 对齐 IDA: 设置目标的 matchingID
        stSendInvite.dwTargetActorID = pUser->GetMatchingID();
        wcscpy_s(stSendInvite.szLeagueName, stInfo.szLeagueName);
        wcscpy_s(stSendInvite.szReqName, stMemberEx.szName);

        XSendPacket xSendPacket(0xF6, 0xC);
        xSendPacket << stSendInvite;
        pUser->SendPacket(xSendPacket);
    } else {
        // 非会长需要检查权限
        if ((stInfo.nAuth[stMemberEx.stMember.byPosition] & 1) == 0) {
            LogHelper::LogDebug("game.league", "No Authority [ReqUCID:%u]", stInfo.dwMasterUCID);
            SendLeagueErrorMsg(pServer, const_cast<ST_REQ_LEAGUE_INVITE&>(stInvite), 57006);
            return;
        }

        AddInviteUser(stInvite.dwTargetActorID, stInfo.nLeagueID);

        // 对齐 IDA: 检查是否已邀请过
        if (CheckInviteUser(stInvite.dwTargetActorID)) {
            LogHelper::LogDebug("game.league", "Failed Invite League [ TargetUCID:%u ]", stInvite.dwTargetActorID);
            SendLeagueErrorMsg(pServer, const_cast<ST_REQ_LEAGUE_INVITE&>(stInvite), 57009);
            return;
        }

        // 发送邀请包给目标用户
        ST_REQ_LEAGUE_INVITE stSendInvite = stInvite;
        wcscpy_s(stSendInvite.szLeagueName, stInfo.szLeagueName);
        wcscpy_s(stSendInvite.szReqName, stMemberEx.szName);

        XSendPacket xSendPacket(0xF6, 0xC);
        xSendPacket << stSendInvite;
        pUser->SendPacket(xSendPacket);
    }
}

void CLeagueManager::ReqInviteAccept(CServer* pServer, const ST_REQ_LEAGUE_INVITE_ACCEPT& stAccept, std::int64_t biJoinDate) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqInviteAccept leagueID=%d", stAccept.nLeagueID);

    // 查找联赛
    auto it = m_mpLeagueList.find(stAccept.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogDebug("game.league", "Not Existence Accept League [ LeagueID:%d]", stAccept.nLeagueID);
        ST_REQ_LEAGUE_INVITE_ACCEPT stError = stAccept;
        stError.nResult = 57016;
        XSendPacket xSendPacket(0xF6, 0xD);
        xSendPacket << stError;
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "ReqInviteAccept League NULL(%d)", 585);
        return;
    }

    // 获取联赛信息
    ST_LEAGUE_INFO stInfo{};
    pLeague->GetLeagueInfo(stInfo);

    // 对齐 IDA 0x1400758a0: 检查成员数量限制
    auto pTBLeague = TXSingleton<XRelayServer>::Instance()->GetResourceMgr().GetTB_LEAGUE_INFO(stInfo.byRating);
    if (pTBLeague) {
        int nMemberCount = pLeague->GetMemberCount();
        if (nMemberCount >= pTBLeague->League_Member) {
            // 成员已满
            LogHelper::LogDebug("game.league", "Full League Member [MemberCount:%d]", nMemberCount);
            ST_REQ_LEAGUE_INVITE_ACCEPT stError = stAccept;
            stError.nResult = 57018;
            XSendPacket xSendPacket(0xF6, 0xD);
            xSendPacket << stError;
            if (pServer) {
                pServer->SendEx(xSendPacket);
            }
            return;
        }
    }

    // 发送到DB处理邀请接受（main=7, sub=0xF）
    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 7, 0xF);
    xSendDBPacket << stAccept;
    xSendDBPacket.XParse << pServer->GetServerID();
    xSendDBPacket.XParse << biJoinDate;

    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
}

// 对齐 IDA 0x140076d60: 广播邀请加入通知
void CLeagueManager::SendLeagueInviteJoin(ST_LEAGUE_MEMBER_EX& stMemberEx, std::uint8_t byApplyState, ST_LEAGUE_INFO_EX& stInfoEx,
                                           ST_LEAGUE_INFO_UPDATE& stInfoUpdate, std::uint32_t dwActorID,
                                           ST_LEAGUE_MEMBER_LIST& stMemberList, ST_LEAGUE_INFO_FOR_GAME stLeagueInfoForGame,
                                           std::int32_t nSyncCount) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::SendLeagueInviteJoin actorID=%u applyState=%d", dwActorID, byApplyState);

    // 对齐 IDA: 发送广播包(0xF6, 0x41)
    XSendPacket xSendPacket(0xF6, 0x41);
    xSendPacket << stMemberEx;
    xSendPacket.XParse << byApplyState;
    xSendPacket << stInfoEx;
    xSendPacket << stInfoUpdate;
    xSendPacket.XParse << dwActorID;
    xSendPacket << stLeagueInfoForGame;
    xSendPacket.XParse << nSyncCount;

    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

// 对齐 IDA 0x140075c70: 邀请成功后的成员数据同步
void CLeagueManager::ResInviteUser(CServer* pServer, std::int32_t nLeagueID, ST_LEAGUE_MEMBER_EX stMemberEx, std::uint32_t dwReqActorID) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ResInviteUser leagueID=%d reqActorID=%u ucid=%u", nLeagueID, dwReqActorID, stMemberEx.dwUCID);

    // 查找联赛
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay", "ResInviteUser error - Not Exist League[ LeagueID%d ]( %d )", nLeagueID, 619);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay", "ResInviteUser error - Not Exist League[ LeagueID%d ]( %d )", nLeagueID, 626);
        return;
    }

    // 对齐 IDA: 获取被邀请用户并更新登录状态和频道信息
    auto pUser = TXSingleton<XRelayServer>::Instance()->GetUser(stMemberEx.dwUCID);
    if (!pUser) {
        return;
    }

    stMemberEx.bLogin = true;
    stMemberEx.byChannel = pUser->GetChannel();

    // 对齐 IDA: 复制成员信息并添加到联赛
    ST_LEAGUE_MEMBER_EX stMemberInfo = stMemberEx;
    pLeague->AddMember(stMemberInfo);

    // 对齐 IDA: 设置联赛游戏信息（根据职位）
    ST_LEAGUE_INFO_FOR_GAME stLeagueInfoForGame{};
    pLeague->SetLeagueInfoForGame(stMemberInfo.stMember.byPosition, stLeagueInfoForGame);

    // 对齐 IDA: 获取联赛信息
    ST_LEAGUE_INFO stInfo{};
    pLeague->GetLeagueInfo(stInfo);

    // 对齐 IDA: 获取各种列表
    ST_LEAGUE_MEMBER_LIST stMemberList{};
    pLeague->GetMemberList(stMemberList);

    ST_LEAGUE_APPLICANT_LIST stApplicantList{};
    pLeague->GetApplicantList(stApplicantList);

    ST_LEAGUE_BOARD_LIST stBoardList{};
    pLeague->GetBoardList(stBoardList);

    // 对齐 IDA: 从申请列表中删除该成员，并获取是否成功删除
    std::uint8_t byApplyState = 0;
    if (pLeague->DelApplicant(stMemberInfo.dwUCID)) {
        byApplyState = 1;
    }

    // 对齐 IDA: 删除申请列表中的所有申请
    DeleteApplicantList(pServer, stMemberInfo.dwUCID);

    // 对齐 IDA: 构建联赛扩展信息 (只设置 nLeagueID, dwLeagueCard, szLeagueName)
    ST_LEAGUE_INFO_EX stInfoEx{};
    stInfoEx.nLeagueID = stInfo.nLeagueID;
    stInfoEx.dwLeagueCard = stInfo.dwLeagueCard;
    wcscpy_s(stInfoEx.szLeagueName, stInfo.szLeagueName);

    // 对齐 IDA: 构建联赛信息更新结构
    ST_LEAGUE_INFO_UPDATE stInfoUpdate{};
    stInfoUpdate.nLeagueID = stInfo.nLeagueID;
    stInfoUpdate.nLeagueRank = stInfo.nLeagueRank;
    stInfoUpdate.biLeagueMoney = stInfo.biMoney;
    stInfoUpdate.shLeagueMemeberCnt = static_cast<std::int16_t>(pLeague->GetMemberCount());
    stInfoUpdate.dwLeagueCard = stInfo.dwLeagueCard;
    stInfoUpdate.biExp = stInfo.biExp;

    // 对齐 IDA: 更新成员数量
    pLeague->SetMemberCount(static_cast<std::int16_t>(pLeague->GetMemberCount()));

    // 对齐 IDA: 再次获取联赛信息 (用于创建记录)
    pLeague->GetLeagueInfo(stInfo);

    // 对齐 IDA: 创建加入记录 (byFlag=1, szValue1=成员名称)
    ST_LEAGUE_RECORD stRecord{};
    stRecord.byFlag = 1;  // 加入记录标志
    stRecord.nLeagueID = stInfo.nLeagueID;
    stRecord.biRegisterDate = TXSingleton<XRelayServer>::Instance()->GetCurDateSec();
    wcscpy_s(stRecord.szValue1, stMemberInfo.szName);  // 成员名称而非联赛名称

    // 对齐 IDA: 更新记录
    pLeague->UpdateRecord(stRecord);

    // 对齐 IDA: 获取记录列表副本
    ST_LEAGUE_RECORD_LIST stRecordList{};
    pLeague->GetRecordList(stRecordList);

    // 对齐 IDA: 更新同步计数
    pLeague->UpdateSyncCount();
    int nSyncCount = pLeague->GetSyncCount();

    // 对齐 IDA: 发送联赛信息给新成员 (第7参数固定为1)
    SendLeagueInfo(stMemberInfo.dwUCID, stInfo, stMemberList, stApplicantList, stBoardList,
                   static_cast<std::uint8_t>(1), stRecordList, stLeagueInfoForGame);

    // 对齐 IDA: 广播邀请加入通知 (使用 byApplyState 而非固定值1)
    SendLeagueInviteJoin(stMemberInfo, byApplyState, stInfoEx, stInfoUpdate, dwReqActorID, stMemberList, stLeagueInfoForGame, nSyncCount);
}

void CLeagueManager::AddInviteUser(std::uint32_t dwUCID, std::int32_t nLeagueID) {
    m_mpLeagueInvite[dwUCID] = ST_LEAGUE_INVITE_INFO{nLeagueID, true, 0};
}

bool CLeagueManager::CheckInviteUser(std::uint32_t dwUCID) {
    return m_mpLeagueInvite.find(dwUCID) != m_mpLeagueInvite.end();
}

std::uint32_t CLeagueManager::DeleteInviteUser(std::uint32_t dwUCID) {
    auto it = m_mpLeagueInvite.find(dwUCID);
    if (it != m_mpLeagueInvite.end()) {
        std::uint32_t leagueID = static_cast<std::uint32_t>(it->second.nLeagueID);
        m_mpLeagueInvite.erase(it);
        return leagueID;
    }
    return 0;
}

// ============================================================================
// 申请者管理
// ============================================================================

void CLeagueManager::ReqLeagueApplicant(const ST_LEAGUE_APPLICANT& stApplicant, CServer* pServer) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueApplicant leagueID=%d actorID=%u",
                       stApplicant.nLeagueID, stApplicant.dwActorID);

    // 查找联赛
    auto it = m_mpLeagueList.find(stApplicant.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        // 联赛不存在
        ST_LEAGUE_APPLICANT stError = stApplicant;
        stError.nResult = 57016;
        XSendPacket xSendPacket(0xF6, 0x19);
        xSendPacket << stError;
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "ReqLeagueApplicant League NULL(%d)", 1207);
        ST_LEAGUE_APPLICANT stError = stApplicant;
        stError.nResult = 57016;
        XSendPacket xSendPacket(0xF6, 0x19);
        xSendPacket << stError;
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
        return;
    }

    // 检查申请者数量限制
    if (!pLeague->CheckApplicantCount(stApplicant.nLeagueID)) {
        LogHelper::LogDebug("game.league", "Full League Applicant List");
        ST_LEAGUE_APPLICANT stError = stApplicant;
        stError.nResult = 57023;
        XSendPacket xSendPacket(0xF6, 0x19);
        xSendPacket << stError;
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
        return;
    }

    // 检查是否已存在申请
    if (pLeague->CheckExistApplicant(stApplicant.dwActorID)) {
        LogHelper::LogDebug("game.league", "Aleady Exist Applicant List [LeagueID:%d]", stApplicant.nLeagueID);
        ST_LEAGUE_APPLICANT stError = stApplicant;
        stError.nResult = 57026;
        XSendPacket xSendPacket(0xF6, 0x19);
        xSendPacket << stError;
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
        return;
    }

    // 发送到DB (main=7, sub=9)
    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 7, 9);
    xSendDBPacket << stApplicant;
    xSendDBPacket.XParse << pServer->GetServerID();
    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
}

void CLeagueManager::ResLeagueApplicant(CServer* pServer, const ST_LEAGUE_APPLICANT& stApplicant) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ResLeagueApplicant leagueID=%d actorID=%u result=%d",
                       stApplicant.nLeagueID, stApplicant.dwActorID, stApplicant.nResult);

    // 查找联赛
    auto it = m_mpLeagueList.find(stApplicant.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogDebug("game.league", "Not Exist League [LeagueID:%d]", stApplicant.nLeagueID);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "ResLeagueApplicant League NULL(%d)", 150);
        return;
    }

    // 添加申请者到联赛申请列表
    pLeague->AddApplicant(stApplicant);

    // 发送确认包给申请者（主命令0xF6，子命令0x19）
    XSendPacket xSendPacket(0xF6, 0x19);
    xSendPacket << stApplicant;
    if (pServer) {
        pServer->SendEx(xSendPacket);
    }

    // 广播给所有成员通知有新申请者（主命令0xF6，子命令0x20）
    XSendPacket xSendPacket2(0xF6, 0x20);
    xSendPacket2 << stApplicant;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket2);
}

void CLeagueManager::ReqLeagueApplicantAccept(CServer* pServer, const ST_REQ_LEAGUE_APPLICANT_ACCEPT& stAccept, std::uint32_t dwActorID) {
    // 对齐 IDA 0x1400774b0: 申请人接受处理
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueApplicantAccept leagueID=%d actorID=%u reqActorID=%u",
                       stAccept.nLeagueID, dwActorID, stAccept.dwReqActorID);

    std::int32_t nServerID = pServer ? pServer->GetServerID() : 0;

    // 查找联赛
    auto it = m_mpLeagueList.find(stAccept.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.league",
                           "ReqLeagueApplicantAccept error - Not Exist League[ LeagueID:%d, UCID:%d ]( %d )",
                           stAccept.nLeagueID, dwActorID, 1000);
        SendLeagueErrorMsg(dwActorID, 57016);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.league",
                           "ReqLeagueApplicantAccept error - Leauge is NULL[ LeagueID:%d, UCID:%d ]( %d )",
                           stAccept.nLeagueID, dwActorID, 1009);
        SendLeagueErrorMsg(dwActorID, 57016);
        return;
    }

    // 获取联赛信息
    ST_LEAGUE_INFO stInfo{};
    pLeague->GetLeagueInfo(stInfo);

    // 对齐 IDA: 通过 stInfo.byRating 查 TB_LEAGUE_INFO 表
    auto pTB_LeagueInfo = TXSingleton<XRelayServer>::Instance()->GetResourceMgr().GetTB_LEAGUE_INFO(stInfo.byRating);
    if (!pTB_LeagueInfo) {
        // 对齐 IDA 0x1400776bf: TB 表不存在报 57019
        LogHelper::LogError("game.league",
                           "ReqLeagueApplicantAccept error - No Table TB_LEAGUE_INFO[ LeagueID:%d, UCID:%d ]( %d )",
                           stAccept.nLeagueID, dwActorID, 1021);
        SendLeagueErrorMsg(dwActorID, 57019);
        return;
    }

    // 对齐 IDA 0x140077718: 检查成员数量上限
    std::int32_t nMemberCount = pLeague->GetMemberCount();
    if (nMemberCount >= static_cast<std::int32_t>(pTB_LeagueInfo->League_Member)) {
        LogHelper::LogError("game.league",
                           "ReqLeagueApplicantAccept error - Full League Member[ LeagueID:%d, UCID:%d, MemberCount:%d ]( %d )",
                           stAccept.nLeagueID, dwActorID, nMemberCount, 1029);
        SendLeagueErrorMsg(dwActorID, 57018);
        return;
    }

    // 获取操作者成员信息
    auto pMember = pLeague->GetLeagueMemberPtr(dwActorID);
    if (!pMember) {
        LogHelper::LogError("game.league",
                           "ReqLeagueApplicantAccept error - Not League Member[ LeagueID:%d, UCID:%d ]( %d )",
                           stAccept.nLeagueID, dwActorID, 1038);
        SendLeagueErrorMsg(dwActorID, 57007);
        return;
    }

    ST_LEAGUE_MEMBER_EX stMemberEx{};
    pMember->GetLeagueMember(stMemberEx);

    // 获取申请者名称
    wchar_t* pApplicantName = pLeague->GetApplicant(stAccept.dwReqActorID);
    if (!pApplicantName) {
        LogHelper::LogError("game.league",
                           "ReqLeagueApplicantAccept error - Invalid Applicant Name[ LeagueID:%d, ApplicantUCID:%d ]( %d )",
                           stAccept.nLeagueID, stAccept.dwReqActorID, 1050);
        SendLeagueErrorMsg(dwActorID, 57007);
        return;
    }

    // 对齐 IDA 0x1400778fe: 拷贝申请者名字到 stAccept.szReqName
    ST_REQ_LEAGUE_APPLICANT_ACCEPT stSendAccept = stAccept;
    wcscpy_s(stSendAccept.szReqName, pApplicantName);

    // 对齐 IDA: 检查权限 - 会长可以直接接受，非会长需要检查权限（auth & 1）
    // IDA 中通过 GetEventID == dwActorID 判断会长，这里使用 IsMaster
    if (pLeague->IsMaster(dwActorID)) {
        // 会长直接发送 DB 包
        IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
        XSendDBPacket xSendDBPacket(pObject, 7, 0x10);
        xSendDBPacket << stSendAccept;
        xSendDBPacket.XParse << nServerID;
        xSendDBPacket.XParse << dwActorID;

        TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
    } else if ((stInfo.nAuth[stMemberEx.stMember.byPosition] & 1) != 0) {
        // 有权限（auth & 1）也可以接受
        IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
        XSendDBPacket xSendDBPacket(pObject, 7, 0x10);
        xSendDBPacket << stSendAccept;
        xSendDBPacket.XParse << nServerID;
        xSendDBPacket.XParse << dwActorID;

        TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
    } else {
        // 无权限
        LogHelper::LogError("game.league",
                           "ReqLeagueApplicantAccept error - No Authority[ LeagueID:%d, UCID:%u ]( %d )",
                           stAccept.nLeagueID, dwActorID, 1070);
        SendLeagueErrorMsg(dwActorID, 57006);
    }
}

void CLeagueManager::ReqLeagueApplicantReject(CServer* pServer, const ST_REQ_LEAGUE_APPLICANT_REJECT& stReject) {
    // 对齐 IDA 0x140077b50: 签名只有 pServer + stReject，无额外 dwActorID 参数
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueApplicantReject leagueID=%d ucid=%u targetID=%u",
                       stReject.nLeagueID, stReject.dwUCID, stReject.dwTargetUCID);

    // 查找联赛
    auto it = m_mpLeagueList.find(stReject.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        // 对齐 IDA: 联赛不存在，发 0xF6/0x21 错误包
        ST_REQ_LEAGUE_APPLICANT_REJECT stError = stReject;
        stError.nResult = 57016;
        XSendPacket xSendPacket(0xF6, 0x21);
        xSendPacket << stError;
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "ReqLeagueApplicantReject League NULL(%d)", 1098);
        return;
    }

    // 获取联赛信息
    ST_LEAGUE_INFO stInfo{};
    pLeague->GetLeagueInfo(stInfo);

    // 对齐 IDA: 用 stReject.dwUCID 查找操作者成员
    auto pMember = pLeague->GetLeagueMemberPtr(stReject.dwUCID);
    if (!pMember) {
        LogHelper::LogDebug("game.league", "ReqLeagueApplicantReject Member NULL(%d)", 1108);
        return;
    }

    ST_LEAGUE_MEMBER_EX stMemberEx{};
    pMember->GetLeagueMember(stMemberEx);

    // 对齐 IDA: 检查权限 - 会长可以直接拒绝，非会长需要检查权限（auth & 1）
    // IDA 中通过 GetEventID() == stReject.dwUCID 判断会长
    if (pLeague->IsMaster(stReject.dwUCID)) {
        // 会长直接发送 DB 包
        IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
        XSendDBPacket xSendDBPacket(pObject, 7, 0x11);
        xSendDBPacket << stReject;
        xSendDBPacket.XParse << pServer->GetServerID();

        TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
    } else if ((stInfo.nAuth[stMemberEx.stMember.byPosition] & 1) != 0) {
        // 有权限（auth & 1）也可以拒绝
        IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
        XSendDBPacket xSendDBPacket(pObject, 7, 0x11);
        xSendDBPacket << stReject;
        xSendDBPacket.XParse << pServer->GetServerID();

        TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
    } else {
        // 无权限
        LogHelper::LogDebug("game.league", "No Authority [UCID:%d]", stReject.dwUCID);
        ST_REQ_LEAGUE_APPLICANT_REJECT stError = stReject;
        stError.nResult = 57006;
        XSendPacket xSendPacket(0xF6, 0x21);
        xSendPacket << stError;
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
    }
}

void CLeagueManager::ApplicantRejectSucc(CServer* pServer, ST_REQ_LEAGUE_APPLICANT_REJECT stReject) {
    // 对齐 IDA 0x140076fa0: 申请者拒绝成功处理
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ApplicantRejectSucc leagueID=%d targetID=%u",
                       stReject.nLeagueID, stReject.dwTargetUCID);

    // 查找联赛
    auto it = m_mpLeagueList.find(stReject.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay", "ApplicantRejectSucc error - Not exist League[ LeagueID:%d ]( %d )",
                           stReject.nLeagueID, 917);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay", "ApplicantRejectSucc error - Not exist League[ LeagueID:%d ]( %d )",
                           stReject.nLeagueID, 924);
        return;
    }

    // 删除申请者
    pLeague->DelApplicant(stReject.dwTargetUCID);

    // 更新同步计数
    pLeague->UpdateSyncCount();
    std::int32_t nSyncCount = pLeague->GetSyncCount();

    // 发送拒绝确认包给请求者（主命令0xF6，子命令0x21）
    XSendPacket xSendPacket(0xF6, 0x21);
    xSendPacket << stReject;
    if (pServer) {
        pServer->SendEx(xSendPacket);
    }

    // 广播申请者移除通知（主命令0xF6，子命令0x22）
    XSendPacket xSendPacket2(0xF6, 0x22);
    xSendPacket2.XParse << stReject.nLeagueID;
    xSendPacket2.XParse << stReject.dwTargetUCID;
    xSendPacket2.XParse << nSyncCount;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket2);
}

void CLeagueManager::ResLeagueApplicantDelete_TimeOver(std::int32_t nLeagueID, std::uint32_t dwActorID) {
    // 对齐 IDA 0x14007b5c0: 申请者超时自动删除
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "ResLeagueApplicantDelete_TimeOver pMember NULL(%d)", 1902);
        return;
    }

    // 删除申请者 - 对齐 IDA: CLeague::DelApplicant
    pLeague->DelApplicant(dwActorID);

    // 广播申请者删除通知（主命令0xF6，子命令0x22）
    XSendPacket xSendPacket(0xF6, 0x22);
    xSendPacket.XParse << nLeagueID;
    xSendPacket.XParse << dwActorID;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

// 对齐 IDA 0x140076ae0: 广播申请者加入通知
void CLeagueManager::SendLeagueApplicantJoin(ST_LEAGUE_MEMBER_EX& stMemberEx, ST_LEAGUE_INFO_EX& stInfoEx,
                                              ST_LEAGUE_INFO_UPDATE& stInfoUpdate, std::uint32_t dwActorID,
                                              ST_LEAGUE_MEMBER_LIST& stMemberList, ST_LEAGUE_INFO_FOR_GAME stLeagueInfoForGame,
                                              std::int32_t nSyncCount) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::SendLeagueApplicantJoin actorID=%u", dwActorID);

    // 对齐 IDA: 发送广播包(0xF6, 0x37)
    XSendPacket xSendPacket(0xF6, 0x37);
    xSendPacket << stMemberEx;
    xSendPacket << stInfoEx;
    xSendPacket << stInfoUpdate;
    xSendPacket.XParse << dwActorID;
    xSendPacket << stLeagueInfoForGame;
    xSendPacket.XParse << nSyncCount;

    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

// 对齐 IDA 0x140076400: 申请者加入成功处理
void CLeagueManager::AppliCantJoinSucc(CServer* pServer, const ST_REQ_LEAGUE_APPLICANT_ACCEPT& stAccept, const ST_LEAGUE_MEMBER_EX& stMemberEx, std::uint32_t dwActorID) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::AppliCantJoinSucc leagueID=%d actorID=%u ucid=%u result=%d", stAccept.nLeagueID, dwActorID, stMemberEx.dwUCID, stAccept.nResult);

    // 对齐 IDA 0x140076330: 使用 stMemberEx.nLeagueID 查找联赛
    auto it = m_mpLeagueList.find(stMemberEx.stMember.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay", "AppliCantJoinSucc error - Not Exist League [ LeagueID:%d ] ( %d )", stAccept.nLeagueID, 758);
        SendLeagueErrorMsg(dwActorID, 57016);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay", "AppliCantJoinSucc error - Not Exist League [ LeagueID:%d ] ( %d )", stAccept.nLeagueID, 767);
        SendLeagueErrorMsg(dwActorID, 57016);
        return;
    }

    // 对齐 IDA: 检查 nResult <= 0 的情况（成功）
    if (stAccept.nResult <= 0) {
        // 复制 stMemberEx 以便修改
        ST_LEAGUE_MEMBER_EX stMemberInfo = stMemberEx;

        // 对齐 IDA: 获取用户并设置登录状态和频道（用户不在线时 bLogin=false）
        std::uint16_t wChannel = 0;
        auto pUser = TXSingleton<XRelayServer>::Instance()->GetUser(stMemberInfo.dwUCID);
        if (!pUser) {
            stMemberInfo.bLogin = false;
        } else {
            wChannel = static_cast<std::uint16_t>(pUser->GetChannel());
            stMemberInfo.bLogin = true;
        }
        stMemberInfo.byChannel = static_cast<std::uint8_t>(wChannel);

        // 对齐 IDA: 添加成员
        pLeague->AddMember(stMemberInfo);

        // 对齐 IDA: 删除申请者 (使用 stAccept.dwReqActorID)
        pLeague->DelApplicant(stAccept.dwReqActorID);

        // 对齐 IDA: 删除申请列表中的所有申请 (使用 stAccept.dwReqActorID)
        DeleteApplicantList(pServer, stAccept.dwReqActorID);

        // 对齐 IDA: 初始化并获取各种列表
        ST_LEAGUE_MEMBER_LIST stMemberList{};
        pLeague->GetMemberList(stMemberList);

        ST_LEAGUE_INFO stInfo{};
        pLeague->GetLeagueInfo(stInfo);

        ST_LEAGUE_APPLICANT_LIST stApplicantList{};
        pLeague->GetApplicantList(stApplicantList);

        ST_LEAGUE_BOARD_LIST stBoardList{};
        pLeague->GetBoardList(stBoardList);

        // 对齐 IDA: 构建联赛扩展信息 (只设置 nLeagueID, dwLeagueCard, szLeagueName)
        ST_LEAGUE_INFO_EX stInfoEx{};
        stInfoEx.nLeagueID = stInfo.nLeagueID;
        stInfoEx.dwLeagueCard = stInfo.dwLeagueCard;
        wcscpy_s(stInfoEx.szLeagueName, stInfo.szLeagueName);

        // 对齐 IDA: 构建联赛信息更新结构
        ST_LEAGUE_INFO_UPDATE stInfoUpdate{};
        stInfoUpdate.nLeagueID = stInfo.nLeagueID;
        stInfoUpdate.nLeagueRank = stInfo.nLeagueRank;
        stInfoUpdate.biLeagueMoney = stInfo.biMoney;
        stInfoUpdate.dwLeagueCard = stInfo.dwLeagueCard;
        stInfoUpdate.shLeagueMemeberCnt = static_cast<std::int16_t>(pLeague->GetMemberCount());
        stInfoUpdate.biExp = stInfo.biExp;

        // 对齐 IDA: 更新成员数量
        pLeague->SetMemberCount(static_cast<std::int16_t>(pLeague->GetMemberCount()));

        // 对齐 IDA: 再次获取联赛信息
        pLeague->GetLeagueInfo(stInfo);

        // 对齐 IDA: 设置联赛游戏信息 (在 SetMemberCount 之后)
        ST_LEAGUE_INFO_FOR_GAME stLeagueInfoForGame{};
        pLeague->SetLeagueInfoForGame(stMemberInfo.stMember.byPosition, stLeagueInfoForGame);

        // 对齐 IDA: 更新同步计数
        pLeague->UpdateSyncCount();
        int nSyncCount = pLeague->GetSyncCount();

        // 对齐 IDA: 通过名称获取申请者用户并设置联赛ID
        auto pApplicantUser = TXSingleton<XRelayServer>::Instance()->GetUser(stMemberInfo.szName);
        if (pApplicantUser) {
            pApplicantUser->SetLeagueID(stInfoUpdate.nLeagueID);
        }

        // 对齐 IDA: 创建加入记录 (byFlag=1, szValue1=成员名称)
        ST_LEAGUE_RECORD stRecord{};
        stRecord.byFlag = 1;
        stRecord.nLeagueID = stInfo.nLeagueID;
        stRecord.biRegisterDate = TXSingleton<XRelayServer>::Instance()->GetCurDateSec();
        wcscpy_s(stRecord.szValue1, stMemberInfo.szName);

        // 对齐 IDA: 更新记录
        pLeague->UpdateRecord(stRecord);

        // 对齐 IDA: 获取记录列表副本
        ST_LEAGUE_RECORD_LIST stRecordList{};
        pLeague->GetRecordList(stRecordList);

        // 对齐 IDA: 发送联赛信息给新成员 (使用 stAccept.dwReqActorID)
        SendLeagueInfo(stAccept.dwReqActorID, stInfo, stMemberList, stApplicantList, stBoardList,
                       static_cast<std::uint8_t>(1), stRecordList, stLeagueInfoForGame);

        // 对齐 IDA: 广播申请者加入通知
        SendLeagueApplicantJoin(stMemberInfo, stInfoEx, stInfoUpdate, dwActorID, stMemberList, stLeagueInfoForGame, nSyncCount);
    } else {
        // 对齐 IDA: nResult > 0 时只删除申请列表
        DeleteApplicantList(pServer, stAccept.dwReqActorID);
    }
}

void CLeagueManager::DeleteApplicantList(CServer* pServer, std::uint32_t dwActorID) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::DeleteApplicantList actorID=%u", dwActorID);

    // 遍历所有联赛，删除该申请者的申请记录
    std::vector<std::int32_t> vecLeagueIDs;
    for (auto it = m_mpLeagueList.begin(); it != m_mpLeagueList.end(); ++it) {
        auto& pLeague = it->second;
        if (!pLeague) {
            continue;
        }

        if (pLeague->DelApplicant(dwActorID)) {
            vecLeagueIDs.push_back(it->first);
        }
    }

    // 广播申请者删除通知（主命令0xF6，子命令0x38）
    XSendPacket xSendPacket(0xF6, 0x38);
    xSendPacket.XParse << dwActorID;
    xSendPacket.XParse << static_cast<std::int32_t>(vecLeagueIDs.size());
    for (auto nLeagueID : vecLeagueIDs) {
        xSendPacket.XParse << nLeagueID;
    }
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

// ============================================================================
// 公告板管理
// ============================================================================

void CLeagueManager::ReqLeagueBoard(CServer* pServer, std::uint32_t dwActorID, const ST_LEAGUE_BOARD& stBoard, std::int32_t nLeagueID) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueBoard leagueID=%d actorID=%u", nLeagueID, dwActorID);

    // 查找联赛
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "ReqLeagueBoard League NULL(%d)", 447);
        return;
    }

    // 获取成员信息
    auto pMember = pLeague->GetLeagueMemberPtr(dwActorID);
    if (!pMember) {
        LogHelper::LogDebug("game.league", "ReqLeagueBoard Member NULL(%d)", 454);
        return;
    }

    ST_LEAGUE_MEMBER_EX stMemberInfo{};
    pMember->GetLeagueMember(stMemberInfo);

    // 检查公告板冷却时间
    if (stMemberInfo.biBoardLimitTime > 0) {
        std::int64_t biRemainTime = stMemberInfo.biBoardLimitTime + 1800 - stBoard.biEnrollDate;
        if (biRemainTime > 0) {
            // 在冷却期内，返回错误
            ST_LEAGUE_BOARD stError = stBoard;
            stError.nResult = 57022;
            XSendPacket xSendPacket(0xF6, 0x14);
            xSendPacket << stError;
            xSendPacket.XParse << dwActorID;
            xSendPacket.XParse << nLeagueID;
            xSendPacket.XParse << biRemainTime;
            if (pServer) {
                pServer->SendEx(xSendPacket);
            }
            return;
        }

        // 冷却时间已过，重置
        pMember->SetEnrollBoardDate(0);
    }

    // 发送到DB处理公告板（main=7, sub=5）
    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 7, 5);
    xSendDBPacket.XParse << pServer->GetServerID();
    xSendDBPacket << stBoard;
    xSendDBPacket.XParse << nLeagueID;
    xSendDBPacket.XParse << dwActorID;

    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
}

void CLeagueManager::ResLeagueBoard(CServer* pServer, std::uint32_t dwActorID, std::int32_t nLeagueID, ST_LEAGUE_BOARD stBoard) {
    // 对齐 IDA 0x140075670: 公告板 DB 响应处理
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ResLeagueBoard leagueID=%d actorID=%u result=%d",
                       nLeagueID, dwActorID, stBoard.nResult);

    // 查找联赛
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "ResLeagueBoard League NULL(%d)", 507);
        return;
    }

    // 添加公告
    pLeague->AddBoard(stBoard);

    // 设置成员公告板限制时间
    pLeague->SetMemberBoardLimit(dwActorID, stBoard.biEnrollDate);

    // 广播公告板更新通知（主命令0xF6，子命令0x14）
    XSendPacket xSendPacket(0xF6, 0x14);
    xSendPacket << stBoard;
    xSendPacket.XParse << dwActorID;
    xSendPacket.XParse << nLeagueID;
    xSendPacket.XParse << 0;  // biRemainTime
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

// ============================================================================
// 公告管理
// ============================================================================

void CLeagueManager::ReqLeagueNoticeChange(CServer* pServer, std::uint32_t dwActorID, const ST_LEAGUE_NOTICE& stNotice) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueNoticeChange leagueID=%d actorID=%u", stNotice.nLeagueID, dwActorID);

    // 查找联赛
    auto it = m_mpLeagueList.find(stNotice.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogDebug("game.league", "Not Exist League [LeagueID:%d]", stNotice.nLeagueID);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "ReqLeagueNoticeChange League NULL(%d)", 1397);
        return;
    }

    // 获取联赛信息
    ST_LEAGUE_INFO stInfo{};
    pLeague->GetLeagueInfo(stInfo);

    // 获取成员信息
    auto pMember = pLeague->GetLeagueMemberPtr(dwActorID);
    if (!pMember) {
        LogHelper::LogDebug("game.league", "ReqLeagueNoticeChange Member NULL(%d)", 1408);
        return;
    }

    ST_LEAGUE_MEMBER_EX stMemberEx{};
    pMember->GetLeagueMember(stMemberEx);

    // 检查权限：会长或拥有公告权限（auth & 0x10）
    if (!pLeague->IsMaster(dwActorID)) {
        if ((stInfo.nAuth[stMemberEx.stMember.byPosition] & 0x10) == 0) {
            LogHelper::LogDebug("game.league", "No Authority [UCID:%u]", dwActorID);
            return;
        }
    }

    // 发送到DB处理公告变更（main=7, sub=0x14）
    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 7, 0x14);
    xSendDBPacket << stNotice;
    xSendDBPacket.XParse << pServer->GetServerID();
    xSendDBPacket.XParse << dwActorID;

    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
}

void CLeagueManager::ResLeagueNoticeChange(CServer* pServer, ST_LEAGUE_NOTICE stNotice, std::uint32_t dwActorID) {
    // 对齐 IDA 0x1400795e0: 公告变更 DB 响应处理
    auto it = m_mpLeagueList.find(stNotice.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogDebug("game.league", "Not Exist League [LeagueID:%d]", stNotice.nLeagueID);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "ResLeagueNoticeChange League NULL(%d)", 1476);
        return;
    }

    // 设置联赛公告 - 对齐 IDA: CLeague::SetLeagueNotice
    pLeague->SetLeagueNotice(stNotice);

    // 通知成员 - 对齐 IDA: CLeague::SendNoticeToMember
    pLeague->SendNoticeToMember(stNotice, dwActorID);

    // 记录公告变更 - 对齐 IDA: ST_LEAGUE_RECORD byFlag=5
    ST_LEAGUE_RECORD stRecord{};
    stRecord.byFlag = 5;
    stRecord.biRegisterDate = TXSingleton<XRelayServer>::Instance()->GetCurDateSec();
    stRecord.nLeagueID = stNotice.nLeagueID;
    pLeague->UpdateRecord(stRecord);
}

void CLeagueManager::ReqLeagueRecruitNotice(CServer* pServer, std::uint32_t dwActorID, const ST_LEAGUE_RECRUIT_NOTICE& stNotice) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueRecruitNotice leagueID=%d actorID=%u", stNotice.nLeagueID, dwActorID);

    // 查找联赛
    auto it = m_mpLeagueList.find(stNotice.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogDebug("game.league", "Not Exist League [LeagueID:%d]", stNotice.nLeagueID);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        return;
    }

    // 获取成员信息
    auto pMember = pLeague->GetLeagueMemberPtr(dwActorID);
    if (!pMember) {
        return;
    }

    ST_LEAGUE_INFO stInfo{};
    pLeague->GetLeagueInfo(stInfo);
    ST_LEAGUE_MEMBER_EX stMemberEx{};
    pMember->GetLeagueMember(stMemberEx);

    // 检查30分钟冷却时间 - 对齐 IDA 0x14007e0f4
    std::int64_t biRegDate = pLeague->GetRecruitNoticeDate();
    if (biRegDate > 0) {
        std::int64_t biRemainTime = biRegDate + 1800 - stNotice.biRegDate;
        if (biRemainTime > 0) {
            // 冷却中，发送错误响应（主命令0xF6，子命令0x46）
            ST_LEAGUE_RECRUIT_NOTICE stErrNotice = stNotice;
            stErrNotice.nResult = 57040;
            XSendPacket xSendPacket(0xF6, 0x46);
            xSendPacket << stErrNotice;
            xSendPacket.XParse << biRemainTime;
            xSendPacket.XParse << dwActorID;
            if (pServer) {
                pServer->SendEx(xSendPacket);
            }
            return;
        }
        // 冷却已过，重置日期
        pLeague->SetLeagueRecruitNoticeDate(0);
    }

    // 检查权限：会长直接允许，非会长需要公告权限（auth & 0x10）
    if (!pLeague->IsMaster(dwActorID)) {
        if ((stInfo.nAuth[stMemberEx.stMember.byPosition] & 0x10) == 0) {
            LogHelper::LogDebug("game.league", "No Authority [UCID:%u]", dwActorID);
            // 权限不足，发送错误响应（主命令0xF6，子命令0x46）
            XSendPacket xSendPacket(0xF6, 0x46);
            ST_LEAGUE_RECRUIT_NOTICE stErrNotice = stNotice;
            stErrNotice.nResult = 57015;
            xSendPacket << stErrNotice;
            xSendPacket.XParse << static_cast<std::int64_t>(0);
            xSendPacket.XParse << dwActorID;
            if (pServer) {
                pServer->SendEx(xSendPacket);
            }
            return;
        }
    }

    // 发送到DB处理招募公告变更（main=7, sub=0x28）
    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 7, 0x28);
    xSendDBPacket << stNotice;
    xSendDBPacket.XParse << pServer->GetServerID();
    xSendDBPacket.XParse << dwActorID;

    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
}

void CLeagueManager::ResLeagueRecruitNotice(CServer* pServer, std::uint32_t dwActorID, const ST_LEAGUE_RECRUIT_NOTICE& stNotice) {
    // 对齐 IDA 0x14007e4e0 - DB 响应后设置招募公告并广播

    auto it = m_mpLeagueList.find(stNotice.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        return;
    }

    // 设置招募公告
    pLeague->SetLeagueRecruitNotice(const_cast<ST_LEAGUE_RECRUIT_NOTICE&>(stNotice));

    // 广播给所有成员（主命令0xF6，子命令0x46）
    XSendPacket xSendPacket(0xF6, 0x46);
    xSendPacket << stNotice;
    xSendPacket.XParse << static_cast<std::int64_t>(0);  // remain time = 0
    xSendPacket.XParse << dwActorID;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

// ============================================================================
// 权限和职位
// ============================================================================

void CLeagueManager::ReqLeagueChangeAuth(CServer* pServer, std::int32_t nLeagueID, std::uint32_t dwActorID, const ST_LEAGUE_AUTH_CHANGE& stAuth) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueChangeAuth leagueID=%d actorID=%u", nLeagueID, dwActorID);

    // 查找联赛
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogDebug("game.league", "Not Exist League [LeagueID:%d]", nLeagueID);
        // 发送错误消息（主命令0xF6，子命令0x28，错误码0xDEB8）
        XSendPacket xSendPacket(0xF6, 0x28);
        xSendPacket.XParse << static_cast<std::int32_t>(0xDEB8);
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "ReqLeagueChangeAuth League NULL(%d)", 1584);
        return;
    }

    // 只有会长可以修改权限
    if (!pLeague->IsMaster(dwActorID)) {
        LogHelper::LogDebug("game.league", "Not League Master [UCID:%u]", dwActorID);
        // 发送错误消息（主命令0xF6，子命令0x28，错误码0xDEAE）
        XSendPacket xSendPacket(0xF6, 0x28);
        xSendPacket.XParse << static_cast<std::int32_t>(0xDEAE);
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
        return;
    }

    // 发送到DB处理权限变更（main=7, sub=0x18）
    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 7, 0x18);
    xSendDBPacket << stAuth;
    xSendDBPacket.XParse << nLeagueID;
    xSendDBPacket.XParse << pServer->GetServerID();
    xSendDBPacket.XParse << dwActorID;

    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
}

void CLeagueManager::ResLeagueAuthChange(CServer* pServer, std::int32_t nLeagueID, ST_LEAGUE_AUTH_CHANGE stChange, std::uint32_t dwActorID) {
    // 对齐 IDA 0x14007a070: 权限变更 DB 响应处理
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ResLeagueAuthChange leagueID=%d actorID=%u result=%d",
                       nLeagueID, dwActorID, stChange.nResult);

    // 查找联赛
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay", "ResLeagueAuthChange error - Not Exist League[ LeagueID:%d ]( %d )",
                           nLeagueID, 1611);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay", "ResLeagueAuthChange error - Not Exist League[ LeagueID:%d ]( %d )",
                           nLeagueID, 1618);
        return;
    }

    // 设置联赛权限
    pLeague->SetLeagueAuth(stChange.nAuth);

    // 更新同步计数
    pLeague->UpdateSyncCount();
    std::int32_t nSyncCount = pLeague->GetSyncCount();

    // 广播权限变更通知（主命令0xF6，子命令0x28）
    XSendPacket xSendPacket(0xF6, 0x28);
    xSendPacket << stChange;
    xSendPacket.XParse << nLeagueID;
    xSendPacket.XParse << dwActorID;
    xSendPacket.XParse << nSyncCount;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

void CLeagueManager::ReqLeaguePositionNameChange(CServer* pServer, std::int32_t nLeagueID, const ST_LEAGUE_POSITION_NAME_CHANGE& stChange, std::uint32_t dwActorID) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeaguePositionNameChange leagueID=%d actorID=%u", nLeagueID, dwActorID);

    // 查找联赛
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogDebug("game.league", "Not Exist League [LeagueID:%d]", nLeagueID);
        // 发送错误消息（主命令0xF6，子命令0x33，错误码0xDEB8=57016）
        XSendPacket xSendPacket(0xF6, 0x33);
        xSendPacket.XParse << static_cast<std::int32_t>(0xDEB8);
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "ReqLeaguePositionNameChange League NULL(%d)", 1648);
        return;
    }

    // 检查是否是会长（只有会长可以修改职位名称）
    if (!pLeague->IsMaster(dwActorID)) {
        LogHelper::LogDebug("game.league", "Master Authority [UCID:%d]", dwActorID);
        // 发送错误消息（主命令0xF6，子命令0x27，错误码0xDEAE=57006）
        XSendPacket xSendPacket(0xF6, 0x27);
        xSendPacket.XParse << static_cast<std::int32_t>(0xDEAE);
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
        return;
    }

    // 发送到DB处理职位名称变更（main=7, sub=0x17）
    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 7, 0x17);
    xSendDBPacket.XParse << nLeagueID;
    xSendDBPacket.XParse << dwActorID;
    xSendDBPacket.XParse << pServer->GetServerID();

    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
}

void CLeagueManager::ResLeaguePositionNameChange(CServer* pServer, std::int32_t nLeagueID, std::uint32_t dwActorID, ST_LEAGUE_POSITION_NAME_CHANGE stChange) {
    // 对齐 IDA 0x14007a500: 职位名称变更 DB 响应处理
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogDebug("game.league", "Not Exist League [LeagueID:%d]", nLeagueID);
        pServer->SendErrorMessage(0xF6, 0x33, 0xDEB8);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "ResLeaguePositionNameChange League NULL(%d)", 1680);
        return;
    }

    // 获取联赛信息
    ST_LEAGUE_INFO stInfo{};
    pLeague->GetLeagueInfo(stInfo);

    // 设置联赛职位名称 - 对齐 IDA: CLeague::SetLeaguePosition
    pLeague->SetLeaguePosition(stChange);

    // 广播职位名称变更通知（主命令0xF6，子命令0x27）
    XSendPacket xSendPacket(0xF6, 0x27);
    xSendPacket.XParse << dwActorID;
    xSendPacket << stChange;
    xSendPacket.XParse << nLeagueID;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

void CLeagueManager::ReqLeagueMemberPositionChange(CServer* pServer, const ST_LEAGUE_MEMBER_POSITION& stPos, std::uint32_t dwActorID, std::int32_t nLeagueID) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueMemberPositionChange leagueID=%d operatorID=%u targetID=%u position=%u",
                       nLeagueID, dwActorID, stPos.dwActorID, stPos.byPosition);

    // 查找联赛
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogDebug("game.league", "Not Exist League [LeagueID:%d]", nLeagueID);
        // 发送错误包（主命令0xF6，子命令0x37）
        ST_LEAGUE_MEMBER_POSITION stError = stPos;
        stError.nResult = 57016;
        ST_LEAGUE_INFO_FOR_GAME stInfoForGame{};
        XSendPacket xSendPacket(0xF6, 0x37);
        xSendPacket << stError;
        xSendPacket.XParse << nLeagueID;
        xSendPacket.XParse << dwActorID;
        xSendPacket.XParse << 0;
        xSendPacket << stInfoForGame;
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "ReqLeagueMemberPositionChange League NULL(%d)", 1740);
        return;
    }

    // 获取操作者成员信息
    auto pMember = pLeague->GetLeagueMemberPtr(dwActorID);
    if (!pMember) {
        LogHelper::LogDebug("game.league", "ReqLeagueMemberPositionChange Member NULL(%d)", 1746);
        return;
    }

    ST_LEAGUE_MEMBER_EX stMemberEx{};
    ST_LEAGUE_INFO stInfo{};
    pLeague->GetLeagueInfo(stInfo);
    pMember->GetLeagueMember(stMemberEx);

    // 不能变更自己的职位
    if (dwActorID == stPos.dwActorID) {
        ST_LEAGUE_MEMBER_POSITION stError = stPos;
        stError.nResult = 57036;
        ST_LEAGUE_INFO_FOR_GAME stInfoForGame{};
        XSendPacket xSendPacket(0xF6, 0x37);
        xSendPacket << stError;
        xSendPacket.XParse << nLeagueID;
        xSendPacket.XParse << dwActorID;
        xSendPacket.XParse << 0;
        xSendPacket << stInfoForGame;
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
        return;
    }

    // 检查目标职位数量限制
    if (!pLeague->CheckPositionCount(stPos.byPosition)) {
        ST_LEAGUE_MEMBER_POSITION stError = stPos;
        stError.nResult = 57030;
        ST_LEAGUE_INFO_FOR_GAME stInfoForGame{};
        XSendPacket xSendPacket(0xF6, 0x37);
        xSendPacket << stError;
        xSendPacket.XParse << nLeagueID;
        xSendPacket.XParse << dwActorID;
        xSendPacket.XParse << 0;
        xSendPacket << stInfoForGame;
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
        return;
    }

    // 检查权限：会长可以直接变更，非会长需要权限位（auth & 0x40）
    if (!pLeague->IsMaster(dwActorID)) {
        if ((stInfo.nAuth[stMemberEx.stMember.byPosition] & 0x40) == 0) {
            LogHelper::LogDebug("game.league", "No Authority [UCID:%u]", dwActorID);
            ST_LEAGUE_MEMBER_POSITION stError = stPos;
            stError.nResult = 57006;
            ST_LEAGUE_INFO_FOR_GAME stInfoForGame{};
            XSendPacket xSendPacket(0xF6, 0x37);
            xSendPacket << stError;
            xSendPacket.XParse << nLeagueID;
            xSendPacket.XParse << dwActorID;
            xSendPacket.XParse << 0;
            xSendPacket << stInfoForGame;
            if (pServer) {
                pServer->SendEx(xSendPacket);
            }
            return;
        }
    }

    // 发送到DB处理职位变更（main=7, sub=0x19）
    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 7, 0x19);
    xSendDBPacket << stPos;
    xSendDBPacket.XParse << dwActorID;
    xSendDBPacket.XParse << nLeagueID;
    xSendDBPacket.XParse << pServer->GetServerID();

    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
}

void CLeagueManager::ResLeagueMemberPositionChange(CServer* pServer, ST_LEAGUE_MEMBER_POSITION stPosition, std::int32_t nLeagueID, std::uint32_t dwActorID) {
    // 对齐 IDA 0x14007b040: 成员职位变更 DB 响应处理
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "ResLeagueMemberPositionChange error - Not Exist League [LeagueID:%d] ( %d )",
                           nLeagueID, 1824);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay",
                           "ResLeagueMemberPositionChange error - Not Exist League [LeagueID:%d] ( %d )",
                           nLeagueID, 1831);
        return;
    }

    // 更新同步计数 - 对齐 IDA: CLeague::UpdateSyncCount
    pLeague->UpdateSyncCount();

    // 变更成员职位 - 对齐 IDA: CLeague::ChangeMemberPosition
    pLeague->ChangeMemberPosition(stPosition, dwActorID);

    // 获取成员指针以获取名称
    auto pMember = pLeague->GetLeagueMemberPtr(stPosition.dwActorID);
    if (!pMember) {
        LogHelper::LogError("game.relay",
                           "ResLeagueMemberPositionChange error - Not Exist League Member[ LeagueID:%d, UCID:%d ] ( %d )",
                           nLeagueID, stPosition.dwActorID, 1843);
        return;
    }

    // 获取成员名称
    wchar_t szTargetName[24] = {};
    pMember->GetName(szTargetName);

    // 记录职位变更 - 对齐 IDA: ST_LEAGUE_RECORD byFlag=4
    ST_LEAGUE_RECORD stRecord{};
    stRecord.byFlag = 4;
    stRecord.biRegisterDate = TXSingleton<XRelayServer>::Instance()->GetCurDateSec();
    stRecord.nLeagueID = nLeagueID;
    wcscpy_s(stRecord.szValue1, szTargetName);
    stRecord.nValue3 = stPosition.byPosition;
    pLeague->UpdateRecord(stRecord);
}

// ============================================================================
// 退出/踢人
// ============================================================================

void CLeagueManager::ReqLeagueWithDraw(CServer* pServer, UXActorID uxActorID, std::int32_t nLeagueID, std::int64_t biPenalty) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueWithDraw leagueID=%d actorID=%u", nLeagueID, uxActorID.dwActorID);

    // 查找联赛
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogDebug("game.league", "Not Exist League [LeagueID:%d]", nLeagueID);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "ReqLeagueWithDraw League NULL(%d)", 218);
        return;
    }

    // 检查是否是成员
    if (!pLeague->IsMember(uxActorID.dwActorID)) {
        LogHelper::LogDebug("game.league", "Not League Member [LeagueID:%d]", nLeagueID);
        return;
    }

    // 检查是否是会长（会长不能退出）
    if (pLeague->IsMaster(uxActorID.dwActorID)) {
        LogHelper::LogDebug("game.league", "Master Can't Withdraw League [UCID:%u]", uxActorID.dwActorID);
        // 对齐 IDA: 调用 SendErrorMessage 而非手动构造错误包
        if (pServer) {
            pServer->SendErrorMessage(0xF6, 8, 0xDEC4);
        }
        return;
    }

    // 发送到DB处理退出（main=7, sub=6）
    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 7, 6);
    xSendDBPacket.XParse << uxActorID.dwActorID;
    xSendDBPacket.XParse << nLeagueID;
    xSendDBPacket.XParse << biPenalty;
    xSendDBPacket.XParse << pServer->GetServerID();

    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
}

bool CLeagueManager::ReqLeagueKick(CServer* pServer, std::uint32_t dwActorID, std::uint32_t dwTargetID, std::int32_t nLeagueID) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueKick leagueID=%d actorID=%u targetID=%u", nLeagueID, dwActorID, dwTargetID);

    // 查找联赛
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogDebug("game.league", "Not Exist League [LeagueID:%d]", nLeagueID);
        // 对齐 IDA 0x1400746e3: SendErrorMessage(0xF6, 9, 0xDEB8)
        if (pServer) {
            pServer->SendErrorMessage(0xF6, 9, 0xDEB8);
        }
        return false;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "ReqLeagueKick League NULL(%d)", 298);
        return false;
    }

    // 获取联赛信息
    ST_LEAGUE_INFO stInfo{};
    pLeague->GetLeagueInfo(stInfo);

    // 获取目标成员和操作者成员
    auto pTargetMember = pLeague->GetLeagueMemberPtr(dwTargetID);
    auto pAuthorityMember = pLeague->GetLeagueMemberPtr(dwActorID);

    if (!pTargetMember || !pAuthorityMember) {
        LogHelper::LogDebug("game.league", "ReqLeagueKick Member NULL(%d)", 311);
        return false;
    }

    ST_LEAGUE_MEMBER_EX stTarget{};
    ST_LEAGUE_MEMBER_EX stAuthority{};
    pTargetMember->GetLeagueMember(stTarget);
    pAuthorityMember->GetLeagueMember(stAuthority);

    // 不能踢会长（会长 dwActorID == GetEventID）
    if (pLeague->IsMaster(dwTargetID)) {
        LogHelper::LogDebug("game.league", "Can not Kick League Master [Target:%u]", dwTargetID);
        // 对齐 IDA 0x1400748dc: SendErrorMessage(0xF6, 9, 0xDEAE)
        if (pServer) {
            pServer->SendErrorMessage(0xF6, 9, 0xDEAE);
        }
        return false;
    }

    // 不能踢自己
    if (dwActorID == dwTargetID) {
        LogHelper::LogDebug("game.league", "Not Exist Kick Member [Target:%u]", dwTargetID);
        // 对齐 IDA 0x14007495b: SendErrorMessage(0xF6, 9, 0xC73E)
        if (pServer) {
            pServer->SendErrorMessage(0xF6, 9, 0xC73E);
        }
        return false;
    }

    // 会长可以直接踢人
    if (pLeague->IsMaster(dwActorID)) {
        IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
        XSendDBPacket xSendDBPacket(pObject, 7, 0xB);
        xSendDBPacket.XParse << dwActorID;
        xSendDBPacket.XParse << dwTargetID;
        xSendDBPacket.XParse << nLeagueID;
        xSendDBPacket.XParse << pServer->GetServerID();

        TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
        return true;
    }

    // 非会长需要检查踢人权限（auth & 2）
    if ((stInfo.nAuth[stAuthority.stMember.byPosition] & 2) == 0) {
        LogHelper::LogDebug("game.league", "No Authority [UCID:%u]", dwActorID);
        // 对齐 IDA 0x140074af6: SendErrorMessage(0xF6, 9, 0xDECA)
        if (pServer) {
            pServer->SendErrorMessage(0xF6, 9, 0xDECA);
        }
        return false;
    }

    // 检查目标是否也有踢人权限（不能踢同级）
    if ((stInfo.nAuth[stTarget.stMember.byPosition] & 2) != 0) {
        LogHelper::LogDebug("game.league", "Authority is Impossible Kick Out[Target:%u]", dwTargetID);
        // 对齐 IDA 0x140074bf9: SendErrorMessage(0xF6, 9, 0xDEAE)
        if (pServer) {
            pServer->SendErrorMessage(0xF6, 9, 0xDEAE);
        }
        return false;
    }

    // 发送踢人请求到DB（main=7, sub=0xB）
    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 7, 0xB);
    xSendDBPacket.XParse << dwActorID;
    xSendDBPacket.XParse << dwTargetID;
    xSendDBPacket.XParse << nLeagueID;
    xSendDBPacket.XParse << pServer->GetServerID();

    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
    return true;
}

// 对齐 IDA 0x14007d690
void CLeagueManager::SendLeagueMemberKick(CServer* pServer, std::int32_t nErrorCode, std::int32_t nLeagueID, std::uint32_t dwUCID, std::uint32_t dwTargetUCID, ST_LEAGUE_INFO_UPDATE& stUpdate, std::uint16_t shLevel, wchar_t* pName) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::SendLeagueMemberKick leagueID=%d errorCode=%d ucid=%u targetUCID=%u", nLeagueID, nErrorCode, dwUCID, dwTargetUCID);

    // 查找联赛
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "SendLeagueMemberKick error - Not Exist League[ LeagueID:%d ]( %d )",
                           nLeagueID, 2362);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay",
                           "SendLeagueMemberKick error - Not Exist League[ LeagueID:%d ]( %d )",
                           nLeagueID, 2369);
        return;
    }

    // 获取成员列表（IDA 中调用了 GetMemberList，但结果未使用，仅作初始化）
    ST_LEAGUE_MEMBER_LIST stMemberList{};
    pLeague->GetMemberList(stMemberList);

    // 更新同步计数
    pLeague->UpdateSyncCount();

    // 对齐 IDA: SendKickoutToMember(nLeagueID, nErrorCode, dwUCID, dwTargetUCID, stUpdate, shLevel)
    pLeague->SendKickoutToMember(nLeagueID, nErrorCode, dwUCID, dwTargetUCID, stUpdate, shLevel);

    // 对齐 IDA: 获取操作者名称
    wchar_t szReqUserName[24] = {};
    auto pMember = pLeague->GetLeagueMemberPtr(dwUCID);
    if (pMember) {
        pMember->GetName(szReqUserName);
    }

    // 对齐 IDA: 创建踢出记录 (byFlag=3 表示踢出)
    ST_LEAGUE_RECORD stRecord{};
    stRecord.nLeagueID = nLeagueID;
    stRecord.byFlag = 3;
    stRecord.biRegisterDate = TXSingleton<XRelayServer>::Instance()->GetCurDateSec();
    wcscpy_s(stRecord.szValue1, pName);
    wcscpy_s(stRecord.szValue2, szReqUserName);

    // 更新记录
    pLeague->UpdateRecord(stRecord);
}

// 对齐 IDA 0x1400815d0
void CLeagueManager::ResLeagueKickout(std::uint32_t dwUCID, std::uint32_t dwKickoutUCID, std::int32_t nLeagueID, std::int32_t nErrorCode, CServer* pServer) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ResLeagueKickout leagueID=%d ucid=%u kickoutUCID=%u errorCode=%d", nLeagueID, dwUCID, dwKickoutUCID, nErrorCode);

    // 查找联赛
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "ResLeagueKickout error - Not exist league[ LeagueID:%d, UCID:%d ] ( %d )",
                           nLeagueID, dwUCID, 3391);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay",
                           "ResLeagueKickout error - Not exist league Member[ LeagueID:%d, UCID:%d ] ( %d )",
                           nLeagueID, dwUCID, 3398);
        return;
    }

    // 对齐 IDA: 获取被踢成员信息
    ST_LEAGUE_MEMBER_EX stMemberInfo{};
    GetMemberInfo(nLeagueID, dwKickoutUCID, stMemberInfo);

    // 对齐 IDA: 如果被踢者是副盟主(position==7)，清空副盟主
    if (stMemberInfo.stMember.byPosition == 7) {
        wchar_t szSubMasterName[24] = {};
        pLeague->SetSubLeagueMaster(szSubMasterName);
    }

    std::int16_t shLevel = stMemberInfo.shLevel;

    // 对齐 IDA: 删除成员并获取名字
    wchar_t szKickoutUser[24] = {};
    DeleteLeagueMember(nLeagueID, dwKickoutUCID, szKickoutUser);

    // 对齐 IDA: 更新联赛信息
    ST_LEAGUE_INFO_UPDATE stInfoUpdate{};
    UpdateLeagueInfo(nLeagueID, stInfoUpdate);

    // 对齐 IDA: 获取被踢用户对象并清空联赛ID
    auto pKickoutUser = TXSingleton<XRelayServer>::Instance()->GetUser(dwKickoutUCID);
    if (pKickoutUser) {
        pKickoutUser->SetLeagueID(0);
    }

    // 对齐 IDA: 发送踢出通知
    SendLeagueMemberKick(pServer, nErrorCode, nLeagueID, dwUCID, dwKickoutUCID, stInfoUpdate, shLevel, szKickoutUser);
}

// ============================================================================
// 转让
// ============================================================================

void CLeagueManager::ReqLeagueDelegate(CServer* pServer, std::uint32_t dwReqUCID, const PS_REQ_LEAGUE_DELEGATE& stDelegate, bool bGMDelegate) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueDelegate leagueID=%d reqUCID=%u targetUCID=%u",
                       stDelegate.nLeagueID, dwReqUCID, stDelegate.dwDelegatedUCID);

    // 查找联赛
    auto it = m_mpLeagueList.find(stDelegate.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "ReqLeagueDelegate error - Not exist league[ LeagueID:%d, ReqUCID:%d ] ( %d )",
                           stDelegate.nLeagueID, dwReqUCID, 2583);
        // 发送错误响应（主命令0xF6，子命令7）
        PS_RES_LEAGUE_DELEGATE stRes{};
        stRes.nLeagueID = stDelegate.nLeagueID;
        stRes.nResult = 57016;
        // 错误响应场景无名称信息，保留空字符串
        XSendPacket xSendPacket(0xF6, 7);
        xSendPacket << stRes;
        xSendPacket.XParse << dwReqUCID;
        xSendPacket.XParse << stDelegate.dwDelegatedUCID;
        xSendPacket.XParse << 0;
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        // 发送错误响应
        PS_RES_LEAGUE_DELEGATE stRes{};
        stRes.nLeagueID = stDelegate.nLeagueID;
        stRes.nResult = 57016;
        // 错误响应场景无名称信息，保留空字符串
        XSendPacket xSendPacket(0xF6, 7);
        xSendPacket << stRes;
        xSendPacket.XParse << dwReqUCID;
        xSendPacket.XParse << stDelegate.dwDelegatedUCID;
        xSendPacket.XParse << 0;
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
        return;
    }

    // 非GM转让需要检查权限
    std::int32_t nResult = 0;
    if (!bGMDelegate) {
        nResult = pLeague->CheckLeagueDelegate(dwReqUCID, stDelegate.dwDelegatedUCID);
    }

    if (nResult <= 0) {
        // 发送到DB处理转让（main=7, sub=0x32）
        IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
        XSendDBPacket xSendDBPacket(pObject, 7, 0x32);
        xSendDBPacket << stDelegate;
        xSendDBPacket.XParse << dwReqUCID;
        xSendDBPacket.XParse << pServer->GetServerID();

        TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
    }
    else {
        // 验证失败，发送错误响应
        PS_RES_LEAGUE_DELEGATE stRes{};
        stRes.nLeagueID = stDelegate.nLeagueID;
        stRes.nResult = nResult;
        // 错误响应场景无名称信息，保留空字符串
        XSendPacket xSendPacket(0xF6, 7);
        xSendPacket << stRes;
        xSendPacket.XParse << dwReqUCID;
        xSendPacket.XParse << stDelegate.dwDelegatedUCID;
        xSendPacket.XParse << 0;
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
    }
}

void CLeagueManager::ResLeagueDelegate(CServer* pServer, std::uint32_t dwReqUCID, const PS_REQ_LEAGUE_DELEGATE& stDelegate, int nErrorCode) {
    // 对齐 IDA 0x14007ec00
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ResLeagueDelegate leagueID=%d reqUCID=%u errCode=%d",
                       stDelegate.nLeagueID, dwReqUCID, nErrorCode);

    PS_RES_LEAGUE_DELEGATE stRes{};

    // 查找联赛
    auto it = m_mpLeagueList.find(stDelegate.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "ResLeagueDelegate error - Not exist league[ LeagueID:%d, ReqUCID:%d ] ( %d )",
                           stDelegate.nLeagueID, dwReqUCID, 2636);
        stRes.nResult = 57016;
    }

    auto pLeague = (it != m_mpLeagueList.end()) ? it->second : std::shared_ptr<CLeague>{};
    if (!pLeague) {
        stRes.nResult = 57016;
    }

    if (stRes.nResult > 0 || nErrorCode) {
        if (nErrorCode) {
            LogHelper::LogError("game.relay",
                               "ResLeagueDelegate error - DB Fail[ ErrorCode:%d, UCID:%d ] ( %d )",
                               nErrorCode, dwReqUCID, 2648);
            stRes.nResult = 57044;
        }
        std::int32_t nSyncCount = 0;
        XSendPacket xSendPacket(0xF6, 7);
        xSendPacket << stRes;
        xSendPacket.XParse << dwReqUCID;
        xSendPacket.XParse << stDelegate.dwDelegatedUCID;
        xSendPacket.XParse << nSyncCount;
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
        return;
    }

    // 成功路径：调用 CLeague::Delegate
    if (pLeague->Delegate(const_cast<PS_REQ_LEAGUE_DELEGATE&>(stDelegate), stRes, dwReqUCID)) {
        pLeague->UpdateSyncCount();
        pLeague->SendDelegateToMember(stRes, dwReqUCID, stDelegate.dwDelegatedUCID);
    } else {
        stRes.nResult = 57007;
        XSendPacket xSendPacket(0xF6, 7);
        xSendPacket << stRes;
        xSendPacket.XParse << dwReqUCID;
        xSendPacket.XParse << stDelegate.dwDelegatedUCID;
        xSendPacket.XParse << 0;
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
    }
}

// ============================================================================
// 开放/关闭
// ============================================================================

void CLeagueManager::ReqLeagueOpenOrNot(CServer* pServer, const ST_LEAGUE_OPEN& stOpen, std::uint32_t dwActorID) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueOpenOrNot leagueID=%d actorID=%u open=%d", stOpen.nLeagueID, dwActorID, stOpen.bOpen);

    // 对齐 IDA 0x14007d960
    // 查找联赛
    auto it = m_mpLeagueList.find(stOpen.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogDebug("game.league", "Not Exist League [LeagueID:%d]", stOpen.nLeagueID);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        return;
    }

    // 检查是否是会长 - 对齐 IDA: GetMasterID() == dwActorID
    if (!pLeague->IsMaster(dwActorID)) {
        // 对齐 IDA: SendErrorMessage(0xF6, 0x45, 0xDEB7)
        pServer->SendErrorMessage(0xF6, 0x45, 0xDEB7);
        return;
    }

    // 发送到DB处理开放状态变更（main=7, sub=0x27）
    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 7, 0x27);
    xSendDBPacket << stOpen;
    xSendDBPacket.XParse << pServer->GetServerID();
    xSendDBPacket.XParse << dwActorID;

    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
}

void CLeagueManager::ResLeagueOpenOrNot(CServer* pServer, ST_LEAGUE_OPEN stOpen, std::uint32_t dwUCID) {
    // 对齐 IDA 0x14007db60: 开放状态变更 DB 响应处理
    auto it = m_mpLeagueList.find(stOpen.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        return;
    }

    // 设置联赛开放状态 - 对齐 IDA: CLeague::SetLeagueOpenOrNot
    pLeague->SetLeagueOpenOrNot(stOpen.bOpen);

    // 维护开放联赛列表 m_vecLeagueList - 对齐 IDA
    if (stOpen.bOpen) {
        // 开放：添加到列表
        m_vecLeagueList.push_back(pLeague);
    } else {
        // 关闭：从列表中移除
        for (auto vecIt = m_vecLeagueList.begin(); vecIt != m_vecLeagueList.end(); ++vecIt) {
            if (*vecIt && (*vecIt)->GetLeagueID() == stOpen.nLeagueID) {
                m_vecLeagueList.erase(vecIt);
                break;
            }
        }
    }

    // 发送开放状态变更通知（主命令0xF6，子命令0x45）
    XSendPacket xSendPacket(0xF6, 0x45);
    xSendPacket << stOpen;
    xSendPacket.XParse << dwUCID;
    if (pServer) {
        pServer->SendEx(xSendPacket);
    }
}

// ============================================================================
// 名称变更
// ============================================================================

bool CLeagueManager::ReqLeagueNameChange(const PS_LEAGUE_NAME_CHANGE_SERVER& stChange) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueNameChange leagueID=%d ucid=%u", stChange.nLeagueID, stChange.dwUCID);

    // 查找联赛
    auto it = m_mpLeagueList.find(stChange.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "ReqLeagueNameChange error - Not exist league[ LeagueID:%d, UCID:%d ] ( %d )",
                           stChange.nLeagueID, stChange.dwUCID, 3434);
        const_cast<PS_LEAGUE_NAME_CHANGE_SERVER&>(stChange).nResult = 57005;
        return false;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay",
                           "ReqLeagueNameChange error - Not exist league Member[ LeagueID:%d, UCID:%d ] ( %d )",
                           stChange.nLeagueID, stChange.dwUCID, 3443);
        const_cast<PS_LEAGUE_NAME_CHANGE_SERVER&>(stChange).nResult = 57005;
        return false;
    }

    // 检查是否是会长
    if (pLeague->IsMaster(stChange.dwUCID)) {
        return true;
    } else {
        LogHelper::LogError("game.relay",
                           "ReqLeagueNameChange error - Master[ LeagueID:%d, UCID:%d ] ( %d )",
                           stChange.nLeagueID, stChange.dwUCID, 3452);
        const_cast<PS_LEAGUE_NAME_CHANGE_SERVER&>(stChange).nResult = 57015;
        return false;
    }
}

// 对齐 IDA 0x140081a80 (ResLeaugeNameChange): DB 名称变更响应处理
bool CLeagueManager::ResLeagueNameChange(PS_LEAGUE_NAME_CHANGE_SERVER stChange) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ResLeagueNameChange leagueID=%d ucid=%u", stChange.nLeagueID, stChange.dwUCID);

    // 查找联赛
    auto it = m_mpLeagueList.find(stChange.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "ResLeaugeNameChange error - Not exist league[ LeagueID:%d, UCID:%d ] ( %d )",
                           stChange.nLeagueID, stChange.dwUCID, 3466);
        stChange.nResult = 57005;
        return false;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay",
                           "ResLeaugeNameChange error - Not exist league Member[ LeagueID:%d, UCID:%d ] ( %d )",
                           stChange.nLeagueID, stChange.dwUCID, 3475);
        stChange.nResult = 57005;
        return false;
    }

    // 对齐 IDA: SetLeagueName → UpdateSyncCount → SendChangeLeagueName
    pLeague->SetLeagueName(stChange.szLeagueName);
    pLeague->UpdateSyncCount();
    pLeague->SendChangeLeagueName(stChange);

    return true;
}

// ============================================================================
// 卡片变更
// ============================================================================

void CLeagueManager::ReqLeagueCardChange(CServer* pServer, std::uint32_t dwActorID, const PS_REQ_LEAGUE_CARD& stCard, const struct PS_RES_STORAGE_INFO& stStorage) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueCardChange leagueID=%d ucid=%u", stCard.nLeagueID, dwActorID);

    // 查找联赛
    auto it = m_mpLeagueList.find(stCard.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "ReqLeagueCardChange error - Not exist league[ LeagueID:%d ] ( %d )",
                           stCard.nLeagueID, 2694);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay",
                           "ReqLeagueCardChange error - Not exist league[ LeagueID:%d ] ( %d )",
                           stCard.nLeagueID, 2701);
        return;
    }

    // 检查卡片变更条件
    std::int32_t nResult = pLeague->CheckLeagueCardChange(dwActorID);
    if (nResult <= 0) {
        // 检查通过，发送到DB处理卡片变更（main=7, sub=0x16）
        IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
        XSendDBPacket xSendDBPacket(pObject, 7, 0x16);
        // 序列化 PS_REQ_LEAGUE_CARD（使用完整结构体序列化器）
        xSendDBPacket << stCard;
        xSendDBPacket.XParse << dwActorID;
        xSendDBPacket.XParse << pServer->GetServerID();
        // 序列化 PS_RES_STORAGE_INFO（物品列表）
        xSendDBPacket.XParse << static_cast<std::uint8_t>(stStorage.vecItem.size());
        for (const auto& item : stStorage.vecItem) {
            xSendDBPacket.XParse << item.byInvenType;
            xSendDBPacket.XParse << item.shSlotPos;
            xSendDBPacket << item.stItem;
        }
        xSendDBPacket.XParse << stStorage.byType;
        TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
    } else {
        // 检查失败，发送错误响应（主命令0xF6，子命令0x48）
        XSendPacket xSendPacket(0xF6, 0x48);
        xSendPacket.XParse << dwActorID;
        // 序列化 PS_REQ_LEAGUE_CARD（使用完整结构体序列化器）
        // 注意：错误码覆盖 nResult 字段
        PS_REQ_LEAGUE_CARD stCardErr = stCard;
        stCardErr.nResult = nResult;
        xSendPacket << stCardErr;
        // 序列化 PS_RES_STORAGE_INFO（物品列表）
        xSendPacket.XParse << static_cast<std::uint8_t>(stStorage.vecItem.size());
        for (const auto& item : stStorage.vecItem) {
            xSendPacket.XParse << item.byInvenType;
            xSendPacket.XParse << item.shSlotPos;
            xSendPacket << item.stItem;
        }
        xSendPacket.XParse << stStorage.byType;
        xSendPacket.XParse << static_cast<std::int32_t>(0);  // nErrorCode
        xSendPacket.XParse << static_cast<std::int32_t>(0);  // nSyncCount
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
    }
}

void CLeagueManager::ResLeagueCardChange(CServer* pServer, PS_REQ_LEAGUE_CARD& stCard, std::uint32_t dwUCID, PS_RES_STORAGE_INFO& stStorage, int nErrorCode) {
    // 对齐 IDA 0x14007f090
    auto it = m_mpLeagueList.find(stCard.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "ReqLeagueCardChange error - Not exist league[ LeagueID:%d ] ( %d )",
                           stCard.nLeagueID, 2738);
        return;
    }

    auto pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay",
                           "ReqLeagueCardChange error - Not exist league[ LeagueID:%d ] ( %d )",
                           stCard.nLeagueID, 2745);
        return;
    }

    // 获取联赛成员
    auto pMember = pLeague->GetLeagueMemberPtr(dwUCID);
    if (!pMember) {
        LogHelper::LogError("game.relay",
                           "ReqLeagueCardChange error - League member is NULL[ LeagueID:%d, UCID:%d ] ( %d )",
                           stCard.nLeagueID, dwUCID, 2752);
        return;
    }

    // 获取成员信息和联赛游戏信息
    ST_LEAGUE_MEMBER_EX stMemberInfo{};
    pMember->GetLeagueMember(stMemberInfo);
    ST_LEAGUE_INFO_FOR_GAME stLeagueInfoForGame{};
    pLeague->SetLeagueInfoForGame(stMemberInfo.stMember.byPosition, stLeagueInfoForGame);

    if (nErrorCode) {
        // DB 返回错误，发送错误响应
        XSendPacket xSendPacket(0xF6, 0x48);
        xSendPacket.XParse << dwUCID;
        xSendPacket << stCard;
        xSendPacket << stStorage;
        xSendPacket.XParse << nErrorCode;
        xSendPacket.XParse << static_cast<std::int32_t>(0);  // nSyncCount
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
    } else {
        // 成功：更新卡片 - 对齐 IDA 0x14007f090
        // 顺序：CardChange -> UpdateSyncCount -> SendPacket -> SendChangeCardToMember -> UpdateRecord
        pLeague->CardChange(stCard, dwUCID);
        pLeague->UpdateSyncCount();

        std::int32_t nSyncCount = pLeague->GetSyncCount();

        // 发送响应包 (0xF6, 0x48)
        XSendPacket xSendPacket(0xF6, 0x48);
        xSendPacket.XParse << dwUCID;
        xSendPacket << stCard;
        xSendPacket << stStorage;
        xSendPacket.XParse << static_cast<std::int32_t>(0);  // nErrorCode
        xSendPacket.XParse << nSyncCount;
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }

        // 通知成员卡片变更
        pLeague->SendChangeCardToMember(dwUCID, stCard);

        // 记录日志
        ST_LEAGUE_RECORD stRecord{};
        stRecord.nLeagueID = stCard.nLeagueID;
        stRecord.byFlag = 11;  // 对齐 IDA - 卡片变更标志
        stRecord.biRegisterDate = TXSingleton<XRelayServer>::Instance()->GetCurDateSec();
        pLeague->UpdateRecord(stRecord);
    }
}

// ============================================================================
// 技能和等级
// ============================================================================

void CLeagueManager::ReqLeagueSkillLearn(CServer* pServer, const PS_REQ_LEAGUE_SKILL& stSkill) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueSkillLearn leagueID=%d ucid=%u", stSkill.nLeagueID, stSkill.dwUCID);

    // 获取请求用户
    std::shared_ptr<CUserObject> pReqUser = TXSingleton<XRelayServer>::Instance()->GetUser(stSkill.dwUCID);
    if (!pReqUser) {
        return;
    }

    // 查找联赛
    auto it = m_mpLeagueList.find(stSkill.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "ReqLeagueSkillLearn error - Not exist league[ LeagueID:%d ] ( %d )",
                           stSkill.nLeagueID, 2883);
        pReqUser->SetLockLeague(0);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay",
                           "ReqLeagueSkillLearn error - Not exist league[ LeagueID:%d ] ( %d )",
                           stSkill.nLeagueID, 2893);
        pReqUser->SetLockLeague(0);
        return;
    }

    // 检查技能学习条件
    PS_RES_LEAGUE_SKILL stResSkill{};
    PS_REQ_LEAGUE_SKILL stSkillCopy = stSkill;
    std::int32_t nCheckResult = pLeague->CheckLearnSkill(stSkillCopy, stResSkill);
    stResSkill.nResult = nCheckResult;

    if (nCheckResult <= 0) {
        // 检查通过，发送到DB处理（main=7, sub=0x35）
        IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
        XSendDBPacket xSendDBPacket(pObject, 7, 0x35);
        // 序列化 PS_RES_LEAGUE_SKILL 字段
        xSendDBPacket.XParse << stResSkill.nLeagueID;
        xSendDBPacket.XParse << stResSkill.dwUCID;
        xSendDBPacket.XParse << stResSkill.bySkillIndex;
        xSendDBPacket.XParse << stResSkill.bySkillGroupID;
        xSendDBPacket.XParse << stResSkill.bySkillLevel;
        xSendDBPacket.XParse << stResSkill.bySkillPoint;
        xSendDBPacket.XParse << stResSkill.biGold;
        xSendDBPacket.XParse << stResSkill.nResult;
        xSendDBPacket.XParse << stSkill.byType;
        TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
    } else {
        // 检查失败，发送错误响应
        LogHelper::LogError("game.relay",
                           "ReqLeagueSKillLearn error - Failed learn league skill[ Error:%d ]( %d)",
                           nCheckResult, 2905);
        pReqUser->SetLockLeague(0);

        std::int32_t nSyncCount = pLeague->GetSyncCount();
        XSendPacket xSendPacket(0xF6, 0x53);
        // 序列化 PS_RES_LEAGUE_SKILL 字段
        xSendPacket.XParse << stResSkill.nLeagueID;
        xSendPacket.XParse << stResSkill.dwUCID;
        xSendPacket.XParse << stResSkill.bySkillIndex;
        xSendPacket.XParse << stResSkill.bySkillGroupID;
        xSendPacket.XParse << stResSkill.bySkillLevel;
        xSendPacket.XParse << stResSkill.bySkillPoint;
        xSendPacket.XParse << stResSkill.biGold;
        xSendPacket.XParse << stResSkill.nResult;
        xSendPacket.XParse << nSyncCount;
        if (pServer) {
            pServer->SendEx(xSendPacket);
        }
    }
}

void CLeagueManager::ResLeagueSkillLearn(const PS_RES_LEAGUE_SKILL& stSkill, std::uint8_t byType) {
    // 对齐 IDA 0x14007fd30
    auto pReqUser = TXSingleton<XRelayServer>::Instance()->GetUser(stSkill.dwUCID);
    if (!pReqUser) {
        return;
    }

    auto it = m_mpLeagueList.find(stSkill.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "ResLeagueSkillLearn error - Not exist league[ LeagueID:%d ] ( %d )",
                           stSkill.nLeagueID, 2933);
        pReqUser->SetLockLeague(0);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay",
                           "ResLeagueSkillLearn error - Not exist league[ LeagueID:%d ] ( %d )",
                           stSkill.nLeagueID, 2943);
        pReqUser->SetLockLeague(0);
        return;
    }

    pReqUser->SetLockLeague(0);
    pLeague->UpdateSyncCount();

    // 学习技能
    pLeague->LearnSkill(stSkill);

    // 记录日志
    ST_LEAGUE_RECORD stRecord{};
    stRecord.byFlag = byType ? 10 : 9;  // 对齐 IDA: byType == 1 => 10, else => 9
    stRecord.biRegisterDate = TXSingleton<XRelayServer>::Instance()->GetCurDateSec();
    stRecord.nLeagueID = stSkill.nLeagueID;
    stRecord.nValue3 = stSkill.bySkillIndex;
    stRecord.nValue4 = stSkill.bySkillLevel;
    pLeague->UpdateRecord(stRecord);
}

void CLeagueManager::ReqLeagueLevelup(std::int32_t nLeagueID, std::uint8_t byType, std::uint32_t dwActorID) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueLevelup leagueID=%d type=%u actorID=%u", nLeagueID, byType, dwActorID);

    // 查找联赛
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "ReqLeagueLevelup error - Not exist league[ LeagueID:%d ] ( %d )",
                           nLeagueID, 2982);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay",
                           "ReqLeagueLevelup error - Not exist league[ LeagueID:%d ] ( %d )",
                           nLeagueID, 2989);
        return;
    }

    // 执行升级（GM作弊）
    pLeague->Levelup_Cheat(byType, dwActorID);
}

void CLeagueManager::ResLeagueLevelup(std::int32_t nLeagueID, std::uint8_t byLevel, std::uint8_t bySkillPoint, const PS_AUTO_SKILL& stSkill, std::uint32_t dwUCID) {
    // 对齐 IDA 0x14007f7d0
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "ResLeagueLevelup error - Not exist league[ LeagueID:%d ] ( %d )",
                           nLeagueID, 2849);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay",
                           "ResLeagueLevelup error - Not exist league[ LeagueID:%d ] ( %d )",
                           nLeagueID, 2856);
        return;
    }

    // 更新同步计数并应用升级
    pLeague->UpdateSyncCount();
    pLeague->ApplyLevelup(byLevel, bySkillPoint, stSkill, dwUCID);

    // 记录日志
    ST_LEAGUE_RECORD stRecord{};
    stRecord.byFlag = 8;  // 对齐 IDA
    stRecord.biRegisterDate = TXSingleton<XRelayServer>::Instance()->GetCurDateSec();
    stRecord.nLeagueID = nLeagueID;
    stRecord.nValue3 = static_cast<std::int32_t>(byLevel);
    pLeague->UpdateRecord(stRecord);
}

void CLeagueManager::ReqLeagueSkillPointUpdate(std::int32_t nLeagueID, std::uint8_t byType, std::uint32_t dwSomething) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueSkillPointUpdate leagueID=%d type=%u something=%u", nLeagueID, byType, dwSomething);

    // 查找联赛
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "ReqLeagueSkillPointUpdate error - Not exist league[ LeagueID:%d ] ( %d )",
                           nLeagueID, 3096);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay",
                           "ReqLeagueSkillPointUpdate error - Not exist league[ LeagueID:%d ] ( %d )",
                           nLeagueID, 3103);
        return;
    }

    // 更新技能点（GM作弊）
    pLeague->UpdateSkillPoint_Cheat(byType, dwSomething);
}

// ============================================================================
// 财富
// ============================================================================

void CLeagueManager::ReqApplyLeagueExp(const PS_LEAGUE_WEALTH_FOR_SERVER& stWealth) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqApplyLeagueExp leagueID=%d", stWealth.nLeagueID);

    // 查找联赛
    auto it = m_mpLeagueList.find(stWealth.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "ReqApplyLeagueExp error - Not exist league[ LeagueID:%d ] ( %d )",
                           stWealth.nLeagueID, 2811);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay",
                           "ReqApplyLeagueExp error - Not exist league[ LeagueID:%d ] ( %d )",
                           stWealth.nLeagueID, 2818);
        return;
    }

    // 计算经验
    PS_LEAGUE_WEALTH_FOR_SERVER stWealthCopy = stWealth;
    pLeague->CalculateExp(stWealthCopy);
}

// 对齐 IDA 0x14007f690: 财富 DB 响应处理
void CLeagueManager::ResApplyLeagueWealth(const PS_LEAGUE_WEALTH_FOR_SERVER& stWealth) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ResApplyLeagueWealth leagueID=%d", stWealth.nLeagueID);

    // 查找联赛
    auto it = m_mpLeagueList.find(stWealth.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "ReqApplyLeagueExp error - Not exist league[ LeagueID:%d ] ( %d )",
                           stWealth.nLeagueID, 2830);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay",
                           "ReqApplyLeagueExp error - Not exist league[ LeagueID:%d ] ( %d )",
                           stWealth.nLeagueID, 2837);
        return;
    }

    // 对齐 IDA: 复制财富结构并调用 ApplyWealth
    PS_LEAGUE_WEALTH_FOR_SERVER stWealthCopy = stWealth;
    pLeague->ApplyWealth(stWealthCopy);
}

// ============================================================================
// 记录
// ============================================================================

void CLeagueManager::ReqLeagueRecordUpdate(const ST_LEAGUE_RECORD& stRecord) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueRecordUpdate leagueID=%d flag=%d", stRecord.nLeagueID, stRecord.byFlag);

    // 查找联赛
    auto it = m_mpLeagueList.find(stRecord.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "ReqLeagueRecordUpdate error - Not exist league[ LeagueID:%d ] ( %d )",
                           stRecord.nLeagueID, 3031);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay",
                           "ReqLeagueRecordUpdate error - Not exist league[ LeagueID:%d ] ( %d )",
                           stRecord.nLeagueID, 3038);
        return;
    }

    // 更新记录
    pLeague->UpdateRecord(const_cast<ST_LEAGUE_RECORD&>(stRecord));
}

// ============================================================================
// 成员经验
// ============================================================================

void CLeagueManager::ReqLeagueMemberExpInit(std::uint32_t dwActorID, std::uint32_t dwSomething) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueMemberExpInit actorID=%u", dwActorID);

    // 注意：IDA 签名是 ReqLeagueMemberExpInit(nLeagueID, dwUCID)
    // 但当前声明签名是 (dwActorID, dwSomething)
    // 从 IDA 反编译看，实际参数是 (nLeagueID, dwUCID)，这里按 IDA 语义调用
    std::int32_t nLeagueID = static_cast<std::int32_t>(dwActorID);
    std::uint32_t dwUCID = dwSomething;

    // 查找联赛
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "ResLeagueMemberExpInit error - Not exist league[ LeagueID:%d ] ( %d )",
                           nLeagueID, 3077);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay",
                           "ResLeagueMemberExpInit error - Not exist league[ LeagueID:%d ] ( %d )",
                           nLeagueID, 3084);
        return;
    }

    // 重置经验初始化日期
    pLeague->ResetExpInitDate(dwUCID);
}

void CLeagueManager::ReqLeagueMemberInitExp(std::int32_t nLeagueID, std::uint32_t dwUCID) {
    // 对齐 IDA ReqLeagueMemberExpInit 0x140080430: 查找联赛 -> ResetExpInitDate
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueMemberInitExp nLeagueID=%d dwUCID=%u", nLeagueID, dwUCID);

    // 查找联赛
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "ResLeagueMemberExpInit error - Not exist league[ LeagueID:%d ] ( %d )",
                           nLeagueID, 3077);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay",
                           "ResLeagueMemberExpInit error - Not exist league[ LeagueID:%d ] ( %d )",
                           nLeagueID, 3084);
        return;
    }

    // 重置经验初始化日期
    pLeague->ResetExpInitDate(dwUCID);
}

// ============================================================================
// 同步
// ============================================================================

bool CLeagueManager::SyncLeagueInfo(const PS_SYNC_LEAGUE_INFO& stSync) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::SyncLeagueInfo leagueID=%d ucid=%u", stSync.nLeagueID, stSync.dwUCID);

    // 查找联赛
    auto it = m_mpLeagueList.find(stSync.nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "SyncLeagueInfo error - Not exist league[ LeagueID:%d ] ( %d )",
                           stSync.nLeagueID, 3115);
        return false;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay",
                           "SyncLeagueInfo error - Not exist league Member[ LeagueID:%d, UCID:%d ] ( %d )",
                           stSync.nLeagueID, stSync.dwUCID, 3122);
        return false;
    }

    // 发送同步联赛信息
    PS_SYNC_LEAGUE_INFO stSyncCopy = stSync;
    pLeague->SendSyncLeagueInfo(stSyncCopy);
    return true;
}

// ============================================================================
// 发送联赛信息
// ============================================================================

void CLeagueManager::SendLeagueInfo(std::uint32_t dwActorID, ST_LEAGUE_INFO& stLeagueInfo, ST_LEAGUE_MEMBER_LIST& stMemberList,
                                     ST_LEAGUE_APPLICANT_LIST& stApplicantList, ST_LEAGUE_BOARD_LIST& stBoardList,
                                     std::uint8_t byState, ST_LEAGUE_RECORD_LIST& stRecordList, ST_LEAGUE_INFO_FOR_GAME& stInfoForGame) {
    // 发送联赛信息包（主命令0xF6，子命令6）
    XSendPacket xSendPacket(0xF6, 6);
    xSendPacket << stLeagueInfo;
    xSendPacket << stMemberList;
    xSendPacket << stApplicantList;
    xSendPacket << stBoardList;
    xSendPacket.XParse << dwActorID;
    xSendPacket.XParse << byState;
    xSendPacket << stRecordList.vecInfo;
    xSendPacket << stInfoForGame;

    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

// ============================================================================
// 仓库
// ============================================================================

void CLeagueManager::ReqLeagueInevntoryInfo(std::uint32_t dwReqUCID, const PS_REQ_LEAGUE_INVEN_INFO& stReq) {
    // 对齐 IDA 0x140080790: 先查联赛，再发送 DB 包（先结构体，再 dwReqUCID）
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueInevntoryInfo leagueID=%d ucid=%u", stReq.nLeagueID, dwReqUCID);

    // 查找联赛
    auto it = m_mpLeagueList.find(stReq.nLeagueID);
    auto itEnd = m_mpLeagueList.end();
    if (it == itEnd) {
        LogHelper::LogError("game.relay",
                           "ReqLeagueInevntoryInfo error - Not exist league[ LeagueID:%d ] ( %d )",
                           stReq.nLeagueID, 3134);
        return;
    }

    // 对齐 IDA: 发送到 DB 查询仓库信息（main=7, sub=0x39）
    // 序列化顺序：先 operator<<(stReq)，再 XParse << dwReqUCID
    XSendDBPacket xSendDBPacket(nullptr, 7, 0x39);
    xSendDBPacket << stReq;  // 先序列化整个结构体
    xSendDBPacket.XParse << dwReqUCID;  // 再写 dwReqUCID
    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
}

void CLeagueManager::ReqLeagueInventoryMove(std::uint32_t dwReqUCID, const struct PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME& stMove) {
    // 对齐 IDA 0x140080ba0
    std::int32_t nLeagueID = stMove.psReqItemMoveInfo.nLeagueID;

    // 查找联赛
    auto it = m_mpLeagueList.find(nLeagueID);
    auto itEnd = m_mpLeagueList.end();
    if (it == itEnd) {
        LogHelper::LogError("game.relay",
                           "ReqLeagueInventoryMove error - Not exist league[ LeagueID:%d, UCID:%d ] ( %d )",
                           nLeagueID, dwReqUCID, 3168);
        return;
    }

    // 发送到DB处理物品移动（main=7, sub=0x37）
    // 对齐 IDA: XSendDBPacket(0, 7, 0x37) + dwReqUCID + operator<<(stMove)
    XSendDBPacket xSendDBPacket(nullptr, 7, 0x37);
    xSendDBPacket.XParse << dwReqUCID;
    xSendDBPacket << stMove;
    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
}

// 对齐 IDA 0x1400808b0: 仓库信息 DB 响应处理
void CLeagueManager::ResLeagueInventoryInfo(std::int32_t nLeagueID, std::uint32_t dwReqUCID, PS_RES_STORAGE_INFO stStorage, PS_ITEM_BROACH_LIST stBroach, PS_ITEM_SOCKET_LIST stSocket, PS_ITEM_PACKAGE_LIST stPackage) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ResLeagueInventoryInfo leagueID=%d reqUCID=%u", nLeagueID, dwReqUCID);

    // 查找联赛
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "ResLeagueInventoryInfo error - Not exist league[ LeagueID:%d, UCID:%d ] ( %d )",
                           nLeagueID, dwReqUCID, 3149);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay",
                           "ResLeagueInventoryInfo error - Not exist league Member[ LeagueID:%d, UCID:%d ] ( %d )",
                           nLeagueID, dwReqUCID, 3156);
        return;
    }

    // 对齐 IDA: 复制参数并调用 SendInventoryInfo
    // 注意: IDA 中创建了临时副本，然后调用 SendInventoryInfo
    pLeague->SendInventoryInfo(dwReqUCID, stStorage, stBroach, stSocket, stPackage);
}

// 对齐 IDA 0x140080cf0: 仓库物品移动 DB 响应处理
void CLeagueManager::ResLeagueInventoryMove(std::uint32_t dwReqUCID, PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME stMove) {
    // 从请求信息提取 nLeagueID
    std::int32_t nLeagueID = stMove.psReqItemMoveInfo.nLeagueID;

    // 查找联赛
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "ResLeagueInventoryMove error - Not exist league[ LeagueID:%d, UCID:%d ] ( %d )",
                           nLeagueID, dwReqUCID, 3183);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay",
                           "ResLeagueInventoryMove error - Not exist league Member[ LeagueID:%d, UCID:%d ] ( %d )",
                           nLeagueID, dwReqUCID, 3190);
        return;
    }

    // 对齐 IDA: 无错误时处理
    if (stMove.nErrorCode == 0) {
        // 更新仓库同步计数
        pLeague->UpdateInventorySyncCount();

        // 复制请求参数
        PS_REQ_ITEM_MOVE_LEAGUE_INVEN psItemMove = stMove.psReqItemMoveInfo;

        // 对齐 IDA: 根据操作类型创建记录
        if (psItemMove.byType == 0) {
            // 存入仓库 (byFlag=6)
            ST_LEAGUE_RECORD stRecord{};
            stRecord.byFlag = 6;
            stRecord.biRegisterDate = TXSingleton<XRelayServer>::Instance()->GetCurDateSec();
            stRecord.nLeagueID = psItemMove.nLeagueID;
            stRecord.nValue3 = psItemMove.nSrcItemID;
            stRecord.nValue4 = stMove.psStorageInfo.stItem.sCount;

            // 获取操作者名称
            auto pReqUser = TXSingleton<XRelayServer>::Instance()->GetUser(dwReqUCID);
            if (pReqUser) {
                std::wstring wsUserName = pReqUser->GetName();
                wcscpy_s(stRecord.szValue1, wsUserName.c_str());
            }

            pLeague->UpdateRecord(stRecord);
        } else if (psItemMove.byType == 1) {
            // 取出仓库 (byFlag=7)
            ST_LEAGUE_RECORD stRecord{};
            stRecord.byFlag = 7;
            stRecord.biRegisterDate = TXSingleton<XRelayServer>::Instance()->GetCurDateSec();
            stRecord.nLeagueID = psItemMove.nLeagueID;
            stRecord.nValue3 = psItemMove.nSrcItemID;
            stRecord.nValue4 = stMove.psOutItemInfo.stItem.sCount;

            // 获取操作者名称
            auto pReqUser = TXSingleton<XRelayServer>::Instance()->GetUser(dwReqUCID);
            if (pReqUser) {
                std::wstring wsUserName = pReqUser->GetName();
                wcscpy_s(stRecord.szValue1, wsUserName.c_str());
            }

            pLeague->UpdateRecord(stRecord);
        }
    }

    // 对齐 IDA: 填充响应参数
    stMove.psResItemMoveInfo.nLeagueID = stMove.psReqItemMoveInfo.nLeagueID;
    stMove.psResItemMoveInfo.byType = stMove.psReqItemMoveInfo.byType;
    stMove.psResItemMoveInfo.nSrcItemID = stMove.psReqItemMoveInfo.nSrcItemID;
    stMove.psResItemMoveInfo.shSrcSlotPos = stMove.psReqItemMoveInfo.shSrcSlotPos;
    stMove.psResItemMoveInfo.nDestItemID = stMove.psReqItemMoveInfo.nDestItemID;
    stMove.psResItemMoveInfo.shDestSlotPos = stMove.psReqItemMoveInfo.shDestSlotPos;

    // 调用 SendInventoryMove
    pLeague->SendInventoryMove(dwReqUCID, stMove);
}

// ============================================================================
// 聊天
// ============================================================================

void CLeagueManager::SendLeagueMessage(const PS_CHAT_LEAGUE& stChat, struct PS_CHAT_ITEM_LINK_FOR_SERVER& stItemLink) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::SendLeagueMessage leagueID=%d", stChat.dwLeagueID);

    // 查找联赛
    auto it = m_mpLeagueList.find(stChat.dwLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.contents",
                           "[LEAGUE] Error SendLeagueMessage - iter == m_mpLeagueList.end() %d",
                           stChat.dwLeagueID);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "SendLeagueMessage League NULL(%d)", 1707);
        return;
    }

    // 广播联赛聊天消息（主命令0xF6，子命令0x36）
    XSendPacket xSendPacket(0xF6, 0x36);
    xSendPacket << stChat;
    xSendPacket << stItemLink;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

// ============================================================================
// 搜索
// ============================================================================

void CLeagueManager::ReqLeagueSearch(CServer* pServer, std::uint32_t dwActorID, std::int32_t nLeagueID) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueSearch actorID=%u", dwActorID);

    // 发送到DB处理联赛搜索（main=7, sub=0x29）
    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 7, 0x29);
    xSendDBPacket.XParse << dwActorID;
    xSendDBPacket.XParse << pServer->GetServerID();

    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
}

// ============================================================================
// 列表
// ============================================================================

void CLeagueManager::ReqLeagueList(CServer* pServer, std::uint32_t dwActorID, std::int32_t nLeagueID) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ReqLeagueList actorID=%u", dwActorID);

    // 发送到DB处理联赛列表（main=7, sub=0x41）
    IXObject* pObject = pServer ? static_cast<IXObject*>(pServer) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 7, 0x41);
    xSendDBPacket.XParse << dwActorID;
    xSendDBPacket.XParse << pServer->GetServerID();

    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
}

void CLeagueManager::ResLeagueSearch(CServer* pServer, PS_LEAGUE_SUMMARY_LIST psLeagueSummaryList, std::uint32_t dwUCID) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ResLeagueSearch ucid=%u", dwUCID);

    // 获取申请者检查列表
    ST_LEAGUE_APPLICANT_CHECK_LIST stApplyList{};
    GetApplicantList(dwUCID, psLeagueSummaryList, stApplyList);

    // 发送联赛搜索结果（主命令0xF6，子命令0x18）
    XSendPacket xSendPacket(0xF6, 0x18);
    xSendPacket.XParse << dwUCID;
    xSendPacket << psLeagueSummaryList;
    xSendPacket << stApplyList;

    if (pServer) {
        pServer->SendEx(xSendPacket);
    }
}

void CLeagueManager::ResLeagueList(CServer* pServer, PS_LEAGUE_SUMMARY_LIST psLeagueList, std::uint32_t dwUCID) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ResLeagueList ucid=%u", dwUCID);

    // 获取申请者检查列表
    ST_LEAGUE_APPLICANT_CHECK_LIST stApplyList{};
    GetApplicantList(dwUCID, psLeagueList, stApplyList);

    // 发送联赛列表结果（主命令0xF6，子命令0x24）
    XSendPacket xSendPacket(0xF6, 0x24);
    xSendPacket.XParse << dwUCID;
    xSendPacket << psLeagueList;
    xSendPacket << stApplyList;

    if (pServer) {
        pServer->SendEx(xSendPacket);
    }
}

// ============================================================================
// 更新回调
// ============================================================================

void CLeagueManager::OnUpdate() {
    // 每秒执行一次的周期更新 - 对齐 IDA 0x14007b740
    std::int64_t tNow = TXSingleton<XRelayServer>::Instance()->GetCurDateSec();

    // 检查是否超过1秒
    if (m_tUpdate + 1 > tNow) {
        return;
    }

    m_tUpdate = tNow;

    // 计算今天9:00的时间戳
    // 使用 localtime 分解当前时间
    std::time_t nowTime = static_cast<std::time_t>(tNow);
    struct tm localTm{};
#ifdef _WIN32
    localtime_s(&localTm, &nowTime);
#else
    localtime_r(&nowTime, &localTm);
#endif

    // 构造今天 9:00 的 tm
    struct tm todayInitTm = localTm;
    todayInitTm.tm_hour = 9;
    todayInitTm.tm_min = 0;
    todayInitTm.tm_sec = 0;
    std::time_t tTodayInit = std::mktime(&todayInitTm);

    // 如果当前小时 < 9，则 todayInit 减一天
    if (localTm.tm_hour < 9) {
        tTodayInit -= 86400;  // 减去一天的秒数
    }

    // 如果 m_tInitDate < todayInit，执行每日初始化
    if (m_tInitDate < tTodayInit) {
        InitLeaguExp(tTodayInit);
        m_tInitDate = tTodayInit;
    }

    // 遍历所有联赛，更新申请列表
    for (auto it = m_mpLeagueList.begin(); it != m_mpLeagueList.end(); ++it) {
        auto& pLeague = it->second;
        if (pLeague) {
            pLeague->UpdateApplyList(tNow);
        }
    }

    // TODO: 推测结果 - GMT League 信息加载依赖 XResourceMgr，当前 RelayServer 不加载该表
    // 原版调用 XResourceMgr::LoadGMTLeagueInfo + SendGMTLeagueInfo
    // 当前暂时跳过 GMT 更新广播
}

void CLeagueManager::InitLeaguExp(std::int64_t biTodayInit) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::InitLeaguExp todayInit=%lld", biTodayInit);

    // 遍历所有联赛，重置每日经验 - 对齐 IDA 0x14007bb00
    for (auto it = m_mpLeagueList.begin(); it != m_mpLeagueList.end(); ++it) {
        auto& pLeague = it->second;
        if (pLeague) {
            pLeague->ResetExp(biTodayInit);
        }
    }

    // 发送 DB 包触发每日 league exp 重置 (main=7, sub=0x40)
    XSendDBPacket xSendDBPacket(nullptr, 7, 0x40);
    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
}

void CLeagueManager::UpdateLeagueInfo(std::int32_t nLeagueID, ST_LEAGUE_INFO_UPDATE& stUpdate) {
    // 对齐 IDA 0x14007bc00: 更新联赛信息到 ST_LEAGUE_INFO_UPDATE 结构
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::UpdateLeagueInfo leagueID=%d", nLeagueID);

    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "UpdateLeagueInfo pLeague NULL(%d)", 1989);
        return;
    }

    // 获取联赛信息
    ST_LEAGUE_INFO stInfo{};
    pLeague->GetLeagueInfo(stInfo);

    // 填充更新结构
    stUpdate.nLeagueID = nLeagueID;
    stUpdate.nLeagueRank = stInfo.nLeagueRank;
    stUpdate.shLeagueMemeberCnt = static_cast<std::int16_t>(pLeague->GetMemberCount());
    stUpdate.biLeagueMoney = stInfo.biMoney;
    stUpdate.biExp = stInfo.biExp;
    stUpdate.dwLeagueCard = stInfo.dwLeagueCard;

    // 对齐 IDA: 再次获取成员数量并设置
    pLeague->SetMemberCount(static_cast<std::int16_t>(pLeague->GetMemberCount()));
}

// ============================================================================
// 成员管理
// ============================================================================

void CLeagueManager::DeleteLeagueMember(std::int32_t nLeagueID, std::uint32_t dwActorID, wchar_t* pName) {
    // 对齐 IDA 0x1400785d0: 删除联赛成员
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "DeleteLeagueMember pLeague NULL(%d)", 1256);
        return;
    }

    // 获取被删除成员的名字
    pLeague->GetDelName(dwActorID, pName);
    // 删除成员
    pLeague->DelMember(dwActorID);
}

void CLeagueManager::ResLeagueWithdraw(std::int32_t nLeagueID, std::uint32_t dwUCID, std::int64_t biPenalty) {
    // 对齐 IDA 0x140074350: 退会成功响应处理
    ST_LEAGUE_INFO_UPDATE stInfoUpdate{};
    wchar_t szLeftUserName[24] = {};

    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "ResLeagueWithdraw error - Not exist league[ LeagueID:%d, UCID:%d ] ( %d )",
                           nLeagueID, dwUCID, 251);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay",
                           "ResLeagueWithdraw error - Not exist league Member[ LeagueID:%d, UCID:%d ] ( %d )",
                           nLeagueID, dwUCID, 258);
        return;
    }

    // 获取成员信息
    ST_LEAGUE_MEMBER_EX stMemberInfo{};
    GetMemberInfo(nLeagueID, dwUCID, stMemberInfo);

    // 如果是副盟主，清空副盟主名称
    if (stMemberInfo.stMember.byPosition == 7) {
        wchar_t szTemp[24] = {};
        pLeague->SetSubLeagueMaster(szTemp);
    }

    // 删除联赛成员
    DeleteLeagueMember(nLeagueID, dwUCID, szLeftUserName);

    // 更新联赛信息
    UpdateLeagueInfo(nLeagueID, stInfoUpdate);

    // 获取用户并清空联赛 ID，设置退会惩罚
    auto pUser = TXSingleton<XRelayServer>::Instance()->GetUser(dwUCID);
    if (pUser) {
        pUser->SetLeagueID(0);
        pUser->SetLeagueWithdrawPenalty(biPenalty);
    }

    // 通知其他成员
    SendLeagueMemberWithdraw(nLeagueID, dwUCID, biPenalty, stInfoUpdate, szLeftUserName);
}

void CLeagueManager::SendLeagueMemberWithdraw(std::int32_t nLeagueID, std::uint32_t dwUCID,
                                               std::int64_t biPenalty, ST_LEAGUE_INFO_UPDATE& stUpdate, wchar_t* pName) {
    // 对齐 IDA 0x14007d400: 广播成员退会通知
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.relay",
                           "SendLeagueMemberWithdraw error - Not Exist League[ LeagueID:%d ]( %d )",
                           nLeagueID, 2328);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogError("game.relay",
                           "SendLeagueMemberWithdraw error - Not Exist League[ LeagueID:%d ]( %d )",
                           nLeagueID, 2335);
        return;
    }

    // 更新同步计数
    pLeague->UpdateSyncCount();

    // 发送退会通知给成员
    pLeague->SendWithdrawToMember(nLeagueID, dwUCID, biPenalty, stUpdate);

    // 记录退会日志
    ST_LEAGUE_RECORD stRecord{};
    stRecord.nLeagueID = nLeagueID;
    stRecord.byFlag = 2;  // 退会标志
    stRecord.biRegisterDate = static_cast<std::int64_t>(std::time(nullptr));
    wcscpy_s(stRecord.szValue1, pName);
    pLeague->UpdateRecord(stRecord);
}

void CLeagueManager::UpdateMemberLevel(std::shared_ptr<CUserObject> pUser, std::uint8_t byLevel) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::UpdateMemberLevel level=%u", byLevel);

    if (!pUser) {
        return;
    }

    // 获取用户的联赛ID
    std::int32_t nLeagueID = pUser->GetLeagueID();
    if (nLeagueID <= 0) {
        return;
    }

    // 找到联赛
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        LogHelper::LogError("game.contents",
                           "[LEAGUE] Failed UpdateMemberLevel - iter == m_mpLeagueList.end() %d",
                           nLeagueID);
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "UpdateMemberLevel pLeague NULL(%d)", 2041);
        return;
    }

    // 获取成员指针
    // 使用dwActorID作为联赛中的成员标识
    std::uint32_t dwUCID = pUser->GetCID();
    auto pMember = pLeague->GetLeagueMemberPtr(dwUCID);
    if (!pMember) {
        LogHelper::LogError("game.contents", "UpdateMemberLevel pMember NULL(%d)", 2048);
        return;
    }

    // 更新等级
    pMember->SetLevel(byLevel);

    // 获取成员信息并发送更新
    ST_LEAGUE_MEMBER_EX stMemberInfo{};
    pMember->GetLeagueMember(stMemberInfo);
    SendMemberUpdate(stMemberInfo);
}

void CLeagueManager::UpdateMemberAwaken(std::shared_ptr<CUserObject> pUser, std::uint8_t byAwaken) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::UpdateMemberAwaken awaken=%u", byAwaken);

    if (!pUser) {
        return;
    }

    std::int32_t nLeagueID = pUser->GetLeagueID();
    if (nLeagueID <= 0) {
        return;
    }

    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        return;
    }

    std::uint32_t dwUCID = pUser->GetCID();
    auto pMember = pLeague->GetLeagueMemberPtr(dwUCID);
    if (!pMember) {
        return;
    }

    pMember->SetAwaken(byAwaken);

    ST_LEAGUE_MEMBER_EX stMemberInfo{};
    pMember->GetLeagueMember(stMemberInfo);
    SendMemberUpdate(stMemberInfo);
}

void CLeagueManager::UpdateMemberProfilePhoto(std::shared_ptr<CUserObject> pUser, std::uint32_t dwProfilePhotoID) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::UpdateMemberProfilePhoto photoID=%u", dwProfilePhotoID);

    if (!pUser) {
        return;
    }

    std::int32_t nLeagueID = pUser->GetLeagueID();
    if (nLeagueID <= 0) {
        return;
    }

    auto it = m_mpLeagueList.find(nLeagueID);
    if (it == m_mpLeagueList.end()) {
        return;
    }

    auto& pLeague = it->second;
    if (!pLeague) {
        return;
    }

    std::uint32_t dwUCID = pUser->GetCID();
    auto pMember = pLeague->GetLeagueMemberPtr(dwUCID);
    if (!pMember) {
        return;
    }

    pMember->SetProfilePhoto(dwProfilePhotoID);

    ST_LEAGUE_MEMBER_EX stMemberInfo{};
    pMember->GetLeagueMember(stMemberInfo);
    SendMemberUpdate(stMemberInfo);
}

void CLeagueManager::UpdateMemberMapInfo(std::uint32_t dwUCID, std::uint16_t wMapID, std::uint8_t byChannel, bool bLogin) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::UpdateMemberMapInfo ucid=%u mapID=%u channel=%u login=%d",
                       dwUCID, wMapID, byChannel, bLogin);

    // 遍历所有联赛查找该成员
    for (auto& pair : m_mpLeagueList) {
        auto& pLeague = pair.second;
        if (!pLeague) {
            continue;
        }

        auto pMember = pLeague->GetLeagueMemberPtr(dwUCID);
        if (pMember) {
            pMember->SetMapInfo(wMapID, byChannel);
        }
    }
}

void CLeagueManager::SendMemberUpdate(ST_LEAGUE_MEMBER_EX stMember) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::SendMemberUpdate ucid=%u", stMember.dwUCID);

    // 构建成员更新结构
    ST_LEAGUE_MEMBER_UPDATE stUpdate{};
    stUpdate.nLeagueID = stMember.stMember.nLeagueID;
    stUpdate.dwActorID = stMember.dwUCID;
    stUpdate.bLogin = stMember.bLogin;
    stUpdate.byLevel = static_cast<std::uint8_t>(stMember.shLevel);
    stUpdate.sWorld = stMember.sWorldID;
    stUpdate.biPlayDate = stMember.biPlayDate;
    stUpdate.byChannel = stMember.byChannel;
    stUpdate.byAwaken = stMember.byAwaken;
    stUpdate.dwProfilePhotoID = stMember.dwProfilePhotoID;

    // 复制名称
    wcscpy_s(stUpdate.szName, 21, stMember.szName);

    // 广播成员更新包（主命令0xF6，子命令0x42）
    XSendPacket xSendPacket(0xF6, 0x42);
    xSendPacket << stUpdate;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

void CLeagueManager::SendInfoToGameServer() {
    // 对齐 IDA 0x14007d200
    // 当所有联赛数据加载完成时，通知 RelayServer

    if (m_bLeague && m_bMember && m_bApplicant && m_bBoard && m_bRecord) {
        m_bLoadLeague = true;
        // E_SERVER_CACHING_LOAD_LEAGUE = 2u (defined in RelayServer.cpp)
        TXSingleton<XRelayServer>::Instance()->SetCachingLoad(2u);
    }
}

void CLeagueManager::UpdateLeagueMemberInfo() {
    // 对齐 IDA 0x14007d270
    // 遍历所有联赛，更新成员信息

    for (auto it = m_mpLeagueList.begin(); it != m_mpLeagueList.end(); ++it) {
        auto& pLeague = it->second;
        if (pLeague) {
            pLeague->UpDateLeagueMemberInfo();
            pLeague->UpdateSyncCount();
        }
    }

    // 发送同步包到所有客户端 (main=0xF6, sub=0x59)
    std::int32_t nSyncCount = 1;
    XSendPacket xSendPacket(0xF6, 0x59);
    xSendPacket.XParse << nSyncCount;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

// ============================================================================
// 加载回调
// ============================================================================

// 对齐 IDA 0x140081e90: 从 DB 加载联赛信息（大函数 0x6f1 bytes）
void CLeagueManager::LoadLeagueInfo(const struct PS_DB_LEAGUE_LOAD& stLoad, ST_LEAGUE_INFO stInfo, struct ST_LEAGUE_MEMBER_LIST stMembers, struct ST_LEAGUE_BOARD_LIST stBoards, struct ST_LEAGUE_APPLICANT_LIST stApplicants, struct ST_LEAGUE_RECORD_LIST stRecords) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::LoadLeagueInfo leagueID=%d ucid=%u", stInfo.nLeagueID, stLoad.dwUCID);

    // 对齐 IDA: 如果提供了UCID，先获取用户信息
    if (stLoad.dwUCID != 0) {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        auto pUser = relayServer.GetUser(stLoad.dwUCID);
        if (!pUser) {
            // 用户不在线，无法加载联赛
            return;
        }

        // 检查联赛是否已在map中
        auto it = m_mpLeagueList.find(stInfo.nLeagueID);
        if (it != m_mpLeagueList.end()) {
            // 对齐 IDA: 联赛已加载 - 只需登录并发送信息
            auto& pLeague = it->second;
            if (pLeague) {
                pLeague->LoginMember(stLoad.dwUCID);
                pLeague->SendLeagueInfo(stLoad.dwUCID);
            }
            return;
        }
    }

    // 联赛未找到 - 创建新的
    auto pLeague = std::make_shared<CLeague>();
    if (!pLeague) {
        LogHelper::LogDebug("game.league", "LoadLeagueInfo NULL - failed to create league");
        return;
    }

    // 设置联赛信息（对齐 IDA: qmemcpy副本）
    pLeague->SetLeagueInfo(stInfo);

    // 从DB加载添加成员（对齐 IDA: qmemcpy副本 + AddMember）
    for (const auto& member : stMembers.vecInfo) {
        pLeague->AddMember(member);
    }

    // 从DB加载添加公告板
    for (const auto& board : stBoards.vecInfo) {
        pLeague->AddBoard(board);
    }

    // 从DB加载添加申请者
    for (const auto& applicant : stApplicants.vecInfo) {
        pLeague->AddApplicant(applicant);
    }

    // 从DB加载记录
    for (auto& record : stRecords.vecInfo) {
        pLeague->LoadRecord(record);
    }

    // 对齐 IDA: 插入联赛map（不操作 m_vecLeagueList）
    m_mpLeagueList.insert({stInfo.nLeagueID, pLeague});

    // 对齐 IDA: 如果提供了UCID，登录成员并发送信息
    if (stLoad.dwUCID != 0) {
        pLeague->LoginMember(stLoad.dwUCID);
        pLeague->SendLeagueInfo(stLoad.dwUCID);
    } else {
        // 对齐 IDA: dwUCID==0 表示服务器启动加载，调用 ReqLeagueApplicant
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        CServer* pServer = relayServer.GetServer(stLoad.dwServerID);
        if (!pServer) {
            LogHelper::LogError("game.relay", "LoadLeagueInfoTest error - Server == NULL");
            return;
        }
        ReqLeagueApplicant(const_cast<ST_LEAGUE_APPLICANT&>(stLoad.stApplicant), pServer);
    }
}

void CLeagueManager::ResLoadLeagueMember(bool bSuccess, struct ST_LEAGUE_MEMBER_LIST& stMembers) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ResLoadLeagueMember success=%d", bSuccess);

    // 对齐 IDA 0x14007ca50
    m_bMember = bSuccess;

    // 遍历所有联赛，加载成员
    for (auto it = m_mpLeagueList.begin(); it != m_mpLeagueList.end(); ++it) {
        auto& pLeague = it->second;
        if (!pLeague) {
            continue;
        }

        ST_LEAGUE_INFO stLeagueInfo{};
        pLeague->GetLeagueInfo(stLeagueInfo);

        for (std::size_t i = 0; i < stMembers.vecInfo.size(); ++i) {
            const auto& stMember = stMembers.vecInfo[i];

            // 检查成员所属联赛
            if (stMember.stMember.nLeagueID == stLeagueInfo.nLeagueID) {
                pLeague->AddMember(stMember);
            }

            // 如果是副会长(position=7)，设置副会长名称
            if (stMember.stMember.byPosition == 7 && stMember.stMember.nLeagueID == stLeagueInfo.nLeagueID) {
                pLeague->SetSubLeagueMaster(const_cast<wchar_t*>(stMember.szName));
            }
        }
    }
}

void CLeagueManager::ResLoadLeagueApplicant(bool bSuccess, struct ST_LEAGUE_APPLICANT_LIST& stApplicants) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ResLoadLeagueApplicant success=%d", bSuccess);

    // 对齐 IDA 0x14007ccb0
    m_bApplicant = bSuccess;

    // 遍历所有联赛，加载申请者
    for (auto it = m_mpLeagueList.begin(); it != m_mpLeagueList.end(); ++it) {
        auto& pLeague = it->second;
        if (!pLeague) {
            continue;
        }

        ST_LEAGUE_INFO stLeagueInfo{};
        pLeague->GetLeagueInfo(stLeagueInfo);

        for (std::size_t i = 0; i < stApplicants.vecInfo.size(); ++i) {
            const auto& stApplicant = stApplicants.vecInfo[i];

            // 检查申请者所属联赛
            if (stApplicant.nLeagueID == stLeagueInfo.nLeagueID) {
                pLeague->AddApplicant(stApplicant);
            }
        }
    }
}

void CLeagueManager::ResLoadLeagueBoard(bool bSuccess, struct ST_LEAGUE_BOARD_LIST& stBoards) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ResLoadLeagueBoard success=%d", bSuccess);

    // 对齐 IDA 0x14007ce90
    m_bBoard = bSuccess;

    // 遍历所有联赛，加载公告板
    for (auto it = m_mpLeagueList.begin(); it != m_mpLeagueList.end(); ++it) {
        auto& pLeague = it->second;
        if (!pLeague) {
            continue;
        }

        ST_LEAGUE_INFO stLeagueInfo{};
        pLeague->GetLeagueInfo(stLeagueInfo);

        for (std::size_t i = 0; i < stBoards.vecInfo.size(); ++i) {
            const auto& stBoard = stBoards.vecInfo[i];

            // 检查公告所属联赛
            if (stBoard.nLeagueID == stLeagueInfo.nLeagueID) {
                pLeague->AddBoard(stBoard);
            }
        }
    }
}

void CLeagueManager::ResLoadLeagueRecord(bool bSuccess, struct ST_LEAGUE_RECORD_LIST& stRecords) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ResLoadLeagueRecord success=%d", bSuccess);

    // 对齐 IDA 0x14007d080
    m_bRecord = bSuccess;

    // 遍历所有联赛，加载记录
    for (auto it = m_mpLeagueList.begin(); it != m_mpLeagueList.end(); ++it) {
        auto& pLeague = it->second;
        if (!pLeague) {
            continue;
        }

        ST_LEAGUE_INFO stLeagueInfo{};
        pLeague->GetLeagueInfo(stLeagueInfo);

        for (std::size_t i = 0; i < stRecords.vecInfo.size(); ++i) {
            const auto& stRecord = stRecords.vecInfo[i];

            // 检查记录所属联赛
            if (stRecord.nLeagueID == stLeagueInfo.nLeagueID) {
                pLeague->LoadRecord(const_cast<ST_LEAGUE_RECORD&>(stRecord));
            }
        }
    }

    // 加载完成后通知 GameServer
    SendInfoToGameServer();
}

// ============================================================================
// 名称变更回调
// ============================================================================

void CLeagueManager::ChangeLeagueMemberName(std::int32_t nLeagueID, struct PS_CHANGE_NAME& stChange) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ChangeLeagueMemberName leagueID=%d", nLeagueID);

    // 对齐 IDA 0x140081c70
    // 查找联赛并变更成员名称
    auto it = m_mpLeagueList.find(nLeagueID);
    if (it != m_mpLeagueList.end() && it->second) {
        it->second->ChangeMemberName(stChange);
    }
}

void CLeagueManager::ChangeLeagueApplicant(const struct PS_SERVER_CHANGE_CHARACTER_NAME& stChange) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::ChangeLeagueApplicant");

    // 对齐 IDA 0x140081d60
    // 遍历申请者所在的联赛列表，更新申请者名称
    for (const auto& nApplyLeagueID : stChange.stApplyList.vecInfo) {
        auto it = m_mpLeagueList.find(static_cast<std::int32_t>(nApplyLeagueID));
        if (it != m_mpLeagueList.end() && it->second) {
            it->second->UpdateApplicantName(stChange.psChangeInfo);
        }
    }
}

// ============================================================================
// GMT
// ============================================================================

void CLeagueManager::SendGMTLeagueInfo(const struct PS_GMT_LEAGUE_UPDATE_LIST& stList) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::SendGMTLeagueInfo");
}

// 对齐 IDA 0x140082590: GMT 更新联赛信息和成员职位
void CLeagueManager::UpdateGMTLeagueInfo(struct ST_LEAGUE_LIST stList, struct ST_LEAGUE_MEMBER_LIST stMembers) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::UpdateGMTLeagueInfo leagueCount=%d memberCount=%d",
                       static_cast<int>(stList.vecInfo.size()), static_cast<int>(stMembers.vecInfo.size()));

    // 对齐 IDA: 第一阶段 - 遍历联赛信息列表，更新每个联赛
    for (std::size_t i = 0; i < stList.vecInfo.size(); ++i) {
        const auto& stInfo = stList.vecInfo[i];

        auto it = m_mpLeagueList.find(stInfo.nLeagueID);
        if (it != m_mpLeagueList.end()) {
            auto& pLeague = it->second;
            if (pLeague) {
                ST_LEAGUE_INFO stInfoCopy = stInfo;
                pLeague->SetLeagueInfo(stInfoCopy);
            }
        }
    }

    // 对齐 IDA: 第二阶段 - 遍历成员列表，更新成员职位
    for (std::size_t j = 0; j < stMembers.vecInfo.size(); ++j) {
        const auto& stMemberInfo = stMembers.vecInfo[j];

        // 对齐 IDA: 成员的联赛ID在 stMember.nLeagueID（来自 ST_LEAGUE_MEMBER_EX.stMember.nLeagueID）
        auto it = m_mpLeagueList.find(stMemberInfo.stMember.nLeagueID);
        if (it == m_mpLeagueList.end()) {
            continue;
        }

        auto& pLeague = it->second;
        if (!pLeague) {
            continue;
        }

        // 获取成员指针
        auto pMember = pLeague->GetLeagueMemberPtr(stMemberInfo.dwUCID);
        if (!pMember) {
            continue;
        }

        // 获取旧职位
        std::uint8_t byPrevPosition = 0;
        pMember->GetPosition(byPrevPosition);

        // 设置新职位
        pMember->SetPosition(stMemberInfo.stMember.byPosition);

        // 对齐 IDA: 如果从副盟主(position==7)变为非副盟主，清空副盟主名称
        if (byPrevPosition == 7 && stMemberInfo.stMember.byPosition != 7) {
            wchar_t szTemp[24] = {};
            pLeague->SetSubLeagueMaster(szTemp);
        }

        // 对齐 IDA: 如果新职位是副盟主(position==7)，设置副盟主名称
        if (stMemberInfo.stMember.byPosition == 7) {
            wchar_t szMemberName[24] = {};
            pMember->GetName(szMemberName);
            pLeague->SetSubLeagueMaster(szMemberName);
        }
    }
}

// ============================================================================
// 检查
// ============================================================================

bool CLeagueManager::CheckLeagueInfo(std::int32_t nLeagueID) {
    return m_mpLeagueList.find(nLeagueID) != m_mpLeagueList.end();
}

// ============================================================================
// 获取器
// ============================================================================

void CLeagueManager::GetApplicantList(std::uint32_t dwUCID, PS_LEAGUE_SUMMARY_LIST& stList, ST_LEAGUE_APPLICANT_CHECK_LIST& stCheckList) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::GetApplicantList ucid=%u", dwUCID);

    // 遍历联赛列表中的每个联赛
    for (const auto& leagueInfo : stList.vecInfo) {
        std::int32_t nLeagueID = leagueInfo.nLeagueID;

        // 查找联赛
        auto it = m_mpLeagueList.find(nLeagueID);
        if (it == m_mpLeagueList.end()) {
            continue;
        }

        auto& pLeague = it->second;
        if (!pLeague) {
            continue;
        }

        // 获取联赛申请者列表
        ST_LEAGUE_APPLICANT_LIST stApplicantList{};
        pLeague->GetApplicantList(stApplicantList);

        // 检查用户是否在申请者列表中
        for (const auto& applicant : stApplicantList.vecInfo) {
            if (applicant.dwActorID == dwUCID) {
                stCheckList.vecInfo.push_back(static_cast<std::uint32_t>(nLeagueID));
                break;
            }
        }
    }

    stCheckList.nCount = static_cast<std::int32_t>(stCheckList.vecInfo.size());
}

// ============================================================================
// 错误消息发送
// ============================================================================

void CLeagueManager::SendLeagueErrorMsg(CServer* pServer, ST_REQ_LEAGUE_INVITE& stInvite, std::int32_t nErrorCode) {
    stInvite.nResult = nErrorCode;
    XSendPacket xSendPacket(0xF6, 0xC);
    xSendPacket << stInvite;
    if (pServer) {
        pServer->SendEx(xSendPacket);
    }
}

void CLeagueManager::SendLeagueErrorMsg(std::uint32_t dwActorID, std::int32_t nErrorCode) {
    LogHelper::LogDebug("game.league", "GreenDamTan_log LeagueManager.cpp::CLeagueManager::SendLeagueErrorMsg actorID=%u error=%d", dwActorID, nErrorCode);

    // 查找用户
    auto pUser = TXSingleton<XRelayServer>::Instance()->GetUser(dwActorID);
    if (!pUser) {
        LogHelper::LogError("game.league",
                           "SendLeagueErrorMsg error - Failed Accept Applicant because User is NULL[ UCID:%d, ErrorCode:%d ]( %d )",
                           dwActorID, nErrorCode, 980);
        return;
    }

    // 发送错误包（主命令0xF6，子命令0x16）
    XSendPacket xSendPacket(0xF6, 0x16);
    xSendPacket.XParse << dwActorID;
    xSendPacket.XParse << nErrorCode;
    pUser->SendPacket(xSendPacket);
}
