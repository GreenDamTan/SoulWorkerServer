// 中文说明：
// 1. 本文件承接 TB_MODE_CARDMATCH_RULE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
struct TB_MODE_CARDMATCH_RULE {
    unsigned int CardMatch_ID = 0;
    unsigned int Win_Reward = 0;
    unsigned int Lose_Reward = 0;
};
static_assert(sizeof(TB_MODE_CARDMATCH_RULE) == 0xC, "TB_MODE_CARDMATCH_RULE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MODE_CARDMATCH_RULE* GetTB_MODE_CARDMATCH_RULE(unsigned int index) ;
    void SetTB_MODE_CARDMATCH_RULE(unsigned int index, const TB_MODE_CARDMATCH_RULE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBModeCardMatchRuleDB() ;
    std::map<unsigned int, TB_MODE_CARDMATCH_RULE> m_mapTB_MODE_CARDMATCH_RULE;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MODE_CARDMATCH_RULE* XResourceMgr::GetTB_MODE_CARDMATCH_RULE(unsigned int index) {
        const auto it = m_mapTB_MODE_CARDMATCH_RULE.find(index);
        return it == m_mapTB_MODE_CARDMATCH_RULE.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MODE_CARDMATCH_RULE(unsigned int index, const TB_MODE_CARDMATCH_RULE& row) {
        m_mapTB_MODE_CARDMATCH_RULE[index] = row;
    }

std::int64_t XResourceMgr::LoadTBModeCardMatchRuleDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [CardMatch_ID], [Win_Reward], [Lose_Reward] from tb_Mode_CardMatch_Rule")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MODE_CARDMATCH_RULE.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MODE_CARDMATCH_RULE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.CardMatch_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Win_Reward, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Lose_Reward, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MODE_CARDMATCH_RULE(row.CardMatch_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
