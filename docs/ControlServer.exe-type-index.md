# ControlServer.exe 类型索引

## 概述

- IDA 实例：Port 10001
- 更新时间：2026-04-30 04:36 +08:00

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
| CRouletteEventMgr | 4 | - | verified | IDA + 源码 |
| PS_GM_ROULETTE_EVENT | 5 | - | verified | IDA + 源码 |
| ST_GM_ROULETTE_EVENT_ITEM | 6 | - | verified | IDA + 源码 |

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

### CRouletteEventMgr 类型 (本轮验证)

| 所属目录 | 文件名 | 类型名 | 字段数 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CRouletteEventMgr.h | CRouletteEventMgr | 4 | verified | IDA |
| XControlServer | CRouletteEventMgr.h | ST_GM_ROULETTE_EVENT_ITEM | 6 | verified | IDA |
| XControlServer | CRouletteEventMgr.h | PS_GM_ROULETTE_EVENT | 5 | verified | IDA |
| XControlServer | CRouletteEventMgr.h | PS_ROULETTE_EVENT_UPDATE_SERVER | 3 | verified | IDA |
| XControlServer | CRouletteEventMgr.h | PS_DB_ROULETTE_REWARD_INFO | 1 | verified | IDA |

## CRouletteEventMgr 成员布局 (IDA constructor @ 0x14003A4C0)
```
+0x00: m_rwEventRouletteLock (CFSRWLock)
+0x??: m_psRouletteEventInfo (PS_GM_ROULETTE_EVENT)
+0x??: m_dw64Tick (uint64_t) = 0
+0x??: m_bLoad (bool) = false
```

## ST_GM_ROULETTE_EVENT_ITEM 成员布局 (IDA)
```
+0x00: nRewradIndex (int, 4 bytes)
+0x04: nItemID (int, 4 bytes)
+0x08: nCount (int, 4 bytes)
+0x0C: nCurCount (int, 4 bytes)
+0x10: nMaxCount (int, 4 bytes)
+0x14: nDayCount (int, 4 bytes)
Total: 24 bytes (0x18)
```

## PS_GM_ROULETTE_EVENT 成员布局 (IDA)
```
+0x00: nEventID (int, 4 bytes)
+0x04: nVer (int, 4 bytes)
+0x08: nStartTime (int, 4 bytes)
+0x0C: nEndTime (int, 4 bytes)
+0x10: psRewardList (std::vector<ST_GM_ROULETTE_EVENT_ITEM>)
```

## CRouletteEventMgr 关键行为 (IDA)
```
Update @ 0x14003AC10:
- 60秒更新间隔 (m_dw64Tick + 60000)
- 首次更新: LoadRouletteEvent + SendDBRouletteRewardInfo
- 后续更新: if(LoadRouletteEvent) SendRouletteEvent(true)

LoadRouletteEvent @ 0x14003A5A0:
- 版本检查: m_psRouletteEventInfo.nVer != psNew.nVer
- 版本不同: CheckItemDayCount + 复制新信息

SendDBRouletteRewardInfo @ 0x14003ACA0:
- XSendDBPacket(0, 0x49, 0x2D)

SendRouletteEvent @ 0x14003AD30:
- XSendPacket(0xF2, 0x74)
- 包含 PS_GM_ROULETTE_EVENT + bSend

UpdateRouletteItemInfo @ 0x14003ADE0:
- 按 nRewardID 匹配并更新 nCurCount

SetRouletteRewardInfo @ 0x14003AE90:
- 遍历 vecInfo 匹配 EventID/RewardID 更新 nCurCount
- 完成后 m_bLoad = true
- 调用 SendRouletteEvent(true)
```

---

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

注: XLoginServer 实现使用继承设计 (CForceManager : CPartyManager)，复用 m_mapParty 存储数据。

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

### CMyRoom 类型验证追加 (2026-04-29 10:44 +08:00)

| 所属目录 | 文件名 | 类型名 | 字段数 | 大小 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- | --- |
| XControlServer | CMyRoom.h | CMyRoom | 4 | 96 | verified | IDA |

## CMyRoom 布局验证记录 (IDA)

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

### CMyRoom 状态机
```
NONE(0) -> READY(1) -> DELETE(2)
- CreateSucc: state=1 (READY)
- DeleteReq: state=2 (DELETE)
- EnterSucc: if(state==2) state=1 (DELETE->READY recovery)
```

---

### CWorldMode 类型验证追加 (2026-04-29 10:50 +08:00)

| 所属目录 | 文件名 | 类型名 | 字段数 | 大小 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- | --- |
| XControlServer | WorldMode.h | CWorldMode | 10 | - | verified | IDA |

## CWorldMode 成员布局 (IDA constructor @ 0x140046BD0)
```
+0x00: __vftable (8 bytes) - CWorldMode 有 vtable
+0x08: m_stInfo (ST_WORLD_MODE_INFO)
+0x??: m_nTableID (int, 4 bytes) = 0
+0x??: m_nNextModeID (int, 4 bytes) = 0
+0x??: m_nLimitTime (int, 4 bytes) = 0
+0x??: m_bReserveFinish (bool, 1 byte) = 0
+0x??: m_uxCompleteMapID (UXMapID, 8 bytes) = 0
+0x??: m_nClearWaitTime (int, 4 bytes) = 0
+0x??: m_bSuccess (bool, 1 byte) = 0
+0x??: m_strKiller (wchar_t[21], 42 bytes)
+0x??: m_nStartType (int, 4 bytes)
+0x??: m_nModeTime (int, 4 bytes)
```

### CWorldMode 状态机
```
状态 nState:
- 0: 待机 (idle)
- 1: 运行 (running)
- 2: 完成 (completed)

StartType:
- 0: 时间触发型 (需要 SetNextEventTime)
- 1: 手动触发型

包 ID:
- 0xFB/0x01: StartMode
- 0xFB/0x03: ClearMode
- 0xFB/0x04: FinishMode (PS_WORLD_MODE_FINISH)
- DB 0x49/0x07: 状态更新
```

---

### CWorldModeMgr 类型验证追加 (2026-04-29 10:50 +08:00)

| 所属目录 | 文件名 | 类型名 | 字段数 | 大小 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- | --- |
| XControlServer | WorldModeMgr.h | CWorldModeMgr | 10 | - | verified | IDA |

## CWorldModeMgr 成员布局 (IDA constructor @ 0x1400497C0)
```
+0x00: m_rwLock (CFSRWLock)
+0x??: m_bLoadReq (bool) = 0
+0x??: m_bLoadDB (bool) = 0
+0x??: m_mapWorldMode (std::map<std::pair<int,int>, std::tr1::shared_ptr<CWorldMode>>)
+0x??: m_tInit (ATL::CTime)
+0x??: m_dwUpdateTime (DWORD64)
+0x??: m_bModeOn (bool)
+0x??: m_bSendMode (bool)
```

### CWorldModeMgr 关键行为
```
Init():
- clear m_mapWorldMode
- m_dwUpdateTime = GetTickCount64()
- m_bModeOn = XResourceMgr::GetServerContents(E_SERVER_OPTION_D6_MODE)

Update():
- if(!m_bLoadReq && m_bModeOn) send DB 0x49/0x06 (load request)
- if(m_bLoadDB && IsCompleteCachingLoad)
  - if(!m_bModeOn) send 0xFB/0x05 to clear user state
  - Update tick interval: 333ms
  - Check day change: InitMode + SendModeList
  - Iterate m_mapWorldMode: CWorldMode::Update
```

---


### CForceManager / CForce 类型验证追加 (2026-04-29 13:42 +08:00)

| 所属目录 | 文件名 | 类型名 | 字段数 | 大小 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- | --- |
| XControlServer | CForceManager.h | CForceManager | 3 | 40 | verified | IDA |
| XControlServer | CForce.h | CForce | 3 | 48 | verified | IDA |

## CForceManager 成员布局 (IDA)
```
+0x00: m_bLoad (bool) = true (1 byte)
+0x01~0x08: padding (7 bytes)
+0x08: m_mapForce (std::map<int, std::tr1::shared_ptr<CForce>>) (32 bytes)
Total: 40 bytes
```

## CForce 成员布局 (IDA constructor @ 0x14003A450)
```
+0x00: m_nForceID (int) = nForceID (4 bytes)
+0x04: padding (4 bytes)
+0x08: m_uxMapID (UXMapID) (8 bytes)
+0x10: m_mapMemberInfo (std::map<int, UXMapID>) (32 bytes)
Total: 48 bytes (0x30)
```

## CForce / CParty 共享行为
```
IsFull():
- return m_mapMemberInfo.size() >= 4

SetMember(nActorID, uxMapID):
- find/insert in m_mapMemberInfo[nActorID] = uxMapID

SetMazeID(uxMapID):
- m_uxMapID = uxMapID

IsEmpty():
- return m_mapMemberInfo.empty()
```


---

### CPartyManager / CParty 类型验证追加 (2026-04-29 13:42 +08:00)

| 所属目录 | 文件名 | 类型名 | 字段数 | 大小 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- | --- |
| XControlServer | CPartyManager.h | CPartyManager | 3 | 40 | verified | IDA |
| XControlServer | CParty.h | CParty | 3 | 48 | verified | IDA |

## CPartyManager 成员布局 (IDA constructor @ 0x140030CA0)
```
+0x00: m_bLoad (bool) = true (1 byte)
+0x01~0x08: padding (7 bytes)
+0x08: m_mapParty (std::map<int, std::tr1::shared_ptr<CParty>>) (32 bytes)
Total: 40 bytes
```

## CParty 成员布局 (IDA)
```
+0x00: m_nPartyID (int) = nPartyID (4 bytes)
+0x04: padding (4 bytes)
+0x08: m_uxMapID (UXMapID) (8 bytes)
+0x10: m_mapMemberInfo (std::map<int, UXMapID>) (32 bytes)
Total: 48 bytes (0x30)
```

## CPartyManager / CParty 关键行为
```
SetMember(nPartyID, nActorID, uxMapID):
- find m_mapParty
- if exists: call CParty::SetMember
- else: create new CParty(48 bytes), SetMember, insert

SetMazeID(nPartyID, uxMapID, uxBeforeMapID):
- find m_mapParty
- GetMazeID and compare with beforeMapID
- if match: SetMazeID and return true
- else: return false

FindSamePlace(nPartyID, dwMapID, dwActorID):
- iterate m_mapMemberInfo
- skip dwActorID, check SWORD2(uxMapID.nMapID) == dwMapID
- return matching UXMapID or 0
```

---


### Process 类类型验证追加 (2026-04-29 14:34 +08:00)

| 所属目录 | 文件名 | 类型名 | 字段数 | 大小 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.h | CServerProcess | 1 | 72 | verified | IDA |
| XControlServer | UserProcess.cpp | CUserProcess | 1 | 72 | verified | IDA |
| XControlServer | GMToolProcess.cpp | CGMToolProcess | 1 | 72 | verified | IDA |
| XControlServer | WorldModeProcess.cpp | CWorldModeProcess | 1 | 72 | verified | IDA |

## Process 类布局 (IDA)
```
CServerProcess:
+0x00: field_0 (TXProcess<CServer>) - 72 bytes
Total: 72 bytes

CUserProcess:
+0x00: field_0 (TXProcess<CServer>) - 72 bytes
Total: 72 bytes

CGMToolProcess:
+0x00: field_0 (TXProcess<CServer>) - 72 bytes
Total: 72 bytes

CWorldModeProcess:
+0x00: field_0 (TXProcess<CServer>) - 72 bytes
Total: 72 bytes
```

## Process 类关键行为
```
CServerProcess::Parse (main=0xF2):
- 33 个子命令: 0x01, 0x03, 0x10, 0x21-0x27, 0x30-0x33, 0x36-0x40, 0x41/0x42(共用), 0x43, 0x49, 0x50-0x59, 0x63, 0x73, 0x75, 0x77
- default 返回 true

CUserProcess::Parse (main=0xF3):
- 17 个子命令: 0x01, 0x03, 0x04, 0x07, 0x11-0x13, 0x16, 0x17, 0x20, 0x26, 0x27, 0x31, 0x32, 0x35, 0x60
- default 返回 false

CGMToolProcess::Parse (main=0xF8):
- 7 个子命令: 0x01-0x07
- default 返回 true

CWorldModeProcess::Parse (main=0xFB):
- 4 个子命令: 0x02, 0x06, 0x07, 0x09
- default 返回 false
```

---


### 本轮验证 (2026-04-29 19:52)

**验证类型：**
- CUserObject multi_index 容器结构: 4 个索引
- CParty 新增方法: IsFull/IsEmpty/RemoveMember/SetPartyID

**验证函数：**
- XControlServer::RemoveUser @ 0x14000C610
- CGMToolProcess::Parse @ 0x140034640
- CWorldModeProcess::Parse @ 0x14004D5F0
- CUserProcess::Parse @ 0x1400450E0

**multi_index 结构验证：**
```cpp
// CUserObject 容器索引定义 (IDA 验证)
boost::multi_index::multi_index_container<
    std::tr1::shared_ptr<CUserObject>,
    indices<
        // [0] CID - hashed_unique
        hashed_unique<const_mem_fun<CUserObject, const unsigned long, &CUserObject::GetCID>>,
        // [1] Name - hashed_unique  
        hashed_unique<const_mem_fun<CUserObject, const std::wstring, &CUserObject::GetName>>,
        // [2] UAID - hashed_unique
        hashed_unique<const_mem_fun<CUserObject, const unsigned long, &CUserObject::GetUAID>>,
        // [3] ServerID - ordered_non_unique
        ordered_non_unique<const_mem_fun<CUserObject, const unsigned long, &CUserObject::GetServerID>>
    >
>
```

---

### CMazeInfo 结构体详细验证追加 (2026-04-29 22:05 +08:00)

| 所属目录 | 文件名 | 类型名 | 字段数 | 大小 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- | --- |
| XControlServer | MazeInfo.h | CMazeInfo | 10 | 728 | verified | IDA struct_info |
| XControlServer | MazeInfo.h | ST_MAP_INFO | 11 | 600 | verified | IDA struct_info |
| PSServer.h | PSServer.h | ST_MAZE_WAIT_ENTER_USER_INFO | 10 | 48 | verified | IDA struct_info |

## CMazeInfo 成员布局 (IDA struct_info @ 728 bytes)
```
+0x000 (16): m_pParentMaze (std::tr1::shared_ptr<CMazeInfo>)
+0x010 (16): m_pChildMaze (std::tr1::shared_ptr<CMazeInfo>)
+0x020 (8): m_stPartyInfo (ST_PARTY_INFO)
+0x028 (32): m_vecEnterMember (std::vector<ST_ENTER_MAZE_MEMBER_INFO>)
+0x048 (4): m_nState (int)
+0x050 (8): m_dwStateTime (unsigned __int64)
+0x058 (600): m_stMazeInfo (ST_MAP_INFO)
+0x2B8 (4): m_nUserCount (int) [offset 688]
+0x2BC (4): m_nType (int) [offset 692]
+0x2C0 (32): m_mapWaitEnterMazeUser (std::map<unsigned long,ST_MAZE_WAIT_ENTER_USER_INFO>) [offset 696]
```

## ST_MAP_INFO 成员布局 (IDA struct_info @ 600 bytes)
```
+0x000: dwUserID (unsigned int, 4 bytes)
+0x004: dwUAID (unsigned int, 4 bytes)
+0x008: dwServerID (unsigned int, 4 bytes)
+0x00C: nJumpID (int, 4 bytes)
+0x010: nPortalID (int, 4 bytes)
+0x018: uxMapID (UXMapID, 8 bytes)
+0x020: uxParentInstanceID (UXMapID, 8 bytes)
+0x028: szIP (char[513], 513 bytes)
+0x22A: sPort (__int16, 2 bytes)
+0x230: stPosInfo (STPosInfo, 32 bytes)
+0x250: byType (unsigned __int8, 1 byte)
```

## ST_MAZE_WAIT_ENTER_USER_INFO 成员布局 (IDA struct_info @ 48 bytes)
```
+0x00: stMemberInfo (ST_ENTER_MAZE_MEMBER_INFO, 8 bytes)
+0x08: dw64ExitTime (unsigned __int64, 8 bytes)
+0x10: byState (unsigned __int8, 1 byte)
+0x14: nTeam (int, 4 bytes)
+0x18: bEnter (bool, 1 byte)
+0x19: bCheckCondition (bool, 1 byte)
+0x1A: bLoadEX (bool, 1 byte)
+0x20: dwCheckSectorPosTick (unsigned __int64, 8 bytes)
+0x28: nReEnterCount (int, 4 bytes)
+0x2C: nRestartState (int, 4 bytes)
```

## CMazeInfo 关键行为验证 (IDA)
```
CMazeInfo::CMazeInfo @ 0x140035F10:
- m_nState = 1 (正常状态)
- m_dwStateTime = 0
- m_nUserCount = 0
- m_nType = 0
- m_pParentMaze/m_pChildMaze reset
- m_vecEnterMember/m_mapWaitEnterMazeUser clear

CMazeInfo::Init (ST_CREATE_MAZE) @ 0x1400360D0:
- 复制 ST_MAP_INFO (600 bytes)
- 复制 m_stPartyInfo
- 复制 m_vecEnterMember
- 初始化 m_mapWaitEnterMazeUser: byState = 10

CMazeInfo::UpdateMazeInfo @ 0x140036560:
- if (m_nState != 3) 执行更新
- 状态 3 且无子迷宫时设置 m_dwStateTime = GetTickCount64()

CMazeInfo::SyncMazeInfo @ 0x140036820:
- 复制 stPartyInfo/dwServerID/sPort/nJumpID/szIP
- 状态 3 且无子迷宫时设置 m_dwStateTime = GetTickCount64()

CMazeInfo::IsDestroy @ 0x140036D70:
- return m_nState == 3 && m_dwStateTime < GetTickCount64()

CMazeInfo::GetPartyID @ 0x140036500:
- if (m_stPartyInfo.byGroupType == 1) return m_stPartyInfo.nID
- else return 0

CMazeInfo::GetForceID @ 0x140036530:
- if (m_stPartyInfo.byGroupType == 2) return m_stPartyInfo.nID
- else return 0

CMazeInfo::ResetParentMaze @ 0x140036B80:
- if (GetUserCount() == 0) SetMazeState(3, GetTickCount64() + 60000)
- ResetChildMaze()
- m_pParentMaze.reset()

CMazeInfo::IsValidEnterMaze @ 0x140036C10:
- 检查 Maze_Type (2, 8, 9) 为 ApocalypseRaid
- 状态检查返回错误码: 55043/55054/55022/55042/55036
```

---

### CWorldMode / CWorldModeMgr 类型验证追加 (2026-04-29 22:05 +08:00)

| 所属目录 | 文件名 | 类型名 | 字段数 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldMode.h | CWorldMode | 10+ | verified | IDA constructor |
| XControlServer | WorldModeMgr.h | CWorldModeMgr | 5+ | verified | IDA constructor |

## CWorldMode 成员布局 (IDA constructor @ 0x140046BD0)
```
+0x00: __vftable (8 bytes) - 有 vtable (多态类)
+0x08: m_stInfo (ST_WORLD_MODE_INFO)
+0x??: m_nTableID (int) = 0
+0x??: m_nNextModeID (int) = 0
+0x??: m_nLimitTime (int) = 0
+0x??: m_bReserveFinish (bool) = 0
+0x??: m_uxCompleteMapID (UXMapID) = 0
+0x??: m_nClearWaitTime (int) = 0
+0x??: m_bSuccess (bool) = 0
+0x??: m_strKiller (wchar_t[21], 42 bytes)
```

## CWorldModeMgr 成员布局 (IDA constructor @ 0x1400497C0)
```
+0x00: m_rwLock (CFSRWLock)
+0x??: m_bLoadReq (bool) = 0
+0x??: m_bLoadDB (bool) = 0
+0x??: m_mapWorldMode (std::map<std::pair<int,int>, std::tr1::shared_ptr<CWorldMode>>)
+0x??: m_tInit (ATL::CTime)
```

---

### CUserObject 关键方法验证追加 (2026-04-29 22:05 +08:00)

## CUserObject 关键行为验证 (IDA)
```
CUserObject::CanChangeWorld @ 0x140044B60:
- return !m_nWorldState && m_dwWorldChangeWaitTick <= GetTickCount64() && !m_uxTransMapID.nMapID
  条件1: m_nWorldState == 0
  条件2: 等待时间已过
  条件3: 无过渡地图

CUserObject::ResetWorldState @ 0x140044BB0:
- m_nWorldState = 0
- m_nBeforeServerID = 0
- if (m_uxTransMapID.nMapID > 0 && m_uxTransMapID != uxMapID) 记录错误日志
- m_uxTransMapID = 0
```

---

### CServer 迷宫验证方法追加 (2026-04-29 22:05 +08:00)

## CServer 迷宫验证关键行为 (IDA)
```
CServer::GetMazeInfo @ 0x140041870:
- 查找 m_mapMazeInfo
- 找到: 返回 shared_ptr
- 未找到: 返回 nullptr

CServer::UpdateMaze @ 0x1400421E0:
- 找到: CMazeInfo::UpdateMazeInfo
- 未找到: ClassFactory 创建, UpdateMazeInfo, insert 到 m_mapMazeInfo

CServer::IsValidEnterPartyMemberInMaze @ 0x140041C80:
- 查找迷宫, GetPartyID/wMapID 比较
- 遍历成员, 找到 actor 时复制 ST_MAP_INFO (600 bytes)
- nResult: 0=成功, 1=未找到, 2=不匹配, 3=非成员

CServer::IsValidEnterForceMemberInMaze @ 0x140041F30:
- 同上逻辑, GetForceID 比较
```


---

### CMyRoom 类型验证追加 (2026-04-29 23:30 +08:00)

| 所属目录 | 文件名 | 类型名 | 字段数 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CMyRoom.h | CMyRoom | 4 | verified | IDA struct_info |

## CMyRoom 成员布局 (IDA struct_info, 96 bytes / 0x60)
```
+0x00: m_stOwnerInfo (ST_MYROOM_OWNER_INFO, 68 bytes)
+0x44: padding (4 bytes)
+0x48: m_pServer (CServer*, 8 bytes)
+0x50: m_uxMapID (UXMapID, 8 bytes)
+0x58: m_byState (uint8_t, 1 byte)
+0x59: padding (7 bytes)
```

## CMyRoom 状态机 (IDA 验证)
```
NONE(0) -> CreateMyRoom() 初始化
CreateSucc() -> READY(1)
DeleteReq() -> DELETE(2)
EnterSucc(): DELETE(2) -> READY(1)
```

---

### CParty / CForce 类型验证追加 (2026-04-29 23:30 +08:00)

| 所属目录 | 文件名 | 类型名 | 字段数 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CParty.h | CParty | 3 | verified | IDA struct_info |
| XControlServer | CForce.h | CForce | 3 | verified | IDA struct_info |

## CParty / CForce 成员布局 (IDA struct_info, 48 bytes)
```
+0x00: m_nPartyID/m_nForceID (int, 4 bytes)
+0x04: padding (4 bytes)
+0x08: m_uxMapID (UXMapID, 8 bytes)
+0x10: m_mapMemberInfo (std::map<int, UXMapID>, 32 bytes)
```

---

### CPartyManager / CForceManager 类型验证追加 (2026-04-29 23:30 +08:00)

| 所属目录 | 文件名 | 类型名 | 字段数 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | PartyManager.h | CPartyManager | 2 | verified | IDA constructor |
| XControlServer | CForceManager.h | CForceManager | 2 | verified | IDA constructor |

## CPartyManager / CForceManager 成员布局 (IDA constructor, 40 bytes)
```
+0x00: m_bLoad (bool, 1 byte)
+0x01: padding (7 bytes)
+0x08: m_mapParty/m_mapForce (std::map<int, shared_ptr<CParty/CForce>>, 32 bytes)
```

---

### CUserObject 类型验证追加 (2026-04-29 23:30 +08:00)

| 所属目录 | 文件名 | 类型名 | 字段数 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserObject.h | CUserObject | 10+ | verified | IDA constructor |

## CUserObject 成员布局 (IDA constructor @ 0x140044A80, 888 bytes / 0x378)
```
+0x00: m_dwConnectTick (uint32_t, 4 bytes)
+0x04: padding (4 bytes)
+0x08: m_pServer (CServer*, 8 bytes)
+0x10: m_dwIP (uint32_t, 4 bytes)
+0x14: padding (4 bytes)
+0x18: m_stCharInfo (STCharInfo)
+0x??: m_stPartyInfo (ST_PARTY_INFO)
+0x??: m_uxMapID (UXMapID, 8 bytes)
+0x??: m_uxTransMapID (UXMapID, 8 bytes)
+0x??: m_dwServerID (uint32_t, 4 bytes)
+0x??: m_byTradePasswordState (uint8_t, 1 byte)
+0x??: m_biAuthSessionID (int64_t, 8 bytes)
+0x??: m_byBlockType (uint8_t, 1 byte)
+0x??: m_dw64GroupIDTick (uint64_t, 8 bytes)
+0x??: m_nWorldState (int, 4 bytes)
+0x??: m_nBeforeServerID (int, 4 bytes)
+0x??: m_dwWorldChangeWaitTick (uint64_t, 8 bytes)
+0x??: m_byAuthType (uint8_t, 1 byte)
```

## XControlServer::m_UserInfos boost::multi_index 索引
```
index<0>: ordered_unique (默认)
index<1>: UAID (GetUAID)
index<2>: Name (GetName)
index<3>: ServerID (GetServerID)
```

---

### XControlServer 关键方法验证追加 (2026-04-29 23:30 +08:00)

## XControlServer::GetUser (IDA 验证)
```
GetUser(dwActorID) @ 0x14000CBB0:
- CFAutoSlimReadLock on m_rwLock
- m_UserInfos.get<1>().find(dwActorID)
- 返回 shared_ptr 或 null

GetUser(pName) @ 0x140010D00:
- CFAutoSlimReadLock on m_rwLock
- m_UserInfos.get<2>().find(pName)
- 返回 shared_ptr 或 null
```

## XControlServer::AddUser (IDA 验证)
```
AddUser @ 0x14000C220:
- CFAutoSlimWriteLock on m_rwLock
- 先查找 UAID (index<3>) 是否存在
- 存在且 authSessionID 更大: 发送 (0xF3, 7) 拒绝
- 存在且 authSessionID 更小: erase 旧用户, KickoutUser_NoLock
- new CUserObject(888 bytes)
- SetIP, SetTradePWState, SetAuthSessionID, SetBlockType, SetGroupIDTick
- insert 到 m_UserInfos
- CalculateServerUserStaus, UpdateUserMap
```

---

### Process 分发器验证汇总 (2026-04-29 23:30 +08:00)

## CServerProcess::Parse (main=0xF2, 27 子命令)
```
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
0x41: ResCreateMatchingMazeFromGame(PARTY)
0x42: ResCreateMatchingMazeFromGame(FORCE)
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
```

## CUserProcess::Parse (main=0xF3, 17 子命令)
```
0x01: SyncSelectCharacter
0x03: SyncLogoutUser
0x04: SyncUpdateUserMap
0x07: SyncUserKickout
0x11: ReqUserChatNotice
0x12: ReqUserChangeServer
0x13: SyncUserMoneyLog
0x16: ReqUserEnterPartyMaze
0x17: ReqUserChatMegaPhone
0x20: ReqUserEnterForceMaze
0x26: ReqUserTradePasswordStateSync
0x27: ReqUserTradePasswordState
0x31: ReqNameChange
0x32: ReqCheckSessionID
0x35: ReqUserUpdateAuthType
0x60: ReqGameServerEnterUser
```

## CGMToolProcess::Parse (main=0xF8, 7 子命令)
```
0x01: ReqGMUserKick
0x02: ReqGMNotice
0x03: ReqGMShutDwon
0x04: ReqGMTimeEvent
0x05: ReqGMValueEvent
0x06: ReqGMServerOption
0x07: ReqGMCashShopBanner
```

---

## CMyRoom 类型 (本轮验证)

### 结构体
| 类型名 | 大小 (bytes) | 字段数 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- |
| CMyRoom | 96 | 4 | verified | IDA |
| ST_MYROOM_OWNER_INFO | 68 | 11 | verified | IDA |
| ST_MYROOM_USER | 28 | 5 | verified | IDA |
| E_MYROOM_STATE | - | 3 | verified | IDA |

### CMyRoom 布局
```
offset 0x00: ST_MYROOM_OWNER_INFO m_stOwnerInfo (68 bytes)
offset 0x44: padding (4 bytes)
offset 0x48: CServer* m_pServer (8 bytes)
offset 0x50: UXMapID m_uxMapID (8 bytes)
offset 0x58: uint8_t m_byState (1 byte)
offset 0x59: padding (7 bytes)
total: 96 bytes (0x60)
```

### ST_MYROOM_OWNER_INFO 布局
```
offset 0x00: DWORD dwOwnerUAID (4 bytes)
offset 0x04: SHORT shMapIndex (2 bytes)
offset 0x06: SHORT shGridNo (2 bytes)
offset 0x08: DWORD dwMapID (4 bytes)
offset 0x0C: BYTE byRoomOpenLevel (1 byte)
offset 0x0D: padding (1 byte)
offset 0x0E: wchar_t szRoomName[20] (40 bytes)
offset 0x36: int nRecommendCount (4 bytes)
offset 0x3A: int nFavoriteCount (4 bytes)
offset 0x3E: bool bFavorite (1 byte)
offset 0x3F: bool bRecommend (1 byte)
total: 68 bytes
```

### ST_MYROOM_USER 布局
```
offset 0x00: int nServerID (4 bytes)
offset 0x04: DWORD dwUAID (4 bytes)
offset 0x08: DWORD dwUCID (4 bytes)
offset 0x0C: DWORD dwBeforeMapID (4 bytes)
offset 0x10: XVec3 xBeforePos (12 bytes)
total: 28 bytes
```

### E_MYROOM_STATE 枚举
```
E_MYROOM_STATE_NONE = 0    // 初始/创建中
E_MYROOM_STATE_READY = 1   // 就绪/可进入
E_MYROOM_STATE_DELETE = 2  // 删除中
```

### MyRoom 包命令
```
0xF2/0x50: MyRoomEnterReq (请求进入)
0xF2/0x51: MyRoomEnterRes (进入响应/失败)
0xF2/0x52: CreateMyRoom (创建请求)
0xF2/0x53: DeleteMyRoomReq (删除请求)
0xF2/0x59: DeleteMyRoomRes (删除响应)
```

### Party/Force 结构体 (本轮验证)
| 类型名 | 大小 (bytes) | 字段数 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- |
| CParty | 48 | 3 | verified | IDA |
| CForce | 48 | 3 | verified | IDA |
| CPartyManager | 40 | 2 | verified | IDA |
| CForceManager | 40 | 2 | verified | IDA |

### CParty 布局
```
offset 0x00: int m_nPartyID (4 bytes)
offset 0x04: padding (4 bytes)
offset 0x08: UXMapID m_uxMapID (8 bytes)
offset 0x10: std::map<int, UXMapID> m_mapMemberInfo (32 bytes)
total: 48 bytes (0x30)
```

### CForce 布局
```
offset 0x00: int m_nForceID (4 bytes)
offset 0x04: padding (4 bytes)
offset 0x08: UXMapID m_uxMapID (8 bytes)
offset 0x10: std::map<int, UXMapID> m_mapMemberInfo (32 bytes)
total: 48 bytes (0x30)
```

### CPartyManager 布局
```
offset 0x00: bool m_bLoad (1 byte)
offset 0x01: padding (7 bytes)
offset 0x08: std::map<int, std::tr1::shared_ptr<CParty>> m_mapParty (32 bytes)
total: 40 bytes (0x28)
```

### CForceManager 布局
```
offset 0x00: bool m_bLoad (1 byte)
offset 0x01: padding (7 bytes)
offset 0x08: std::map<int, std::tr1::shared_ptr<CForce>> m_mapForce (32 bytes)
total: 40 bytes (0x28)
```

### Party/Force 类型混淆说明
```
IDA 显示 CParty 和 CForce 具有完全相同的布局 (48 bytes)，
导致类型识别混淆。CPartyManager::SetMember 实际创建 CForce 对象，
但存储为 shared_ptr<CParty>。这是合法的，因为两者内存结构相同。
```

---

### 本轮验证追加 (2026-04-30 04:12 +08:00)

## Process 类布局验证 (IDA constructor)
```
CServerProcess (72 bytes / 0x48):
+0x00: TXProcess<CServer> 基类 (72 bytes)
Total: 72 bytes

CUserProcess (72 bytes / 0x48):
+0x00: TXProcess<CServer> 基类 (72 bytes)
Total: 72 bytes

CGMToolProcess (72 bytes / 0x48):
+0x00: TXProcess<CServer> 基类 (72 bytes)
Total: 72 bytes

CWorldModeProcess (72 bytes / 0x48):
+0x00: TXProcess<CServer> 基类 (72 bytes)
Total: 72 bytes

Process 类关键行为:
- RegisterProcess @ 0x140041180: 注册 4 个 Process (0xF2, 0xF3, 0xF8, 0xFB)
- 每个 Process 分配 0x48 bytes
- 使用 XClient::Register 注册到对应 MainCmd
```

## STItem 结构体验证
```
STItem (大小可变):
+0x00: nItemID (int, 4 bytes) = -1 (初始化)
+0x04: xSerial (__int64, 8 bytes) = -1 (初始化)
+0x0C: sCount (short, 2 bytes) = 0 (初始化)
+0x0E: bBindType (byte, 1 byte) = 0 (初始化)
+0x10: stExtendOption[5] (数组, 每个 8 bytes)
+0x38: byUpgrade (byte, 1 byte) = 0
+0x39: eFlag (enum, 4 bytes) = 0
+0x3D: byEndurance (byte, 1 byte) = 0
+0x3E: bySocketActiveCount (byte, 1 byte) = 0
+0x3F: nCashDate (int, 4 bytes) = 0
+0x43: byUpgradeCount (byte, 1 byte) = 0
+0x44: byUpgradeLimit (byte, 1 byte) = 0
+0x45: nExp (int, 4 bytes) = 0
+0x49: szBroachState[16] (char, 16 bytes) = "000000000000000"
+0x59: byRestoreCount (byte, 1 byte) = 0
+0x5A: bySealCount (byte, 1 byte) = 0
+0x5B: bySealDelCount (byte, 1 byte) = 0
+0x5C: nAttack (int, 4 bytes) = 0
+0x60: nDefense (int, 4 bytes) = 0
+0x64: nTitleID (int, 4 bytes) = 0
+0x68: byUseCount (byte, 1 byte) = 0
+0x69: nDyeID (int, 4 bytes) = 0
```

## XControlServer 成员布局验证 (IDA destructor)
```
析构顺序 (从后向前):
1. m_mapEventMazeToEnter (std::map)
2. m_listWaitEnterServer (std::list)
3. m_xItemFactory (XItemFactory)
4. m_rouletteEventManager (CRouletteEventMgr)
5. m_dayEventManager (CDayEventMgr)
6. m_worldModeManager (CWorldModeMgr)
7. m_forceManager (CForceManager)
8. m_partyManager (CPartyManager)
9. m_xResourceMgr (XResourceMgr)
10. m_worldManager (CWorldManager)
11. m_scObserveSocket (CObserveSocket)
12. m_xDBAgentMgr (XGameDBSocketMgr)
13. m_xSeed (XSeed)
14. m_mapBattleZoneInfo (std::map)
15. m_mapCreateModeMazeReq (std::map)
16. m_mapCreateMazeReq (std::map)
17. m_mapMyRoomServer (std::map)
18. m_mapMazeServer (std::map)
19. m_mapGameServer (std::map)
20. m_factoryMaze (ClassFactory<CMazeInfo,64>)
21. m_UserInfos (boost::multi_index)
```

## CWorldModeMgr 方法验证汇总
```
Init @ 0x140049840: 清空 m_mapWorldMode, 设置 m_dwUpdateTime
LoadMode @ 0x1400498A0: 复杂 DB 加载逻辑, World_ID 匹配
Update @ 0x14004A050: 更新循环, 333ms 间隔
SendModeSync @ 0x14004A630: 同步模式状态
IsTodayModeList @ 0x14004AFC0: 检查今日模式列表
InitMode @ 0x14004B140: 初始化模式时间
IsEnterMode @ 0x14004B8A0: 进入验证, 错误码 55064/55090
GetActiveMode @ 0x14004BB40: 获取激活模式
```

## CServer 方法验证汇总
```
GetMazeInfo @ 0x140041870: 查找并返回 shared_ptr<CMazeInfo>
IsValidEnterPartyMemberInMaze @ 0x140041C80: Party 成员验证
IsValidEnterForceMemberInMaze @ 0x140041F30: Force 成员验证
UpdateMaze @ 0x1400421E0: 更新或创建迷宫
RegisterProcess @ 0x140041180: 注册 4 个 Process
```


---

### CDayEventMgr 类型 (2026-04-30 06:37 +08:00 验证)

| 所属目录 | 文件名 | 类型名 | 字段数 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | DayEventManager.h | CDayEventMgr | 4 | verified | IDA + 源码 |
| XLoginServer | DayEventManager.h | ST_DAY_EVENT_INFO | 2 | verified | IDA + 源码 |
| XLoginServer | DayEventManager.h | PS_DAY_EVENT_LIST | 2 | verified | IDA + 源码 |

## CDayEventMgr 成员布局 (IDA constructor @ 0x14002D8B0)
```
+0x00: m_rwEventLock (CFSRWLock)
+0x??: m_tLastSetTime (ATL::CTime)
+0x??: m_dw64Tick (uint64_t) = 0
+0x??: m_mapDayEvent (std::map<uint16_t, ST_DAY_EVENT_INFO>)
Total: 56 bytes (IDA ControlServer.h offset 0x20B40)
```

## ST_DAY_EVENT_INFO 成员布局 (IDA)
```
+0x00: wMazeID (uint16_t, 2 bytes)
+0x02: wBoosterID (uint16_t, 2 bytes)
Total: 4 bytes
```

### CUserObject 方法验证追加 (2026-04-30 06:38 +08:00)

| 所属目录 | 文件名 | 方法名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | UserObject.cpp | CUserObject::SetWorldState | 0x14000A500 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetPartyInfo | 0x14000A1E0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::CanChangeWorld | 0x140044B60 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::SetBeforeServerID | 0x14000A560 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetServerID | 0x14000AAB0 | verified | IDA |
| XControlServer | UserObject.cpp | CUserObject::GetEnterWorldModeInfo | 0x14000A860 | verified | IDA |

## CUserObject 关键成员布局 (IDA size = 888 bytes)
```
+0x00: m_dwConnectTick (uint64_t)
+0x08: m_pServer (CServer*)
+0x10: m_dwIP (uint32_t)
+0x??: m_stCharInfo (STCharInfo, ~600 bytes)
+0x??: m_uxMapID (UXMapID, 8 bytes)
+0x??: m_dwServerID (uint32_t)
+0x??: m_stPartyInfo (ST_PARTY_INFO)
+0x??: m_nWorldState (int)
+0x??: m_nBeforeServerID (int)
+0x??: m_dwWorldChangeWaitTick (uint64_t)
+0x??: m_uxTransMapID (UXMapID, 8 bytes)
+0x??: m_byAuthType (uint8_t)
+0x??: m_biAuthSessionID (int64_t)
+0x??: m_byBlockType (uint8_t)
+0x??: m_byTradePasswordState (uint8_t)
+0x??: m_dw64GroupIDTick (uint64_t)
+0x??: m_stEnterWorldModeInfo (ST_ENTER_WORLD_MODE_INFO)
Total: 888 bytes (0x378)
```

### CMyRoom 类型 (2026-04-30 06:28 +08:00 验证)

| 所属目录 | 文件名 | 类型名 | 字段数 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CMyRoom.h | CMyRoom | 4 | verified | IDA + 源码 |
| XCore/XServer | GreenDamTan_MyRoomStructs.h | ST_MYROOM_OWNER_INFO | 17 | verified | IDA + 源码 |
| XCore/XServer | GreenDamTan_MyRoomStructs.h | ST_MYROOM_USER | 7 | verified | IDA + 源码 |

## CMyRoom 成员布局 (IDA size = 96 bytes / 0x60)
```
+0x00: m_stOwnerInfo (ST_MYROOM_OWNER_INFO, 68 bytes)
+0x44: padding (4 bytes, 对齐到 8 字节边界)
+0x48: m_pServer (CServer*, 8 bytes)
+0x50: m_uxMapID (UXMapID, 8 bytes)
+0x58: m_byState (uint8_t, 1 byte)
+0x59: padding (7 bytes)
Total: 96 bytes (0x60)
```

## CMyRoom 状态机 (IDA 验证)
```
E_MYROOM_STATE_NONE = 0    // 初始状态 (CreateMyRoom 设置)
E_MYROOM_STATE_READY = 1   // 就绪 (CreateSucc 设置)
E_MYROOM_STATE_DELETE = 2  // 删除中 (DeleteReq 设置)

状态转换:
- NONE -> READY: CreateSucc() 直接设置 m_byState = 1
- READY -> DELETE: DeleteReq() 直接设置 m_byState = 2
- DELETE -> READY: EnterSucc() 条件转换 (if m_byState == 2)
```

## ST_MYROOM_OWNER_INFO 成员布局 (IDA size = 68 bytes)
```
+0x00: dwOwnerUAID (uint32_t, 4 bytes)
+0x04: dwOwnerUCID (uint32_t, 4 bytes)
+0x08: szOwnerName (wchar_t[32], 64 bytes)
Total: 72 bytes (IDA 显示复制 68 bytes = 0x44)
```

## ST_MYROOM_USER 成员布局 (IDA)
```
+0x00: dwUCID (uint32_t)
+0x04: nServerID (int)
+0x08: uxMapID (UXMapID, 8 bytes)
+0x10: szName (wchar_t[32], 64 bytes)
Total: ~80 bytes
```

### Process 类类型 (2026-04-30 06:28 +08:00 验证)

| 所属目录 | 文件名 | 类型名 | 字段数 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ServerProcess.h | CServerProcess | 0 | verified | IDA |
| XControlServer | UserProcess.h | CUserProcess | 0 | verified | IDA |
| XControlServer | GMToolProcess.h | CGMToolProcess | 0 | verified | IDA |
| XControlServer | WorldModeProcess.h | CWorldModeProcess | 0 | verified | IDA |

## Process 类继承关系 (IDA 验证)
```
CServerProcess : TXProcess<CServer> (72 bytes)
CUserProcess : TXProcess<CServer> (72 bytes)
CGMToolProcess : TXProcess<CServer> (72 bytes)
CWorldModeProcess : TXProcess<CServer> (72 bytes)

所有 Process 类:
- 继承 TXProcess<CServer>
- Parse 方法使用 GetSubCmd() 分发子命令
- 子命令处理方法为 protected/private
```

## CServerProcess::Parse 子命令映射 (IDA 0x14003CAC0)
```
main = 0xF2
subcmd 0x01: ReqCreateServer
subcmd 0x03: ReqUpdateServerInfo
subcmd 0x10: ReqChangeChannel
subcmd 0x21: ReqCreateMaze
subcmd 0x22: ResCreateMaze
subcmd 0x23: SyncMaze
subcmd 0x25: SyncRemoveMaze
subcmd 0x26: SyncUpdateMaze
subcmd 0x27: ReqGoBackMaze
subcmd 0x30: ReqCreateMap
subcmd 0x31: ReqEnterMap
subcmd 0x32: ReqCheckPartyInMaze
subcmd 0x33: SyncUsersInfo
subcmd 0x36: SyncUserPartyInfo
subcmd 0x37: SyncLogicThreadCount
subcmd 0x38: ReqCheckEnterMaze
subcmd 0x39: ReqDisconnectUserSync
subcmd 0x40: SyncMaxMazeID
subcmd 0x41: ResCreateMatchingMazeFromGame (Party)
subcmd 0x42: ResCreateMatchingMazeFromGame (Force)
subcmd 0x43: ReqCreateMatchingMazeFromCommunity
subcmd 0x49: ReqCreateMatchingModeMazeFromCommunity
subcmd 0x50: ReqMyRoomEnterReq
subcmd 0x51: ReqMyRoomEnterRes
subcmd 0x52: ReqMyRoomCreate
subcmd 0x53: ReqMyRoomDelete
subcmd 0x54: EnterOtherMap_cheat
subcmd 0x55: PartyMazeSync
subcmd 0x56: ReqPostSend
subcmd 0x58: ReqFindUser
subcmd 0x59: ResMyRoomDelete
subcmd 0x63: ForceMazeSync
subcmd 0x73: ResCreateModeMaze
subcmd 0x75: ReqUpdateRouletteEvent
subcmd 0x77: ReqReEnterMap
```

## CUserProcess::Parse 子命令映射 (IDA 0x1400450E0)
```
main = 0xF3
subcmd 0x01: SyncSelectCharacter
subcmd 0x03: SyncLogoutUser
subcmd 0x04: SyncUpdateUserMap
subcmd 0x07: SyncUserKickout
subcmd 0x11: ReqUserChatNotice
subcmd 0x12: ReqUserChangeServer
subcmd 0x13: SyncUserMoneyLog
subcmd 0x16: ReqUserEnterPartyMaze
subcmd 0x17: ReqUserChatMegaPhone
subcmd 0x20: ReqUserEnterForceMaze
subcmd 0x26: ReqUserTradePasswordStateSync
subcmd 0x27: ReqUserTradePasswordState
subcmd 0x31: ReqNameChange
subcmd 0x32: ReqCheckSessionID
subcmd 0x35: ReqUserUpdateAuthType
subcmd 0x60: ReqGameServerEnterUser
```

## CGMToolProcess::Parse 子命令映射 (IDA 0x140034640)
```
main = 0xF8
subcmd 0x01: ReqGMUserKick
subcmd 0x02: ReqGMNotice
subcmd 0x03: ReqGMShutDwon
subcmd 0x04: ReqGMTimeEvent
subcmd 0x05: ReqGMValueEvent
subcmd 0x06: ReqGMServerOption
subcmd 0x07: ReqGMCashShopBanner
```

## CWorldModeProcess::Parse 子命令映射 (IDA 0x14004D5F0)
```
main = 0xFB
subcmd 0x02: ReqWorldModeUpdate
subcmd 0x06: ReqWorldModeCommand
subcmd 0x07: ReqWorldModeComplete
subcmd 0x09: ReqWorldModeEnterList
```

---

## IDA 类型结构验证追加 (2026-04-30 11:03 +08:00)

### CChannelOfMap (64 bytes, IDA verified)

| 偏移 | 字段名 | 类型 | 大小 |
|------|--------|------|------|
| 0x00 | m_uxMapID | UXMapID | 8 |
| 0x08 | m_pServer | CServer* | 8 |
| 0x10 | m_nState | int | 4 |
| 0x14 | m_nUserCount | int | 4 |
| 0x18 | m_nMaxUserCount | int | 4 |
| 0x20 | m_mapSelectUser | std::map<int,__int64> | 32 |

关键方法:
- AddUserCount(nUserID, nAdd): m_nUserCount += nAdd, 清理超时 m_mapSelectUser
- AddSelectUser(nUserID): m_mapSelectUser[nUserID] = GetTickCount64 + 300000
- IsOKToEnter(): return m_nMaxUserCount > m_nUserCount
- GetServerState(): 通过 m_pServer->GetServerState
- ChangeChannelState(): 根据用户数/状态自动更新

### CMapWithChannel (88 bytes, IDA verified)

| 偏移 | 字段名 | 类型 | 大小 |
|------|--------|------|------|
| 0x00 | __vftable | CMapWithChannel_vtbl* | 8 |
| 0x08 | m_mapChannel | std::map<int,shared_ptr<CChannelOfMap>> | 32 |
| 0x28 | m_mapChannelForServer | std::map<int,ST_CHANNEL_MAP_INFO*> | 32 |
| 0x48 | m_nTableID | int | 4 |
| 0x4C | m_nMaxChannelCount | int | 4 |
| 0x50 | m_nChannel_Index | int | 4 |
| 0x54 | m_nStartChannel | int | 4 |

关键方法:
- AddChannel(pServer, uxMapID, nMax, nCur): 创建 CChannelOfMap 插入双容器
- FindChannel(dwServerID, stPartyInfo, dwActorID, bRaid): Party->ServerID->UserCount 三级查找
- DeleteChannel(pServer): 遍历清理匹配服务器的频道
- GetCount(): m_mapChannel.size()
- SendDBAddChannelMap: 发送 DB 包 (0x43, 0)
- SendServerAddChannelMap: 发送包 0xF2/0x30

### CDayEventMgr (56 bytes, IDA verified)

| 偏移 | 字段名 | 类型 | 大小 |
|------|--------|------|------|
| 0x00 | m_dw64Tick | uint64 | 8 |
| 0x08 | m_tLastSetTime | ATL::CTime | 8 |
| 0x10 | m_rwEventLock | CFSRWLock | 8 |
| 0x18 | m_mapDayEvent | std::map<ushort,ST_DAY_EVENT_INFO> | 32 |

关键逻辑:
- Update: 60秒 tick, 9点检查更新
- SetDayEvent: 每日9点根据 DayOfWeek 配置事件
- SendDayEventAllGameServer: 发送包 0xF2/0x66

### ST_DAY_EVENT_INFO (4 bytes, IDA verified)

| 偏移 | 字段名 | 类型 | 大小 |
|------|--------|------|------|
| 0x00 | wMazeID | uint16 | 2 |
| 0x02 | wBoosterID | uint16 | 2 |

### ST_CREATE_MAZE_FOR_RELAY (64 bytes, IDA verified)

| 偏移 | 字段名 | 类型 | 大小 |
|------|--------|------|------|
| 0x00 | pReqChannelServer | CServer* | 8 |
| 0x08 | pReqMazeServer | CServer* | 8 |
| 0x10 | stPartyInfo | ST_PARTY_INFO | 8 |
| 0x18 | vecEnterMember | vector<ST_ENTER_MAZE_MEMBER_INFO> | 32 |
| 0x38 | dwTime | uint32 | 4 |

### ST_CREATE_MAZE (696 bytes, IDA verified)

| 偏移 | 字段名 | 类型 | 大小 |
|------|--------|------|------|
| 0x00 | stMapInfo | ST_MAP_INFO | 600 |
| 0x258 | wReqMapID | uint16 | 2 |
| 0x25C | stPartyInfo | ST_PARTY_INFO | 8 |
| 0x268 | uxParentMazeID | UXMapID | 8 |
| 0x270 | stEnterDistrictPos | STPosInfo | 32 |
| 0x290 | vecEnterMember | vector<ST_ENTER_MAZE_MEMBER_INFO> | 32 |
| 0x2B0 | nResult | int | 4 |
| 0x2B4 | nCreateType | int | 4 |


---

## 2026-05-01 04:30 验证追加

本轮验证 CMyRoom、CForce、CPartyManager、XGameDBSocketMgr 等类型：

| 类型名 | 字段数 | 大小(bytes) | 状态 | 来源 |
|------|--------|-------------|------|------|
| CForce | 4 | 48 | verified | IDA + 源码 |
| CParty | 4 | 48 | verified | IDA + 源码 |
| CPartyManager | 2 | 40 | verified | IDA + 源码 |
| CForceManager | 2 | 40 | verified | IDA + 源码 |
| XGameDBSocketMgr | 10 | 72 | verified | IDA + 源码 |
| CGameDBSocket | 5 | - | verified | IDA + 源码 |
| CServerProcess | 37+ | - | verified | IDA + 源码 |
| CUserProcess | 17+ | - | verified | IDA + 源码 |

## CForce / CParty 成员布局 (IDA - 48 bytes / 0x30)
```
+0x00: m_nForceID / m_nPartyID (int, 4 bytes)
+0x04: padding (4 bytes)
+0x08: m_uxMapID (UXMapID, 8 bytes)
+0x10: m_mapMemberInfo (std::map<int, UXMapID>, 32 bytes)
Total: 48 bytes (0x30)
```

## CPartyManager / CForceManager 成员布局 (IDA - 40 bytes / 0x28)
```
+0x00: m_bLoad (bool, 1 byte)
+0x01: padding (7 bytes)
+0x08: m_mapParty / m_mapForce (std::map<int, shared_ptr>, 32 bytes)
Total: 40 bytes (0x28)
```

## XGameDBSocketMgr 成员布局 (IDA - 72 bytes / 0x48)
```
+0x00: vtable* (8 bytes)
+0x08: m_pAccountDBAgent* (8 bytes)
+0x10: m_pGameDBAgent* (8 bytes)
+0x18: m_pLogDBAgent* (8 bytes)
+0x20: m_pStatisticsDBAgent* (8 bytes)
+0x28: m_pSGLogDBAgent* (8 bytes)
+0x30: m_nAccountAgentCnt (4 bytes)
+0x34: m_nGameAgentCnt (4 bytes)
+0x38: m_nLogAgentCnt (4 bytes)
+0x3C: m_nStatisticsAgentCnt (4 bytes)
+0x40: m_nSGLogAgentCnt (4 bytes)
+0x44: padding (4 bytes)
Total: 72 bytes (0x48)
```

## CServerProcess / CUserProcess 关键发现

- CServerProcess: main=0xF2, 37 个子命令处理方法
- CUserProcess: main=0xF3, 17 个子命令处理方法
- 构造函数: TXProcess<CServer> 基类初始化 + SetCmd/SetName

