// 中文说明：
// 1. 本文件承接 TB_ITEM_PACKAGE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_ITEM_PACKAGE {
    unsigned int Package_ID = 0;
    unsigned int P_Gold = 0;
    unsigned int P_BP = 0;
    unsigned int P_Ether = 0;
    unsigned int Item_ID[15] = {};
    std::uint8_t Unpacking_Function_Type[15] = {};
    unsigned int Count[15] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_ITEM_PACKAGE) == 0x97, "TB_ITEM_PACKAGE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_ITEM_PACKAGE* GetTB_ITEM_PACKAGE(unsigned int index) ;
    void SetTB_ITEM_PACKAGE(unsigned int index, const TB_ITEM_PACKAGE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBItemPackageDB() ;
    std::unordered_map<unsigned int, TB_ITEM_PACKAGE> itemPackageRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_ITEM_PACKAGE* XResourceMgr::GetTB_ITEM_PACKAGE(unsigned int index) {
        auto it = itemPackageRows_.find(index);
        if (it == itemPackageRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_ITEM_PACKAGE(unsigned int index, const TB_ITEM_PACKAGE& row) {
        itemPackageRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBItemPackageDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Package_ID], [P_Gold], [P_BP], [P_Ether], [Item_01], [Unpacking_Function_Type_01], [Count_01], [Item_02], [Unpacking_Function_Type_02], [Count_02], [Item_03], [Unpacking_Function_Type_03], [Count_03], [Item_04], [Unpacking_Function_Type_04], [Count_04], [Item_05], [Unpacking_Function_Type_05], [Count_05], [Item_06], [Unpacking_Function_Type_06], [Count_06], [Item_07], [Unpacking_Function_Type_07], [Count_07], [Item_08], [Unpacking_Function_Type_08], [Count_08], [Item_09], [Unpacking_Function_Type_09], [Count_09], [Item_10], [Unpacking_Function_Type_10], [Count_10], [Item_11], [Unpacking_Function_Type_11], [Count_11], [Item_12], [Unpacking_Function_Type_12], [Count_12], [Item_13], [Unpacking_Function_Type_13], [Count_13], [Item_14], [Unpacking_Function_Type_14], [Count_14], [Item_15], [Unpacking_Function_Type_15], [Count_15] from tb_Item_Package")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        itemPackageRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_ITEM_PACKAGE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Package_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.P_Gold, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.P_BP, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.P_Ether, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 15; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Unpacking_Function_Type[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Count[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_ITEM_PACKAGE(row.Package_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
