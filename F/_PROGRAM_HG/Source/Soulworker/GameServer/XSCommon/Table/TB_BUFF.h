// 中文说明：
// 1. 本文件承接 TB_BUFF 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_BUFF {
    std::uint16_t Buff_Index = 0;
    std::uint8_t AllowLocation_Type = 0;
    std::uint8_t System_Type = 0;
    std::uint8_t Buff_Type = 0;
    std::uint16_t EffectType_01 = 0;
    std::uint16_t Buff_Group = 0;
    int Buff_Success_Rate = 0;
    std::uint8_t Buff_Grade = 0;
    unsigned int Buff_Time = 0;
    unsigned int StandBy_Time = 0;
    unsigned int DotDelay_Time = 0;
    std::uint8_t Overlap_Type = 0;
    std::uint8_t Overlap_Count = 0;
    std::uint8_t Delete_Type = 0;
    char SetBuffActionName[255] = {};
    std::uint16_t Call_Out_Buff = 0;
    unsigned int Call_Out_Skill = 0;
    std::uint16_t Hit_Call_Buff = 0;
    std::uint16_t Change_Buff = 0;
    std::uint8_t EffectType_Status_01 = 0;
    int Option_Value_01 = 0;
    std::uint8_t EffectType_Status_02 = 0;
    int Option_Value_02 = 0;
    std::uint8_t EffectType_Status_03 = 0;
    int Option_Value_03 = 0;
    std::uint8_t Buff_Send_Type = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_BUFF) == 0x136, "TB_BUFF size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_BUFF* GetTB_BUFF(std::uint16_t index) ;
    void SetTB_BUFF(std::uint16_t index, const TB_BUFF& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBBuffDB() ;
    std::map<std::uint16_t, TB_BUFF> m_mapTB_BUFF;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_BUFF* XResourceMgr::GetTB_BUFF(std::uint16_t index) {
        const auto it = m_mapTB_BUFF.find(index);
        return it == m_mapTB_BUFF.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_BUFF(std::uint16_t index, const TB_BUFF& row) {
        m_mapTB_BUFF[index] = row;
    }

std::int64_t XResourceMgr::LoadTBBuffDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Buff_Index], [AllowLocation_Type], [System_Type], [Buff_Type], [EffectType_01], [Buff_Group], [Buff_Success_Rate], [Buff_Grade], [Buff_Time], [StandBy_Time], [DotDelay_Time], [Overlap_Type], [Overlap_Count], [Delete_Type], [SetBuffActionName], [Call_Out_Buff], [Call_Out_Skill], [Hit_Call_Buff], [Change_Buff], [EffectType_Status_01], [Option_Value_01], [EffectType_Status_02], [Option_Value_02], [EffectType_Status_03], [Option_Value_03], [Buff_Send_Type] from tb_Buff")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_BUFF.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_BUFF row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Buff_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.AllowLocation_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.System_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Buff_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.EffectType_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Buff_Group, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Buff_Success_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Buff_Grade, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Buff_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.StandBy_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.DotDelay_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Overlap_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Overlap_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Delete_Type, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.SetBuffActionName, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Call_Out_Buff, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Call_Out_Skill, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Hit_Call_Buff, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Change_Buff, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.EffectType_Status_01, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Option_Value_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.EffectType_Status_02, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Option_Value_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.EffectType_Status_03, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Option_Value_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Buff_Send_Type, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_BUFF(row.Buff_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
