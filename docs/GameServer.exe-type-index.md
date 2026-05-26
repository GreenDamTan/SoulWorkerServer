# GameServer.exe Type Index

| directory | file | type | size | status | source | verified | verification |
| --- | --- | --- | --- | --- | --- | --- | --- |
| XGameServer | Monster.h | tagDamageMeter | 8 | verified | IDA struct | yes | 2026-05-26 |
| XGameServer | Monster.h | CMonster | 60896 | verified | IDA struct | yes | 2026-05-26 |
| XGameServer | Ai.h | CAi | ~32 | pending | IDA stub | partial | 2026-05-26 |
| XGameServer | Ai.h | E_AI_ACTION_TYPE | - | verified | IDA enum | yes | 2026-05-26 |
| XGameServer | Mover.h | CMover | 58592 | verified | IDA struct | yes | 2026-05-26 |
| XGameServer | MoverEx.h | CMoverEx | 60392 | verified | IDA struct | yes | 2026-05-26 |
| XGameServer | User.h | CUser | ~128000 | pending | IDA struct | partial | 2026-05-26 |
| XGameServer | BattleZone.h | CBattleZone | - | pending | IDA struct | partial | 2026-05-26 |
| XGameServer | GroupAggro.h | CGroupAggro | - | pending | IDA struct | partial | 2026-05-26 |
| XGameServer | STMonsterInfo.h | STMonsterInfo | 128 | verified | IDA struct | yes | 2026-05-26 |

## Notes

This file records structs, enums, and type definitions that need to be restored for GameServer.exe.

Type sources:
1. Types inferred from IDA decompilation
2. Type definitions from PDB symbol table
3. Types already defined in source code

Status values:
- pending: awaiting analysis/restoration
- verified: verified (matches source code)
- blocked: blocked (depends on other conditions or is a system type)
