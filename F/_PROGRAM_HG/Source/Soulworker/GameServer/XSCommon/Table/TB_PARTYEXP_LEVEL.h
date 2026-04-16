// 中文说明：
// 1. 本文件承接 TB_PARTYEXP_LEVEL 的单表还原片段，字段与装载顺序按 LoginServer.pdb / IDA 保持一致。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 不要把 `Party_LevelInterval` 键类型擅自改成更宽整数，避免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_PARTYEXP_LEVEL {
    std::uint8_t Party_LevelInterval = 0;
    float LevelInterval_Value = 0.0f;
};
#pragma pack(pop)
static_assert(sizeof(TB_PARTYEXP_LEVEL) == 0x5, "TB_PARTYEXP_LEVEL size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_PARTYEXP_LEVEL* GetTB_PARTYEXP_LEVEL(std::uint8_t index) ;
    void SetTB_PARTYEXP_LEVEL(std::uint8_t index, const TB_PARTYEXP_LEVEL& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBPartyExpLevelDB() ;
    std::map<std::uint8_t, TB_PARTYEXP_LEVEL> m_mapTB_PARTYEXP_LEVEL;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_PARTYEXP_LEVEL* XResourceMgr::GetTB_PARTYEXP_LEVEL(std::uint8_t index) {
        const auto it = m_mapTB_PARTYEXP_LEVEL.find(index);
        return it == m_mapTB_PARTYEXP_LEVEL.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_PARTYEXP_LEVEL(std::uint8_t index, const TB_PARTYEXP_LEVEL& row) {
        m_mapTB_PARTYEXP_LEVEL[index] = row;
    }

std::int64_t XResourceMgr::LoadTBPartyExpLevelDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Party_LevelInterval], [LevelInterval_Value] from tb_PartyExp_Level")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_PARTYEXP_LEVEL.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_PARTYEXP_LEVEL row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Party_LevelInterval, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.LevelInterval_Value, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_PARTYEXP_LEVEL(row.Party_LevelInterval, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
