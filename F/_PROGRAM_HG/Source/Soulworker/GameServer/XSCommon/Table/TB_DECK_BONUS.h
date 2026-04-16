// 中文说明：
// 1. 本文件承接 TB_DECK_BONUS 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_DECK_BONUS {
    std::uint16_t Bonus_Index = 0;
    std::uint8_t Use_DeckPhase = 0;
    std::uint8_t Bonus_Type = 0;
    std::uint8_t Bonus_Need_Lv = 0;
    float Bonus_Value = 0.0f;
};
#pragma pack(pop)
static_assert(sizeof(TB_DECK_BONUS) == 0x9, "TB_DECK_BONUS size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_DECK_BONUS* GetTB_DECK_BONUS(std::uint16_t index) ;
    void SetTB_DECK_BONUS(std::uint16_t index, const TB_DECK_BONUS& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBDeckBonusDB() ;
    std::unordered_map<std::uint16_t, TB_DECK_BONUS> deckBonusRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_DECK_BONUS* XResourceMgr::GetTB_DECK_BONUS(std::uint16_t index) {
        auto it = deckBonusRows_.find(index);
        if (it == deckBonusRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_DECK_BONUS(std::uint16_t index, const TB_DECK_BONUS& row) {
        deckBonusRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBDeckBonusDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Bonus_Index], [Use_DeckPhase], [Bonus_Type], [Bonus_Need_Lv], [Bonus_Value] from tb_Deck_Bonus")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        deckBonusRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_DECK_BONUS row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Bonus_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Use_DeckPhase, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Bonus_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Bonus_Need_Lv, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Bonus_Value, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_DECK_BONUS(row.Bonus_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
