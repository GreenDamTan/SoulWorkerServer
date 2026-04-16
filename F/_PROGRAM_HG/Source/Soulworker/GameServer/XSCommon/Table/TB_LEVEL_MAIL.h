// 中文说明：
// 1. 本文件承接 TB_LEVEL_MAIL 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_LEVEL_MAIL {
    std::uint16_t LevelMail_ID = 0;
    char Des[511] = {};
    std::uint8_t LevelMail_Post_Type_ID = 0;
    std::uint8_t LevelMail_Condition_Character_Group_ID = 0;
    std::uint8_t LevelMail_Condition_ID = 0;
    unsigned int Start_Date = 0;
    std::uint8_t Start_Hour = 0;
    unsigned int End_Date = 0;
    std::uint8_t End_Hour = 0;
    std::uint8_t LevelMail_Character_Type_ID = 0;
    std::uint8_t LevelMail_Value = 0;
    std::uint8_t LevelMail_Type_On_Off = 0;
    std::uint8_t Level_Link_SystemMail_ID = 0;
    unsigned int LevelMail_ADD_Gold = 0;
    unsigned int LevelMail_ADD_Item_01 = 0;
    unsigned int LevelMail_ADD_Item_02 = 0;
    unsigned int LevelMail_ADD_Item_03 = 0;
    unsigned int LevelMail_ADD_Item_04 = 0;
    unsigned int LevelMail_ADD_Item_05 = 0;
    std::uint16_t LevelMail_ADD_Count_01 = 0;
    std::uint16_t LevelMail_ADD_Count_02 = 0;
    std::uint16_t LevelMail_ADD_Count_03 = 0;
    std::uint16_t LevelMail_ADD_Count_04 = 0;
    std::uint16_t LevelMail_ADD_Count_05 = 0;
};
#pragma pack(pop)
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_LEVEL_MAIL* GetTB_LEVEL_MAIL(std::uint16_t index) ;
    void SetTB_LEVEL_MAIL(std::uint16_t index, const TB_LEVEL_MAIL& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBLevelMailDB() ;
    std::map<std::uint16_t, TB_LEVEL_MAIL> m_mapTB_LEVEL_MAIL;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_LEVEL_MAIL* XResourceMgr::GetTB_LEVEL_MAIL(std::uint16_t index) {
        const auto it = m_mapTB_LEVEL_MAIL.find(index);
        return it == m_mapTB_LEVEL_MAIL.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_LEVEL_MAIL(std::uint16_t index, const TB_LEVEL_MAIL& row) {
        m_mapTB_LEVEL_MAIL[index] = row;
    }

std::int64_t XResourceMgr::LoadTBLevelMailDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [LevelMail_ID], [Des], [LevelMail_Post_Type_ID], [LevelMail_Condition_Character_Group_ID], [LevelMail_Condition_ID], [Start_Date], [Start_Hour], [End_Date], [End_Hour], [LevelMail_Character_Type_ID], [LevelMail_Value], [LevelMail_Type_On_Off], [Level_Link_SystemMail_ID], [LevelMail_ADD_Gold], [LevelMail_ADD_Item_01], [LevelMail_ADD_Count_01], [LevelMail_ADD_Item_02], [LevelMail_ADD_Count_02], [LevelMail_ADD_Item_03], [LevelMail_ADD_Count_03], [LevelMail_ADD_Item_04], [LevelMail_ADD_Count_04], [LevelMail_ADD_Item_05], [LevelMail_ADD_Count_05] from tb_Level_Mail")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_LEVEL_MAIL.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_LEVEL_MAIL row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.LevelMail_ID, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Des, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.LevelMail_Post_Type_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.LevelMail_Condition_Character_Group_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.LevelMail_Condition_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Start_Date, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Start_Hour, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.End_Date, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.End_Hour, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.LevelMail_Character_Type_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.LevelMail_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.LevelMail_Type_On_Off, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Level_Link_SystemMail_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.LevelMail_ADD_Gold, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.LevelMail_ADD_Item_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.LevelMail_ADD_Count_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.LevelMail_ADD_Item_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.LevelMail_ADD_Count_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.LevelMail_ADD_Item_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.LevelMail_ADD_Count_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.LevelMail_ADD_Item_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.LevelMail_ADD_Count_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.LevelMail_ADD_Item_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.LevelMail_ADD_Count_05, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_LEVEL_MAIL(row.LevelMail_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
