# GameServer.exe Type Index

| directory | file | type | size | status | source | verified | verification |
| --- | --- | --- | --- | --- | --- | --- | --- |
| XGameServer | GameWorldMode.h | CGameWorldMode | 152 | implemented | IDA struct | no | 世界模式管理类 (0x98) |
| XGameServer | GameWorldMode.h | ST_WORLD_MODE_INFO | - | implemented | IDA struct | no | 世界模式信息结构 |
| XGameServer | Monster.h | tagDamageMeter | 8 | implemented | IDA struct | no | - |
| XGameServer | Monster.h | CMonster | 60896 | implemented | IDA struct | no | - |
| XGameServer | Ai.h | CAi | ~200 | implemented | IDA struct | no | - |
| XGameServer | Ai.h | E_AI_ACTION_TYPE | - | implemented | IDA enum | no | - |
| XGameServer | Ai.h | E_FSMDATATYPE | - | implemented | IDA enum | no | - |
| XGameServer | Ai.h | E_FSMVARIABLES | - | implemented | IDA enum | no | - |
| XGameServer | Ai.h | E_PROTECT_STATE | - | implemented | IDA enum | no | - |
| XGameServer | Ai.h | StateVarInfo | 16 | implemented | IDA struct | no | - |
| XGameServer | Mover.h | CMover | 58592 | implemented | IDA struct | no | - |
| XGameServer | MoverEx.h | CMoverEx | 60392 | implemented | IDA struct | no | - |
| XGameServer | User.h | CUser | ~128000 | implemented | IDA struct | no | - |
| XGameServer | BattleZone.h | CBattleZone | - | implemented | IDA struct | no | - |
| XGameServer | GroupAggro.h | CGroupAggro | - | implemented | IDA struct | no | 群体仇恨管理类 |
| XGameServer | STMonsterInfo.h | STMonsterInfo | 128 | implemented | IDA struct | no | - |
| XGameServer | MySkillList.h | CMySkillList | - | implemented | IDA struct | no | - |
| XGameServer | Xigncode.h | CXigncode | - | implemented | IDA struct | no | - |
| XGameServer | Xigncode.h | IXigncodeServer | - | implemented | IDA struct | no | - |
| XGameServer | Achieve.h | CAchieve | - | implemented | IDA struct | no | - |
| XGameServer | AchieveType.h | CAchieveType | - | implemented | IDA struct | no | - |
| XGameServer | ActionResMgr.h | XActionResMgr | 256 | implemented | IDA struct | no | - |
| XCore | VisionEngineTypes.h | tagCOOLTIME | 16 | implemented | IDA struct | no | 冷却时间结构 |
| XCore | VisionEngineTypes.h | tagHIT_COLLISION | 29 | implemented | IDA struct | no | Hit Collision 单项数据 |
| XCore | VisionEngineTypes.h | tagHIT_COLLISION_DATA | 40 | implemented | IDA struct | no | Hit Collision 数据容器 |
| XCore | VisionEngineTypes.h | tagHIT_TRACE_BONE_NAME_DATA | 32 | implemented | IDA struct | no | Hit Trace Bone 名称数据 |
| XCore | VisionEngineTypes.h | VArray<T> | - | implemented | IDA struct | yes | Vision Engine 动态数组模板 |
| XCore | VisionEngineTypes.h | VRefCounter | 16 | implemented | IDA struct | yes | Vision Engine 引用计数基类 |
| XCore | VisionEngineTypes.h | ActionTrigger | 168 | implemented | IDA struct | yes | 动作触发器结构 (完整布局已还原) |
| XCore | VisionEngineTypes.h | VAnimationInfo | 312 | implemented | IDA struct | yes | Vision Engine 动画信息 (完整布局已还原) |
| XCore | VisionEngineTypes.h | VBaseResourceLump | 104 | implemented | IDA struct | yes | Vision Engine 基础资源块 |
| XCore | VisionEngineTypes.h | VActionResourceLump | 232 | implemented | IDA struct | yes | Vision Engine 动画资源块 (完整布局已还原) |
| XCore | VisionEngineTypes.h | tagHIT_COLLISION_CYLINDER | 52 | implemented | IDA struct | no | 圆柱碰撞体数据 |
| XCore | VisionEngineTypes.h | TypeOfActionBufferBehavior | - | implemented | IDA enum | yes | 动作缓冲行为类型枚举 |
| XCore | VisionEngineTypes.h | TypeOfMoving | - | implemented | IDA enum | yes | 移动类型枚举 |
| XCore | VisionEngineTypes.h | TypeOfDefense | - | implemented | IDA enum | yes | 防御类型枚举 |
| XCore | VisionEngineTypes.h | TypeOfHUD | - | implemented | IDA enum | yes | HUD类型枚举 |
| XCore | VisionEngineTypes.h | TypeOfAnimationBehavior | - | implemented | IDA enum | yes | 动画行为类型枚举 |
| XCore | VisionEngineTypes.h | EndOfAnimationType | - | implemented | IDA enum | yes | 动画结束类型枚举 |
| XCore | XServer/XSeed.h | XRand<T> | - | implemented | IDA template | no | 随机数生成模板函数 |

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
