#pragma once

#include <cstdint>
#include <deque>
#include <map>
#include <memory>

#include "Soulworker/GameServer/XRelayServer/LeagueManager.h"

// 前向声明
class CServer;

/**
 * @brief 联赛类
 *
 * 管理单个联赛的信息、成员、申请者、公告板等。
 * 总大小 0x8A8 字节。
 */
class CLeague {
public:
    CLeague();
    ~CLeague();

    // 禁止拷贝
    CLeague(const CLeague&) = delete;
    CLeague& operator=(const CLeague&) = delete;

    // 联赛信息
    void SetLeagueInfo(const ST_LEAGUE_INFO& stInfo);
    void GetLeagueInfo(ST_LEAGUE_INFO& stInfo);
    void Clear();

    // 成员管理
    void AddMember(const ST_LEAGUE_MEMBER_EX& stMember);
    void DelMember(std::uint32_t dwUCID);
    bool LoginMember(std::uint32_t dwUCID);
    void LogOutMember(std::uint32_t dwUCID, std::int64_t biPenalty);
    bool IsMember(std::uint32_t dwUCID);
    bool IsMaster(std::uint32_t dwUCID);
    std::shared_ptr<class CLeagueMember> GetLeagueMemberPtr(std::uint32_t dwUCID);
    void GetMemberList(ST_LEAGUE_MEMBER_LIST& stList);
    void SetMemberCount(std::int16_t nCount);
    std::int32_t GetMemberCount();
    void LeagueMemberUpdate(const ST_LEAGUE_MEMBER_EX& stMember, ST_LEAGUE_MEMBER_UPDATE& stUpdate);
    void SetMemberBoardLimit(std::uint32_t dwUCID, std::int64_t biLimitTime);

    // 职位管理
    void SetLeaguePosition(ST_LEAGUE_POSITION_NAME_CHANGE& stChange);
    void ChangeMemberPosition(ST_LEAGUE_MEMBER_POSITION& stPos, std::uint32_t dwActorID);
    bool CheckPositionCount(std::uint8_t byPosition);
    void SetSubLeagueMaster(wchar_t* szName);
    void SetMasterName(wchar_t* szName);
    void SetLeagueName(wchar_t* szName);

    // 申请者管理
    void AddApplicant(const ST_LEAGUE_APPLICANT& stApplicant);
    bool DelApplicant(std::uint32_t dwUCID);
    bool CheckExistApplicant(std::uint32_t dwUCID);
    bool CheckApplicantCount(std::int32_t nCount);
    void GetApplicantList(ST_LEAGUE_APPLICANT_LIST& stList);
    wchar_t* GetApplicant(std::uint32_t dwUCID);

    // 公告板管理
    void AddBoard(const ST_LEAGUE_BOARD& stBoard);
    void GetBoardList(ST_LEAGUE_BOARD_LIST& stList);

    // 记录管理
    void LoadRecord(ST_LEAGUE_RECORD& stRecord);
    void UpdateRecord(ST_LEAGUE_RECORD& stRecord);
    void GetRecordList(ST_LEAGUE_RECORD_LIST& stList);

    // 公告管理
    void SetLeagueNotice(const ST_LEAGUE_NOTICE& stNotice);
    void SetLeagueRecruitNotice(ST_LEAGUE_RECRUIT_NOTICE& stNotice);
    void SetLeagueNoticeDate();
    void SetLeagueRecruitNoticeDate(std::int64_t biDate);
    void GetNoticeDate(std::int64_t& biDate);
    std::int64_t GetRecruitNoticeDate();

    // 权限管理
    void SetLeagueAuth(std::int32_t* pnAuth);
    void SetLeagueOpenOrNot(bool bOpen);

    // 名称变更
    void ChangeMemberName(const PS_CHANGE_NAME& stChange);
    void UpdateApplicantName(const PS_CHANGE_NAME& stChange);
    void GetDelName(std::uint32_t dwUCID, wchar_t* szName);

    // 转让
    std::int32_t CheckLeagueDelegate(std::uint32_t dwReqActorID, std::uint32_t dwTargetActorID);
    bool Delegate(PS_REQ_LEAGUE_DELEGATE& stReq, PS_RES_LEAGUE_DELEGATE& stRes, std::uint32_t dwActorID);

    // 卡片变更
    std::int32_t CheckLeagueCardChange(std::uint32_t dwActorID);
    void CardChange(PS_REQ_LEAGUE_CARD& stCard, std::uint32_t dwActorID);

    // 技能和等级
    bool HaveSkill(enum E_LEAGUE_SKILL eSkill);
    std::int32_t CheckLearnSkill(const PS_REQ_LEAGUE_SKILL& stSkill, PS_RES_LEAGUE_SKILL& stRes);
    void LearnSkill(const PS_RES_LEAGUE_SKILL& stSkill);
    void Levelup(std::uint8_t byAddLevel, std::uint32_t dwUCID);
    void ApplyLevelup(std::uint8_t byLevel, std::uint8_t bySkillPoint, const PS_AUTO_SKILL& stSkill, std::uint32_t dwUCID);

    // 经验和财富
    void CalculateExp(const PS_LEAGUE_WEALTH_FOR_SERVER& stWealth);
    void ApplyWealth(const PS_LEAGUE_WEALTH_FOR_SERVER& stWealth);
    void ResetExp(std::int64_t biExp);
    void ResetExpInitDate(std::uint32_t dwParam);

    // 更新
    void UpdateApplyList(std::int64_t tNow);
    void UpDateLeagueMemberInfo();
    void UpdateSyncCount();
    void UpdateInventorySyncCount();
    void SetLeagueInfoForGame(std::uint8_t byPosition, ST_LEAGUE_INFO_FOR_GAME& stInfo);
    void UpdateSkillPoint_Cheat(std::uint8_t byType, std::uint32_t dwParam);

    // 同步计数
    bool CompareSyncCount(std::int32_t nCount);
    std::int32_t GetSyncCount();

    // 联赛ID获取
    std::int32_t GetLeagueID() const { return m_stLeagueInfo.nLeagueID; }

    // 发送消息
    void SendLeagueInfo(std::uint32_t dwUCID);
    void SendSyncLeagueInfo(const PS_SYNC_LEAGUE_INFO& stSync);
    void SendInventoryInfo(std::uint32_t dwActorID, const PS_RES_STORAGE_INFO& stStorage, const PS_ITEM_BROACH_LIST& stBroach, const PS_ITEM_SOCKET_LIST& stSocket, const PS_ITEM_PACKAGE_LIST& stPackage);
    void SendInventoryMove(std::uint32_t dwActorID, const PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME& stMove);
    void SendChangeLeagueName(const PS_LEAGUE_NAME_CHANGE_SERVER& stChange);
    void SendWithdrawToMember(std::int32_t nLeagueID, std::uint32_t dwUCID, std::int64_t biPenalty, ST_LEAGUE_INFO_UPDATE& stUpdate);
    void SendKickoutToMember(std::int32_t nLeagueID, std::int32_t nErrorCode, std::uint32_t dwReqUCID, std::uint32_t dwTargetUCID, ST_LEAGUE_INFO_UPDATE& stUpdate, std::int16_t shLevel);
    void SendChangePositionToMember(ST_LEAGUE_MEMBER_POSITION& stPos, std::uint32_t dwReqUCID, std::uint8_t byPrevPosition, std::int32_t nLeagueID, ST_LEAGUE_INFO_FOR_GAME& stInfo);
    void SendDelegateToMember(PS_RES_LEAGUE_DELEGATE& stRes, std::uint32_t dwReqActorID, std::uint32_t dwTargetActorID);
    void SendNoticeToMember(ST_LEAGUE_NOTICE& stNotice, std::uint32_t dwActorID);
    void SendRecordToMember(ST_LEAGUE_RECORD& stRecord);
    void SendChangeCardToMember(std::uint32_t dwActorID, PS_REQ_LEAGUE_CARD& stCard);
    void SendLeagueWealthToMember(std::uint32_t dwActorID, const ST_LEAGUE_INFO_UPDATE& stUpdate);
    void SendLevelupToMember(std::uint8_t byLevel, std::uint8_t bySkillPoint, const PS_AUTO_SKILL& stSkill);
    void SendLearnSkillToMember(const PS_RES_LEAGUE_SKILL& stSkill);
    void SendChangeApplicantName(const ST_LEAGUE_APPLICANT& stApplicant);
    void SendChangeMemberName(ST_LEAGUE_MEMBER_UPDATE& stUpdate);
    void Levelup_Cheat(std::uint8_t byType, std::uint32_t dwParam);

private:
    // 成员变量 (0x8A8 bytes total)
    ST_LEAGUE_INFO m_stLeagueInfo{};                                        // 联赛信息
    std::map<std::uint32_t, std::shared_ptr<CLeagueMember>> m_mpLeagueMember; // 成员map (UCID -> CLeagueMember)
    std::map<std::uint32_t, ST_LEAGUE_APPLICANT> m_mpApplicant;            // 申请者map (UCID -> ST_LEAGUE_APPLICANT)
    std::deque<ST_LEAGUE_BOARD> m_deqBoard;                                // 公告板deque
    std::deque<ST_LEAGUE_RECORD> m_deqRecord;                              // 记录deque
    // NOTE: m_szMasterName/m_szSubMasterName 已移除，改用 m_stLeagueInfo.szMasterName/szSubMasterName
    wchar_t m_szPositionName[9][21] = {};                                   // 职位名称
    ST_LEAGUE_NOTICE m_stNotice{};                                          // 公告
    ST_LEAGUE_RECRUIT_NOTICE m_stRecruitNotice{};                          // 招募公告
    std::int64_t m_biNoticeDate = 0;                                        // 公告日期
    std::int64_t m_biRecruitNoticeDate = 0;                                 // 招募公告日期
    std::int32_t m_nSyncCount = 0;                                          // 同步计数
    std::int32_t m_nInventorySyncCount = 0;                                 // 仓库同步计数
    std::int32_t m_nSkillPoint[2] = {};                                     // 技能点
    ST_LEAGUE_INFO_FOR_GAME m_stInfoForGame{};                              // 游戏端联赛信息
};
