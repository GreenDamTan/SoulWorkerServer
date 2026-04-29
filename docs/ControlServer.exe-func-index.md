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
| CChannelOfMap | 10 | verified |
| CMapWithChannel | 13 | verified |
| CWorldManager | 11 | verified |
| XControlServer | ~50 | verified |
| CWorldMode | 18 | verified |
| CWorldModeMgr | 17 | verified |
| CMazeInfo | ~? | verified |
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
| XControlServer | CMyRoom.cpp | CMyRoom::CMyRoom | 0x1400395F0 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::Init | 0x140039630 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::GetMyRoomState | 0x140027150 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::DeleteReq | 0x140027140 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CreateSucc | 0x1400271E0 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::EnterSucc | 0x1400398B0 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::RoomEnterUser | 0x140039690 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CreateMyRoom | 0x140039820 | verified | IDA |
| XControlServer | CMyRoom.h | CMyRoom::GetOwnerInfo | 0x140027190 | verified | IDA |
| XControlServer | CMyRoom.h | CMyRoom::GetUxMapID | 0x140027160 | verified | IDA |

### CMazeInfo 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | MazeInfo.cpp | CMazeInfo::CMazeInfo | 0x140035F10 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::~CMazeInfo | 0x140036070 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::Init (ST_CREATE_MAZE) | 0x1400360D0 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::Init (PS_CREATE_MAP) | 0x140036410 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::Init (ST_CREATE_MODE_MAZE) | 0x140036450 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetPartyID | 0x140036500 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetForceID | 0x140036530 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::UpdateMazeInfo | 0x140036560 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::SyncMazeInfo | 0x140036820 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::ResetChildMaze | 0x140036B60 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::ResetParentMaze | 0x140036B80 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::IsValidEnterMaze | 0x140036C10 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::IsDestroy | 0x140036D70 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::IsEnterMember | 0x140036DA0 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::SetDisconnectUserState | 0x140036E20 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::CheckDisconnecUsertState | 0x140036EC0 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetMazeInfo (PS_ENTER_MAP_RES) | 0x1400370A0 | verified | IDA |
| XControlServer | MazeInfo.h | CMazeInfo::GetMazeInfo (ST_MAP_INFO) | 0x140027AF0 | verified | IDA |
| XControlServer | MazeInfo.h | CMazeInfo::GetParentMaze | 0x140027B50 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::SetMazeState | 0x140029070 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetMazeState | 0x140039480 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetUserCount | 0x140039490 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetChildMaze | 0x1400394B0 | verified | IDA |

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
| XControlServer | ControlServer.cpp | XControlServer::CheckSessionID | 0x140014590 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ReqCreateMyRoom | 0x1400147E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::DeleteMyRoomReq | 0x140014B10 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::DeleteMyRoomRes | 0x140014C30 | verified | IDA |
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

### CWorldMode 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldMode.cpp | CWorldMode::CWorldMode | 0x140046BD0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::~CWorldMode | - | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::Init | 0x140046CD0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::Clear | 0x140047920 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::Update | 0x140047990 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::IsFinish | 0x140047AC0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::StartMode | 0x140047AF0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::FinishMode | 0x140047E10 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::ClearMode | 0x140048350 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::GetNextModeID | 0x140048590 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::GetModeInfo | 0x1400485A0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::ReqModeComplete | 0x1400485F0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::AddMonsterCount | 0x140048BD0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::GetModeStartTime | 0x140048CE0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::SetStartTime | 0x140048D00 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::SetClearTime | 0x14004D470 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::SetFinishTime | 0x14004D490 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::IsSuccess | 0x14004D4B0 | verified | IDA |

### CWorldModeMgr 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::CWorldModeMgr | 0x1400497C0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::~CWorldModeMgr | - | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::Init | 0x140049840 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::LoadMode | 0x1400498A0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::Update | 0x14004A050 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::SendModeSync | 0x14004A630 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::SetModeStartTime | 0x14004A9A0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::SetModeFinishTime | 0x14004AAB0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::ReqModeComplete | 0x14004ABC0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::IsActiveMode | 0x14004ACC0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::GetModeList | 0x14004AE00 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::IsTodayModeList | 0x14004AFC0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::InitMode | 0x14004B140 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::SendModeList | 0x14004B490 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::UpdateMonsterCount | 0x14004B730 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::IsEnterMode | 0x14004B8A0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::GetActiveMode | 0x14004BB40 | verified | IDA |

### CUserProcess 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserProcess.cpp | CUserProcess::Parse | 0x1400450E0 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::SyncSelectCharacter | 0x140045310 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::SyncLogoutUser | 0x1400454B0 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::SyncUpdateUserMap | 0x140045570 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::SyncUserKickout | 0x1400455D0 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqUserChatNotice | 0x140045640 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqUserChatMegaPhone | 0x1400456B0 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqUserChangeServer | 0x140045A10 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::SyncUserMoneyLog | 0x140045A80 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqUserEnterPartyMaze | 0x140045AC0 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqUserEnterForceMaze | 0x140045B70 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqUserTradePasswordStateSync | 0x140045C30 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqUserTradePasswordState | 0x140045CB0 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqNameChange | 0x140045D10 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqCheckSessionID | 0x140045D70 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqGameServerEnterUser | 0x140045E00 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqUserUpdateAuthType | 0x140045F40 | verified | IDA |

### CGMToolProcess 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::Parse | 0x140034640 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMUserKick | 0x140034730 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMNotice | 0x1400347C0 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMShutDwon | 0x140034850 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMTimeEvent | 0x1400348B0 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMValueEvent | 0x140034910 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMServerOption | 0x1400349A0 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMCashShopBanner | 0x140034A10 | verified | IDA |

### CServer 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CServer.cpp | CServer::SyncMaze | 0x140042340 | verified | IDA |
| XControlServer | CServer.cpp | CServer::RemoveMaze | 0x140041930 | verified | IDA |
| XControlServer | CServer.cpp | CServer::UpdateMaze | 0x1400421E0 | verified | IDA |
| XControlServer | CServer.cpp | CServer::OnUpdate | 0x140042550 | verified | IDA |
| XControlServer | CServer.cpp | CServer::RegisterProcess | 0x140041180 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::AddMaze | 0x140041410 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::GetMapID | 0x140041710 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::IsMaze | 0x140041800 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::GetMazeInfo | 0x140041870 | verified | IDA |

### CServerProcess 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServerProcess::Parse | 0x14003CAC0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateServer | 0x14003D0A0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMaze | 0x14003D1F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResCreateMaze | 0x14003D360 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncMaze | 0x14003D4F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncRemoveMaze | 0x14003D410 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncUpdateMaze | 0x14003D480 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqGoBackMaze | 0x14003D2A0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMap | 0x14003D5D0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqEnterMap | 0x14003D6E0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncUsersInfo | 0x14003D820 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncUserPartyInfo | 0x14003D9E0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqChangeChannel | 0x14003D130 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCheckPartyInMaze | 0x14003D8C0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCheckEnterMaze | 0x14003DC40 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncLogicThreadCount | 0x14003DBF0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqDisconnectUserSync | 0x14003DCA0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResCreateMatchingMazeFromGame | 0x14003DFF0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMatchingMazeFromCommunity | 0x14003DD80 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMatchingModeMazeFromCommunity | 0x14003DF20 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncMaxMazeID | 0x14003DD10 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::PartyMazeSync | 0x14003E9F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ForceMazeSync | 0x14003EA60 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomCreate | 0x14003ED40 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomDelete | 0x14003EE80 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterReq | 0x14003E460 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterRes | 0x14003E680 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResMyRoomDelete | 0x14003EF20 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqPostSend | 0x14003EAD0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqFindUser | 0x14003CEE0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResCreateModeMaze | 0x14003EFC0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqUpdateRouletteEvent | 0x14003F070 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqReEnterMap | 0x14003EE20 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::EnterOtherMap_cheat | 0x14003D750 | verified | IDA |

### CWorldModeProcess 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::Parse | 0x14004D5D0 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeCommand | 0x14004D680 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeComplete | 0x14004DAF0 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeUpdate | 0x14004DC20 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeEnterList | 0x14004DC90 | verified | IDA |

### CUserObject 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserObject.cpp | CUserObject::SetPartyInfo | 0x14000A1E0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetWorldState | 0x14000A500 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetBeforeServerID | 0x14000A560 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetEnterWorldModeInfo | 0x14000A860 | verified | IDA |
| XControlServer | UserObject.h | CUserObject::GetServerID | 0x14000AAB0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::CanChangeWorld | - | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::ResetWorldState | - | verified | IDA |

### XControlServer Matching/Maze 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::EnterMemberInMaze | 0x140012200 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::EnterMemberInMazeForce | 0x140012330 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CreateMatchingMaze | 0x140012460 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CreateMatchingModeMaze | 0x140013030 | verified | IDA |

---

### XControlServer Matching 重载函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::CreateMatchingMaze (Force) | 0x140012A40 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ResCreateMatchingMaze (Party) | 0x140013BE0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ResCreateMatchingMaze (Force) | 0x140013F70 | verified | IDA |

### CPartyManager 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CPartyManager.h | CPartyManager::IsLoad | 0x140027530 | verified | IDA |
| XControlServer | CPartyManager.h | CPartyManager::IsParty | 0x140030E80 | verified | IDA |
| XControlServer | CPartyManager.h | CPartyManager::FindSamePlace | 0x140030FE0 | verified | IDA |
| XControlServer | CPartyManager.h | CPartyManager::SetMember | 0x1400399A0 | verified | IDA |
| XControlServer | CPartyManager.h | CPartyManager::SetMazeID | 0x140039C00 | verified | IDA |

### CForceManager 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CForceManager.h | CForceManager::SetMember | 0x140030CF0 | verified | IDA |
| XControlServer | CForceManager.h | CForceManager::IsFull | 0x140030EE0 | verified | IDA |
| XControlServer | CForceManager.h | CForceManager::SetMazeID | 0x140030F60 | verified | IDA |
| XControlServer | CForceManager.h | CForceManager::RemoveMember | 0x140039B30 | verified | IDA |
| XControlServer | CForceManager.h | CForceManager::GetMazeID | 0x140039CD0 | verified | IDA |

### CParty 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CParty.h | CParty::IsFull | 0x140030B80 | verified | IDA |
| XControlServer | CParty.h | CParty::RemoveMember | 0x140030B00 | verified | IDA |
| XControlServer | CParty.h | CParty::FindSamePlace | 0x140030BC0 | verified | IDA |
| XControlServer | CParty.h | CParty::SetMember | 0x1400398D0 | verified | IDA |
| XControlServer | CParty.h | CParty::GetMazeID | 0x1400394B0 | verified | IDA |

### CForce 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CForce.h | CForce::CForce | 0x14003A450 | verified | IDA |
| XControlServer | CForce.h | CForce::SetMember | 0x1400398D0 | verified | IDA |
| XControlServer | CForce.h | CForce::SetMazeID | 0x14003A420 | verified | IDA |
| XControlServer | CForce.h | CForce::IsEmpty | 0x140030B80 | verified | IDA |

---

### XControlServer 其他函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::ResCreateModeMaze | 0x140013560 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ReqCheckEnterMaze | 0x1400137F0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ReqReEnterMap | 0x1400157C0 | verified | IDA |

---

### XControlServer Maze/Map 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::UpdateUserMap | 0x14000CCA0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::KickoutUser_NoLock | 0x14000D7E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ReqCreateMaze | 0x14000E3A0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ResCreateMaze | 0x14000ED10 | verified | IDA |

---

### CServer MyRoom 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.h | CServer::CreateMyRoom | 0x1400427C0 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::DelMyRoom | 0x140042A00 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::FindMyRoom | 0x140042A30 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::AddMaze | 0x140041410 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::RemoveMaze | 0x140041930 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::UpdateMaze | 0x1400421E0 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::SyncMaze | 0x140042340 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::IsMaze | 0x140041800 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::GetMazeInfo | 0x140041870 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::GetMapID | 0x140041710 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::GetSerial | 0x1400416D0 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::OnUpdate | 0x140042550 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::RegisterProcess | 0x140041180 | verified | IDA |

---

### CUserProcess 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserProcess.cpp | CUserProcess::Parse | 0x1400450E0 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::SyncSelectCharacter | 0x140045310 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::SyncLogoutUser | 0x1400454B0 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::SyncUpdateUserMap | 0x140045570 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::SyncUserKickout | 0x1400455D0 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqUserChatNotice | 0x140045640 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqUserChatMegaPhone | 0x1400456B0 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqUserChangeServer | 0x140045A10 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::SyncUserMoneyLog | 0x140045A80 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqUserEnterPartyMaze | 0x140045AC0 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqUserEnterForceMaze | 0x140045B70 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqUserTradePasswordStateSync | 0x140045C30 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqUserTradePasswordState | 0x140045CB0 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqNameChange | 0x140045D10 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqCheckSessionID | 0x140045D70 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqGameServerEnterUser | 0x140045E00 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqUserUpdateAuthType | 0x140045F40 | verified | IDA |

---

### CGMToolProcess 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::Parse | 0x140034640 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMUserKick | 0x140034730 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMNotice | 0x1400347C0 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMShutDwon | 0x140034850 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMTimeEvent | 0x1400348B0 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMValueEvent | 0x140034910 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMServerOption | 0x1400349A0 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMCashShopBanner | 0x140034A10 | verified | IDA |

---

### CChannelOfMap 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::AddUserCount | 0x140001000 | verified | IDA |
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::AddSelectUser | 0x1400011C0 | verified | IDA |
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::ChangeChannelState | 0x140001270 | verified | IDA |
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::CalculateChannelState | 0x140001410 | verified | IDA |
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::IsOKToEnter | 0x140001600 | verified | IDA |

---

### CWorldModeProcess 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::Parse | 0x14004D5D0 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeCommand | 0x14004D680 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeComplete | 0x14004DAF0 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeUpdate | 0x14004DC20 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeEnterList | 0x14004DC90 | verified | IDA |

---

### XControlServer 核心函数 (续)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::AddServerInfo | 0x14000DF20 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::RemoveServerInfo | 0x14000E310 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::RemoveGameServerInfo | 0x14000E0B0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::UpdateUserMap | 0x14000CCA0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::KickoutUser_NoLock | 0x14000D7E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::KickoutUser_UseLock | 0x14000D7E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendPacketToLoginServer | 0x14000D950 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendPacketToGameServer | 0x14000D990 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendPacketAll | 0x14000DA60 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::AddCommunityServerInfo | 0x14000DBB0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::AddGameServerInfo | 0x14000DBD0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::AddMazeServerInfo | 0x14000DD70 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ReqCreateMaze | 0x14000E3A0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::OnUpdate | 0x14000F240 | verified | IDA |

---

### CServer 函数 (MyRoom / Maze)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CServer.cpp | CServer::CreateMyRoom | 0x1400427C0 | verified | IDA |
| XControlServer | CServer.cpp | CServer::DelMyRoom | 0x140042A00 | verified | IDA |
| XControlServer | CServer.cpp | CServer::FindMyRoom | 0x140042A30 | verified | IDA |
| XControlServer | CServer.cpp | CServer::SetSyncLoad | 0x140042AF0 | verified | IDA |
| XControlServer | CServer.cpp | CServer::GetMapID | 0x140041710 | verified | IDA |
| XControlServer | CServer.cpp | CServer::IsValidEnterPartyMemberInMaze | 0x140041C80 | verified | IDA |
| XControlServer | CServer.cpp | CServer::IsValidEnterForceMemberInMaze | 0x140041F30 | verified | IDA |

---

### CServer 函数 (Basic Getters)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CServer.cpp | CServer::GetServerState | 0x14000A090 | verified | IDA |
| XControlServer | CServer.cpp | CServer::GetServerInfo | 0x14000A4E0 | verified | IDA |
| XControlServer | CServer.cpp | CServer::GetServerID | 0x14000B2B0 | verified | IDA |
| XControlServer | CServer.cpp | CServer::GetUserCount | 0x140027C60 | verified | IDA |
| XControlServer | CServer.cpp | CServer::GetMaxUserCount | 0x140027C80 | verified | IDA |
| XControlServer | CServer.cpp | CServer::GetServerType | 0x140027CA0 | verified | IDA |
| XControlServer | CServer.cpp | CServer::GetPublicIP | 0x140029010 | verified | IDA |
| XControlServer | CServer.cpp | CServer::GetPort | 0x140029030 | verified | IDA |
| XControlServer | CServer.cpp | CServer::IsSyncLoad | 0x140027110 | verified | IDA |
| XControlServer | CServer.cpp | CServer::IsRecvServerInfo | 0x140027540 | verified | IDA |

---

### CParty / CPartyManager 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CParty.cpp | CParty::RemoveMember | 0x140030B00 | verified | IDA |
| XControlServer | CParty.cpp | CParty::IsFull | 0x140030B80 | verified | IDA |
| XControlServer | CParty.cpp | CParty::FindSamePlace | 0x140030BC0 | verified | IDA |
| XControlServer | CParty.cpp | CParty::~CParty | 0x14003A3D0 | verified | IDA |
| XControlServer | CParty.cpp | CParty::GetMazeID | 0x14003A3F0 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::CPartyManager | 0x140030CA0 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::~CPartyManager | 0x140030CD0 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::IsParty | 0x140030E80 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::FindSamePlace | 0x140030FE0 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::SetMember | 0x1400399A0 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::SetMazeID | 0x140039C00 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::IsLoad | 0x140027530 | verified | IDA |

---

### CForce / CForceManager 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CForce.cpp | CForce::CForce | 0x14003A450 | verified | IDA |
| XControlServer | CForce.cpp | CForce::SetMember | 0x1400398D0 | verified | IDA |
| XControlServer | CForce.cpp | CForce::IsEmpty | 0x140039980 | verified | IDA |
| XControlServer | CForce.cpp | CForce::SetMazeID | 0x14003A420 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::SetMember | 0x140030CF0 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::IsFull | 0x140030EE0 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::SetMazeID | 0x140030F60 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::RemoveMember | 0x140039B30 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::GetMazeID | 0x140039CD0 | verified | IDA |

---### CUserObject 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserObject.cpp | CUserObject::SetPartyInfo | 0x14000A1E0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetWorldState | 0x14000A500 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetBeforeServerID | 0x14000A560 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetEnterWorldModeInfo | 0x14000A860 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetServerID | 0x14000AAB0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetServer | 0x1400290C0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::CUserObject | 0x14002A580 | verified | IDA |

---

### CMyRoom 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CMyRoom.cpp | CMyRoom::DeleteReq | 0x140027140 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::GetMyRoomState | 0x140027150 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::GetUxMapID | 0x140027160 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::GetOwnerInfo | 0x140027190 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CreateSucc | 0x1400271E0 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CMyRoom | 0x1400395F0 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::Init | 0x140039630 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::RoomEnterUser | 0x140039690 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CreateMyRoom | 0x140039820 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::EnterSucc | 0x1400398B0 | verified | IDA |

---

### CMyRoom 方法验证追加 (2026-04-29 07:22)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CMyRoom.cpp | CMyRoom::GetMyRoomState | 0x140027150 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::DeleteReq | 0x140027140 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CreateSucc | 0x1400271E0 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::Init | 0x140039630 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::GetUxMapID | 0x140027160 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::GetOwnerInfo | 0x140027190 | verified | IDA |

---

### CServerProcess MyRoom handlers 验证追加 (2026-04-29 07:22)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterReq | 0x14003E460 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterRes | 0x14003E680 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqMyRoomCreate | 0x14003ED40 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqMyRoomDelete | 0x14003EE80 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ResMyRoomDelete | 0x14003EF20 | verified | IDA |

---

### XControlServer MyRoom 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::MyRoomEnterReq | 0x140012050 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ReqCreateMyRoom | 0x1400147E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::DeleteMyRoomReq | 0x140014B10 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::DeleteMyRoomRes | 0x140014C30 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::AddMyRoomServerInfo | 0x140011CB0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GetMyRoomServer | 0x140011E00 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::FindMyRoom | 0x140011EE0 | verified | IDA |

---

### XControlServer Matching 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::CreateMatchingMaze (Party) | 0x140012460 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CreateMatchingMaze (Force) | 0x140012A40 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ResCreateMatchingMaze (Party) | 0x140013BE0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ResCreateMatchingMaze (Force) | 0x140013F70 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ResCreateModeMaze | 0x140013560 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendDBAccount | 0x140011880 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::UpdateServerState | 0x140011A70 | verified | IDA |

---

### XControlServer Caching 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::SetCachingLoad | 0x140014D40 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendCachingLoad | 0x140014E80 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CalculateServerUserStaus | 0x1400151F0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ReqReEnterMap | 0x1400157C0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CheckSessionID | 0x140014590 | verified | IDA |

---

### Process 类 Parse 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XRelayServer | ServerProcess.h | CServerProcess::Parse | 0x14003CAC0 | verified | IDA |
| XRelayServer | ServerProcess.h | CUserProcess::Parse | 0x1400450E0 | verified | IDA |
| XRelayServer | ServerProcess.h | CGMToolProcess::Parse | 0x140034640 | verified | IDA |
| XRelayServer | ServerProcess.h | CWorldModeProcess::Parse | 0x14004D5F0 | verified | IDA |

---

### CServerProcess 子命令处理函数 (main=0xF2)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqCreateServer | 0x14003D0A0 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqCreateMaze | 0x14003D1F0 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ResCreateMaze | 0x14003D360 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::SyncRemoveMaze | 0x14003D410 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::SyncUpdateMaze | 0x14003D480 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::SyncMaze | 0x14003D4F0 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqCreateMap | 0x14003D5D0 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqEnterMap | 0x14003D6E0 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::EnterOtherMap_cheat | 0x14003D750 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterReq | 0x14003E460 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterRes | 0x14003E680 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqMyRoomCreate | 0x14003ED40 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqMyRoomDelete | 0x14003EE80 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ResMyRoomDelete | 0x14003EF20 | verified | IDA |

---

### CUserProcess 子命令处理函数 (main=0xF3)

| 所属目录 | 文件名 | 函数名 | 地址 | 子命令 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- | --- |
| XRelayServer | ServerProcess.cpp | CUserProcess::SyncSelectCharacter | 0x140045310 | 0x01 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CUserProcess::SyncLogoutUser | 0x1400454B0 | 0x03 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CUserProcess::SyncUpdateUserMap | 0x140045570 | 0x04 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CUserProcess::SyncUserKickout | 0x1400455D0 | 0x07 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CUserProcess::ReqUserChatNotice | 0x140045640 | 0x11 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CUserProcess::ReqUserChangeServer | 0x140045A10 | 0x12 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CUserProcess::SyncUserMoneyLog | 0x140045A80 | 0x13 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CUserProcess::ReqUserEnterPartyMaze | 0x140045AC0 | 0x16 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CUserProcess::ReqUserChatMegaPhone | 0x1400456B0 | 0x17 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CUserProcess::ReqUserEnterForceMaze | 0x140045B70 | 0x20 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CUserProcess::ReqUserTradePasswordStateSync | 0x140045C30 | 0x26 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CUserProcess::ReqUserTradePasswordState | 0x140045CB0 | 0x27 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CUserProcess::ReqNameChange | 0x140045D10 | 0x31 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CUserProcess::ReqCheckSessionID | 0x140045D70 | 0x32 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CUserProcess::ReqUserUpdateAuthType | 0x140045F40 | 0x35 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CUserProcess::ReqGameServerEnterUser | 0x140045E00 | 0x60 | verified | IDA |

---

### CGMToolProcess 子命令处理函数 (main=0xF8)

| 所属目录 | 文件名 | 函数名 | 地址 | 子命令 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- | --- |
| XRelayServer | ServerProcess.cpp | CGMToolProcess::ReqGMUserKick | 0x140034730 | 0x01 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CGMToolProcess::ReqGMNotice | 0x1400347C0 | 0x02 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CGMToolProcess::ReqGMShutDwon | 0x140034850 | 0x03 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CGMToolProcess::ReqGMTimeEvent | 0x1400348B0 | 0x04 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CGMToolProcess::ReqGMValueEvent | 0x140034910 | 0x05 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CGMToolProcess::ReqGMServerOption | 0x1400349A0 | 0x06 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CGMToolProcess::ReqGMCashShopBanner | 0x140034A10 | 0x07 | verified | IDA |

---

### CWorldModeProcess 子命令处理函数 (main=0xFB)

| 所属目录 | 文件名 | 函数名 | 地址 | 子命令 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- | --- |
| XRelayServer | ServerProcess.cpp | CWorldModeProcess::ReqWorldModeUpdate | 0x14004DC20 | 0x02 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CWorldModeProcess::ReqWorldModeCommand | 0x14004D680 | 0x06 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CWorldModeProcess::ReqWorldModeComplete | 0x14004DAF0 | 0x07 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CWorldModeProcess::ReqWorldModeEnterList | 0x14004DC90 | 0x09 | verified | IDA |

---

### CServer 函数 (2026-04-29 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XRelayServer | ServerProcess.h | CServer::GetServerState | 0x14000A090 | verified | IDA |
| XRelayServer | ServerProcess.h | CServer::GetServerInfo | 0x14000A4E0 | verified | IDA |
| XRelayServer | ServerProcess.h | CServer::GetServerID | 0x14000B2B0 | verified | IDA |
| XRelayServer | ServerProcess.h | CServer::GetServerType | 0x140027CA0 | verified | IDA |
| XRelayServer | ServerProcess.h | CServer::GetMaxUserCount | 0x140027C80 | verified | IDA |
| XRelayServer | ServerProcess.h | CServer::GetUserCount | inline | verified | IDA |
| XRelayServer | ServerProcess.h | CServer::SetUserCount | inline | verified | IDA |
| XRelayServer | ServerProcess.h | CServer::GetSyncLoad | inline | verified | IDA |
| XRelayServer | ServerProcess.h | CServer::SetSyncLoad | inline | verified | IDA |
| XRelayServer | ServerProcess.h | CServer::CanMakeMaze | inline | verified | IDA |
| XRelayServer | ServerProcess.h | CServer::IsMaze | 0x140041800 | verified | IDA |
| XRelayServer | ServerProcess.h | CServer::GetMazeInfo | 0x140041870 | verified | IDA |
| XRelayServer | ServerProcess.h | CServer::AddMaze | 0x140041410 | verified | IDA |
| XRelayServer | ServerProcess.h | CServer::GetSerial | 0x1400416D0 | verified | IDA |
| XRelayServer | ServerProcess.h | CServer::GetMapID | 0x140041710 | verified | IDA |
| XRelayServer | ServerProcess.h | CServer::IsValidEnterPartyMemberInMaze | 0x140041C80 | verified | IDA |
| XRelayServer | ServerProcess.h | CServer::IsValidEnterForceMemberInMaze | 0x140041F30 | verified | IDA |
| XRelayServer | ServerProcess.h | CServer::CreateMyRoom | 0x1400427C0 | verified | IDA |
| XRelayServer | ServerProcess.h | CServer::DelMyRoom | 0x140042A00 | verified | IDA |
| XRelayServer | ServerProcess.h | CServer::FindMyRoom | 0x140042A30 | verified | IDA |

---

### CUserObject 函数 (2026-04-29 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserObject.cpp | CUserObject::GetServerID | 0x14000AAB0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetCID | inline | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetUAID | inline | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetPartyInfo | 0x14000A1E0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetWorldState | 0x14000A500 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetBeforeServerID | 0x14000A560 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetEnterWorldModeInfo | 0x14000A860 | verified | IDA |

---

---

## [2026-04-29 04:46 +08:00] [glm-5] - Process 类与核心方法验证追加

### Process 类 Parse 方法

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServerProcess::Parse | 0x14003CAC0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::Parse | 0x1400450E0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CGMToolProcess::Parse | 0x140034640 | verified | IDA |
| XControlServer | ServerProcess.cpp | CWorldModeProcess::Parse | 0x14004D5F0 | verified | IDA |

### CServerProcess 子处理器

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateServer | 0x14003D0A0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMaze | 0x14003D1F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResCreateMaze | 0x14003D360 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncLogicThreadCount | 0x14003DBF0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMap | 0x14003D5D0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqEnterMap | 0x14003D6E0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::EnterOtherMap_cheat | 0x14003D750 | verified | IDA |

### CUserProcess 子处理器

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CUserProcess::SyncSelectCharacter | 0x140045310 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::SyncLogoutUser | 0x1400454B0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqUserChatNotice | 0x140045640 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqGameServerEnterUser | 0x140045E00 | verified | IDA |

### CMazeInfo 方法

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetMemberVector | 0x140006690 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::IsDestroy | 0x140036D70 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::UpdateMazeInfo | 0x140036560 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::SyncMazeInfo | 0x140036820 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::ResetParentMaze | 0x140036B80 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetChildMaze | 0x1400394B0 | verified | IDA |

### CUserObject 方法

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserObject.cpp | CUserObject::GetServerID | 0x14000AAB0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetBlockType | 0x140046640 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetUAID | 0x140029260 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetCID | 0x140029220 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetName | 0x140026A40 | verified | IDA |

### XControlServer 核心方法

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::AddUser | 0x14000C220 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::RemoveUser | 0x14000C610 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GetServer | 0x14000FB10 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GetUser | 0x14000CBB0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GetMyRoomServer | 0x140011E00 | verified | IDA |

### CWorldMode / CWorldModeMgr 方法

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldMode.cpp | CWorldMode::Init | 0x140046CD0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::Init | 0x140049840 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::LoadMode | 0x1400498A0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::GetModeList | 0x14004AE00 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::SendModeList | 0x14004B490 | verified | IDA |


---

### CWorldManager 验证追加

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldManager.cpp | CWorldManager::DeleteMap | 0x140002B70 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::SendChannelInfoAll | 0x140004F70 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::UpdateUserMap | 0x1400050E0 | verified | IDA |

### CChannelOfMap 验证追加

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::AddUserCount | 0x140001000 | verified | IDA |
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::IsOKToEnter | 0x140001600 | verified | IDA |
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::CalculateChannelState | 0x140001410 | verified | IDA |

### Party/Force Manager 验证

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CPartyManager.cpp | CPartyManager::SetMazeID | 0x140039C00 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::SetMazeID | 0x140030F60 | verified | IDA |
| XControlServer | CParty.cpp | CParty::RemoveMember | 0x140030B00 | verified | IDA |


---

### CUserObject 完整验证追加 (2026-04-29)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserObject.cpp | CUserObject::GetUAID | 0x140029260 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetServerID | 0x14000AAB0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetAuthSessionID | 0x14002A730 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetAuthType | 0x1400291A0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetTradePWState | 0x140027270 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetBlockType | 0x140046640 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetBeforeServerID | 0x1400290A0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetMapIns | 0x1400291F0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetPartyInfo | 0x1400291C0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetAuthSessionID | 0x140029320 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetIP | 0x140029360 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetAuthType | 0x1400270C0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetTradePWState | 0x140029340 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetBlockType | 0x140029300 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetGroupIDTick | 0x1400292E0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetBeforeServerID | 0x14000A560 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetMapIns | 0x140029130 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetPartyInfo | 0x14000A1E0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetServer | 0x1400290C0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetWorldState | 0x14000A500 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::CanChangeWorld | 0x140044B60 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::ResetWorldState | 0x140044BB0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SendPacket | 0x140044AF0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetEnterWorldModeInfo | 0x140044C60 | verified | IDA |

### XControlServer 用户管理验证追加 (2026-04-29)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::AddUser | 0x14000C220 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::RemoveUser | 0x14000C610 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::KickoutUser_NoLock | 0x14000D7E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GetServer | 0x14000FB10 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::FindMyRoom | 0x140011EE0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GetMyRoomServer | 0x140011E00 | verified | IDA |

### CServer MyRoom 验证追加 (2026-04-29)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XRelayServer | ServerProcess.cpp | CServer::FindMyRoom | 0x140042A30 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServer::DelMyRoom | 0x140042A00 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServer::CreateMyRoom | 0x1400427C0 | verified | IDA |


### CServerProcess 验证追加 (2026-04-29 05:52)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XRelayServer | ServerProcess.cpp | CServerProcess::Parse | 0x14003CAC0 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqCreateServer | 0x14003CE10 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqUpdateServerInfo | 0x14003CEA50 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqChangeChannel | 0x14003D130 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqCreateMaze | 0x14003D1F0 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ResCreateMaze | 0x14003D360 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::SyncMaze | 0x14003D4F0 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::SyncRemoveMaze | 0x14003D410 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::SyncUpdateMaze | 0x14003D480 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqGoBackMaze | 0x14003D2A0 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqCreateMap | 0x14003D5D0 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqEnterMap | 0x14003D6E0 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqCheckPartyInMaze | 0x14003D8C0 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::SyncUsersInfo | 0x14003CE40 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::SyncUserPartyInfo | 0x14003D9E0 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::SyncLogicThreadCount | 0x14003DBF0 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqCheckEnterMaze | 0x14003DC40 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqDisconnectUserSync | 0x14003DCA0 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::SyncMaxMazeID | 0x14003DD10 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ResCreateMatchingMazeFromGame | 0x14003DFF0 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqCreateMatchingMazeFromCommunity | 0x14003DD60 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqCreateMatchingModeMazeFromCommunity | 0x14003DF20 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterReq | 0x14003E460 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterRes | 0x14003E680 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqMyRoomCreate | 0x14003ED40 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqMyRoomDelete | 0x14003EE80 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::EnterOtherMap_cheat | 0x14003D750 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::PartyMazeSync | 0x14003E9F0 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqPostSend | 0x14003EAD0 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqFindUser | 0x14003CEE0 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ResMyRoomDelete | 0x14003EF20 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ForceMazeSync | 0x14003EA60 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ResCreateModeMaze | 0x14003EFC0 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqUpdateRouletteEvent | 0x14003F070 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqReEnterMap | 0x14003EE20 | verified | IDA |

### CServer 方法验证追加 (2026-04-29 05:52)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XRelayServer | ServerProcess.cpp | CServer::RemoveMaze | 0x140041930 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServer::UpdateMaze | 0x1400421E0 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServer::SyncMaze | 0x140042340 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServer::OnUpdate | 0x140042550 | verified | IDA |

---


### CWorldModeMgr 方法验证追加 (2026-04-29 06:08)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::Init | 0x140049840 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::LoadMode | 0x1400498A0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::Update | 0x14004A050 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::SendModeSync | 0x14004A630 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::SetModeStartTime | 0x14004A9A0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::SetModeFinishTime | 0x14004AAB0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::ReqModeComplete | 0x14004ABC0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::IsActiveMode | 0x14004ACC0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::GetModeList | 0x14004AE00 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::IsTodayModeList | 0x14004AFC0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::InitMode | 0x14004B140 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::SendModeList | 0x14004B490 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::UpdateMonsterCount | 0x14004B730 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::IsEnterMode | 0x14004B8A0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::GetActiveMode | 0x14004BB40 | verified | IDA |

### GreenDamTan 时间兼容层 (人工补充)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore/XServer | GreenDamTan_TimeCompat.h | GreenDamTan::GetTickCount64Compat | N/A | verified | 人工补充 |
| XCore/XServer | GreenDamTan_TimeCompat.h | GreenDamTan::GetCurrentTime | N/A | verified | 人工补充 |
| XCore/XServer | GreenDamTan_TimeCompat.h | GreenDamTan::GetCurrentTimeMs | N/A | verified | 人工补充 |
| XCore/XServer | GreenDamTan_TimeCompat.h | GreenDamTan::GetDayOfWeek | N/A | verified | 人工补充 |
| XCore/XServer | GreenDamTan_TimeCompat.h | GreenDamTan::GetTimeHM | N/A | verified | 人工补充 |
| XCore/XServer | GreenDamTan_TimeCompat.h | GreenDamTan::SleepMs | N/A | verified | 人工补充 |

---

### XControlServer MyRoom handlers 验证追加 (2026-04-29 06:15)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::MyRoomEnterReq | 0x140012050 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ReqCreateMyRoom | 0x1400147E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::DeleteMyRoomReq | 0x140014B10 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::DeleteMyRoomRes | 0x140014C30 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::FindMyRoom | 0x140011EE0 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServer::CreateMyRoom | 0x1400427C0 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServer::DelMyRoom | 0x140042A00 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServer::FindMyRoom | 0x140042A30 | verified | IDA |

---

### CWorldMode 方法验证追加 (2026-04-29 06:25)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldMode.cpp | CWorldMode::InitMode | 0x140048D60 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::SetNextEventTime | 0x1400486E0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::GetEventTime | 0x140048B50 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::SendLogNextModeTime | 0x1400489D0 | verified | IDA |

---


### CMazeInfo 方法验证追加 (2026-04-29 06:43)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | MazeInfo.cpp | CMazeInfo::CMazeInfo | 0x140035F10 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::ResetParentMaze | 0x140036B80 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::UpdateMazeInfo | 0x140036560 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::SyncMazeInfo | 0x140036820 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::CheckDisconnecUsertState | 0x140036EC0 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::Init(ST_CREATE_MAZE) | 0x1400360D0 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::Init(ST_CREATE_MODE_MAZE) | 0x140036450 | verified | IDA |

### CMyRoom 方法验证追加 (2026-04-29 06:43)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CMyRoom.cpp | CMyRoom::RoomEnterUser | 0x140039690 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CreateMyRoom | 0x140039820 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::EnterSucc | 0x1400398B0 | verified | IDA |

---

### CMyRoom 方法验证追加 (2026-04-29 07:22)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CMyRoom.cpp | CMyRoom::GetMyRoomState | 0x140027150 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::DeleteReq | 0x140027140 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CreateSucc | 0x1400271E0 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::Init | 0x140039630 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::GetUxMapID | 0x140027160 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::GetOwnerInfo | 0x140027190 | verified | IDA |

---

### CServerProcess MyRoom handlers 验证追加 (2026-04-29 07:22)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterReq | 0x14003E460 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterRes | 0x14003E680 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqMyRoomCreate | 0x14003ED40 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ReqMyRoomDelete | 0x14003EE80 | verified | IDA |
| XRelayServer | ServerProcess.cpp | CServerProcess::ResMyRoomDelete | 0x14003EF20 | verified | IDA |

---

---

### 新增验证函数 (2026-04-29)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::UpdateUserMap | 0x14000CCA0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendPacketToLoginServer | 0x14000D950 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendPacketToGameServer | 0x14000D990 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendPacketAll | 0x14000DA60 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GetUser | 0x14000CBB0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GetServer | 0x14000FB10 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::KickoutUser_UseLock | 0x14000D780 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::KickoutUser_NoLock | 0x14000D7E0 | verified | IDA |


---

### 新增验证函数 (2026-04-29 11:30)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::AddUserCount | 0x140001000 | verified | IDA |
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::ChangeChannelState | 0x140001270 | verified | IDA |
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::CalculateChannelState | 0x140001410 | verified | IDA |
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::IsOKToEnter | 0x140001600 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::UpdateUserMap | 0x1400050E0 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::AddChannelOfMap | 0x140005230 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::RemoveChannelOfMap | 0x140005300 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::SendChannelInfoAll | 0x140004F70 | verified | IDA |



---

### Party/Force 成员管理验证追加 (2026-04-29 08:16)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CParty.cpp | CParty::RemoveMember | 0x140030B00 | verified | IDA |
| XControlServer | CParty.cpp | CParty::IsFull | 0x140030B80 | verified | IDA |
| XControlServer | CParty.cpp | CParty::FindSamePlace | 0x140030BC0 | verified | IDA |
| XControlServer | CParty.cpp | CParty::GetMazeID | 0x14003A3F0 | verified | IDA |
| XControlServer | CForce.cpp | CForce::SetMember | 0x1400398D0 | verified | IDA |
| XControlServer | CForce.cpp | CForce::IsEmpty | 0x140039980 | verified | IDA |
| XControlServer | CForce.cpp | CForce::SetMazeID | 0x14003A420 | verified | IDA |
| XControlServer | CForce.cpp | CForce::CForce | 0x14003A450 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::SetMember | 0x1400399A0 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::SetMazeID | 0x140039C00 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::FindSamePlace | 0x140030FE0 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::SetMember | 0x140030CF0 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::RemoveMember | 0x140039B30 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::IsFull | 0x140030EE0 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::SetMazeID | 0x140030F60 | verified | IDA |

---

### Maze 管理方法验证追加 (2026-04-29 08:16)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::FindServerFromMaze | 0x1400115F0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GetMazeInfo | 0x1400116E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ReqCreateMaze | 0x14000E3A0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ResCreateMaze | 0x14000ED10 | verified | IDA |


---

### CMazeInfo 核心方法验证追加 (2026-04-29 08:16)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | MazeInfo.cpp | CMazeInfo::CMazeInfo | 0x140035F10 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::Init | 0x1400360D0 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetMazeInfo | 0x1400370A0 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::IsDestroy | 0x140036D70 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::IsValidEnterMaze | 0x140036C10 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::IsEnterMember | 0x140036DA0 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetMazeState | 0x140039480 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetUserCount | 0x140039490 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetPartyID | 0x140036500 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetForceID | 0x140036530 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::UpdateMazeInfo | 0x140036560 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::SyncMazeInfo | 0x140036820 | verified | IDA |


---

### CServer 迷宫管理方法验证追加 (2026-04-29 08:17)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CServer.cpp | CServer::AddMaze | 0x140041410 | verified | IDA |
| XControlServer | CServer.cpp | CServer::GetMazeInfo | 0x140041870 | verified | IDA |
| XControlServer | CServer.cpp | CServer::RemoveMaze | 0x140041930 | verified | IDA |
| XControlServer | CServer.cpp | CServer::GetSerial | 0x1400416D0 | verified | IDA |
| XControlServer | CServer.cpp | CServer::GetMapID | 0x140041710 | verified | IDA |
| XControlServer | CServer.cpp | CServer::CanMakeMaze | 0x140042780 | verified | IDA |
| XControlServer | CServer.cpp | CServer::CreateMyRoom | 0x1400427C0 | verified | IDA |
| XControlServer | CServer.cpp | CServer::OnUpdate | 0x140042550 | verified | IDA |
