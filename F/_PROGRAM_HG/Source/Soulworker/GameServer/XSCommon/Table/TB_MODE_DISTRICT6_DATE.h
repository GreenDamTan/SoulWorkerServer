// 中文说明：
// 1. 本文件承接 TB_MODE_DISTRICT6_DATE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_MODE_DISTRICT6_DATE {
    std::uint16_t ID = 0;
    std::uint8_t Day_Type = 0;
    std::uint16_t Start_Time = 0;
    std::uint16_t End_Time = 0;
    unsigned int Booster_Limit_Time = 0;
    std::uint8_t Appear_Group = 0;
    std::uint16_t Clear_Count[6] = {};
    unsigned int Clear_Booster[6] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_MODE_DISTRICT6_DATE) == 0x30, "TB_MODE_DISTRICT6_DATE size must match decompile layout");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MODE_DISTRICT6_DATE* GetTB_MODE_DISTRICT6_DATE(std::uint16_t index) ;
    void SetTB_MODE_DISTRICT6_DATE(std::uint16_t index, const TB_MODE_DISTRICT6_DATE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBModeDistrict6DateDB() ;
    std::map<std::uint16_t, TB_MODE_DISTRICT6_DATE> m_mapTB_MODE_DISTRICT6_DATE;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MODE_DISTRICT6_DATE* XResourceMgr::GetTB_MODE_DISTRICT6_DATE(std::uint16_t index) {
        const auto it = m_mapTB_MODE_DISTRICT6_DATE.find(index);
        return it == m_mapTB_MODE_DISTRICT6_DATE.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MODE_DISTRICT6_DATE(std::uint16_t index, const TB_MODE_DISTRICT6_DATE& row) {
        m_mapTB_MODE_DISTRICT6_DATE[index] = row;
    }

std::int64_t XResourceMgr::LoadTBModeDistrict6DateDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Day_Type], [Start_Time], [End_Time], [Booster_Limit_Time], [Appear_Group], [Clear_Count_01], [Clear_Booster_01], [Clear_Count_02], [Clear_Booster_02], [Clear_Count_03], [Clear_Booster_03], [Clear_Count_04], [Clear_Booster_04], [Clear_Count_05], [Clear_Booster_05], [Clear_Count_06], [Clear_Booster_06] from tb_Mode_District6_Date")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MODE_DISTRICT6_DATE.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MODE_DISTRICT6_DATE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Day_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Start_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.End_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_Limit_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Appear_Group, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 6; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Clear_Count[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Clear_Booster[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_MODE_DISTRICT6_DATE(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
