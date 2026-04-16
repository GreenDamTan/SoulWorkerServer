// 中文说明：
// 1. 本文件承接 TB_PARTYEXP_MEMBER 的单表还原片段，字段与装载顺序按 LoginServer.pdb / IDA 保持一致。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 不要把 `Party_Member_ID` 键类型、`Booster_ID` 宽度擅自改写，避免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_PARTYEXP_MEMBER {
    std::uint8_t Party_Member_ID = 0;
    std::uint8_t Party_Member = 0;
    std::uint8_t Friend_Value = 0;
    std::uint16_t Booster_ID = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_PARTYEXP_MEMBER) == 0x5, "TB_PARTYEXP_MEMBER size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_PARTYEXP_MEMBER* GetTB_PARTYEXP_MEMBER(std::uint8_t index) ;
    void SetTB_PARTYEXP_MEMBER(std::uint8_t index, const TB_PARTYEXP_MEMBER& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBPartyExpMemberDB() ;
    std::map<std::uint8_t, TB_PARTYEXP_MEMBER> m_mapTB_PARTYEXP_MEMBER;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_PARTYEXP_MEMBER* XResourceMgr::GetTB_PARTYEXP_MEMBER(std::uint8_t index) {
        const auto it = m_mapTB_PARTYEXP_MEMBER.find(index);
        return it == m_mapTB_PARTYEXP_MEMBER.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_PARTYEXP_MEMBER(std::uint8_t index, const TB_PARTYEXP_MEMBER& row) {
        m_mapTB_PARTYEXP_MEMBER[index] = row;
    }

std::int64_t XResourceMgr::LoadTBPartyExpMemberDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Party_Member_ID], [Party_Member], [Friend_Value], [Booster_ID] from tb_PartyExp_Member")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_PARTYEXP_MEMBER.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_PARTYEXP_MEMBER row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Party_Member_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Party_Member, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Friend_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Booster_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_PARTYEXP_MEMBER(row.Party_Member_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
