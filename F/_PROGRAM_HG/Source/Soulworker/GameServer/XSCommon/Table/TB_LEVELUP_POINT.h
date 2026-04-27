// 中文说明：
// 1. 本文件承接 TB_LEVELUP_POINT 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_LEVELUP_POINT {
    std::uint8_t ID = 0;
    std::uint8_t Level = 0;
    int Need_EXP = 0;
    std::uint8_t Give_Skill_Point = 0;
    std::uint16_t SV_Max_Point = 0;
};
static_assert(sizeof(TB_LEVELUP_POINT) == 0x9, "TB_LEVELUP_POINT size mismatch with IDA");
#pragma pack(pop)
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_LEVELUP_POINT* GetTB_LEVELUP_POINT(std::uint8_t index) ;
    void SetTB_LEVELUP_POINT(std::uint8_t index, const TB_LEVELUP_POINT& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBLevelupPointDB() ;
    std::map<std::uint8_t, TB_LEVELUP_POINT> m_mapTB_LEVELUP_POINT;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_LEVELUP_POINT* XResourceMgr::GetTB_LEVELUP_POINT(std::uint8_t index) {
        const auto it = m_mapTB_LEVELUP_POINT.find(index);
        return it == m_mapTB_LEVELUP_POINT.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_LEVELUP_POINT(std::uint8_t index, const TB_LEVELUP_POINT& row) {
        m_mapTB_LEVELUP_POINT[index] = row;
    }

std::int64_t XResourceMgr::LoadTBLevelupPointDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Level], [Need_EXP], [Give_Skill_Point], [SV_Max_Point] from tb_Levelup_Point")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_LEVELUP_POINT.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_LEVELUP_POINT row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Level, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Need_EXP, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Give_Skill_Point, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SV_Max_Point, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_LEVELUP_POINT(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
