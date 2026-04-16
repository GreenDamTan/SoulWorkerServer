// 中文说明：
// 1. 本文件承接 TB_SOCIAL_MOTION 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_SOCIAL_MOTION {
    std::uint16_t ID = 0;
    std::uint16_t Social_Item_ID = 0;
    std::uint8_t Class_Type = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_SOCIAL_MOTION) == 0x5, "TB_SOCIAL_MOTION size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_SOCIAL_MOTION* GetTB_SOCIAL_MOTION(std::uint16_t index) ;
    void SetTB_SOCIAL_MOTION(std::uint16_t index, const TB_SOCIAL_MOTION& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBSocialMotionDB() ;
    std::map<std::uint16_t, TB_SOCIAL_MOTION> m_mapTB_SOCIAL_MOTION;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_SOCIAL_MOTION* XResourceMgr::GetTB_SOCIAL_MOTION(std::uint16_t index) {
        const auto it = m_mapTB_SOCIAL_MOTION.find(index);
        return it == m_mapTB_SOCIAL_MOTION.end() ? nullptr : &it->second;
    }

void XResourceMgr::SetTB_SOCIAL_MOTION(std::uint16_t index, const TB_SOCIAL_MOTION& row) {
        m_mapTB_SOCIAL_MOTION[index] = row;
    }

std::int64_t XResourceMgr::LoadTBSocialMotionDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [ID], [Social_Item_ID], [Class_Type] from tb_Social_Motion")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        m_mapTB_SOCIAL_MOTION.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_SOCIAL_MOTION row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Social_Item_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Class_Type, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_SOCIAL_MOTION(row.ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
