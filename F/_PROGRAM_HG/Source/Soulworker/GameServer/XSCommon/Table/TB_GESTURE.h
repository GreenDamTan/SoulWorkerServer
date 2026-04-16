// 中文说明：
// 1. 本文件承接 TB_GESTURE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_GESTURE {
    std::uint16_t Gesture_ID = 0;
    std::uint8_t Use_Class = 0;
    std::uint8_t Req_Min_Level = 0;
    std::uint8_t InitialGesture = 0;
    unsigned int CoolTime = 0;
    std::uint16_t CoolTime_Global = 0;
    std::uint8_t Gesture_Type = 0;
    std::uint16_t Gesture_Gain_Type = 0;
};
#pragma pack(pop)
static_assert(sizeof(TB_GESTURE) == 0xE, "TB_GESTURE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_GESTURE* GetTB_GESTURE(std::uint16_t index) ;
    void SetTB_GESTURE(std::uint16_t index, const TB_GESTURE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBGestureDB() ;
    std::unordered_map<std::uint16_t, TB_GESTURE> gestureRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_GESTURE* XResourceMgr::GetTB_GESTURE(std::uint16_t index) {
        auto it = gestureRows_.find(index);
        if (it == gestureRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_GESTURE(std::uint16_t index, const TB_GESTURE& row) {
        gestureRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBGestureDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Gesture_ID], [Use_Class], [Req_Min_Level], [InitialGesture], [CoolTime], [CoolTime_Global], [Gesture_Type], [Gesture_Gain_Type] from tb_Gesture")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        gestureRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_GESTURE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Gesture_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Use_Class, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Req_Min_Level, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.InitialGesture, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.CoolTime, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.CoolTime_Global, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Gesture_Type, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Gesture_Gain_Type, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            SetTB_GESTURE(row.Gesture_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif
