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
    return m_eActorType == E_ACTOR_TYPE_PLAYER;
}

/**
 * @brief Check if actor is a monster
 *
 * PDB: 0x1401AD040
 * 
 * @return true if actor type is monster
 */
bool XActor::IsMonster() const {
    return m_eActorType == E_ACTOR_TYPE_MONSTER;
}

/**
 * @brief Check if actor is an NPC
 *
 * PDB: 0x1402A4FE0
 * 
 * @return true if actor type is NPC
 */
bool XActor::IsNPC() const {
    return m_eActorType == E_ACTOR_TYPE_NPC;
}
