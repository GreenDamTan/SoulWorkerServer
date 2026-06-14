// Npc.cpp
// CNpc - NPC class implementation
// IDA decompilation from GameServer.exe

#include "Npc.h"
#include "Soulworker/GameServer/XGameServer/WayPoint.h"
#include "Soulworker/Common/XNet/XCommon/Packet/XSendPacket.h"
#include "Soulworker/GameServer/XGameServer/VString.h"
#include <cmath>

// IDA: ??0CNpc@@QEAA@XZ @ 0x1403a2ec0
CNpc::CNpc() : CMoverEx() {
    // Initialize member variables
    m_stNpcInfo.nSectorID = -1;
    m_stNpcInfo.nNpcID = 0;
    m_stNpcInfo.dwActorID = 0;
    m_stNpcInfo.fPosX = 0.0f;
    m_stNpcInfo.fPosY = 0.0f;
    m_stNpcInfo.fPosZ = 0.0f;
    m_stNpcInfo.fRot = 0.0f;
    m_stNpcInfo.nTableID = 0;
    m_stNpcInfo.nHP = 0;
    m_stNpcInfo.byLevel = 0;

    m_pSector = nullptr;
    m_nSpawnBoxID = 0;
    m_pNpcTableRef = nullptr;
    m_vMoveTargetPos = hkvVec3(0.0f, 0.0f, 0.0f);
    m_fUpdatePatrolTime = 0.0f;
    m_nMoveWayPointID = 0;
    m_bCallMovingYaw = false;
    m_fDefTurnSpeed = 0.0f;
    m_fDieFadeTime = 0.0f;
    m_bCollisionEnable = true;
    m_fAbility = nullptr;

    SetInfo();
    // TODO: Call CAkashicObject::InitComponant(this) when available
}

// IDA: ??1CNpc@@UEAA@XZ @ 0x1403a3100
CNpc::~CNpc() {
    m_pSector = nullptr;
    m_pNpcTableRef = nullptr;
}

// IDA: ?UpdateSectorID@CNpc@@QEAAXXZ @ 0x1403a3200
void CNpc::UpdateSectorID() {
    int nSectorBoxID = -1;
    if (m_pSector) {
        nSectorBoxID = m_pSector->GetSectorBoxID();
    }
    m_stNpcInfo.nSectorID = nSectorBoxID;
}

// IDA: ?InitialObjectInfo@CNpc@@UEAAXKHVhkvVec3@@M@Z @ 0x1403a3250
void CNpc::InitialObjectInfo(std::uint32_t dwID, int nTableIdx, const hkvVec3& vPos, float fRot) {
    // Get NPC attribute component
    auto pAttr = GetGOC<CGocNpcAttribute>();
    if (pAttr) {
        // Initialize attribute with NPC table reference
        pAttr->Init(1, m_pNpcTableRef);

        // Get ability stats
        m_fAbility = pAttr->GetFinalStats();
        if (m_fAbility) {
            m_stNpcInfo.nHP = static_cast<int>(m_fAbility[10]);
        }

        m_stNpcInfo.nTableID = nTableIdx;

        // Set actor ID (combine session ID with type flags)
        m_stNpcInfo.uxActorID.dwActorID = (m_stNpcInfo.uxActorID.dwActorID & 0x1FFFFFFF) | 0x20000000;
        std::uint32_t dwSessionID = GetSessionID();
        m_stNpcInfo.uxActorID.dwActorID = (dwSessionID & 0x1FFFFFFF) | (m_stNpcInfo.uxActorID.dwActorID & 0xE0000000);
    }

    // Set position and rotation
    m_stNpcInfo.stPosInfo.vPos = vPos;
    m_stNpcInfo.stPosInfo.fRot = fRot;

    // Initialize way point
    m_xWayPoint.Init(this);

    // Set default turn speed from table
    if (m_pNpcTableRef) {
        m_fDefTurnSpeed = static_cast<float>(m_pNpcTableRef->NPC_Turn_Speed);
    }

    // Setup animation
    SetupAnimation();
}

// IDA: ?GetActorID@CNpc@@UEAA?ATUXActorID@@XZ @ 0x1403a3440
UXActorID CNpc::GetActorID() const {
    UXActorID result;
    result.dwActorID = m_stNpcInfo.uxActorID.dwActorID;
    return result;
}

// IDA: ?GetID@CNpc@@UEAAKXZ @ 0x1403a3470
std::uint64_t CNpc::GetID() const {
    return m_stNpcInfo.uxActorID.dwActorID;
}

// IDA: ?GetActionResourceFN@CNpc@@UEAA?AVVString@@XZ @ 0x1403a3490
VString CNpc::GetActionResourceFN() const {
    VString result;
    if (m_pNpcTableRef) {
        VString strFilePath;
        VString::Format(&strFilePath, "%s.adf", m_pNpcTableRef->NPC_Code_Name);
        result = strFilePath;
    }
    return result;
}

// IDA: ?SetSyncInfo@CNpc@@UEAAXXZ @ 0x1403a3520
void CNpc::SetSyncInfo() {
    // Get way point ID and store in sync info
    m_stNpcInfo.nWayPointID = GetWayPointID();
}

// IDA: ?SetPositionXVec3@CNpc@@UEAAXAEAUXVec3@@@Z @ 0x1403a3550
void CNpc::SetPositionXVec3(XVec3& vPos) {
    // Update NPC info position
    m_stNpcInfo.stPosInfo.vPos = vPos;

    // Call base class SetPositionXVec3
    CMover::SetPositionXVec3(vPos);
}

// IDA: ?SetInfoPacket@CNpc@@UEAAXAEAVXSendPacket@@@Z @ 0x1403a3590
void CNpc::SetInfoPacket(XSendPacket& xSendPacket) {
    // Get variable value
    GetVariableValue();

    // Write NPC info to packet
    xSendPacket << m_stNpcInfo;
}

// IDA: ?SetTablePtr@CNpc@@QEAAXPEAUTB_NPC@@@Z @ 0x1403a35e0
void CNpc::SetTablePtr(TB_NPC* pTBNPC) {
    if (pTBNPC) {
        m_pNpcTableRef = pTBNPC;
        m_stNpcInfo.byLevel = pTBNPC->NPC_Level;
    }
}

// IDA: ?ThinkFunction@CNpc@@UEAAXXZ @ 0x1403a3620
void CNpc::ThinkFunction() {
    // Call base class think function
    CMoverEx::ThinkFunction();

    // Get delta time from timer
    auto pTimer = ThreadLocalData::GetTimer();
    float fDeltaTime = pTimer->GetTimeDifference();

    // Handle die fade time
    if (m_fDieFadeTime > 0.0f) {
        m_fDieFadeTime -= fDeltaTime;
        if (m_fDieFadeTime <= 0.0f) {
            m_fDieFadeTime = 0.0f;

            // Add to destroy list if area exists
            XArea* pArea = GetArea();
            if (pArea) {
                pArea->AddDestoryObject(this);
            }
        }
    }

    // Check collision
    if (!m_bCollisionEnable && m_nMotionClass != 3) {
        SetCollisionEnable(true, false);
    }

    // Update patrol
    _UpdatePatrol(fDeltaTime);
}

// IDA: ?StopMoving@CNpc@@UEAAX_N@Z @ 0x1403a3780
void CNpc::StopMoving(bool bSendPacket) {
    // Set socket active state
    SetSocketActive(bSendPacket);

    // Check way point
    CheckWayPoint();
}

// IDA: ?CheckWayPoint@CNpc@@UEAAXXZ @ 0x1403a37c0
void CNpc::CheckWayPoint() {
    // Check current way point ID
    if (m_xWayPoint.GetCurID() > 0) {
        // Get destination position
        hkvVec3 destPos;
        const hkvVec3* pDestPos = m_xWayPoint.GetDestPosition(&destPos);

        // Calculate distance to destination
        hkvVec3 vDist = m_vPosition - *pDestPos;

        // If within range, update way point
        if (vDist.getLengthSquared() < 9.0f) {
            m_xWayPoint.Update(0.001f);
        }
    }

    // Check move way point ID
    if (m_nMoveWayPointID > 0) {
        // Calculate distance to move target position
        float fDiffX = m_vPosition.x - m_vMoveTargetPos.x;
        float fDiffY = m_vPosition.y - m_vMoveTargetPos.y;

        // If close enough, call arrive callback
        if (std::fabs(fDiffX) < 3.0f && std::fabs(fDiffY) < 3.0f) {
            OnArriveWayPoint();
        }
    }
}

// IDA: ?UpdateRotation@CNpc@@UEAAXM@Z @ 0x1403a3d60
void CNpc::UpdateRotation(float fDeltaTime) {
    // Call base class update rotation
    CMoverEx::UpdateRotation(fDeltaTime);

    // Check if moving yaw callback is pending
    if (m_bCallMovingYaw) {
        // Calculate yaw difference
        float fDiffYaw = m_fMovingYaw - GetOrientationYaw();

        // If close enough to target yaw
        if (std::fabs(fDiffYaw) < 1.0f) {
            // Get area and check if it's a maze
            XArea* pArea = GetArea();
            XMaze* pMaze = dynamic_cast<XMaze*>(pArea);

            if (pMaze) {
                // Get script instance from maze
                IVScriptInstance* pScriptInst = pMaze->GetArea();
                if (pScriptInst) {
                    // Store and clear callback state
                    int nOutParam = m_nCallMovingYawKey;
                    m_nCallMovingYawKey = 0;
                    m_bCallMovingYaw = false;

                    // Execute script callback
                    pScriptInst->ExecuteFunctionArg("OnNpcRotation", "iiTSoulworker:XMaze;",
                        GetTableID(), nOutParam, pMaze);
                }
            }
        }
    }
}

// IDA: ?SetDirectionTo@CNpc@@UEAAXAEBVhkvVec3@@@Z @ 0x1403a3f00
void CNpc::SetDirectionTo(const hkvVec3& vTargetPos) {
    // Calculate direction vector
    hkvVec3 vDirVector = vTargetPos - m_vPosition;

    // Get yaw from direction vector
    float fYaw = GetYawFromVector(vDirVector);

    // Set moving yaw
    SetMovingYaw(fYaw);
}

// IDA: ?_UpdatePatrol@CNpc@@AEAAXM@Z @ 0x1403a39e0
void CNpc::_UpdatePatrol(float fElapsedTime) {
    m_fUpdatePatrolTime += fElapsedTime;

    if (m_fUpdatePatrolTime >= 1.0f) {
        float fAccumTime = m_fUpdatePatrolTime;
        m_fUpdatePatrolTime = 0.0f;

        // Check if we have a way point
        int nWayPointID = GetWayPointID();
        if (nWayPointID) {
            // Check if not moving and in idle motion
            if (m_stMovePos.IsZero() && m_nMotionClass == 1) {
                // Update way point
                auto pWayPoint = GetWayPoint();
                pWayPoint->Update(fAccumTime);

                // Check if way point changed
                if (pWayPoint->IsChangeWayPoint()) {
                    // Get current position
                    hkvVec3 vMyPos = GetPosition();
                    SetCreatePos(vMyPos);

                    // Get area and check for nav mesh
                    XArea* pArea = GetArea();
                    if (pArea) {
                        if (pArea->GetNavMeshInstance()) {
                            // Get way point ID
                            int nWPID = GetWayPointID();
                            if (nWPID) {
                                // Get object resource and search for way point info
                                auto pResource = pArea->GetObjectResource();
                                const VEventObjectInfo* pInfo = pResource->SearchFromID(nWPID);
                                if (pInfo) {
                                    hkvVec3 vTargetPos;
                                    VEventObjectResourceManager::GetCenterPos(pInfo, &vTargetPos);
                                    SetCreatePos(vTargetPos);
                                    _MoveToPos(vMyPos, vTargetPos);
                                }
                            }
                        }
                    } else {
                        // No area - log error
                        UXMapID mapID = GetMapInsID();
                        XPRINT("if( NULL==pMaze ) [GetMazeID:%u]", mapID.nMapID);
                    }
                }
            }
        }
    }
}

// IDA: ?_MoveToPos@CNpc@@AEAAXAEAVhkvVec3@@0@Z @ 0x1403a3f50
void CNpc::_MoveToPos(hkvVec3& vMyPos, hkvVec3& vTargetPos) {
    // Set move position to target
    SetMovePosition(vTargetPos.x, vTargetPos.y);

    // Calculate direction and set rotation
    hkvVec3 vDirection(vTargetPos.x, vTargetPos.y, vMyPos.z);
    SetDirectionTo(vDirection);

    // Clear status and send move packet
    ClearStatus(0x100);
    send_eSUB_CMD_MOVE(this, vTargetPos.x, vTargetPos.y, 0);
}

// IDA: ?MoveToWayPoint@CNpc@@QEAAXH@Z @ 0x1403a4010
void CNpc::MoveToWayPoint(int nWayPointID) {
    // Initialize target position
    hkvVec3 vTargetPos;
    vTargetPos.setZero();

    // Get current position
    hkvVec3 vMyPos = GetPosition();
    SetCreatePos(vMyPos);

    // Get area and check for nav mesh
    XArea* pArea = GetArea();
    if (pArea) {
        if (pArea->GetNavMeshInstance()) {
            // Search for way point info
            const VEventObjectInfo* pInfo = nullptr;
            if (nWayPointID) {
                auto pResource = pArea->GetObjectResource();
                pInfo = pResource->SearchFromID(nWayPointID);
            }

            if (pInfo) {
                // Get center position of way point
                VEventObjectResourceManager::GetCenterPos(pInfo, &vTargetPos);
                SetCreatePos(vTargetPos);

                // Move to position
                _MoveToPos(vMyPos, vTargetPos);

                // Store move way point info
                m_nMoveWayPointID = nWayPointID;
                m_vMoveTargetPos = vTargetPos;
            }
        }
    } else {
        // No area - log error
        UXMapID mapID = GetMapInsID();
        XPRINT("if( NULL==pMaze ) [GetMazeID:%u]", mapID.nMapID);
    }
}

// IDA: ?CallMovingYaw@CNpc@@QEAAXMH@Z @ 0x1403a3cd0
void CNpc::CallMovingYaw(float fYaw, int nKey) {
    // Clear movement state
    MoveingValueClear();
    ClearMotion();
    send_eSUB_CMD_MOVE_STOP(this);

    // Set callback state
    m_bCallMovingYaw = true;
    m_nCallMovingYawKey = nKey;

    // Set moving yaw and send idle packet
    SetMovingYaw(fYaw);
    send_eSUB_CMD_MOVE_IDLE(this, 0.0);
}

// IDA: ?OnArriveWayPoint@CNpc@@MEAAXXZ
void CNpc::OnArriveWayPoint() {
    m_nMoveWayPointID = 0;
    // TODO: Handle arrival at way point
}
