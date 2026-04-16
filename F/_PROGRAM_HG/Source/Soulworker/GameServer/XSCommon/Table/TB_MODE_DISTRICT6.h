// 中文说明：
// 1. 本文件承接 TB_MODE_DISTRICT6 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_MODE_DISTRICT6 {
    std::uint8_t ID = 0;
    std::uint8_t World_ID = 0;
    std::uint8_t Start_Type = 0;
    std::uint8_t Group = 0;
    std::uint16_t Limit_Time = 0;
    unsigned int Ready_Message = 0;
    unsigned int Start_Message = 0;
    unsigned int Success_Message = 0;
    unsigned int Fail_Message = 0;
    std::uint8_t After_Mode_ID = 0;
    std::uint16_t Portal = 0;
    std::uint8_t Summon_Group_Count = 0;
    unsigned int Summon_Mob[10] = {};
    std::uint16_t Summon_Mob_SpawnBox[10] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_MODE_DISTRICT6) == 0x56, "TB_MODE_DISTRICT6 size must match decompile layout");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MODE_DISTRICT6* GetTB_MODE_DISTRICT6(std::uint8_t index) ;
    void SetTB_MODE_DISTRICT6(std::uint8_t index, const TB_MODE_DISTRICT6& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBModeDistrict6DB() ;
    std::map<std::uint8_t, TB_MODE_DISTRICT6> m_mapTB_MODE_DISTRICT6;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MODE_DISTRICT6* XResourceMgr::GetTB_MODE_DISTRICT6(std::uint8_t index) {
        const auto it = m_mapTB_MODE_DISTRICT6.find(index);
        return it == m_mapTB_MODE_DISTRICT6.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MODE_DISTRICT6(std::uint8_t index, const TB_MODE_DISTRICT6& row) {
        m_mapTB_MODE_DISTRICT6[index] = row;
    }

std::int64_t XResourceMgr::LoadTBModeDistrict6DB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [World_ID], [Start_Type], [Group], [Limit_Time], [Ready_Message], [Start_Message], [Success_Message], [Fail_Message], [After_Mode_ID], [Portal], [Summon_Group_Count], [Summon_Mob_01], [Summon_Mob_SpawnBox_01], [Summon_Mob_02], [Summon_Mob_SpawnBox_02], [Summon_Mob_03], [Summon_Mob_SpawnBox_03], [Summon_Mob_04], [Summon_Mob_SpawnBox_04], [Summon_Mob_05], [Summon_Mob_SpawnBox_05], [Summon_Mob_06], [Summon_Mob_SpawnBox_06], [Summon_Mob_07], [Summon_Mob_SpawnBox_07], [Summon_Mob_08], [Summon_Mob_SpawnBox_08], [Summon_Mob_09], [Summon_Mob_SpawnBox_09], [Summon_Mob_10], [Summon_Mob_SpawnBox_10] from tb_Mode_District6")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MODE_DISTRICT6.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MODE_DISTRICT6 row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.World_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Start_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Group, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Limit_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ready_Message, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Start_Message, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Success_Message, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Fail_Message, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.After_Mode_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Portal, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Summon_Group_Count, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 10; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Summon_Mob[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Summon_Mob_SpawnBox[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_MODE_DISTRICT6(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
