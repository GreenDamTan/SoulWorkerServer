// 中文说明：
// 1. 本文件承接 TB_CASHSHOP 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_CASHSHOP {
    unsigned int Shop_Index = 0;
    std::uint16_t MainTab_ID = 0;
    std::uint16_t SubTab_ID = 0;
    std::uint8_t Sell_Active = 0;
    unsigned int SellItem_ID = 0;
    std::uint8_t Need_Slot = 0;
    std::uint8_t Sell_Type = 0;
    std::uint8_t Item_Gift = 0;
    unsigned int Sell_priority = 0;
    std::uint8_t Limit_Type = 0;
    char Limit_Start_Date[511] = {};
    char Limit_End_Date[511] = {};
    std::uint8_t Sell_Number_Type = 0;
    std::uint16_t Sell_Number_Value = 0;
    std::uint8_t Sell_Level = 0;
    std::uint16_t BillingInfo_ID = 0;
    std::uint16_t Mileage_ID = 0;
    std::uint8_t Hot_Icon = 0;
    std::uint8_t New_Icon = 0;
    std::uint8_t Sale_Icon = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_CASHSHOP) == 0x41E, "TB_CASHSHOP size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_CASHSHOP* GetTB_CASHSHOP(unsigned int index) ;
    void SetTB_CASHSHOP(unsigned int index, const TB_CASHSHOP& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBCashShopDB() ;
    std::map<unsigned int, TB_CASHSHOP> m_mapTB_CASHSHOP;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_CASHSHOP* XResourceMgr::GetTB_CASHSHOP(unsigned int index) {
        const auto it = m_mapTB_CASHSHOP.find(index);
        return it == m_mapTB_CASHSHOP.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_CASHSHOP(unsigned int index, const TB_CASHSHOP& row) {
        m_mapTB_CASHSHOP[index] = row;
    }

std::int64_t XResourceMgr::LoadTBCashShopDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Shop_Index], [MainTab_ID], [SubTab_ID], [Sell_Active], [SellItem_ID], [Need_Slot], [Sell_Type], [Item_Gift], [Sell_priority], [Limit_Type], [Limit_Start_Date], [Limit_End_Date], [Sell_Number_Type], [Sell_Number_Value], [Sell_Level], [BillingInfo_ID], [Mileage_ID], [Hot_Icon], [New_Icon], [Sale_Icon] from tb_CashShop")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_CASHSHOP.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_CASHSHOP row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Shop_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.MainTab_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SubTab_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sell_Active, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SellItem_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_Slot, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sell_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Gift, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sell_priority, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Limit_Type, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Limit_Start_Date, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Limit_End_Date, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sell_Number_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sell_Number_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sell_Level, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BillingInfo_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Mileage_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Hot_Icon, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.New_Icon, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sale_Icon, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_CASHSHOP(row.Shop_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
