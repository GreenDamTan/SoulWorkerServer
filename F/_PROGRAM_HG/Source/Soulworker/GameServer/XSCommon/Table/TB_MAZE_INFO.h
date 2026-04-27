// 中文说明：
// 1. 本文件承接 TB_MAZE_INFO 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_MAZE_INFO {
    std::uint16_t ID = 0;
    std::uint8_t Maze_Type = 0;
    std::uint16_t Maze_Group = 0;
    std::uint8_t Req_Min_Lv = 0;
    std::uint8_t Admission_Member = 0;
    std::uint16_t Fatigue_Point = 0;
    std::uint16_t Maze_Enter_Count_Group = 0;
    std::uint8_t Maze_Enter_Count_Type = 0;
    std::uint8_t Maze_Enter_Count = 0;
    std::uint8_t Maze_Enter_Count_PC_Room = 0;
    std::uint8_t Maze_Enter_Plus_Day = 0;
    std::uint8_t Maze_Enter_Plus_Value = 0;
    std::uint8_t Helper_Summon_Check = 0;
    unsigned int NeedQuest_ID = 0;
    unsigned int NeedItem_ID = 0;
    std::uint8_t NeedItem_Value = 0;
    std::uint8_t Revival_Type = 0;
    std::uint8_t Revival_Item_Count = 0;
    std::uint8_t Maze_Difficulty_Type = 0;
    std::uint8_t Layer_BitMask = 0;
    std::uint8_t Maze_Episode_No = 0;
    std::uint8_t Clear_Cal_Type = 0;
    std::uint8_t Clear_Con_Type_01 = 0;
    std::uint8_t Clear_Con_Type_02 = 0;
    std::uint8_t Clear_Con_Type_03 = 0;
    unsigned int Clear_Con_Value_01 = 0;
    unsigned int Clear_Con_Value_02 = 0;
    unsigned int Clear_Con_Value_03 = 0;
    unsigned int Check_Clear_Maze = 0;
    std::uint16_t Open_Maze_ID_01 = 0;
    std::uint16_t Open_Maze_Group_ID = 0;
    std::uint16_t Maze_ClearTime = 0;
    unsigned int Maze_Reward_EXP = 0;
    unsigned int Maze_Reward_Money = 0;
    std::uint16_t Maze_Bind_Rate = 0;
    char ServerMap[511] = {};
    char Server_SceneScript_File[511] = {};
    unsigned int Maze_SceneAttack_Monster = 0;
    float Share_Point_Ratio = 0.0f;
    char UI_String[511] = {};
    std::uint16_t Maze_Portal_ID = 0;
    int Maze_Start_X = 0;
    int Maze_Start_Y = 0;
    unsigned int Maze_Size_X = 0;
    unsigned int Maze_Size_Y = 0;
    std::int16_t Escape_District_ID = 0;
    unsigned int Escape_StartBox_ID = 0;
    unsigned int Ing_Quest_ID_01 = 0;
    unsigned int Ing_Quest_ID_02 = 0;
    unsigned int Ing_Quest_ID_03 = 0;
    unsigned int Ing_Quest_ID_04 = 0;
    unsigned int Ing_Quest_ID_05 = 0;
    std::uint8_t Arkashic_Use = 0;
    std::uint8_t Potion_Limit = 0;
    unsigned int WarpOrb_Item_ID = 0;
    std::uint8_t Maze_Re_Enter = 0;
};
static_assert(sizeof(TB_MAZE_INFO) == 0x67B, "TB_MAZE_INFO size mismatch with IDA memcpy_0");
#pragma pack(pop)
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MAZE_INFO* GetTB_MAZE_INFO(std::uint16_t index) ;
    void SetTB_MAZE_INFO(std::uint16_t index, const TB_MAZE_INFO& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBMazeInfoDB() ;
    std::map<std::uint16_t, TB_MAZE_INFO> m_mapTB_MAZE_INFO;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MAZE_INFO* XResourceMgr::GetTB_MAZE_INFO(std::uint16_t index) {
        const auto it = m_mapTB_MAZE_INFO.find(index);
        return it == m_mapTB_MAZE_INFO.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MAZE_INFO(std::uint16_t index, const TB_MAZE_INFO& row) {
        m_mapTB_MAZE_INFO[index] = row;
    }

std::int64_t XResourceMgr::LoadTBMazeInfoDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Maze_Type], [Maze_Group], [Req_Min_Lv], [Admission_Member], [Fatigue_Point], [Maze_Enter_Count_Group], [Maze_Enter_Count_Type], [Maze_Enter_Count], [Maze_Enter_Count_PC_Room], [Maze_Enter_Plus_Day], [Maze_Enter_Plus_Value], [Helper_Summon_Check], [NeedQuest_ID], [NeedItem_ID], [NeedItem_Value], [Revival_Type], [Revival_Item_Count], [Maze_Difficulty_Type], [Layer_BitMask], [Maze_Episode_No], [Clear_Cal_Type], [Clear_Con_Type_01], [Clear_Con_Value_01], [Clear_Con_Type_02], [Clear_Con_Value_02], [Clear_Con_Type_03], [Clear_Con_Value_03], [Check_Clear_Maze], [Open_Maze_ID_01], [Open_Maze_Group_ID], [Maze_ClearTime], [Maze_Reward_EXP], [Maze_Reward_Money], [Maze_Bind_Rate], [ServerMap], [Server_SceneScript_File], [Maze_SceneAttack_Monster], [Share_Point_Ratio], [UI_String], [Maze_Portal_ID], [Maze_Start_X], [Maze_Start_Y], [Maze_Size_X], [Maze_Size_Y], [Escape_District_ID], [Escape_StartBox_ID], [Ing_Quest_ID_01], [Ing_Quest_ID_02], [Ing_Quest_ID_03], [Ing_Quest_ID_04], [Ing_Quest_ID_05], [Arkashic_Use], [Potion_Limit], [WarpOrb_Item_ID], [Maze_Re_Enter] from tb_Maze_Info")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MAZE_INFO.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MAZE_INFO row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Group, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Req_Min_Lv, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Admission_Member, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Fatigue_Point, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Enter_Count_Group, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Enter_Count_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Enter_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Enter_Count_PC_Room, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Enter_Plus_Day, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Enter_Plus_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Helper_Summon_Check, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NeedQuest_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NeedItem_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NeedItem_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Revival_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Revival_Item_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Difficulty_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Layer_BitMask, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Episode_No, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Clear_Cal_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Clear_Con_Type_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Clear_Con_Value_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Clear_Con_Type_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Clear_Con_Value_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Clear_Con_Type_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Clear_Con_Value_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Check_Clear_Maze, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Open_Maze_ID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Open_Maze_Group_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_ClearTime, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Reward_EXP, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Reward_Money, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Bind_Rate, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.ServerMap, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Server_SceneScript_File, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_SceneAttack_Monster, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Share_Point_Ratio, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.UI_String, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Portal_ID, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Maze_Start_X, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Maze_Start_Y, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Size_X, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Size_Y, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Escape_District_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Escape_StartBox_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ing_Quest_ID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ing_Quest_ID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ing_Quest_ID_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ing_Quest_ID_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ing_Quest_ID_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Arkashic_Use, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Potion_Limit, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.WarpOrb_Item_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_Re_Enter, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MAZE_INFO(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
