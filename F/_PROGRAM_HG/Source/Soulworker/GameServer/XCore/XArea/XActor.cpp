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
 */

#include "Soulworker/GameServer/XCore/XArea/XActor.h"
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

// ============================================================================
// Network Functions
// ============================================================================

/**
 * @brief Check if actor can receive network sync
 *
 * IDA: XActor::CanSync
 * Base implementation returns true. Override in CUser to check connection state.
 *
 * @return true if actor can sync
 */
bool XActor::CanSync() const {
    // Base implementation - always return true
    // CUser overrides this to check connection state
    return true;
}

/**
 * @brief Send packet to this actor
 *
 * IDA: XActor::BridgeSend
 * Base implementation does nothing. Override in CUser to actually send.
 *
 * @param packet Packet to send
 * @return true if sent successfully
 */
bool XActor::BridgeSend(XSendPacket& packet) {
    // TODO: 汇编还原 - XActor::BridgeSend
    // Base implementation does nothing - CUser overrides this
    (void)packet;
    return false;
}

/**
 * @brief Send packet to this actor after loading
 *
 * IDA: XActor::BridgeSend_AfterLoading
 * Base implementation does nothing. Override in CUser to actually send.
 *
 * @param packet Packet to send
 * @return true if sent successfully
 */
bool XActor::BridgeSend_AfterLoading(XSendPacket& packet) {
    // TODO: 汇编还原 - XActor::BridgeSend_AfterLoading
    // Base implementation does nothing - CUser overrides this
    (void)packet;
    return false;
}
