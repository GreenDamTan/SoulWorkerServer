// 中文说明：
// 1. 本文件承接 TB_ITEM_TITLE_GROUP 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_ITEM_TITLE_GROUP {
    unsigned int T_Item_Title_G_ID = 0;
    unsigned int T_Group01_ID_1 = 0;
    unsigned int T_Group01_ID_2 = 0;
    unsigned int T_Group01_ID_3 = 0;
    unsigned int T_Group01_ID_4 = 0;
    unsigned int T_Group01_ID_5 = 0;
    unsigned int T_Group01_ID_6 = 0;
    unsigned int T_Group01_ID_7 = 0;
    unsigned int T_Group01_ID_8 = 0;
    unsigned int T_Group01_ID_9 = 0;
    unsigned int T_Group01_ID_10 = 0;
    std::uint16_t T_Group01_Rate_1 = 0;
    std::uint16_t T_Group01_Rate_2 = 0;
    std::uint16_t T_Group01_Rate_3 = 0;
    std::uint16_t T_Group01_Rate_4 = 0;
    std::uint16_t T_Group01_Rate_5 = 0;
    std::uint16_t T_Group01_Rate_6 = 0;
    std::uint16_t T_Group01_Rate_7 = 0;
    std::uint16_t T_Group01_Rate_8 = 0;
    std::uint16_t T_Group01_Rate_9 = 0;
    std::uint16_t T_Group01_Rate_10 = 0;

    std::array<unsigned int, 10> GetTitleIDs() const {
        return {T_Group01_ID_1, T_Group01_ID_2, T_Group01_ID_3, T_Group01_ID_4, T_Group01_ID_5,
                T_Group01_ID_6, T_Group01_ID_7, T_Group01_ID_8, T_Group01_ID_9, T_Group01_ID_10};
    }

    std::array<std::uint16_t, 10> GetTitleRates() const {
        return {T_Group01_Rate_1, T_Group01_Rate_2, T_Group01_Rate_3, T_Group01_Rate_4, T_Group01_Rate_5,
                T_Group01_Rate_6, T_Group01_Rate_7, T_Group01_Rate_8, T_Group01_Rate_9, T_Group01_Rate_10};
    }
};
#pragma pack(pop)
static_assert(sizeof(TB_ITEM_TITLE_GROUP) == 0x40, "TB_ITEM_TITLE_GROUP size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_ITEM_TITLE_GROUP* GetTB_ITEM_TITLE_GROUP(unsigned int index) ;
    void SetTB_ITEM_TITLE_GROUP(unsigned int index, const TB_ITEM_TITLE_GROUP& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBItemTitleGroupDB() ;
    std::unordered_map<unsigned int, TB_ITEM_TITLE_GROUP> itemTitleGroupRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_ITEM_TITLE_GROUP* XResourceMgr::GetTB_ITEM_TITLE_GROUP(unsigned int index) {
        auto it = itemTitleGroupRows_.find(index);
        if (it == itemTitleGroupRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_ITEM_TITLE_GROUP(unsigned int index, const TB_ITEM_TITLE_GROUP& row) {
        itemTitleGroupRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBItemTitleGroupDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [T_Item_Title_G_ID], [T_Group01_ID_1], [T_Group01_Rate_1], [T_Group01_ID_2], [T_Group01_Rate_2], [T_Group01_ID_3], [T_Group01_Rate_3], [T_Group01_ID_4], [T_Group01_Rate_4], [T_Group01_ID_5], [T_Group01_Rate_5], [T_Group01_ID_6], [T_Group01_Rate_6], [T_Group01_ID_7], [T_Group01_Rate_7], [T_Group01_ID_8], [T_Group01_Rate_8], [T_Group01_ID_9], [T_Group01_Rate_9], [T_Group01_ID_10], [T_Group01_Rate_10] from tb_item_Title_Group")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        itemTitleGroupRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_ITEM_TITLE_GROUP row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.T_Item_Title_G_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            auto titleIds = std::array<unsigned int*, 10>{&row.T_Group01_ID_1,&row.T_Group01_ID_2,&row.T_Group01_ID_3,&row.T_Group01_ID_4,&row.T_Group01_ID_5,&row.T_Group01_ID_6,&row.T_Group01_ID_7,&row.T_Group01_ID_8,&row.T_Group01_ID_9,&row.T_Group01_ID_10};
            auto titleRates = std::array<std::uint16_t*, 10>{&row.T_Group01_Rate_1,&row.T_Group01_Rate_2,&row.T_Group01_Rate_3,&row.T_Group01_Rate_4,&row.T_Group01_Rate_5,&row.T_Group01_Rate_6,&row.T_Group01_Rate_7,&row.T_Group01_Rate_8,&row.T_Group01_Rate_9,&row.T_Group01_Rate_10};
            for (int index = 0; index < 10; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, titleIds[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, titleRates[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_ITEM_TITLE_GROUP(row.T_Item_Title_G_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
