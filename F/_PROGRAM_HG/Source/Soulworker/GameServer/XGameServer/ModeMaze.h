// ModeMaze.h - XModeMaze class declaration
// Restored from GameServer.exe IDA decompilation

#pragma once

#include "Soulworker/GameServer/XGameServer/Maze.h"
#include <map>
#include <vector>
#include <string>
#include <cstdint>

// Forward declarations
struct TB_OPERATION_INFO;
struct PS_UPDATE_MAZE_ENTER_LIMIT_COUNT;
struct ST_MAZE_WAIT_ENTER_USER_INFO;
struct ST_ENTER_MAZE_MEMBER_INFO;
struct ST_CREATE_MAZE;
struct STMagePotalBox;
struct CWarpPotal;
struct STPosInfo;

// ST_MODE_MAZE_USER_SCORE - User score for mode maze ranking
struct ST_MODE_MAZE_USER_SCORE {
    unsigned long dwUserID;
    unsigned long dwScore;
    unsigned short wRank;
    bool bLeave;
    std::wstring strName;  // User name
    int nReviveCount;

    ST_MODE_MAZE_USER_SCORE()
        : dwUserID(0)
        , dwScore(0)
        , wRank(0)
        , bLeave(false)
        , nReviveCount(0)
    {}
};

// ST_MODE_MAZE_USER_REWARD - User reward info for operation mode
struct ST_MODE_MAZE_USER_REWARD {
    unsigned long dwUCID;
    int nAttachPoint;
    unsigned short wRank;
    std::wstring strName;
    int nRewardItem;
    int nRewardCount;
    bool bLeave;
    unsigned char byLevel;
    unsigned char byClass;
    unsigned char byAwaken;
    unsigned long dwProfilePhotoID;

    ST_MODE_MAZE_USER_REWARD()
        : dwUCID(0)
        , nAttachPoint(0)
        , wRank(0)
        , nRewardItem(0)
        , nRewardCount(0)
        , bLeave(false)
        , byLevel(0)
        , byClass(0)
        , byAwaken(0)
        , dwProfilePhotoID(0)
    {}
};

// PS_MODE_MAZE_REWARD_INFO - Reward info packet for dimension shutter
struct PS_MODE_MAZE_REWARD_INFO {
    bool bHotTime;
    std::vector<ST_MODE_MAZE_USER_REWARD> vecUser;

    PS_MODE_MAZE_REWARD_INFO()
        : bHotTime(false)
    {}
};

// PS_WORLD_WARP_INFO - Warp portal info packet
struct PS_WORLD_WARP_INFO {
    int nBoxIndex;
    bool bFlag;
    bool bMazeComplete;
    int nBuffID;

    PS_WORLD_WARP_INFO()
        : nBoxIndex(0)
        , bFlag(false)
        , bMazeComplete(false)
        , nBuffID(0)
    {}
};

// ST_MODE_MAZE_USER_SCORE_INFO - User score info sub-structure (from IDA)
struct ST_MODE_MAZE_USER_SCORE_INFO {
    unsigned long dwUAID;
    unsigned long dwUCID;
    wchar_t strName[21];
    int nPoint;
    bool bLeave;
    unsigned short wRank;

    ST_MODE_MAZE_USER_SCORE_INFO()
        : dwUAID(0)
        , dwUCID(0)
        , nPoint(0)
        , bLeave(true)
        , wRank(0)
    {
        memset(strName, 0, sizeof(strName));
    }
};

// ST_SERVER_MODE_MAZE_USER_SCORE - User score for mode maze (internal tracking)
// IDA aligned: contains stInfo sub-structure and additional fields
struct ST_SERVER_MODE_MAZE_USER_SCORE {
    ST_MODE_MAZE_USER_SCORE_INFO stInfo;  // User info sub-structure
    unsigned long long dw64ReviveTick;    // Revive time for dimension shutter
    int nReviveCount;                     // Revive count
    unsigned short wReturnMapID;          // Return map ID

    ST_SERVER_MODE_MAZE_USER_SCORE()
        : dw64ReviveTick(0)
        , nReviveCount(0)
        , wReturnMapID(0)
    {}
};

// ============================================================================
// XModeMaze - Mode Maze class for special maze modes (Operation, etc.)
// IDA: Inherits from XMaze
//
// Function: Manages special mode maze instances including:
// - Dimension score tracking
// - Operation mode specific logic
// - Enter limit management
// ============================================================================
class XModeMaze : public XMaze {
public:
    // === Constructor/Destructor ===
    // IDA: ??0XModeMaze@@QEAA@XZ (0x14028D540)
    XModeMaze();

    // IDA: ??1XModeMaze@@UEAA@XZ (0x14028D680)
    virtual ~XModeMaze();

    // === Initialization ===
    // IDA: ?Init@XModeMaze@@UEAA_NXZ (0x14028D700)
    virtual bool Init();

    // IDA: ?Clear@XModeMaze@@UEAAXXZ (0x14028D9A0)
    virtual void Clear();

    // === Creation ===
    // IDA: ?Create@XModeMaze@@UEAA_NAEAUST_CREATE_MAZE@@@Z (0x14028DA40)
    virtual bool Create(ST_CREATE_MAZE& stCreateMaze);

    // IDA: ?Create@XModeMaze@@QEAA_NAEAUST_CREATE_MODE_MAZE@@@Z (0x14028DCA0)
    bool CreateModeMaze(struct ST_CREATE_MODE_MAZE& stCreateModeMaze);

    // === Object Info ===
    // IDA: ?SendObjectInfo@XModeMaze@@UEAA_NPEAVXActor@@_N@Z (0x14028E2A0)
    virtual bool SendObjectInfo(XActor* pActor, bool bSend);

    // === Update ===
    // IDA: ?OnUpdate@XModeMaze@@UEAAXM@Z (0x14028E7A0)
    virtual void OnUpdate(float fElapsed);

    // === Load Complete ===
    // IDA: ?LoadComplete@XModeMaze@@UEAAXPEAVXActor@@@Z (0x14028E8E0)
    virtual void LoadComplete(XActor* pActor);

    // IDA: ?LoadCompleteEX@XModeMaze@@UEAAXPEAVXActor@@@Z (0x14028E910)
    virtual void LoadCompleteEX(XActor* pActor);

    // === Game Start ===
    // IDA: ?SendLoadEx_GameStart@XModeMaze@@UEAAXXZ (0x14028EBC0)
    virtual void SendLoadEx_GameStart();

    // === Position ===
    // IDA: ?SetPosToParty@XModeMaze@@UEAA_NPEAVCUser@@@Z (0x14028ED20)
    virtual bool SetPosToParty(CUser* pUser);

    // === Enter/Exit ===
    // IDA: ?EnterGameObject@XModeMaze@@UEAAGPEAVXActor@@W4E_SEND_INFO_TYPE@IXArea@@@Z (0x14028F0C0)
    virtual std::uint16_t EnterGameObject(XActor* pActor, E_SEND_INFO_TYPE eType);

    // IDA: ?ExitGameObject@XModeMaze@@UEAAGPEAVXActor@@W4E_SEND_INFO_TYPE@IXArea@@@Z (0x14028F430)
    virtual std::uint16_t ExitGameObject(XActor* pActor, E_SEND_INFO_TYPE eType);

    // === Scanner ===
    // IDA: ?GetScanner@XModeMaze@@QEAAPEAV?$Range2DScanner@PEAVCMover@@@@PEAVXActor@@@Z (0x14028F7B0)
    void* GetScanner(XActor* pActor);  // TODO: Return proper Range2DScanner<CMover*>*

    // === Actor Enter/Exit ===
    // IDA: ?EnterActor@XModeMaze@@UEAAGPEAVXActor@@@Z (0x14028F810)
    // Note: Base class returns void, XModeMaze override returns uint16_t (different signature)
    std::uint16_t EnterActorEx(XActor* pActor);

    // IDA: ?EnterGridActor@XModeMaze@@QEAA_NPEAVXActor@@@Z (0x14028FF70)
    bool EnterGridActor(XActor* pActor);

    // IDA: ?ExitActor@XModeMaze@@UEAAGPEAVXActor@@@Z (0x1402902E0)
    // Note: Base class returns void, XModeMaze override returns uint16_t (different signature)
    std::uint16_t ExitActorEx(XActor* pActor);

    // IDA: ?ExitArea@XModeMaze@@UEAAXPEAVXActor@@@Z (0x140290EB0)
    virtual void ExitArea(XActor* pActor);

    // IDA: ?ExitGridActor@XModeMaze@@QEAA_NPEAVXActor@@@Z (0x1402911B0)
    bool ExitGridActor(XActor* pActor);

    // === Move Actor ===
    // IDA: ?MoveActor@XModeMaze@@UEAAGPEAVXActor@@AEAUXVec3@@M_N@Z (0x140291520)
    virtual std::uint16_t MoveActor(XActor* pActor, XVec3& vNextPos, float fRot, bool bSend);

    // === Process Move Object ===
    // IDA: ?ProcessMoveObject@XModeMaze@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@111@Z (0x140291A20)
    void ProcessMoveObject(XActor* pActor, std::vector<CMover*>& vecEnterPlayerList,
        std::vector<CMover*>& vecEnterObjList, std::vector<CMover*>& vecLeavePlayerList,
        std::vector<CMover*>& vecLeaveObjList);

    // IDA: ?ProcessSendLeaveObjectToOthers@XModeMaze@@QEAAXAEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@PEAVXActor@@_N@Z (0x140291B10)
    void ProcessSendLeaveObjectToOthers(std::vector<CMover*>& vecPlayerList, XActor* pLeaveActor, bool bExitActor);

    // IDA: ?ProcessSendLeaveObjectListToPlayer@XModeMaze@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z (0x140291EC0)
    void ProcessSendLeaveObjectListToPlayer(XActor* pActor, std::vector<CMover*>& vecLeaveObjList);

    // === Send Info Functions ===
    // IDA: ?ProcessSendEnterObjectListToPlayer@XModeMaze@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@1@Z (0x140292700)
    void ProcessSendEnterObjectListToPlayer(XActor* pActor, std::vector<CMover*>& vecPlayerList, std::vector<CMover*>& vecObjList);

    // IDA: ?ProcessSendTranslateInfoToPlayer@XModeMaze@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z (0x1402927B0)
    void ProcessSendTranslateInfoToPlayer(XActor* pActor, std::vector<CMover*>& vecPlayerList);

    // IDA: ?SendPcInfo@XModeMaze@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@1@Z (0x140292D40)
    void SendPcInfo(XActor* pActor, std::vector<CMover*>& vecPlayerList, std::vector<CMover*>& vecObjList);

    // IDA: ?SendNpcInfo@XModeMaze@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z (0x140293070)
    void SendNpcInfo(XActor* pActor, std::vector<CMover*>& vecObjList);

    // IDA: ?SendMonsterInfo@XModeMaze@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z (0x1402933A0)
    void SendMonsterInfo(XActor* pActor, std::vector<CMover*>& vecObjList);

    // IDA: ?SendInteractionInfo@XModeMaze@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z (0x1402937E0)
    void SendInteractionInfo(XActor* pActor, std::vector<CMover*>& vecObjList);

    // IDA: ?SendSocialInfo@XModeMaze@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z (0x140293A30)
    void SendSocialInfo(XActor* pActor, std::vector<CMover*>& vecObjList);

    // === Player Info ===
    // IDA: ?SendSameGridOtherPlayerPosition@XModeMaze@@QEAAXPEAVCUser@@@Z (0x1402929B0)
    void SendSameGridOtherPlayerPosition(CUser* pUser);

    // IDA: ?SendPlayerInfoAll@XModeMaze@@QEAAXPEAVCUser@@@Z (0x140293CC0)
    void SendPlayerInfoAll(CUser* pUser);

    // IDA: ?SendEnterPlayerInfo@XModeMaze@@QEAAXPEAVCUser@@@Z (0x140294040)
    void SendEnterPlayerInfo(CUser* pUser);

    // IDA: ?SendExitPlayerInfo@XModeMaze@@QEAAXPEAVCUser@@@Z (0x140294120)
    void SendExitPlayerInfo(CUser* pUser);

    // === PVP ===
    // IDA: ?IsEnemyPVP@XModeMaze@@UEAA_NPEAVXActor@@0@Z (0x140294200)
    virtual bool IsEnemyPVP(XActor* pAtk, XActor* pDef);

    // === Monster Quest ===
    // IDA: ?ProcessMonsterQuest@XModeMaze@@UEAAXPEAVXActor@@H@Z (0x140294230)
    virtual void ProcessMonsterQuest(XActor* pAttacker, int nMonsterID);

    // === Dimension Point ===
    // IDA: ?DropDemensionPoint@XModeMaze@@QEAAXPEAVCUser@@K@Z (0x140294610)
    void DropDemensionPoint(CUser* pUser, unsigned long dwValue);

    // === Interaction ===
    // IDA: ?ClickInteractionBox@XModeMaze@@UEAAXHPEAVCUser@@@Z (0x140294860)
    virtual void ClickInteractionBox(int nBoxID, CUser* pUser);

    // IDA: ?ClickInteractionBoxAnotherWork@XModeMaze@@QEAAXPEAUTB_INTERACTION_OBJECT@@PEAVCUser@@@Z (0x140294900)
    void ClickInteractionBoxAnotherWork(struct TB_INTERACTION_OBJECT* pTB, CUser* pUser);

    // === Reward ===
    // IDA: ?ProcessReward@XModeMaze@@UEAAXXZ (0x140294A20)
    virtual void ProcessReward();

    // IDA: ?OperationReward@XModeMaze@@QEAAXXZ (0x140294D80)
    void OperationReward();

    // === Portal ===
    // IDA: ?WarpPortal@XModeMaze@@UEAA_NPEAVCUser@@HHH@Z (0x1402957C0)
    virtual bool WarpPortal(CUser* pUser, int nMapID, int nJumpID, int nPortalID);

    // === Death ===
    // IDA: ?DiePlayer@XModeMaze@@UEAAXPEAVCUser@@@Z (0x140295E40)
    virtual void DiePlayer(CUser* pUser);

    // === Exit Box ===
    // IDA: ?GetExitBoxInfo@XModeMaze@@UEAA_NAEAGAEAH@Z (0x140295E80)
    virtual bool GetExitBoxInfo(unsigned short& wMapID, int& nJumpID);

    // === Return Map ===
    // IDA: ?GetReturnMapID@XModeMaze@@UEAAGK@Z (0x140295F50)
    virtual unsigned short GetReturnMapID(unsigned long dwUCID);

    // === Dimension Point ===
    // IDA: ?GetDemensionPoint@XModeMaze@@QEAAHK@Z (0x140295FC0)
    int GetDemensionPoint(unsigned long dwUCID);

    // IDA: ?SetDemensionShutterNextRevive@XModeMaze@@QEAAXPEAVCUser@@@Z (0x140296030)
    void SetDemensionShutterNextRevive(CUser* pUser);

    // IDA: ?CheckDemensionShutterRevive@XModeMaze@@QEAAX_K@Z (0x140296180)
    void CheckDemensionShutterRevive(unsigned __int64 dwTick);

    // IDA: ?Revive@XModeMaze@@QEAAXK@Z (0x140296390)
    void Revive(unsigned long dwUCID);

    // IDA: ?UpdateDemensionShutterUserLeave@XModeMaze@@QEAAXK_N@Z (0x140296480)
    void UpdateDemensionShutterUserLeave(unsigned long dwUCID, bool bLeave);

    // IDA: ?UpdateDemensionShutterUserPoint@XModeMaze@@QEAAXKH_N@Z (0x140296540)
    void UpdateDemensionShutterUserPoint(unsigned long dwUCID, int nPoint, bool bAdd);

    // IDA: ?UpdateDemensionShutterUserAttachPoint@XModeMaze@@QEAAXKH@Z (0x140296770)
    void UpdateDemensionShutterUserAttachPoint(unsigned long dwUCID, int nPoint);

    // IDA: ?UpdateDemensionShutterUserRank@XModeMaze@@QEAAXKG@Z (0x140296A30)
    void UpdateDemensionShutterUserRank(unsigned long dwUCID, unsigned short wRank);

    // IDA: ?UpdateDemensionShutterRankingPoint@XModeMaze@@QEAAHPEAVCUser@@G_N@Z (0x140296AF0)
    int UpdateDemensionShutterRankingPoint(CUser* pUser, unsigned short wPoint, bool bAdd);

    // === Dimension Ranking ===
    // IDA: ?CalculateDemensionShutterRank@XModeMaze@@QEAAXPEAV?$vector@UST_MODE_MAZE_USER_SCORE@@@Z (0x140296???)
    void CalculateDemensionShutterRank(std::vector<ST_MODE_MAZE_USER_SCORE>* pVecRank);

    // === Send Dimension Info ===
    // IDA: ?SendDemensionPoint@XModeMaze@@QEAAXK@Z (0x1402977D0)
    void SendDemensionPoint(unsigned long dwUCID);

    // IDA: ?SendDemensionShutterInfo@XModeMaze@@QEAAXKH_K@Z (0x1402978F0)
    void SendDemensionShutterInfo(unsigned long dwUCID, int nType, unsigned __int64 dwTick);

    // IDA: ?SendDemensionShutterReward@XModeMaze@@QEAAXAEAUPS_MODE_MAZE_REWARD_INFO@@@Z (0x140297A40)
    void SendDemensionShutterReward(struct PS_MODE_MAZE_REWARD_INFO& stInfo);

    // IDA: ?SendDemensionShutterEventMatchingReward@XModeMaze@@QEAAXAEAUPS_MODE_MAZE_REWARD_INFO@@@Z (0x140297AD0)
    void SendDemensionShutterEventMatchingReward(struct PS_MODE_MAZE_REWARD_INFO& stInfo);

    // === Sector Monster ===
    // IDA: ?AllDestroySectorMonster@XModeMaze@@QEAAXXZ (0x140297C50)
    void AllDestroySectorMonster();

    // === Sector Warp ===
    // IDA: ?WarpSectorStartPos@XModeMaze@@UEAA_NPEAVCUser@@@Z (0x140297CE0)
    virtual bool WarpSectorStartPos(CUser* pUser);

    // IDA: ?MoveNextSector@XModeMaze@@UEAAXPEAVCUser@@H@Z (0x1402981A0)
    virtual void MoveNextSector(CUser* pUser, int nNextStep);

    // === Generate ===
    // IDA: ?Generate@XModeMaze@@UEAAXXZ (0x1402984C0)
    virtual void Generate();

    // IDA: ?SpawnGenerateMonster@XModeMaze@@UEAAXXZ (0x1402984E0)
    virtual void SpawnGenerateMonster();

    // === Broadcast ===
    // IDA: ?SendBroadCast@XModeMaze@@UEAAXAEAVXSendPacket@@PEAVXActor@@W4E_BROADCAST_TYPE@IXArea@@@Z (0x140298870)
    virtual void SendBroadCast(XSendPacket* pPacket, XActor* pExceptActor, E_BROADCAST_TYPE eType);

    // IDA: ?SendBroadCast@XModeMaze@@UEAAXAEAVXSendPacket@@PEAVXActor@@_NW4E_BROADCAST_TYPE@IXArea@@@Z (0x1402989A0)
    virtual void SendBroadCast(XSendPacket* pPacket, XActor* pExceptActor, bool bSend, E_BROADCAST_TYPE eType);

    // IDA: ?SendBroadCastAll@XModeMaze@@QEAAXAEAVXSendPacket@@_N@Z (0x140298C00)
    void SendBroadCastAll(XSendPacket* pPacket, bool bSend);

    // === Cheat ===
    // IDA: ?SetPlayTimeStop_Cheat@XModeMaze@@QEAAXXZ (0x140298D20)
    void SetPlayTimeStop_Cheat();

protected:
    // === IDA confirmed member variables ===

    // Operation Info Table
    TB_OPERATION_INFO* m_pTB_OPERATION_INFO;

    // Party member count (from XMaze but re-declared here)
    int m_nPartyMemeberCount;
    int m_nMaxUserCount;

    // Dimension/Shutter variables
    int m_nMaxDemensionTime;
    int m_nMaxDemensionPoint;
    int m_nTotalDemensionPoint;
    int m_nGoalDemensionPoint;
    std::uint64_t m_dw64DemensionInfoSendTick;
    std::uint64_t m_dw64DemensionReviveTick;
    std::uint32_t m_dwLastCheckPlayTime;
    bool m_bPlayTimeStop_Cheat;
    bool m_bHotTime;
    std::uint32_t m_dwEventRoomID;

    // Grid Scanner for objects
    // TODO: AREA_OBJECT m_objectGridScanner;
    char m_objectGridScanner_dummy[256];  // Placeholder for AREA_OBJECT

    // Enter limit maps
    std::map<std::uint32_t, std::uint32_t> m_mapFirstJumpID;      // UXActorID -> JumpID
    std::map<std::uint32_t, std::uint32_t> m_mapFirstSectorID;    // UXActorID -> SectorID
    std::map<unsigned long, int> m_mapCheckSectorUser;            // UCID -> UniqueID for sector check

    // ModeMaze wait enter user info map (stores ST_MAZE_WAIT_ENTER_USER_INFO)
    std::map<unsigned long, ST_MAZE_WAIT_ENTER_USER_INFO> m_mapModeMazeWaitEnterUser;

    // Dimension Score Map
    std::map<unsigned long, ST_SERVER_MODE_MAZE_USER_SCORE> m_mapDemensionScore;
};
