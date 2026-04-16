// 中文说明：
// 1. 本文件承接 TB_PARTYREVISE 的单表还原片段，字段与装载顺序按 LoginServer.pdb / IDA 保持一致。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 不要把 Rev_* 与 SuperArmor 字段擅自抽象成推测结构，避免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_PARTYREVISE {
    unsigned int ID = 0;
    unsigned int GroupID = 0;
    std::uint8_t Member_Count = 0;
    float Rev_HP_RAT = 0.0f;
    float Rev_ATK_RAT = 0.0f;
    float Rev_DEF_RAT = 0.0f;
    float Rev_ASR_RAT = 0.0f;
    float Rev_MSR_RAT = 0.0f;
    float SuperArmor_Guage = 0.0f;
    unsigned int SA_Regen_Time = 0;
    unsigned int SA_Recover_Time = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_PARTYREVISE) == 0x29, "TB_PARTYREVISE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_PARTYREVISE* GetTB_PARTYREVISE(unsigned int index) ;
    void SetTB_PARTYREVISE(unsigned int index, const TB_PARTYREVISE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBPartyReviseDB() ;
    std::map<unsigned int, TB_PARTYREVISE> m_mapTB_PARTYREVISE;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_PARTYREVISE* XResourceMgr::GetTB_PARTYREVISE(unsigned int index) {
        const auto it = m_mapTB_PARTYREVISE.find(index);
        return it == m_mapTB_PARTYREVISE.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_PARTYREVISE(unsigned int index, const TB_PARTYREVISE& row) {
        m_mapTB_PARTYREVISE[index] = row;
    }

std::int64_t XResourceMgr::LoadTBPartyReviseDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [GroupID], [Member_Count], [Rev_HP_RAT], [Rev_ATK_RAT], [Rev_DEF_RAT], [Rev_ASR_RAT], [Rev_MSR_RAT], [SuperArmor_Guage], [SA_Regen_Time], [SA_Recover_Time] from tb_PartyRevise")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_PARTYREVISE.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_PARTYREVISE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.GroupID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Member_Count, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Rev_HP_RAT, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Rev_ATK_RAT, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Rev_DEF_RAT, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Rev_ASR_RAT, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Rev_MSR_RAT, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.SuperArmor_Guage, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SA_Regen_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SA_Recover_Time, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_PARTYREVISE(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
