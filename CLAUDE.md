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

- Work on only the current user-specified target, such as `GameServer.exe`, `LoginServer.exe`, `RelayServer.exe`, or `ControlServer.exe`. Do not switch targets unless the user explicitly requests it.
- Run commands from this repository root and use repository-relative paths without a leading `src/`.
- Treat `tmp/pdb/` PDB dumps as project metadata and `tmp/export-for-ai/` as secondary offline evidence.
- Prefer shared modules (`XCore`, `XSCommon`, common protocol code) for shared logic; do not copy shared behavior into a specific server directory when it belongs in the shared layer.
- Preserve original function granularity and structure layout. Do not simplify, inline, merge, or drop fields/functions just because current code does not use them yet.
- Artificial helper code that is not recovered from original symbols must use the `GreenDamTan_` prefix. Reconstruction-only diagnostic logs should use the `GreenDamTan_log` prefix.
- For reconstruction rounds, keep the target ledgers in sync under `docs/`: `<Target>-current-target-progress.md`, `<Target>-func-index.md`, `<Target>-type-index.md`, and `<Target>-path-recovery-index.md`.
- When committing reconstruction work, commit the code changes and the corresponding ledger/documentation updates together in the same commit. Do not split restored code from its `docs/` ledger updates.
- Commit subjects must describe the specific reviewed change. Do not use meaningless progress labels such as `round 99`, `Round X`, or similar review-empty wording.
- When updating `<Target>-current-target-progress.md`, do not reorganize, rewrite, summarize, or clean up historical entries. Always append only the latest current-round entry to the end of the file.
- Newly written ledger/progress/index documentation should use English text to avoid encoding issues. Preserve original source comments and identifiers as-is.

## IDA MCP usage

IDA MCP may be available for reverse-engineering work. When multiple IDA instances are open, list/select the correct instance and pass the target port explicitly. Use IDA/PDB evidence before relying on `tmp/export-for-ai` summaries.

Useful IDA operations include listing functions/globals/types/strings, decompiling or disassembling functions, checking xrefs, and setting names/prototypes/comments when appropriate.
