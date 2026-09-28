// GameModeBase.h
// GameModeBase - Base class for game modes (Defence, Survival, Operation, Unity, etc.)
// IDA: ?Init@GameModeBase@@QEAAXXZ (0x1402a7240)

#pragma once

#include <cstdint>
#include "Soulworker/GameServer/XCore/XServer/IXObject.h"

class XMaze;
class CUser;
enum eGAMEMODE_TYPE : int;

// GameModeBase - Base class for game modes
class GameModeBase : public IXObject {
public:
    GameModeBase();
    ~GameModeBase() override;

    void Init();
    virtual void Tick(float fDelta);
    virtual void StartMode(XMaze* pMaze);
    virtual bool CheckEndMode();
    virtual void SetModeState(int nState);
    virtual void Intrusion(CUser* pUser);

    bool IsPlaying() const { return m_bPlayMode; }
    std::uint8_t GetModeResult() { return m_byModeResult; }
    XMaze* GetMaze() { return m_pMaze; }
    eGAMEMODE_TYPE GetModeType() { return m_eModeType; }

    void SendNoticePacket(XMaze* pMaze, int nType, int nValue, float fTime, bool bExceptDie);
    void SendNoticePacket(CUser* pUser, int nType, int nValue, float fTime);

protected:
    bool m_bPlayMode;
    std::uint8_t m_byModeResult;
    XMaze* m_pMaze;
    eGAMEMODE_TYPE m_eModeType;
    int m_nModeState;
};

static_assert(sizeof(GameModeBase) == 88, "GameModeBase size must match GameServer PDB");
