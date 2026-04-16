// 中文说明：
// 1. 本文件承接 TB_BATTLE_ARENA_INFO 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_BATTLE_ARENA_INFO {
    std::uint16_t ID = 0;
    std::uint8_t Contents_Type = 0;
    unsigned int Contents_ID = 0;
    std::uint16_t Maze_ID = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_BATTLE_ARENA_INFO) == 0x9, "TB_BATTLE_ARENA_INFO size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_BATTLE_ARENA_INFO* GetTB_BATTLE_ARENA_INFO(std::uint16_t index) ;
    void SetTB_BATTLE_ARENA_INFO(std::uint16_t index, const TB_BATTLE_ARENA_INFO& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBBattleArenaInfoDB() ;
    std::map<std::uint16_t, TB_BATTLE_ARENA_INFO> m_mapTB_BATTLE_ARENA_INFO;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_BATTLE_ARENA_INFO* XResourceMgr::GetTB_BATTLE_ARENA_INFO(std::uint16_t index) {
        const auto it = m_mapTB_BATTLE_ARENA_INFO.find(index);
        return it == m_mapTB_BATTLE_ARENA_INFO.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_BATTLE_ARENA_INFO(std::uint16_t index, const TB_BATTLE_ARENA_INFO& row) {
        m_mapTB_BATTLE_ARENA_INFO[index] = row;
    }

std::int64_t XResourceMgr::LoadTBBattleArenaInfoDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Contents_Type], [Contents_ID], [Maze_ID] from tb_Battle_Arena_Info")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_BATTLE_ARENA_INFO.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_BATTLE_ARENA_INFO row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Contents_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Contents_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_BATTLE_ARENA_INFO(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
