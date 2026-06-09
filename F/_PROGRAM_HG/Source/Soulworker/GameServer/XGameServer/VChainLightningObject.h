// VChainLightningObject.h
// VChainLightningObject - Chain lightning effect object for skills
// IDA decompilation from GameServer.exe
//
// Functions:
// - VChainLightningObject::Init (0x14070b370)
// - VChainLightningObject::GetTypeId (0x14070b0e0)
// - VChainLightningObject::SetFinish (0x14070e530)
// - VChainLightningObject::CreateObject (0x14070b080)

#pragma once

#include <cstdint>
#include <vector>

// Forward declarations
class VType;
class XArea;
struct SSkillInfo;
struct ST_ACTION_EVENT;

// VChainBase_cl - Base class for chain objects (Vision Engine)
class VChainBase_cl {
public:
    virtual ~VChainBase_cl() = default;
    virtual void Init() {}
};

// VChainLightningObject - Vision Engine chain lightning object
// Used for chain lightning skill effects
// IDA: object size = 0x518 bytes (from CreateObject)
class VChainLightningObject : public VChainBase_cl {
public:
    // === Static Factory ===

    // CreateObject - Static factory method to create new instance
    // IDA: ?CreateObject@VChainLightningObject@@SAPEAVVTypedObject@@XZ @ 0x14070B080
    static VChainLightningObject* CreateObject();

    // === Constructor ===
    VChainLightningObject();

    // === Destructor ===
    virtual ~VChainLightningObject();

    // === Virtual Methods ===

    // GetTypeId - Get type ID for RTTI
    // IDA: ?GetTypeId@VChainLightningObject@@UEBAPEAUVType@@XZ @ 0x14070B0E0
    virtual VType* GetTypeId() const;

    // Init - Initialize the chain lightning object
    // IDA: ?Init@VChainLightningObject@@UEAAXXZ @ 0x14070B370
    virtual void Init() override;

    // SetFinish - Sets the chain lightning to finished state
    // IDA: ?SetFinish@VChainLightningObject@@UEAAXXZ @ 0x14070e530
    virtual void SetFinish();

    // === Accessors ===
    bool IsActivated() const { return m_bActivated != 0; }
    float GetChainLife() const { return m_fChainLife; }
    void SetChainLife(float fLife) { m_fChainLife = fLife; }
    void Activate() { m_bActivated = 1; }
    void Deactivate() { m_bActivated = 0; }

    XArea* GetArea() const { return m_pArea; }
    void SetArea(XArea* pArea) { m_pArea = pArea; }

    void SetSkillInfo(SSkillInfo* pSkillInfo) { m_pSkillInfo = pSkillInfo; }
    SSkillInfo* GetSkillInfo() const { return m_pSkillInfo; }
    int GetSessionID() const { return m_nSessionID; }
    void SetSessionID(int nSessionID) { m_nSessionID = nSessionID; }

    int GetCurChainCount() const { return m_iCurChainCount; }
    void SetCurChainCount(int count) { m_iCurChainCount = count; }

protected:
    // === Member Variables (from IDA) ===
    ST_ACTION_EVENT* m_pActionEvent;       // Action event pointer
    int m_iCurChainCount;                  // Current chain count
    std::vector<uint64_t> m_vecChainEffect; // Chain effect vector
    int m_bActivated;                      // Activation flag (0 = inactive, non-zero = active)
    XArea* m_pArea;                        // Area pointer
    float m_fChainLife;                    // Chain lightning lifetime
    SSkillInfo* m_pSkillInfo;              // Chain skill metadata
    int m_nSessionID;                      // Object session id

private:
    // Static type info for RTTI
    static VType classVChainLightningObject;
};
