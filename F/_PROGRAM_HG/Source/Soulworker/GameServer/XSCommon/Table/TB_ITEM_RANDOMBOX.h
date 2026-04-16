// 中文说明：
// 1. 本文件承接 TB_ITEM_RANDOMBOX 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_ITEM_RANDOMBOX {
    unsigned int RandomBox_ID = 0;
    std::uint8_t Give_Count = 0;
    unsigned int Rate_Sum = 0;
    unsigned int RB_Gold = 0;
    unsigned int Rate[9] = {};
    unsigned int RB_BP = 0;
    unsigned int RB_Ether = 0;
    unsigned int Item_ID[15] = {};
    unsigned int Count[15] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_ITEM_RANDOMBOX) == 0xB1, "TB_ITEM_RANDOMBOX size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_ITEM_RANDOMBOX* GetTB_ITEM_RANDOMBOX(unsigned int index) ;
    void SetTB_ITEM_RANDOMBOX(unsigned int index, const TB_ITEM_RANDOMBOX& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBItemRandomBoxDB() ;
    std::unordered_map<unsigned int, TB_ITEM_RANDOMBOX> itemRandomBoxRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_ITEM_RANDOMBOX* XResourceMgr::GetTB_ITEM_RANDOMBOX(unsigned int index) {
        auto it = itemRandomBoxRows_.find(index);
        if (it == itemRandomBoxRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_ITEM_RANDOMBOX(unsigned int index, const TB_ITEM_RANDOMBOX& row) {
        itemRandomBoxRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBItemRandomBoxDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [RandomBox_ID], [Give_Count], [Rate_Sum], [RB_Gold], [Rate_01], [Rate_02], [Rate_03], [Rate_04], [Rate_05], [Rate_06], [Rate_07], [Rate_08], [Rate_09], [RB_BP], [RB_Ether], [Item_01], [Count_01], [Item_02], [Count_02], [Item_03], [Count_03], [Item_04], [Count_04], [Item_05], [Count_05], [Item_06], [Count_06], [Item_07], [Count_07], [Item_08], [Count_08], [Item_09], [Count_09], [Item_10], [Count_10], [Item_11], [Count_11], [Item_12], [Count_12], [Item_13], [Count_13], [Item_14], [Count_14], [Item_15], [Count_15] from tb_Item_RandomBox")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        itemRandomBoxRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_ITEM_RANDOMBOX row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.RandomBox_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Give_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Rate_Sum, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.RB_Gold, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 9; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Rate[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.RB_BP, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.RB_Ether, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 15; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Count[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_ITEM_RANDOMBOX(row.RandomBox_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
