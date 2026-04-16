// 中文说明：
// 1. 本文件承接 TB_PC_AKASHIC 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
struct TB_PC_AKASHIC {
    unsigned int ID = 0;
    unsigned int Akashic_ID = 0;
};
static_assert(sizeof(TB_PC_AKASHIC) == 0x8, "TB_PC_AKASHIC size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_PC_AKASHIC* GetTB_PC_AKASHIC(unsigned int index) ;
    void SetTB_PC_AKASHIC(unsigned int index, const TB_PC_AKASHIC& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBPCAkashicDB() ;
    bool LoadPCAkashicFile(const std::filesystem::path& basePath) ;
    std::map<unsigned int, TB_PC_AKASHIC> m_mapTB_PC_AKASHIC;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_PC_AKASHIC* XResourceMgr::GetTB_PC_AKASHIC(unsigned int index) {
        const auto it = m_mapTB_PC_AKASHIC.find(index);
        return it == m_mapTB_PC_AKASHIC.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_PC_AKASHIC(unsigned int index, const TB_PC_AKASHIC& row) {
        m_mapTB_PC_AKASHIC[index] = row;
    }

std::int64_t XResourceMgr::LoadTBPCAkashicDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Akashic_ID] from tb_PC_Akashic")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_PC_AKASHIC.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_PC_AKASHIC row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Akashic_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_PC_AKASHIC(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

bool XResourceMgr::LoadPCAkashicFile(const std::filesystem::path& basePath) {
        std::vector<std::uint8_t> buffer;
        if (!GreenDamTanDBLoadDetail::LoadBinaryFile(basePath / "tb_PC_Akashic.res", buffer) ||
            buffer.size() < sizeof(unsigned int)) {
            return false;
        }

        GreenDamTanDBLoadDetail::BinaryCursor cursor(buffer);
        unsigned int rowCount = 0;
        if (!cursor.ReadU32(rowCount)) {
            return false;
        }

        CTableLoader_S loader{};
        loader.m_nRowCount = static_cast<int>(rowCount);
        loader.m_strTableName = "tb_PC_Akashic";
        for (unsigned int index = 0; index < rowCount; ++index) {
            TB_PC_AKASHIC row{};
            if (!cursor.ReadCheckSumU32(&row.ID) || !cursor.ReadCheckSumU32(&row.Akashic_ID)) {
                return false;
            }
            SetTB_PC_AKASHIC(row.ID, row);
        }

        loader.m_biCheckSum = cursor.GetCheckSum();
        std::string md5Value;
        if (!cursor.ReadAsciiString(md5Value, 32) || !cursor.IsAtEnd()) {
            return false;
        }
        std::memset(loader.m_szMD5, 0, sizeof(loader.m_szMD5));
        const std::size_t md5CopyLength =
            md5Value.size() < sizeof(loader.m_szMD5) ? md5Value.size() : sizeof(loader.m_szMD5);
        std::memcpy(loader.m_szMD5, md5Value.data(), md5CopyLength);
        return CheckSum(loader);
    }

#endif
