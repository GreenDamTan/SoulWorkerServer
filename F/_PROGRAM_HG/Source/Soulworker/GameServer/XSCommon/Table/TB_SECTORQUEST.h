// 中文说明：
// 1. 本文件承接 TB_SECTORQUEST 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_SECTORQUEST {
    unsigned int ID = 0;
    std::uint8_t uniBeforeType[3] = {};
    std::uint8_t uniBeforeState[3] = {};
    unsigned int uniBeforeID[3] = {};
    std::uint8_t uniFinalType[3] = {};
    std::uint8_t uniFinalState[3] = {};
    unsigned int uniFinalID[3] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_SECTORQUEST) == 0x28, "TB_SECTORQUEST size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_SECTORQUEST* GetTB_SECTORQUEST(unsigned int index) ;
    void SetTB_SECTORQUEST(unsigned int index, const TB_SECTORQUEST& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBSectorQuestDB() ;
    std::map<unsigned int, TB_SECTORQUEST> m_mapTB_SECTORQUEST;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_SECTORQUEST* XResourceMgr::GetTB_SECTORQUEST(unsigned int index) {
        const auto it = m_mapTB_SECTORQUEST.find(index);
        return it == m_mapTB_SECTORQUEST.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_SECTORQUEST(unsigned int index, const TB_SECTORQUEST& row) {
        m_mapTB_SECTORQUEST[index] = row;
    }

std::int64_t XResourceMgr::LoadTBSectorQuestDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Before_Check_Quest_Type_01], [Before_Check_Quest_State_01], [Before_Check_Quest_ID_01], [Final_Check_Quest_Type_01], [Final_Check_Quest_State_01], [Final_Check_Quest_ID_01], [Before_Check_Quest_Type_02], [Before_Check_Quest_State_02], [Before_Check_Quest_ID_02], [Final_Check_Quest_Type_02], [Final_Check_Quest_State_02], [Final_Check_Quest_ID_02], [Before_Check_Quest_Type_03], [Before_Check_Quest_State_03], [Before_Check_Quest_ID_03], [Final_Check_Quest_Type_03], [Final_Check_Quest_State_03], [Final_Check_Quest_ID_03] from tb_SectorQuest")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_SECTORQUEST.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_SECTORQUEST row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBeforeType[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBeforeState[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBeforeID[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniFinalType[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniFinalState[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniFinalID[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBeforeType[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBeforeState[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBeforeID[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniFinalType[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniFinalState[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniFinalID[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBeforeType[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBeforeState[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBeforeID[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniFinalType[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniFinalState[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniFinalID[2], &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_SECTORQUEST(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
