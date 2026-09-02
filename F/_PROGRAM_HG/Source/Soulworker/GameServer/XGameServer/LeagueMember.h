// LeagueMember.h
// CLeagueMember - GameServer 侧 League 成员管理类
// 对齐 IDA GameServer.exe；原始归属 XGameServer/LeagueMember.obj
// (PDB cvdump modules 0042: "F:\_PROGRAM_HG\Bin\Server_Release\x64\XGameServer\LeagueMember.obj")
//
// 说明: 该类管理同一 League 在本逻辑线程内的在线成员表，负责广播、
// 同步计数与状态更新。此前定义被临时聚合在 XGameServer/GocLeague.h
// 历史遗留文件中；按 PDB OBJ 归属收敛到本文件。
// 注意: 与 XRelayServer/LeagueMember.h 的同名类是不同语义的 Relay 侧单成员
// 信息类，互不兼容。

#pragma once

#include <map>

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerLeague.h"
// ST_LEAGUE_INFO / ST_LEAGUE_INFO_UPDATE / ST_LEAGUE_LIST 定义在 LeagueManager.h
#include "Soulworker/GameServer/XRelayServer/LeagueManager.h"

// 前置声明
class CUser;
class XSendPacket;
struct PS_CHAT_LEAGUE;
struct PS_CHAT_ITEM_LINK_FOR_SERVER;
struct ST_LEAGUE_AUTH_CHANGE;
struct ST_LEAGUE_POSITION_NAME_CHANGE;
struct ST_LEAGUE_MEMBER_POSITION;
struct ST_LEAGUE_RECORD;
struct PS_RES_LEAGUE_SKILL;
struct PS_RES_LEAGUE_DELEGATE;
struct PS_REQ_LEAGUE_CARD;
struct PS_RES_ITEM_MOVE_LEAGUE_INVEN;
struct PS_RES_LEAGUE_NAME_CHANGE;

// ============================================================================
// CLeagueMember - Guild Member Management
// Handles guild member operations, broadcasting, and synchronization
// ============================================================================
class CLeagueMember
{
public:
    CLeagueMember();
    ~CLeagueMember();

    // Member management
    void Clear();
    void AddLeagueMember(CUser* pUser);
    void DeleteLeagueMember(unsigned int dwActorID);
    void WithDrawLeagueMember(unsigned int dwUCID, ST_LEAGUE_INFO_UPDATE stUpdateInfo);
    void JoinLeagueUser(ST_LEAGUE_MEMBER_EX stMemberEx, ST_LEAGUE_INFO_UPDATE stInfoUpdate, unsigned char byApplyState);

    // Broadcasting functions
    void SendLeagueInfo(ST_LEAGUE_INFO stInfo);
    void SendLeagueBoard(ST_LEAGUE_BOARD stBoard, unsigned int dwWriterUCID);
    void SendLeagueNotice(ST_LEAGUE_NOTICE stNotice, unsigned int dwWriterUCID);
    void SendLeagueApply(ST_LEAGUE_APPLICANT stApplicant);
    void SendLeagueMsg(PS_CHAT_LEAGUE psChat, PS_CHAT_ITEM_LINK_FOR_SERVER psItemLink);
    void SendLeagueRecruitNotice(ST_LEAGUE_RECRUIT_NOTICE stNotice, __int64 nDate);
    void SendSyncLeagueInfo(CUser* pUser);

    // Guild operations
    void LoginLeagueMember(ST_LEAGUE_MEMBER_UPDATE stUpdate);
    void KickoutLeagueMember(int dwReqUCID, unsigned int dwKickoutUCID, ST_LEAGUE_INFO_UPDATE stUpdateInfo);
    void LeagueApplicantDelete(unsigned int dwUCID);
    void ChangeLeagueAuth(ST_LEAGUE_AUTH_CHANGE stAuthChange);
    void ChangeLeaguePositionName(ST_LEAGUE_POSITION_NAME_CHANGE stPosNameChange);
    void ChangeLeagueMemberPosition(ST_LEAGUE_MEMBER_POSITION stMemberPos, unsigned int dwUCID);
    void UpdateLeagueMember(ST_LEAGUE_MEMBER_UPDATE stUpdate);
    void UpdateApplicantList(unsigned int dwUCID);

    // Guild progression
    void Record(ST_LEAGUE_RECORD stRecord);
    void Wealth(ST_LEAGUE_INFO_UPDATE stUpdateInfo);
    void Levelup(int nLeagueID, unsigned char byLevel, char bySkillPoint, PS_AUTO_SKILL psAutoSkillInfo);
    void SkillLearn(PS_RES_LEAGUE_SKILL psSkill);
    void Delegate(PS_RES_LEAGUE_DELEGATE psDelegate, unsigned int dwUCID);
    void CardChange(PS_REQ_LEAGUE_CARD psCard);
    void InventoryMove(unsigned int dwUCID, PS_RES_ITEM_MOVE_LEAGUE_INVEN psMove);
    void ChangeName(PS_RES_LEAGUE_NAME_CHANGE psNameChange);

    // Sync operations
    void UpdateSyncCount(int nCount);
    void UpdateInventorySyncCount(int nCount);
    void CompareSyncCount(unsigned int dwUCID);
    bool CompareInventorySyncCount(unsigned int dwUCID);

private:
    // PDB UDT 0xad5b (40 bytes): m_mapUser(0x00, std::map<unsigned long,CUser*>)
    // + m_nSyncCount(0x20) + m_nInventorySyncCount(0x24)
    std::map<unsigned int, CUser*> m_mapUser;
    int m_nSyncCount = 0;
    int m_nInventorySyncCount = 0;
};
