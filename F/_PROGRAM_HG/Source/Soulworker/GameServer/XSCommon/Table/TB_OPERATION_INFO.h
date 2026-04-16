// 中文说明：
// 1. 本文件承接 TB_OPERATION_INFO 的单表还原片段，字段与装载顺序按 LoginServer.pdb / IDA 保持一致。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 不要把 8 组 Reward / Reward_Count / Reward_Score 与 8 组 User_SectorID / User_Start_Point 擅自折叠为推测结构体。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_OPERATION_INFO {
    unsigned int ID = 0;
    std::uint8_t Min_Member = 0;
    std::uint8_t Max_Member = 0;
    int Matching_Progress_Time = 0;
    int Matching_Confirm_Time = 0;
    int Matching_Wating_Time = 0;
    int HotTime_Start_1st = 0;
    int HotTime_End_1st = 0;
    int HotTime_Start_2nd = 0;
    int HotTime_End_2nd = 0;
    int HotTime_Start_3rd = 0;
    int HotTime_End_3rd = 0;
    int Reward_Attend = 0;
    int Reward_01 = 0;
    int Reward_02 = 0;
    int Reward_03 = 0;
    int Reward_04 = 0;
    int Reward_05 = 0;
    int Reward_06 = 0;
    int Reward_07 = 0;
    int Reward_08 = 0;
    int Reward_Count_01 = 0;
    int Reward_Count_02 = 0;
    int Reward_Count_03 = 0;
    int Reward_Count_04 = 0;
    int Reward_Count_05 = 0;
    int Reward_Count_06 = 0;
    int Reward_Count_07 = 0;
    int Reward_Count_08 = 0;
    int Reward_Score_01 = 0;
    int Reward_Score_02 = 0;
    int Reward_Score_03 = 0;
    int Reward_Score_04 = 0;
    int Reward_Score_05 = 0;
    int Reward_Score_06 = 0;
    int Reward_Score_07 = 0;
    int Reward_Score_08 = 0;
    std::uint16_t User_SectorID_1 = 0;
    std::uint16_t User_SectorID_2 = 0;
    std::uint16_t User_SectorID_3 = 0;
    std::uint16_t User_SectorID_4 = 0;
    std::uint16_t User_SectorID_5 = 0;
    std::uint16_t User_SectorID_6 = 0;
    std::uint16_t User_SectorID_7 = 0;
    std::uint16_t User_SectorID_8 = 0;
    int User_Start_Point_1 = 0;
    int User_Start_Point_2 = 0;
    int User_Start_Point_3 = 0;
    int User_Start_Point_4 = 0;
    int User_Start_Point_5 = 0;
    int User_Start_Point_6 = 0;
    int User_Start_Point_7 = 0;
    int User_Start_Point_8 = 0;
    int User_Kill_Point = 0;
    unsigned int Ranking_Before_Reward = 0;
    unsigned int Ranking_Now_Reward = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_OPERATION_INFO) == 0xCA, "TB_OPERATION_INFO size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_OPERATION_INFO* GetTB_OPERATION_INFO(unsigned int index) ;
    void SetTB_OPERATION_INFO(unsigned int index, const TB_OPERATION_INFO& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBOperationInfoDB() ;
    std::map<unsigned int, TB_OPERATION_INFO> m_mapTB_OPERATION_INFO;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_OPERATION_INFO* XResourceMgr::GetTB_OPERATION_INFO(unsigned int index) {
        const auto it = m_mapTB_OPERATION_INFO.find(index);
        return it == m_mapTB_OPERATION_INFO.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_OPERATION_INFO(unsigned int index, const TB_OPERATION_INFO& row) {
        m_mapTB_OPERATION_INFO[index] = row;
    }

std::int64_t XResourceMgr::LoadTBOperationInfoDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Min_Member], [Max_Member], [Matching_Progress_Time], [Matching_Confirm_Time], [Matching_Wating_Time], [HotTime_Start_1st], [HotTime_End_1st], [HotTime_Start_2nd], [HotTime_End_2nd], [HotTime_Start_3rd], [HotTime_End_3rd], [Reward_Attend], [Reward_01], [Reward_Count_01], [Reward_Score_01], [Reward_02], [Reward_Count_02], [Reward_Score_02], [Reward_03], [Reward_Count_03], [Reward_Score_03], [Reward_04], [Reward_Count_04], [Reward_Score_04], [Reward_05], [Reward_Count_05], [Reward_Score_05], [Reward_06], [Reward_Count_06], [Reward_Score_06], [Reward_07], [Reward_Count_07], [Reward_Score_07], [Reward_08], [Reward_Count_08], [Reward_Score_08], [User_SectorID_1], [User_Start_Point_1], [User_SectorID_2], [User_Start_Point_2], [User_SectorID_3], [User_Start_Point_3], [User_SectorID_4], [User_Start_Point_4], [User_SectorID_5], [User_Start_Point_5], [User_SectorID_6], [User_Start_Point_6], [User_SectorID_7], [User_Start_Point_7], [User_SectorID_8], [User_Start_Point_8], [User_Kill_Point], [Ranking_Before_Reward], [Ranking_Now_Reward] from tb_Operation_Info")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_OPERATION_INFO.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_OPERATION_INFO row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Min_Member, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Max_Member, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Matching_Progress_Time, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Matching_Confirm_Time, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Matching_Wating_Time, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.HotTime_Start_1st, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.HotTime_End_1st, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.HotTime_Start_2nd, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.HotTime_End_2nd, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.HotTime_Start_3rd, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.HotTime_End_3rd, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_Attend, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_01, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_Count_01, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_Score_01, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_02, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_Count_02, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_Score_02, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_03, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_Count_03, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_Score_03, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_04, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_Count_04, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_Score_04, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_05, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_Count_05, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_Score_05, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_06, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_Count_06, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_Score_06, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_07, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_Count_07, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_Score_07, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_08, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_Count_08, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Reward_Score_08, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.User_SectorID_1, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.User_Start_Point_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.User_SectorID_2, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.User_Start_Point_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.User_SectorID_3, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.User_Start_Point_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.User_SectorID_4, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.User_Start_Point_4, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.User_SectorID_5, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.User_Start_Point_5, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.User_SectorID_6, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.User_Start_Point_6, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.User_SectorID_7, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.User_Start_Point_7, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.User_SectorID_8, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.User_Start_Point_8, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.User_Kill_Point, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ranking_Before_Reward, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ranking_Now_Reward, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_OPERATION_INFO(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
