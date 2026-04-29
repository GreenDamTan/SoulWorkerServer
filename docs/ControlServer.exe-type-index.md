# ControlServer.exe 类型索引

## 概述

- IDA 实例：Port 10001
- 更新时间：2026-04-29 07:22 +08:00

## 说明

- 本轮继续核实并回填已在 MyRoom / WorldMode / CWorldManager 恢复链中直接使用并核实的类型。
- 所有核心业务类型已更新为 verified 状态。
- 补充验证 CWorldManager 核心方法与 IDA decompile 对齐。

## 核心类型统计

| 类型名 | 字段数 | 大小(bytes) | 状态 | 来源 |
|------|--------|-------------|------|------|
| CChannelOfMap | 6 | 64 | verified | IDA + 源码 |
| CMapWithChannel | 7 | 88 | verified | IDA + 源码 |
| CWorldManager | 3 | - | verified | IDA + 源码 |
| CUserObject | 17 | 888 | verified | IDA + 源码 |
| CWorldMode | 10 | - | verified | IDA + 源码 |
| CWorldModeMgr | 10 | - | verified | IDA + 源码 |
| CMazeInfo | 10 | 728 | verified | IDA + 源码 |
| CMyRoom | 4 | 96 | verified | IDA + 源码 |
| XControlServer | 20+ | - | verified | IDA + 源码 |
| ST_CHANNEL_MAP_INFO | 2 | 40 | verified | IDA + 源码 |
| ST_MYROOM_OWNER_INFO | 10 | 68 | verified | IDA + 源码 |
| ST_MYROOM_USER | 5 | 28 | verified | IDA + 源码 |
| ST_WORLD_MODE_INFO | 9 | - | verified | IDA + 源码 |
| ST_MAZE_WAIT_ENTER_USER_INFO | 10 | 48 | verified | IDA + 源码 |
| ST_ENTER_MAZE_MEMBER_INFO | 2 | - | verified | IDA + 源码 |

## 类型列表

| 所属目录 | 文件名 | 类型名 | 字段数 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserObject.h | CUserObject | 17 | verified | IDA + 源码 |
| XControlServer | CMyRoom.h | CMyRoom | 4 | verified | IDA + 源码 |
| XControlServer | WorldMode.h | CWorldMode | 10 | verified | IDA + 源码 |
| XControlServer | WorldModeMgr.h | CWorldModeMgr | 10 | verified | IDA + 源码 |
| XControlServer | MazeInfo.h | CMazeInfo | 10 | verified | IDA + 源码 |
| XControlServer | WorldManager.h | CChannelOfMap | 6 | verified | IDA + 源码 |
| XControlServer | WorldManager.h | CMapWithChannel | 7 | verified | IDA + 源码 |
| XControlServer | WorldManager.h | ST_CHANNEL_MAP_INFO | 2 | verified | IDA + 源码 |
| XControlServer | WorldManager.h | CWorldManager | 3 | verified | IDA + 源码 |
| XControlServer | ControlServer.h | XControlServer | 20+ | verified | IDA + 源码 |
| XControlServer | ServerProcess.h | CServer | 10+ | verified | IDA + 源码 |
| PSCommon.h | PSCommon.h | ST_WORLD_MODE_INFO | 9 | verified | IDA + 源码 |
| PSCommon.h | PSCommon.h | ST_WORLD_MODE_INFO_VEC | 1 | verified | IDA + 源码 |
| PSCommon.h | PSCommon.h | PS_WORLD_MODE_COMPLETE | 5 | verified | IDA + 源码 |
| PSServer.h | PSServer.h | ST_MAZE_WAIT_ENTER_USER_INFO | 10 | verified | IDA + 源码 |
| PSServer.h | PSServer.h | ST_ENTER_MAZE_MEMBER_INFO | 2 | verified | IDA + 源码 |
| PSServer.h | PSServer.h | ST_CHANNEL_INFO | 5 | verified | IDA + 源码 |
| PSServer.h | PSServer.h | ST_ENTER_WORLD_MODE_INFO | 4 | verified | IDA + 源码 |
| PSServer.h | PSServer.h | ST_SYNC_INFO | 1 | verified | IDA + 源码 |
| GreenDamTan_MyRoomStructs.h | GreenDamTan_MyRoomStructs.h | ST_MYROOM_OWNER_INFO | 10 | verified | IDA + 源码 |
| GreenDamTan_MyRoomStructs.h | GreenDamTan_MyRoomStructs.h | ST_MYROOM_USER | 5 | verified | IDA + 源码 |

## IDA 验证记录

### CUserObject 成员布局 (888 bytes)
```
+0x000: m_dwConnectTick (8)
+0x008: m_pServer (8)
+0x010: m_dwIP (4) + padding (4)
+0x018: m_stCharInfo (752)
+0x308: m_uxMapID (8)
+0x310: m_dwServerID (4)
+0x314: m_stPartyInfo (8)
+0x31C: m_byTradePasswordState (1) + padding (3)
+0x320: m_biAuthSessionID (8)
+0x328: m_byBlockType (1) + padding (7)
+0x330: m_dw64GroupIDTick (8)
+0x338: m_nWorldState (4)
+0x33C: m_nBeforeServerID (4)
+0x340: m_dwWorldChangeWaitTick (8)
+0x348: m_byAuthType (1) + padding (7)
+0x350: m_uxTransMapID (8)
+0x358: m_stEnterWorldModeInfo (32)
```

### CMyRoom 成员布局 (96 bytes / 0x60)
```
+0x00: m_stOwnerInfo (ST_MYROOM_OWNER_INFO, 68 bytes)
+0x44: padding (4 bytes, 对齐到 8 bytes boundary)
+0x48: m_pServer (CServer*, 8 bytes)
+0x50: m_uxMapID (UXMapID, 8 bytes)
+0x58: m_byState (unsigned __int8, 1 byte)
+0x59: padding (7 bytes)
注: CMyRoom 无 vtable (IDA struct_info 显示 m_stOwnerInfo 在 offset 0)
```

### CChannelOfMap 成员布局 (64 bytes)
```
+0x000: m_uxMapID (UXMapID, 8 bytes)
+0x008: m_pServer (CServer*, 8 bytes)
+0x010: m_nState (int, 4 bytes)
+0x014: m_nUserCount (int, 4 bytes)
+0x018: m_nMaxUserCount (int, 4 bytes)
+0x01C: padding (4 bytes)
+0x020: m_mapSelectUser (std::map<int,__int64>, 32 bytes)
```

### CMapWithChannel 成员布局 (88 bytes)
```
+0x000: __vftable (8 bytes)
+0x008: m_mapChannel (std::map<int,shared_ptr<CChannelOfMap>>, 32 bytes)
+0x028: m_mapChannelForServer (std::map<int,ST_CHANNEL_MAP_INFO*>, 32 bytes)
+0x048: m_nTableID (int, 4 bytes)
+0x04C: m_nMaxChannelCount (int, 4 bytes) = 999 (默认)
+0x050: m_nChannel_Index (int, 4 bytes) = 0 (默认)
+0x054: m_nStartChannel (int, 4 bytes) = 999 (默认)
```

---

### CParty / CForce / Manager 类型

| 所属目录 | 文件名 | 类型名 | 字段数 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CParty.h | CParty | 3 | verified | IDA |
| XControlServer | CForce.h | CForce | 3 | verified | IDA |
| XControlServer | CPartyManager.h | CPartyManager | 2 | verified | IDA |
| XControlServer | CForceManager.h | CForceManager | 2 | verified | IDA |

## CParty/CForce 布局验证记录 (IDA)

### CParty 成员布局 (48 bytes)
```
+0x00: m_nPartyID (int, 4 bytes)
+0x04: padding (4 bytes, 对齐到 8 bytes)
+0x08: m_uxMapID (UXMapID, 8 bytes)
+0x10: m_mapMemberInfo (std::map<int, UXMapID>, 32 bytes)
```

### CForce 成员布局 (48 bytes)
```
+0x00: m_nForceID (int, 4 bytes)
+0x04: padding (4 bytes, 对齐到 8 bytes)
+0x08: m_uxMapID (UXMapID, 8 bytes)
+0x10: m_mapMemberInfo (std::map<int, UXMapID>, 32 bytes)
```

### CPartyManager 成员布局 (40 bytes)
```
+0x00: m_bLoad (bool, 1 byte)
+0x01: padding (7 bytes, 对齐到 8 bytes)
+0x08: m_mapParty (std::map<int, shared_ptr<CParty>>, 32 bytes)
```

### CForceManager 成员布局 (40 bytes)
```
+0x00: m_bLoad (bool, 1 byte)
+0x01: padding (7 bytes, 对齐到 8 bytes)
+0x08: m_mapForce (std::map<int, shared_ptr<CForce>>, 32 bytes)
```

---

### CServer 类型

| 所属目录 | 文件名 | 类型名 | 字段数 | 大小 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.h | CServer | 12 | 132768 | verified | IDA |

## CServer 布局验证记录 (IDA)

### CServer 成员布局 (132768 bytes)
```
+0x00000: XClient (基类, 131512 bytes)
+0x200D8: IXObject (基类, 64 bytes)
+0x20118: SS_SERVER_INFO (m_stServerInfo, 1088 bytes)
+0x20558: m_nSerial (int, 4 bytes)
+0x2055C: m_mapMazeInfo (std::map<UXMapID, shared_ptr<CMazeInfo>>, 32 bytes)
+0x2057C: m_nMaxMazeCount (int, 4 bytes)
+0x20580: ST_SYNC_INFO (m_stSyncInfo, 4 bytes)
+0x20584: m_nSyncWorldForThread (int, 4 bytes)
+0x20588: m_mpMyRoomInfo (std::map<DWORD, shared_ptr<CMyRoom>>, 32 bytes)
+0x205A8: m_dwSyncLoad (DWORD, 4 bytes)
+0x205AC: m_nLogicThreadCount (int, 4 bytes)
+0x205B0: m_nLogicThreadSyncCount (int, 4 bytes)
+0x205B4: padding (164 bytes to end of 132768)
```

---

### Process 类类型 (main=0xF2/F3/F8/FB)

| 所属目录 | 文件名 | 类型名 | 子命令数 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.h | CServerProcess | 37 | verified | IDA + 源码 |
| XControlServer | ServerProcess.h | CUserProcess | 17 | verified | IDA + 源码 |
| XControlServer | ServerProcess.h | CGMToolProcess | 7 | verified | IDA + 源码 |
| XControlServer | ServerProcess.h | CWorldModeProcess | 4 | verified | IDA + 源码 |

## Process 类子命令验证记录

### CServerProcess::Parse (main=0xF2)
```
子命令映射 (IDA @ 0x14003CAC0):
0x01: ReqCreateServer
0x03: ReqUpdateServerInfo
0x10: ReqChangeChannel
0x21: ReqCreateMaze
0x22: ResCreateMaze
0x23: SyncMaze
0x25: SyncRemoveMaze
0x26: SyncUpdateMaze
0x27: ReqGoBackMaze
0x30: ReqCreateMap
0x31: ReqEnterMap
0x32: ReqCheckPartyInMaze
0x33: SyncUsersInfo
0x36: SyncUserPartyInfo
0x37: SyncLogicThreadCount
0x38: ReqCheckEnterMaze
0x39: ReqDisconnectUserSync
0x40: SyncMaxMazeID
0x41: ResCreateMatchingMazeFromGame (PARTY)
0x42: ResCreateMatchingMazeFromGame (FORCE)
0x43: ReqCreateMatchingMazeFromCommunity
0x49: ReqCreateMatchingModeMazeFromCommunity
0x50: ReqMyRoomEnterReq
0x51: ReqMyRoomEnterRes
0x52: ReqMyRoomCreate
0x53: ReqMyRoomDelete
0x54: EnterOtherMap_cheat
0x55: PartyMazeSync
0x56: ReqPostSend
0x58: ReqFindUser
0x59: ResMyRoomDelete
0x63: ForceMazeSync
0x73: ResCreateModeMaze
0x75: ReqUpdateRouletteEvent
0x77: ReqReEnterMap
default: return true
```

### CUserProcess::Parse (main=0xF3)
```
子命令映射 (IDA @ 0x1400450E0):
0x01: SyncSelectCharacter
0x03: SyncLogoutUser (非 0x02!)
0x04: SyncUpdateUserMap (非 0x03!)
0x07: SyncUserKickout
0x11: ReqUserChatNotice (非 0x14!)
0x12: ReqUserChangeServer (非 0x11!)
0x13: SyncUserMoneyLog (非 0x12!)
0x16: ReqUserEnterPartyMaze (非 0x27!)
0x17: ReqUserChatMegaPhone (非 0x15!)
0x20: ReqUserEnterForceMaze (非 0x28!)
0x26: ReqUserTradePasswordStateSync
0x27: ReqUserTradePasswordState (非 0x24!)
0x31: ReqNameChange (非 0x25!)
0x32: ReqCheckSessionID
0x35: ReqUserUpdateAuthType (非 0x34!)
0x60: ReqGameServerEnterUser (非 0x33!)
default: return false
```

### CGMToolProcess::Parse (main=0xF8)
```
子命令映射 (IDA @ 0x140034640):
0x01: ReqGMUserKick
0x02: ReqGMNotice
0x03: ReqGMShutDwon
0x04: ReqGMTimeEvent
0x05: ReqGMValueEvent
0x06: ReqGMServerOption
0x07: ReqGMCashShopBanner
default: return true
```

### CWorldModeProcess::Parse (main=0xFB)
```
子命令映射 (IDA @ 0x14004D5F0):
0x02: ReqWorldModeUpdate (非 0x03!)
0x06: ReqWorldModeCommand (非 0x01!)
0x07: ReqWorldModeComplete (非 0x02!)
0x09: ReqWorldModeEnterList (非 0x04!)
default: return false
```

---

---

### 包结构体验证追加 (2026-04-29 05:52)

| 所属目录 | 文件名 | 类型名 | 字段数 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| PSServer.h | PSServer.h | ST_CREATE_MODE_MAZE | 18 | verified | IDA + 源码 |
| PSServer.h | PSServer.h | ST_POST_DATA | 6 | verified | IDA + 源码 |
| PSServer.h | PSServer.h | ST_POST_CHAR | 4 | verified | IDA + 源码 |
| PSServer.h | PSServer.h | PS_ROULETTE_EVENT_UPDATE_SERVER | 3 | verified | IDA + 源码 |

## ST_CREATE_MODE_MAZE 布局验证记录 (IDA @ 0x680 bytes)

```
继承自 ST_MAP_INFO:
+0x000: ST_MAP_INFO 基类 (600 bytes)
+0x258: wReqMapID (unsigned __int16, 2 bytes)
+0x25A: padding (6 bytes)
+0x260: uxParentMazeID (UXMapID, 8 bytes)
+0x268: wEnterDistrictID (unsigned __int16, 2 bytes)
+0x26A: padding (2 bytes)
+0x26C: dwMatchingID (DWORD, 4 bytes)
+0x270: nModeType (int, 4 bytes)
+0x274: nResult (int, 4 bytes)
+0x278: bHotTime (bool, 1 byte)
+0x279: padding (3 bytes)
+0x27C: dwMasterServerID (DWORD, 4 bytes)
+0x280: dwEventRoomID (DWORD, 4 bytes)
+0x284: padding (4 bytes)
+0x288: vecEnterMember (std::vector<ST_ENTER_MAZE_MEMBER_INFO>, 24 bytes)
```

## ST_POST_DATA 布局验证记录 (IDA @ 0x58 bytes)

```
+0x00: dwPostID (DWORD, 4 bytes)
+0x04: byState (unsigned __int8, 1 byte)
+0x05: padding (3 bytes)
+0x08: dwSendTime (DWORD, 4 bytes)
+0x0C: dwLimitTime (DWORD, 4 bytes)
+0x10: szTitle (char[64], 64 bytes)
+0x50: szBody (char[256], 256 bytes)
注: 包含 ST_POST_CHAR stCharInfo 嵌套
```

## ST_POST_CHAR 布局验证记录 (IDA @ 0x30 bytes)

```
+0x00: dwCharID (DWORD, 4 bytes)
+0x04: szCharName (char[32], 32 bytes)
+0x24: byState (unsigned __int8, 1 byte)
+0x25: padding (11 bytes)
```

---

