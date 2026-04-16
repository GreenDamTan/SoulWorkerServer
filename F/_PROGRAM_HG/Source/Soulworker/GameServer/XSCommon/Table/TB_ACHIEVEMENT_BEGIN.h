// 中文说明：
// 1. 本文件承接 TB_ACHIEVEMENT_BEGIN 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_ACHIEVEMENT_BEGIN {
    std::uint16_t ID = 0;
    unsigned int Achievement_ID = 0;
    std::uint8_t Achievement_Open_Lv = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_ACHIEVEMENT_BEGIN) == 0x7, "TB_ACHIEVEMENT_BEGIN size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_ACHIEVEMENT_BEGIN* GetTB_ACHIEVEMENT_BEGIN(std::uint16_t index) ;
    void SetTB_ACHIEVEMENT_BEGIN(std::uint16_t index, const TB_ACHIEVEMENT_BEGIN& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBAchievementBeginDB() ;
    std::map<std::uint16_t, TB_ACHIEVEMENT_BEGIN> m_mapTB_ACHIEVEMENT_BEGIN;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_ACHIEVEMENT_BEGIN* XResourceMgr::GetTB_ACHIEVEMENT_BEGIN(std::uint16_t index) {
        const auto it = m_mapTB_ACHIEVEMENT_BEGIN.find(index);
        return it == m_mapTB_ACHIEVEMENT_BEGIN.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_ACHIEVEMENT_BEGIN(std::uint16_t index, const TB_ACHIEVEMENT_BEGIN& row) {
        m_mapTB_ACHIEVEMENT_BEGIN[index] = row;
    }

std::int64_t XResourceMgr::LoadTBAchievementBeginDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Achievement_ID], [Achievement_Open_Lv] from tb_Achievement_begin")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_ACHIEVEMENT_BEGIN.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_ACHIEVEMENT_BEGIN row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Achievement_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Achievement_Open_Lv, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_ACHIEVEMENT_BEGIN(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
