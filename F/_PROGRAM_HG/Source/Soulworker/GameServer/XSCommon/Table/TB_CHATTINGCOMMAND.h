// 中文说明：
// 1. 本文件承接 TB_CHATTINGCOMMAND 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_CHATTINGCOMMAND {
    unsigned int Command_Index = 0;
    std::uint8_t Permission_Level = 0;
    char Command_1[511] = {};
    char Command_2[511] = {};
    char Command_3[511] = {};
    char Command_4[511] = {};
    char Command_5[511] = {};
    unsigned int Act_Type = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_CHATTINGCOMMAND) == 0xA04, "TB_CHATTINGCOMMAND size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_CHATTINGCOMMAND* GetTB_CHATTINGCOMMAND(unsigned int index) ;
    void SetTB_CHATTINGCOMMAND(unsigned int index, const TB_CHATTINGCOMMAND& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBChattingCommandDB() ;
    std::unordered_map<unsigned int, TB_CHATTINGCOMMAND> chattingCommandRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_CHATTINGCOMMAND* XResourceMgr::GetTB_CHATTINGCOMMAND(unsigned int index) {
        auto it = chattingCommandRows_.find(index);
        if (it == chattingCommandRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_CHATTINGCOMMAND(unsigned int index, const TB_CHATTINGCOMMAND& row) {
        chattingCommandRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBChattingCommandDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Command_Index], [Permission_Level], [Command_1], [Command_2], [Command_3], [Command_4], [Command_5], [Act_Type] from tb_ChattingCommand")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        chattingCommandRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_CHATTINGCOMMAND row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Command_Index, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Permission_Level, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Command_1, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Command_2, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Command_3, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Command_4, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Command_5, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Act_Type, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_CHATTINGCOMMAND(row.Command_Index, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
