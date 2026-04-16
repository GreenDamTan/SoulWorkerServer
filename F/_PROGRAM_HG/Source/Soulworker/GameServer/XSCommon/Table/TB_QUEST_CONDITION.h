// 中文说明：
// 1. 本文件承接 TB_QUEST_CONDITION 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_QUEST_CONDITION {
    unsigned int ID = 0;
    std::uint8_t Condition = 0;
    unsigned int High_Condition_ID = 0;
    unsigned int Link_Condition_ID = 0;
    std::uint8_t CutScene_Portal_control = 0;
    std::uint8_t Target_Type = 0;
    unsigned int Target_ID = 0;
    unsigned int Field_ID = 0;
    unsigned int Maze_group_ID = 0;
    unsigned int uniMaze_ID[12] = {};
    unsigned int uniSector_ID[12] = {};
    std::uint8_t Group_Counting = 0;
    std::uint8_t Counter_Value = 0;
    unsigned int uniAdd_item_condition[2] = {};
    std::uint8_t uniAdd_item_condition_Count[2] = {};
    unsigned int uniRemove_item_condition[2] = {};
    std::uint8_t uniRemove_item_condition_Count[2] = {};
    unsigned int Fail_Death_ID = 0;
    unsigned int Fail_Range_ID = 0;
    unsigned int Fail_Range = 0;
    char Quest_Script[511] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_QUEST_CONDITION) == 0x29C, "TB_QUEST_CONDITION size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_QUEST_CONDITION* GetTB_QUEST_CONDITION(unsigned int index) ;
    void SetTB_QUEST_CONDITION(unsigned int index, const TB_QUEST_CONDITION& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBQuestConditionDB() ;
    std::map<unsigned int, TB_QUEST_CONDITION> m_mapTB_QUEST_CONDITION;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_QUEST_CONDITION* XResourceMgr::GetTB_QUEST_CONDITION(unsigned int index) {
        const auto it = m_mapTB_QUEST_CONDITION.find(index);
        return it == m_mapTB_QUEST_CONDITION.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_QUEST_CONDITION(unsigned int index, const TB_QUEST_CONDITION& row) {
        m_mapTB_QUEST_CONDITION[index] = row;
    }

std::int64_t XResourceMgr::LoadTBQuestConditionDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Condition], [High_Condition_ID], [Link_Condition_ID], [CutScene_Portal_control], [Target_Type], [Target_ID], [Field_ID], [Maze_group_ID], [Maze_ID_1], [Sector_ID_1], [Maze_ID_2], [Sector_ID_2], [Maze_ID_3], [Sector_ID_3], [Maze_ID_4], [Sector_ID_4], [Maze_ID_5], [Sector_ID_5], [Maze_ID_6], [Sector_ID_6], [Maze_ID_7], [Sector_ID_7], [Maze_ID_8], [Sector_ID_8], [Maze_ID_9], [Sector_ID_9], [Maze_ID_10], [Sector_ID_10], [Maze_ID_11], [Sector_ID_11], [Maze_ID_12], [Sector_ID_12], [Group_Counting], [Counter_Value], [Add_item_condition_01], [Add_item_condition_Count_01], [Add_item_condition_02], [Add_item_condition_Count_02], [Remove_item_condition_01], [Remove_item_condition_Count_01], [Remove_item_condition_02], [Remove_item_condition_Count_02], [Fail_Death_ID], [Fail_Range_ID], [Fail_Range], [Quest_Script] from tb_Quest_Condition")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_QUEST_CONDITION.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_QUEST_CONDITION row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Condition, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.High_Condition_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Link_Condition_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.CutScene_Portal_control, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Target_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Target_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Field_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_group_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 12; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniMaze_ID[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniSector_ID[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_Counting, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Counter_Value, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 2; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniAdd_item_condition[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniAdd_item_condition_Count[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniRemove_item_condition[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniRemove_item_condition_Count[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Fail_Death_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Fail_Range_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Fail_Range, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Quest_Script, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_QUEST_CONDITION(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
