// TXPool.h
// Object pool template class for SoulWorker server
// Reconstructed from IDA decompilation

#pragma once

#include "GreenDamTan_TxPool.h"

class CMonster;

class TXMonsterCreator : public TXPool<IXObject>::TXCreator<CMonster> {
public:
    TXMonsterCreator();
    virtual ~TXMonsterCreator() = default;

    IXObject* Create() override;
    CMonster* CreateMonster();
};

class TXMonsterDeletor : public TXPool<IXObject>::IXDeletor {
public:
    TXMonsterDeletor();
    virtual ~TXMonsterDeletor() = default;

    void Delete(IXObject* pObj) override;
};
