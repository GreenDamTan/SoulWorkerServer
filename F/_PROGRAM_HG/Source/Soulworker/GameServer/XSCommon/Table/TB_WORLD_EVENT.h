// 中文说明：
// 1. 本文件承接 TB_WORLD_EVENT 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_WORLD_EVENT {
    unsigned int event_ID = 0;
    std::uint8_t event_activation = 0;
    char event_start_date[511] = {};
    char event_end_date[511] = {};
    char event_name[511] = {};
    unsigned int event_item_ID = 0;
    unsigned int event_item_amount_max = 0;
    unsigned int event_daily_reward_item_ID = 0;
    std::uint16_t event_daily_reward_item_amount = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_WORLD_EVENT) == 0x610, "TB_WORLD_EVENT size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_WORLD_EVENT* GetTB_WORLD_EVENT(unsigned int index) ;
    void SetTB_WORLD_EVENT(unsigned int index, const TB_WORLD_EVENT& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBWorldEventDB() ;
    std::map<unsigned int, TB_WORLD_EVENT> m_mapTB_WORLD_EVENT;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_WORLD_EVENT* XResourceMgr::GetTB_WORLD_EVENT(unsigned int index) {
        const auto it = m_mapTB_WORLD_EVENT.find(index);
        return it == m_mapTB_WORLD_EVENT.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_WORLD_EVENT(unsigned int index, const TB_WORLD_EVENT& row) {
        m_mapTB_WORLD_EVENT[index] = row;
    }

std::int64_t XResourceMgr::LoadTBWorldEventDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [event_ID], [event_activation], [event_start_date], [event_end_date], [event_name], [event_item_ID], [event_item_amount_max], [event_daily_reward_item_ID], [event_daily_reward_item_amount] from tb_world_event")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_WORLD_EVENT.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_WORLD_EVENT row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.event_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.event_activation, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.event_start_date, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.event_end_date, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.event_name, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.event_item_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.event_item_amount_max, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.event_daily_reward_item_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.event_daily_reward_item_amount, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_WORLD_EVENT(row.event_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
