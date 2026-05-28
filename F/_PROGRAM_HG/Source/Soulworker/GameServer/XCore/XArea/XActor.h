#pragma once

#include "Soulworker/GameServer/XCore/XServer/IXObject.h"
#include <cstdint>

// 前置声明
class XArea;
struct STPosInfo;
// UXActorID is defined as union in PSCommon.h

// E_ACTOR_TYPE - Actor 类型枚举
enum E_ACTOR_TYPE : std::int32_t {
    E_ACTOR_TYPE_NONE = 0,
    E_ACTOR_TYPE_PLAYER = 1,
    E_ACTOR_TYPE_MONSTER = 2,
    E_ACTOR_TYPE_NPC = 3,
    E_ACTOR_TYPE_ITEM = 4,
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
        , m_eActorType(E_ACTOR_TYPE_NONE)
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

    // 重置
    void Reset();  // PDB: 0x1408F10D0

    // 区域
    XArea* GetArea() const { return m_pArea; }
    void SetArea(XArea* pArea) { m_pArea = pArea; }

    // 状态
    std::uint32_t GetStatus() const { return m_dwStatus; }
    void SetStatusValue(std::uint32_t dwStatus) { m_dwStatus = dwStatus; }  // Direct assignment
    bool IsStatus(std::uint32_t dwStatusFlag) const;  // PDB: 0x140048FD0 - Check flag
    void SetStatus(std::uint32_t dwStatusFlag);       // PDB: 0x140276490 - Set flag (OR)
    void ClearStatus(std::uint32_t dwStatusFlag);     // PDB: 0x1402764B0 - Clear flag (AND NOT)

    // 位置信息
    STPosInfo* GetPosInfo() const { return m_pPosInfo; }
    void SetPosInfo(STPosInfo* pPosInfo) { m_pPosInfo = pPosInfo; }

    // Bot 标志
    bool IsBot() const { return m_bIsBot; }
    void SetBot(bool bIsBot) { m_bIsBot = bIsBot; }

    // Origin ID
    std::uint32_t GetOriginID() const { return m_uxOriginID; }
    void SetOriginID(std::uint32_t uxOriginID) { m_uxOriginID = uxOriginID; }

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
