// 中文说明：
// 1. 本文件承接 TB_ECHELON 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_ECHELON {
    std::uint8_t Echelon_Index = 0;
    std::uint16_t Echelon_Name = 0;
    std::uint16_t Echelon_Tooltip = 0;
    char Echelon_Icon[511] = {};
    char BonusEff_Icon[511] = {};
    unsigned int Echelon_Exp = 0;
    std::uint16_t Echelon_Booster_ID = 0;
    std::uint8_t Echelon_Invalid = 0;
    unsigned int Echelon_Title = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_ECHELON) == 0x40E, "TB_ECHELON size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_ECHELON* GetTB_ECHELON(std::uint8_t index) ;
    void SetTB_ECHELON(std::uint8_t index, const TB_ECHELON& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBEchelonDB() ;
    std::unordered_map<std::uint8_t, TB_ECHELON> echelonRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_ECHELON* XResourceMgr::GetTB_ECHELON(std::uint8_t index) {
        auto it = echelonRows_.find(index);
        if (it == echelonRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_ECHELON(std::uint8_t index, const TB_ECHELON& row) {
        echelonRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBEchelonDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Echelon_Index], [Echelon_Name], [Echelon_Tooltip], [Echelon_Icon], [BonusEff_Icon], [Echelon_Exp], [Echelon_Booster_ID], [Echelon_Invalid], [Echelon_Title] from tb_Echelon")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        echelonRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_ECHELON row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Echelon_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Echelon_Name, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Echelon_Tooltip, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Echelon_Icon, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.BonusEff_Icon, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Echelon_Exp, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Echelon_Booster_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Echelon_Invalid, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Echelon_Title, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_ECHELON(row.Echelon_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
