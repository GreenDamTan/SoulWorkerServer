// 中文说明：
// 1. 本文件承接 TB_ITEM_COUNTBOX 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_ITEM_COUNTBOX {
    unsigned int CountBox_ID = 0;
    std::uint8_t Usage_Cnt = 0;
    unsigned int Usage_Group_ID[15] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_ITEM_COUNTBOX) == 0x41, "TB_ITEM_COUNTBOX size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_ITEM_COUNTBOX* GetTB_ITEM_COUNTBOX(unsigned int index) ;
    void SetTB_ITEM_COUNTBOX(unsigned int index, const TB_ITEM_COUNTBOX& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBItemCountBoxDB() ;
    std::unordered_map<unsigned int, TB_ITEM_COUNTBOX> itemCountBoxRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_ITEM_COUNTBOX* XResourceMgr::GetTB_ITEM_COUNTBOX(unsigned int index) {
        auto it = itemCountBoxRows_.find(index);
        if (it == itemCountBoxRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_ITEM_COUNTBOX(unsigned int index, const TB_ITEM_COUNTBOX& row) {
        itemCountBoxRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBItemCountBoxDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [CountBox_ID], [Usage_Cnt], [Usage_Group_1_ID], [Usage_Group_2_ID], [Usage_Group_3_ID], [Usage_Group_4_ID], [Usage_Group_5_ID], [Usage_Group_6_ID], [Usage_Group_7_ID], [Usage_Group_8_ID], [Usage_Group_9_ID], [Usage_Group_10_ID], [Usage_Group_11_ID], [Usage_Group_12_ID], [Usage_Group_13_ID], [Usage_Group_14_ID], [Usage_Group_15_ID], [Usage_Group_16_ID], [Usage_Group_17_ID], [Usage_Group_18_ID], [Usage_Group_19_ID], [Usage_Group_20_ID], [Usage_Group_21_ID], [Usage_Group_22_ID], [Usage_Group_23_ID], [Usage_Group_24_ID], [Usage_Group_25_ID], [Usage_Group_26_ID], [Usage_Group_27_ID], [Usage_Group_28_ID], [Usage_Group_29_ID], [Usage_Group_30_ID] from tb_Item_CountBox")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        itemCountBoxRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_ITEM_COUNTBOX row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.CountBox_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Usage_Cnt, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 15; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Usage_Group_ID[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_ITEM_COUNTBOX(row.CountBox_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
