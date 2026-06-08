#pragma once

#include "GOComponent.h"
#include <vector>
#include <map>
#include <memory>

// Forward declarations
class CMonster;
class CUser;
class XPacket;
class XSendPacket;

// Helper support structures (from IDA patterns)
struct ST_HELPER_INFO
{
    unsigned long dwHelperID = 0;
    unsigned long dwTableID = 0;
    unsigned char byOrder = 0;
    unsigned char byAutoSummon = 0;
    unsigned char bySlotPos = 0;
    char szName[64] = {};
    int nLevel = 0;
    int nHP = 0;
    int nMaxHP = 0;
    float fAbility[32] = {};
    // Additional fields based on IDA decompile
    unsigned long dwSummonTime = 0;
    unsigned char bySummonState = 0;
    unsigned long dwOwnerID = 0;
};

struct PS_HELPER_LIST_RES
{
    unsigned char byCount = 0;
    ST_HELPER_INFO stHelperInfo[10] = {};
};

struct PS_HELPER_SUMMON_REQ
{
    unsigned long dwHelperID = 0;
    unsigned char bySummon = 0;  // 1=summon, 0=release
};

struct PS_HELPER_SUMMON_RES
{
    unsigned long dwHelperID = 0;
    unsigned char byResult = 0;
    unsigned char bySummonState = 0;
};

struct PS_HELPER_STAT_UPDATE
{
    unsigned long dwHelperID = 0;
    float fAbility[32] = {};
};

struct PS_HELPER_SUPPORT_INFO_RES
{
    unsigned char bySupportType = 0;
    float fSupportValue = 0.0f;
    unsigned long dwSupportTime = 0;
};

struct PS_HELPER_SUPPORT_REGISTER_REQ
{
    unsigned char bySupportType = 0;
    unsigned long dwItemID = 0;
};

struct PS_HELPER_SUPPORT_EQUIP_REQ
{
    unsigned long dwHelperID = 0;
    unsigned long dwItemID = 0;
    unsigned char bySlotPos = 0;
};

struct PS_HELPER_EQUIP_REQ
{
    unsigned long dwHelperID = 0;
    unsigned long dwItemID = 0;
    unsigned char bySlotPos = 0;
    unsigned char byEquip = 1;  // 1=equip, 0=unequip
};

struct PS_HELPER_CHANGE_ORDER
{
    unsigned long dwHelperID = 0;
    unsigned char byNewOrder = 0;
};

struct PS_HELPER_CHANGE_AUTO_SUMMON
{
    unsigned long dwHelperID = 0;
    unsigned char byAutoSummon = 0;
};

struct PS_SERVER_HELPER_SUPPORT_REGISTER
{
    unsigned char byResult = 0;
    unsigned long dwSupportID = 0;
};

struct PS_SERVER_HELPER_SUPPORT_REWARD
{
    unsigned char byResult = 0;
    unsigned long dwRewardID = 0;
};

struct PS_HELPER_SUPPORT_LIST_RES
{
    unsigned char byCount = 0;
    // Support list items
};

struct PS_HELPER_SUPPORT_EQUIP_RES
{
    unsigned char byResult = 0;
    unsigned long dwHelperID = 0;
};

struct PS_DB_HELPER_EQUIP_RES
{
    unsigned char byResult = 0;
    unsigned long dwHelperID = 0;
};

struct PS_DB_HELPER_SUPPORT_RELEASE
{
    unsigned char byResult = 0;
};

struct ST_HELPER_SUPPORT_INFO
{
    unsigned char bySupportType = 0;
    float fSupportValue = 0.0f;
    unsigned long dwSupportTime = 0;
};

struct ST_HELPER_SUPPORT_FRIEND_INFO
{
    unsigned long dwFriendID = 0;
    unsigned char bySupportType = 0;
};

// Helper component for managing player companions/helpers
class CGocHelper : public GOComponent
{
public:
    CGocHelper();
    virtual ~CGocHelper();

    // GOComponent interface
    virtual void Init() override;
    virtual void Clear() override;
    void Reset();

    // Helper list management
    bool SetHelperList(PS_HELPER_LIST_RES& stRes);
    bool GetHelperList(std::vector<ST_HELPER_INFO>& vecList, bool bSummonedOnly);
    void SendHelperList();
    bool GetHelperInfo(unsigned long dwHelperID, ST_HELPER_INFO& stInfo);
    bool FindHelper(unsigned long dwHelperID);
    bool AddMyHelper(ST_HELPER_INFO& stInfo, bool bNew);

    // Helper summon/release
    void HelperProcess(PS_HELPER_SUMMON_REQ& stReq);
    bool HelperSummon(unsigned long dwHelperID);
    bool HelperRelease(unsigned long dwHelperID);
    bool HelperWarp(unsigned long dwHelperID);

    // Helper state management
    void SetHelperSummonState(unsigned long dwHelperID, bool bSummoned);
    bool CheckHelperSummonDelay();
    bool CheckSummonHelper(unsigned long dwHelperID);
    bool CheckReleaseHelper(unsigned long dwHelperID);
    void CheckSummonHelperCount();
    void CheckAllHelperSummon();
    void AllHelperRelease();
    void AllHelperWarp();
    void OtherHelperClear();

    // Helper instance management
    CMonster* GetSummonedHelper(unsigned long dwHelperID);
    void GetSummonedHelperList(std::map<unsigned long, unsigned long>& mapList);
    void SyncSummonedInfo();

    // Helper stats
    void GetMyHelperStatsALL(PS_HELPER_STAT_UPDATE& stUpdate);
    void CalcHelperStatsALL(ST_HELPER_INFO& stInfo, int nType);
    void CalcOriginStats(ST_HELPER_INFO& stInfo);
    void CalcEquipItemStats(ST_HELPER_INFO& stInfo);
    void CalcFriendItemStats(ST_HELPER_INFO& stInfo);
    void UnEquipHelperItemStats(unsigned long dwHelperID, std::vector<unsigned long>& vecItems);
    void UnEquipHelperFriendItemStats(unsigned long dwHelperID);

    // Helper support system
    float GetSupportTypeRate();
    float GetSupportTypeValue();
    void SetMySupportInfo(PS_HELPER_SUPPORT_INFO_RES& stRes);
    void HelperSupportRelease();
    void HelperSupportRelease(unsigned long dwHelperID);

    // Client request handlers
    void ReqHelperSupportInfo();
    void ReqHelperSupportRegister(PS_HELPER_SUPPORT_REGISTER_REQ& stReq);
    void ReqHelperSupportReward();
    void ReqHelperSupportList();
    void ReqHelperSupportEquip(PS_HELPER_SUPPORT_EQUIP_REQ& stReq);
    void ReqHelperEquip(PS_HELPER_EQUIP_REQ& stReq);
    void ReqHelperChangeOrder(PS_HELPER_CHANGE_ORDER& stReq);
    void ReqHelperChangeAutoSummon(PS_HELPER_CHANGE_AUTO_SUMMON& stReq);

    // DB response handlers
    void ResHelperSupportInfo(PS_HELPER_SUPPORT_INFO_RES& stRes);
    void ResHelperSupportRegister(PS_SERVER_HELPER_SUPPORT_REGISTER& stRes);
    void ResHelperSupportReward(PS_SERVER_HELPER_SUPPORT_REWARD& stRes);
    void ResHelperSupportList(PS_HELPER_SUPPORT_LIST_RES& stRes);
    void ResHelperSupportEquip(PS_HELPER_SUPPORT_EQUIP_RES& stRes);
    void ResHelperSupportEquipReward(PS_HELPER_SUPPORT_EQUIP_RES& stRes);
    void ResHelperEquip(PS_DB_HELPER_EQUIP_RES& stRes);
    void ResHelperSupportRelease(PS_DB_HELPER_SUPPORT_RELEASE& stRes);
    void ResHelperChangeOrder(PS_HELPER_CHANGE_ORDER& stReq);
    void ResHelperChangeAutoSummon(PS_HELPER_CHANGE_AUTO_SUMMON& stReq);

    // Database operations
    bool SendDBAddHelper(unsigned long dwHelperID);
    void SendDBHelperList();

    // Auto summon management
    void SetAutoSummonFlag(unsigned char byFlag);
    bool IsAutoSummon();
    void SetHelperSummonTime();
    unsigned char GetLastOrderNumber();

    // Static utility
    static int GetFamilyID();

private:
    // Helper data storage
    std::map<unsigned char, ST_HELPER_INFO> m_mapHelperList;  // Ordered by slot
    std::map<unsigned long, unsigned long> m_mapSummonedHelper;  // HelperID -> ActorID
    std::map<unsigned long, ST_HELPER_SUPPORT_INFO> m_mapSupportInfo;  // Support data

    // Auto summon settings
    unsigned char m_byAutoSummonFlag;
    unsigned long m_dwLastSummonTime;
    unsigned char m_byMaxSummonCount;
    unsigned char m_byCurrentSummonCount;

    // Support system
    unsigned char m_byMySupportType;
    float m_fMySupportValue;
    unsigned long m_dwMySupportTime;
};

// Helper type definitions
enum HELPER_SUMMON_STATE
{
    HELPER_STATE_NONE = 0,
    HELPER_STATE_SUMMONED = 1,
    HELPER_STATE_WARPING = 2,
    HELPER_STATE_RELEASING = 3
};

enum HELPER_SUPPORT_TYPE
{
    SUPPORT_TYPE_NONE = 0,
    SUPPORT_TYPE_ATTACK = 1,
    SUPPORT_TYPE_DEFENSE = 2,
    SUPPORT_TYPE_HEAL = 3,
    SUPPORT_TYPE_BUFF = 4
};
