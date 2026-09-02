// UtilFunc.cpp
// 游戏服务器通用工具函数实现
// 原始归属（PDB lines）：common/xnet/xutil/utility.h（共享 XUtil 层）
// GameServer 侧实例：IsUsableNameFilter 访问 XGameServer 单例内嵌
// m_xResourceMgr.m_mapTB_NAMEFILTER（PDB offset 46520+4552）。

#include "UtilFunc.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include <cwctype>
#include <string>
#include <windows.h>

namespace UtilFunc {

// IsUsableNameFilter (0x1400187F0)
// 已精确还原 - 名字过滤检查：
// 1. 入参空返回 false
// 2. 构造 wstring 副本并逐字符 towupper 大写化
// 3. 遍历 XGameServer 单例的 m_mapTB_NAMEFILTER（std::map 有序遍历）
// 4. Filter_Word 经 MultiByteToWideChar(CP_ACP) 转宽字符后大写化
// 5. Filter_Type==1 完全匹配（wcscmp 语义）命中返回 false
// 6. 否则 wcsstr 包含匹配命中返回 false
// 7. 全部通过返回 true
bool IsUsableNameFilter(wchar_t* wszString) {
    if (!wszString)
        return false;

    std::wstring wstrCharacterName(wszString);
    for (auto& ch : wstrCharacterName)
        ch = static_cast<wchar_t>(std::towupper(ch));

    const std::map<unsigned int, TB_NAMEFILTER>& mapFilter =
        XGameServer::Instance()->GetResourceMgr().GetTB_NAMEFILTER();

    std::wstring wstrFilterString;
    for (const auto& entry : mapFilter) {
        const TB_NAMEFILTER* pTB_NameFilter = &entry.second;

        wchar_t szFilterWord[512] = {};
        MultiByteToWideChar(0, 0, pTB_NameFilter->Filter_Word, -1,
                            szFilterWord, 511);
        wstrFilterString = szFilterWord;
        for (auto& ch : wstrFilterString)
            ch = static_cast<wchar_t>(std::towupper(ch));

        if (pTB_NameFilter->Filter_Type == 1) {
            if (wcscmp(wstrFilterString.c_str(),
                       wstrCharacterName.c_str()) == 0) {
                return false;
            }
        } else {
            if (wcsstr(wstrCharacterName.c_str(),
                       wstrFilterString.c_str())) {
                return false;
            }
        }
    }

    return true;
}

// CheckValidString (0x1404529A0)
// 已精确还原 - 按地区类型检查宽字符合法性：
// KOR：数字 0x30-0x39 / 大写 0x41-0x5A / 小写 0x61-0x7A / 韩文 AC00-D7A3
// JPN：数字/大写/小写 + 平假名 3040-309F / 片假名 30A0-30FF /
//      汉字 4E00-9FFF + 一系列符号码点；任一字符非法即返回 false
bool CheckValidString(wchar_t* wszString, NATION_TYPE eNationType) {
    bool bRetValue = true;
    const int nLength = static_cast<int>(std::wcslen(wszString));

    if (eNationType == NATION_TYPE_KOR) {
        for (int i = 0; i < nLength && bRetValue; ++i)
            bRetValue = (wszString[i] >= 0x30 && wszString[i] <= 0x39)
                     || (wszString[i] >= 0x41 && wszString[i] <= 0x5A)
                     || (wszString[i] >= 0x61 && wszString[i] <= 0x7A)
                     || (wszString[i] >= 0xAC00 && wszString[i] <= 0xD7A3);
    } else if (eNationType == NATION_TYPE_JPN) {
        for (int j = 0; j < nLength && bRetValue; ++j) {
            if (wszString[j] < 0x30 || wszString[j] > 0x39) {
                if ((wszString[j] < 0x41 || wszString[j] > 0x5A)
                    && (wszString[j] < 0x61 || wszString[j] > 0x7A)) {
                    if (wszString[j] < 0x3040 || wszString[j] > 0x309F)
                        bRetValue = (wszString[j] >= 0x30A0 && wszString[j] <= 0x30FF)
                                  || (wszString[j] >= 0x4E00 && wszString[j] <= 0x9FFF)
                                  || wszString[j] == 12539
                                  || wszString[j] == 65281
                                  || wszString[j] == 12540
                                  || wszString[j] == 9734
                                  || wszString[j] == 9733
                                  || wszString[j] == 9675
                                  || wszString[j] == 9679
                                  || wszString[j] == 9678
                                  || wszString[j] == 9671
                                  || wszString[j] == 9670
                                  || wszString[j] == 9633
                                  || wszString[j] == 9632
                                  || wszString[j] == 9651
                                  || wszString[j] == 9650
                                  || wszString[j] == 9661
                                  || wszString[j] == 9660
                                  || wszString[j] == 9834
                                  || wszString[j] == 65343
                                  || wszString[j] == 8213
                                  || wszString[j] == 8208
                                  || wszString[j] == 65291
                                  || wszString[j] == 65293
                                  || wszString[j] == 8224;
                    else
                        bRetValue = true;
                } else {
                    bRetValue = true;
                }
            } else {
                bRetValue = true;
            }
        }
    }
    return bRetValue;
}

// _CheckValidString (0x1403E1E90)
// 已精确还原 - 窄字符版本：仅数字/大写/小写合法
bool _CheckValidString(char* szString) {
    bool bRetValue = true;
    const unsigned __int64 nLength = strlen(szString) + 1;
    for (int i = 0; i < static_cast<int>(nLength) - 1 && bRetValue; ++i)
        bRetValue = (szString[i] >= 48 && szString[i] <= 57)
                 || (szString[i] >= 65 && szString[i] <= 90)
                 || (szString[i] >= 97 && szString[i] <= 122);
    return bRetValue;
}

} // namespace UtilFunc
