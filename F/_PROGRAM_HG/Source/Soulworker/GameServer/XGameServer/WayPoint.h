// WayPoint.h
// WayPoint system for NPC patrolling
// Note: CWayPoint class is defined in VisionEngineTypes.h
// This file contains VWayPointInfo structure and extended method declarations

#pragma once

#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include <cstdint>

// Forward declarations
class CMoverEx;
struct XArea;
struct VEventObjectResource;
struct VEventObjectInfo;

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

// CWayPoint extended methods - implemented in WayPoint.cpp
// The base CWayPoint class is defined in VisionEngineTypes.h
// These are additional methods that need implementation:

// IDA: ?NextWayPoint@CWayPoint@@QEAAXXZ (0x140199370)
void CWayPoint_NextWayPoint(CWayPoint* pWayPoint);

// IDA: ?CheckIdleAction@CWayPoint@@QEAAXXZ (0x1401993d0)
void CWayPoint_CheckIdleAction(CWayPoint* pWayPoint);

// IDA: ?Update@CWayPoint@@QEAAXM@Z (0x1401995a0)
void CWayPoint_Update(CWayPoint* pWayPoint, float fDeltaTime);

// IDA: ?GetRandomNextID@CWayPoint@@IEAAHPEAUVWayPointInfo@@@Z (0x140199650)
int CWayPoint_GetRandomNextID(CWayPoint* pWayPoint, VWayPointInfo* pCurInfo);

// IDA: ?GetNextID@CWayPoint@@IEAAHPEAUVWayPointInfo@@H@Z (0x140199740)
int CWayPoint_GetNextID(CWayPoint* pWayPoint, VWayPointInfo* pCurInfo, int bModifyDirection);

// IDA: ?SetCurID@CWayPoint@@QEAAXH_N@Z (0x140199930)
void CWayPoint_SetCurID(CWayPoint* pWayPoint, int nID, bool bChangePose);

// IDA: ?ChangeCurWayPoint@CWayPoint@@QEAAXH@Z (0x140199a70)
void CWayPoint_ChangeCurWayPoint(CWayPoint* pWayPoint, int nID);

// IDA: ?GetWayPointInfo@CWayPoint@@IEAAPEAUVWayPointInfo@@H@Z (0x140199ab0)
VWayPointInfo* CWayPoint_GetWayPointInfo(CWayPoint* pWayPoint, int nID);

// IDA: ?GetDestPosition@CWayPoint@@QEAA?AVhkvVec3@@XZ (0x140199bd0)
hkvVec3 CWayPoint_GetDestPosition(CWayPoint* pWayPoint);

// IDA: ?GetNextPosition@CWayPoint@@QEAA_NAEAVhkvVec3@@@Z (0x140199c20)
bool CWayPoint_GetNextPosition(CWayPoint* pWayPoint, hkvVec3& vPos);

// IDA: ?CheckHitCollisonBox@CWayPoint@@QEAAEAEBVhkvVec3@@@Z (0x140199d70)
char CWayPoint_CheckHitCollisonBox(CWayPoint* pWayPoint, const float* vPos);

// IDA: ?IsChangeWayPoint@CWayPoint@@QEAA_NXZ (0x140199e00)
bool CWayPoint_IsChangeWayPoint(CWayPoint* pWayPoint);
