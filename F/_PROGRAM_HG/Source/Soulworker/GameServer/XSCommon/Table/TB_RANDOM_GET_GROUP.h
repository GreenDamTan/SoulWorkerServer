// 中文说明：
// 1. 本文件承接 TB_RANDOM_GET_GROUP 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_RANDOM_GET_GROUP {
    std::uint16_t Random_Get_ID = 0;
    unsigned int Group_Des = 0;
    std::uint8_t UI_Effect_Type = 0;
    std::uint8_t Period_Use = 0;
    std::uint16_t Start_Year = 0;
    std::uint8_t Start_Month = 0;
    std::uint8_t Start_Day = 0;
    std::uint16_t End_Year = 0;
    std::uint8_t End_Month = 0;
    std::uint8_t End_Day = 0;
    std::uint16_t Gacha_Group_ID = 0;
    std::uint16_t uniGroup_ID[20] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_RANDOM_GET_GROUP) == 0x3A, "TB_RANDOM_GET_GROUP size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_RANDOM_GET_GROUP* GetTB_RANDOM_GET_GROUP(std::uint16_t index) ;
    void SetTB_RANDOM_GET_GROUP(std::uint16_t index, const TB_RANDOM_GET_GROUP& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBRandomGetGroupDB() ;
    std::map<std::uint16_t, TB_RANDOM_GET_GROUP> m_mapTB_RANDOM_GET_GROUP;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_RANDOM_GET_GROUP* XResourceMgr::GetTB_RANDOM_GET_GROUP(std::uint16_t index) {
        const auto it = m_mapTB_RANDOM_GET_GROUP.find(index);
        return it == m_mapTB_RANDOM_GET_GROUP.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_RANDOM_GET_GROUP(std::uint16_t index, const TB_RANDOM_GET_GROUP& row) {
        m_mapTB_RANDOM_GET_GROUP[index] = row;
    }

std::int64_t XResourceMgr::LoadTBRandomGetGroupDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Random_Get_ID], [Group_Des], [UI_Effect_Type], [Period_Use], [Start_Year], [Start_Month], [Start_Day], [End_Year], [End_Month], [End_Day], [Gacha_Group_ID], [Group_ID_01], [Group_ID_02], [Group_ID_03], [Group_ID_04], [Group_ID_05], [Group_ID_06], [Group_ID_07], [Group_ID_08], [Group_ID_09], [Group_ID_10], [Group_ID_11], [Group_ID_12], [Group_ID_13], [Group_ID_14], [Group_ID_15], [Group_ID_16], [Group_ID_17], [Group_ID_18], [Group_ID_19], [Group_ID_20] from tb_Random_Get_Group")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_RANDOM_GET_GROUP.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_RANDOM_GET_GROUP row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Random_Get_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_Des, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.UI_Effect_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Period_Use, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Start_Year, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Start_Month, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Start_Day, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.End_Year, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.End_Month, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.End_Day, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Gacha_Group_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (auto& groupId : row.uniGroup_ID) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &groupId, &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_RANDOM_GET_GROUP(row.Random_Get_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
