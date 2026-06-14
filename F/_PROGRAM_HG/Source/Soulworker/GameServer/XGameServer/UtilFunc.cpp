// UtilFunc.cpp
// 游戏服务器通用工具函数实现

#include "UtilFunc.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include <cwctype>
#include <string>

namespace {

std::wstring ToUpperWide(const wchar_t* value) {
    if (!value) {
        return {};
    }

    std::wstring result(value);
    for (wchar_t& ch : result) {
        ch = static_cast<wchar_t>(std::towupper(ch));
    }
    return result;
}

std::wstring WidenFilterWord(const char* value) {
    if (!value) {
        return {};
    }

    std::wstring result;
    while (*value != '\0') {
        result.push_back(static_cast<unsigned char>(*value));
        ++value;
    }
    return result;
}

} // anonymous namespace

namespace UtilFunc {

bool IsUsableNameFilter(wchar_t* wszString, XResourceMgr& resourceMgr) {
    if (!wszString) {
        return false;
    }

    const std::wstring upperName = ToUpperWide(wszString);
    const auto& rows = resourceMgr.GetTB_NAMEFILTERRows();

    for (const auto& entry : rows) {
        const TB_NAMEFILTER& row = entry.second;
        const std::wstring upperFilter = ToUpperWide(WidenFilterWord(row.Filter_Word).c_str());
        if (upperFilter.empty()) {
            continue;
        }

        if (row.Filter_Type == 1) {
            // 完全匹配
            if (upperName == upperFilter) {
                return false;
            }
            continue;
        }

        // 包含匹配
        if (upperName.find(upperFilter) != std::wstring::npos) {
            return false;
        }
    }

    return true;
}

} // namespace UtilFunc
