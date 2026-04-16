// 中文说明：
// 1. 本文件承接 TB_MAZEREWARD_ITEM 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_MAZEREWARD_ITEM {
    unsigned int Maze_ID = 0;
    std::uint16_t Mob_Rate = 0;
    unsigned int Box_ID = 0;
    unsigned int B_F_ID = 0;
    std::uint16_t B_F_Rate = 0;
    unsigned int B_C_ID = 0;
    std::uint16_t B_C_Rate = 0;
    unsigned int B_B_ID = 0;
    std::uint16_t B_B_Rate = 0;
    unsigned int B_A_ID = 0;
    std::uint16_t B_A_Rate = 0;
    unsigned int B_S_ID = 0;
    std::uint16_t B_S_Rate = 0;
    unsigned int B_SS_ID = 0;
    std::uint16_t B_SS_Rate = 0;
    int B_F_Min = 0;
    int B_F_Max = 0;
    int B_C_Min = 0;
    int B_C_Max = 0;
    int B_B_Min = 0;
    int B_B_Max = 0;
    int B_A_Min = 0;
    int B_A_Max = 0;
    int B_S_Min = 0;
    int B_S_Max = 0;
    int B_SS_Min = 0;
    int B_SS_Max = 0;
    unsigned int Cash_Reward_item_ID = 0;
    std::uint8_t Cash_Reward_item_EA_1 = 0;
    unsigned int Cash_Reward_item_ID_2 = 0;
    std::uint8_t Cash_Reward_item_EA_2 = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_MAZEREWARD_ITEM) == 0x68, "TB_MAZEREWARD_ITEM size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MAZEREWARD_ITEM* GetTB_MAZEREWARD_ITEM(unsigned int index) ;
    void SetTB_MAZEREWARD_ITEM(unsigned int index, const TB_MAZEREWARD_ITEM& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBMazeRewardItemDB() ;
    std::map<unsigned int, TB_MAZEREWARD_ITEM> m_mapTB_MAZEREWARD_ITEM;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MAZEREWARD_ITEM* XResourceMgr::GetTB_MAZEREWARD_ITEM(unsigned int index) {
        const auto it = m_mapTB_MAZEREWARD_ITEM.find(index);
        return it == m_mapTB_MAZEREWARD_ITEM.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MAZEREWARD_ITEM(unsigned int index, const TB_MAZEREWARD_ITEM& row) {
        m_mapTB_MAZEREWARD_ITEM[index] = row;
    }

std::int64_t XResourceMgr::LoadTBMazeRewardItemDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Maze_ID], [Mob_Rate], [Box_ID], [B_F_ID], [B_F_Rate], [B_C_ID], [B_C_Rate], [B_B_ID], [B_B_Rate], [B_A_ID], [B_A_Rate], [B_S_ID], [B_S_Rate], [B_SS_ID], [B_SS_Rate], [B_F_Min], [B_F_Max], [B_C_Min], [B_C_Max], [B_B_Min], [B_B_Max], [B_A_Min], [B_A_Max], [B_S_Min], [B_S_Max], [B_SS_Min], [B_SS_Max], [Cash_Reward_item_ID], [Cash_Reward_item_EA_1], [Cash_Reward_item_ID_2], [Cash_Reward_item_EA_2] from tb_MazeReward_Item")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MAZEREWARD_ITEM.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MAZEREWARD_ITEM row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Maze_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Mob_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Box_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.B_F_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.B_F_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.B_C_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.B_C_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.B_B_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.B_B_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.B_A_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.B_A_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.B_S_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.B_S_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.B_SS_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.B_SS_Rate, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.B_F_Min, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.B_F_Max, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.B_C_Min, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.B_C_Max, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.B_B_Min, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.B_B_Max, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.B_A_Min, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.B_A_Max, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.B_S_Min, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.B_S_Max, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.B_SS_Min, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.B_SS_Max, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Cash_Reward_item_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Cash_Reward_item_EA_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Cash_Reward_item_ID_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Cash_Reward_item_EA_2, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MAZEREWARD_ITEM(row.Maze_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
