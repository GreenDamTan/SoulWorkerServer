// 中文说明：
// 1. 本文件承接 TB_MODE_DEFENCE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。
// 4. 使用 pack(1) 因为 Quest_ID 在 offset 1 (奇数位置)。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_MODE_DEFENCE {
    std::uint8_t ID = 0;                        // offset 0x00
    unsigned int Quest_ID = 0;                  // offset 0x01 (pack(1) required)
    unsigned int Maze_ID = 0;                   // offset 0x05
    unsigned int Sector_ID = 0;                 // offset 0x09
    unsigned int Def_Monster_ID = 0;            // offset 0x0D
    unsigned int Def_Monster_AgrroValue = 0;    // offset 0x11
    unsigned int Def_Monster_AgrroInterval = 0;// offset 0x15
    std::uint16_t Def_Monster_AgrroRange = 0;  // offset 0x19
    unsigned int Jump_StartBox_ID = 0;          // offset 0x1B
    char Script_Name[511] = {};                 // offset 0x1F
    std::uint16_t SpawnBox_ID[10] = {};        // offset 0x21E
};
#pragma pack(pop)
static_assert(sizeof(TB_MODE_DEFENCE) == 0x232, "TB_MODE_DEFENCE size must match PDB 562 bytes");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MODE_DEFENCE* GetTB_MODE_DEFENCE(std::uint8_t index) ;
    void SetTB_MODE_DEFENCE(std::uint8_t index, const TB_MODE_DEFENCE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBModeDefenceDB() ;
    std::map<std::uint8_t, TB_MODE_DEFENCE> m_mapTB_MODE_DEFENCE;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MODE_DEFENCE* XResourceMgr::GetTB_MODE_DEFENCE(std::uint8_t index) {
        const auto it = m_mapTB_MODE_DEFENCE.find(index);
        return it == m_mapTB_MODE_DEFENCE.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MODE_DEFENCE(std::uint8_t index, const TB_MODE_DEFENCE& row) {
        m_mapTB_MODE_DEFENCE[index] = row;
    }

std::int64_t XResourceMgr::LoadTBModeDefenceDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Quest_ID], [Maze_ID], [Sector_ID], [Def_Monster_ID], [Def_Monster_AgrroValue], [Def_Monster_AgrroInterval], [Def_Monster_AgrroRange], [Jump_StartBox_ID], [Script_Name], [SpawnBox_ID_01], [SpawnBox_ID_02], [SpawnBox_ID_03], [SpawnBox_ID_04], [SpawnBox_ID_05], [SpawnBox_ID_06], [SpawnBox_ID_07], [SpawnBox_ID_08], [SpawnBox_ID_09], [SpawnBox_ID_10] from tb_Mode_Defence")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MODE_DEFENCE.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MODE_DEFENCE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Quest_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Sector_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Def_Monster_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Def_Monster_AgrroValue, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Def_Monster_AgrroInterval, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Def_Monster_AgrroRange, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Jump_StartBox_ID, &executeResult) ||
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
            SetTB_MODE_DEFENCE(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
