// 中文说明：
// 1. 本文件承接 TB_AKASHIC_MAKE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_AKASHIC_MAKE {
    unsigned int Akashic_Make_Index = 0;
    std::uint8_t Main_Material_Type = 0;
    unsigned int Main_Material = 0;
    unsigned int Card_Item = 0;
    std::uint8_t Cash_Material_Value = 0;
    unsigned int Need_Gold = 0;
    unsigned int Create_Akashic = 0;
    unsigned int Random_Need_Meterial = 0;
    unsigned int Random_Need_Gold = 0;
    unsigned int Akashic_Group = 0;
    unsigned int Hidden_Need_Gold = 0;
    unsigned int Hidden_Need_Item = 0;
    unsigned int Create_Hidden = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_AKASHIC_MAKE) == 0x2E, "TB_AKASHIC_MAKE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_AKASHIC_MAKE* GetTB_AKASHIC_MAKE(unsigned int index) ;
    void SetTB_AKASHIC_MAKE(unsigned int index, const TB_AKASHIC_MAKE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBAkashicMakeDB() ;
    std::map<unsigned int, TB_AKASHIC_MAKE> m_mapTB_AKASHIC_MAKE;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_AKASHIC_MAKE* XResourceMgr::GetTB_AKASHIC_MAKE(unsigned int index) {
        const auto it = m_mapTB_AKASHIC_MAKE.find(index);
        return it == m_mapTB_AKASHIC_MAKE.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_AKASHIC_MAKE(unsigned int index, const TB_AKASHIC_MAKE& row) {
        m_mapTB_AKASHIC_MAKE[index] = row;
    }

std::int64_t XResourceMgr::LoadTBAkashicMakeDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Akashic_Make_Index], [Main_Material_Type], [Main_Material], [Card_Item], [Cash_Material_Value], [Need_Gold], [Create_Akashic], [Random_Need_Meterial], [Random_Need_Gold], [Akashic_Group], [Hidden_Need_Gold], [Hidden_Need_Item], [Create_Hidden] from tb_Akashic_Make")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_AKASHIC_MAKE.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_AKASHIC_MAKE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Akashic_Make_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Main_Material_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Main_Material, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Card_Item, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Cash_Material_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_Gold, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Create_Akashic, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Random_Need_Meterial, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Random_Need_Gold, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Akashic_Group, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Hidden_Need_Gold, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Hidden_Need_Item, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Create_Hidden, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_AKASHIC_MAKE(row.Akashic_Make_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
