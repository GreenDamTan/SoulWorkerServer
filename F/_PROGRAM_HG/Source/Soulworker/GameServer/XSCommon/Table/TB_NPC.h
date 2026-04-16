// 中文说明：
// 1. 本文件承接 TB_NPC 的单表还原片段，字段与装载顺序按 LoginServer.pdb / IDA 保持一致。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 不要把 NPC_Function_Type / NPC_Function_ID 折叠成推测结构体，避免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_NPC {
    unsigned int NPC_ID = 0;
    std::uint16_t Npc_Group_ID = 0;
    std::uint8_t NPC_Faction = 0;
    char NPC_Code_Name[255] = {};
    std::uint8_t NPC_Status_Type = 0;
    unsigned int NPC_Status_ID = 0;
    std::uint8_t NPC_Level = 0;
    unsigned int NPC_Sight = 0;
    std::uint16_t NPC_Walk_Speed = 0;
    std::uint16_t NPC_Run_Speed = 0;
    std::uint16_t NPC_Turn_Speed = 0;
    std::uint8_t NPC_League = 0;
    std::uint8_t NPC_Intersperse_Type = 0;
    std::uint8_t NPC_Function_Type_01 = 0;
    std::uint8_t NPC_Function_Type_02 = 0;
    std::uint8_t NPC_Function_Type_03 = 0;
    std::uint16_t NPC_Function_ID_01 = 0;
    std::uint16_t NPC_Function_ID_02 = 0;
    std::uint16_t NPC_Function_ID_03 = 0;
    unsigned int Def_Talk_ID = 0;
    unsigned int NPC_TalkList_ID = 0;
    std::uint16_t NPC_CollisionRadius = 0;
    std::uint16_t NPC_CollisionHeight = 0;
    std::uint16_t NPC_RealHeight = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_NPC) == 0x12F, "TB_NPC size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_NPC* GetTB_NPC(unsigned int index) ;
    void SetTB_NPC(unsigned int index, const TB_NPC& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBNpcDB() ;
    std::map<unsigned int, TB_NPC> m_mapTB_NPC;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_NPC* XResourceMgr::GetTB_NPC(unsigned int index) {
        const auto it = m_mapTB_NPC.find(index);
        return it == m_mapTB_NPC.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_NPC(unsigned int index, const TB_NPC& row) {
        m_mapTB_NPC[index] = row;
    }

std::int64_t XResourceMgr::LoadTBNpcDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [NPC_ID], [Npc_Group_ID], [NPC_Faction], [NPC_Code_Name], [NPC_Status_Type], [NPC_Status_ID], [NPC_Level], [NPC_Sight], [NPC_Walk_Speed], [NPC_Run_Speed], [NPC_Turn_Speed], [NPC_League], [NPC_Intersperse_Type], [NPC_Function_Type_01], [NPC_Function_Type_02], [NPC_Function_Type_03], [NPC_Function_ID_01], [NPC_Function_ID_02], [NPC_Function_ID_03], [Def_Talk_ID], [NPC_TalkList_ID], [NPC_CollisionRadius], [NPC_CollisionHeight], [NPC_RealHeight] from tb_NPC")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_NPC.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_NPC row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.NPC_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Npc_Group_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NPC_Faction, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.NPC_Code_Name, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NPC_Status_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NPC_Status_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NPC_Level, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NPC_Sight, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NPC_Walk_Speed, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NPC_Run_Speed, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NPC_Turn_Speed, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NPC_League, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NPC_Intersperse_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NPC_Function_Type_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NPC_Function_Type_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NPC_Function_Type_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NPC_Function_ID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NPC_Function_ID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NPC_Function_ID_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Def_Talk_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NPC_TalkList_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NPC_CollisionRadius, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NPC_CollisionHeight, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.NPC_RealHeight, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_NPC(row.NPC_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
