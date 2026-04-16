// 中文说明：
// 1. 本文件承接 TB_ITEM_SIMILARGROUP 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_ITEM_SIMILARGROUP {
    unsigned int Group_Index = 0;
    unsigned int Similar_Item_ID[5] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_ITEM_SIMILARGROUP) == 0x18, "TB_ITEM_SIMILARGROUP size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    void SetTB_ITEM_SIMILARGROUP(unsigned int index, const TB_ITEM_SIMILARGROUP& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBItemSimilarGroupDB() ;
    std::unordered_map<unsigned int, TB_ITEM_SIMILARGROUP> itemSimilarGroupRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
void XResourceMgr::SetTB_ITEM_SIMILARGROUP(unsigned int index, const TB_ITEM_SIMILARGROUP& row) {
        itemSimilarGroupRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBItemSimilarGroupDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Group_Index], [Similar_Item_01], [Similar_Item_02], [Similar_Item_03], [Similar_Item_04], [Similar_Item_05] from tb_item_SimilarGroup")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        itemSimilarGroupRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_ITEM_SIMILARGROUP row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_Index, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 5; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Similar_Item_ID[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_ITEM_SIMILARGROUP(row.Group_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
