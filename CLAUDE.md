# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository root and path rules

**Git repository root:** `E:\software\GAME\SoulWorker\SoulWorkerServer\SoulOfWar_Server\server\src`

All commands, git operations, build commands, source edits, and documentation updates should be run from this directory unless the user explicitly says otherwise. Relative paths in this repository are relative to this directory.

Important path anchors in this root:

- `CMakeLists.txt`
- `F/`
- `docs/`
- `tmp/`

Do not prepend another `src/` segment to repository-relative paths. Correct examples are `docs/GameServer.exe-func-index.md`, `F/_PROGRAM_HG/Source/Soulworker/...`, and `build/`. Incorrect examples are `src/docs/...`, `src/F/...`, and `src/build/...` when already working from this repository root.

## Build and run

This is a CMake C++20 project. The top-level `CMakeLists.txt` wires the reconstructed server targets through subdirectories under `F/_PROGRAM_HG/Source/Soulworker/GameServer/`.

Configured CMake targets currently include:

- `LoginServer`
- `RelayServer`
- `GameServer`
- `ControlServer`
- `DBAgent` support code through `XDBAgent`

### Configure

Use the unified build tree at `build/` from this repository root:

```powershell
cmake -S . -B build -G Ninja `
  -DCMAKE_CXX_COMPILER=clang-cl `
  -DCMAKE_MT="C:/Program Files/LLVM/bin/llvm-mt.exe"
```

`GREENDAMTAN_ENABLE_NATIVE_ODBC` defaults to `ON`. To use the old stub path, configure with:

```powershell
-DGREENDAMTAN_ENABLE_NATIVE_ODBC=OFF
```

If the Windows `clang-cl` toolchain has UBSan runtime link issues, reconfigure with trap mode or disable UBSan:

```powershell
cmake -S . -B build -G Ninja `
  -DCMAKE_CXX_COMPILER=clang-cl `
  -DCMAKE_MT="C:/Program Files/LLVM/bin/llvm-mt.exe" `
  -DUBSAN_MODE=trap
```

```powershell
cmake -S . -B build -G Ninja `
  -DCMAKE_CXX_COMPILER=clang-cl `
  -DCMAKE_MT="C:/Program Files/LLVM/bin/llvm-mt.exe" `
  -DENABLE_UBSAN=OFF
```

### Build

```powershell
cmake --build build --target LoginServer
cmake --build build --target RelayServer
cmake --build build --target GameServer
cmake --build build --target ControlServer
```

For faster parallel compilation, use `-j` with the number of cores (e.g., `-j8` for 8 cores):

```powershell
cmake --build build --target GameServer -- -j8
```

If parallel compilation triggers LLVM memory pressure, use a serial build for the target being checked:

```powershell
cmake --build build --target GameServer -- -j1
```

### Run and smoke checks

Run executables from this repository root so relative resource paths resolve against `config/`, `res/`, `table.res`, and `Log/`.

```powershell
& "build/bin/LoginServer.exe"
& "build/bin/RelayServer.exe"
& "build/bin/GameServer.exe"
& "build/bin/ControlServer.exe"
```

Quick autostop smoke run:

```powershell
$env:GREENDAMTAN_AUTOSTOP_MS=5000
& "build/bin/LoginServer.exe"
```

RelayServer has a one-shot `/TEST` mode for startup smoke:

```powershell
& "build/bin/RelayServer.exe" /TEST
```

Common logs include:

- `Log/Login/System.log`
- `Log/Login/Game.log`

### Tests and lint

There is currently no configured CMake test or lint target: no `enable_testing()`, no `add_test()`, no `ctest` wiring, and no dedicated lint script discovered. The normal validation loop is rebuilding the touched target, optionally running the executable from this repository root, and checking generated logs.

## High-level architecture

Source code is rooted at `F/_PROGRAM_HG/Source/Soulworker/`.

The reconstructed server is split into shared layers and executable-specific business layers:

- `Common/XNet/XCommon/`: shared protocol and packet payload definitions such as `PSCommon.h`, `PSServer.h`, `PSOption.h`, `PSWorld.h`, and command identifiers.
- `GameServer/XCore/`: reusable server/runtime skeleton, including IOCP-style server/client abstractions, option loading, object/session primitives, and reconstruction-only `GreenDamTan_` runtime helpers.
- `GameServer/XSCommon/`: shared game/server common code, including the resource/table subsystem.
- `GameServer/XSCommon/Table/DBLoadTable.h`: central table/resource access layer used by multiple server flows.
- `GameServer/XLoginServer/`: LoginServer business logic, user/session state, login and character processes, DB replies, control/relay sockets, and process bootstrap.
- `GameServer/XRelayServer/`: RelayServer coordination and social systems such as party, force, league, relay users, DB, and control communication.
- `GameServer/XControlServer/`: ControlServer coordinator logic, channels, maze/world-mode state, server registration, and heartbeat-related behavior.
- `GameServer/XGameServer/`: gameplay server logic, users, movers, monsters, battle zones, actor components, packet processes, and game threads.
- `GameServer/XDBAgent/`: database abstraction and SQL/log/server connection handling.

The top-level `CMakeLists.txt` currently adds these subdirectories:

```text
F/_PROGRAM_HG/Source/Soulworker/GameServer/XDBAgent
F/_PROGRAM_HG/Source/Soulworker/GameServer/XLoginServer
F/_PROGRAM_HG/Source/Soulworker/GameServer/XRelayServer
F/_PROGRAM_HG/Source/Soulworker/GameServer/XGameServer
F/_PROGRAM_HG/Source/Soulworker/GameServer/XControlServer
```

## Reverse-engineering workflow notes

`docs/reconstruction-workflow.md` is the authoritative reconstruction workflow for this repository. Follow it for target locking, IDA/PDB evidence priority, documentation ledgers, naming rules, and verification expectations.

Key operational rules from the workflow:

- Work on only the current user-specified target, such as `GameServer.exe`, `LoginServer.exe`, `RelayServer.exe`, or `ControlServer.exe`. Do not switch targets unless the user explicitly requests it, and never infer the active target from summaries, history, previous progress logs, open IDA instances, or nearby files.
- Run commands from this repository root and use repository-relative paths without a leading `src/`.
- Treat `tmp/pdb/<Target>.pdb.*.txt` dumps as authoritative project metadata equivalent to the PDB. Use `llvm-pdbutil` dumps for files/modules/symbols/types/globals/publics, and `cvdump` dumps for lines/OMAP/FPO/seccontrib/headers. Cross-check module, file, symbol, and line evidence before deciding source ownership.
- Treat `tmp/export-for-ai/` as secondary offline evidence only. Prefer current-target IDA/PDB evidence, then other explicitly selected IDA instances, before using export summaries.
- Evidence priority for path/function ownership is: PDB/module/OBJ > symbol > lines > files > IDA/export summaries > inference.
- Prefer shared modules (`XCore`, `XSCommon`, common protocol code) for shared logic; do not copy shared behavior into a specific server directory when it belongs in the shared layer.
- Shared layers (`XCore`, `XSCommon`, `base`, `XBaseServer`) must not depend on service-specific business layers. Do not include service headers or instantiate service objects from shared fallback/runtime code; use interfaces, callbacks, factories, or injection instead.
- Preserve original function granularity and structure layout. Do not simplify, inline, merge, or drop fields/functions just because current code does not use them yet.
- Restored code must remain portable across the intended compiler matrix: Windows MSVC/clang-cl and Linux GCC/Clang. Platform-specific APIs, headers, handles, sockets, time, threads, filesystem behavior, and calling conventions require explicit `_WIN32` / Linux handling or a shared compatibility wrapper.
- Preserve existing fragment/partial-include architectures. Do not put new table structs back into `DBLoadTable.h` or new protocol structs back into monolithic `PSServer.h` when the corresponding fragment directories exist. Add or update the appropriate fragment file and keep the aggregator include order intact.
- Artificial helper code that is not recovered from original symbols must use the `GreenDamTan_` prefix. Reconstruction-only diagnostic logs should use the `GreenDamTan_log` prefix.
- Any non-final logic must be explicitly marked with the standard TODO labels: `// TODO: 汇编还原`, `// TODO: 推测结果`, `// TODO: 需人工审查`, `// TODO: 推测目录归属`, or `// TODO: 仅做测试用`. Stub/partial functions must include the IDA address, recovery status, missing logic, and dependencies.
- For reconstruction rounds, keep the target ledgers in sync under `docs/`: `<Target>-current-target-progress.md`, `<Target>-func-index.md`, `<Target>-type-index.md`, and `<Target>-path-recovery-index.md`.
- A reconstruction round is not complete until ledgers are synchronized in this order: func-index, type-index, path-recovery-index, then current-target-progress. If an index has no changes, say so explicitly in the progress entry.
- `*-func-index.md`, `*-type-index.md`, and `*-path-recovery-index.md` must remain pure index tables: no dates, batch notes, round summaries, cumulative stats, or process logs. Put process notes only in `*-current-target-progress.md`.
- Never mark a function or type as `verified` unless the landed source has been compared against IDA/PDB/ASM evidence and the result is match, semantically equivalent, or fixed. Newly added index entries default to unverified/pending or decompiled.
- When updating `<Target>-current-target-progress.md`, get the local timestamp from `date "+%Y-%m-%d %H:%M %Z"`, append only to the end, include the active model name, and do not rewrite historical entries.
- Newly written ledger/progress/index documentation should use English text to avoid encoding issues. Preserve original source comments and identifiers as-is.
- For non-trivial reconstruction changes touching 3+ files, shared/common layers, runtime/network/DB/resource loading, or central aggregators/include chains, build alone is not enough. Also run an appropriate smoke check and inspect key logs/stages when feasible.
- When committing reconstruction work, commit the code changes and the corresponding ledger/documentation updates together in the same commit. Do not split restored code from its `docs/` ledger updates.
- Reconstruction commits must use `type(scope): 中文描述`. The title and body should be Chinese, must not contain `Round`, and the body should group changes by file. For restored functions, list each function as `FunctionName (IDA address) 动作：说明`, followed by verification results.

## Function Restoration Workflow (函数还原工作流程)

**CRITICAL: You must write decompiled code to source files, not just decompile.**

### 每批次工作流程（严格按顺序执行）

1. **检查 func-index**: 读取 `docs/<Target>-func-index.md`，找到 `pending` 状态的函数
2. **选取目标函数**: 选取一组待处理函数（约5个），优先选择有实际业务逻辑的函数，跳过模板/STL辅助函数
3. **反编译**: 使用 IDA MCP `decompile` 工具获取函数源码
4. **写入源文件**: 将反编译结果写入源文件
  - **如果未实现**: 将 IDA 反编译结果写入源文件
  - **如果已实现**: 对比 IDA 反编译结果，**必须精确还原**：
    - 如果现有实现是简化版本（如用 std::map 代替 boost::multi_index），需要替换为精确实现
    - 如果现有实现与 IDA 结果一致，确认正确
5. **检查写入结果**: 对比写入的代码是否符合反编译逻辑，确保函数签名和逻辑流程正确
6. **维护 func-index**: 更新 `docs/<Target>-func-index.md`
  - 状态改为 `implemented`
  - 验证列说明：精确实现则留空，简化实现则标注"简化实现(缺失xxx)"
7. **编译验证**: 定期编译验证修改是否正确

### 精确还原要求

- 函数签名必须与 IDA 完全一致
- 逻辑流程必须与 IDA 反编译结果匹配
- **库替换规则**：可以使用 std 替代 boost 等第三方库，但必须保证逻辑一致
  - `boost::multi_index` → 可用 `std::map`/`std::unordered_map` 等替代，但索引查找/插入/删除逻辑必须一致
  - `std::tr1::shared_ptr` → 使用 `std::shared_ptr`
  - `boost::function` → 使用 `std::function`
- 锁、异常处理等细节必须保留

### 禁止的行为

- ❌ 只反编译不写入源文件
- ❌ 反编译后直接标记为完成但不写代码
- ❌ 跳过写入源文件步骤
- ❌ 批量反编译多个函数后才写代码（必须每批约5个函数就写入）

### 台账文档维护

- `docs/<Target>-func-index.md`: 每个函数还原后更新，将已还原函数状态从 `pending` 改为 `implemented`
- `docs/<Target>-type-index.md`: **必要时维护**，新增类型定义时更新
- `docs/<Target>-path-recovery-index.md`: **必要时维护**，新增源文件路径时更新
- `docs/<Target>-current-target-progress.md`: 编译测试通过后追加进度记录

## IDA MCP usage

IDA MCP may be available for reverse-engineering work. When multiple IDA instances are open, list/select the correct instance and pass the target port explicitly. Use IDA/PDB evidence before relying on `tmp/export-for-ai` summaries.

Useful IDA operations include listing functions/globals/types/strings, decompiling or disassembling functions, checking xrefs, and setting names/prototypes/comments when appropriate.
