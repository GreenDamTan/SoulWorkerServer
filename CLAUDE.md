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
cmake --build build --target LoginServer -- -j8
cmake --build build --target RelayServer -- -j8
cmake --build build --target GameServer -- -j8
cmake --build build --target ControlServer -- -j8
```

Use a multi-threaded build by default to avoid unnecessary compile time. If a build fails, inspect the complete build output or saved error log before changing code. If parallel compilation triggers LLVM memory pressure, retry the affected target serially:

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

## Reverse-engineering workflow

`docs/reconstruction-workflow.md` remains the authoritative detailed workflow. This section integrates its day-to-day mandatory rules; detailed examples, full schemas, historical notes, and target-specific procedures remain in that document.

### Target, workspace, and evidence discipline

- Work only on the EXE, DLL, or PDB explicitly specified by the user. Never infer the active target from summaries, prior progress, filenames, nearby artifacts, open IDA instances, or an apparent recovery frontier. Only an explicit user instruction may switch targets.
- Run commands and repository operations from this root. After an interruption or any uncertainty about the working directory, verify the `CMakeLists.txt`, `F/`, `docs/`, and `tmp/` anchors before proceeding.
- Treat `tmp/pdb/<Target>.pdb.*.txt` as authoritative project metadata equivalent to the PDB. Use `llvm-pdbutil` dumps for files, modules, symbols, types, globals, and publics; use `cvdump` for lines, OMAP, FPO, section contributions, and headers. When available, use both layers rather than treating either as optional.
- For source ownership, use evidence in this order: PDB module/OBJ, symbol, line, PDB file path, IDA or export summaries, then inference. `lines.txt` improves location accuracy but does not override module/OBJ ownership. Consult OMAP before treating raw cvdump procedure addresses as IDA virtual addresses.
- Current-target IDA, PDB metadata, assembly, and landed source must be cross-checked. `tmp/export-for-ai/` and other targets' PDBs are read-only secondary evidence; they may support shared-symbol, protocol, RTTI, ABI, or dependency analysis but never replace the user-selected primary target.
- Do not invent PDB names, fields, path casing, file ownership, or OBJ-to-source mappings. Preserve uncertainty with the standard TODO markers rather than presenting inference as fact.
- Use PDB/OBJ evidence to recover PascalCase file and directory names. If the landed temporary path differs, record the original PDB ownership separately and do not pretend the temporary path is the final conclusion.
- In `<Target>-path-recovery-index.md`, `original_lower_path` must hold the PDB-derived path relative to `F/_PROGRAM_HG/Source` (for example, `Soulworker/GameServer/...`), never a drive-qualified PDB path. Record the independently recovered PascalCase path in `recovered_pascal_path`.

### Source fidelity and architectural boundaries

- Preserve original function boundaries, overloads, branch order, structure layout, field order, padding, initialization, locking, exception behavior, cleanup, and serialization. Do not simplify, merge, inline, omit, or rewrite original logic merely because it is unused, repetitive, or difficult to compile.
- A standard-library replacement for a legacy library is allowed only when all observable semantics, indexes, ordering, ownership, cleanup, and synchronization behavior match the evidence.
- Decompilation is evidence, not a final answer. For complex layouts, containers, state machines, SQL mappings, initialization/destruction chains, and call forwarding, compare IDA/PDB/assembly with callers, callees, and current source before claiming a result.
- Preserve existing Chinese source comments. Do not delete or translate them during formatting, refactoring, or reconstruction. Add comments only when evidence or a non-obvious unresolved condition needs to be recorded.
- Put shared logic in shared modules such as `XCore`, `XSCommon`, `base`, and `XBaseServer`. Shared layers must not include, instantiate, or otherwise depend on service-specific business types; use interfaces, callbacks, factories, or injection at the boundary.
- Preserve established fragment architecture and include ordering. Keep `PSServer.h` as the aggregator/shim for `PSServer/` fragments, with each protocol type and its serialization operators in the appropriate fragment. Keep `DBLoadTable.h` as the section-driven aggregator; table definitions belong in their `TB_*.h` fragments, which must retain their required repeated-include/section-macro behavior.
- When restoring a new source file, splitting a file, or adding an artificial support file, update the relevant `CMakeLists.txt` wiring in the same change so the target remains buildable.
- Maintain Windows MSVC/clang-cl and Linux GCC/Clang portability. Isolate repeated platform differences behind a shared compatibility layer; do not scatter business-logic `#ifdef`s. A minimal compatibility implementation may preserve buildability but must not change business behavior.
- Artificial code not recovered from an original symbol—including files, functions, inferred names, temporary types, wrappers, mocks, compatibility layers, and test helpers—must use the `GreenDamTan_` prefix. Reconstruction-only diagnostic logging must use the `GreenDamTan_log` prefix with stable, searchable context.

### Function and type restoration loop

1. Lock `CURRENT_TARGET` to the user-selected target, read its latest progress tail plus function and type indexes, and resume the recorded frontier before opening unrelated work.
2. Select a source-owned batch sized to the evidence and function complexity. When PDB/IDA ownership and dependencies are clear, prefer a coherent batch of approximately 10 to 15 small or medium, tightly related functions before one proportionate verification pass, avoiding redundant builds and reviews. Keep large or control-flow-heavy functions isolated or in very small batches so decompilation, assembly comparison, and landed source do not exhaust the working context. Prioritize real business logic and close `decompiled`, `asm_restored`, or `blocked` entries before randomly expanding the frontier.
3. Decompile one batch, immediately write each recovered implementation into its source file, then compare the landed result with IDA/PDB/assembly evidence. Decompiling without a source edit is not completion.
4. If a signature conflicts with source, investigate overloads first. Every overload has an independent IDA address, implementation, and func-index row; never validate or overwrite one overload using another.
5. If decompilation is insufficient, recover from assembly and mark the unresolved portion `// TODO: 汇编还原`. Any inferred, test-only, path-ownership, or human-review-required logic must use exactly one of: `// TODO: 推测结果`, `// TODO: 仅做测试用`, `// TODO: 推测目录归属`, or `// TODO: 需人工审查`.
6. Every stub or partial implementation must state its recovery status, IDA address, missing behavior, and dependencies. A bare `// TODO` or placeholder return is not acceptable.
7. Build periodically, and build the touched target before reporting the round complete.

### Ledger and progress requirements

For every reconstruction round, maintain these target-bound documents under `docs/` in this exact order:

1. `<Target>-func-index.md`
2. `<Target>-type-index.md`
3. `<Target>-path-recovery-index.md`
4. `<Target>-current-target-progress.md`

- The three index documents are pure, stable indexes: title, table header, and entries only. Do not add dates, batch narratives, cumulative counts, process notes, or long verification prose. Put analysis, blockers, and verification narrative exclusively in current-target progress.
- Function, type, and path indexes must be full inventories, not lists limited to already analyzed items. Use conservative placeholders when evidence is incomplete, then improve records as evidence arrives. Keep stable sorting and remove duplicates in favor of the more complete, stronger-evidence entry.
- Function entries use stable statuses: `pending`, `decompiled`, `implemented`, `asm_restored`, `verified`, or `blocked`. New entries default to `verified = no` and `verification = -`. A function may be `verified` only after a successful build and a source-to-IDA/PDB/assembly comparison establishes a match, semantic equivalence, or a fixed discrepancy.
- Type entries record only types, not methods. Preserve stable `field_count`, byte `size`, source, and confidence values; do not conflate partial layout evidence with `verified` status.
- Path entries distinguish current-target business files, shared-layer files, cross-target dependencies, and manual additions. PDB-backed paths replace weaker placeholders when evidence becomes available; manual support files must not masquerade as original PDB paths.
- Every round appends a new English progress record at the file end. Obtain the real local timestamp before writing it, include the local offset and active model name, maintain chronological order, and never rewrite historical entries except to correct a verified timestamp error.
- Progress must identify the actual frontier, source evidence, landed work, comparison result, blockers, backlog, and a concrete next target. If an index had no relevant change, state that explicitly in progress.
- New ledger prose must be English and ASCII-safe unless preserving original evidence. After an edit, read the relevant lines back as UTF-8/UTF-8 BOM where applicable; terminal mojibake alone is not evidence of file corruption.

### Editing, commands, validation, and commits

- Before each edit, read the latest target content and use unique surrounding context. After the edit, read the changed section back. Favor minimal incremental edits and do not overwrite protected nearby comments or historical progress while changing an unrelated rule.
- Use `Read`, `Glob`, and `Grep` before writing scripts. Use scripts only for work that cannot be reliably inspected directly; validate script prerequisites first, keep output ASCII-safe and narrow, do not parallelize dependent scripts, and diagnose a failed assumption before rerunning it.
- On Windows, use the provided `bash`, `python`, or PowerShell environment as appropriate, but never hard-code Unix executable paths such as `/usr/bin/bash`, `/bin/sh`, or `/usr/bin/python`. Clearly label commands that require Linux or WSL.
- Documentation-only changes may use a focused documentation check: verify table headers and rule consistency, scan prohibited index content, confirm progress append position, ensure target purity, and re-read the changed documentation. Source, build, resource, runtime, network, database, shared-layer, aggregator, or include-chain changes require an appropriate target build.
- Any non-trivial round touching three or more files, shared/common code, runtime/network/database/resource loading, or central aggregators/dispatch/include chains also requires a smoke run and inspection of the relevant logs or startup stage. Build success alone is not sufficient.
- Independently review large changes for reverse dependencies, boundary violations, missed include/cleanup/call ordering, and unsupported claims of certainty.
- When committing reconstruction work, include restored code and synchronized ledger updates in the same commit. Use `type(scope): 中文描述`; write title and body in Chinese, omit `Round`, group changes by file, list restored functions as `FunctionName (IDA address) 动作：说明`, and include verification results.

#### Commit authorization and message format

- Only the user may initiate a commit. Do not stage, commit, amend, push, create a branch, or treat completed work or a discussion of commit format as authorization unless the user explicitly asks to commit.
- Use a concise, concrete Conventional Commit title: `type(scope): 中文结果`. Select `feat` for recovered behavior, `fix` for a correction to recovered behavior, and `docs` only for documentation-only work. Do not use vague titles such as `update`, `progress`, or `Round`. 标题必须能看出本次改了什么（例如具体的类/基类 ABI/工厂等），不要写"还原了 N 个函数"这类不说明内容的计数描述。
- Start the body with one truthful tool/model provenance line when it is needed, then group the actual diff by complete repository-relative file path. Do not copy another commit's provenance or claim a model/tool that did not produce the change. provenance 行应写明实际使用的大模型名称与编程工具名称及版本（例如 `京东 GLM-5 ClaudeCodeBest v2.6.13`），不要写 IDA 端口、反编译地址等对后续 review 无帮助的内容。
- Under each file, describe precise additions, corrections, removals, or ABI/layout changes. Every restored function uses `FunctionName (IDA address) 动作：说明`; include the actual address and behavior, not a generic statement that functions were restored.
- End with a `验证` section containing only checks actually run and their outcomes. For reconstruction commits, include the relevant build, smoke or startup-stage check when required, and IDA/PDB comparison evidence.
- Large milestone commits may end with a concise factual scope summary and actual file count. Do not paste generated diff statistics or inflate the body with unrelated files.

```text
feat(GameServer): 中文、具体的恢复结果

<actual tool/model provenance when needed>

F/path/to/file.cpp
- FunctionName (0x140000000) 恢复：精确行为说明。

F/path/to/file.h
- StructName 修正：PDB 布局或 ABI 说明。

docs/Target-func-index.md
- 装饰符号状态和重复项处理说明。

验证
- <actual build command>：<actual result>。
- <actual smoke or startup check>：<actual result>。
- IDA/PDB：<actual comparison result>。
```

## IDA MCP usage

When multiple IDA instances are open, list and select the correct instance, then pass its port explicitly. Use current-target IDA/PDB evidence before relying on exports. Relevant operations include listing functions, globals, types, and strings; decompiling and disassembling; xref analysis; and setting names, prototypes, comments, or type declarations when warranted by evidence.

## Agent operational note (user requirement)

- 思考深度（reasoning effort）：默认按系统配置与当前任务复杂度决定（本项目的 API 调用在未开启思考时会报错）。最低限度为 low，任何对话轮次都不得关闭思考、不得以无思考模式运行；若任务简单可按 low 运行，复杂任务则相应提高。
- 加速策略：在必要且合适的时机，尽量多开后台 agent 并行加快速度。多开时注意避免互相冲突：不并行修改同一文件/同一构建产物/同一工作树可变状态；读取型证据预取（IDA 反编译、PDB 布局提取、搜索）可以放心并行，写文件与构建必须串行或明确分工。
- 不开 agent 的默认原则：由于等待 agent 完成的机制存在故障，如果当前不需要并行跑东西，就不要开 agent 执行；只有在确有多路独立工作（如只读证据预取、独立验证）需要并行时才开，且开完必须能收到其结果。
- 后台 agent 轮询纪律：当子 agent 在后台运行时，应当等待其完成通知，而不是频繁轮询。若确有必要查询进度，两次查询之间的间隔最低不得低于 30 秒。因沙箱限制，Sleep 工具会被自动续跑机制立即打断，bash/PowerShell 前台的长 `sleep` / `Start-Sleep` 也会被沙箱拦截；经验证，唯一可用的阻塞方法是用 Python 脚本 sleep（`python -c "import time; time.sleep(45)"`，实测成功），其余方式（Sleep 工具、前台 sleep、Start-Sleep、后台 run_in_background）都会被沙箱/续跑机制拦截或打断。用 Python sleep 阻却频繁查询，且阻塞结束后应当立即检查后台 agent 的最新情况（查看其输出文件末尾或完成通知），而不是无限阻塞空等；也可以直接结束当前回合等待后台 agent 的完成通知。
- commit 节奏：不要频繁进行小的 commit。应当把还原工作积累到一定量（例如一批约 10 个左右函数的完整还原 + 台账同步），再进行统一的编译验证与 commit 编写。中间过程以构建/冒烟做阶段性检查即可，不逐个小改动提交。
- 更新文件方式：更新文件时应尽可能使用工具能力（如 Edit / Read 直接编辑）而不是编写脚本代码解决（如用 Python 脚本做字符串替换）。只有当目标变更确实无法用工具可靠完成（如大规模批量替换、精确按模式重排）时，才退而使用脚本，并先验证脚本前提。
