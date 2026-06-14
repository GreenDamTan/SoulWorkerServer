// 中文说明：
// 1. 本文件承接 TB_MONSTER 的单表还原片段，字段与装载顺序按 LoginServer.pdb / IDA 严格保持一致。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 不要把 Monster_Switching_*、Monster_Skill*、各类 INT 属性字段随意折叠、删减或改名，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_MONSTER {
    unsigned int ID = 0;
    char Monster_Code_Name[511] = {};
    char Monster_Name_D[511] = {};
    unsigned int Monster_Parts_ID_02 = 0;
    unsigned int Monster_Parts_ID_03 = 0;
    std::uint8_t Summon_Hit = 0;
    std::uint8_t Monster_Faction = 0;
    unsigned int Quest_Group_1 = 0;
    unsigned int Quest_Group_2 = 0;
    unsigned int Quest_Group_3 = 0;
    unsigned int Quest_Group_4 = 0;
    unsigned int Quest_Group_5 = 0;
    unsigned int Mission_Group_1 = 0;
    unsigned int Mission_Group_2 = 0;
    unsigned int Mission_Group_3 = 0;
    std::uint8_t Monster_Lv = 0;
    float Exp = 0.0f;
    unsigned int Moster_Kill_Score = 0;
    std::uint8_t Monster_Rank = 0;
    std::uint8_t Monster_Type = 0;
    std::uint8_t Monster_Flag = 0;
    std::uint8_t Monster_ClearCondition_Type = 0;
    std::uint8_t Object_MaxDamage_Value = 0;
    char Object_Default_Ani_File[511] = {};
    char Object_Open_Ani_File[511] = {};
    std::uint8_t Monster_Element = 0;
    std::uint8_t Monster_Aggression_Type = 0;
    unsigned int Melee_Skill_Inclination = 0;
    unsigned int Range_Skill_Inclination = 0;
    unsigned int Heal_Skill_Inclination = 0;
    unsigned int Other_Skill_Inclination = 0;
    float Protection_Aggro_Ratio = 0.0f;
    unsigned int Monster_Sight = 0;
    std::uint8_t Monster_WeightRank = 0;
    std::uint8_t Monster_Defence_Type = 0;
    std::uint8_t Monster_DMGAction_Type = 0;
    std::uint8_t Monster_HeadAction_Type = 0;
    std::uint8_t Monster_Defence_Direction_Type = 0;
    std::uint16_t Monster_Walk_Speed = 0;
    std::uint16_t Monster_Run_Speed = 0;
    std::uint16_t Monster_Turn_Speed = 0;
    std::uint16_t Monster_Gaze_Speed = 0;
    std::uint8_t Monster_Status_Type = 0;
    unsigned int Monster_Status_ID = 0;
    std::uint16_t Monster_Weapon_ID = 0;
    std::uint8_t Monster_BattleMode_Type = 0;
    char Monster_AI_ID[511] = {};
    std::uint8_t Monster_AI_Type = 0;
    std::uint8_t Monster_Death_Type = 0;
    char Monster_Special_Damage[511] = {};
    std::uint8_t Monster_Default_Action_Type = 0;
    std::int16_t Monster_NormalStand_Type = 0;
    std::uint8_t Monster_Default_Action_Type_01 = 0;
    std::uint8_t Monster_Default_Action_Type_02 = 0;
    std::uint8_t Monster_Default_Action_Type_03 = 0;
    std::int16_t Monster_NormalStand_Type_01 = 0;
    char Monster_Switching_AI_01[511] = {};
    char Monster_Switching_AI_02[511] = {};
    char Monster_Switching_AI_03[511] = {};
    std::uint8_t Monster_AI_Type_01 = 0;
    std::uint8_t Monster_AI_Type_02 = 0;
    std::uint8_t Monster_AI_Type_03 = 0;
    char Monster_Special_Damage_01[511] = {};
    char Monster_Special_Damage_02[511] = {};
    char Monster_Special_Damage_03[511] = {};
    std::uint8_t Monster_Switching_AI_Condition_01 = 0;
    std::uint8_t Monster_Switching_AI_Condition_02 = 0;
    std::uint8_t Monster_Switching_AI_Condition_03 = 0;
    unsigned int Monster_Switching_AI_Value_01 = 0;
    unsigned int Monster_Switching_AI_Value2_02 = 0;
    unsigned int Monster_Switching_AI_Value_03 = 0;
    std::uint8_t Monster_Switching_AI_Step_01 = 0;
    std::uint8_t Monster_Switching_AI_Step_02 = 0;
    std::uint8_t Monster_Switching_AI_Step_03 = 0;
    char Monster_Switching_Action_01[511] = {};
    char Monster_Switching_Action_02[511] = {};
    char Monster_Switching_Action_03[511] = {};
    std::int16_t Monster_NormalStand_Type_02 = 0;
    std::int16_t Monster_NormalStand_Type_03 = 0;
    unsigned int Monster_Skill1_ID = 0;
    unsigned int Monster_Skill2_ID = 0;
    unsigned int Monster_Skill3_ID = 0;
    unsigned int Monster_Skill4_ID = 0;
    unsigned int Monster_Skill5_ID = 0;
    unsigned int Monster_Skill6_ID = 0;
    unsigned int Monster_Skill7_ID = 0;
    unsigned int Monster_Skill8_ID = 0;
    unsigned int Monster_Skill9_ID = 0;
    unsigned int Monster_Skill10_ID = 0;
    unsigned int Monster_Drop_ID = 0;
    unsigned int Monster_Hit_Drop_ID = 0;
    float Monster_Scale = 0.0f;
    std::uint16_t Monster_CollisionRadius = 0;
    std::uint16_t Monster_CollisionHeight = 0;
    std::uint16_t Monster_RealHeight = 0;
    unsigned int Monster_Party_Revise = 0;
    std::uint8_t Monster_SA_Type = 0;
    int HP_Max_INT_Value = 0;
    int SG_Max_INT_Value = 0;
    int PATK_Max_INT_Value = 0;
    int MATK_Max_INT_Value = 0;
    int PDPR_RAT_Value = 0;
    int MDPR_RAT_Value = 0;
    int PDEF_INT_Value = 0;
    int MDEF_INT_Value = 0;
    int PAR_INT_Value = 0;
    int MAR_INT_Value = 0;
    int PARP_INT_Value = 0;
    int MARP_INT_Value = 0;
    int ADR_INT_Value = 0;
    int PCP_INT_Value = 0;
    int MCP_INT_Value = 0;
    int PCRP_INT_Value = 0;
    int MCRP_INT_Value = 0;
    int PCA_INT_Value = 0;
    int MCA_INT_Value = 0;
    int DAR_INT_Value = 0;
    int PDSR_INT_Value = 0;
    int MDSR_INT_Value = 0;
    int AtkPower_Light_INT_Value = 0;
    int AtkPower_Dark_INT_Value = 0;
    int AtkPower_Blue_INT_Value = 0;
    int AtkPower_Red_INT_Value = 0;
    int AtkPower_White_INT_Value = 0;
    int AtkPower_Green_INT_Value = 0;
    int Resist_Light_INT_Value = 0;
    int Resist_Dark_INT_Value = 0;
    int Resist_Blue_INT_Value = 0;
    int Resist_Red_INT_Value = 0;
    int Resist_White_INT_Value = 0;
    int Resist_Green_INT_Value = 0;
    int Res_Burn_INT_Value = 0;
    int Res_Poision_INT_Value = 0;
    int Res_Shcok_INT_Value = 0;
    int Res_Bleed_INT_Value = 0;
    int Res_Stun_INT_Value = 0;
    int Res_Paralysis_INT_Value = 0;
    int Res_Sleep_INT_Value = 0;
    int Res_Freeze_INT_Value = 0;
    int Res_Charm_INT_Value = 0;
    int Res_Confusion_INT_Value = 0;
    int Res_Silence_INT_Value = 0;
    int Res_Weak_INT_Value = 0;
    std::uint8_t IS_Item_Rate = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_MONSTER) == 0x1F77, "TB_MONSTER size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_MONSTER* GetTB_MONSTER(unsigned int index) ;
    void SetTB_MONSTER(unsigned int index, const TB_MONSTER& row) ;
    // Public iterator access for table traversal
    auto& GetMonsterRows() { return m_mapTB_MONSTER; }
    const auto& GetMonsterRows() const { return m_mapTB_MONSTER; }
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBMonsterDB() ;
    std::map<unsigned int, TB_MONSTER> m_mapTB_MONSTER;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_MONSTER* XResourceMgr::GetTB_MONSTER(unsigned int index) {
        const auto it = m_mapTB_MONSTER.find(index);
        return it == m_mapTB_MONSTER.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_MONSTER(unsigned int index, const TB_MONSTER& row) {
        m_mapTB_MONSTER[index] = row;
    }

std::int64_t XResourceMgr::LoadTBMonsterDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Monster_Code_Name], [Monster_Name_D], [Monster_Parts_ID_02], [Monster_Parts_ID_03], [Summon_Hit], [Monster_Faction], [Quest_Group_1], [Quest_Group_2], [Quest_Group_3], [Quest_Group_4], [Quest_Group_5], [Mission_Group_1], [Mission_Group_2], [Mission_Group_3], [Monster_Lv], [Exp], [Moster_Kill_Score], [Monster_Rank], [Monster_Type], [Monster_Flag], [Monster_ClearCondition_Type], [Object_MaxDamage_Value], [Object_Default_Ani_File], [Object_Open_Ani_File], [Monster_Element], [Monster_Aggression_Type], [Melee_Skill_Inclination], [Range_Skill_Inclination], [Heal_Skill_Inclination], [Other_Skill_Inclination], [Protection_Aggro_Ratio], [Monster_Sight], [Monster_WeightRank], [Monster_Defence_Type], [Monster_DMGAction_Type], [Monster_HeadAction_Type], [Monster_Defence_Direction_Type], [Monster_Walk_Speed], [Monster_Run_Speed], [Monster_Turn_Speed], [Monster_Gaze_Speed], [Monster_Status_Type], [Monster_Status_ID], [Monster_Weapon_ID], [Monster_BattleMode_Type], [Monster_AI_ID], [Monster_AI_Type], [Monster_Death_Type], [Monster_Special_Damage], [Monster_Default_Action_Type], [Monster_NormalStand_Type], [Monster_Default_Action_Type_01], [Monster_NormalStand_Type_01], [Monster_Switching_AI_01], [Monster_AI_Type_01], [Monster_Special_Damage_01], [Monster_Switching_AI_Condition_01], [Monster_Switching_AI_Value_01], [Monster_Switching_AI_Step_01], [Monster_Switching_Action_01], [Monster_Default_Action_Type_02], [Monster_NormalStand_Type_02], [Monster_Switching_AI_02], [Monster_AI_Type_02], [Monster_Special_Damage_02], [Monster_Switching_AI_Condition_02], [Monster_Switching_AI_Value2_02], [Monster_Switching_AI_Step_02], [Monster_Switching_Action_02], [Monster_Default_Action_Type_03], [Monster_NormalStand_Type_03], [Monster_Switching_AI_03], [Monster_AI_Type_03], [Monster_Special_Damage_03], [Monster_Switching_AI_Condition_03], [Monster_Switching_AI_Value_03], [Monster_Switching_AI_Step_03], [Monster_Switching_Action_03], [Monster_Skill1_ID], [Monster_Skill2_ID], [Monster_Skill3_ID], [Monster_Skill4_ID], [Monster_Skill5_ID], [Monster_Skill6_ID], [Monster_Skill7_ID], [Monster_Skill8_ID], [Monster_Skill9_ID], [Monster_Skill10_ID], [Monster_Drop_ID], [Monster_Hit_Drop_ID], [Monster_Scale], [Monster_CollisionRadius], [Monster_CollisionHeight], [Monster_RealHeight], [Monster_Party_Revise], [Monster_SA_Type], [HP_Max_INT_Value], [SG_Max_INT_Value], [PATK_Max_INT_Value], [MATK_Max_INT_Value], [PDPR_RAT_Value], [MDPR_RAT_Value], [PDEF_INT_Value], [MDEF_INT_Value], [PAR_INT_Value], [MAR_INT_Value], [PARP_INT_Value], [MARP_INT_Value], [ADR_INT_Value], [PCP_INT_Value], [MCP_INT_Value], [PCRP_INT_Value], [MCRP_INT_Value], [PCA_INT_Value], [MCA_INT_Value], [DAR_INT_Value], [PDSR_INT_Value], [MDSR_INT_Value], [AtkPower_Light_INT_Value], [AtkPower_Dark_INT_Value], [AtkPower_Blue_INT_Value], [AtkPower_Red_INT_Value], [AtkPower_White_INT_Value], [AtkPower_Green_INT_Value], [Resist_Light_INT_Value], [Resist_Dark_INT_Value], [Resist_Blue_INT_Value], [Resist_Red_INT_Value], [Resist_White_INT_Value], [Resist_Green_INT_Value], [Res_Burn_INT_Value], [Res_Poision_INT_Value], [Res_Shcok_INT_Value], [Res_Bleed_INT_Value], [Res_Stun_INT_Value], [Res_Paralysis_INT_Value], [Res_Sleep_INT_Value], [Res_Freeze_INT_Value], [Res_Charm_INT_Value], [Res_Confusion_INT_Value], [Res_Silence_INT_Value], [Res_Weak_INT_Value], [IS_Item_Rate] from tb_Monster")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_MONSTER.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_MONSTER row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Monster_Code_Name, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Monster_Name_D, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Parts_ID_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Parts_ID_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Summon_Hit, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Faction, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Quest_Group_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Quest_Group_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Quest_Group_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Quest_Group_4, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Quest_Group_5, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Mission_Group_1, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Mission_Group_2, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Mission_Group_3, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Lv, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Exp, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Moster_Kill_Score, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Rank, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Flag, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_ClearCondition_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Object_MaxDamage_Value, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Object_Default_Ani_File, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Object_Open_Ani_File, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Element, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Aggression_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Melee_Skill_Inclination, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Range_Skill_Inclination, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Heal_Skill_Inclination, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Other_Skill_Inclination, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Protection_Aggro_Ratio, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Sight, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_WeightRank, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Defence_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_DMGAction_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_HeadAction_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Defence_Direction_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Walk_Speed, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Run_Speed, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Turn_Speed, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Gaze_Speed, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Status_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Status_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Weapon_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_BattleMode_Type, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Monster_AI_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_AI_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Death_Type, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Monster_Special_Damage, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Default_Action_Type, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Monster_NormalStand_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Default_Action_Type_01, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Monster_NormalStand_Type_01, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Monster_Switching_AI_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_AI_Type_01, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Monster_Special_Damage_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Switching_AI_Condition_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Switching_AI_Value_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Switching_AI_Step_01, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Monster_Switching_Action_01, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Default_Action_Type_02, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Monster_NormalStand_Type_02, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Monster_Switching_AI_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_AI_Type_02, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Monster_Special_Damage_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Switching_AI_Condition_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Switching_AI_Value2_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Switching_AI_Step_02, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Monster_Switching_Action_02, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Default_Action_Type_03, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Monster_NormalStand_Type_03, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Monster_Switching_AI_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_AI_Type_03, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Monster_Special_Damage_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Switching_AI_Condition_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Switching_AI_Value_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Switching_AI_Step_03, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Monster_Switching_Action_03, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Skill1_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Skill2_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Skill3_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Skill4_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Skill5_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Skill6_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Skill7_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Skill8_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Skill9_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Skill10_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Drop_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Hit_Drop_ID, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Monster_Scale, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_CollisionRadius, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_CollisionHeight, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_RealHeight, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_Party_Revise, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Monster_SA_Type, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.HP_Max_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.SG_Max_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.PATK_Max_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.MATK_Max_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.PDPR_RAT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.MDPR_RAT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.PDEF_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.MDEF_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.PAR_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.MAR_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.PARP_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.MARP_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.ADR_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.PCP_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.MCP_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.PCRP_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.MCRP_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.PCA_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.MCA_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.DAR_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.PDSR_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.MDSR_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.AtkPower_Light_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.AtkPower_Dark_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.AtkPower_Blue_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.AtkPower_Red_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.AtkPower_White_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.AtkPower_Green_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Resist_Light_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Resist_Dark_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Resist_Blue_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Resist_Red_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Resist_White_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Resist_Green_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Res_Burn_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Res_Poision_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Res_Shcok_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Res_Bleed_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Res_Stun_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Res_Paralysis_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Res_Sleep_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Res_Freeze_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Res_Charm_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Res_Confusion_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Res_Silence_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Res_Weak_INT_Value, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.IS_Item_Rate, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_MONSTER(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
