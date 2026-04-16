// 中文说明：
// 1. 本文件承接 TB_RANDOM_OPTION 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
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
static_assert(sizeof(TB_RANDOM_OPTION) == 0x105, "TB_RANDOM_OPTION size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_RANDOM_OPTION* GetTB_RANDOM_OPTION(unsigned int index) ;
    void SetTB_RANDOM_OPTION(unsigned int index, const TB_RANDOM_OPTION& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBRandomOptionDB() ;
    std::unordered_map<unsigned int, TB_RANDOM_OPTION> randomOptionRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_RANDOM_OPTION* XResourceMgr::GetTB_RANDOM_OPTION(unsigned int index) {
        auto it = randomOptionRows_.find(index);
        if (it == randomOptionRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_RANDOM_OPTION(unsigned int index, const TB_RANDOM_OPTION& row) {
        randomOptionRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBRandomOptionDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Option_ID], [Option_Min], [Option_Max], [Option_Refer_Range], [Option1_Type], [Option1_1_Min], [Option1_1_Max], [Option2_Type], [Option1_2_Min], [Option1_2_Max], [Option3_Type], [Option1_3_Min], [Option1_3_Max], [Option4_Type], [Option1_4_Min], [Option1_4_Max], [Option5_Type], [Option1_5_Min], [Option1_5_Max], [Option6_Type], [Option1_6_Min], [Option1_6_Max], [Option7_Type], [Option1_7_Min], [Option1_7_Max], [Option8_Type], [Option1_8_Min], [Option1_8_Max], [Option9_Type], [Option1_9_Min], [Option1_9_Max], [Option10_Type], [Option1_10_Min], [Option1_10_Max], [Option11_Type], [Option1_11_Min], [Option1_11_Max], [Option12_Type], [Option1_12_Min], [Option1_12_Max], [Option13_Type], [Option1_13_Min], [Option1_13_Max], [Option14_Type], [Option1_14_Min], [Option1_14_Max], [Option15_Type], [Option1_15_Min], [Option1_15_Max], [Option16_Type], [Option1_16_Min], [Option1_16_Max], [Option17_Type], [Option1_17_Min], [Option1_17_Max], [Option18_Type], [Option1_18_Min], [Option1_18_Max], [Option19_Type], [Option1_19_Min], [Option1_19_Max], [Option20_Type], [Option1_20_Min], [Option1_20_Max], [Option21_Type], [Option1_21_Min], [Option1_21_Max], [Option22_Type], [Option1_22_Min], [Option1_22_Max], [Option23_Type], [Option1_23_Min], [Option1_23_Max], [Option24_Type], [Option1_24_Min], [Option1_24_Max], [Option25_Type], [Option1_25_Min], [Option1_25_Max], [Option_Change_ID] from tb_Random_Option")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        randomOptionRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_RANDOM_OPTION row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Option_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Option_Min, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Option_Max, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Option_Refer_Range, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            auto optionTypes = std::array<std::uint16_t*, 25>{&row.Option1_Type,&row.Option2_Type,&row.Option3_Type,&row.Option4_Type,&row.Option5_Type,&row.Option6_Type,&row.Option7_Type,&row.Option8_Type,&row.Option9_Type,&row.Option10_Type,&row.Option11_Type,&row.Option12_Type,&row.Option13_Type,&row.Option14_Type,&row.Option15_Type,&row.Option16_Type,&row.Option17_Type,&row.Option18_Type,&row.Option19_Type,&row.Option20_Type,&row.Option21_Type,&row.Option22_Type,&row.Option23_Type,&row.Option24_Type,&row.Option25_Type};
            auto optionMins = std::array<int*, 25>{&row.Option1_1_Min,&row.Option1_2_Min,&row.Option1_3_Min,&row.Option1_4_Min,&row.Option1_5_Min,&row.Option1_6_Min,&row.Option1_7_Min,&row.Option1_8_Min,&row.Option1_9_Min,&row.Option1_10_Min,&row.Option1_11_Min,&row.Option1_12_Min,&row.Option1_13_Min,&row.Option1_14_Min,&row.Option1_15_Min,&row.Option1_16_Min,&row.Option1_17_Min,&row.Option1_18_Min,&row.Option1_19_Min,&row.Option1_20_Min,&row.Option1_21_Min,&row.Option1_22_Min,&row.Option1_23_Min,&row.Option1_24_Min,&row.Option1_25_Min};
            auto optionMaxs = std::array<int*, 25>{&row.Option1_1_Max,&row.Option1_2_Max,&row.Option1_3_Max,&row.Option1_4_Max,&row.Option1_5_Max,&row.Option1_6_Max,&row.Option1_7_Max,&row.Option1_8_Max,&row.Option1_9_Max,&row.Option1_10_Max,&row.Option1_11_Max,&row.Option1_12_Max,&row.Option1_13_Max,&row.Option1_14_Max,&row.Option1_15_Max,&row.Option1_16_Max,&row.Option1_17_Max,&row.Option1_18_Max,&row.Option1_19_Max,&row.Option1_20_Max,&row.Option1_21_Max,&row.Option1_22_Max,&row.Option1_23_Max,&row.Option1_24_Max,&row.Option1_25_Max};
            for (int index = 0; index < 25; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, optionTypes[index], &executeResult) ||
                    !GreenDamTan_DBGetSigned(xDBBinder, optionMins[index], &executeResult) ||
                    !GreenDamTan_DBGetSigned(xDBBinder, optionMaxs[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Option_Change_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_RANDOM_OPTION(row.Option_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
