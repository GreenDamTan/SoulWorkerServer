// 中文说明：
// 1. 本文件承接 TB_RANK_CHANGE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_RANK_CHANGE {
    unsigned int Smelting_ID = 0;
    std::int16_t uniChange_Rank_Rate[5] = {};
    std::int16_t Zeny_rate = 0;
    std::int16_t ether_rate = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_RANK_CHANGE) == 0x12, "TB_RANK_CHANGE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_RANK_CHANGE* GetTB_RANK_CHANGE(unsigned int index) ;
    void SetTB_RANK_CHANGE(unsigned int index, const TB_RANK_CHANGE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBRankChangeDB() ;
    std::map<unsigned int, TB_RANK_CHANGE> m_mapTB_RANK_CHANGE;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_RANK_CHANGE* XResourceMgr::GetTB_RANK_CHANGE(unsigned int index) {
        const auto it = m_mapTB_RANK_CHANGE.find(index);
        return it == m_mapTB_RANK_CHANGE.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_RANK_CHANGE(unsigned int index, const TB_RANK_CHANGE& row) {
        m_mapTB_RANK_CHANGE[index] = row;
    }

std::int64_t XResourceMgr::LoadTBRankChangeDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Smelting_ID], [Rank_Rate_1], [Rank_Rate_2], [Rank_Rate_3], [Rank_Rate_4], [Rank_Rate_5], [Zeny_rate], [ether_rate] from tb_Rank_Change")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_RANK_CHANGE.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_RANK_CHANGE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Smelting_ID, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.uniChange_Rank_Rate[0], &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.uniChange_Rank_Rate[1], &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.uniChange_Rank_Rate[2], &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.uniChange_Rank_Rate[3], &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.uniChange_Rank_Rate[4], &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Zeny_rate, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.ether_rate, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_RANK_CHANGE(row.Smelting_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
