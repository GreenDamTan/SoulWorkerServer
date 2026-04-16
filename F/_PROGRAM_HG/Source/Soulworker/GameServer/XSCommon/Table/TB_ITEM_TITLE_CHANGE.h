// 中文说明：
// 1. 本文件承接 TB_ITEM_TITLE_CHANGE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_ITEM_TITLE_CHANGE {
    std::uint16_t Tag_Change_ID = 0;
    std::uint16_t Basic_Success_Rate = 0;
    std::uint16_t Need_Point = 0;
    unsigned int Need_Zeny = 0;
    unsigned int Need_Ether = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_ITEM_TITLE_CHANGE) == 0xE, "TB_ITEM_TITLE_CHANGE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_ITEM_TITLE_CHANGE* GetTB_ITEM_TITLE_CHANGE(std::uint16_t index) ;
    void SetTB_ITEM_TITLE_CHANGE(std::uint16_t index, const TB_ITEM_TITLE_CHANGE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBItemTitleChangeDB() ;
    std::unordered_map<std::uint16_t, TB_ITEM_TITLE_CHANGE> itemTitleChangeRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_ITEM_TITLE_CHANGE* XResourceMgr::GetTB_ITEM_TITLE_CHANGE(std::uint16_t index) {
        auto it = itemTitleChangeRows_.find(index);
        if (it == itemTitleChangeRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_ITEM_TITLE_CHANGE(std::uint16_t index, const TB_ITEM_TITLE_CHANGE& row) {
        itemTitleChangeRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBItemTitleChangeDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Tag_Change_ID], [Basic_Success_Rate], [Need_Point], [Need_Zeny], [Need_Ether] from tb_Item_Title_Change")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        itemTitleChangeRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_ITEM_TITLE_CHANGE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Tag_Change_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Basic_Success_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_Point, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_Zeny, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_Ether, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_ITEM_TITLE_CHANGE(row.Tag_Change_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
