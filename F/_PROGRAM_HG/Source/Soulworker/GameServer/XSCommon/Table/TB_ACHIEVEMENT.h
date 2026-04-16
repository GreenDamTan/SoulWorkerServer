// 中文说明：
// 1. 本文件承接 TB_ACHIEVEMENT 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_ACHIEVEMENT {
    unsigned int ID = 0;
    std::uint8_t Achievement_Category = 0;
    std::uint16_t Achievement_Group = 0;
    std::uint8_t Achievement_Order = 0;
    std::uint16_t Complete_Bit = 0;
    std::uint16_t Achievement_type = 0;
    unsigned int taget_ID = 0;
    char Achievement_Name[511] = {};
    unsigned int Achievement_count = 0;
    unsigned int achievement_Reward_gold = 0;
    unsigned int achievement_Reward_Ether = 0;
    unsigned int achievement_Reward_BP = 0;
    unsigned int uniachievement_Reward_title[2] = {};
    std::uint8_t uniachievement_Reward_item_type[2] = {};
    unsigned int uniachievement_Reward_item[2] = {};
    std::uint8_t uniachievement_Reward_item_count[2] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_ACHIEVEMENT) == 0x233, "TB_ACHIEVEMENT size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_ACHIEVEMENT* GetTB_ACHIEVEMENT(unsigned int index) ;
    void SetTB_ACHIEVEMENT(unsigned int index, const TB_ACHIEVEMENT& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBAchievementDB() ;
    std::map<unsigned int, TB_ACHIEVEMENT> m_mapTB_ACHIEVEMENT;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_ACHIEVEMENT* XResourceMgr::GetTB_ACHIEVEMENT(unsigned int index) {
        const auto it = m_mapTB_ACHIEVEMENT.find(index);
        return it == m_mapTB_ACHIEVEMENT.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_ACHIEVEMENT(unsigned int index, const TB_ACHIEVEMENT& row) {
        m_mapTB_ACHIEVEMENT[index] = row;
    }

std::int64_t XResourceMgr::LoadTBAchievementDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Achievement_Category], [Achievement_Group], [Achievement_Order], [Complete_Bit], [Achievement_type], [taget_ID], [Achievement_Name], [Achievement_count], [achievement_Reward_gold], [achievement_Reward_Ether], [achievement_Reward_BP], [achievement_Reward_title_1], [achievement_Reward_title_2], [achievement_Reward_item_type_1], [achievement_Reward_item_1], [achievement_Reward_item_count_1], [achievement_Reward_item_type_2], [achievement_Reward_item_2], [achievement_Reward_item_count_2] from tb_Achievement")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_ACHIEVEMENT.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_ACHIEVEMENT row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Achievement_Category, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Achievement_Group, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Achievement_Order, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Complete_Bit, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Achievement_type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.taget_ID, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Achievement_Name, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Achievement_count, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.achievement_Reward_gold, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.achievement_Reward_Ether, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.achievement_Reward_BP, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniachievement_Reward_title[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniachievement_Reward_title[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniachievement_Reward_item_type[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniachievement_Reward_item[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniachievement_Reward_item_count[0], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniachievement_Reward_item_type[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniachievement_Reward_item[1], &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniachievement_Reward_item_count[1], &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_ACHIEVEMENT(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
