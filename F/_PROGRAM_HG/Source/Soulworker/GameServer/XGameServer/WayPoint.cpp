// WayPoint.cpp
// WayPoint system implementation
// Reconstructed from IDA decompilation: GameServer.exe
//
// Functions:
// - CWayPoint::NextWayPoint (0x140199370)
// - CWayPoint::CheckIdleAction (0x1401993d0)
// - CWayPoint::Update (0x1401995a0)
// - CWayPoint::GetRandomNextID (0x140199650)
// - CWayPoint::GetNextID (0x140199740)
// - CWayPoint::SetCurID (0x140199930)
// - CWayPoint::ChangeCurWayPoint (0x140199a70)
// - CWayPoint::GetWayPointInfo (0x140199ab0)
// - CWayPoint::GetDestPosition (0x140199bd0)
// - CWayPoint::GetNextPosition (0x140199c20)

#include "WayPoint.h"
#include "MoverEx.h"
#include <cstdlib>
#include <cstring>

// Forward declarations for external types
struct hkvVec3;
struct XArea;
struct VEventObjectResource;
struct VEventObjectInfo;

// External function declarations
extern "C" void XPRINT(const char* format, ...);
extern "C" void* _RTDynamicCast_0(void* ptr, long v1, void* srcType, void* dstType, long v2);

// ============================================================================
// CWayPoint constructor
// ============================================================================
CWayPoint::CWayPoint()
    : m_pOwner(nullptr)
    , m_pCurPointInfo(nullptr)
    , m_nCurID(0)
    , m_eState(E_WAYSTAT_NONE)
    , m_eDirection(E_WAYDIR_FORWARD)
    , m_fWaitTime(0.0f)
    , m_nRepeatCount(0)
    , m_bInited(false)
    , m_bChangedID(false)
{
}

// ============================================================================
// CWayPoint destructor
// ============================================================================
CWayPoint::~CWayPoint() {
    m_pOwner = nullptr;
    m_pCurPointInfo = nullptr;
}

// ============================================================================
// CWayPoint::Init - Initialize waypoint system
// IDA @ 0x140199350
// ============================================================================
void CWayPoint::Init(CMoverEx* pOwner) {
    m_pOwner = pOwner;
    m_pCurPointInfo = nullptr;
    m_nCurID = 0;
    m_eState = E_WAYSTAT_NONE;
    m_eDirection = E_WAYDIR_FORWARD;
    m_fWaitTime = 0.0f;
    m_nRepeatCount = 0;
    m_bInited = false;
    m_bChangedID = false;
}

// ============================================================================
// CWayPoint::NextWayPoint - Move to next waypoint
// IDA @ 0x140199370
// ============================================================================
void CWayPoint::NextWayPoint() {
    int nNewID = GetNextID(m_pCurPointInfo, 1);
    if (GetCurID() == nNewID) {
        m_eState = E_WAYSTAT_NONE;
    } else {
        ChangeCurWayPoint(nNewID);
    }
}

// ============================================================================
// CWayPoint::CheckIdleAction - Check and execute idle action
// IDA @ 0x1401993d0
// ============================================================================
void CWayPoint::CheckIdleAction() {
    if (m_fWaitTime > 0.0f) {
        return;
    }

    // Check if idle action is configured
    if (!m_pCurPointInfo || !m_pCurPointInfo->m_uiIdleActionRatio) {
        // No idle action - check if we need to change battle pose
        if (m_pOwner && m_pOwner->IsBattlePose()) {
            m_pOwner->SetBattlePose(false);
            // ChangeMotion_3(1, 1, 23) - reset to idle
            // send_eSUB_CMD_MOVE_IDLE(m_pOwner, m_pOwner, 0.0f);
        }
        return;
    }

    // Check if idle action name is not empty
    bool bHasIdleAction = false;
    for (int i = 0; i < 64 && m_pCurPointInfo->m_szIdle[i]; ++i) {
        bHasIdleAction = true;
        break;
    }

    if (bHasIdleAction) {
        // Check if should play idle action based on probability
        if ((std::rand() % 100) <= static_cast<int>(m_pCurPointInfo->m_uiIdleActionRatio)) {
            // Get current motion and check if different from idle action
            // const VAnimationInfo* pCurMotion = m_pOwner->GetCurMotionEvent();
            // if (pCurMotion && std::strcmp(pCurMotion->szName, m_pCurPointInfo->m_szIdle) != 0) {
            //     m_pOwner->ChangeMotion_2(m_pCurPointInfo->m_szIdle, 1);
            //     m_pOwner->send_eSUB_CMD_MOVE_IDLE(m_pOwner, m_pOwner, 0.0f);
            // }
        }
    } else {
        // No idle action name - check battle pose
        if (m_pOwner && m_pOwner->IsBattlePose()) {
            m_pOwner->SetBattlePose(false);
            // m_pOwner->ChangeMotion_3(m_pOwner, 1, 1, 23);
            // m_pOwner->send_eSUB_CMD_MOVE_IDLE(m_pOwner, m_pOwner, 0.0f);
        }
    }
}

// ============================================================================
// CWayPoint::Update - Update waypoint state
// IDA @ 0x1401995a0
// ============================================================================
void CWayPoint::Update(float fDeltaTime) {
    // TODO: 汇编还原 - full implementation requires more context
    // Update wait time
    if (m_fWaitTime > 0.0f) {
        m_fWaitTime -= fDeltaTime;
        if (m_fWaitTime <= 0.0f) {
            m_fWaitTime = 0.0f;
        }
    }
}

// ============================================================================
// CWayPoint::GetRandomNextID - Get random next waypoint ID
// IDA @ 0x140199650
// ============================================================================
int CWayPoint::GetRandomNextID(VWayPointInfo* pCurInfo) {
    if (!pCurInfo) {
        return 0;
    }

    // Count available next links
    int nCount = (pCurInfo->m_iNextLink > 0) ? 1 : 0;
    if (pCurInfo->m_iNextLink2 > 0) ++nCount;
    if (pCurInfo->m_iNextLink3 > 0) ++nCount;
    if (pCurInfo->m_iNextLink4 > 0) ++nCount;

    // If no links available, return first link (may be 0)
    if (nCount == 0) {
        return pCurInfo->m_iNextLink;
    }

    // Random selection
    int nIndex = std::rand() % nCount;
    switch (nIndex) {
        case 1:
            return pCurInfo->m_iNextLink2;
        case 2:
            return pCurInfo->m_iNextLink3;
        case 3:
            return pCurInfo->m_iNextLink4;
        default:
            return pCurInfo->m_iNextLink;
    }
}

// ============================================================================
// CWayPoint::GetNextID - Get next waypoint ID based on direction and type
// IDA @ 0x140199740
// ============================================================================
int CWayPoint::GetNextID(VWayPointInfo* pCurInfo, int bModifyDirection) {
    // Use current info if not provided
    if (!pCurInfo && !m_pCurPointInfo) {
        return 0;
    }
    if (!pCurInfo) {
        pCurInfo = m_pCurPointInfo;
    }

    unsigned int iNextID = 0;
    int eDir = m_eDirection;

    switch (pCurInfo->m_eType) {
        case 0:  // Normal - random or reverse
        case 3:  // Patrol point
        case 5:  // Special point
            if (eDir == E_WAYDIR_REVERSE) {
                iNextID = pCurInfo->m_iBeforeLink;
            } else {
                iNextID = GetRandomNextID(pCurInfo);
            }
            break;

        case 1:  // One-way forward
            if (eDir == E_WAYDIR_FORWARD) {
                iNextID = pCurInfo->m_iNextLink;
            }
            break;

        case 2:  // Patrol back-and-forth
            // Check if should reverse direction
            if (eDir == E_WAYDIR_REVERSE && !pCurInfo->m_iBeforeLink) {
                // At start point, change to forward
                if (bModifyDirection) {
                    if (m_nRepeatCount <= 0 || (--m_nRepeatCount) != 0) {
                        eDir = E_WAYDIR_FORWARD;
                    }
                }
            }

            // Get next ID based on direction
            if (eDir == E_WAYDIR_REVERSE) {
                iNextID = pCurInfo->m_iBeforeLink;
            } else {
                iNextID = pCurInfo->m_iNextLink;
            }
            break;

        case 4:  // Alternating
            // Toggle direction
            if (m_eDirection == E_WAYDIR_REVERSE) {
                eDir = 1;  // Forward next
                iNextID = pCurInfo->m_iBeforeLink;
            } else {
                eDir = 2;  // Reverse next
                iNextID = pCurInfo->m_iNextLink;
            }
            break;

        default:
            break;
    }

    // Update direction if requested
    if (bModifyDirection) {
        m_eDirection = eDir;
    }

    return iNextID;
}

// ============================================================================
// CWayPoint::SetCurID - Set current waypoint ID
// IDA @ 0x140199930
// ============================================================================
void CWayPoint::SetCurID(int nID, bool bChangePose) {
    m_nCurID = nID;
    m_eState = (nID > 0) ? E_WAYSTAT_MOVE : E_WAYSTAT_NONE;

    if (nID <= 0) {
        m_pCurPointInfo = nullptr;
    } else {
        m_pCurPointInfo = GetWayPointInfo(nID);
    }

    if (m_pCurPointInfo && m_pOwner) {
        bool isBattlePose = (m_pCurPointInfo->m_eBattleType == 1);

        if (bChangePose) {
            // m_pOwner->ChangeBattlePose(isBattlePose, false);
        } else {
            m_pOwner->SetBattlePose(isBattlePose);
        }

        if (!m_bInited) {
            m_nRepeatCount = m_pCurPointInfo->m_RepeatCount;
            m_bInited = true;
        }
    }
}

// ============================================================================
// CWayPoint::ChangeCurWayPoint - Change to new waypoint
// IDA @ 0x140199a70
// ============================================================================
void CWayPoint::ChangeCurWayPoint(int nCurID) {
    SetCurID(nCurID, true);
    m_fWaitTime = 0.0f;
    m_bChangedID = true;
}

// ============================================================================
// CWayPoint::GetWayPointInfo - Get waypoint info by ID
// IDA @ 0x140199ab0
// ============================================================================
VWayPointInfo* CWayPoint::GetWayPointInfo(int nID) {
    if (!m_pOwner) {
        return nullptr;
    }

    // TODO: 需要完整实现 - requires XArea and VEventObjectResource
    // This needs access to:
    // - XActor::GetArea()
    // - XArea::GetObjectResource()
    // - VEventObjectResource::SearchFromID()
    // - _RTDynamicCast for type conversion

    // Simplified implementation
    return nullptr;
}

// ============================================================================
// CWayPoint::CheckHitCollisonBox - Check hit collision box
// IDA @ 0x140199d70
// ============================================================================
char CWayPoint::CheckHitCollisonBox(const float* vPos) {
    if (!m_pCurPointInfo) {
        return 0;
    }

    // TODO: 需要RTTI转换检查VEscortPointInfo
    // VEscortPointInfo* escortPoint = dynamic_cast<VEscortPointInfo*>(m_pCurPointInfo);
    // if (!escortPoint) return 0;

    // TODO: 调用VEventObjectInfo::Collision检查碰撞
    // if (!VEventObjectInfo::Collision(escortPoint, vPos)) return 0;

    // IDA逻辑:
    // 1. 检查当前航点信息是否存在
    // 2. 尝试将VWayPointInfo转换为VEscortPointInfo
    // 3. 调用Collision检查位置是否在碰撞体内
    // 4. 如果有下一个链接返回1，否则返回2

    return 0;
}

// ============================================================================
// CWayPoint::IsChangeWayPoint - Check if should change waypoint
// IDA @ 0x140199e00
// ============================================================================
bool CWayPoint::IsChangeWayPoint() {
    if (!m_bChangedID) {
        return false;
    }
    m_bChangedID = false;
    return true;
}

// ============================================================================
// CWayPoint::GetDestPosition - Get destination position
// IDA @ 0x140199BD0
// ============================================================================
hkvVec3 CWayPoint::GetDestPosition() {
    // IDA 0x140199BD0 精确还原:
    // hkvVec3 *__fastcall CWayPoint::GetDestPosition(CWayPoint *this, hkvVec3 *result)
    // {
    //   if ( this->m_pCurPointInfo )
    //     VEventObjectInfo::GetCenter(this->m_pCurPointInfo, result);
    //   else
    //     hkvVec3::ZeroVector(result);
    //   return result;
    // }

    if (m_pCurPointInfo) {
        // Get center position from current waypoint info
        // TODO: VEventObjectInfo::GetCenter(m_pCurPointInfo, result);
        // For now, return zero vector
        return hkvVec3(0.0f, 0.0f, 0.0f);
    } else {
        return hkvVec3(0.0f, 0.0f, 0.0f);
    }
}
