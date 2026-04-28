# ControlServer.exe 类型索引

## 概述

- IDA 实例：Port 10001
- 更新时间：2026-04-28

## 说明

- 本轮仅核实并回填 `CMyRoom` 类型条目。
- 其余核心类型虽然已有历史源码/IDA 证据，但索引同步长期滞后，后续应单独按类型恢复轮次继续整理，当前不在本轮强行扩大为 `verified`。

## 核心类型统计

| 类型名 | 字段数 | 状态 | 来源 |
|------|--------|------|------|
| CChannelOfMap | ? | pending | IDA |
| CMapWithChannel | ? | pending | IDA |
| CWorldManager | ? | pending | IDA |
| XControlServer | ? | pending | IDA |
| CWorldMode | ? | pending | IDA |
| CMazeInfo | ? | pending | IDA |
| CMyRoom | 4 | verified | IDA + 源码 |
| ST_CHANNEL_MAP_INFO | ? | pending | IDA |
| ST_CHANNEL_INFO | ? | pending | IDA |
| ST_ENTER_WORLD_MODE_INFO | ? | pending | IDA |
| PS_CHANNEL_INFO | ? | pending | IDA |

## 类型列表

| 所属目录 | 文件名 | 类型名 | 字段数 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | CMyRoom.h | CMyRoom | 4 | verified | IDA + 源码 |
| XControlServer | ? | CChannelOfMap | ? | pending | IDA |
| XControlServer | ? | CMapWithChannel | ? | pending | IDA |
| XControlServer | ? | CWorldManager | ? | pending | IDA |
| XControlServer | ? | XControlServer | ? | pending | IDA |
| XControlServer | ? | CMazeInfo | ? | pending | IDA |
| XControlServer | ? | ST_CHANNEL_MAP_INFO | ? | pending | IDA |
| XControlServer | ? | ST_CHANNEL_INFO | ? | pending | IDA |
| XControlServer | ? | ST_ENTER_WORLD_MODE_INFO | ? | pending | IDA |

---
