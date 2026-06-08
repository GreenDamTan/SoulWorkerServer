#pragma once

#include "XProcess.h"
#include "XVec3.h"
#include <map>
#include <list>
#include <memory>

// Forward declarations
class CUser;
class CMonster;
struct XVec3;
struct PS_REQ_PICKUP;
struct PS_RES_PICKUP;
struct PS_DROP_INFOS;
struct PS_TOOL_MAZE_DROP;
struct ST_DROP_ITEM_INFO;
struct TB_DROPRATE_MOB;
struct TB_DROPRATE_LEVEL;
struct TB_DROPRATE_MEMBER;

// Drop item group for managing dropped items with disappear time
class CDropItemGroup
{
public:
    CDropItemGroup();
    ~CDropItemGroup();

    // Drop item disappear time (3 minutes)
    static const DWORD DROP_ITEM_DISAPPEAR_TIME = 180000;

    // Get drop item serial
    __int64 GetSerial() const { return m_xSerial; }
    
    // Get object ID
    int GetObjectID() const { return m_nObjectID; }
    
    // Get item count
    int GetCount() const { return m_nCount; }
    
    // Check if item has disappeared
    bool IsDisappeared(DWORD dwCurrentTime) const;

private:
    __int64 m_xSerial;          // Item serial number
    int m_nObjectID;            // Object ID (item template ID)
    int m_nCount;               // Item count
    DWORD m_dwDropTime;         // Time when item was dropped
};

// Drop process component for handling item drops and pickups
// Inherits from TXProcess<CUser> with CMD 0x14
class CDropProcess : public TXProcess<CUser>
{
public:
    CDropProcess();
    virtual ~CDropProcess();

    // TXProcess interface
    virtual void Init() override;
    virtual bool Clear() override;
    virtual bool Parse(XPacket& packet) override;

    // Pickup functions
    bool ReqItemPickUp(XPacket& packet);
    void SendResPickup(CUser* pUser, __int64 xSerial, int nResult);
    bool IsPickupDropItem(XVec3 vPos, int nObjectID, float& fDist);

    // Drop functions
    bool MakeDropItems(CUser* pUser, int nMonsterID, XVec3& vPos, float fDropRate, bool bParty, int nLevel, int nMemberCount, int nPartyDropRateAdd, float fDropAddValue);
    void MazeDrop(PS_TOOL_MAZE_DROP& stMazeDrop);
    bool MakeDropItemDemensionShutter_PVP(CUser* pUser, int nItemID, __int64 xSerial, XVec3& vPos);

    // Drop update
    void UpdateDropItem();
    void ClearToolDropItem();

    // Drop info management
    void GetDropInfos(std::map<int, std::tr1::shared_ptr<CDropItemGroup>>& mapDropInfo);
    void SetDropInfos(std::map<int, std::tr1::shared_ptr<CDropItemGroup>>& mapDropInfo);
    void GetMonsterDropInfo(PS_DROP_INFOS& stDropInfos);

    // Drop rate calculation
    float GetDropAddValue(int nLevel, int nMemberCount, int nPartyDropRateAdd, int nDropRateType, int nMonsterID, int nMonsterLevel);
    bool IsApplyDropRate(int nDropRateType);

private:
    // Drop item storage
    std::map<int, std::tr1::shared_ptr<CDropItemGroup>> m_mapDropInfo;  // Map of object ID to drop item group
    std::list<ST_DROP_ITEM_INFO> m_listToolDropItem;  // List of tool drop items

    // Helper functions for drop rate calculation
    float CalculateDropRate(int nLevel, int nMonsterLevel, int nMemberCount, int nPartyDropRateAdd, int nDropRateType);
    bool CheckDropRateTable(int nDropRateType, int nMonsterID);
};
