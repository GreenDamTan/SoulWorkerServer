// 中文说明：
// 1. 本文件承接 TB_MAZEREWARD_NORMAL 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 2)
struct TB_MAZEREWARD_NORMAL {
    std::uint16_t ID = 0;
    unsigned int Count = 0;
    unsigned int Combo_Score = 0;
    unsigned int Climax_Score = 0;
    unsigned int Erosion_Score = 0;
    unsigned int Collaboration_Score = 0;
    unsigned int Damage_Score = 0;
    unsigned int VictimHit_Rate = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_MAZEREWARD_NORMAL) == 0x1E, "TB_MAZEREWARD_NORMAL size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MAZEREWARD_NORMAL* GetTB_MAZEREWARD_NORMAL(std::uint16_t index) ;
    void SetTB_MAZEREWARD_NORMAL(std::uint16_t index, const TB_MAZEREWARD_NORMAL& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBMazeRewardNormalDB() ;
    std::map<std::uint16_t, TB_MAZEREWARD_NORMAL> m_mapTB_MAZEREWARD_NORMAL;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MAZEREWARD_NORMAL* XResourceMgr::GetTB_MAZEREWARD_NORMAL(std::uint16_t index) {
        const auto it = m_mapTB_MAZEREWARD_NORMAL.find(index);
        return it == m_mapTB_MAZEREWARD_NORMAL.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MAZEREWARD_NORMAL(std::uint16_t index, const TB_MAZEREWARD_NORMAL& row) {
        m_mapTB_MAZEREWARD_NORMAL[index] = row;
    }

std::int64_t XResourceMgr::LoadTBMazeRewardNormalDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Count], [Combo_Score], [Climax_Score], [Erosion_Score], [Collaboration_Score], [Damage_Score], [VictimHit_Rate] from tb_MazeReward_Normal")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MAZEREWARD_NORMAL.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MAZEREWARD_NORMAL row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Combo_Score, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Climax_Score, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Erosion_Score, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Collaboration_Score, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Damage_Score, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.VictimHit_Rate, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MAZEREWARD_NORMAL(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
