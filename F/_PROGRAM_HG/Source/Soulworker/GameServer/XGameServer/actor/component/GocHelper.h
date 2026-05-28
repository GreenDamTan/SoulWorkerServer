#pragma once

#include "GOComponent.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerFriend.h"
#include <cstdint>
#include <map>
#include <vector>
#include <ctime>

// Forward declarations
class CMonster;
class CUser;
class XMaze;

/**
 * @brief CGocHelper - Game Object Component for helper/assistant system
 *
 * Handles helper NPCs, auto-battle assistants, and helper-related functionality.
 * IDA 地址范围: 0x140091E90 - 0x14009489C
 */
class CGocHelper : public GOComponent {
public:
    // IDA: ??0CGocHelper@@QEAA@XZ (0x140091E90)
    CGocHelper();

    // IDA: ??1CGocHelper@@UEAA@XZ (0x140091F80)
    virtual ~CGocHelper();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // IDA: ?Init@CGocHelper@@QEAAXXZ (0x140091FE0)
    void Init();

    // IDA: ?Clear@CGocHelper@@QEAAXXZ (0x140092000)
    void Clear();

    // IDA: ?Reset@CGocHelper@@QEAAXXZ (0x140092080)
    void Reset();

    // Static helper
    // IDA: ?GetFamilyID@CGocHelper@@SAHXZ (0x140039020)
    static int GetFamilyID();

    // Helper list management
    // IDA: ?SetHelperList@CGocHelper@@QEAA_NAEAUPS_HELPER_LIST_RES@@@Z (0x1400920C0)
    bool SetHelperList(PS_HELPER_LIST_RES& psList);

    // IDA: ?GetHelperList@CGocHelper@@QEAA_NAEAV?$vector@UST_HELPER_INFO@@V?$allocator@UST_HELPER_INFO@@@std@@@std@@_N@Z (0x1400922B0)
    bool GetHelperList(std::vector<ST_HELPER_INFO>& vecHelper, bool isOrder);

    // IDA: ?SendHelperList@CGocHelper@@QEAAXXZ (0x140092560)
    void SendHelperList();

    // Helper summon/release
    // IDA: ?HelperSummon@CGocHelper@@QEAA_NK@Z (0x140093410)
    bool HelperSummon(std::uint32_t dwHelperID);

    // IDA: ?HelperRelease@CGocHelper@@QEAA_NK@Z (0x140094300)
    bool HelperRelease(std::uint32_t dwHelperID);

    // IDA: ?HelperProcess@CGocHelper@@QEAAXAEAUPS_HELPER_SUMMON_REQ@@@Z (0x140093080)
    void HelperProcess(struct PS_HELPER_SUMMON_REQ& psInfo);

    // Helper info access
    bool GetHelperInfo(std::uint32_t dwHelperID, ST_HELPER_INFO& stInfo);

    // IDA: ?SetHelperSummonState@CGocHelper@@QEAAXK_N@Z (0x140092C20)
    void SetHelperSummonState(std::uint32_t dwHelperID, bool bSummon);
    void SetHelperSummonTime();
    CMonster* GetSummonedHelper(std::uint32_t dwHelperID);

    // IDA: ?GetSummonedHelperList@CGocHelper@@QEAAXAEAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@@Z (0x140092B40)
    void GetSummonedHelperList(std::map<std::uint32_t, std::uint32_t>& mapSummonHelperList);

    // IDA: ?CheckSummonHelper@CGocHelper@@QEAA_NK@Z (0x140092D10)
    bool CheckSummonHelper(std::uint32_t dwHelperID);

    // IDA: ?FindHelper@CGocHelper@@QEAA_NK@Z (0x140092AE0)
    bool FindHelper(std::uint32_t dwHelperID);

    // IDA: ?CheckReleaseHelper@CGocHelper@@QEAA_NK@Z
    bool CheckReleaseHelper(std::uint32_t dwHelperID);

    // IDA: ?CheckHelperSummonDelay@CGocHelper@@QEAA_NXZ
    bool CheckHelperSummonDelay();

    // IDA: ?CheckSummonHelperCount@CGocHelper@@QEAA_NXZ
    bool CheckSummonHelperCount();

    // Auto summon
    void SetAutoSummonFlag(std::uint8_t byFlag);
    bool IsAutoSummon() const;
    std::uint8_t GetAutoSummonFlag() const { return m_byAutoSummonFlag; }

    // Helper operations
    bool AddMyHelper(ST_HELPER_INFO& stInfo, bool bUpdate);
    void CalcHelperStatsALL(ST_HELPER_INFO* pstInfo, bool bRecalc);
    void HelperSupportRelease(std::uint32_t dwHelperID);

    // Support info
    void SetMySupportInfo(const ST_HELPER_SUPPORT_INFO& stInfo);
    const ST_HELPER_SUPPORT_INFO& GetMySupportInfo() const { return m_stMySupport; }

    bool IsMySupportRegistered() const { return m_bMySupportResister != 0; }
    std::uint8_t GetMySupportReward() const { return m_byMySupportReward; }

protected:
    // Time of last helper summon
    // IDA: m_tLastSummon (offset after vftable and GOComponent base)
    std::time_t m_tLastSummon;

    // Map of summoned helpers: HelperID -> CMonster*
    // IDA: m_mapSummonedHelper
    std::map<std::uint32_t, CMonster*> m_mapSummonedHelper;

    // Map of helper info: Order -> ST_HELPER_INFO
    // IDA: m_mapHelper
    std::map<std::uint8_t, ST_HELPER_INFO> m_mapHelper;

    // My support info for friend helper system
    // IDA: m_stMySupport
    ST_HELPER_SUPPORT_INFO m_stMySupport;

    // Whether registered as support helper
    // IDA: m_bMySupportResister
    std::uint8_t m_bMySupportResister;

    // Support reward state
    // IDA: m_byMySupportReward
    std::uint8_t m_byMySupportReward;

    // Auto summon flag (default: 1)
    // IDA: m_byAutoSummonFlag
    std::uint8_t m_byAutoSummonFlag;
};
