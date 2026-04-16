// 中文说明：
// 1. 本文件承接 TB_ITEM_SETITEM 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_ITEM_SETITEM {
    unsigned int SetItem_ID = 0;
    std::uint16_t Set_Name_ID = 0;
    std::uint8_t Set_Count = 0;
    unsigned int SetItem_IDs[6] = {};
    std::uint8_t Group[5] = {};
    std::uint8_t SetOptionClass[5][4] = {};
    std::uint16_t SetOptionType[5][4] = {};
    unsigned int SetOptionValue[5][4] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_ITEM_SETITEM) == 0xB0, "TB_ITEM_SETITEM size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_ITEM_SETITEM* GetTB_ITEM_SETITEM(unsigned int index) ;
    void SetTB_ITEM_SETITEM(unsigned int index, const TB_ITEM_SETITEM& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBItemSetItemDB() ;
    std::unordered_map<unsigned int, TB_ITEM_SETITEM> itemSetItemRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_ITEM_SETITEM* XResourceMgr::GetTB_ITEM_SETITEM(unsigned int index) {
        auto it = itemSetItemRows_.find(index);
        if (it == itemSetItemRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_ITEM_SETITEM(unsigned int index, const TB_ITEM_SETITEM& row) {
        itemSetItemRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBItemSetItemDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [SetItem_ID], [Set_Name_ID], [Set_Count], [SetItem_01], [SetItem_02], [SetItem_03], [SetItem_04], [SetItem_05], [SetItem_06], [Group_01], [Set01_Class_1], [Set01_Type1], [Set01_Value1], [Set01_Class_2], [Set01_Type2], [Set01_Value2], [Set01_Class_3], [Set01_Type3], [Set01_Value3], [Set01_Class_4], [Set01_Type4], [Set01_Value4], [Group_02], [Set02_Class_1], [Set02_Type1], [Set02_Value1], [Set02_Class_2], [Set02_Type2], [Set02_Value2], [Set02_Class_3], [Set02_Type3], [Set02_Value3], [Set02_Class_4], [Set02_Type4], [Set02_Value4], [Group_03], [Set03_Class_1], [Set03_Type1], [Set03_Value1], [Set03_Class_2], [Set03_Type2], [Set03_Value2], [Set03_Class_3], [Set03_Type3], [Set03_Value3], [Set03_Class_4], [Set03_Type4], [Set03_Value4], [Group_04], [Set04_Class_1], [Set04_Type1], [Set04_Value1], [Set04_Class_2], [Set04_Type2], [Set04_Value2], [Set04_Class_3], [Set04_Type3], [Set04_Value3], [Set04_Class_4], [Set04_Type4], [Set04_Value4], [Group_05], [Set05_Class_1], [Set05_Type1], [Set05_Value1], [Set05_Class_2], [Set05_Type2], [Set05_Value2], [Set05_Class_3], [Set05_Type3], [Set05_Value3], [Set05_Class_4], [Set05_Type4], [Set05_Value4] from tb_Item_SetItem")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        itemSetItemRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_ITEM_SETITEM row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.SetItem_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Set_Name_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Set_Count, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 6; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.SetItem_IDs[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            for (int group = 0; group < 5; ++group) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group[group], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
                for (int option = 0; option < 4; ++option) {
                    if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.SetOptionClass[group][option], &executeResult) ||
                        !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SetOptionType[group][option], &executeResult) ||
                        !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SetOptionValue[group][option], &executeResult)) {
                        xDBBinder.Close();
                        return executeResult;
                    }
                }
            }
            SetTB_ITEM_SETITEM(row.SetItem_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
