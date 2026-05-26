# GameServer.exe Path Recovery Index

| original_lower_path | recovered_pascal_path | file | evidence | confirmed |
| --- | --- | --- | --- | --- |
| Soulworker/GameServer/XGameServer/GameWorldMode.cpp | XGameServer/GameWorldMode.cpp | GameWorldMode.cpp | IDA推断 | yes |
| Soulworker/GameServer/XGameServer/GameWorldMode.h | XGameServer/GameWorldMode.h | GameWorldMode.h | IDA推断 | yes |
| Soulworker/GameServer/XGameServer/Monster.cpp | XGameServer/Monster.cpp | Monster.cpp | IDA source path | yes |
| Soulworker/GameServer/XGameServer/Monster.h | XGameServer/Monster.h | Monster.h | IDA source path | yes |
| Soulworker/GameServer/XGameServer/Ai.cpp | XGameServer/Ai.cpp | Ai.cpp | IDA source path | yes |
| Soulworker/GameServer/XGameServer/Ai.h | XGameServer/Ai.h | Ai.h | IDA source path | yes |
| Soulworker/GameServer/XGameServer/Mover.cpp | XGameServer/Mover.cpp | Mover.cpp | IDA source path | yes |
| Soulworker/GameServer/XGameServer/Mover.h | XGameServer/Mover.h | Mover.h | IDA source path | yes |
| Soulworker/GameServer/XGameServer/MoverEx.cpp | XGameServer/MoverEx.cpp | MoverEx.cpp | IDA source path | yes |
| Soulworker/GameServer/XGameServer/MoverEx.h | XGameServer/MoverEx.h | MoverEx.h | IDA source path | yes |
| Soulworker/GameServer/XGameServer/User.cpp | XGameServer/User.cpp | User.cpp | IDA source path | yes |
| Soulworker/GameServer/XGameServer/User.h | XGameServer/User.h | User.h | IDA source path | yes |
| Soulworker/GameServer/XGameServer/BattleZone.cpp | XGameServer/BattleZone.cpp | BattleZone.cpp | IDA source path | yes |
| Soulworker/GameServer/XGameServer/BattleZone.h | XGameServer/BattleZone.h | BattleZone.h | IDA source path | yes |
| Soulworker/GameServer/XGameServer/GroupAggro.cpp | XGameServer/GroupAggro.cpp | GroupAggro.cpp | IDA source path | yes |
| Soulworker/GameServer/XGameServer/GroupAggro.h | XGameServer/GroupAggro.h | GroupAggro.h | IDA source path | yes |
| Soulworker/GameServer/XGameServer/STMonsterInfo.h | XGameServer/STMonsterInfo.h | STMonsterInfo.h | IDA source path | yes |
| Soulworker/GameServer/XGameServer/MySkillList.cpp | XGameServer/MySkillList.cpp | MySkillList.cpp | IDA source path | yes |
| Soulworker/GameServer/XGameServer/MySkillList.h | XGameServer/MySkillList.h | MySkillList.h | IDA source path | yes |
| Soulworker/GameServer/XGameServer/Npc.h | XGameServer/Npc.h | Npc.h | IDA source path | yes |
| Soulworker/GameServer/XGameServer/Xigncode.cpp | XGameServer/Xigncode.cpp | Xigncode.cpp | IDA source path | yes |
| Soulworker/GameServer/XGameServer/Xigncode.h | XGameServer/Xigncode.h | Xigncode.h | IDA source path | yes |
| Soulworker/GameServer/XGameServer/Achieve.cpp | XGameServer/Achieve.cpp | Achieve.cpp | IDA source path | yes |
| Soulworker/GameServer/XGameServer/Achieve.h | XGameServer/Achieve.h | Achieve.h | IDA source path | yes |
| Soulworker/GameServer/XGameServer/AchieveType.cpp | XGameServer/AchieveType.cpp | AchieveType.cpp | IDA source path | yes |
| Soulworker/GameServer/XGameServer/AchieveType.h | XGameServer/AchieveType.h | AchieveType.h | IDA source path | yes |
| Soulworker/GameServer/XGameServer/ActionResMgr.cpp | XGameServer/ActionResMgr.cpp | ActionResMgr.cpp | IDA source path | yes |
| Soulworker/GameServer/XGameServer/ActionResMgr.h | XGameServer/ActionResMgr.h | ActionResMgr.h | IDA source path | yes |

## Notes

This file records the recovery of original source path ownership for functions/types in GameServer.exe.

Path sources:
1. Source file paths identified by IDA (from .pdb info)
2. Module/unit ownership inference
3. Code structure analysis inference

Status values:
- pending: awaiting analysis/recovery
- verified: verified (path is correct)
- blocked: blocked (cannot determine ownership)
