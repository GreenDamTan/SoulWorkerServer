// 中文说明：
// 1. 本文件承接 TB_SHOP 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_SHOP {
    unsigned int Shop_Index = 0;
    unsigned int Group_ID = 0;
    std::uint8_t Sell_Active = 0;
    unsigned int SellItem_ID = 0;
    std::uint8_t Need_Slot = 0;
    std::uint8_t Customer_Grade = 0;
    std::uint8_t Limit_Type = 0;
    std::uint8_t Period_Type = 0;
    std::uint8_t Day_Limit = 0;
    std::uint8_t Sell_Type = 0;
    char Start_Date[511] = {};
    char End_Date[511] = {};
    std::uint8_t Price_Type = 0;
    std::uint16_t Item_Count = 0;
    unsigned int Price_Base = 0;
    unsigned int Price_1day = 0;
    unsigned int Price_7day = 0;
    unsigned int Price_15day = 0;
    unsigned int Price_30day = 0;
    std::uint8_t Sell_AddIcon = 0;
    std::uint8_t Item_Gift = 0;
    unsigned int Sell_priority = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_SHOP) == 0x42E, "TB_SHOP size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_SHOP* GetTB_SHOP(unsigned int index) ;
    void SetTB_SHOP(unsigned int index, const TB_SHOP& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBShopDB() ;
    std::map<unsigned int, TB_SHOP> m_mapTB_SHOP;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_SHOP* XResourceMgr::GetTB_SHOP(unsigned int index) {
        const auto it = m_mapTB_SHOP.find(index);
        return it == m_mapTB_SHOP.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_SHOP(unsigned int index, const TB_SHOP& row) {
        m_mapTB_SHOP[index] = row;
    }

std::int64_t XResourceMgr::LoadTBShopDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Shop_Index], [Group_ID], [Sell_Active], [SellItem_ID], [Need_Slot], [Customer_Grade], [Limit_Type], [Period_Type], [Day_Limit], [Sell_Type], [Start_Date], [End_Date], [Price_Type], [Item_Count], [Price_Base], [Price_1day], [Price_7day], [Price_15day], [Price_30day], [Sell_AddIcon], [Item_Gift], [Sell_priority] from tb_Shop")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_SHOP.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_SHOP row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Shop_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sell_Active, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SellItem_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Need_Slot, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Customer_Grade, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Limit_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Period_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Day_Limit, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sell_Type, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Start_Date, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.End_Date, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Price_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Price_Base, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Price_1day, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Price_7day, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Price_15day, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Price_30day, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sell_AddIcon, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Gift, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sell_priority, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_SHOP(row.Shop_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
