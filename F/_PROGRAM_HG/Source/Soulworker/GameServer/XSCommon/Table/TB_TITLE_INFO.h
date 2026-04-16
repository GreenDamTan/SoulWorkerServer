// 中文说明：
// 1. 本文件承接 TB_TITLE_INFO 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_TITLE_INFO {
    unsigned int ID = 0;
    char Title_Name[511] = {};
    std::uint8_t Title_Type = 0;
    std::uint8_t Use_Class = 0;
    std::uint16_t Title_Array_Num = 0;
    std::uint8_t Open_Condition = 0;
    unsigned int Open_Condition_Value = 0;
    std::uint8_t Info_Type = 0;
    std::uint8_t uniClass[5] = {};
    std::uint16_t uniTitle[5] = {};
    float uniValue[5] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_TITLE_INFO) == 0x230, "TB_TITLE_INFO size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_TITLE_INFO* GetTB_TITLE_INFO(unsigned int index) ;
    void SetTB_TITLE_INFO(unsigned int index, const TB_TITLE_INFO& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBTitleInfoDB() ;
    std::map<unsigned int, TB_TITLE_INFO> m_mapTB_TITLE_INFO;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_TITLE_INFO* XResourceMgr::GetTB_TITLE_INFO(unsigned int index) {
        const auto it = m_mapTB_TITLE_INFO.find(index);
        return it == m_mapTB_TITLE_INFO.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_TITLE_INFO(unsigned int index, const TB_TITLE_INFO& row) {
        m_mapTB_TITLE_INFO[index] = row;
    }

std::int64_t XResourceMgr::LoadTBTitleInfoDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Title_Name], [Title_Type], [Use_Class], [Title_Array_Num], [Open_Condition], [Open_Condition_Value], [Info_Type], [Effect_Class_01], [Effect_Type_01], [Effect_value_01], [Effect_Class_02], [Effect_Type_02], [Effect_value_02], [Effect_Class_03], [Effect_Type_03], [Effect_value_03], [Effect_Class_04], [Effect_Type_04], [Effect_value_04], [Effect_Class_05], [Effect_Type_05], [Effect_value_05] from tb_Title_Info")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_TITLE_INFO.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_TITLE_INFO row{};
            const auto loadPackedFloat = [&](std::size_t offset) {
                float value = 0.0f;
                if (!GreenDamTan_DBGetFloat(xDBBinder, &value, &executeResult)) {
                    return false;
                }
                std::memcpy(reinterpret_cast<char*>(&row) + offset, &value, sizeof(value));
                return true;
            };
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Title_Name, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Title_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Use_Class, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Title_Array_Num, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Open_Condition, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Open_Condition_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Info_Type, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 5; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniClass[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniTitle[index], &executeResult) ||
                    !loadPackedFloat(offsetof(TB_TITLE_INFO, uniValue) + sizeof(float) * static_cast<std::size_t>(index))) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_TITLE_INFO(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
