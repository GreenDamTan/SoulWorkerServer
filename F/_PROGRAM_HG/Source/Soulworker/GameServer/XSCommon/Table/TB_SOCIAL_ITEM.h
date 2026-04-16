// 中文说明：
// 1. 本文件承接 TB_SOCIAL_ITEM 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_SOCIAL_ITEM {
    std::uint16_t ID = 0;
    unsigned int Collision_Range = 0;
    unsigned int Use_Range = 0;
    unsigned int Offset_Dis = 0;
    std::uint8_t Use_Max = 0;
    unsigned int Duration_Time = 0;
    std::uint16_t Aura_ID = 0;
    char Social_Code_Name[255] = {};
    unsigned int Furniture_Object_ID = 0;
    std::uint8_t Social_Item_Type = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_SOCIAL_ITEM) == 0x119, "TB_SOCIAL_ITEM size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_SOCIAL_ITEM* GetTB_SOCIAL_ITEM(std::uint16_t index) ;
    void SetTB_SOCIAL_ITEM(std::uint16_t index, const TB_SOCIAL_ITEM& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBSocialItemDB() ;
    std::map<std::uint16_t, TB_SOCIAL_ITEM> m_mapTB_SOCIAL_ITEM;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_SOCIAL_ITEM* XResourceMgr::GetTB_SOCIAL_ITEM(std::uint16_t index) {
        const auto it = m_mapTB_SOCIAL_ITEM.find(index);
        return it == m_mapTB_SOCIAL_ITEM.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_SOCIAL_ITEM(std::uint16_t index, const TB_SOCIAL_ITEM& row) {
        m_mapTB_SOCIAL_ITEM[index] = row;
    }

std::int64_t XResourceMgr::LoadTBSocialItemDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Collision_Range], [Use_Range], [Offset_Dis], [Use_Max], [Duration_Time], [Aura_ID], [Social_Code_Name], [Furniture_Object_ID], [Social_Item_Type] from tb_Social_Item")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_SOCIAL_ITEM.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_SOCIAL_ITEM row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Collision_Range, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Use_Range, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Offset_Dis, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Use_Max, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Duration_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Aura_ID, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Social_Code_Name, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Furniture_Object_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Social_Item_Type, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_SOCIAL_ITEM(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
