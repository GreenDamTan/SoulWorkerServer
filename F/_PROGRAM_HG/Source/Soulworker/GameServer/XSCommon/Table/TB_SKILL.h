// 中文说明：
// 1. 本文件承接 TB_SKILL 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_SKILL {
    unsigned int Skill_Index = 0;
    char Skill_NameS[511] = {};
    unsigned int Skill_Group = 0;
    unsigned int Swap_Skill_Index = 0;
    std::uint8_t Skill_LV = 0;
    std::uint8_t Use_Position = 0;
    std::uint8_t Use_Class = 0;
    std::uint8_t Req_Min_LV = 0;
    std::uint8_t Req_Min_AwakeningGrade = 0;
    std::uint8_t Req_Skill_Point = 0;
    std::uint8_t Skill_Max_Level = 0;
    std::uint8_t Req_Consume_Point = 0;
    unsigned int Req_Prev_Skill_ID = 0;
    unsigned int Build_Up_Skill_ID = 0;
    unsigned int Div_GroupID_01 = 0;
    unsigned int Div_GroupID_02 = 0;
    unsigned int Prev_Skill_ID = 0;
    std::uint8_t Skill_SkipPriority_Group = 0;
    std::uint8_t Skill_Type = 0;
    std::uint8_t Passive_Type = 0;
    std::uint8_t Behavior_Condition = 0;
    std::uint16_t Passive_Value = 0;
    float Skill_Basic_Damage = 0.0f;
    float Skill_Add_Damage_INT = 0.0f;
    float Skill_Add_Damage_RAT = 0.0f;
    float Con_SG_Absorb = 0.0f;
    float SuperArmor_Attack = 0.0f;
    std::uint8_t Target_Damage_Count = 0;
    std::uint16_t Skill_Rate_Movement = 0;
    std::uint8_t Monster_Skill_Target_State = 0;
    std::uint8_t Skill_Attribute = 0;
    std::uint8_t Skill_Attribute_Sub = 0;
    std::uint8_t Skill_Cost_Attribute = 0;
    unsigned int Skill_Cost = 0;
    std::uint8_t Cost_Type = 0;
    std::uint8_t Skill_Direction = 0;
    std::uint8_t Control_Type = 0;
    std::uint8_t Charging_Count = 0;
    int Time_Value[4] = {};  // IDA: stored as int (milliseconds), converted to float (seconds) via *0.001f
    unsigned int Charging_Max_Value = 0;
    unsigned int Skill_Movement_Value_Min = 0;
    unsigned int Skill_Movement_Value_Max = 0;
    std::uint8_t Skill_Targeting_Max_Value = 0;
    std::uint16_t Skill_Range_Min = 0;
    std::uint16_t Skill_Range_Max = 0;
    std::uint8_t Skill_Angle_Value = 0;
    std::uint8_t Skill_Ratation_Type = 0;
    std::uint8_t Use_State = 0;
    std::uint8_t Normal_State = 0;
    std::uint8_t Collision_Check_Type = 0;
    std::uint8_t Action_Conditional_Activation = 0;
    unsigned int Next_Chain_Skill_ID = 0;
    std::uint8_t Next_Chain_Skill_Rate = 0;
    unsigned int CoolTime = 0;
    unsigned int CoolTime_Group = 0;
    std::uint8_t CoolTime_Save = 0;
    std::uint16_t CoolTime_Global = 0;
    std::uint16_t Bullet_Move_Time = 0;
    unsigned int Bullet_Life_Time = 0;
    std::uint8_t Bullet_Sync_Type = 0;
    std::uint8_t Agro_Type = 0;
    std::uint16_t Agro_Value = 0;
    std::uint16_t RankPoint_Value = 0;
    char Ani_Res_Start[255] = {};
    char Ani_Res_Loop[255] = {};
    char Ani_Res_Extra[255] = {};
    char Ani_Res_End[255] = {};
    std::uint8_t Attack_Action_link_Type = 0;
    unsigned int Ani_Loop_Count = 0;
    std::uint16_t Ani_Time = 0;
    std::uint8_t Skill_Motion_Skip_Type = 0;
    std::uint8_t Ani_Processing_Link_Type = 0;
    std::uint16_t Ani_Move_Distance = 0;
    std::uint8_t Res_Add_Type = 0;
    std::uint8_t Camera_Lock = 0;
    std::uint8_t Target_Type = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_SKILL) == 0x69D, "TB_SKILL size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_SKILL* GetTB_SKILL(unsigned int index) ;
    void SetTB_SKILL(unsigned int index, const TB_SKILL& row) ;
    const std::map<unsigned int, TB_SKILL>& GetAllTB_SKILL() const { return m_mapTB_SKILL; }
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBSkillDB() ;
    std::map<unsigned int, TB_SKILL> m_mapTB_SKILL;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_SKILL* XResourceMgr::GetTB_SKILL(unsigned int index) {
        const auto it = m_mapTB_SKILL.find(index);
        return it == m_mapTB_SKILL.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_SKILL(unsigned int index, const TB_SKILL& row) {
        m_mapTB_SKILL[index] = row;
    }

std::int64_t XResourceMgr::LoadTBSkillDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Skill_Index], [Skill_NameS], [Skill_Group], [Swap_Skill_Index], [Skill_LV], [Use_Position], [Use_Class], [Req_Min_LV], [Req_Min_AwakeningGrade], [Req_Skill_Point], [Skill_Max_Level], [Req_Consume_Point], [Req_Prev_Skill_ID], [Build_Up_Skill_ID], [Div_GroupID_01], [Div_GroupID_02], [Prev_Skill_ID], [Skill_SkipPriority_Group], [Skill_Type], [Passive_Type], [Behavior_Condition], [Passive_Value], [Skill_Basic_Damage], [Skill_Add_Damage_INT], [Skill_Add_Damage_RAT], [Con_SG_Absorb], [SuperArmor_Attack], [Target_Damage_Count], [Skill_Rate_Movement], [Monster_Skill_Target_State], [Skill_Attribute], [Skill_Attribute_Sub], [Skill_Cost_Attribute], [Skill_Cost], [Cost_Type], [Skill_Direction], [Control_Type], [Charging_Count], [Time_Value_01], [Time_Value_02], [Time_Value_03], [Time_Value_04], [Charging_Max_Value], [Skill_Movement_Value_Min], [Skill_Movement_Value_Max], [Skill_Targeting_Max_Value], [Skill_Range_Min], [Skill_Range_Max], [Skill_Angle_Value], [Skill_Ratation_Type], [Use_State], [Normal_State], [Collision_Check_Type], [Action_Conditional_Activation], [Next_Chain_Skill_ID], [Next_Chain_Skill_Rate], [CoolTime], [CoolTime_Group], [CoolTime_Save], [CoolTime_Global], [Bullet_Move_Time], [Bullet_Life_Time], [Bullet_Sync_Type], [Agro_Type], [Agro_Value], [RankPoint_Value], [Ani_Res_Start], [Ani_Res_Loop], [Ani_Res_Extra], [Ani_Res_End], [Attack_Action_link_Type], [Ani_Loop_Count], [Ani_Time], [Skill_Motion_Skip_Type], [Ani_Processing_Link_Type], [Ani_Move_Distance], [Res_Add_Type], [Camera_Lock], [Target_Type] from tb_Skill")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_SKILL.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_SKILL row{};
            const auto loadPackedFloat = [&](std::size_t offset) {
                float value = 0.0f;
                if (!GreenDamTan_DBGetFloat(xDBBinder, &value, &executeResult)) {
                    return false;
                }
                std::memcpy(reinterpret_cast<char*>(&row) + offset, &value, sizeof(value));
                return true;
            };
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Skill_Index, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Skill_NameS, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Skill_Group, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Swap_Skill_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Skill_LV, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Use_Position, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Use_Class, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Req_Min_LV, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Req_Min_AwakeningGrade, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Req_Skill_Point, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Skill_Max_Level, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Req_Consume_Point, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Req_Prev_Skill_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Build_Up_Skill_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Div_GroupID_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Div_GroupID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Prev_Skill_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Skill_SkipPriority_Group, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Skill_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Passive_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Behavior_Condition, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Passive_Value, &executeResult) ||
                !loadPackedFloat(offsetof(TB_SKILL, Skill_Basic_Damage)) ||
                !loadPackedFloat(offsetof(TB_SKILL, Skill_Add_Damage_INT)) ||
                !loadPackedFloat(offsetof(TB_SKILL, Skill_Add_Damage_RAT)) ||
                !loadPackedFloat(offsetof(TB_SKILL, Con_SG_Absorb)) ||
                !loadPackedFloat(offsetof(TB_SKILL, SuperArmor_Attack)) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Target_Damage_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Skill_Rate_Movement, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Skill_Target_State, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Skill_Attribute, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Skill_Attribute_Sub, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Skill_Cost_Attribute, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Skill_Cost, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Cost_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Skill_Direction, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Control_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Charging_Count, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 4; ++index) {
                if (!GreenDamTan_DBGetSigned(xDBBinder, &row.Time_Value[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Charging_Max_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Skill_Movement_Value_Min, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Skill_Movement_Value_Max, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Skill_Targeting_Max_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Skill_Range_Min, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Skill_Range_Max, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Skill_Angle_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Skill_Ratation_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Use_State, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Normal_State, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Collision_Check_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Action_Conditional_Activation, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Next_Chain_Skill_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Next_Chain_Skill_Rate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.CoolTime, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.CoolTime_Group, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.CoolTime_Save, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.CoolTime_Global, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Bullet_Move_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Bullet_Life_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Bullet_Sync_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Agro_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Agro_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.RankPoint_Value, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Ani_Res_Start, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Ani_Res_Loop, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Ani_Res_Extra, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Ani_Res_End, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Attack_Action_link_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ani_Loop_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ani_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Skill_Motion_Skip_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ani_Processing_Link_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Ani_Move_Distance, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Res_Add_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Camera_Lock, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Target_Type, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_SKILL(row.Skill_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
