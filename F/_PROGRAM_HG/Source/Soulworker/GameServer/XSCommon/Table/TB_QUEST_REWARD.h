// 中文说明：
// 1. 本文件承接 TB_QUEST_REWARD 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_QUEST_REWARD {
    std::uint16_t ID = 0;
    unsigned int uniitemset[8] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_QUEST_REWARD) == 0x22, "TB_QUEST_REWARD size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_QUEST_REWARD* GetTB_QUEST_REWARD(std::uint16_t index) ;
    void SetTB_QUEST_REWARD(std::uint16_t index, const TB_QUEST_REWARD& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBQuestRewardDB() ;
    std::map<std::uint16_t, TB_QUEST_REWARD> m_mapTB_QUEST_REWARD;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_QUEST_REWARD* XResourceMgr::GetTB_QUEST_REWARD(std::uint16_t index) {
        const auto it = m_mapTB_QUEST_REWARD.find(index);
        return it == m_mapTB_QUEST_REWARD.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_QUEST_REWARD(std::uint16_t index, const TB_QUEST_REWARD& row) {
        m_mapTB_QUEST_REWARD[index] = row;
    }

std::int64_t XResourceMgr::LoadTBQuestRewardDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Item_PC_A], [Item_PC_B], [Item_PC_C], [Item_PC_D], [Item_PC_E], [Item_PC_F], [Item_PC_G], [Item_PC_H] from tb_Quest_Reward")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_QUEST_REWARD.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_QUEST_REWARD row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (auto& itemId : row.uniitemset) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &itemId, &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_QUEST_REWARD(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
