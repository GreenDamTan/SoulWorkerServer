#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "Soulworker/Common/XNet/XCommon/PSOption.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XCore/XServer/TXDBSocket.h"

struct TB_SYSTEMMAIL_ADD {
    std::uint16_t AutoMail_ADD_ID = 0;
    char Des[511] = {};
    std::uint8_t MailBox_Type = 0;
    std::uint8_t AutoMail_Type_ID = 0;
    std::uint8_t AutoMail_Type_On_Off = 0;
    std::uint16_t AutoMail_Time_Value = 0;
    std::uint8_t Link_SystemMail_ID = 0;
    unsigned int AutoMail_ADD_Gold = 0;
};

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

#pragma pack(push, 1)
struct TB_APPEARANCE {
    unsigned int Appearance_ID = 0;
    std::uint8_t Appearance_Classify = 0;
    std::uint8_t Appearance_Type = 0;
    std::uint8_t Appearance_Class = 0;
};

struct TB_CREATE_CLOTH {
    std::uint16_t ID = 0;
    unsigned int Item_ID_Head = 0;
    unsigned int Item_ID_Body = 0;
    unsigned int Item_ID_Hands = 0;
    unsigned int Item_ID_Stocking = 0;
    unsigned int Item_ID_Foot = 0;
    unsigned int Item_ID_Pants = 0;
    char Icon_FileName[511] = {};

    std::array<unsigned int, 6> GetItemIDs() const {
        return {Item_ID_Head, Item_ID_Body, Item_ID_Hands, Item_ID_Stocking, Item_ID_Foot, Item_ID_Pants};
    }
};

struct TB_CHARACTER_INFO {
    std::uint16_t ID = 0;
    std::uint8_t Character_ID = 0;
    std::uint16_t Character_Group_ID = 0;
    char PC_Code_Name[511] = {};
    unsigned int Item_ID_Weapon_01 = 0;
    unsigned int Default_Cloth_ID_01 = 0;
    unsigned int Default_Cloth_ID_02 = 0;
    unsigned int Default_Cloth_ID_03 = 0;
    unsigned int Default_Cloth_ID_04 = 0;
    std::uint16_t Default_Provide_Item = 0;
    std::uint16_t District_ID = 0;
    std::uint16_t District_Position_X = 0;
    std::uint16_t District_Position_Y = 0;
    std::uint16_t District_Position_Z = 0;
    std::uint16_t Maze_ID = 0;
    unsigned int Maze_Spawnbox_ID = 0;
    unsigned int Character_Start_Quest_ID = 0;
    unsigned int Character_Skip_Quest_ID = 0;
    std::uint8_t Character_Status_Type = 0;
    unsigned int Character_Status = 0;
    std::uint16_t Char_Height_Value = 0;
    std::uint16_t Nomal_Walk_Speed = 0;
    std::uint16_t Nomal_Run_Speed = 0;
    std::uint16_t Nomal_JumpMove_Speed = 0;
    std::uint16_t Battle_Run_Speed = 0;
    std::uint16_t Battle_JumpMove_Speed = 0;
    std::uint16_t Battle_Dash_Speed = 0;
    float Battle_BackMove_Ratio = 0.0f;
    std::uint8_t PC_Faction = 0;
    std::uint16_t Char_CollisionRadius = 0;
    std::uint16_t Char_CollisionHeight = 0;
    std::uint16_t Char_RealHeight = 0;
    unsigned int Default_Skill_ID_01 = 0;
    unsigned int Default_Skill_ID_02 = 0;
    unsigned int Default_Skill_ID_03 = 0;
    unsigned int Default_Skill_ID_04 = 0;
    unsigned int Default_Skill_ID_05 = 0;
    unsigned int Default_Skill_ID_06 = 0;
    unsigned int Default_Skill_ID_07 = 0;
    unsigned int Default_Skill_ID_08 = 0;
    unsigned int Default_Skill_ID_09 = 0;
    unsigned int Default_Skill_ID_10 = 0;
    unsigned int Default_Skill_ID_11 = 0;
    unsigned int Default_Skill_ID_12 = 0;
    unsigned int Default_Skill_ID_13 = 0;
    unsigned int Default_Skill_ID_14 = 0;
    unsigned int Default_Skill_ID_15 = 0;
    unsigned int Default_Skill_ID_16 = 0;
    unsigned int Default_Skill_ID_17 = 0;
    unsigned int Default_Skill_ID_18 = 0;
    unsigned int Default_Skill_ID_19 = 0;
    unsigned int Default_Skill_ID_20 = 0;
    std::uint8_t Set_Quick_Slot_ID_01 = 0;
    std::uint8_t Set_Quick_Slot_ID_02 = 0;
    std::uint8_t Set_Quick_Slot_ID_03 = 0;
    std::uint8_t Set_Quick_Slot_ID_04 = 0;
    std::uint8_t Set_Quick_Slot_ID_05 = 0;
    std::uint8_t Set_Quick_Slot_ID_06 = 0;
    std::uint8_t Set_Quick_Slot_ID_07 = 0;
    std::uint8_t Set_Quick_Slot_ID_08 = 0;
    std::uint8_t Set_Quick_Slot_ID_09 = 0;
    std::uint8_t Set_Quick_Slot_ID_10 = 0;
    std::uint8_t Set_Quick_Slot_ID_11 = 0;
    std::uint8_t Set_Quick_Slot_ID_12 = 0;
    std::uint8_t Set_Quick_Slot_ID_13 = 0;
    std::uint8_t Set_Quick_Slot_ID_14 = 0;
    std::uint8_t Set_Quick_Slot_ID_15 = 0;
    std::uint8_t Set_Quick_Slot_ID_16 = 0;
    std::uint8_t Set_Quick_Slot_ID_17 = 0;
    std::uint8_t Set_Quick_Slot_ID_18 = 0;
    std::uint8_t Set_Quick_Slot_ID_19 = 0;
    std::uint8_t Set_Quick_Slot_ID_20 = 0;
    std::uint16_t Set_Gesture_Slot_ID_01 = 0;
    std::uint16_t Set_Gesture_Slot_ID_02 = 0;
    std::uint16_t Set_Gesture_Slot_ID_03 = 0;
    std::uint16_t Set_Gesture_Slot_ID_04 = 0;
    std::uint16_t Set_Gesture_Slot_ID_05 = 0;
    std::uint16_t Set_Gesture_Slot_ID_06 = 0;
    float Akashic_Active_Time = 0.0f;
    std::uint8_t Character_On_Type = 0;
    std::uint8_t Character_On = 0;

    std::array<unsigned int, 20> GetDefaultSkillIDs() const {
        return {Default_Skill_ID_01, Default_Skill_ID_02, Default_Skill_ID_03, Default_Skill_ID_04,
                Default_Skill_ID_05, Default_Skill_ID_06, Default_Skill_ID_07, Default_Skill_ID_08,
                Default_Skill_ID_09, Default_Skill_ID_10, Default_Skill_ID_11, Default_Skill_ID_12,
                Default_Skill_ID_13, Default_Skill_ID_14, Default_Skill_ID_15, Default_Skill_ID_16,
                Default_Skill_ID_17, Default_Skill_ID_18, Default_Skill_ID_19, Default_Skill_ID_20};
    }

    std::array<std::uint8_t, 20> GetQuickSlotIDs() const {
        return {Set_Quick_Slot_ID_01, Set_Quick_Slot_ID_02, Set_Quick_Slot_ID_03, Set_Quick_Slot_ID_04,
                Set_Quick_Slot_ID_05, Set_Quick_Slot_ID_06, Set_Quick_Slot_ID_07, Set_Quick_Slot_ID_08,
                Set_Quick_Slot_ID_09, Set_Quick_Slot_ID_10, Set_Quick_Slot_ID_11, Set_Quick_Slot_ID_12,
                Set_Quick_Slot_ID_13, Set_Quick_Slot_ID_14, Set_Quick_Slot_ID_15, Set_Quick_Slot_ID_16,
                Set_Quick_Slot_ID_17, Set_Quick_Slot_ID_18, Set_Quick_Slot_ID_19, Set_Quick_Slot_ID_20};
    }

    std::array<std::uint16_t, 6> GetGestureSlotIDs() const {
        return {Set_Gesture_Slot_ID_01, Set_Gesture_Slot_ID_02, Set_Gesture_Slot_ID_03,
                Set_Gesture_Slot_ID_04, Set_Gesture_Slot_ID_05, Set_Gesture_Slot_ID_06};
    }
};
#pragma pack(pop)

static_assert(sizeof(TB_CHARACTER_INFO) == 0x2C4, "TB_CHARACTER_INFO size must match PDB");

#pragma pack(push, 2)
struct TB_PROVIDE_ITEM {
    std::uint16_t ID = 0;
    unsigned int uniItem_ID[40] = {};
    unsigned int uniItem_Count[40] = {};

    std::array<unsigned int, 40> GetItemIDs() const {
        std::array<unsigned int, 40> result{};
        std::memcpy(result.data(), uniItem_ID, sizeof(uniItem_ID));
        return result;
    }

    std::array<unsigned int, 40> GetItemCounts() const {
        std::array<unsigned int, 40> result{};
        std::memcpy(result.data(), uniItem_Count, sizeof(uniItem_Count));
        return result;
    }
};
#pragma pack(pop)

struct TB_PHOTO_ITEM {
    unsigned int ID = 0;
    unsigned int Photo_Name = 0;
    std::uint16_t Photo_Group = 0;
    std::uint8_t Char_Class = 0;
    std::uint8_t Char_Promotion_Info = 0;
};

#pragma pack(push, 1)
struct TB_QUEST_EPISODE {
    unsigned int ID = 0;
    unsigned int Complete_Bit = 0;
    char Name[511] = {};
    unsigned int Chapter_ID = 0;
    std::uint8_t Contents_Type = 0;
    std::uint8_t Progress_Type = 0;
    std::uint8_t Repeat_Value = 0;
    unsigned int Before_Episode_ID_1 = 0;
    unsigned int Before_Episode_ID_2 = 0;
    unsigned int Before_Episode_ID_3 = 0;
    unsigned int Before_Episode_ID_4 = 0;
    unsigned int Before_Episode_ID_5 = 0;
    unsigned int Before_Episode_ID_6 = 0;
    unsigned int Before_Episode_ID_7 = 0;
    unsigned int Before_Episode_ID_8 = 0;
    unsigned int Before_Episode_ID_9 = 0;
    unsigned int Before_Episode_ID_10 = 0;
    unsigned int Link_Episode_ID_01 = 0;
    unsigned int Link_Episode_ID_02 = 0;
    unsigned int Link_Episode_ID_03 = 0;
    unsigned int Link_Episode_ID_04 = 0;
    unsigned int Link_Episode_ID_05 = 0;
    unsigned int Link_Episode_ID_06 = 0;
    unsigned int Link_Episode_ID_07 = 0;
    unsigned int Link_Episode_ID_08 = 0;
    unsigned int Link_Episode_ID_09 = 0;
    unsigned int Link_Episode_ID_10 = 0;
    unsigned int Time_Limit = 0;
    std::uint8_t Quest_Level = 0;
    std::uint16_t Class_Type = 0;
    std::uint8_t Auto_Start = 0;
    std::uint8_t Auto_Start_Event = 0;
    unsigned int Auto_Start_Event_ID = 0;
    std::uint8_t Start_Object_Type = 0;
    unsigned int Start_Object_ID = 0;
    std::uint8_t Finish_Type = 0;
    std::uint8_t Finish_Object_Type = 0;
    unsigned int Finish_Object_ID = 0;
    unsigned int Field_ID = 0;
    unsigned int Maze_group_ID = 0;
    unsigned int Maze_ID_1 = 0;
    unsigned int Maze_ID_2 = 0;
    unsigned int Maze_ID_3 = 0;
    unsigned int Maze_ID_4 = 0;
    unsigned int Sector_ID_1 = 0;
    unsigned int Sector_ID_2 = 0;
    unsigned int Sector_ID_3 = 0;
    unsigned int Sector_ID_4 = 0;
    std::uint8_t Give_Up = 0;
    std::uint8_t Add_Object_Type_01 = 0;
    std::uint8_t Add_Object_Type_02 = 0;
    unsigned int Add_Object_ID_01 = 0;
    unsigned int Add_Object_ID_02 = 0;
    unsigned int Remove_Item_ID_01 = 0;
    unsigned int Remove_Item_ID_02 = 0;
    unsigned int Remove_Item_ID_03 = 0;
    unsigned int Remove_Item_ID_04 = 0;
    unsigned int Remove_Item_ID_05 = 0;
    unsigned int Remove_Item_Count_01 = 0;
    unsigned int Remove_Item_Count_02 = 0;
    unsigned int Remove_Item_Count_03 = 0;
    unsigned int Remove_Item_Count_04 = 0;
    unsigned int Remove_Item_Count_05 = 0;
    char Quest_Script_Name[511] = {};
    std::uint8_t Episode_Reward_Type = 0;
    unsigned int Reward_EXP = 0;
    unsigned int Reward_Money = 0;
    unsigned int Reward_Ether = 0;
    unsigned int Reward_BP = 0;
    unsigned int Reward_Title_01 = 0;
    unsigned int Reward_Title_02 = 0;
    unsigned int Reward_Awakening = 0;
    std::uint8_t Reward_Item_type = 0;
    unsigned int Reward_Item_ID_01 = 0;
    unsigned int Reward_Item_ID_02 = 0;
    unsigned int Reward_Item_ID_03 = 0;
    unsigned int Reward_Item_ID_04 = 0;
    unsigned int Reward_Item_ID_05 = 0;
    unsigned int Reward_Item_Count_01 = 0;
    unsigned int Reward_Item_Count_02 = 0;
    unsigned int Reward_Item_Count_03 = 0;
    unsigned int Reward_Item_Count_04 = 0;
    unsigned int Reward_Item_Count_05 = 0;
    std::uint8_t Select_Reward_Item_type = 0;
    unsigned int Select_Reward_Item_01 = 0;
    unsigned int Select_Reward_Item_02 = 0;
    unsigned int Select_Reward_Item_03 = 0;
    unsigned int Select_Reward_Item_04 = 0;
    unsigned int Select_Reward_Item_05 = 0;
    unsigned int Select_Reward_Item_Count_01 = 0;
    unsigned int Select_Reward_Item_Count_02 = 0;
    unsigned int Select_Reward_Item_Count_03 = 0;
    unsigned int Select_Reward_Item_Count_04 = 0;
    unsigned int Select_Reward_Item_Count_05 = 0;
    unsigned int Reward_Class_Skill_01 = 0;
    unsigned int Reward_Class_Skill_02 = 0;
    unsigned int Reward_Class_Skill_03 = 0;
    unsigned int Reward_Class_Skill_04 = 0;
    unsigned int Reward_Helper = 0;
    std::uint8_t Condition_Type = 0;
    unsigned int Condition_ID_01 = 0;
    unsigned int Condition_ID_02 = 0;
    unsigned int Condition_ID_03 = 0;
    unsigned int Condition_ID_04 = 0;
    unsigned int Condition_ID_05 = 0;
    unsigned int Condition_ID_06 = 0;
    unsigned int Condition_ID_07 = 0;
    unsigned int Condition_ID_08 = 0;
    unsigned int Condition_ID_09 = 0;
    unsigned int Condition_ID_10 = 0;
    unsigned int Restore_Item = 0;
    std::uint8_t Save_Quest = 0;

    std::array<unsigned int, 10> GetBeforeEpisodeIDs() const {
        return {Before_Episode_ID_1, Before_Episode_ID_2, Before_Episode_ID_3, Before_Episode_ID_4,
                Before_Episode_ID_5, Before_Episode_ID_6, Before_Episode_ID_7, Before_Episode_ID_8,
                Before_Episode_ID_9, Before_Episode_ID_10};
    }

    std::array<unsigned int, 10> GetLinkEpisodeIDs() const {
        return {Link_Episode_ID_01, Link_Episode_ID_02, Link_Episode_ID_03, Link_Episode_ID_04,
                Link_Episode_ID_05, Link_Episode_ID_06, Link_Episode_ID_07, Link_Episode_ID_08,
                Link_Episode_ID_09, Link_Episode_ID_10};
    }
};
#pragma pack(pop)

struct TB_PC_AKASHIC {
    unsigned int ID = 0;
    unsigned int Akashic_ID = 0;
};

#pragma pack(push, 1)
struct TB_AKASHIC_RECORDS {
    unsigned int ID = 0;
    char UI_String[511] = {};
    unsigned int Akashic_Group = 0;
    unsigned int Akashic_Effect_Group = 0;
    std::uint16_t Array_Index = 0;
    std::uint8_t Type = 0;
    std::uint8_t Cash_Type = 0;
    std::uint8_t Rank = 0;
    std::uint8_t Rare_Point = 0;
    std::uint8_t Hidden_Type = 0;
    char Code_Name[511] = {};
    std::uint16_t Action_Group = 0;
    std::uint8_t Combo_Count_Type = 0;
    unsigned int Use_Skill_ID = 0;
    std::uint8_t Invoke_Condition = 0;
    std::uint16_t Invoke_Rate = 0;
    unsigned int Skill_ID = 0;
    unsigned int CoolTime = 0;
    unsigned int CoolTime_Group = 0;
    std::uint8_t CoolTime_Save = 0;
    std::uint16_t CoolTime_Global = 0;
    std::uint8_t Akashic_Limit_Type = 0;
    unsigned int Akashic_Disassemble = 0;
    unsigned int EXP_Add = 0;
    unsigned int Akashic_Combination_Point = 0;
    unsigned int Gold_Add = 0;
    unsigned int Make_Need_Exp = 0;
    std::uint8_t Level_Max = 0;
    unsigned int Lease_Period = 0;
    unsigned int Lease_Cooltime = 0;
    unsigned int Gold_Need = 0;
    unsigned int Gold_Send = 0;
    unsigned int Basic_Exp = 0;
    std::uint8_t Make_Group_Enable_Type = 0;
    unsigned int Make_Group_Enable_Item = 0;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct TB_ITEM {
    unsigned int Item_ID = 0;
    unsigned int Item_Classify_Index = 0;
    std::uint8_t Item_Rank = 0;
    std::uint16_t Item_Lv = 0;
    unsigned int Sell_Price = 0;
    unsigned int Buy_Price = 0;
    unsigned int Recycle_Sell_Price = 0;
    unsigned int Recycle_Buy_Price = 0;
    std::uint16_t Item_Stack_Max = 0;
    std::uint8_t Item_Bind_Type = 0;
    unsigned int Item_Model_ID = 0;
    char Item_NameS[511] = {};
    std::uint16_t Item_Limit_Lv = 0;
    std::uint8_t Item_Limit_Class = 0;
    std::uint8_t Item_Limit_Sell_Type = 0;
    std::uint8_t Item_Sub_Type = 0;
    std::uint8_t Item_CostumeSet = 0;
    unsigned int Item_CostumeSet_ID = 0;
    char Item_Slot_Disable[511] = {};
    std::uint8_t Item_Endurance_Max = 0;
    std::uint8_t Item_Use_Value = 0;
    unsigned int Item_physical_Attack_Min = 0;
    unsigned int Item_physical_Attack = 0;
    unsigned int Item_Magic_Attack = 0;
    unsigned int Item_physical_Defense_Min = 0;
    unsigned int Item_physical_Defense = 0;
    unsigned int Item_Magic_Defense = 0;
    std::uint8_t Option_Class_1 = 0;
    std::uint8_t Option_Class_2 = 0;
    std::uint8_t Option_Class_3 = 0;
    std::uint8_t Option_Class_4 = 0;
    std::uint8_t Option_Class_5 = 0;
    unsigned int S_Option_Type_1 = 0;
    unsigned int S_Option_Type_2 = 0;
    unsigned int S_Option_Type_3 = 0;
    unsigned int S_Option_Type_4 = 0;
    unsigned int S_Option_Type_5 = 0;
    int S_Option_Value_1 = 0;
    int S_Option_Value_2 = 0;
    int S_Option_Value_3 = 0;
    int S_Option_Value_4 = 0;
    int S_Option_Value_5 = 0;
    unsigned int Item_Ex_Option_ID = 0;
    unsigned int Item_Socket_ID = 0;
    unsigned int Item_SetItem_ID = 0;
    std::uint16_t Item_Reinforce_ID = 0;
    unsigned int Item_Reinforce_Option_ID = 0;
    unsigned int Item_Title_ID = 0;
    unsigned int Item_Evolution_ID = 0;
    std::uint16_t Item_Disassemble_ID = 0;
    unsigned int Furniture_ID = 0;
    std::uint16_t Cooltime_Group = 0;
    unsigned int Cooltime_Value = 0;
    std::uint8_t Cooltime_Save = 0;
    std::uint16_t Item_Effect_Type = 0;
    unsigned int Item_Effect_ID = 0;
    std::uint8_t Item_Cash = 0;
    std::uint8_t Item_Use_Period_Type = 0;
    unsigned int Item_Use_Period_Value = 0;
    std::uint8_t Sealing_Cnt = 0;
    std::uint8_t Break_Cnt = 0;
    unsigned int Item_SimilarGroup_ID = 0;
    unsigned int RepackageItem_ID = 0;

    std::array<std::uint8_t, 5> GetOptionClasses() const {
        return {Option_Class_1, Option_Class_2, Option_Class_3, Option_Class_4, Option_Class_5};
    }

    std::array<unsigned int, 5> GetOptionTypes() const {
        return {S_Option_Type_1, S_Option_Type_2, S_Option_Type_3, S_Option_Type_4, S_Option_Type_5};
    }

    std::array<int, 5> GetOptionValues() const {
        return {S_Option_Value_1, S_Option_Value_2, S_Option_Value_3, S_Option_Value_4, S_Option_Value_5};
    }
};
#pragma pack(pop)

#pragma pack(push, 1)
struct TB_RANDOM_OPTION {
    unsigned int Option_ID = 0;
    std::uint8_t Option_Min = 0;
    std::uint8_t Option_Max = 0;
    std::uint8_t Option_Refer_Range = 0;
    std::uint16_t Option1_Type = 0;
    std::uint16_t Option2_Type = 0;
    std::uint16_t Option3_Type = 0;
    std::uint16_t Option4_Type = 0;
    std::uint16_t Option5_Type = 0;
    std::uint16_t Option6_Type = 0;
    std::uint16_t Option7_Type = 0;
    std::uint16_t Option8_Type = 0;
    std::uint16_t Option9_Type = 0;
    std::uint16_t Option10_Type = 0;
    std::uint16_t Option11_Type = 0;
    std::uint16_t Option12_Type = 0;
    std::uint16_t Option13_Type = 0;
    std::uint16_t Option14_Type = 0;
    std::uint16_t Option15_Type = 0;
    std::uint16_t Option16_Type = 0;
    std::uint16_t Option17_Type = 0;
    std::uint16_t Option18_Type = 0;
    std::uint16_t Option19_Type = 0;
    std::uint16_t Option20_Type = 0;
    std::uint16_t Option21_Type = 0;
    std::uint16_t Option22_Type = 0;
    std::uint16_t Option23_Type = 0;
    std::uint16_t Option24_Type = 0;
    std::uint16_t Option25_Type = 0;
    int Option1_1_Min = 0;
    int Option1_2_Min = 0;
    int Option1_3_Min = 0;
    int Option1_4_Min = 0;
    int Option1_5_Min = 0;
    int Option1_6_Min = 0;
    int Option1_7_Min = 0;
    int Option1_8_Min = 0;
    int Option1_9_Min = 0;
    int Option1_10_Min = 0;
    int Option1_11_Min = 0;
    int Option1_12_Min = 0;
    int Option1_13_Min = 0;
    int Option1_14_Min = 0;
    int Option1_15_Min = 0;
    int Option1_16_Min = 0;
    int Option1_17_Min = 0;
    int Option1_18_Min = 0;
    int Option1_19_Min = 0;
    int Option1_20_Min = 0;
    int Option1_21_Min = 0;
    int Option1_22_Min = 0;
    int Option1_23_Min = 0;
    int Option1_24_Min = 0;
    int Option1_25_Min = 0;
    int Option1_1_Max = 0;
    int Option1_2_Max = 0;
    int Option1_3_Max = 0;
    int Option1_4_Max = 0;
    int Option1_5_Max = 0;
    int Option1_6_Max = 0;
    int Option1_7_Max = 0;
    int Option1_8_Max = 0;
    int Option1_9_Max = 0;
    int Option1_10_Max = 0;
    int Option1_11_Max = 0;
    int Option1_12_Max = 0;
    int Option1_13_Max = 0;
    int Option1_14_Max = 0;
    int Option1_15_Max = 0;
    int Option1_16_Max = 0;
    int Option1_17_Max = 0;
    int Option1_18_Max = 0;
    int Option1_19_Max = 0;
    int Option1_20_Max = 0;
    int Option1_21_Max = 0;
    int Option1_22_Max = 0;
    int Option1_23_Max = 0;
    int Option1_24_Max = 0;
    int Option1_25_Max = 0;
    unsigned int Option_Change_ID = 0;

    std::array<std::uint16_t, 25> GetOptionTypes() const {
        return {Option1_Type,  Option2_Type,  Option3_Type,  Option4_Type,  Option5_Type,
                Option6_Type,  Option7_Type,  Option8_Type,  Option9_Type,  Option10_Type,
                Option11_Type, Option12_Type, Option13_Type, Option14_Type, Option15_Type,
                Option16_Type, Option17_Type, Option18_Type, Option19_Type, Option20_Type,
                Option21_Type, Option22_Type, Option23_Type, Option24_Type, Option25_Type};
    }

    std::array<int, 25> GetOptionMins() const {
        return {Option1_1_Min,  Option1_2_Min,  Option1_3_Min,  Option1_4_Min,  Option1_5_Min,
                Option1_6_Min,  Option1_7_Min,  Option1_8_Min,  Option1_9_Min,  Option1_10_Min,
                Option1_11_Min, Option1_12_Min, Option1_13_Min, Option1_14_Min, Option1_15_Min,
                Option1_16_Min, Option1_17_Min, Option1_18_Min, Option1_19_Min, Option1_20_Min,
                Option1_21_Min, Option1_22_Min, Option1_23_Min, Option1_24_Min, Option1_25_Min};
    }

    std::array<int, 25> GetOptionMaxs() const {
        return {Option1_1_Max,  Option1_2_Max,  Option1_3_Max,  Option1_4_Max,  Option1_5_Max,
                Option1_6_Max,  Option1_7_Max,  Option1_8_Max,  Option1_9_Max,  Option1_10_Max,
                Option1_11_Max, Option1_12_Max, Option1_13_Max, Option1_14_Max, Option1_15_Max,
                Option1_16_Max, Option1_17_Max, Option1_18_Max, Option1_19_Max, Option1_20_Max,
                Option1_21_Max, Option1_22_Max, Option1_23_Max, Option1_24_Max, Option1_25_Max};
    }
};
#pragma pack(pop)

#pragma pack(push, 1)
struct TB_BROACH_SET {
    std::uint16_t Broach_Set_Index = 0;
    char Broach_Option_Name_Ref[255] = {};
    std::uint16_t Broach_Set_Buff = 0;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct TB_COSTUME_SOCKET {
    unsigned int Socket_ID = 0;
    std::uint8_t Socket_Piece_EA = 0;
    std::uint8_t Fix_Socket1 = 0;
    std::uint8_t Fix_Socket2 = 0;
    std::uint8_t Fix_Socket3 = 0;
    std::uint8_t Fix_Socket4 = 0;
    std::uint8_t Fix_Socket5 = 0;
    std::uint8_t Extend_Socket1 = 0;
    std::uint8_t Extend_Socket2 = 0;
    std::uint8_t Extend_Socket3 = 0;
    std::uint8_t Extend_Socket4 = 0;
    std::uint8_t Extend_Socket5 = 0;
    unsigned int Add_Item = 0;
    std::uint16_t Add_Count = 0;

    std::array<std::uint8_t, 5> GetFixSocketCounts() const {
        return {Fix_Socket1, Fix_Socket2, Fix_Socket3, Fix_Socket4, Fix_Socket5};
    }

    std::array<std::uint8_t, 5> GetExtendSocketCounts() const {
        return {Extend_Socket1, Extend_Socket2, Extend_Socket3, Extend_Socket4, Extend_Socket5};
    }
};

struct TB_ITEM_TITLE {
    unsigned int Title_Group_ID = 0;
    unsigned int Group_ID01 = 0;
    unsigned int Group_ID02 = 0;
    unsigned int Group_ID03 = 0;
    unsigned int Group_ID04 = 0;
    unsigned int Group_ID05 = 0;
    unsigned int Group_ID06 = 0;
    unsigned int Group_ID07 = 0;
    unsigned int Group_ID08 = 0;
    unsigned int Group_ID09 = 0;
    unsigned int Group_ID10 = 0;
    std::uint16_t Group_rate01 = 0;
    std::uint16_t Group_rate02 = 0;
    std::uint16_t Group_rate03 = 0;
    std::uint16_t Group_rate04 = 0;
    std::uint16_t Group_rate05 = 0;
    std::uint16_t Group_rate06 = 0;
    std::uint16_t Group_rate07 = 0;
    std::uint16_t Group_rate08 = 0;
    std::uint16_t Group_rate09 = 0;
    std::uint16_t Group_rate10 = 0;

    std::array<unsigned int, 10> GetGroupIDs() const {
        return {Group_ID01, Group_ID02, Group_ID03, Group_ID04, Group_ID05,
                Group_ID06, Group_ID07, Group_ID08, Group_ID09, Group_ID10};
    }

    std::array<std::uint16_t, 10> GetGroupRates() const {
        return {Group_rate01, Group_rate02, Group_rate03, Group_rate04, Group_rate05,
                Group_rate06, Group_rate07, Group_rate08, Group_rate09, Group_rate10};
    }
};

struct TB_ITEM_TITLE_GROUP {
    unsigned int T_Item_Title_G_ID = 0;
    unsigned int T_Group01_ID_1 = 0;
    unsigned int T_Group01_ID_2 = 0;
    unsigned int T_Group01_ID_3 = 0;
    unsigned int T_Group01_ID_4 = 0;
    unsigned int T_Group01_ID_5 = 0;
    unsigned int T_Group01_ID_6 = 0;
    unsigned int T_Group01_ID_7 = 0;
    unsigned int T_Group01_ID_8 = 0;
    unsigned int T_Group01_ID_9 = 0;
    unsigned int T_Group01_ID_10 = 0;
    std::uint16_t T_Group01_Rate_1 = 0;
    std::uint16_t T_Group01_Rate_2 = 0;
    std::uint16_t T_Group01_Rate_3 = 0;
    std::uint16_t T_Group01_Rate_4 = 0;
    std::uint16_t T_Group01_Rate_5 = 0;
    std::uint16_t T_Group01_Rate_6 = 0;
    std::uint16_t T_Group01_Rate_7 = 0;
    std::uint16_t T_Group01_Rate_8 = 0;
    std::uint16_t T_Group01_Rate_9 = 0;
    std::uint16_t T_Group01_Rate_10 = 0;

    std::array<unsigned int, 10> GetTitleIDs() const {
        return {T_Group01_ID_1, T_Group01_ID_2, T_Group01_ID_3, T_Group01_ID_4, T_Group01_ID_5,
                T_Group01_ID_6, T_Group01_ID_7, T_Group01_ID_8, T_Group01_ID_9, T_Group01_ID_10};
    }

    std::array<std::uint16_t, 10> GetTitleRates() const {
        return {T_Group01_Rate_1, T_Group01_Rate_2, T_Group01_Rate_3, T_Group01_Rate_4, T_Group01_Rate_5,
                T_Group01_Rate_6, T_Group01_Rate_7, T_Group01_Rate_8, T_Group01_Rate_9, T_Group01_Rate_10};
    }
};

struct TB_ITEM_TITLE_VALUE {
    unsigned int Item_Title_ID = 0;
    char UI_String[511] = {};
    std::uint8_t Additional_Option0_Type = 0;
    std::uint8_t Additional_Option1_Type = 0;
    std::uint8_t Additional_Option2_Type = 0;
    unsigned int Additional_Option0_ID = 0;
    unsigned int Additional_Option1_ID = 0;
    unsigned int Additional_Option2_ID = 0;
    std::uint16_t Increase_Default_Value0 = 0;
    std::uint16_t Additional_Option_Value1 = 0;
    std::uint16_t Additional_Option_Value2 = 0;
};

struct TB_SOCKET {
    unsigned int Socket_ID = 0;
    std::uint8_t Socket_Type = 0;
    std::uint8_t Max_Socket = 0;
    std::uint8_t Fix_Socket = 0;
    std::uint8_t Random_Socket = 0;
    std::uint16_t R1_Chance = 0;
    std::uint16_t R2_Chance = 0;
    std::uint16_t R3_Chance = 0;
    unsigned int Euqip_Cost = 0;
    unsigned int Euqip_Item = 0;
    std::uint16_t Euqip_Count = 0;
    unsigned int Remove_Cost = 0;
    unsigned int Remove_Item = 0;
    std::uint16_t Remove_Count = 0;
    unsigned int Add_Cost = 0;
    unsigned int Add_Item = 0;
    std::uint16_t Add_Count = 0;
    unsigned int Extraction_Item = 0;
    std::uint16_t Extraction_Item_Count = 0;

    std::array<std::uint16_t, 3> GetRandomChances() const {
        return {R1_Chance, R2_Chance, R3_Chance};
    }
};

/**
 * @brief 强化资源表当前按 10 段数组承接 `*_R01 ~ *_R10` 的 PDB 字段组。
 */
struct TB_REINFORCE {
    unsigned int Reinforce_ID = 0;
    std::uint16_t Max_Reinforce = 0;
    std::uint16_t Basic_Try = 0;
    std::uint16_t Max_Try = 0;
    unsigned int uniEther[10] = {};
    unsigned int uniGold[10] = {};
    unsigned int uniBP[10] = {};
    unsigned int uniItem01[10] = {};
    std::uint16_t uniCount01[10] = {};
    unsigned int uniItem02[10] = {};
    std::uint16_t uniCount02[10] = {};
    std::uint16_t uniStay[10] = {};
    std::uint16_t uniUp[10] = {};
    std::uint16_t uniBreak[10] = {};
    float uniStat[10] = {};
};
#pragma pack(pop)

#pragma pack(push, 1)
/**
 * @brief `tb_Reinforce_Option.res` 的资源行。
 *
 * 当前已在 IDA 中确认字段布局与加载顺序，但在 `LoginServer.exe` 内尚未核实到业务消费函数。
 */
struct TB_REINFORCE_OPTION {
    unsigned int Reinforce_Option_ID = 0;
    std::uint8_t Reinforce_Count = 0;
    std::uint8_t Phase_R01 = 0;
    std::uint8_t Phase_R02 = 0;
    std::uint8_t Phase_R03 = 0;
    std::uint8_t Phase_R04 = 0;
    std::uint8_t Phase_R05 = 0;
    std::uint8_t OptionClass_R01 = 0;
    std::uint8_t OptionClass_R02 = 0;
    std::uint8_t OptionClass_R03 = 0;
    std::uint8_t OptionClass_R04 = 0;
    std::uint8_t OptionClass_R05 = 0;
    std::uint16_t OptionID_R01 = 0;
    std::uint16_t OptionID_R02 = 0;
    std::uint16_t OptionID_R03 = 0;
    std::uint16_t OptionID_R04 = 0;
    std::uint16_t OptionID_R05 = 0;
    unsigned int OptionValue_R01 = 0;
    unsigned int OptionValue_R02 = 0;
    unsigned int OptionValue_R03 = 0;
    unsigned int OptionValue_R04 = 0;
    unsigned int OptionValue_R05 = 0;

    std::array<std::uint8_t, 5> GetPhases() const {
        return {Phase_R01, Phase_R02, Phase_R03, Phase_R04, Phase_R05};
    }

    std::array<std::uint8_t, 5> GetOptionClasses() const {
        return {OptionClass_R01, OptionClass_R02, OptionClass_R03, OptionClass_R04, OptionClass_R05};
    }

    std::array<std::uint16_t, 5> GetOptionIDs() const {
        return {OptionID_R01, OptionID_R02, OptionID_R03, OptionID_R04, OptionID_R05};
    }

    std::array<unsigned int, 5> GetOptionValues() const {
        return {OptionValue_R01, OptionValue_R02, OptionValue_R03, OptionValue_R04, OptionValue_R05};
    }
};

struct TB_ITEM_TITLE_CHANGE {
    std::uint16_t Tag_Change_ID = 0;
    std::uint16_t Basic_Success_Rate = 0;
    std::uint16_t Need_Point = 0;
    unsigned int Need_Zeny = 0;
    unsigned int Need_Ether = 0;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct TB_ITEM_CLASSIFY {
    unsigned int Item_Classify_Index = 0;
    std::uint8_t GroupID = 0;
    std::uint8_t SubGroupID = 0;
    std::uint8_t CategoryID = 0;
    std::uint8_t SubCategoryID = 0;
    std::uint8_t Item_Gain_Type = 0;
    std::uint8_t Item_Inven_Type = 0;
    std::uint8_t Item_Slot_Type = 0;
    std::uint8_t Item_Repair_Type = 0;
    std::uint8_t Item_Use_State = 0;
    std::uint8_t Item_Use_Type = 0;
    std::uint8_t Consume_Type = 0;
    std::uint16_t Exchange_Apply_Fee = 0;
    std::int16_t Classify_Align_Type = 0;
};
#pragma pack(pop)

struct TB_NAMEFILTER {
    unsigned int Filter_Index = 0;
    std::uint8_t Filter_Type = 0;
    char Filter_Word[511] = {};
};

struct TB_COMMON {
    unsigned int Define_ID = 0;
    char Key[511] = {};
    float Value = 0.0f;
};

#pragma pack(push, 1)
struct TB_WEEK_GROUP {
    std::uint8_t mission_group = 0;
    std::uint8_t Check_Mission_Type = 0;
    std::uint8_t Event_Type = 0;
    std::uint16_t Event_Start_Year = 0;
    std::uint8_t Event_Start_Month = 0;
    std::uint8_t Event_Start_Day = 0;
    std::uint16_t Event_End_Year = 0;
    std::uint8_t Event_End_Month = 0;
    std::uint8_t Event_End_Day = 0;
    std::uint8_t grop_count = 0;
    unsigned int reward_item_id = 0;
    std::uint16_t reward_count = 0;
};
#pragma pack(pop)

static_assert(sizeof(ST_PHOTO_ID) == 0x2, "ST_PHOTO_ID size must match PDB");
static_assert(sizeof(TB_APPEARANCE) == 0x7, "TB_APPEARANCE size must match PDB");
static_assert(sizeof(TB_CREATE_CLOTH) == 0x219, "TB_CREATE_CLOTH size must match PDB");
static_assert(sizeof(TB_PROVIDE_ITEM) == 0x142, "TB_PROVIDE_ITEM size must match PDB");
static_assert(sizeof(TB_PHOTO_ITEM) == 0xC, "TB_PHOTO_ITEM size must match PDB");
static_assert(sizeof(TB_CHARACTER_INFO) == 0x2C4, "TB_CHARACTER_INFO size must match PDB");
static_assert(sizeof(TB_ITEM) == 0x4AE, "TB_ITEM size must match PDB");
static_assert(sizeof(TB_RANDOM_OPTION) == 0x105, "TB_RANDOM_OPTION size must match PDB");
static_assert(sizeof(TB_BROACH_SET) == 0x103, "TB_BROACH_SET size must match PDB");
static_assert(sizeof(TB_COSTUME_SOCKET) == 0x15, "TB_COSTUME_SOCKET size must match PDB");
static_assert(sizeof(TB_ITEM_TITLE) == 0x40, "TB_ITEM_TITLE size must match PDB");
static_assert(sizeof(TB_ITEM_TITLE_GROUP) == 0x40, "TB_ITEM_TITLE_GROUP size must match PDB");
static_assert(sizeof(TB_ITEM_TITLE_VALUE) == 0x218, "TB_ITEM_TITLE_VALUE size must match PDB");
static_assert(sizeof(TB_SOCKET) == 0x32, "TB_SOCKET size must match PDB");
static_assert(sizeof(TB_REINFORCE) == 0x15E, "TB_REINFORCE size must match PDB");
static_assert(sizeof(TB_REINFORCE_OPTION) == 0x2D, "TB_REINFORCE_OPTION size must match PDB");
static_assert(sizeof(TB_ITEM_TITLE_CHANGE) == 0xE, "TB_ITEM_TITLE_CHANGE size must match PDB");
static_assert(sizeof(TB_ITEM_CLASSIFY) == 0x13, "TB_ITEM_CLASSIFY size must match PDB");
static_assert(sizeof(TB_NAMEFILTER) == 0x204, "TB_NAMEFILTER size must match PDB");
static_assert(sizeof(TB_WEEK_GROUP) == 0x12, "TB_WEEK_GROUP size must match PDB");
static_assert(sizeof(TB_QUEST_EPISODE) == 0x581, "TB_QUEST_EPISODE size must match PDB");
static_assert(sizeof(TB_PC_AKASHIC) == 0x8, "TB_PC_AKASHIC size must match PDB");
static_assert(sizeof(TB_AKASHIC_RECORDS) == 0x459, "TB_AKASHIC_RECORDS size must match PDB");

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

    TB_SYSTEMMAIL_ADD* GetTB_SYSTEMMAIL_ADD(std::uint16_t index) {
        auto it = autoMailRows_.find(index);
        if (it == autoMailRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

    TB_CHARACTER_INFO* GetTB_CHARACTER_INFO(std::uint16_t index) {
        auto it = characterInfoRows_.find(index);
        if (it == characterInfoRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

    TB_APPEARANCE* GetTB_APPEARANCE(unsigned int index) {
        auto it = appearanceRows_.find(index);
        if (it == appearanceRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

    TB_CREATE_CLOTH* GetTB_CREATE_CLOTH(std::uint16_t index) {
        auto it = createClothRows_.find(index);
        if (it == createClothRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

    TB_PROVIDE_ITEM* GetTB_PROVIDE_ITEM(std::uint16_t index) {
        auto it = provideItemRows_.find(index);
        if (it == provideItemRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

    TB_PHOTO_ITEM* GetTB_PHOTO_ITEM(unsigned int index) {
        auto it = photoItemRows_.find(index);
        if (it == photoItemRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

    TB_ITEM* GetTB_ITEM(unsigned int index) {
        auto it = itemRows_.find(index);
        if (it == itemRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

    TB_RANDOM_OPTION* GetTB_RANDOM_OPTION(unsigned int index) {
        auto it = randomOptionRows_.find(index);
        if (it == randomOptionRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

    TB_BROACH_SET* GetTB_BROACH_SET(std::uint16_t index) {
        auto it = broachSetRows_.find(index);
        if (it == broachSetRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

    TB_COSTUME_SOCKET* GetTB_COSTUME_SOCKET(unsigned int index) {
        auto it = costumeSocketRows_.find(index);
        if (it == costumeSocketRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

    TB_ITEM_TITLE* GetTB_ITEM_TITLE(unsigned int index) {
        auto it = itemTitleRows_.find(index);
        if (it == itemTitleRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

    TB_ITEM_TITLE_GROUP* GetTB_ITEM_TITLE_GROUP(unsigned int index) {
        auto it = itemTitleGroupRows_.find(index);
        if (it == itemTitleGroupRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

    TB_ITEM_TITLE_VALUE* GetTB_ITEM_TITLE_VALUE(unsigned int index) {
        auto it = itemTitleValueRows_.find(index);
        if (it == itemTitleValueRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

    TB_REINFORCE* GetTB_REINFORCE(unsigned int index) {
        auto it = reinforceRows_.find(index);
        if (it == reinforceRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

    TB_SOCKET* GetTB_SOCKET(unsigned int index) {
        auto it = socketRows_.find(index);
        if (it == socketRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

    TB_ITEM_CLASSIFY* GetTB_ITEM_CLASSIFY(unsigned int index) {
        auto it = itemClassifyRows_.find(index);
        if (it == itemClassifyRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

    TB_PHOTO_ITEM* FindDefaultPhotoItemID(std::uint8_t byClass, std::uint8_t byType) {
        const auto it = photoItemIdRows_.find(ST_PHOTO_ID{byClass, byType});
        if (it == photoItemIdRows_.end()) {
            return nullptr;
        }
        return it->second;
    }

    TB_QUEST_EPISODE* GetTB_QUEST_EPISODE(unsigned int index) {
        const auto it = m_mapTB_QUEST_EPISODE.find(index);
        return it == m_mapTB_QUEST_EPISODE.end() ? nullptr : &it->second;
    }

    TB_PC_AKASHIC* GetTB_PC_AKASHIC(unsigned int index) {
        const auto it = m_mapTB_PC_AKASHIC.find(index);
        return it == m_mapTB_PC_AKASHIC.end() ? nullptr : &it->second;
    }

    TB_AKASHIC_RECORDS* GetTB_AKASHIC_RECORDS(unsigned int index) {
        const auto it = m_mapTB_AKASHIC_RECORDS.find(index);
        return it == m_mapTB_AKASHIC_RECORDS.end() ? nullptr : &it->second;
    }

    TB_AKASHIC_RECORDS* GetPCAkashic(unsigned int index) {
        const auto it = m_mapPCAkashic.find(index);
        return it == m_mapPCAkashic.end() ? nullptr : it->second;
    }

    TB_COMMON* GetTB_COMMON(unsigned int index) {
        auto it = commonRows_.find(index);
        if (it == commonRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

    const std::unordered_map<unsigned int, TB_NAMEFILTER>& GetTB_NAMEFILTERRows() const {
        return nameFilterRows_;
    }

    void SetTB_SYSTEMMAIL_ADD(std::uint16_t index, const TB_SYSTEMMAIL_ADD& row) {
        autoMailRows_[index] = row;
    }

    void SetTB_CHARACTER_INFO(std::uint16_t index, const TB_CHARACTER_INFO& row) {
        characterInfoRows_[index] = row;
    }

    void SetTB_APPEARANCE(unsigned int index, const TB_APPEARANCE& row) {
        appearanceRows_[index] = row;
    }

    void SetTB_CREATE_CLOTH(std::uint16_t index, const TB_CREATE_CLOTH& row) {
        createClothRows_[index] = row;
    }

    void SetTB_PROVIDE_ITEM(std::uint16_t index, const TB_PROVIDE_ITEM& row) {
        provideItemRows_[index] = row;
    }

    void SetTB_PHOTO_ITEM(unsigned int index, const TB_PHOTO_ITEM& row) {
        photoItemRows_[index] = row;
    }

    void SetTB_QUEST_EPISODE(unsigned int index, const TB_QUEST_EPISODE& row) {
        m_mapTB_QUEST_EPISODE[index] = row;
    }

    void SetTB_PC_AKASHIC(unsigned int index, const TB_PC_AKASHIC& row) {
        m_mapTB_PC_AKASHIC[index] = row;
    }

    void SetTB_AKASHIC_RECORDS(unsigned int index, const TB_AKASHIC_RECORDS& row) {
        m_mapTB_AKASHIC_RECORDS[index] = row;
    }

    void SetTB_ITEM(unsigned int index, const TB_ITEM& row) {
        itemRows_[index] = row;
    }

    void SetTB_RANDOM_OPTION(unsigned int index, const TB_RANDOM_OPTION& row) {
        randomOptionRows_[index] = row;
    }

    void SetTB_BROACH_SET(std::uint16_t index, const TB_BROACH_SET& row) {
        broachSetRows_[index] = row;
    }

    void SetTB_COSTUME_SOCKET(unsigned int index, const TB_COSTUME_SOCKET& row) {
        costumeSocketRows_[index] = row;
    }

    void SetTB_ITEM_TITLE(unsigned int index, const TB_ITEM_TITLE& row) {
        itemTitleRows_[index] = row;
    }

    void SetTB_ITEM_TITLE_GROUP(unsigned int index, const TB_ITEM_TITLE_GROUP& row) {
        itemTitleGroupRows_[index] = row;
    }

    void SetTB_ITEM_TITLE_VALUE(unsigned int index, const TB_ITEM_TITLE_VALUE& row) {
        itemTitleValueRows_[index] = row;
    }

    void SetTB_REINFORCE(unsigned int index, const TB_REINFORCE& row) {
        reinforceRows_[index] = row;
    }

    void SetTB_SOCKET(unsigned int index, const TB_SOCKET& row) {
        socketRows_[index] = row;
    }

    void SetTB_ITEM_CLASSIFY(unsigned int index, const TB_ITEM_CLASSIFY& row) {
        itemClassifyRows_[index] = row;
    }

    void SetTB_COMMON(unsigned int index, const TB_COMMON& row) {
        commonRows_[index] = row;
    }

    void SetTB_NAMEFILTER(unsigned int index, const TB_NAMEFILTER& row) {
        nameFilterRows_[index] = row;
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

    const std::unordered_map<unsigned int, TB_COMMON>& GetTB_COMMONRows() const {
        return commonRows_;
    }

    const std::map<std::uint8_t, TB_WEEK_GROUP>& GetTB_WEEK_GROUPRows() const {
        return m_mapTB_WEEK_GROUP;
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
    bool LoadAppearanceFile(const std::filesystem::path& basePath) {
        std::vector<std::uint8_t> buffer;
        if (!GreenDamTanDBLoadDetail::LoadBinaryFile(basePath / "tb_Appearance.res", buffer) ||
            buffer.size() < sizeof(unsigned int)) {
            return false;
        }

        unsigned int rowCount = 0;
        std::memcpy(&rowCount, buffer.data(), sizeof(rowCount));
        const std::size_t rowOffset = sizeof(rowCount);
        const std::size_t rowBytes = static_cast<std::size_t>(rowCount) * sizeof(TB_APPEARANCE);
        if (buffer.size() < rowOffset + rowBytes) {
            return false;
        }

        for (unsigned int index = 0; index < rowCount; ++index) {
            TB_APPEARANCE row{};
            std::memcpy(&row,
                        buffer.data() + rowOffset + static_cast<std::size_t>(index) * sizeof(TB_APPEARANCE),
                        sizeof(TB_APPEARANCE));
            SetTB_APPEARANCE(row.Appearance_ID, row);
        }
        return true;
    }

    bool LoadCreateClothFile(const std::filesystem::path& basePath) {
        std::vector<std::uint8_t> buffer;
        if (!GreenDamTanDBLoadDetail::LoadBinaryFile(basePath / "tb_Create_Cloth.res", buffer) ||
            buffer.size() < sizeof(unsigned int)) {
            return false;
        }

        GreenDamTanDBLoadDetail::BinaryCursor cursor(buffer);
        unsigned int rowCount = 0;
        if (!cursor.ReadU32(rowCount)) {
            return false;
        }

        CTableLoader_S loader{};
        loader.m_nRowCount = static_cast<int>(rowCount);
        loader.m_strTableName = "tb_Create_Cloth";
        for (unsigned int index = 0; index < rowCount; ++index) {
            TB_CREATE_CLOTH row{};
            std::string utf8Text;
            if (!cursor.ReadCheckSumU16(&row.ID) ||
                !cursor.ReadCheckSumU32(&row.Item_ID_Head) ||
                !cursor.ReadCheckSumU32(&row.Item_ID_Body) ||
                !cursor.ReadCheckSumU32(&row.Item_ID_Hands) ||
                !cursor.ReadCheckSumU32(&row.Item_ID_Stocking) ||
                !cursor.ReadCheckSumU32(&row.Item_ID_Foot) ||
                !cursor.ReadCheckSumU32(&row.Item_ID_Pants) ||
                !cursor.ReadCheckSumUtf16String(utf8Text)) {
                return false;
            }

            GreenDamTanDBLoadDetail::CopyString(row.Icon_FileName, utf8Text);
            SetTB_CREATE_CLOTH(row.ID, row);
        }

        loader.m_biCheckSum = cursor.GetCheckSum();
        std::string md5Value;
        if (!cursor.ReadAsciiString(md5Value, 32) || !cursor.IsAtEnd()) {
            return false;
        }
        std::memset(loader.m_szMD5, 0, sizeof(loader.m_szMD5));
        const std::size_t md5CopyLength =
            md5Value.size() < sizeof(loader.m_szMD5) ? md5Value.size() : sizeof(loader.m_szMD5);
        std::memcpy(loader.m_szMD5, md5Value.data(), md5CopyLength);
        return CheckSum(loader);
    }

    bool LoadCharacterInfoFile(const std::filesystem::path& basePath) {
        std::vector<std::uint8_t> buffer;
        if (!GreenDamTanDBLoadDetail::LoadBinaryFile(basePath / "tb_Character_Info.res", buffer) ||
            buffer.size() < sizeof(unsigned int)) {
            return false;
        }

        GreenDamTanDBLoadDetail::BinaryCursor cursor(buffer);
        unsigned int rowCount = 0;
        if (!cursor.ReadU32(rowCount)) {
            return false;
        }

        CTableLoader_S loader{};
        loader.m_nRowCount = static_cast<int>(rowCount);
        loader.m_strTableName = "tb_Character_Info";
        for (unsigned int index = 0; index < rowCount; ++index) {
            TB_CHARACTER_INFO row{};
            std::string utf8Text;
            if (!cursor.ReadCheckSumU16(&row.ID) ||
                !cursor.ReadCheckSumU8(&row.Character_ID) ||
                !cursor.ReadCheckSumU16(&row.Character_Group_ID) ||
                !cursor.ReadCheckSumUtf16String(utf8Text)) {
                return false;
            }
            GreenDamTanDBLoadDetail::CopyString(row.PC_Code_Name, utf8Text);

            if (!cursor.ReadCheckSumU32(&row.Item_ID_Weapon_01)) {
                return false;
            }

            std::array<unsigned int, 4> defaultClothIds = {};
            for (int clothIndex = 0; clothIndex < 4; ++clothIndex) {
                if (!cursor.ReadCheckSumU32(&defaultClothIds[clothIndex])) {
                    return false;
                }
            }
            row.Default_Cloth_ID_01 = defaultClothIds[0];
            row.Default_Cloth_ID_02 = defaultClothIds[1];
            row.Default_Cloth_ID_03 = defaultClothIds[2];
            row.Default_Cloth_ID_04 = defaultClothIds[3];

            if (!cursor.ReadCheckSumU16(&row.Default_Provide_Item) ||
                !cursor.ReadCheckSumU16(&row.District_ID) ||
                !cursor.ReadCheckSumU16(&row.District_Position_X) ||
                !cursor.ReadCheckSumU16(&row.District_Position_Y) ||
                !cursor.ReadCheckSumU16(&row.District_Position_Z) ||
                !cursor.ReadCheckSumU16(&row.Maze_ID) ||
                !cursor.ReadCheckSumU32(&row.Maze_Spawnbox_ID) ||
                !cursor.ReadCheckSumU32(&row.Character_Start_Quest_ID) ||
                !cursor.ReadCheckSumU32(&row.Character_Skip_Quest_ID) ||
                !cursor.ReadCheckSumU8(&row.Character_Status_Type) ||
                !cursor.ReadCheckSumU32(&row.Character_Status) ||
                !cursor.ReadCheckSumU16(&row.Char_Height_Value) ||
                !cursor.ReadCheckSumU16(&row.Nomal_Walk_Speed) ||
                !cursor.ReadCheckSumU16(&row.Nomal_Run_Speed) ||
                !cursor.ReadCheckSumU16(&row.Nomal_JumpMove_Speed) ||
                !cursor.ReadCheckSumU16(&row.Battle_Run_Speed) ||
                !cursor.ReadCheckSumU16(&row.Battle_JumpMove_Speed) ||
                !cursor.ReadCheckSumU16(&row.Battle_Dash_Speed) ||
                !cursor.ReadCheckSumF32(&row.Battle_BackMove_Ratio) ||
                !cursor.ReadCheckSumU8(&row.PC_Faction) ||
                !cursor.ReadCheckSumU16(&row.Char_CollisionRadius) ||
                !cursor.ReadCheckSumU16(&row.Char_CollisionHeight) ||
                !cursor.ReadCheckSumU16(&row.Char_RealHeight)) {
                return false;
            }

            std::array<unsigned int, 20> defaultSkillIds = {};
            for (int skillIndex = 0; skillIndex < 20; ++skillIndex) {
                if (!cursor.ReadCheckSumU32(&defaultSkillIds[skillIndex])) {
                    return false;
                }
            }
            std::memcpy(&row.Default_Skill_ID_01, defaultSkillIds.data(),
                        sizeof(unsigned int) * defaultSkillIds.size());

            std::array<std::uint8_t, 20> quickSlotIds = {};
            for (int quickIndex = 0; quickIndex < 20; ++quickIndex) {
                if (!cursor.ReadCheckSumU8(&quickSlotIds[quickIndex])) {
                    return false;
                }
            }
            std::memcpy(&row.Set_Quick_Slot_ID_01, quickSlotIds.data(),
                        sizeof(std::uint8_t) * quickSlotIds.size());

            std::array<std::uint16_t, 6> gestureSlotIds = {};
            for (int gestureIndex = 0; gestureIndex < 6; ++gestureIndex) {
                if (!cursor.ReadCheckSumU16(&gestureSlotIds[gestureIndex])) {
                    return false;
                }
            }
            std::memcpy(&row.Set_Gesture_Slot_ID_01, gestureSlotIds.data(),
                        sizeof(std::uint16_t) * gestureSlotIds.size());

            if (!cursor.ReadCheckSumF32(&row.Akashic_Active_Time) ||
                !cursor.ReadCheckSumU8(&row.Character_On_Type) ||
                !cursor.ReadCheckSumU8(&row.Character_On)) {
                return false;
            }

            SetTB_CHARACTER_INFO(row.ID, row);
        }

        loader.m_biCheckSum = cursor.GetCheckSum();
        std::string md5Value;
        if (!cursor.ReadAsciiString(md5Value, 32) || !cursor.IsAtEnd()) {
            return false;
        }
        std::memset(loader.m_szMD5, 0, sizeof(loader.m_szMD5));
        const std::size_t md5CopyLength =
            md5Value.size() < sizeof(loader.m_szMD5) ? md5Value.size() : sizeof(loader.m_szMD5);
        std::memcpy(loader.m_szMD5, md5Value.data(), md5CopyLength);
        return CheckSum(loader);
    }

    bool LoadPhotoItemFile(const std::filesystem::path& basePath) {
        std::vector<std::uint8_t> buffer;
        if (!GreenDamTanDBLoadDetail::LoadBinaryFile(basePath / "tb_Photo_Item.res", buffer) ||
            buffer.size() < sizeof(unsigned int)) {
            return false;
        }

        GreenDamTanDBLoadDetail::BinaryCursor cursor(buffer);
        unsigned int rowCount = 0;
        if (!cursor.ReadU32(rowCount)) {
            return false;
        }

        CTableLoader_S loader{};
        loader.m_nRowCount = static_cast<int>(rowCount);
        loader.m_strTableName = "tb_Photo_Item";
        for (unsigned int index = 0; index < rowCount; ++index) {
            TB_PHOTO_ITEM row{};
            if (!cursor.ReadCheckSumU32(&row.ID) || !cursor.ReadCheckSumU32(&row.Photo_Name) ||
                !cursor.ReadCheckSumU16(&row.Photo_Group) || !cursor.ReadCheckSumU8(&row.Char_Class) ||
                !cursor.ReadCheckSumU8(&row.Char_Promotion_Info)) {
                return false;
            }
            SetTB_PHOTO_ITEM(row.ID, row);
        }

        loader.m_biCheckSum = cursor.GetCheckSum();
        std::string md5Value;
        if (!cursor.ReadAsciiString(md5Value, 32) || !cursor.IsAtEnd()) {
            return false;
        }
        std::memset(loader.m_szMD5, 0, sizeof(loader.m_szMD5));
        const std::size_t md5CopyLength =
            md5Value.size() < sizeof(loader.m_szMD5) ? md5Value.size() : sizeof(loader.m_szMD5);
        std::memcpy(loader.m_szMD5, md5Value.data(), md5CopyLength);
        return CheckSum(loader);
    }

    bool LoadProvideItemFile(const std::filesystem::path& basePath) {
        std::vector<std::uint8_t> buffer;
        if (!GreenDamTanDBLoadDetail::LoadBinaryFile(basePath / "tb_Provide_Item.res", buffer) ||
            buffer.size() < sizeof(unsigned int)) {
            return false;
        }

        unsigned int rowCount = 0;
        std::memcpy(&rowCount, buffer.data(), sizeof(rowCount));
        const std::size_t rowOffset = sizeof(rowCount);
        const std::size_t rowBytes = static_cast<std::size_t>(rowCount) * sizeof(TB_PROVIDE_ITEM);
        if (buffer.size() < rowOffset + rowBytes) {
            return false;
        }

        for (unsigned int index = 0; index < rowCount; ++index) {
            TB_PROVIDE_ITEM row{};
            std::memcpy(&row,
                        buffer.data() + rowOffset + static_cast<std::size_t>(index) * sizeof(TB_PROVIDE_ITEM),
                        sizeof(TB_PROVIDE_ITEM));
            SetTB_PROVIDE_ITEM(row.ID, row);
        }
        return true;
    }

    bool LoadCommonFile(const std::filesystem::path& basePath) {
        std::vector<std::uint8_t> buffer;
        if (!GreenDamTanDBLoadDetail::LoadBinaryFile(basePath / "tb_Common.res", buffer) ||
            buffer.size() < sizeof(unsigned int)) {
            return false;
        }

        GreenDamTanDBLoadDetail::BinaryCursor cursor(buffer);
        unsigned int rowCount = 0;
        if (!cursor.ReadU32(rowCount)) {
            return false;
        }

        for (unsigned int index = 0; index < rowCount; ++index) {
            TB_COMMON row{};
            std::string utf8Text;
            if (!cursor.ReadU32(&row.Define_ID) ||
                !cursor.ReadUtf16String(utf8Text) ||
                !cursor.ReadF32(&row.Value)) {
                return false;
            }
            GreenDamTanDBLoadDetail::CopyString(row.Key, utf8Text);
            SetTB_COMMON(row.Define_ID, row);
        }
        return true;
    }

    bool LoadWeekGroupFile(const std::filesystem::path& basePath) {
        std::vector<std::uint8_t> buffer;
        if (!GreenDamTanDBLoadDetail::LoadBinaryFile(basePath / "tb_Week_Group.res", buffer) ||
            buffer.size() < sizeof(unsigned int)) {
            return false;
        }

        unsigned int rowCount = 0;
        std::memcpy(&rowCount, buffer.data(), sizeof(rowCount));
        const std::size_t rowOffset = sizeof(rowCount);
        const std::size_t rowBytes = static_cast<std::size_t>(rowCount) * sizeof(TB_WEEK_GROUP);
        if (buffer.size() < rowOffset + rowBytes) {
            return false;
        }

        for (unsigned int index = 0; index < rowCount; ++index) {
            TB_WEEK_GROUP row{};
            std::memcpy(&row,
                        buffer.data() + rowOffset + static_cast<std::size_t>(index) * sizeof(TB_WEEK_GROUP),
                        sizeof(TB_WEEK_GROUP));
            m_mapTB_WEEK_GROUP[row.mission_group] = row;
        }
        return true;
    }

    bool LoadPCAkashicFile(const std::filesystem::path& basePath) {
        std::vector<std::uint8_t> buffer;
        if (!GreenDamTanDBLoadDetail::LoadBinaryFile(basePath / "tb_PC_Akashic.res", buffer) ||
            buffer.size() < sizeof(unsigned int)) {
            return false;
        }

        GreenDamTanDBLoadDetail::BinaryCursor cursor(buffer);
        unsigned int rowCount = 0;
        if (!cursor.ReadU32(rowCount)) {
            return false;
        }

        CTableLoader_S loader{};
        loader.m_nRowCount = static_cast<int>(rowCount);
        loader.m_strTableName = "tb_PC_Akashic";
        for (unsigned int index = 0; index < rowCount; ++index) {
            TB_PC_AKASHIC row{};
            if (!cursor.ReadCheckSumU32(&row.ID) || !cursor.ReadCheckSumU32(&row.Akashic_ID)) {
                return false;
            }
            SetTB_PC_AKASHIC(row.ID, row);
        }

        loader.m_biCheckSum = cursor.GetCheckSum();
        std::string md5Value;
        if (!cursor.ReadAsciiString(md5Value, 32) || !cursor.IsAtEnd()) {
            return false;
        }
        std::memset(loader.m_szMD5, 0, sizeof(loader.m_szMD5));
        const std::size_t md5CopyLength =
            md5Value.size() < sizeof(loader.m_szMD5) ? md5Value.size() : sizeof(loader.m_szMD5);
        std::memcpy(loader.m_szMD5, md5Value.data(), md5CopyLength);
        return CheckSum(loader);
    }

    bool LoadAkashicRecordsFile(const std::filesystem::path& basePath) {
        std::vector<std::uint8_t> buffer;
        if (!GreenDamTanDBLoadDetail::LoadBinaryFile(basePath / "tb_Akashic_Records.res", buffer) ||
            buffer.size() < sizeof(unsigned int)) {
            return false;
        }

        GreenDamTanDBLoadDetail::BinaryCursor cursor(buffer);
        unsigned int rowCount = 0;
        if (!cursor.ReadU32(rowCount)) {
            return false;
        }

        CTableLoader_S loader{};
        loader.m_nRowCount = static_cast<int>(rowCount);
        loader.m_strTableName = "tb_Akashic_Records";
        for (unsigned int index = 0; index < rowCount; ++index) {
            TB_AKASHIC_RECORDS row{};
            std::string utf8Text;
            if (!cursor.ReadCheckSumU32(&row.ID) || !cursor.ReadCheckSumUtf16String(utf8Text)) {
                return false;
            }
            GreenDamTanDBLoadDetail::CopyString(row.UI_String, utf8Text);

            if (!cursor.ReadCheckSumU32(&row.Akashic_Group) ||
                !cursor.ReadCheckSumU32(&row.Akashic_Effect_Group) ||
                !cursor.ReadCheckSumU16(&row.Array_Index) ||
                !cursor.ReadCheckSumU8(&row.Type) ||
                !cursor.ReadCheckSumU8(&row.Cash_Type) ||
                !cursor.ReadCheckSumU8(&row.Rank) ||
                !cursor.ReadCheckSumU8(&row.Rare_Point) ||
                !cursor.ReadCheckSumU8(&row.Hidden_Type) ||
                !cursor.ReadCheckSumUtf16String(utf8Text)) {
                return false;
            }
            GreenDamTanDBLoadDetail::CopyString(row.Code_Name, utf8Text);

            if (!cursor.ReadCheckSumU16(&row.Action_Group) ||
                !cursor.ReadCheckSumU8(&row.Combo_Count_Type) ||
                !cursor.ReadCheckSumU32(&row.Use_Skill_ID) ||
                !cursor.ReadCheckSumU8(&row.Invoke_Condition) ||
                !cursor.ReadCheckSumU16(&row.Invoke_Rate) ||
                !cursor.ReadCheckSumU32(&row.Skill_ID) ||
                !cursor.ReadCheckSumU32(&row.CoolTime) ||
                !cursor.ReadCheckSumU32(&row.CoolTime_Group) ||
                !cursor.ReadCheckSumU8(&row.CoolTime_Save) ||
                !cursor.ReadCheckSumU16(&row.CoolTime_Global) ||
                !cursor.ReadCheckSumU8(&row.Akashic_Limit_Type) ||
                !cursor.ReadCheckSumU32(&row.Akashic_Disassemble) ||
                !cursor.ReadCheckSumU32(&row.EXP_Add) ||
                !cursor.ReadCheckSumU32(&row.Akashic_Combination_Point) ||
                !cursor.ReadCheckSumU32(&row.Gold_Add) ||
                !cursor.ReadCheckSumU32(&row.Make_Need_Exp) ||
                !cursor.ReadCheckSumU8(&row.Level_Max) ||
                !cursor.ReadCheckSumU32(&row.Lease_Period) ||
                !cursor.ReadCheckSumU32(&row.Lease_Cooltime) ||
                !cursor.ReadCheckSumU32(&row.Gold_Need) ||
                !cursor.ReadCheckSumU32(&row.Gold_Send) ||
                !cursor.ReadCheckSumU32(&row.Basic_Exp) ||
                !cursor.ReadCheckSumU8(&row.Make_Group_Enable_Type) ||
                !cursor.ReadCheckSumU32(&row.Make_Group_Enable_Item)) {
                return false;
            }

            SetTB_AKASHIC_RECORDS(row.ID, row);
        }

        loader.m_biCheckSum = cursor.GetCheckSum();
        std::string md5Value;
        if (!cursor.ReadAsciiString(md5Value, 32) || !cursor.IsAtEnd()) {
            return false;
        }
        std::memset(loader.m_szMD5, 0, sizeof(loader.m_szMD5));
        const std::size_t md5CopyLength =
            md5Value.size() < sizeof(loader.m_szMD5) ? md5Value.size() : sizeof(loader.m_szMD5);
        std::memcpy(loader.m_szMD5, md5Value.data(), md5CopyLength);
        return CheckSum(loader);
    }

    bool LoadQuestEpisodeFile(const std::filesystem::path& basePath) {
        std::vector<std::uint8_t> buffer;
        if (!GreenDamTanDBLoadDetail::LoadBinaryFile(basePath / "tb_Quest_Episode.res", buffer) ||
            buffer.size() < sizeof(unsigned int)) {
            return false;
        }

        GreenDamTanDBLoadDetail::BinaryCursor cursor(buffer);
        unsigned int rowCount = 0;
        if (!cursor.ReadU32(rowCount)) {
            return false;
        }

        CTableLoader_S loader{};
        loader.m_nRowCount = static_cast<int>(rowCount);
        loader.m_strTableName = "tb_Quest_Episode";
        for (unsigned int index = 0; index < rowCount; ++index) {
            TB_QUEST_EPISODE row{};
            std::string utf8Text;
            if (!cursor.ReadCheckSumU32(&row.ID) || !cursor.ReadCheckSumU32(&row.Complete_Bit) ||
                !cursor.ReadCheckSumUtf16String(utf8Text)) {
                return false;
            }
            GreenDamTanDBLoadDetail::CopyString(row.Name, utf8Text);

            if (!cursor.ReadCheckSumU32(&row.Chapter_ID) ||
                !cursor.ReadCheckSumU8(&row.Contents_Type) ||
                !cursor.ReadCheckSumU8(&row.Progress_Type) ||
                !cursor.ReadCheckSumU8(&row.Repeat_Value) ||
                !cursor.ReadCheckSumU32(&row.Before_Episode_ID_1) ||
                !cursor.ReadCheckSumU32(&row.Before_Episode_ID_2) ||
                !cursor.ReadCheckSumU32(&row.Before_Episode_ID_3) ||
                !cursor.ReadCheckSumU32(&row.Before_Episode_ID_4) ||
                !cursor.ReadCheckSumU32(&row.Before_Episode_ID_5) ||
                !cursor.ReadCheckSumU32(&row.Before_Episode_ID_6) ||
                !cursor.ReadCheckSumU32(&row.Before_Episode_ID_7) ||
                !cursor.ReadCheckSumU32(&row.Before_Episode_ID_8) ||
                !cursor.ReadCheckSumU32(&row.Before_Episode_ID_9) ||
                !cursor.ReadCheckSumU32(&row.Before_Episode_ID_10) ||
                !cursor.ReadCheckSumU32(&row.Link_Episode_ID_01) ||
                !cursor.ReadCheckSumU32(&row.Link_Episode_ID_02) ||
                !cursor.ReadCheckSumU32(&row.Link_Episode_ID_03) ||
                !cursor.ReadCheckSumU32(&row.Link_Episode_ID_04) ||
                !cursor.ReadCheckSumU32(&row.Link_Episode_ID_05) ||
                !cursor.ReadCheckSumU32(&row.Link_Episode_ID_06) ||
                !cursor.ReadCheckSumU32(&row.Link_Episode_ID_07) ||
                !cursor.ReadCheckSumU32(&row.Link_Episode_ID_08) ||
                !cursor.ReadCheckSumU32(&row.Link_Episode_ID_09) ||
                !cursor.ReadCheckSumU32(&row.Link_Episode_ID_10) ||
                !cursor.ReadCheckSumU32(&row.Time_Limit) ||
                !cursor.ReadCheckSumU8(&row.Quest_Level) ||
                !cursor.ReadCheckSumU16(&row.Class_Type) ||
                !cursor.ReadCheckSumU8(&row.Auto_Start) ||
                !cursor.ReadCheckSumU8(&row.Auto_Start_Event) ||
                !cursor.ReadCheckSumU32(&row.Auto_Start_Event_ID) ||
                !cursor.ReadCheckSumU8(&row.Start_Object_Type) ||
                !cursor.ReadCheckSumU32(&row.Start_Object_ID) ||
                !cursor.ReadCheckSumU8(&row.Finish_Type) ||
                !cursor.ReadCheckSumU8(&row.Finish_Object_Type) ||
                !cursor.ReadCheckSumU32(&row.Finish_Object_ID) ||
                !cursor.ReadCheckSumU32(&row.Field_ID) ||
                !cursor.ReadCheckSumU32(&row.Maze_group_ID) ||
                !cursor.ReadCheckSumU32(&row.Maze_ID_1) ||
                !cursor.ReadCheckSumU32(&row.Maze_ID_2) ||
                !cursor.ReadCheckSumU32(&row.Maze_ID_3) ||
                !cursor.ReadCheckSumU32(&row.Maze_ID_4) ||
                !cursor.ReadCheckSumU32(&row.Sector_ID_1) ||
                !cursor.ReadCheckSumU32(&row.Sector_ID_2) ||
                !cursor.ReadCheckSumU32(&row.Sector_ID_3) ||
                !cursor.ReadCheckSumU32(&row.Sector_ID_4) ||
                !cursor.ReadCheckSumU8(&row.Give_Up) ||
                !cursor.ReadCheckSumU8(&row.Add_Object_Type_01) ||
                !cursor.ReadCheckSumU8(&row.Add_Object_Type_02) ||
                !cursor.ReadCheckSumU32(&row.Add_Object_ID_01) ||
                !cursor.ReadCheckSumU32(&row.Add_Object_ID_02) ||
                !cursor.ReadCheckSumU32(&row.Remove_Item_ID_01) ||
                !cursor.ReadCheckSumU32(&row.Remove_Item_ID_02) ||
                !cursor.ReadCheckSumU32(&row.Remove_Item_ID_03) ||
                !cursor.ReadCheckSumU32(&row.Remove_Item_ID_04) ||
                !cursor.ReadCheckSumU32(&row.Remove_Item_ID_05) ||
                !cursor.ReadCheckSumU32(&row.Remove_Item_Count_01) ||
                !cursor.ReadCheckSumU32(&row.Remove_Item_Count_02) ||
                !cursor.ReadCheckSumU32(&row.Remove_Item_Count_03) ||
                !cursor.ReadCheckSumU32(&row.Remove_Item_Count_04) ||
                !cursor.ReadCheckSumU32(&row.Remove_Item_Count_05) ||
                !cursor.ReadCheckSumUtf16String(utf8Text)) {
                return false;
            }
            GreenDamTanDBLoadDetail::CopyString(row.Quest_Script_Name, utf8Text);

            if (!cursor.ReadCheckSumU8(&row.Episode_Reward_Type) ||
                !cursor.ReadCheckSumU32(&row.Reward_EXP) ||
                !cursor.ReadCheckSumU32(&row.Reward_Money) ||
                !cursor.ReadCheckSumU32(&row.Reward_Ether) ||
                !cursor.ReadCheckSumU32(&row.Reward_BP) ||
                !cursor.ReadCheckSumU32(&row.Reward_Title_01) ||
                !cursor.ReadCheckSumU32(&row.Reward_Title_02) ||
                !cursor.ReadCheckSumU32(&row.Reward_Awakening) ||
                !cursor.ReadCheckSumU8(&row.Reward_Item_type) ||
                !cursor.ReadCheckSumU32(&row.Reward_Item_ID_01) ||
                !cursor.ReadCheckSumU32(&row.Reward_Item_ID_02) ||
                !cursor.ReadCheckSumU32(&row.Reward_Item_ID_03) ||
                !cursor.ReadCheckSumU32(&row.Reward_Item_ID_04) ||
                !cursor.ReadCheckSumU32(&row.Reward_Item_ID_05) ||
                !cursor.ReadCheckSumU32(&row.Reward_Item_Count_01) ||
                !cursor.ReadCheckSumU32(&row.Reward_Item_Count_02) ||
                !cursor.ReadCheckSumU32(&row.Reward_Item_Count_03) ||
                !cursor.ReadCheckSumU32(&row.Reward_Item_Count_04) ||
                !cursor.ReadCheckSumU32(&row.Reward_Item_Count_05) ||
                !cursor.ReadCheckSumU8(&row.Select_Reward_Item_type) ||
                !cursor.ReadCheckSumU32(&row.Select_Reward_Item_01) ||
                !cursor.ReadCheckSumU32(&row.Select_Reward_Item_02) ||
                !cursor.ReadCheckSumU32(&row.Select_Reward_Item_03) ||
                !cursor.ReadCheckSumU32(&row.Select_Reward_Item_04) ||
                !cursor.ReadCheckSumU32(&row.Select_Reward_Item_05) ||
                !cursor.ReadCheckSumU32(&row.Select_Reward_Item_Count_01) ||
                !cursor.ReadCheckSumU32(&row.Select_Reward_Item_Count_02) ||
                !cursor.ReadCheckSumU32(&row.Select_Reward_Item_Count_03) ||
                !cursor.ReadCheckSumU32(&row.Select_Reward_Item_Count_04) ||
                !cursor.ReadCheckSumU32(&row.Select_Reward_Item_Count_05) ||
                !cursor.ReadCheckSumU32(&row.Reward_Class_Skill_01) ||
                !cursor.ReadCheckSumU32(&row.Reward_Class_Skill_02) ||
                !cursor.ReadCheckSumU32(&row.Reward_Class_Skill_03) ||
                !cursor.ReadCheckSumU32(&row.Reward_Class_Skill_04) ||
                !cursor.ReadCheckSumU32(&row.Reward_Helper) ||
                !cursor.ReadCheckSumU8(&row.Condition_Type) ||
                !cursor.ReadCheckSumU32(&row.Condition_ID_01) ||
                !cursor.ReadCheckSumU32(&row.Condition_ID_02) ||
                !cursor.ReadCheckSumU32(&row.Condition_ID_03) ||
                !cursor.ReadCheckSumU32(&row.Condition_ID_04) ||
                !cursor.ReadCheckSumU32(&row.Condition_ID_05) ||
                !cursor.ReadCheckSumU32(&row.Condition_ID_06) ||
                !cursor.ReadCheckSumU32(&row.Condition_ID_07) ||
                !cursor.ReadCheckSumU32(&row.Condition_ID_08) ||
                !cursor.ReadCheckSumU32(&row.Condition_ID_09) ||
                !cursor.ReadCheckSumU32(&row.Condition_ID_10) ||
                !cursor.ReadCheckSumU32(&row.Restore_Item) ||
                !cursor.ReadCheckSumU8(&row.Save_Quest)) {
                return false;
            }

            SetTB_QUEST_EPISODE(row.ID, row);
        }

        loader.m_biCheckSum = cursor.GetCheckSum();
        std::string md5Value;
        if (!cursor.ReadAsciiString(md5Value, 32) || !cursor.IsAtEnd()) {
            return false;
        }
        std::memset(loader.m_szMD5, 0, sizeof(loader.m_szMD5));
        const std::size_t md5CopyLength =
            md5Value.size() < sizeof(loader.m_szMD5) ? md5Value.size() : sizeof(loader.m_szMD5);
        std::memcpy(loader.m_szMD5, md5Value.data(), md5CopyLength);
        return CheckSum(loader);
    }

    bool InitCommonDB(RES_LOAD_TYPE eResLoadType,
                      const char* szResFilePath,
                      int nWorldID,
                      int nServerID) {
        (void)nWorldID;
        (void)nServerID;

        if (eResLoadType == RES_LOAD_TYPE_FILE) {
            return Load(szResFilePath);
        }

        // TODO: 需人工审查：`InitCommonDB` 的 ODBC 取连接、事务和表加载链尚未恢复。
        return true;
    }

    bool InitGameDB() {
        // TODO: 需人工审查：原版会在 `m_bGameDBLoad` 时继续接 `InitGameDB`。
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

        appearanceRows_.clear();
        createClothRows_.clear();
        characterInfoRows_.clear();
        provideItemRows_.clear();
        photoItemRows_.clear();
        photoItemIdRows_.clear();
        commonRows_.clear();
        m_mapTB_WEEK_GROUP.clear();
        m_mapTB_QUEST_EPISODE.clear();
        m_mapTB_PC_AKASHIC.clear();
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
               LoadProvideItemFile(basePath) && LoadCommonFile(basePath) &&
               LoadWeekGroupFile(basePath) &&
               LoadPCAkashicFile(basePath) && LoadAkashicRecordsFile(basePath) &&
               LoadQuestEpisodeFile(basePath);
    }

    void SetStatusTable() {
        // TODO: 需人工审查：原版会把多张状态表折叠成运行时索引。
    }

    void LoadFactionInfo() {
        // TODO: 需人工审查：阵营信息初始化链尚未恢复。
    }

    void InitInfiniteTowerTable() {
        // TODO: 需人工审查：无限塔资源后处理尚未恢复。
    }

    void InitTitleOpenCondition() {
        // TODO: 需人工审查：称号开放条件表尚未恢复。
    }

    void InitPartyRevise() {
        // TODO: 需人工审查：组队修正表尚未恢复。
    }

    void InitCharacterInfo() {
        // TODO: 需人工审查：角色附加索引表尚未恢复。
    }

    void InitMaze() {
        // TODO: 需人工审查：Maze 资源后处理尚未恢复。
    }

    void InitLevelUpMail() {
        // TODO: 需人工审查：升级邮件映射尚未恢复。
    }

    void InitWorldModeGroup() {
        // TODO: 需人工审查：WorldMode Group 初始化尚未恢复。
    }

    void InitWorldModeDate() {
        // TODO: 需人工审查：WorldMode Date 初始化尚未恢复。
    }

    void InitDayEvent() {
        // TODO: 需人工审查：日常活动初始化尚未恢复。
    }

    void InitWeeklyEventGroupID() {
        // TODO: 需人工审查：周常活动组索引尚未恢复。
    }

    void InitRankingInfoTable() {
        // TODO: 需人工审查：排行表附加索引尚未恢复。
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
        for (auto& entry : photoItemRows_) {
            TB_PHOTO_ITEM& row = entry.second;
            if (row.Char_Class == 0 || row.Char_Class >= 9) {
                LogHelper::LogError("game.system",
                                    "[InitDefaultPhotoItemID] Error TB_PHOTO_ITEM Table - Char_Class [ID:%d]",
                                    row.ID);
                continue;
            }

            if (row.Char_Promotion_Info > 2) {
                LogHelper::LogError(
                    "game.system",
                    "[InitDefaultPhotoItemID] Error TB_PHOTO_ITEM Table - Char_Promotion_Info  [ID:%d]",
                    row.ID);
                continue;
            }

            if (row.Char_Promotion_Info == 0) {
                continue;
            }

            const ST_PHOTO_ID key{row.Char_Class, row.Char_Promotion_Info};
            const auto [it, inserted] = photoItemIdRows_.emplace(key, &row);
            (void)it;
            if (!inserted) {
                LogHelper::LogError(
                    "game.system",
                    "[InitDefaultPhotoItemID] Error TB_PHOTO_ITEM Table - Already  [ID:%d]",
                    row.ID);
            }
        }
    }

    void InitPCCostume() {
        // TODO: 需人工审查：职业默认时装索引尚未恢复。
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

    void InitNetCafeMissionList() {
        // TODO: 需人工审查：网吧任务列表初始化尚未恢复。
    }

    void InitRoguelikeData() {
        // TODO: 需人工审查：Roguelike 数据索引尚未恢复。
    }

    std::string m_strCommonDNS;
    std::string m_strGameDNS;
    std::string m_strPath;
    bool m_bGameDBLoad = false;
    unsigned int m_dwServerID = 0;
    int m_nMainVersion = 0;
    int m_nSubVersion = 0;
    int m_nDataVersion = 0;
    std::unordered_map<std::uint16_t, TB_SYSTEMMAIL_ADD> autoMailRows_;
    std::unordered_map<std::uint16_t, TB_CHARACTER_INFO> characterInfoRows_;
    std::unordered_map<unsigned int, TB_APPEARANCE> appearanceRows_;
    std::unordered_map<std::uint16_t, TB_CREATE_CLOTH> createClothRows_;
    std::unordered_map<std::uint16_t, TB_PROVIDE_ITEM> provideItemRows_;
    std::map<unsigned int, TB_PHOTO_ITEM> photoItemRows_;
    std::unordered_map<unsigned int, TB_ITEM> itemRows_;
    std::unordered_map<unsigned int, TB_RANDOM_OPTION> randomOptionRows_;
    std::unordered_map<std::uint16_t, TB_BROACH_SET> broachSetRows_;
    std::unordered_map<unsigned int, TB_COSTUME_SOCKET> costumeSocketRows_;
    std::unordered_map<unsigned int, TB_ITEM_TITLE> itemTitleRows_;
    std::unordered_map<unsigned int, TB_ITEM_TITLE_GROUP> itemTitleGroupRows_;
    std::unordered_map<unsigned int, TB_ITEM_TITLE_VALUE> itemTitleValueRows_;
    std::unordered_map<unsigned int, TB_REINFORCE> reinforceRows_;
    std::unordered_map<unsigned int, TB_SOCKET> socketRows_;
    std::unordered_map<unsigned int, TB_ITEM_CLASSIFY> itemClassifyRows_;
    std::unordered_map<unsigned int, TB_NAMEFILTER> nameFilterRows_;
    std::map<ST_PHOTO_ID, TB_PHOTO_ITEM*> photoItemIdRows_;
    std::map<unsigned int, TB_QUEST_EPISODE> m_mapTB_QUEST_EPISODE;
    std::map<unsigned int, unsigned int> m_mapQuestCompleteBit;
    std::map<unsigned int, unsigned int> m_mapAutoAcceptQuest;
    std::map<unsigned int, TB_PC_AKASHIC> m_mapTB_PC_AKASHIC;
    std::map<unsigned int, TB_AKASHIC_RECORDS> m_mapTB_AKASHIC_RECORDS;
    std::map<unsigned int, TB_AKASHIC_RECORDS*> m_mapPCAkashic;
    std::map<unsigned int, unsigned int> m_mapMainQuestOrder_Char1;
    unsigned int m_dwMainQuestLastOrder_Char1 = 0;
    std::map<unsigned int, unsigned int> m_mapMainQuestOrder_Char2;
    unsigned int m_dwMainQuestLastOrder_Char2 = 0;
    std::map<unsigned int, unsigned int> m_mapMainQuestOrder_Char3;
    unsigned int m_dwMainQuestLastOrder_Char3 = 0;
    std::vector<unsigned int> m_vecQuestEpisodeOtherContents;
    std::map<std::uint8_t, TB_WEEK_GROUP> m_mapTB_WEEK_GROUP;
    std::unordered_map<unsigned int, TB_COMMON> commonRows_;
    std::unordered_map<int, bool> serverContents_;
    PS_CONTENTS_INFO contentsInfo_{};
    XDBManager m_xCommonDBMgr;
    XDBManager m_xGameDBMgr;
};
