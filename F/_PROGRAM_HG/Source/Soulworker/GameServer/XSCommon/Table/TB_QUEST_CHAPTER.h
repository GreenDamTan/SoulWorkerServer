// 中文说明：
// 1. 本文件承接 TB_QUEST_CHAPTER 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_QUEST_CHAPTER {
    unsigned int ID = 0;
    unsigned int Season_ID = 0;
    unsigned int uniEpisode[15] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_QUEST_CHAPTER) == 0x44, "TB_QUEST_CHAPTER size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_QUEST_CHAPTER* GetTB_QUEST_CHAPTER(unsigned int index) ;
    void SetTB_QUEST_CHAPTER(unsigned int index, const TB_QUEST_CHAPTER& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBQuestChapterDB() ;
    std::map<unsigned int, TB_QUEST_CHAPTER> m_mapTB_QUEST_CHAPTER;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_QUEST_CHAPTER* XResourceMgr::GetTB_QUEST_CHAPTER(unsigned int index) {
        const auto it = m_mapTB_QUEST_CHAPTER.find(index);
        return it == m_mapTB_QUEST_CHAPTER.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_QUEST_CHAPTER(unsigned int index, const TB_QUEST_CHAPTER& row) {
        m_mapTB_QUEST_CHAPTER[index] = row;
    }

std::int64_t XResourceMgr::LoadTBQuestChapterDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Season_ID], [Episode_ID_01], [Episode_ID_02], [Episode_ID_03], [Episode_ID_04], [Episode_ID_05], [Episode_ID_06], [Episode_ID_07], [Episode_ID_08], [Episode_ID_09], [Episode_ID_10], [Episode_ID_11], [Episode_ID_12], [Episode_ID_13], [Episode_ID_14], [Episode_ID_15] from tb_Quest_Chapter")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_QUEST_CHAPTER.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_QUEST_CHAPTER row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Season_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (auto& episodeId : row.uniEpisode) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &episodeId, &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_QUEST_CHAPTER(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
