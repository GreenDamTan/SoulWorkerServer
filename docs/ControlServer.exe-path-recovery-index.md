# ControlServer.exe 路径恢复索引

## 概述

- IDA 实例：Port 10001
- 更新时间：2026-04-28

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
| (待 PDB 导出) | XControlServer/ControlServer.cpp | ControlServer.cpp | MyRoom 调用链 + 已落地源码 | 已确认 |
| (待 PDB 导出) | XControlServer/ControlServer.h | ControlServer.h | MyRoom 调用链 + 已落地源码 | 已确认 |
| (待 PDB 导出) | XRelayServer/ServerProcess.h | ServerProcess.h | MyRoom 调用链 + 已落地源码 | 已确认 |

---
