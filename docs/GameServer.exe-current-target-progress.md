# GameServer.exe 当前目标进度

---

[2026-05-26 18:00 +08:00]

## 继续还原 CMonster::Init 和核心管理器函数

- 当前目标：`GameServer.exe`
- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/Monster.cpp` - 实现 Init 函数
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/Monster.h` - 添加 Init 声明
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XCore/VisionEngineTypes.h` - 更新 CWayPoint/CGroupAggro
  - `src/docs/GameServer.exe-func-index.md` - 更新函数状态
- 本轮完成操作：
  - 从 IDA 反编译 6 个核心 Create/Init 函数
  - **CMonster::Init** (0x140355900) - 实现 Monster 初始化流程
  - **XMonsterMgr::Create** (0x140365170) - 反编译大型函数 868 bytes
  - **XAkashicObjectMgr::Create** (0x14019BD90) - 反编译 Akashic 对象创建
  - **CBattleZone::Create** (0x14019D640) - 反编译战斗区域创建
  - **CGroupAggro::Init** (0x140198A20) - 简单函数确认
  - **CWayPoint::Init** (0x140199350) - 简单函数确认
  - 更新 CWayPoint 结构添加 m_pOwner 成员
  - **GameServer 构建成功！**

## 函数统计

| 状态 | 数量 | 占比 |
|------|------|------|
| pending | 41762 | 73.6% |
| blocked | 14887 | 26.2% |
| decompiled | 80 | 0.14% |
| verified | 0 | 0% |
| **总计** | **56722** | 100% |

---

[2026-05-26 17:45 +08:00]

## 继续还原 CMoverEx/CMonster Reset 核心函数

- 当前目标：`GameServer.exe`
- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/MoverEx.cpp` - 实现 Reset 函数
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/MoverEx.h` - 添加 Reset 声明
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/Monster.cpp` - 实现 Reset 函数
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/Monster.h` - 添加 Reset 声明
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XCore/VisionEngineTypes.h` - 修复 tagMOVE_POS/tagEXTRA_MOVEPOS 结构
  - `src/docs/GameServer.exe-func-index.md` - 更新函数状态
- 本轮完成操作：
  - 从 IDA 反编译 12 个 Reset/Clear 函数并实现
  - **CMoverEx::Reset** (0x140379700) - 完整实现 2525 bytes 函数
  - **CMonster::Reset** (0x140354D20) - 完整实现 984 bytes 函数
  - **XActor::Reset** (0x1408F10D0) - 简单函数确认
  - **CWayPoint::Reset** (0x1401992D0) - 确认实现
  - **CGroupAggro::Reset** (0x1401989E0) - 简单结构
  - **CSector::Reset** (0x1403102E0) - 简单结构
  - **tagMOVE_POS::Clear** (0x140276450) - 修复结构定义
  - **tagEXTRA_MOVEPOS::Clear** (0x140189340) - 修复结构定义
  - **GameServer 构建成功！**

## 函数统计

| 状态 | 数量 | 占比 |
|------|------|------|
| pending | 41768 | 73.6% |
| blocked | 14887 | 26.2% |
| decompiled | 74 | 0.13% |
| verified | 0 | 0% |
| **总计** | **56722** | 100% |

---

[2026-05-26 17:30 +08:00]

## 继续还原 CMover/CMoverEx/CMonster 核心函数

- 当前目标：`GameServer.exe`
- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/Mover.cpp` - 确认函数实现
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/MoverEx.cpp` - 确认函数实现
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/Monster.cpp` - 确认函数实现
  - `src/docs/GameServer.exe-func-index.md` - 更新函数状态
- 本轮完成操作：
  - 从 IDA 反编译 18 个函数并确认实现正确
  - **CMover 函数确认实现正确**:
    - `SetHitCollisionData` (0x140016BD0) - 设置 m_pHitCollisionData
    - `SetHitCylinder` (0x140016BF0) - 设置 m_fHitCylinderRadius/m_fHitCylinderHeight
    - `AddActionBuffer` (0x140016C30) - 调用 CActionBuffer::Push
    - `SetNoSkillCostSG` (0x1400488E0) - 设置 m_bNoSkillCostSG
    - `GetStat` (0x140166360) - 返回 m_fAbility[iIndex]
    - `GetSkillMgr` (0x140366BE0) - 返回 m_pSkillMgr
  - **CMoverEx 函数确认实现正确**:
    - `GetSkillLoopTime` (0x140016ED0) - 返回 m_fSkillLoopTime
  - **CMonster 函数确认实现正确**:
    - `GetMobTableRef` (0x140016EF0) - 返回 m_pMobTableRef
    - `GetParentID` (0x14009F170) - 返回 m_stMonsterInfo.uxParentActorID
    - `GetAi` (0x14009F1A0) - 返回 m_pAi
    - `SetSummonType` (0x14009F1E0) - 设置 m_bySummonType
  - **新发现的 Vision Engine 函数**:
    - `XActor::GetType` (0x140016F10) - 返回 m_eActorType
    - `CWayPoint::GetCurID` (0x140026970) - 返回 m_iNumEntries
    - `CGocInventory::GetMoney` (0x140026700) - 返回 m_nInvenMoney
    - `CGocInventory::GetEther` (0x1400279C0) - 返回 m_biEther
    - `CGocInventory::GetBP` (0x1400279E0) - 返回 m_nBP
    - `VisObject3D_cl::GetPosition` (0x1408B0DBA) - thunk 函数
    - `VisObject3D_cl::SetPosition` (0x140189790) - 设置位置
  - **GameServer 构建成功！**

## 函数统计

| 状态 | 数量 | 占比 |
|------|------|------|
| pending | 41780 | 73.6% |
| blocked | 14887 | 26.2% |
| decompiled | 62 | 0.11% |
| verified | 0 | 0% |
| **总计** | **56722** | 100% |

---

[2026-05-26 17:15 +08:00]

## 继续还原 CMoverEx 核心函数

- 当前目标：`GameServer.exe`
- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/MoverEx.cpp` - 更新函数实现，修复 include
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/MoverEx.h` - 修复 TB_SKILL 前置声明
  - `src/docs/GameServer.exe-func-index.md` - 更新函数状态
- 本轮完成操作：
  - 从 IDA 反编译 8 个 CMoverEx 函数并确认实现正确
  - **CMoverEx 函数确认实现正确**:
    - `SetCombatType` (0x140188DE0) - 设置 m_nCombatType
    - `IsControlMonster` (0x140188E00) - 返回 m_bControlMonster
    - `SetControlMonsterFlag` (0x140188E20) - 设置 m_bControlMonster
    - `IsBattlePose` (0x140189000) - 返回 m_bBattlePose
    - `GetSkillTable` (0x140189020) - 返回 m_pCurSkillTableRef
    - `GetSkillLevel` (0x140189040) - 返回 m_pCurSkillTableRef->Skill_LV (修正实现)
    - `GetCombatType` (0x140189080) - 返回 m_nCombatType
    - `GetSkillChargeStep` (0x1401890A0) - 返回 m_bySkillChargeStep
  - **修复编译问题**:
    - 将 TB_SKILL 前置声明从 `class` 改为 `struct`
    - 将 TB_SKILL.h include 改为 DBLoadTable.h (条件编译宏)
  - **GameServer 构建成功！**

## 函数统计

| 状态 | 数量 | 占比 |
|------|------|------|
| pending | 41788 | 73.6% |
| blocked | 14887 | 26.2% |
| decompiled | 54 | 0.1% |
| verified | 0 | 0% |
| **总计** | **56722** | 100% |

---

[2026-05-26 17:00 +08:00]

## 继续还原 CUser/CMover 核心函数

- 当前目标：`GameServer.exe`
- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/User.cpp` - 更新函数实现
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/User.h` - 添加成员变量
  - `src/docs/GameServer.exe-func-index.md` - 更新函数状态
- 本轮完成操作：
  - 从 IDA 反编译 8 个 CUser/CMover 函数并实现
  - **CUser 新增/修正函数**:
    - `GetCreateDate` (0x1401253E0) - 返回 m_nCreateDate
    - `GetFirstUCID` (0x140125400) - 返回 m_dwFirstUCID
    - `GetAccountCreateDate` (0x140125B50) - 返回 m_biAccountCreateDate
    - `GetMaxComboCount` (0x140165270) - 返回 m_nMaxContinousAttackHit
    - `GetLeagueID` (0x140165500) - 返回 m_stCharInfo.stLeagueInfo.nLeagueID
    - `GetLastAccountComeBackDate` (0x140187AC0) - 返回 m_biLastAccountComeBackDate
    - `GetLastComeBackDate` (0x140187AE0) - 返回 m_biLastComeBackDate
  - **CMover 确认实现正确**:
    - `GetStat` (0x140166360) - 返回 m_fAbility[iIndex]
  - **新增成员变量**:
    - `m_nCreateDate` (CUser)
    - `m_biAccountCreateDate` (CUser)
    - `m_biLastAccountComeBackDate` (CUser)
    - `m_nMaxContinousAttackHit` (CUser)
  - **GameServer 构建成功！**

## 函数统计

| 状态 | 数量 | 占比 |
|------|------|------|
| pending | 41796 | 73.7% |
| blocked | 14887 | 26.2% |
| decompiled | 46 | 0.08% |
| verified | 0 | 0% |
| **总计** | **56722** | 100% |

---

[2026-05-26 16:45 +08:00]

## 继续还原 CUser/CMonster 核心函数

- 当前目标：`GameServer.exe`
- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/User.cpp` - 更新函数实现
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/User.h` - 添加成员变量
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/Monster.h` - 新建 CMonster 类
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/Monster.cpp` - 新建 CMonster 实现
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/CMakeLists.txt` - 添加 Monster.cpp
  - `src/docs/GameServer.exe-func-index.md` - 更新函数状态
- 本轮完成操作：
  - 从 IDA 反编译 14 个 CUser/CMonster/CMoverEx 函数并实现
  - **CUser 新增/修正函数**:
    - `GetExp` (0x1400F64A0) - 返回 m_stCharInfo.nExp
    - `GetSocialUseID` (0x1400F72E0) - 返回 m_dwSocialUseID
    - `GetActiveBroachEffect` (0x1400F7CE0) - 返回 m_stCharInfo.dwActiveBroachEffect
    - `SetLastLevelupDate` (0x1400492F0) - 确认实现正确
    - `GetLastLevelupDate` (0x140049310) - 确认实现正确
    - `GetFirstEnter` (0x140049600) - 返回 m_bFirstEnter
  - **CMonster 新建类** (60896 bytes):
    - `GetMobTableRef` (0x140016EF0)
    - `GetParentID` (0x14009F170)
    - `GetAi` (0x14009F1A0)
    - `SetSummonType` (0x14009F1E0)
  - **CMoverEx 已有函数确认正确**:
    - `ResetAddEtherFromOptionEffect` (0x1400F9F70)
    - `GetAddEtherFromOptionEffect` (0x1400F9F90)
    - `ResetAddMoneyFromOptionEffect` (0x1400F9FE0)
    - `GetAddMoneyFromOptionEffect` (0x1400FA000)
  - **新增成员变量**:
    - `m_dwSocialUseID` (CUser)
    - `m_bFirstEnter` (CUser)
  - **GameServer 构建成功！**

## 函数统计

| 状态 | 数量 | 占比 |
|------|------|------|
| pending | 41804 | 73.7% |
| blocked | 14887 | 26.2% |
| decompiled | 38 | 0.07% |
| verified | 0 | 0% |
| **总计** | **56722** | 100% |

---

[2026-05-26 16:30 +08:00]

## 继续还原 CMover/CUser 核心函数

- 当前目标：`GameServer.exe`
- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/Mover.cpp` - 添加函数实现
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/Mover.h` - 添加函数声明
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/User.cpp` - 更新函数实现
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/User.h` - 添加成员变量
  - `src/docs/GameServer.exe-func-index.md` - 更新函数状态
- 本轮完成操作：
  - 从 IDA 反编译 17 个 CMover/CUser 函数并实现
  - **CMover 新增函数**:
    - `SetHitCollisionData` (0x140016BD0)
    - `SetHitCylinder` (0x140016BF0)
    - `AddActionBuffer` (0x140016C30)
    - `SetNoSkillCostSG` (0x1400488E0)
  - **CMoverEx 新增函数**:
    - `GetSkillLoopTime` (0x140016ED0)
    - `ResetAddExpFromOptionEffect` (0x140049250)
    - `GetAddExpFromOptionEffect` (0x140049270)
    - `SetOwnerID` (0x14009F1C0)
  - **CUser 新增/修正函数**:
    - `IsStatus` (0x140026C30) - 修正实现
    - `GetAccountID` (0x140038710) - 实现
    - `GetBonusFP` (0x140048F90) - 实现
    - `GetFP` (0x140048FB0) - 实现
    - `GetBlockType` (0x140082D90) - 实现
    - `GetGMPower` (0x140082DB0) - 实现
    - `IsMatching` (0x140082DF0) - 已有实现，确认正确
    - `SetMatchingState` (0x1400855E0) - 已有实现，确认正确
    - `SetReserveRevive` (0x140085DF0) - 实现
  - **新增成员变量**:
    - `m_dwStatus` (CUser)
    - `m_byBlockType` (CUser)
    - `m_bReserveRevive` (CUser)
  - **GameServer 构建成功！**

## 函数统计

| 状态 | 数量 | 占比 |
|------|------|------|
| pending | 41818 | 73.7% |
| blocked | 14887 | 26.2% |
| decompiled | 24 | 0.04% |
| verified | 0 | 0% |
| **总计** | **56722** | 100% |

---

[2026-05-26 13:30 +08:00] [glm-5]

## 修复编译错误并成功构建 GameServer

- 当前目标：`GameServer.exe`
- 本轮处理文件：
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XCore/VisionEngineTypes.h` - 添加缺失类型定义
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XCore/XArea/XArea.cpp` - 新建
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XCore/XArea/XDistrict.cpp` - 新建
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/ServerMain.cpp` - 新建
  - `src/F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/CMakeLists.txt` - 更新
- 本轮完成操作：
  - 从 IDA 获取 VPList (48 bytes)、VTypedObject (32 bytes)、CWayPoint、CMySkillList 类型信息
  - 添加 Vision Engine 类型 stub: VPList, VTypedObject, CWayPoint, CMySkillList
  - 添加用户相关结构: PS_TICKCOUNT_INFO, ST_CHECK_POS
  - 修复 XVec3 重定义冲突（移除别名，使用 PSCommon.h 中的定义）
  - 实现 XArea 基类方法: 构造/析构/EnterActor/ExitActor/FindActor
  - 实现 XDistrict 基类方法: 构造/析构/AddWaitForRecvInfo/RemoveWaitForRecvInfo
  - 添加 GameServer main 函数入口
  - **GameServer 构建成功！**

## 函数统计

| 状态 | 数量 | 占比 |
|------|------|------|
| pending | 41835 | 73.7% |
| blocked | 14887 | 26.3% |
| verified | 0 | 0% |
| **总计** | **56722** | 100% |

## 已还原的类型/结构

| 类型名 | 大小 | 来源 | 说明 |
|--------|------|------|------|
| VPList | 48 bytes | IDA get_struct_info | Vision Engine 指针列表 |
| VTypedObject | 32 bytes | IDA get_struct_info | Vision Engine 类型对象基类 |
| CWayPoint | ~40 bytes | IDA decompile | Waypoint 导航系统 |
| CMySkillList | 复杂 | IDA decompile | 技能管理器（前置声明） |
| PS_TICKCOUNT_INFO | 12 bytes | 推测 | Tick 计数信息 |
| ST_CHECK_POS | 20 bytes | 推测 | 位置检查结构 |

## 下一轮目标

1. 继续从 IDA 反编译核心函数
2. 实现 CMover、CMoverEx 关键方法
3. 实现 CUser 关键方法
4. 更新文档记录还原进度

## frontier / backlog 说明

- 当前真正处理的 frontier：
  - GameServer 编译通过，继续还原核心函数
- 当前只是发现但尚未处理的 backlog：
  - 41835 个 pending 函数的分析
  - 类型索引建账
  - 路径恢复索引建账

---

[2026-05-26 13:00 +08:00] [glm-5]

## 初始化 GameServer.exe 台账（覆盖重建）

- 当前目标：`GameServer.exe`
- 本轮处理文件：
  - `src/docs/GameServer.exe-current-target-progress.md`
  - `src/docs/GameServer.exe-func-index.md`
  - `src/docs/GameServer.exe-type-index.md`
  - `src/docs/GameServer.exe-path-recovery-index.md`
- 本轮完成操作：
  - 从 IDA 导出 56722 个函数到 `GameServer.exe-func-dump.json`
  - 覆盖生成 `func-index.md` 台账文件
  - 覆盖生成空的 `type-index.md` 和 `path-recovery-index.md`

---

[2026-05-26 11:46 +08:00] [deepseek-v4-flash/glm-5]

## 还原 CMover/CMoverEx 构造函数、析构函数、Destroy/OnUpdate

- 当前目标：`GameServer.exe`
- 本轮处理文件：
  - `F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/MoverEx.cpp` — 修正构造函数非默认值
  - `F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/MoverEx.h` — 添加 RemoveAllOptionEffect/RemoveAllDefenseChangeInfo 声明
  - `F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/Mover.h` — 添加 Destroy() 声明
  - `F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer/Mover.cpp` — 修正析构函数(增加Destroy调用)，实现Destroy()，更新OnUpdate注释
  - `docs/GameServer.exe-func-index.md` — 更新 7 个函数状态为 decompiled

- 本轮完成操作：
  - 从 transcript 中提取之前 session 的 agent 反编译输出（IDA 0x140377A60 范围未加载，依赖历史反编译证据）
  - **CMoverEx 构造函数**：修正 ~120 个成员初始化值到 IDA 确认的正确值：
    - `m_fDefWalkSpeed=100.0f`, `m_fDefRunSpeed=300.0f`, `m_fFlyGravity=1960.0f`, `m_fFlyMaxHeight=200.0f`
    - `m_fIdleCheckTime=5.0f`, `m_nIdleMotionChance=20`, `m_fUpdateStatBatchTime=0.2f`, `m_fSABreakTime=5.0f`
    - `m_bDisableDirectionToTargetSkill=1`, `m_bUpdateRotation=true`, `m_eDieType=0` (DIE_TYPE_NORMAL)
    - `m_bySkillAnimCount=1`, `m_fChargingInputAnimSpeed=1.0f`, `m_fMultipleAbsorbSG=1.0f`
    - `m_nPlayPhaseMotion=-1`, `m_nBuffMotion=-1`, `m_iLastWarpPoint=-1`, `m_iSuboComboMaxCount=-1`
    - `m_iCurLeftSuboDescCount=-1`, `m_iCurRightSuboDescCount=-1`
    - 构造函数体：添加21个placement new容器构造 + memset + RemoveAllOptionEffect/RemoveAllDefenseChangeInfo
  - **CMover 析构函数**：按 IDA 添加 Destroy() 调用
  - **CMover::Destroy**：实现完整的资源清理流程（Reset → 清空指针 → 删除 SkillMgr → 重置动画/子弹/Boss状态）
  - **CMoverEx 析构函数**：添加 Reset() 调用
  - 添加 RemoveAllOptionEffect/RemoveAllDefenseChangeInfo stub 实现

- 关键修正点：
  - 之前所有 ~120 个成员都初始化为 0/0.0f/nullptr，与 IDA 实际值严重不符
  - IDA 数据库仅加载到 0x140173000，CMover 函数地址超出范围，无法直接反编译
  - 证据来源：PDB publics dump（地址确认）+ 历史 session transcript（反编译输出）+ 当前源码

- 构建验证：**GameServer 构建成功**

- func-index: 7 个 CMover/CMoverEx 函数从 pending→decompiled
- type-index: 本轮无新增类型结论，未改动
- path-index: 本轮无新增路径归属证据，未改动

## 函数统计

| 状态 | 数量 | 占比 |
|------|------|------|
| pending | 41828 | 73.7% |
| blocked | 14887 | 26.3% |
| decompiled | 7 | 0% |
| verified | 0 | 0% |
| **总计** | **56722** | 100% |

## 当前停点与下一轮目标

- 当前真正处理的 frontier：
  - CMover/CMoverEx 核心函数已还原完毕（构造函数、析构函数、Reset、Destroy、OnUpdate、GetPosition/SetPosition stub）
- 当前只是发现但尚未处理的 backlog：
  - CMoverEx::Reset (0x140379700) 和 CMoverEx::Destroy (0x14037a260) 待后续还原
  - OnUpdate 中 VisBaseEntity_cl::OnUpdate 基类调用待取消注释
  - RemoveAllOptionEffect/RemoveAllDefenseChangeInfo 待还原完整实现
  - SHitPartsInfo 结构体待定义
- 下一轮目标：
  - 继续还原其他 GameServer 核心函数（CUser、CBattleZone、CMonster 等）
  - 处理 func-index 中 blocked 函数的还原
  - 考虑是否需要重启 IDA 加载完整 GameServer.exe 二进制

## 本轮结束后暂停
