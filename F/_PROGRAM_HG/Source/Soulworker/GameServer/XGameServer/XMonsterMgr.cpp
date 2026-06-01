// XMonsterMgr.cpp
// Monster manager class for GameServer
// Reconstructed from IDA decompilation

#include "XMonsterMgr.h"
#include "Monster.h"

// ============================================================================
// Constructor / Destructor
// ============================================================================

// IDA 0x140364EB0 - Constructor
XMonsterMgr::XMonsterMgr()
    : m_nMaxSize(0)
    , m_nNextSessionID(1000)
{
    // Initialize the read-write lock
    // CFSRWLock constructor is called here in IDA
}

// IDA 0x140364F30 - Destructor
XMonsterMgr::~XMonsterMgr()
{
    ClearAll();
}

// ============================================================================
// Initialization
// ============================================================================

// IDA 0x140364F60 - Initialize monster pool
bool XMonsterMgr::Init(int nMaxSize)
{
    // IDA shows:
    // TXPool<IXObject>::TXCreator<CMonster> xCreator;
    // if (!TXPool<IXObject>::Init(&this->m_xPool, nMaxSize, &xCreator, 1))
    //     return false;

    m_nMaxSize = nMaxSize;
    m_xObjectMap.clear();
    m_xStorage.clear();

    if (nMaxSize <= 0) {
        return false;
    }

    // std::list doesn't have reserve, so we just clear it

    // TODO: 汇编还原 - Pre-allocate monsters by iterating TB_MONSTER table
    // This is done in the original to warm up the object pool
    // The original code iterates through XResourceMgr::GetTB_MONSTER map
    // and creates/deletes one monster of each type to initialize the pool

    return true;
}

// ============================================================================
// Monster Creation / Deletion
// ============================================================================

// Get next session ID (similar to IXObjectMgr::GetSessionID)
int XMonsterMgr::GetNextSessionID()
{
    if (++m_nNextSessionID > 0x1FFFFFFF) {
        m_nNextSessionID = 1000;
    }
    return m_nNextSessionID;
}

// IDA 0x140365170 - Create a new monster
CMonster* XMonsterMgr::Create(UXMapID uxMapInsID, unsigned int nMonsterID, XVec3* vPos, float fRot)
{
    // TODO: 汇编还原 - Acquire write lock for thread safety
    // CFAutoSlimWriteLock _autolock(&m_rwLock);

    // Check if we've reached max capacity
    if (m_nMaxSize > 0 && static_cast<int>(m_xObjectMap.size()) >= m_nMaxSize) {
        // TODO: LogHelper::LogError("game.contents", "Create error - No Enough pool [ MonsterID:%d ]", nMonsterID);
        return nullptr;
    }

    // Create monster
    CMonster* pMonster = new CMonster();
    if (!pMonster) {
        return nullptr;
    }

    // Get session ID
    int nSessionID = GetNextSessionID();

    // Add to map
    m_xObjectMap[nSessionID] = pMonster;

    // Store in storage list for memory management
    m_xStorage.push_back(std::unique_ptr<CMonster>(pMonster));

    // TODO: 汇编还原 - Get monster table data
    // XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    // TB_MONSTER* ptblMonster = pGameServer->m_xResourceMgr.GetTB_MONSTER(nMonsterID);
    // if (!ptblMonster) {
    //     Delete(pMonster);
    //     return nullptr;
    // }

    // TODO: 汇编还原 - Initialize monster with table data
    // pMonster->Init();
    // pMonster->SetTablePtr(ptblMonster);
    // pMonster->SetMapInsID(uxMapInsID);
    // pMonster->InitialObjectInfo(nSessionID, nMonsterID, vPos, fRot);
    // pMonster->CalcAbility();
    // pMonster->ChangeAiScript(ptblMonster->Monster_AI_ID);

    (void)uxMapInsID;  // Suppress unused parameter warning
    (void)nMonsterID;
    (void)vPos;
    (void)fRot;

    return pMonster;
}

// IDA 0x140365110 - Clear all monsters
void XMonsterMgr::ClearAll()
{
    // IDA decompilation:
    // TXMonsterDeletor<CMonster> xDeletor;
    // TXObjectMgr<CMonster>::Clear(&xDeletor);

    // Clear object map
    m_xObjectMap.clear();

    // Clear storage (this will delete all monsters)
    m_xStorage.clear();
}

// Delete monster from pool
void XMonsterMgr::Delete(CMonster* pMonster)
{
    if (!pMonster) {
        return;
    }

    // Find and remove from map
    for (auto it = m_xObjectMap.begin(); it != m_xObjectMap.end(); ++it) {
        if (it->second == pMonster) {
            m_xObjectMap.erase(it);
            break;
        }
    }

    // Remove from storage
    for (auto it = m_xStorage.begin(); it != m_xStorage.end(); ++it) {
        if (it->get() == pMonster) {
            m_xStorage.erase(it);
            break;
        }
    }
}

// Find monster by session ID
CMonster* XMonsterMgr::Find(int nSessionID)
{
    auto it = m_xObjectMap.find(nSessionID);
    if (it == m_xObjectMap.end()) {
        return nullptr;
    }
    return it->second;
}
