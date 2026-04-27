# ControlServer.exe 函数索引

## 概述

- 总函数数：6797
- IDA 实例：Port 10001
- 更新时间：2026-04-28
- 索引覆盖：核心 + 选定外围

## 核心类函数统计

| 类名 | 函数数 | 状态 |
|------|--------|------|
| CChannelOfMap | ~10 | pending |
| CMapWithChannel | ~20 | pending |
| CWorldManager | ~15 | pending |
| XControlServer | ~? | pending |
| CWorldMode | ~? | pending |
| CMazeInfo | ~? | pending |
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
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::AddUserCount | 0x140001000 | pending | IDA |
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::AddSelectUser | 0x1400011C0 | pending | IDA |
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::ChangeChannelState | 0x140001270 | pending | IDA |
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::CalculateChannelState | 0x140001410 | pending | IDA |
| XControlServer | ChannelOfMap.cpp | CChannelOfMap::IsOKToEnter | 0x140001600 | pending | IDA |

### CMapWithChannel 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::CMapWithChannel | 0x140001620 | pending | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::~CMapWithChannel | 0x140001730 | pending | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::AddChannel | 0x1400018B0 | pending | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::FindChannel | 0x140001E10 | pending | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::DeleteChannel | 0x1400020A0 | pending | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::SendDBAddChannelMap | 0x1400023C0 | pending | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::SendServerAddChannelMap | 0x140002550 | pending | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::GetChannel | 0x140002610 | pending | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::FindChannelByParty | 0x140005380 | pending | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::FindChannelByServerID | 0x140005630 | pending | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::FindChannelByUserCount | 0x140005820 | pending | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::CheckChannelUserCount | 0x1400058E0 | pending | IDA |
| XControlServer | MapWithChannel.cpp | CMapWithChannel::FindChannelUserLow | 0x140005A40 | pending | IDA |

### CWorldManager 函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | WorldManager.cpp | CWorldManager::CWorldManager | 0x140002710 | pending | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::~CWorldManager | 0x140002760 | pending | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::AddMap | 0x1400027A0 | pending | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::DeleteMap | 0x140002B70 | pending | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::ReqEnterMap | 0x140002CA0 | pending | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::ReqEnterMapToOther | 0x140003D30 | pending | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::ReqChangeChannel | 0x140004720 | pending | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::SendChannelInfoAll | 0x140004F70 | pending | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::UpdateUserMap | 0x1400050E0 | pending | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::AddChannelOfMap | 0x140005230 | pending | IDA |
| XControlServer | WorldManager.cpp | CWorldManager::RemoveChannelOfMap | 0x140005300 | pending | IDA |

### XControlServer 核心函数

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ControlServer.cpp | TXSingleton<XControlServer>::Instance | 0x140006100 | pending | IDA |

### ST_CHANNEL_MAP_INFO 结构相关

| 所属目录 | 文件名 | 函数名 | 地址 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XControlServer | ? | ST_CHANNEL_MAP_INFO::~ST_CHANNEL_MAP_INFO | 0x140001890 | pending | IDA |
| XControlServer | ? | ST_CHANNEL_MAP_INFO::ST_CHANNEL_MAP_INFO | 0x140001DE0 | pending | IDA |

---