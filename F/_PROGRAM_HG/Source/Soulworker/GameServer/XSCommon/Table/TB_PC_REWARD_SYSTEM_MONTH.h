// 中文说明：
// 1. 本文件承接 TB_PC_REWARD_SYSTEM_MONTH 的单表还原片段，字段与装载顺序按 LoginServer.pdb / IDA 保持一致。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 不要把 12 个 Month_Item_ID_* 字段擅自折叠成推测容器以外的结构，避免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_PC_REWARD_SYSTEM_MONTH {
    std::uint16_t Month_Reward_ID = 0;
    unsigned int Month_Item_ID_1 = 0;
    unsigned int Month_Item_ID_2 = 0;
    unsigned int Month_Item_ID_3 = 0;
    unsigned int Month_Item_ID_4 = 0;
    unsigned int Month_Item_ID_5 = 0;
    unsigned int Month_Item_ID_6 = 0;
    unsigned int Month_Item_ID_7 = 0;
    unsigned int Month_Item_ID_8 = 0;
    unsigned int Month_Item_ID_9 = 0;
    unsigned int Month_Item_ID_10 = 0;
    unsigned int Month_Item_ID_11 = 0;
    unsigned int Month_Item_ID_12 = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_PC_REWARD_SYSTEM_MONTH) == 0x32, "TB_PC_REWARD_SYSTEM_MONTH size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_PC_REWARD_SYSTEM_MONTH* GetTB_PC_REWARD_SYSTEM_MONTH(std::uint16_t index) ;
    void SetTB_PC_REWARD_SYSTEM_MONTH(std::uint16_t index, const TB_PC_REWARD_SYSTEM_MONTH& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBPCRewardSystemMonthDB() ;
    std::map<std::uint16_t, TB_PC_REWARD_SYSTEM_MONTH> m_mapTB_PC_REWARD_SYSTEM_MONTH;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_PC_REWARD_SYSTEM_MONTH* XResourceMgr::GetTB_PC_REWARD_SYSTEM_MONTH(std::uint16_t index) {
        const auto it = m_mapTB_PC_REWARD_SYSTEM_MONTH.find(index);
        return it == m_mapTB_PC_REWARD_SYSTEM_MONTH.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_PC_REWARD_SYSTEM_MONTH(std::uint16_t index, const TB_PC_REWARD_SYSTEM_MONTH& row) {
        m_mapTB_PC_REWARD_SYSTEM_MONTH[index] = row;
    }

std::int64_t XResourceMgr::LoadTBPCRewardSystemMonthDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Month_Reward_ID], [Month_Item_ID_1], [Month_Item_ID_2], [Month_Item_ID_3], [Month_Item_ID_4], [Month_Item_ID_5], [Month_Item_ID_6], [Month_Item_ID_7], [Month_Item_ID_8], [Month_Item_ID_9], [Month_Item_ID_10], [Month_Item_ID_11], [Month_Item_ID_12] from tb_PC_Reward_System_Month")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_PC_REWARD_SYSTEM_MONTH.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_PC_REWARD_SYSTEM_MONTH row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Month_Reward_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Month_Item_ID_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Month_Item_ID_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Month_Item_ID_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Month_Item_ID_4, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Month_Item_ID_5, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Month_Item_ID_6, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Month_Item_ID_7, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Month_Item_ID_8, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Month_Item_ID_9, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Month_Item_ID_10, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Month_Item_ID_11, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Month_Item_ID_12, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_PC_REWARD_SYSTEM_MONTH(row.Month_Reward_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
