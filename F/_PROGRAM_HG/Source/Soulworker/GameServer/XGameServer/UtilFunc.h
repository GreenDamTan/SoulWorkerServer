#pragma once

#include <cwchar>

// Forward declarations
class XResourceMgr;
struct TB_NAMEFILTER;

/**
 * @brief UtilFunc - 工具函数命名空间
 *
 * 包含各种游戏服务器通用工具函数
 */
namespace UtilFunc {

/**
 * @brief 检查名称是否通过过滤器
 *
 * @param wszString 要检查的名称字符串
 * @param resourceMgr 资源管理器引用
 * @return true 如果名称可用（未命中过滤器）
 * @return false 如果名称被过滤
 */
bool IsUsableNameFilter(wchar_t* wszString, class XResourceMgr& resourceMgr);

} // namespace UtilFunc
