// 中文说明：
// 1. 本文件承接 TB_INTERACTION_ITEM 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_INTERACTION_ITEM {
    unsigned int ID = 0;
    std::uint16_t uniItem_Rate[10] = {};
    unsigned int uniItem_ID[10] = {};
    std::uint16_t uniItem_Value_Min[10] = {};
    std::uint16_t uniItem_Value_Max[10] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_INTERACTION_ITEM) == 0x68, "TB_INTERACTION_ITEM size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_INTERACTION_ITEM* GetTB_INTERACTION_ITEM(unsigned int index) ;
    void SetTB_INTERACTION_ITEM(unsigned int index, const TB_INTERACTION_ITEM& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBInteractionItemDB() ;
    std::unordered_map<unsigned int, TB_INTERACTION_ITEM> interactionItemRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_INTERACTION_ITEM* XResourceMgr::GetTB_INTERACTION_ITEM(unsigned int index) {
        auto it = interactionItemRows_.find(index);
        if (it == interactionItemRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_INTERACTION_ITEM(unsigned int index, const TB_INTERACTION_ITEM& row) {
        interactionItemRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBInteractionItemDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Item_Rate_01], [Item_ID_01], [Item_Value_Min_01], [Item_Value_Max_01], [Item_Rate_02], [Item_ID_02], [Item_Value_Min_02], [Item_Value_Max_02], [Item_Rate_03], [Item_ID_03], [Item_Value_Min_03], [Item_Value_Max_03], [Item_Rate_04], [Item_ID_04], [Item_Value_Min_04], [Item_Value_Max_04], [Item_Rate_05], [Item_ID_05], [Item_Value_Min_05], [Item_Value_Max_05], [Item_Rate_06], [Item_ID_06], [Item_Value_Min_06], [Item_Value_Max_06], [Item_Rate_07], [Item_ID_07], [Item_Value_Min_07], [Item_Value_Max_07], [Item_Rate_08], [Item_ID_08], [Item_Value_Min_08], [Item_Value_Max_08], [Item_Rate_09], [Item_ID_09], [Item_Value_Min_09], [Item_Value_Max_09], [Item_Rate_10], [Item_ID_10], [Item_Value_Min_10], [Item_Value_Max_10] from tb_Interaction_Item")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        interactionItemRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_INTERACTION_ITEM row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 10; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItem_Rate[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItem_ID[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItem_Value_Min[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItem_Value_Max[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_INTERACTION_ITEM(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
