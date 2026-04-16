// 中文说明：
// 1. 本文件承接 TB_SELECT_ITEM 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_SELECT_ITEM {
    unsigned int ID = 0;
    std::uint8_t Selcect_CNT = 0;
    unsigned int Item_ID[15] = {};
    std::uint16_t Item_ID_Cnt[15] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_SELECT_ITEM) == 0x5F, "TB_SELECT_ITEM size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_SELECT_ITEM* GetTB_SELECT_ITEM(unsigned int index) ;
    void SetTB_SELECT_ITEM(unsigned int index, const TB_SELECT_ITEM& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBSelectItemDB() ;
    std::map<unsigned int, TB_SELECT_ITEM> m_mapTB_SELECT_ITEM;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_SELECT_ITEM* XResourceMgr::GetTB_SELECT_ITEM(unsigned int index) {
        const auto it = m_mapTB_SELECT_ITEM.find(index);
        return it == m_mapTB_SELECT_ITEM.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_SELECT_ITEM(unsigned int index, const TB_SELECT_ITEM& row) {
        m_mapTB_SELECT_ITEM[index] = row;
    }

std::int64_t XResourceMgr::LoadTBSelectItemDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Selcect_CNT], [Item_ID_1], [Item_ID_Cnt_1], [Item_ID_2], [Item_ID_Cnt_2], [Item_ID_3], [Item_ID_Cnt_3], [Item_ID_4], [Item_ID_Cnt_4], [Item_ID_5], [Item_ID_Cnt_5], [Item_ID_6], [Item_ID_Cnt_6], [Item_ID_7], [Item_ID_Cnt_7], [Item_ID_8], [Item_ID_Cnt_8], [Item_ID_9], [Item_ID_Cnt_9], [Item_ID_10], [Item_ID_Cnt_10], [Item_ID_11], [Item_ID_Cnt_11], [Item_ID_12], [Item_ID_Cnt_12], [Item_ID_13], [Item_ID_Cnt_13], [Item_ID_14], [Item_ID_Cnt_14], [Item_ID_15], [Item_ID_Cnt_15] from tb_Select_Item")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_SELECT_ITEM.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_SELECT_ITEM row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Selcect_CNT, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 15; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID_Cnt[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_SELECT_ITEM(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
