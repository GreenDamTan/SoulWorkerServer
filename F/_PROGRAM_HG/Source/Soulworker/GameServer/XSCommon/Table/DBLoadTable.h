#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "Soulworker/Common/XNet/XCommon/PSOption.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XCore/XServer/TXDBSocket.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerGM.h"

#define GREENDAMTAN_TB_STRUCT_SECTION
#include "Soulworker/GameServer/XSCommon/Table/TB_ACHIEVEMENT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ACHIEVEMENT_BEGIN.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ACHIEVEMENT_EMBLEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AKASHIC_COMBINATION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AKASHIC_DISASSEMBLE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AKASHIC_MAKE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AKASHIC_RANDOM_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AKASHIC_RANDOM_GROUP_IN.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AKASHIC_SLOT_EXTEND.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_APPEARANCE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AURA.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BANK_SLOT_EXTEND.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BATTLE_ARENA_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BOOSTER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BROACHCOMPOSE_RANK.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BROACHCOMPOSE_THEME.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BROACHCOMPOSE_TYPE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BROACHDATA.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BUFF.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CASHBILLING_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CASHSHOP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CASHSHOP_TAB.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHANGE_MOB_ID.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CREATE_CLOTH.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHARACTER_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHARACTER_PARTS.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHATTINGCOMMAND.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHECK_ACCESS_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHECK_ATTENDANCE_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHECK_ATTENDANCE_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHECK_ATTENDANCE_STREAK.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CLASSBATTLE_ROLE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_COLOR.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_COMBO_BUFF.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CREATEOPTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CREDIT_RAISE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CULTIVATION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CUSTOMER_BENEFIT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CUSTOMER_GRADE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DAILY_MISSION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DAILYMAZE_PORTAL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DAY_EVENT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DAY_EVENT_BOOSTER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DECK_BONUS.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DEFENSIVE_WEAPON.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DISASSEMBLE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DISTRICT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DISTRICT_TRANSPORT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DIVERGENCE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DIVISIONRANKREWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DROP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DROP_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DROP_GROUP_CHARACTER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DROPRATE_LEVEL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DROPRATE_MEMBER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DROPRATE_MOB.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DS_POINT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DYE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DYE_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ECHELON.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_EQUALIZER_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_EVENT_CONDITION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_FACTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_FRAGMENT_EXCHANGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_FRAGMENT_EXTRACTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_GACHA_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_GEAR_RATE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_GESTURE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_HELPER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_HELPER_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_HIDDEN_EVENT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_INFINITE_TOWER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_INTERACTION_ITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_INTERACTION_OBJECT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_INVEN_SLOT_EXTEND.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PROVIDE_ITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_QUEST_CHAPTER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_QUEST_CONDITION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_QUEST_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_QUEST_SEASON.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANDOM_CHANGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANDOM_GET.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANDOM_GET_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PC_AKASHIC.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PC_COSTUME.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PC_REWARD_SYSTEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PC_REWARD_SYSTEM_MONTH.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PHOTO_ITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_POLLEN.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_QUEST_EPISODE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AKASHIC_RECORDS.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANDOM_OPTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANK_CHANGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANK_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANK_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANK_REWARD_TOTALPOINT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_REINFORCE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_REINFORCE_EXCHANGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_REINFORCE_OPTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_REPACKAGECOSTUME.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ROOMRANK_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SECTORQUEST.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SELECT_ITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SERVERINFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SHOP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SKILL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SKILL_ACTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SKILL_OPTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SKILL_SLOT_EXTEND.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SOCIAL_ITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SOCIAL_MOTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SOCKET.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SOUL_GUAGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SOUL_METRY.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SOULSTONE_LEVELUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SPAWN_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SPAWNBOX_FRONTLINE_01.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SPAWNBOX_RSQUARE_01.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SPECIALMAZE_OPEN.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_STATUS.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SYSTEMMAIL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SYSTEMMAIL_ADD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_TALK.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_TALK_LIST.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_TALK_STRING.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_TITLE_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_TITLE_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_TITLE_STRING.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_TRANSPORT_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_UNITY_EVENT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_VERSION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WARLORD_EVENT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WARLORD_GUI.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WEAPON_RATE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WEEK_DAY.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WEEK_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WEEK_MISSION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WORLD_EVENT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WORLD_EVENT_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BROACH_SET.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_COSTUME_SOCKET.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_TITLE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_TITLE_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_TITLE_VALUE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_TITLE_CHANGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_CLASSREWARD_LINK.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_COSTUMESET.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_COUNTBOX.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_COUNTBOX_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_ENDURANCE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_EVOLUTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_EXTRACTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_LIMIT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_PACKAGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_RANDOMBOX.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_RANK_RATE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_REPAIR.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_SCRIPT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_SETITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_SIMILARGROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_CLASSIFY.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_NAMEFILTER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_NPC.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_NPC_PARTS.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_OPERATION_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_OPTION_STRING.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PARTYEXP_LEVEL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PARTYEXP_MEMBER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PARTYEXP_MOB.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PARTYREVISE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_COMMON.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_LEAGUE_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_LEAGUE_SKILL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_LEVEL_MAIL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_LEVELUP_POINT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAKE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZE_ENTER_COUNT_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZE_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZE_OPEN_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZE_OPENCONTROL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_DIFFICULTY.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_ITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_LEVEL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_NORMAL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_PARTYVALUE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_RANK.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_REVISION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_SOULVAPER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_STANDARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_TIME.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MILEAGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_DISTRICT6.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_DISTRICT6_DATE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_OPERATION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_SURVIVAL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MONSTER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MONSTER_BROKEN_PARTS.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MONSTER_EXP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MONSTER_PARTS.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MONSTER_WEAPON.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MYROOM_FURNITURE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MYROOM_GREED.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MYROOM_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_BI_CLASS_CORRECTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_BI_CLASS_STARTSKILL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_BI_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_BI_SECTOR_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_BI_UPGRADE_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_BI_SKILL_EDIT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_BI_UPGRADE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_CARDMATCH_CARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_CARDMATCH_RULE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_DEFENCE.h"
#undef GREENDAMTAN_TB_STRUCT_SECTION

struct ST_PHOTO_ID {
    std::uint8_t byClass = 0;
    std::uint8_t byType = 0;

    bool operator<(const ST_PHOTO_ID& rhs) const {
        if (byClass != rhs.byClass) {
            return byClass < rhs.byClass;
        }
        return byType < rhs.byType;
    }
};

struct ST_RANK_INFO_KEY {
    unsigned int m_dwMazeID = 0;
    std::uint32_t m_eType = 0;
    std::uint32_t m_eCategory = 0;

    bool operator<(const ST_RANK_INFO_KEY& rhs) const {
        if (m_dwMazeID != rhs.m_dwMazeID) {
            return m_dwMazeID < rhs.m_dwMazeID;
        }
        if (m_eType != rhs.m_eType) {
            return m_eType < rhs.m_eType;
        }
        return m_eCategory < rhs.m_eCategory;
    }
};

struct ST_RANKING_INFO {
    std::uint16_t wRankInfoIndex = 0;
    int nTopRank = 0;
    std::uint8_t byType = 0;
    std::uint8_t byClassType = 0;
    std::uint8_t byRewradType = 0;
    unsigned int dwMazeID = 0;
    std::uint8_t byClass = 0;
};

struct ST_NETCAFE_MISSION_INFO {
    unsigned int dwID = 0;
    std::int64_t nStartTime = 0;
    std::int64_t nEndTime = 0;
    unsigned int dwValue = 0;
    std::int64_t nUpdateTime = 0;
};

// ST_NETCAFE_MISSION_INFO 序列化运算符
inline void operator>>(XPacket& packet, ST_NETCAFE_MISSION_INFO& value) {
    packet.XParse >> value.dwID;
    packet.XParse >> value.nStartTime;
    packet.XParse >> value.nEndTime;
    packet.XParse >> value.dwValue;
    packet.XParse >> value.nUpdateTime;
}

inline XPacket& operator<<(XPacket& packet, const ST_NETCAFE_MISSION_INFO& value) {
    packet.XParse << value.dwID;
    packet.XParse << value.nStartTime;
    packet.XParse << value.nEndTime;
    packet.XParse << value.dwValue;
    packet.XParse << value.nUpdateTime;
    return packet;
}

// PS_NETCAFE_MISSION_LIST - 网吧任务列表
struct PS_NETCAFE_MISSION_LIST {
    std::uint32_t dwUAID = 0;
    std::vector<ST_NETCAFE_MISSION_INFO> vecList;
};

inline void operator>>(XPacket& packet, PS_NETCAFE_MISSION_LIST& value) {
    packet.XParse >> value.dwUAID;
    std::uint16_t wCount = 0;
    packet.XParse >> wCount;
    for (std::uint16_t i = 0; i < wCount; ++i) {
        ST_NETCAFE_MISSION_INFO info;
        packet >> info;
        value.vecList.push_back(info);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_NETCAFE_MISSION_LIST& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << static_cast<std::uint16_t>(value.vecList.size());
    for (const ST_NETCAFE_MISSION_INFO& info : value.vecList) {
        packet << info;
    }
    return packet;
}

// PS_NETCAFE_MISSION_UPDATE - 网吧任务更新
struct PS_NETCAFE_MISSION_UPDATE {
    std::uint32_t dwUAID = 0;
    ST_NETCAFE_MISSION_INFO stMission;
};

inline void operator>>(XPacket& packet, PS_NETCAFE_MISSION_UPDATE& value) {
    packet.XParse >> value.dwUAID;
    packet >> value.stMission;
}

inline XPacket& operator<<(XPacket& packet, const PS_NETCAFE_MISSION_UPDATE& value) {
    packet.XParse << value.dwUAID;
    packet << value.stMission;
    return packet;
}

static_assert(sizeof(ST_RANK_INFO_KEY) == 0xC, "ST_RANK_INFO_KEY size must match PDB");
static_assert(sizeof(ST_RANKING_INFO) == 0x14, "ST_RANKING_INFO size must match PDB");
static_assert(sizeof(ST_NETCAFE_MISSION_INFO) == 0x28, "ST_NETCAFE_MISSION_INFO size must match PDB");

// 注意: ST_KRR_MONSTER_INFO 定义在 PSCommon.h 中

#pragma pack(push, 4)
struct CTableLoader_S {
    std::FILE* m_pFile = nullptr;
    std::int64_t m_biCheckSum = 0;
    int m_nRowCount = 0;
    char m_szMD5[32] = {};
    std::string m_strTableName;
    int m_nMainVersion = 0;
    int m_nSubVersion = 0;
    int m_nDataVersion = 0;
};
#pragma pack(pop)

#ifdef _WIN32
static_assert(sizeof(CTableLoader_S) == 0x68, "CTableLoader_S size must match PDB");
#endif

namespace GreenDamTanDBLoadDetail {
inline std::array<std::uint8_t, 16> MakeMd5Digest(const std::string& input) {
    static constexpr std::uint32_t kShift[64] = {
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21,
    };
    static constexpr std::uint32_t kTable[64] = {
        0xd76aa478u, 0xe8c7b756u, 0x242070dbu, 0xc1bdceeeu, 0xf57c0fafu, 0x4787c62au,
        0xa8304613u, 0xfd469501u, 0x698098d8u, 0x8b44f7afu, 0xffff5bb1u, 0x895cd7beu,
        0x6b901122u, 0xfd987193u, 0xa679438eu, 0x49b40821u, 0xf61e2562u, 0xc040b340u,
        0x265e5a51u, 0xe9b6c7aau, 0xd62f105du, 0x02441453u, 0xd8a1e681u, 0xe7d3fbc8u,
        0x21e1cde6u, 0xc33707d6u, 0xf4d50d87u, 0x455a14edu, 0xa9e3e905u, 0xfcefa3f8u,
        0x676f02d9u, 0x8d2a4c8au, 0xfffa3942u, 0x8771f681u, 0x6d9d6122u, 0xfde5380cu,
        0xa4beea44u, 0x4bdecfa9u, 0xf6bb4b60u, 0xbebfbc70u, 0x289b7ec6u, 0xeaa127fau,
        0xd4ef3085u, 0x04881d05u, 0xd9d4d039u, 0xe6db99e5u, 0x1fa27cf8u, 0xc4ac5665u,
        0xf4292244u, 0x432aff97u, 0xab9423a7u, 0xfc93a039u, 0x655b59c3u, 0x8f0ccc92u,
        0xffeff47du, 0x85845dd1u, 0x6fa87e4fu, 0xfe2ce6e0u, 0xa3014314u, 0x4e0811a1u,
        0xf7537e82u, 0xbd3af235u, 0x2ad7d2bbu, 0xeb86d391u,
    };

    auto rotateLeft = [](std::uint32_t value, std::uint32_t shift) {
        return (value << shift) | (value >> (32 - shift));
    };

    std::vector<std::uint8_t> buffer(input.begin(), input.end());
    const std::uint64_t bitLength = static_cast<std::uint64_t>(buffer.size()) * 8u;
    buffer.push_back(0x80);
    while ((buffer.size() % 64u) != 56u) {
        buffer.push_back(0);
    }
    for (int index = 0; index < 8; ++index) {
        buffer.push_back(static_cast<std::uint8_t>((bitLength >> (index * 8)) & 0xFFu));
    }

    std::uint32_t a0 = 0x67452301u;
    std::uint32_t b0 = 0xefcdab89u;
    std::uint32_t c0 = 0x98badcfeu;
    std::uint32_t d0 = 0x10325476u;

    for (std::size_t offset = 0; offset < buffer.size(); offset += 64u) {
        std::uint32_t words[16] = {};
        for (int index = 0; index < 16; ++index) {
            const std::size_t wordOffset = offset + static_cast<std::size_t>(index) * 4u;
            words[index] = static_cast<std::uint32_t>(buffer[wordOffset]) |
                           (static_cast<std::uint32_t>(buffer[wordOffset + 1]) << 8) |
                           (static_cast<std::uint32_t>(buffer[wordOffset + 2]) << 16) |
                           (static_cast<std::uint32_t>(buffer[wordOffset + 3]) << 24);
        }

        std::uint32_t a = a0;
        std::uint32_t b = b0;
        std::uint32_t c = c0;
        std::uint32_t d = d0;

        for (int index = 0; index < 64; ++index) {
            std::uint32_t f = 0;
            int g = 0;
            if (index < 16) {
                f = (b & c) | (~b & d);
                g = index;
            } else if (index < 32) {
                f = (d & b) | (~d & c);
                g = (5 * index + 1) % 16;
            } else if (index < 48) {
                f = b ^ c ^ d;
                g = (3 * index + 5) % 16;
            } else {
                f = c ^ (b | ~d);
                g = (7 * index) % 16;
            }

            const std::uint32_t next = d;
            d = c;
            c = b;
            b = b + rotateLeft(a + f + kTable[index] + words[g], kShift[index]);
            a = next;
        }

        a0 += a;
        b0 += b;
        c0 += c;
        d0 += d;
    }

    std::array<std::uint8_t, 16> digest{};
    const std::uint32_t state[4] = {a0, b0, c0, d0};
    for (int index = 0; index < 4; ++index) {
        digest[index * 4] = static_cast<std::uint8_t>(state[index] & 0xFFu);
        digest[index * 4 + 1] = static_cast<std::uint8_t>((state[index] >> 8) & 0xFFu);
        digest[index * 4 + 2] = static_cast<std::uint8_t>((state[index] >> 16) & 0xFFu);
        digest[index * 4 + 3] = static_cast<std::uint8_t>((state[index] >> 24) & 0xFFu);
    }
    return digest;
}

class BinaryCursor {
public:
    explicit BinaryCursor(const std::vector<std::uint8_t>& buffer) : buffer_(buffer) {}

    bool ReadU8(std::uint8_t& value) {
        return ReadScalar(&value);
    }

    bool ReadU8(std::uint8_t* value) {
        return ReadScalar(value);
    }

    bool ReadU16(std::uint16_t& value) {
        return ReadScalar(&value);
    }

    bool ReadU16(std::uint16_t* value) {
        return ReadScalar(value);
    }

    bool ReadU32(unsigned int& value) {
        return ReadScalar(&value);
    }

    bool ReadU32(unsigned int* value) {
        return ReadScalar(value);
    }

    bool ReadI16(std::int16_t& value) {
        return ReadScalar(&value);
    }

    bool ReadI16(std::int16_t* value) {
        return ReadScalar(value);
    }

    bool ReadI32(int& value) {
        return ReadScalar(&value);
    }

    bool ReadI32(int* value) {
        return ReadScalar(value);
    }

    bool ReadF32(float& value) {
        return ReadScalar(&value);
    }

    bool ReadF32(float* value) {
        return ReadScalar(value);
    }

    bool ReadCheckSumU8(std::uint8_t* value) {
        return ReadCheckSumScalar(value);
    }

    bool ReadCheckSumU16(std::uint16_t* value) {
        return ReadCheckSumScalar(value);
    }

    bool ReadCheckSumU32(unsigned int* value) {
        return ReadCheckSumScalar(value);
    }

    bool ReadCheckSumI16(std::int16_t* value) {
        return ReadCheckSumScalar(value);
    }

    bool ReadCheckSumI32(int* value) {
        return ReadCheckSumScalar(value);
    }

    bool ReadCheckSumF32(float* value) {
        return ReadCheckSumScalar(value);
    }

    bool ReadUtf16String(std::string& value) {
        std::uint16_t charCount = 0;
        if (!ReadU16(charCount)) {
            return false;
        }

        const std::size_t byteCount = static_cast<std::size_t>(charCount) * sizeof(char16_t);
        if (offset_ + byteCount > buffer_.size()) {
            return false;
        }

        std::u16string text;
        text.resize(charCount);
        if (byteCount != 0) {
            std::memcpy(text.data(), buffer_.data() + offset_, byteCount);
        }
        offset_ += byteCount;
        value = Utf16ToUtf8(text);
        return true;
    }

    bool ReadCheckSumUtf16String(std::string& value) {
        std::uint16_t charCount = 0;
        if (!ReadU16(charCount)) {
            return false;
        }

        const std::size_t byteCount = static_cast<std::size_t>(charCount) * sizeof(char16_t);
        if (offset_ + byteCount > buffer_.size()) {
            return false;
        }

        std::u16string text;
        text.resize(charCount);
        if (byteCount != 0) {
            std::memcpy(text.data(), buffer_.data() + offset_, byteCount);
            for (std::size_t index = 0; index < byteCount; ++index) {
                checkSum_ += buffer_[offset_ + index];
            }
        }
        offset_ += byteCount;
        checkSum_ += charCount;
        value = Utf16ToUtf8(text);
        return true;
    }

    bool ReadAsciiString(std::string& value, std::uint16_t maxLen) {
        std::uint16_t charCount = 0;
        if (!ReadU16(charCount)) {
            return false;
        }
        if (charCount == 0 || charCount > maxLen || offset_ + charCount > buffer_.size()) {
            return false;
        }

        value.assign(reinterpret_cast<const char*>(buffer_.data() + offset_), charCount);
        offset_ += charCount;
        return true;
    }

    std::int64_t GetCheckSum() const {
        return checkSum_;
    }

    bool IsAtEnd() const {
        return offset_ == buffer_.size();
    }

private:
    template <typename T>
    bool ReadScalar(T* value) {
        if (value == nullptr || offset_ + sizeof(T) > buffer_.size()) {
            return false;
        }

        std::memcpy(value, buffer_.data() + offset_, sizeof(T));
        offset_ += sizeof(T);
        return true;
    }

    template <typename T>
    bool ReadCheckSumScalar(T* value) {
        if (!ReadScalar(value)) {
            return false;
        }

        T alignedValue{};
        std::memcpy(&alignedValue, value, sizeof(T));
        checkSum_ += static_cast<std::int64_t>(alignedValue);
        return true;
    }

    static std::string Utf16ToUtf8(const std::u16string& text) {
        std::string result;
        result.reserve(text.size());
        for (std::size_t index = 0; index < text.size(); ++index) {
            std::uint32_t codePoint = text[index];
            if (codePoint >= 0xD800 && codePoint <= 0xDBFF && index + 1 < text.size()) {
                const std::uint32_t low = text[index + 1];
                if (low >= 0xDC00 && low <= 0xDFFF) {
                    codePoint = 0x10000 + ((codePoint - 0xD800) << 10) + (low - 0xDC00);
                    ++index;
                }
            }

            if (codePoint <= 0x7F) {
                result.push_back(static_cast<char>(codePoint));
            } else if (codePoint <= 0x7FF) {
                result.push_back(static_cast<char>(0xC0 | (codePoint >> 6)));
                result.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
            } else if (codePoint <= 0xFFFF) {
                result.push_back(static_cast<char>(0xE0 | (codePoint >> 12)));
                result.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
                result.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
            } else {
                result.push_back(static_cast<char>(0xF0 | (codePoint >> 18)));
                result.push_back(static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F)));
                result.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
                result.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
            }
        }
        return result;
    }

    const std::vector<std::uint8_t>& buffer_;
    std::size_t offset_ = 0;
    std::int64_t checkSum_ = 0;
};

inline bool LoadBinaryFile(const std::filesystem::path& path, std::vector<std::uint8_t>& buffer) {
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        return false;
    }

    input.seekg(0, std::ios::end);
    const std::streamsize size = input.tellg();
    if (size < 0) {
        return false;
    }
    input.seekg(0, std::ios::beg);

    buffer.resize(static_cast<std::size_t>(size));
    if (size == 0) {
        return true;
    }

    input.read(reinterpret_cast<char*>(buffer.data()), size);
    return input.good() || input.eof();
}

template <std::size_t N>
inline void CopyString(char (&destination)[N], const std::string& source) {
    std::memset(destination, 0, N);
    if (source.empty()) {
        return;
    }
#ifdef _WIN32
    strncpy_s(destination, N, source.c_str(), _TRUNCATE);
#else
    std::strncpy(destination, source.c_str(), N - 1);
    destination[N - 1] = '\0';
#endif
}
}  // namespace GreenDamTanDBLoadDetail

class XResourceMgr {
public:
    bool Init(const char* szCommonDNS, const char* szGameDNS, unsigned int dwServerID) {
        m_strCommonDNS = szCommonDNS ? szCommonDNS : "";
        m_strGameDNS = szGameDNS ? szGameDNS : "";
        m_bGameDBLoad = false;
        m_dwServerID = dwServerID;

        if (!m_xCommonDBMgr.Init(
                reinterpret_cast<unsigned char*>(const_cast<char*>(m_strCommonDNS.c_str())), 1)) {
            LogHelper::LogLegacy("error %s ", m_strCommonDNS.c_str());
            LogHelper::LogLegacy("Common DB init False ");
            return false;
        }

        if (szGameDNS && *szGameDNS) {
            if (!m_xGameDBMgr.Init(
                    reinterpret_cast<unsigned char*>(const_cast<char*>(m_strGameDNS.c_str())), 1)) {
                LogHelper::LogLegacy("error %s ", m_strCommonDNS.c_str());
                LogHelper::LogLegacy("Game DB init False ");
                return false;
            }
            m_bGameDBLoad = true;
        }

        m_mapPCAkashic.clear();
        m_vecKRRInfo.clear();
        m_bLoadKRRData = false;
        return true;
    }

    bool Load(RES_LOAD_TYPE eResLoadType, const char* szResFilePath, int nWorldID, int nServerID) {
        if (!InitCommonDB(eResLoadType, szResFilePath, nWorldID, nServerID) ||
            (m_bGameDBLoad && !InitGameDB())) {
            return false;
        }

        SetStatusTable();
        LoadFactionInfo();
        InitInfiniteTowerTable();
        InitTitleOpenCondition();
        InitPartyRevise();
        InitCharacterInfo();
        InitMaze();
        InitLevelUpMail();
        InitWorldModeGroup();
        InitWorldModeDate();
        InitDayEvent();
        InitWeeklyEventGroupID();
        InitRankingInfoTable();
        InitQuestTable();
        InitDefaultPhotoItemID();
        InitPCCostume();
        InitPCAkashic();
        InitNetCafeMissionList();
        InitRoguelikeData();
        return true;
    }

    /// 对齐 IDA 0x140138880 XResourceMgr::Clear: 清理加载的表数据
    void Clear() {
        m_vecCasualMazeID.clear();
        m_mapDayEvent.clear();
        m_listWeeklyEvent.clear();
        m_mapAutoAcceptQuest.clear();
        m_mapPCCostume.clear();
        m_mapQuestCompleteBit.clear();
    }

    /// 对齐 IDA 0x1408E7790 XResourceMgr::GetCasualMazeID: 拷贝休闲迷宫 ID 列表
    void GetCasualMazeID(std::vector<std::uint16_t>& vecMazeID) {
        vecMazeID = m_vecCasualMazeID;
    }

    void LoadVersion() {
        m_nMainVersion = 0;
        m_nSubVersion = 0;
        m_nDataVersion = 0;

        if (m_strPath.empty()) {
            return;
        }

        std::vector<std::uint8_t> buffer;
        if (!GreenDamTanDBLoadDetail::LoadBinaryFile(
                std::filesystem::path(m_strPath) / "Data_s.res", buffer) ||
            buffer.size() < sizeof(int) * 3u) {
            return;
        }

        std::memcpy(&m_nMainVersion, buffer.data(), sizeof(m_nMainVersion));
        std::memcpy(&m_nSubVersion, buffer.data() + sizeof(m_nMainVersion), sizeof(m_nSubVersion));
        std::memcpy(&m_nDataVersion,
                    buffer.data() + sizeof(m_nMainVersion) + sizeof(m_nSubVersion),
                    sizeof(m_nDataVersion));
    }

    /**
     * @brief 判断当前是否处于代表角色榜单切换禁用窗口外。
     *
     * 对应 IDA 中 `XResourceMgr::CheckRankingTime(0x140074640)`：
     * - `0xC351` -> `RANKING_RESET_DAY`
     * - `0xC352` -> `RANKING_RESET_TIME`
     * - `0xC353` -> `RANKING_RESET_WAIT`
     */
    bool CheckRankingTime() const {
        constexpr unsigned int kRankingResetDay = 0xC351;
        constexpr unsigned int kRankingResetTime = 0xC352;
        constexpr unsigned int kRankingResetWait = 0xC353;

        const auto dayIt = commonRows_.find(kRankingResetDay);
        if (dayIt == commonRows_.end()) {
            LogHelper::LogError("game.contents",
                                "CheckRankingTime Table Error DayOfWeek [RANKING_RESET_DAY]");
            return false;
        }

        const auto hourIt = commonRows_.find(kRankingResetTime);
        if (hourIt == commonRows_.end()) {
            LogHelper::LogError("game.contents",
                                "CheckRankingTime Table Error Hour [RANKING_RESET_TIME]");
            return false;
        }

        const auto waitIt = commonRows_.find(kRankingResetWait);
        if (waitIt == commonRows_.end()) {
            LogHelper::LogError("game.contents",
                                "CheckRankingTime Table Error Wait [RANKING_RESET_WAIT]");
            return false;
        }

        const int rankingDay = static_cast<int>(dayIt->second.Value);
        const int rankingHour = static_cast<int>(hourIt->second.Value);
        const int rankingWaitHour = static_cast<int>(waitIt->second.Value);

        std::time_t now = std::time(nullptr);
        std::tm localTime{};
#ifdef _WIN32
        if (localtime_s(&localTime, &now) != 0) {
            return false;
        }
#else
        if (localtime_r(&now, &localTime) == nullptr) {
            return false;
        }
#endif

        const int currentWeekDay = localTime.tm_wday + 1;
        if (currentWeekDay != rankingDay) {
            return true;
        }

        const int currentHour = localTime.tm_hour;
        if (rankingHour > currentHour) {
            return true;
        }

        return currentHour >= (rankingHour + rankingWaitHour);
    }

#define GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION
#include "Soulworker/GameServer/XSCommon/Table/TB_ACHIEVEMENT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ACHIEVEMENT_BEGIN.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ACHIEVEMENT_EMBLEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AKASHIC_COMBINATION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AKASHIC_DISASSEMBLE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AKASHIC_MAKE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AKASHIC_RANDOM_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AKASHIC_RANDOM_GROUP_IN.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AKASHIC_SLOT_EXTEND.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_APPEARANCE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AURA.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BANK_SLOT_EXTEND.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BATTLE_ARENA_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BOOSTER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BROACHCOMPOSE_RANK.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BROACHCOMPOSE_THEME.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BROACHCOMPOSE_TYPE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BROACHDATA.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BUFF.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CASHBILLING_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CASHSHOP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CASHSHOP_TAB.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHANGE_MOB_ID.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CREATE_CLOTH.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHARACTER_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHARACTER_PARTS.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHATTINGCOMMAND.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHECK_ACCESS_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHECK_ATTENDANCE_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHECK_ATTENDANCE_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHECK_ATTENDANCE_STREAK.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CLASSBATTLE_ROLE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_COLOR.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_COMBO_BUFF.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CREATEOPTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CREDIT_RAISE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CULTIVATION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CUSTOMER_BENEFIT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CUSTOMER_GRADE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DAILY_MISSION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DAILYMAZE_PORTAL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DAY_EVENT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DAY_EVENT_BOOSTER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DECK_BONUS.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DEFENSIVE_WEAPON.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DISASSEMBLE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DISTRICT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DISTRICT_TRANSPORT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DIVERGENCE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DIVISIONRANKREWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DROP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DROP_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DROP_GROUP_CHARACTER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DROPRATE_LEVEL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DROPRATE_MEMBER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DROPRATE_MOB.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DS_POINT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DYE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DYE_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ECHELON.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_EQUALIZER_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_EVENT_CONDITION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_FACTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_FRAGMENT_EXCHANGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_FRAGMENT_EXTRACTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_GACHA_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_GEAR_RATE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_GESTURE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_HELPER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_HELPER_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_HIDDEN_EVENT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_INFINITE_TOWER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_INTERACTION_ITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_INTERACTION_OBJECT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_INVEN_SLOT_EXTEND.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PROVIDE_ITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_QUEST_CHAPTER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_QUEST_CONDITION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_QUEST_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_QUEST_SEASON.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANDOM_CHANGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANDOM_GET.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANDOM_GET_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PC_AKASHIC.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PC_COSTUME.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PC_REWARD_SYSTEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PC_REWARD_SYSTEM_MONTH.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PHOTO_ITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_POLLEN.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_QUEST_EPISODE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AKASHIC_RECORDS.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANDOM_OPTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANK_CHANGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANK_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANK_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANK_REWARD_TOTALPOINT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_REINFORCE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_REINFORCE_EXCHANGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_REINFORCE_OPTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_REPACKAGECOSTUME.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ROOMRANK_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SECTORQUEST.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SELECT_ITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SERVERINFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SHOP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SKILL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SKILL_ACTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SKILL_OPTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SKILL_SLOT_EXTEND.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SOCIAL_ITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SOCIAL_MOTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SOCKET.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SOUL_GUAGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SOUL_METRY.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SOULSTONE_LEVELUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SPAWN_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SPAWNBOX_FRONTLINE_01.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SPAWNBOX_RSQUARE_01.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SPECIALMAZE_OPEN.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_STATUS.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SYSTEMMAIL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SYSTEMMAIL_ADD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_TALK.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_TALK_LIST.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_TALK_STRING.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_TITLE_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_TITLE_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_TITLE_STRING.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_TRANSPORT_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_UNITY_EVENT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_VERSION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WARLORD_EVENT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WARLORD_GUI.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WEAPON_RATE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WEEK_DAY.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WEEK_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WEEK_MISSION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WORLD_EVENT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WORLD_EVENT_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BROACH_SET.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_COSTUME_SOCKET.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_TITLE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_TITLE_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_TITLE_VALUE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_TITLE_CHANGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_CLASSREWARD_LINK.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_COSTUMESET.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_COUNTBOX.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_COUNTBOX_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_ENDURANCE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_EVOLUTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_EXTRACTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_LIMIT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_PACKAGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_RANDOMBOX.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_RANK_RATE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_REPAIR.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_SCRIPT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_SETITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_SIMILARGROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_CLASSIFY.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_NAMEFILTER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_NPC.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_NPC_PARTS.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_OPERATION_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_OPTION_STRING.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PARTYEXP_LEVEL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PARTYEXP_MEMBER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PARTYEXP_MOB.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PARTYREVISE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_COMMON.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_LEAGUE_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_LEAGUE_SKILL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_LEVEL_MAIL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_LEVELUP_POINT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAKE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZE_ENTER_COUNT_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZE_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZE_OPEN_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZE_OPENCONTROL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_DIFFICULTY.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_ITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_LEVEL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_NORMAL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_PARTYVALUE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_RANK.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_REVISION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_SOULVAPER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_STANDARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_TIME.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MILEAGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_DISTRICT6.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_DISTRICT6_DATE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_OPERATION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_SURVIVAL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MONSTER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MONSTER_BROKEN_PARTS.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MONSTER_EXP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MONSTER_PARTS.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MONSTER_WEAPON.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MYROOM_FURNITURE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MYROOM_GREED.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MYROOM_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_BI_CLASS_CORRECTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_BI_CLASS_STARTSKILL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_BI_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_BI_SECTOR_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_BI_UPGRADE_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_BI_SKILL_EDIT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_BI_UPGRADE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_CARDMATCH_CARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_CARDMATCH_RULE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_DEFENCE.h"
#undef GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION

    TB_PHOTO_ITEM* FindDefaultPhotoItemID(std::uint8_t byClass, std::uint8_t byType) {
        const auto it = photoItemIdRows_.find(ST_PHOTO_ID{byClass, byType});
        if (it == photoItemIdRows_.end()) {
            std::size_t matchingRawRows = 0;
            unsigned int firstMatchingId = 0;
            unsigned int firstMatchingPhotoName = 0;
            unsigned int firstMatchingPhotoGroup = 0;
            for (const auto& entry : photoItemRows_) {
                const TB_PHOTO_ITEM& row = entry.second;
                if (row.Char_Class != byClass || row.Char_Promotion_Info != byType) {
                    continue;
                }
                ++matchingRawRows;
                if (firstMatchingId == 0) {
                    firstMatchingId = row.ID;
                    firstMatchingPhotoName = row.Photo_Name;
                    firstMatchingPhotoGroup = row.Photo_Group;
                }
            }
            LogHelper::LogDebug("game.system",
                                "GreenDamTan_log DBLoadTable.h::XResourceMgr::FindDefaultPhotoItemID miss class=%u type=%u photoRows=%zu photoIdRows=%zu matchingRawRows=%zu firstId=%u firstPhotoName=%u firstPhotoGroup=%u",
                                static_cast<unsigned int>(byClass),
                                static_cast<unsigned int>(byType),
                                photoItemRows_.size(),
                                photoItemIdRows_.size(),
                                matchingRawRows,
                                firstMatchingId,
                                firstMatchingPhotoName,
                                firstMatchingPhotoGroup);
            return nullptr;
        }
        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log DBLoadTable.h::XResourceMgr::FindDefaultPhotoItemID hit class=%u type=%u id=%u photoName=%u photoGroup=%u",
                            static_cast<unsigned int>(byClass),
                            static_cast<unsigned int>(byType),
                            it->second ? it->second->ID : 0u,
                            it->second ? it->second->Photo_Name : 0u,
                            it->second ? static_cast<unsigned int>(it->second->Photo_Group) : 0u);
        return it->second;
    }

    TB_AKASHIC_RECORDS* GetPCAkashic(unsigned int index) {
        const auto it = m_mapPCAkashic.find(index);
        return it == m_mapPCAkashic.end() ? nullptr : it->second;
    }

    void GetPCAkashic(std::map<std::uint32_t, TB_AKASHIC_RECORDS*>& outMap) {
        outMap = m_mapPCAkashic;
    }

    bool GetServerContents(int optionId) const {
        auto it = serverContents_.find(optionId);
        return it != serverContents_.end() && it->second;
    }

    void SetServerContents(int optionId, bool enabled) {
        serverContents_[optionId] = enabled;
    }

    void GetServerContents(PS_CONTENTS_INFO* outContents) const {
        if (!outContents) {
            return;
        }
        *outContents = contentsInfo_;
    }

    void SetServerContents(const PS_CONTENTS_INFO& contentsInfo) {
        contentsInfo_ = contentsInfo;
        for (std::size_t index = 0; index < (sizeof(contentsInfo.bContents) / sizeof(contentsInfo.bContents[0])); ++index) {
            serverContents_[static_cast<int>(index)] = contentsInfo.bContents[index];
        }
    }

    std::string MakeMD5(std::string strMd5) {
        constexpr char kHex[] = "0123456789abcdef";
        const auto digest = GreenDamTanDBLoadDetail::MakeMd5Digest(strMd5);
        std::string result(32, '\0');
        for (std::size_t index = 0; index < 16; ++index) {
            result[index * 2] = kHex[(digest[index] >> 4) & 0x0F];
            result[index * 2 + 1] = kHex[digest[index] & 0x0F];
        }
        return result;
    }

    bool CheckSum(CTableLoader_S& loader) {
        std::size_t md5Length = 0;
        while (md5Length < sizeof(loader.m_szMD5) && loader.m_szMD5[md5Length] != '\0') {
            ++md5Length;
        }
        if (md5Length == 0) {
            md5Length = sizeof(loader.m_szMD5);
        }

        const std::string expectedMd5 = MakeMD5(std::to_string(loader.m_biCheckSum));
        const std::string currentMd5(loader.m_szMD5, loader.m_szMD5 + md5Length);
        if (expectedMd5 == currentMd5) {
            return true;
        }

        LogHelper::LogError(
            "game.contents",
            "<GAME> Failed loading table file.(%s) ",
            loader.m_strTableName.c_str());
        return false;
    }

private:
    bool GreenDamTan_DBGetText(XDBBinder& xDBBinder,
                               char* buffer,
                               std::size_t bufferSize,
                               std::int64_t* outResult = nullptr) {
        if (!buffer || bufferSize == 0) {
            return false;
        }

        buffer[0] = '\0';
        std::int64_t strLenOrInd = 0;
        const std::uint16_t outParam = xDBBinder.m_sOutParam++;
        const std::int64_t result =
            m_xDBStmt.SQLGetData(outParam, 1, buffer, static_cast<std::int64_t>(bufferSize), &strLenOrInd);
        if (outResult) {
            *outResult = result;
        }
        if ((result & ~1LL) != 0) {
            return false;
        }

        if (strLenOrInd < 0) {
            buffer[0] = '\0';
            return true;
        }

        buffer[bufferSize - 1] = '\0';
        return true;
    }

    template <typename T>
    bool GreenDamTan_DBGetUnsigned(XDBBinder& xDBBinder,
                                   T* value,
                                   std::int64_t* outResult = nullptr) {
        if (!value) {
            return false;
        }

        char buffer[128] = {};
        if (!GreenDamTan_DBGetText(xDBBinder, buffer, sizeof(buffer), outResult)) {
            return false;
        }

        T parsedValue{};
        if (buffer[0]) {
            parsedValue = static_cast<T>(std::strtoull(buffer, nullptr, 10));
        }

        std::memcpy(value, &parsedValue, sizeof(parsedValue));
        return true;
    }

    template <typename T>
    bool GreenDamTan_DBGetSigned(XDBBinder& xDBBinder,
                                 T* value,
                                 std::int64_t* outResult = nullptr) {
        if (!value) {
            return false;
        }

        char buffer[128] = {};
        if (!GreenDamTan_DBGetText(xDBBinder, buffer, sizeof(buffer), outResult)) {
            return false;
        }

        T parsedValue{};
        if (buffer[0]) {
            parsedValue = static_cast<T>(std::strtoll(buffer, nullptr, 10));
        }

        std::memcpy(value, &parsedValue, sizeof(parsedValue));
        return true;
    }

    bool GreenDamTan_DBGetFloat(XDBBinder& xDBBinder,
                                float* value,
                                std::int64_t* outResult = nullptr) {
        if (!value) {
            return false;
        }

        char buffer[128] = {};
        if (!GreenDamTan_DBGetText(xDBBinder, buffer, sizeof(buffer), outResult)) {
            return false;
        }

        float parsedValue = 0.0f;
        if (buffer[0]) {
            parsedValue = std::strtof(buffer, nullptr);
        }

        std::memcpy(value, &parsedValue, sizeof(parsedValue));
        return true;
    }

    template <std::size_t N>
    bool GreenDamTan_DBGetString(XDBBinder& xDBBinder,
                                 char (&destination)[N],
                                 std::int64_t* outResult = nullptr) {
        std::array<char, 2048> buffer{};
        if (!GreenDamTan_DBGetText(xDBBinder, buffer.data(), buffer.size(), outResult)) {
            return false;
        }

        GreenDamTanDBLoadDetail::CopyString(destination, std::string(buffer.data()));
        return true;
    }

    std::int64_t GreenDamTan_LogTableLoadStep(const char* stageName, std::int64_t result) {
        if ((result & ~1LL) != 0) {
            LogHelper::LogError("game.system",
                                "GreenDamTan_log DBLoadTable.h::XResourceMgr::TableLoad fail stage=%s result=%lld error=%s",
                                stageName,
                                static_cast<long long>(result),
                                m_xDBStmt.m_szErrorMsg);
        }
        return result;
    }

#define GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION
#include "Soulworker/GameServer/XSCommon/Table/TB_ACHIEVEMENT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ACHIEVEMENT_BEGIN.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ACHIEVEMENT_EMBLEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AKASHIC_COMBINATION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AKASHIC_DISASSEMBLE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AKASHIC_MAKE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AKASHIC_RANDOM_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AKASHIC_RANDOM_GROUP_IN.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AKASHIC_SLOT_EXTEND.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_APPEARANCE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AURA.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BANK_SLOT_EXTEND.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BATTLE_ARENA_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BOOSTER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BROACHCOMPOSE_RANK.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BROACHCOMPOSE_THEME.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BROACHCOMPOSE_TYPE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BROACHDATA.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BUFF.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CASHBILLING_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CASHSHOP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CASHSHOP_TAB.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHANGE_MOB_ID.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CREATE_CLOTH.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHARACTER_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHARACTER_PARTS.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHATTINGCOMMAND.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHECK_ACCESS_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHECK_ATTENDANCE_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHECK_ATTENDANCE_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CHECK_ATTENDANCE_STREAK.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CLASSBATTLE_ROLE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_COLOR.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_COMBO_BUFF.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CREATEOPTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CREDIT_RAISE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CULTIVATION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CUSTOMER_BENEFIT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_CUSTOMER_GRADE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DAILY_MISSION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DAILYMAZE_PORTAL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DAY_EVENT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DAY_EVENT_BOOSTER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DECK_BONUS.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DEFENSIVE_WEAPON.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DISASSEMBLE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DISTRICT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DISTRICT_TRANSPORT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DIVERGENCE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DIVISIONRANKREWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DROP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DROP_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DROP_GROUP_CHARACTER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DROPRATE_LEVEL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DROPRATE_MEMBER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DROPRATE_MOB.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DS_POINT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DYE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_DYE_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ECHELON.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_EQUALIZER_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_EVENT_CONDITION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_FACTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_FRAGMENT_EXCHANGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_FRAGMENT_EXTRACTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_GACHA_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_GEAR_RATE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_GESTURE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_HELPER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_HELPER_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_HIDDEN_EVENT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_INFINITE_TOWER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_INTERACTION_ITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_INTERACTION_OBJECT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_INVEN_SLOT_EXTEND.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PROVIDE_ITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_QUEST_CHAPTER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_QUEST_CONDITION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_QUEST_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_QUEST_SEASON.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANDOM_CHANGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANDOM_GET.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANDOM_GET_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PC_AKASHIC.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PC_COSTUME.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PC_REWARD_SYSTEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PC_REWARD_SYSTEM_MONTH.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PHOTO_ITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_POLLEN.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_QUEST_EPISODE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_AKASHIC_RECORDS.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANDOM_OPTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANK_CHANGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANK_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANK_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_RANK_REWARD_TOTALPOINT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_REINFORCE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_REINFORCE_EXCHANGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_REINFORCE_OPTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_REPACKAGECOSTUME.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ROOMRANK_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SECTORQUEST.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SELECT_ITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SERVERINFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SHOP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SKILL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SKILL_ACTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SKILL_OPTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SKILL_SLOT_EXTEND.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SOCIAL_ITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SOCIAL_MOTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SOCKET.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SOUL_GUAGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SOUL_METRY.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SOULSTONE_LEVELUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SPAWN_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SPAWNBOX_FRONTLINE_01.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SPAWNBOX_RSQUARE_01.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SPECIALMAZE_OPEN.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_STATUS.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SYSTEMMAIL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_SYSTEMMAIL_ADD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_TALK.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_TALK_LIST.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_TALK_STRING.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_TITLE_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_TITLE_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_TITLE_STRING.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_TRANSPORT_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_UNITY_EVENT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_VERSION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WARLORD_EVENT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WARLORD_GUI.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WEAPON_RATE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WEEK_DAY.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WEEK_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WEEK_MISSION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WORLD_EVENT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_WORLD_EVENT_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_BROACH_SET.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_COSTUME_SOCKET.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_TITLE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_TITLE_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_TITLE_VALUE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_TITLE_CHANGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_CLASSREWARD_LINK.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_COSTUMESET.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_COUNTBOX.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_COUNTBOX_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_ENDURANCE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_EVOLUTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_EXTRACTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_LIMIT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_PACKAGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_RANDOMBOX.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_RANK_RATE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_REPAIR.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_SCRIPT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_SETITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_SIMILARGROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_ITEM_CLASSIFY.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_NAMEFILTER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_NPC.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_NPC_PARTS.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_OPERATION_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_OPTION_STRING.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PARTYEXP_LEVEL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PARTYEXP_MEMBER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PARTYEXP_MOB.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_PARTYREVISE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_COMMON.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_LEAGUE_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_LEAGUE_SKILL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_LEVEL_MAIL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_LEVELUP_POINT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAKE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZE_ENTER_COUNT_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZE_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZE_OPEN_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZE_OPENCONTROL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_DIFFICULTY.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_ITEM.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_LEVEL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_NORMAL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_PARTYVALUE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_RANK.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_REVISION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_SOULVAPER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_STANDARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MAZEREWARD_TIME.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MILEAGE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_DISTRICT6.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_DISTRICT6_DATE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_OPERATION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_SURVIVAL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MONSTER.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MONSTER_BROKEN_PARTS.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MONSTER_EXP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MONSTER_PARTS.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MONSTER_WEAPON.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MYROOM_FURNITURE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MYROOM_GREED.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MYROOM_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_BI_CLASS_CORRECTION.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_BI_CLASS_STARTSKILL.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_BI_REWARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_BI_SECTOR_INFO.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_BI_UPGRADE_GROUP.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_BI_SKILL_EDIT.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_BI_UPGRADE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_CARDMATCH_CARD.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_CARDMATCH_RULE.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_MODE_DEFENCE.h"
#undef GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION

    std::filesystem::path GreenDamTan_ResolveResBasePath(const char* szResFilePath) const {
        const auto hasPhotoTable = [](const std::filesystem::path& candidate) {
            if (candidate.empty()) {
                return false;
            }
            std::error_code error;
            return std::filesystem::exists(candidate / "tb_Photo_Item.res", error);
        };

        if (szResFilePath && *szResFilePath) {
            const std::filesystem::path configuredPath(szResFilePath);
            if (hasPhotoTable(configuredPath)) {
                return configuredPath;
            }
        }

        std::error_code error;
        std::filesystem::path currentPath = std::filesystem::current_path(error);
        if (error) {
            return {};
        }

        for (int depth = 0; depth < 16 && !currentPath.empty(); ++depth) {
            if (hasPhotoTable(currentPath)) {
                return currentPath;
            }
            if (hasPhotoTable(currentPath / "res")) {
                return currentPath / "res";
            }
            const std::filesystem::path parentPath = currentPath.parent_path();
            if (parentPath == currentPath) {
                break;
            }
            currentPath = parentPath;
        }

        return {};
    }

    bool InitCommonDB(RES_LOAD_TYPE eResLoadType,
                      const char* szResFilePath,
                      int nWorldID,
                      int nServerID) {
        const std::filesystem::path fallbackBasePath = GreenDamTan_ResolveResBasePath(szResFilePath);
        if (eResLoadType == RES_LOAD_TYPE_FILE) {
            LogHelper::LogInfo("game.system",
                               "GreenDamTan_log DBLoadTable.h::XResourceMgr::InitCommonDB resource-load-mode=file path=%s",
                               (szResFilePath && *szResFilePath) ? szResFilePath : "");
            return Load(szResFilePath);
        }

        LogHelper::LogInfo("game.system",
                           "GreenDamTan_log DBLoadTable.h::XResourceMgr::InitCommonDB resource-load-mode=db world=%d server=%d",
                           nWorldID,
                           nServerID);

        auto runCommonStage = [&](const char* stageName, auto&& stage) {
            XDBConnect* dbConnect = m_xCommonDBMgr.GetDBConnect();
            if (!dbConnect) {
                LogHelper::LogError("game.system",
                                    "GreenDamTan_log DBLoadTable.h::XResourceMgr::InitCommonDB db-stage-no-connect stage=%s",
                                    stageName);
                return false;
            }

            void** hdbc = dbConnect->GetHDBC();
            if (!hdbc || !*hdbc) {
                m_xCommonDBMgr.CollectDBConnect(dbConnect);
                LogHelper::LogError("game.system",
                                    "GreenDamTan_log DBLoadTable.h::XResourceMgr::InitCommonDB db-stage-no-hdbc stage=%s",
                                    stageName);
                return false;
            }

            if (m_xDBStmt.Init(dbConnect, nullptr, nullptr) != 0) {
                m_xCommonDBMgr.CollectDBConnect(dbConnect);
                LogHelper::LogError("game.system",
                                    "GreenDamTan_log DBLoadTable.h::XResourceMgr::InitCommonDB db-stage-init-fail stage=%s",
                                    stageName);
                return false;
            }

            const std::int64_t result = stage();
            dbConnect->SetEndTran((result & ~1LL) != 0 ? 1 : 0);
            m_xDBStmt.Clear();
            m_xCommonDBMgr.CollectDBConnect(dbConnect);

            if ((result & ~1LL) != 0) {
                LogHelper::LogError("game.system",
                                    "GreenDamTan_log DBLoadTable.h::XResourceMgr::InitCommonDB db-stage-fail stage=%s result=%lld",
                                    stageName,
                                    static_cast<long long>(result));
                return false;
            }

            LogHelper::LogInfo("game.system",
                               "GreenDamTan_log DBLoadTable.h::XResourceMgr::InitCommonDB db-stage-ok stage=%s",
                               stageName);
            return true;
        };

        if (!runCommonStage("TableLoad", [&]() { return TableLoad(); }) ||
            !runCommonStage("ServerOptionLoad", [&]() { return ServerOptionLoad(); }) ||
            !runCommonStage("ServerChannelInfoLoad", [&]() {
                return ServerChannelInfoLoad(nWorldID, nServerID);
            }) ||
            !runCommonStage("ServerChannelDistrict6InfoLoad", [&]() {
                return ServerChannelDistrict6InfoLoad(nWorldID, nServerID);
            })) {
            if (fallbackBasePath.empty()) {
                return false;
            }

            LogHelper::LogInfo("game.system",
                               "GreenDamTan_log DBLoadTable.h::XResourceMgr::InitCommonDB fallback-db-to-file path=%s reason=db-stage-fail",
                               fallbackBasePath.string().c_str());
            return Load(fallbackBasePath.string().c_str());
        }

        if (!itemRows_.empty() && !itemClassifyRows_.empty() && !characterInfoRows_.empty() &&
            !createClothRows_.empty() && !provideItemRows_.empty()) {
            return true;
        }

        if (fallbackBasePath.empty()) {
            LogHelper::LogError(
                "game.system",
                "GreenDamTan_log DBLoadTable.h::XResourceMgr::InitCommonDB db-mode-without-fallback-res-path");
            return true;
        }

        LogHelper::LogInfo(
            "game.system",
            "GreenDamTan_log DBLoadTable.h::XResourceMgr::InitCommonDB fallback-db-to-file path=%s reason=db-core-tables-empty item=%zu classify=%zu char=%zu cloth=%zu provide=%zu",
            fallbackBasePath.string().c_str(),
            itemRows_.size(),
            itemClassifyRows_.size(),
            characterInfoRows_.size(),
            createClothRows_.size(),
            provideItemRows_.size());
        return Load(fallbackBasePath.string().c_str());
    }


    bool InitGameDB() {
        XDBConnect* dbConnect = m_xGameDBMgr.GetDBConnect();
        if (!dbConnect) {
            return false;
        }

        void** hdbc = dbConnect->GetHDBC();
        if (!hdbc || !*hdbc) {
            m_xGameDBMgr.CollectDBConnect(dbConnect);
            return false;
        }

        if (m_xGameDBStmt.Init(dbConnect, nullptr, nullptr) != 0) {
            m_xGameDBMgr.CollectDBConnect(dbConnect);
            return false;
        }

        dbConnect->SetEndTran(0);
        m_xGameDBStmt.Clear();
        m_xGameDBMgr.CollectDBConnect(dbConnect);
        return true;
    }

    bool Load(const char* szResFilePath) {
        if (!szResFilePath || !*szResFilePath) {
            return false;
        }

        m_strPath = szResFilePath;
        LoadVersion();

        const std::filesystem::path basePath(szResFilePath);
        LogHelper::LogLegacy("===== Load Res File =====");
        LogHelper::LogLegacy("%s", basePath.string().c_str());

        districtRows_.clear();
        districtTransportRows_.clear();
        divergenceRows_.clear();
        divisionRankRewardRows_.clear();
        dropRows_.clear();
        dropGroupRows_.clear();
        dropGroupCharacterRows_.clear();
        dropRateLevelRows_.clear();
        dropRateMemberRows_.clear();
        dropRateMobRows_.clear();
        dsPointRows_.clear();
        dyeRows_.clear();
        dyeInfoRows_.clear();
        echelonRows_.clear();
        equalizerInfoRows_.clear();
        eventConditionRows_.clear();
        factionRows_.clear();
        fragmentExchangeRows_.clear();
        fragmentExtractionRows_.clear();
        gachaGroupRows_.clear();
        gearRateRows_.clear();
        gestureRows_.clear();
        helperRows_.clear();
        helperRewardRows_.clear();
        hiddenEventRows_.clear();
        infiniteTowerRows_.clear();
        interactionItemRows_.clear();
        interactionObjectRows_.clear();
        invenSlotExtendRows_.clear();
        createOptionRows_.clear();
        creditRaiseRows_.clear();
        cultivationRows_.clear();
        customerBenefitRows_.clear();
        customerGradeRows_.clear();
        dailyMissionRows_.clear();
        dailyMazePortalRows_.clear();
        dayEventRows_.clear();
        dayEventBoosterRows_.clear();
        deckBonusRows_.clear();
        defensiveWeaponRows_.clear();
        disassembleRows_.clear();
        m_mapTB_ACHIEVEMENT.clear();
        m_mapTB_ACHIEVEMENT_BEGIN.clear();
        m_mapTB_ACHIEVEMENT_EMBLEM.clear();
        m_mapTB_AKASHIC_COMBINATION.clear();
        m_mapTB_AKASHIC_DISASSEMBLE.clear();
        m_mapTB_AKASHIC_MAKE.clear();
        m_mapTB_AKASHIC_RANDOM_GROUP.clear();
        m_mapTB_AKASHIC_RANDOM_GROUP_IN.clear();
        m_mapTB_AKASHIC_SLOT_EXTEND.clear();
        m_mapTB_AURA.clear();
        m_mapTB_BANK_SLOT_EXTEND.clear();
        m_mapTB_BATTLE_ARENA_INFO.clear();
        m_mapTB_BOOSTER.clear();
        m_mapTB_BROACHCOMPOSE_RANK.clear();
        m_mapTB_BROACHCOMPOSE_THEME.clear();
        m_mapTB_BROACHCOMPOSE_TYPE.clear();
        m_mapTB_BROACHDATA.clear();
        m_mapTB_BUFF.clear();
        m_mapTB_CASHBILLING_INFO.clear();
        m_mapTB_CASHSHOP.clear();
        m_mapTB_CASHSHOP_TAB.clear();
        m_mapTB_CHANGE_MOB_ID.clear();
        appearanceRows_.clear();
        autoMailRows_.clear();
        broachSetRows_.clear();
        createClothRows_.clear();
        characterInfoRows_.clear();
        provideItemRows_.clear();
        photoItemRows_.clear();
        photoItemIdRows_.clear();
        m_mapTB_POLLEN.clear();
        m_mapTB_QUEST_CHAPTER.clear();
        m_mapTB_QUEST_CONDITION.clear();
        m_mapTB_QUEST_REWARD.clear();
        m_mapTB_QUEST_SEASON.clear();
        m_mapTB_RANDOM_CHANGE.clear();
        m_mapTB_RANDOM_GET.clear();
        m_mapTB_RANDOM_GET_GROUP.clear();
        m_mapTB_RANK_CHANGE.clear();
        m_mapTB_RANK_INFO.clear();
        m_mapTB_RANK_REWARD.clear();
        m_mapTB_RANK_REWARD_TOTALPOINT.clear();
        m_mapTB_REPACKAGECOSTUME.clear();
        m_mapTB_ROOMRANK_REWARD.clear();
        m_mapTB_SECTORQUEST.clear();
        m_mapTB_SELECT_ITEM.clear();
        m_mapTB_SERVERINFO.clear();
        m_mapTB_SHOP.clear();
        m_mapTB_SKILL.clear();
        m_mapTB_SKILL_ACTION.clear();
        m_mapTB_SKILL_OPTION.clear();
        m_mapTB_SKILL_SLOT_EXTEND.clear();
        m_mapTB_SOCIAL_ITEM.clear();
        m_mapTB_SOCIAL_MOTION.clear();
        reinforceRows_.clear();
        reinforceOptionRows_.clear();
        socketRows_.clear();
        m_mapTB_SOUL_GUAGE.clear();
        m_mapTB_SOUL_METRY.clear();
        m_mapTB_SOULSTONE_LEVELUP.clear();
        m_mapTB_SPAWN_GROUP.clear();
        m_mapTB_SPAWNBOX_FRONTLINE_01.clear();
        m_mapTB_SPAWNBOX_RSQUARE_01.clear();
        m_mapTB_SPECIALMAZE_OPEN.clear();
        m_mapTB_STATUS.clear();
        m_mapTB_SYSTEMMAIL.clear();
        autoMailRows_.clear();
        m_mapTB_TALK.clear();
        m_mapTB_TALK_LIST.clear();
        m_mapTB_TALK_STRING.clear();
        m_mapTB_TITLE_INFO.clear();
        m_mapTB_TITLE_REWARD.clear();
        m_mapTB_TITLE_STRING.clear();
        m_mapTB_TRANSPORT_INFO.clear();
        m_mapTB_UNITY_EVENT.clear();
        m_mapTB_VERSION.clear();
        m_mapTB_WARLORD_EVENT.clear();
        m_mapTB_WARLORD_GUI.clear();
        m_mapTB_WEAPON_RATE.clear();
        m_mapTB_WEEK_DAY.clear();
        m_mapTB_WEEK_MISSION.clear();
        m_mapTB_WORLD_EVENT.clear();
        m_mapTB_WORLD_EVENT_REWARD.clear();
        m_mapTB_REINFORCE_EXCHANGE.clear();
        itemRows_.clear();
        itemClassifyRows_.clear();
        commonRows_.clear();
        m_mapTB_LEAGUE_INFO.clear();
        m_mapTB_LEAGUE_SKILL.clear();
        m_mapTB_LEVEL_MAIL.clear();
        m_mapTB_LEVELUP_POINT.clear();
        m_mapTB_MAKE.clear();
        m_mapTB_MAZE_ENTER_COUNT_GROUP.clear();
        m_mapTB_MAZE_INFO.clear();
        m_mapTB_MAZE_OPEN_GROUP.clear();
        m_mapTB_MAZE_OPENCONTROL.clear();
        m_mapTB_MAZEREWARD_DIFFICULTY.clear();
        m_mapTB_MAZEREWARD_ITEM.clear();
        m_mapTB_MAZEREWARD_LEVEL.clear();
        m_mapTB_MAZEREWARD_NORMAL.clear();
        m_mapTB_MAZEREWARD_PARTYVALUE.clear();
        m_mapTB_MAZEREWARD_RANK.clear();
        m_mapTB_MAZEREWARD_REVISION.clear();
        m_mapTB_MAZEREWARD_SOULVAPER.clear();
        m_mapTB_MAZEREWARD_STANDARD.clear();
        m_mapTB_MAZEREWARD_TIME.clear();
        m_mapTB_MILEAGE.clear();
        m_mapTB_MONSTER.clear();
        m_mapTB_MONSTER_BROKEN_PARTS.clear();
        m_mapTB_MONSTER_EXP.clear();
        m_mapTB_MONSTER_PARTS.clear();
        m_mapTB_MONSTER_WEAPON.clear();
        m_mapTB_MYROOM_FURNITURE.clear();
        m_mapTB_MYROOM_GREED.clear();
        m_mapTB_MYROOM_INFO.clear();
        nameFilterRows_.clear();
        m_mapTB_NPC.clear();
        m_mapTB_NPC_PARTS.clear();
        m_mapTB_OPERATION_INFO.clear();
        m_mapTB_OPTION_STRING.clear();
        m_mapTB_PARTYEXP_LEVEL.clear();
        m_mapTB_PARTYEXP_MEMBER.clear();
        m_mapTB_PARTYEXP_MOB.clear();
        m_mapTB_PARTYREVISE.clear();
        m_mapTB_WEEK_GROUP.clear();
        m_mapTB_QUEST_EPISODE.clear();
        m_mapTB_PC_AKASHIC.clear();
        m_mapTB_PC_COSTUME.clear();
        m_mapTB_PC_REWARD_SYSTEM.clear();
        m_mapTB_PC_REWARD_SYSTEM_MONTH.clear();
        m_mapTB_AKASHIC_RECORDS.clear();
        m_mapPCAkashic.clear();
        m_mapQuestCompleteBit.clear();
        m_mapAutoAcceptQuest.clear();
        m_mapMainQuestOrder_Char1.clear();
        m_mapMainQuestOrder_Char2.clear();
        m_mapMainQuestOrder_Char3.clear();
        m_vecQuestEpisodeOtherContents.clear();
        m_dwMainQuestLastOrder_Char1 = 0;
        m_dwMainQuestLastOrder_Char2 = 0;
        m_dwMainQuestLastOrder_Char3 = 0;

        return LoadAppearanceFile(basePath) && LoadCreateClothFile(basePath) &&
               LoadCharacterInfoFile(basePath) && LoadPhotoItemFile(basePath) &&
               LoadItemFile(basePath) && LoadItemClassifyFile(basePath) &&
               LoadProvideItemFile(basePath) && LoadCommonFile(basePath) &&
               LoadWeekGroupFile(basePath) &&
               LoadPCAkashicFile(basePath) && LoadAkashicRecordsFile(basePath) &&
               LoadQuestEpisodeFile(basePath);
    }

    void SetStatusTable() {
        m_mapFirstStatus.clear();
        const auto roundStatusValue = [](float value) {
            return static_cast<float>(std::floor(static_cast<double>(value) + 0.5));
        };

        for (const auto& entry : m_mapTB_STATUS) {
            const TB_STATUS& row = entry.second;
            std::map<std::uint16_t, FIRST_STATUS_TABLE> mapStatusByLevel;
            for (std::uint16_t level = 1; level <= 100; ++level) {
                const float levelOffset = static_cast<float>(level - 1);
                FIRST_STATUS_TABLE status{};
                status.fSTR = roundStatusValue(row.STR_First_Value + levelOffset * row.STR_UP_Value);
                status.fAGI = roundStatusValue(row.AGI_First_Value + levelOffset * row.AGI_UP_Value);
                status.fBAL = roundStatusValue(row.BAL_First_Value + levelOffset * row.BAL_UP_Value);
                status.fINT = roundStatusValue(row.INT_First_Value + levelOffset * row.INT_UP_Value);
                status.fVIT = roundStatusValue(row.VIT_First_Value + levelOffset * row.VIT_UP_Value);
                status.fLUC = roundStatusValue(row.LUC_First_Value + levelOffset * row.LUC_UP_Value);
                mapStatusByLevel.emplace(level, status);
            }
            m_mapFirstStatus.emplace(entry.first, std::move(mapStatusByLevel));
        }
    }

    void LoadFactionInfo() {
        m_xFaction.clear();
        for (const auto& entry : factionRows_) {
            const TB_FACTION& row = entry.second;
            const unsigned int key = static_cast<unsigned int>(row.Enamy_Faction_Type) |
                                     (static_cast<unsigned int>(row.Local_Faction_Type) << 8);
            m_xFaction.emplace(key, row.Faction_Type);
        }
    }

    void InitInfiniteTowerTable() {
        m_mapInfiniteTower.clear();
        for (const auto& entry : infiniteTowerRows_) {
            const TB_INFINITE_TOWER& row = entry.second;
            for (const auto mazeId : row.uniMazeID) {
                m_mapInfiniteTower.emplace(mazeId, static_cast<std::int16_t>(row.Chapter_ID));
            }
        }
    }

    void InitTitleOpenCondition() {
        for (auto& conditionMaps : m_mapPrefixTitleOpenCondition) {
            for (auto& titleMap : conditionMaps) {
                titleMap.clear();
            }
        }
        for (auto& conditionMaps : m_mapSuffixTitleOpenCondition) {
            for (auto& titleMap : conditionMaps) {
                titleMap.clear();
            }
        }

        for (const auto& entry : m_mapTB_TITLE_INFO) {
            const TB_TITLE_INFO& row = entry.second;
            if (row.Info_Type == 1) {
                continue;
            }
            if (row.Open_Condition >= 11 || row.Use_Class >= 9) {
                continue;
            }

            auto& titleMap = row.Title_Type != 0
                                 ? m_mapSuffixTitleOpenCondition[row.Open_Condition][row.Use_Class]
                                 : m_mapPrefixTitleOpenCondition[row.Open_Condition][row.Use_Class];
            titleMap.emplace(static_cast<int>(row.Open_Condition_Value), static_cast<int>(row.ID));
        }
    }

    void InitPartyRevise() {
        m_mapPartyRevise.clear();
        for (auto& entry : m_mapTB_PARTYREVISE) {
            TB_PARTYREVISE& row = entry.second;
            m_mapPartyRevise[std::make_pair(row.GroupID, row.Member_Count)] = &row;
        }
    }

    void InitCharacterInfo() {
        m_bOpenCharacterClass.fill(false);
        m_nOpenCharacterCount = 0;
        for (const auto& entry : characterInfoRows_) {
            const TB_CHARACTER_INFO& row = entry.second;
            if (row.Character_On_Type == 0) {
                continue;
            }
            m_bOpenCharacterClass[row.Character_ID] = row.Character_On != 0;
            if (row.Character_On == 1) {
                ++m_nOpenCharacterCount;
            }
        }
    }

    void InitMaze() {
        m_vecCasualMazeID.clear();
        for (const auto& entry : m_mapTB_MAZE_INFO) {
            const TB_MAZE_INFO& row = entry.second;
            if (row.Maze_Type == 2) {
                m_vecCasualMazeID.push_back(row.ID);
            }
        }
    }

    void InitLevelUpMail() {
        m_mapLevelEventID.clear();
        for (const auto& entry : m_mapTB_LEVEL_MAIL) {
            const TB_LEVEL_MAIL& row = entry.second;
            if (row.LevelMail_Type_On_Off == 1) {
                m_mapLevelEventID[static_cast<int>(row.LevelMail_Value)]
                    .push_back(static_cast<int>(entry.first));
            }
        }
    }

    void InitWorldModeGroup() {
        m_mpWorldModeGroup.clear();
        for (auto& entry : m_mapTB_MODE_DISTRICT6) {
            TB_MODE_DISTRICT6& row = entry.second;
            m_mpWorldModeGroup[row.Group].push_back(&row);
        }
    }

    void InitWorldModeDate() {
        m_mpWorldModeDate.clear();
        for (auto& entry : m_mapTB_MODE_DISTRICT6_DATE) {
            TB_MODE_DISTRICT6_DATE& row = entry.second;
            m_mpWorldModeDate[row.Day_Type].push_back(&row);
        }
    }

    void InitDayEvent() {
        m_mapDayEvent.clear();

        std::vector<std::pair<unsigned int, const TB_DAY_EVENT*>> sortedDayEvents;
        sortedDayEvents.reserve(dayEventRows_.size());
        for (const auto& entry : dayEventRows_) {
            sortedDayEvents.emplace_back(entry.first, &entry.second);
        }
        std::sort(sortedDayEvents.begin(),
                  sortedDayEvents.end(),
                  [](const auto& lhs, const auto& rhs) { return lhs.first < rhs.first; });

        for (const auto& entry : sortedDayEvents) {
            const TB_DAY_EVENT& row = *entry.second;
            const int day = static_cast<int>(row.ID % 10);
            if (day < 1 || day > 7) {
                LogHelper::LogError("game.system", "[DayEvent] InitDayEvent - ERROR Day %d", day);
                continue;
            }
            m_mapDayEvent[day].push_back(row.ID);
        }
    }

public:
    // 对齐 IDA 0x1400A3F00: GetDayEvent - 获取指定星期几的迷宫ID列表
    void GetDayEvent(int nDay, std::list<unsigned int>& listMazeID) {
        if (nDay < 1 || nDay > 7) {
            LogHelper::LogError("game.system", "[DayEvent] GetDayEvent - ERROR Day %d", nDay);
            return;
        }
        auto it = m_mapDayEvent.find(nDay);
        if (it != m_mapDayEvent.end()) {
            listMazeID = it->second;
        } else {
            listMazeID.clear();
        }
    }

    // 对齐 IDA 0x14009B430: GetWorldMode - 获取指定星期几的世界模式列表
    // 参数:
    //   byDayType - 星期几 (1-7)
    //   bLoad - 是否只加载 Start_Type == 1 的模式
    //   listID - 输出的世界模式列表
    void GetWorldMode(std::uint8_t byDayType, bool bLoad, std::list<ST_WORLD_MODE>* listID) {
        if (!listID) return;

        listID->clear();

        // 对齐 IDA: 在 m_mpWorldModeDate 中查找 byDayType
        auto itDate = m_mpWorldModeDate.find(byDayType);
        if (itDate == m_mpWorldModeDate.end()) {
            return;
        }

        // 对齐 IDA: 遍历 vector<TB_MODE_DISTRICT6_DATE*>
        const auto& vecDate = itDate->second;
        for (size_t i = 0; i < vecDate.size(); ++i) {
            TB_MODE_DISTRICT6_DATE* pDate = vecDate[i];
            if (!pDate) continue;

            // 对齐 IDA: 使用 Appear_Group (偏移 11) 查找 m_mpWorldModeGroup
            std::uint8_t byGroup = pDate->Appear_Group;
            auto itGroup = m_mpWorldModeGroup.find(byGroup);
            if (itGroup == m_mpWorldModeGroup.end()) {
                continue;
            }

            // 对齐 IDA: 遍历 vector<TB_MODE_DISTRICT6*>
            const auto& vecMode = itGroup->second;
            for (size_t j = 0; j < vecMode.size(); ++j) {
                TB_MODE_DISTRICT6* pMode = vecMode[j];
                if (!pMode) continue;

                // 对齐 IDA: 如果 bLoad == true，只取 Start_Type == 1
                if (bLoad && pMode->Start_Type != 1) {
                    continue;
                }

                // 对齐 IDA: 构建 ST_WORLD_MODE 并添加到列表
                ST_WORLD_MODE stInfo{};
                stInfo.nModeDateID = static_cast<int>(pDate->ID);
                stInfo.nModeID = static_cast<int>(pMode->ID);
                stInfo.biEnterDate = 0;
                listID->push_back(stInfo);
            }
        }
    }

    void InitWeeklyEventGroupID() {
        m_listWeeklyEvent.clear();
        for (const auto& entry : m_mapTB_WEEK_GROUP) {
            m_listWeeklyEvent.push_back(entry.first);
        }
    }

    void InitRankingInfoTable() {
        m_mapRankInfoTable.clear();
        m_vecRankInfoList.clear();

        for (auto& entry : m_mapTB_RANK_INFO) {
            TB_RANK_INFO& row = entry.second;
            if (row.Ranking_Total_Type == 0 || row.Ranking_Total_Type >= 6) {
                LogHelper::LogError(
                    "game.system",
                    "[InitRankingInfoTable] Failed Load TB_RANK_INFO : Ranking_Total_Type Error!! ");
                continue;
            }

            const ST_RANK_INFO_KEY key{
                static_cast<unsigned int>(row.Ranking_Maze),
                static_cast<std::uint32_t>(row.Ranking_Total_Type),
                static_cast<std::uint32_t>(row.Ranking_Category),
            };
            const auto [it, inserted] = m_mapRankInfoTable.emplace(key, &row);
            (void)it;
            if (!inserted) {
                LogHelper::LogError(
                    "game.system",
                    "[InitRankingInfoTable] Failed Load TB_RANK_INFO : Ranking_Maze Duplication!! ");
                continue;
            }

            if (row.Ranking_Category == 2) {
                continue;
            }

            ST_RANKING_INFO rankingInfo{};
            rankingInfo.wRankInfoIndex = row.Ranking_Index;
            rankingInfo.nTopRank = static_cast<int>(row.Ranking_Visible);
            rankingInfo.byType = row.Ranking_Total_Type;
            rankingInfo.byClassType = row.Ranking_Total_Class_Type;
            rankingInfo.byRewradType = row.Ranking_Reward_Receive;
            rankingInfo.dwMazeID = row.Ranking_Maze;

            if (row.Ranking_Total_Class_Type == 1) {
                for (std::uint8_t classType = 1; classType < 9; ++classType) {
                    rankingInfo.byClass = classType;
                    m_vecRankInfoList.push_back(rankingInfo);
                }
            } else {
                m_vecRankInfoList.push_back(rankingInfo);
            }
        }
    }

    void InitQuestTable() {
        m_mapQuestCompleteBit.clear();
        m_mapAutoAcceptQuest.clear();
        m_mapMainQuestOrder_Char1.clear();
        m_mapMainQuestOrder_Char2.clear();
        m_mapMainQuestOrder_Char3.clear();
        m_vecQuestEpisodeOtherContents.clear();
        m_dwMainQuestLastOrder_Char1 = 0;
        m_dwMainQuestLastOrder_Char2 = 0;
        m_dwMainQuestLastOrder_Char3 = 0;

        for (const auto& entry : m_mapTB_QUEST_EPISODE) {
            const TB_QUEST_EPISODE& row = entry.second;
            m_mapQuestCompleteBit[row.Complete_Bit] = row.ID;
            if (row.Auto_Start != 0 && row.Before_Episode_ID_1 != 0) {
                m_mapAutoAcceptQuest[row.ID] = row.Before_Episode_ID_1;
            }
            if (row.Contents_Type != 0 && row.Contents_Type != 2) {
                m_vecQuestEpisodeOtherContents.push_back(row.ID);
            }
        }

        auto findQuest = [this](unsigned int questId) -> const TB_QUEST_EPISODE* {
            const auto it = m_mapTB_QUEST_EPISODE.find(questId);
            return it == m_mapTB_QUEST_EPISODE.end() ? nullptr : &it->second;
        };

        unsigned int nextQuestId = 100001;
        unsigned int questOrder = 1;
        while (const TB_QUEST_EPISODE* quest = findQuest(nextQuestId)) {
            if (quest->Contents_Type != 0) {
                break;
            }

            m_mapMainQuestOrder_Char1[quest->ID] = questOrder++;
            m_dwMainQuestLastOrder_Char1 = quest->ID;

            unsigned int selectedQuestId = 0;
            const auto linkIds = quest->GetLinkEpisodeIDs();
            for (int index = 0; index < 10; ++index) {
                const unsigned int linkQuestId = linkIds[index];
                if (linkQuestId == 0) {
                    break;
                }

                const TB_QUEST_EPISODE* linkedQuest = findQuest(linkQuestId);
                if (!linkedQuest) {
                    break;
                }
                if (linkedQuest->Contents_Type != 0) {
                    continue;
                }
                if (linkedQuest->Class_Type == 0 || linkedQuest->Class_Type == 101) {
                    if (selectedQuestId != 0) {
                        LogHelper::LogError("game.system",
                                            "[InitQuestTable] Error Link quest [1] (QuestID:%d, LinkID:%d, LinkID:%d)",
                                            quest->ID,
                                            selectedQuestId,
                                            linkedQuest->ID);
                    }
                    selectedQuestId = linkedQuest->ID;
                }
            }

            if (selectedQuestId == 0) {
                break;
            }
            nextQuestId = selectedQuestId;
        }

        LogHelper::LogError("game.system",
                            "[InitQuestTable] m_mapMainQuestOrder_Char1 size (count:%d)",
                            static_cast<int>(m_mapMainQuestOrder_Char1.size()));

        auto buildMainQuestOrder =
            [this, &findQuest](unsigned int startQuestId,
                               std::uint16_t specialClassType,
                               const char* duplicateNormalLog,
                               const char* duplicateSpecialLog,
                               std::map<unsigned int, unsigned int>& outOrders,
                               unsigned int& outLastQuestId) {
                unsigned int order = 1;
                unsigned int currentQuestId = startQuestId;
                while (const TB_QUEST_EPISODE* quest = findQuest(currentQuestId)) {
                    if (quest->Contents_Type != 0) {
                        break;
                    }

                    outOrders[quest->ID] = order++;
                    outLastQuestId = quest->ID;

                    unsigned int selectedQuestId = 0;
                    const auto linkIds = quest->GetLinkEpisodeIDs();
                    for (int index = 0; index < 10; ++index) {
                        const unsigned int linkQuestId = linkIds[index];
                        if (linkQuestId == 0) {
                            break;
                        }

                        const TB_QUEST_EPISODE* linkedQuest = findQuest(linkQuestId);
                        if (!linkedQuest) {
                            break;
                        }
                        if (linkedQuest->Contents_Type != 0) {
                            continue;
                        }

                        const std::uint16_t classType = linkedQuest->Class_Type;
                        if (classType != 0) {
                            if (classType != specialClassType && classType != 102) {
                                continue;
                            }
                            if (selectedQuestId != 0) {
                                const TB_QUEST_EPISODE* previousQuest = findQuest(selectedQuestId);
                                if (previousQuest && previousQuest->Class_Type != 0) {
                                    LogHelper::LogError("game.system",
                                                        duplicateSpecialLog,
                                                        quest->ID,
                                                        selectedQuestId,
                                                        linkedQuest->ID);
                                    continue;
                                }
                            }
                        } else if (selectedQuestId != 0) {
                            LogHelper::LogError("game.system",
                                                duplicateNormalLog,
                                                quest->ID,
                                                selectedQuestId,
                                                linkedQuest->ID);
                        }

                        selectedQuestId = linkedQuest->ID;
                    }

                    if (selectedQuestId == 0) {
                        break;
                    }
                    currentQuestId = selectedQuestId;
                }
            };

        buildMainQuestOrder(570005,
                            7,
                            "[InitQuestTable] Error Link quest [7-1] (QuestID:%d, LinkID:%d, LinkID:%d)",
                            "[InitQuestTable] Error Link quest [7-2] (QuestID:%d, LinkID:%d, LinkID:%d)",
                            m_mapMainQuestOrder_Char2,
                            m_dwMainQuestLastOrder_Char2);
        LogHelper::LogError("game.system",
                            "[InitQuestTable] m_mapMainQuestOrder_Char2 size (count:%d)",
                            static_cast<int>(m_mapMainQuestOrder_Char2.size()));

        buildMainQuestOrder(570005,
                            8,
                            "[InitQuestTable] Error Link quest [8-1] (QuestID:%d, LinkID:%d, LinkID:%d)",
                            "[InitQuestTable] Error Link quest [8-2] (QuestID:%d, LinkID:%d, LinkID:%d)",
                            m_mapMainQuestOrder_Char3,
                            m_dwMainQuestLastOrder_Char3);
        LogHelper::LogError("game.system",
                            "[InitQuestTable] m_mapMainQuestOrder_Char3 size (count:%d)",
                            static_cast<int>(m_mapMainQuestOrder_Char3.size()));
    }

    void InitDefaultPhotoItemID() {
        photoItemIdRows_.clear();
        std::size_t insertedCount = 0;
        std::size_t skippedClassCount = 0;
        std::size_t skippedPromotionCount = 0;
        std::size_t skippedZeroPromotionCount = 0;
        std::size_t duplicateCount = 0;
        for (auto& entry : photoItemRows_) {
            TB_PHOTO_ITEM& row = entry.second;
            if (row.Char_Class == 0 || row.Char_Class >= 9) {
                ++skippedClassCount;
                LogHelper::LogError("game.system",
                                    "[InitDefaultPhotoItemID] Error TB_PHOTO_ITEM Table - Char_Class [ID:%d]",
                                    row.ID);
                continue;
            }

            if (row.Char_Promotion_Info > 2) {
                ++skippedPromotionCount;
                LogHelper::LogError(
                    "game.system",
                    "[InitDefaultPhotoItemID] Error TB_PHOTO_ITEM Table - Char_Promotion_Info  [ID:%d]",
                    row.ID);
                continue;
            }

            if (row.Char_Promotion_Info == 0) {
                ++skippedZeroPromotionCount;
                continue;
            }

            const ST_PHOTO_ID key{row.Char_Class, row.Char_Promotion_Info};
            const auto [it, inserted] = photoItemIdRows_.emplace(key, &row);
            (void)it;
            if (!inserted) {
                ++duplicateCount;
                LogHelper::LogError(
                    "game.system",
                    "[InitDefaultPhotoItemID] Error TB_PHOTO_ITEM Table - Already  [ID:%d]",
                    row.ID);
                continue;
            }
            ++insertedCount;
        }

        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log DBLoadTable.h::XResourceMgr::InitDefaultPhotoItemID photoRows=%zu photoIdRows=%zu inserted=%zu skippedClass=%zu skippedPromotion=%zu skippedZeroPromotion=%zu duplicate=%zu class3Promo1=%zu firstClass3Promo1Id=%u firstClass3Promo1PhotoName=%u firstClass3Promo1PhotoGroup=%u",
                            photoItemRows_.size(),
                            photoItemIdRows_.size(),
                            insertedCount,
                            skippedClassCount,
                            skippedPromotionCount,
                            skippedZeroPromotionCount,
                            duplicateCount,
                            [&]() {
                                std::size_t count = 0;
                                for (const auto& entry : photoItemRows_) {
                                    const TB_PHOTO_ITEM& row = entry.second;
                                    if (row.Char_Class == 3 && row.Char_Promotion_Info == 1) {
                                        ++count;
                                    }
                                }
                                return count;
                            }(),
                            [&]() {
                                for (const auto& entry : photoItemRows_) {
                                    const TB_PHOTO_ITEM& row = entry.second;
                                    if (row.Char_Class == 3 && row.Char_Promotion_Info == 1) {
                                        return row.ID;
                                    }
                                }
                                return 0u;
                            }(),
                            [&]() {
                                for (const auto& entry : photoItemRows_) {
                                    const TB_PHOTO_ITEM& row = entry.second;
                                    if (row.Char_Class == 3 && row.Char_Promotion_Info == 1) {
                                        return row.Photo_Name;
                                    }
                                }
                                return 0u;
                            }(),
                            [&]() {
                                for (const auto& entry : photoItemRows_) {
                                    const TB_PHOTO_ITEM& row = entry.second;
                                    if (row.Char_Class == 3 && row.Char_Promotion_Info == 1) {
                                        return static_cast<unsigned int>(row.Photo_Group);
                                    }
                                }
                                return 0u;
                            }());
    }

    std::int64_t TableLoad() {
        std::int64_t result = GreenDamTan_LogTableLoadStep("Load_TB_ACHIEVEMENT", LoadTBAchievementDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ACHIEVEMENT_BEGIN", LoadTBAchievementBeginDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ACHIEVEMENT_EMBLEM", LoadTBAchievementEmblemDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_AKASHIC_COMBINATION", LoadTBAkashicCombinationDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_AKASHIC_DISASSEMBLE", LoadTBAkashicDisassembleDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_AKASHIC_MAKE", LoadTBAkashicMakeDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_AKASHIC_RANDOM_GROUP", LoadTBAkashicRandomGroupDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_AKASHIC_RANDOM_GROUP_IN", LoadTBAkashicRandomGroupInDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_AKASHIC_SLOT_EXTEND", LoadTBAkashicSlotExtendDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_APPEARANCE", LoadTBAppearanceDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_AURA", LoadTBAuraDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_BANK_SLOT_EXTEND", LoadTBBankSlotExtendDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_BATTLE_ARENA_INFO", LoadTBBattleArenaInfoDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_BOOSTER", LoadTBBoosterDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_BROACH_SET", LoadTBBroachSetDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_BROACHCOMPOSE_RANK", LoadTBBroachComposeRankDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_BROACHCOMPOSE_THEME", LoadTBBroachComposeThemeDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_BROACHCOMPOSE_TYPE", LoadTBBroachComposeTypeDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_BROACHDATA", LoadTBBroachDataDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_BUFF", LoadTBBuffDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_CASHBILLING_INFO", LoadTBCashBillingInfoDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_CASHSHOP", LoadTBCashShopDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_CASHSHOP_TAB", LoadTBCashShopTabDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_CHANGE_MOB_ID", LoadTBChangeMobIdDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_CHARACTER_INFO", LoadTBCharacterInfoDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_CHARACTER_PARTS", LoadTBCharacterPartsDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_CHATTINGCOMMAND", LoadTBChattingCommandDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_CHECK_ACCESS_REWARD", LoadTBCheckAccessRewardDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_CHECK_ATTENDANCE_INFO", LoadTBCheckAttendanceInfoDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_CHECK_ATTENDANCE_REWARD", LoadTBCheckAttendanceRewardDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_CHECK_ATTENDANCE_STREAK", LoadTBCheckAttendanceStreakDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_CLASSBATTLE_ROLE", LoadTBClassBattleRoleDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_COLOR", LoadTBColorDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_COMBO_BUFF", LoadTBComboBuffDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_COMMON", LoadTBCommonDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_COSTUME_SOCKET", LoadTBCostumeSocketDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_CREATE_CLOTH", LoadTBCreateClothDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_CREATEOPTION", LoadTBCreateOptionDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_CREDIT_RAISE", LoadTBCreditRaiseDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_CULTIVATION", LoadTBCultivationDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_CUSTOMER_BENEFIT", LoadTBCustomerBenefitDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_CUSTOMER_GRADE", LoadTBCustomerGradeDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_DAILY_MISSION", LoadTBDailyMissionDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_DAILYMAZE_PORTAL", LoadTBDailyMazePortalDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_DAY_EVENT", LoadTBDayEventDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_DAY_EVENT_BOOSTER", LoadTBDayEventBoosterDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_DECK_BONUS", LoadTBDeckBonusDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_DEFENSIVE_WEAPON", LoadTBDefensiveWeaponDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_DISASSEMBLE", LoadTBDisassembleDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_DISTRICT", LoadTBDistrictDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_DISTRICT_TRANSPORT", LoadTBDistrictTransportDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_DIVERGENCE", LoadTBDivergenceDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_DIVISIONRANKREWARD", LoadTBDivisionRankRewardDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_DROP", LoadTBDropDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_DROP_GROUP", LoadTBDropGroupDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_DROP_GROUP_CHARACTER", LoadTBDropGroupCharacterDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_DROPRATE_LEVEL", LoadTBDropRateLevelDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_DROPRATE_MEMBER", LoadTBDropRateMemberDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_DROPRATE_MOB", LoadTBDropRateMobDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_DS_POINT", LoadTBDSPointDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_DYE", LoadTBDyeDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_DYE_INFO", LoadTBDyeInfoDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ECHELON", LoadTBEchelonDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_EQUALIZER_INFO", LoadTBEqualizerInfoDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_EVENT_CONDITION", LoadTBEventConditionDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_FACTION", LoadTBFactionDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_FRAGMENT_EXCHANGE", LoadTBFragmentExchangeDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_FRAGMENT_EXTRACTION", LoadTBFragmentExtractionDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_GACHA_GROUP", LoadTBGachaGroupDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_GEAR_RATE", LoadTBGearRateDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_GESTURE", LoadTBGestureDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_HELPER", LoadTBHelperDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_HELPER_REWARD", LoadTBHelperRewardDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_HIDDEN_EVENT", LoadTBHiddenEventDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_INFINITE_TOWER", LoadTBInfiniteTowerDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_INTERACTION_ITEM", LoadTBInteractionItemDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_INTERACTION_OBJECT", LoadTBInteractionObjectDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_INVEN_SLOT_EXTEND", LoadTBInvenSlotExtendDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ITEM", LoadTBItemDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ITEM_CLASSIFY", LoadTBItemClassifyDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ITEM_CLASSREWARD_LINK", LoadTBItemClassRewardLinkDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ITEM_COSTUMESET", LoadTBItemCostumeSetDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ITEM_COUNTBOX", LoadTBItemCountBoxDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ITEM_COUNTBOX_GROUP", LoadTBItemCountBoxGroupDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ITEM_ENDURANCE", LoadTBItemEnduranceDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ITEM_EVOLUTION", LoadTBItemEvolutionDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ITEM_EXTRACTION", LoadTBItemExtractionDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ITEM_LIMIT", LoadTBItemLimitDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ITEM_PACKAGE", LoadTBItemPackageDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ITEM_RANDOMBOX", LoadTBItemRandomBoxDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ITEM_RANK_RATE", LoadTBItemRankRateDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ITEM_REPAIR", LoadTBItemRepairDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ITEM_SCRIPT", LoadTBItemScriptDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ITEM_SETITEM", LoadTBItemSetItemDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ITEM_SIMILARGROUP", LoadTBItemSimilarGroupDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ITEM_TITLE", LoadTBItemTitleDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ITEM_TITLE_CHANGE", LoadTBItemTitleChangeDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ITEM_TITLE_GROUP", LoadTBItemTitleGroupDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ITEM_TITLE_VALUE", LoadTBItemTitleValueDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_LEAGUE_INFO", LoadTBLeagueInfoDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_LEAGUE_SKILL", LoadTBLeagueSkillDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_LEVEL_MAIL", LoadTBLevelMailDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_LEVELUP_POINT", LoadTBLevelupPointDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MAKE", LoadTBMakeDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MAZE_ENTER_COUNT_GROUP", LoadTBMazeEnterCountGroupDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MAZE_INFO", LoadTBMazeInfoDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MAZE_OPEN_GROUP", LoadTBMazeOpenGroupDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MAZE_OPENCONTROL", LoadTBMazeOpenControlDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MAZEREWARD_DIFFICULTY", LoadTBMazeRewardDifficultyDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MAZEREWARD_ITEM", LoadTBMazeRewardItemDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MAZEREWARD_LEVEL", LoadTBMazeRewardLevelDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MAZEREWARD_NORMAL", LoadTBMazeRewardNormalDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MAZEREWARD_PARTYVALUE", LoadTBMazeRewardPartyValueDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MAZEREWARD_RANK", LoadTBMazeRewardRankDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MAZEREWARD_REVISION", LoadTBMazeRewardRevisionDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MAZEREWARD_SOULVAPER", LoadTBMazeRewardSoulvaperDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MAZEREWARD_STANDARD", LoadTBMazeRewardStandardDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MAZEREWARD_TIME", LoadTBMazeRewardTimeDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MILEAGE", LoadTBMileageDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MODE_BI_CLASS_CORRECTION", LoadTBModeBIClassCorrectionDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MODE_BI_CLASS_STARTSKILL", LoadTBModeBIClassStartSkillDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MODE_BI_REWARD", LoadTBModeBIRewardDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MODE_BI_SECTOR_INFO", LoadTBModeBISectorInfoDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MODE_BI_SKILL_EDIT", LoadTBModeBISkillEditDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MODE_BI_UPGRADE", LoadTBModeBIUpgradeDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MODE_BI_UPGRADE_GROUP", LoadTBModeBIUpgradeGroupDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MODE_CARDMATCH_CARD", LoadTBModeCardMatchCardDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MODE_CARDMATCH_RULE", LoadTBModeCardMatchRuleDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MODE_DEFENCE", LoadTBModeDefenceDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MODE_DISTRICT6", LoadTBModeDistrict6DB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MODE_DISTRICT6_DATE", LoadTBModeDistrict6DateDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MODE_OPERATION", LoadTBModeOperationDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MODE_SURVIVAL", LoadTBModeSurvivalDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MONSTER", LoadTBMonsterDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MONSTER_BROKEN_PARTS", LoadTBMonsterBrokenPartsDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MONSTER_EXP", LoadTBMonsterExpDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MONSTER_PARTS", LoadTBMonsterPartsDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MONSTER_WEAPON", LoadTBMonsterWeaponDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MYROOM_FURNITURE", LoadTBMyroomFurnitureDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MYROOM_GREED", LoadTBMyroomGreedDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_MYROOM_INFO", LoadTBMyroomInfoDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_NAMEFILTER", LoadTBNameFilterDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_NPC", LoadTBNpcDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_NPC_PARTS", LoadTBNpcPartsDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_OPERATION_INFO", LoadTBOperationInfoDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_OPTION_STRING", LoadTBOptionStringDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_PARTYEXP_LEVEL", LoadTBPartyExpLevelDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_PARTYEXP_MEMBER", LoadTBPartyExpMemberDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_PARTYEXP_MOB", LoadTBPartyExpMobDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_PARTYREVISE", LoadTBPartyReviseDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_PC_AKASHIC", LoadTBPCAkashicDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_PC_COSTUME", LoadTBPCCostumeDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_PC_REWARD_SYSTEM", LoadTBPCRewardSystemDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_PC_REWARD_SYSTEM_MONTH", LoadTBPCRewardSystemMonthDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_PHOTO_ITEM", LoadTBPhotoItemDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_POLLEN", LoadTBPollenDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_PROVIDE_ITEM", LoadTBProvideItemDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_QUEST_CHAPTER", LoadTBQuestChapterDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_QUEST_CONDITION", LoadTBQuestConditionDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_QUEST_EPISODE", LoadTBQuestEpisodeDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_QUEST_REWARD", LoadTBQuestRewardDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_QUEST_SEASON", LoadTBQuestSeasonDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_RANDOM_CHANGE", LoadTBRandomChangeDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_RANDOM_GET", LoadTBRandomGetDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_RANDOM_GET_GROUP", LoadTBRandomGetGroupDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_RANDOM_OPTION", LoadTBRandomOptionDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_RANK_CHANGE", LoadTBRankChangeDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_RANK_INFO", LoadTBRankInfoDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_RANK_REWARD", LoadTBRankRewardDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_RANK_REWARD_TOTALPOINT", LoadTBRankRewardTotalPointDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_REINFORCE", LoadTBReinforceDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_REINFORCE_EXCHANGE", LoadTBReinforceExchangeDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_REINFORCE_OPTION", LoadTBReinforceOptionDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_REPACKAGECOSTUME", LoadTBRepackageCostumeDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_ROOMRANK_REWARD", LoadTBRoomRankRewardDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_SECTORQUEST", LoadTBSectorQuestDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_SELECT_ITEM", LoadTBSelectItemDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_SERVERINFO", LoadTBServerInfoDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_SHOP", LoadTBShopDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_SKILL", LoadTBSkillDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_SKILL_ACTION", LoadTBSkillActionDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_SKILL_OPTION", LoadTBSkillOptionDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_SKILL_SLOT_EXTEND", LoadTBSkillSlotExtendDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_SOCIAL_ITEM", LoadTBSocialItemDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_SOCIAL_MOTION", LoadTBSocialMotionDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_SOCKET", LoadTBSocketDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_SOUL_GUAGE", LoadTBSoulGuageDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_SOUL_METRY", LoadTBSoulMetryDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_SOULSTONE_LEVELUP", LoadTBSoulStoneLevelUpDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_SPAWN_GROUP", LoadTBSpawnGroupDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_SPAWNBOX_FRONTLINE_01", LoadTBSpawnBoxFrontLine01DB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_SPAWNBOX_RSQUARE_01", LoadTBSpawnBoxRSquare01DB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_SPECIALMAZE_OPEN", LoadTBSpecialMazeOpenDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_STATUS", LoadTBStatusDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_SYSTEMMAIL", LoadTBSystemMailDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_SYSTEMMAIL_ADD", LoadTBSystemMailAddDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_TALK", LoadTBTalkDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_TALK_LIST", LoadTBTalkListDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_TALK_STRING", LoadTBTalkStringDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_TITLE_INFO", LoadTBTitleInfoDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_TITLE_REWARD", LoadTBTitleRewardDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_TITLE_STRING", LoadTBTitleStringDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_TRANSPORT_INFO", LoadTBTransportInfoDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_UNITY_EVENT", LoadTBUnityEventDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_VERSION", LoadTBVersionDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_WARLORD_EVENT", LoadTBWarlordEventDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_WARLORD_GUI", LoadTBWarlordGUIDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_WEAPON_RATE", LoadTBWeaponRateDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_WEEK_DAY", LoadTBWeekDayDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_WEEK_GROUP", LoadTBWeekGroupDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_WEEK_MISSION", LoadTBWeekMissionDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_WORLD_EVENT", LoadTBWorldEventDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        result = GreenDamTan_LogTableLoadStep("Load_TB_WORLD_EVENT_REWARD", LoadTBWorldEventRewardDB());
        if ((result & ~1LL) != 0) {
            return result;
        }

        return 0;
    }

    std::int64_t ServerOptionLoad() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult =
            xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_SERVER_OPTION_LOAD }")));

        std::uint8_t byUseAttendance = 1;
        std::uint8_t byUseSecondPW = 1;
        std::uint8_t byUsePvpDistrict = 1;
        std::uint8_t byRanking = 1;
        std::uint8_t byCashshop = 1;
        std::uint8_t byD6Mode = 1;
        std::uint8_t byBroachEvent = 1;
        std::uint8_t byOverIndulgence = 1;
        std::uint8_t byWeeklyMission = 1;
        std::uint8_t byNetCafe = 1;
        std::uint8_t byItemExchange = 1;
        std::uint8_t byWaitSystem = 1;
        std::uint8_t byOperationMaze = 1;

        if ((executeResult & ~1LL) == 0) {
            const std::int64_t fetchResult = xDBBinder.Fetch();
            if ((fetchResult & ~1LL) == 0) {
                std::int64_t strLenOrInd = 0;
                std::uint16_t outParam = xDBBinder.m_sOutParam++;
                m_xDBStmt.SQLGetData(outParam, -28, &byUseAttendance, 0, &strLenOrInd);
                strLenOrInd = 0;
                outParam = xDBBinder.m_sOutParam++;
                m_xDBStmt.SQLGetData(outParam, -28, &byUseSecondPW, 0, &strLenOrInd);
                strLenOrInd = 0;
                outParam = xDBBinder.m_sOutParam++;
                m_xDBStmt.SQLGetData(outParam, -28, &byUsePvpDistrict, 0, &strLenOrInd);
                strLenOrInd = 0;
                outParam = xDBBinder.m_sOutParam++;
                m_xDBStmt.SQLGetData(outParam, -28, &byRanking, 0, &strLenOrInd);
                strLenOrInd = 0;
                outParam = xDBBinder.m_sOutParam++;
                m_xDBStmt.SQLGetData(outParam, -28, &byCashshop, 0, &strLenOrInd);
                strLenOrInd = 0;
                outParam = xDBBinder.m_sOutParam++;
                m_xDBStmt.SQLGetData(outParam, -28, &byD6Mode, 0, &strLenOrInd);
                strLenOrInd = 0;
                outParam = xDBBinder.m_sOutParam++;
                m_xDBStmt.SQLGetData(outParam, -28, &byBroachEvent, 0, &strLenOrInd);
                strLenOrInd = 0;
                outParam = xDBBinder.m_sOutParam++;
                m_xDBStmt.SQLGetData(outParam, -28, &byOverIndulgence, 0, &strLenOrInd);
                strLenOrInd = 0;
                outParam = xDBBinder.m_sOutParam++;
                m_xDBStmt.SQLGetData(outParam, -28, &byWeeklyMission, 0, &strLenOrInd);
                strLenOrInd = 0;
                outParam = xDBBinder.m_sOutParam++;
                m_xDBStmt.SQLGetData(outParam, -28, &byNetCafe, 0, &strLenOrInd);
                strLenOrInd = 0;
                outParam = xDBBinder.m_sOutParam++;
                m_xDBStmt.SQLGetData(outParam, -28, &byItemExchange, 0, &strLenOrInd);
                strLenOrInd = 0;
                outParam = xDBBinder.m_sOutParam++;
                m_xDBStmt.SQLGetData(outParam, -28, &byWaitSystem, 0, &strLenOrInd);
                strLenOrInd = 0;
                outParam = xDBBinder.m_sOutParam++;
                executeResult = m_xDBStmt.SQLGetData(outParam, -28, &byOperationMaze, 0, &strLenOrInd);
            }
        }

        xDBBinder.Close();
        if ((executeResult & ~1LL) == 0) {
            PS_CONTENTS_INFO contents{};
            contents.bContents[E_SERVER_OPTION_ATTENDANCE] = byUseAttendance != 0;
            contents.bContents[E_SERVER_OPTION_SECOND_PW] = byUseSecondPW != 0;
            contents.bContents[E_SERVER_OPTION_PVP_DISTRICT] = byUsePvpDistrict != 0;
            contents.bContents[E_SERVER_OPTION_RANKING] = byRanking != 0;
            contents.bContents[E_SERVER_OPTION_CASHSHOP] = byCashshop != 0;
            contents.bContents[E_SERVER_OPTION_D6_MODE] = byD6Mode != 0;
            contents.bContents[E_SERVER_OPTION_BROACH_EVENT] = byBroachEvent != 0;
            contents.bContents[E_SERVER_OPTION_OVER_INDULGENCE] = byOverIndulgence != 0;
            contents.bContents[E_SERVER_OPTION_SOUL_WEEKLY_MISSION] = byWeeklyMission != 0;
            contents.bContents[E_SERVER_OPTION_NETCAFE] = byNetCafe != 0;
            contents.bContents[E_SERVER_OPTION_SOUL_EVENT] = false;
            contents.bContents[E_SERVER_OPTION_ITEM_EXCHANGE] = byItemExchange != 0;
            contents.bContents[E_SERVER_OPTION_WAIT_SYSTEM] = byWaitSystem != 0;
            contents.bContents[E_SERVER_OPTION_OPERATION_MAZE] = byOperationMaze != 0;
            SetServerContents(contents);
        }
        return executeResult;
    }

    std::int64_t LoadKRRData() {
        int serverGroup = static_cast<int>(static_cast<std::uint8_t>((m_dwServerID >> 8) & 0xFF));
        XDBBinder xDBBinder(&m_xDBStmt);
        ST_KRR_MONSTER_INFO stKRRInfo{};
        std::uint64_t nGroup = 0;
        std::int64_t strLenOrInd = 0;

        std::uint16_t inParam = xDBBinder.m_sInParam++;
        std::int64_t executeResult =
            m_xDBStmt.SQLBindParameter(inParam, 1, -16, 4, 0, 0, &serverGroup, 0, &strLenOrInd);
        if (executeResult != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        executeResult = xDBBinder.Execute(
            reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_KRR_SYSTEM_LOAD(?)}")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_vecKRRInfo.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        if ((fetchResult & ~1LL) == 0) {
            do {
                strLenOrInd = 0;
                std::uint16_t outParam = xDBBinder.m_sOutParam++;
                m_xDBStmt.SQLGetData(outParam, -18, &stKRRInfo.byChannel, 0, &strLenOrInd);
                strLenOrInd = 0;
                outParam = xDBBinder.m_sOutParam++;
                m_xDBStmt.SQLGetData(outParam, -28, &stKRRInfo.yPos, 0, &strLenOrInd);
                strLenOrInd = 0;
                outParam = xDBBinder.m_sOutParam++;
                m_xDBStmt.SQLGetData(outParam, -18, &stKRRInfo.xPos, 0, &strLenOrInd);
                strLenOrInd = 0;
                outParam = xDBBinder.m_sOutParam++;
                m_xDBStmt.SQLGetData(outParam, 7, &stKRRInfo.zPos, 0, &strLenOrInd);
                strLenOrInd = 0;
                outParam = xDBBinder.m_sOutParam++;
                m_xDBStmt.SQLGetData(outParam, 7, &stKRRInfo.dwRemoveTime, 0, &strLenOrInd);
                strLenOrInd = 0;
                outParam = xDBBinder.m_sOutParam++;
                m_xDBStmt.SQLGetData(outParam,
                                     7,
                                     reinterpret_cast<char*>(&stKRRInfo.dwRemoveTime) + 4,
                                     0,
                                     &strLenOrInd);
                strLenOrInd = 0;
                outParam = xDBBinder.m_sOutParam++;
                executeResult = m_xDBStmt.SQLGetData(outParam, -27, &nGroup, 0, &strLenOrInd);
                stKRRInfo.dwMonsterID = static_cast<unsigned int>(nGroup & 0xFFFFFFFFu);
                stKRRInfo.dwTableID = static_cast<unsigned int>((nGroup >> 32) & 0xFFFFFFFFu);
                m_vecKRRInfo.push_back(stKRRInfo);
                fetchResult = xDBBinder.Fetch();
            } while ((fetchResult & ~1LL) == 0);
        }

        xDBBinder.Close();
        m_bLoadKRRData = true;
        return executeResult;
    }

    std::int64_t ServerChannelInfoLoad(int nWorldID, int nServerID) {
        int nWorldIDa = nWorldID;
        int nServerIDa = nServerID;
        XDBBinder xDBBinder(&m_xDBStmt);
        CHANNEL_INFO stInfo{};
        std::int64_t strLenOrInd = 0;

        std::int64_t bindResult = 0;
        std::uint16_t inParam = xDBBinder.m_sInParam++;
        bindResult = m_xDBStmt.SQLBindParameter(inParam, 1, -16, 4, 0, 0, &nServerIDa, 0, &strLenOrInd);
        if (bindResult != 0) {
            xDBBinder.Close();
            return bindResult;
        }

        strLenOrInd = 0;
        inParam = xDBBinder.m_sInParam++;
        bindResult = m_xDBStmt.SQLBindParameter(inParam, 1, -16, 4, 0, 0, &nWorldIDa, 0, &strLenOrInd);
        if (bindResult != 0) {
            xDBBinder.Close();
            return bindResult;
        }

        std::int64_t executeResult = xDBBinder.Execute(
            reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_SERVER_CHANNEL_LOAD(?,?) }")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        std::int64_t fetchResult = xDBBinder.Fetch();
        if ((fetchResult & ~1LL) == 0) {
            do {
                strLenOrInd = 0;
                std::uint16_t outParam = xDBBinder.m_sOutParam++;
                m_xDBStmt.SQLGetData(outParam, -16, &stInfo.nID, 0, &strLenOrInd);
                strLenOrInd = 0;
                outParam = xDBBinder.m_sOutParam++;
                m_xDBStmt.SQLGetData(outParam, -17, &stInfo.wFrom, 0, &strLenOrInd);
                strLenOrInd = 0;
                outParam = xDBBinder.m_sOutParam++;
                executeResult = m_xDBStmt.SQLGetData(outParam, -17, &stInfo.wTo, 0, &strLenOrInd);
                AddChannelInfo(stInfo);
                fetchResult = xDBBinder.Fetch();
            } while ((fetchResult & ~1LL) == 0);
        }

        xDBBinder.Close();
        return executeResult;
    }

    std::int64_t ServerChannelDistrict6InfoLoad(int nWorldID, int nServerID) {
        int nWorldIDa = nWorldID;
        int nServerIDa = nServerID;
        XDBBinder xDBBinder(&m_xDBStmt);
        CHANNEL_INFO stInfo{};
        std::int64_t strLenOrInd = 0;

        std::int64_t bindResult = 0;
        std::uint16_t inParam = xDBBinder.m_sInParam++;
        bindResult = m_xDBStmt.SQLBindParameter(inParam, 1, -16, 4, 0, 0, &nServerIDa, 0, &strLenOrInd);
        if (bindResult != 0) {
            xDBBinder.Close();
            return bindResult;
        }

        strLenOrInd = 0;
        inParam = xDBBinder.m_sInParam++;
        bindResult = m_xDBStmt.SQLBindParameter(inParam, 1, -16, 4, 0, 0, &nWorldIDa, 0, &strLenOrInd);
        if (bindResult != 0) {
            xDBBinder.Close();
            return bindResult;
        }

        std::int64_t executeResult = xDBBinder.Execute(
            reinterpret_cast<unsigned char*>(const_cast<char*>("{call SP_SERVER_CHANNEL_DISTRICT6_LOAD(?,?) }")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        std::int64_t fetchResult = xDBBinder.Fetch();
        if ((fetchResult & ~1LL) == 0) {
            do {
                strLenOrInd = 0;
                std::uint16_t outParam = xDBBinder.m_sOutParam++;
                m_xDBStmt.SQLGetData(outParam, -16, &stInfo.nID, 0, &strLenOrInd);
                strLenOrInd = 0;
                outParam = xDBBinder.m_sOutParam++;
                m_xDBStmt.SQLGetData(outParam, -17, &stInfo.wFrom, 0, &strLenOrInd);
                strLenOrInd = 0;
                outParam = xDBBinder.m_sOutParam++;
                executeResult = m_xDBStmt.SQLGetData(outParam, -17, &stInfo.wTo, 0, &strLenOrInd);
                AddChannelDistrict6Info(stInfo);
                fetchResult = xDBBinder.Fetch();
            } while ((fetchResult & ~1LL) == 0);
        }

        xDBBinder.Close();
        return executeResult;
    }

    void InitPCCostume() {
        m_mapPCCostume.clear();
        for (auto& entry : m_mapTB_PC_COSTUME) {
            TB_PC_COSTUME& row = entry.second;
            m_mapPCCostume.emplace(row.Item_ID, &row);
        }
    }

    void InitPCAkashic() {
        m_mapPCAkashic.clear();
        for (auto& entry : m_mapTB_PC_AKASHIC) {
            const unsigned int akashicId = entry.second.Akashic_ID;
            auto it = m_mapTB_AKASHIC_RECORDS.find(akashicId);
            if (it == m_mapTB_AKASHIC_RECORDS.end()) {
                LogHelper::LogError("game.system",
                                    "[InitPCAkashic] Error TB_AKASHIC_RECORDS (%d)",
                                    akashicId);
                continue;
            }
            m_mapPCAkashic[akashicId] = &it->second;
        }
    }

    void AddChannelInfo(CHANNEL_INFO& stInfo) {
        m_mapChannelInfo[stInfo.nID].push_back(stInfo);
    }

    void AddChannelDistrict6Info(CHANNEL_INFO& stInfo) {
        m_mapChannelDistrict6Info[stInfo.nID].push_back(stInfo);
    }

    void InitNetCafeMissionList() {
        m_vecNetCafeMission.clear();
        for (const auto& entry : m_mapTB_PC_REWARD_SYSTEM) {
            const TB_PC_REWARD_SYSTEM& row = entry.second;
            if (row.PC_Reward_ISApply == 0) {
                continue;
            }

            ST_NETCAFE_MISSION_INFO mission{};
            mission.dwID = row.ID;
            m_vecNetCafeMission.push_back(mission);
        }
    }

    void InitRoguelikeData() {
        m_mapRoguelikeData.clear();
        m_setRoguelikeMap.clear();
        m_mapRoguelikeRandomSklll.clear();
        m_mapRoguelikeShopSklll_LV.clear();
        m_mapRoguelikeShopSklll_Count.clear();
        m_vecRoguelikePortalBuff.clear();

        for (auto& entry : m_mapTB_MODE_BI_SECTOR_INFO) {
            TB_MODE_BI_SECTOR_INFO& row = entry.second;
            unsigned int sectorId = 0;
            unsigned int bindMaze = 0;
            std::memcpy(&sectorId, &row.ID, sizeof(sectorId));
            std::memcpy(&bindMaze, &row.BI_Sector_Bind_Maze, sizeof(bindMaze));
            const unsigned int key = (sectorId % 100u) + 100u * bindMaze;
            m_mapRoguelikeData.emplace(key, &row);
            m_setRoguelikeMap.emplace(bindMaze);
        }

        for (const auto& entry : m_mapTB_MODE_BI_UPGRADE) {
            const TB_MODE_BI_UPGRADE& row = entry.second;
            unsigned int upgradeId = 0;
            std::uint8_t upgradeClass = 0;
            std::uint8_t fixType = 0;
            std::memcpy(&upgradeId, &row.ID, sizeof(upgradeId));
            std::memcpy(&upgradeClass, &row.Upgrade_Class, sizeof(upgradeClass));
            std::memcpy(&fixType, &row.Upgrade_Skill_Fix_Type, sizeof(fixType));
            if (upgradeClass == 0) {
                continue;
            }

            if (fixType == 0) {
                m_mapRoguelikeRandomSklll[static_cast<int>(upgradeClass)].push_back(upgradeId);
            } else if (fixType == 1) {
                m_mapRoguelikeShopSklll_LV[static_cast<int>(upgradeClass)].push_back(upgradeId);
            } else if (fixType == 2) {
                m_mapRoguelikeShopSklll_Count[static_cast<int>(upgradeClass)].push_back(upgradeId);
            }
        }

        for (const auto& entry : m_mapTB_BUFF) {
            const TB_BUFF& row = entry.second;
            std::uint16_t buffGroup = 0;
            std::uint16_t buffIndex = 0;
            std::memcpy(&buffGroup, &row.Buff_Group, sizeof(buffGroup));
            std::memcpy(&buffIndex, &row.Buff_Index, sizeof(buffIndex));
            if (buffGroup == 1000) {
                m_vecRoguelikePortalBuff.push_back(buffIndex);
            }
        }
    }


    std::string m_strCommonDNS;
    std::string m_strGameDNS;
    std::string m_strPath;
    bool m_bGameDBLoad = false;
    unsigned int m_dwServerID = 0;
    int m_nMainVersion = 0;
    int m_nSubVersion = 0;
    int m_nDataVersion = 0;
    std::map<unsigned int, unsigned int> m_mapQuestCompleteBit;
    std::map<unsigned short, std::map<unsigned short, FIRST_STATUS_TABLE>> m_mapFirstStatus;
    std::map<unsigned int, std::uint8_t> m_xFaction;
    std::map<unsigned short, std::int16_t> m_mapInfiniteTower;
    std::map<int, int> m_mapPrefixTitleOpenCondition[11][9];
    std::map<int, int> m_mapSuffixTitleOpenCondition[11][9];
    std::map<std::pair<unsigned int, std::uint8_t>, TB_PARTYREVISE*> m_mapPartyRevise;
    std::array<bool, 256> m_bOpenCharacterClass{};
    int m_nOpenCharacterCount = 0;
    std::vector<std::uint16_t> m_vecCasualMazeID;
    std::map<int, std::list<int>> m_mapLevelEventID;
    std::map<std::uint8_t, std::vector<TB_MODE_DISTRICT6*>> m_mpWorldModeGroup;
    std::map<std::uint8_t, std::vector<TB_MODE_DISTRICT6_DATE*>> m_mpWorldModeDate;
    std::map<int, std::list<unsigned int>> m_mapDayEvent;
    std::list<std::uint8_t> m_listWeeklyEvent;
    std::map<ST_RANK_INFO_KEY, TB_RANK_INFO*> m_mapRankInfoTable;
    std::vector<ST_RANKING_INFO> m_vecRankInfoList;
    std::map<unsigned int, TB_PC_COSTUME*> m_mapPCCostume;
    std::vector<ST_NETCAFE_MISSION_INFO> m_vecNetCafeMission;
    std::map<unsigned int, TB_MODE_BI_SECTOR_INFO*> m_mapRoguelikeData;
    std::set<unsigned int> m_setRoguelikeMap;
    std::map<int, std::vector<unsigned int>> m_mapRoguelikeRandomSklll;
    std::map<int, std::vector<unsigned int>> m_mapRoguelikeShopSklll_LV;
    std::map<int, std::vector<unsigned int>> m_mapRoguelikeShopSklll_Count;
    std::vector<unsigned int> m_vecRoguelikePortalBuff;
    std::map<unsigned int, unsigned int> m_mapAutoAcceptQuest;
    std::map<unsigned int, unsigned int> m_mapMainQuestOrder_Char1;
    unsigned int m_dwMainQuestLastOrder_Char1 = 0;
    std::map<unsigned int, unsigned int> m_mapMainQuestOrder_Char2;
    unsigned int m_dwMainQuestLastOrder_Char2 = 0;
    std::map<unsigned int, unsigned int> m_mapMainQuestOrder_Char3;
    unsigned int m_dwMainQuestLastOrder_Char3 = 0;
    std::vector<unsigned int> m_vecQuestEpisodeOtherContents;
    std::map<int, std::vector<CHANNEL_INFO>> m_mapChannelInfo;
    std::map<int, std::vector<CHANNEL_INFO>> m_mapChannelDistrict6Info;
    std::vector<ST_KRR_MONSTER_INFO> m_vecKRRInfo;
    bool m_bLoadKRRData = false;
    std::unordered_map<int, bool> serverContents_;
    PS_CONTENTS_INFO contentsInfo_{};
    XDBManager m_xCommonDBMgr;
    XDBManager m_xGameDBMgr;
    XDBStmt m_xDBStmt;
    XDBStmt m_xGameDBStmt;

    // Member variables for non-GetTB functions
    unsigned int m_dwCashItemVersion = 0;
    int m_nRevision = 0;
    int m_nRealPVPOption = 0;
    bool m_bCheckMazeOpenTime = false;
    std::vector<unsigned int> m_vecQuestListOtherMain;

    // Banner 数据存储 (从 ControlServer 同步)
    std::vector<ST_BANNER_INFO> m_vecBannerInfo;

public:
    // 对齐 IDA: XResourceMgr::GetBannerInfo - 获取 Banner 列表
    void GetBannerInfo(ST_BANNER_LIST* pList) {
        if (!pList) return;
        pList->vecInfo = m_vecBannerInfo;
    }

    // 设置 Banner 列表 (从 ControlServer 接收)
    void SetBannerInfo(const ST_BANNER_LIST& stList) {
        m_vecBannerInfo = stList.vecInfo;
    }

    // ========== Non-GetTB functions from IDA ==========

    /**
     * @brief 获取商城版本号
     * @note IDA: 0x1402F6C70 XResourceMgr::GetCashshopVersion
     */
    unsigned int GetCashshopVersion() {
        return m_dwCashItemVersion;
    }

    /**
     * @brief 获取版本修订号
     * @note IDA: 0x140638BD0 XResourceMgr::GetRevision
     */
    int GetRevision() {
        return m_nRevision;
    }

    /**
     * @brief 获取开放角色数量
     * @note IDA: 0x140411F90 XResourceMgr::GetOpenCharacterCount
     */
    int GetOpenCharacterCount() {
        return m_nOpenCharacterCount;
    }

    /**
     * @brief 判断角色职业是否开放
     * @param nClass 角色职业ID
     * @note IDA: 0x1408B34D0 XResourceMgr::IsOpenCharacter
     */
    bool IsOpenCharacter(int nClass) {
        if (nClass < 0 || nClass >= static_cast<int>(m_bOpenCharacterClass.size())) {
            return false;
        }
        return m_bOpenCharacterClass[nClass];
    }

    /**
     * @brief 获取自动接受任务列表
     * @note IDA: 0x1408B3960 XResourceMgr::GetAutoAcceptQuest
     */
    std::map<unsigned int, unsigned int>* GetAutoAcceptQuest() {
        return &m_mapAutoAcceptQuest;
    }

    /**
     * @brief 获取主线任务最后顺序
     * @param byClass 角色职业 (默认返回Char1, 7返回Char2, 其他返回Char1)
     * @note IDA: 0x1408B3970 XResourceMgr::GetMainQuestLastOrder
     */
    unsigned int GetMainQuestLastOrder(std::uint8_t byClass = 0) {
        if (byClass == 7) {
            return m_dwMainQuestLastOrder_Char2;
        } else if (byClass == 8) {
            return m_dwMainQuestLastOrder_Char3;
        }
        return m_dwMainQuestLastOrder_Char1;
    }

    /**
     * @brief 获取真实PVP选项
     * @note IDA: 0x1401E7BB0 XResourceMgr::GetRealPVPOption
     */
    int GetRealPVPOption() {
        return m_nRealPVPOption;
    }

    /**
     * @brief 设置真实PVP选项
     * @param nState PVP状态
     * @note IDA: 0x1408B3950 XResourceMgr::SetRealPVPOption
     */
    void SetRealPVPOption(int nState) {
        m_nRealPVPOption = nState;
    }

    /**
     * @brief 获取Roguelike传送门Buff列表
     * @note IDA: 0x1408B3990 XResourceMgr::GetRoguelikePortalBuff
     */
    std::vector<unsigned int>* GetRoguelikePortalBuff() {
        return &m_vecRoguelikePortalBuff;
    }

    /**
     * @brief 获取签到类型
     * @note IDA: 0x1408D1820 XResourceMgr::GetCheckAttendanceType
     */
    unsigned int GetCheckAttendanceType() {
        if (m_mapTB_CHECK_ATTENDANCE_INFO.empty()) {
            return 0;
        }
        auto it = m_mapTB_CHECK_ATTENDANCE_INFO.begin();
        return it->second.Check_Attendance_Type;
    }

    /**
     * @brief 获取连续签到类型
     * @note IDA: 0x1408D1840 XResourceMgr::GetCheckAttendanceContinueType
     */
    unsigned int GetCheckAttendanceContinueType() {
        if (m_mapTB_CHECK_ATTENDANCE_STREAK.empty()) {
            return 0;
        }
        auto it = m_mapTB_CHECK_ATTENDANCE_STREAK.begin();
        return it->second.Check_Attendance_Type;
    }

    /**
     * @brief 获取游戏时间签到类型
     * @note IDA: 0x1408D1860 XResourceMgr::GetCheckAttendancePlayTimeType
     */
    unsigned int GetCheckAttendancePlayTimeType() {
        if (m_mapTB_CHECK_ACCESS_REWARD.empty()) {
            return 0;
        }
        auto it = m_mapTB_CHECK_ACCESS_REWARD.begin();
        return it->second.Check_Attendance_Type;
    }

    /**
     * @brief 获取阵营关系
     * @param byNation 本方阵营
     * @param byTargetNation 目标阵营
     * @return 阵营关系 (0=敌对, 2=友好/同阵营)
     * @note IDA: 0x1408D56D0 XResourceMgr::GetFaction
     */
    std::uint8_t GetFaction(std::uint8_t byNation, std::uint8_t byTargetNation) {
        if (byNation == byTargetNation) {
            return 2;  // 同阵营
        }

        const unsigned int key = static_cast<unsigned int>(byTargetNation) |
                                 (static_cast<unsigned int>(byNation) << 8);
        auto it = m_xFaction.find(key);
        if (it == m_xFaction.end()) {
            return 0;  // 敌对
        }
        return it->second;
    }

    /**
     * @brief 设置检查迷宫开放时间标志
     * @param bOn 是否开启
     * @note IDA: 0x140406D00 XResourceMgr::SetCheckMazeOpenTime
     */
    void SetCheckMazeOpenTime(bool bOn) {
        m_bCheckMazeOpenTime = bOn;
    }

    /**
     * @brief 获取其他主线任务列表
     * @param vecList 输出任务ID列表
     * @note IDA: 0x140144670 XResourceMgr::GetQuestListOtherMain
     */
    void GetQuestListOtherMain(std::vector<unsigned int>& vecList) {
        vecList = m_vecQuestListOtherMain;
    }

    /**
     * @brief 获取Roguelike奖励
     * @param nMoney 当前金币
     * @param nItemID 输出物品ID
     * @param nCount 输出物品数量
     * @note IDA: 0x1408D1880 XResourceMgr::GetRoguelikeReward
     */
    void GetRoguelikeReward(int nMoney, unsigned int& nItemID, unsigned int& nCount) {
        nItemID = 0;
        nCount = 0;

        for (auto& entry : m_mapTB_MODE_BI_REWARD) {
            TB_MODE_BI_REWARD& row = entry.second;
            if (nMoney >= row.BI_Reward_Coin_Value) {
                nItemID = row.BI_Reward_Item;
                nCount = row.BI_Reward_Item_Value;
            } else {
                break;
            }
        }
    }

    /**
     * @brief 获取队伍修正表
     * @param dwGroupID 组ID
     * @param byCount 成员数量
     * @return 队伍修正表指针
     * @note IDA: 0x1408D5AB0 XResourceMgr::GetPartyReviseTable
     */
    TB_PARTYREVISE* GetPartyReviseTable(unsigned int dwGroupID, std::uint8_t byCount) {
        auto it = m_mapPartyRevise.find(std::make_pair(dwGroupID, byCount));
        return it == m_mapPartyRevise.end() ? nullptr : it->second;
    }

    /**
     * @brief 获取前缀称号开启条件
     * @param nCondition 开启条件类型
     * @param nClass 角色职业
     * @param nValue 条件值
     * @return 称号ID
     * @note IDA: 0x1408D57D0 XResourceMgr::GetPrefixTitleToOpen
     */
    int GetPrefixTitleToOpen(int nCondition, int nClass, int nValue) {
        if (nCondition >= 11 || nClass >= 9) {
            return 0;
        }

        // 首先查找特定职业的称号
        auto& titleMap = m_mapPrefixTitleOpenCondition[nCondition][nClass];
        auto it = titleMap.lower_bound(nValue);
        if (it != titleMap.end() && it->first == nValue) {
            return it->second;
        }
        if (it != titleMap.begin()) {
            --it;
            return it->second;
        }

        // 如果没有找到，查找通用职业(class=0)的称号
        auto& classZeroMap = m_mapPrefixTitleOpenCondition[nCondition][0];
        auto it2 = classZeroMap.lower_bound(nValue);
        if (it2 != classZeroMap.end() && it2->first == nValue) {
            return it2->second;
        }
        if (it2 != classZeroMap.begin()) {
            --it2;
            return it2->second;
        }

        return 0;
    }

    /**
     * @brief 获取后缀称号开启条件
     * @param nCondition 开启条件类型
     * @param nClass 角色职业
     * @param nValue 条件值
     * @return 称号ID
     * @note IDA: 0x1408D58E0 XResourceMgr::GetSuffixTitleToOpen
     */
    int GetSuffixTitleToOpen(int nCondition, int nClass, int nValue) {
        if (nCondition >= 11 || nClass >= 9) {
            return 0;
        }

        // 首先查找特定职业的称号
        auto& titleMap = m_mapSuffixTitleOpenCondition[nCondition][nClass];
        auto it = titleMap.lower_bound(nValue);
        if (it != titleMap.end() && it->first == nValue) {
            return it->second;
        }
        if (it != titleMap.begin()) {
            --it;
            return it->second;
        }

        // 如果没有找到，查找通用职业(class=0)的称号
        auto& classZeroMap = m_mapSuffixTitleOpenCondition[nCondition][0];
        auto it2 = classZeroMap.lower_bound(nValue);
        if (it2 != classZeroMap.end() && it2->first == nValue) {
            return it2->second;
        }
        if (it2 != classZeroMap.begin()) {
            --it2;
            return it2->second;
        }

        return 0;
    }

    /**
     * @brief 检查迷宫开放时间
     * @param dwMazeID 迷宫ID
     * @return 是否开放
     * @note IDA: 0x1408D5B40 XResourceMgr::CheckMazeOpenTime
     */
    bool CheckMazeOpenTime(unsigned int dwMazeID) {
        if (!m_bCheckMazeOpenTime) {
            return true;
        }

        std::time_t now = std::time(nullptr);
        std::tm localTime{};
#ifdef _WIN32
        if (localtime_s(&localTime, &now) != 0) {
            return false;
        }
#else
        if (localtime_r(&now, &localTime) == nullptr) {
            return false;
        }
#endif

        const int weekDay = localTime.tm_wday + 1;
        const unsigned int v6 = 100 * dwMazeID;
        const unsigned int v7 = 100 * (v6 + weekDay) + 1;
        const unsigned int v10 = 100 * (v6 + weekDay) + 20;

        if (v7 > v10) {
            return true;
        }

        bool found = false;
        for (unsigned int key = v7; key <= v10; ++key) {
            auto it = m_mapTB_MAZE_OPENCONTROL.find(key);
            if (it == m_mapTB_MAZE_OPENCONTROL.end()) {
                continue;
            }

            found = true;
            const TB_MAZE_OPENCONTROL& row = it->second;
            const unsigned int openHour = row.Open_Time / 100;
            const unsigned int openMin = row.Open_Time % 100;

            std::tm openTm = localTime;
            openTm.tm_hour = openHour;
            openTm.tm_min = openMin;
            openTm.tm_sec = 0;
            openTm.tm_isdst = -1;

            std::time_t openTime = std::mktime(&openTm);
            if (openTime == -1) {
                continue;
            }

            std::time_t closeTime = openTime + static_cast<std::time_t>(row.Remain_Time) * 60;

            if (now >= openTime && now <= closeTime) {
                return true;
            }
        }

        return !found;
    }


};
