// 中文说明：
// 1. 本文件承接 TB_MODE_BI_UPGRADE_GROUP 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_MODE_BI_UPGRADE_GROUP {
    unsigned int ID = 0;
    std::uint16_t Upgrade_Rate[25] = {};
    std::uint8_t Upgrade_Class[25] = {};
    unsigned int Upgrade_Type[25] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_MODE_BI_UPGRADE_GROUP) == 0xB3, "TB_MODE_BI_UPGRADE_GROUP size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MODE_BI_UPGRADE_GROUP* GetTB_MODE_BI_UPGRADE_GROUP(unsigned int index) ;
    void SetTB_MODE_BI_UPGRADE_GROUP(unsigned int index, const TB_MODE_BI_UPGRADE_GROUP& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBModeBIUpgradeGroupDB() ;
    std::map<unsigned int, TB_MODE_BI_UPGRADE_GROUP> m_mapTB_MODE_BI_UPGRADE_GROUP;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MODE_BI_UPGRADE_GROUP* XResourceMgr::GetTB_MODE_BI_UPGRADE_GROUP(unsigned int index) {
        const auto it = m_mapTB_MODE_BI_UPGRADE_GROUP.find(index);
        return it == m_mapTB_MODE_BI_UPGRADE_GROUP.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MODE_BI_UPGRADE_GROUP(unsigned int index, const TB_MODE_BI_UPGRADE_GROUP& row) {
        m_mapTB_MODE_BI_UPGRADE_GROUP[index] = row;
    }

std::int64_t XResourceMgr::LoadTBModeBIUpgradeGroupDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Upgrade_Rate_01], [Upgrade_Class_01], [Upgrade_Type_01], [Upgrade_Rate_02], [Upgrade_Class_02], [Upgrade_Type_02], [Upgrade_Rate_03], [Upgrade_Class_03], [Upgrade_Type_03], [Upgrade_Rate_04], [Upgrade_Class_04], [Upgrade_Type_04], [Upgrade_Rate_05], [Upgrade_Class_05], [Upgrade_Type_05], [Upgrade_Rate_06], [Upgrade_Class_06], [Upgrade_Type_06], [Upgrade_Rate_07], [Upgrade_Class_07], [Upgrade_Type_07], [Upgrade_Rate_08], [Upgrade_Class_08], [Upgrade_Type_08], [Upgrade_Rate_09], [Upgrade_Class_09], [Upgrade_Type_09], [Upgrade_Rate_10], [Upgrade_Class_10], [Upgrade_Type_10], [Upgrade_Rate_11], [Upgrade_Class_11], [Upgrade_Type_11], [Upgrade_Rate_12], [Upgrade_Class_12], [Upgrade_Type_12], [Upgrade_Rate_13], [Upgrade_Class_13], [Upgrade_Type_13], [Upgrade_Rate_14], [Upgrade_Class_14], [Upgrade_Type_14], [Upgrade_Rate_15], [Upgrade_Class_15], [Upgrade_Type_15], [Upgrade_Rate_16], [Upgrade_Class_16], [Upgrade_Type_16], [Upgrade_Rate_17], [Upgrade_Class_17], [Upgrade_Type_17], [Upgrade_Rate_18], [Upgrade_Class_18], [Upgrade_Type_18], [Upgrade_Rate_19], [Upgrade_Class_19], [Upgrade_Type_19], [Upgrade_Rate_20], [Upgrade_Class_20], [Upgrade_Type_20], [Upgrade_Rate_21], [Upgrade_Class_21], [Upgrade_Type_21], [Upgrade_Rate_22], [Upgrade_Class_22], [Upgrade_Type_22], [Upgrade_Rate_23], [Upgrade_Class_23], [Upgrade_Type_23], [Upgrade_Rate_24], [Upgrade_Class_24], [Upgrade_Type_24], [Upgrade_Rate_25], [Upgrade_Class_25], [Upgrade_Type_25] from tb_Mode_BI_Upgrade_Group")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MODE_BI_UPGRADE_GROUP.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MODE_BI_UPGRADE_GROUP row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 25; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Upgrade_Rate[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Upgrade_Class[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Upgrade_Type[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_MODE_BI_UPGRADE_GROUP(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
