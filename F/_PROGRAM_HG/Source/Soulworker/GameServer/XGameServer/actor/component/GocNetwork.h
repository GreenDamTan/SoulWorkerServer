#pragma once

#include "GOComponent.h"
#include "Soulworker/GameServer/XCore/XArea/IXArea.h"
#include <cstdint>
#include <vector>

// Forward declarations
class XActor;
class XSendPacket;
class CMover;
class XArea;

/**
 * @brief CGocNetwork - Game Object Component for network operations
 *
 * IDA: CGocNetwork inherits from GOComponent (16 bytes)
 * This component provides network sending and broadcasting functionality
 * for actors in the game world.
 */
class CGocNetwork : public GOComponent {
public:
    // IDA: ??0CGocNetwork@@QEAA@XZ (0x140103820)
    CGocNetwork();

    // IDA: ??1CGocNetwork@@UEAA@XZ (0x140103890)
    virtual ~CGocNetwork();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Family ID for GOC component indexing
    static int GetFamilyID() { return 1; }

    // === Static Send Functions ===

    /**
     * @brief Send packet to a single actor
     * IDA: ?Send@CGocNetwork@@SA_NPEAVXActor@@AEAVXSendPacket@@@Z (0x1401038C0)
     * @param pActor Target actor (must not be null and must be sync-able)
     * @param packet Packet to send
     * @return true if sent successfully
     */
    static bool Send(XActor* pActor, XSendPacket& packet);

    /**
     * @brief Send packet to a single actor after loading
     * IDA: ?SendAfterLoading@CGocNetwork@@SA_NPEAVXActor@@AEAVXSendPacket@@@Z (0x140103910)
     * @param pActor Target actor (must not be null and must be sync-able)
     * @param packet Packet to send
     * @return true if sent successfully
     */
    static bool SendAfterLoading(XActor* pActor, XSendPacket& packet);

    /**
     * @brief Send packet to a list of movers
     * IDA: ?Send@CGocNetwork@@SAXAEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@AEAVXSendPacket@@@Z (0x140103960)
     * @param gobList Vector of CMover pointers
     * @param packet Packet to send
     */
    static void Send(std::vector<CMover*>& gobList, XSendPacket& packet);

    /**
     * @brief Send packet to a list of movers, excluding one actor
     * IDA: ?Send@CGocNetwork@@SAXAEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@AEAVXSendPacket@@PEAVXActor@@@Z (0x140103A40)
     * @param gobList Vector of CMover pointers
     * @param packet Packet to send
     * @param pExceptActor Actor to exclude from broadcast
     */
    static void Send(std::vector<CMover*>& gobList, XSendPacket& packet, XActor* pExceptActor);

    /**
     * @brief Send packet to a list of movers after loading, excluding one actor
     * IDA: ?SendAfterLoading@CGocNetwork@@SAXAEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@AEAVXSendPacket@@PEAVXActor@@@Z (0x140103B30)
     * @param gobList Vector of CMover pointers
     * @param packet Packet to send
     * @param pExceptActor Actor to exclude from broadcast
     */
    static void SendAfterLoading(std::vector<CMover*>& gobList, XSendPacket& packet, XActor* pExceptActor);

    /**
     * @brief Broadcast packet to nearby actors
     * IDA: ?BroadcastNearby@CGocNetwork@@SA_NPEAVXActor@@0AEAVXSendPacket@@@Z (0x140103C20)
     * @param pActor Source actor (used to get area and scan grid)
     * @param pExceptActor Actor to exclude from broadcast (can be null)
     * @param packet Packet to broadcast
     * @return true if broadcast was initiated
     */
    static bool BroadcastNearby(XActor* pActor, XActor* pExceptActor, XSendPacket& packet);

    /**
     * @brief Send broadcast via area
     * IDA: ?SendBroadCast@CGocNetwork@@SAXPEAVCMover@@AEAVXSendPacket@@W4E_BROADCAST_TYPE@IXArea@@@Z (0x140103CD0)
     * @param pObject Source mover
     * @param packet Packet to broadcast
     * @param eBroadCastType Broadcast type
     */
    static void SendBroadCast(CMover* pObject, XSendPacket& packet, E_BROADCAST_TYPE eBroadCastType);

    /**
     * @brief Send broadcast via area after loading
     * IDA: ?SendBroadCastAfterLoading@CGocNetwork@@SAXPEAVCMover@@AEAVXSendPacket@@W4E_BROADCAST_TYPE@IXArea@@@Z (0x140103D60)
     * @param pObject Source mover
     * @param packet Packet to broadcast
     * @param eBroadCastType Broadcast type
     */
    static void SendBroadCastAfterLoading(CMover* pObject, XSendPacket& packet, E_BROADCAST_TYPE eBroadCastType);

    /**
     * @brief Send error message to a mover (casts to CUser)
     * IDA: ?SendErrorMessage@CGocNetwork@@SA_NPEAVCMover@@EEG@Z (0x140103DF0)
     * @param pObject Target mover (must be CUser)
     * @param ucMainCmd Main command
     * @param ucSubCmd Sub command
     * @param xErrorCode Error code
     * @return true if pObject is not null
     */
    static bool SendErrorMessage(CMover* pObject, std::uint8_t ucMainCmd, std::uint8_t ucSubCmd, std::uint16_t xErrorCode);
};
