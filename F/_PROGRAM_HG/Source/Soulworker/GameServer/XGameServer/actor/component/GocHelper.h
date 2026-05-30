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
 * IDA 地址范围: 0x140091DA0 - 0x140098850
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

    // IDA: ?HelperWarp@CGocHelper@@QEAA_NK@Z (0x140094130)
    bool HelperWarp(std::uint32_t dwHelperID);

    // IDA: ?HelperProcess@CGocHelper@@QEAAXAEAUPS_HELPER_SUMMON_REQ@@@Z (0x140093080)
    void HelperProcess(struct PS_HELPER_SUMMON_REQ& psInfo);

    // Helper info access
    // IDA: ?GetHelperInfo@CGocHelper@@QEAA_NKAEAUST_HELPER_INFO@@@Z (0x140092700)
    bool GetHelperInfo(std::uint32_t dwHelperID, ST_HELPER_INFO& stInfo);

    // IDA: ?SetHelperSummonState@CGocHelper@@QEAAXK_N@Z (0x140092C20)
    void SetHelperSummonState(std::uint32_t dwHelperID, bool bSummon);

    // IDA: ?SetHelperSummonTime@CGocHelper@@QEAAXXZ (0x140091DF0)
    void SetHelperSummonTime();

    // IDA: ?GetSummonedHelper@CGocHelper@@QEAAPEAVCMonster@@K@Z (0x140092AD0)
    CMonster* GetSummonedHelper(std::uint32_t dwHelperID);

    // IDA: ?GetSummonedHelperList@CGocHelper@@QEAAXAEAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@@Z (0x140092B40)
    void GetSummonedHelperList(std::map<std::uint32_t, std::uint32_t>& mapSummonHelperList);

    // IDA: ?CheckSummonHelper@CGocHelper@@QEAA_NK@Z (0x140092D10)
    bool CheckSummonHelper(std::uint32_t dwHelperID);

    // IDA: ?FindHelper@CGocHelper@@QEAA_NK@Z (0x1400928A0)
    bool FindHelper(std::uint32_t dwHelperID);

    // IDA: ?CheckReleaseHelper@CGocHelper@@QEAA_NK@Z (0x140092DC0)
    bool CheckReleaseHelper(std::uint32_t dwHelperID);

    // IDA: ?CheckHelperSummonDelay@CGocHelper@@QEAA_NXZ (0x140092CA0)
    bool CheckHelperSummonDelay();

    // IDA: ?CheckSummonHelperCount@CGocHelper@@QEAA_NXZ (0x140092E70)
    bool CheckSummonHelperCount();

    // Auto summon
    // IDA: ?SetAutoSummonFlag@CGocHelper@@QEAAXE@Z (0x140091DA0)
    void SetAutoSummonFlag(std::uint8_t byFlag);

    // IDA: ?IsAutoSummon@CGocHelper@@QEAA_NXZ (0x140091DC0)
    bool IsAutoSummon() const;

    std::uint8_t GetAutoSummonFlag() const { return m_byAutoSummonFlag; }

    // Helper operations
    // IDA: ?AddMyHelper@CGocHelper@@QEAA_NAEAUST_HELPER_INFO@@_N@Z (0x140092900)
    bool AddMyHelper(ST_HELPER_INFO& stInfo, bool bSend);

    // IDA: ?CalcHelperStatsALL@CGocHelper@@QEAAXAEAUST_HELPER_INFO@@H@Z (0x140095280)
    void CalcHelperStatsALL(ST_HELPER_INFO* pstInfo, int nMode);

    // IDA: ?CalcOriginStats@CGocHelper@@QEAAXAEAUST_HELPER_INFO@@@Z (0x140095360)
    void CalcOriginStats(ST_HELPER_INFO& stInfo);

    // IDA: ?CalcEquipItemStats@CGocHelper@@QEAAXAEAUST_HELPER_INFO@@@Z (0x140095500)
    void CalcEquipItemStats(ST_HELPER_INFO& stInfo);

    // IDA: ?CalcFriendItemStats@CGocHelper@@QEAAXAEAUST_HELPER_INFO@@@Z (0x1400958B0)
    void CalcFriendItemStats(ST_HELPER_INFO& stInfo);

    // IDA: ?HelperSupportRelease@CGocHelper@@QEAAXXZ (0x1400966D0)
    void HelperSupportRelease();

    // IDA: ?HelperSupportRelease@CGocHelper@@QEAAXK@Z (0x140096750)
    void HelperSupportRelease(std::uint32_t dwHelperID);

    // IDA: ?SyncSummonedInfo@CGocHelper@@QEAAXXZ (0x140096060)
    void SyncSummonedInfo();

    // IDA: ?CheckAllHelperSummon@CGocHelper@@QEAAXXZ (0x1400948A0)
    void CheckAllHelperSummon();

    // IDA: ?AllHelperRelease@CGocHelper@@QEAAXXZ (0x140094AF0)
    void AllHelperRelease();

    // IDA: ?AllHelperWarp@CGocHelper@@QEAAXXZ (0x140094C10)
    void AllHelperWarp();

    // IDA: ?OtherHelperClear@CGocHelper@@QEAAXXZ (0x140094D30)
    void OtherHelperClear();

    // Support info
    // IDA: ?SetMySupportInfo@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_INFO_RES@@@Z (0x140096660)
    void SetMySupportInfo(const ST_HELPER_SUPPORT_INFO& stInfo);
    // IDA: ?SetMySupportInfo@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_INFO_RES@@@Z (0x140096660)
    void SetMySupportInfo(struct PS_HELPER_SUPPORT_INFO_RES& stMyInfo);
    const ST_HELPER_SUPPORT_INFO& GetMySupportInfo() const { return m_stMySupport; }

    bool IsMySupportRegistered() const { return m_bMySupportResister != 0; }
    std::uint8_t GetMySupportReward() const { return m_byMySupportReward; }

    // IDA: ?GetSupportTypeRate@CGocHelper@@QEAAME@Z (0x140096500)
    float GetSupportTypeRate(std::uint8_t byType);

    // IDA: ?GetSupportTypeValue@CGocHelper@@QEAAME@Z (0x140096540)
    float GetSupportTypeValue(std::uint8_t byType);

    // IDA: ?GetMyHelperStatsALL@CGocHelper@@QEAAXAEAUPS_HELPER_STAT_UPDATE@@@Z (0x140095170)
    void GetMyHelperStatsALL(struct PS_HELPER_STAT_UPDATE& psStat);

    // IDA: ?UnEquipHelperFriendItemStats@CGocHelper@@QEAAXK@Z (0x140095e80)
    void UnEquipHelperFriendItemStats(std::uint32_t dwHelperID);

    // IDA: ?ReqHelperSupportInfo@CGocHelper@@QEAAXXZ (0x1400968f0)
    void ReqHelperSupportInfo();

    // IDA: ?ReqHelperSupportRegister@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_REGISTER_REQ@@@Z (0x140096a10)
    void ReqHelperSupportRegister(struct PS_HELPER_SUPPORT_REGISTER_REQ& psSupport);

    // IDA: ?ReqHelperSupportReward@CGocHelper@@QEAAXXZ (0x140096be0)
    void ReqHelperSupportReward();

    // IDA: ?ReqHelperSupportList@CGocHelper@@QEAAXXZ (0x140096f70)
    void ReqHelperSupportList();

    // IDA: ?GetLastOrderNumber@CGocHelper@@QEAAEXZ (0x14009a780)
    std::uint8_t GetLastOrderNumber();

    // IDA: ?ReqHelperEquip@CGocHelper@@QEAAXAEAUPS_HELPER_EQUIP_REQ@@@Z (0x140097200)
    void ReqHelperEquip(struct PS_HELPER_EQUIP_REQ& psEquip);

    // IDA: ?ReqHelperChangeOrder@CGocHelper@@QEAAXAEAUPS_HELPER_CHANGE_ORDER@@@Z (0x140097eb0)
    void ReqHelperChangeOrder(struct PS_HELPER_CHANGE_ORDER& psOrder);

    // IDA: ?ReqHelperChangeAutoSummon@CGocHelper@@QEAAXAEAUPS_HELPER_CHANGE_AUTO_SUMMON@@@Z (0x1400980c0)
    void ReqHelperChangeAutoSummon(struct PS_HELPER_CHANGE_AUTO_SUMMON& psFlag);

    // IDA: ?ReqHelperSupportEquip@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_EQUIP_REQ@@@Z (0x140097080)
    void ReqHelperSupportEquip(struct PS_HELPER_SUPPORT_EQUIP_REQ& psEquip);

    // IDA: ?SendDBAddHelper@CGocHelper@@QEAA_NK@Z (0x14009a290)
    bool SendDBAddHelper(std::uint32_t dwHelperID);

    // Response handlers (from DB/Server)
    // IDA: ?ResHelperSupportInfo@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_INFO_RES@@@Z (0x140098280)
    void ResHelperSupportInfo(struct PS_HELPER_SUPPORT_INFO_RES& psInfo);

    // IDA: ?ResHelperSupportRegister@CGocHelper@@QEAAXAEAUPS_SERVER_HELPER_SUPPORT_REGISTER@@@Z (0x140098370)
    void ResHelperSupportRegister(struct PS_SERVER_HELPER_SUPPORT_REGISTER& psSupport);

    // IDA: ?ResHelperSupportReward@CGocHelper@@QEAAXAEAUPS_SERVER_HELPER_SUPPORT_REWARD@@@Z (0x140098600)
    void ResHelperSupportReward(struct PS_SERVER_HELPER_SUPPORT_REWARD& psReward);

    // IDA: ?ResHelperSupportList@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_LIST_RES@@@Z (0x140098850)
    void ResHelperSupportList(struct PS_HELPER_SUPPORT_LIST_RES& psList);

    // IDA: ?ResHelperSupportEquip@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_EQUIP_RES@@@Z (0x140098920)
    void ResHelperSupportEquip(struct PS_HELPER_SUPPORT_EQUIP_RES& psEquip);

    // IDA: ?ResHelperSupportEquipReward@CGocHelper@@QEAAXAEAUPS_HELPER_SUPPORT_EQUIP_REWARD_RES@@@Z (0x140098ca0)
    void ResHelperSupportEquipReward(struct PS_HELPER_SUPPORT_EQUIP_REWARD_RES& psReward);

    // IDA: ?ResHelperEquip@CGocHelper@@QEAAXAEAUPS_DB_HELPER_EQUIP_RES@@@Z (0x140098d20)
    void ResHelperEquip(struct PS_DB_HELPER_EQUIP_RES& psRes);

    // IDA: ?ResHelperSupportRelease@CGocHelper@@QEAAXAEAUPS_DB_HELPER_SUPPORT_RELEASE@@@Z (0x140099d60)
    void ResHelperSupportRelease(struct PS_DB_HELPER_SUPPORT_RELEASE& psRelease);

    // IDA: ?ResHelperChangeOrder@CGocHelper@@QEAAXAEAUPS_HELPER_CHANGE_ORDER@@@Z (0x140099f60)
    void ResHelperChangeOrder(struct PS_HELPER_CHANGE_ORDER& psOrder);

    // IDA: ?ResHelperChangeAutoSummon@CGocHelper@@QEAAXAEAUPS_HELPER_CHANGE_AUTO_SUMMON@@@Z (0x14009a140)
    void ResHelperChangeAutoSummon(struct PS_HELPER_CHANGE_AUTO_SUMMON& psFlag);

protected:
    // Time of last helper summon
    // IDA: m_tLastSummon (offset after vftable and GOComponent base)
    std::time_t m_tLastSummon;

    // Map of summoned helpers: HelperID -> CMonster*
    // IDA: m_mapSummonedHelper
    std::map<std::uint32_t, CMonster*> m_mapSummonedHelper;

    // Map of helper info: HelperID -> ST_HELPER_INFO
    // IDA: m_mapHelper (key is dwHelperID, not byOrder)
    std::map<std::uint32_t, ST_HELPER_INFO> m_mapHelper;

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
