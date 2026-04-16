// 中文说明：
// 1. 本文件承接 TB_ITEM_EVOLUTION 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_ITEM_EVOLUTION {
    unsigned int Evolution_ID = 0;
    std::uint8_t Result_Sum = 0;
    std::uint16_t Success_Rate = 0;
    unsigned int Need_Item = 0;
    unsigned int Item_Count = 0;
    unsigned int Need_Ether = 0;
    unsigned int Need_Gold = 0;
    unsigned int Need_BP = 0;
    unsigned int Item_ID[6] = {};
    std::uint16_t Rate[6] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_ITEM_EVOLUTION) == 0x3F, "TB_ITEM_EVOLUTION size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_ITEM_EVOLUTION* GetTB_ITEM_EVOLUTION(unsigned int index) ;
    void SetTB_ITEM_EVOLUTION(unsigned int index, const TB_ITEM_EVOLUTION& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBItemEvolutionDB() ;
    std::unordered_map<unsigned int, TB_ITEM_EVOLUTION> itemEvolutionRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_ITEM_EVOLUTION* XResourceMgr::GetTB_ITEM_EVOLUTION(unsigned int index) {
        auto it = itemEvolutionRows_.find(index);
        if (it == itemEvolutionRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_ITEM_EVOLUTION(unsigned int index, const TB_ITEM_EVOLUTION& row) {
        itemEvolutionRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBItemEvolutionDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Evolution_ID], [Result_Sum], [Success_Rate], [Need_Item], [Item_Count], [Need_Ether], [Need_Gold], [Need_BP], [Item_01], [Rate_01], [Item_02], [Rate_02], [Item_03], [Rate_03], [Item_04], [Rate_04], [Item_05], [Rate_05], [Item_06], [Rate_06] from tb_Item_Evolution")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        itemEvolutionRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_ITEM_EVOLUTION row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Evolution_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Result_Sum, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Success_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_Item, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_Ether, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_Gold, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_BP, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 6; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Rate[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_ITEM_EVOLUTION(row.Evolution_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
