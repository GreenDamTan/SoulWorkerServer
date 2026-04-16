// 中文说明：
// 1. 本文件承接 TB_MODE_CARDMATCH_CARD 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
struct TB_MODE_CARDMATCH_CARD {
    unsigned int Card_ID = 0;
};
static_assert(sizeof(TB_MODE_CARDMATCH_CARD) == 0x4, "TB_MODE_CARDMATCH_CARD size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MODE_CARDMATCH_CARD* GetTB_MODE_CARDMATCH_CARD(unsigned int index) ;
    void SetTB_MODE_CARDMATCH_CARD(unsigned int index, const TB_MODE_CARDMATCH_CARD& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBModeCardMatchCardDB() ;
    std::map<unsigned int, TB_MODE_CARDMATCH_CARD> m_mapTB_MODE_CARDMATCH_CARD;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MODE_CARDMATCH_CARD* XResourceMgr::GetTB_MODE_CARDMATCH_CARD(unsigned int index) {
        const auto it = m_mapTB_MODE_CARDMATCH_CARD.find(index);
        return it == m_mapTB_MODE_CARDMATCH_CARD.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MODE_CARDMATCH_CARD(unsigned int index, const TB_MODE_CARDMATCH_CARD& row) {
        m_mapTB_MODE_CARDMATCH_CARD[index] = row;
    }

std::int64_t XResourceMgr::LoadTBModeCardMatchCardDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Card_ID] from tb_Mode_CardMatch_Card")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MODE_CARDMATCH_CARD.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MODE_CARDMATCH_CARD row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Card_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MODE_CARDMATCH_CARD(row.Card_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
