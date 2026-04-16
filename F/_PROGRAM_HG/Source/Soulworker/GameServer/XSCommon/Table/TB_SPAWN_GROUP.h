// 中文说明：
// 1. 本文件承接 TB_SPAWN_GROUP 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_SPAWN_GROUP {
    std::uint16_t ID = 0;
    std::uint16_t uniSpawnGroup[10] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_SPAWN_GROUP) == 0x16, "TB_SPAWN_GROUP size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_SPAWN_GROUP* GetTB_SPAWN_GROUP(std::uint16_t index) ;
    void SetTB_SPAWN_GROUP(std::uint16_t index, const TB_SPAWN_GROUP& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBSpawnGroupDB() ;
    std::map<std::uint16_t, TB_SPAWN_GROUP> m_mapTB_SPAWN_GROUP;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_SPAWN_GROUP* XResourceMgr::GetTB_SPAWN_GROUP(std::uint16_t index) {
        const auto it = m_mapTB_SPAWN_GROUP.find(index);
        return it == m_mapTB_SPAWN_GROUP.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_SPAWN_GROUP(std::uint16_t index, const TB_SPAWN_GROUP& row) {
        m_mapTB_SPAWN_GROUP[index] = row;
    }

std::int64_t XResourceMgr::LoadTBSpawnGroupDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [SpawnGroup_01], [SpawnGroup_02], [SpawnGroup_03], [SpawnGroup_04], [SpawnGroup_05], [SpawnGroup_06], [SpawnGroup_07], [SpawnGroup_08], [SpawnGroup_09], [SpawnGroup_10] from tb_Spawn_Group")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_SPAWN_GROUP.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_SPAWN_GROUP row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (auto& value : row.uniSpawnGroup) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &value, &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_SPAWN_GROUP(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
