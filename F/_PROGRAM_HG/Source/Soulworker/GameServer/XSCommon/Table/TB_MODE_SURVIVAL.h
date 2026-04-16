// 中文说明：
// 1. 本文件承接 TB_MODE_SURVIVAL 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_MODE_SURVIVAL {
    std::uint8_t ID = 0;
    unsigned int Quest_ID = 0;
    unsigned int Maze_ID = 0;
    unsigned int Sector_ID = 0;
    char Script_Name[511] = {};
    std::uint16_t SpawnBox_ID[10] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_MODE_SURVIVAL) == 0x220, "TB_MODE_SURVIVAL size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MODE_SURVIVAL* GetTB_MODE_SURVIVAL(std::uint8_t index) ;
    void SetTB_MODE_SURVIVAL(std::uint8_t index, const TB_MODE_SURVIVAL& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBModeSurvivalDB() ;
    std::map<std::uint8_t, TB_MODE_SURVIVAL> m_mapTB_MODE_SURVIVAL;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MODE_SURVIVAL* XResourceMgr::GetTB_MODE_SURVIVAL(std::uint8_t index) {
        const auto it = m_mapTB_MODE_SURVIVAL.find(index);
        return it == m_mapTB_MODE_SURVIVAL.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MODE_SURVIVAL(std::uint8_t index, const TB_MODE_SURVIVAL& row) {
        m_mapTB_MODE_SURVIVAL[index] = row;
    }

std::int64_t XResourceMgr::LoadTBModeSurvivalDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Quest_ID], [Maze_ID], [Sector_ID], [Script_Name], [SpawnBox_ID_01], [SpawnBox_ID_02], [SpawnBox_ID_03], [SpawnBox_ID_04], [SpawnBox_ID_05], [SpawnBox_ID_06], [SpawnBox_ID_07], [SpawnBox_ID_08], [SpawnBox_ID_09], [SpawnBox_ID_10] from tb_Mode_Survival")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MODE_SURVIVAL.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MODE_SURVIVAL row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Quest_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sector_ID, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Script_Name, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 10; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.SpawnBox_ID[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_MODE_SURVIVAL(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
