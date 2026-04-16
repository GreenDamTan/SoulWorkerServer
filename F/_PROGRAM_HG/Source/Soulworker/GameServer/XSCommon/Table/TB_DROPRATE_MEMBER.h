// 中文说明：
// 1. 本文件承接 TB_DROPRATE_MEMBER 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_DROPRATE_MEMBER {
    std::uint8_t DropRate_Party_Member = 0;
    float DropRate_Member_Value = 0.0f;
};
#pragma pack(pop)
static_assert(sizeof(TB_DROPRATE_MEMBER) == 0x5, "TB_DROPRATE_MEMBER size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_DROPRATE_MEMBER* GetTB_DROPRATE_MEMBER(std::uint8_t index) ;
    void SetTB_DROPRATE_MEMBER(std::uint8_t index, const TB_DROPRATE_MEMBER& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBDropRateMemberDB() ;
    std::unordered_map<std::uint8_t, TB_DROPRATE_MEMBER> dropRateMemberRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_DROPRATE_MEMBER* XResourceMgr::GetTB_DROPRATE_MEMBER(std::uint8_t index) {
        auto it = dropRateMemberRows_.find(index);
        if (it == dropRateMemberRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_DROPRATE_MEMBER(std::uint8_t index, const TB_DROPRATE_MEMBER& row) {
        dropRateMemberRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBDropRateMemberDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [DropRate_Party_Member], [DropRate_Member_Value] from tb_DropRate_Member")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        dropRateMemberRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_DROPRATE_MEMBER row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.DropRate_Party_Member, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.DropRate_Member_Value, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_DROPRATE_MEMBER(row.DropRate_Party_Member, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
