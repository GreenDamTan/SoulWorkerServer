// 中文说明：
// 1. 本文件承接 TB_QUEST_SEASON 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_QUEST_SEASON {
    unsigned int ID = 0;
    unsigned int uniChapter[9] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_QUEST_SEASON) == 0x28, "TB_QUEST_SEASON size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_QUEST_SEASON* GetTB_QUEST_SEASON(unsigned int index) ;
    void SetTB_QUEST_SEASON(unsigned int index, const TB_QUEST_SEASON& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBQuestSeasonDB() ;
    std::map<unsigned int, TB_QUEST_SEASON> m_mapTB_QUEST_SEASON;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_QUEST_SEASON* XResourceMgr::GetTB_QUEST_SEASON(unsigned int index) {
        const auto it = m_mapTB_QUEST_SEASON.find(index);
        return it == m_mapTB_QUEST_SEASON.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_QUEST_SEASON(unsigned int index, const TB_QUEST_SEASON& row) {
        m_mapTB_QUEST_SEASON[index] = row;
    }

std::int64_t XResourceMgr::LoadTBQuestSeasonDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Chapter_ID_01], [Chapter_ID_02], [Chapter_ID_03], [Chapter_ID_04], [Chapter_ID_05], [Chapter_ID_06], [Chapter_ID_07], [Chapter_ID_08], [Chapter_ID_09] from tb_Quest_Season")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_QUEST_SEASON.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_QUEST_SEASON row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (auto& chapterId : row.uniChapter) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &chapterId, &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_QUEST_SEASON(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
