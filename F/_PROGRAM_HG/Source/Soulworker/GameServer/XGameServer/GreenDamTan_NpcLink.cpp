// ============================================================================
// GreenDamTan_NpcLink.cpp - 人工链接桩层
// 用途: Npc.cpp 原始文件因历史遗留不完整类型错误被排除出活跃编译
//       (CMakeLists 注释行), 但 CNpc::UpdateSectorID 已有 IDA 精确还原体,
//       且被 XMaze::CreateNpc (0x14031A250) 活跃调用链需要。
// 本文件仅承接该符号的链接, 待 Npc.cpp 整体修复后应迁回原始文件。
// ============================================================================
#include "Soulworker/GameServer/XGameServer/Npc.h"
#include "Soulworker/GameServer/XGameServer/Sector.h"

// ============================================================================
// CNpc::UpdateSectorID
// IDA: ?UpdateSectorID@CNpc@@QEAAXXZ (0x1403A3200)
// 已精确还原 - m_pSector 非空: nSectorBoxID = GetSectorBoxID();
//   空: -1; m_stNpcInfo.nSectorID = nSectorBoxID
// ============================================================================
void CNpc::UpdateSectorID() {
    int nSectorBoxID = -1;
    if (m_pSector) {
        nSectorBoxID = m_pSector->GetSectorBoxID();
    }
    m_stNpcInfo.nSectorID = nSectorBoxID;
}
