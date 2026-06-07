// CutsceneManager.h - Cutscene Management System
// Restored from GameServer.exe IDA decompilation

#pragma once

#include <cstdint>
#include <list>
#include <map>
#include <memory>
#include <string>

class XMaze;
class CUser;

// ST_CUTSCENE_MEMBER - Cutscene member structure (IDA: 296 bytes / 0x128)
// IDA struct info: size=296, members: pUser, szCutscene, nOrder, nTime, bChangeState, bInvincible, nType, bRecvScene
struct ST_CUTSCENE_MEMBER {
    CUser* pUser = nullptr;                 // offset 0x00 (8 bytes) - User pointer
    char szCutscene[256] = {};              // offset 0x08 (256 bytes) - Cutscene name
    std::int32_t nOrder = 0;                // offset 0x108 (4 bytes) - Order
    std::int64_t nTime = 0;                 // offset 0x110 (8 bytes) - End time (GetTickCount64)
    bool bChangeState = false;              // offset 0x118 (1 byte) - Change state flag
    bool bInvincible = false;               // offset 0x119 (1 byte) - Invincible flag
    std::int32_t nType = 0;                 // offset 0x11C (4 bytes) - Type
    bool bRecvScene = false;                // offset 0x120 (1 byte) - Received scene flag

    ST_CUTSCENE_MEMBER() = default;
    ~ST_CUTSCENE_MEMBER() = default;
};

// ST_CUTSCENE_INFO - Cutscene information structure
struct ST_CUTSCENE_INFO {
    std::uint32_t dwCutsceneID = 0;
    std::int32_t nOrder = 0;
    std::int32_t nSectorID = 0;
    std::int32_t nState = 0;          // 0 = none, 1 = playing, 2 = finished
    std::int32_t nType = 0;           // Cutscene type (0 = server, 1 = client)
    std::int32_t nConditionType = 0;  // Condition type for triggering
    std::int32_t nConditionValue = 0; // Condition value for triggering
    std::uint32_t dwTime = 0;         // Duration/time for cutscene
    bool bClientCutscene = false;
    std::list<std::uint32_t> listShowActorID; // List of actors already shown this cutscene

    ST_CUTSCENE_INFO() = default;
    ST_CUTSCENE_INFO(std::uint32_t id, std::int32_t order, std::int32_t sector, bool bClient = false)
        : dwCutsceneID(id), nOrder(order), nSectorID(sector), nState(0), nType(0), nConditionType(0), nConditionValue(0), dwTime(0), bClientCutscene(bClient) {}
};

/**
 * @brief CCutsceneManager - Manages cutscenes in maze instances
 *
 * Handles cutscene playback, member tracking, and state synchronization.
 * Each XMaze has a CCutsceneManager instance.
 *
 * IDA Functions:
 * - Constructor: 0x1401B0540
 * - Destructor: 0x1401B05B0
 * - Init: 0x1401B05F0
 * - Clear: 0x1401B0630
 * - AddMember: 0x1401B0680
 * - DeleteMember: 0x1401B07A0
 * - ClearMember: 0x1401B0830
 * - ChangeState: 0x1401B0A50
 * - AddCutsceneInfo: 0x1401B1070
 * - CheckCutsceneState: 0x1401B1240, 0x1401B17C0, 0x1401B1C40
 * - OnUpdate: 0x1401B2090
 * - SkipCutscene: 0x1401B2280
 * - SetCutscene: 0x1401B24B0
 * - GetCutsceneOrder: 0x1401B2750
 * - IsPlayCutscene: 0x1401B27A0
 * - SetNotifyMessage: 0x140353400
 * - IsCutsceneState: 0x1403534D0
 * - SetActiveSectorID: 0x140353E30
 */
class CCutsceneManager {
public:
    // IDA: ??0CCutsceneManager@@QEAA@XZ (0x1401B0540)
    CCutsceneManager();

    // IDA: ??1CCutsceneManager@@QEAA@XZ (0x1401B05B0)
    ~CCutsceneManager();

    // IDA: ?Init@CCutsceneManager@@QEAAXPEAVXMaze@@@Z (0x1401B05F0)
    void Init(XMaze* pMaze);

    // IDA: ?Clear@CCutsceneManager@@QEAAXXZ (0x1401B0630)
    void Clear();

    // IDA: ?AddMember@CCutsceneManager@@QEAAXPEAVCUser@@@Z (0x1401B0680)
    void AddMember(CUser* pUser);

    // IDA: ?DeleteMember@CCutsceneManager@@QEAAXK@Z (0x1401B07A0)
    void DeleteMember(std::uint32_t dwActorID);

    // IDA: ?ClearMember@CCutsceneManager@@QEAAXXZ (0x1401B0830)
    void ClearMember();

    // IDA: ?ChangeState@CCutsceneManager@@QEAAXXZ (0x1401B0A50)
    void ChangeState();

    // IDA: ?AddCutsceneInfo@CCutsceneManager@@QEAAXPEADHKHH@Z (0x1401B1070)
    // IDA parameters: (char* szName, int nType, unsigned int dwTime, int nConditionType, int nConditionValue)
    void AddCutsceneInfo(char* szName, int nType, unsigned int dwTime, int nConditionType, int nConditionValue);

    // IDA: ?CheckCutsceneState@CCutsceneManager@@QEAAXHH@Z (0x1401B1240)
    void CheckCutsceneState(std::int32_t nSectorID, std::int32_t nState);

    // IDA: ?CheckCutsceneState@CCutsceneManager@@QEAAXKHH@Z (0x1401B17C0)
    void CheckCutsceneState(std::uint32_t dwActorID, std::int32_t nSectorID, std::int32_t nState);

    // IDA: ?CheckCutsceneState@CCutsceneManager@@QEAAXKPEAD@Z (0x1401B1C40)
    void CheckCutsceneState(std::uint32_t dwActorID, char* pszCutsceneName);

    // IDA: ?OnUpdate@CCutsceneManager@@QEAAXXZ (0x1401B2090)
    void OnUpdate();

    // IDA: ?SkipCutscene@CCutsceneManager@@QEAAXKPEAD@Z (0x1401B2280)
    void SkipCutscene(std::uint32_t dwActorID, char* pszCutsceneName);

    // IDA: ?SetCutscene@CCutsceneManager@@QEAAXKPEAD@Z (0x1401B24B0)
    void SetCutscene(std::uint32_t dwActorID, char* pszCutsceneName);

    // IDA: ?GetCutsceneOrder@CCutsceneManager@@QEAAHH@Z (0x1401B2750)
    std::int32_t GetCutsceneOrder(std::int32_t nSectorID);

    // IDA: ?IsPlayCutscene@CCutsceneManager@@QEAA_NK@Z (0x1401B27A0)
    bool IsPlayCutscene(std::uint32_t dwCutsceneID);

    // IDA: ?SetNotifyMessage@CCutsceneManager@@QEAAX_N@Z (0x140353400)
    void SetNotifyMessage(bool bNotify) { m_bNotifyMessage = bNotify; }

    // IDA: ?IsCutsceneState@CCutsceneManager@@QEAA_NXZ (0x1403534D0)
    bool IsCutsceneState() const { return m_bSyncCutsceneState; }

    // IDA: ?SetActiveSectorID@CCutsceneManager@@QEAAXH@Z (0x140353E30)
    void SetActiveSectorID(std::int32_t nSectorID) { m_nActiveSectorID = nSectorID; }

protected:
    // IDA: m_pMaze - pointer to parent maze
    XMaze* m_pMaze = nullptr;

    // IDA: m_mapCutsceneMember - map of actor ID to ST_CUTSCENE_MEMBER shared_ptr
    // Type: std::map<unsigned long, std::tr1::shared_ptr<ST_CUTSCENE_MEMBER>>
    std::map<std::uint32_t, std::shared_ptr<ST_CUTSCENE_MEMBER>> m_mapCutsceneMember;

    // IDA: m_mapCutsceneInfo - map of cutscene name to info
    // Type: std::map<std::string, ST_CUTSCENE_INFO>
    std::map<std::string, ST_CUTSCENE_INFO> m_mapCutsceneInfo;

    // IDA: m_strFinishCutsceneName - name of finished cutscene
    char m_strFinishCutsceneName[256] = {};

    // IDA: m_bSyncCutsceneState - cutscene state synchronization flag
    bool m_bSyncCutsceneState = false;

    // IDA: m_bClientCutscenePlay - client cutscene playing flag
    bool m_bClientCutscenePlay = false;

    // IDA: m_bNotifyMessage - notify message flag
    bool m_bNotifyMessage = false;

    // IDA: m_nActiveSectorID - active sector ID
    std::int32_t m_nActiveSectorID = 0;
};
