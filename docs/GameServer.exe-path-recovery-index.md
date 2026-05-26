# GameServer.exe Path Recovery Index

| original_lower_path | recovered_pascal_path | file | evidence | confirmed |
| --- | --- | --- | --- | --- |

## Notes

This file records the recovery of original source path ownership for functions/types in GameServer.exe.

Path sources:
1. Source file paths identified by IDA (from .pdb info)
2. Module/unit ownership inference
3. Code structure analysis inference

Status values:
- pending: awaiting analysis/recovery
- verified: verified (path is correct)
- blocked: blocked (cannot determine ownership)
