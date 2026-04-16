// 中文说明：
// 1. 本文件承接 TB_RANDOM_GET 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_RANDOM_GET {
    std::uint16_t Random_Get_ID = 0;
    unsigned int uniItem_ID[30] = {};
    std::uint16_t uniI_Chance[30] = {};
    std::uint8_t uniItem_Get_type[30] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_RANDOM_GET) == 0xD4, "TB_RANDOM_GET size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_RANDOM_GET* GetTB_RANDOM_GET(std::uint16_t index) ;
    void SetTB_RANDOM_GET(std::uint16_t index, const TB_RANDOM_GET& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBRandomGetDB() ;
    std::map<std::uint16_t, TB_RANDOM_GET> m_mapTB_RANDOM_GET;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_RANDOM_GET* XResourceMgr::GetTB_RANDOM_GET(std::uint16_t index) {
        const auto it = m_mapTB_RANDOM_GET.find(index);
        return it == m_mapTB_RANDOM_GET.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_RANDOM_GET(std::uint16_t index, const TB_RANDOM_GET& row) {
        m_mapTB_RANDOM_GET[index] = row;
    }

std::int64_t XResourceMgr::LoadTBRandomGetDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Random_Get_ID], [Item_ID_01], [I_Chance_01], [Item_Get_type_01], [Item_ID_02], [I_Chance_02], [Item_Get_type_02], [Item_ID_03], [I_Chance_03], [Item_Get_type_03], [Item_ID_04], [I_Chance_04], [Item_Get_type_04], [Item_ID_05], [I_Chance_05], [Item_Get_type_05], [Item_ID_06], [I_Chance_06], [Item_Get_type_06], [Item_ID_07], [I_Chance_07], [Item_Get_type_07], [Item_ID_08], [I_Chance_08], [Item_Get_type_08], [Item_ID_09], [I_Chance_09], [Item_Get_type_09], [Item_ID_10], [I_Chance_10], [Item_Get_type_10], [Item_ID_11], [I_Chance_11], [Item_Get_type_11], [Item_ID_12], [I_Chance_12], [Item_Get_type_12], [Item_ID_13], [I_Chance_13], [Item_Get_type_13], [Item_ID_14], [I_Chance_14], [Item_Get_type_14], [Item_ID_15], [I_Chance_15], [Item_Get_type_15], [Item_ID_16], [I_Chance_16], [Item_Get_type_16], [Item_ID_17], [I_Chance_17], [Item_Get_type_17], [Item_ID_18], [I_Chance_18], [Item_Get_type_18], [Item_ID_19], [I_Chance_19], [Item_Get_type_19], [Item_ID_20], [I_Chance_20], [Item_Get_type_20], [Item_ID_21], [I_Chance_21], [Item_Get_type_21], [Item_ID_22], [I_Chance_22], [Item_Get_type_22], [Item_ID_23], [I_Chance_23], [Item_Get_type_23], [Item_ID_24], [I_Chance_24], [Item_Get_type_24], [Item_ID_25], [I_Chance_25], [Item_Get_type_25], [Item_ID_26], [I_Chance_26], [Item_Get_type_26], [Item_ID_27], [I_Chance_27], [Item_Get_type_27], [Item_ID_28], [I_Chance_28], [Item_Get_type_28], [Item_ID_29], [I_Chance_29], [Item_Get_type_29], [Item_ID_30], [I_Chance_30], [Item_Get_type_30] from tb_Random_Get")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_RANDOM_GET.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_RANDOM_GET row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Random_Get_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 30; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItem_ID[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniI_Chance[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItem_Get_type[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_RANDOM_GET(row.Random_Get_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
