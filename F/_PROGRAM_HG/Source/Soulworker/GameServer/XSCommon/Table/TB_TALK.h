// 中文说明：
// 1. 本文件承接 TB_TALK 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_TALK {
    unsigned int Talk_Index = 0;
    std::uint8_t Talk_Type = 0;
    std::uint16_t Talk_Title_ID = 0;
    unsigned int Linked_Order_ID = 0;
    std::uint8_t Linked_Order_Type = 0;
    unsigned int Linked_Episode_ID = 0;
    std::uint8_t Linked_Episode_Type = 0;
    unsigned int Playing_Order_ID = 0;
    unsigned int Complete_Episode_ID = 0;
    unsigned int uniSpeechID[30] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_TALK) == 0x91, "TB_TALK size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_TALK* GetTB_TALK(unsigned int index) ;
    void SetTB_TALK(unsigned int index, const TB_TALK& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBTalkDB() ;
    std::map<unsigned int, TB_TALK> m_mapTB_TALK;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_TALK* XResourceMgr::GetTB_TALK(unsigned int index) {
        const auto it = m_mapTB_TALK.find(index);
        return it == m_mapTB_TALK.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_TALK(unsigned int index, const TB_TALK& row) {
        m_mapTB_TALK[index] = row;
    }

std::int64_t XResourceMgr::LoadTBTalkDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Talk_Index], [Talk_Type], [Talk_Title_ID], [Linked_Order_ID], [Linked_Order_Type], [Linked_Episode_ID], [Linked_Episode_Type], [Playing_Order_ID], [Complete_Episode_ID], [Speech_ID_01], [Speech_ID_02], [Speech_ID_03], [Speech_ID_04], [Speech_ID_05], [Speech_ID_06], [Speech_ID_07], [Speech_ID_08], [Speech_ID_09], [Speech_ID_10], [Speech_ID_11], [Speech_ID_12], [Speech_ID_13], [Speech_ID_14], [Speech_ID_15], [Speech_ID_16], [Speech_ID_17], [Speech_ID_18], [Speech_ID_19], [Speech_ID_20], [Speech_ID_21], [Speech_ID_22], [Speech_ID_23], [Speech_ID_24], [Speech_ID_25], [Speech_ID_26], [Speech_ID_27], [Speech_ID_28], [Speech_ID_29], [Speech_ID_30] from tb_Talk")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_TALK.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_TALK row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Talk_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Talk_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Talk_Title_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Linked_Order_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Linked_Order_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Linked_Episode_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Linked_Episode_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Playing_Order_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Complete_Episode_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (auto& speechId : row.uniSpeechID) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &speechId, &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_TALK(row.Talk_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
