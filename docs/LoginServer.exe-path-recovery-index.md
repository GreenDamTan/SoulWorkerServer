# LoginServer.exe 路径恢复索引

## 概述

- 更新时间：2026-04-26 23:20 +08:00
- IDA 实例：Port 10003

## 已恢复路径

| 原始路径 | 恢复 PascalCase 路径 | 文件名 | 来源依据 | 是否确认 |
| --- | --- | --- | --- | --- |
| xloginserver | XLoginServer | LoginServer.h/cpp | PDB + OBJ | 是 |
| xloginserver | XLoginServer | User.h/cpp | PDB + OBJ | 是 |
| xloginserver | XLoginServer | LoginProcess.h/cpp | PDB + OBJ | 是 |
| xloginserver | XLoginServer | CharacterProcess.h/cpp | PDB + OBJ | 是 |
| xloginserver | XLoginServer | SystemProcess.h/cpp | PDB + OBJ | 是 |
| xloginserver | XLoginServer | LoginControlSocket.h/cpp | PDB + OBJ | 是 |
| xloginserver | XLoginServer | GameDBSocket.h/cpp | PDB + OBJ | 是 |
| xloginserver | XLoginServer | ServerMain.cpp | PDB + OBJ | 是 |

## 依赖的共享模块

| 模块名 | 路径 | 用途 |
| --- | --- | --- |
| XCore | GameServer/XCore/XServer | 核心服务器框架 |
| XSCommon | GameServer/XSCommon/Table | 表加载系统 |
| XCommon | Common/XNet/XCommon | 公共协议定义 |

## 目录结构

```
src/F/_PROGRAM_HG/Source/Soulworker/
├── GameServer/
│   ├── XLoginServer/          # LoginServer 专属代码
│   │   ├── LoginServer.h/cpp
│   │   ├── User.h/cpp
│   │   ├── LoginProcess.h/cpp
│   │   ├── CharacterProcess.h/cpp
│   │   ├── SystemProcess.h/cpp
│   │   ├── LoginControlSocket.h/cpp
│   │   ├── GameDBSocket.h/cpp
│   │   └── ServerMain.cpp
│   ├── XCore/XServer/         # 共享核心框架
│   └── XSCommon/Table/        # 共享表加载
└── Common/XNet/XCommon/       # 公共协议
```
