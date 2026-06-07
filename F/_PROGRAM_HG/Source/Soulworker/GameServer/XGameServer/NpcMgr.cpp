// NpcMgr.cpp
// XNpcMgr - NPC Manager for GameServer
// IDA decompilation from GameServer.exe

#include "NpcMgr.h"
#include "Npc.h"

// IDA: ??0XNpcMgr@@QEAA@XZ @ 0x1403A4300
XNpcMgr::XNpcMgr() : TXObjectMgr<CNpc>() {
    // Set vtable pointer
    // Initialize read-write lock
}

// IDA: ??1XNpcMgr@@UEAA@XZ @ 0x1403A4380
XNpcMgr::~XNpcMgr() {
    // Call base class destructor
}

// IDA: ?Init@XNpcMgr@@UEAA_NH@Z @ 0x1403A43B0
bool XNpcMgr::Init(int nMaxSize) {
    // Create NPC creator
    TXNpcCreator<CNpc> xCreator;

    // Initialize pool with creator
    if (!m_xPool.Init(nMaxSize, &xCreator, nullptr)) {
        return false;
    }

    m_nMaxSize = nMaxSize;

    // Iterate existing objects (for reinitialization)
    for (auto it = m_xObjectMap.Begin(); it; m_xObjectMap.GetNext(&it)) {
        CNpc* pNpc = dynamic_cast<CNpc*>(m_xObjectMap.GetValueAt(it));
        // Process existing NPC if needed
    }

    return true;
}

// IDA: ?ClearAll@XNpcMgr@@QEAAXXZ @ 0x1403A44B0
void XNpcMgr::ClearAll() {
    // Create NPC deletor
    TXNpcDeletor<CNpc> xDeletor;

    // Clear with deletor
    Clear(&xDeletor);
}
