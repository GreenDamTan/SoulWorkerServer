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

## 路径恢复列表

| 原始小写路径 | 恢复 PascalCase 路径 | 文件名 | 来源依据 | 是否确认 |
| --- | --- | --- | --- | --- |
| (待 PDB 导出) | XControlServer/WorldManager.cpp | WorldManager.cpp | 函数名推断 | 待确认 |
| (待 PDB 导出) | XControlServer/MapWithChannel.cpp | MapWithChannel.cpp | 函数名推断 | 待确认 |
| (待 PDB 导出) | XControlServer/ChannelOfMap.cpp | ChannelOfMap.cpp | 函数名推断 | 待确认 |

---