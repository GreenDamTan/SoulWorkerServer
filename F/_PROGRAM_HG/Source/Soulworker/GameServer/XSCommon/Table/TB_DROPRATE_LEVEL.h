// 中文说明：
// 1. 本文件承接 TB_DROPRATE_LEVEL 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_DROPRATE_LEVEL {
    std::uint8_t DropRate_Party_LevelInterval = 0;
    float DropRate_LevelInterval_Value = 0.0f;
};
#pragma pack(pop)
static_assert(sizeof(TB_DROPRATE_LEVEL) == 0x5, "TB_DROPRATE_LEVEL size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_DROPRATE_LEVEL* GetTB_DROPRATE_LEVEL(std::uint8_t index) ;
    void SetTB_DROPRATE_LEVEL(std::uint8_t index, const TB_DROPRATE_LEVEL& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBDropRateLevelDB() ;
    std::unordered_map<std::uint8_t, TB_DROPRATE_LEVEL> dropRateLevelRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_DROPRATE_LEVEL* XResourceMgr::GetTB_DROPRATE_LEVEL(std::uint8_t index) {
        auto it = dropRateLevelRows_.find(index);
        if (it == dropRateLevelRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_DROPRATE_LEVEL(std::uint8_t index, const TB_DROPRATE_LEVEL& row) {
        dropRateLevelRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBDropRateLevelDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [DropRate_Party_LevelInterval], [DropRate_LevelInterval_Value] from tb_DropRate_Level")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        dropRateLevelRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_DROPRATE_LEVEL row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.DropRate_Party_LevelInterval, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.DropRate_LevelInterval_Value, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_DROPRATE_LEVEL(row.DropRate_Party_LevelInterval, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
