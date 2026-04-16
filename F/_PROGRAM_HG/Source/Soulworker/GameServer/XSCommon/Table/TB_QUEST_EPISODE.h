// 中文说明：
// 1. 本文件承接 TB_QUEST_EPISODE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
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
static_assert(sizeof(TB_QUEST_EPISODE) == 0x581, "TB_QUEST_EPISODE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_QUEST_EPISODE* GetTB_QUEST_EPISODE(unsigned int index) ;
    void SetTB_QUEST_EPISODE(unsigned int index, const TB_QUEST_EPISODE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBQuestEpisodeDB() ;
    bool LoadQuestEpisodeFile(const std::filesystem::path& basePath) ;
    std::map<unsigned int, TB_QUEST_EPISODE> m_mapTB_QUEST_EPISODE;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_QUEST_EPISODE* XResourceMgr::GetTB_QUEST_EPISODE(unsigned int index) {
        const auto it = m_mapTB_QUEST_EPISODE.find(index);
        return it == m_mapTB_QUEST_EPISODE.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_QUEST_EPISODE(unsigned int index, const TB_QUEST_EPISODE& row) {
        m_mapTB_QUEST_EPISODE[index] = row;
    }

std::int64_t XResourceMgr::LoadTBQuestEpisodeDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Complete_Bit], [Name], [Chapter_ID], [Contents_Type], [Progress_Type], [Repeat_Value], [Before_Episode_ID_1], [Before_Episode_ID_2], [Before_Episode_ID_3], [Before_Episode_ID_4], [Before_Episode_ID_5], [Before_Episode_ID_6], [Before_Episode_ID_7], [Before_Episode_ID_8], [Before_Episode_ID_9], [Before_Episode_ID_10], [Link_Episode_ID_01], [Link_Episode_ID_02], [Link_Episode_ID_03], [Link_Episode_ID_04], [Link_Episode_ID_05], [Link_Episode_ID_06], [Link_Episode_ID_07], [Link_Episode_ID_08], [Link_Episode_ID_09], [Link_Episode_ID_10], [Time_Limit], [Quest_Level], [Class_Type], [Auto_Start], [Auto_Start_Event], [Auto_Start_Event_ID], [Start_Object_Type], [Start_Object_ID], [Finish_Type], [Finish_Object_Type], [Finish_Object_ID], [Field_ID], [Maze_group_ID], [Maze_ID_1], [Sector_ID_1], [Maze_ID_2], [Sector_ID_2], [Maze_ID_3], [Sector_ID_3], [Maze_ID_4], [Sector_ID_4], [Give_Up], [Add_Object_Type_01], [Add_Object_ID_01], [Add_Object_Type_02], [Add_Object_ID_02], [Remove_Item_ID_01], [Remove_Item_Count_01], [Remove_Item_ID_02], [Remove_Item_Count_02], [Remove_Item_ID_03], [Remove_Item_Count_03], [Remove_Item_ID_04], [Remove_Item_Count_04], [Remove_Item_ID_05], [Remove_Item_Count_05], [Quest_Script_Name], [Episode_Reward_Type], [Reward_EXP], [Reward_Money], [Reward_Ether], [Reward_BP], [Reward_Title_01], [Reward_Title_02], [Reward_Awakening], [Reward_Item_type], [Reward_Item_ID_01], [Reward_Item_Count_01], [Reward_Item_ID_02], [Reward_Item_Count_02], [Reward_Item_ID_03], [Reward_Item_Count_03], [Reward_Item_ID_04], [Reward_Item_Count_04], [Reward_Item_ID_05], [Reward_Item_Count_05], [Select_Reward_Item_type], [Select_Reward_Item_01], [Select_Reward_Item_Count_01], [Select_Reward_Item_02], [Select_Reward_Item_Count_02], [Select_Reward_Item_03], [Select_Reward_Item_Count_03], [Select_Reward_Item_04], [Select_Reward_Item_Count_04], [Select_Reward_Item_05], [Select_Reward_Item_Count_05], [Reward_Class_Skill_01], [Reward_Class_Skill_02], [Reward_Class_Skill_03], [Reward_Class_Skill_04], [Reward_Helper], [Condition_Type], [Condition_ID_01], [Condition_ID_02], [Condition_ID_03], [Condition_ID_04], [Condition_ID_05], [Condition_ID_06], [Condition_ID_07], [Condition_ID_08], [Condition_ID_09], [Condition_ID_10], [Restore_Item], [Save_Quest] from tb_Quest_Episode")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_QUEST_EPISODE.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_QUEST_EPISODE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Complete_Bit, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Name, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Chapter_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Contents_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Progress_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Repeat_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Before_Episode_ID_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Before_Episode_ID_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Before_Episode_ID_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Before_Episode_ID_4, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Before_Episode_ID_5, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Before_Episode_ID_6, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Before_Episode_ID_7, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Before_Episode_ID_8, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Before_Episode_ID_9, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Before_Episode_ID_10, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Link_Episode_ID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Link_Episode_ID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Link_Episode_ID_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Link_Episode_ID_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Link_Episode_ID_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Link_Episode_ID_06, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Link_Episode_ID_07, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Link_Episode_ID_08, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Link_Episode_ID_09, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Link_Episode_ID_10, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Time_Limit, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Quest_Level, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Class_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Auto_Start, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Auto_Start_Event, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Auto_Start_Event_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Start_Object_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Start_Object_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Finish_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Finish_Object_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Finish_Object_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Field_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_group_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_ID_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sector_ID_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_ID_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sector_ID_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_ID_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sector_ID_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_ID_4, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sector_ID_4, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Give_Up, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Add_Object_Type_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Add_Object_ID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Add_Object_Type_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Add_Object_ID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Remove_Item_ID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Remove_Item_Count_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Remove_Item_ID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Remove_Item_Count_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Remove_Item_ID_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Remove_Item_Count_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Remove_Item_ID_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Remove_Item_Count_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Remove_Item_ID_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Remove_Item_Count_05, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Quest_Script_Name, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Episode_Reward_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_EXP, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Money, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Ether, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_BP, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Title_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Title_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Awakening, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Item_type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Item_ID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Item_Count_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Item_ID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Item_Count_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Item_ID_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Item_Count_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Item_ID_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Item_Count_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Item_ID_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Item_Count_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Select_Reward_Item_type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Select_Reward_Item_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Select_Reward_Item_Count_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Select_Reward_Item_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Select_Reward_Item_Count_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Select_Reward_Item_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Select_Reward_Item_Count_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Select_Reward_Item_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Select_Reward_Item_Count_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Select_Reward_Item_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Select_Reward_Item_Count_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Class_Skill_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Class_Skill_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Class_Skill_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Class_Skill_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Helper, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Condition_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Condition_ID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Condition_ID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Condition_ID_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Condition_ID_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Condition_ID_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Condition_ID_06, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Condition_ID_07, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Condition_ID_08, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Condition_ID_09, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Condition_ID_10, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Restore_Item, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Save_Quest, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_QUEST_EPISODE(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

bool XResourceMgr::LoadQuestEpisodeFile(const std::filesystem::path& basePath) {
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

#endif
