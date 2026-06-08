/**
 * @file XActor.cpp
 * @brief XActor implementation - Vision Engine Actor base class
 *
 * PDB addresses:
 * - XActor::Reset       @ 0x1408F10D0
 * - XActor::IsStatus    @ 0x140048FD0
 * - XActor::SetStatus   @ 0x140276490
 * - XActor::ClearStatus @ 0x1402764B0
 * - XActor::GetType     @ 0x140016F10
 * - XActor::IsPlayer    @ 0x140049380
 * - XActor::IsMonster   @ 0x1401AD040
 * - XActor::IsNPC       @ 0x1402A4FE0
 * - XActor::GetWorldID  @ 0x1400855A0
 */

#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"  // For STPosInfo definition
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"

/**
 * @brief Reset actor state
 *
 * PDB: 0x1408F10D0
 * Resets actor to default state, clearing status and sync status.
 */
void XActor::Reset() {
    m_dwStatus = 0;
    m_nSyncStatus = 0;
    m_pPosInfo = nullptr;
    m_pArea = nullptr;
}

/**
 * @brief Check if specific status flag is set
 *
 * PDB: 0x140048FD0
 *
 * @param dwStatusFlag Status flag to check
 * @return true if flag is set
 */
bool XActor::IsStatus(std::uint32_t dwStatusFlag) const {
    return (m_dwStatus & dwStatusFlag) != 0;
}

/**
 * @brief Set status flag
 *
 * PDB: 0x140276490
 *
 * @param dwStatusFlag Status flag to set
 */
void XActor::SetStatus(std::uint32_t dwStatusFlag) {
    m_dwStatus |= dwStatusFlag;
}

/**
 * @brief Clear status flag
 *
 * PDB: 0x1402764B0
 *
 * @param dwStatusFlag Status flag to clear
 */
void XActor::ClearStatus(std::uint32_t dwStatusFlag) {
    m_dwStatus &= ~dwStatusFlag;
}

/**
 * @brief Check if actor is in die status
 *
 * IDA: 0x140364500
 * Returns true if status 2 or 4 is set (dead or dying states)
 *
 * @return true if in die status
 */
bool XActor::IsDieStatus() const {
    return IsStatus(2u) || IsStatus(4u);
}

/**
 * @brief Get actor type
 *
 * PDB: 0x140016F10
 *
 * @return E_ACTOR_TYPE Actor type enumeration
 */
E_ACTOR_TYPE XActor::GetType() const {
    return m_eActorType;
}

/**
 * @brief Check if actor is a player
 *
 * PDB: 0x140049380
 *
 * @return true if actor type is player
 */
bool XActor::IsPlayer() const {
    return m_eActorType == eActorUser;
}

/**
 * @brief Check if actor is a monster
 *
 * PDB: 0x1401AD040
 *
 * @return true if actor type is monster
 */
bool XActor::IsMonster() const {
    return m_eActorType == eActorMonster;
}

/**
 * @brief Check if actor is an NPC
 *
 * PDB: 0x1402A4FE0
 *
 * @return true if actor type is NPC
 */
bool XActor::IsNPC() const {
    return m_eActorType == eActorNPC;
}

/**
 * @brief Get world ID from position info
 *
 * PDB: 0x1400855A0
 * IDA: return this->m_pPosInfo->sWorldID;
 *
 * @return World ID (short)
 */
std::int16_t XActor::GetWorldID() const {
    if (m_pPosInfo) {
        return m_pPosInfo->sWorldID;
    }
    return 0;
}

/**
 * @brief Get map instance ID from position info
 *
 * IDA: ?GetMapInsID@XActor@@QEAA?ATUXMapID@@XZ (0x140085E10)
 * return this->m_pPosInfo->uxMapID;
 *
 * @return Map instance ID (UXMapID)
 */
UXMapID XActor::GetMapInsID() const {
    if (m_pPosInfo) {
        return m_pPosInfo->uxMapID;
    }
    return UXMapID{};
}

/**
 * @brief Set world ID in position info
 *
 * IDA: ?SetWorldID@XActor@@QEAAXF@Z (0x1401897E0)
 * this->m_pPosInfo->sWorldID = sWorldID;
 *
 * @param sWorldID World ID to set
 */
void XActor::SetWorldID(std::int16_t sWorldID) {
    if (m_pPosInfo) {
        m_pPosInfo->sWorldID = sWorldID;
    }
}

/**
 * @brief Set map instance ID in position info
 *
 * IDA: ?SetMapInsID@XActor@@QEAAXTUXMapID@@@Z (0x140189800)
 * Copies uxMapID to m_pPosInfo->uxMapID
 *
 * @param uxMapID Map instance ID to set
 */
void XActor::SetMapInsID(UXMapID uxMapID) {
    if (m_pPosInfo) {
        m_pPosInfo->uxMapID = uxMapID;
    }
}

/**
 * @brief Set position info (position and rotation)
 *
 * IDA: ?SetPosInfo@XActor@@UEAAXUXVec3@@M@Z (0x140189830)
 * XVec3::operator=(&this->m_pPosInfo->vPos, vPos);
 * this->m_pPosInfo->fRot = fRot;
 *
 * @param vPos Position vector
 * @param fRot Rotation angle
 */
void XActor::SetPosInfo(XVec3 vPos, float fRot) {
    if (m_pPosInfo) {
        m_pPosInfo->vPos = vPos;
        m_pPosInfo->fRot = fRot;
    }
}

// ============================================================================
// Network Functions
// ============================================================================

/**
 * @brief Check if actor can receive network sync
 *
 * IDA: XActor::CanSync @ 0x140103ED0
 * return this->m_nSyncStatus == 0;
 *
 * @return true if sync status is 0 (can sync)
 */
bool XActor::CanSync() const {
    return m_nSyncStatus == 0;
}

/**
 * @brief Send packet to this actor
 *
 * IDA: XActor::BridgeSend - base class stub (no implementation in XActor)
 * CUser overrides this at 0x1406E8B50 to actually send packets.
 * Base implementation returns false.
 *
 * @param packet Packet to send
 * @return true if sent successfully
 */
bool XActor::BridgeSend(XSendPacket& packet) {
    // Base class stub - CUser overrides this to actually send
    (void)packet;
    return false;
}

/**
 * @brief Send packet to this actor after loading
 *
 * IDA: XActor::BridgeSend_AfterLoading - base class stub (no implementation in XActor)
 * CUser overrides this at 0x1406E8D00 to actually send packets after loading.
 * Base implementation returns false.
 *
 * @param packet Packet to send
 * @return true if sent successfully
 */
bool XActor::BridgeSend_AfterLoading(XSendPacket& packet) {
    // Base class stub - CUser overrides this to actually send
    (void)packet;
    return false;
}
