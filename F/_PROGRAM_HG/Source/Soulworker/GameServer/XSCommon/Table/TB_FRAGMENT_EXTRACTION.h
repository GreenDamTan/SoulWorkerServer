// 中文说明：
// 1. 本文件承接 TB_FRAGMENT_EXTRACTION 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_FRAGMENT_EXTRACTION {
    unsigned int Fragment_Extraction_ID = 0;
    unsigned int uniID[5] = {};
    std::uint16_t uniCount[5] = {};
    std::uint16_t uniRate1[5] = {};
    unsigned int uniBonus1[5] = {};
    std::uint16_t uniRate2[5] = {};
    unsigned int uniBonus2[5] = {};
    std::uint16_t uniRate3[5] = {};
    unsigned int uniBonus3[5] = {};
    std::uint16_t Extraction_ExtraBonus_Rate = 0;
    unsigned int Extraction_ExtraBonus_ItemID = 0;
    std::uint8_t Extraction_ExtraBonus_Count = 0;
    unsigned int Extraction_BonusItemID = 0;
    std::uint8_t Extraction_BonusItem_Count = 0;
    unsigned int Extraction_NeedJeny = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_FRAGMENT_EXTRACTION) == 0x8C, "TB_FRAGMENT_EXTRACTION size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_FRAGMENT_EXTRACTION* GetTB_FRAGMENT_EXTRACTION(unsigned int index) ;
    void SetTB_FRAGMENT_EXTRACTION(unsigned int index, const TB_FRAGMENT_EXTRACTION& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBFragmentExtractionDB() ;
    std::unordered_map<unsigned int, TB_FRAGMENT_EXTRACTION> fragmentExtractionRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_FRAGMENT_EXTRACTION* XResourceMgr::GetTB_FRAGMENT_EXTRACTION(unsigned int index) {
        auto it = fragmentExtractionRows_.find(index);
        if (it == fragmentExtractionRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_FRAGMENT_EXTRACTION(unsigned int index, const TB_FRAGMENT_EXTRACTION& row) {
        fragmentExtractionRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBFragmentExtractionDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Fragment_Extraction_ID], [Extraction01_ItemID_01], [Extraction01_ItemCount_01], [Extraction01_Rate_01], [Extraction01_Bonus_Count_01], [Extraction01_Rate_02], [Extraction01_Bonus_Count_02], [Extraction01_Rate_03], [Extraction01_Bonus_Count_03], [Extraction02_ItemID_02], [Extraction02_ItemCount_02], [Extraction02_Rate_01], [Extraction02_Bonus_Count_01], [Extraction02_Rate_02], [Extraction02_Bonus_Count_02], [Extraction02_Rate_03], [Extraction02_Bonus_Count_03], [Extraction03_ItemID_03], [Extraction03_ItemCount_03], [Extraction03_Rate_01], [Extraction03_Bonus_Count_01], [Extraction03_Rate_02], [Extraction03_Bonus_Count_02], [Extraction03_Rate_03], [Extraction03_Bonus_Count_03], [Extraction04_ItemID_04], [Extraction04_ItemCount_04], [Extraction04_Rate_01], [Extraction04_Bonus_Count_01], [Extraction04_Rate_02], [Extraction04_Bonus_Count_02], [Extraction04_Rate_03], [Extraction04_Bonus_Count_03], [Extraction05_ItemID_05], [Extraction05_ItemCount_05], [Extraction05_Rate_01], [Extraction05_Bonus_Count_01], [Extraction05_Rate_02], [Extraction05_Bonus_Count_02], [Extraction05_Rate_03], [Extraction05_Bonus_Count_03], [Extraction_ExtraBonus_Rate], [Extraction_ExtraBonus_ItemID], [Extraction_ExtraBonus_Count], [Extraction_BonusItemID], [Extraction_BonusItem_Count], [Extraction_NeedJeny] from tb_Fragment_Extraction")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        fragmentExtractionRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_FRAGMENT_EXTRACTION row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Fragment_Extraction_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniID[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCount[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniRate1[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBonus1[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniRate2[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBonus2[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniRate3[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBonus3[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniID[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCount[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniRate1[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBonus1[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniRate2[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBonus2[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniRate3[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBonus3[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniID[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCount[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniRate1[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBonus1[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniRate2[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBonus2[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniRate3[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBonus3[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniID[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCount[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniRate1[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBonus1[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniRate2[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBonus2[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniRate3[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBonus3[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniID[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCount[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniRate1[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBonus1[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniRate2[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBonus2[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniRate3[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBonus3[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Extraction_ExtraBonus_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Extraction_ExtraBonus_ItemID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Extraction_ExtraBonus_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Extraction_BonusItemID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Extraction_BonusItem_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Extraction_NeedJeny, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_FRAGMENT_EXTRACTION(row.Fragment_Extraction_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
