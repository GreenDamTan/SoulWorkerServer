// 中文说明：
// 1. 本文件承接 TB_MAZEREWARD_STANDARD 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 2)
struct TB_MAZEREWARD_STANDARD {
    std::int16_t Episode_ID = 0;
    std::uint16_t Episode_Clear_Point = 0;
    std::int16_t Monster_Kill = 0;
    std::int16_t Kill_Success = 0;
    std::int16_t Kill_Fail = 0;
    std::int16_t Party_1 = 0;
    std::int16_t Party_2 = 0;
    std::int16_t Party_3 = 0;
    std::int16_t Party_4 = 0;
    int Rank_C_Point = 0;
    int Rank_B_Point = 0;
    int Rank_A_Point = 0;
    int Rank_S_Point = 0;
    int Rank_SS_Point = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_MAZEREWARD_STANDARD) == 0x26, "TB_MAZEREWARD_STANDARD size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MAZEREWARD_STANDARD* GetTB_MAZEREWARD_STANDARD(std::int16_t index) ;
    void SetTB_MAZEREWARD_STANDARD(std::int16_t index, const TB_MAZEREWARD_STANDARD& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBMazeRewardStandardDB() ;
    std::map<std::int16_t, TB_MAZEREWARD_STANDARD> m_mapTB_MAZEREWARD_STANDARD;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MAZEREWARD_STANDARD* XResourceMgr::GetTB_MAZEREWARD_STANDARD(std::int16_t index) {
        const auto it = m_mapTB_MAZEREWARD_STANDARD.find(index);
        return it == m_mapTB_MAZEREWARD_STANDARD.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MAZEREWARD_STANDARD(std::int16_t index, const TB_MAZEREWARD_STANDARD& row) {
        m_mapTB_MAZEREWARD_STANDARD[index] = row;
    }

std::int64_t XResourceMgr::LoadTBMazeRewardStandardDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Episode_ID], [Episode_Clear_Point], [Monster_Kill], [Kill_Success], [Kill_Fail], [Party_1], [Party_2], [Party_3], [Party_4], [Rank_C_Point], [Rank_B_Point], [Rank_A_Point], [Rank_S_Point], [Rank_SS_Point] from tb_MazeReward_Standard")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MAZEREWARD_STANDARD.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MAZEREWARD_STANDARD row{};
            if (!GreenDamTan_DBGetSigned(xDBBinder, &row.Episode_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Episode_Clear_Point, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Monster_Kill, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Kill_Success, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Kill_Fail, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Party_1, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Party_2, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Party_3, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Party_4, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Rank_C_Point, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Rank_B_Point, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Rank_A_Point, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Rank_S_Point, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Rank_SS_Point, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MAZEREWARD_STANDARD(row.Episode_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
