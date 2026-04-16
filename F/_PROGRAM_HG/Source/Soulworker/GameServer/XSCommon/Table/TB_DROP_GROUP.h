// 中文说明：
// 1. 本文件承接 TB_DROP_GROUP 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_DROP_GROUP {
    unsigned int Group_Index = 0;
    std::uint16_t I_Chance_01 = 0;
    std::uint16_t I_Chance_02 = 0;
    std::uint16_t I_Chance_03 = 0;
    std::uint16_t I_Chance_04 = 0;
    std::uint16_t I_Chance_05 = 0;
    std::uint16_t I_Chance_06 = 0;
    std::uint16_t I_Chance_07 = 0;
    std::uint16_t I_Chance_08 = 0;
    std::uint16_t I_Chance_09 = 0;
    std::uint16_t I_Chance_10 = 0;
    unsigned int Item_ID_01 = 0;
    unsigned int Item_ID_02 = 0;
    unsigned int Item_ID_03 = 0;
    unsigned int Item_ID_04 = 0;
    unsigned int Item_ID_05 = 0;
    unsigned int Item_ID_06 = 0;
    unsigned int Item_ID_07 = 0;
    unsigned int Item_ID_08 = 0;
    unsigned int Item_ID_09 = 0;
    unsigned int Item_ID_10 = 0;
    std::uint8_t Item_Min_Cnt_01 = 0;
    std::uint8_t Item_Min_Cnt_02 = 0;
    std::uint8_t Item_Min_Cnt_03 = 0;
    std::uint8_t Item_Min_Cnt_04 = 0;
    std::uint8_t Item_Min_Cnt_05 = 0;
    std::uint8_t Item_Min_Cnt_06 = 0;
    std::uint8_t Item_Min_Cnt_07 = 0;
    std::uint8_t Item_Min_Cnt_08 = 0;
    std::uint8_t Item_Min_Cnt_09 = 0;
    std::uint8_t Item_Min_Cnt_10 = 0;
    std::uint8_t Item_Max_Cnt_01 = 0;
    std::uint8_t Item_Max_Cnt_02 = 0;
    std::uint8_t Item_Max_Cnt_03 = 0;
    std::uint8_t Item_Max_Cnt_04 = 0;
    std::uint8_t Item_Max_Cnt_05 = 0;
    std::uint8_t Item_Max_Cnt_06 = 0;
    std::uint8_t Item_Max_Cnt_07 = 0;
    std::uint8_t Item_Max_Cnt_08 = 0;
    std::uint8_t Item_Max_Cnt_09 = 0;
    std::uint8_t Item_Max_Cnt_10 = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_DROP_GROUP) == 0x54, "TB_DROP_GROUP size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_DROP_GROUP* GetTB_DROP_GROUP(unsigned int index) ;
    void SetTB_DROP_GROUP(unsigned int index, const TB_DROP_GROUP& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBDropGroupDB() ;
    std::unordered_map<unsigned int, TB_DROP_GROUP> dropGroupRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_DROP_GROUP* XResourceMgr::GetTB_DROP_GROUP(unsigned int index) {
        auto it = dropGroupRows_.find(index);
        if (it == dropGroupRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_DROP_GROUP(unsigned int index, const TB_DROP_GROUP& row) {
        dropGroupRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBDropGroupDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Group_Index], [I_Chance_01], [Item_ID_01], [Item_Min_Cnt_01], [Item_Max_Cnt_01], [I_Chance_02], [Item_ID_02], [Item_Min_Cnt_02], [Item_Max_Cnt_02], [I_Chance_03], [Item_ID_03], [Item_Min_Cnt_03], [Item_Max_Cnt_03], [I_Chance_04], [Item_ID_04], [Item_Min_Cnt_04], [Item_Max_Cnt_04], [I_Chance_05], [Item_ID_05], [Item_Min_Cnt_05], [Item_Max_Cnt_05], [I_Chance_06], [Item_ID_06], [Item_Min_Cnt_06], [Item_Max_Cnt_06], [I_Chance_07], [Item_ID_07], [Item_Min_Cnt_07], [Item_Max_Cnt_07], [I_Chance_08], [Item_ID_08], [Item_Min_Cnt_08], [Item_Max_Cnt_08], [I_Chance_09], [Item_ID_09], [Item_Min_Cnt_09], [Item_Max_Cnt_09], [I_Chance_10], [Item_ID_10], [Item_Min_Cnt_10], [Item_Max_Cnt_10] from tb_Drop_Group")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        dropGroupRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_DROP_GROUP row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.I_Chance_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Min_Cnt_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Max_Cnt_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.I_Chance_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Min_Cnt_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Max_Cnt_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.I_Chance_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Min_Cnt_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Max_Cnt_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.I_Chance_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Min_Cnt_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Max_Cnt_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.I_Chance_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Min_Cnt_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Max_Cnt_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.I_Chance_06, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID_06, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Min_Cnt_06, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Max_Cnt_06, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.I_Chance_07, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID_07, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Min_Cnt_07, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Max_Cnt_07, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.I_Chance_08, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID_08, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Min_Cnt_08, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Max_Cnt_08, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.I_Chance_09, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID_09, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Min_Cnt_09, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Max_Cnt_09, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.I_Chance_10, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID_10, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Min_Cnt_10, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Max_Cnt_10, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_DROP_GROUP(row.Group_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
