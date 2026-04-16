// 中文说明：
// 1. 本文件承接 TB_UNITY_EVENT 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_UNITY_EVENT {
    std::uint16_t ID = 0;
    std::uint16_t uniSpawn[15] = {};
    std::uint16_t uniDSPoint[15] = {};
    std::uint16_t Grouton_Spawn_Ratio = 0;
    std::uint16_t SpawnGroup_Grouton = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_UNITY_EVENT) == 0x42, "TB_UNITY_EVENT size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_UNITY_EVENT* GetTB_UNITY_EVENT(std::uint16_t index) ;
    void SetTB_UNITY_EVENT(std::uint16_t index, const TB_UNITY_EVENT& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBUnityEventDB() ;
    std::map<std::uint16_t, TB_UNITY_EVENT> m_mapTB_UNITY_EVENT;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_UNITY_EVENT* XResourceMgr::GetTB_UNITY_EVENT(std::uint16_t index) {
        const auto it = m_mapTB_UNITY_EVENT.find(index);
        return it == m_mapTB_UNITY_EVENT.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_UNITY_EVENT(std::uint16_t index, const TB_UNITY_EVENT& row) {
        m_mapTB_UNITY_EVENT[index] = row;
    }

std::int64_t XResourceMgr::LoadTBUnityEventDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [SpawnGroup_Stage_01], [DS_Point_Stage_01], [SpawnGroup_Stage_02], [DS_Point_Stage_02], [SpawnGroup_Stage_03], [DS_Point_Stage_03], [SpawnGroup_Stage_04], [DS_Point_Stage_04], [SpawnGroup_Stage_05], [DS_Point_Stage_05], [SpawnGroup_Stage_06], [DS_Point_Stage_06], [SpawnGroup_Stage_07], [DS_Point_Stage_07], [SpawnGroup_Stage_08], [DS_Point_Stage_08], [SpawnGroup_Stage_09], [DS_Point_Stage_09], [SpawnGroup_Stage_10], [DS_Point_Stage_10], [SpawnGroup_Stage_11], [DS_Point_Stage_11], [SpawnGroup_Stage_12], [DS_Point_Stage_12], [SpawnGroup_Stage_13], [DS_Point_Stage_13], [SpawnGroup_Stage_14], [DS_Point_Stage_14], [SpawnGroup_Stage_15], [DS_Point_Stage_15], [Grouton_Spawn_Ratio], [SpawnGroup_Grouton] from tb_Unity_Event")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_UNITY_EVENT.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_UNITY_EVENT row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 15; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniSpawn[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniDSPoint[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Grouton_Spawn_Ratio, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SpawnGroup_Grouton, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_UNITY_EVENT(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
