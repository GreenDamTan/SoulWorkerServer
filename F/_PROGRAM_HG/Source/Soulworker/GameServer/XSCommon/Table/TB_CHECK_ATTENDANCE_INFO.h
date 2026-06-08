// 中文说明：
// 1. 本文件承接 TB_CHECK_ATTENDANCE_INFO 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_CHECK_ATTENDANCE_INFO {
    unsigned int ID = 0;
    unsigned int Check_Attendance_Type = 0;
    unsigned int Check_Attendance_Num = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_CHECK_ATTENDANCE_INFO) == 0xC, "TB_CHECK_ATTENDANCE_INFO size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_CHECK_ATTENDANCE_INFO* GetTB_CHECK_ATTENDANCE_INFO(unsigned int index) ;
    void SetTB_CHECK_ATTENDANCE_INFO(unsigned int index, const TB_CHECK_ATTENDANCE_INFO& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBCheckAttendanceInfoDB() ;
    // IDA-verified member variable name from XResourceMgr
    std::map<unsigned int, TB_CHECK_ATTENDANCE_INFO> m_mapTB_CHECK_ATTENDANCE_INFO;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_CHECK_ATTENDANCE_INFO* XResourceMgr::GetTB_CHECK_ATTENDANCE_INFO(unsigned int index) {
        auto it = m_mapTB_CHECK_ATTENDANCE_INFO.find(index);
        if (it == m_mapTB_CHECK_ATTENDANCE_INFO.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_CHECK_ATTENDANCE_INFO(unsigned int index, const TB_CHECK_ATTENDANCE_INFO& row) {
        m_mapTB_CHECK_ATTENDANCE_INFO[index] = row;
    }

std::int64_t XResourceMgr::LoadTBCheckAttendanceInfoDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(
            const_cast<char*>("select [ID], [Check_Attendance_Type], [Check_Attendance_Num] from tb_Check_Attendance_Info")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_CHECK_ATTENDANCE_INFO.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_CHECK_ATTENDANCE_INFO row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Check_Attendance_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Check_Attendance_Num, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_CHECK_ATTENDANCE_INFO(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
