// 中文说明：
// 1. 本文件承接 TB_TALK_LIST 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_TALK_LIST {
    std::uint16_t Talk_List_ID = 0;
    std::uint16_t uniTalkID[25] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_TALK_LIST) == 0x34, "TB_TALK_LIST size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_TALK_LIST* GetTB_TALK_LIST(std::uint16_t index) ;
    void SetTB_TALK_LIST(std::uint16_t index, const TB_TALK_LIST& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBTalkListDB() ;
    std::map<std::uint16_t, TB_TALK_LIST> m_mapTB_TALK_LIST;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_TALK_LIST* XResourceMgr::GetTB_TALK_LIST(std::uint16_t index) {
        const auto it = m_mapTB_TALK_LIST.find(index);
        return it == m_mapTB_TALK_LIST.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_TALK_LIST(std::uint16_t index, const TB_TALK_LIST& row) {
        m_mapTB_TALK_LIST[index] = row;
    }

std::int64_t XResourceMgr::LoadTBTalkListDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Talk_List_ID], [Talk_ID_01], [Talk_ID_02], [Talk_ID_03], [Talk_ID_04], [Talk_ID_05], [Talk_ID_06], [Talk_ID_07], [Talk_ID_08], [Talk_ID_09], [Talk_ID_10], [Talk_ID_11], [Talk_ID_12], [Talk_ID_13], [Talk_ID_14], [Talk_ID_15], [Talk_ID_16], [Talk_ID_17], [Talk_ID_18], [Talk_ID_19], [Talk_ID_20], [Talk_ID_21], [Talk_ID_22], [Talk_ID_23], [Talk_ID_24], [Talk_ID_25] from tb_Talk_List")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_TALK_LIST.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_TALK_LIST row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Talk_List_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (auto& talkId : row.uniTalkID) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &talkId, &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_TALK_LIST(row.Talk_List_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
