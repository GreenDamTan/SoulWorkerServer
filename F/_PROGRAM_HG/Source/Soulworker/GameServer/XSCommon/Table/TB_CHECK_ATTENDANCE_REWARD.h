// 中文说明：
// 1. 本文件承接 TB_CHECK_ATTENDANCE_REWARD 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_CHECK_ATTENDANCE_REWARD {
    unsigned int ID = 0;
    unsigned int Link_ID = 0;
    unsigned int Attendance_Reward_Gold = 0;
    unsigned int Attendance_Reward_Ether = 0;
    unsigned int Attendance_Item_Reward_ID[5] = {};
    unsigned int Attendance_Item_Reward_Num[5] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_CHECK_ATTENDANCE_REWARD) == 0x38, "TB_CHECK_ATTENDANCE_REWARD size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_CHECK_ATTENDANCE_REWARD* GetTB_CHECK_ATTENDANCE_REWARD(unsigned int index) ;
    void SetTB_CHECK_ATTENDANCE_REWARD(unsigned int index, const TB_CHECK_ATTENDANCE_REWARD& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBCheckAttendanceRewardDB() ;
    std::unordered_map<unsigned int, TB_CHECK_ATTENDANCE_REWARD> checkAttendanceRewardRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_CHECK_ATTENDANCE_REWARD* XResourceMgr::GetTB_CHECK_ATTENDANCE_REWARD(unsigned int index) {
        auto it = checkAttendanceRewardRows_.find(index);
        if (it == checkAttendanceRewardRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_CHECK_ATTENDANCE_REWARD(unsigned int index, const TB_CHECK_ATTENDANCE_REWARD& row) {
        checkAttendanceRewardRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBCheckAttendanceRewardDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Link_ID], [Attendance_Reward_Gold], [Attendance_Reward_Ether], [Attendance_Item_Reward_ID_1], [Attendance_Item_Reward_Num_1], [Attendance_Item_Reward_ID_2], [Attendance_Item_Reward_Num_2], [Attendance_Item_Reward_ID_3], [Attendance_Item_Reward_Num_3], [Attendance_Item_Reward_ID_4], [Attendance_Item_Reward_Num_4], [Attendance_Item_Reward_ID_5], [Attendance_Item_Reward_Num_5] from tb_Check_Attendance_Reward")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        checkAttendanceRewardRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_CHECK_ATTENDANCE_REWARD row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Link_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Attendance_Reward_Gold, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Attendance_Reward_Ether, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 5; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Attendance_Item_Reward_ID[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Attendance_Item_Reward_Num[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_CHECK_ATTENDANCE_REWARD(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
