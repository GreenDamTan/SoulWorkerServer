// XAkashicResMgr.h
// Akashic Resource Manager Stub

#pragma once

#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include <cstdint>

// Forward declaration
struct ActionTrigger;

// XAkashicResMgr - Akashic 资源管理器存根
class XAkashicResMgr {
public:
    XAkashicResMgr() = default;
    ~XAkashicResMgr() = default;

    void Clear() {}
    static void LoadAll(XAkashicResMgr* pMgr) {
        // TODO: 对齐 IDA 实现
    }

    // IDA: ?GetActionDataList@XAkashicResMgr@@QEAAPEAV?$VArray@PEAVActionTrigger@@AEBQEAH@Z
    // Returns action data list for Akashic skills
    // param nType: 1 for Akashic action data
    // param strKey: key in format "GroupID:SeqName"
    VArray<ActionTrigger*>* GetActionDataList(int nType, const VString& strKey) {
        // TODO: Implement from IDA
        (void)nType;
        (void)strKey;
        return nullptr;
    }
};
