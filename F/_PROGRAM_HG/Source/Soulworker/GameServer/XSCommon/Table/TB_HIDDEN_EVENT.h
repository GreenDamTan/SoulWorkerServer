// 中文说明：
// 1. 本文件承接 TB_HIDDEN_EVENT 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_HIDDEN_EVENT {
    std::uint16_t Hidden_Event_ID = 0;
    unsigned int uniEvent_Condition_ID[5] = {};
    std::uint16_t uniCondition_Select_Rate[5] = {};
    unsigned int Reward_Item = 0;
    std::uint8_t Reward_Count = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_HIDDEN_EVENT) == 0x25, "TB_HIDDEN_EVENT size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_HIDDEN_EVENT* GetTB_HIDDEN_EVENT(std::uint16_t index) ;
    void SetTB_HIDDEN_EVENT(std::uint16_t index, const TB_HIDDEN_EVENT& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBHiddenEventDB() ;
    std::unordered_map<std::uint16_t, TB_HIDDEN_EVENT> hiddenEventRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_HIDDEN_EVENT* XResourceMgr::GetTB_HIDDEN_EVENT(std::uint16_t index) {
        auto it = hiddenEventRows_.find(index);
        if (it == hiddenEventRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_HIDDEN_EVENT(std::uint16_t index, const TB_HIDDEN_EVENT& row) {
        hiddenEventRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBHiddenEventDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Hidden_Event_ID], [Event_Condition_ID_01], [Condition_Select_Rate_01], [Event_Condition_ID_02], [Condition_Select_Rate_02], [Event_Condition_ID_03], [Condition_Select_Rate_03], [Event_Condition_ID_04], [Condition_Select_Rate_04], [Event_Condition_ID_05], [Condition_Select_Rate_05], [Reward_Item], [Reward_Count] from tb_Hidden_Event")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        hiddenEventRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_HIDDEN_EVENT row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Hidden_Event_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniEvent_Condition_ID[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCondition_Select_Rate[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniEvent_Condition_ID[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCondition_Select_Rate[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniEvent_Condition_ID[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCondition_Select_Rate[2], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniEvent_Condition_ID[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCondition_Select_Rate[3], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniEvent_Condition_ID[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCondition_Select_Rate[4], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Item, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reward_Count, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_HIDDEN_EVENT(row.Hidden_Event_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
