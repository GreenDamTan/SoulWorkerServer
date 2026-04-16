// 中文说明：
// 1. 本文件承接 TB_CASHSHOP_TAB 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_CASHSHOP_TAB {
    std::uint16_t MainTab_ID_INFO = 0;
    std::uint16_t MainTab_ORDER = 0;
    std::uint16_t SubTab_ID_INFO_01 = 0;
    std::uint16_t SubTab_ORDER_01 = 0;
    std::uint16_t SubTab_ID_INFO_02 = 0;
    std::uint16_t SubTab_ORDER_02 = 0;
    std::uint16_t SubTab_ID_INFO_03 = 0;
    std::uint16_t SubTab_ORDER_03 = 0;
    std::uint16_t SubTab_ID_INFO_04 = 0;
    std::uint16_t SubTab_ORDER_04 = 0;
    std::uint16_t SubTab_ID_INFO_05 = 0;
    std::uint16_t SubTab_ORDER_05 = 0;
    std::uint16_t SubTab_ID_INFO_06 = 0;
    std::uint16_t SubTab_ORDER_06 = 0;
    std::uint16_t SubTab_ID_INFO_07 = 0;
    std::uint16_t SubTab_ORDER_07 = 0;
    std::uint16_t SubTab_ID_INFO_08 = 0;
    std::uint16_t SubTab_ORDER_08 = 0;
    std::uint16_t SubTab_ID_INFO_09 = 0;
    std::uint16_t SubTab_ORDER_09 = 0;
    std::uint16_t SubTab_ID_INFO_10 = 0;
    std::uint16_t SubTab_ORDER_10 = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_CASHSHOP_TAB) == 0x2C, "TB_CASHSHOP_TAB size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_CASHSHOP_TAB* GetTB_CASHSHOP_TAB(std::uint16_t index) ;
    void SetTB_CASHSHOP_TAB(std::uint16_t index, const TB_CASHSHOP_TAB& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBCashShopTabDB() ;
    std::map<std::uint16_t, TB_CASHSHOP_TAB> m_mapTB_CASHSHOP_TAB;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_CASHSHOP_TAB* XResourceMgr::GetTB_CASHSHOP_TAB(std::uint16_t index) {
        const auto it = m_mapTB_CASHSHOP_TAB.find(index);
        return it == m_mapTB_CASHSHOP_TAB.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_CASHSHOP_TAB(std::uint16_t index, const TB_CASHSHOP_TAB& row) {
        m_mapTB_CASHSHOP_TAB[index] = row;
    }

std::int64_t XResourceMgr::LoadTBCashShopTabDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [MainTab_ID_INFO], [MainTab_ORDER], [SubTab_ID_INFO_01], [SubTab_ORDER_01], [SubTab_ID_INFO_02], [SubTab_ORDER_02], [SubTab_ID_INFO_03], [SubTab_ORDER_03], [SubTab_ID_INFO_04], [SubTab_ORDER_04], [SubTab_ID_INFO_05], [SubTab_ORDER_05], [SubTab_ID_INFO_06], [SubTab_ORDER_06], [SubTab_ID_INFO_07], [SubTab_ORDER_07], [SubTab_ID_INFO_08], [SubTab_ORDER_08], [SubTab_ID_INFO_09], [SubTab_ORDER_09], [SubTab_ID_INFO_10], [SubTab_ORDER_10] from tb_CashShop_Tab")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_CASHSHOP_TAB.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_CASHSHOP_TAB row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.MainTab_ID_INFO, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.MainTab_ORDER, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubTab_ID_INFO_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubTab_ORDER_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubTab_ID_INFO_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubTab_ORDER_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubTab_ID_INFO_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubTab_ORDER_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubTab_ID_INFO_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubTab_ORDER_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubTab_ID_INFO_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubTab_ORDER_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubTab_ID_INFO_06, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubTab_ORDER_06, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubTab_ID_INFO_07, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubTab_ORDER_07, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubTab_ID_INFO_08, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubTab_ORDER_08, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubTab_ID_INFO_09, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubTab_ORDER_09, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubTab_ID_INFO_10, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubTab_ORDER_10, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_CASHSHOP_TAB(row.MainTab_ID_INFO, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
