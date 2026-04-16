// 中文说明：
// 1. 本文件承接 TB_MAZEREWARD_REVISION 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 2)
struct TB_MAZEREWARD_REVISION {
    std::uint16_t Character_ID = 0;
    float Revision_Value = 0.0f;
    float Revision_Value_Thething = 0.0f;
};
#pragma pack(pop)
static_assert(sizeof(TB_MAZEREWARD_REVISION) == 0xA, "TB_MAZEREWARD_REVISION size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MAZEREWARD_REVISION* GetTB_MAZEREWARD_REVISION(std::uint16_t index) ;
    void SetTB_MAZEREWARD_REVISION(std::uint16_t index, const TB_MAZEREWARD_REVISION& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBMazeRewardRevisionDB() ;
    std::map<std::uint16_t, TB_MAZEREWARD_REVISION> m_mapTB_MAZEREWARD_REVISION;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MAZEREWARD_REVISION* XResourceMgr::GetTB_MAZEREWARD_REVISION(std::uint16_t index) {
        const auto it = m_mapTB_MAZEREWARD_REVISION.find(index);
        return it == m_mapTB_MAZEREWARD_REVISION.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MAZEREWARD_REVISION(std::uint16_t index, const TB_MAZEREWARD_REVISION& row) {
        m_mapTB_MAZEREWARD_REVISION[index] = row;
    }

std::int64_t XResourceMgr::LoadTBMazeRewardRevisionDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Character_ID], [Revision_Value], [Revision_Value_Thething] from tb_MazeReward_Revision")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MAZEREWARD_REVISION.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MAZEREWARD_REVISION row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Character_ID, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Revision_Value, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Revision_Value_Thething, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MAZEREWARD_REVISION(row.Character_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
