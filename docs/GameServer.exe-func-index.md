# GameServer.exe 函数索引

## 统计摘要
- 总函数数：56722
- 已验证：0
- 待恢复：56722

## 核心架构类
| 类名 | 函数数 | 状态 | 说明 |
|------|--------|------|------|
| XGameServer | 172 | pending | 游戏服务器主类 |
| CUser | 1426 | pending | 用户/玩家类 |
| CBattleZone | 161 | pending | 战斗区域/地图类 |
| CMonster | TBD | pending | 怪物类 |
| CNpc | TBD | pending | NPC 类 |
| CItem | TBD | pending | 物品类 |

| 所属目录 | 文件名 | 函数名 | 当前状态 | 来源 | 是否验证 |
| --- | --- | --- | --- | --- | --- |
| `GameServer/XGameServer` | `GameServer.cpp` | `XGameServer::XGameServer` | pending | IDA 0x1402D86D0 | no |
| `GameServer/XGameServer` | `GameServer.cpp` | `XGameServer::~XGameServer` | pending | IDA 0x1402D8B50 | no |
| `GameServer/XGameServer` | `GameServer.cpp` | `XGameServer::InitServer` | pending | IDA 0x1402D8DE0 | no |
| `GameServer/XGameServer` | `GameServer.cpp` | `XGameServer::Clear` | pending | IDA 0x1402D9900 | no |
| `GameServer/XGameServer` | `GameServer.cpp` | `XGameServer::OnUpdate` | pending | IDA 0x1402DA160 | no |
| `GameServer/XGameServer` | `GameServer.cpp` | `XGameServer::OnAccect` | pending | IDA 0x1402D9B00 | no |
| `GameServer/XGameServer` | `GameServer.cpp` | `XGameServer::EnterUser` | pending | IDA 0x1402D9BD0 | no |
| `GameServer/XGameServer` | `GameServer.cpp` | `XGameServer::ExitUser` | pending | IDA 0x1402D9F30 | no |
| `GameServer/XGameServer` | `BattleZone.cpp` | `CBattleZone::CBattleZone` | pending | IDA 0x14019D2B0 | no |
| `GameServer/XGameServer` | `BattleZone.cpp` | `CBattleZone::~CBattleZone` | pending | IDA 0x14019D4E0 | no |
| `GameServer/XGameServer` | `BattleZone.cpp` | `CBattleZone::Create` | pending | IDA 0x14019D640 | no |
| `GameServer/XGameServer` | `BattleZone.cpp` | `CBattleZone::Clear` | pending | IDA 0x14019DBD0 | no |
| `GameServer/XGameServer` | `BattleZone.cpp` | `CBattleZone::OnUpdate` | pending | IDA 0x14019E1A0 | no |
| `GameServer/XGameServer` | `BattleZone.cpp` | `CBattleZone::ClickInteractionBox` | pending | IDA 0x1401A28F0 | no |
| `GameServer/XGameServer` | `Monster.cpp` | `CMonster` 相关函数 | pending | IDA search needed | no |
