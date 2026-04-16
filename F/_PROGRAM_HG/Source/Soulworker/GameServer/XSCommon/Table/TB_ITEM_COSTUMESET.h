// 中文说明：
// 1. 本文件承接 TB_ITEM_COSTUMESET 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_ITEM_COSTUMESET {
    unsigned int Item_CostumeSet_ID = 0;
    std::uint8_t Item_CostumeSet_Max = 0;
    unsigned int ItemID[6] = {};
    std::uint8_t Need_Gesture_Item1 = 0;
    unsigned int CostumeSet_Gesture1_ID = 0;
    std::uint8_t Need_Gesture_Item2 = 0;
    unsigned int CostumeSet_Gesture2_ID = 0;
    std::uint8_t Need_Gesture_Item3 = 0;
    unsigned int CostumeSet_Gesture3_ID = 0;
    std::uint8_t Need_Equalizer_Item1 = 0;
    unsigned int CostumeSet_Equalizer1_ID = 0;
    std::uint8_t Need_Equalizer_Item2 = 0;
    unsigned int CostumeSet_Equalizer2_ID = 0;
    std::uint8_t Need_Equalizer_Item3 = 0;
    unsigned int CostumeSet_Equalizer3_ID = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_ITEM_COSTUMESET) == 0x3B, "TB_ITEM_COSTUMESET size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_ITEM_COSTUMESET* GetTB_ITEM_COSTUMESET(unsigned int index) ;
    void SetTB_ITEM_COSTUMESET(unsigned int index, const TB_ITEM_COSTUMESET& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBItemCostumeSetDB() ;
    std::unordered_map<unsigned int, TB_ITEM_COSTUMESET> itemCostumeSetRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_ITEM_COSTUMESET* XResourceMgr::GetTB_ITEM_COSTUMESET(unsigned int index) {
        auto it = itemCostumeSetRows_.find(index);
        if (it == itemCostumeSetRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_ITEM_COSTUMESET(unsigned int index, const TB_ITEM_COSTUMESET& row) {
        itemCostumeSetRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBItemCostumeSetDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Item_CostumeSet_ID], [Item_CostumeSet_Max], [ItemID_1], [ItemID_2], [ItemID_3], [ItemID_4], [ItemID_5], [ItemID_6], [Need_Gesture_Item1], [CostumeSet_Gesture1_ID], [Need_Gesture_Item2], [CostumeSet_Gesture2_ID], [Need_Gesture_Item3], [CostumeSet_Gesture3_ID], [Need_Equalizer_Item1], [CostumeSet_Equalizer1_ID], [Need_Equalizer_Item2], [CostumeSet_Equalizer2_ID], [Need_Equalizer_Item3], [CostumeSet_Equalizer3_ID] from tb_Item_CostumeSet")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        itemCostumeSetRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_ITEM_COSTUMESET row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_CostumeSet_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_CostumeSet_Max, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 6; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ItemID[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_Gesture_Item1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.CostumeSet_Gesture1_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_Gesture_Item2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.CostumeSet_Gesture2_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_Gesture_Item3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.CostumeSet_Gesture3_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_Equalizer_Item1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.CostumeSet_Equalizer1_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_Equalizer_Item2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.CostumeSet_Equalizer2_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_Equalizer_Item3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.CostumeSet_Equalizer3_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_ITEM_COSTUMESET(row.Item_CostumeSet_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
