// 中文说明：
// 1. 本文件承接 TB_PC_REWARD_SYSTEM 的单表还原片段，字段与装载顺序按 LoginServer.pdb / IDA 保持一致。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 不要把 5 组 Achieve_Point / Achieve_Reward 字段擅自折叠成推测结构体，避免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_PC_REWARD_SYSTEM {
    unsigned int ID = 0;
    std::uint8_t PC_Reward_ISApply = 0;
    unsigned int Check_Type_Value = 0;
    std::uint8_t Apply_Level = 0;
    std::uint16_t Achieve_Point_1 = 0;
    unsigned int Achieve_Reward_1 = 0;
    std::uint16_t Achieve_Point_2 = 0;
    unsigned int Achieve_Reward_2 = 0;
    std::uint16_t Achieve_Point_3 = 0;
    unsigned int Achieve_Reward_3 = 0;
    std::uint16_t Achieve_Point_4 = 0;
    unsigned int Achieve_Reward_4 = 0;
    std::uint16_t Achieve_Point_5 = 0;
    unsigned int Achieve_Reward_5 = 0;
    std::uint8_t SysMail_ID = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_PC_REWARD_SYSTEM) == 0x29, "TB_PC_REWARD_SYSTEM size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_PC_REWARD_SYSTEM* GetTB_PC_REWARD_SYSTEM(unsigned int index) ;
    void SetTB_PC_REWARD_SYSTEM(unsigned int index, const TB_PC_REWARD_SYSTEM& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBPCRewardSystemDB() ;
    std::map<unsigned int, TB_PC_REWARD_SYSTEM> m_mapTB_PC_REWARD_SYSTEM;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_PC_REWARD_SYSTEM* XResourceMgr::GetTB_PC_REWARD_SYSTEM(unsigned int index) {
        const auto it = m_mapTB_PC_REWARD_SYSTEM.find(index);
        return it == m_mapTB_PC_REWARD_SYSTEM.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_PC_REWARD_SYSTEM(unsigned int index, const TB_PC_REWARD_SYSTEM& row) {
        m_mapTB_PC_REWARD_SYSTEM[index] = row;
    }

std::int64_t XResourceMgr::LoadTBPCRewardSystemDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [PC_Reward_ISApply], [Check_Type_Value], [Apply_Level], [Achieve_Point_1], [Achieve_Reward_1], [Achieve_Point_2], [Achieve_Reward_2], [Achieve_Point_3], [Achieve_Reward_3], [Achieve_Point_4], [Achieve_Reward_4], [Achieve_Point_5], [Achieve_Reward_5], [SysMail_ID] from tb_PC_Reward_System")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_PC_REWARD_SYSTEM.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_PC_REWARD_SYSTEM row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.PC_Reward_ISApply, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Check_Type_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Apply_Level, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Achieve_Point_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Achieve_Reward_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Achieve_Point_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Achieve_Reward_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Achieve_Point_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Achieve_Reward_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Achieve_Point_4, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Achieve_Reward_4, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Achieve_Point_5, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Achieve_Reward_5, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.SysMail_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_PC_REWARD_SYSTEM(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
