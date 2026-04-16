// 中文说明：
// 1. 本文件承接 TB_MODE_BI_SECTOR_INFO 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_MODE_BI_SECTOR_INFO {
    unsigned int ID = 0;
    std::uint8_t BI_Sector_Type = 0;
    std::uint8_t BI_Sector_Score = 0;
    std::uint8_t BI_Sector_Start_Message_Type = 0;
    unsigned int BI_Sector_Start_Message = 0;
    unsigned int BI_Sector_Value = 0;
    std::uint8_t BI_Sector_Value_Type = 0;
    unsigned int BI_Sector_Value_Massage = 0;
    std::uint8_t BI_Sector_End_Message_Type = 0;
    unsigned int BI_Sector_End_Message = 0;
    unsigned int BI_Sector_Bind_Maze = 0;
    unsigned int BI_Sector_Upgrade_Group = 0;
    std::uint8_t BI_Sector_Upgrade_Qty = 0;
    std::uint8_t BI_Sector_Pocket_Qty = 0;
    std::uint16_t BI_Sector_Reward_Item_Qty = 0;
    unsigned int BI_Sector_Clear_Random_Stat = 0;
    unsigned int BI_Portal_Buff_Use = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_MODE_BI_SECTOR_INFO) == 0x2D, "TB_MODE_BI_SECTOR_INFO size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MODE_BI_SECTOR_INFO* GetTB_MODE_BI_SECTOR_INFO(unsigned int index) ;
    void SetTB_MODE_BI_SECTOR_INFO(unsigned int index, const TB_MODE_BI_SECTOR_INFO& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBModeBISectorInfoDB() ;
    std::map<unsigned int, TB_MODE_BI_SECTOR_INFO> m_mapTB_MODE_BI_SECTOR_INFO;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MODE_BI_SECTOR_INFO* XResourceMgr::GetTB_MODE_BI_SECTOR_INFO(unsigned int index) {
        const auto it = m_mapTB_MODE_BI_SECTOR_INFO.find(index);
        return it == m_mapTB_MODE_BI_SECTOR_INFO.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MODE_BI_SECTOR_INFO(unsigned int index, const TB_MODE_BI_SECTOR_INFO& row) {
        m_mapTB_MODE_BI_SECTOR_INFO[index] = row;
    }

std::int64_t XResourceMgr::LoadTBModeBISectorInfoDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [BI_Sector_Type], [BI_Sector_Score], [BI_Sector_Start_Message_Type], [BI_Sector_Start_Message], [BI_Sector_Value], [BI_Sector_Value_Type], [BI_Sector_Value_Massage], [BI_Sector_End_Message_Type], [BI_Sector_End_Message], [BI_Sector_Bind_Maze], [BI_Sector_Upgrade_Group], [BI_Sector_Upgrade_Qty], [BI_Sector_Pocket_Qty], [BI_Sector_Reward_Item_Qty], [BI_Sector_Clear_Random_Stat], [BI_Portal_Buff_Use] from tb_Mode_BI_Sector_Info")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MODE_BI_SECTOR_INFO.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MODE_BI_SECTOR_INFO row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_Sector_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_Sector_Score, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_Sector_Start_Message_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_Sector_Start_Message, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_Sector_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_Sector_Value_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_Sector_Value_Massage, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_Sector_End_Message_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_Sector_End_Message, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_Sector_Bind_Maze, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_Sector_Upgrade_Group, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_Sector_Upgrade_Qty, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_Sector_Pocket_Qty, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_Sector_Reward_Item_Qty, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_Sector_Clear_Random_Stat, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.BI_Portal_Buff_Use, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MODE_BI_SECTOR_INFO(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
