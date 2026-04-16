// 中文说明：
// 1. 本文件承接 TB_MODE_OPERATION 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_MODE_OPERATION {
    std::uint8_t ID = 0;
    unsigned int Taget_Monster = 0;
    std::uint8_t Start_Condition[5] = {};
    std::uint16_t OP_Mode_Maze_ID = 0;
    std::uint16_t OP_Mode_Sector = 0;
    unsigned int OP_Mode_Time = 0;
    unsigned int OP_Mode_Start_String = 0;
    std::uint16_t OP_Mode_After_time[3] = {};
    unsigned int OP_Mode_After_time_String[3] = {};
    unsigned int OP_Mode_Success_Taget = 0;
    unsigned int OP_Mode_Success_Buff = 0;
    unsigned int OP_Mode_Fail_Buff = 0;
    unsigned int OP_Mode_Object[5] = {};
    unsigned int Success_Message = 0;
    unsigned int Fail_Message = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_MODE_OPERATION) == 0x50, "TB_MODE_OPERATION size must match decompile layout");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MODE_OPERATION* GetTB_MODE_OPERATION(std::uint8_t index) ;
    void SetTB_MODE_OPERATION(std::uint8_t index, const TB_MODE_OPERATION& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBModeOperationDB() ;
    std::map<std::uint8_t, TB_MODE_OPERATION> m_mapTB_MODE_OPERATION;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MODE_OPERATION* XResourceMgr::GetTB_MODE_OPERATION(std::uint8_t index) {
        const auto it = m_mapTB_MODE_OPERATION.find(index);
        return it == m_mapTB_MODE_OPERATION.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MODE_OPERATION(std::uint8_t index, const TB_MODE_OPERATION& row) {
        m_mapTB_MODE_OPERATION[index] = row;
    }

std::int64_t XResourceMgr::LoadTBModeOperationDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Taget_Monster], [Start_Condition_1], [Start_Condition_2], [Start_Condition_3], [Start_Condition_4], [Start_Condition_5], [OP_Mode_Maze_ID], [OP_Mode_Sector], [OP_Mode_Time], [OP_Mode_Start_String], [OP_Mode_After_time_1], [OP_Mode_After_time_String_1], [OP_Mode_After_time_2], [OP_Mode_After_time_String_2], [OP_Mode_After_time_3], [OP_Mode_After_time_String_3], [OP_Mode_Success_Taget], [OP_Mode_Success_Buff], [OP_Mode_Fail_Buff], [OP_Mode_Object_1], [OP_Mode_Object_2], [OP_Mode_Object_3], [OP_Mode_Object_4], [OP_Mode_Object_5], [Success_Message], [Fail_Message] from tb_Mode_Operation")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MODE_OPERATION.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MODE_OPERATION row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Taget_Monster, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 5; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Start_Condition[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.OP_Mode_Maze_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.OP_Mode_Sector, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.OP_Mode_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.OP_Mode_Start_String, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 3; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.OP_Mode_After_time[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.OP_Mode_After_time_String[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.OP_Mode_Success_Taget, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.OP_Mode_Success_Buff, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.OP_Mode_Fail_Buff, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 5; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.OP_Mode_Object[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Success_Message, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Fail_Message, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MODE_OPERATION(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
