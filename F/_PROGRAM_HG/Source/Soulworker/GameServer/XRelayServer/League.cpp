#include "Soulworker/GameServer/XRelayServer/League.h"

#include "Soulworker/GameServer/XRelayServer/LeagueMember.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"

CLeague::CLeague() {
}

CLeague::~CLeague() {
}

void CLeague::Clear() {
    // 对齐 IDA 0x1400643c0: 仅重置两个计数器
    m_nInventoryCount = 0;
    m_nSyncCount = 0;
}

void CLeague::SetLeagueInfo(ST_LEAGUE_INFO stInfo) {
    m_stLeagueInfo = stInfo;
}

void CLeague::GetLeagueInfo(ST_LEAGUE_INFO& stInfo) {
    stInfo = m_stLeagueInfo;
}

// ============================================================================
// 成员管理
// ============================================================================

void CLeague::AddMember(ST_LEAGUE_MEMBER_EX stMember) {
    // 对齐 IDA 0x140064720: 成员已存在则更新，不存在则新建
    auto it = m_mpLeagueMember.find(stMember.dwUCID);
    if (it != m_mpLeagueMember.end()) {
        // 成员已存在，更新信息
        it->second->SetLeagueMember(stMember);
    } else {
        // 新成员，创建并插入
        auto pMember = std::make_shared<CLeagueMember>();
        if (pMember) {
            pMember->SetLeagueMember(stMember);
            m_mpLeagueMember.insert({stMember.dwUCID, pMember});
        }
    }
}

void CLeague::DelMember(std::uint32_t dwUCID) {
    LogHelper::LogDebug("game.league", "CLeague::DelMember ucid=%u", dwUCID);
    m_mpLeagueMember.erase(dwUCID);
}

bool CLeague::LoginMember(std::uint32_t dwUCID) {
    // 对齐 IDA 0x140064a00: 查找成员→判空→Login
    auto it = m_mpLeagueMember.find(dwUCID);
    if (it == m_mpLeagueMember.end()) {
        return false;
    }

    auto pMember = it->second;
    if (!pMember) {
        LogHelper::LogDebug("game.league", "LoginMember Member NULL(%d)", 123);
        return false;
    }

    pMember->Login();
    return true;
}

void CLeague::LogOutMember(std::uint32_t dwUCID, std::int64_t biLogoutDate) {
    // 对齐 IDA 0x140064af0: 查找成员→LogOut→SetPlayDate
    auto it = m_mpLeagueMember.find(dwUCID);
    if (it != m_mpLeagueMember.end()) {
        it->second->LogOut();
        it->second->SetPlayDate(biLogoutDate);
    }
}

bool CLeague::IsMember(std::uint32_t dwUCID) {
    return m_mpLeagueMember.find(dwUCID) != m_mpLeagueMember.end();
}

bool CLeague::IsMaster(std::uint32_t dwUCID) {
    return m_stLeagueInfo.dwMasterUCID == dwUCID;
}

std::shared_ptr<CLeagueMember> CLeague::GetLeagueMemberPtr(std::uint32_t dwUCID) {
    auto it = m_mpLeagueMember.find(dwUCID);
    if (it != m_mpLeagueMember.end()) {
        return it->second;
    }
    return nullptr;
}

void CLeague::GetMemberList(ST_LEAGUE_MEMBER_LIST& stList) {
    // 对齐 IDA 0x140065130: 遍历成员map，获取每个成员信息
    for (auto it = m_mpLeagueMember.begin(); it != m_mpLeagueMember.end(); ++it) {
        auto& pMember = it->second;
        if (pMember) {
            ST_LEAGUE_MEMBER_EX stMemberEx{};
            pMember->GetLeagueMember(stMemberEx);
            stList.vecInfo.push_back(stMemberEx);
        }
    }
}

void CLeague::SetMemberCount(std::int16_t nCount) {
    m_stLeagueInfo.shMemberCount = nCount;
}

std::int32_t CLeague::GetMemberCount() {
    return m_stLeagueInfo.shMemberCount;
}

void CLeague::LeagueMemberUpdate(const ST_LEAGUE_MEMBER_EX& stMember, ST_LEAGUE_MEMBER_UPDATE& stUpdate) {
    // 对齐 IDA 0x140064620 - 从 ST_LEAGUE_MEMBER_EX 填充 ST_LEAGUE_MEMBER_UPDATE
    wcscpy_s(stUpdate.szName, 21, stMember.szName);
    stUpdate.bLogin = stMember.bLogin;
    stUpdate.nLeagueID = stMember.stMember.nLeagueID;
    stUpdate.dwActorID = stMember.dwUCID;
    stUpdate.byLevel = static_cast<std::uint8_t>(stMember.shLevel);
    stUpdate.sWorld = stMember.sWorldID;
    stUpdate.biPlayDate = stMember.biPlayDate;
    stUpdate.byChannel = stMember.byChannel;
    stUpdate.byAwaken = stMember.byAwaken;
    stUpdate.dwProfilePhotoID = stMember.dwProfilePhotoID;
}

void CLeague::SetMemberBoardLimit(std::uint32_t dwUCID, std::int64_t biLimitTime) {
    // 对齐 IDA 0x140064e40: 设置成员公告板限制时间
    auto it = m_mpLeagueMember.find(dwUCID);
    if (it != m_mpLeagueMember.end()) {
        auto& pMember = it->second;
        if (!pMember) {
            LogHelper::LogDebug("game.league", "PossibleEnrollBoard Member NULL(%d)", 232);
        } else {
            pMember->SetEnrollBoardDate(biLimitTime);
        }
    }
}

// ============================================================================
// 职位管理
// ============================================================================

void CLeague::SetLeaguePosition(ST_LEAGUE_POSITION_NAME_CHANGE& stChange) {
    // 对齐 IDA 0x140064520
    if (stChange.nPosition == 1) {
        wcscpy_s(stChange.szPrevPositionName, 11, m_stLeagueInfo.szPosition_1);
        wcscpy_s(m_stLeagueInfo.szPosition_1, 11, stChange.szLeagueName);
    }
    if (stChange.nPosition == 2) {
        wcscpy_s(stChange.szPrevPositionName, 11, m_stLeagueInfo.szPosition_2);
        wcscpy_s(m_stLeagueInfo.szPosition_2, 11, stChange.szLeagueName);
    }
    if (stChange.nPosition == 3) {
        wcscpy_s(stChange.szPrevPositionName, 11, m_stLeagueInfo.szPosition_3);
        wcscpy_s(m_stLeagueInfo.szPosition_3, 11, stChange.szLeagueName);
    }
}

void CLeague::ChangeMemberPosition(ST_LEAGUE_MEMBER_POSITION& stPos, std::uint32_t dwActorID) {
    // 对齐 IDA 0x140065580
    auto pMember = GetLeagueMemberPtr(stPos.dwActorID);
    if (!pMember) {
        return;
    }

    std::uint8_t byPrevPosition = 0;
    pMember->GetPosition(byPrevPosition);
    pMember->SetPosition(stPos.byPosition);

    // 如果从副盟主(7)变更到其他职位，清空副盟主名称
    if (byPrevPosition == 7 && stPos.byPosition != 7) {
        wchar_t szTemp[21] = {};
        SetSubLeagueMaster(szTemp);
    }

    // 如果变更为副盟主(7)，设置副盟主名称
    if (stPos.byPosition == 7) {
        wchar_t szMemberName[21] = {};
        pMember->GetName(szMemberName);
        SetSubLeagueMaster(szMemberName);
    }

    ST_LEAGUE_MEMBER_EX stMemberInfo{};
    pMember->GetLeagueMember(stMemberInfo);
    std::int32_t nLeagueID = stMemberInfo.stMember.nLeagueID;

    ST_LEAGUE_INFO_FOR_GAME stLeagueInfoForGame{};
    SetLeagueInfoForGame(stPos.byPosition, stLeagueInfoForGame);

    SendChangePositionToMember(stPos, dwActorID, byPrevPosition, nLeagueID, stLeagueInfoForGame);
}

bool CLeague::CheckPositionCount(std::uint8_t byChangePosition) {
    // 对齐 IDA 0x140065360
    std::uint8_t nSubMaster = 0;
    std::uint8_t nManager = 0;
    std::uint8_t nElder = 0;

    for (auto it = m_mpLeagueMember.begin(); it != m_mpLeagueMember.end(); ++it) {
        auto& pMember = it->second;
        if (!pMember) continue;

        ST_LEAGUE_MEMBER_EX stMemberEx{};
        pMember->GetLeagueMember(stMemberEx);

        switch (stMemberEx.stMember.byPosition) {
            case 7:  // 副团长
                ++nSubMaster;
                break;
            case 5:  // 管理员
                ++nManager;
                break;
            case 4:  //长老
                ++nElder;
                break;
        }
    }

    switch (byChangePosition) {
        case 7:  // 副团长最多1人
            return nSubMaster != 1;
        case 5:  // 管理员最多3人
            return nManager != 3;
        case 4:  // 长老最多10人
            return nElder != 10;
    }

    return true;
}

void CLeague::SetSubLeagueMaster(wchar_t* szName) {
    LogHelper::LogDebug("game.league", "CLeague::SetSubLeagueMaster");
    if (szName) {
        wcscpy_s(m_stLeagueInfo.szSubMasterName, 21, szName);
    }
}

void CLeague::SetMasterName(wchar_t* szName) {
    LogHelper::LogDebug("game.league", "CLeague::SetMasterName");
    if (szName) {
        wcscpy_s(m_stLeagueInfo.szMasterName, 21, szName);
    }
}

void CLeague::SetLeagueName(wchar_t* szName) {
    LogHelper::LogDebug("game.league", "CLeague::SetLeagueName");
    if (szName) {
        wcscpy_s(m_stLeagueInfo.szLeagueName, 10, szName);
    }
}

// ============================================================================
// 申请者管理
// ============================================================================

void CLeague::AddApplicant(ST_LEAGUE_APPLICANT stApplicant) {
    // 对齐 IDA 0x140064ba0: 插入申请者到map
    m_mpLeagueApplicant.insert({stApplicant.dwActorID, stApplicant});
}

bool CLeague::DelApplicant(std::uint32_t dwActorID) {
    LogHelper::LogDebug("game.league", "CLeague::DelApplicant actorID=%u", dwActorID);
    return m_mpLeagueApplicant.erase(dwActorID) > 0;
}

bool CLeague::CheckExistApplicant(std::uint32_t dwActorID) {
    return m_mpLeagueApplicant.find(dwActorID) != m_mpLeagueApplicant.end();
}

bool CLeague::CheckApplicantCount(std::int32_t nCount) {
    // 对齐 IDA 0x140064d70: 申请人数小于50则允许
    return m_mpLeagueApplicant.size() < 50;
}

void CLeague::GetApplicantList(ST_LEAGUE_APPLICANT_LIST& stList) {
    // 对齐 IDA 0x140064f20: 遍历申请者map，添加到输出vector
    for (auto it = m_mpLeagueApplicant.begin(); it != m_mpLeagueApplicant.end(); ++it) {
        stList.vecInfo.push_back(it->second);
    }
}

wchar_t* CLeague::GetApplicant(std::uint32_t dwActorID) {
    auto it = m_mpLeagueApplicant.find(dwActorID);
    if (it != m_mpLeagueApplicant.end()) {
        return it->second.szName;
    }
    return nullptr;
}

// ============================================================================
// 公告板管理
// ============================================================================

void CLeague::AddBoard(ST_LEAGUE_BOARD stBoard) {
    // 对齐 IDA 0x140064ca0: 公告板最多50条，超出则移除最旧的
    if (m_deqBoard.size() >= 50) {
        m_deqBoard.pop_front();
    }
    m_deqBoard.push_back(stBoard);
}

void CLeague::GetBoardList(ST_LEAGUE_BOARD_LIST& stList) {
    // 对齐 IDA 0x140065080: 遍历公告板数据，最多50条
    std::int32_t nCount = static_cast<std::int32_t>(m_deqBoard.size());
    std::int32_t nLimit = std::min(nCount, 50);
    for (std::int32_t i = 0; i < nLimit; ++i) {
        stList.vecInfo.push_back(m_deqBoard[i]);
    }
}

// ============================================================================
// 记录管理
// ============================================================================

void CLeague::LoadRecord(ST_LEAGUE_RECORD& stRecord) {
    LogHelper::LogDebug("game.league", "CLeague::LoadRecord");
    m_deqRecord.push_back(stRecord);
}

void CLeague::UpdateRecord(ST_LEAGUE_RECORD& stRecord) {
    // 对齐 IDA 0x140067540
    // 如果记录数超过 100，移除最旧的
    if (m_deqRecord.size() > 100) {
        m_deqRecord.pop_front();
    }
    m_deqRecord.push_back(stRecord);

    // 发送 DB 更新 (main=7, sub=0x30)
    XSendDBPacket xSendDBPacket(nullptr, 7, 0x30);
    xSendDBPacket << stRecord;
    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);

    SendRecordToMember(stRecord);
}

void CLeague::GetRecordList(ST_LEAGUE_RECORD_LIST& stList) {
    // 对齐 IDA 0x140065f30: 遍历记录数据，最多100条
    std::int32_t nCount = static_cast<std::int32_t>(m_deqRecord.size());
    std::int32_t nLimit = std::min(nCount, 100);
    for (std::int32_t i = 0; i < nLimit; ++i) {
        stList.vecInfo.push_back(m_deqRecord[i]);
    }
}

// ============================================================================
// 公告管理
// ============================================================================

void CLeague::SetLeagueNotice(ST_LEAGUE_NOTICE stNotice) {
    // 对齐 IDA 0x1400644d0: 复制到 m_stLeagueInfo
    wcscpy_s(m_stLeagueInfo.szNotice, 801, stNotice.szNotice);
    m_stLeagueInfo.biNoticeDate = stNotice.biEnrollDate;
}

void CLeague::SetLeagueRecruitNotice(ST_LEAGUE_RECRUIT_NOTICE& stNotice) {
    // 对齐 IDA 0x140065530: 复制到 m_stLeagueInfo
    wcscpy_s(m_stLeagueInfo.szRecruitNotice, 51, stNotice.szNotice);
    m_stLeagueInfo.biRecruitNoticeDate = stNotice.biRegDate;
}

void CLeague::SetLeagueNoticeDate() {
    // 对齐 IDA 0x1400732f0: 清空公告日期
    m_stLeagueInfo.biNoticeDate = 0;
}

void CLeague::SetLeagueRecruitNoticeDate(std::int64_t biDate) {
    m_biRecruitNoticeDate = biDate;
}

void CLeague::GetNoticeDate(std::int64_t& biDate) {
    biDate = m_biNoticeDate;
}

std::int64_t CLeague::GetRecruitNoticeDate() {
    return m_biRecruitNoticeDate;
}

// ============================================================================
// 权限管理
// ============================================================================

void CLeague::SetLeagueAuth(std::int32_t* pnAuth) {
    LogHelper::LogDebug("game.league", "CLeague::SetLeagueAuth");
    if (pnAuth) {
        for (int i = 0; i < 9; ++i) {
            m_stLeagueInfo.nAuth[i] = pnAuth[i];
        }
    }
}

void CLeague::SetLeagueOpenOrNot(bool bOpen) {
    m_stLeagueInfo.bOpen = bOpen;
}

// ============================================================================
// 名称变更
// ============================================================================

void CLeague::ChangeMemberName(PS_CHANGE_NAME stChange) {
    // 对齐 IDA 0x140067ef0
    auto pMember = GetLeagueMemberPtr(stChange.dwActorID);
    if (!pMember) {
        return;
    }

    std::uint8_t byPosition = 0;
    pMember->GetPosition(byPosition);

    // 如果是会长，更新会长名称
    if (IsMaster(stChange.dwActorID)) {
        SetMasterName(const_cast<wchar_t*>(stChange.szChangeName));
    }
    else if (byPosition == 7) {
        // 如果是副会长，更新副会长名称
        SetSubLeagueMaster(const_cast<wchar_t*>(stChange.szChangeName));
    }

    // 更新成员名称
    pMember->SetName(stChange.szChangeName);

    // 构造更新广播
    ST_LEAGUE_MEMBER_EX stMemberEx{};
    pMember->GetLeagueMember(stMemberEx);

    ST_LEAGUE_MEMBER_UPDATE stUpdate{};
    LeagueMemberUpdate(stMemberEx, stUpdate);

    SendChangeMemberName(stUpdate);
}

void CLeague::UpdateApplicantName(PS_CHANGE_NAME stChange) {
    // 对齐 IDA 0x1400680a0: 更新申请者名称后广播
    auto it = m_mpLeagueApplicant.find(stChange.dwActorID);
    if (it != m_mpLeagueApplicant.end()) {
        wcscpy_s(it->second.szName, 21, stChange.szChangeName);
        // 对齐 IDA: 复制申请者信息并广播名称变更
        ST_LEAGUE_APPLICANT stApplicantInfo = it->second;
        SendChangeApplicantName(stApplicantInfo);
    }
}

// 对齐 IDA 0x1400648e0: 获取待删除成员的名字
void CLeague::GetDelName(std::uint32_t dwUCID, wchar_t* szName) {
    // 对齐 IDA: 获取成员指针
    auto pMember = GetLeagueMemberPtr(dwUCID);
    if (!pMember) {
        if (szName) szName[0] = L'\0';
        return;
    }

    // 对齐 IDA: 获取名字并复制到输出参数
    wchar_t szBuffer[21] = {};
    pMember->GetName(szBuffer);

    // 复制名字
    if (szName) {
        wcscpy(szName, szBuffer);
    }
}

// ============================================================================
// 转让
// ============================================================================

std::int32_t CLeague::CheckLeagueDelegate(std::uint32_t dwReqActorID, std::uint32_t dwTargetActorID) {
    // 对齐 IDA 0x1400657c0

    // 检查请求者是否为会长
    if (!IsMaster(dwReqActorID)) {
        return 57015;  // 不是会长
    }

    // 检查被转让者是否存在
    auto pDelegatedUser = GetLeagueMemberPtr(dwTargetActorID);
    if (!pDelegatedUser) {
        return 57007;  // 被转让者不存在
    }

    // 检查请求者是否仍存在
    auto pReqUser = GetLeagueMemberPtr(dwReqActorID);
    if (!pReqUser) {
        return 57007;  // 请求者不存在
    }

    // 检查成员数量 >= 2
    if (m_stLeagueInfo.shMemberCount < 2) {
        return 57044;  // 成员数量不足
    }

    // 检查联赛评分 >= 2
    if (m_stLeagueInfo.byRating < 2) {
        return 57044;  // 评分不足
    }

    return 0;  // 成功
}

bool CLeague::Delegate(PS_REQ_LEAGUE_DELEGATE& stReq, PS_RES_LEAGUE_DELEGATE& stRes, std::uint32_t dwActorID) {
    // 对齐 IDA 0x140065910

    auto pDelegateUser = GetLeagueMemberPtr(dwActorID);
    auto pDelegatedUser = GetLeagueMemberPtr(stReq.dwDelegatedUCID);

    if (!pDelegateUser || !pDelegatedUser) {
        return false;
    }

    // 原会长降为普通成员 (position=0)
    pDelegateUser->SetPosition(0);
    wchar_t szDelegateName[21] = {};
    pDelegateUser->GetName(szDelegateName);

    // 新会长设置为会长 (position=100=0x64)
    pDelegatedUser->SetPosition(100);
    wchar_t szDelegatedName[21] = {};
    pDelegatedUser->GetName(szDelegatedName);

    // 更新联赛信息中的会长
    m_stLeagueInfo.dwMasterUCID = stReq.dwDelegatedUCID;
    wcscpy_s(m_stLeagueInfo.szMasterName, 21, szDelegatedName);

    // 填充响应结构体
    stRes.nLeagueID = m_stLeagueInfo.nLeagueID;
    wcscpy_s(stRes.szDelegateName, 21, szDelegateName);
    wcscpy_s(stRes.szDelegatedName, 21, szDelegatedName);
    stRes.nResult = 0;

    // 创建 ST_LEAGUE_RECORD 记录转让事件 (byFlag=12)
    ST_LEAGUE_RECORD stRecordInfo{};
    stRecordInfo.nLeagueID = m_stLeagueInfo.nLeagueID;
    stRecordInfo.byFlag = 12;
    stRecordInfo.biRegisterDate = TXSingleton<XRelayServer>::Instance()->GetCurDateSec();
    wcscpy_s(stRecordInfo.szValue1, 21, szDelegateName);
    wcscpy_s(stRecordInfo.szValue2, 21, szDelegatedName);
    UpdateRecord(stRecordInfo);

    // 获取 dwUAID
    std::uint32_t dwUAID = 0;
    auto pUser = TXSingleton<XRelayServer>::Instance()->GetUser(dwActorID);
    if (pUser) {
        dwUAID = pUser->GetUAID();
    }

    // 发送 DB 日志 (main=15, sub=22)
    TXSingleton<XRelayServer>::Instance()->SendDBLog(
        static_cast<int>(dwUAID),
        static_cast<int>(dwActorID),
        15, 22,
        m_stLeagueInfo.nLeagueID,
        static_cast<int>(stReq.dwDelegatedUCID),
        0, 0, 0,
        0,
        static_cast<int>(m_stLeagueInfo.byRating),
        L""
    );

    return true;
}

// ============================================================================
// 卡片变更
// ============================================================================

std::int32_t CLeague::CheckLeagueCardChange(std::uint32_t dwActorID) {
    // 对齐 IDA 0x140065ee0
    if (!HaveSkill(E_SKILL_CARD)) {
        return 57046;
    }
    if (IsMaster(dwActorID)) {
        return 0;
    }
    return 57015;
}

void CLeague::CardChange(PS_REQ_LEAGUE_CARD& stCard, std::uint32_t dwActorID) {
    // 对齐 IDA 0x140065fe0
    m_stLeagueInfo.dwLeagueCard = stCard.dwLeagueCard;
}

// ============================================================================
// 技能和等级
// ============================================================================

bool CLeague::HaveSkill(enum E_LEAGUE_SKILL eSkill) {
    // 对齐 IDA 0x140066010
    if (eSkill >= E_LEAGUE_SKILL_MAX) {
        return false;
    }
    return m_stLeagueInfo.bySkill[eSkill] != 0;
}

std::int32_t CLeague::CheckLearnSkill(PS_REQ_LEAGUE_SKILL stSkill, PS_RES_LEAGUE_SKILL& stRes) {
    // 对齐 IDA 0x140066050
    stRes.nLeagueID = stSkill.nLeagueID;
    stRes.dwUCID = stSkill.dwUCID;

    if (!IsMaster(stSkill.dwUCID)) {
        return 57015;
    }

    std::uint8_t byLevel = m_stLeagueInfo.byRating;
    auto pTB_SkillInfo = TXSingleton<XRelayServer>::Instance()->GetResourceMgr().GetTB_LEAGUE_SKILL(stSkill.bySkillIndex);
    if (!pTB_SkillInfo) {
        LogHelper::LogError("game.relay", "CheckLearnSkill error - No Table TB_LEAGUE_SKILL( %d )", 554);
        return 57046;
    }

    std::int32_t nGroupID = static_cast<std::int32_t>(pTB_SkillInfo->League_Skill_Group);
    if (nGroupID <= 0) {
        return 57046;
    }

    // 检查前置技能
    if (stSkill.byType == 1 && !HaveSkill(static_cast<E_LEAGUE_SKILL>(nGroupID - 1))) {
        return 57046;
    }

    // 检查等级上限
    if (m_stLeagueInfo.bySkill[nGroupID - 1] >= pTB_SkillInfo->League_Skill_Level_Max) {
        return 57050;
    }

    // 检查联赛等级要求
    if (pTB_SkillInfo->League_Get_Skill_Level > byLevel) {
        return 57047;
    }

    // 检查技能点
    if (pTB_SkillInfo->League_Skill_Need_Point > m_stLeagueInfo.bySkillPoint) {
        return 57048;
    }

    // 检查金币
    if (pTB_SkillInfo->League_Skill_Need_Gold > m_stLeagueInfo.biMoney) {
        return 57049;
    }

    // 填充响应
    stRes.nLeagueID = stSkill.nLeagueID;
    stRes.dwUCID = stSkill.dwUCID;
    stRes.bySkillIndex = stSkill.bySkillIndex;
    stRes.bySkillGroupID = static_cast<std::uint8_t>(nGroupID - 1);
    stRes.bySkillLevel = m_stLeagueInfo.bySkill[stRes.bySkillGroupID] + 1;
    stRes.bySkillPoint = static_cast<std::uint8_t>(pTB_SkillInfo->League_Skill_Need_Point);
    stRes.biGold = pTB_SkillInfo->League_Skill_Need_Gold;

    return 0;
}

void CLeague::LearnSkill(PS_RES_LEAGUE_SKILL stSkill) {
    // 对齐 IDA 0x140066280
    std::uint8_t byPrevSkillLv = m_stLeagueInfo.bySkill[stSkill.bySkillGroupID];
    std::uint8_t byPrevSkillPoint = stSkill.bySkillPoint;

    m_stLeagueInfo.biMoney -= stSkill.biGold;
    m_stLeagueInfo.bySkillPoint -= stSkill.bySkillPoint;
    m_stLeagueInfo.bySkill[stSkill.bySkillGroupID] = stSkill.bySkillLevel;

    // 更新响应中的剩余值
    PS_RES_LEAGUE_SKILL stResCopy = stSkill;
    stResCopy.biGold = m_stLeagueInfo.biMoney;
    stResCopy.bySkillPoint = m_stLeagueInfo.bySkillPoint;

    SendLearnSkillToMember(stResCopy);

    // 获取 UAID
    std::uint32_t dwUAID = 0;
    auto pUser = TXSingleton<XRelayServer>::Instance()->GetUser(stSkill.dwUCID);
    if (pUser) {
        dwUAID = pUser->GetUAID();
    }

    // 发送 DB 日志 (main=15, sub=21) - 技能学习日志
    TXSingleton<XRelayServer>::Instance()->SendDBLog(
        static_cast<int>(dwUAID),
        static_cast<int>(stSkill.dwUCID),
        15, 21,
        m_stLeagueInfo.nLeagueID,
        static_cast<int>(stSkill.bySkillGroupID),
        static_cast<int>(byPrevSkillLv),
        static_cast<int>(stSkill.bySkillLevel),
        0, 0,
        m_stLeagueInfo.byRating,
        L""
    );

    // 发送 DB 日志 (main=15, sub=20) - 技能点日志
    TXSingleton<XRelayServer>::Instance()->SendDBLog(
        static_cast<int>(dwUAID),
        static_cast<int>(stSkill.dwUCID),
        15, 20,
        m_stLeagueInfo.nLeagueID,
        1,
        static_cast<int>(stSkill.bySkillPoint),
        static_cast<int>(byPrevSkillPoint),
        static_cast<int>(m_stLeagueInfo.bySkillPoint),
        0,
        m_stLeagueInfo.byRating,
        L""
    );
}

void CLeague::Levelup(std::uint8_t byAddLevel, std::uint32_t dwUCID) {
    // 对齐 IDA 0x1400666c0
    bool bCheat = false;

    // 检查等级上限 (最大 10 级)
    if (byAddLevel + m_stLeagueInfo.byRating > 10) {
        return;
    }

    std::uint32_t dwIndex = byAddLevel + m_stLeagueInfo.byRating;
    auto pTB_LeagueInfo = TXSingleton<XRelayServer>::Instance()->GetResourceMgr().GetTB_LEAGUE_INFO(dwIndex);
    if (!pTB_LeagueInfo) {
        LogHelper::LogError("game.relay", "Levelup error - No Table TB_LEAGUE_INFO( %d )", 634);
        return;
    }

    std::uint8_t byLevel = byAddLevel + m_stLeagueInfo.byRating;
    std::uint8_t bySkillPoint = static_cast<std::uint8_t>(pTB_LeagueInfo->League_Skill_Get_Point + m_stLeagueInfo.bySkillPoint);

    if (byLevel == 0) {
        byLevel = 1;
    }

    // 构建 PS_AUTO_SKILL - 复制现有技能
    PS_AUTO_SKILL psAutoSkillInfo{};
    for (int i = 0; i < 8; ++i) {
        psAutoSkillInfo.bySkillInfo[i] = m_stLeagueInfo.bySkill[i];
    }

    // 遍历 TB_LEAGUE_SKILL 表，自动学习满足条件的技能
    // 对齐 IDA：League_Skill_Type == 0 且 League_Get_Skill_Level <= byLevel
    const auto& mapLeagueSkill = TXSingleton<XRelayServer>::Instance()->GetResourceMgr().GetTB_LEAGUE_SKILL_Map();
    for (auto it = mapLeagueSkill.begin(); it != mapLeagueSkill.end(); ++it) {
        const TB_LEAGUE_SKILL& rSkill = it->second;

        // 检查 League_Skill_Type == 0 (被动技能) 且等级要求满足
        if (rSkill.League_Skill_Type != 0) {
            continue;
        }

        if (rSkill.League_Get_Skill_Level > byLevel) {
            continue;
        }

        // League_Skill_Group 从 1 开始，数组索引从 0 开始
        std::int32_t nSkillGroupID = static_cast<std::int32_t>(rSkill.League_Skill_Group) - 1;
        if (nSkillGroupID < 0 || nSkillGroupID >= 8) {
            continue;
        }

        // 设置自动学习的技能等级
        psAutoSkillInfo.bySkillInfo[nSkillGroupID] = static_cast<std::uint8_t>(rSkill.League_Skill_Level);
    }

    UpdateSyncCount();

    // 发送 DB 更新请求 (main=7, sub=0x34)
    XSendDBPacket xSendDBPacket(nullptr, 7, 0x34);
    xSendDBPacket.XParse << m_stLeagueInfo.nLeagueID;
    xSendDBPacket.XParse << static_cast<std::uint8_t>(byLevel);
    xSendDBPacket.XParse << static_cast<std::int8_t>(bySkillPoint);
    xSendDBPacket << psAutoSkillInfo;
    xSendDBPacket.XParse << bCheat;
    xSendDBPacket.XParse << dwUCID;
    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
}

void CLeague::ApplyLevelup(std::uint8_t byLevel, std::uint8_t bySkillPoint, PS_AUTO_SKILL stSkill, std::uint32_t dwUCID) {
    // 对齐 IDA 0x140066910

    std::uint8_t byPrevLevel = m_stLeagueInfo.byRating;
    std::uint8_t byAddSkillPoint = bySkillPoint - m_stLeagueInfo.bySkillPoint;
    std::uint8_t byPrevSkillPoint = m_stLeagueInfo.bySkillPoint;

    // 更新联赛信息
    m_stLeagueInfo.byRating = byLevel;
    m_stLeagueInfo.bySkillPoint = bySkillPoint;
    for (int i = 0; i < 8; ++i) {
        m_stLeagueInfo.bySkill[i] = stSkill.bySkillInfo[i];
    }

    SendLevelupToMember(byLevel, bySkillPoint, stSkill);

    // 获取 UAID
    std::uint32_t dwUAID = 0;
    auto pUser = TXSingleton<XRelayServer>::Instance()->GetUser(dwUCID);
    if (pUser) {
        dwUAID = pUser->GetUAID();
    }

    // 发送 DB 日志 (main=15, sub=17) - 等级变化日志
    TXSingleton<XRelayServer>::Instance()->SendDBLog(
        static_cast<int>(dwUAID),
        static_cast<int>(dwUCID),
        15, 17,
        m_stLeagueInfo.nLeagueID,
        static_cast<int>(byPrevLevel),
        0, 0, 0, 0,
        static_cast<std::int64_t>(m_stLeagueInfo.byRating),
        L""
    );

    // 发送 DB 日志 (main=15, sub=20) - 技能点日志
    TXSingleton<XRelayServer>::Instance()->SendDBLog(
        static_cast<int>(dwUAID),
        static_cast<int>(dwUCID),
        15, 20,
        m_stLeagueInfo.nLeagueID,
        0,
        static_cast<int>(byAddSkillPoint),
        static_cast<int>(byPrevSkillPoint),
        static_cast<int>(m_stLeagueInfo.bySkillPoint),
        0,
        static_cast<std::int64_t>(m_stLeagueInfo.byRating),
        L""
    );
}

// ============================================================================
// 经验和财富
// ============================================================================

void CLeague::CalculateExp(PS_LEAGUE_WEALTH_FOR_SERVER stWealth) {
    // 对齐 IDA 0x140066d90

    auto pMember = GetLeagueMemberPtr(stWealth.dwUCID);
    if (!pMember) {
        LogHelper::LogError("game.relay", "CalculateExp error - Cant find member[UCID:%d]( %d )", stWealth.dwUCID, 707);
        return;
    }

    std::uint8_t byLevel = m_stLeagueInfo.byRating;
    auto pTB_LeagueInfo = TXSingleton<XRelayServer>::Instance()->GetResourceMgr().GetTB_LEAGUE_INFO(byLevel);
    if (!pTB_LeagueInfo) {
        LogHelper::LogError("game.relay", "CalculateExp error - No Table TB_LEAGUE_INFO[ LeagueID:%d ](%d)", m_stLeagueInfo.nLeagueID, 716);
        return;
    }

    // 保存原始值
    std::int64_t biTempLeagueExp = m_stLeagueInfo.biExp;
    std::int32_t nTempLeagueLimitExp = m_stLeagueInfo.nLimitExp;
    std::int64_t biTempMoney = m_stLeagueInfo.biMoney;

    // 获取成员每日经验上限（对齐 IDA：CFriendRecruitManager::IsLoad 实际是 GetDailyExpLimit）
    std::int64_t byTempMemberLimitExp = 0;
    if (pMember) {
        byTempMemberLimitExp = pMember->GetDailyExpLimit();
    }

    PS_LEAGUE_WEALTH_FOR_SERVER stWealthCopy = stWealth;
    stWealthCopy.biPrevExp = biTempLeagueExp;

    // 添加经验和金币
    m_stLeagueInfo.nLimitExp += stWealthCopy.shExp;
    m_stLeagueInfo.biExp += stWealthCopy.shExp;
    m_stLeagueInfo.biMoney += stWealthCopy.nGold;

    bool bApplyExp = true;

    // 检查联赛每日经验上限
    if (m_stLeagueInfo.nLimitExp >= pTB_LeagueInfo->Day_Exp_Get_Point) {
        m_stLeagueInfo.nLimitExp = pTB_LeagueInfo->Day_Exp_Get_Point;
        stWealthCopy.shExp = pTB_LeagueInfo->Day_Exp_Get_Point - nTempLeagueLimitExp;
        m_stLeagueInfo.biExp = stWealthCopy.shExp + biTempLeagueExp;
        bApplyExp = stWealthCopy.shExp > 0;
    }

    // 成员经验上限检查 (200) - 对齐 IDA
    if (static_cast<std::uint8_t>(stWealthCopy.shExp + byTempMemberLimitExp) >= 200) {
        stWealthCopy.shExp = static_cast<std::int16_t>(200 - byTempMemberLimitExp);
        m_stLeagueInfo.biExp = stWealthCopy.shExp + biTempLeagueExp;
        if (stWealthCopy.shExp <= 0) {
            bApplyExp = false;
        }
    }

    // 检查金币上限 (999999999)
    if (m_stLeagueInfo.biMoney > 999999999) {
        stWealthCopy.nGold = 999999999 - biTempMoney;
        m_stLeagueInfo.biMoney = 999999999;
        if (stWealthCopy.nGold <= 0) {
            stWealthCopy.nGold = 0;
        }
    } else if (m_stLeagueInfo.biMoney < 0) {
        m_stLeagueInfo.biMoney = 0;
        stWealthCopy.nGold = 0;
    }

    if (bApplyExp || stWealthCopy.nGold) {
        std::uint8_t byAddLevel = 0;

        // 检查是否可升级（等级 < 10）
        if (byLevel < 10 && bApplyExp) {
            std::int32_t nTempExp = static_cast<std::int32_t>(m_stLeagueInfo.biExp);

            // 计算升级
            while (nTempExp >= pTB_LeagueInfo->Need_Exp) {
                nTempExp -= pTB_LeagueInfo->Need_Exp;
                std::uint32_t dwNewLevel = ++byAddLevel + byLevel;

                auto pNextLevelInfo = TXSingleton<XRelayServer>::Instance()->GetResourceMgr().GetTB_LEAGUE_INFO(dwNewLevel);
                if (!pNextLevelInfo) {
                    --byAddLevel;
                    LogHelper::LogError("game.contents", "ApplyExp error - No Table TB_LEAGUE_INFO(%d)", 795);
                    break;
                }
                pTB_LeagueInfo = pNextLevelInfo;
            }

            // 如果升级了
            if (byAddLevel > 0) {
                Levelup(byAddLevel, stWealthCopy.dwUCID);

                if (byLevel >= 10) {
                    nTempExp = 0;
                }
                m_stLeagueInfo.biExp = nTempExp;
            }

            stWealthCopy.nTotalExp = nTempExp;

            // 发送 DB 更新 (main=7, sub=0x33)
            XSendDBPacket xSendDBPacket(nullptr, 7, 0x33);
            xSendDBPacket << stWealthCopy;
            TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
        }

        // 等级已达上限 (10级)，仅更新经验
        if (byLevel == 10 && bApplyExp) {
            stWealthCopy.nTotalExp = m_stLeagueInfo.biExp;
            XSendDBPacket xSendPacket(nullptr, 7, 0x33);
            xSendPacket << stWealthCopy;
            TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendPacket);
        }

        // 仅金币，无经验更新
        if (!bApplyExp) {
            stWealthCopy.nTotalExp = m_stLeagueInfo.biExp;
            stWealthCopy.shExp = 0;
            XSendDBPacket xSendPacket(nullptr, 7, 0x33);
            xSendPacket << stWealthCopy;
            TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendPacket);
        }
    }
}

void CLeague::ApplyWealth(PS_LEAGUE_WEALTH_FOR_SERVER stWealth) {
    // 对齐 IDA 0x140067390

    auto pMember = GetLeagueMemberPtr(stWealth.dwUCID);
    if (!pMember) {
        return;
    }

    std::int64_t biPrevGold = m_stLeagueInfo.biMoney - stWealth.nGold;
    if (biPrevGold < 0) {
        biPrevGold = 0;
    }

    // 添加成员经验
    pMember->AddExp(stWealth.shExp);

    // 构建更新信息
    ST_LEAGUE_INFO_UPDATE stUpdate{};
    stUpdate.biExp = stWealth.nTotalExp;
    stUpdate.biLeagueMoney = m_stLeagueInfo.biMoney;
    stUpdate.dwLeagueCard = m_stLeagueInfo.dwLeagueCard;
    stUpdate.nLeagueID = m_stLeagueInfo.nLeagueID;
    stUpdate.nLeagueRank = m_stLeagueInfo.nLeagueRank;
    stUpdate.shLeagueMemeberCnt = m_stLeagueInfo.shMemberCount;

    SendLeagueWealthToMember(stWealth.dwUCID, stUpdate);

    // 获取 UAID
    std::uint32_t dwUAID = 0;
    auto pUser = TXSingleton<XRelayServer>::Instance()->GetUser(stWealth.dwUCID);
    if (pUser) {
        dwUAID = pUser->GetUAID();
    }

    // 发送经验日志 (main=15, sub=18)
    if (stWealth.shExp > 0) {
        TXSingleton<XRelayServer>::Instance()->SendDBLog(
            static_cast<int>(dwUAID),
            static_cast<int>(stWealth.dwUCID),
            15, 18,
            m_stLeagueInfo.nLeagueID,
            static_cast<int>(stWealth.shExp),
            0, 0,
            static_cast<int>(m_stLeagueInfo.byRating),
            stWealth.biPrevExp,
            m_stLeagueInfo.biExp,
            L""
        );
    }

    // 发送金币日志 (main=15, sub=19)
    if (stWealth.nGold > 0) {
        TXSingleton<XRelayServer>::Instance()->SendDBLog(
            static_cast<int>(dwUAID),
            static_cast<int>(stWealth.dwUCID),
            15, 19,
            m_stLeagueInfo.nLeagueID,
            static_cast<int>(stWealth.nGold),
            0, 0,
            static_cast<int>(m_stLeagueInfo.byRating),
            biPrevGold,
            m_stLeagueInfo.biMoney,
            L""
        );
    }
}

void CLeague::ResetExp(std::int64_t biInitDate) {
    // 对齐 IDA 0x140067640
    // 设置初始化日期并重置每日经验上限
    m_stLeagueInfo.biInitDate = biInitDate;
    m_stLeagueInfo.nLimitExp = 0;

    // 遍历所有成员，重置其每日经验
    for (auto it = m_mpLeagueMember.begin(); it != m_mpLeagueMember.end(); ++it) {
        auto& pMember = it->second;
        if (pMember) {
            pMember->ResetExp();
        }
    }
}

void CLeague::ResetExpInitDate(std::uint32_t dwParam) {
    // 对齐 IDA 0x140067730: 发送DB重置请求 + 重置成员经验
    // 1. 发送 DB 包 (main=7, sub=0x36)
    XSendDBPacket xSendDBPacket(nullptr, 7, 0x36);
    xSendDBPacket.XParse << m_stLeagueInfo.nLeagueID;
    xSendDBPacket.XParse << dwParam;
    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);

    // 2. 获取成员并重置经验
    auto pMember = GetLeagueMemberPtr(dwParam);
    if (pMember) {
        pMember->ResetExp();
    }
}

// ============================================================================
// 更新
// ============================================================================

void CLeague::UpdateApplyList(ATL::CTime tNow) {
    // 对齐 IDA 0x140067820: 使用 ATL::CTime 参数
    // 遍历申请者，检查超时（1天 = 86400秒 = 0x15180）

    for (auto it = m_mpLeagueApplicant.begin(); it != m_mpLeagueApplicant.end(); ) {
        const auto& stApplicant = it->second;

        // 对齐 IDA: CTime(stApplicant.biApplicantDate) + CTimeSpan(86400) < tNow
        if (ATL::CTime(stApplicant.biApplicantDate) + ATL::CTimeSpan(86400) < tNow) {
            // 超时，发送 DB 删除请求 (main=7, sub=0x20)
            XSendDBPacket xSendDBPacket(nullptr, 7, 0x20);
            xSendDBPacket.XParse << stApplicant.nLeagueID;
            xSendDBPacket.XParse << stApplicant.dwActorID;
            TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);

            // 从 map 中移除
            it = m_mpLeagueApplicant.erase(it);
        } else {
            ++it;
        }
    }
}

void CLeague::UpDateLeagueMemberInfo() {
    // 对齐 IDA 0x140067cb0
    // 遍历联赛成员，从在线用户同步最新数据

    for (auto it = m_mpLeagueMember.begin(); it != m_mpLeagueMember.end(); ++it) {
        auto& pMember = it->second;
        if (!pMember) {
            continue;
        }

        // 获取当前存储的成员信息
        ST_LEAGUE_MEMBER_EX stMemberInfo{};
        pMember->GetLeagueMember(stMemberInfo);

        // 从在线用户获取最新数据
        auto pMemberUser = TXSingleton<XRelayServer>::Instance()->GetUser(stMemberInfo.dwUCID);
        if (pMemberUser) {
            ST_LEAGUE_MEMBER_EX stUpdateInfo{};
            pMemberUser->GetLeagueMemberInfo(stUpdateInfo);
            pMember->SetLeagueMember(stUpdateInfo);
        }
    }
}

void CLeague::UpdateSyncCount() {
    ++m_nSyncCount;
}

void CLeague::UpdateInventorySyncCount() {
    ++m_nInventoryCount;
}

void CLeague::SetLeagueInfoForGame(std::uint8_t byPosition, ST_LEAGUE_INFO_FOR_GAME& stInfo) {
    // 对齐 IDA 0x140067a00
    stInfo.byLeagueLevel = m_stLeagueInfo.byRating;
    stInfo.byPosition = byPosition;
    stInfo.dwMasterUCID = m_stLeagueInfo.dwMasterUCID;
    for (int i = 0; i < 8; ++i) {
        stInfo.bySkillInfo[i] = m_stLeagueInfo.bySkill[i];
    }
    for (int j = 0; j < 9; ++j) {
        stInfo.nAuth[j] = m_stLeagueInfo.nAuth[j];
    }
}

void CLeague::UpdateSkillPoint_Cheat(std::uint8_t byType, std::uint32_t dwParam) {
    // 对齐 IDA 0x140067ac0: GM技能点修改
    // 参数: byType=bySkillPoint(新技能点), dwParam=dwUCID(目标成员)
    bool bCheat = true;

    // 更新技能点
    m_stLeagueInfo.bySkillPoint = byType;

    // 复制当前技能到 PS_AUTO_SKILL
    PS_AUTO_SKILL psAutoSkill{};
    for (int i = 0; i < 8; ++i) {
        psAutoSkill.bySkillInfo[i] = m_stLeagueInfo.bySkill[i];
    }

    // 发送 DB 更新 (main=7, sub=0x34)
    XSendDBPacket xSendDBPacket(nullptr, 7, 0x34);
    xSendDBPacket.XParse << m_stLeagueInfo.nLeagueID;
    xSendDBPacket.XParse << m_stLeagueInfo.byRating;
    xSendDBPacket.XParse << byType;
    xSendDBPacket << psAutoSkill;
    xSendDBPacket.XParse << bCheat;
    xSendDBPacket.XParse << dwParam;
    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);

    // 广播给成员
    SendLevelupToMember(m_stLeagueInfo.byRating, m_stLeagueInfo.bySkillPoint, psAutoSkill);
}

// ============================================================================
// 同步计数
// ============================================================================

bool CLeague::CompareSyncCount(std::int32_t nCount) {
    // 对齐 IDA 0x140069380: 返回 true 表示不同步(需要同步)
    return m_nSyncCount != nCount;
}

std::int32_t CLeague::GetSyncCount() {
    return m_nSyncCount;
}

// ============================================================================
// 发送消息
// ============================================================================

void CLeague::SendLeagueInfo(std::uint32_t dwUCID) {
    // 对齐 IDA 0x1400688c0: 获取成员信息→构建联赛信息广播包 (0xF6, 3)
    ST_LEAGUE_MEMBER_LIST stMemberList{};
    ST_LEAGUE_MEMBER_EX stMemberEx{};
    ST_LEAGUE_MEMBER_UPDATE stUpdate{};
    ST_LEAGUE_APPLICANT_LIST stApplicant{};
    ST_LEAGUE_BOARD_LIST stBoard{};
    ST_LEAGUE_INFO_EX stInfoEx{};
    ST_LEAGUE_RECORD_LIST stRecordList{};
    ST_LEAGUE_INFO_FOR_GAME stLeagueInfoForGame{};

    auto pMember = GetLeagueMemberPtr(dwUCID);
    if (!pMember) {
        return;
    }

    pMember->GetLeagueMember(stMemberEx);
    LeagueMemberUpdate(stMemberEx, stUpdate);
    GetMemberList(stMemberList);
    GetApplicantList(stApplicant);
    GetBoardList(stBoard);
    GetRecordList(stRecordList);

    stInfoEx.nLeagueID = m_stLeagueInfo.nLeagueID;
    stInfoEx.dwLeagueCard = m_stLeagueInfo.dwLeagueCard;
    wcscpy_s(stInfoEx.szLeagueName, m_stLeagueInfo.szLeagueName);

    std::uint8_t byState = 0;
    bool bLogin = true;

    SetLeagueInfoForGame(stMemberEx.stMember.byPosition, stLeagueInfoForGame);

    XSendPacket xSendPacket(0xF6, 3);
    xSendPacket.XParse << bLogin;
    xSendPacket << stUpdate;
    xSendPacket << m_stLeagueInfo;
    xSendPacket << stMemberList;
    xSendPacket << stApplicant;
    xSendPacket << stBoard;
    xSendPacket.XParse << byState;
    xSendPacket << stInfoEx;
    xSendPacket << stRecordList;
    xSendPacket << stLeagueInfoForGame;
    xSendPacket.XParse << m_nSyncCount;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

void CLeague::SendSyncLeagueInfo(PS_SYNC_LEAGUE_INFO stSync) {
    // 对齐 IDA 0x1400693c0: 同步联赛信息→单播给指定用户 (0xF6, 0x58)
    if (!CompareSyncCount(stSync.nSyncCount)) {
        stSync.bSync = 1;
    }

    ST_LEAGUE_MEMBER_LIST stMemberList{};
    ST_LEAGUE_MEMBER_EX stMemberEx{};
    ST_LEAGUE_MEMBER_UPDATE stUpdate{};
    ST_LEAGUE_APPLICANT_LIST stApplyList{};
    ST_LEAGUE_BOARD_LIST stBoardList{};
    ST_LEAGUE_INFO_EX stInfoEx{};
    ST_LEAGUE_RECORD_LIST stRecordList{};
    ST_LEAGUE_INFO_FOR_GAME stLeagueInfoForGame{};

    auto pMemberPtr = GetLeagueMemberPtr(stSync.dwUCID);
    if (pMemberPtr) {
        pMemberPtr->GetLeagueMember(stMemberEx);
    }
    LeagueMemberUpdate(stMemberEx, stUpdate);
    GetApplicantList(stApplyList);
    GetBoardList(stBoardList);
    GetMemberList(stMemberList);
    GetRecordList(stRecordList);

    stInfoEx.nLeagueID = m_stLeagueInfo.nLeagueID;
    stInfoEx.dwLeagueCard = m_stLeagueInfo.dwLeagueCard;
    wcscpy_s(stInfoEx.szLeagueName, m_stLeagueInfo.szLeagueName);

    std::uint8_t byState = 0;
    SetLeagueInfoForGame(stMemberEx.stMember.byPosition, stLeagueInfoForGame);

    auto pUser = TXSingleton<XRelayServer>::Instance()->GetUser(stSync.dwUCID);
    if (!pUser) {
        LogHelper::LogError("game.relay", "SendSyncLeagueInfo error - User NULL[ LeagueID:%d, UCID:%d ]( %d )",
            m_stLeagueInfo.nLeagueID, stSync.dwUCID, 1359);
        return;
    }

    XSendPacket xSendPacket(0xF6, 0x58);
    xSendPacket.XParse << byState;
    xSendPacket << m_stLeagueInfo;
    xSendPacket << stMemberList;
    xSendPacket << stApplyList;
    xSendPacket << stBoardList;
    xSendPacket << stRecordList;
    xSendPacket << stLeagueInfoForGame;
    xSendPacket << stSync;
    pUser->SendPacket(xSendPacket);
}

void CLeague::SendInventoryInfo(std::uint32_t dwActorID, PS_RES_STORAGE_INFO stStorage, PS_ITEM_BROACH_LIST stBroach, PS_ITEM_SOCKET_LIST stSocket, PS_ITEM_PACKAGE_LIST stPackage) {
    // 对齐 IDA 0x1400697c0: 发送联赛仓库信息广播包 (0xF6, 0x60)
    XSendPacket xSendPacket(0xF6, 0x60);
    xSendPacket.XParse << dwActorID;
    xSendPacket << stStorage;
    xSendPacket << stBroach;
    xSendPacket << stSocket;
    xSendPacket << stPackage;
    xSendPacket.XParse << m_nInventoryCount;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

void CLeague::SendInventoryMove(std::uint32_t dwActorID, PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME stMove) {
    // 对齐 IDA 0x140069900: 发送联赛仓库物品移动广播包 (0xF6, 0x61)
    stMove.nInventorySync = m_nInventoryCount;
    XSendPacket xSendPacket(0xF6, 0x61);
    xSendPacket.XParse << dwActorID;
    xSendPacket << stMove;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

void CLeague::SendChangeLeagueName(PS_LEAGUE_NAME_CHANGE_SERVER stChange) {
    // 对齐 IDA 0x140068360
    // 创建可修改的本地副本
    PS_LEAGUE_NAME_CHANGE_SERVER stChangeInfo = stChange;
    stChangeInfo.nSysnCount = m_nSyncCount;  // 设置同步计数

    XSendPacket xSendPacket(0xF6, 0x25);
    xSendPacket << stChangeInfo;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

void CLeague::SendWithdrawToMember(std::int32_t nLeagueID, std::uint32_t dwUCID, std::int64_t biPenalty, ST_LEAGUE_INFO_UPDATE& stUpdate) {
    // 对齐 IDA 0x140068420
    XSendPacket xSendPacket(0xF6, 8);
    xSendPacket.XParse << nLeagueID;
    xSendPacket.XParse << dwUCID;
    xSendPacket.XParse << biPenalty;
    xSendPacket << stUpdate;
    xSendPacket.XParse << m_nSyncCount;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

void CLeague::SendKickoutToMember(std::int32_t nLeagueID, std::int32_t nErrorCode, std::uint32_t dwReqUCID, std::uint32_t dwTargetUCID, ST_LEAGUE_INFO_UPDATE& stUpdate, std::int16_t shLevel) {
    // 对齐 IDA 0x140068510
    XSendPacket xSendPacket(0xF6, 9);
    xSendPacket.XParse << nErrorCode;
    xSendPacket.XParse << nLeagueID;
    xSendPacket.XParse << dwReqUCID;
    xSendPacket.XParse << dwTargetUCID;
    xSendPacket << stUpdate;
    xSendPacket.XParse << shLevel;
    xSendPacket.XParse << m_nSyncCount;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

void CLeague::SendChangePositionToMember(ST_LEAGUE_MEMBER_POSITION& stPos, std::uint32_t dwReqUCID, std::uint8_t byPrevPosition, std::int32_t nLeagueID, ST_LEAGUE_INFO_FOR_GAME stInfo) {  // 对齐 IDA: 最后参数按值传递
    // 对齐 IDA 0x140068630
    XSendPacket xSendPacket(0xF6, 0x37);
    xSendPacket << stPos;
    xSendPacket.XParse << nLeagueID;
    xSendPacket.XParse << dwReqUCID;
    xSendPacket.XParse << byPrevPosition;
    xSendPacket << stInfo;
    xSendPacket.XParse << m_nSyncCount;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

void CLeague::SendDelegateToMember(PS_RES_LEAGUE_DELEGATE& stRes, std::uint32_t dwReqActorID, std::uint32_t dwTargetActorID) {
    // 对齐 IDA 0x140068730
    XSendPacket xSendPacket(0xF6, 7);
    xSendPacket << stRes;
    xSendPacket.XParse << dwReqActorID;
    xSendPacket.XParse << dwTargetActorID;
    xSendPacket.XParse << m_nSyncCount;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

void CLeague::SendNoticeToMember(ST_LEAGUE_NOTICE& stNotice, std::uint32_t dwActorID) {
    // 对齐 IDA 0x140068810
    XSendPacket xSendPacket(0xF6, 0x23);
    xSendPacket << stNotice;
    xSendPacket.XParse << dwActorID;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

void CLeague::SendRecordToMember(ST_LEAGUE_RECORD& stRecord) {
    // 对齐 IDA 0x140068c40: 广播记录到所有成员 (0xF6, 0x47)
    XSendPacket xSendPacket(0xF6, 0x47);
    xSendPacket << stRecord;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

void CLeague::SendChangeCardToMember(std::uint32_t dwActorID, PS_REQ_LEAGUE_CARD& stCard) {
    // 对齐 IDA 0x140068cd0
    XSendPacket xSendPacket(0xF6, 0x26);
    xSendPacket << stCard;
    xSendPacket.XParse << m_nSyncCount;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

void CLeague::SendLeagueWealthToMember(std::uint32_t dwActorID, ST_LEAGUE_INFO_UPDATE stUpdate) {
    // 对齐 IDA 0x140068d80
    XSendPacket xSendPacket(0xF6, 0x55);
    xSendPacket.XParse << dwActorID;
    xSendPacket << stUpdate;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

void CLeague::SendLevelupToMember(std::uint8_t byLevel, std::uint8_t bySkillPoint, PS_AUTO_SKILL stSkill) {
    // 对齐 IDA 0x140068e30
    XSendPacket xSendPacket(0xF6, 0x51);
    xSendPacket.XParse << m_stLeagueInfo.nLeagueID;
    xSendPacket.XParse << byLevel;
    xSendPacket.XParse << static_cast<std::int8_t>(bySkillPoint);
    xSendPacket << stSkill;
    xSendPacket.XParse << m_nSyncCount;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

void CLeague::SendLearnSkillToMember(PS_RES_LEAGUE_SKILL stSkill) {
    // 对齐 IDA 0x140068f30
    XSendPacket xSendPacket(0xF6, 0x53);
    xSendPacket << stSkill;
    xSendPacket.XParse << m_nSyncCount;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

void CLeague::SendChangeApplicantName(ST_LEAGUE_APPLICANT stApplicant) {
    // 对齐 IDA 0x140068240
    XSendPacket xSendPacket(0xF6, 0x20);
    xSendPacket << stApplicant;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

void CLeague::SendChangeMemberName(ST_LEAGUE_MEMBER_UPDATE& stUpdate) {
    // 对齐 IDA 0x1400682d0
    XSendPacket xSendPacket(0xF6, 0x42);
    xSendPacket << stUpdate;
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(xSendPacket);
}

void CLeague::Levelup_Cheat(std::uint8_t byType, std::uint32_t dwParam) {
    // 对齐 IDA 0x140068fe0: GM等级修改
    // 参数: byType=byAddLevel(新等级), dwParam=dwUCID(目标成员)
    bool bSkillPointCheat = false;

    // 检查等级上限 (<=10)
    if (byType > 10) {
        return;
    }

    // 查表验证
    auto& resourceMgr = TXSingleton<XRelayServer>::Instance()->GetResourceMgr();
    auto pTB_LeagueInfo = resourceMgr.GetTB_LEAGUE_INFO(byType);
    if (!pTB_LeagueInfo) {
        LogHelper::LogError("game.relay", "Levelup error - No Table TB_LEAGUE_INFO( %d )", 1237);
        return;
    }

    std::uint8_t byLevel = byType;
    std::int8_t bySkillPoint = m_stLeagueInfo.bySkillPoint;
    std::uint8_t byGap = byType - m_stLeagueInfo.byRating;

    // 计算新增技能点
    if (byGap > 0) {
        std::uint8_t byCurLevel = m_stLeagueInfo.byRating;
        for (int i = 0; i < byGap; ++i) {
            std::uint32_t dwNextLevel = byCurLevel + 1;
            auto pNextLevelInfo = resourceMgr.GetTB_LEAGUE_INFO(dwNextLevel);
            if (pNextLevelInfo) {
                bySkillPoint += static_cast<std::int8_t>(pNextLevelInfo->League_Skill_Get_Point);
                ++byCurLevel;
            }
        }
    }

    if (byLevel == 0) {
        byLevel = 1;
    }

    // 复制当前技能
    PS_AUTO_SKILL psAutoSkill{};
    for (int j = 0; j < 8; ++j) {
        psAutoSkill.bySkillInfo[j] = m_stLeagueInfo.bySkill[j];
    }

    // 自动学习技能 (League_Skill_Type==0 && League_Get_Skill_Level<=byLevel)
    const auto& mapLeagueSkill = resourceMgr.GetTB_LEAGUE_SKILL_Map();
    for (auto it = mapLeagueSkill.begin(); it != mapLeagueSkill.end(); ++it) {
        const TB_LEAGUE_SKILL& rSkill = it->second;
        if (rSkill.League_Skill_Type == 0 && rSkill.League_Get_Skill_Level <= byLevel) {
            std::int32_t nSkillGroupID = static_cast<std::int32_t>(rSkill.League_Skill_Group) - 1;
            if (nSkillGroupID >= 0 && nSkillGroupID < 8) {
                psAutoSkill.bySkillInfo[nSkillGroupID] = static_cast<std::uint8_t>(rSkill.League_Skill_Level);
            }
        }
    }

    // 发送 DB 更新 (main=7, sub=0x34)
    XSendDBPacket xSendDBPacket(nullptr, 7, 0x34);
    xSendDBPacket.XParse << m_stLeagueInfo.nLeagueID;
    xSendDBPacket.XParse << byLevel;
    xSendDBPacket.XParse << bySkillPoint;
    xSendDBPacket << psAutoSkill;
    xSendDBPacket.XParse << bSkillPointCheat;
    xSendDBPacket.XParse << dwParam;
    TXSingleton<XRelayServer>::Instance()->SendDBGame(xSendDBPacket);
}
