// VResourceManager.h
// VResourceManager - Vision Engine resource manager
// IDA decompilation from GameServer.exe
//
// Functions:
// - VResourceManager::OnTickFunction (0x140188db0)

#pragma once

// Forward declarations
class VActionResourceManager;

// ============================================================================
// VResourceManager - Base resource manager class
// Manages loading/unloading of game resources
// ============================================================================
class VResourceManager {
public:
    // OnTickFunction - Called every frame to update resources
    // IDA: ?OnTickFunction@VResourceManager@@UEAAXM@Z @ 0x140188DB0
    virtual void OnTickFunction(float dtime) {
        // IDA: Empty function - just returns
        // This is a placeholder that derived classes can override
    }

    virtual ~VResourceManager() = default;
};
