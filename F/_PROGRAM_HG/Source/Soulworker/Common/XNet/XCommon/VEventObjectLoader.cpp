#include "VEventObjectDefine.h"

VEventObjectInfo::VEventObjectInfo() = default;

VEventBoxInfo::VEventBoxInfo() : VEventObjectInfo() {}

VEventPointInfo::VEventPointInfo() : VEventObjectInfo() {}

VCheckEventSpawnBoxInfo::VCheckEventSpawnBoxInfo() : VEventBoxInfo() {}

VMonsterSpawnInfo::VMonsterSpawnInfo() : VEventBoxInfo() {}
