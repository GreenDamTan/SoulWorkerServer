# ControlServer.exe 函数索引

## 概述

- 总函数数：6797
- IDA 实例：Port 10001
- 更新时间：2026-04-30
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
| CServer | ~28 | verified |
| CUserObject | ~15 | verified |
| CRouletteEventMgr | 10 | verified |

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
| XControlServer | WorldManager.cpp | CChannelOfMap::AddUserCount | 0x140001000 | verified | IDA |
| XControlServer | WorldManager.cpp | CChannelOfMap::AddSelectUser | 0x1400011C0 | verified | IDA |
| XControlServer | WorldManager.cpp | CChannelOfMap::ChangeChannelState | 0x140001270 | verified | IDA |
| XControlServer | WorldManager.cpp | CChannelOfMap::CalculateChannelState | 0x140001410 | verified | IDA |
| XControlServer | WorldManager.cpp | CChannelOfMap::IsOKToEnter | 0x140001600 | verified | IDA |

### CMapWithChannel 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldManager.cpp | CMapWithChannel::CMapWithChannel | 0x140001620 | verified | IDA |
| XControlServer | WorldManager.cpp | CMapWithChannel::~CMapWithChannel | 0x140001730 | verified | IDA |
| XControlServer | WorldManager.cpp | CMapWithChannel::AddChannel | 0x1400018B0 | verified | IDA |
| XControlServer | WorldManager.cpp | CMapWithChannel::FindChannel(dwServerID,PartyInfo,ActorID,Raid) | 0x140001E10 | verified | IDA |
| XControlServer | WorldManager.cpp | CMapWithChannel::FindChannel(int nChannel) | 0x140001FF0 | verified | IDA |
| XControlServer | WorldManager.cpp | CMapWithChannel::DeleteChannel | 0x1400020A0 | verified | IDA |
| XControlServer | WorldManager.cpp | CMapWithChannel::SendDBAddChannelMap | 0x1400023C0 | verified | IDA |
| XControlServer | WorldManager.cpp | CMapWithChannel::SendServerAddChannelMap | 0x140002550 | verified | IDA |
| XControlServer | WorldManager.cpp | CMapWithChannel::GetChannel | 0x140002610 | verified | IDA |
| XControlServer | WorldManager.cpp | CMapWithChannel::FindChannelByParty | 0x140005380 | verified | IDA |
| XControlServer | WorldManager.cpp | CMapWithChannel::FindChannelByServerID | 0x140005630 | verified | IDA |
| XControlServer | WorldManager.cpp | CMapWithChannel::FindChannelByUserCount | 0x140005820 | verified | IDA |
| XControlServer | WorldManager.cpp | CMapWithChannel::CheckChannelUserCount | 0x1400058E0 | verified | IDA |
| XControlServer | WorldManager.cpp | CMapWithChannel::FindChannelUserLow(pInfo,nNowChannel,Raid) | 0x140005A40 | verified | IDA |
| XControlServer | WorldManager.cpp | CMapWithChannel::FindChannelUserLow(bRaid) | 0x140005D70 | verified | IDA |

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

### CPartyManager 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CPartyManager.cpp | CPartyManager::CPartyManager | 0x140030CA0 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::~CPartyManager | 0x140030CD0 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::SetMember | 0x1400399A0 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::SetMazeID | 0x140039C00 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::IsParty | 0x140030E80 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::FindSamePlace | 0x140030FE0 | verified | IDA |
| XControlServer | CPartyManager.h | CPartyManager::IsLoad | 0x140027530 | verified | IDA |

### CParty 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CParty.cpp | CParty::FindSamePlace | 0x140030BC0 | verified | IDA |
| XControlServer | CParty.cpp | CParty::RemoveMember | 0x140030B00 | verified | IDA |
| XControlServer | CParty.cpp | CParty::IsFull | 0x140030B80 | verified | IDA |
| XControlServer | CParty.h | CParty::GetMazeID | 0x14003A3F0 | verified | IDA |
| XControlServer | CParty.cpp | CParty::~CParty | 0x14003A3D0 | verified | IDA |

### CForceManager 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CForceManager.cpp | CForceManager::SetMember | 0x140030CF0 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::IsFull | 0x140030EE0 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::SetMazeID | 0x140030F60 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::RemoveMember | 0x140039B30 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::GetMazeID | 0x140039CD0 | verified | IDA |

### CForce 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CForce.cpp | CForce::CForce | 0x14003A450 | verified | IDA |
| XControlServer | CForce.cpp | CForce::SetMember | 0x1400398D0 | verified | IDA |
| XControlServer | CForce.cpp | CForce::IsEmpty | 0x140039980 | verified | IDA |
| XControlServer | CForce.h | CForce::SetMazeID | 0x14003A420 | verified | IDA |

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
| XControlServer | ControlServer.cpp | XControlServer::SendChatNotice | 0x14000FBC0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendChatMegaPhone | 0x14000FC50 | verified | IDA |
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
| XControlServer | ControlServer.cpp | XControlServer::EnterMemberInMaze | 0x140012200 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::EnterMemberInMazeForce | 0x140012330 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CreateMatchingMaze (Party) | 0x140012460 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CreateMatchingMaze (Force) | 0x140012A40 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CreateMatchingModeMaze | 0x140013030 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ResCreateModeMaze | 0x140013560 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ReqReEnterMap | 0x1400157C0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SyncEventMaze | 0x140015B20 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CheckAdmissionMember | 0x140015300 | verified | IDA |

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

### XControlServer 辅助函数 (已验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::SetName | 0x14000BCD0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SetConsoleHandler | 0x14000C1F0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::KickoutUser_UseLock | 0x14000D780 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::KickoutUser_NoLock | 0x14000D7E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendPacketToLoginServer | 0x14000D950 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendPacketToGameServer | 0x14000D990 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendPacketAll | 0x14000DA60 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GetServer | 0x14000FB10 | verified | IDA |

### CServerProcess 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServerProcess::Parse | 0x14003CAC0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateServer | 0x14003D0A0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqUpdateServerInfo | 0x14003D1A0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqChangeChannel | 0x14003D130 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMaze | 0x14003D1F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResCreateMaze | 0x14003D360 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncMaze | 0x14003D4F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncRemoveMaze | 0x14003D410 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncUpdateMaze | 0x14003D480 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqGoBackMaze | 0x14003D2A0 | verified | IDA |

### CWorldModeProcess 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::Parse | 0x14004D5F0 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeCommand | 0x14004D680 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeComplete | 0x14004DAF0 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeUpdate | 0x14004DC20 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeEnterList | 0x14004DC90 | verified | IDA |

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
| XControlServer | CServer.cpp | CServer::GetSerial | 0x1400416D0 | verified | IDA |
| XControlServer | CServer.cpp | CServer::GetMapID | 0x140041710 | verified | IDA |
| XControlServer | CServer.cpp | CServer::CreateMyRoom | 0x1400427C0 | verified | IDA |
| XControlServer | CServer.cpp | CServer::IsValidEnterPartyMemberInMaze | 0x140041C80 | verified | IDA |
| XControlServer | CServer.cpp | CServer::IsValidEnterForceMemberInMaze | 0x140041F30 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::AddMaze | 0x140041410 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::GetMapID | 0x140041710 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::IsMaze | 0x140041800 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::GetMazeInfo | 0x140041870 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::Init | 0x140041320 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::SetServerInfo | 0x140041590 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::UpdateServerInfo | 0x140041660 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::IsSyncLoad | 0x140027110 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::GetUserCount | 0x140027C60 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::GetServerType | 0x140027CA0 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::GetPublicIP | 0x140029010 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::GetPort | 0x140029030 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::GetGroupID | 0x140029050 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::ReqEnterMap | 0x140002CA0 | verified | IDA |

### CServer MyRoom 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.h | CServer::UpdateMaze | 0x1400421E0 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::SyncMaze | 0x140042340 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::OnUpdate | 0x140042550 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::CanMakeMaze | 0x140042780 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::CreateMyRoom | 0x1400427C0 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::DelMyRoom | 0x140042A00 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::FindMyRoom | 0x140042A30 | verified | IDA |
| XControlServer | CServer.cpp | CServer::RegisterProcess | 0x140041180 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::GetMapID | 0x140041710 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::GetSerial | 0x1400416D0 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::AddMaze | 0x140041410 | verified | IDA |
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
| XControlServer | UserObject.cpp | CUserObject::CanChangeWorld | 0x140044B60 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::ResetWorldState | 0x140044BB0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetEnterWorldModeInfo | 0x140044C60 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::LoadEnterWorldModeInfo | 0x14004E040 | verified | IDA |

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
| XControlServer | WorldManager.cpp | CChannelOfMap::AddUserCount | 0x140001000 | verified | IDA |
| XControlServer | WorldManager.cpp | CChannelOfMap::AddSelectUser | 0x1400011C0 | verified | IDA |
| XControlServer | WorldManager.cpp | CChannelOfMap::ChangeChannelState | 0x140001270 | verified | IDA |
| XControlServer | WorldManager.cpp | CChannelOfMap::CalculateChannelState | 0x140001410 | verified | IDA |
| XControlServer | WorldManager.cpp | CChannelOfMap::IsOKToEnter | 0x140001600 | verified | IDA |

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
| XControlServer | CServer.h | CServer::GetServerState | 0x14000A090 | verified | IDA |
| XControlServer | CServer.h | CServer::GetServerInfo | 0x14000A4E0 | verified | IDA |
| XControlServer | CServer.h | CServer::GetServerID | 0x14000B2B0 | verified | IDA |
| XControlServer | CServer.h | CServer::GetUserCount | 0x140027C60 | verified | IDA |
| XControlServer | CServer.h | CServer::GetMaxUserCount | 0x140027C80 | verified | IDA |
| XControlServer | CServer.h | CServer::GetServerType | 0x140027CA0 | verified | IDA |
| XControlServer | CServer.h | CServer::GetPublicIP | 0x140029010 | verified | IDA |
| XControlServer | CServer.h | CServer::GetPort | 0x140029030 | verified | IDA |
| XControlServer | CServer.h | CServer::IsSyncLoad | 0x140027110 | verified | IDA |
| XControlServer | CServer.h | CServer::IsRecvServerInfo | 0x140027540 | verified | IDA |

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
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterReq | 0x14003E460 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterRes | 0x14003E680 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomCreate | 0x14003ED40 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomDelete | 0x14003EE80 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResMyRoomDelete | 0x14003EF20 | verified | IDA |

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
| XControlServer | ServerProcess.h | CServerProcess::Parse | 0x14003CAC0 | verified | IDA |
| XControlServer | ServerProcess.h | CUserProcess::Parse | 0x1400450E0 | verified | IDA |
| XControlServer | ServerProcess.h | CGMToolProcess::Parse | 0x140034640 | verified | IDA |
| XControlServer | ServerProcess.h | CWorldModeProcess::Parse | 0x14004D5F0 | verified | IDA |

---

### CServerProcess 子命令处理函数 (main=0xF2)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateServer | 0x14003D0A0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMaze | 0x14003D1F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResCreateMaze | 0x14003D360 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncRemoveMaze | 0x14003D410 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncUpdateMaze | 0x14003D480 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncMaze | 0x14003D4F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMap | 0x14003D5D0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqEnterMap | 0x14003D6E0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::EnterOtherMap_cheat | 0x14003D750 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterReq | 0x14003E460 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterRes | 0x14003E680 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomCreate | 0x14003ED40 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomDelete | 0x14003EE80 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResMyRoomDelete | 0x14003EF20 | verified | IDA |

---

### CUserProcess 子命令处理函数 (main=0xF3)

| 所属目录 | 文件名 | 函数名 | 地址 | 子命令 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CUserProcess::SyncSelectCharacter | 0x140045310 | 0x01 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::SyncLogoutUser | 0x1400454B0 | 0x03 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::SyncUpdateUserMap | 0x140045570 | 0x04 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::SyncUserKickout | 0x1400455D0 | 0x07 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqUserChatNotice | 0x140045640 | 0x11 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqUserChangeServer | 0x140045A10 | 0x12 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::SyncUserMoneyLog | 0x140045A80 | 0x13 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqUserEnterPartyMaze | 0x140045AC0 | 0x16 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqUserChatMegaPhone | 0x1400456B0 | 0x17 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqUserEnterForceMaze | 0x140045B70 | 0x20 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqUserTradePasswordStateSync | 0x140045C30 | 0x26 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqUserTradePasswordState | 0x140045CB0 | 0x27 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqNameChange | 0x140045D10 | 0x31 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqCheckSessionID | 0x140045D70 | 0x32 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqUserUpdateAuthType | 0x140045F40 | 0x35 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqGameServerEnterUser | 0x140045E00 | 0x60 | verified | IDA |

---

### CGMToolProcess 子命令处理函数 (main=0xF8)

| 所属目录 | 文件名 | 函数名 | 地址 | 子命令 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CGMToolProcess::ReqGMUserKick | 0x140034730 | 0x01 | verified | IDA |
| XControlServer | ServerProcess.cpp | CGMToolProcess::ReqGMNotice | 0x1400347C0 | 0x02 | verified | IDA |
| XControlServer | ServerProcess.cpp | CGMToolProcess::ReqGMShutDwon | 0x140034850 | 0x03 | verified | IDA |
| XControlServer | ServerProcess.cpp | CGMToolProcess::ReqGMTimeEvent | 0x1400348B0 | 0x04 | verified | IDA |
| XControlServer | ServerProcess.cpp | CGMToolProcess::ReqGMValueEvent | 0x140034910 | 0x05 | verified | IDA |
| XControlServer | ServerProcess.cpp | CGMToolProcess::ReqGMServerOption | 0x1400349A0 | 0x06 | verified | IDA |
| XControlServer | ServerProcess.cpp | CGMToolProcess::ReqGMCashShopBanner | 0x140034A10 | 0x07 | verified | IDA |

---

### CWorldModeProcess 子命令处理函数 (main=0xFB)

| 所属目录 | 文件名 | 函数名 | 地址 | 子命令 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CWorldModeProcess::ReqWorldModeUpdate | 0x14004DC20 | 0x02 | verified | IDA |
| XControlServer | ServerProcess.cpp | CWorldModeProcess::ReqWorldModeCommand | 0x14004D680 | 0x06 | verified | IDA |
| XControlServer | ServerProcess.cpp | CWorldModeProcess::ReqWorldModeComplete | 0x14004DAF0 | 0x07 | verified | IDA |
| XControlServer | ServerProcess.cpp | CWorldModeProcess::ReqWorldModeEnterList | 0x14004DC90 | 0x09 | verified | IDA |

---

### CServer 函数 (2026-04-29 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.h | CServer::GetServerState | 0x14000A090 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::GetServerInfo | 0x14000A4E0 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::GetServerID | 0x14000B2B0 | verified | IDA |
| XControlServer | CServer.h | CServer::GetServerType | 0x140027CA0 | verified | IDA |
| XControlServer | CServer.h | CServer::GetMaxUserCount | 0x140027C80 | verified | IDA |
| XControlServer | CServer.h | CServer::GetUserCount | inline | verified | IDA |
| XControlServer | CServer.h | CServer::SetUserCount | inline | verified | IDA |
| XControlServer | CServer.h | CServer::GetSyncLoad | inline | verified | IDA |
| XControlServer | CServer.h | CServer::SetSyncLoad | inline | verified | IDA |
| XControlServer | CServer.h | CServer::CanMakeMaze | inline | verified | IDA |
| XControlServer | CServer.h | CServer::IsMaze | 0x140041800 | verified | IDA |
| XControlServer | CServer.h | CServer::GetMazeInfo | 0x140041870 | verified | IDA |
| XControlServer | CServer.h | CServer::AddMaze | 0x140041410 | verified | IDA |
| XControlServer | CServer.h | CServer::GetSerial | 0x1400416D0 | verified | IDA |
| XControlServer | CServer.h | CServer::GetMapID | 0x140041710 | verified | IDA |
| XControlServer | CServer.h | CServer::IsValidEnterPartyMemberInMaze | 0x140041C80 | verified | IDA |
| XControlServer | CServer.h | CServer::IsValidEnterForceMemberInMaze | 0x140041F30 | verified | IDA |
| XControlServer | CServer.h | CServer::CreateMyRoom | 0x1400427C0 | verified | IDA |
| XControlServer | CServer.h | CServer::DelMyRoom | 0x140042A00 | verified | IDA |
| XControlServer | CServer.h | CServer::FindMyRoom | 0x140042A30 | verified | IDA |

---

### CUserObject 函数 (2026-04-29 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserObject.h | CUserObject::GetServerID | 0x14000AAB0 | verified | IDA |
| XControlServer | UserObject.h | CUserObject::GetCID | inline | verified | IDA |
| XControlServer | UserObject.h | CUserObject::GetUAID | inline | verified | IDA |
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
| XControlServer | MazeInfo.h | CMazeInfo::GetMemberVector | 0x140006690 | verified | IDA |
| XControlServer | MazeInfo.h | CMazeInfo::IsDestroy | 0x140036D70 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::UpdateMazeInfo | 0x140036560 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::SyncMazeInfo | 0x140036820 | verified | IDA |
| XControlServer | MazeInfo.h | CMazeInfo::ResetParentMaze | 0x140036B80 | verified | IDA |
| XControlServer | MazeInfo.h | CMazeInfo::GetChildMaze | 0x1400394B0 | verified | IDA |

### CUserObject 方法

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserObject.h | CUserObject::GetServerID | 0x14000AAB0 | verified | IDA |
| XControlServer | UserObject.h | CUserObject::GetBlockType | 0x140046640 | verified | IDA |
| XControlServer | UserObject.h | CUserObject::GetUAID | 0x140029260 | verified | IDA |
| XControlServer | UserObject.h | CUserObject::GetCID | 0x140029220 | verified | IDA |
| XControlServer | UserObject.h | CUserObject::GetName | 0x140026A40 | verified | IDA |

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
| XControlServer | UserObject.h | CUserObject::GetUAID | 0x140029260 | verified | IDA |
| XControlServer | UserObject.h | CUserObject::GetServerID | 0x14000AAB0 | verified | IDA |
| XControlServer | UserObject.h | CUserObject::GetAuthSessionID | 0x14002A730 | verified | IDA |
| XControlServer | UserObject.h | CUserObject::GetAuthType | 0x1400291A0 | verified | IDA |
| XControlServer | UserObject.h | CUserObject::GetTradePWState | 0x140027270 | verified | IDA |
| XControlServer | UserObject.h | CUserObject::GetBlockType | 0x140046640 | verified | IDA |
| XControlServer | UserObject.h | CUserObject::GetBeforeServerID | 0x1400290A0 | verified | IDA |
| XControlServer | UserObject.h | CUserObject::GetMapIns | 0x1400291F0 | verified | IDA |
| XControlServer | UserObject.h | CUserObject::GetPartyInfo | 0x1400291C0 | verified | IDA |
| XControlServer | UserObject.h | CUserObject::SetAuthSessionID | 0x140029320 | verified | IDA |
| XControlServer | UserObject.h | CUserObject::SetIP | 0x140029360 | verified | IDA |
| XControlServer | UserObject.h | CUserObject::SetAuthType | 0x1400270C0 | verified | IDA |
| XControlServer | UserObject.h | CUserObject::SetTradePWState | 0x140029340 | verified | IDA |
| XControlServer | UserObject.h | CUserObject::SetBlockType | 0x140029300 | verified | IDA |
| XControlServer | UserObject.h | CUserObject::SetGroupIDTick | 0x1400292E0 | verified | IDA |
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
| XControlServer | CServer.cpp | CServer::FindMyRoom | 0x140042A30 | verified | IDA |
| XControlServer | CServer.cpp | CServer::DelMyRoom | 0x140042A00 | verified | IDA |
| XControlServer | CServer.cpp | CServer::CreateMyRoom | 0x1400427C0 | verified | IDA |


### CServerProcess 验证追加 (2026-04-29 05:52)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServerProcess::Parse | 0x14003CAC0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateServer | 0x14003CE10 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqUpdateServerInfo | 0x14003CEA50 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqChangeChannel | 0x14003D130 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMaze | 0x14003D1F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResCreateMaze | 0x14003D360 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncMaze | 0x14003D4F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncRemoveMaze | 0x14003D410 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncUpdateMaze | 0x14003D480 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqGoBackMaze | 0x14003D2A0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMap | 0x14003D5D0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqEnterMap | 0x14003D6E0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCheckPartyInMaze | 0x14003D8C0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncUsersInfo | 0x14003CE40 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncUserPartyInfo | 0x14003D9E0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncLogicThreadCount | 0x14003DBF0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCheckEnterMaze | 0x14003DC40 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqDisconnectUserSync | 0x14003DCA0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncMaxMazeID | 0x14003DD10 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResCreateMatchingMazeFromGame | 0x14003DFF0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMatchingMazeFromCommunity | 0x14003DD60 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMatchingModeMazeFromCommunity | 0x14003DF20 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterReq | 0x14003E460 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterRes | 0x14003E680 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomCreate | 0x14003ED40 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomDelete | 0x14003EE80 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::EnterOtherMap_cheat | 0x14003D750 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::PartyMazeSync | 0x14003E9F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqPostSend | 0x14003EAD0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqFindUser | 0x14003CEE0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResMyRoomDelete | 0x14003EF20 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ForceMazeSync | 0x14003EA60 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResCreateModeMaze | 0x14003EFC0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqUpdateRouletteEvent | 0x14003F070 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqReEnterMap | 0x14003EE20 | verified | IDA |

### CServer 方法验证追加 (2026-04-29 05:52)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServer::RemoveMaze | 0x140041930 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServer::UpdateMaze | 0x1400421E0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServer::SyncMaze | 0x140042340 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServer::OnUpdate | 0x140042550 | verified | IDA |

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
| XControlServer | ServerProcess.cpp | CServer::CreateMyRoom | 0x1400427C0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServer::DelMyRoom | 0x140042A00 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServer::FindMyRoom | 0x140042A30 | verified | IDA |

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
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterReq | 0x14003E460 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterRes | 0x14003E680 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomCreate | 0x14003ED40 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomDelete | 0x14003EE80 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResMyRoomDelete | 0x14003EF20 | verified | IDA |

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

### CUserObject 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserObject.cpp | CUserObject::GetCID | 0x140029220 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetName | 0x140026A40 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetUAID | 0x140029260 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetPartyInfo | 0x14000A1E0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetWorldState | 0x14000A500 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetBeforeServerID | 0x14000A560 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetEnterWorldModeInfo | 0x14000A860 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetServerID | 0x14000AAB0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetServer | 0x1400290C0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::CUserObject | 0x14002A580 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::~CUserObject | 0x140020920 | verified | IDA |

### CWorldModeMgr 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::CWorldModeMgr | 0x1400497C0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::~CWorldModeMgr | 0x140049820 | verified | IDA |
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

### CPartyManager 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | PartyManager.cpp | CPartyManager::CPartyManager | 0x140030CA0 | verified | IDA |
| XControlServer | PartyManager.cpp | CPartyManager::~CPartyManager | 0x140030CD0 | verified | IDA |
| XControlServer | PartyManager.cpp | CPartyManager::IsLoad | 0x140027530 | verified | IDA |
| XControlServer | PartyManager.cpp | CPartyManager::IsParty | 0x140030E80 | verified | IDA |
| XControlServer | PartyManager.cpp | CPartyManager::FindSamePlace | 0x140030FE0 | verified | IDA |
| XControlServer | PartyManager.cpp | CPartyManager::SetMember | 0x1400399A0 | verified | IDA |
| XControlServer | PartyManager.cpp | CPartyManager::SetMazeID | 0x140039C00 | verified | IDA |

### CForce / CParty 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | Force.cpp | CForce::CForce | 0x14003A450 | verified | IDA |
| XControlServer | Force.cpp | CForce::SetMember | 0x14003A590 | verified | IDA |
| XControlServer | Force.cpp | CForce::IsEmpty | 0x14003A6A0 | verified | IDA |
| XControlServer | Force.cpp | CForce::SetMazeID | 0x14003A6D0 | verified | IDA |
| XControlServer | Party.cpp | CParty::RemoveMember | 0x140030B00 | verified | IDA |
| XControlServer | Party.cpp | CParty::IsFull | 0x140030F50 | verified | IDA |
| XControlServer | Party.cpp | CParty::FindSamePlace | 0x140030F80 | verified | IDA |
| XControlServer | Party.cpp | CParty::~CParty | 0x140030CF0 | verified | IDA |
| XControlServer | Party.cpp | CParty::GetMazeID | 0x14003A0F0 | verified | IDA |


---

### Process 类和发送方法验证追加 (2026-04-29 10:59 +08:00)

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
| XControlServer | GMToolProcess.cpp | CGMToolProcess::Parse | 0x140034640 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMUserKick | 0x140034730 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMNotice | 0x1400347C0 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMShutDwon | 0x140034850 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMTimeEvent | 0x1400348B0 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMValueEvent | 0x140034910 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMServerOption | 0x1400349A0 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMCashShopBanner | 0x140034A10 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::Parse | 0x14004D5F0 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeCommand | 0x14004D680 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeComplete | 0x14004DAF0 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeUpdate | 0x14004DC20 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeEnterList | 0x14004DC90 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendPacketAll | 0x14000DA60 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendPacketToGameServer | 0x14000D990 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendPacketToLoginServer | 0x14000D950 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::KickoutUser_UseLock | 0x14000D780 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::KickoutUser_NoLock | 0x14000D7E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ChangeServer | 0x140010410 | verified | IDA |
| XControlServer | CServer.cpp | CServer::OnUpdate | 0x140042550 | verified | IDA |
| XControlServer | CServer.cpp | CServer::UpdateMaze | 0x1400421E0 | verified | IDA |

---

### XControlServer 核心方法验证追加 (2026-04-29 11:06 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::AddUser | 0x14000C220 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::RemoveUser | 0x14000C610 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::UpdateAuthType | 0x140015560 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CalculateServerUserStaus | 0x1400151F0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::MyRoomEnterReq | 0x140012050 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::InitServer | 0x14000BD00 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GetUser | 0x14000CBB0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GetUser | 0x140010D00 | verified | IDA |

---

### CWorldManager 核心方法验证追加 (2026-04-29 11:09 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldManager.cpp | CWorldManager::ReqEnterMap | 0x140002CA0 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::AddMap | 0x1400027A0 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::DeleteMap | 0x140002B70 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::ReqEnterMapToOther | 0x140003D30 | verified | IDA |

---

### MyRoom 核心函数验证追加 (2026-04-29 11:35 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CMyRoom.cpp | CMyRoom::GetMyRoomState | 0x140027150 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::DeleteReq | 0x140027140 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CreateSucc | 0x1400271E0 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::EnterSucc | 0x1400398B0 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CreateMyRoom | 0x140039820 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::RoomEnterUser | 0x140039690 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CMyRoom | 0x1400395F0 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::Init | 0x140039630 | verified | IDA |
| XControlServer | CServer.cpp | CServer::CreateMyRoom | 0x1400427C0 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::DelMyRoom | 0x140042A00 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::FindMyRoom | 0x140042A30 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::MyRoomEnterReq | 0x140012050 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ReqCreateMyRoom | 0x1400147E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::DeleteMyRoomReq | 0x140014B10 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::DeleteMyRoomRes | 0x140014C30 | verified | IDA |

---

### Party/Force Manager 核心函数验证追加 (2026-04-29 11:35 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CPartyManager.h | CPartyManager::SetMember | 0x1400399A0 | verified | IDA |
| XControlServer | CPartyManager.h | CPartyManager::SetMazeID | 0x140039C00 | verified | IDA |
| XControlServer | CParty.h | CParty::RemoveMember | 0x140030B00 | verified | IDA |
| XControlServer | CParty.h | CParty::IsFull | 0x140030B80 | verified | IDA |
| XControlServer | CParty.h | CParty::SetMember | 0x1400398D0 | verified | IDA |
| XControlServer | CParty.h | CParty::SetMazeID | 0x14003A420 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::CMazeInfo | 0x140035F10 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::ReqChangeChannel | 0x140004720 | verified | IDA |

### DB 发送方法验证追加 (2026-04-29 11:45 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::SendDBAccount | 0x140011880 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendDBGame | 0x140011910 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendDBLog | 0x1400119A0 | verified | IDA |
| XControlServer | CServer.cpp | CServer::CanMakeMaze | 0x140042780 | verified | IDA |
| XControlServer | CServer.cpp | CServer::RegisterProcess | 0x140041180 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::UpdateServerState | 0x140011A70 | verified | IDA |

---

---

### CServerProcess / CWorldManager 函数验证追加 (2026-04-29 12:58 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServerProcess::Parse | 0x14003CAC0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncUsersInfo | 0x14003D950 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqDisconnectUserSync | 0x14003DCA0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterReq | 0x14003E460 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::PartyMazeSync | 0x14003E9F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ForceMazeSync | 0x14003EA60 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResMyRoomDelete | 0x14003EF20 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::UpdateUserMap | 0x1400050E0 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::AddChannelOfMap | 0x140005230 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::RemoveChannelOfMap | 0x140005300 | verified | IDA |


---

### XControlServer 核心函数验证追加 (2026-04-29 13:05 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::SetUsersInfo | 0x140010740 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::UpdateUserMap | 0x14000CCA0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ReqGoBackMaze | 0x14000EAC0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::OnUpdate | 0x14000F240 | verified | IDA |


---

### GM 函数验证追加 (2026-04-29 13:05 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::GM_UserKick | 0x140010FF0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_Notice | 0x1400110F0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_Shutdown | 0x140011200 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_TimeEvent | 0x1400112E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_ValueEvent | 0x140011380 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_ServerOption | 0x140011420 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_CashShopBanner | 0x140011550 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::GetServerState | 0x14000A090 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::GetServerInfo | 0x14000A4E0 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::GetServerID | 0x14000B2B0 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::UpdateMaze | 0x1400421E0 | verified | IDA |


---

### CServer / CMazeInfo / CMyRoom 函数验证追加 (2026-04-29 13:09 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.h | CServer::OnUpdate | 0x140042550 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::RemoveMaze | 0x140041930 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::IsValidEnterPartyMemberInMaze | 0x140041C80 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::RoomEnterUser | 0x140039690 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CreateMyRoom | 0x140039820 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetMemberVector | 0x140006690 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::CMazeInfo | 0x140035F10 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::Init | 0x1400360D0 | verified | IDA |



---

### CForceManager / CForce 函数验证追加 (2026-04-29 13:42 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CForceManager.cpp | CForceManager::SetMember | 0x140030CF0 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::IsFull | 0x140030EE0 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::SetMazeID | 0x140030F60 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::RemoveMember | 0x140039B30 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::GetMazeID | 0x140039CD0 | verified | IDA |
| XControlServer | CForce.cpp | CForce::SetMember | 0x1400398D0 | verified | IDA |
| XControlServer | CForce.cpp | CForce::IsEmpty | 0x140039980 | verified | IDA |
| XControlServer | CForce.cpp | CForce::SetMazeID | 0x14003A420 | verified | IDA |
| XControlServer | CForce.cpp | CForce::CForce(int) | 0x14003A450 | verified | IDA |


---

### CPartyManager / CParty 函数验证追加 (2026-04-29 13:42 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CPartyManager.cpp | CPartyManager::SetMember | 0x1400399A0 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::SetMazeID | 0x140039C00 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::IsParty | 0x140030E80 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::CPartyManager | 0x140030CA0 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::~CPartyManager | 0x140030CD0 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::IsLoad | 0x140027530 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::FindSamePlace | 0x140030FE0 | verified | IDA |
| XControlServer | CParty.cpp | CParty::RemoveMember | 0x140030B00 | verified | IDA |
| XControlServer | CParty.cpp | CParty::IsFull | 0x140030B80 | verified | IDA |
| XControlServer | CParty.cpp | CParty::FindSamePlace | 0x140030BC0 | verified | IDA |
| XControlServer | CParty.cpp | CParty::GetMazeID | 0x14003A3F0 | verified | IDA |
| XControlServer | CParty.cpp | CParty::~CParty | 0x14003A3D0 | verified | IDA |


---

### Process 类 Parse 方法验证追加 (2026-04-29 14:34 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServerProcess::Parse | 0x14003CAC0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateServer | 0x14003D0A0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMaze | 0x14003D1F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResCreateMaze | 0x14003D360 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMap | 0x14003D5D0 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::Parse | 0x1400450E0 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::Parse | 0x140034640 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::Parse | 0x14004D5F0 | verified | IDA |
| XCore | XPacket.cpp | XPacket::GetSubCmd | 0x140032750 | verified | IDA |


---

### ServerProcess/UserProcess/WorldManager 验证追加 (2026-04-29 14:41 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqEnterMap | 0x14003D6E0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqDisconnectUserSync | 0x14003DCA0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncMaxMazeID | 0x14003DD10 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::SyncSelectCharacter | 0x140045310 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::SyncLogoutUser | 0x1400454B0 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::SyncUserKickout | 0x1400455D0 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMUserKick | 0x140034730 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMNotice | 0x1400347C0 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMShutDwon | 0x140034850 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeCommand | 0x14004D680 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeComplete | 0x14004DAF0 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeUpdate | 0x14004DC20 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::AddUser | 0x14000C220 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::RemoveUser | 0x14000C610 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::AddMap | 0x1400027A0 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::DeleteMap | 0x140002B70 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::ReqEnterMap | 0x140002CA0 | verified | IDA |
| XControlServer | CServer.cpp | CServer::OnUpdate | 0x140042550 | verified | IDA |
| XControlServer | CServer.cpp | CServer::RegisterProcess | 0x140041180 | verified | IDA |


---

### CChannelOfMap 验证追加 (2026-04-29 14:45 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldManager.cpp | CChannelOfMap::AddUserCount | 0x140001000 | verified | IDA |
| XControlServer | WorldManager.cpp | CChannelOfMap::AddSelectUser | 0x1400011C0 | verified | IDA |
| XControlServer | WorldManager.cpp | CChannelOfMap::ChangeChannelState | 0x140001270 | verified | IDA |
| XControlServer | WorldManager.cpp | CChannelOfMap::CalculateChannelState | 0x140001410 | verified | IDA |
| XControlServer | WorldManager.cpp | CChannelOfMap::IsOKToEnter | 0x140001600 | verified | IDA |


---

### Process 类完整实现 (2026-04-29 16:32 +08:00)

#### CGMToolProcess 函数 (main=0xF8)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CGMToolProcess::Parse | 0x140034640 | verified | IDA |
| XControlServer | ServerProcess.cpp | CGMToolProcess::ReqGMUserKick | 0x140034730 | verified | IDA |
| XControlServer | ServerProcess.cpp | CGMToolProcess::ReqGMNotice | 0x1400347C0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CGMToolProcess::ReqGMShutDwon | 0x140034850 | verified | IDA |
| XControlServer | ServerProcess.cpp | CGMToolProcess::ReqGMTimeEvent | 0x1400348B0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CGMToolProcess::ReqGMValueEvent | 0x140034910 | verified | IDA |
| XControlServer | ServerProcess.cpp | CGMToolProcess::ReqGMServerOption | 0x1400349A0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CGMToolProcess::ReqGMCashShopBanner | 0x140034A10 | verified | IDA |

#### CUserProcess 函数 (main=0xF3)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CUserProcess::Parse | 0x1400450E0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::SyncSelectCharacter | 0x140045310 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::SyncLogoutUser | 0x1400454B0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::SyncUpdateUserMap | 0x140045570 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::SyncUserKickout | 0x1400455D0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqUserChatNotice | 0x140045640 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqUserChangeServer | 0x140045A10 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::SyncUserMoneyLog | 0x140045A80 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqUserEnterPartyMaze | 0x140045AC0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqUserChatMegaPhone | 0x1400456B0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqUserEnterForceMaze | 0x140045B70 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqUserTradePasswordStateSync | 0x140045C30 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqUserTradePasswordState | 0x140045CB0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqNameChange | 0x140045D10 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqCheckSessionID | 0x140045D70 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqUserUpdateAuthType | 0x140045F40 | verified | IDA |
| XControlServer | ServerProcess.cpp | CUserProcess::ReqGameServerEnterUser | 0x140045E00 | verified | IDA |

#### CWorldModeProcess 函数 (main=0xFB)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CWorldModeProcess::Parse | 0x14004D5F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CWorldModeProcess::ReqWorldModeUpdate | 0x14004DC20 | verified | IDA |
| XControlServer | ServerProcess.cpp | CWorldModeProcess::ReqWorldModeCommand | 0x14004D680 | verified | IDA |
| XControlServer | ServerProcess.cpp | CWorldModeProcess::ReqWorldModeComplete | 0x14004DAF0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CWorldModeProcess::ReqWorldModeEnterList | 0x14004DC90 | verified | IDA |

#### XControlServer GM 方法

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::GM_UserKick | 0x140010FF0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_Notice | 0x1400110F0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_Shutdown | 0x140011200 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_TimeEvent | 0x1400112E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_ValueEvent | 0x140011380 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_ServerOption | 0x140011420 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_CashShopBanner | 0x140011550 | verified | IDA |

### CUserObject 函数 (已验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserObject.cpp | CUserObject::SetPartyInfo | 0x14000A1E0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetBeforeServerID | 0x14000A560 | verified | IDA |


---

### XControlServer 迷宫/匹配/缓存方法验证追加 (2026-04-30 13:44 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::EnterMemberInMaze | 0x140012200 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::EnterMemberInMazeForce | 0x140012330 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CreateMatchingMaze(Party) | 0x140012460 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CreateMatchingModeMaze | 0x140013030 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ResCreateModeMaze | 0x140013560 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ReqCheckEnterMaze | 0x1400137F0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ResCheckServerGroupID | 0x140013B20 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ResCreateMatchingMaze(Party) | 0x140013BE0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendUserTradePasswordStateSync | 0x1400142E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendUserTradePasswordState | 0x140014350 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CharacterNameChange | 0x140014440 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CheckSessionID | 0x140014590 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SetCachingLoad | 0x140014D40 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::UnSetCachingLoad | 0x140014E00 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendCachingLoad | 0x140014E80 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CheckUserCount | 0x1400150F0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::UpdateMaxServerUserCount | 0x140015110 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::UpdateRouletteEventInfo | 0x140015790 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ReqReEnterMap | 0x1400157C0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SyncEventMaze | 0x140015B20 | verified | IDA |

说明：
- XControlServer::EnterMemberInMaze: 查找迷宫服务器，验证 Party 成员，发送 0xF4/0x08 响应
- XControlServer::EnterMemberInMazeForce: 查找迷宫服务器，验证 Force 成员，发送 0xFA/0x08 响应
- XControlServer::CreateMatchingMaze(Party): 遍历 m_mapMazeServer 选择最低用户数服务器，发送 0xF2/0x41
- XControlServer::CreateMatchingModeMaze: 选择迷宫服务器，发送 0xF2/0x72
- XControlServer::ResCreateModeMaze: 查找 m_mapCreateModeMazeReq，创建 CMazeInfo，添加到服务器，发送 0xF2/0x4A
- XControlServer::ReqCheckEnterMaze: 检查迷宫状态，处理断线用户重入逻辑
- XControlServer::ResCheckServerGroupID: ServerGroupID 不匹配则踢出用户
- XControlServer::ResCreateMatchingMaze(Party): 创建迷宫，更新 Party 成员 MazeID，发送 0xF2/0x44
- XControlServer::CheckSessionID: 验证 AuthSessionID，返回结果码 (0=踢出, 1=验证通过, 2=服务器满)
- XControlServer::SetCachingLoad: 设置缓存加载标志，满足条件时发送 SendCachingLoad
- XControlServer::UnSetCachingLoad: 清除缓存加载标志，发送 SendCachingLoad
- XControlServer::SendCachingLoad: 遍历 GameServer 检查同步状态，发送 0xF2/0x70
- XControlServer::CheckUserCount: 检查服务器组状态是否为 4
- XControlServer::UpdateMaxServerUserCount: 更新最大用户数，发送 0xF2/0x71
- XControlServer::ReqReEnterMap: 处理事件迷宫重入，验证 Maze_Type=13/19
- XControlServer::SyncEventMaze: 将事件迷宫用户加入 m_mapEventMazeToEnter
| XControlServer | UserObject.cpp | CUserObject::GetServerID | 0x14000AAB0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetEnterWorldModeInfo | 0x14000A860 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetEnterWorldModeInfo | 0x140044C60 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::CanChangeWorld | 0x140044B60 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::ResetWorldState | 0x140044BB0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::LoadEnterWorldModeInfo | 0x14004E040 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetWorldState | 0x14000A500 | verified | IDA |

### CServer 函数 (已验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.h | CServer::FindMyRoom | 0x140042A30 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::DelMyRoom | 0x140042A00 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::CreateMyRoom | 0x1400427C0 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::GetServerState | 0x14000A090 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::GetServerInfo | 0x14000A4E0 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::GetServerID | 0x14000B2B0 | verified | IDA |

### CPartyManager / CForceManager 函数 (已验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | PartyManager.cpp | CPartyManager::SetMember | 0x1400399A0 | verified | IDA |
| XLoginServer | PartyManager.cpp | CPartyManager::IsParty | 0x140030E80 | verified | IDA |
| XLoginServer | PartyManager.cpp | CPartyManager::FindSamePlace | 0x140030FE0 | verified | IDA |
| XLoginServer | PartyManager.cpp | CPartyManager::SetMazeID | 0x140039C00 | verified | IDA |
| XLoginServer | PartyManager.cpp | CForceManager::SetMember | 0x140030CF0 | verified | IDA |
| XLoginServer | PartyManager.cpp | CForceManager::SetMazeID | 0x140030F60 | verified | IDA |
| XLoginServer | PartyManager.cpp | CForceManager::RemoveMember | 0x140039B30 | verified | IDA |
| XLoginServer | PartyManager.cpp | CForceManager::GetMazeID | 0x140039CD0 | verified | IDA |

### CServerProcess 函数 (已验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServerProcess::Parse | 0x14003CAC0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMaze | 0x14003D1F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterReq | 0x14003E460 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterRes | 0x14003E680 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomCreate | 0x14003ED40 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomDelete | 0x14003EE80 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResMyRoomDelete | 0x14003EF20 | verified | IDA |


### XControlServer::RemoveUser (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::RemoveUser | 0x14000C610 | verified | IDA |

### CGMToolProcess 函数 (本轮验证)

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

### CWorldModeProcess 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::Parse | 0x14004D5F0 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeUpdate | 0x14004DC20 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeCommand | 0x14004D680 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeComplete | 0x14004DAF0 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeEnterList | 0x14004DC90 | verified | IDA |

### CParty 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | Party.h | CParty::IsFull | inline | verified | IDA |
| XControlServer | Party.h | CParty::IsEmpty | inline | verified | IDA |
| XControlServer | Party.h | CParty::RemoveMember | inline | verified | IDA |
| XControlServer | Party.h | CParty::SetPartyID | inline | verified | IDA |

### XControlServer::AddUser (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::AddUser | 0x14000C220 | verified | IDA |

### CUserProcess 更多函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserProcess.cpp | CUserProcess::SyncSelectCharacter | 0x140045310 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::SyncLogoutUser | 0x1400454B0 | verified | IDA |

### CMazeInfo 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | MazeInfo.h | CMazeInfo::Init (ST_CREATE_MAZE) | 0x1400360D0 | verified | IDA |
| XControlServer | MazeInfo.h | CMazeInfo::Init (ST_CREATE_MODE_MAZE) | 0x140036450 | verified | IDA |
| XControlServer | MazeInfo.h | CMazeInfo::IsValidEnterMaze | 0x140036C10 | verified | IDA |
| XControlServer | MazeInfo.h | CMazeInfo::GetMazeState | 0x140039480 | verified | IDA |
| XControlServer | MazeInfo.h | CMazeInfo::IsEnterMember | 0x140036DA0 | verified | IDA |
| XControlServer | MazeInfo.h | CMazeInfo::GetUserCount | 0x140039490 | verified | IDA |

### CWorldManager 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldManager.cpp | CWorldManager::ReqEnterMap | 0x140002CA0 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::UpdateUserMap | 0x1400050E0 | verified | IDA |

### CChannelOfMap 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldManager.cpp | CChannelOfMap::AddUserCount | 0x140001000 | verified | IDA |
| XControlServer | WorldManager.cpp | CChannelOfMap::IsOKToEnter | 0x140001600 | verified | IDA |

### CServer MyRoom 方法 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.h | CServer::CreateMyRoom | 0x1400427C0 | verified | IDA |
| XControlServer | ServerProcess.h | CServer::FindMyRoom | 0x140042A30 | verified | IDA |

### CPartyManager 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CPartyManager.h | CPartyManager::CPartyManager | 0x140030CA0 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::IsParty | 0x140030E80 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::FindSamePlace | 0x140030FE0 | verified | IDA |

### CForceManager 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CForceManager.cpp | CForceManager::SetMember | 0x140030CF0 | verified | IDA |

### CForce 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CForce.h | CForce::CForce | 0x14003A450 | verified | IDA |

### CServerProcess 子命令 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMap | 0x14003D5E0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCheckPartyInMaze | 0x14003D8C0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncUserPartyInfo | 0x14003DB20 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResCreateMatchingMazeFromGame | 0x14003DFF0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterReq | 0x14003E460 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqPostSend | 0x14003EAD0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomCreate | 0x14003ED40 | verified | IDA |

### CRouletteEventMgr 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | RouletteEventMgr.cpp | CRouletteEventMgr::CRouletteEventMgr | 0x14003A4C0 | verified | IDA |
| XControlServer | RouletteEventMgr.cpp | CRouletteEventMgr::~CRouletteEventMgr | 0x14003A530 | verified | IDA |
| XControlServer | RouletteEventMgr.cpp | CRouletteEventMgr::LoadRouletteEvent | 0x14003A5A0 | verified | IDA |
| XControlServer | RouletteEventMgr.cpp | CRouletteEventMgr::CheckItemDayCount | 0x14003A9E0 | verified | IDA |

### PS_GM_ROULETTE_EVENT 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | PSServer.h | PS_GM_ROULETTE_EVENT::PS_GM_ROULETTE_EVENT | 0x14003A720 | verified | IDA |
| XControlServer | PSServer.h | PS_GM_ROULETTE_EVENT::Clear | 0x14003BDE0 | verified | IDA |

### 服务管理函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerMain.cpp | ServiceInit | 0x14003C140 | verified | IDA |
| XControlServer | ServerMain.cpp | InstallService | 0x14003C5D0 | verified | IDA |

### CServer 更多函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CServer.cpp | CServer::RegisterProcess | 0x140041180 | verified | IDA |
| XControlServer | CServer.cpp | CServer::RemoveMaze | 0x140041930 | verified | IDA |
| XControlServer | CServer.cpp | CServer::IsValidEnterForceMemberInMaze | 0x140041F30 | verified | IDA |
| XControlServer | CServer.cpp | CServer::SyncMaze | 0x140042340 | verified | IDA |

### CServerProcess 更多子命令 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMatchingMazeFromCommunity | 0x14003DD60 | verified | IDA |

### PS_PARTY_INFO 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| PSCommon | PSServer.h | PS_PARTY_INFO::PS_PARTY_INFO (copy) | 0x14003E280 | verified | IDA |

### CUserProcess 更多函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserProcess.cpp | CUserProcess::SyncSelectCharacter | 0x140045310 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqUserChatNotice | 0x140045640 | verified | IDA |

### CUserProcess 更多子命令 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserProcess.cpp | CUserProcess::ReqUserEnterForceMaze | 0x140045B70 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqUserUpdateAuthType | 0x140045F40 | verified | IDA |

### CServerProcess MyRoom 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterReq | 0x14003E460 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterRes | 0x14003E680 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomCreate | 0x14003ED40 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomDelete | 0x14003EE80 | verified | IDA |

### CWorldModeProcess 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeCommand | 0x14004D680 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeComplete | 0x14004DAF0 | verified | IDA |

### CServer Core Getters (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CServer.cpp | CServer::GetServerState | 0x14000A090 | verified | IDA |
| XControlServer | CServer.cpp | CServer::GetServerInfo | 0x14000A4E0 | verified | IDA |
| XControlServer | CServer.cpp | CServer::GetServerID | 0x14000B2B0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GetServer | 0x14000FB10 | verified | IDA |

### XControlServer Matching Functions (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::ResCreateMatchingMaze (Party) | 0x140013BE0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ResCreateMatchingMaze (Force) | 0x140013F70 | verified | IDA |

### CUserProcess 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserProcess.cpp | CUserProcess::ReqGameServerEnterUser | 0x140045E00 | verified | IDA |

### XControlServer Startup 函数 (本轮修复)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::SetName | 0x14000BCD0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendDBAccount | 0x140011880 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendDBGame | 0x140011910 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendDBLog | 0x1400119A0 | verified | IDA |

### CWorldModeMgr 核心函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::CWorldModeMgr | 0x1400497C0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::~CWorldModeMgr | 0x140049820 | verified | IDA |
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

### CMyRoom 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CMyRoom.cpp | CMyRoom::CMyRoom | 0x1400395F0 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::Init | 0x140039630 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::RoomEnterUser | 0x140039690 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CreateMyRoom | 0x140039820 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::EnterSucc | 0x1400398B0 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::DeleteReq | 0x140027140 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::GetMyRoomState | 0x140027150 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::GetUxMapID | 0x140027160 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::GetOwnerInfo | 0x140027190 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CreateSucc | 0x1400271E0 | verified | IDA |

### CServer MyRoom 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServer::CreateMyRoom | 0x1400427C0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServer::FindMyRoom | 0x140042A30 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServer::DelMyRoom | 0x140042A00 | verified | IDA |

### XControlServer MyRoom 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::MyRoomEnterReq | 0x140012050 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::FindMyRoom | 0x140011EE0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ReqCreateMyRoom | 0x1400147E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::DeleteMyRoomReq | 0x140014B10 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::DeleteMyRoomRes | 0x140014C30 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GetMyRoomServer | 0x140011E00 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::AddMyRoomServerInfo | 0x140011CB0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::RemoveMyRoomServerInfo | 0x140011D50 | verified | IDA |

### CParty 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CParty.cpp | CParty::CParty | - | verified | IDA struct |
| XControlServer | CParty.cpp | CParty::RemoveMember | 0x140030B00 | verified | IDA |
| XControlServer | CParty.cpp | CParty::IsFull | 0x140030B80 | verified | IDA |
| XControlServer | CParty.cpp | CParty::FindSamePlace | 0x140030BC0 | verified | IDA |
| XControlServer | CParty.cpp | CParty::GetMazeID | 0x14003A3F0 | verified | IDA |
| XControlServer | CParty.cpp | CParty::~CParty | 0x14003A3D0 | verified | IDA |

### CForce 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CForce.cpp | CForce::CForce | 0x14003A450 | verified | IDA |
| XControlServer | CForce.cpp | CForce::SetMember | 0x1400398D0 | verified | IDA |
| XControlServer | CForce.cpp | CForce::SetMazeID | 0x14003A420 | verified | IDA |
| XControlServer | CForce.cpp | CForce::IsEmpty | 0x140039980 | verified | IDA |

### CPartyManager 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CPartyManager.cpp | CPartyManager::CPartyManager | 0x140030CA0 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::~CPartyManager | 0x140030CD0 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::IsLoad | 0x140027530 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::SetMember | 0x1400399A0 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::SetMazeID | 0x140039C00 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::IsParty | 0x140030E80 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::FindSamePlace | 0x140030FE0 | verified | IDA |

### CForceManager 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CForceManager.cpp | CForceManager::SetMember | 0x140030CF0 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::SetMazeID | 0x140030F60 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::IsFull | 0x140030EE0 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::GetMazeID | 0x140039CD0 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::RemoveMember | 0x140039B30 | verified | IDA |

### CServerProcess Party/Force Sync 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServerProcess::PartyMazeSync | 0x14003E9F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ForceMazeSync | 0x14003EA60 | verified | IDA |

### CGMToolProcess 函数 (本轮验证)

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

### CUserProcess 函数 (本轮验证)

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
| XControlServer | UserProcess.cpp | CUserProcess::SyncUserPartyInfo | 0x14003D9E0 | verified | IDA |

### XControlServer GM 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::GM_UserKick | 0x140010FF0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_Shutdown | 0x140011200 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_Notice | 0x1400110F0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_TimeEvent | 0x1400112E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_ValueEvent | 0x140011380 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_ServerOption | 0x140011420 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_CashShopBanner | 0x140011550 | verified | IDA |

### CWorldManager 函数 (本轮补充验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldManager.cpp | CWorldManager::ReqEnterMapToOther | 0x140003D30 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::ReqChangeChannel | 0x140004720 | verified | IDA |

### CMazeInfo 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetMemberVector | 0x140006690 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::CMazeInfo | 0x140035F10 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::ResetParentMaze | 0x140036B80 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::UpdateMazeInfo | 0x140036560 | verified | IDA |

### CServerProcess 其他函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterRes | 0x14003E680 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqPostSend | 0x14003EAD0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResCreateModeMaze | 0x14003EFC0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResCreateMatchingMazeFromGame | 0x14003DFF0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMatchingMazeFromCommunity | 0x14003DD60 | verified | IDA |

### CDayEventMgr 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | DayEventMgr.cpp | CDayEventMgr::SetDayEvent | 0x14002DAF0 | verified | IDA |

---

### [2026-04-30 04:02 +08:00] [glm-5] - CGameDBSocket / CRouletteEventMgr 验证追加

### CGameDBSocket 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore/XServer | XGameDBSocketMgr.h | CGameDBSocket::CGameDBSocket | 0x1400313F0 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | CGameDBSocket::~CGameDBSocket | 0x140031440 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | CGameDBSocket::FindUser | 0x140031470 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | CGameDBSocket::SetInfomation | 0x1400314A0 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | CGameDBSocket::OnParse | 0x140031530 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | CGameDBSocket::DBParse | 0x140031580 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | CGameDBSocket::DBLoginParse | 0x140031620 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | CGameDBSocket::DBEventParse | 0x1400316B0 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | CGameDBSocket::ResAddServerGroupInfo | 0x140031720 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | CGameDBSocket::ResServerGroupMaxUserCount | 0x1400317A0 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | CGameDBSocket::ResCheckServerGroupID | 0x140031830 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | CGameDBSocket::OnDisConnect | 0x1400318C0 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | CGameDBSocket::OnNotConnect | 0x140031990 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | CGameDBSocket::ResWorldModeLoad | 0x140031A30 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | CGameDBSocket::ResRouletteEventRewardLoad | 0x140031AA0 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | CGameDBSocket::WriteLog | 0x140031B90 | verified | IDA |

### XGameDBSocketMgr 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore/XServer | XGameDBSocketMgr.h | XGameDBSocketMgr::XGameDBSocketMgr | - | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | XGameDBSocketMgr::~XGameDBSocketMgr | - | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | XGameDBSocketMgr::Init | - | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | XGameDBSocketMgr::AutoConnect | - | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | XGameDBSocketMgr::DisConnect | - | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | XGameDBSocketMgr::GetAccountDBAgentCount | 0x140027590 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | XGameDBSocketMgr::GetGameDBAgentCount | 0x140027580 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | XGameDBSocketMgr::GetLogDBAgentCount | 0x140027570 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | XGameDBSocketMgr::SendAccountDBAgent | 0x140034290 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | XGameDBSocketMgr::SendGameDBAgent | 0x140034310 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.h | XGameDBSocketMgr::SendLogDBAgent | 0x140034390 | verified | IDA |

### CRouletteEventMgr 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | RouletteEventMgr.cpp | CRouletteEventMgr::CRouletteEventMgr | 0x14003A4C0 | verified | IDA |
| XControlServer | RouletteEventMgr.cpp | CRouletteEventMgr::~CRouletteEventMgr | 0x14003A530 | verified | IDA |
| XControlServer | RouletteEventMgr.cpp | CRouletteEventMgr::Clear | 0x14003A560 | verified | IDA |
| XControlServer | RouletteEventMgr.cpp | CRouletteEventMgr::LoadRouletteEvent | 0x14003A5A0 | verified | IDA |
| XControlServer | RouletteEventMgr.cpp | CRouletteEventMgr::CheckItemDayCount | 0x14003A9E0 | verified | IDA |
| XControlServer | RouletteEventMgr.cpp | CRouletteEventMgr::Update | 0x14003AC10 | verified | IDA |
| XControlServer | RouletteEventMgr.cpp | CRouletteEventMgr::SendDBRouletteRewardInfo | 0x14003ACA0 | verified | IDA |
| XControlServer | RouletteEventMgr.cpp | CRouletteEventMgr::SendRouletteEvent | 0x14003AD30 | verified | IDA |
| XControlServer | RouletteEventMgr.cpp | CRouletteEventMgr::UpdateRouletteItemInfo | 0x14003ADE0 | verified | IDA |
| XControlServer | RouletteEventMgr.cpp | CRouletteEventMgr::SetRouletteRewardInfo | 0x14003AE90 | verified | IDA |

### CUserObject Getter/Setter 函数 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserObject.cpp | CUserObject::GetUAID | 0x140029260 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetCID | 0x140029220 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetName | 0x140026A40 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetAuthType | 0x1400291A0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetTradePWState | 0x140027270 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetBlockType | 0x140046640 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetBeforeServerID | 0x1400290A0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetMapIns | 0x1400291F0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetServer | 0x1400290C0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetAuthSessionID | 0x140029320 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetIP | 0x140029360 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetTradePWState | 0x140029340 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetMapIns | 0x140029130 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetBlockType | 0x140029300 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetAuthType | 0x1400270C0 | verified | IDA |

### CWorldModeMgr 核心函数 (本轮完整验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::Init | 0x140049840 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::LoadMode | 0x1400498A0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::GetActiveMode | 0x14004BB40 | verified | IDA |

### CWorldMode 核心函数追加 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldMode.cpp | CWorldMode::CWorldMode | 0x140046BD0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::~CWorldMode | 0x140046CB0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::Init | 0x140046CD0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::Update | 0x140047990 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::StartMode | 0x140047AF0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::FinishMode | 0x140047E10 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::Clear | 0x140047920 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::ClearMode | 0x140048350 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::SetNextEventTime | 0x1400486E0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::SendLogNextModeTime | 0x1400489D0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::AddMonsterCount | 0x140048BD0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::GetNextModeID | 0x140048590 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::GetModeInfo | 0x1400485A0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::ReqModeComplete | 0x1400485F0 | verified | IDA |

### CWorldModeMgr 函数追加 (本轮验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::Update | 0x14004A050 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::SendModeSync | 0x14004A630 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::IsTodayModeList | 0x14004AFC0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::InitMode | 0x14004B140 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::UpdateMonsterCount | 0x14004B730 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::IsEnterMode | 0x14004B8A0 | verified | IDA |

### CMyRoom 核心函数 (2026-04-30 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CMyRoom.cpp | CMyRoom::CMyRoom | 0x1400395F0 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::~CMyRoom | - | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::Init | 0x140039630 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CreateMyRoom | 0x140039820 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::RoomEnterUser | 0x140039690 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::DeleteReq | 0x140027140 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CreateSucc | 0x1400271E0 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::EnterSucc | 0x1400398B0 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::GetMyRoomState | 0x140027150 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::GetOwnerInfo | 0x140027190 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::GetUxMapID | 0x140027160 | verified | IDA |

### CServer MyRoom 函数 (2026-04-30 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CServer.cpp | CServer::CreateMyRoom | 0x1400427C0 | verified | IDA |
| XControlServer | CServer.cpp | CServer::FindMyRoom | 0x140042A30 | verified | IDA |
| XControlServer | CServer.cpp | CServer::DelMyRoom | 0x140042A00 | verified | IDA |

### CServerProcess MyRoom 处理器 (2026-04-30 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServerProcess::Parse | 0x14003CAC0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterReq | 0x14003E460 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterRes | 0x14003E680 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomCreate | 0x14003ED40 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomDelete | 0x14003EE80 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResMyRoomDelete | 0x14003EF20 | verified | IDA |

### XControlServer MyRoom 处理器 (2026-04-30 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::MyRoomEnterReq | 0x140012050 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ReqCreateMyRoom | 0x1400147E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::DeleteMyRoomReq | 0x140014B10 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::DeleteMyRoomRes | 0x140014C30 | verified | IDA |

### Process 类 Parse 方法 (2026-04-30 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServerProcess::Parse | 0x14003CAC0 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::Parse | 0x1400450E0 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::Parse | 0x140034640 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::Parse | 0x14004D5F0 | verified | IDA |


### CUserProcess 核心函数 (2026-04-30 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserProcess.cpp | CUserProcess::Parse | 0x1400450E0 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::SyncSelectCharacter | 0x140045310 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::SyncLogoutUser | 0x1400454B0 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::SyncUpdateUserMap | 0x140045570 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::SyncUserKickout | 0x1400455D0 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqCheckSessionID | 0x140045D70 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::ReqGameServerEnterUser | 0x140045E00 | verified | IDA |

### CGMToolProcess 核心函数 (2026-04-30 验证)

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

### CWorldModeProcess 核心函数 (2026-04-30 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::Parse | 0x14004D5F0 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeCommand | 0x14004D680 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeComplete | 0x14004DAF0 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeUpdate | 0x14004DC20 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeEnterList | 0x14004DC90 | verified | IDA |


### XControlServer 用户管理函数 (2026-04-30 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::AddUser | 0x14000C220 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::RemoveUser | 0x14000C610 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::UpdateUserMap | 0x14000CCA0 | verified | IDA |



### CWorldManager 核心函数 (2026-04-30 验证)

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

### CServer 核心函数 (2026-04-30 08:10 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CServer.cpp | CServer::OnUpdate | 0x140042550 | verified | IDA |
| XControlServer | CServer.cpp | CServer::RemoveMaze | 0x140041930 | verified | IDA |
| XControlServer | CServer.cpp | CServer::SyncMaze | 0x140042340 | verified | IDA |
| XControlServer | CServer.cpp | CServer::RegisterProcess | 0x140041180 | verified | IDA |
| XControlServer | CServer.cpp | CServer::AddMaze | 0x140041410 | verified | IDA |
| XControlServer | CServer.cpp | CServer::GetMazeInfo | 0x140041870 | verified | IDA |
| XControlServer | CServer.cpp | CServer::GetServerState | 0x14000A090 | verified | IDA |
| XControlServer | CServer.cpp | CServer::GetServerInfo | 0x14000A4E0 | verified | IDA |
| XControlServer | CServer.cpp | CServer::GetServerID | 0x14000B2B0 | verified | IDA |

### CMazeInfo 核心函数 (2026-04-30 08:10 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | MazeInfo.cpp | CMazeInfo::CMazeInfo | 0x140035F10 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::~CMazeInfo | 0x140036070 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::Init(ST_CREATE_MAZE) | 0x1400360D0 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::Init(PS_CREATE_MAP) | 0x140036410 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::Init(ST_CREATE_MODE_MAZE) | 0x140036450 | verified | IDA |
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
| XControlServer | MazeInfo.cpp | CMazeInfo::GetMazeInfo | 0x140027AF0 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetParentMaze | 0x140027B50 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::SetMazeState | 0x140029070 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetMazeState | 0x140039480 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetUserCount | 0x140039490 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetChildMaze | 0x1400394B0 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetMazeType | 0x140044890 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetMapID | 0x1400448B0 | verified | IDA |

### CParty/CForce 函数 (2026-04-30 08:10 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | PartyManager.cpp | CParty::IsFull | 0x140030B80 | verified | IDA |
| XLoginServer | PartyManager.cpp | CPartyManager::SetMember | 0x1400399A0 | verified | IDA |
| XLoginServer | PartyManager.cpp | CPartyManager::RemoveMember | 0x140039B30 | verified | IDA |
| XLoginServer | PartyManager.cpp | CPartyManager::SetMazeID | 0x140030F60 | verified | IDA |
| XLoginServer | PartyManager.cpp | CPartyManager::GetMazeID | 0x140039CD0 | verified | IDA |
| XLoginServer | PartyManager.cpp | CForceManager::SetMember | 0x140030CF0 | verified | IDA |
| XLoginServer | PartyManager.cpp | CForceManager::IsFull | 0x140030EE0 | verified | IDA |
| XLoginServer | PartyManager.cpp | CForce::CForce | 0x14003A450 | verified | IDA |
| XLoginServer | PartyManager.cpp | CForce::SetMember | 0x1400398D0 | verified | IDA |
| XLoginServer | PartyManager.cpp | CForce::IsEmpty | 0x140039980 | verified | IDA |
| XLoginServer | PartyManager.cpp | CForce::SetMazeID | 0x14003A420 | verified | IDA |

### CServerProcess 核心函数 (2026-04-30 08:30 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServerProcess::Parse | 0x140009870 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMaze | 0x14003D1F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqGoBackMaze | 0x14003D2A0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResCreateMaze | 0x14003D360 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncMaze | 0x14003D4F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMap | 0x14003D5D0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqEnterMap | 0x14003D6E0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::EnterOtherMap_cheat | 0x14003D750 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCheckPartyInMaze | 0x14003D8C0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncUserPartyInfo | 0x14003D9E0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncLogicThreadCount | 0x14003DBF0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqDisconnectUserSync | 0x14003DCA0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMatchingMazeFromCommunity | 0x14003DD60 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResCreateMatchingMazeFromGame | 0x14003DFF0 | verified | IDA |

### CUserProcess 核心函数 (2026-04-30 08:35 验证)

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

### CMyRoom 核心函数 (2026-04-30 08:28 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CMyRoom.cpp | CMyRoom::CMyRoom | 0x1400395F0 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CreateMyRoom | 0x140039820 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::RoomEnterUser | 0x140039690 | verified | IDA |

### CServer 迷宫管理函数 (2026-04-30 08:35 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CServer.cpp | CServer::CreateMyRoom | 0x1400427C0 | verified | IDA |
| XControlServer | CServer.cpp | CServer::FindMyRoom | 0x140042A30 | verified | IDA |
| XControlServer | CServer.cpp | CServer::DelMyRoom | 0x140042A00 | verified | IDA |
| XControlServer | CServer.cpp | CServer::UpdateMaze | 0x1400421E0 | verified | IDA |
| XControlServer | CServer.cpp | CServer::IsValidEnterForceMemberInMaze | 0x140041F30 | verified | IDA |
| XControlServer | CServer.cpp | CServer::UpdateServerInfo | 0x140041660 | verified | IDA |
| XControlServer | CServer.cpp | CServer::IsValidEnterPartyMemberInMaze | 0x140041C80 | verified | IDA |

### XControlServer MyRoom/迷宫函数 (2026-04-30 08:28 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::MyRoomEnterReq | 0x140012050 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::EnterMemberInMaze | 0x140012200 | verified | IDA |

### CWorldMode 核心函数 (2026-04-30 09:25 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldMode.cpp | CWorldMode::InitMode | 0x140048D60 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::ClearMode | 0x140048350 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::FinishMode | 0x140047E10 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::SetNextEventTime | 0x1400486E0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::AddMonsterCount | 0x140048BD0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::SetStartTime | 0x140048D00 | verified | IDA |

### CMazeInfo 补充函数 (2026-04-30 09:25 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetMazeType | 0x140044890 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetMapID | 0x1400448B0 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::IsValidEnterMaze | 0x140036C10 | verified | IDA |

### CWorldModeMgr 补充函数 (2026-04-30 09:25 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::Update | 0x14004A050 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::SetModeStartTime | 0x14004A9A0 | verified | IDA |

### CWorldMode 补充函数 (2026-04-30 09:40 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldMode.cpp | CWorldMode::SendLogNextModeTime | 0x1400489D0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::Clear | 0x140047920 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::StartMode | 0x140047AF0 | verified | IDA |

### CMazeInfo 补充函数 (2026-04-30 09:40 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | MazeInfo.cpp | CMazeInfo::ResetChildMaze | 0x140036B60 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::ResetParentMaze | 0x140036B80 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::SyncMazeInfo | 0x140036820 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::IsDestroy | 0x140036D70 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::IsEnterMember | 0x140036DA0 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::SetDisconnectUserState | 0x140036E20 | verified | IDA |

### CUserObject 补充函数 (2026-04-30 09:40 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserObject.cpp | CUserObject::CanChangeWorld | 0x140044B60 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::ResetWorldState | 0x140044BB0 | verified | IDA |

### CRouletteEventMgr 补充函数 (2026-04-30 12:22 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::CRouletteEventMgr | 0x14003A4C0 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::Clear | 0x14003A560 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::Update | 0x14003AC10 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::CheckItemDayCount | 0x14003A9E0 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::SendRouletteEvent | 0x14003AD30 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::SendDBRouletteRewardInfo | 0x14003ACA0 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::UpdateRouletteItemInfo | 0x14003ADE0 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::SetRouletteRewardInfo | 0x14003AEB0 | verified | IDA |

### CMazeInfo 深度验证函数 (2026-04-30 12:22 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | MazeInfo.cpp | CMazeInfo::SyncMazeInfo | 0x140036820 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::IsValidEnterMaze | 0x140036C10 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::IsEnterMember | 0x140036DA0 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::SetDisconnectUserState | 0x140036E20 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::CheckDisconnecUsertState | 0x140036EC0 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::ResetParentMaze | 0x140036B80 | verified | IDA |

### CServer 深度验证函数 (2026-04-30 12:22 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CServer.cpp | CServer::OnUpdate | 0x140042550 | verified | IDA |
| XControlServer | CServer.cpp | CServer::RemoveMaze | 0x140041930 | verified | IDA |
| XControlServer | CServer.cpp | CServer::SyncMaze | 0x140042340 | verified | IDA |
| XControlServer | CServer.cpp | CServer::RegisterProcess | 0x140041180 | verified | IDA |
| XControlServer | CServer.cpp | CServer::CreateMyRoom | 0x1400427C0 | verified | IDA |
| XControlServer | CServer.cpp | CServer::FindMyRoom | 0x140042A30 | verified | IDA |
| XControlServer | CServer.cpp | CServer::DelMyRoom | 0x140042A00 | verified | IDA |

### CUserObject 深度验证函数 (2026-04-30 12:22 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserObject.cpp | CUserObject::CanChangeWorld | 0x140044B60 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::ResetWorldState | 0x140044BB0 | verified | IDA |

### 入口点函数 (2026-04-30 12:22 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerMain.cpp | main | 0x14003BED0 | verified | IDA |
| XControlServer | ServerMain.cpp | ServiceMain | 0x14003BF40 | verified | IDA |
| XControlServer | ServerMain.cpp | SERVICE_HANDLER | 0x14003BFF0 | verified | IDA |

### CChannelOfMap 验证函数 (2026-04-30 12:22 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::GetServerState | 0x14000A0B0 | verified | IDA |


### XGameDBSocketMgr 函数 (2026-04-30 12:30 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::Init | 0x140032E50 | verified | IDA |
| XControlServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::AutoConnect | 0x140033C30 | verified | IDA |
| XControlServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::DisConnect | 0x140033EA0 | verified | IDA |

### CGameDBSocket 函数 (2026-04-30 12:30 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::CGameDBSocket | 0x1400313F0 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::OnParse | 0x140031530 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::DBLoginParse | 0x140031620 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::ResServerGroupMaxUserCount | 0x1400317A0 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::OnDisConnect | 0x1400318C0 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::ResWorldModeLoad | 0x140031A30 | verified | IDA |

### TXObjectMgr 函数 (2026-04-30 12:30 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | XObjectMgr.h | TXObjectMgr<CServer>::Find | 0x140031FB0 | verified | IDA |


---

### CMyRoom 补充函数 (2026-04-30 12:35 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CMyRoom.cpp | CMyRoom::DeleteReq | 0x140027140 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::GetMyRoomState | 0x140027150 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::GetUxMapID | 0x140027160 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::GetOwnerInfo | 0x140027190 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CreateSucc | 0x1400271E0 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::Init | 0x140039630 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::EnterSucc | 0x1400398B0 | verified | IDA |

说明：
- CMyRoom::DeleteReq: m_byState = 2 (DELETE状态)
- CMyRoom::GetMyRoomState: return m_byState
- CMyRoom::GetUxMapID: return m_uxMapID (结构体复制)
- CMyRoom::GetOwnerInfo: qmemcpy 68 bytes from this to result
- CMyRoom::CreateSucc: m_byState = 1 (READY状态)
- CMyRoom::Init: memset 0x44, m_pServer=nullptr, m_uxMapID=0, m_byState=0
- CMyRoom::EnterSucc: if state==2, state=1 (允许从DELETE恢复到READY)

---

### CWorldMode 补充函数 (2026-04-30 12:35 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldMode.cpp | CWorldMode::Init | 0x140046CD0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::Clear | 0x140047920 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::Update | 0x140047990 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::IsFinish | 0x140047AC0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::StartMode | 0x140047AF0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::FinishMode | 0x140047E10 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::ClearMode | 0x140048350 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::GetNextModeID | 0x140048590 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::ReqModeComplete | 0x1400485F0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::SetNextEventTime | 0x1400486E0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::SendLogNextModeTime | 0x1400489D0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::InitMode | 0x140048D60 | verified | IDA |

说明：
- CWorldMode::Init: 复杂初始化，从 TB_MODE_DISTRICT6 和 TB_MODE_DISTRICT6_DATE 读取配置
- CWorldMode::Clear: nState=0, bSuccess=0, 清空字段
- CWorldMode::Update: 状态机：0->StartMode, 1->FinishMode, 2->ClearMode
- CWorldMode::IsFinish: return nState == 2
- CWorldMode::StartMode: state=1, send 0xFB/1, log "START_D6_MODE"
- CWorldMode::FinishMode: state=2, send 0xFB/4, log "FINISH_D6_MODE"
- CWorldMode::ClearMode: state=0, send 0xFB/3
- CWorldMode::GetNextModeID: return m_nNextModeID
- CWorldMode::ReqModeComplete: check state==1, set m_bReserveFinish, m_bSuccess
- CWorldMode::SetNextEventTime: 计算下次事件时间
- CWorldMode::SendLogNextModeTime: 发送日志 ST_LOG_GAME main=25 sub=6
- CWorldMode::InitMode: 用时间参数初始化模式

---

### CWorldModeMgr 补充函数 (2026-04-30 12:35 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::CWorldModeMgr | 0x1400497C0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::LoadMode | 0x1400498A0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::Update | 0x14004A050 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::SendModeSync | 0x14004A630 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::ReqModeComplete | 0x14004ABC0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::GetModeList | 0x14004AE00 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::InitMode | 0x14004B140 | verified | IDA |

说明：
- CWorldModeMgr::CWorldModeMgr: init CFSRWLock, m_bLoadReq=0, m_bLoadDB=0, map, CTime
- CWorldModeMgr::LoadMode: 从数据库加载模式，创建 CWorldMode 对象
- CWorldModeMgr::Update: 333ms tick 间隔，检查是否需要加载/更新模式
- CWorldModeMgr::SendModeSync: 发送 0xFB/5 包
- CWorldModeMgr::ReqModeComplete: 转发到 CWorldMode::ReqModeComplete
- CWorldModeMgr::GetModeList: 获取模式列表
- CWorldModeMgr::InitMode: 用时间参数初始化所有模式


---

### CServerProcess 补充函数 (2026-04-30 12:40 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServerProcess::Parse | 0x14003CAC0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterReq | 0x14003E460 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterRes | 0x14003E680 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomCreate | 0x14003ED40 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomDelete | 0x14003EE80 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqChangeChannel | 0x14003D130 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncMaze | 0x14003D4F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqEnterMap | 0x14003D6E0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncMaxMazeID | 0x14003DD10 | verified | IDA |

说明：
- CServerProcess::Parse: 33 个子命令 (main=0xF2)
  - 1:reqCreateServer, 3:reqUpdateServerInfo, 0x10:reqChangeChannel
  - 0x21:reqCreateMaze, 0x22:resCreateMaze, 0x23:syncMaze
  - 0x25:syncRemoveMaze, 0x26:syncUpdateMaze, 0x27:reqGoBackMaze
  - 0x30:reqCreateMap, 0x31:reqEnterMap, 0x32:reqCheckPartyInMaze
  - 0x33:syncUsersInfo, 0x36:syncUserPartyInfo, 0x37:syncLogicThreadCount
  - 0x38:reqCheckEnterMaze, 0x39:reqDisconnectUserSync, 0x40:syncMaxMazeID
  - 0x41/0x42:resCreateMatchingMazeFromGame, 0x43:reqCreateMatchingMazeFromCommunity
  - 0x49:reqCreateMatchingModeMazeFromCommunity
  - 0x50:reqMyRoomEnterReq, 0x51:reqMyRoomEnterRes, 0x52:reqMyRoomCreate
  - 0x53:reqMyRoomDelete, 0x54:enterOtherMap_cheat, 0x55:partyMazeSync
  - 0x56:reqPostSend, 0x58:reqFindUser, 0x59:resMyRoomDelete
  - 0x63:forceMazeSync, 0x73:resCreateModeMaze, 0x75:reqUpdateRouletteEvent
  - 0x77:reqReEnterMap
- CServerProcess::ReqMyRoomEnterReq: 解析 ST_MYROOM_USER + ST_MYROOM_OWNER_INFO
- CServerProcess::ReqMyRoomEnterRes: 成功时调用 CMyRoom::EnterSucc
- CServerProcess::ReqMyRoomCreate: 调用 XControlServer::ReqCreateMyRoom
- CServerProcess::ReqMyRoomDelete: 调用 XControlServer::DeleteMyRoomReq

---

### CMazeInfo 补充函数 (2026-04-30 12:40 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | MazeInfo.cpp | CMazeInfo::Init (ST_CREATE_MAZE) | 0x1400360D0 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::ResetParentMaze | 0x140036B80 | verified | IDA |

说明：
- CMazeInfo::Init: qmemcpy 600 bytes, m_stPartyInfo, m_vecEnterMember, m_nUserCount=0
- CMazeInfo::ResetParentMaze: 检查 m_pParentMaze, GetTickCount64+60000, SetMazeState(3)


---

### CWorldModeProcess 函数 (2026-04-30 12:45 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::Parse | 0x14004D5F0 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeCommand | 0x14004D680 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeComplete | 0x14004DAF0 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeUpdate | 0x14004DC20 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeEnterList | 0x14004DC90 | verified | IDA |

说明：
- CWorldModeProcess::Parse: main=0xFB, 4 个子命令
  - 2: ReqWorldModeUpdate
  - 6: ReqWorldModeCommand
  - 7: ReqWorldModeComplete
  - 9: ReqWorldModeEnterList
- CWorldModeProcess::ReqWorldModeCommand:
  - state 3: 发送模式列表 (0xFB/6)
  - state 1: 开始模式 (SetModeStartTime + SetModeFinishTime)
  - state 2: 完成模式 (ReqModeComplete, bFinish=1)
- CWorldModeProcess::ReqWorldModeComplete: 调用 CWorldModeMgr::ReqModeComplete, 发送 0xFB/7
- CWorldModeProcess::ReqWorldModeUpdate: 调用 CWorldModeMgr::UpdateMonsterCount
- CWorldModeProcess::ReqWorldModeEnterList: 加载用户进入世界模式信息

---

### CWorldModeMgr 补充函数 (2026-04-30 12:45 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::GetActiveMode | 0x14004BB40 | verified | IDA |

说明：
- CWorldModeMgr::GetActiveMode: 获取当前活跃模式的 nModeDateID 和 nModeID
- 查找 state==1 的模式并返回其 ID


---

### CUserProcess 函数 (2026-04-30 12:50 验证)

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

说明：
- CUserProcess::Parse: main=0xF3, 16 个子命令
  - 1:SyncSelectCharacter, 3:SyncLogoutUser, 4:SyncUpdateUserMap
  - 7:SyncUserKickout, 0x11:ReqUserChatNotice, 0x12:ReqUserChangeServer
  - 0x13:SyncUserMoneyLog, 0x16:ReqUserEnterPartyMaze, 0x17:ReqUserChatMegaPhone
  - 0x20:ReqUserEnterForceMaze, 0x26:ReqUserTradePasswordStateSync
  - 0x27:ReqUserTradePasswordState, 0x31:ReqNameChange, 0x32:ReqCheckSessionID
  - 0x35:ReqUserUpdateAuthType, 0x60:ReqGameServerEnterUser
- CUserProcess::SyncSelectCharacter: 调用 XControlServer::AddUser
- CUserProcess::SyncLogoutUser: 调用 XControlServer::RemoveUser
- CUserProcess::ReqGameServerEnterUser: 发送 0xF3/0x60 响应包


---

### CGMToolProcess 函数 (2026-04-30 12:55 验证)

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

说明：
- CGMToolProcess::Parse: main=0xF8, 7 个子命令
  - 1:ReqGMUserKick, 2:ReqGMNotice, 3:ReqGMShutDwon
  - 4:ReqGMTimeEvent, 5:ReqGMValueEvent, 6:ReqGMServerOption
  - 7:ReqGMCashShopBanner
- CGMToolProcess::ReqGMUserKick: 调用 XControlServer::GM_UserKick
- CGMToolProcess::ReqGMNotice: 调用 XControlServer::GM_Notice
- CGMToolProcess::ReqGMShutDwon: 调用 XControlServer::GM_Shutdown
- CGMToolProcess::ReqGMTimeEvent: 调用 XControlServer::GM_TimeEvent



---

### CRouletteEventMgr 方法验证追加 (2026-04-30 13:19)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::SendRouletteEvent | 0x14003AD30 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::UpdateRouletteItemInfo | 0x14003ADE0 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::SetRouletteRewardInfo | 0x14003AEB0 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::SendDBRouletteRewardInfo | 0x14003ACA0 | verified | IDA |

说明：
- SendRouletteEvent: main=0xF2, sub=0x74, operator<<(m_psRouletteEventInfo), SendPacketAll
- UpdateRouletteItemInfo: CFAutoSlimWriteLock, 遍历 psRewardList.vecInfo, nRewradIndex==nRewardID 时更新 nCurCount
- SetRouletteRewardInfo: 双层循环匹配 EventID+RewardID, m_bLoad=1, SendRouletteEvent(1)
- SendDBRouletteRewardInfo: main=0x49, sub=0x2D, operator<<(nEventID), SendDBGame


---

### CMazeInfo 方法验证追加 (2026-04-30 13:19)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | MazeInfo.cpp | CMazeInfo::Init | 0x1400360D0 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::SyncMazeInfo | 0x140036820 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::CheckDisconnecUsertState | 0x140036EC0 | verified | IDA |

说明：
- CMazeInfo::Init: ST_MAP_INFO::operator=, vector<ST_ENTER_MAZE_MEMBER_INFO>::assign, 创建 m_mapWaitEnterMazeUser
- CMazeInfo::SyncMazeInfo: 同步 m_stMazeInfo 各字段, 重建 m_vecEnterMember/m_mapWaitEnterMazeUser, nState==3 时设置 m_dwStateTime
- CMazeInfo::CheckDisconnecUsertState: GetTB_MAZE_INFO, Maze_Type(2/8/9) 检查, 状态判断 (10->2, 11->0, 12->设置13->3)


---

### CPartyManager 方法验证追加 (2026-04-30 13:19)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | PartyManager.cpp | CPartyManager::SetMember | 0x1400399A0 | verified | IDA |

说明：
- CPartyManager::SetMember: find m_mapParty, found 时调用 CForce::SetMember, else new CForce + insert


---

### XGameDBSocketMgr 方法验证追加 (2026-04-30 13:19)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore/XServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::Init | 0x140032E50 | verified | IDA |
| XCore/XServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::DisConnect | 0x140033EA0 | verified | IDA |

说明：
- XGameDBSocketMgr::Init: 5 类 DBAgent (Game/Account/Log/Statistics/SGLog), operator new(66776*count+8), Init_2 + m_bState=1
- XGameDBSocketMgr::DisConnect: 遍历所有 Agent 数组, SetSafetyShutdown(true) + XIOCPClient::DisConnect



---

### CForce/CParty 方法验证追加 (2026-04-30 13:26)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | PartyManager.cpp | CForce::SetMember | 0x1400398D0 | verified | IDA |
| XLoginServer | PartyManager.cpp | CForce::IsEmpty | 0x140039980 | verified | IDA |
| XLoginServer | PartyManager.cpp | CForceManager::RemoveMember | 0x140039B30 | verified | IDA |
| XLoginServer | PartyManager.cpp | CParty::~CParty | 0x14003A3E0 | verified | IDA |
| XLoginServer | PartyManager.cpp | CForce::SetMazeID | 0x14003A420 | verified | IDA |
| XLoginServer | PartyManager.cpp | CForce::CForce | 0x14003A450 | verified | IDA |

说明：
- CForce::SetMember: 在 m_mapMemberInfo 中查找 nActorID, 存在则更新 UXMapID, 不存在则插入
- CForce::IsEmpty: return m_mapMemberInfo.empty()
- CForceManager::RemoveMember: 查找 m_mapParty, 调用 CParty::RemoveMember, 如果 IsEmpty() 则 erase
- CParty::~CParty: 析构 m_mapMemberInfo
- CForce::SetMazeID: UXMapID 复制赋值到 m_uxMapID
- CForce::CForce: 构造函数, m_mapMemberInfo 初始化, m_nPartyID=nForceID, m_uxMapID=0, size=0x30 bytes


---

### CUserObject 方法验证追加 (2026-04-30 13:26)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserObject.cpp | CUserObject::CanChangeWorld | 0x140044B60 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::ResetWorldState | 0x140044BB0 | verified | IDA |

说明：
- CUserObject::CanChangeWorld: 三条件检查: !m_nWorldState && m_dwWorldChangeWaitTick <= GetTickCount64() && !m_uxTransMapID.nMapID
- CUserObject::ResetWorldState: m_nWorldState=0, m_nBeforeServerID=0, 如果 m_uxTransMapID 不匹配则 LogError, 重置 m_uxTransMapID=0


---

### CServer 方法验证追加 (2026-04-30 13:26)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CServer.cpp | CServer::RegisterProcess | 0x140041180 | verified | IDA |
| XControlServer | CServer.cpp | CServer::RemoveMaze | 0x140041930 | verified | IDA |
| XControlServer | CServer.cpp | CServer::AddMaze | 0x140041410 | verified | IDA |
| XControlServer | CServer.cpp | CServer::OnUpdate | 0x140042550 | verified | IDA |
| XControlServer | CServer.cpp | CServer::SyncMaze | 0x140042340 | verified | IDA |
| XControlServer | CServer.cpp | CServer::UpdateMaze | 0x1400421E0 | verified | IDA |

说明：
- CServer::RegisterProcess: 注册 4 个 Process 处理器 (CServerProcess:0xF2, CUserProcess:0xF3, CGMToolProcess:0xF8, CWorldModeProcess:0xFB), 每个大小 0x48 bytes
- CServer::RemoveMaze: 查找 m_mapMazeInfo, bResult=true 时 ResetParentMaze/ResetChildMaze, PartyID/ForceID 存在时发送 0xF2/0x45 或 0xF2/0x46 包
- CServer::AddMaze: insert 到 m_mapMazeInfo, 发送 0xF7/3 监控包
- CServer::OnUpdate: 遍历 m_mapMazeInfo, IsDestroy() 时发送 0xF2/0x25 包, RemoveMaze, erase
- CServer::SyncMaze: bLast=true 时递增 m_nLogicThreadSyncCount, 达到 m_nLogicThreadCount 时 SetSyncLoad; 否则查找/插入 CMazeInfo 并 SyncMazeInfo
- CServer::UpdateMaze: 查找 m_mapMazeInfo, 存在时调用 CMazeInfo::UpdateMazeInfo, 不存在时创建新 CMazeInfo 并 insert


---

### XGameDBSocketMgr 方法验证追加 (2026-04-30 13:26)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore/XServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::SendAccountDBAgent | 0x140034290 | verified | IDA |
| XCore/XServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::IsLogDBConnectionAll | 0x140034490 | verified | IDA |
| XCore/XServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::IsAccountDBConnectionAll | 0x140034410 | verified | IDA |

说明：
- XGameDBSocketMgr::SendAccountDBAgent: 检查 m_pAccountDBAgent[iIndex].m_bState, 调用 XIOCPClient::Send
- XGameDBSocketMgr::IsLogDBConnectionAll: 遍历 m_pLogDBAgent 数组, 检查每个 XIOCPClient::IsConnection
- XGameDBSocketMgr::IsAccountDBConnectionAll: 遍历 m_pAccountDBAgent 数组, 检查每个 XIOCPClient::IsConnection


---

### CGMToolProcess 构造函数验证追加 (2026-04-30 13:26)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::CGMToolProcess | 0x140034550 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::scalar deleting destructor | 0x1400345D0 | verified | IDA |

说明：
- CGMToolProcess::CGMToolProcess: TXProcess<CServer> 构造, SetCmd(0xF8), SetName("CGMToolProcess")
- CGMToolProcess::scalar deleting destructor: 调用析构函数, 如果参数 & 1 则 operator delete


---

### CMazeInfo 方法验证追加 (2026-04-30 13:31)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | MazeInfo.cpp | CMazeInfo::CMazeInfo | 0x140035F10 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::UpdateMazeInfo | 0x140036560 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::IsValidEnterMaze | 0x140036C10 | verified | IDA |

说明：
- CMazeInfo::CMazeInfo: 构造函数初始化 m_pParentMaze/m_pChildMaze (shared_ptr), m_stPartyInfo, m_vecEnterMember, m_stMazeInfo, m_mapWaitEnterMazeUser, m_nState=1, m_dwStateTime=0, m_nUserCount=0, m_nType=0
- CMazeInfo::UpdateMazeInfo: m_nState!=3 时更新 uxMapID/nUserCount/nState, 清空 m_vecEnterMember/m_mapWaitEnterMazeUser, 重建成员列表, nState==3 && GetChildMaze==null 时设置 m_dwStateTime
- CMazeInfo::IsValidEnterMaze: GetTB_MAZE_INFO 检查 Maze_Type(2/8/9), GetMazeState 检查 (1=可进入, 2=销毁中, 4=销毁完成), IsEnterMember 检查, CheckDisconnecUsertState 检查


---

### std::map 析构/构造验证追加 (2026-04-30 13:31)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | MazeInfo.cpp | std::map<unsigned long,ST_MAZE_WAIT_ENTER_USER_INFO>::~map | 0x140036050 | verified | IDA |
| XControlServer | MazeInfo.cpp | std::_Tree::_Tidy | 0x1400377B0 | verified | IDA |
| XControlServer | MazeInfo.cpp | std::_Tree_val::_Tree_val | 0x140037AD0 | verified | IDA |

说明：
- std::map::~map: 调用 std::_Tree::~_Tree
- std::_Tree::_Tidy: 遍历 erase(begin, end)
- std::_Tree_val::_Tree_val: 初始化 _Mysize=0, allocate head node, 设置 _Parent/_Right/_Left=_Myhead, _Color=1, _Isnil=1


---

### CGameDBSocket 方法验证追加 (2026-04-30 13:31)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore/XServer | GameDBSocket.cpp | CGameDBSocket::OnParse | 0x140031530 | verified | IDA |
| XCore/XServer | GameDBSocket.cpp | CGameDBSocket::SetInfomation | 0x1400314A0 | verified | IDA |
| XCore/XServer | GameDBSocket.cpp | CGameDBSocket::DBLoginParse | 0x140031620 | verified | IDA |
| XCore/XServer | GameDBSocket.cpp | CGameDBSocket::DBEventParse | 0x1400316B0 | verified | IDA |
| XCore/XServer | GameDBSocket.cpp | CGameDBSocket::ResAddServerGroupInfo | 0x140031720 | verified | IDA |

说明：
- CGameDBSocket::OnParse: operator>>(xSessionID), DBParse(this, nullptr, xPacket)
- CGameDBSocket::SetInfomation: sprintf_s(m_szName, "DBAGENT"), GetServerPrivateIPAndPort
- CGameDBSocket::DBLoginParse: SubCmd switch: '!'=ResAddServerGroupInfo, '"'=ResServerGroupMaxUserCount, 'P'=ResCheckServerGroupID
- CGameDBSocket::DBEventParse: SubCmd switch: 6=ResWorldModeLoad, 45=ResRouletteEventRewardLoad
- CGameDBSocket::ResAddServerGroupInfo: operator>>(nErrorCode), nErrorCode<=0 时 XControlServer::SetRegisterAuth(1)


---

### CPartyManager 方法验证追加 (2026-04-30 13:31)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | PartyManager.cpp | CPartyManager::IsParty | 0x140030E80 | verified | IDA |
| XLoginServer | PartyManager.cpp | CForce::scalar deleting destructor | 0x140031320 | verified | IDA |

说明：
- CPartyManager::IsParty: find(m_mapParty, nPartyID) != end()
- CForce::scalar deleting destructor: ~CParty(), if (a2 & 1) operator delete

---

### CDayEventMgr 函数验证追加 (2026-04-30 13:39 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | DayEventManager.cpp | CDayEventMgr::Update | 0x14002D9A0 | verified | IDA |
| XControlServer | DayEventManager.cpp | CDayEventMgr::SetDayEvent | 0x14002DAF0 | verified | IDA |
| XControlServer | DayEventManager.cpp | CDayEventMgr::SendDayEventAllGameServer | 0x14002E4D0 | verified | IDA |

说明：
- CDayEventMgr::Update @ 0x14002D9A0: 60秒 tick, 首次调用 SetDayEvent + SendDayEventAllGameServer
- CDayEventMgr::SetDayEvent @ 0x14002DAF0: 复杂事件配置逻辑, TB_DAY_EVENT + random_shuffle + TB_DAY_EVENT_BOOSTER
- CDayEventMgr::SendDayEventAllGameServer @ 0x14002E4D0: XSendPacket(0xF2, 0x66), GetDayEvent, SendPacketAll

---

### CMazeInfo 函数验证追加 (2026-04-30 13:39 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | MazeInfo.cpp | CMazeInfo::Init (ST_CREATE_MODE_MAZE) | 0x140036450 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::UpdateMazeInfo | 0x140036560 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::SyncMazeInfo | 0x140036820 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::IsValidEnterMaze | 0x140036C10 | verified | IDA |

说明：
- CMazeInfo::Init (ST_CREATE_MODE_MAZE) @ 0x140036450: qmemcpy + m_nType=2
- CMazeInfo::UpdateMazeInfo @ 0x140036560: m_nState 更新, vecMemberInfo 同步, state==3 时检查子迷宫
- CMazeInfo::SyncMazeInfo @ 0x140036820: stPartyInfo + m_stMazeInfo 同步, vecMemberInfo 清空重建
- CMazeInfo::IsValidEnterMaze @ 0x140036C10: Maze_Type(2/8/9) 检查, GetMazeState 验证, 错误码 55022/55036/55042/55043/55054

---

### CServer 函数验证追加 (2026-04-30 13:39 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CServer.cpp | CServer::Init | 0x140041320 | verified | IDA |
| XControlServer | CServer.cpp | CServer::SetServerInfo | 0x140041590 | verified | IDA |
| XControlServer | CServer.cpp | CServer::IsValidEnterPartyMemberInMaze | 0x140041C80 | verified | IDA |
| XControlServer | CServer.cpp | CServer::UpdateMaze | 0x1400421E0 | verified | IDA |

说明：
- CServer::Init @ 0x140041320: 初始化 m_nMaxMazeCount=0, m_stSyncInfo, 清空 m_mapMazeInfo/m_mpMyRoomInfo
- CServer::SetServerInfo @ 0x140041590: 复制 m_serverInfo, 设置 SyncData 标志, m_nSyncWorldForThread
- CServer::IsValidEnterPartyMemberInMaze @ 0x140041C80: 检查 PartyID, MapID 匹配, 遍历 m_vecEnterMember
- CServer::UpdateMaze @ 0x1400421E0: 查找或创建 CMazeInfo, 调用 UpdateMazeInfo

---

### XGameDBSocketMgr 函数验证追加 (2026-04-30 13:39 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::AutoConnect | 0x140033C30 | verified | IDA |
| XControlServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::DisConnect | 0x140033EA0 | verified | IDA |
| XControlServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::IsAccountDBConnectionAll | 0x140034410 | verified | IDA |
| XControlServer | GameDBSocketMgr.cpp | TXDBSocket<CServer>::AutoConnect | 0x140034510 | verified | IDA |

说明：
- XGameDBSocketMgr::AutoConnect @ 0x140033C30: 遍历 Account/Game/Log/Statistics/SGLog DBAgent
- XGameDBSocketMgr::DisConnect @ 0x140033EA0: SetSafetyShutdown(true) + XIOCPClient::DisConnect
- XGameDBSocketMgr::IsAccountDBConnectionAll @ 0x140034410: 检查所有 AccountDBAgent 连接状态
- TXDBSocket<CServer>::AutoConnect @ 0x140034510: Connect(m_szIP, m_sPort)

---

---

### CRouletteEventMgr 函数验证追加 (2026-04-30 13:40 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::CRouletteEventMgr | 0x14003A4C0 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::~CRouletteEventMgr | 0x14003A530 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::Clear | 0x14003A560 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::LoadRouletteEvent | 0x14003A5A0 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::CheckItemDayCount | 0x14003A9E0 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::Update | 0x14003AC10 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::SendDBRouletteRewardInfo | 0x14003ACA0 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::SendRouletteEvent | 0x14003AD30 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::UpdateRouletteItemInfo | 0x14003ADE0 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::SetRouletteRewardInfo | 0x14003AEB0 | verified | IDA |

说明：
- CRouletteEventMgr::CRouletteEventMgr @ 0x14003A4C0: CFSRWLock + PS_GM_ROULETTE_EVENT 构造 + Clear()
- CRouletteEventMgr::~CRouletteEventMgr @ 0x14003A530: Clear() + PS_GM_ROULETTE_EVENT 析构
- CRouletteEventMgr::Clear @ 0x14003A560: m_dw64Tick=0, m_bLoad=false, PS_GM_ROULETTE_EVENT::Clear
- CRouletteEventMgr::LoadRouletteEvent @ 0x14003A5A0: 版本检查, m_bLoad ? CheckItemDayCount : operator=
- CRouletteEventMgr::CheckItemDayCount @ 0x14003A9E0: nEventID 检查, 复制 nCurCount, Clear + operator=
- CRouletteEventMgr::Update @ 0x14003AC10: 60秒 tick, 首次调用 LoadRouletteEvent + SendDBRouletteRewardInfo
- CRouletteEventMgr::SendDBRouletteRewardInfo @ 0x14003ACA0: XSendPacket(0xF2, 0x73)
- CRouletteEventMgr::SendRouletteEvent @ 0x14003AD30: XSendPacket(0xF2, 0x74), bAll 参数
- CRouletteEventMgr::UpdateRouletteItemInfo @ 0x14003ADE0: 按 nRewardID 匹配更新 nCurCount
- CRouletteEventMgr::SetRouletteRewardInfo @ 0x14003AEB0: 遍历 vecInfo 更新计数, 完成后 SendRouletteEvent(true)

---

### CUserProcess 函数验证追加 (2026-04-30 13:58 +08:00)

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

说明：
- CUserProcess::Parse @ 0x1400450E0: 17 个子命令 switch (main=0xF3)
- CUserProcess::SyncSelectCharacter @ 0x140045310: 读取 STCharInfo, 调用 XControlServer::AddUser
- CUserProcess::SyncLogoutUser @ 0x1400454B0: 读取 dwActorID/nAccountState, 调用 XControlServer::RemoveUser
- CUserProcess::SyncUpdateUserMap @ 0x140045570: 读取 PS_UPDATE_USER_MAP_INFO, 调用 XControlServer::UpdateUserMap
- CUserProcess::SyncUserKickout @ 0x1400455D0: 读取 PS_KICK_USER_INFO, 调用 KickoutUser_UseLock
- CUserProcess::ReqUserChatNotice @ 0x140045640: 读取 PS_CHAT_NOTICE, 调用 SendChatNotice
- CUserProcess::ReqUserChatMegaPhone @ 0x1400456B0: 读取 PS_CHAT_MEGAPHONE + PS_CHAT_ITEM_LINK_FOR_SERVER
- CUserProcess::ReqUserChangeServer @ 0x140045A10: 读取 PS_REQ_CHANGE_SERVER, 调用 ChangeServer
- CUserProcess::SyncUserMoneyLog @ 0x140045A80: 简单记录日志
- CUserProcess::ReqUserEnterPartyMaze @ 0x140045AC0: 读取 dwPartyID+UXMapID+PS_ENTER_MAP_REQ, 调用 EnterMemberInMaze
- CUserProcess::ReqUserEnterForceMaze @ 0x140045B70: 类似 PartyMaze, 使用 ForceID
- CUserProcess::ReqUserTradePasswordStateSync @ 0x140045C30: 同步交易密码状态
- CUserProcess::ReqUserTradePasswordState @ 0x140045CB0: 查询交易密码状态
- CUserProcess::ReqNameChange @ 0x140045D10: 读取 PS_CHANGE_NAME, 调用 CharacterNameChange
- CUserProcess::ReqCheckSessionID @ 0x140045D70: 读取 dwUAID+biAuthSessionID, 调用 CheckSessionID
- CUserProcess::ReqGameServerEnterUser @ 0x140045E00: 获取用户 BlockType, 发送 0xF2/0x60 响应
- CUserProcess::ReqUserUpdateAuthType @ 0x140045F40: 读取 PS_USER_UPDATE_AUTH_TYPE, 调用 UpdateAuthType

---

### CWorldModeProcess 函数验证追加 (2026-04-30 13:58 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::~CWorldModeProcess | 0x14004D5C0 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::Parse | 0x14004D5F0 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeUpdate | 0x14004D5F0 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeCommand | 0x14004D680 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeComplete | 0x14004DAF0 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeEnterList | 0x14004DB20 | verified | IDA |

说明：
- CWorldModeProcess::Parse @ 0x14004D5F0: 4 个子命令 (main=0xFB): 0x02/0x06/0x07/0x09
- CWorldModeProcess::ReqWorldModeCommand @ 0x14004D680: nState==3 获取 ModeList, nState==1/2 模式控制
- ReqWorldModeCommand nState==1: 检查 ActiveMode, SetModeStartTime/SetModeFinishTime
- ReqWorldModeCommand nState==2: 调用 CWorldModeMgr::ReqModeComplete
- ReqWorldModeCommand nState==3: 发送 0xFB/0x06 响应包 (dwUCID + ModeList)

---

### XGameDBSocketMgr Init 函数验证追加 (2026-04-30 13:58 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::Init | 0x140032E50 | verified | IDA |
| XControlServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::SendAccountDBAgent | 0x140034290 | verified | IDA |
| XControlServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::SendGameDBAgent | 0x140034310 | verified | IDA |
| XControlServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::SendLogDBAgent | 0x140034390 | verified | IDA |
| XControlServer | GameDBSocketMgr.cpp | XGameDBSocketMgr::IsLogDBConnectionAll | 0x140034490 | verified | IDA |

说明：
- XGameDBSocketMgr::Init @ 0x140032E50: 初始化 5 类 DBAgent 数组
  - type=0: GameDBAgent, m_byType=0
  - type=1: AccountDBAgent, m_byType=1
  - type=2: LogDBAgent, m_byType=2
  - type=3: StatisticsDBAgent, m_byType=3
  - type=4: SGLogDBAgent, m_byType=4
- Init 使用 XOption::GetDBAgentInfo(type, IP, Port, Name, MaxIOPoolCount, Count)
- 每个 Agent 使用 CGameDBSocket 数组存储, 大小 0x104D8 (66776 bytes)
- XGameDBSocketMgr::SendAccountDBAgent @ 0x140034290: 检查索引和 m_bState
- XGameDBSocketMgr::SendGameDBAgent @ 0x140034310: 检查索引和 m_bState
- XGameDBSocketMgr::SendLogDBAgent @ 0x140034390: 检查索引和 m_bState
- XGameDBSocketMgr::IsLogDBConnectionAll @ 0x140034490: 遍历检查 XIOCPClient::IsConnection

---

### XControlServer 函数验证追加 (2026-04-30 14:05 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::AddUser | 0x14000C220 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::RemoveUser | 0x14000C610 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::UpdateAuthType | 0x140015560 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CheckSessionID | 0x140014590 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::MyRoomEnterReq | 0x140012050 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ReqCreateMyRoom | 0x1400147E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::DeleteMyRoomReq | 0x140014B10 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SetCachingLoad | 0x140014D40 | verified | IDA |

说明：
- XControlServer::AddUser @ 0x14000C220:
  - 使用 boost::multi_index 容器管理用户 (CID/UAID 索引)
  - 检查已存在用户: 如果 sessionID 更大则踢出新登录, 否则踢出旧用户
  - 创建 CUserObject (0x378 bytes), 设置 IP/TradePW/SessionID/BlockType
  - SetGroupIDTick(GetTickCount64() + 300000) - 5分钟超时
  - 调用 CWorldManager::UpdateUserMap 更新用户地图
- XControlServer::RemoveUser @ 0x14000C610:
  - 发送 AccountDB 包 (main=0, sub=2)
  - 计算 playTime = (GetTickCount64 - vftable) / 1000
  - 处理 Party/Force 成员移除
  - 更新 AuthType 计数 m_nSGAuthTypeCount
- XControlServer::UpdateAuthType @ 0x140015560:
  - 使用 UAID 索引查找用户
  - 更新 m_nSGAuthTypeCount[old]-- 和 m_nSGAuthTypeCount[new]++
  - byAuthType 范围检查: 1-3 有效
- XControlServer::CheckSessionID @ 0x140014590:
  - SessionID 比较: AuthSessionID <= biAuthSessionID 则踢出
  - byResult: 0=踢出, 1=保留, 2=服务器满
  - 发送 0xF3/0x32 响应包
- XControlServer::MyRoomEnterReq @ 0x140012050:
  - FindMyRoom: 不存在则 CreateMyRoom, 存在则 RoomEnterUser
  - 需要 GetMyRoomServer 返回有效服务器
- XControlServer::ReqCreateMyRoom @ 0x1400147E0:
  - nResult 非 0 则删除 MyRoom 并返回错误 58203
  - 否则调用 CMyRoom::CreateSucc, 发送 0xF2/0x51 响应
  - 响应包含 PS_ENTER_MAP_RES + ST_MYROOM_OWNER_INFO + dwOwnerUCID
- XControlServer::DeleteMyRoomReq @ 0x140014B10:
  - 检查 GetMyRoomState == 1 才调用 DeleteReq
  - 发送 0xF2/0x53 响应包 (dwOwnerUAID + UXMapID)
- XControlServer::SetCachingLoad @ 0x140014D40:
  - 使用位运算 m_dwCachingLoad |= eServerLoad
  - E_SERVER_CACHING_LOAD_DB_COMPLETE 时检查所有位
  - 如果 DB_COMPLETE + DB(4) + ACCOUNT(1) + LOG(2) 都置位则 SendCachingLoad

---

### CGameDBSocket 函数验证追加 (2026-04-30 14:10 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::OnParse | 0x140031530 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::DBParse | 0x140031580 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::DBLoginParse | 0x140031620 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::DBEventParse | 0x1400316B0 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::ResAddServerGroupInfo | 0x140031720 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::ResCheckServerGroupID | 0x140031830 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::OnDisConnect | 0x1400318C0 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::ResWorldModeLoad | 0x140031A30 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::ResRouletteEventRewardLoad | 0x140031AA0 | verified | IDA |

说明：
- CGameDBSocket::OnParse @ 0x140031530: 读取 xSessionID, 调用 DBParse
- CGameDBSocket::DBParse @ 0x140031580:
  - main==2: 调用 DBLoginParse
  - main==73(0x49): 调用 DBEventParse
- CGameDBSocket::DBEventParse @ 0x1400316B0:
  - sub==6: 调用 ResWorldModeLoad
  - sub==45(0x2D): 调用 ResRouletteEventRewardLoad
- CGameDBSocket::ResAddServerGroupInfo @ 0x140031720:
  - errorCode<=0: SetRegisterAuth(true), 日志 "Register Server Group!"
  - errorCode>0: 错误日志
- CGameDBSocket::ResCheckServerGroupID @ 0x140031830:
  - 读取 dwUAID + nServerGroupID + nLastServerGroupID
  - 调用 XControlServer::ResCheckServerGroupID
- CGameDBSocket::OnDisConnect @ 0x1400318C0:
  - m_byType!=4: 调用 Shutdown
  - m_byType==0 && !m_bSafetyShutdown: Sleep(1000), SET_SERVICE_STATE, Shutdown
  - GameDB 断连时整个服务器关闭
- CGameDBSocket::ResWorldModeLoad @ 0x140031A30:
  - 读取 ST_WORLD_MODE_INFO_VEC
  - 调用 CWorldModeMgr::LoadMode
- CGameDBSocket::ResRouletteEventRewardLoad @ 0x140031AA0:
  - 读取 PS_DB_ROULETTE_REWARD_INFO
  - 调用 CRouletteEventMgr::SetRouletteRewardInfo

---

### XControlServer 核心函数验证追加 (2026-04-30 14:20 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::InitServer | 0x14000BD00 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::OnUpdate | 0x14000F240 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ReqCreateMaze | 0x14000E3A0 | verified | IDA |

说明：
- XControlServer::InitServer @ 0x14000BD00:
  - 初始化 CLogThreadManager, XSeed, m_xOption
  - XResourceMgr::Init + XResourceMgr::Load
  - 设置 ServerContents 选项 (E_SERVER_OPTION_ATTENDANCE 到 MAX)
  - XItemFactory::Init(GroupID, Channel)
  - XGameDBSocketMgr::Init + AutoConnect
  - CObserveSocket::StartUp
  - CWorldModeMgr::Init
  - SetCachingLoad(E_SERVER_CACHING_LOAD_DB_COMPLETE)
- XControlServer::OnUpdate @ 0x14000F240:
  - dwDeleteTime: 每 5 秒遍历 m_mapMazeServer, 调用 CServer::OnUpdate
  - m_nMoneyTick: 每 60 秒发送 DBLog(100, 2) + DBLog(100, 3)
  - dwServerGroupSync: 每 5 秒发送 ServerGroupInfo + SendAccountDBLoginAddServerGroupInfo
  - dwSGUpdate: 每 60 秒更新
  - dwCheckGroupIDTick: 每 600 秒检查
  - CObserveSocket::OnUpdate(IP, Port, UserCount)
  - CWorldModeMgr::Update
  - CDayEventMgr::Update
  - CRouletteEventMgr::Update(dw64CurrentTick, ServerGroupInfo.wID)
- XControlServer::ReqCreateMaze @ 0x14000E3A0:
  - 服务器选择逻辑:
    - 如果有 ParentMazeID: FindServerFromMaze, 检查 0.7 阈值
    - 如果 CreateType==2: 直接使用请求服务器
    - 否则遍历 m_mapMazeServer 找最低负载服务器 (<0.95 阈值)
  - 创建 ST_CREATE_MAZE_FOR_RELAY, 插入 m_mapCreateMazeReq
  - 发送 0xF2/0x21 包到选中服务器
  - 如果没有可用服务器: 发送 0xF2/0x22 错误响应 (nResult=3)

---

### CGameDBSocket 函数验证追加 (2026-04-30 14:15 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CGameDBSocket.cpp | CGameDBSocket::OnDisConnect | 0x1400318C0 | verified | IDA |
| XControlServer | CGameDBSocket.cpp | CGameDBSocket::OnNotConnect | 0x140031990 | verified | IDA |
| XControlServer | CGameDBSocket.cpp | CGameDBSocket::ResWorldModeLoad | 0x140031A30 | verified | IDA |
| XControlServer | CGameDBSocket.cpp | CGameDBSocket::ResRouletteEventRewardLoad | 0x140031AA0 | verified | IDA |

说明：
- CGameDBSocket::OnDisConnect @ 0x1400318C0:
  - 日志: "[DB_SOCKET] OnDisconnect ( %d ) !!"
  - m_byType != 4: 调用 XIOCPClient::Shutdown
  - m_byType == 0 && !m_bSafetyShutdown: Sleep(1000), SET_SERVICE_STATE(1, 3), 整个服务器 Shutdown
- CGameDBSocket::OnNotConnect @ 0x140031990:
  - 日志: "[DB_SOCKET] OnNotConnect ( %d ) !!"
  - !m_bCloseProcess && m_byType != 4: 设置 m_bCloseProcess=1, Sleep(1000), SET_SERVICE_STATE, Shutdown
- CGameDBSocket::ResWorldModeLoad @ 0x140031A30:
  - 读取 ST_WORLD_MODE_INFO_VEC
  - 调用 CWorldModeMgr::LoadMode(&m_worldModeManager, &stInfo.vecInfo)
- CGameDBSocket::ResRouletteEventRewardLoad @ 0x140031AA0:
  - 读取 PS_DB_ROULETTE_REWARD_INFO
  - 调用 CRouletteEventMgr::SetRouletteRewardInfo

---

### XControlServer 方法验证追加 (2026-04-30 14:20 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::UpdateUserMap | 0x14000CCA0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::KickoutUser_NoLock | 0x14000D7E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendPacketToGameServer | 0x14000D990 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::AddLoginServerInfo | 0x14000DAB0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::AddGameServerInfo | 0x14000DBD0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::AddMazeServerInfo | 0x14000DD70 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::RemoveGameServerInfo | 0x14000E0B0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ResCreateMaze | 0x14000ED10 | verified | IDA |

说明：
- XControlServer::UpdateUserMap @ 0x14000CCA0:
  - 查找用户，检查 AuthSessionID 匹配
  - 地图变化: 更新 CUserObject::SetMapIns, CWorldManager::UpdateUserMap
  - 处理 Party/Force 成员更新和离开
  - Event Maze (Maze_Type==13||19): 处理 m_mapEventMazeToEnter
  - 发送 0xF3/0x07 kick 包到 BeforeServer
- XControlServer::KickoutUser_NoLock @ 0x14000D7E0:
  - 按 UAID 从 m_UserInfos 查找并删除
  - 发送 0xF3/0x07 包到所有服务器 (SendPacketAll)
  - 日志: "Kickout User : %d / %d"
- XControlServer::SendPacketToGameServer @ 0x14000D990:
  - 遍历 m_mapGameServer, 调用 XClient::SendEx
  - 不排除 pExceptSession (参数未使用)
- XControlServer::AddLoginServerInfo @ 0x14000DAB0:
  - 设置 m_pLoginServer, m_bAddLogin=1
  - 复制 m_stServerGroupInfo (wID, sPort, szPublicIP, nState=1)
  - 从 TB_SERVERINFO 获取 Server_Name
  - 调用 SendAccountDBLoginAddServerGroupInfo
- XControlServer::AddGameServerInfo @ 0x14000DBD0:
  - 插入 m_mapGameServer[ServerID] = pServer
  - 发送 0xF2/0x66 (DayEventList) 到新服务器
  - 调用 CRouletteEventMgr::SendRouletteEvent(1)
  - UnSetCachingLoad(E_SERVER_CACHING_LOAD_USER)
- XControlServer::AddMazeServerInfo @ 0x14000DD70:
  - 检查服务器是否已在 m_mapGameServer 中
  - 如果 nCount > 0: 设置 CServer::SetMaxMazeCount, 插入 m_mapMazeServer
  - 调用 CServer::RecvMapInfo
- XControlServer::RemoveGameServerInfo @ 0x14000E0B0:
  - 按 ServerID 从 m_UserInfos 删除所有用户
  - 从 m_mapGameServer 和 m_mapMazeServer 移除
  - 调用 RemoveMyRoomServerInfo
- XControlServer::ResCreateMaze @ 0x14000ED10:
  - 从 m_mapCreateMazeReq 查找请求
  - 发送 0xF2/0x22 响应到 pReqChannelServer
  - Party: 调用 CPartyManager::SetMazeID, 发送 0xF2/0x45
  - Force: 调用 CForceManager::SetMazeID, 发送 0xF2/0x46
  - 创建 CMazeInfo 并添加到服务器

---

### XControlServer GM 方法验证追加 (2026-04-30 14:25 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::ReqGoBackMaze | 0x14000EAC0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::UpdateAccountState | 0x14000F8B0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ClearUserState | 0x14000F960 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CheckPartyInMaze | 0x14000FD50 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CheckForceInMaze | 0x1400100B0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ChangeServer | 0x140010410 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SetUsersInfo | 0x140010740 | verified | IDA |

说明：
- XControlServer::ReqGoBackMaze @ 0x14000EAC0:
  - GetMazeInfo + GetParentMaze
  - 发送 0xF2/0x28 响应 (bResult=0/1)
- XControlServer::UpdateAccountState @ 0x14000F8B0:
  - 发送 DBPacket(0, 2, 0x13) 到 AccountDB
  - 参数: dwUCID + nState
- XControlServer::ClearUserState @ 0x14000F960:
  - 按 ServerID 遍历 m_UserInfos
  - 收集所有 UAID 到 PS_REQ_CLEAR_USER_STATE
  - 发送 DBPacket(0, 2, 0x12) 到 AccountDB
- XControlServer::CheckPartyInMaze @ 0x14000FD50:
  - CForceManager::GetMazeID 检查 Party 迷宫
  - GetTB_MAZE_INFO + CheckAdmissionMember
  - CMazeInfo::IsValidEnterMaze
  - 发送 0xF2/0x32 响应
- XControlServer::CheckForceInMaze @ 0x1400100B0:
  - CForceManager::GetMazeID 检查 Force 迷宫
  - 同 Party 流程，错误码 53144
- XControlServer::ChangeServer @ 0x140010410:
  - byType==1: LoginServer, byType==0: AuthServer
  - UpdateAccountState(dwUAID, 1/2)
  - 发送 0xF3/0x12 响应
  - SendDBLog(2, 50)
- XControlServer::SetUsersInfo @ 0x140010740:
  - 遍历 vecUserInfo, 调用 AddUser
  - bFinish: 记录用户数, RecvUserInfo, SetSyncLoad(E_SERVER_SYNC_LOAD_USER)

---

### XControlServer GM Tool 方法验证追加 (2026-04-30 14:30 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::GM_UserKick | 0x140010FF0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_Notice | 0x1400110F0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_Shutdown | 0x140011200 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_TimeEvent | 0x1400112E0 | verified | IDA |

说明：
- XControlServer::GM_UserKick @ 0x140010FF0:
  - 设置 PS_KICK_USER_INFO(dwUAID, byKickType=6, strMsg)
  - 调用 KickoutUser_UseLock
  - 发送 0xF8/0x01 响应到 GMTool
- XControlServer::GM_Notice @ 0x1400110F0:
  - 设置 PS_CHAT_NOTICE(byType, strColor, strMsg)
  - 调用 SendChatNotice
  - 发送 0xF8/0x02 响应
- XControlServer::GM_Shutdown @ 0x140011200:
  - 发送 0xF2/0x08 到所有服务器
  - 发送 0xF8/0x03 响应
- XControlServer::GM_TimeEvent @ 0x1400112E0:
  - 发送 0xF2/0x09 到所有服务器 (SendPacketAll with bLoginWith=0)

---

### XControlServer 辅助方法验证追加 (2026-04-30 14:35 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::GM_ValueEvent | 0x140011380 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_ServerOption | 0x140011420 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_CashShopBanner | 0x140011550 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::FindServerFromMaze | 0x1400115F0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GetMazeInfo | 0x1400116E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendDBAccount | 0x140011880 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::MyRoomEnterReq | 0x140012050 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::EnterMemberInMaze | 0x140012200 | verified | IDA |

说明：
- XControlServer::GM_ValueEvent @ 0x140011380:
  - 发送 0xF2/0x14 到所有服务器
- XControlServer::GM_ServerOption @ 0x140011420:
  - 遍历 14 个 ServerContents 选项
  - 调用 XResourceMgr::SetServerContents
  - 发送 0xF2/0x61 到所有 GameServer
- XControlServer::GM_CashShopBanner @ 0x140011550:
  - 发送 0xF2/0x15 到所有 GameServer
- XControlServer::FindServerFromMaze @ 0x1400115F0:
  - 遍历 m_mapMazeServer
  - 调用 CServer::IsMaze(pServer, uxMapID) 匹配
- XControlServer::GetMazeInfo @ 0x1400116E0:
  - 遍历 m_mapMazeServer
  - 调用 CServer::GetMazeInfo 查找指定迷宫
- XControlServer::SendDBAccount @ 0x140011880:
  - OrderID % GetAccountDBAgentCount 计算 iIndex
  - 调用 XGameDBSocketMgr::SendAccountDBAgent
- XControlServer::MyRoomEnterReq @ 0x140012050:
  - FindMyRoom: 不存在则 GetMyRoomServer + CreateMyRoom
  - 存在则 CMyRoom::RoomEnterUser
- XControlServer::EnterMemberInMaze @ 0x140012200:
  - FindServerFromMaze + IsValidEnterPartyMemberInMaze
  - 发送 0xF4/0x08 响应

---

### XControlServer Matching 方法验证追加 (2026-04-30 14:40 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::CreateMatchingMaze (Party) | 0x140012460 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CreateMatchingMaze (Force) | 0x140012A40 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CreateMatchingModeMaze | 0x140013030 | verified | IDA |

说明：
- XControlServer::CreateMatchingMaze (Party) @ 0x140012460:
  - 获取 PartyMaster 用户，检查其服务器
  - 服务器选择: 0.7 阈值优先，0.95 阈值遍历
  - 发送 0xF2/0x41 包
- XControlServer::CreateMatchingMaze (Force) @ 0x140012A40:
  - 获取 ForceMaster 用户，同上逻辑
  - 发送 0xF2/0x42 包
- XControlServer::CreateMatchingModeMaze @ 0x140013030:
  - 按 dwMasterServerID 查找服务器
  - 同样使用 0.7/0.95 阈值选择
  - 发送 0xF2/0x72 包

---

---

### CGameDBSocket 方法验证追加 (2026-04-30 14:26 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::CGameDBSocket | 0x1400313F0 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::~CGameDBSocket | 0x140031440 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::FindUser | 0x140031470 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::SetInfomation | 0x1400314A0 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::OnParse | 0x140031530 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::DBParse | 0x140031580 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::DBLoginParse | 0x140031620 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::DBEventParse | 0x1400316B0 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::ResAddServerGroupInfo | 0x140031720 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::ResServerGroupMaxUserCount | 0x1400317A0 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::ResCheckServerGroupID | 0x140031830 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::OnDisConnect | 0x1400318C0 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::OnNotConnect | 0x140031990 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::ResWorldModeLoad | 0x140031A30 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::ResRouletteEventRewardLoad | 0x140031AA0 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::WriteLog | 0x140031B90 | verified | IDA |

说明：
- CGameDBSocket::CGameDBSocket @ 0x1400313F0: 初始化 TXDBSocket, m_bState=0, m_byType=5
- CGameDBSocket::FindUser @ 0x140031470: XControlServer::FindUser 调用
- CGameDBSocket::SetInfomation @ 0x1400314A0: sprintf_s(m_szName,"DBAGENT"), GetServerPrivateIPAndPort
- CGameDBSocket::OnParse @ 0x140031530: 解析 xSessionID, DBParse(nullptr, xPacket)
- CGameDBSocket::DBParse @ 0x140031580: mainCmd==2->DBLoginParse, mainCmd==73(0x49)->DBEventParse
- CGameDBSocket::DBLoginParse @ 0x140031620: subCmd=='!'->ResAddServerGroupInfo, '"'->ResServerGroupMaxUserCount, 'P'->ResCheckServerGroupID
- CGameDBSocket::DBEventParse @ 0x1400316B0: subCmd==6->ResWorldModeLoad, subCmd==45->ResRouletteEventRewardLoad
- CGameDBSocket::OnDisConnect @ 0x1400318C0: m_byType!=4->Shutdown, m_byType==0 && !m_bSafetyShutdown->服务关闭
- CGameDBSocket::OnNotConnect @ 0x140031990: m_bCloseProcess=1, Sleep(1000), Shutdown
- CGameDBSocket::ResWorldModeLoad @ 0x140031A30: operator>>(stInfo), CWorldModeMgr::LoadMode
- CGameDBSocket::ResRouletteEventRewardLoad @ 0x140031AA0: operator>>(psRewardInfo), CRouletteEventMgr::SetRouletteRewardInfo

---

### CWorldModeProcess 方法验证追加 (2026-04-30 14:26 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::CWorldModeProcess | 0x14004D500 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::~CWorldModeProcess | 0x14004D5C0 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::Parse | 0x14004D5F0 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeCommand | 0x14004D680 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeComplete | 0x14004DAF0 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeUpdate | 0x14004DC20 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeEnterList | 0x14004DC90 | verified | IDA |

说明：
- CWorldModeProcess::CWorldModeProcess @ 0x14004D500: TXProcess 构造, SetCmd(0xFB), SetName("CWorldModeProcess")
- CWorldModeProcess::Parse @ 0x14004D5F0: subCmd==2->ReqWorldModeUpdate, 6->ReqWorldModeCommand, 7->ReqWorldModeComplete, 9->ReqWorldModeEnterList
- CWorldModeProcess::ReqWorldModeCommand @ 0x14004D680: 
  - nState==3: GetModeList, XSendPacket(0xFB,6), operator<<(stModeList), SendEx
  - nState==1: GetActiveMode 检查, IsActiveMode->SetModeStartTime/SetModeFinishTime
  - nState==2: GetActiveMode, PS_WORLD_MODE_COMPLETE 填充, ReqModeComplete(bFinish=1)
- CWorldModeProcess::ReqWorldModeComplete @ 0x14004DAF0: operator>>(stComplete), dwMonsterID, ReqModeComplete, XSendPacket(0xFB,7)
- CWorldModeProcess::ReqWorldModeUpdate @ 0x14004DC20: operator>>(stUpdate), CWorldModeMgr::UpdateMonsterCount
- CWorldModeProcess::ReqWorldModeEnterList @ 0x14004DC90: dwUCID, operator>>(stEnterInfo), GetUser, LoadEnterWorldModeInfo

---

### CWorldModeMgr 方法验证追加 (2026-04-30 14:26 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::CWorldModeMgr | 0x1400497C0 | verified | IDA |
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

说明：
- CWorldModeMgr::CWorldModeMgr @ 0x1400497C0: CFSRWLock 初始化, m_bLoadReq=0, m_bLoadDB=0, map 初始化
- CWorldModeMgr::Init @ 0x140049840: clear map, m_dwUpdateTime=GetTickCount64, m_bModeOn=GetServerContents(D6_MODE)
- CWorldModeMgr::LoadMode @ 0x1400498A0: m_bLoadDB=1, 遍历 1..7 获取 WorldMode, TB_MODE_DISTRICT6 查找, new CWorldMode + insert
- CWorldModeMgr::Update @ 0x14004A050: 
  - SendDBGame(0x49,6) 加载请求
  - 333ms tick: CWorldMode::Update + IsFinish + GetNextModeID + StartMode
  - DayOfWeek 检查: InitMode + SendModeList
- CWorldModeMgr::SendModeSync @ 0x14004A630: GetDayOfWeek, GetWorldMode, 遍历 + CWorldMode::GetModeInfo, XSendPacket(0xFB,5)
- CWorldModeMgr::SetModeStartTime @ 0x14004A9A0: 遍历 vecModeID, find + CWorldMode::SetStartTime
- CWorldModeMgr::ReqModeComplete @ 0x14004ABC0: make_pair(nModeDateID,nModeID), find + ReqModeComplete + SetClearTime
- CWorldModeMgr::IsActiveMode @ 0x14004ACC0: 遍历 vecModeID, find + GetState==1 返回 true
- CWorldModeMgr::GetModeList @ 0x14004AE00: GetDayOfWeek, GetWorldMode, 遍历 + push_back
- CWorldModeMgr::IsTodayModeList @ 0x14004AFC0: GetDayOfWeek, GetWorldMode, nModeDateID 匹配 push_back
- CWorldModeMgr::GetActiveMode @ 0x14004BB40: GetDayOfWeek, GetWorldMode, GetState==1 时返回 nActiveModeDateID/nModeID


---

### CRouletteEventMgr 方法验证追加 (2026-04-30 14:34 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::CRouletteEventMgr | 0x14003A4C0 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::~CRouletteEventMgr | 0x14003A530 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::Clear | 0x14003A570 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::Update | 0x14003AC10 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::LoadRouletteEvent | 0x14003A5A0 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::SendRouletteEvent | 0x14003AD30 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::UpdateRouletteItemInfo | 0x14003ADE0 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::SetRouletteRewardInfo | 0x14003AEB0 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::SendDBRouletteRewardInfo | 0x14003AF40 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::CheckItemDayCount | 0x14003A620 | verified | IDA |

说明：
- CRouletteEventMgr::CRouletteEventMgr @ 0x14003A4C0: CFSRWLock 初始化, PS_GM_ROULETTE_EVENT, m_dw64Tick=0, m_bLoad=false
- CRouletteEventMgr::~CRouletteEventMgr @ 0x14003A530: Clear(), 析构 PS_GM_ROULETTE_EVENT
- CRouletteEventMgr::Clear @ 0x14003A570: m_mapRouletteEvent.clear(), m_mapReward.clear()
- CRouletteEventMgr::Update @ 0x14003AC10: 60秒 tick, 首次调用 LoadRouletteEvent + SendDBRouletteRewardInfo
- CRouletteEventMgr::LoadRouletteEvent @ 0x14003A5A0: 版本检查, m_bLoad ? CheckItemDayCount : operator=
- CRouletteEventMgr::SendRouletteEvent @ 0x14003AD30: XSendPacket(0xF2,0x74), operator<<(psInfo), SendEx
- CRouletteEventMgr::UpdateRouletteItemInfo @ 0x14003ADE0: 按 nRewardID 匹配更新 nCurCount
- CRouletteEventMgr::SetRouletteRewardInfo @ 0x14003AEB0: 遍历 vecInfo 更新计数, m_bLoad=true, SendRouletteEvent(true)
- CRouletteEventMgr::SendDBRouletteRewardInfo @ 0x14003AF40: SendDBGame(0x49,45), subCmd='-'
- CRouletteEventMgr::CheckItemDayCount @ 0x14003A620: 检查物品日计数，重置过期计数

---

### CDayEventMgr 方法验证追加 (2026-04-30 14:34 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | DayEventManager.cpp | CDayEventMgr::CDayEventMgr | 0x14002D8B0 | verified | IDA |
| XControlServer | DayEventManager.cpp | CDayEventMgr::~CDayEventMgr | 0x14002D910 | verified | IDA |
| XControlServer | DayEventManager.cpp | CDayEventMgr::Clear | 0x14002D950 | verified | IDA |
| XControlServer | DayEventManager.cpp | CDayEventMgr::Update | 0x14002D9A0 | verified | IDA |
| XControlServer | DayEventManager.cpp | CDayEventMgr::GetDayEvent | 0x14002DA90 | verified | IDA |
| XControlServer | DayEventManager.cpp | CDayEventMgr::SetDayEvent | 0x14002DAF0 | verified | IDA |
| XControlServer | DayEventManager.cpp | CDayEventMgr::SendDayEventAllGameServer | 0x14002E9C0 | verified | IDA |

说明：
- CDayEventMgr::CDayEventMgr @ 0x14002D8B0: CFSRWLock 初始化, PS_GM_DAY_EVENT, m_dw64Tick=0, m_bLoad=false
- CDayEventMgr::~CDayEventMgr @ 0x14002D910: Clear(), 析构 PS_GM_DAY_EVENT
- CDayEventMgr::Clear @ 0x14002D950: m_mapDayEvent.clear()
- CDayEventMgr::Update @ 0x14002D9A0: 60秒 tick, hour==9 检查触发 SetDayEvent + SendDayEventAllGameServer
- CDayEventMgr::GetDayEvent @ 0x14002DA90: 从 m_mapDayEvent 按日期获取事件
- CDayEventMgr::SetDayEvent @ 0x14002DAF0: 复杂日事件配置逻辑:
  - TB_DAY_EVENT 表查询: Event_Count_Min/Max, Fix_Maze_Select_*, Maze_Select_*, Booster_Group_*
  - random_shuffle 迷宫选择
  - 多级字段填充 PS_GM_DAY_EVENT
- CDayEventMgr::SendDayEventAllGameServer @ 0x14002E9C0: 遍历 GameServer, XSendPacket(0xF2,0x73)


---

### CServerProcess 方法验证追加 (2026-04-30 14:40 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServerProcess::CServerProcess | 0x14003C9D0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::~CServerProcess | 0x14003CA90 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::Parse | 0x14003CAC0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqFindUser | 0x14003CEE0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateServer | 0x14003D0A0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMaze | 0x14003D1F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResCreateMaze | 0x14003D360 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterReq | 0x14003E460 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterRes | 0x14003E680 | verified | IDA |

说明：
- CServerProcess::Parse @ 0x14003CAC0: 37 个子命令分发 (0x01~0x77), 包含迷宫/地图/MyRoom相关
- CServerProcess::ReqFindUser @ 0x14003CEE0: XParse wchar szOtherName[21], dwUCID, byState, GetUser + XSendPacket(0xF3,0x58)
- CServerProcess::ReqCreateMaze @ 0x14003D1F0: operator>>(ST_CREATE_MAZE), XControlServer::ReqCreateMaze
- CServerProcess::ResCreateMaze @ 0x14003D360: operator>>(ST_CREATE_MAZE), XControlServer::ResCreateMaze
- CServerProcess::ReqMyRoomEnterReq @ 0x14003E460: ST_MYROOM_USER + ST_MYROOM_OWNER_INFO 解析, XControlServer::MyRoomEnterReq
- CServerProcess::ReqMyRoomEnterRes @ 0x14003E680: nErrorCode==0->FindMyRoom+EnterSucc, PS_ENTER_MAP_RES 填充, XSendPacket(0xF2,0x51)


---

### CServerProcess 方法验证追加 (续) (2026-04-30 14:44 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncMaze | 0x14003D4F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMap | 0x14003D5D0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncUserPartyInfo | 0x14003D9E0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMatchingMazeFromCommunity | 0x14003DD60 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResCreateMatchingMazeFromGame | 0x14003DFF0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::PartyMazeSync | 0x14003EA00 | verified | IDA |

说明：
- CServerProcess::SyncMaze @ 0x14003D4F0: PS_MAZE_UPDATE_INFO_SYNC 解析, CServer::SyncMaze + XControlServer::SyncEventMaze
- CServerProcess::ReqCreateMap @ 0x14003D5D0: PS_CREATE_MAP_LIST + nMazeCount, CWorldManager::AddMap + SendChannelInfoAll
- CServerProcess::SyncUserPartyInfo @ 0x14003D9E0: ST_PARTY_INFO + UXMapID 解析, byGroupType 分发 Party/Force, SetMember/SetMazeID
- CServerProcess::ReqCreateMatchingMazeFromCommunity @ 0x14003DD60: ST_CREATE_MAZE + PS_PARTY_INFO/PS_FORCE_INFO + dwMatchingID, byGroupType 分发
- CServerProcess::ResCreateMatchingMazeFromGame @ 0x14003DFF0: dwMatchingID + ST_CREATE_MAZE + PS_PARTY_INFO, eType 分发 PARTY/FORCE
- CServerProcess::PartyMazeSync @ 0x14003EA00: dwPartyID + UXMapID, CForceManager::SetMazeID


---

### CServer 方法验证追加 (2026-04-30 14:48 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CServer.cpp | CServer::RegisterProcess | 0x140041180 | verified | IDA |
| XControlServer | CServer.cpp | CServer::RemoveMaze | 0x140041930 | verified | IDA |
| XControlServer | CServer.cpp | CServer::SyncMaze | 0x140042340 | verified | IDA |
| XControlServer | CServer.cpp | CServer::OnUpdate | 0x140042550 | verified | IDA |
| XControlServer | CServer.cpp | CServer::CreateMyRoom | 0x1400427C0 | verified | IDA |
| XControlServer | CServer.cpp | CServer::FindMyRoom | 0x140042A30 | verified | IDA |

说明：
- CServer::RegisterProcess @ 0x140041180: 注册 CServerProcess(0xF2), CUserProcess(0xF3), CGMToolProcess(0xF8), CWorldModeProcess(0xFB)
- CServer::RemoveMaze @ 0x140041930: ResetParentMaze/ResetChildMaze, Party/Force MazeID 处理, SetMazeState(1,0)
- CServer::SyncMaze @ 0x140042340: bLast->m_nLogicThreadSyncCount++, SetSyncLoad(E_SERVER_SYNC_LOAD_MAZE_INFO)
- CServer::OnUpdate @ 0x140042550: 遍历 m_mapMazeInfo, IsDestroy 发送 0xF2/0x25, RemoveMaze
- CServer::CreateMyRoom @ 0x1400427C0: GetMapID, new CMyRoom(96), insert m_mpMyRoomInfo, XSendPacket(0xF2,0x52)
- CServer::FindMyRoom @ 0x140042A30: m_mpMyRoomInfo.find(dwUAID)


---

### CUserProcess 方法验证追加 (2026-04-30 14:52 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserProcess.cpp | CUserProcess::CUserProcess | 0x140044FF0 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::Parse | 0x1400450E0 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::SyncSelectCharacter | 0x140045310 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::SyncUserKickout | 0x1400455D0 | verified | IDA |

说明：
- CUserProcess::CUserProcess @ 0x140044FF0: TXProcess 构造, SetCmd(0xF3), SetName("CUserProcess")
- CUserProcess::Parse @ 0x1400450E0: 16 个子命令分发 (1/3/4/7/0x11~0x35/0x60)
- CUserProcess::SyncSelectCharacter @ 0x140045310: STCharInfo + dwIP + byTradePasswordState + biAuthSessionID + byBlockType 解析, XControlServer::AddUser
- CUserProcess::SyncUserKickout @ 0x1400455D0: PS_KICK_USER_INFO 解析, XControlServer::KickoutUser_UseLock(bUseLock=1)


---

### CGMToolProcess 方法验证追加 (2026-04-30 14:58 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::CGMToolProcess | 0x140034550 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::~CGMToolProcess | 0x140034610 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::Parse | 0x140034640 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMUserKick | 0x140034730 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMNotice | 0x1400347C0 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMShutDwon | 0x140034850 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMTimeEvent | 0x1400348B0 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMValueEvent | 0x140034910 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMServerOption | 0x1400349A0 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMCashShopBanner | 0x140034A10 | verified | IDA |

说明：
- CGMToolProcess::CGMToolProcess @ 0x140034550: TXProcess 构造, SetCmd(0xF8), SetName("CGMToolProcess")
- CGMToolProcess::Parse @ 0x140034640: 7 个子命令分发 (1~7), 默认返回 1
- CGMToolProcess::ReqGMUserKick @ 0x140034730: ST_GM_USER_KICK_INFO 解析, XControlServer::GM_UserKick
- CGMToolProcess::ReqGMNotice @ 0x1400347C0: ST_GM_NOTICE_INFO 解析, XControlServer::GM_Notice
- CGMToolProcess::ReqGMShutDwon @ 0x140034850: dwNo 参数, XControlServer::GM_Shutdown
- CGMToolProcess::ReqGMTimeEvent @ 0x1400348B0: ST_GM_TIME_EVENT_INFO 解析, XControlServer::GM_TimeEvent
- CGMToolProcess::ReqGMValueEvent @ 0x140034910: PS_GM_VALUE_EVENT_LIST 解析
- CGMToolProcess::ReqGMServerOption @ 0x1400349A0: PS_CONTENTS_INFO 解析
- CGMToolProcess::ReqGMCashShopBanner @ 0x140034A10: ST_BANNER_LIST 解析


---

### CWorldMode 方法验证追加 (2026-04-30 15:02 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldMode.cpp | CWorldMode::CWorldMode | 0x140046BD0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::~CWorldMode | 0x140046CB0 | verified | IDA |
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
| XControlServer | WorldMode.cpp | CWorldMode::SetNextEventTime | 0x1400486E0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::AddMonsterCount | 0x140048BD0 | verified | IDA |

说明：
- CWorldMode::CWorldMode @ 0x140046BD0: 初始化 ST_WORLD_MODE_INFO, m_nNextModeID=0, m_nLimitTime=0
- CWorldMode::Init @ 0x140046CD0: TB_MODE_DISTRICT6/TB_MODE_DISTRICT6_DATE 表查询, Start_Type 处理
- CWorldMode::Update @ 0x140047990: nState==0->StartMode, nState==1->FinishMode, nState==2->ClearMode
- CWorldMode::StartMode @ 0x140047AF0: nState=1, XSendPacket(0xFB,1), SendDBGame(0x49,7)
- CWorldMode::FinishMode @ 0x140047E10: nState=2, PS_WORLD_MODE_FINISH 填充, XSendPacket(0xFB,4)
- CWorldMode::ReqModeComplete @ 0x1400485F0: bFinish->m_bReserveFinish=1, m_bSuccess=1
- 状态机: nState 0=idle, 1=running, 2=completed
- 时间计算: TB_MODE_DISTRICT6_DATE Day_Type 匹配, Start_Time/End_Time 解析


---

### XControlServer GM/MyRoom 方法验证追加 (2026-04-30 15:08 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::GM_Notice | 0x1400110F0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_Shutdown | 0x140011200 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::MyRoomEnterReq | 0x140012050 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ReqCreateMyRoom | 0x1400147E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::DeleteMyRoomReq | 0x140014B10 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::DeleteMyRoomRes | 0x140014C30 | verified | IDA |

说明：
- XControlServer::GM_Notice @ 0x1400110F0: PS_CHAT_NOTICE 填充, SendChatNotice, XSendPacket(0xF8,2)
- XControlServer::GM_Shutdown @ 0x140011200: XSendPacket(0xF2,8), SendPacketAll, XSendPacket(0xF8,3)
- XControlServer::MyRoomEnterReq @ 0x140012050: FindMyRoom 判断, 不存在->CreateMyRoom, 存在->RoomEnterUser
- XControlServer::ReqCreateMyRoom @ 0x1400147E0: nResult==0->CreateSucc, 否则 DelMyRoom, XSendPacket(0xF2,0x51)
- XControlServer::DeleteMyRoomReq @ 0x140014B10: GetMyRoomState!=1 返回, DeleteReq, XSendPacket(0xF2,0x53)
- XControlServer::DeleteMyRoomRes @ 0x140014C30: GetMyRoomState!=2 返回, DelMyRoom, XSendPacket(0xF2,0x59)
- MyRoom 状态机: NONE(0) -> READY(1) -> DELETE(2)



---

### CChannelOfMap 方法验证追加 (2026-04-30 14:50 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldManager.cpp | CChannelOfMap::AddUserCount | 0x140001000 | verified | IDA |
| XControlServer | WorldManager.cpp | CChannelOfMap::AddSelectUser | 0x1400011C0 | verified | IDA |
| XControlServer | WorldManager.cpp | CChannelOfMap::ChangeChannelState | 0x140001270 | verified | IDA |
| XControlServer | WorldManager.cpp | CChannelOfMap::CalculateChannelState | 0x140001410 | verified | IDA |
| XControlServer | WorldManager.cpp | CChannelOfMap::IsOKToEnter | 0x140001600 | verified | IDA |
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::GetServerState | 0x14000A0B0 | verified | IDA |

说明：
- CChannelOfMap::AddUserCount @ 0x140001000: m_nUserCount+=nAdd, m_mapSelectUser 操作, ChangeChannelState
- CChannelOfMap::AddSelectUser @ 0x1400011C0: m_mapSelectUser[nUserID] = GetTickCount64()+300000
- CChannelOfMap::ChangeChannelState @ 0x140001270: CalculateChannelState, bChange 则发送 0xF2/0x12
- CChannelOfMap::CalculateChannelState @ 0x140001410: 0/1/2/3 四状态基于 UserCount/MaxUserCount 比例
- 状态比例: 0(<40%), 1(40-65%), 2(65-90%), 3(>95%)


---

### CMapWithChannel 方法验证追加 (2026-04-30 14:50 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::CMapWithChannel | 0x140001620 | verified | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::AddChannel | 0x1400018B0 | verified | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::FindChannel | 0x140001E10 | verified | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::FindChannel | 0x140001FF0 | verified | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::DeleteChannel | 0x1400020A0 | verified | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::FindChannelByParty | 0x140005380 | verified | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::FindChannelByServerID | 0x140005630 | verified | IDA |

说明：
- CMapWithChannel::AddChannel @ 0x1400018B0: new CChannelOfMap, insert m_mapChannel, SendDBAddChannelMap
- CMapWithChannel::FindChannel @ 0x140001E10: FindChannelByParty/FindChannelByServerID/FindChannelByUserCount 顺序
- CMapWithChannel::DeleteChannel @ 0x1400020A0: 遍历 m_mapChannel, RemoveChannelOfMap, erase
- FindChannelByParty: byGroupType==1/2 调用 PartyManager/ForceManager.FindSamePlace
- FindChannelByServerID: m_mapChannelForServer 查询, CheckChannelUserCount


---

### CWorldManager 方法验证追加 (2026-04-30 14:50 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldManager.cpp | CWorldManager::CWorldManager | 0x140002710 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::AddMap | 0x1400027A0 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::DeleteMap | 0x140002B70 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::ReqEnterMap | 0x140002CA0 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::ReqChangeChannel | 0x140004720 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::SendChannelInfoAll | 0x140004F70 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::UpdateUserMap | 0x1400050E0 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::AddChannelOfMap | 0x140005230 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::RemoveChannelOfMap | 0x140005300 | verified | IDA |

说明：
- CWorldManager::CWorldManager @ 0x140002710: 初始化 CFSRWLock, m_mapMapInfo, m_mapChannelOfMap
- CWorldManager::AddMap @ 0x1400027A0: MapID/10000==2 则 Maze, 否则 CMapWithChannel::AddChannel
- CWorldManager::DeleteMap @ 0x140002B70: 遍历 m_mapMapInfo, CMapWithChannel::DeleteChannel
- CWorldManager::ReqEnterMap @ 0x140002CA0: GetUser, CanChangeWorld, FindChannel, SetWorldState, XSendPacket(0xF2,0x31)
- CWorldManager::ReqChangeChannel @ 0x140004720: FindChannel(nChannel), SetWorldState, SendPacket(0xF2,0x10)
- CWorldManager::SendChannelInfoAll @ 0x140004F70: 遍历 m_mapMapInfo, GetChannel, XSendPacket(0xF2,0x11)
- CWorldManager::UpdateUserMap @ 0x1400050E0: beforeMap AddUserCount(-1), AfterMap AddUserCount(+1)


---

### TXSingleton 方法验证追加 (2026-04-30 14:50 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore | TXSingleton.h | TXSingleton<XControlServer>::Instance | 0x140006100 | verified | IDA |

说明：
- TXSingleton<XControlServer>::Instance @ 0x140006100: 惰性单例, _pInstance==null 则 new XControlServer(0x20CF0)
- 单例模式：全局唯一 ControlServer 实例入口



---

### CMapWithChannel 方法验证追加 (2026-04-30 14:55 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::FindChannelByUserCount | 0x140005820 | verified | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::CheckChannelUserCount | 0x1400058E0 | verified | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::FindChannelUserLow | 0x140005A40 | verified | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::FindChannelUserLow | 0x140005D70 | verified | IDA |

说明：
- FindChannelByUserCount @ 0x140005820: FindChannelUserLow(bRaid) 返回用户数最少 Channel
- CheckChannelUserCount @ 0x1400058E0: FindChannel(nChannel), IsState==0 则 FindChannelUserLow
- FindChannelUserLow @ 0x140005A40: 遍历 pInfo->mapUXMapID, GetServerState==2, GetTotalCount 最小


---

### CWorldManager 方法验证追加 (2026-04-30 14:55 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldManager.cpp | CWorldManager::ReqEnterMapToOther | 0x140003D30 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::AddChannelOfMap | 0x140005230 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::RemoveChannelOfMap | 0x140005300 | verified | IDA |

说明：
- ReqEnterMapToOther @ 0x140003D30: GetUser, FindChannel(nChannel), AddSelectUser, XSendPacket(0xF2,0x54)
- AddChannelOfMap @ 0x140005230: m_mapChannelOfMap.find/insert
- RemoveChannelOfMap @ 0x140005300: m_mapChannelOfMap.find/erase


---

### CMazeInfo / CServer 方法验证追加 (2026-04-30 14:55 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetMemberVector | 0x140006690 | verified | IDA |
| XControlServer | CServer.cpp | CServer::AddMaze | 0x140041E60 | verified | IDA |
| XControlServer | CServer.cpp | CServer::RemoveMaze | 0x140041930 | verified | IDA |
| XControlServer | CServer.cpp | CServer::UpdateMaze | 0x1400422D0 | verified | IDA |
| XControlServer | CServer.cpp | CServer::SyncMaze | 0x140042340 | verified | IDA |

说明：
- CMazeInfo::GetMemberVector @ 0x140006690: 返回 m_vecMemberInfo 指针
- CServer::AddMaze @ 0x140041E60: m_mapMazeInfo[uxMapID] = pMazeInfo, XSendPacket(0xF2,0x04)
- CServer::RemoveMaze @ 0x140041930: ResetParentMaze/ResetChildMaze, m_mapMazeInfo.erase
- CServer::SyncMaze @ 0x140042340: 遍历 m_vecMemberInfo, 发送 0xF2/0x25, 处理最后包时 ++m_nSyncPacketCount


---

### CMyRoom 方法验证追加 (2026-04-30 14:57 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CMyRoom.cpp | CMyRoom::CMyRoom | 0x1400395F0 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::Init | 0x140039630 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::RoomEnterUser | 0x140039690 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CreateMyRoom | 0x140039820 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::EnterSucc | 0x1400398B0 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::DeleteReq | 0x140027140 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::GetMyRoomState | 0x140027150 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::GetUxMapID | 0x140027160 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::GetOwnerInfo | 0x140027190 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CreateSucc | 0x1400271E0 | verified | IDA |

说明：
- CMyRoom::CMyRoom @ 0x1400395F0: 构造函数初始化 m_stOwnerInfo, 调用 Init
- CMyRoom::Init @ 0x140039630: memset(this, 0, 0x44), m_pServer=nullptr, m_byState=0
- CMyRoom::RoomEnterUser @ 0x140039690: 状态检查 m_byState==1 则 XSendPacket(0xF2,0x50), 否则发送 0xF2/0x51+error(100)
- CMyRoom::CreateMyRoom @ 0x140039820: qmemcpy(this, stOwnerInfo, 68), UXMapID 复制, m_pServer=pServer, m_byState=0
- CMyRoom::EnterSucc @ 0x1400398B0: if (m_byState==2) m_byState=1; 允许 DELETE->READY 恢复
- CMyRoom::DeleteReq @ 0x140027140: m_byState = 2
- CMyRoom::GetMyRoomState @ 0x140027150: return m_byState
- CMyRoom::GetUxMapID @ 0x140027160: 返回 m_uxMapID
- CMyRoom::GetOwnerInfo @ 0x140027190: qmemcpy(result, this, 68)
- CMyRoom::CreateSucc @ 0x1400271E0: m_byState = 1

状态机：NONE(0) -> READY(1) -> DELETE(2), EnterSucc 允许 DELETE->READY 恢复


---

### CServer MyRoom 方法验证追加 (2026-04-30 14:57 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServer::CreateMyRoom | 0x1400427C0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServer::FindMyRoom | 0x140042A30 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServer::DelMyRoom | 0x140042A00 | verified | IDA |

说明：
- CServer::CreateMyRoom @ 0x1400427C0: GetMapID, new CMyRoom(96), insert m_mpMyRoomInfo, XSendPacket(0xF2,0x52)
- CServer::FindMyRoom @ 0x140042A30: m_mpMyRoomInfo.find(dwUAID), 返回 shared_ptr
- CServer::DelMyRoom @ 0x140042A00: m_mpMyRoomInfo.erase(dwUAID)


---

### XControlServer MyRoom 方法验证追加 (2026-04-30 14:57 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::MyRoomEnterReq | 0x140012050 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ReqCreateMyRoom | 0x1400147E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::DeleteMyRoomReq | 0x140014B10 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::DeleteMyRoomRes | 0x140014C30 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::FindMyRoom | 0x140011EE0 | verified | IDA |

说明：
- XControlServer::MyRoomEnterReq @ 0x140012050: FindMyRoom 不存在则 CreateMyRoom, 存在则 RoomEnterUser
- XControlServer::ReqCreateMyRoom @ 0x1400147E0: FindMyRoom, nResult!=0 则 DelMyRoom+error(58203), 否则 CreateSucc+SendEnterMapRes
- XControlServer::DeleteMyRoomReq @ 0x140014B10: FindMyRoom, GetMyRoomState==1 则 DeleteReq, XSendPacket(0xF2,0x53)
- XControlServer::DeleteMyRoomRes @ 0x140014C30: FindMyRoom, GetMyRoomState==2 则 DelMyRoom, XSendPacket(0xF2,0x59)


---

### CRouletteEventMgr 方法验证追加 (2026-04-30 14:58 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::CRouletteEventMgr | 0x14003A4C0 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::~CRouletteEventMgr | 0x14003A530 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::Update | 0x14003AC10 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::LoadRouletteEvent | 0x14003A5A0 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::SendRouletteEvent | 0x14003AD30 | verified | IDA |
| XControlServer | CRouletteEventMgr.cpp | CRouletteEventMgr::UpdateRouletteItemInfo | 0x14003ADE0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::UpdateRouletteEventInfo | 0x140015790 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::ResRouletteEventRewardLoad | 0x140031AA0 | verified | IDA |

说明：
- CRouletteEventMgr::CRouletteEventMgr @ 0x14003A4C0: CFSRWLock + PS_GM_ROULETTE_EVENT + Clear()
- CRouletteEventMgr::~CRouletteEventMgr @ 0x14003A530: Clear() + 析构 PS_GM_ROULETTE_EVENT
- CRouletteEventMgr::Update @ 0x14003AC10: 60秒 tick, 首次 LoadRouletteEvent + SendDBRouletteRewardInfo
- CRouletteEventMgr::LoadRouletteEvent @ 0x14003A5A0: 版本检查, m_bLoad ? CheckItemDayCount : operator=
- CRouletteEventMgr::SendRouletteEvent @ 0x14003AD30: XSendPacket(0xF2, 0x74), SendPacketAll
- CRouletteEventMgr::UpdateRouletteItemInfo @ 0x14003ADE0: 按匹配更新 nCurCount, SendRouletteEvent(false)


---

### CDayEventMgr 方法验证追加 (2026-04-30 14:58 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | DayEventManager.cpp | CDayEventMgr::CDayEventMgr | 0x14002D8B0 | verified | IDA |
| XControlServer | DayEventManager.cpp | CDayEventMgr::~CDayEventMgr | 0x14002D910 | verified | IDA |
| XControlServer | DayEventManager.cpp | CDayEventMgr::Clear | 0x14002D950 | verified | IDA |
| XControlServer | DayEventManager.cpp | CDayEventMgr::Update | 0x14002D9A0 | verified | IDA |
| XControlServer | DayEventManager.cpp | CDayEventMgr::GetDayEvent | 0x14002DA90 | verified | IDA |
| XControlServer | DayEventManager.cpp | CDayEventMgr::SetDayEvent | 0x14002DAF0 | verified | IDA |
| XControlServer | DayEventManager.cpp | CDayEventMgr::SendDayEventAllGameServer | 0x14002E4D0 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::GetDayEvent | 0x1400A3F00 | verified | IDA |
| XCore | XResourceMgr.cpp | XResourceMgr::InitDayEvent | 0x1400A84B0 | verified | IDA |

说明：
- CDayEventMgr::CDayEventMgr @ 0x14002D8B0: ATL::CTime + CFSRWLock + map<unsigned short,ST_DAY_EVENT_INFO> + Clear()
- CDayEventMgr::Update @ 0x14002D9A0: 60秒 tick, 9点检查, SetDayEvent + SendDayEventAllGameServer
- CDayEventMgr::SetDayEvent @ 0x14002DAF0: 复杂事件配置逻辑, GetTB_DAY_EVENT, XItemFactory::nRand, random_shuffle
- CDayEventMgr::GetDayEvent @ 0x14002DA90: 返回 m_mapDayEvent
- CDayEventMgr::SendDayEventAllGameServer @ 0x14002E4D0: 广播事件信息


---

### CServer 迷宫管理验证追加 (2026-04-30 15:00 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServer::RegisterProcess | 0x140041180 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServer::OnUpdate | 0x140042550 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServer::GetMapID | 0x140041710 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServer::IsValidEnterPartyMemberInMaze | 0x140041C80 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServer::UpdateMaze | 0x1400421E0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServer::SyncMaze | 0x140042340 | verified | IDA |

说明：
- CServer::RegisterProcess @ 0x140041180: 注册 CServerProcess(0xF2) + CUserProcess(0xF3) + CGMToolProcess(0xF8) + CWorldModeProcess(0xFB)
- CServer::OnUpdate @ 0x140042550: 遍历 m_mapMazeInfo, IsDestroy 则发送 0xF2/0x25 并 RemoveMaze
- CServer::GetMapID @ 0x140041710: 组合 channel + wMapID + serial 成 UXMapID
- CServer::IsValidEnterPartyMemberInMaze @ 0x140041C80: 验证 PartyID + MapID + MemberVector
- CServer::UpdateMaze @ 0x1400421E0: find 或创建 CMazeInfo, UpdateMazeInfo
- CServer::SyncMaze @ 0x140042340: bLast 递增 m_nLogicThreadSyncCount, 否则 SyncMazeInfo


---

### XControlServer 核心方法验证追加 (2026-04-30 15:02 +08:00)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::XControlServer | 0x14000B6F0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::~XControlServer | 0x14000BA90 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::InitServer | 0x14000BD00 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::Clear | 0x14000C0C0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::OnUpdate | 0x14000F240 | verified | IDA |
| XControlServer | ControlServer.cpp | TXSingleton<XControlServer>::Instance | 0x140006100 | verified | IDA |

说明：
- XControlServer::XControlServer @ 0x14000B6F0: TXMultiPoolServer + boost::multi_index + ClassFactory<CMazeInfo> + m_factoryMaze + CWorldManager + XResourceMgr + CPartyManager + CWorldModeMgr + CDayEventMgr + CRouletteEventMgr
- XControlServer::InitServer @ 0x14000BD00: CLogThreadManager::Start + XResourceMgr::Init + XItemFactory::Init + XGameDBSocketMgr::Init + CObserveSocket::StartUp + CWorldModeMgr::Init
- XControlServer::OnUpdate @ 0x14000F240: 5秒 tick 遍历 m_mapMazeServer, 60秒 tick SendDBLog money/user, CWorldModeMgr::Update + CDayEventMgr::Update + CRouletteEventMgr::Update
- TXSingleton<XControlServer>::Instance @ 0x140006100: 静态 local static 一次性初始化


---

### CWorldManager 方法验证追加 (2026-04-30 15:04 +08:00)

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

说明：
- CWorldManager::CWorldManager @ 0x140002710: CFSRWLock + m_mapMapInfo + m_mapChannelOfMap
- CWorldManager::AddMap @ 0x1400027A0: MapID/10000==2 -> Maze (ClassFactory<CMazeInfo>::create + CServer::AddMaze), else CMapWithChannel::AddChannel
- CWorldManager::DeleteMap @ 0x140002B70: m_mapMapInfo.erase + m_mapChannelOfMap.erase
- CWorldManager::ReqEnterMap @ 0x140002CA0: GetUser, CanChangeWorld, FindChannel, AddSelectUser, SetWorldState, XSendPacket(0xF2,0x31)
- CWorldManager::ReqEnterMapToOther @ 0x140003D30: GetUser, FindChannel(nChannel), AddSelectUser, XSendPacket(0xF2,0x54)
- CWorldManager::ReqChangeChannel @ 0x140004720: 频道切换逻辑
- CWorldManager::SendChannelInfoAll @ 0x140004F70: 广播频道信息
- CWorldManager::UpdateUserMap @ 0x1400050E0: 更新用户地图信息


---

### CGameDBSocket 函数 (2026-04-30 15:13 反编译追加，待源码对比)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::CGameDBSocket | 0x1400313F0 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::~CGameDBSocket | 0x140031440 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::FindUser | 0x140031470 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::SetInfomation | 0x1400314A0 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::OnParse | 0x140031530 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::DBParse | 0x140031580 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::DBLoginParse | 0x140031620 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::DBEventParse | 0x1400316B0 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::ResAddServerGroupInfo | 0x140031720 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::ResServerGroupMaxUserCount | 0x1400317A0 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::ResCheckServerGroupID | 0x140031830 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::OnDisConnect | 0x1400318C0 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::OnNotConnect | 0x140031990 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::ResWorldModeLoad | 0x140031A30 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::ResRouletteEventRewardLoad | 0x140031AA0 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::WriteLog | 0x140031B90 | verified | IDA |

说明：
- CGameDBSocket 继承 TXDBSocket<CServer>
- m_byType=5 在构造函数中设置
- DBParse 分发: main=2 -> DBLoginParse, main=73 -> DBEventParse
- DBLoginParse 分发: sub='!' (0x21) -> ResAddServerGroupInfo, sub='"' (0x22) -> ResServerGroupMaxUserCount, sub='P' (0x50) -> ResCheckServerGroupID
- DBEventParse 分发: sub=6 -> ResWorldModeLoad, sub=45 -> ResRouletteEventRewardLoad
- OnDisConnect: m_byType==0 且非安全关闭时触发全局 Shutdown

---

### CObserveSocket 函数 (2026-04-30 15:13 反编译追加，待源码对比)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ObserveSocket.cpp | CObserveSocket::CObserveSocket | 0x140071DF0 | verified | IDA |
| XControlServer | ObserveSocket.cpp | CObserveSocket::~CObserveSocket | 0x14002ABB0 | verified | IDA |
| XControlServer | ObserveSocket.cpp | CObserveSocket::OnConnect | 0x140071680 | verified | IDA |
| XControlServer | ObserveSocket.cpp | CObserveSocket::OnDisConnect | 0x1400716A0 | verified | IDA |
| XControlServer | ObserveSocket.cpp | CObserveSocket::OnNotConnect | 0x1400716C0 | verified | IDA |
| XControlServer | ObserveSocket.cpp | CObserveSocket::StartUp | 0x1400716D0 | verified | IDA |
| XControlServer | ObserveSocket.cpp | CObserveSocket::SetMyInfo | 0x140071780 | verified | IDA |
| XControlServer | ObserveSocket.cpp | CObserveSocket::CalculateThreadStatus | 0x140071AA0 | verified | IDA |
| XControlServer | ObserveSocket.cpp | CObserveSocket::SendReportServerStatus | 0x140071EB0 | verified | IDA |
| XControlServer | ObserveSocket.cpp | CObserveSocket::OnUpdate | 0x140072020 | verified | IDA |

说明：
- CObserveSocket 继承 XRelaySocket
- m_observeInfo.szName = "OBSERVEAGENT"
- OnUpdate: 断线时每 10 秒重试连接，成功时每 3 秒发送状态报告
- SendReportServerStatus: 发送 SS_REPORT_SERVER_STATUS 包 (main=0xF2, sub=0x13)
- CalculateThreadStatus: 使用 SRWLock 保护，计算线程状态字符串

---

### XControlServer 服务器管理函数 (2026-04-30 15:13 反编译追加，待源码对比)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::KickoutUser_UseLock | 0x14000D780 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::KickoutUser_NoLock | 0x14000D7E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendPacketToLoginServer | 0x14000D950 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendPacketAll | 0x14000DA60 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::UpdateAccountState | 0x14000F8B0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ClearUserState | 0x14000F960 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GetServer | 0x14000FB10 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SendChatNotice | 0x14000FBC0 | verified | IDA |

说明：
- KickoutUser_UseLock: 写锁 + KickoutUser_NoLock
- KickoutUser_NoLock: 从 m_UserInfos 删除 + SendPacketAll(0xF3, 7)
- UpdateAccountState: XSendDBPacket(main=2, sub=0x13) + SendDBAccount
- ClearUserState: 按 ServerID 遍历用户 + 发送 PS_REQ_CLEAR_USER_STATE


---

### CMyRoom 函数 (2026-05-01 04:25 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CMyRoom.cpp | CMyRoom::CMyRoom | 0x1400395F0 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::Init | 0x140039630 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::RoomEnterUser | 0x140039690 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CreateMyRoom | 0x140039820 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::EnterSucc | 0x1400398B0 | verified | IDA |

### CForce / CParty 函数 (2026-05-01 04:25 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CForce.cpp | CForce::SetMember | 0x1400398D0 | verified | IDA |
| XControlServer | CForce.cpp | CForce::IsEmpty | 0x140039980 | verified | IDA |
| XControlServer | CForce.cpp | CForce::SetMazeID | 0x14003A420 | verified | IDA |
| XControlServer | CForce.cpp | CForce::GetMazeID | 0x140039CD0 | verified | IDA |

### CPartyManager / CForceManager 函数 (2026-05-01 04:25 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CPartyManager.cpp | CPartyManager::SetMember | 0x1400399A0 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::SetMazeID | 0x140039C00 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::IsParty | 0x140030E80 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::FindSamePlace | 0x140030FE0 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::SetMember | 0x140030CF0 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::RemoveMember | 0x140039B30 | verified | IDA |

### XGameDBSocketMgr 函数 (2026-05-01 04:25 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore/XServer | XGameDBSocketMgr.cpp | XGameDBSocketMgr::IsAccountDBConnectionAll | 0x140034410 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.cpp | XGameDBSocketMgr::SendAccountDBAgent | 0x140034290 | verified | IDA |

### CServerProcess 函数 (2026-05-01 04:35 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServerProcess::CServerProcess | 0x14003C9D0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::Parse | 0x14003CAC0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateServer | 0x14003D0A0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMaze | 0x14003D1F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResCreateMaze | 0x14003D360 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncRemoveMaze | 0x14003D410 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncUpdateMaze | 0x14003D480 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncMaze | 0x14003D4F0 | verified | IDA |

### CUserProcess 函数 (2026-05-01 04:35 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserProcess.cpp | CUserProcess::CUserProcess | 0x140044FF0 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::Parse | 0x1400450E0 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::SyncSelectCharacter | 0x140045310 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess::SyncLogoutUser | 0x1400454B0 | verified | IDA |

### CMazeInfo 函数 (2026-05-01 04:40 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | MazeInfo.cpp | CMazeInfo::CMazeInfo | 0x140035F10 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::~CMazeInfo | 0x140036070 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::Init (ST_CREATE_MAZE) | 0x1400360D0 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::UpdateMazeInfo | 0x140036560 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::SyncMazeInfo | 0x140036820 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::IsDestroy | 0x140036D70 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::IsEnterMember | 0x140036DA0 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::SetDisconnectUserState | 0x140036E20 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::IsValidEnterMaze | 0x140036C10 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetMazeInfo | 0x1400370A0 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetMazeState | 0x140039480 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetUserCount | 0x140039490 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetChildMaze | 0x1400394B0 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetMazeType | 0x140044890 | verified | IDA |
| XControlServer | MazeInfo.cpp | CMazeInfo::GetMapID | 0x1400448B0 | verified | IDA |

### CGMToolProcess 函数 (2026-05-01 04:55 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::CGMToolProcess | 0x140034550 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::Parse | 0x140034640 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMUserKick | 0x140034730 | verified | IDA |

### CWorldModeProcess 函数 (2026-05-01 04:55 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::CWorldModeProcess | 0x14004D500 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::Parse | 0x14004D5F0 | verified | IDA |

### XControlServer AddUser/RemoveUser 函数 (2026-05-01 04:35 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::AddUser | 0x14000C220 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::RemoveUser | 0x14000C610 | verified | IDA |


### CGameDBSocket 函数 (2026-05-01 04:38 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::CGameDBSocket | 0x1400313F0 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::~CGameDBSocket | 0x140031440 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::DBParse | 0x140031580 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::DBLoginParse | 0x140031620 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::DBEventParse | 0x1400316B0 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::ResAddServerGroupInfo | 0x140031720 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::ResServerGroupMaxUserCount | 0x1400317A0 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::ResCheckServerGroupID | 0x140031830 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::OnDisConnect | 0x1400318C0 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::OnNotConnect | 0x140031990 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::ResWorldModeLoad | 0x140031A30 | verified | IDA |
| XControlServer | GameDBSocket.cpp | CGameDBSocket::ResRouletteEventRewardLoad | 0x140031AA0 | verified | IDA |

### XGameDBSocketMgr 扩展函数 (2026-05-01 04:38 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XCore/XServer | XGameDBSocketMgr.cpp | XGameDBSocketMgr::XGameDBSocketMgr | 0x140032D90 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.cpp | XGameDBSocketMgr::~XGameDBSocketMgr | 0x140032E30 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.cpp | XGameDBSocketMgr::Init | 0x140032E50 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.cpp | XGameDBSocketMgr::AutoConnect | 0x140033C30 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.cpp | XGameDBSocketMgr::DisConnect | 0x140033EA0 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.cpp | XGameDBSocketMgr::SendGameDBAgent | 0x140034310 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.cpp | XGameDBSocketMgr::SendLogDBAgent | 0x140034390 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.cpp | XGameDBSocketMgr::IsLogDBConnectionAll | 0x140034490 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.cpp | XGameDBSocketMgr::GetAccountDBAgentCount | 0x140027590 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.cpp | XGameDBSocketMgr::GetGameDBAgentCount | 0x140027580 | verified | IDA |
| XCore/XServer | XGameDBSocketMgr.cpp | XGameDBSocketMgr::GetLogDBAgentCount | 0x140027570 | verified | IDA |

### CGMToolProcess 扩展函数 (2026-05-01 04:38 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMTimeEvent | 0x1400348B0 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMValueEvent | 0x140034910 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMServerOption | 0x1400349A0 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess::ReqGMCashShopBanner | 0x140034A10 | verified | IDA |

### CWorldModeProcess 扩展函数 (2026-05-01 04:38 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeCommand | 0x14004D680 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeComplete | 0x14004DAF0 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeUpdate | 0x14004DC20 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess::ReqWorldModeEnterList | 0x14004DC90 | verified | IDA |

### CWorldModeMgr 核心函数 (2026-05-01 04:38 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::LoadMode | 0x1400498A0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::Update | 0x14004A050 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::ReqModeComplete | 0x14004ABC0 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::UpdateMonsterCount | 0x14004B730 | verified | IDA |

### CServerProcess 函数 (2026-05-01 07:44 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.cpp | CServerProcess::Parse | 0x14003CAC0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateServer | 0x14003D0A0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqUpdateServerInfo | 0x14003D1A0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMaze | 0x14003D1F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResCreateMaze | 0x14003D360 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncMaze | 0x14003D4F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncRemoveMaze | 0x14003D410 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncUpdateMaze | 0x14003D480 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMap | 0x14003D5D0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqEnterMap | 0x14003D6E0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncLogicThreadCount | 0x14003DBF0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqDisconnectUserSync | 0x14003DCA0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::SyncMaxMazeID | 0x14003DD10 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResCreateMatchingMazeFromGame | 0x14003DFF0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqFindUser | 0x14003CEE0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::PartyMazeSync | 0x14003E9F0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ForceMazeSync | 0x14003EA60 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqPostSend | 0x14003EAD0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMatchingMazeFromCommunity | 0x14003DD80 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCheckPartyInMaze | 0x14003D8C0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResCreateModeMaze | 0x14003EFC0 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::EnterOtherMap_cheat | 0x14003D750 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqReEnterMap | 0x14003EE20 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomCreate | 0x14003ED40 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomDelete | 0x14003EE80 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterReq | 0x14003E460 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqMyRoomEnterRes | 0x14003E680 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ResMyRoomDelete | 0x14003EF20 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqUpdateRouletteEvent | 0x14003F070 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqChangeChannel | 0x14003D130 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCheckEnterMaze | 0x14003DC40 | verified | IDA |
| XControlServer | ServerProcess.cpp | CServerProcess::ReqCreateMatchingModeMazeFromCommunity | 0x14003DF20 | verified | IDA |

### CUserProcess 函数 (2026-05-01 07:44 验证)

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

### CGMToolProcess 完整函数 (2026-05-01 07:44 验证)

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

### CServer 函数 (2026-05-01 09:23 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CServer.cpp | CServer::Init | 0x140041320 | verified | IDA |
| XControlServer | CServer.cpp | CServer::SetServerInfo | 0x140041590 | verified | IDA |
| XControlServer | CServer.cpp | CServer::GetMapID | 0x1400417C0 | verified | IDA |
| XControlServer | CServer.cpp | CServer::RemoveMaze | 0x140041930 | verified | IDA |
| XControlServer | CServer.cpp | CServer::SyncMaze | 0x140042340 | verified | IDA |
| XControlServer | CServer.cpp | CServer::UpdateMaze | 0x1400421E0 | verified | IDA |

### XControlServer 用户管理函数 (2026-05-01 09:23 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::AddUser | 0x14000C220 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::RemoveUser | 0x14000C610 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ReqCreateMaze | 0x14000E3A0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GetServer | 0x14000FB10 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::FindServerFromMaze | 0x1400115F0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CheckSessionID | 0x140014590 | verified | IDA |

### CWorldModeMgr 函数 (2026-05-01 09:23 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::Init | 0x140049840 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::Update | 0x14004A050 | verified | IDA |
| XControlServer | WorldModeMgr.cpp | CWorldModeMgr::SendModeSync | 0x14004A630 | verified | IDA |

### CWorldMode 函数 (2026-05-01 09:23 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldMode.cpp | CWorldMode::Init | 0x140046CD0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::Update | 0x140047990 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::StartMode | 0x140047AF0 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::ClearMode | 0x140048350 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::GetEventTime | 0x140048B50 | verified | IDA |
| XControlServer | WorldMode.cpp | CWorldMode::AddMonsterCount | 0x140048BD0 | verified | IDA |

### XControlServer GM函数 (2026-05-01 17:10 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::GM_UserKick | 0x140010FF0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_Notice | 0x1400110F0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_Shutdown | 0x140011200 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_TimeEvent | 0x1400112E0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_ValueEvent | 0x140011380 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_ServerOption | 0x140011420 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GM_CashShopBanner | 0x140011550 | verified | IDA |

### XControlServer MyRoom函数 (2026-05-01 17:12 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::FindServerFromMaze | 0x1400115F0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GetMyRoomServer | 0x140011E00 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::MyRoomEnterReq | 0x140012050 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::AddMyRoomServerInfo | 0x140011CB0 | verified | IDA |

### CWorldManager 函数 (2026-05-01 17:14 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldManager.cpp | CWorldManager::AddMap | 0x1400027A0 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::DeleteMap | 0x140002B70 | verified | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::ReqEnterMap | 0x140002CA0 | verified | IDA |

### CParty/CPartyManager 函数 (2026-05-01 17:16 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CParty.cpp | CParty::RemoveMember | 0x140030B00 | verified | IDA |
| XControlServer | CParty.cpp | CParty::IsFull | 0x140030B80 | verified | IDA |
| XControlServer | CParty.cpp | CParty::FindSamePlace | 0x140030BC0 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::CPartyManager | 0x140030CA0 | verified | IDA |
| XControlServer | CPartyManager.cpp | CPartyManager::IsParty | 0x140030E80 | verified | IDA |

### CForce/CForceManager 函数 (2026-05-01 17:18 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CForceManager.cpp | CForceManager::SetMember | 0x140030CF0 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::IsFull | 0x140030EE0 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::SetMazeID | 0x140030F60 | verified | IDA |
| XControlServer | CForceManager.cpp | CForceManager::RemoveMember | 0x140039B30 | verified | IDA |
| XControlServer | CForce.cpp | CForce::SetMember | 0x1400398D0 | verified | IDA |

### CServer 基础函数 (2026-05-01 17:20 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CServer.cpp | CServer::GetServerState | 0x14000A090 | verified | IDA |
| XControlServer | CServer.cpp | CServer::GetServerInfo | 0x14000A4E0 | verified | IDA |
| XControlServer | CServer.cpp | CServer::GetServerID | 0x14000B2B0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::GetServer | 0x14000FB10 | verified | IDA |
| XControlServer | WorldManager.cpp | CChannelOfMap::CChannelOfMap | 0x14000B2D0 | verified | IDA |

### CMyRoom 函数 (2026-05-01 17:22 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CMyRoom.cpp | CMyRoom::CMyRoom | 0x1400395F0 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::Init | 0x140039630 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::RoomEnterUser | 0x140039690 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CreateMyRoom | 0x140039820 | verified | IDA |
| XControlServer | CMyRoom.cpp | CMyRoom::CreateSucc | 0x1400271E0 | verified | IDA |

### XControlServer 业务函数 (2026-05-01 17:24 验证)

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | XControlServer::CheckPartyInMaze | 0x14000FD50 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::CheckForceInMaze | 0x1400100B0 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::ChangeServer | 0x140010410 | verified | IDA |
| XControlServer | ControlServer.cpp | XControlServer::SetUsersInfo | 0x140010740 | verified | IDA |

## 累计验证统计 (2026-05-01 17:24)

- 本轮新增验证: ~40 函数
- 累计验证函数: 290+
- 验证类覆盖: 17 个核心类
