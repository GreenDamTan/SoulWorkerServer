# ControlServer.exe 函数索引

## 概述

- 总函数数：6797
- IDA 实例：Port 10001
- 更新时间：2026-04-28
- 索引覆盖：核心 + 选定外围
- 本索引为 ControlServer.exe 的累计函数索引，保留历史已验证条目；本轮仅追加 `CMyRoom` 条目，不回滚既有 `verified` 记录。

## 核心类函数统计

| 类名 | 函数数 | 状态 |
|------|--------|------|
| CChannelOfMap | ~10 | pending |
| CMapWithChannel | ~20 | pending |
| CWorldManager | ~15 | pending |
| XControlServer | ~? | pending |
| CWorldMode | ~? | pending |
| CMazeInfo | ~? | pending |
| CServer | ~? | pending |
| CUserObject | ~? | pending |

## 状态值说明

- `pending`: 待恢复
- `decompiled`: 已反编译，待验证
- `asm_restored`: 汇编已还原
- `verified`: 已验证
- `blocked`: 阻塞中

## 函数列表

### CChannelOfMap 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::AddUserCount | 0x140001000 | verified | IDA |
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::AddSelectUser | 0x1400011C0 | verified | IDA |
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::ChangeChannelState | 0x140001270 | verified | IDA |
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::CalculateChannelState | 0x140001410 | verified | IDA |
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::IsOKToEnter | 0x140001600 | verified | IDA |

### CMapWithChannel 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::CMapWithChannel | 0x140001620 | verified | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::~CMapWithChannel | 0x140001730 | verified | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::AddChannel | 0x1400018B0 | verified | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::FindChannel | 0x140001E10 | verified | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::DeleteChannel | 0x1400020A0 | verified | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::SendDBAddChannelMap | 0x1400023C0 | verified | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::SendServerAddChannelMap | 0x140002550 | verified | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::GetChannel | 0x140002610 | verified | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::FindChannelByParty | 0x140005380 | verified | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::FindChannelByServerID | 0x140005630 | verified | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::FindChannelByUserCount | 0x140005820 | verified | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::CheckChannelUserCount | 0x1400058E0 | verified | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::FindChannelUserLow | 0x140005A40 | verified | IDA |

### CWorldManager 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldManager.cpp | CWorldManager::CWorldManager | 0x140002710 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::~CWorldManager | 0x140002760 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::AddMap | 0x1400027A0 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::DeleteMap | 0x140002B70 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::ReqEnterMap | 0x140002CA0 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::ReqEnterMapToOther | 0x140003D30 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::ReqChangeChannel | 0x140004720 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::SendChannelInfoAll | 0x140004F70 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::UpdateUserMap | 0x1400050E0 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::AddChannelOfMap | 0x140005230 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::RemoveChannelOfMap | 0x140005300 | verified | IDA |

### XControlServer 核心函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | TXSingleton<XControlServer>::Instance | 0x140006100 | verified | IDA |

### ST_CHANNEL_MAP_INFO 结构相关

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldManager.cpp | ST_CHANNEL_MAP_INFO::~ST_CHANNEL_MAP_INFO | 0x140001890 | verified | IDA |
| XControlServer | WorldManager.cpp | ST_CHANNEL_MAP_INFO::ST_CHANNEL_MAP_INFO | 0x140001DE0 | verified | IDA |

### CMyRoom 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CMyRoom.cpp | CMyRoom::RoomEnterUser | 0x140039690 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CreateMyRoom | 0x140039820 | verified | IDA |

---
### XControlServer 核心函数 (已实现)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::Instance | 0x140006100 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::InitServer | 0x14000BD00 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::Clear | 0x14000C0C0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::AddUser | 0x14000C220 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::RemoveUser | 0x14000C610 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::RemoveUser | 0x14000C560 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GetUser | 0x14000C6B0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::AddLoginServerInfo | 0x14000DAB0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::AddGameServerInfo | 0x14000DBD0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::AddCommunityServerInfo | 0x14000C9A0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::AddMazeServerInfo | 0x14000DD70 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::AddServerInfo | 0x14000DF20 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::RemoveServerInfo | 0x14000E310 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::RemoveGameServerInfo | 0x14000E0B0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ReqCreateMaze | 0x14000E3A0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ReqGoBackMaze | 0x14000EAC0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ResCreateMaze | 0x14000ED10 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::OnUpdate | 0x14000F240 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::UpdateAccountState | 0x14000F8B0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ClearUserState | 0x14000F960 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CalculateServerUserStaus | 0x1400151F0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendChatNotice | 0x14000FB10 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendChatMegaPhone | 0x14000FC60 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CheckPartyInMaze | 0x14000FD50 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CheckForceInMaze | 0x1400100B0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ChangeServer | 0x140010410 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SetUsersInfo | 0x140010740 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendDBLog (params) | 0x140010DF0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendDBLog (ST_LOG_GAME) | 0x140010F50 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_UserKick | 0x140010FF0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_Notice | 0x1400110F0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_Shutdown | 0x140011200 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_TimeEvent | 0x1400112E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_ValueEvent | 0x140011380 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_ServerOption | 0x140011420 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_CashShopBanner | 0x140011550 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::FindServerFromMaze | 0x1400115F0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GetMazeInfo | 0x1400116E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendDBAccount | 0x140011880 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendDBGame | 0x140011910 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendDBLog (XSendDBPacket) | 0x1400119A0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendCommunity | 0x140011A30 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::UpdateServerState | 0x140011A70 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::AddMyRoomServerInfo | 0x140011CB0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::RemoveMyRoomServerInfo | 0x140011D50 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GetMyRoomServer | 0x140011DC0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::FindMyRoom | 0x140011E10 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::MyRoomEnterReq | 0x140012050 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CheckAdmissionMember | 0x140015300 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendAccountDBLoginAddServerGroupInfo | 0x1400153D0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::UpdateRouletteEventInfo | - | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GetUser (by name) | 0x140010D00 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::AddServerInfo | 0x14000DF20 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::UpdateServerState | 0x140011A70 | verified | IDA |

### XControlServer GM 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::GM_UserKick | 0x140010FF0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_Notice | 0x1400110F0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_Shutdown | 0x140011200 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_TimeEvent | 0x1400112E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_ValueEvent | 0x140011380 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_ServerOption | 0x140011420 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_CashShopBanner | 0x140011550 | verified | IDA |
