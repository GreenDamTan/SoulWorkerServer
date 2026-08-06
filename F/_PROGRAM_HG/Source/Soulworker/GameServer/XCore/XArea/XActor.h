#pragma once

#include "Soulworker/GameServer/XCore/XServer/IXObject.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"  // For UXMapID, XVec3
#include <cstdint>

// XActor Status Flags (位标志)
// 这些是 XActor::m_dwStatus 的位标志
constexpr std::uint32_t STATUS_ATTACK = 0x00000001u;     // 攻击状态
constexpr std::uint32_t STATUS_DIE = 0x00000002u;      // 死亡状态
constexpr std::uint32_t STATUS_DIEFADE = 0x00000004u;  // 死亡渐隐状态
constexpr std::uint32_t STATUS_FLYING = 0x00000008u;   // 飞行状态
constexpr std::uint32_t STATUS_STUN = 0x00000010u;     // 眩晕状态
constexpr std::uint32_t STATUS_HIT = 0x00000020u;      // 受击状态

// 前置声明
class XArea;
class XSendPacket;
struct STPosInfo;
// UXActorID is defined as union in PSCommon.h

// E_ACTOR_TYPE - Actor 类型枚举 (完整定义)
// Note: This enum is also referenced from BattleZone.h
enum E_ACTOR_TYPE : std::int32_t {
    eActorUser = 0,
    eActorNPC = 1,
    eActorMonster = 2,
    eActorAkashic = 3,
    eActorInteraction = 4,
    eActorVaccum = 5,
};

/**
 * @brief XActor - Vision Engine Actor 基类
 *
 * IDA struct XActor (104 bytes):
 * - field_0: IXObject (64 bytes) - 继承自 IXObject
 * - m_pPosInfo: STPosInfo* (8 bytes)
 * - m_eActorType: E_ACTOR_TYPE (4 bytes)
 * - m_pArea: XArea* (8 bytes)
 * - m_dwStatus: unsigned int (4 bytes)
 * - m_nSyncStatus: int (4 bytes)
 * - m_bIsBot: bool (1 byte)
 * - m_byNation: unsigned __int8 (1 byte)
 * - m_uxOriginID: UXActorID (4 bytes)
 */
class XActor : public IXObject {
public:
    XActor()
        : IXObject()
        , m_pPosInfo(nullptr)
        , m_eActorType(eActorUser)
        , m_pArea(nullptr)
        , m_dwStatus(0)
        , m_nSyncStatus(0)
        , m_bIsBot(false)
        , m_byNation(0)
        , m_uxOriginID(0)
    {}

    virtual ~XActor() = default;

    // Actor 类型
    E_ACTOR_TYPE GetActorType() const { return m_eActorType; }
    void SetActorType(E_ACTOR_TYPE eType) { m_eActorType = eType; }
    E_ACTOR_TYPE GetType() const;  // PDB: 0x140016F10

    // 类型检查
    bool IsPlayer() const;   // PDB: 0x140049380
    bool IsMonster() const;  // PDB: 0x1401AD040
    bool IsNPC() const;      // PDB: 0x1402A4FE0

    // 世界ID
    std::int16_t GetWorldID() const;  // PDB: 0x1400855A0

    // 地图实例ID
    // IDA: ?GetMapInsID@XActor@@QEAA?ATUXMapID@@XZ (0x140085E10)
    // Returns m_pPosInfo->uxMapID
    UXMapID GetMapInsID() const;

    // 重置
    void Reset();  // PDB: 0x1408F10D0

    // 设置信息 (虚函数，基类空实现)
    // IDA: ?SetInfo@XActor@@UEAAXXZ
    virtual void SetInfo() {}

    // IDA: ?SendWorldEventBooster@XActor@@UEAAXK_J@Z (0x140188CF0)
    virtual void SendWorldEventBooster(unsigned long dwBuffID, std::int64_t biEndDate);

    // 区域
    // IDA: ?GetArea@XActor@@UEAAPEAVXArea@@XZ (0x140188D20)
    XArea* GetArea() const { return m_pArea; }
    void SetArea(XArea* pArea) { m_pArea = pArea; }

    // 状态
    std::uint32_t GetStatus() const { return m_dwStatus; }
    void SetStatusValue(std::uint32_t dwStatus) { m_dwStatus = dwStatus; }  // Direct assignment
    bool IsStatus(std::uint32_t dwStatusFlag) const;  // PDB: 0x140048FD0 - Check flag
    void SetStatus(std::uint32_t dwStatusFlag);       // PDB: 0x140276490 - Set flag (OR)
    void ClearStatus(std::uint32_t dwStatusFlag);     // PDB: 0x1402764B0 - Clear flag (AND NOT)
    bool IsDieStatus() const;  // IDA: 0x140364500 - Check if actor is in die status (status 2 or 4)

    // 位置信息
    STPosInfo* GetPosInfo() const { return m_pPosInfo; }
    void SetPosInfo(STPosInfo* pPosInfo) { m_pPosInfo = pPosInfo; }

    // Bot 标志
    bool IsBot() const { return m_bIsBot; }
    void SetBot(bool bIsBot) { m_bIsBot = bIsBot; }

    // Origin ID
    std::uint32_t GetOriginID() const { return m_uxOriginID; }
    void SetOriginID(std::uint32_t uxOriginID) { m_uxOriginID = uxOriginID; }

    // Actor ID - returns UXActorID from derived class
    // IDA: Virtual function, overridden by CUser, CMover, CMonster, etc.
    virtual UXActorID GetActorID() const { return UXActorID(m_uxOriginID); }

    // IsLive - check if actor is alive and connected
    // IDA: Virtual function, overridden by CUser
    virtual bool IsLive() const { return true; }  // Base implementation returns true

    // Nation
    // IDA: ?SetNation@XActor@@UEAAXE@Z (0x140188D00)
    std::uint8_t GetNation() const { return m_byNation; }
    virtual void SetNation(std::uint8_t byNation) { m_byNation = byNation; }

    // World ID setter
    // IDA: ?SetWorldID@XActor@@QEAAXF@Z (0x1401897E0)
    void SetWorldID(std::int16_t sWorldID);

    // Map Instance ID setter
    // IDA: ?SetMapInsID@XActor@@QEAAXTUXMapID@@@Z (0x140189800)
    void SetMapInsID(UXMapID uxMapID);

    // Position info setter
    // IDA: ?SetPosInfo@XActor@@UEAAXUXVec3@@M@Z (0x140189830)
    virtual void SetPosInfo(XVec3 vPos, float fRot);

    // Network sync capability check
    // IDA: XActor::CanSync - checks if actor can receive network sync
    bool CanSync() const;

    // Network send functions (virtual, overridden by CUser)
    // IDA: XActor::BridgeSend - send packet to this actor
    virtual bool BridgeSend(XSendPacket& packet);
    // IDA: XActor::BridgeSend_AfterLoading - send packet after loading
    virtual bool BridgeSend_AfterLoading(XSendPacket& packet);

protected:
    // === IDA 确认的成员变量 ===
    STPosInfo* m_pPosInfo;
    E_ACTOR_TYPE m_eActorType;
    XArea* m_pArea;
    std::uint32_t m_dwStatus;
    int m_nSyncStatus;
    bool m_bIsBot;
    std::uint8_t m_byNation;
    std::uint32_t m_uxOriginID;  // UXActorID (4 bytes)
};

static_assert(sizeof(XActor) == 104, "XActor size mismatch - expected 104 bytes from IDA");
