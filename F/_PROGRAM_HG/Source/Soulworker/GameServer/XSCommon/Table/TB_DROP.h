// 中文说明：
// 1. 本文件承接 TB_DROP 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_DROP {
    unsigned int Drop_Index = 0;
    std::uint8_t Drop_Rate_Clear = 0;
    std::uint16_t Gold_Chance = 0;
    std::uint16_t Gold_Min = 0;
    std::uint16_t Gold_Max = 0;
    std::uint16_t Gold_Jp_Rate = 0;
    std::uint8_t Gold_Jp_Multi = 0;
    std::uint16_t SV_Chance = 0;
    std::uint16_t SV_Min = 0;
    std::uint16_t SV_Max = 0;
    std::uint16_t Et_Chance = 0;
    std::uint16_t Et_Min = 0;
    std::uint16_t Et_Max = 0;
    std::uint16_t Et_Unit = 0;
    std::uint16_t Et_Jp_Rate = 0;
    std::uint16_t Et_Jp_Multi = 0;
    std::uint16_t BP_Chance = 0;
    std::uint16_t BP_Max = 0;
    std::uint16_t BP_Min = 0;
    std::uint16_t G_Chance_01 = 0;
    std::uint16_t G_Chance_02 = 0;
    std::uint16_t G_Chance_03 = 0;
    std::uint16_t G_Chance_04 = 0;
    std::uint16_t G_Chance_05 = 0;
    std::uint16_t G_Chance_06 = 0;
    std::uint16_t G_Chance_07 = 0;
    std::uint16_t G_Chance_08 = 0;
    std::uint16_t G_Chance_09 = 0;
    std::uint16_t G_Chance_10 = 0;
    std::uint16_t G_Chance_11 = 0;
    std::uint16_t G_Chance_12 = 0;
    std::uint16_t G_Chance_13 = 0;
    std::uint16_t G_Chance_14 = 0;
    std::uint16_t G_Chance_15 = 0;
    std::uint16_t G_Chance_16 = 0;
    std::uint16_t G_Chance_17 = 0;
    std::uint16_t G_Chance_18 = 0;
    std::uint16_t G_Chance_19 = 0;
    std::uint16_t G_Chance_20 = 0;
    std::uint16_t G_Chance_21 = 0;
    unsigned int Group_ID_01 = 0;
    unsigned int Group_ID_02 = 0;
    unsigned int Group_ID_03 = 0;
    unsigned int Group_ID_04 = 0;
    unsigned int Group_ID_05 = 0;
    unsigned int Group_ID_06 = 0;
    unsigned int Group_ID_07 = 0;
    unsigned int Group_ID_08 = 0;
    unsigned int Group_ID_09 = 0;
    unsigned int Group_ID_10 = 0;
    unsigned int Group_ID_11 = 0;
    unsigned int Group_ID_12 = 0;
    unsigned int Group_ID_13 = 0;
    unsigned int Group_ID_14 = 0;
    unsigned int Group_ID_15 = 0;
    unsigned int Group_ID_16 = 0;
    unsigned int Group_ID_17 = 0;
    unsigned int Group_ID_18 = 0;
    unsigned int Group_ID_19 = 0;
    unsigned int Group_ID_20 = 0;
    unsigned int Group_ID_21 = 0;
    std::uint16_t C_Chance_01 = 0;
    std::uint16_t C_Chance_02 = 0;
    std::uint16_t C_Chance_03 = 0;
    std::uint16_t C_Chance_04 = 0;
    std::uint16_t C_Chance_05 = 0;
    std::uint16_t C_Chance_06 = 0;
    std::uint16_t C_Chance_07 = 0;
    std::uint16_t C_Chance_08 = 0;
    std::uint16_t C_Chance_09 = 0;
    std::uint16_t C_Chance_10 = 0;
    unsigned int C_Group_ID_01 = 0;
    unsigned int C_Group_ID_02 = 0;
    unsigned int C_Group_ID_03 = 0;
    unsigned int C_Group_ID_04 = 0;
    unsigned int C_Group_ID_05 = 0;
    unsigned int C_Group_ID_06 = 0;
    unsigned int C_Group_ID_07 = 0;
    unsigned int C_Group_ID_08 = 0;
    unsigned int C_Group_ID_09 = 0;
    unsigned int C_Group_ID_10 = 0;
    unsigned int Quest_ID_01 = 0;
    unsigned int Quest_ID_02 = 0;
    std::uint16_t Q_Chance_01 = 0;
    std::uint16_t Q_Chance_02 = 0;
    std::uint8_t Q_Item_Drop_Type_01 = 0;
    std::uint8_t Q_Item_Drop_Type_02 = 0;
    unsigned int Q_Item_ID_01 = 0;
    unsigned int Q_Item_ID_02 = 0;
    std::uint16_t Q_Item_Min_01 = 0;
    std::uint16_t Q_Item_Min_02 = 0;
    std::uint16_t Q_Item_Max_01 = 0;
    std::uint16_t Q_Item_Max_02 = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_DROP) == 0xFE, "TB_DROP size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_DROP* GetTB_DROP(unsigned int index) ;
    void SetTB_DROP(unsigned int index, const TB_DROP& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBDropDB() ;
    std::unordered_map<unsigned int, TB_DROP> dropRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_DROP* XResourceMgr::GetTB_DROP(unsigned int index) {
        auto it = dropRows_.find(index);
        if (it == dropRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_DROP(unsigned int index, const TB_DROP& row) {
        dropRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBDropDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Drop_Index], [Drop_Rate_Clear], [Gold_Chance], [Gold_Min], [Gold_Max], [Gold_Jp_Rate], [Gold_Jp_Multi], [SV_Chance], [SV_Min], [SV_Max], [Et_Chance], [Et_Min], [Et_Max], [Et_Unit], [Et_Jp_Rate], [Et_Jp_Multi], [BP_Chance], [BP_Max], [BP_Min], [G_Chance_01], [Group_ID_01], [G_Chance_02], [Group_ID_02], [G_Chance_03], [Group_ID_03], [G_Chance_04], [Group_ID_04], [G_Chance_05], [Group_ID_05], [G_Chance_06], [Group_ID_06], [G_Chance_07], [Group_ID_07], [G_Chance_08], [Group_ID_08], [G_Chance_09], [Group_ID_09], [G_Chance_10], [Group_ID_10], [G_Chance_11], [Group_ID_11], [G_Chance_12], [Group_ID_12], [G_Chance_13], [Group_ID_13], [G_Chance_14], [Group_ID_14], [G_Chance_15], [Group_ID_15], [G_Chance_16], [Group_ID_16], [G_Chance_17], [Group_ID_17], [G_Chance_18], [Group_ID_18], [G_Chance_19], [Group_ID_19], [G_Chance_20], [Group_ID_20], [G_Chance_21], [Group_ID_21], [C_Chance_01], [C_Group_ID_01], [C_Chance_02], [C_Group_ID_02], [C_Chance_03], [C_Group_ID_03], [C_Chance_04], [C_Group_ID_04], [C_Chance_05], [C_Group_ID_05], [C_Chance_06], [C_Group_ID_06], [C_Chance_07], [C_Group_ID_07], [C_Chance_08], [C_Group_ID_08], [C_Chance_09], [C_Group_ID_09], [C_Chance_10], [C_Group_ID_10], [Quest_ID_01], [Q_Chance_01], [Q_Item_Drop_Type_01], [Q_Item_ID_01], [Q_Item_Min_01], [Q_Item_Max_01], [Quest_ID_02], [Q_Chance_02], [Q_Item_Drop_Type_02], [Q_Item_ID_02], [Q_Item_Min_02], [Q_Item_Max_02] from tb_Drop")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        dropRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_DROP row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Drop_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Drop_Rate_Clear, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Gold_Chance, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Gold_Min, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Gold_Max, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Gold_Jp_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Gold_Jp_Multi, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SV_Chance, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SV_Min, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SV_Max, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Et_Chance, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Et_Min, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Et_Max, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Et_Unit, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Et_Jp_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Et_Jp_Multi, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BP_Chance, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BP_Max, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BP_Min, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.G_Chance_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.G_Chance_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.G_Chance_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.G_Chance_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.G_Chance_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.G_Chance_06, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID_06, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.G_Chance_07, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID_07, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.G_Chance_08, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID_08, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.G_Chance_09, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID_09, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.G_Chance_10, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID_10, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.G_Chance_11, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID_11, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.G_Chance_12, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID_12, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.G_Chance_13, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID_13, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.G_Chance_14, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID_14, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.G_Chance_15, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID_15, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.G_Chance_16, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID_16, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.G_Chance_17, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID_17, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.G_Chance_18, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID_18, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.G_Chance_19, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID_19, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.G_Chance_20, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID_20, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.G_Chance_21, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_ID_21, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.C_Chance_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.C_Group_ID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.C_Chance_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.C_Group_ID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.C_Chance_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.C_Group_ID_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.C_Chance_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.C_Group_ID_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.C_Chance_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.C_Group_ID_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.C_Chance_06, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.C_Group_ID_06, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.C_Chance_07, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.C_Group_ID_07, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.C_Chance_08, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.C_Group_ID_08, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.C_Chance_09, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.C_Group_ID_09, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.C_Chance_10, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.C_Group_ID_10, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Quest_ID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Q_Chance_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Q_Item_Drop_Type_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Q_Item_ID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Q_Item_Min_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Q_Item_Max_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Quest_ID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Q_Chance_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Q_Item_Drop_Type_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Q_Item_ID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Q_Item_Min_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Q_Item_Max_02, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_DROP(row.Drop_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
