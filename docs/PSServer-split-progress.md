# PSServer.h 拆分进度文档

## 概述

PSServer.h 原文件共 **11575 行**，包含服务器间通信协议结构体定义。本文档记录拆分进度和每个文件的内容范围。

## 拆分目标

将大型头文件拆分为多个逻辑模块，提高代码可维护性和编译速度。

## 拆分计划

| 文件名 | 内容范围 | 原文件行号 | 状态 |
|--------|----------|------------|------|
| PSServerCore.h | 核心枚举、基础结构、辅助函数 | 1-500, 2514-2526 | ✅ 已完成 |
| PSServerParty.h | 组队/公会结构及序列化 | 68-500, 2528-2700, 2699-4545 | ✅ 已完成 |
| PSServerFriend.h | 好友/黑名单结构及序列化 | 338-700, 5403-5900 | ✅ 已完成 |
| PSServerLeague.h | 联赛结构及序列化 | 4546-4700 | ✅ 已完成 |
| PSServerExchange.h | 交易所结构及序列化 | 787-1000, 6393-6600 | ✅ 已完成 |
| PSServerMail.h | 邮件系统结构及序列化 | 6562-6860, 9966-10000 | ✅ 已完成 |
| PSServerLogin.h | 登录平台结构（TWN/CHN/GF） | 8700-8900 | ✅ 已完成 |
| PSServerGM.h | GM工具结构及序列化 | 7750-7850 | ✅ 已完成 |
| PSServerDB.h | DBAgent相关结构（金币/BP/称号/统计/任务） | 8867-9200, 11000-11575 | ✅ 已完成 |

## 详细内容说明

### 1. PSServerCore.h - 核心枚举和基础结构

**包含内容：**
- `E_PARTY_GROUP_TYPE` - 队伍/联盟类型枚举
- `SERVER_OPTION` - 服务器内容开关 ID
- `CHANGE_SERVER_TYPE` - 切服/进图类型
- `ST_PARTY_INFO` - 队伍进入地图附带的组信息
- `ST_PARTY_MEMBER` - 队伍成员信息
- `ST_UPDATE_PARTY_MEMBER` - 更新队伍成员
- `ST_APPLY_MEMBER` / `ST_APPLY_MEMBER_LIST` - 申请成员
- `ST_PARTY_RECRUIT` 系列结构 - 招募相关
- `ST_MATCHING_INFO` - 匹配信息
- `GreenDamTan_BoundedWideString` / `GreenDamTan_BoundedString` - 辅助函数

**依赖：**
- PSCommon.h
- PSOption.h
- Packet.h

### 2. PSServerParty.h - 组队/公会结构

**包含内容：**
- `PS_REQ_PARTY_ENTER_SERVER` - 组队进入服务器请求
- `PS_PARTY_INFO` - 组队信息
- `PS_RES_PARTY_ENTER_SERVER` - 组队进入服务器响应
- `PS_REQ_PARTY_CREATE` - 创建组队请求
- `PS_PARTY_ADDMEMBER` - 添加成员
- `PS_PARTY_LEAVE` / `PS_PARTY_DELETE` - 离开/删除组队
- `PS_PARTY_CHANGE_MASTER` - 更改队长
- `PS_REQ_PARTY_LOGIN_MEMBER` - 组队登录成员请求
- `PS_FORCE_*` 系列 - 公会相关结构
- 所有组队/公会结构的序列化运算符

**依赖：**
- PSServerCore.h

### 3. PSServerFriend.h - 好友/黑名单结构

**包含内容：**
- `PS_REQ_FRIEND_DELETE` - 删除好友请求
- `PS_REQ_FRIEND_ACCEPT` - 接受好友请求
- `PS_REQ_FRIEND_BLOCK_ADD` / `DELETE` - 黑名单添加/删除
- `PS_REQ_FRIEND_FIND` - 查找好友
- `DB_BLOCK_INFO` - 黑名单条目（DB格式）
- `PS_DB_FRIEND` / `PS_DB_FRIEND_LIST` - 好友信息（DB格式）
- `DB_FRIEND_INFO` - 好友信息（DB格式）
- `ST_CHAR_COMMUNITY` - 角色社区状态
- `ST_FIND_FRIEND` - 查找好友结果
- `ST_RECRUIT_INFO` / `ST_RECRUIT_LIST` - 招募信息
- `ST_RECOMMAND_FRIEND_INFO` - 推荐好友
- `PS_DAILY_MISSION_FRIEND_*` - 每日任务好友相关
- `ST_HELPER_SUPPORT_*` 系列 - 助战支持
- 所有好友相关序列化运算符

**依赖：**
- PSServerCore.h

### 4. PSServerLeague.h - 联赛结构

**包含内容：**
- `ST_LEAGUE_MEMBER` / `ST_LEAGUE_MEMBER_EX` - 联赛成员
- `ST_LEAGUE_INVITE_INFO` - 联赛邀请
- `ST_REQ_LEAGUE_CREATE` - 创建联赛请求
- `PS_LEAGUE_CREATE_FOR_SERVER` - 联赛创建（服务器）
- `ST_REQ_LEAGUE_INVITE` 系列 - 联赛邀请
- `ST_REQ_LEAGUE_SEARCH` - 联赛搜索
- `PS_REQ_ITEM_MOVE_LEAGUE_INVEN` 系列 - 联赛仓库物品移动
- 所有联赛相关序列化运算符

**依赖：**
- PSServerCore.h

### 5. PSServerExchange.h - 交易所结构

**包含内容：**
- `ST_EXCHANGE_PRICE_INFO` - 交易所价格历史条目
- `PS_EXCHANGE_PRICE_HISTORY_REQ/RES` - 价格历史请求/响应
- `PS_EXCHANGE_SEARCH_REQ/RES` - 交易所搜索
- `ST_EXCHANGE_BROACH_INFO` - 交易所镂刻信息
- `ST_EXCHANGE_EXTEND_OPTION` - 扩展选项
- `ST_EXCHANGE_ITEM` - 交易所物品
- `PS_EXCHANGE_INTEREST_*` - 关注物品
- `PS_EXCHANGE_MY_LIST_*` - 我的上架列表
- `PS_EXCHANGE_ITEM_BUY_*` - 购买物品
- 所有交易所相关序列化运算符

**依赖：**
- PSServerCore.h

### 6. PSServerMail.h - 邮件系统结构

**包含内容：**
- `ST_POST_CHAR` - 邮件发送者角色信息
- `ST_POST_DATA` - 邮件数据（含附件）
- `ST_ACCOUNT_POST_DATA` - 账号邮件数据
- `PS_ACCOUNT_POST_LIST` - 账号邮件列表
- `PS_LOG_ITEM` / `PS_LOG_ITEM_LIST` - 邮件物品日志
- `PS_POST_DELETE_*` 系列 - 邮件删除
- `ST_POST_LEVEL_UP_EVENT_*` - 升级活动事件邮件
- `ST_PROFILE_PHOTO_INFO` 系列 - 头像照片
- `ST_SYSTEM_POST` 系列 - 系统邮件（GM工具）
- `ST_GMT_POST_*` 系列 - GM邮件
- 所有邮件相关序列化运算符

**依赖：**
- PSServerCore.h

### 7. PSServerLogin.h - 登录平台结构

**包含内容：**
- `PS_LOGIN_REQ_FOR_TWN` - 台湾平台登录请求
- `PS_LOGIN_REQ_FOR_CHN` - 中国平台登录请求
- `PS_LOGIN_REQ_FOR_GF` - GF平台登录请求
- 各平台登录反序列化运算符

**依赖：**
- PSCommon.h
- Packet.h

### 8. PSServerGM.h - GM工具结构

**包含内容：**
- `ST_GM_USER_KICK_INFO` - GM踢人信息
- `ST_GM_NOTICE_INFO` - GM公告信息
- `ST_GM_TIME_EVENT_INFO` - GM时间事件
- `ST_GM_VALUE_EVENT_INFO` - GM数值事件
- GM相关序列化运算符

**依赖：**
- PSServerCore.h

### 9. PSServerDB.h - DBAgent相关结构

**包含内容：**
- `PS_DB_GOLD_UPDATE` - 金币更新
- `PS_BP_INFO` / `PS_DB_BP_UPDATE` - BP更新
- `ST_TITLE_INFO_DB` / `PS_TITLE_LOAD` - 称号加载
- `PS_TITLE_ADD` / `PS_REQ_TITLE_UPDATE` - 称号操作
- `PS_CHARACTER_UPDATE_POS` - 角色位置更新
- `ST_MAZE_STATE` 系列 - 迷宫状态
- `ST_DISTRICT_STATE` 系列 - 区域状态
- `ST_ACHIEVE_*` 系列 - 成就相关
- `PS_INFINITE_TOWER_INFO` - 无限塔
- `ST_STATISTICS_*` 系列 - 统计相关
- `ST_DAILY_MISSION_*` 系列 - 每日任务
- `ST_WEEKLY_MISSION_*` 系列 - 每周任务
- `PS_PLAY_TIME_FOR_DAY` - 每日游戏时间
- 所有DBAgent相关序列化运算符

**依赖：**
- PSServerCore.h

## 拆分规则

1. **保持向后兼容**：原 PSServer.h 保留为聚合头文件，include 所有拆分后的子文件
2. **依赖顺序**：子文件可依赖 PSServerCore.h，但避免互相依赖
3. **结构完整性**：每个结构体及其序列化运算符放在同一文件
4. **文档同步**：每完成一个模块，更新本文档状态

## 变更历史

| 日期 | 操作 | 说明 |
|------|------|------|
| 2026-05-25 | 创建文档 | 初始拆分计划 |
| 2026-05-25 | 创建拆分文件 | 创建 9 个拆分文件（PSServerCore.h 等） |
| 2026-05-25 | 开始迁移 | 从 PSServer.h 删除已迁移到 PSServerCore.h 的代码，行数从 11575 减到 11130 |
| 2026-05-25 | 添加 include | 在 PSServer.h 顶部添加 PSServerCore.h include |
| 2026-05-25 | 文件迁移完成 | 将所有 23 个 PSServer*.h 拆分文件移动到 PSServer/ 子目录 |
| 2026-05-25 | include路径更新 | 更新所有拆分文件和 PSServer.h 的 include 路径使用 PSServer/ 前缀 |
| 2026-05-25 | static_assert迁移 | 将 197 个 static_assert 验证语句迁移到对应的拆分文件 |
| 2026-05-25 | 清理完成 | PSServer.h 精简为 73 行聚合头文件，仅保留 include 和说明注释 |
| 2026-05-25 | 修复DBAgent错误 | 修复 DBAgent 构建错误：PS_DB_CHECK_LOCATION、PS_NPC_CREDIT、PS_DB_WORLD_EVENT_REGISTER、PS_HAN_BILLING_ORDER_NO_VEC 等结构体和序列化运算符 |
| 2026-05-25 | 修复ControlServer错误 | 删除 RouletteEventManager.h 中的重复定义 PS_DB_ROULETTE_REWARD_INFO |

---

## 当前状态

**PSServer.h 行数**: 74 行（原 11575 行，减少 11501 行，约 **99.4%**）

**拆分文件目录**: `F/_PROGRAM_HG/Source/Soulworker/Common/XNet/XCommon/PSServer/`

**拆分文件数量**: 23 个模块文件

**构建状态** (2026-05-25):
- ✅ LoginServer: 构建成功
- ✅ ControlServer: 构建成功
- ✅ RelayServer: 构建成功
- ✅ DBAgent: 构建成功

**已完成的拆分文件**:

### PSServerCore.h (已完成 ✅)
包含核心枚举、基础结构、辅助函数及其序列化运算符：
- 枚举: `E_PARTY_GROUP_TYPE`, `SERVER_OPTION`, `CHANGE_SERVER_TYPE`
- 结构体: `ST_PARTY_INFO`, `ST_PARTY_MEMBER`, `ST_UPDATE_PARTY_MEMBER`
- 结构体: `ST_SOCKET_DATA`, `ST_ITEM_SOCKET`, `PS_ITEM_SOCKET_LIST`
- 结构体: `ST_ITEM_BROACH`, `PS_ITEM_BROACH_LIST`
- 结构体: `ST_UPDATE_STAT`, `ST_STAT_VEC`
- 结构体: `ST_UPDATE_SPECIAL_OPTION`, `ST_UPDATE_SPECIAL_OPTION_LIST`
- 结构体: `PS_QUICKSLOT_CARD`, `ST_ACHIEVE_CATEGORY`
- 结构体: `ST_ITEM_PACKAGE_PARTS`, `PS_ITEM_PACKAGE`, `PS_ITEM_PACKAGE_LIST`
- 辅助函数: `GreenDamTan_BoundedWideString<N>`, `GreenDamTan_BoundedString<N>`

### PSServerParty.h (已完成 ✅)
包含所有组队/公会结构及其序列化运算符（54个结构体）：
- 组队进入服务器、申请成员、招募相关、匹配信息
- 组队操作、公会创建、公会成员、公会操作
- 邀请/拒绝相关结构

### PSServerFriend.h (已完成 ✅)
包含所有好友/黑名单/助战结构及其序列化运算符（44个结构体）：
- 好友请求/响应: `PS_REQ_FRIEND_DELETE`, `PS_REQ_FRIEND_ACCEPT`, `PS_RES_FRIEND_ACCEPT` 等
- 黑名单: `DB_BLOCK_INFO`, `PS_REQ_FRIEND_BLOCK_ADD`, `PS_RES_BLOCKLIST_ADD` 等
- 助战系统: `ST_HELPER_SUPPORT_INFO`, `PS_HELPER_SUPPORT_INFO_RES`, `PS_SERVER_HELPER_SUPPORT_REWARD` 等
- 招募系统: `ST_RECRUIT_INFO`, `PS_RECRUIT_ADD`, `PS_RES_RECRUIT_LIST` 等
- 每日任务好友: `PS_DAILY_MISSION_FRIEND_REQ`, `PS_DAILY_MISSION_FRIEND_RES` 等
- 其他: `ST_CREATE_ITEM`, `PS_DB_CHARACTER_INFO_OTHER_REQ`, `PS_DB_HELPER_SUPPORT_EQUIP` 等

### PSServerLeague.h (已完成 ✅)
包含所有联赛结构及其序列化运算符：
- 联赛成员: `ST_LEAGUE_MEMBER`, `ST_LEAGUE_MEMBER_EX`
- 联赛邀请: `ST_LEAGUE_INVITE_INFO`, `ST_REQ_LEAGUE_INVITE`, `ST_RES_LEAGUE_INVITE`
- 联赛创建: `ST_REQ_LEAGUE_CREATE`, `PS_LEAGUE_CREATE_FOR_SERVER`
- 联赛搜索: `ST_REQ_LEAGUE_SEARCH`, `PS_RES_LEAGUE_SEARCH`
- 联赛仓库: `PS_REQ_ITEM_MOVE_LEAGUE_INVEN`, `PS_LEAGUE_INVENTORY_FOR_LOG`, `PS_LEAGUE_INVENTORY_FOR_LOG_LIST`
- 其他联赛操作结构及其序列化运算符

### PSServerExchange.h (已完成 ✅)
包含所有交易所结构及其序列化运算符：
- 价格历史: `ST_EXCHANGE_PRICE_INFO`, `PS_EXCHANGE_PRICE_HISTORY_REQ/RES`, `PS_DB_EXCHANGE_PRICE_HISTORY_REQ/RES`
- 交易所搜索: `PS_EXCHANGE_SEARCH_REQ/RES`, `ST_EXCHANGE_ITEM`, `ST_EXCHANGE_EXTEND_OPTION`, `ST_EXCHANGE_BROACH_INFO`
- 关注列表: `PS_EXCHANGE_INTEREST_LIST_REQ/RES`, `PS_EXCHANGE_INTEREST_ITEM_REQ/RES`
- 我的交易所: `ST_MY_EXCHANGE_ITEM`, `PS_EXCHANGE_MY_LIST_REQ/RES`
- 购买物品: `PS_EXCHANGE_ITEM_BUY_REQ/RES`
- 价格更新: `PS_EXCHANGE_PRICE_HISTORY_UPDATE`
- 所有交易所相关序列化运算符

### PSServerMail.h (已完成 ✅)
包含所有邮件系统结构及其序列化运算符：
- 邮件数据: `ST_POST_CHAR`, `ST_POST_DATA`, `ST_ACCOUNT_POST_DATA`, `PS_ACCOUNT_POST_LIST`
- 邮件日志: `PS_LOG_ITEM`, `PS_LOG_ITEM_LIST`
- 邮件删除: `PS_POST_DELETE_INFO`, `PS_POST_DELETE_LIST`, `PS_POST_DELETE_ALL_SERVER`
- 升级活动: `ST_POST_LEVEL_UP_EVENT_INFO`, `PS_POST_LEVEL_UP_EVENT_INFO_VEC`, `PS_POST_LEVEL_UP_EVENT_UPDATE`
- 头像照片: `ST_PROFILE_PHOTO_INFO`, `PS_PROFILE_PHOTO_LOAD`, `PS_DB_PROFILE_PHOTO_UPDATE/CHANGE/ADD`
- 批量领取: `PS_POST_RECEIPT_ALL_SERVER`
- GM邮件: `ST_SYSTEM_POST_ITEM`, `ST_SYSTEM_POST`, `ST_GMT_POST_CONDITION`, `ST_GMT_POST_INFO`, `ST_GMT_POST_SEND`
- GM邮件列表: `PS_GMT_POST_LIST`, `PS_GMT_POST_SEND_LIST`, `ST_POST_LIST`
- 所有邮件相关序列化运算符

### PSServerLogin.h (已完成 ✅)
包含各平台登录请求结构：
- `PS_LOGIN_REQ_FOR_TWN`, `PS_LOGIN_REQ_FOR_CHN`, `PS_LOGIN_REQ_FOR_GF`
- `PS_LOGIN_REQ_FOR_WM`, `PS_LOGIN_REQ_FOR_SG`
- `ST_WM_AUTH_INFO`, `PS_ACCOUNT_EVENT_LIST`, `PS_HAN_BILLING_ORDER_NO` 等

### PSServerGM.h (已完成 ✅)
包含 GM 工具结构：
- `ST_GM_USER_KICK_INFO`, `ST_GM_NOTICE_INFO`, `ST_GM_TIME_EVENT_INFO`, `ST_GM_VALUE_EVENT_INFO`

### PSServerDB.h (已完成 ✅)
包含所有 DBAgent 相关结构（约 2100 行）：
- 金币/BP更新: `PS_DB_GOLD_UPDATE`, `PS_BP_INFO`, `PS_DB_BP_UPDATE`
- 称号系统: `ST_TITLE_INFO_DB`, `PS_TITLE_LOAD`, `PS_TITLE_ADD`
- 角色位置: `PS_CHARACTER_UPDATE_POS`
- 迷宫/区域状态: `ST_MAZE_STATE`, `ST_DISTRICT_STATE`
- 成就系统: `ST_ACHIEVE_INFO`, `PS_ACHIEVE_LOAD`
- 统计数据: `ST_STATISTICS_INFO`
- 每日/每周任务: `ST_DAILY_MISSION_INFO`, `ST_WEEKLY_MISSION_INFO`
- 世界事件: `PS_DB_WORLD_EVENT_INFO_REQ/RES`, `PS_DB_WORLD_EVENT_REGISTER_REQ/RES`, `PS_DB_WORLD_EVENT_REWARD`
- 计费订单: `PS_HAN_BILLING_ORDER_NO`, `PS_HAN_BILLING_ORDER_NO_VEC`
- 位置检查: `PS_DB_CHECK_LOCATION`（使用 `#pragma pack(1)` 实现非对齐布局）
- NPC信用度: `PS_NPC_CREDIT`, `PS_NPC_CREDIT_LIST`
- 轮盘事件: `PS_DB_ROULETTE_EVENT_INFO`, `PS_DB_ROULETTE_REWARD_INFO`, `PS_ROULETTE_INFO`
- 阿卡夏记录: `ST_AKASHIC_RECORD`, `ST_AKASHIC_LIST`
- 快捷栏更新: `PS_QUICKSLOT_UPDATE_CARD_VEC`

### PSServerMisc.h (已完成 ✅)
包含杂项结构及序列化运算符：
- 用户认证类型更新: `PS_USER_UPDATE_AUTH_TYPE`
- 防沉迷信息: `PS_INDULGENCE_INFO`
- 优惠券使用: `PS_DB_USE_COUPON_REQ`, `PS_DB_USE_COUPON_RES`
- 自动封禁检查: `PS_DB_CHECK_AUTO_BLOCK_INFO`
- NPC信用度: `PS_NPC_CREDIT`, `PS_NPC_CREDIT_LIST`

### PSServerChat.h (已完成 ✅)
包含聊天系统结构及序列化运算符：
- 广播公告: `PS_CHAT_NOTICE`
- 喇叭消息: `PS_CHAT_MEGAPHONE`
- 物品链接: `PS_CHAT_ITEM_LINK`, `PS_CHAT_ITEM_LINK_FOR_SERVER`
- 私聊消息: `PS_CHAT_WHISPER`

### PSServerAttendance.h (已完成 ✅)
包含签到系统结构及序列化运算符：
- 签到信息: `PS_ATTENDANCE_INFO`
- 连续签到: `PS_ATTENDANCE_CONTINUE`
- 签到游戏时间: `PS_ATTENDANCE_PLAY_TIME`

### PSServerCashShop.h (已完成 ✅)
包含现金商店结构及序列化运算符：
- 现金购买计数: `PS_CASH_BUY_COUNT`, `PS_CASH_BUY_COUNT_LIST`
- 现金套装: `PS_CASH_SET`, `PS_CASH_SET_LIST`
- 商店物品: `ST_SHOP_ITEM`, `ST_SHOP_ITEM_LIST`, `PS_DB_SHOP_ITEM`

### PSServerWorldMode.h (已完成 ✅)
包含 World Mode 序列化运算符：
- World Mode 进入: `ST_ENTER_WORLD_MODE_INFO`
- World Mode 信息: `ST_WORLD_MODE_INFO`, `ST_WORLD_MODE_INFO_VEC`
- World Mode 完成: `PS_WORLD_MODE_COMPLETE`
- World Mode 更新: `PS_WORLD_MODE_UPDATE`
- World Mode 结束: `PS_WORLD_MODE_FINISH`
- 注意: World Mode 结构体定义在 PSCommon.h 中

### PSServerMaze.h (已完成 ✅)
包含迷宫进入限制结构及序列化运算符：
- 进入次数更新: `PS_UPDATE_MAZE_ENTER_LIMIT_COUNT`
- 组更新: `PS_DB_MAZE_ENTER_LIMIT_COUNT_GROUP_UPDATE`
- 清除: `PS_MAZE_ENTER_LIMIT_COUNT_CLEAR`, `PS_MAZE_ENTER_LIMIT_COUNT_GROUP_CLEAR`
- 列表: `PS_MAZE_ENTER_LIMIT_COUNT_LIST`, `PS_MAZE_ENTER_LIMIT_COUNT_GROUP`, `PS_MAZE_ENTER_LIMIT_COUNT_GROUP_LIST`
- GF计费重载: `PS_GF_BILLING_RELOAD_RES`

### PSServerModeMaze.h (已完成 ✅)
包含 ModeMaze 匹配结构及序列化运算符：
- 成员信息: `ST_MODE_MAZE_MEMBER_INFO`
- 匹配进入: `PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ`, `PS_MODE_MAZE_MATCHING_ENTER_RES`
- 匹配退出: `PS_MODE_MAZE_MATCHING_EXIT`
- 匹配等待: `PS_MODE_MAZE_MATCHING_WAIT`
- 匹配事件: `PS_SERVER_MODE_MAZE_MATCHING_EVENT`
- 运营时间: `PS_SERVER_MODE_MAZE_MATCHING_TIME_INFO`

### PSServerServerInfo.h (已完成 ✅)
包含服务器信息结构及序列化运算符：
- 服务器状态同步: `SS_SERVER_INFO`
- 服务器统计信息: `PS_SERVER_COMMON_INFO`
- 同步信息: `ST_SYNC_INFO`
- 服务器组信息: `ST_SERVER_GROUP_INFO`, `ST_SERVER_GROUP_INFO_VEC`
- 客户端服务器信息: `ST_SERVER_INFO_FOR_USER`, `PS_SERVERS_INFO_FOR_USER`

### PSServerMapMaze.h (已完成 ✅)
包含地图/迷宫创建结构及序列化运算符：
- 地图信息: `ST_MAP_INFO`, `PS_ENTER_MAP_RES`, `PS_ENTER_MAP_REQ`
- 创建地图: `PS_CREATE_MAP`, `PS_CREATE_MAP_RES`, `PS_CREATE_MAP_LIST`
- DB频道地图: `ST_DB_CHANNEL_MAP`
- 迷宫成员: `ST_ENTER_MAZE_MEMBER_INFO`
- 创建迷宫: `ST_CREATE_MAZE`, `ST_CREATE_MODE_MAZE`
- 迷宫中继: `ST_CREATE_MAZE_FOR_RELAY`, `ST_CREATE_MODE_MAZE_FOR_RELAY`
- 返回迷宫: `ST_GO_BACK_MAZE`
- 地图统计: `ST_STATISTICS_MAP_SAVE`（含输入运算符）
- 切服: `PS_REQ_CHANGE_SERVER`, `PS_RES_CHANGE_SERVER`
- 角色计数: `ST_USER_CHARACTER_COUNT_FOR_SERVER`, `ST_USER_CHARACTER_COUNT_FOR_SERVER_VEC`
- 强制匹配: `PS_SERVER_FORCE_MATCHING_ENTER`, `PS_SERVER_FORCE_MATCHING_CHECK`

### PSServerUser.h (已完成 ✅)
包含用户进入服务器相关结构及序列化运算符：
- 进入服务器: `ST_ENTER_SERVER`
- Relay同步用户: `PS_USER_INFO_FOR_RELAY`, `PS_USERS_INFO`
- 用户地图变化: `PS_UPDATE_USER_MAP_INFO`

### PSServerItem.h (已完成 ✅)
包含物品相关结构及序列化运算符：
- 创建物品: `ST_CREATE_ITEMS`
- 物品使用获取信息: `ST_GET_INFO`
- Countbox更新: `PS_DB_ITEM_COUNTBOX`

### PSServerMyroom.h (已完成 ✅)
包含 Myroom 相关结构及序列化运算符：
- 花粉互助用户: `PS_MYROOM_POLLEN_HELP_USER`
- 清除用户状态: `PS_REQ_CLEAR_USER_STATE`

### PSServerMazeSync.h (已完成 ✅)
包含迷宫同步/更新相关结构及序列化运算符：
- 迷宫进入检查: `ST_SERVER_CHECK_ENTER_MAZE`
- 迷宫等待进入用户: `ST_MAZE_WAIT_ENTER_USER_INFO`
- 迷宫成员同步: `ST_MAZE_MEMBER_INFO_SYNC`, `PS_MAZE_INFO_SYNC`
- 迷宫更新同步: `PS_MAZE_UPDATE_INFO_SYNC`, `PS_MAZE_UPDATE_INFO`
- 轮盘事件: `PS_ROULETTE_EVENT_UPDATE_SERVER`, `PS_DB_INIT_ROULETTE_INFO`
- 账户游戏时间: `PS_PLAY_TIME_BY_ACCOUNT`
- 改名请求: `PS_CHANGE_NAME`

### PSServerItemMake.h (已完成 ✅)
包含物品制作限制相关结构及序列化运算符：
- 物品制作限制信息: `PS_ITEM_MAKE_LIMIT_INFO`, `PS_ITEM_MAKE_LIMIT_LIST`
- DB层限制信息: `PS_DB_ITEM_MAKE_LIMIT_INFO`
- DB层更新请求: `PS_DB_ITEM_MAKE_LIMIT_UPDATE`, `PS_DB_ITEM_MAKE_LIMIT_INIT`

## 已修复的问题

1. **ST_NETCAFE_MISSION_INFO 重复定义**: 保留 DBLoadTable.h 版本（int64_t 时间字段），PSServer.h 中添加注释说明
2. **ST_KRR_MONSTER_INFO 重复定义**: 保留 PSCommon.h 版本，删除 DBLoadTable.h 中的重复
3. **PS_DB_ROULETTE_REWARD_INFO 重复定义**: 保留 PSServerDB.h 版本，删除 RouletteEventManager.h 中的重复
4. **RouletteEventManager.cpp 字段名错误**: 修复 nIndex/nValue 为 nRewardID/nRemainCount
5. **PS_DB_CHECK_LOCATION 结构体对齐问题**: 使用 `#pragma pack(1)` 实现与 IDA 匹配的非对齐布局
6. **UXActorID 赋值运算符**: 添加构造函数和赋值运算符支持 `unsigned int` 类型
7. **ST_USE_ITEM_INFO_LIST 字段名**: 修复 `vecList` 为 `vecInfo` 以匹配 IDA
8. **ST_PROFILE_PHOTO_INFO 序列化**: 添加缺失的输入/输出序列化运算符
9. **PS_NPC_CREDIT/PS_NPC_CREDIT_LIST**: 添加新结构体定义及序列化运算符
10. **PS_DB_WORLD_EVENT_REGISTER_REQ/RES**: 添加世界事件注册结构体及序列化运算符
11. **PS_DB_WORLD_EVENT_REWARD**: 添加世界事件奖励结构体及序列化运算符
12. **PS_HAN_BILLING_ORDER_NO_VEC XSendDBPacket 运算符**: 添加缺失的 XSendDBPacket 输出运算符
13. **ST_AKASHIC_RECORD/LIST 序列化**: 添加缺失的 XPacket 输出运算符和输入运算符
14. **PS_QUICKSLOT_UPDATE_CARD_VEC**: 添加结构体定义和输入运算符
15. **PS_DB_WORLD_EVENT_DAILY_REWARD**: 添加结构体定义及序列化运算符
16. **PS_DB_ROULETTE_EVENT_INFO**: 添加结构体定义及序列化运算符
17. **ST_STATISTICS_MAP_SAVE 输入运算符**: 添加缺失的输入运算符

*最后更新: 2026-05-25*
