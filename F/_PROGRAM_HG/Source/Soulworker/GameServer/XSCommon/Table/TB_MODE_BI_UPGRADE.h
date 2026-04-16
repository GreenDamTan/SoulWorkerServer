// 中文说明：
// 1. 本文件承接 TB_MODE_BI_UPGRADE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 2)
struct TB_MODE_BI_UPGRADE {
    unsigned int ID = 0;
    std::uint8_t Upgrade_Class = 0;
    std::uint8_t Upgrade_Skill_Fix_Type = 0;
    unsigned int Upgrade_Type = 0;
    unsigned int Upgrade_Value = 0;
    unsigned int Upgrade_Cost_Item = 0;
    unsigned int Upgrade_Cost = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_MODE_BI_UPGRADE) == 0x16, "TB_MODE_BI_UPGRADE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MODE_BI_UPGRADE* GetTB_MODE_BI_UPGRADE(unsigned int index) ;
    void SetTB_MODE_BI_UPGRADE(unsigned int index, const TB_MODE_BI_UPGRADE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBModeBIUpgradeDB() ;
    std::map<unsigned int, TB_MODE_BI_UPGRADE> m_mapTB_MODE_BI_UPGRADE;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MODE_BI_UPGRADE* XResourceMgr::GetTB_MODE_BI_UPGRADE(unsigned int index) {
        const auto it = m_mapTB_MODE_BI_UPGRADE.find(index);
        return it == m_mapTB_MODE_BI_UPGRADE.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MODE_BI_UPGRADE(unsigned int index, const TB_MODE_BI_UPGRADE& row) {
        m_mapTB_MODE_BI_UPGRADE[index] = row;
    }

std::int64_t XResourceMgr::LoadTBModeBIUpgradeDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Upgrade_Class], [Upgrade_Skill_Fix_Type], [Upgrade_Type], [Upgrade_Value], [Upgrade_Cost_Item], [Upgrade_Cost] from tb_Mode_BI_Upgrade")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MODE_BI_UPGRADE.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MODE_BI_UPGRADE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Upgrade_Class, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Upgrade_Skill_Fix_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Upgrade_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Upgrade_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Upgrade_Cost_Item, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Upgrade_Cost, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MODE_BI_UPGRADE(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
