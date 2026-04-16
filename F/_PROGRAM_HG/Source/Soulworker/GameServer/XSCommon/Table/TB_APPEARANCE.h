// 中文说明：
// 1. 本文件承接 TB_APPEARANCE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_APPEARANCE {
    unsigned int Appearance_ID = 0;
    std::uint8_t Appearance_Classify = 0;
    std::uint8_t Appearance_Type = 0;
    std::uint8_t Appearance_Class = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_APPEARANCE) == 0x7, "TB_APPEARANCE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_APPEARANCE* GetTB_APPEARANCE(unsigned int index) ;
    void SetTB_APPEARANCE(unsigned int index, const TB_APPEARANCE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBAppearanceDB() ;
    bool LoadAppearanceFile(const std::filesystem::path& basePath) ;
    std::unordered_map<unsigned int, TB_APPEARANCE> appearanceRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_APPEARANCE* XResourceMgr::GetTB_APPEARANCE(unsigned int index) {
        auto it = appearanceRows_.find(index);
        if (it == appearanceRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_APPEARANCE(unsigned int index, const TB_APPEARANCE& row) {
        appearanceRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBAppearanceDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Appearance_ID], [Appearance_Classify], [Appearance_Type], [Appearance_Class] from tb_Appearance")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        appearanceRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_APPEARANCE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Appearance_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Appearance_Classify, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Appearance_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Appearance_Class, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_APPEARANCE(row.Appearance_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

bool XResourceMgr::LoadAppearanceFile(const std::filesystem::path& basePath) {
        std::vector<std::uint8_t> buffer;
        if (!GreenDamTanDBLoadDetail::LoadBinaryFile(basePath / "tb_Appearance.res", buffer) ||
            buffer.size() < sizeof(unsigned int)) {
            return false;
        }

        unsigned int rowCount = 0;
        std::memcpy(&rowCount, buffer.data(), sizeof(rowCount));
        const std::size_t rowOffset = sizeof(rowCount);
        const std::size_t rowBytes = static_cast<std::size_t>(rowCount) * sizeof(TB_APPEARANCE);
        if (buffer.size() < rowOffset + rowBytes) {
            return false;
        }

        for (unsigned int index = 0; index < rowCount; ++index) {
            TB_APPEARANCE row{};
            std::memcpy(&row,
                        buffer.data() + rowOffset + static_cast<std::size_t>(index) * sizeof(TB_APPEARANCE),
                        sizeof(TB_APPEARANCE));
            SetTB_APPEARANCE(row.Appearance_ID, row);
        }
        return true;
    }

#endif
