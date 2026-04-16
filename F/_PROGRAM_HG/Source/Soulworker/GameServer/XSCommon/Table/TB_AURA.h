// 中文说明：
// 1. 本文件承接 TB_AURA 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_AURA {
    std::uint16_t Aura_Index = 0;
    std::uint8_t Apply_Target = 0;
    unsigned int Valid_Distance = 0;
    std::uint8_t Check_Type = 0;
    unsigned int Check_Time = 0;
    unsigned int Aura_Time = 0;
    std::uint16_t Apply_Buff = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_AURA) == 0x12, "TB_AURA size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_AURA* GetTB_AURA(std::uint16_t index) ;
    void SetTB_AURA(std::uint16_t index, const TB_AURA& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBAuraDB() ;
    std::map<std::uint16_t, TB_AURA> m_mapTB_AURA;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_AURA* XResourceMgr::GetTB_AURA(std::uint16_t index) {
        const auto it = m_mapTB_AURA.find(index);
        return it == m_mapTB_AURA.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_AURA(std::uint16_t index, const TB_AURA& row) {
        m_mapTB_AURA[index] = row;
    }

std::int64_t XResourceMgr::LoadTBAuraDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Aura_Index], [Apply_Target], [Valid_Distance], [Check_Type], [Check_Time], [Aura_Time], [Apply_Buff] from tb_Aura")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_AURA.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_AURA row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Aura_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Apply_Target, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Valid_Distance, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Check_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Check_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Aura_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Apply_Buff, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_AURA(row.Aura_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
