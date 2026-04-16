// 中文说明：
// 1. 本文件承接 TB_CREATE_CLOTH 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_CREATE_CLOTH {
    std::uint16_t ID = 0;
    unsigned int Item_ID_Head = 0;
    unsigned int Item_ID_Body = 0;
    unsigned int Item_ID_Hands = 0;
    unsigned int Item_ID_Stocking = 0;
    unsigned int Item_ID_Foot = 0;
    unsigned int Item_ID_Pants = 0;
    char Icon_FileName[511] = {};

    std::array<unsigned int, 6> GetItemIDs() const {
        return {Item_ID_Head, Item_ID_Body, Item_ID_Hands, Item_ID_Stocking, Item_ID_Foot, Item_ID_Pants};
    }
};
#pragma pack(pop)
static_assert(sizeof(TB_CREATE_CLOTH) == 0x219, "TB_CREATE_CLOTH size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_CREATE_CLOTH* GetTB_CREATE_CLOTH(std::uint16_t index) ;
    void SetTB_CREATE_CLOTH(std::uint16_t index, const TB_CREATE_CLOTH& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBCreateClothDB() ;
    bool LoadCreateClothFile(const std::filesystem::path& basePath) ;
    std::unordered_map<std::uint16_t, TB_CREATE_CLOTH> createClothRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_CREATE_CLOTH* XResourceMgr::GetTB_CREATE_CLOTH(std::uint16_t index) {
        auto it = createClothRows_.find(index);
        if (it == createClothRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_CREATE_CLOTH(std::uint16_t index, const TB_CREATE_CLOTH& row) {
        createClothRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBCreateClothDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(
            const_cast<char*>("select [ID], [Item_ID_Head], [Item_ID_Body], [Item_ID_Hands], [Item_ID_Stocking], [Item_ID_Foot], [Item_ID_Pants], [Icon_FileName] from tb_Create_Cloth")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        createClothRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_CREATE_CLOTH row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID_Head, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID_Body, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID_Hands, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID_Stocking, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID_Foot, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Item_ID_Pants, &executeResult) ||
                !GreenDamTan_DBGetString(xDBBinder, row.Icon_FileName, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }

            SetTB_CREATE_CLOTH(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

bool XResourceMgr::LoadCreateClothFile(const std::filesystem::path& basePath) {
        std::vector<std::uint8_t> buffer;
        if (!GreenDamTanDBLoadDetail::LoadBinaryFile(basePath / "tb_Create_Cloth.res", buffer) ||
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
        loader.m_strTableName = "tb_Create_Cloth";
        for (unsigned int index = 0; index < rowCount; ++index) {
            TB_CREATE_CLOTH row{};
            std::string utf8Text;
            if (!cursor.ReadCheckSumU16(&row.ID) ||
                !cursor.ReadCheckSumU32(&row.Item_ID_Head) ||
                !cursor.ReadCheckSumU32(&row.Item_ID_Body) ||
                !cursor.ReadCheckSumU32(&row.Item_ID_Hands) ||
                !cursor.ReadCheckSumU32(&row.Item_ID_Stocking) ||
                !cursor.ReadCheckSumU32(&row.Item_ID_Foot) ||
                !cursor.ReadCheckSumU32(&row.Item_ID_Pants) ||
                !cursor.ReadCheckSumUtf16String(utf8Text)) {
                return false;
            }

            GreenDamTanDBLoadDetail::CopyString(row.Icon_FileName, utf8Text);
            SetTB_CREATE_CLOTH(row.ID, row);
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
