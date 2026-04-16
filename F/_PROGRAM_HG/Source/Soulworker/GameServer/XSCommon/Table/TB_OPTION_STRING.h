// 中文说明：
// 1. 本文件承接 TB_OPTION_STRING 的单表还原片段，字段与装载顺序按 LoginServer.pdb / IDA 保持一致。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 不要把 `UI_String` 或 `Output_Type` 擅自改成推测结构，避免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_OPTION_STRING {
    std::uint16_t Option_Name_ID = 0;
    std::uint8_t EffectType = 0;
    std::uint16_t Effect_Num = 0;
    char UI_String[511] = {};
    std::uint8_t Output_Type = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_OPTION_STRING) == 0x205, "TB_OPTION_STRING size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_OPTION_STRING* GetTB_OPTION_STRING(std::uint16_t index) ;
    void SetTB_OPTION_STRING(std::uint16_t index, const TB_OPTION_STRING& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBOptionStringDB() ;
    std::map<std::uint16_t, TB_OPTION_STRING> m_mapTB_OPTION_STRING;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_OPTION_STRING* XResourceMgr::GetTB_OPTION_STRING(std::uint16_t index) {
        const auto it = m_mapTB_OPTION_STRING.find(index);
        return it == m_mapTB_OPTION_STRING.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_OPTION_STRING(std::uint16_t index, const TB_OPTION_STRING& row) {
        m_mapTB_OPTION_STRING[index] = row;
    }

std::int64_t XResourceMgr::LoadTBOptionStringDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Option_Name_ID], [EffectType], [Effect_Num], [UI_String], [Output_Type] from tb_Option_String")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_OPTION_STRING.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_OPTION_STRING row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Option_Name_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.EffectType, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Effect_Num, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.UI_String, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Output_Type, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_OPTION_STRING(row.Option_Name_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
