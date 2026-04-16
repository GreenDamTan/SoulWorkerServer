// 中文说明：
// 1. 本文件承接 TB_TITLE_REWARD 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_TITLE_REWARD {
    unsigned int Title_Reward_ID = 0;
    unsigned int uniTCLASS[8] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_TITLE_REWARD) == 0x24, "TB_TITLE_REWARD size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_TITLE_REWARD* GetTB_TITLE_REWARD(unsigned int index) ;
    void SetTB_TITLE_REWARD(unsigned int index, const TB_TITLE_REWARD& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBTitleRewardDB() ;
    std::map<unsigned int, TB_TITLE_REWARD> m_mapTB_TITLE_REWARD;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_TITLE_REWARD* XResourceMgr::GetTB_TITLE_REWARD(unsigned int index) {
        const auto it = m_mapTB_TITLE_REWARD.find(index);
        return it == m_mapTB_TITLE_REWARD.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_TITLE_REWARD(unsigned int index, const TB_TITLE_REWARD& row) {
        m_mapTB_TITLE_REWARD[index] = row;
    }

std::int64_t XResourceMgr::LoadTBTitleRewardDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Title_Reward_ID], [Title_ID_01], [Title_ID_02], [Title_ID_03], [Title_ID_04], [Title_ID_05], [Title_ID_06], [Title_ID_07], [Title_ID_08] from tb_Title_Reward")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_TITLE_REWARD.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_TITLE_REWARD row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Title_Reward_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (auto& value : row.uniTCLASS) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &value, &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_TITLE_REWARD(row.Title_Reward_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
