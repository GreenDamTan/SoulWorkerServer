// 中文说明：
// 1. 本文件承接 TB_DISASSEMBLE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_DISASSEMBLE {
    unsigned int DA_ID = 0;
    unsigned int MIN_Ethel = 0;
    unsigned int MAX_Ethel = 0;
    unsigned int DA_Item_01 = 0;
    unsigned int DA_Item_02 = 0;
    unsigned int DA_Item_03 = 0;
    unsigned int DA_Item_04 = 0;
    unsigned int DA_Item_05 = 0;
    unsigned int DA_Item_06 = 0;
    unsigned int DA_Item_07 = 0;
    std::uint16_t DA_Rate_01 = 0;
    std::uint16_t DA_Rate_02 = 0;
    std::uint16_t DA_Rate_03 = 0;
    std::uint16_t DA_Rate_04 = 0;
    std::uint16_t DA_Rate_05 = 0;
    std::uint16_t DA_Rate_06 = 0;
    std::uint16_t DA_Rate_07 = 0;
    std::uint8_t Min_Value_01 = 0;
    std::uint8_t Min_Value_02 = 0;
    std::uint8_t Min_Value_03 = 0;
    std::uint8_t Min_Value_04 = 0;
    std::uint8_t Min_Value_05 = 0;
    std::uint8_t Min_Value_06 = 0;
    std::uint8_t Min_Value_07 = 0;
    std::uint8_t Max_Value_01 = 0;
    std::uint8_t Max_Value_02 = 0;
    std::uint8_t Max_Value_03 = 0;
    std::uint8_t Max_Value_04 = 0;
    std::uint8_t Max_Value_05 = 0;
    std::uint8_t Max_Value_06 = 0;
    std::uint8_t Max_Value_07 = 0;

    std::array<unsigned int, 7> GetItemIDs() const {
        return {DA_Item_01, DA_Item_02, DA_Item_03, DA_Item_04, DA_Item_05, DA_Item_06, DA_Item_07};
    }

    std::array<std::uint16_t, 7> GetRates() const {
        return {DA_Rate_01, DA_Rate_02, DA_Rate_03, DA_Rate_04, DA_Rate_05, DA_Rate_06, DA_Rate_07};
    }

    std::array<std::uint8_t, 7> GetMinValues() const {
        return {Min_Value_01, Min_Value_02, Min_Value_03, Min_Value_04, Min_Value_05, Min_Value_06,
                Min_Value_07};
    }

    std::array<std::uint8_t, 7> GetMaxValues() const {
        return {Max_Value_01, Max_Value_02, Max_Value_03, Max_Value_04, Max_Value_05, Max_Value_06,
                Max_Value_07};
    }
};
#pragma pack(pop)
static_assert(sizeof(TB_DISASSEMBLE) == 0x44, "TB_DISASSEMBLE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_DISASSEMBLE* GetTB_DISASSEMBLE(unsigned int index) ;
    void SetTB_DISASSEMBLE(unsigned int index, const TB_DISASSEMBLE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBDisassembleDB() ;
    std::unordered_map<unsigned int, TB_DISASSEMBLE> disassembleRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_DISASSEMBLE* XResourceMgr::GetTB_DISASSEMBLE(unsigned int index) {
        auto it = disassembleRows_.find(index);
        if (it == disassembleRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_DISASSEMBLE(unsigned int index, const TB_DISASSEMBLE& row) {
        disassembleRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBDisassembleDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [DA_ID], [MIN_Ethel], [MAX_Ethel], [DA_Item_01], [DA_Rate_01], [Min_Value_01], [Max_Value_01], [DA_Item_02], [DA_Rate_02], [Min_Value_02], [Max_Value_02], [DA_Item_03], [DA_Rate_03], [Min_Value_03], [Max_Value_03], [DA_Item_04], [DA_Rate_04], [Min_Value_04], [Max_Value_04], [DA_Item_05], [DA_Rate_05], [Min_Value_05], [Max_Value_05], [DA_Item_06], [DA_Rate_06], [Min_Value_06], [Max_Value_06], [DA_Item_07], [DA_Rate_07], [Min_Value_07], [Max_Value_07] from tb_Disassemble")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        disassembleRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_DISASSEMBLE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.DA_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.MIN_Ethel, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.MAX_Ethel, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.DA_Item_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.DA_Rate_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Min_Value_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Max_Value_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.DA_Item_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.DA_Rate_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Min_Value_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Max_Value_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.DA_Item_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.DA_Rate_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Min_Value_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Max_Value_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.DA_Item_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.DA_Rate_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Min_Value_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Max_Value_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.DA_Item_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.DA_Rate_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Min_Value_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Max_Value_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.DA_Item_06, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.DA_Rate_06, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Min_Value_06, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Max_Value_06, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.DA_Item_07, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.DA_Rate_07, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Min_Value_07, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Max_Value_07, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_DISASSEMBLE(row.DA_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
