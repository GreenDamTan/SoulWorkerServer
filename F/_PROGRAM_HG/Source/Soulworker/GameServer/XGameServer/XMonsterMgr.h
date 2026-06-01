// XMonsterMgr.h
// Monster manager class for GameServer
// Reconstructed from IDA decompilation

#pragma once

#include "Soulworker/GameServer/XCore/XServer/IXObject.h"
#include "Soulworker/GameServer/XCore/XServer/CFSRWLock.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include <cstdint>
#include <unordered_map>
#include <list>
#include <memory>

// Forward declarations
class CMonster;
class TB_MONSTER;
struct XVec3;
class XArea;

// ============================================================================
// XMonsterMgr - Monster object pool manager
// IDA: inherits from TXObjectMgr<CMonster>
// IDA Constructor @ 0x140364EB0
// IDA Destructor @ 0x140364F30
// ============================================================================

class XMonsterMgr {
public:
    XMonsterMgr();
    virtual ~XMonsterMgr();

    // IDA @ 0x140364F60 - Initialize monster pool with pre-allocation
    bool Init(int nMaxSize);

    // IDA @ 0x140365170 - Create a new monster
    CMonster* Create(UXMapID uxMapInsID, unsigned int nMonsterID, XVec3* vPos, float fRot);

    // IDA @ 0x140365110 - Clear all monsters from pool
    void ClearAll();

    // Delete monster from pool
    void Delete(CMonster* pMonster);

    // Find monster by session ID
    CMonster* Find(int nSessionID);

    // Get monster count
    size_t GetCount() const { return m_xObjectMap.size(); }

private:
    // Read-write lock for thread-safe access
    CFSRWLock m_rwLock;
    int m_nMaxSize;
    int m_nNextSessionID;

    // Object storage
    std::unordered_map<int, CMonster*> m_xObjectMap;
    std::list<std::unique_ptr<CMonster>> m_xStorage;

    // Get next session ID
    int GetNextSessionID();
};
