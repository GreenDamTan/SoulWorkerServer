// 中文说明：
// 1. 本文件承接 TB_ITEM_COUNTBOX_GROUP 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_ITEM_COUNTBOX_GROUP {
    unsigned int Count_Group_ID = 0;
    unsigned int Item_ID[15] = {};
    std::uint8_t Item_Cnt[15] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_ITEM_COUNTBOX_GROUP) == 0x4F, "TB_ITEM_COUNTBOX_GROUP size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_ITEM_COUNTBOX_GROUP* GetTB_ITEM_COUNTBOX_GROUP(unsigned int index) ;
    void SetTB_ITEM_COUNTBOX_GROUP(unsigned int index, const TB_ITEM_COUNTBOX_GROUP& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBItemCountBoxGroupDB() ;
    std::unordered_map<unsigned int, TB_ITEM_COUNTBOX_GROUP> itemCountBoxGroupRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_ITEM_COUNTBOX_GROUP* XResourceMgr::GetTB_ITEM_COUNTBOX_GROUP(unsigned int index) {
        auto it = itemCountBoxGroupRows_.find(index);
        if (it == itemCountBoxGroupRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_ITEM_COUNTBOX_GROUP(unsigned int index, const TB_ITEM_COUNTBOX_GROUP& row) {
        itemCountBoxGroupRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBItemCountBoxGroupDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Count_Group_ID], [Item_1], [Item_Cnt_1], [Item_2], [Item_Cnt_2], [Item_3], [Item_Cnt_3], [Item_4], [Item_Cnt_4], [Item_5], [Item_Cnt_5], [Item_6], [Item_Cnt_6], [Item_7], [Item_Cnt_7], [Item_8], [Item_Cnt_8], [Item_9], [Item_Cnt_9], [Item_10], [Item_Cnt_10], [Item_11], [Item_Cnt_11], [Item_12], [Item_Cnt_12], [Item_13], [Item_Cnt_13], [Item_14], [Item_Cnt_14], [Item_15], [Item_Cnt_15] from tb_Item_CountBox_Group")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        itemCountBoxGroupRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_ITEM_COUNTBOX_GROUP row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Count_Group_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 15; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Cnt[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_ITEM_COUNTBOX_GROUP(row.Count_Group_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
