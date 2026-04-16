// 中文说明：
// 1. 本文件承接 TB_CASHBILLING_INFO 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_CASHBILLING_INFO {
    std::uint16_t ID = 0;
    char Billing_Code_1st[511] = {};
    unsigned int uniItem[5] = {};
    unsigned int univalue[5] = {};
    unsigned int uniB_Price[5] = {};
    unsigned int uniS_Price[5] = {};
    char Billing_Code_2st[511] = {};
    char Billing_Code_3st[511] = {};
    char Billing_Code_4st[511] = {};
    char Billing_Code_5st[511] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_CASHBILLING_INFO) == 0xA4D, "TB_CASHBILLING_INFO size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_CASHBILLING_INFO* GetTB_CASHBILLING_INFO(std::uint16_t index) ;
    void SetTB_CASHBILLING_INFO(std::uint16_t index, const TB_CASHBILLING_INFO& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBCashBillingInfoDB() ;
    std::map<std::uint16_t, TB_CASHBILLING_INFO> m_mapTB_CASHBILLING_INFO;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_CASHBILLING_INFO* XResourceMgr::GetTB_CASHBILLING_INFO(std::uint16_t index) {
        const auto it = m_mapTB_CASHBILLING_INFO.find(index);
        return it == m_mapTB_CASHBILLING_INFO.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_CASHBILLING_INFO(std::uint16_t index, const TB_CASHBILLING_INFO& row) {
        m_mapTB_CASHBILLING_INFO[index] = row;
    }

std::int64_t XResourceMgr::LoadTBCashBillingInfoDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Billing_Code_1st], [Item_ID_1st], [Value_1st], [B_Price_1st], [S_Price_1st], [Billing_Code_2st], [Item_ID_2st], [Value_2nd], [B_Price_2nd], [S_Price_2nd], [Billing_Code_3st], [Item_ID_3st], [Value_3rd], [B_Price_3rd], [S_Price_3rd], [Billing_Code_4st], [Item_ID_4st], [Value_4th], [B_Price_4th], [S_Price_4th], [Billing_Code_5st], [Item_ID_5st], [Value_5th], [B_Price_5th], [S_Price_5th] from tb_CashBilling_Info")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_CASHBILLING_INFO.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_CASHBILLING_INFO row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Billing_Code_1st, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItem[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.univalue[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniB_Price[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniS_Price[0], &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Billing_Code_2st, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItem[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.univalue[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniB_Price[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniS_Price[1], &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Billing_Code_3st, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItem[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.univalue[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniB_Price[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniS_Price[2], &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Billing_Code_4st, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItem[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.univalue[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniB_Price[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniS_Price[3], &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Billing_Code_5st, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItem[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.univalue[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniB_Price[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniS_Price[4], &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_CASHBILLING_INFO(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
