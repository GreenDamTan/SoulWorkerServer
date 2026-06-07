// AkashicObject.h
// CAkashicObject - Akashic game item object class
// IDA decompilation from GameServer.exe
//
// Functions:
// - CAkashicObject::CAkashicObject (0x140199e70)
// - CAkashicObject::~CAkashicObject (0x14019a0a0)
// - CAkashicObject::InitComponant (0x14019a1e0)
// - CAkashicObject::SetAkashicTableRef (0x14019c390)
// - CAkashicObject::IsComboAkashic (0x14019b6b0)
// - CAkashicObject::CreateObject (0x14019b700)
// - CAkashicObject::GetTypeId (0x14019b760)
// - CAkashicObject::IsCanHit (0x14019b8f0)

#pragma once

#include "MoverEx.h"
#include <cstdint>

// Forward declarations
struct TB_AKASHIC_RECORDS;
class CMySkillList;
class CGocNpcAttribute;
class VType;

// ============================================================================
// CAkashicObject - Akashic game item (inherits from CMoverEx)
// Akashic items are special collectible/power-up objects in SoulWorker
// ============================================================================
class CAkashicObject : public CMoverEx {
public:
    // === Static Factory ===

    // CreateObject - Static factory method to create new instance
    // IDA: ?CreateObject@CAkashicObject@@SAPEAVVTypedObject@@XZ @ 0x14019B700
    static CAkashicObject* CreateObject();

    // === Constructor ===
    // IDA: ??0CAkashicObject@@QEAA@XZ @ 0x140199E70
    CAkashicObject();

    // === Destructor ===
    // IDA: ??1CAkashicObject@@UEAA@XZ @ 0x14019A0A0
    virtual ~CAkashicObject();

    // === Virtual Methods ===

    // GetTypeId - Get type ID for RTTI
    // IDA: ?GetTypeId@CAkashicObject@@UEBAPEAUVType@@XZ @ 0x14019B760
    virtual VType* GetTypeId() const override;

    // IsCanHit - Check if can be hit (always returns 0 for Akashic objects)
    // IDA: ?IsCanHit@CAkashicObject@@UEAAHHH@Z @ 0x14019B8F0
    virtual __int64 IsCanHit(int nDownAttack, int bNormalAttack);

    // === Initialization ===

    // InitComponant - Initialize components (creates CGocNpcAttribute)
    // IDA: ?InitComponant@CAkashicObject@@QEAAXXZ @ 0x14019A1E0
    void InitComponant();

    // === Table Reference ===

    // SetAkashicTableRef - Set Akashic table reference
    // IDA: ?SetAkashicTableRef@CAkashicObject@@QEAAXPEAUTB_AKASHIC_RECORDS@@@Z @ 0x14019C390
    void SetAkashicTableRef(TB_AKASHIC_RECORDS* pTableRef);

    // === Type Check ===

    // IsComboAkashic - Check if this is a combo-type Akashic
    // IDA: ?IsComboAkashic@CAkashicObject@@UEAAHXZ @ 0x14019B6B0
    virtual bool IsComboAkashic();

    // === Skill/Damage Processing ===

    // ClearBuffProcess - Clear buff process and apply damage
    // IDA: ?ClearBuffProcess@CAkashicObject@@UEAAHHPEAVAttackJudgmentTrigger@@VhkvVec3@@@Z @ 0x14019AD00
    virtual __int64 ClearBuffProcess(int nSkillID, void* pTrigger, void* vCurPos);

    // ApplySkillDamageFrame - Apply skill damage frame to targets
    // IDA: ?ApplySkillDamageFrame@CAkashicObject@@UEAAXHFEAEAVhkvVec3@@MHE_N@Z @ 0x14019B1C0
    virtual void ApplySkillDamageFrame(
        unsigned int nSkillID,
        short nTriggerIdx,
        unsigned char byAttackTargetCnt,
        void* vPos,
        float fAttackRot,
        int nContinueAttack,
        unsigned char byDamageType,
        bool bPenetrate);

    // ClearMotion - Clear motion state
    // IDA: ?ClearMotion@CAkashicObject@@UEAAXXZ @ 0x14019B540
    virtual void ClearMotion();

    // === Accessors ===
    TB_AKASHIC_RECORDS* GetAkashicTableRef() const { return m_pAkashicTableRef; }
    CMySkillList* GetSkillMgr() const { return m_pSkillMgr; }

protected:
    // === Member Variables ===
    TB_AKASHIC_RECORDS* m_pAkashicTableRef;  // Akashic table reference
    CMySkillList* m_pSkillMgr;               // Skill manager
    unsigned int m_dwParentID;               // Parent ID (-1 = no parent)

private:
    // Static type info for RTTI
    static VType classCAkashicObject;
};
