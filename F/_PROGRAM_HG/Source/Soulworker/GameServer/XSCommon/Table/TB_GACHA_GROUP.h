// 中文说明：
// 1. 本文件承接 TB_GACHA_GROUP 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_GACHA_GROUP {
    std::uint16_t Gacha_Chance_ID = 0;
    char Des[511] = {};
    std::uint16_t Gacha_Group_ID = 0;
    std::uint8_t Gacha_Chance_On_Off = 0;
    std::uint16_t uniG_Chance[20] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_GACHA_GROUP) == 0x22C, "TB_GACHA_GROUP size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_GACHA_GROUP* GetTB_GACHA_GROUP(std::uint16_t index) ;
    void SetTB_GACHA_GROUP(std::uint16_t index, const TB_GACHA_GROUP& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBGachaGroupDB() ;
    std::unordered_map<std::uint16_t, TB_GACHA_GROUP> gachaGroupRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_GACHA_GROUP* XResourceMgr::GetTB_GACHA_GROUP(std::uint16_t index) {
        auto it = gachaGroupRows_.find(index);
        if (it == gachaGroupRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_GACHA_GROUP(std::uint16_t index, const TB_GACHA_GROUP& row) {
        gachaGroupRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBGachaGroupDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Gacha_Chance_ID], [Des], [Gacha_Group_ID], [Gacha_Chance_On_Off], [G_Chance_01], [G_Chance_02], [G_Chance_03], [G_Chance_04], [G_Chance_05], [G_Chance_06], [G_Chance_07], [G_Chance_08], [G_Chance_09], [G_Chance_10], [G_Chance_11], [G_Chance_12], [G_Chance_13], [G_Chance_14], [G_Chance_15], [G_Chance_16], [G_Chance_17], [G_Chance_18], [G_Chance_19], [G_Chance_20] from tb_Gacha_Group")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        gachaGroupRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_GACHA_GROUP row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Gacha_Chance_ID, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Des, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Gacha_Group_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Gacha_Chance_On_Off, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniG_Chance[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniG_Chance[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniG_Chance[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniG_Chance[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniG_Chance[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniG_Chance[5], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniG_Chance[6], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniG_Chance[7], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniG_Chance[8], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniG_Chance[9], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniG_Chance[10], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniG_Chance[11], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniG_Chance[12], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniG_Chance[13], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniG_Chance[14], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniG_Chance[15], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniG_Chance[16], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniG_Chance[17], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniG_Chance[18], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniG_Chance[19], &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_GACHA_GROUP(row.Gacha_Chance_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
