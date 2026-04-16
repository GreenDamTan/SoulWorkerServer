// 中文说明：
// 1. 本文件承接 TB_MAKE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 2)
struct TB_MAKE {
    unsigned int Make_Index = 0;
    unsigned int Group_ID = 0;
    unsigned int MakeItem_ID_01 = 0;
    unsigned int MakeItem_ID_02 = 0;
    unsigned int MakeItem_ID_03 = 0;
    std::uint16_t Rate_ID_01 = 0;
    std::uint16_t Rate_ID_02 = 0;
    std::uint16_t Rate_ID_03 = 0;
    unsigned int Need_M_1 = 0;
    std::uint16_t M_Count_1 = 0;
    unsigned int Need_M_2 = 0;
    std::uint16_t M_Count_2 = 0;
    unsigned int Need_M_3 = 0;
    std::uint16_t M_Count_3 = 0;
    unsigned int Need_M_4 = 0;
    std::uint16_t M_Count_4 = 0;
    std::uint16_t Need_Fate = 0;
    unsigned int Need_Gold = 0;
    std::uint16_t M_priority = 0;
    std::uint8_t Limit_Type = 0;
    std::uint16_t Limit_Count = 0;
    std::uint8_t Limit_Reset_Time_PeriodType = 0;
};
#pragma pack(pop)
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MAKE* GetTB_MAKE(unsigned int index) ;
    void SetTB_MAKE(unsigned int index, const TB_MAKE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBMakeDB() ;
    std::map<unsigned int, TB_MAKE> m_mapTB_MAKE;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MAKE* XResourceMgr::GetTB_MAKE(unsigned int index) {
        const auto it = m_mapTB_MAKE.find(index);
        return it == m_mapTB_MAKE.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MAKE(unsigned int index, const TB_MAKE& row) {
        m_mapTB_MAKE[index] = row;
    }

std::int64_t XResourceMgr::LoadTBMakeDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Make_Index], [Group_ID], [MakeItem_ID_01], [Rate_ID_01], [MakeItem_ID_02], [Rate_ID_02], [MakeItem_ID_03], [Rate_ID_03], [Need_M_1], [M_Count_1], [Need_M_2], [M_Count_2], [Need_M_3], [M_Count_3], [Need_M_4], [M_Count_4], [Need_Fate], [Need_Gold], [M_priority], [Limit_Type], [Limit_Count], [Limit_Reset_Time_PeriodType] from tb_Make")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MAKE.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MAKE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Make_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.MakeItem_ID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Rate_ID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.MakeItem_ID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Rate_ID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.MakeItem_ID_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Rate_ID_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_M_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.M_Count_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_M_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.M_Count_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_M_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.M_Count_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_M_4, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.M_Count_4, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_Fate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_Gold, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.M_priority, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Limit_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Limit_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Limit_Reset_Time_PeriodType, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MAKE(row.Make_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
