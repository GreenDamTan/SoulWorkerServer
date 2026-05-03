#pragma once

#include "Soulworker/GameServer/XCore/XServer/IXObject.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_ClientBase.h"

// Forward declaration
class XDBStmt;

// XServerCon - DBAgent connection class
// Size: 131584 bytes per IDA
// Inherits: XClient (131512 bytes) + IXObject (64 bytes) + m_pDBStmt (8 bytes)
class XServerCon : public XClient, public IXObject
{
public:
    XServerCon();
    virtual ~XServerCon();

    bool RegisterProcess();
    bool OnLogOut() override;

    XDBStmt* m_pDBStmt = nullptr;
};
