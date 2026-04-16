// 中文说明：
// 1. 本文件承接 TB_INTERACTION_OBJECT 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_INTERACTION_OBJECT {
    unsigned int ID = 0;
    std::uint8_t Object_type = 0;
    std::uint8_t Private_activate = 0;
    unsigned int Act_Delay_time = 0;
    std::uint8_t Resource_Type = 0;
    char FileName[255] = {};
    std::uint8_t Enable_State = 0;
    std::uint8_t Show_Sight = 0;
    std::uint8_t Change_Show_Sights = 0;
    unsigned int Show_Quest_ID = 0;
    unsigned int Show_Condition_Quest_ID = 0;
    unsigned int Show_Condition_Class_ID = 0;
    unsigned int Check_Item_ID = 0;
    unsigned int Check_Item_Count = 0;
    unsigned int Check_Quest_ID = 0;
    unsigned int Check_Quest_Condition_ID = 0;
    std::int16_t Interaction_Count_Min = 0;
    std::int16_t Interaction_Count_Max = 0;
    unsigned int Interaction_CoolTime = 0;
    unsigned int Interaction_Range = 0;
    std::uint8_t Interaction_Delete = 0;
    unsigned int Remove_Item_ID = 0;
    unsigned int Remove_Item_Count = 0;
    unsigned int Add_Item_ID = 0;
    unsigned int Add_Item_Count = 0;
    std::uint16_t Respawn_Value_Max = 0;
    unsigned int Respawn_Delay_Time = 0;
    unsigned int Add_Random_Item_ID = 0;
    unsigned int Object_Link_Aura = 0;
    std::uint8_t Public_Quest_Check = 0;
    char Script[255] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_INTERACTION_OBJECT) == 0x254, "TB_INTERACTION_OBJECT size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_INTERACTION_OBJECT* GetTB_INTERACTION_OBJECT(unsigned int index) ;
    void SetTB_INTERACTION_OBJECT(unsigned int index, const TB_INTERACTION_OBJECT& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBInteractionObjectDB() ;
    std::unordered_map<unsigned int, TB_INTERACTION_OBJECT> interactionObjectRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_INTERACTION_OBJECT* XResourceMgr::GetTB_INTERACTION_OBJECT(unsigned int index) {
        auto it = interactionObjectRows_.find(index);
        if (it == interactionObjectRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_INTERACTION_OBJECT(unsigned int index, const TB_INTERACTION_OBJECT& row) {
        interactionObjectRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBInteractionObjectDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Object_type], [Private_activate], [Act_Delay_time], [Resource_Type], [FileName], [Enable_State], [Show_Sight], [Change_Show_Sights], [Show_Quest_ID], [Show_Condition_Quest_ID], [Show_Condition_Class_ID], [Check_Item_ID], [Check_Item_Count], [Check_Quest_ID], [Check_Quest_Condition_ID], [Interaction_Count_Min], [Interaction_Count_Max], [Interaction_CoolTime], [Interaction_Range], [Interaction_Delete], [Remove_Item_ID], [Remove_Item_Count], [Add_Item_ID], [Add_Item_Count], [Respawn_Value_Max], [Respawn_Delay_Time], [Add_Random_Item_ID], [Object_Link_Aura], [Public_Quest_Check], [Script] from tb_Interaction_Object")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        interactionObjectRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_INTERACTION_OBJECT row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Object_type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Private_activate, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Act_Delay_time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Resource_Type, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.FileName, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Enable_State, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Show_Sight, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Change_Show_Sights, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Show_Quest_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Show_Condition_Quest_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Show_Condition_Class_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Check_Item_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Check_Item_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Check_Quest_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Check_Quest_Condition_ID, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Interaction_Count_Min, &executeResult) ||
                !GreenDamTan_DBGetSigned(xDBBinder, &row.Interaction_Count_Max, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Interaction_CoolTime, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Interaction_Range, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Interaction_Delete, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Remove_Item_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Remove_Item_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Add_Item_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Add_Item_Count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Respawn_Value_Max, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Respawn_Delay_Time, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Add_Random_Item_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Object_Link_Aura, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Public_Quest_Check, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Script, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_INTERACTION_OBJECT(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
