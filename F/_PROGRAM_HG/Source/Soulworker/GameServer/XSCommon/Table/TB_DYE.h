// 中文说明：
// 1. 本文件承接 TB_DYE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_DYE {
    std::uint16_t Dye_ID = 0;
    std::uint8_t Icon_Dye_Color_R = 0;
    std::uint8_t Icon_Dye_Color_G = 0;
    std::uint8_t Icon_Dye_Color_B = 0;
    std::uint8_t Hidden_Info = 0;
    char Color_Hex[511] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_DYE) == 0x205, "TB_DYE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_DYE* GetTB_DYE(std::uint16_t index) ;
    void SetTB_DYE(std::uint16_t index, const TB_DYE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBDyeDB() ;
    std::unordered_map<std::uint16_t, TB_DYE> dyeRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_DYE* XResourceMgr::GetTB_DYE(std::uint16_t index) {
        auto it = dyeRows_.find(index);
        if (it == dyeRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_DYE(std::uint16_t index, const TB_DYE& row) {
        dyeRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBDyeDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Dye_ID], [Icon_Dye_Color_R], [Icon_Dye_Color_G], [Icon_Dye_Color_B], [Hidden_Info], [Color_Hex] from tb_Dye")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        dyeRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_DYE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Dye_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Icon_Dye_Color_R, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Icon_Dye_Color_G, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Icon_Dye_Color_B, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Hidden_Info, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Color_Hex, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_DYE(row.Dye_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
