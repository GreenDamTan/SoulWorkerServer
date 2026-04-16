// 中文说明：
// 1. 本文件承接 TB_ITEM_RANK_RATE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_ITEM_RANK_RATE {
    std::uint8_t Item_Rank_Value = 0;
    float Item_Rank_Rate = 0.0f;
};
#pragma pack(pop)
static_assert(sizeof(TB_ITEM_RANK_RATE) == 0x5, "TB_ITEM_RANK_RATE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_ITEM_RANK_RATE* GetTB_ITEM_RANK_RATE(std::uint8_t index) ;
    void SetTB_ITEM_RANK_RATE(std::uint8_t index, const TB_ITEM_RANK_RATE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBItemRankRateDB() ;
    std::unordered_map<std::uint8_t, TB_ITEM_RANK_RATE> itemRankRateRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_ITEM_RANK_RATE* XResourceMgr::GetTB_ITEM_RANK_RATE(std::uint8_t index) {
        auto it = itemRankRateRows_.find(index);
        if (it == itemRankRateRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_ITEM_RANK_RATE(std::uint8_t index, const TB_ITEM_RANK_RATE& row) {
        itemRankRateRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBItemRankRateDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Item_Rank_Value], [Item_Rank_Rate] from tb_Item_Rank_Rate")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        itemRankRateRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_ITEM_RANK_RATE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Rank_Value, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Item_Rank_Rate, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_ITEM_RANK_RATE(row.Item_Rank_Value, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
