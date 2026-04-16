// 中文说明：
// 1. 本文件承接 TB_EVENT_CONDITION 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_EVENT_CONDITION {
    unsigned int Event_Condition_ID = 0;
    std::uint8_t Condition_Type = 0;
    std::uint8_t Target_Type = 0;
    unsigned int Target_ID = 0;
    unsigned int Finish_Count = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_EVENT_CONDITION) == 0xE, "TB_EVENT_CONDITION size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_EVENT_CONDITION* GetTB_EVENT_CONDITION(unsigned int index) ;
    void SetTB_EVENT_CONDITION(unsigned int index, const TB_EVENT_CONDITION& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBEventConditionDB() ;
    std::unordered_map<unsigned int, TB_EVENT_CONDITION> eventConditionRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_EVENT_CONDITION* XResourceMgr::GetTB_EVENT_CONDITION(unsigned int index) {
        auto it = eventConditionRows_.find(index);
        if (it == eventConditionRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_EVENT_CONDITION(unsigned int index, const TB_EVENT_CONDITION& row) {
        eventConditionRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBEventConditionDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Event_Condition_ID], [Condition_Type], [Target_Type], [Target_ID], [Finish_Count] from tb_Event_Condition")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        eventConditionRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_EVENT_CONDITION row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Event_Condition_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Condition_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Target_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Target_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Finish_Count, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_EVENT_CONDITION(row.Event_Condition_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
