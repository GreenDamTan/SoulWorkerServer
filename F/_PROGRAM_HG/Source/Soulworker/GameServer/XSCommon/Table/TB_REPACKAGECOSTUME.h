// 中文说明：
// 1. 本文件承接 TB_REPACKAGECOSTUME 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_REPACKAGECOSTUME {
    unsigned int RePackageItem_ID = 0;
    union {
        struct {
            unsigned int Item_01;
            unsigned int Item_02;
            unsigned int Item_03;
            unsigned int Item_04;
            unsigned int Item_05;
            unsigned int Item_06;
            unsigned int Item_07;
            unsigned int Item_08;
            unsigned int Item_09;
            unsigned int Item_10;
            unsigned int Item_11;
            unsigned int Item_12;
            unsigned int Item_13;
        };
        unsigned int uniItem[13];
    };
};
#pragma pack(pop)
static_assert(sizeof(TB_REPACKAGECOSTUME) == 0x38, "TB_REPACKAGECOSTUME size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_REPACKAGECOSTUME* GetTB_REPACKAGECOSTUME(unsigned int index) ;
    void SetTB_REPACKAGECOSTUME(unsigned int index, const TB_REPACKAGECOSTUME& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBRepackageCostumeDB() ;
    std::map<unsigned int, TB_REPACKAGECOSTUME> m_mapTB_REPACKAGECOSTUME;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_REPACKAGECOSTUME* XResourceMgr::GetTB_REPACKAGECOSTUME(unsigned int index) {
        const auto it = m_mapTB_REPACKAGECOSTUME.find(index);
        return it == m_mapTB_REPACKAGECOSTUME.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_REPACKAGECOSTUME(unsigned int index, const TB_REPACKAGECOSTUME& row) {
        m_mapTB_REPACKAGECOSTUME[index] = row;
    }

std::int64_t XResourceMgr::LoadTBRepackageCostumeDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [RePackageItem_ID], [Item_01], [Item_02], [Item_03], [Item_04], [Item_05], [Item_06], [Item_07], [Item_08], [Item_09], [Item_10], [Item_11], [Item_12], [Item_13] from tb_RepackageCostume")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_REPACKAGECOSTUME.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_REPACKAGECOSTUME row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.RePackageItem_ID, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (auto& itemId : row.uniItem) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &itemId, &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_REPACKAGECOSTUME(row.RePackageItem_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
