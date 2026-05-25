#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include "Soulworker/Common/XNet/XCommon/PSOption.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerParty.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerFriend.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMazeSync.h"  // 移到前面，PS_CHANGE_NAME 被 PSServerLeague.h 需要
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerLeague.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerExchange.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMail.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerLogin.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerGM.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerChat.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerAttendance.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCashShop.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerWorldMode.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMaze.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMisc.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerModeMaze.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerServerInfo.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMapMaze.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerUser.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerItem.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMyroom.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerItemMake.h"

// ============================================================================
// PSServer.h - 服务器间通信协议结构体聚合头文件
//
// 此文件是聚合头文件，include 所有拆分后的子模块文件。
// 原文件共 11575 行，现已拆分为 23 个模块文件。
//
// 拆分文件列表（位于 PSServer/ 子目录）：
// - PSServerCore.h       - 核心枚举、基础结构、辅助函数
// - PSServerParty.h      - 组队/公会结构及序列化
// - PSServerFriend.h     - 好友/黑名单/助战结构
// - PSServerLeague.h     - 联赛结构及序列化
// - PSServerExchange.h   - 交易所结构及序列化
// - PSServerMail.h       - 邮件系统结构及序列化
// - PSServerLogin.h      - 登录平台结构
// - PSServerGM.h         - GM工具结构及序列化
// - PSServerDB.h         - DBAgent相关结构
// - PSServerChat.h       - 聊天系统结构
// - PSServerAttendance.h - 签到系统结构
// - PSServerCashShop.h   - 现金商店结构
// - PSServerWorldMode.h  - World Mode结构
// - PSServerMaze.h       - 迷宫进入限制结构
// - PSServerMisc.h       - 杂项结构
// - PSServerModeMaze.h   - ModeMaze匹配结构
// - PSServerServerInfo.h - 服务器信息结构
// - PSServerMapMaze.h    - 地图/迷宫创建结构
// - PSServerUser.h       - 用户进入服务器结构
// - PSServerItem.h       - 物品相关结构
// - PSServerMyroom.h     - Myroom相关结构
// - PSServerMazeSync.h   - 迷宫同步/更新结构
// - PSServerItemMake.h   - 物品制作限制结构
//
// 所有结构体定义、序列化运算符和 static_assert 验证已迁移到对应的拆分文件中。
// ============================================================================

// 对齐 IDA: PS_CHAT_FORCE 是 PS_CHAT_PARTY 的别名，用于 Force 聊天消息
using PS_CHAT_FORCE = PS_CHAT_PARTY;
