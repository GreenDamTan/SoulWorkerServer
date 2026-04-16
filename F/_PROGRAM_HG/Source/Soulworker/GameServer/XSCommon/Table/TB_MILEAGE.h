// 中文说明：
// 1. 本文件承接 TB_MILEAGE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 2)
struct TB_MILEAGE {
    unsigned int Mileage_ID = 0;
    std::uint16_t Mileage_Point_01 = 0;
    std::uint16_t Mileage_Point_02 = 0;
    std::uint16_t Mileage_Point_03 = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_MILEAGE) == 0xA, "TB_MILEAGE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MILEAGE* GetTB_MILEAGE(unsigned int index) ;
    void SetTB_MILEAGE(unsigned int index, const TB_MILEAGE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBMileageDB() ;
    std::map<unsigned int, TB_MILEAGE> m_mapTB_MILEAGE;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MILEAGE* XResourceMgr::GetTB_MILEAGE(unsigned int index) {
        const auto it = m_mapTB_MILEAGE.find(index);
        return it == m_mapTB_MILEAGE.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MILEAGE(unsigned int index, const TB_MILEAGE& row) {
        m_mapTB_MILEAGE[index] = row;
    }

std::int64_t XResourceMgr::LoadTBMileageDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Mileage_ID], [Mileage_Point_01], [Mileage_Point_02], [Mileage_Point_03] from tb_Mileage")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MILEAGE.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MILEAGE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Mileage_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Mileage_Point_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Mileage_Point_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Mileage_Point_03, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MILEAGE(row.Mileage_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
