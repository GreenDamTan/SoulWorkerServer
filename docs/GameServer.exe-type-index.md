# GameServer.exe Type Index

| directory | file | type | size | status | source | verified | verification |
| --- | --- | --- | --- | --- | --- | --- | --- |
| XGameServer | Ai.h | CAi (新增成员) | - | implemented | IDA struct | no | 添加m_nSkillGroupRatio[10]技能组比率数组 |
| XGameServer | Ai.h | CAi (新增成员) | - | implemented | IDA struct | no | 添加m_nRequestHelpCnt/m_fRequestHelpDistance/m_nRequestHelpMonsterID/m_nRequestHelpMonsterCount/m_bIsFirstAttacker/m_fRunwayMinTimeOut/m_fRunwayMaxTimeOut/m_vecStateData |
| XGameServer | Ai.h | DelegateTarget | 24 | implemented | IDA struct | no | 代理目标结构 (nIndex, nMobID1-5) |
| XGameServer | Ai.h | CAi::m_stDelegateSkill | - | implemented | IDA struct | no | 代理技能结构 (nSkillID, nDelegateMobID, vecTarget) |
| XGameServer | Ai.h | CAi (新增成员) | - | implemented | IDA struct | no | 添加m_fAiCheckTime/m_fRunDistance/m_nReturnHP/m_fMoveDelayToTarget/m_fMoveDistanceToTarget |
| XGameServer | Ai.h | CAi (新增成员) | - | implemented | IDA struct | no | 添加m_fSuicideTime/m_bEnableHelperWarp |
| XGameServer | Ai.h | CAi (新增成员) | - | implemented | IDA struct | no | 添加m_mapCooltimeList/m_fGlobalCooltime/m_fSumElapsedTime/m_fActivateTime/m_fLastDamageTime/m_fProtectEffectDist/m_fProtectWaitTimeOut |
| XGameServer | Ai.h | E_FSMCONDITIONS | - | implemented | IDA enum | no | FSM条件函数枚举 (CONDITION_EQUAL, CONDITION_NOT_EQUAL等) |
| XGameServer | Ai.h | FsmData | - | implemented | IDA struct | no | FSM数据结构 |
| XGameServer | Ai.h | FsmDataEx | - | implemented | IDA struct | no | FSM扩展数据结构 |
| XGameServer | TraceHPState.h | CTraceHPState | 56 | implemented | IDA struct | no | HP追踪状态类 |
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
| XGameServer | actor/component/GocTrade.h | eTRADE_STATE | 4 | implemented | IDA enum | no | Trade state enumeration (NONE, REQUESTING, TRADING, CONFIRMING, LOCKED) |
| XGameServer | actor/component/GocTrade.h | ST_TRADE_ITEM | 80 | implemented | IDA struct | no | Trade item structure with serial, ID, money, and name |
| XGameServer | actor/component/GocTrade.h | ST_TRADE_ITEM_LIST | 24 | implemented | IDA struct | no | Vector of trade items |
| XGameServer | actor/component/GocTrade.h | PS_REQ_ITEM_TRADE | 12 | implemented | IDA struct | no | Trade item request packet (serial, slot type, count) |
| XGameServer | actor/component/GocTrade.h | PS_TRADE_PW_REQ | 65 | implemented | IDA struct | no | Trade password request (old password, new password, type) |
| XGameServer | actor/component/GocTrade.h | PS_TRADE_RESULT | 8 | implemented | IDA struct | no | Trade result packet (result code, error code) |
| XGameServer | actor/component/GocTrade.h | PS_TRADE_DB_CONFIRM | 32 | implemented | IDA struct | no | Trade DB confirmation (actor IDs, item lists) |
| XGameServer | actor/component/GocTrade.h | ST_MY_TRADE_INFO | 32 | implemented | IDA struct | no | My trade information structure |
| XGameServer | actor/component/GocTrade.h | CGocTrade | 160 | implemented | IDA struct | no | Trade component class |
| XGameServer | actor/component/GocExchange.h | ST_EXCHANGE_PRICE_INFO | 24 | implemented | IDA struct | no | Exchange price info (price, time, count) |
| XGameServer | actor/component/GocExchange.h | ST_MY_EXCHANGE_ITEM | 48 | implemented | IDA struct | no | My exchange item structure |
| XGameServer | actor/component/GocExchange.h | ST_EXCHANGE_SEARCH_RESULT | 64 | implemented | IDA struct | no | Exchange search result structure |
| XGameServer | actor/component/GocExchange.h | PS_EXCHANGE_SEARCH_REQ | 24 | implemented | IDA struct | no | Exchange search request packet |
| XGameServer | actor/component/GocExchange.h | PS_EXCHANGE_SEARCH_RES | 32 | implemented | IDA struct | no | Exchange search result packet |
| XGameServer | actor/component/GocExchange.h | PS_EXCHANGE_ITEM_BUY_REQ | 20 | implemented | IDA struct | no | Exchange item buy request |
| XGameServer | actor/component/GocExchange.h | PS_EXCHANGE_ITEM_RECALL_REQ | 12 | implemented | IDA struct | no | Exchange item recall request |
| XGameServer | actor/component/GocExchange.h | PS_EXCHANGE_SELL_REGISTER_REQ | 28 | implemented | IDA struct | no | Exchange sell registration request |
| XGameServer | actor/component/GocExchange.h | PS_DB_EXCHANGE_SELL_REGISTER | 48 | implemented | IDA struct | no | DB exchange sell registration packet |
| XGameServer | actor/component/GocExchange.h | PS_DB_EXCHANGE_ITEM_BUY | 32 | implemented | IDA struct | no | DB exchange item buy packet |
| XGameServer | actor/component/GocExchange.h | PS_DB_EXCHANGE_ITEM_RECALL_REQ | 32 | implemented | IDA struct | no | DB exchange item recall request |
| XGameServer | actor/component/GocExchange.h | PS_EXCHANGE_PRICE_HISTORY_REQ | 12 | implemented | IDA struct | no | Price history request packet |
| XGameServer | actor/component/GocExchange.h | PS_EXCHANGE_PRICE_HISTORY_RES | 32 | implemented | IDA struct | no | Price history response packet |
| XGameServer | actor/component/GocExchange.h | PS_EXCHANGE_PRICE_HISTORY_UPDATE | 32 | implemented | IDA struct | no | Price history update packet |
| XGameServer | actor/component/GocExchange.h | PS_EXCHANGE_INTEREST_LIST_REQ | 8 | implemented | IDA struct | no | Interest list request packet |
| XGameServer | actor/component/GocExchange.h | PS_EXCHANGE_INTEREST_LIST_RES | 24 | implemented | IDA struct | no | Interest list response packet |
| XGameServer | actor/component/GocExchange.h | PS_EXCHANGE_INTEREST_ITEM_REQ | 8 | implemented | IDA struct | no | Interest item request packet |
| XGameServer | actor/component/GocExchange.h | PS_EXCHANGE_MY_LIST_REQ | 8 | implemented | IDA struct | no | My list request packet |
| XGameServer | actor/component/GocExchange.h | PS_EXCHANGE_MY_LIST_RES | 24 | implemented | IDA struct | no | My list response packet |
| XGameServer | actor/component/GocExchange.h | CGocExchange | 128 | implemented | IDA struct | no | Exchange component class |
| XGameServer | TradeProcess.h | CTradeProcess | 32 | implemented | IDA struct | no | Trade process handler class |
| XCore | VisionEngineTypes.h | IVTimer | 8 | implemented | IDA decompile | no | Vision Engine Timer Interface (m_fTime, m_fTimeDifference) |
| XCore | VisionEngineTypes.h | VDefaultTimer | 8 | implemented | IDA decompile | no | Default Timer Implementation (inherits IVTimer) |
| XCore | VisionEngineTypes.h | ThreadLocalData | - | implemented | IDA decompile | no | Thread-local storage for game data |
| XCore | VisionEngineTypes.h | TypeOfDefense | - | implemented | IDA enum | yes | 防御类型枚举 |
| XCore | VisionEngineTypes.h | TypeOfHUD | - | implemented | IDA enum | yes | HUD类型枚举 |
| XCore | VisionEngineTypes.h | TypeOfAnimationBehavior | - | implemented | IDA enum | yes | 动画行为类型枚举 |
| XCore | VisionEngineTypes.h | EndOfAnimationType | - | implemented | IDA enum | yes | 动画结束类型枚举 |
| XCore | XServer/XSeed.h | XRand<T> | - | implemented | IDA template | no | 随机数生成模板函数 |
| XGameServer | BattleZone.h | ST_MONSTER_DAMAGE_INFO | 24 | implemented | PDB cvdump types + IDA decompile | no | Fields restored: dwUCID, nDamage, byClass; explicit padding added to preserve 24-byte layout |
| XGameServer | CutsceneManager.h | ST_CUTSCENE_MEMBER | 296 | implemented | IDA struct | yes | 过场动画成员结构 (pUser, szCutscene[256], nOrder, nTime, bChangeState, bInvincible, nType, bRecvScene) |
| XGameServer | CutsceneManager.h | ST_CUTSCENE_INFO | - | implemented | IDA struct | no | 过场动画信息结构 |
| XGameServer | Mover.h | ST_MOVE | 56 | implemented | IDA struct | yes | Move packet structure (IDA 0x14036EAC0) |
| XGameServer | Mover.h | ST_MOVE_STOP | 40 | implemented | IDA struct | yes | Move stop packet structure (IDA 0x14036EE90) |
| XGameServer | Mover.h | ST_MOVE_BATTLE | 28 | implemented | IDA struct | yes | Battle move packet structure (IDA 0x14036F1E0) |
| XGameServer | Mover.h | PS_MOVING_TARGET | 24 | implemented | IDA struct | yes | Moving target packet structure (IDA 0x140373890) |
| XGameServer | Mover.h | PS_MOVING_TARGET_LIST | 32 | implemented | IDA struct | yes | Moving target list packet structure |
| Common/PSServer | PSServerDB.h | PS_CUTSCENE_UPDATE_RES | 4 | implemented | IDA struct | yes | 过场动画更新响应 (nPlayState) |
| XGameServer | GocAttendance.h | PS_ATTENDANCE_INFO | 128 | implemented | IDA struct | no | Daily attendance tracking structure (byApplyAttendance, dwType, dwAttendanceID, byAttendanceCount, nAttendance[14]) |
| XGameServer | GocAttendance.h | PS_ATTENDANCE_CONTINUE | 24 | implemented | IDA struct | no | Streak attendance tracking structure (byApplyAttendance, dwType, byAttendanceCount, nLastAttendanceDate) |
| XGameServer | GocAttendance.h | PS_ATTENDANCE_PLAY_TIME | 32 | implemented | IDA struct | no | Play time attendance tracking structure (byApplyAttendance, dwType, byCurPos, nPlaySec, nUpdateDate) |
| XGameServer | GocAttendance.h | CGocAttendance | ~200 | implemented | IDA struct | no | Attendance and login reward system component |

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

