// 中文说明：
// 1. 本文件承接 TB_CLASSBATTLE_ROLE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_CLASSBATTLE_ROLE {
    std::uint16_t Index = 0;
    char Server_Map[511] = {};
    std::uint8_t Role_Index = 0;
    std::uint8_t Custom_Role = 0;
    std::uint8_t Req_Min_Lv = 0;
    std::uint8_t Admission_Member = 0;
    std::uint16_t Waiting_Time = 0;
    std::uint16_t Game_Time = 0;
    std::uint16_t Finish_Time = 0;
    std::uint8_t Win_Count = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_CLASSBATTLE_ROLE) == 0x20C, "TB_CLASSBATTLE_ROLE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_CLASSBATTLE_ROLE* GetTB_CLASSBATTLE_ROLE(std::uint16_t index) ;
    void SetTB_CLASSBATTLE_ROLE(std::uint16_t index, const TB_CLASSBATTLE_ROLE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBClassBattleRoleDB() ;
    std::unordered_map<std::uint16_t, TB_CLASSBATTLE_ROLE> classBattleRoleRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_CLASSBATTLE_ROLE* XResourceMgr::GetTB_CLASSBATTLE_ROLE(std::uint16_t index) {
        auto it = classBattleRoleRows_.find(index);
        if (it == classBattleRoleRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_CLASSBATTLE_ROLE(std::uint16_t index, const TB_CLASSBATTLE_ROLE& row) {
        classBattleRoleRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBClassBattleRoleDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Index], [Server_Map], [Role_Index], [Custom_Role], [Req_Min_Lv], [Admission_Member], [Waiting_Time], [Game_Time], [Finish_Time], [Win_Count] from tb_ClassBattle_Role")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        classBattleRoleRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_CLASSBATTLE_ROLE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Index, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Server_Map, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Role_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Custom_Role, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Req_Min_Lv, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Admission_Member, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Waiting_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Game_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Finish_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Win_Count, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_CLASSBATTLE_ROLE(row.Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
