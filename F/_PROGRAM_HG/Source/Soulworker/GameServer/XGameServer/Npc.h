// Npc.h
// CNpc - NPC class for game server
// IDA decompilation from GameServer.exe

#pragma once

#include "Soulworker/GameServer/XGameServer/MoverEx.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes/hkvVec3.h"
#include <cstdint>

class CSector;
class CWayPoint;
struct TB_NPC;
class XArea;
class XSendPacket;
class VString;

// STNpcInfo - NPC information structure
// IDA: ??0STNpcInfo@@QEAA@XZ @ 0x140354a40
struct STNpcInfo {
    int nSectorID;                      // +0x00
    int nNpcID;                         // +0x04
    std::uint32_t dwActorID;            // +0x08
    float fPosX;                        // +0x0C
    float fPosY;                        // +0x10
    float fPosZ;                        // +0x14
    float fRot;                         // +0x18
    // Additional fields from IDA
    int nTableID;                       // +0x1C
    int nHP;                            // +0x20
    std::uint8_t byLevel;               // +0x24
    // TODO: Add more fields as discovered from IDA
};

// CNpc - NPC class inheriting from CMoverEx
// IDA: ??0CNpc@@QEAA@XZ @ 0x1403a2ec0
// IDA: ??1CNpc@@UEAA@XZ @ 0x1403a3100
class CNpc : public CMoverEx {
public:
    // IDA: ??0CNpc@@QEAA@XZ @ 0x1403a2ec0
    CNpc();

    // IDA: ??1CNpc@@UEAA@XZ @ 0x1403a3100
    virtual ~CNpc();

    // === Virtual overrides ===

    // IDA: ?UpdateSectorID@CNpc@@QEAAXXZ @ 0x1403a3200
    // Updates the sector ID based on current sector
    void UpdateSectorID();

    // IDA: ?InitialObjectInfo@CNpc@@UEAAXKHVhkvVec3@@M@Z @ 0x1403a3250
    // Initialize NPC object info
    void InitialObjectInfo(std::uint32_t dwID, int nTableIdx, const hkvVec3& vPos, float fRot);

    // IDA: ?GetActorID@CNpc@@UEAA?ATUXActorID@@XZ @ 0x1403a3440
    UXActorID GetActorID() const;

    // IDA: ?GetID@CNpc@@UEAAKXZ @ 0x1403a3470
    std::uint64_t GetID() const;

    // IDA: ?GetActionResourceFN@CNpc@@UEAA?AVVString@@XZ @ 0x1403a3490
    VString GetActionResourceFN() const;

    // IDA: ?SetSyncInfo@CNpc@@UEAAXXZ @ 0x1403a3520
    void SetSyncInfo();

    // IDA: ?SetPositionXVec3@CNpc@@UEAAXAEAUXVec3@@@Z @ 0x1403a3550
    void SetPositionXVec3(XVec3& vPos);

    // IDA: ?SetInfoPacket@CNpc@@UEAAXAEAVXSendPacket@@@Z @ 0x1403a3590
    void SetInfoPacket(XSendPacket& xSendPacket);

    // IDA: ?ThinkFunction@CNpc@@UEAAXXZ @ 0x1403a3620
    void ThinkFunction() override;

    // IDA: ?StopMoving@CNpc@@UEAAX_N@Z @ 0x1403a3780
    void StopMoving(bool bSendPacket) override;

    // IDA: ?CheckWayPoint@CNpc@@UEAAXXZ @ 0x1403a37c0
    void CheckWayPoint() override;

    // IDA: ?UpdateRotation@CNpc@@UEAAXM@Z @ 0x1403a3d60
    void UpdateRotation(float fDeltaTime);

    // IDA: ?SetDirectionTo@CNpc@@UEAAXAEBVhkvVec3@@@Z @ 0x1403a3f00
    void SetDirectionTo(const hkvVec3& vTargetPos);

    // === Accessors ===

    STNpcInfo& GetNpcInfo() { return m_stNpcInfo; }
    const STNpcInfo& GetNpcInfo() const { return m_stNpcInfo; }

    CSector* GetSector() const { return m_pSector; }
    void SetSector(CSector* pSector) { m_pSector = pSector; }

    // IDA: ?SetSpawnBoxID@CNpc@@QEAAXH@Z @ 0x1401add00
    void SetSpawnBoxID(int nVal) { m_nSpawnBoxID = nVal; }
    int GetSpawnBoxID() const { return m_nSpawnBoxID; }

    // IDA: ?SetTablePtr@CNpc@@QEAAXPEAUTB_NPC@@@Z @ 0x1403a35e0
    void SetTablePtr(TB_NPC* pTBNPC);

    // IDA: ?GetNpcTableRef@CNpc@@QEAAPEAUTB_NPC@@XZ @ 0x1404eb9b0
    TB_NPC* GetNpcTableRef() { return m_pNpcTableRef; }
    const TB_NPC* GetNpcTableRef() const { return m_pNpcTableRef; }

    // === Movement ===

    // IDA: ?MoveToWayPoint@CNpc@@QEAAXH@Z @ 0x1403a4010
    void MoveToWayPoint(int nWayPointID);

    // IDA: ?CallMovingYaw@CNpc@@QEAAXMH@Z @ 0x1403a3cd0
    void CallMovingYaw(float fYaw, int nSend);

protected:
    // IDA: ?_UpdatePatrol@CNpc@@AEAAXM@Z @ 0x1403a39e0
    void _UpdatePatrol(float fElapsedTime);

    // IDA: ?_MoveToPos@CNpc@@AEAAXAEAVhkvVec3@@0@Z @ 0x1403a3f50
    void _MoveToPos(hkvVec3& vFrom, hkvVec3& vTo);

    // IDA: ?OnArriveWayPoint@CNpc@@MEAAXXZ
    void OnArriveWayPoint();

private:
    STNpcInfo m_stNpcInfo;          // NPC info structure
    CSector* m_pSector;             // Current sector pointer
    int m_nSpawnBoxID;              // Spawn box ID
    TB_NPC* m_pNpcTableRef;         // NPC table reference
    hkvVec3 m_vMoveTargetPos;       // Move target position
    float m_fUpdatePatrolTime;      // Patrol update timer
    int m_nMoveWayPointID;          // Current way point ID
    bool m_bCallMovingYaw;          // Moving yaw flag
    float m_fDefTurnSpeed;          // Default turn speed
    float m_fDieFadeTime;           // Die fade time
    bool m_bCollisionEnable;        // Collision enable flag
    CWayPoint m_xWayPoint;          // Way point handler
    float* m_fAbility;              // Ability stats pointer
    // TODO: Add more members as discovered
};
