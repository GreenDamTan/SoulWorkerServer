// WayPoint.h
// WayPoint system for NPC patrolling
// IDA: ?NextWayPoint@CWayPoint@@QEAAXXZ (0x140199370)
// IDA: ?CheckIdleAction@CWayPoint@@QEAAXXZ (0x1401993d0)
// IDA: ?GetRandomNextID@CWayPoint@@IEAAHPEAUVWayPointInfo@@@Z (0x140199650)
// IDA: ?GetNextID@CWayPoint@@IEAAHPEAUVWayPointInfo@@H@Z (0x140199740)
// IDA: ?SetCurID@CWayPoint@@QEAAXH_N@Z (0x140199930)
// IDA: ?ChangeCurWayPoint@CWayPoint@@QEAAXH@Z (0x140199a70)
// IDA: ?GetWayPointInfo@CWayPoint@@IEAAPEAUVWayPointInfo@@H@Z (0x140199ab0)

#pragma once

#include <cstdint>

// Forward declarations
class CMoverEx;
struct VWayPointInfo;
struct XArea;
struct VEventObjectResource;
struct VEventObjectInfo;

// Waypoint direction enumeration
enum CWayPoint_eWAYPOINT_DIR {
    E_WAYDIR_FORWARD = 0,
    E_WAYDIR_REVERSE = 1
};

// Waypoint state enumeration
enum CWayPoint_eWAYSTATE {
    E_WAYSTAT_NONE = 0,
    E_WAYSTAT_MOVE = 1,
    E_WAYSTAT_WAIT = 2
};

// VWayPointInfo structure - waypoint information
// This is a simplified representation; actual structure inherits from VEventObjectInfo
struct VWayPointInfo {
    // Base fields from VEventObjectInfo
    virtual ~VWayPointInfo() {}

    // Waypoint fields (from IDA analysis)
    int m_iNextLink;          // Next link ID
    int m_iNextLink2;         // Alternative next link 2
    int m_iNextLink3;         // Alternative next link 3
    int m_iNextLink4;         // Alternative next link 4
    int m_iBeforeLink;        // Previous link ID
    int m_eType;              // Waypoint type
    unsigned int m_uiIdleActionRatio;  // Idle action probability (0-100)
    char m_szIdle[64];        // Idle action name
    int m_eBattleType;        // Battle type (0=normal, 1=battle)
    int m_RepeatCount;        // Repeat count for type 2 waypoints
};

// CWayPoint - Waypoint management class
class CWayPoint {
public:
    CWayPoint();
    ~CWayPoint();

    // IDA: ?Init@CWayPoint@@QEAAXPEAVCMoverEx@@@Z (0x140199350)
    void Init(CMoverEx* pOwner);

    // IDA: ?NextWayPoint@CWayPoint@@QEAAXXZ (0x140199370)
    void NextWayPoint();

    // IDA: ?CheckIdleAction@CWayPoint@@QEAAXXZ (0x1401993d0)
    void CheckIdleAction();

    // IDA: ?Update@CWayPoint@@QEAAXM@Z (0x1401995a0)
    void Update(float fDeltaTime);

    // IDA: ?GetRandomNextID@CWayPoint@@IEAAHPEAUVWayPointInfo@@@Z (0x140199650)
    int GetRandomNextID(VWayPointInfo* pCurInfo);

    // IDA: ?GetNextID@CWayPoint@@IEAAHPEAUVWayPointInfo@@H@Z (0x140199740)
    int GetNextID(VWayPointInfo* pCurInfo, int bModifyDirection);

    // IDA: ?SetCurID@CWayPoint@@QEAAXH_N@Z (0x140199930)
    void SetCurID(int nID, bool bChangePose);

    // IDA: ?ChangeCurWayPoint@CWayPoint@@QEAAXH@Z (0x140199a70)
    void ChangeCurWayPoint(int nID);

    // IDA: ?GetWayPointInfo@CWayPoint@@IEAAPEAUVWayPointInfo@@H@Z (0x140199ab0)
    VWayPointInfo* GetWayPointInfo(int nID);

    // IDA: ?GetDestPosition@CWayPoint@@QEAA?AVhkvVec3@@XZ (0x140199bd0)
    // Returns destination position from current waypoint
    hkvVec3 GetDestPosition();

    // IDA: ?GetNextPosition@CWayPoint@@QEAA_NAEAVhkvVec3@@@Z (0x140199c20)
    // Gets next position in the waypoint path

    // IDA: ?CheckHitCollisonBox@CWayPoint@@QEAAEAEBVhkvVec3@@@Z (0x140199d70)
    char CheckHitCollisonBox(const float* vPos);

    // IDA: ?IsChangeWayPoint@CWayPoint@@QEAA_NXZ (0x140199e00)
    bool IsChangeWayPoint();

    // Accessors
    int GetCurID() const { return m_nCurID; }
    VWayPointInfo* GetCurPointInfo() const { return m_pCurPointInfo; }
    int GetState() const { return m_eState; }

private:
    CMoverEx* m_pOwner;              // Owner mover
    VWayPointInfo* m_pCurPointInfo;  // Current waypoint info
    int m_nCurID;                    // Current waypoint ID
    int m_eState;                    // Current state (E_WAYSTAT_*)
    int m_eDirection;                // Direction (E_WAYDIR_*)
    float m_fWaitTime;               // Wait time at current waypoint
    int m_nRepeatCount;              // Repeat count for type 2 waypoints
    bool m_bInited;                  // Initialization flag
    bool m_bChangedID;               // Waypoint changed flag
};
