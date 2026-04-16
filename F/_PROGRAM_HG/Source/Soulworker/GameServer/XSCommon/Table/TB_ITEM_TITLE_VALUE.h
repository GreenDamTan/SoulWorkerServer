// 中文说明：
// 1. 本文件承接 TB_ITEM_TITLE_VALUE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_ITEM_TITLE_VALUE {
    unsigned int Item_Title_ID = 0;
    char UI_String[511] = {};
    std::uint8_t Additional_Option0_Type = 0;
    std::uint8_t Additional_Option1_Type = 0;
    std::uint8_t Additional_Option2_Type = 0;
    unsigned int Additional_Option0_ID = 0;
    unsigned int Additional_Option1_ID = 0;
    unsigned int Additional_Option2_ID = 0;
    std::uint16_t Increase_Default_Value0 = 0;
    std::uint16_t Additional_Option_Value1 = 0;
    std::uint16_t Additional_Option_Value2 = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_ITEM_TITLE_VALUE) == 0x218, "TB_ITEM_TITLE_VALUE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_ITEM_TITLE_VALUE* GetTB_ITEM_TITLE_VALUE(unsigned int index) ;
    void SetTB_ITEM_TITLE_VALUE(unsigned int index, const TB_ITEM_TITLE_VALUE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBItemTitleValueDB() ;
    std::unordered_map<unsigned int, TB_ITEM_TITLE_VALUE> itemTitleValueRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_ITEM_TITLE_VALUE* XResourceMgr::GetTB_ITEM_TITLE_VALUE(unsigned int index) {
        auto it = itemTitleValueRows_.find(index);
        if (it == itemTitleValueRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_ITEM_TITLE_VALUE(unsigned int index, const TB_ITEM_TITLE_VALUE& row) {
        itemTitleValueRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBItemTitleValueDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Item_Title_ID], [UI_String], [Additional_Option0_Type], [Additional_Option0_ID], [Increase_Default_Value0], [Additional_Option1_Type], [Additional_Option1_ID], [Additional_Option_Value1], [Additional_Option2_Type], [Additional_Option2_ID], [Additional_Option_Value2] from tb_item_Title_Value")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        itemTitleValueRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_ITEM_TITLE_VALUE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Title_ID, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.UI_String, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Additional_Option0_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Additional_Option0_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Increase_Default_Value0, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Additional_Option1_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Additional_Option1_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Additional_Option_Value1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Additional_Option2_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Additional_Option2_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Additional_Option_Value2, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_ITEM_TITLE_VALUE(row.Item_Title_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
