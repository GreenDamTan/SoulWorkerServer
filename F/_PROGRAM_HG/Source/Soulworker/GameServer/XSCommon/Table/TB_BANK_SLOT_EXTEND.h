// 中文说明：
// 1. 本文件承接 TB_BANK_SLOT_EXTEND 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_BANK_SLOT_EXTEND {
    std::uint8_t Extend_Index = 0;
    std::uint8_t Extend_Step = 0;
    std::uint8_t Bank_Lv_1 = 0;
    std::uint8_t Extend_Need_1 = 0;
    unsigned int Extend_Value_1 = 0;
    std::uint8_t Add_Slot_1 = 0;
    std::uint16_t Accrue_Slot_1 = 0;
    std::uint8_t Bank_Lv_2 = 0;
    std::uint8_t Extend_Need_2 = 0;
    unsigned int Extend_Value_2 = 0;
    std::uint8_t Add_Slot_2 = 0;
    std::uint16_t Accrue_Slot_2 = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_BANK_SLOT_EXTEND) == 0x14, "TB_BANK_SLOT_EXTEND size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_BANK_SLOT_EXTEND* GetTB_BANK_SLOT_EXTEND(std::uint8_t index) ;
    void SetTB_BANK_SLOT_EXTEND(std::uint8_t index, const TB_BANK_SLOT_EXTEND& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBBankSlotExtendDB() ;
    std::map<std::uint8_t, TB_BANK_SLOT_EXTEND> m_mapTB_BANK_SLOT_EXTEND;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_BANK_SLOT_EXTEND* XResourceMgr::GetTB_BANK_SLOT_EXTEND(std::uint8_t index) {
        const auto it = m_mapTB_BANK_SLOT_EXTEND.find(index);
        return it == m_mapTB_BANK_SLOT_EXTEND.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_BANK_SLOT_EXTEND(std::uint8_t index, const TB_BANK_SLOT_EXTEND& row) {
        m_mapTB_BANK_SLOT_EXTEND[index] = row;
    }

std::int64_t XResourceMgr::LoadTBBankSlotExtendDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Extend_Index], [Extend_Step], [Bank_Lv_1], [Extend_Need_1], [Extend_Value_1], [Add_Slot_1], [Accrue_Slot_1], [Bank_Lv_2], [Extend_Need_2], [Extend_Value_2], [Add_Slot_2], [Accrue_Slot_2] from tb_Bank_Slot_Extend")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_BANK_SLOT_EXTEND.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_BANK_SLOT_EXTEND row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Extend_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Extend_Step, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Bank_Lv_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Extend_Need_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Extend_Value_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Add_Slot_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Accrue_Slot_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Bank_Lv_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Extend_Need_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Extend_Value_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Add_Slot_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Accrue_Slot_2, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_BANK_SLOT_EXTEND(row.Extend_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
