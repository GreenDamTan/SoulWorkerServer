# LoginServer.exe 类型索引

## 概述

- 更新时间：2026-04-26 23:26 +08:00
- IDA 实例：Port 10003

## 核心类型

| 所属目录 | 文件名 | 类型名 | 字段数 | 当前状态 | 来源 |
| --- | --- | --- | --- | --- | --- |
| XLoginServer | LoginServer.h | XLoginServer | 26 (IDA) | partial | IDA struct 201224 bytes |
| XLoginServer | User.h | CUser | 41 (IDA) | partial | IDA struct 136576 bytes |
| XLoginServer | LoginProcess.h | XLoginProcess | TBD | pending | IDA |
| XLoginServer | CharacterProcess.h | CCharacterProcess | TBD | pending | IDA |
| XLoginServer | SystemProcess.h | CSystemProcess | TBD | pending | IDA |
| XLoginServer | LoginControlSocket.h | CLoginControlSocket | TBD | partial | IDA struct 68208 bytes |
| XLoginServer | GameDBSocket.h | XGameDBSocket | TBD | partial | IDA |

## XLoginServer 成员验证 (IDA offset)

| 成员名 | 偏移 | 大小 | 类型 | 现有代码 | 状态 |
|--------|------|------|------|----------|------|
| TXServer<CUser> 基类 | 0 | 46264 | TXServer<CUser> | 继承 | OK |
| m_xItemFactory | 46272 | 272 | XItemFactory | m_xItemFactory | OK |
| m_xResourceMgr | 46544 | 16216 | XResourceMgr | resourceMgr_ | OK |
| m_xDBAgentMgr | 62760 | 72 | XGameDBSocketMgr | m_xDBAgentMgr | OK |
| m_controlSocket | 62832 | 68208 | CLoginControlSocket | controlSocket_ | OK |
| m_scObserveSocket | 131040 | 69720 | CObserveSocket | m_scObserveSocket | OK |
| m_dwConnectTick | 200760 | 8 | uint64 | m_dwConnectTick | OK |
| m_UserInfos | 200768 | 96 | boost::multi_index | usersByUaid_ (unordered_map) | 差异 |
| m_mapCharacterInfo | 200864 | 32 | std::map | usersByActorId_ (unordered_map) | 差异 |
| m_rwLock | 200928 | 8 | CFSRWLock | usersByUaidLock_ | OK |
| m_rwLock_actor | 200936 | 8 | CFSRWLock | usersByActorIdLock_ | OK |
| m_bAcceptClose | 200944 | 1 | bool | acceptClosed_ | OK |
| m_xignCode | 200952 | 8 | CXigncode | m_xignCode | OK |
| m_xSeed | 200968 | 168 | XSeed | m_xSeed | OK |
| m_nWaitTicket | 201192 | 8 | int64 | m_nWaitTicket | OK |
| m_nLastEnterWaitTicket | 201200 | 8 | int64 | m_nLastEnterWaitTicket | OK |
| m_nSendGameDBUserCount | 201208 | 4 | int | m_nSendGameDBUserCount | OK |
| m_nMaxServerUserCount | 201212 | 4 | int | m_nMaxServerUserCount | OK |
| m_nControlServerUserCount | 201216 | 4 | int | m_nControlServerUserCount | OK |

## CUser 成员验证 (IDA offset)

| 成员名 | 偏移 | 大小 | 类型 | 现有代码 | 状态 |
|--------|------|------|------|----------|------|
| XClient 基类 | 0 | 131512 | XClient | 继承 | OK |
| IXObject | 131512 | 64 | IXObject | 继承 | OK |
| m_rwLock | 131576 | 8 | CFSRWLock | m_rwLock | ✅ 已存在 (User.h:422) |
| m_stUserInfo | 131584 | 40 | STUserInfo | m_stUserInfo | OK |
| m_wCharacterCount | 131624 | 2 | uint16 | m_wCharacterCount | OK |
| m_dwSelectedUCID | 131628 | 4 | uint32 | m_dwSelectedUCID | OK |
| m_eEnterServerState | 131632 | 4 | enum | m_eEnterServerState | OK |
| m_biAuthSessionID | 131680 | 8 | int64 | m_biAuthSessionID | OK |
| m_byGM | 131732 | 1 | uint8 | m_byGM | OK |
| m_szAccountID | 131768 | 21 | char[21] | m_szAccountID | OK |
| m_dw64CheckSessionIDTick | 131800 | 8 | uint64 | m_dw64CheckSessionIDTick | OK |

**验证说明 (2026-04-27 00:01)**：
- `m_rwLock` 成员已存在于 User.h:422，使用 `mutable CFSRWLock m_rwLock{}`
- 其他核心成员位置和名称匹配正确

## 状态说明

- `pending`: 待恢复
- `partial`: 部分恢复
- `verified`: 已验证
- `blocked`: 阻塞中
