// 中文说明：
// 1. 本文件承接 TB_PHOTO_ITEM 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
struct TB_PHOTO_ITEM {
    unsigned int ID = 0;
    unsigned int Photo_Name = 0;
    std::uint16_t Photo_Group = 0;
    std::uint8_t Char_Class = 0;
    std::uint8_t Char_Promotion_Info = 0;
};
static_assert(sizeof(TB_PHOTO_ITEM) == 0xC, "TB_PHOTO_ITEM size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_PHOTO_ITEM* GetTB_PHOTO_ITEM(unsigned int index) ;
    void SetTB_PHOTO_ITEM(unsigned int index, const TB_PHOTO_ITEM& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBPhotoItemDB() ;
    bool LoadPhotoItemFile(const std::filesystem::path& basePath) ;
    std::map<unsigned int, TB_PHOTO_ITEM> photoItemRows_;
    std::map<ST_PHOTO_ID, TB_PHOTO_ITEM*> photoItemIdRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_PHOTO_ITEM* XResourceMgr::GetTB_PHOTO_ITEM(unsigned int index) {
        auto it = photoItemRows_.find(index);
        if (it == photoItemRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_PHOTO_ITEM(unsigned int index, const TB_PHOTO_ITEM& row) {
        photoItemRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBPhotoItemDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(
            const_cast<char*>("select [ID], [Photo_Name], [Photo_Group], [Char_Class], [Char_Promotion_Info] from tb_Photo_Item")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        photoItemRows_.clear();
        photoItemIdRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_PHOTO_ITEM row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Photo_Name, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Photo_Group, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Char_Class, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Char_Promotion_Info, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }

            SetTB_PHOTO_ITEM(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

bool XResourceMgr::LoadPhotoItemFile(const std::filesystem::path& basePath) {
        std::vector<std::uint8_t> buffer;
        if (!GreenDamTanDBLoadDetail::LoadBinaryFile(basePath / "tb_Photo_Item.res", buffer) ||
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
        loader.m_strTableName = "tb_Photo_Item";
        for (unsigned int index = 0; index < rowCount; ++index) {
            TB_PHOTO_ITEM row{};
            if (!cursor.ReadCheckSumU32(&row.ID) || !cursor.ReadCheckSumU32(&row.Photo_Name) ||
                !cursor.ReadCheckSumU16(&row.Photo_Group) || !cursor.ReadCheckSumU8(&row.Char_Class) ||
                !cursor.ReadCheckSumU8(&row.Char_Promotion_Info)) {
                return false;
            }
            SetTB_PHOTO_ITEM(row.ID, row);
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
