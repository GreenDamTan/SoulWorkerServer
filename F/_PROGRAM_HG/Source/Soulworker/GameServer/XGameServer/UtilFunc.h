#pragma once

// 原始归属（PDB lines）：common/xnet/xutil/utility.h（共享 XUtil 层）
// GameServer.exe 中 UtilFunc::IsUsableNameFilter / CheckValidString /
// _CheckValidString 三函数由该共享头承载，每服务各编译一份实例。

#include <cwchar>
#include "Soulworker/GameServer/XCore/XServer/Option.h"

namespace UtilFunc {

bool IsUsableNameFilter(wchar_t* wszString);
bool CheckValidString(wchar_t* wszString, NATION_TYPE eNationType);
bool _CheckValidString(char* szString);

} // namespace UtilFunc
