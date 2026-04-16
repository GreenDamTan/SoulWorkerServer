// 中文说明：
// 1. 本文件承接 TB_COMMON 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_COMMON {
    unsigned int Define_ID = 0;
    char Key[511] = {};
    float Value = 0.0f;
};
#pragma pack(pop)
static_assert(sizeof(TB_COMMON) == 0x207, "TB_COMMON size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_COMMON* GetTB_COMMON(unsigned int index) ;
    void SetTB_COMMON(unsigned int index, const TB_COMMON& row) ;
    const std::unordered_map<unsigned int, TB_COMMON>& GetTB_COMMONRows() const ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBCommonDB() ;
    bool LoadCommonFile(const std::filesystem::path& basePath) ;
    std::unordered_map<unsigned int, TB_COMMON> commonRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_COMMON* XResourceMgr::GetTB_COMMON(unsigned int index) {
        auto it = commonRows_.find(index);
        if (it == commonRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_COMMON(unsigned int index, const TB_COMMON& row) {
        commonRows_[index] = row;
    }

const std::unordered_map<unsigned int, TB_COMMON>& XResourceMgr::GetTB_COMMONRows() const {
        return commonRows_;
    }

std::int64_t XResourceMgr::LoadTBCommonDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(
            const_cast<char*>("select [Define_ID], [Key], [Value] from tb_Common")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        commonRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_COMMON row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Define_ID, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Key, &executeResult) ||
                !GreenDamTan_DBGetFloat(xDBBinder, &row.Value, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_COMMON(row.Define_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

bool XResourceMgr::LoadCommonFile(const std::filesystem::path& basePath) {
        std::vector<std::uint8_t> buffer;
        if (!GreenDamTanDBLoadDetail::LoadBinaryFile(basePath / "tb_Common.res", buffer) ||
            buffer.size() < sizeof(unsigned int)) {
            return false;
        }

        GreenDamTanDBLoadDetail::BinaryCursor cursor(buffer);
        unsigned int rowCount = 0;
        if (!cursor.ReadU32(rowCount)) {
            return false;
        }

        for (unsigned int index = 0; index < rowCount; ++index) {
            TB_COMMON row{};
            std::string utf8Text;
            if (!cursor.ReadU32(&row.Define_ID) ||
                !cursor.ReadUtf16String(utf8Text) ||
                !cursor.ReadF32(&row.Value)) {
                return false;
            }
            GreenDamTanDBLoadDetail::CopyString(row.Key, utf8Text);
            SetTB_COMMON(row.Define_ID, row);
        }
        return true;
    }

#endif
