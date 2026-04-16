// 中文说明：
// 1. 本文件承接 TB_PARTYEXP_MOB 的单表还原片段，字段与装载顺序按 LoginServer.pdb / IDA 保持一致。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 不要把 `Mob_LevelInterval` 的有符号性质擅自改成无符号，避免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_PARTYEXP_MOB {
    char Mob_LevelInterval = 0;
    float Mob_IntervalValue = 0.0f;
};
#pragma pack(pop)
static_assert(sizeof(TB_PARTYEXP_MOB) == 0x5, "TB_PARTYEXP_MOB size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_PARTYEXP_MOB* GetTB_PARTYEXP_MOB(char index) ;
    void SetTB_PARTYEXP_MOB(char index, const TB_PARTYEXP_MOB& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBPartyExpMobDB() ;
    std::map<char, TB_PARTYEXP_MOB> m_mapTB_PARTYEXP_MOB;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_PARTYEXP_MOB* XResourceMgr::GetTB_PARTYEXP_MOB(char index) {
        const auto it = m_mapTB_PARTYEXP_MOB.find(index);
        return it == m_mapTB_PARTYEXP_MOB.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_PARTYEXP_MOB(char index, const TB_PARTYEXP_MOB& row) {
        m_mapTB_PARTYEXP_MOB[index] = row;
    }

std::int64_t XResourceMgr::LoadTBPartyExpMobDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Mob_LevelInterval], [Mob_IntervalValue] from tb_PartyExp_Mob")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_PARTYEXP_MOB.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_PARTYEXP_MOB row{};
            if (!GreenDamTan_DBGetSigned(xDBBinder, &row.Mob_LevelInterval, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Mob_IntervalValue, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_PARTYEXP_MOB(row.Mob_LevelInterval, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
