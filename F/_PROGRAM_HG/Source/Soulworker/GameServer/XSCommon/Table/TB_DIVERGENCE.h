// 中文说明：
// 1. 本文件承接 TB_DIVERGENCE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_DIVERGENCE {
    unsigned int Div_ID = 0;
    unsigned int Div_GroupID = 0;
    unsigned int Swap_Div_ID = 0;
    std::uint8_t Div_Level = 0;
    std::uint8_t Div_Max_Level = 0;
    unsigned int Next_Div_ID = 0;
    unsigned int Prev_Div_ID = 0;
    std::uint8_t Div_Need_Level = 0;
    std::uint8_t Div_Need_Point = 0;
    std::uint8_t Req_Min_AwakeningGrade = 0;
    char Skill_NameS[511] = {};
    std::uint8_t Div_Option_Type = 0;
    unsigned int Div_Option_Value = 0;
    float Div_SubOption_Value = 0.0f;
};
#pragma pack(pop)
static_assert(sizeof(TB_DIVERGENCE) == 0x221, "TB_DIVERGENCE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_DIVERGENCE* GetTB_DIVERGENCE(unsigned int index) ;
    void SetTB_DIVERGENCE(unsigned int index, const TB_DIVERGENCE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBDivergenceDB() ;
    std::unordered_map<unsigned int, TB_DIVERGENCE> divergenceRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_DIVERGENCE* XResourceMgr::GetTB_DIVERGENCE(unsigned int index) {
        auto it = divergenceRows_.find(index);
        if (it == divergenceRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_DIVERGENCE(unsigned int index, const TB_DIVERGENCE& row) {
        divergenceRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBDivergenceDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Div_ID], [Div_GroupID], [Swap_Div_ID], [Div_Level], [Div_Max_Level], [Next_Div_ID], [Prev_Div_ID], [Div_Need_Level], [Div_Need_Point], [Req_Min_AwakeningGrade], [Skill_NameS], [Div_Option_Type], [Div_Option_Value], [Div_SubOption_Value] from tb_Divergence")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        divergenceRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_DIVERGENCE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Div_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Div_GroupID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Swap_Div_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Div_Level, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Div_Max_Level, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Next_Div_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Prev_Div_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Div_Need_Level, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Div_Need_Point, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Req_Min_AwakeningGrade, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Skill_NameS, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Div_Option_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Div_Option_Value, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Div_SubOption_Value, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_DIVERGENCE(row.Div_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
