// 中文说明：
// 1. 本文件承接 TB_DROP_GROUP_CHARACTER 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_DROP_GROUP_CHARACTER {
    unsigned int ID = 0;
    unsigned int Item_Char_ID_01 = 0;
    unsigned int Item_Char_ID_02 = 0;
    unsigned int Item_Char_ID_03 = 0;
    unsigned int Item_Char_ID_04 = 0;
    unsigned int Item_Char_ID_05 = 0;
    unsigned int Item_Char_ID_06 = 0;
    unsigned int Item_Char_ID_07 = 0;
    unsigned int Item_Char_ID_08 = 0;
    unsigned int Item_Char_ID_09 = 0;
    unsigned int Item_Char_ID_10 = 0;
    std::uint8_t My_Item_Select_Value = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_DROP_GROUP_CHARACTER) == 0x2D, "TB_DROP_GROUP_CHARACTER size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_DROP_GROUP_CHARACTER* GetTB_DROP_GROUP_CHARACTER(unsigned int index) ;
    void SetTB_DROP_GROUP_CHARACTER(unsigned int index, const TB_DROP_GROUP_CHARACTER& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBDropGroupCharacterDB() ;
    std::unordered_map<unsigned int, TB_DROP_GROUP_CHARACTER> dropGroupCharacterRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_DROP_GROUP_CHARACTER* XResourceMgr::GetTB_DROP_GROUP_CHARACTER(unsigned int index) {
        auto it = dropGroupCharacterRows_.find(index);
        if (it == dropGroupCharacterRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_DROP_GROUP_CHARACTER(unsigned int index, const TB_DROP_GROUP_CHARACTER& row) {
        dropGroupCharacterRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBDropGroupCharacterDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Item_Char_ID_01], [Item_Char_ID_02], [Item_Char_ID_03], [Item_Char_ID_04], [Item_Char_ID_05], [Item_Char_ID_06], [Item_Char_ID_07], [Item_Char_ID_08], [Item_Char_ID_09], [Item_Char_ID_10], [My_Item_Select_Value] from tb_Drop_Group_Character")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        dropGroupCharacterRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_DROP_GROUP_CHARACTER row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Char_ID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Char_ID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Char_ID_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Char_ID_04, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Char_ID_05, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Char_ID_06, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Char_ID_07, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Char_ID_08, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Char_ID_09, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_Char_ID_10, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.My_Item_Select_Value, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_DROP_GROUP_CHARACTER(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
