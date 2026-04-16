// 中文说明：
// 1. 本文件承接 TB_RANDOM_CHANGE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_RANDOM_CHANGE {
    unsigned int Option_Change_ID = 0;
    std::uint8_t Max_Change_Cnt = 0;
    unsigned int Need_Item = 0;
    std::uint8_t uniNeeditem_Cnt[5] = {};
    std::uint16_t Need_BP = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_RANDOM_CHANGE) == 0x10, "TB_RANDOM_CHANGE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_RANDOM_CHANGE* GetTB_RANDOM_CHANGE(unsigned int index) ;
    void SetTB_RANDOM_CHANGE(unsigned int index, const TB_RANDOM_CHANGE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBRandomChangeDB() ;
    std::map<unsigned int, TB_RANDOM_CHANGE> m_mapTB_RANDOM_CHANGE;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_RANDOM_CHANGE* XResourceMgr::GetTB_RANDOM_CHANGE(unsigned int index) {
        const auto it = m_mapTB_RANDOM_CHANGE.find(index);
        return it == m_mapTB_RANDOM_CHANGE.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_RANDOM_CHANGE(unsigned int index, const TB_RANDOM_CHANGE& row) {
        m_mapTB_RANDOM_CHANGE[index] = row;
    }

std::int64_t XResourceMgr::LoadTBRandomChangeDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Option_Change_ID], [Max_Change_Cnt], [Need_Item], [Needitem_Cnt1], [Needitem_Cnt2], [Needitem_Cnt3], [Needitem_Cnt4], [Needitem_Cnt5], [Need_BP] from tb_Random_Change")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_RANDOM_CHANGE.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_RANDOM_CHANGE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Option_Change_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Max_Change_Cnt, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_Item, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (auto& count : row.uniNeeditem_Cnt) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &count, &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_BP, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_RANDOM_CHANGE(row.Option_Change_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
