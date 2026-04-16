// 中文说明：
// 1. 本文件承接 TB_MAZEREWARD_TIME 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 2)
struct TB_MAZEREWARD_TIME {
    std::uint16_t ID = 0;
    std::uint16_t Time_Value_Min = 0;
    std::uint16_t Time_Value_Max = 0;
    float ClearTime_Value = 0.0f;
};
#pragma pack(pop)
static_assert(sizeof(TB_MAZEREWARD_TIME) == 0xA, "TB_MAZEREWARD_TIME size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MAZEREWARD_TIME* GetTB_MAZEREWARD_TIME(std::uint16_t index) ;
    void SetTB_MAZEREWARD_TIME(std::uint16_t index, const TB_MAZEREWARD_TIME& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBMazeRewardTimeDB() ;
    std::map<std::uint16_t, TB_MAZEREWARD_TIME> m_mapTB_MAZEREWARD_TIME;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MAZEREWARD_TIME* XResourceMgr::GetTB_MAZEREWARD_TIME(std::uint16_t index) {
        const auto it = m_mapTB_MAZEREWARD_TIME.find(index);
        return it == m_mapTB_MAZEREWARD_TIME.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MAZEREWARD_TIME(std::uint16_t index, const TB_MAZEREWARD_TIME& row) {
        m_mapTB_MAZEREWARD_TIME[index] = row;
    }

std::int64_t XResourceMgr::LoadTBMazeRewardTimeDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Time_Value_Min], [Time_Value_Max], [ClearTime_Value] from tb_MazeReward_Time")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MAZEREWARD_TIME.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MAZEREWARD_TIME row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Time_Value_Min, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Time_Value_Max, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.ClearTime_Value, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MAZEREWARD_TIME(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
