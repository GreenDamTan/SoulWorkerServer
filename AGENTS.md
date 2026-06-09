# Project Agent Instructions

This repository is a reverse-engineering reconstruction workspace for the SoulWorker server source tree.

Before any decompilation, restoration, source reconstruction, ledger update, build verification, or commit-related work, agents must read and follow:

- `docs/reconstruction-workflow.md`

The workflow document is authoritative for reconstruction work in this project. If these project instructions, generic agent behavior, or a delegation prompt conflict with `docs/reconstruction-workflow.md`, the workflow document wins.

## Workspace Lock

- Use the current repository root as the only working directory.
- Do not prepend parent directory names to documented relative paths.
- When context may have drifted, confirm the workspace anchors before running shell, git, cmake, python, or document-editing commands:
  - `CMakeLists.txt`
  - `F/`
  - `docs/`
  - `tmp/`
- Do not rely on paths outside this workspace unless the user explicitly provides them and they are accessible from the current sandbox.

## Target Lock

- Never switch reconstruction target without explicit user instruction.
- Each reconstruction round must bind to the user-specified `CURRENT_TARGET`.
- Other PDBs, exports, or IDA instances may be used only as reference evidence according to `docs/reconstruction-workflow.md`; they must not become the active reconstruction target unless the user says so.

## IDA MCP Target Selection

- Before using any IDA MCP decompile, disassemble, struct, type, function, xref, or patch-inspection tool, call `ida-mcp_list_instances` and select the instance whose `input_file` basename matches the current `CURRENT_TARGET` exactly.
- Use only an instance where `ready` is `true` and `effective_state` is `ready`. Do not use instances that are `starting`, stale, or registered for a different executable.
- Do not rely on the IDA MCP default port. Multiple IDA instances can share or conflict on default selection; always pass the selected `port` explicitly in every IDA MCP call.
- For `CURRENT_TARGET = GameServer.exe` in the current workspace, the verified ready instance during this session was `port: 10004`; still re-check `ida-mcp_list_instances` after compaction, restart, or any context drift before assuming that port remains valid.
- If a tool call unexpectedly goes to `port: 10000` or reports that the instance is `starting`, treat it as a wrong/default-port call unless `ida-mcp_list_instances` proves that `port: 10000` is the ready instance for the current target.
- When delegating reconstruction work, include the selected IDA port and the requirement to revalidate it with `ida-mcp_list_instances` before use.

## Evidence Rules

- Treat `tmp/pdb/<CURRENT_TARGET>.pdb.*.txt` and related `cvdump` / `llvm-pdbutil` exports as project-level authoritative metadata, not casual reference text.
- Cross-check function, type, path, and layout conclusions across multiple evidence sources before recording them.
- Prefer original PDB/module/OBJ evidence over current temporary source placement when file ownership conflicts.
- Do not present guessed names, guessed layouts, or decompiler-shaped logic as confirmed evidence.

## Reconstruction Fidelity

- Preserve original function boundaries, file ownership, type layouts, field order, serialization behavior, and module boundaries as far as evidence allows.
- Do not modernize, simplify, merge, split, inline, optimize, or redesign original logic merely for style or readability.
- Preserve existing Chinese source comments. New generated ledger/progress/index text must be English, but source comments may remain Chinese when they are part of restored code or existing context.
- Keep Windows and Linux build compatibility in mind. Platform compatibility code should be minimal and should follow the workflow document's `GreenDamTan_` conventions when applicable.

## Ledger Gate

Every reconstruction round must update or explicitly account for the current target ledgers:

- `docs/<target>-func-index.md`
- `docs/<target>-type-index.md`
- `docs/<target>-path-recovery-index.md`
- `docs/<target>-current-target-progress.md`

Rules:

- New ledger, progress, index, and global index content must be written in English.
- `current-target-progress.md` entries must be appended to the end, never inserted at the beginning or into history.
- Use the actual local time command required by the workflow before appending progress timestamps.
- New function records default to `verified = no` unless the workflow's verification requirements are actually satisfied.
- Reconstruction work is not complete until the relevant ledgers are updated and verified.

## Build And Verification

- Keep `CMakeLists.txt` or related build wiring in sync when adding, moving, or splitting source files.
- Run the most relevant build target after source changes when the toolchain is available.
- Record verification results, skipped verification, or blockers in the required ledger files.

## Commit Gate

Before any git commit for reconstruction work:

- Complete the required ledger updates first.
- Output the exact ledger confirmation checklist required by `docs/reconstruction-workflow.md`.
- Use the commit-message standard from `docs/reconstruction-workflow.md`.
- Do not commit without explicit user request.

## Delegation Rule

When delegating reconstruction work to sub-agents, the prompt must include:

- the current `CURRENT_TARGET`
- the relevant ledger paths
- the requirement to read `docs/reconstruction-workflow.md`
- the rule that all newly generated ledger content must be English
- the rule that the agent must not switch targets
- the expected build or verification command, if applicable
