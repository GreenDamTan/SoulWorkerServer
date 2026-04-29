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

