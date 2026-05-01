# ControlServer.exe 路径恢复索引

## 概述

- IDA 实例：Port 10001
- 更新时间：2026-04-29 03:54 +08:00

## 路径恢复规则

根据 promote.md 规则：
1. 禁止直接使用 PDB 导出的全小写路径
2. 优先使用 OBJ 文件名恢复源文件名
3. 目录统一按 PascalCase 恢复

## 预期目录结构

根据初步分析，ControlServer 应属于：

```
src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XControlServer/
```

核心文件可能包括：
- ControlServer.cpp / ControlServer.h
- WorldManager.cpp / WorldManager.h
- MapWithChannel.cpp / MapWithChannel.h
- ChannelOfMap.cpp / ChannelOfMap.h
- ServerMain.cpp

## 说明

- 本轮仅回填已在当前 MyRoom 恢复链中直接使用并核实的路径。
- 其余已落地源码路径后续再按模块统一整理，避免把非本轮内容误记成当前轮已验证范围。

## 路径恢复列表

| 原始小写路径 | 恢复 PascalCase 路径 | 文件名 | 来源依据 | 是否确认 |
| --- | --- | --- | --- | --- |
| (待 PDB 导出) | XControlServer/CMyRoom.cpp | CMyRoom.cpp | 已落地源码 + 函数索引 + IDA | 已确认 |
| (待 PDB 导出) | XControlServer/CMyRoom.h | CMyRoom.h | 已落地源码 + 类型索引 + IDA | 已确认 |
| (待 PDB 导出) | XControlServer/MazeInfo.cpp | MazeInfo.cpp | 已落地源码 + 函数索引 + IDA | 已确认 |
| (待 PDB 导出) | XControlServer/MazeInfo.h | MazeInfo.h | 已落地源码 + 类型索引 + IDA | 已确认 |
| (待 PDB 导出) | XControlServer/ControlServer.cpp | ControlServer.cpp | MyRoom/MazeInfo 调用链 + 已落地源码 | 已确认 |
| (待 PDB 导出) | XControlServer/ControlServer.h | ControlServer.h | MyRoom/MazeInfo 调用链 + 已落地源码 | 已确认 |
| (待 PDB 导出) | XControlServer/WorldMode.cpp | WorldMode.cpp | 已落地源码 + 函数索引 + IDA | 已确认 |
| (待 PDB 导出) | XControlServer/WorldMode.h | WorldMode.h | 已落地源码 + 类型索引 + IDA | 已确认 |
| (待 PDB 导出) | XControlServer/WorldModeMgr.cpp | WorldModeMgr.cpp | 已落地源码 + 函数索引 + IDA | 已确认 |
| (待 PDB 导出) | XControlServer/WorldModeMgr.h | WorldModeMgr.h | 已落地源码 + 类型索引 + IDA | 已确认 |
| (待 PDB 导出) | XRelayServer/ServerProcess.h | ServerProcess.h | MyRoom/MazeInfo 调用链 + 已落地源码 | 已确认 |
| (待 PDB 导出) | XCore/XServer/GreenDamTan_MyRoomStructs.h | GreenDamTan_MyRoomStructs.h | 人工补充类型定义 + 已落地源码 | 已确认 |
| (待 PDB 导出) | XControlServer/UserObject.cpp | UserObject.cpp | 已落地源码 + 函数索引 + IDA | 已确认 |
| (待 PDB 导出) | XControlServer/UserObject.h | UserObject.h | 已落地源码 + 类型索引 + IDA | 已确认 |
| (待 PDB 导出) | XControlServer/WorldManager.cpp | WorldManager.cpp | 已落地源码 + 函数索引 + IDA | 已确认 |
| (待 PDB 导出) | XControlServer/WorldManager.h | WorldManager.h | 已落地源码 + 类型索引 + IDA | 已确认 |

---
| (待 PDB 导出) | XControlServer/CParty.h | CParty.h | 已落地源码 + 类型索引 + IDA | 已确认 |
| (待 PDB 导出) | XControlServer/CForce.h | CForce.h | 已落地源码 + 类型索引 + IDA | 已确认 |
| (待 PDB 导出) | XControlServer/CPartyManager.h | CPartyManager.h | 已落地源码 + 类型索引 + IDA | 已确认 |
| (待 PDB 导出) | XControlServer/CForceManager.h | CForceManager.h | 已落地源码 + 类型索引 + IDA | 已确认 |

---
| (待 PDB 导出) | XControlServer/CParty.cpp | CParty.cpp | 已落地源码 + 函数索引 + IDA | 已确认 |
| (待 PDB 导出) | XControlServer/CForce.cpp | CForce.cpp | 已落地源码 + 函数索引 + IDA | 已确认 |
| (待 PDB 导出) | XControlServer/CPartyManager.cpp | CPartyManager.cpp | 已落地源码 + 函数索引 + IDA | 已确认 |
| (待 PDB 导出) | XControlServer/CForceManager.cpp | CForceManager.cpp | 已落地源码 + 函数索引 + IDA | 已确认 |

---

| (待 PDB 导出) | XControlServer/ServerProcess.cpp | ServerProcess.cpp | 已落地源码 + 函数索引 + IDA | 已确认 |
| (待 PDB 导出) | XControlServer/CServer.cpp | CServer.cpp | 已落地源码 + 函数索引 + IDA | 已确认 |
| (待 PDB 导出) | XControlServer/GMToolProcess.cpp | GMToolProcess.cpp | 已落地源码 + 函数索引 + IDA | 已确认 |
| (待 PDB 导出) | XControlServer/UserProcess.cpp | UserProcess.cpp | 已落地源码 + 函数索引 + IDA | 已确认 |
| (待 PDB 导出) | XControlServer/WorldModeProcess.cpp | WorldModeProcess.cpp | 已落地源码 + 函数索引 + IDA | 已确认 |
| (待 PDB 导出) | XControlServer/ChannelOfMap.cpp | ChannelOfMap.cpp | 已落地源码 + 函数索引 + IDA | 已确认 |
| (待 PDB 导出) | XControlServer/MapWithChannel.cpp | MapWithChannel.cpp | 已落地源码 + 函数索引 + IDA | 已确认 |

---

---

### ServerProcess 相关文件验证追加 (2026-04-29 05:52)

| 原始小写路径 | 恢复 PascalCase 路径 | 文件名 | 来源依据 | 是否确认 |
| --- | --- | --- | --- | --- |
| (待 PDB 导出) | XRelayServer/ServerProcess.cpp | ServerProcess.cpp | IDA 函数索引 + 已落地源码 | 已确认 |
| (待 PDB 导出) | XRelayServer/ServerProcess.h | ServerProcess.h | IDA 类型索引 + 已落地源码 | 已确认 |

说明：
- `CServerProcess` 类包含 37 个子命令处理方法，对应 `main=0xF2` 的所有子命令
- `CServer` 类包含迷宫管理方法
- 所有 TODO 占位已在本轮移除并替换为 IDA 验证实现

---


### 时间兼容层文件追加 (2026-04-29 06:08)

| 原始小写路径 | 恢复 PascalCase 路径 | 文件名 | 来源依据 | 是否确认 |
| --- | --- | --- | --- | --- |
| (人工补充) | XCore/XServer/GreenDamTan_TimeCompat.h | GreenDamTan_TimeCompat.h | 人工补充跨平台兼容层 | 已确认 |

说明：
- `GreenDamTan_TimeCompat.h` 提供跨平台时间工具函数
- 用于替换原始代码中的 `ATL::CTime` 和 `GetTickCount64` 调用
- 符合 `GreenDamTan_` 前缀规范

---

### 路径验证追加 (2026-04-29 10:56 +08:00)

本轮验证以下路径已确认：

| 原始小写路径 | 恢复 PascalCase 路径 | 文件名 | 来源依据 | 是否确认 |
| --- | --- | --- | --- | --- |
| xcontrolserver/cmyroom.cpp | XControlServer/CMyRoom.cpp | CMyRoom.cpp | IDA 函数索引 + 已落地源码 | 已确认 |
| xcontrolserver/cmyroom.h | XControlServer/CMyRoom.h | CMyRoom.h | IDA 类型索引 + 已落地源码 | 已确认 |
| xcontrolserver/worldmode.cpp | XControlServer/WorldMode.cpp | WorldMode.cpp | IDA 函数索引 + 已落地源码 | 已确认 |
| xcontrolserver/worldmode.h | XControlServer/WorldMode.h | WorldMode.h | IDA 类型索引 + 已落地源码 | 已确认 |
| xcontrolserver/worldmodemgr.cpp | XControlServer/WorldModeMgr.cpp | WorldModeMgr.cpp | IDA 函数索引 + 已落地源码 | 已确认 |
| xcontrolserver/worldmodemgr.h | XControlServer/WorldModeMgr.h | WorldModeMgr.h | IDA 类型索引 + 已落地源码 | 已确认 |
| xcontrolserver/cparty.h | XControlServer/CParty.h | CParty.h | IDA 类型索引 + 已落地源码 | 已确认 |
| xcontrolserver/cforce.h | XControlServer/CForce.h | CForce.h | IDA 类型索引 + 已落地源码 | 已确认 |

说明：
- CMyRoom 状态机：NONE(0) -> READY(1) -> DELETE(2)，EnterSucc 允许 DELETE->READY 恢复
- CWorldMode 状态机：state 0=idle, 1=running, 2=completed
- CWorldModeMgr Update tick 间隔 333ms
- CServer 迷宫管理：AddMaze/RemoveMaze/UpdateMaze/SyncMaze/OnUpdate

---


### 路径验证追加 (2026-04-29 13:42 +08:00)

本轮验证以下路径已确认：

| 原始小写路径 | 恢复 PascalCase 路径 | 文件名 | 来源依据 | 是否确认 |
| --- | --- | --- | --- | --- |
| xcontrolserver/cforcemanager.cpp | XControlServer/CForceManager.cpp | CForceManager.cpp | IDA 函数索引 + 已落地源码 | 已确认 |
| xcontrolserver/cforcemanager.h | XControlServer/CForceManager.h | CForceManager.h | IDA 类型索引 + 已落地源码 | 已确认 |
| xcontrolserver/cpartymanager.cpp | XControlServer/CPartyManager.cpp | CPartyManager.cpp | IDA 函数索引 + 已落地源码 | 已确认 |
| xcontrolserver/cpartymanager.h | XControlServer/CPartyManager.h | CPartyManager.h | IDA 类型索引 + 已落地源码 | 已确认 |
| xcontrolserver/cparty.cpp | XControlServer/CParty.cpp | CParty.cpp | IDA 函数索引 + 已落地源码 | 已确认 |
| xcontrolserver/cforce.cpp | XControlServer/CForce.cpp | CForce.cpp | IDA 函数索引 + 已落地源码 | 已确认 |

说明：
- CForceManager (40 bytes): m_bLoad + m_mapForce
- CForce (48 bytes): m_nForceID + m_uxMapID + m_mapMemberInfo
- CPartyManager (40 bytes): m_bLoad + m_mapParty
- CParty (48 bytes): m_nPartyID + m_uxMapID + m_mapMemberInfo
- CParty::IsFull 和 CForce::IsFull 共享实现 (size >= 4)
- IDA 类型混淆：CForceManager 方法显示为 CPartyManager，CForce 显示为 CParty

---


### Process 类路径验证追加 (2026-04-29 14:34 +08:00)

本轮验证以下路径已确认：

| 原始小写路径 | 恢复 PascalCase 路径 | 文件名 | 来源依据 | 是否确认 |
| --- | --- | --- | --- | --- |
| xcontrolserver/serverprocess.cpp | XControlServer/ServerProcess.cpp | ServerProcess.cpp | IDA 函数索引 + 已落地源码 | 已确认 |
| xcontrolserver/userprocess.cpp | XControlServer/UserProcess.cpp | UserProcess.cpp | IDA 函数索引 + 已落地源码 | 已确认 |
| xcontrolserver/gmtoolprocess.cpp | XControlServer/GMToolProcess.cpp | GMToolProcess.cpp | IDA 函数索引 + 已落地源码 | 已确认 |
| xcontrolserver/worldmodeprocess.cpp | XControlServer/WorldModeProcess.cpp | WorldModeProcess.cpp | IDA 函数索引 + 已落地源码 | 已确认 |
| xrelayserver/serverprocess.h | XRelayServer/ServerProcess.h | ServerProcess.h | IDA 类型索引 + 已落地源码 | 已确认 |

说明：
- Process 类继承自 TXProcess<CServer>，大小均为 72 bytes
- CServerProcess::Parse 处理 main=0xF2 (33 个子命令)
- CUserProcess::Parse 处理 main=0xF3 (17 个子命令)
- CGMToolProcess::Parse 处理 main=0xF8 (7 个子命令)
- CWorldModeProcess::Parse 处理 main=0xFB (4 个子命令)
- Parse 方法必须使用 `xPacket.GetSubCmd()` 而非从流读取

---


### CServer.cpp / CUserObject.cpp 路径验证追加 (2026-04-29 18:55 +08:00)

本轮验证以下路径已确认：

| 原始小写路径 | 恢复 PascalCase 路径 | 文件名 | 来源依据 | 是否确认 |
| --- | --- | --- | --- | --- |
| xcontrolserver/cserver.cpp | XControlServer/CServer.cpp | CServer.cpp | IDA 函数地址 + 已落地源码 | 已确认 |
| xcontrolserver/userobject.cpp | XControlServer/UserObject.cpp | UserObject.cpp | IDA 函数地址 + 已落地源码 | 已确认 |

说明：
- CServer::OnUpdate @ 0x140042550 遍历 m_mapMazeInfo，销毁时发送 0xF2/0x25
- CServer::RemoveMaze @ 0x140041930 重置父/子迷宫，处理 Party/Force maze ID
- CServer::SyncMaze @ 0x140042340 同步迷宫信息，处理最后同步包时递增计数
- CServer::RegisterProcess @ 0x140041180 注册 CServerProcess/CUserProcess/CGMToolProcess/CWorldModeProcess
- CUserObject::CanChangeWorld @ 0x140044B60 三条件检查: !m_nWorldState && tick <= GetTickCount64 && !m_uxTransMapID.nMapID
- CUserObject::ResetWorldState @ 0x140044BB0 重置状态并检查数据一致性

---

### CMyRoom.cpp 路径验证追加 (2026-04-30 03:08 +08:00)

本轮验证以下路径已确认：

| 原始小写路径 | 恢复 PascalCase 路径 | 文件名 | 来源依据 | 是否确认 |
| --- | --- | --- | --- | --- |
| xcontrolserver/cmyroom.cpp | XControlServer/CMyRoom.cpp | CMyRoom.cpp | IDA 函数地址 + 已落地源码 | 已确认 |
| xcontrolserver/cmyroom.h | XControlServer/CMyRoom.h | CMyRoom.h | IDA struct_info + 已落地源码 | 已确认 |
| xrelayserver/serverprocess.h | XRelayServer/ServerProcess.h | ServerProcess.h | IDA CServer MyRoom 函数 | 已确认 |

说明：
- CMyRoom::CMyRoom @ 0x1400395F0 - 构造函数初始化 m_stOwnerInfo (memset 68 bytes)
- CMyRoom::CreateMyRoom @ 0x140039820 - qmemcpy(this, stOwnerInfo, 68); m_pServer/p; m_byState=0
- CMyRoom::RoomEnterUser @ 0x140039690 - 状态检查: READY(1) 发送 0xF2/0x50, 否则 0xF2/0x51 + error(100)
- CServer::CreateMyRoom @ 0x1400427C0 - GetMapID, new CMyRoom(96), insert m_mpMyRoomInfo, 发送 0xF2/0x52
- CServer::FindMyRoom @ 0x140042A30 - m_mpMyRoomInfo.find(dwUAID)
- CServer::DelMyRoom @ 0x140042A00 - m_mpMyRoomInfo.erase(dwUAID)
- XControlServer::MyRoomEnterReq @ 0x140012050 - FindMyRoom: 不存在则 CreateMyRoom, 存在则 RoomEnterUser

---


### CRouletteEventMgr 路径验证追加 (2026-04-30 04:36 +08:00)

本轮验证以下路径已确认：

| 原始小写路径 | 恢复 PascalCase 路径 | 文件名 | 来源依据 | 是否确认 |
| --- | --- | --- | --- | --- |
| xcontrolserver/crouletteeventmgr.cpp | XControlServer/CRouletteEventMgr.cpp | CRouletteEventMgr.cpp | IDA 函数地址 + 已落地源码 | 已确认 |
| xcontrolserver/crouletteeventmgr.h | XControlServer/CRouletteEventMgr.h | CRouletteEventMgr.h | IDA struct_info + 已落地源码 | 已确认 |

说明：
- CRouletteEventMgr::CRouletteEventMgr @ 0x14003A4C0 - 构造函数初始化 m_dw64Tick=0, m_bLoad=false
- CRouletteEventMgr::~CRouletteEventMgr @ 0x14003A530 - 调用 Clear()
- CRouletteEventMgr::Update @ 0x14003AC10 - 60秒更新间隔，首次调用 LoadRouletteEvent + SendDBRouletteRewardInfo
- CRouletteEventMgr::LoadRouletteEvent @ 0x14003A5A0 - 版本检查，m_bLoad ? CheckItemDayCount : operator=
- CRouletteEventMgr::SendRouletteEvent @ 0x14003AD30 - XSendPacket(0xF2, 0x74)
- CRouletteEventMgr::UpdateRouletteItemInfo @ 0x14003ADE0 - 按 nRewardID 匹配更新 nCurCount
- CRouletteEventMgr::SetRouletteRewardInfo @ 0x14003AE90 - 遍历 vecInfo 更新计数，完成后 SendRouletteEvent(true)

---

### XLoginServer/PartyManager 路径验证追加 (2026-04-30 05:12 +08:00)

本轮验证以下路径已确认：

| 原始小写路径 | 恢复 PascalCase 路径 | 文件名 | 来源依据 | 是否确认 |
| --- | --- | --- | --- | --- |
| xloginserver/partymanager.cpp | XLoginServer/PartyManager.cpp | PartyManager.cpp | IDA 函数地址 + CMakeLists + 已落地源码 | 已确认 |
| xloginserver/partymanager.h | XLoginServer/PartyManager.h | PartyManager.h | IDA struct_info + CMakeLists + 已落地源码 | 已确认 |

说明：
- CMakeLists.txt 显式引用 PartyManager.cpp from XLoginServer for ControlServer target
- XLoginServer 实现使用继承设计: CForce 继承 CParty, CForceManager 继承 CPartyManager
- CForceManager 复用 m_mapParty 存储数据 (IDA 显示 m_mapForce，但源码使用继承访问)
- CPartyManager::SetMember @ 0x1400399A0 - 查找或创建 Party，调用 SetMember
- CForceManager::SetMember @ 0x140030CF0 - 查找或创建 Force，调用 SetMember
- CForceManager::IsFull @ 0x140030EE0 - 查找 Force 并调用 IsFull
- CForceManager::GetMazeID @ 0x140039CD0 - nActorID 参数未使用，获取 MazeID
- CForce::CForce @ 0x14003A450 - 构造函数，初始化 m_nForceID + m_uxMapID + m_mapMemberInfo
- CForce::SetMember @ 0x1400398D0 - 查找或插入 m_mapMemberInfo
- CForce::IsEmpty @ 0x140039980 - return m_mapMemberInfo.empty()
- CForce::SetMazeID @ 0x14003A420 - UXMapID 复制

---

### CRouletteEventMgr 路径验证追加 (2026-04-30 06:05 +08:00)

本轮验证以下路径已确认：

| 原始小写路径 | 恢复 PascalCase 路径 | 文件名 | 来源依据 | 是否确认 |
| --- | --- | --- | --- | --- |
| xcontrolserver/crouletteeventmgr.cpp | XControlServer/CRouletteEventMgr.cpp | CRouletteEventMgr.cpp | IDA 函数地址 + 已落地源码 | 已确认 |
| xcontrolserver/crouletteeventmgr.h | XControlServer/CRouletteEventMgr.h | CRouletteEventMgr.h | IDA struct_info + 已落地源码 | 已确认 |

说明：
- CRouletteEventMgr::CRouletteEventMgr @ 0x14003A4C0 - 构造函数初始化 CFSRWLock + PS_GM_ROULETTE_EVENT + Clear()
- CRouletteEventMgr::~CRouletteEventMgr @ 0x14003A530 - Clear() + 析构 PS_GM_ROULETTE_EVENT
- CRouletteEventMgr::Update @ 0x14003AC10 - 60秒更新间隔，首次 LoadRouletteEvent + SendDBRouletteRewardInfo
- CRouletteEventMgr::LoadRouletteEvent @ 0x14003A5A0 - 版本检查，m_bLoad ? CheckItemDayCount : operator=
- CRouletteEventMgr::SendRouletteEvent @ 0x14003AD30 - XSendPacket(0xF2, 0x74)
- CRouletteEventMgr::UpdateRouletteItemInfo @ 0x14003ADE0 - 按 nRewardID 匹配更新 nCurCount

---

---

### 事件管理器路径验证追加 (2026-04-30 06:40 +08:00)

本轮验证以下路径已确认：

| 原始小写路径 | 恢复 PascalCase 路径 | 文件名 | 来源依据 | 是否确认 |
| --- | --- | --- | --- | --- |
| xloginserver/dayeventmanager.cpp | XLoginServer/DayEventManager.cpp | DayEventManager.cpp | IDA 函数地址 + 已落地源码 | 已确认 |
| xloginserver/dayeventmanager.h | XLoginServer/DayEventManager.h | DayEventManager.h | IDA struct_info + 已落地源码 | 已确认 |
| xloginserver/rouletteeventmanager.cpp | XLoginServer/RouletteEventManager.cpp | RouletteEventManager.cpp | IDA 函数地址 + 已落地源码 | 已确认 |
| xloginserver/rouletteeventmanager.h | XLoginServer/RouletteEventManager.h | RouletteEventManager.h | IDA struct_info + 已落地源码 | 已确认 |

说明：
- CDayEventMgr::Update @ 0x14002D9A0 - 60秒 tick，9点更新检查
- CDayEventMgr::SetDayEvent @ 0x14002DAF0 - 复杂事件配置逻辑
- CRouletteEventMgr::Update @ 0x14003AC10 - 60秒 tick，版本检查
- CRouletteEventMgr::LoadRouletteEvent @ 0x14003A5A0 - 版本比较 + CheckItemDayCount
- 事件管理器代码位于 XLoginServer 目录，ControlServer 通过 XControlServer 成员使用


---

### 2026-05-01 04:30 验证追加

| 原始小写路径 | 恢复 PascalCase 路径 | 文件名 | 来源依据 | 是否确认 |
| --- | --- | --- | --- | --- |
| (待 PDB 导出) | XControlServer/ServerProcess.h | ServerProcess.h | IDA 0x14003C9D0 + 已落地源码 | 已确认 |
| (待 PDB 导出) | XControlServer/UserProcess.h | UserProcess.h | IDA 0x140044FF0 + 已落地源码 | 已确认 |
| (待 PDB 导出) | XCore/XServer/XGameDBSocketMgr.h | XGameDBSocketMgr.h | IDA 0x140034290 + 已落地源码 | 已确认 |

说明：
- `CServerProcess::Parse` (0x14003CAC0): 37 个子命令路由，main=0xF2
- `CUserProcess::Parse` (0x1400450E0): 17 个子命令路由，main=0xF3
- `XGameDBSocketMgr::IsAccountDBConnectionAll` (0x140034410): 遍历检查连接状态
- `XGameDBSocketMgr::SendAccountDBAgent` (0x140034290): 状态检查后发送

