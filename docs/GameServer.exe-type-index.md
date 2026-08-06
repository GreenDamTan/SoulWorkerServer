# GameServer.exe Type Index

| directory | file | type | size | status | source | verified | verification |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Common/XNet/XCommon | PSCommon.h | PS_ITEM_SLOT_INFO | 4 | verified | GameServer PDB UDT 0x4847 + source layout + source build | yes | byInvenType at +0x00 and signed shSlotPos at +0x02; ItemUseEffect receives two values by value. |
| Common/XNet/XCommon/PSServer | PSServerCore.h | PS_REQ_ITEM_MOVE | 20 | verified | GameServer PDB field list + source layout assertions + source build | yes | Source inventory type +0x00, source item ID +0x04, source slot +0x08, destination type +0x0A, destination item ID +0x0C, destination slot +0x10. |
| Common/XNet/XCommon/PSServer | PSServerCore.h | PS_REQ_ITEM_USE | 24 | verified | GameServer PDB field list + source layout assertions + source build | yes | Inventory type +0x00, slot +0x02, serial +0x08, and count +0x10. |
| Common/XNet/XCommon/PSServer | PSServerCore.h | PS_REQ_ITEM_COMBINE | 24 | verified | GameServer PDB field list + source layout assertions + source build | yes | PS_REQ_ITEM_MOVE base fields plus signed combine count at +0x14. |
| Common/XNet/XCommon/PSServer | PSServerCore.h | PS_REQ_ITEM_DIVIDE | 16 | verified | GameServer PDB field list + source layout assertions + source build | yes | Source inventory type +0x00, source item ID +0x04, source slot +0x08, destination type +0x0A, destination slot +0x0C, and signed count +0x0E. |
| Common/XNet/XCommon/PSServer | PSServerCore.h | PS_REQ_TICKCOUNT | 16 | verified | GameServer PDB field list + source layout assertions + source build | yes | Tick number +0x00 and 64-bit tick count +0x08. |
| Common/XNet/XCommon | PSCommon.h | PS_RES_STORAGE_INFO | 40 | verified | GameServer PDB UDT 0x4830 + existing layout assertions + source build | yes | vecItem at +0x00 and byType at +0x20. |
| Common/XNet/XCommon | PSCommon.h | ST_LOG_GAME | 488 | verified | GameServer PDB + existing layout assertions + source build | yes | Item-creation log payload used by both overloads. |
| Common/XNet/XCommon/PSServer | PSServerItem.h | eITEM_CREATE_TYPE | 4 | verified | GameServer PDB UDT 0x34f0 + source enum + source build | yes | 50 signed int32 creation reasons, values 1 through 50. |
| Common/XNet/XCommon/PSServer | PSServerItem.h | ST_CREATE_ITEM | 8 | verified | GameServer PDB UDT 0x4860 + source definition + source build | yes | nItemID +0x00, shCount +0x04, byUpgrade +0x06. |
| Common/XNet/XCommon/PSServer | PSServerItem.h | ST_CREATE_ITEMS | 32 | verified | GameServer PDB UDT 0x4866 + source definition + source build | yes | vecInfo is the sole member at +0x00. |
| Common/XNet/XCommon/PSServer | PSServerItem.h | PS_TOOL_ITEM | 16 | verified | GameServer PDB UDT 0x468a7 + source layout assertion + source build | yes | nItemID +0x00, nCount +0x04, byComplete +0x08, nCurIndex +0x0C. |
| Common/XNet/XCommon/PSServer | PSServerItem.h | PS_TOOL_SOULSTONE_INFO | 120 | verified | GameServer PDB UDT 0x139e1 + source layout assertion + source build | yes | stItemInfo +0x00, byRate +0x10, szItemName +0x11. |
| Common/XNet/XCommon/PSServer | PSServerItem.h | PS_TOOL_ITEM_INFO | 120 | verified | GameServer PDB UDT 0x139de + source layout assertion + source build | yes | nItemID through nValue3 at +0x00 through +0x10, szItemName +0x14. |
| Common/XNet/XCommon/PSServer | PSServerItem.h | PS_RES_TOOL_DROP_INFO | 32 | verified | GameServer PDB UDT 0x468ac + source layout assertion + source build | yes | vecInfo is the sole member at +0x00. |
| Common/XNet/XCommon/PSServer | PSServerItem.h | PS_RES_TOOL_SOULSTONE | 40 | verified | GameServer PDB UDT 0x468b1 + source layout assertion + source build | yes | byType +0x00 and vecInfo +0x08. |
| Common/XNet/XCommon/PSServer | PSServerCashShop.h | E_CASH_MILEAGE_TYPE | 4 | verified | GameServer PDB UDT 0x10A1B + source enum + source build | yes | Four signed int32 enumerators: AKASHIC=0, BROACH=1, TAG=2, MAX=3; original PDB ownership is PSCharacter.h. |
| Common/XNet/XCommon/PSServer | PSServerCashShop.h | PS_CASH_MILEAGE | 8 | verified | GameServer PDB UDT 0x13B00 + field list + source layout assertions + source build | yes | byMileageType is a byte at +0x00, padding preserves nCashMileage int32 at +0x04; original PDB ownership is PSCharacter.h. |
| Common/XNet/XCommon/PSServer | PSServerCashShop.h | PS_CASH_MILEAGE_LIST | 32 | verified | GameServer PDB UDT 0x13B0A + field list + source layout assertion + source build | yes | vecInfo is the sole vector<PS_CASH_MILEAGE> member; the restored serializer writes a byte count; original PDB ownership is PSCharacter.h. |
| Common/XNet/XCommon | PSCommon.h | STItem | 120 | verified | GameServer PDB UDT 0x487C + staged reduction paths + source build | yes | The staged payload exposes nItemID and sCount for inventory planning. |
| Common/XNet/XCommon | PSCommon.h | PS_STORAGE_INFO | 128 | verified | GameServer PDB UDT 0x4884 + staged reduction paths + source build | yes | byInvenType +0x00, shSlotPos +0x02, and STItem payload +0x08. |
| Common/XNet/XCommon | PSCommon.h | PS_ITEM_UPDATE | 8 | verified | GameServer PDB UDT 0x13922 + source layout assertions + source build | yes | byType +0x00, byInvenType +0x01, signed shSlotPos +0x02, nCount +0x04. |
| Common/XNet/XCommon | PSCommon.h | PS_RES_ITEM_DIVIDE | 136 | verified | GameServer PDB UDT 0x13c8a + source layout assertions + source build | yes | nItemID +0x00, source fields +0x04 through +0x08, destination fields +0x0A through +0x0C, STItem +0x10. |
| Common/XNet/XCommon | PSCommon.h | PS_DB_ITEM_REDUCE | 24 | verified | GameServer PDB UDT 0x13c8c + source layout assertions + source build | yes | dwUCID +0x00, byInvenType +0x04, signed shSlotPos +0x06, xSerial +0x08, nReduceCount +0x10. |
| Common/XNet/XCommon | PSCommon.h | ST_APPEARANCE_INFO | 16 | verified | GameServer PDB UDT 0x13b30 + source layout + source build | yes | wAppearanceID +0x00 and biEndDate +0x08. |
| Common/XNet/XCommon | PSCommon.h | ST_APPEARANCE_LIST | 32 | verified | GameServer PDB UDT 0x13b2e + source layout + source build | yes | vecInfo is the sole member at +0x00. |
| Common/XNet/XCommon/PSServer | PSServerCore.h | PS_QUICKSLOT_ITEM | 20 | verified | GameServer PDB UDT 0x13abd + source layout + source build | yes | uxActorID +0x00 and four uint32 item IDs +0x04 through +0x10. |
| Common/XNet/XCommon/PSServer | PSServerCore.h | PS_QUICKSLOT_UPDATE_ITEM | 16 | verified | GameServer PDB UDT 0x359f1 + source layout + source build | yes | Four uint32 item IDs at offsets 0, 4, 8, and 12. |
| Common/XNet/XCommon | PSCommon.h | PS_ITEM_COOLTIME_INFO | 24 | verified | GameServer PDB UDT 0x13c66 + source layout + source build | yes | byCooltimeGroupID +0x00, biRemainDate +0x08, and biCooltimeValue +0x10. |
| Common/XNet/XCommon | PSCommon.h | PS_DB_ITEM_COOLTIME_UPDATE | 32 | verified | GameServer PDB UDT 0x13c64 + source layout + source build | yes | dwUCID +0x00 and PS_ITEM_COOLTIME_INFO +0x08. |
| Common/XNet/XCommon | PSCommon.h | PS_ITEM_COOMTIME_LIST | 32 | verified | GameServer PDB UDT 0x13c70 + source layout + source build | yes | vecInfo is the sole vector<PS_ITEM_COOLTIME_INFO> member. |
| XGameServer/XSCommon/Table | TB_ITEM.h | TB_ITEM | 1198 | verified | GameServer PDB UDT 0x749F8 + reduction table lookup + source build | yes | Item_ID, Item_Classify_Index, stack, and period fields drive reduction planning. |
| XGameServer/XSCommon/Table | TB_ITEM_CLASSIFY.h | TB_ITEM_CLASSIFY | 19 | verified | GameServer PDB UDT 0x6CE71 + reduction inventory selection + source build | yes | Item_Inven_Type selects the target inventory. |
| XGameServer/XSCommon/Table | TB_WORLD_EVENT.h | TB_WORLD_EVENT | 1552 | verified | GameServer PDB UDT 0x6df86 field list + source layout assertion + active GameServer build | yes | event_ID +0x00, event_activation +0x04, three 511-byte strings at +0x05/+0x204/+0x403, and event item/reward fields at +0x602 through +0x60E preserve the 0x610-byte PDB layout. |
| XGameServer/XSCommon/Table | TB_WORLD_EVENT_REWARD.h | TB_WORLD_EVENT_REWARD | 16 | verified | GameServer PDB UDT 0x74b7e field list + source layout assertion + active GameServer build | yes | world_reward_ID +0x00, type/threshold fields +0x04 through +0x09, event_reward_value +0x0A, and item amount +0x0E preserve the 0x10-byte PDB layout. |
| XGameServer/XSCommon/Table | TB_ITEM_ENDURANCE.h | TB_ITEM_ENDURANCE | 40 | verified | GameServer PDB UDT 0x6caf0 + source layout assertion + source build | yes | Endurance_ID +0x00, paired weapon values, and gear values through +0x24 preserve PDB order. |
| XGameServer/XSCommon/Table | TB_REPACKAGECOSTUME.h | TB_REPACKAGECOSTUME | 56 | verified | GameServer PDB UDT 0x6a1ea + source layout assertion + source build | yes | RePackageItem_ID +0x00 followed by contiguous Item_01 through Item_13 at +0x04 through +0x34. |
| XGameServer | Ai.h | DelegateTarget | 24 | implemented | IDA struct | no | 代理目标结构 (nIndex, nMobID1-5) |
| XGameServer | Ai.h | CAi::m_stDelegateSkill | - | implemented | IDA struct | no | 代理技能结构 (nSkillID, nDelegateMobID, vecTarget) |
| XGameServer | Ai.h | CAi (新增成员) | - | implemented | IDA struct | no | 添加m_mapCooltimeList/m_fGlobalCooltime/m_fSumElapsedTime/m_fActivateTime/m_fLastDamageTime/m_fProtectEffectDist/m_fProtectWaitTimeOut |
| XGameServer | Ai.h | E_FSMCONDITIONS | - | implemented | IDA enum | no | FSM条件函数枚举 (CONDITION_EQUAL, CONDITION_NOT_EQUAL等) |
| XGameServer | Ai.h | FsmData | - | implemented | IDA struct | no | FSM数据结构 |
| XGameServer | Ai.h | FsmDataEx | - | implemented | IDA struct | no | FSM扩展数据结构 |
| XGameServer | TraceHPState.h | CTraceHPState | 56 | implemented | IDA struct | no | HP追踪状态类 |
| XGameServer | GameWorldMode.h | CGameWorldMode | 152 | implemented | IDA struct | no | 世界模式管理类 (0x98) |
| XGameServer | GameWorldMode.h | ST_WORLD_MODE_INFO | - | implemented | IDA struct | no | 世界模式信息结构 |
| XGameServer | PSCommon.h | PS_WORLD_MODE_FINISH | 0x50 bytes | implemented | PDB/common packet struct + IDA FinishWorldMode serialization use | no | Finish-world-mode packet structure used by `XDistrict::FinishWorldMode` and `PSServerWorldMode.h` serialization. |
| XGameServer | TB_MODE_DISTRICT6.h | TB_MODE_DISTRICT6 | 0x56 bytes | implemented | IDA table access + existing static_assert | no | World-mode table row used by `XDistrict::FinishWorldMode` for `Start_Type`. |
| XGameServer | TB_MODE_DISTRICT6_DATE.h | TB_MODE_DISTRICT6_DATE | 0x30 bytes | implemented | IDA table access + existing static_assert | no | World-mode date table row used by `XDistrict::FinishWorldMode` for clear-count booster thresholds. |
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
| XGameServer | actor/component/GocInventory.h | eTRADE_STATE | 4 | verified | GameServer PDB UDT 0x10A35 + source enum + source build | yes | NONE=0, READY_BEFORE=1, READY=2, UPDATE=3, CHECK=4, CHECK_BOTH=5, CONFIRM=6. |
| XGameServer | actor/component/GocTrade.h | ST_TRADE_ITEM | 80 | implemented | IDA struct | no | Trade item structure with serial, ID, money, and name |
| XGameServer | actor/component/GocTrade.h | ST_TRADE_ITEM_LIST | 24 | implemented | IDA struct | no | Vector of trade items |
| XGameServer | actor/component/GocInventory.h | PS_REQ_ITEM_TRADE | 4 | verified | GameServer PDB UDT 0x13926 + source layout assertion + source build | yes | byInvenType +0x00 and signed shSlotPos +0x02. |
| Common/XNet/XCommon/PSServer | PSServerDB.h | PS_DB_HAN_NET_CAFE | 36 | verified | GameServer PDB UDT 0x6C943 + field list + source build | yes | dwUAID, szAccountID[21], nServerID, and bLogin preserve the 36-byte record. |
| Common/XNet/XCommon/PSServer | PSServerDB.h | PS_DB_HAN_NET_CAFE_LIST | 40 | verified | GameServer PDB UDT 0x293BE + field list + source build | yes | vecList and bLast preserve the 40-byte response container. |
| Common/XNet/XCommon/PSServer | PSServerDB.h | ST_WORLD_EVENT_REWARD_INFO | 8 | verified | GameServer PDB UDT 0xEA78 + field list + source build | yes | nRewardIndex, byRewardType, and byRewardState preserve the response element. |
| Common/XNet/XCommon/PSServer | PSServerDB.h | PS_WORLD_EVENT_INFO_RES | 48 | verified | GameServer PDB UDT 0xEA58 + field list + source build | yes | Event counters, daily reward state, and vector reward information preserve the 48-byte layout. |
| Common/XNet/XCommon/PSServer | PSServerChat.h | PS_CHAT_NOTICE | 532 | verified | GameServer PDB UDT 0x54E5 + field list + source build | yes | byType, fixed wide message/color buffers, and nMessageCode preserve the packet layout. |
| Common/XNet/XCommon | PSCommon.h | PS_SECOND_PW_REQ | 8 | verified | GameServer PDB UDT 0x12BA4 + field list + source build | yes | byCheckType +0x00 and strPassword[7] +0x01. |
| Common/XNet/XCommon | PSCommon.h | PS_SECOND_PW_RES | 8 | verified | GameServer PDB + source layout + source build | yes | nErrorID +0x00 and bySecondPWState +0x04. |
| Common/XNet/XCommon | PSCommon.h | PS_TRADE_PW_REQ | 6 | verified | GameServer PDB UDT 0x13C5B + field list + source layout + source build | yes | byCheckType +0x00 and strPassword[5] +0x01. |
| Common/XNet/XCommon | PSCommon.h | PS_TRADE_PW_RES | 8 | verified | GameServer PDB UDT 0x13C60 + field list + source layout + source build | yes | nErrorID +0x00 and byTradePWState +0x04. |
| XGameServer | actor/component/GocTrade.h | PS_TRADE_RESULT | 8 | implemented | IDA struct | no | Trade result packet (result code, error code) |
| XGameServer | actor/component/GocTrade.h | PS_TRADE_DB_CONFIRM | 32 | implemented | IDA struct | no | Trade DB confirmation (actor IDs, item lists) |
| XGameServer | actor/component/GocInventory.h | ST_MY_TRADE_INFO | 32 | verified | GameServer PDB UDT 0x767a7 + source layout assertion + source build | yes | signed int64 biMoney +0x00 and list<PS_REQ_ITEM_TRADE> listInfo +0x08. |
| Common/XNet/XCommon/PSServer | PSServerCore.h | ST_USE_ITEM_INFO | 16 | verified | GameServer PDB UDT 0x1373c + field list + source layout assertion + source build | yes | nItemType int32 at +0x00, byCount byte at +0x04, padding through +0x07, nUseDate int64 at +0x08. |
| Common/XNet/XCommon/PSServer | PSServerCore.h | ST_USE_ITEM_INFO_LIST | 32 | verified | GameServer PDB UDT 0x1373a + vector element UDT + source layout assertion + source build | yes | vecInfo is vector<ST_USE_ITEM_INFO> at +0x00. |
| XGameServer | actor/component/GocInventory.h | std::map<int, ST_USE_ITEM_INFO> | 32 | verified | GameServer PDB nested UDT 0x10bf8 + member type 0x10bf8 + source declaration + source build | yes | Concrete member type of m_mpUseItemInfo; no artificial MAP_USE_ITEM_INFO alias is introduced. |
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
| XGameServer | VaccumCube.h | PS_VACCUM_CUBE_IN | 12 | implemented | IDA disasm/source build | no | Corrected field layout to nID at offset 0, nTableID at offset 4, byInType at offset 8; build verified only. |
| XGameServer | VaccumCube.h | PS_VACCUM_CUBE_OUT | 8 | implemented | IDA disasm/source build | no | Corrected field layout to nID at offset 0 and byOutType at offset 4; build verified only. |
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
| XGameServer | ManagerStubs.h | CTimeEventMgr | 432 | implemented | IDA struct info | no | Added IDA-backed lock and map fields needed by CheckTimeEvent: m_rwTimeEventLock, m_rwValueEventLock, m_rwRouletteEventLock, m_mapTimeEvent. |
| XGameServer | ManagerStubs.h | CWorldEventMgr | 48 | implemented | IDA struct info | no | Added IDA-backed lock and map fields needed by CheckWorldEvent: m_rwWorldEventLock, m_mapWorldEventBooster, m_dw64CurrentTick. |
| XGameServer | GocEvent.h / ManagerStubs.h | ST_WORLD_EVENT_BOOSTER | 32 | implemented | IDA struct info | no | Corrected shared definition to expose IDA booster fields nBoosterID, biStart, and biEnd while preserving existing CGocEvent aliases nMyCount, biLastRegisterDate, and biDailyRewardDate. |
| XLoginServer | DayEventManager.h | CDayEventMgr | 48 | implemented | IDA struct info | no | Added GameServer-target GetDatEventBoosterID helper using existing day event map storage. |
| XGameServer | VaccumCubeProcess.cpp | PS_REQ_VACCUM_CLICK_START | 4 | implemented | IDA decompile + operator>> evidence | no | Source-local request packet structure with one IDA-backed field: int nID. |
| XGameServer | VaccumCubeProcess.cpp | PS_REQ_VACCUM_CLICK_CANCEL | 4 | implemented | IDA decompile + operator>> evidence | no | Source-local request packet structure with one IDA-backed field: int nID. |
| XGameServer | Maze.h | STSpawnBoxGroupInfo | 16 | implemented | IDA struct | no | Spawn box group limit info (nGroupID, nLimit, fWaitTime, bStart) |
| XGameServer | Maze.h | STMonterGroupMonsterData | 16 | implemented | IDA struct | no | Monster spawn data in group (pInfo, fWaitTime, bSpawn) |
| XGameServer | Maze.h | STMonterGroupMonsterInfo | - | implemented | IDA struct | no | Monster group info (nGroupID, vecBoxList) |
| XGameServer | Maze.h | MAZE_GAME_RULE | 24 | implemented | IDA disasm | no | Maze game rule struct for ProcessGameRule functions (nParam1-6) |

| XCore | HavokTypes.h | hkaiNavMeshSilhouetteSelector | 16 | implemented | IDA struct | no | Base class for hkaiOverlapManager |
| XCore | HavokTypes.h | hkaiOverlapManager | 48+ | implemented | IDA struct | no | Overlap manager with m_silhouetteFilter and m_sections |
| XCore | HavokTypes.h | hkaiOverlapManager::Section | 64 | implemented | IDA struct | no | Section struct with tree map and array data |
| XCore | HavokTypes.h | hkaiNavMeshInstance | 104 | implemented | IDA struct | no | Expanded with 10 field pairs + source pointer |
| Common/XNet/XCommon | PSCommon.h | PS_OPEN_SLOT | 6 | verified | GameServer PDB UDT 0x13bb9 + IDA 0x140735DE0 + source build | yes | byInvenType +0x00, shOpenSlot +0x02, and byExtendStep +0x04; XPacket wire order is byte, int16, byte. |
| Common/XNet/XCommon | PSCommon.h | PS_OPEN_SLOT_INFO | 32 | verified | GameServer PDB UDT 0x13bb7 + IDA 0x140735E50 + source build | yes | vecInfo is the sole member at +0x00; the original serializes and iterates its count as signed char. |
| XCommon | PSServerCashShop.h | PS_RES_ITEM_REPURCHASER_LIST | - | implemented | IDA decompile 0x1400A57A0 | no | Repurchase item list with vecInfo, psSocketList, psBroachList |
| XCommon | PSCommon.h | STItem::operator== | - | implemented | IDA decompile 0x1400A5490 | yes | Added comparison operator for std::find support |
| Common/XNet/XCommon/PSServer | PSServerCore.h | PS_ACTIVE_BROACH_EFFECT | 4 | verified | PDB type + IDA packet path + source build | yes | One uint32_t active set-buff ID with normal XPacket read/write operators. |
| Common/XNet/XCommon/PSServer | PSServerCore.h | ST_ITEM_BROACH | 0x48 | verified | PDB field layout + CItemCostume decompile + source build | yes | Uses biSerial == -1 as the empty sentinel and stores fifteen item IDs. |
| XGameServer/Item | CItem.h | CItem | 0xD8 | verified | PDB layout + IDA constructor + source build | yes | Field offsets from +0x08 through +0xD7, including embedded PS_ITEM_PACKAGE, are asserted in source. |
| XGameServer/Item | CItem.h | CItemCostume | 0x138 | verified | PDB layout + IDA constructor + source build | yes | Contains ST_ITEM_BROACH at +0xD8 and five uint32_t set-buff IDs at +0x120. |
| XGameServer/Item | CItem.h | CItemEquip | 0x1E8 | verified | PDB layout + IDA constructor + source build | yes | Contains ST_ITEM_SOCKET[4] at +0xD8, m_bRenovate at +0x1B8, and ST_EXTEND_OPTION[5] at +0x1BC. |
| XGameServer/actor/component | XBaseInventory.h | XBaseEquip | 0x188 | implemented | PDB layout + IDA decompile | no | Original slot, lock, set-item, and type offsets are recovered; the current map container is a semantic ABI substitute. |
| XGameServer | StatusEffect.h | EFFECT_SKILL_OPTION | 0x04 | verified | GameServer PDB enum 0x55DE + IDA + source build | yes | Enumerators NONE through MAX are 0 through 6; the accepted option-effect interval is [DAMAGE, MAX). |
| XCore | VisionEngineTypes.h | SItemRateInfo | 0x0C | verified | GameServer PDB type 0x73165 + IDA constructors + source static_assert | yes | Fields are int value, uint16 level, uint8 rank, padding, and int critical value at +0x08. |
| XGameServer/actor/component | GocAttribute.h | CGocAttribute::MAP_SKILL_OPTION | 0x20 | implemented | GameServer PDB CGocAttribute member type + source static_assert | no | PDB specialization is map<pair<int,EFFECT_SKILL_OPTION>,int>; the current STL uses an ABI wrapper to preserve the original 0x20 footprint. |
| XGameServer/actor/component | GocAttribute.h | CGocAttribute::MAP_ITEM_RATE_INFO | 0x20 | implemented | GameServer PDB CGocAttribute member type + source static_assert | no | PDB specialization is map<uint8_t,SItemRateInfo>; the current STL uses an ABI wrapper to preserve the original 0x20 footprint. |
| XGameServer/actor/component | GocAttribute.h | CGocAttribute | 0xBA8 | implemented | GameServer PDB type 0x49618 + IDA + source static_assert + build | no | Member offsets through +0xBA4 are preserved; concrete PDB map value types now occupy the +0x910 and +0x930 ABI-compatible slots. |
| XGameServer/actor/component | GocQuest.h | CQuestCondition | 0x28 | verified | GameServer PDB layout + source static_assert + build | yes | Stores quest ID, episode pointer, condition pointer, condition index, and table pointer. |
| Common/XNet/XCommon/PSServer | PSServerDB.h | ST_QUEST_CONDITION | 0x08 | verified | GameServer PDB layout + source static_assert + build | yes | Contains a uint32_t condition ID and an 8-bit condition value with PDB padding. |
| Common/XNet/XCommon/PSServer | PSServerDB.h | ST_QUEST_EPISODE | 0x60 | verified | GameServer PDB layout + source static_assert + build | yes | Preserves completion bit, table pointer, and ten condition entries. |
| XGameServer/XSCommon/Table | TB_QUEST_CONDITION.h | TB_QUEST_CONDITION | 0x29C | verified | GameServer PDB UDT 0x67F04 + source static_assert + IDA 0x140126860 + source build | yes | Target_Type at +0x0E, Target_ID at +0x0F, and two add/remove ID/count pairs from +0x7D. |
| Common/XNet/XCommon/PSServer | PSServerDB.h | PS_QUEST_CONDITION | 0x20 | verified | GameServer PDB layout + serializer path + source static_assert + build | yes | Vector payload serializes a signed 16-bit count followed by condition records. |
