#pragma once

// Guild/League System Component Header
// Decompiled from IDA GameServer.exe
// Target: GameServer.exe
// Address range: 0x1400FA870 - 0x1402AC5A0

#include "GOComponent.h"
#include "..\XSCommon\STDefine.h"
#include <string>
#include <map>

// Forward declarations
class CUser;
class CLeagueMember;

// League/Guild structures
struct ST_LEAGUE_INFO
{
    int nLeagueID = 0;
    char szLeagueName[64] = {};
    int nLevel = 0;
    int nExp = 0;
    int nMaxMember = 0;
    int nGold = 0;
    __int64 nCreateDate = 0;
    unsigned char byOpen = 0;
    unsigned char byRecruit = 0;
    char szRecruitNotice[256] = {};
};

struct ST_LEAGUE_MEMBER_EX
{
    int nLeagueID = 0;
    unsigned int dwUCID = 0;
    char szName[64] = {};
    unsigned char byPosition = 0;
    unsigned char byLevel = 0;
    int nContribution = 0;
    __int64 nJoinDate = 0;
    unsigned char byAuth = 0;
};

struct ST_LEAGUE_INFO_UPDATE
{
    int nLeagueID = 0;
    int nExp = 0;
    int nGold = 0;
    int nLevel = 0;
};

struct PS_LEAGUE_WEALTH_FOR_SERVER
{
    unsigned int dwUCID = 0;
    int nLeagueID = 0;
    short shExp = 0;
    int nGold = 0;
};

struct PS_AUTO_SKILL
{
    unsigned char bySkillInfo[8] = {};
};

// CGocLeague - Guild Component
// Manages guild membership and basic guild operations
class CGocLeague : public GOComponent
{
public:
    // Constructor & Destructor
    CGocLeague();
    virtual ~CGocLeague();

    // Component lifecycle
    virtual void Init() override;
    
    // Getters
    static int GetFamilyID();
    
private:
    CLeagueMember* m_stLeagueMember = nullptr;
    unsigned int m_dwInviteActorID = 0;
};

// CLeagueMember - Guild Member Management
// Handles guild member operations, broadcasting, and synchronization
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
    void SendLeagueBoard(struct ST_LEAGUE_BOARD stBoard, unsigned int dwWriterUCID);
    void SendLeagueNotice(struct ST_LEAGUE_NOTICE stNotice, unsigned int dwWriterUCID);
    void SendLeagueApply(struct ST_LEAGUE_APPLICANT stApplicant);
    void SendLeagueMsg(struct PS_CHAT_LEAGUE psChat, struct PS_CHAT_ITEM_LINK_FOR_SERVER psItemLink);
    void SendLeagueRecruitNotice(struct ST_LEAGUE_RECRUIT_NOTICE stNotice, __int64 nDate);
    void SendSyncLeagueInfo(CUser* pUser);
    
    // Guild operations
    void LoginLeagueMember(struct ST_LEAGUE_MEMBER_UPDATE stUpdate);
    void KickoutLeagueMember(int dwReqUCID, unsigned int dwKickoutUCID, ST_LEAGUE_INFO_UPDATE stUpdateInfo);
    void LeagueApplicantDelete(unsigned int dwUCID);
    void ChangeLeagueAuth(struct ST_LEAGUE_AUTH_CHANGE stAuthChange);
    void ChangeLeaguePositionName(struct ST_LEAGUE_POSITION_NAME_CHANGE stPosNameChange);
    void ChangeLeagueMemberPosition(struct ST_LEAGUE_MEMBER_POSITION stMemberPos, unsigned int dwUCID);
    void UpdateLeagueMember(struct ST_LEAGUE_MEMBER_UPDATE stUpdate);
    void UpdateApplicantList(unsigned int dwUCID);
    
    // Guild progression
    void Record(struct ST_LEAGUE_RECORD stRecord);
    void Wealth(ST_LEAGUE_INFO_UPDATE stUpdateInfo);
    void Levelup(int nLeagueID, unsigned char byLevel, char bySkillPoint, PS_AUTO_SKILL psAutoSkillInfo);
    void SkillLearn(struct PS_RES_LEAGUE_SKILL psSkill);
    void Delegate(struct PS_RES_LEAGUE_DELEGATE psDelegate, unsigned int dwUCID);
    void CardChange(struct PS_REQ_LEAGUE_CARD psCard);
    void InventoryMove(unsigned int dwUCID, struct PS_RES_ITEM_MOVE_LEAGUE_INVEN psMove);
    void ChangeName(struct PS_RES_LEAGUE_NAME_CHANGE psNameChange);
    
    // Sync operations
    void UpdateSyncCount(int nCount);
    void UpdateInventorySyncCount(int nCount);
    bool CompareSyncCount(unsigned int dwUCID);
    bool CompareInventorySyncCount(unsigned int dwUCID);
    
private:
    std::map<unsigned int, CUser*> m_mapMembers;
    int m_nSyncCount = 0;
    int m_nInventorySyncCount = 0;
};

// CLeagueProcess - Guild Request Processing
// Handles client requests for guild operations
class CLeagueProcess : public TXProcess<CUser>
{
public:
    CLeagueProcess();
    virtual ~CLeagueProcess();
    
    virtual bool Parse(XPacket& xPacket) override;
    
    // Guild creation and deletion
    bool ReqLeagueCreate(XPacket& xPacket);
    bool ReqLeagueDelete(XPacket& xPacket);
    bool ReqLeagueOverlapName(XPacket& xPacket);
    bool ResLeagueOverlapName(XPacket& xPacket);
    
    // Guild search and listing
    bool ReqLeagueSearch(XPacket& xPacket);
    bool ReqLeagueList(XPacket& xPacket);
    bool ReqLeagueInfo(XPacket& xPacket);
    
    // Membership operations
    bool ReqLeagueApplicant(XPacket& xPacket);
    bool ReqLeagueApplicantAccept(XPacket& xPacket);
    bool ReqLeagueApplicantReject(XPacket& xPacket);
    bool ReqLeagueInvite(XPacket& xPacket);
    bool ReqLeagueInviteAccept(XPacket& xPacket);
    bool ReqLeagueInviteReject(XPacket& xPacket);
    bool ReqLeagueKick(XPacket& xPacket);
    bool ReqLeagueWithDraw(XPacket& xPacket);
    bool ReqLeagueDelegate(XPacket& xPacket);
    
    // Guild management
    bool ReqLeagueBoard(XPacket& xPacket);
    bool ReqLeagueNoticeChange(XPacket& xPacket);
    bool ReqLeagueAuthChange(XPacket& xPacket);
    bool ReqLeaguePositionNameChange(XPacket& xPacket);
    bool ReqLeagueMemberPositionChange(XPacket& xPacket);
    bool ReqLeagueOpenOrNot(XPacket& xPacket);
    bool ReqLeagueRecruitNotice(XPacket& xPacket);
    bool ReqLeagueNameChange(XPacket& xPacket);
    bool ReqLeagueCardChange(XPacket& xPacket);
    
    // Guild progression
    bool ReqLeagueSkillLearn(XPacket& xPacket);
    bool ReqLeagueInventoryMove(XPacket& xPacket);
    bool ReqLeagueInventoryInfo(XPacket& xPacket);
    
    // Database response
    bool DBLeagueParse(XPacket& xPacket);
    
    // Helper functions
    int CheckNameChangeItem(struct PS_ITEM_SLOT_INFO psSlotInfo, struct PS_RES_STORAGE_INFO& psResInfo);
    void SendLeagueNameChangeMsg(int nResult, struct PS_REQ_LEAGUE_NAME_CHANGE psNameChange);
    // PDB: ?CheckNpc@CLeagueProcess@@QEAA_NKEH@Z @ 0x1404F4380
    bool CheckNpc(unsigned int dwNpcID, unsigned char byNpcType, int nNpcFunID);
    // PDB: ?CheckLeagueInventoryIn@CLeagueProcess@@QEAAHUPS_REQ_ITEM_MOVE_LEAGUE_INVEN@@AEAUSTItem@@AEAUPS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME@@@Z @ 0x1404F6C40
    int CheckLeagueInventoryIn(struct PS_REQ_ITEM_MOVE_LEAGUE_INVEN psReq, struct STItem& stItem, struct PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME& psMove);
    // PDB: ?CheckLeagueInventoryOut@CLeagueProcess@@QEAAHUPS_REQ_ITEM_MOVE_LEAGUE_INVEN@@@Z @ 0x1404F72A0
    int CheckLeagueInventoryOut(struct PS_REQ_ITEM_MOVE_LEAGUE_INVEN psReq);
    // PDB: ?CheckLeagueInventoryInMove@CLeagueProcess@@QEAAHUPS_REQ_ITEM_MOVE_LEAGUE_INVEN@@@Z @ 0x1404F75E0
    int CheckLeagueInventoryInMove(struct PS_REQ_ITEM_MOVE_LEAGUE_INVEN psReq);
    
private:
    // Internal data
    char m_szName[256] = {};
};
