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
| Common/XNet/XCommon/PSServer | PSServerCore.h | PS_NETCAFE_INFO | 8 | verified | GameServer PDB UDT 0xe400 field list + source layout assertion + source build | yes | bool nNetCafe +0x00, int nEventNo +0x04; default constructor. |
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
| XCore | VisionEngineTypes.h | VRefCounter | 16 | implemented | PDB LF_CLASS 0x4A80B | yes | Vision Engine 引用计数基类；继承 VBaseObject(8B vptr) + m_iRefCount@8，修正基类 ABI 消除隐式 vptr |
| XCore | VisionEngineTypes.h | hkvVec4 | 16 | implemented | PDB LF_CLASS 0xF359 | yes | 4D 向量 (ShaderChangeTrigger::vecParamValue +0x228 元素类型) |
| XCore | VisionEngineTypes.h | ActionTrigger | 168 | implemented | IDA struct | yes | 动作触发器结构 (完整布局已还原) |
| XCore | VisionEngineTypes.h | SummonMonsterTrigger | 528 | implemented | PDB LF_CLASS 0x76669 | yes | 召唤怪物触发器 (完整 29-field 布局已还原至 VisionEngineTypes.h) |
| XCore | VisionEngineTypes.h | SRangeInfo | 36 | implemented | PDB LF_FIELDLIST 0x26B5A | yes | 攻击范围信息 (vCenterPos +0x00, fBoxSizeX +0x0C, fBoxSizeY +0x10, fRadius +0x14, fAngle +0x18, fHeight +0x1C, fStartPos +0x20) |
| XCore | VisionEngineTypes.h | DeathTrigger | 304 | implemented | PDB LF_FIELDLIST 0x7682F | yes | 死亡触发器 (szDeathAnim[128] +0xA8, IsLocalClient +0x128, bMustExcute +0x129) |
| XCore | VisionEngineTypes.h | InvisibleTrigger | 192 | implemented | PDB LF_FIELDLIST 0x79A64 | yes | 隐身触发器 (4 bools +0xA8..0xAB, 5 ints +0xAC..0xBC) |
| XCore | VisionEngineTypes.h | WarpToPointTrigger | 184 | implemented | PDB LF_FIELDLIST 0x70C00 | yes | 传送触发器 (WarpPattern +0xA8, WarpPoint +0xAC, WarpYaw +0xB0) |
| XCore | VisionEngineTypes.h | LuaFunctionCallTrigger | 560 | implemented | PDB LF_FIELDLIST 0x6D4D4 | yes | Lua调用触发器 (LuaFilename[260] +0xA8, LuaFunction[128] +0x1AC) |
| XCore | VisionEngineTypes.h | MovingInputTrigger | 240 | implemented | PDB LF_FIELDLIST 0x69F88 | yes | 移动输入触发器 (8 fields incl SRangeInfo +0xC0, vPullPoint +0xE4) |
| XCore | VisionEngineTypes.h | JumpAttackTrigger | 184 | implemented | PDB LF_FIELDLIST 0x6D640 | yes | 跳跃攻击触发器 (4 floats +0xA8..0xB4) |
| XCore | VisionEngineTypes.h | DetachTrigger | 312 | implemented | PDB LF_FIELDLIST 0x760D2 | yes | 分离触发器 (DropTime +0xA8, szAniName[128] +0xAC, SkillID +0x12C, RefEventID +0x130) |
| XCore | VisionEngineTypes.h | RandomSummonTrigger | 352 | implemented | PDB LF_FIELDLIST 0x6D45D | yes | 随机召唤触发器 (szSummonAnim[128] +0xA8, SummonPos +0x128, 3 ID + 3 rate + count + 2 radii + bSuicidePossible) |
| XCore | VisionEngineTypes.h | CheckAttackSkillTrigger | 192 | implemented | PDB LF_FIELDLIST 0x729C5 | yes | 检查攻击技能触发器 (nAngle +0xA8, nMinRange +0xAC, nMaxRange +0xB0, nSkillID +0xB4, nProbability +0xB8, fDuration +0xBC) |
| XCore | VisionEngineTypes.h | DelSummonMonsterTrigger | 304 | implemented | PDB LF_FIELDLIST 0x6BB09 | yes | 删除召唤怪物触发器 (MonsterID +0xA8, szSummonAnim[128] +0xAC) |
| XCore | VisionEngineTypes.h | ExtraInputTrigger | 176 | implemented | PDB LF_FIELDLIST 0x6F422 | yes | 额外输入触发器 (SkipTime +0xA8, ReplayTime +0xAC) |
| XCore | VisionEngineTypes.h | ChargingInputTrigger | 184 | implemented | PDB LF_FIELDLIST 0x78CDA | yes | 蓄力输入触发器 (fAnimSpeed +0xA8, fMaxMultiple +0xAC, fDuration +0xB0) |
| XCore | VisionEngineTypes.h | UserDataTrigger | 184 | implemented | PDB LF_FIELDLIST 0x6D759 | yes | 用户数据触发器 (DataType +0xA8, DataValue +0xAC, DataValue2 +0xB0) |
| XCore | VisionEngineTypes.h | AkashicTrigger | 192 | implemented | PDB LF_FIELDLIST 0x69293 | yes | 阿卡西记录触发器 (AlphaValue +0xA8, BlendingTime +0xAC, SummonPos +0xB0, ApplyRotation +0xBC) |
| XCore | VisionEngineTypes.h | AttachToAttackerTrigger | 200 | implemented | PDB LF_FIELDLIST 0x7601F | yes | 附着攻击者触发器 (fLifeTime +0xA8, fAttachDist +0xAC, fAngle +0xB0, vOffset +0xB4, iApplyWeightRAnk +0xC0) |
| XCore | VisionEngineTypes.h | AnimSpeedTrigger | 176 | implemented | PDB LF_FIELDLIST 0x69464 | yes | 动画速度触发器 (fDuration +0xA8, fSpeed +0xAC) |
| XCore | VisionEngineTypes.h | CounterAttackTrigger | 320 | implemented | PDB LF_FIELDLIST 0x72B38 | yes | 反击触发器 (fMinRange +0xA8, fMaxRange +0xAC, fAngle +0xB0, szAniName[128] +0xB4, iProbability +0x134, fDuration +0x138) |
| XCore | VisionEngineTypes.h | DefenseTypeTrigger | 176 | implemented | PDB LF_FIELDLIST 0x6B82D | yes | 防御类型触发器 (sDefenseType +0xA8, bImmunity +0xAA, nHitEffect +0xAC) |
| XCore | VisionEngineTypes.h | CollisionChangeTrigger | 176 | implemented | PDB LF_FIELDLIST 0x6C7D0 | yes | 碰撞变更触发器 (nCollisionType +0xA8) |
| XCore | VisionEngineTypes.h | AutoRotationTrigger | 176 | implemented | PDB LF_FIELDLIST 0x6F47F | yes | 自动旋转触发器 (fDuration +0xA8, fSpeed +0xAC) |
| XCore | VisionEngineTypes.h | LinkSkillTrigger | 184 | implemented | PDB LF_FIELDLIST 0x7872D | yes | 链接技能触发器 (nLinkType +0xA8, nSkillID +0xAC, nProbability +0xB0, fDuration +0xB4) |
| XCore | VisionEngineTypes.h | ApplyPassiveSkillTrigger | 176 | implemented | PDB LF_FIELDLIST 0x6DA23 | yes | 应用被动技能触发器 (iSkillGroupID +0xA8) |
| XCore | VisionEngineTypes.h | MyBuffControlTrigger | 192 | implemented | PDB LF_FIELDLIST 0x6E06E | yes | 我的Buff控制触发器 (cMyBuffControlType +0xA8, cBuffType +0xA9, shApplyCount +0xAA, arBuffID[5] +0xAC) |
| XCore | VisionEngineTypes.h | CameraShakingTrigger | 192 | implemented | PDB LF_FIELDLIST 0x6D913 | yes | 镜头震动触发器 (Frequence +0xA8, Intencity +0xAC, RangeMin +0xB0, RangeMax +0xB4, iType +0xB8) |
| XCore | VisionEngineTypes.h | CameraZoomTrigger | 184 | implemented | PDB LF_FIELDLIST 0x753F7 | yes | 镜头缩放触发器 (ZoomDepth +0xA8, ZoomHeight +0xAC, ZoomEndType +0xB0) |
| XCore | VisionEngineTypes.h | SoundPlayTrigger | 856 | implemented | PDB LF_FIELDLIST 0x7899D | yes | 音效播放触发器 (SoundID +0xA8, szEventName[128] +0xAC, szSoundEventName[128] +0x12C, szAttachBone[128] +0x1AC, fFadeOutTime +0x22C, ChargeLevel +0x230, iSkillLevel +0x234, sSkillCondition +0x238, CombatType +0x23C, nLinkTriggerType +0x240, bOnlyPlayOne +0x244, szLinkAnimation[128] +0x245, szDivergenceValue[128] +0x2C5, pszProjectName +0x348, nPromotionLevelLimit +0x350, bDontStopWhenHide +0x354) |
| XCore | VisionEngineTypes.h | TrajectoryTrigger | 176 | implemented | PDB LF_FIELDLIST 0x6EC8B | yes | 弹道触发器 (LeftHand +0xA8) |
| XCore | VisionEngineTypes.h | ScreenBlurTrigger | 176 | implemented | PDB LF_FIELDLIST 0x683FB | yes | 屏幕模糊触发器 (BlurType +0xA8, Speed +0xAC) |
| XCore | VisionEngineTypes.h | AlphaBlendingTrigger | 184 | implemented | PDB LF_FIELDLIST 0x69F97 | yes | 阿尔法混合触发器 (StartAlphaValue +0xA8, EndAlphaValue +0xAC, OnlyWeapon +0xB0, OnlySubWeapon +0xB1) |
| XCore | VisionEngineTypes.h | CreateEffectTrigger | 864 | implemented | PDB LF_FIELDLIST 0x76821 | yes | 创建特效触发器 (szCreateParticleFile[128] +0xA8, szDestroyParticleFile[128] +0x128, szLinkAnimation[128] +0x1A8, szAttachableBone[128] +0x228, ParticleOffsetPos +0x2A8, ParticleOffsetRot +0x2B4, ParentDirection +0x2C0, ChargeLevel +0x2C4, iSkillLevel +0x2C8, sSkillCondition +0x2CC, CombatType +0x2D0, MinEndTime +0x2D4, bEffectShowAlways +0x2D8, szDivergenceValue[128] +0x2D9) |
| XCore | VisionEngineTypes.h | ShaderChangeTrigger | 568 | implemented | PDB LF_FIELDLIST 0x77134 | yes | 着色器变更触发器 (szShaderFile[128] +0xA8, szTechnicque[128] +0x128, szParamName[128] +0x1A8, vecParamValue +0x228) |
| XCore | VisionEngineTypes.h | MeshAttachmentTrigger | 600 | implemented | PDB LF_FIELDLIST 0x73E35 | yes | 网格附着触发器 (AttachPos +0xA8, AttachRot +0xB4, AttachScale +0xC0, szBoneName[128] +0xCC, szModelFileName[128] +0x14C, szAnimName[128] +0x1CC, AttachToBone +0x24C, UseSubWeapon +0x24D, ModelID +0x250) |
| XCore | VisionEngineTypes.h | CharacterCameraLockTrigger | 168 | implemented | PDB LF_FIELDLIST 0x76CE0 | yes | 角色镜头锁定触发器 (仅基类，无自有成员) |
| XCore | VisionEngineTypes.h | AttackJunctionTrigger | 192 | implemented | PDB LF_FIELDLIST 0x4A946 | yes | 攻击连接触发器 (nJunctionType +0xA8, fCheckTime +0xAC, nNextSkillID +0xB0, fCheckTime2 +0xB4, nNextSkillID2 +0xB8) |
| XCore | VisionEngineTypes.h | InputFlagTrigger | 176 | implemented | PDB LF_FIELDLIST 0x73560 | yes | 输入标记触发器 (nInputFlag +0xA8, nSkillLevel +0xAC) |
| XCore | VisionEngineTypes.h | DeathShaderTrigger | 296 | implemented | PDB LF_FIELDLIST 0x77BF4 | yes | 死亡着色器触发器 (szShaderFileName[128] +0xA8) |
| XCore | VisionEngineTypes.h | ShaderEffectTrigger | 216 | implemented | PDB LF_FIELDLIST 0x7673E | yes | 着色器特效触发器 (ShaderEffectType +0xA8, LifeTime +0xAC, CaptureTime +0xB0, FrameTime +0xB4, Strength +0xB8, Speed +0xBC, ColorR +0xC0, ColorG +0xC4, ColorB +0xC8, ColorA +0xCC, SkillLevel +0xD0, OnlyWeapon +0xD4) |
| XCore | VisionEngineTypes.h | CameraAnimTrigger | 304 | implemented | PDB LF_FIELDLIST 0x6D01B | yes | 镜头动画触发器 (szAnimName[128] +0xA8, bIsSkill +0x128, nCameraFlag +0x12C) |
| XCore | VisionEngineTypes.h | CameraFOVTrigger | 176 | implemented | PDB LF_FIELDLIST 0x76EEB | yes | 镜头视野触发器 (BlendingTime +0xA8, FovValue +0xAC) |
| XCore | VisionEngineTypes.h | VAnimationInfo | 312 | implemented | IDA struct | yes | Vision Engine 动画信息 (完整布局已还原) |
| XCore | VisionEngineTypes.h | VBaseResourceLump | 104 | implemented | IDA struct | yes | Vision Engine 基础资源块 |
| XCore | VisionEngineTypes.h | VActionResourceLump | 232 | implemented | IDA struct | yes | Vision Engine 动画资源块 (完整布局已还原)；m_arJumpInfos 为 std::vector<VJumpInfo> |
| XCore | VisionEngineTypes.h | VJumpFrame | 8 | implemented | PDB LF_STRUCTURE 0x2C04B | yes | 跳跃关键帧 (fTime +0x00, fZPos +0x04) |
| XCore | VisionEngineTypes.h | VJumpInfo | 152 | implemented | PDB LF_FIELDLIST 0x77C03 | yes | 跳跃信息 (szName[128] +0x00, arTranslationFrames VArray<VJumpFrame> +0x80) |
| XCore | VisionEngineTypes.h | VChunkFile | 240 | implemented | PDB LF_CLASS 0x49820 | no | 分块文件读取类; Read 为外部引擎导入, 当前重建提供最小兼容接口 (ReadShort/ReadInt/Readbool) |
| XCore | VisionEngineTypes.h | SuboComboDesc | 132 | implemented | PDB tagSUBO_COMBO_DESC 0x6CD04 | yes | 从属连击描述 (szAniName[128] +0x00, iUseableLevel +0x80) |
| XCore | VisionEngineTypes.h | tagHIT_COLLISION_CYLINDER | 52 | implemented | IDA struct | no | 圆柱碰撞体数据 |
| XGameServer | ActionResMgr.h | AttackJudgmentTrigger | 2464 | implemented | PDB LF_FIELDLIST 0x485B3 | yes | 攻击判定触发器 (sAttackRangeType +0xA8, sAttackType +0xAA, sAttackCollision +0xAC, iSpawnObjectID +0xB0, sAttackRange +0xB4, sProjInfo +0xD8, sHitEffect +0x520, sReactionInfo +0x550, sConnectionInfo +0x5A8, sGrapInfo +0x7E0, sContinuousMeleeInfo +0x884, iChargeLevel +0x90C, iSkillLevel +0x910, sSkillCondition +0x914, iCombatType +0x918, szDivergenceValue +0x91C, sWeakAttackStiffenRatio +0x99C, shGroupID +0x99E) |
| XGameServer | ActionResMgr.h | tagREACTION_INFO | 88 | implemented | PDB LF_STRUCTURE 0x78396 | yes | 反应信息 (iTargetType +0x00, iTargetStatus +0x04, iTargetGrade +0x08, bUseTargetWeight +0x0C, bApplyPcSABreak +0x0D, fDamageRate +0x10, iBuffID +0x14, iAuraID +0x18, iReactionType +0x1C, fReactionDist +0x20, fReactionHeight +0x24, fReactionSpeed +0x28, iReactionArrow +0x2C, fReactionHeightAir +0x30, fReactionAngle +0x34, fSlowRate +0x38, fSlowTime +0x3C, fSlowDelayTime +0x40, fHitFreezeTime +0x44, bCheckCounter +0x48, iConditionBuffID +0x4C, fConditionBuffDamageRate +0x50, bIgnoreTargetInvincible +0x54) |
| XGameServer | ActionResMgr.h | tagHIT_EFFECT | 48 | implemented | PDB LF_STRUCTURE 0x67F89 | yes | 命中特效信息 (bTraceFXView +0x00, fTraceFXRotation +0x04, iBattleMaterialID +0x08, fVictimMoveDist +0x0C, iVictimShakingValue +0x10, fVictimShakingTime +0x14, bHitShaderView +0x18, iHitShaderR +0x1C, iHitShaderG +0x20, iHitShaderB +0x24, fHitShaderStrength +0x28, fHitShaderLifeTime +0x2C) |
| XGameServer | ActionResMgr.h | tagGRAP_INFO | 164 | implemented | PDB LF_STRUCTURE 0x68225 | yes | 抓取信息 (strAttachBoneName[128] +0x00, vecOffset +0x80, vecDropPos +0x8C, qYaw +0x98, qPitch +0x9C, qRoll +0xA0) |
| XGameServer | ActionResMgr.h | tagPROJECTILE_INFO | 1096 | implemented | PDB LF_STRUCTURE 0x75C30 | yes | 投射物信息 (vStartPos +0x00, vDir +0x0C, fMoveSpeed +0x18, fLifeTime +0x1C, 3x char[128] +0x20/0xA0/0x120, bIsGravity +0x1A0, eDirectionType +0x1A4, ..., sCollisionRange +0x400, fExplodeDelayTime +0x424, ..., bEffectShowAlways +0x446) |
| XGameServer | ActionResMgr.h | tagCONNECTION_INFO | 568 | implemented | PDB LF_STRUCTURE 0x6E7EA | yes | 连接信息 (iConnectionType +0x00, fLifeTime +0x04, fMaxCount +0x08, fDamageMutiple +0x0C, bUseLaserEffect +0x10, bRecalcTarget +0x11, bInvisibleWhenNoTarget +0x12, strEffect[128] +0x13, strEffect2[128] +0x93, strStartBone[128] +0x113, strEndBone[128] +0x193, sAttackRange +0x214) |
| XGameServer | ActionResMgr.h | tagCONTINUOUS_MELEE_INFO | 136 | implemented | PDB LF_STRUCTURE 0x6B179 | yes | 连续近战信息 (fLifeTime +0x00, fIntervalTime +0x04, szTraceBoneName[128] +0x08) |
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
| Common/XNet/XCommon/PSServer | PSServerDB.h | PS_WORLD_EVENT_INFO_REQ | 4 | verified | GameServer PDB LF_CLASS 0x2F4A4 + source build | yes | nEventID int32 at +0x00 preserves the 4-byte request. |
| Common/XNet/XCommon/PSServer | PSServerDB.h | PS_WORLD_EVENT_DAILY_REWARD_REQ | 4 | verified | GameServer PDB LF_CLASS 0x2F4AA + source build | yes | nEventID int32 at +0x00 preserves the 4-byte request. |
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
| XGameServer | ThreadLocalData.h | ThreadLocalData | 5200 | blocked | GameServer PDB UDT + IDA constructor | no | Original embedded aggregate size is 0x1450; the active ThreadLocalData_Stub.cpp adapter is intentionally not layout-compatible and does not claim embedded manager, pool, script, or navmesh recovery. |
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
| Common/XNet/XCommon/PSServer | PSServerMail.h | PS_PROFILE_PHOTO_UPDATE | 0x18 | verified | GameServer PDB UDT 0xc554 field list + source static_assert + build | yes | ST_PROFILE_PHOTO_INFO stPhoto +0x00, bool bDelete +0x10. |
| XGameServer/actor/component | GocEntity.h | ST_HAVE_PROFILE_PHOTO_INFO | 0x18 | verified | GameServer PDB UDT 0x3680 field list + source static_assert + build | yes | TB_PHOTO_ITEM* pTB_PHOTO_ITEM +0x00, ST_PROFILE_PHOTO_INFO stInfo +0x08; two-arg constructor. |
| Common/XNet/XCommon/PSServer | PSServerFriend.h | PS_REQ_FRIEND_BLOCK_ADD | 0x30 | verified | GameServer PDB UDT 0xfb75 field list + source static_assert + build | yes | dwReqUAID +0x00, wchar strTargetName[21] +0x04. |
| Common/XNet/XCommon/PSServer | PSServerFriend.h | PS_REQ_FRIEND_BLOCK_DELETE | 0x30 | verified | GameServer PDB UDT 0xfb73 field list + source static_assert + build | yes | dwReqUAID +0x00, wchar strTargetName[21] +0x04. |
| Common/XNet/XCommon/PSServer | PSServerFriend.h | PS_RES_FRIEND_RECOMMAND | 0x28 | verified | GameServer PDB UDT 0xfb6b field list + source static_assert + build | yes | dwUCID +0x00, vector vecFriends +0x08. |
| XGameServer/actor/component | GocFriend.h | CGocFriend m_tNextRecommandTime/m_tNextRecruitTime | 0x08 | verified | GameServer PDB member type 0x52FE (ATL::CTime) + source build | yes | PDB shows both time members as ATL::CTime (8 bytes), not map. |
| XGameServer/actor/component | GocBooster.h | E_BOOSTER_TYPE | 0x01 | verified | GameServer PDB enum 0x55e2 field list + source enum + build | yes | Party=0, Echelon=1, Item=2, Event=3, NetCafe=4, Day_Event=5, NetCafe_SG_EVENT=6, Max=7. |
| Common/XNet/XCommon | PSCommon.h | PS_EVENT_NETCAFE_ITEM_DELETE | 0x20 | verified | GameServer PDB UDT 0xe4d0 field list + source build | yes | Sole member vector<unsigned long> vecDeleteItem. |
| Common/XNet/XCommon/PSServer | PSServerDB.h | PS_WORLD_EVENT_REGISTER_REQ | 0x04 | verified | GameServer PDB UDT 0x2f4a6 field list + source static_assert + build | yes | nEventID int +0x00. |
| Common/XNet/XCommon/PSServer | PSServerDB.h | PS_WORLD_EVENT_DAILY_REWARD_RES | 0x04 | verified | GameServer PDB UDT 0xea4e field list + source static_assert + build | yes | nEventID int +0x00. |
| Common/XNet/XCommon/PSServer | PSServerDB.h | PS_WORLD_EVENT_REGISTER_RES | 0x14 | verified | GameServer PDB UDT 0xea50 field list + source static_assert + build | yes | nEventID+0, nTotalCount+4, nMyCount+8, nCount+12, byDailyRewardState+16. |
| Common/XNet/XCommon/PSServer | PSServerDB.h | PS_WORLD_EVENT_REWARD_RES | 0x0C | verified | GameServer PDB UDT 0xea76 field list + source static_assert + build | yes | nEventID+0, byRewardType+4, nRewardIndex+8 (REQ 同布局). |
| Common/XNet/XCommon/PSServer | PSServerChat.h | PS_CHAT_NOTIFY | 0x08 | verified | GameServer PDB UDT 0x6a026 field list + source static_assert + build | yes | nType int+0, nValue int+4. |
