// GameModeBase.h
// GameModeBase - Base class for game modes (Defence, Survival, Operation, Unity, etc.)
// IDA: ?Init@GameModeBase@@QEAAXXZ (0x1402a7240)

#pragma once

#include <cstdint>

class XMaze;

// GameModeBase - Base class for game modes
class GameModeBase {
public:
    GameModeBase() : m_pMaze(nullptr), m_bPlayMode(0), m_byModeResult(0) {}
    virtual ~GameModeBase() = default;

    // IDA: ?Init@GameModeBase@@QEAAXXZ (0x1402a7240)
    // Initializes the game mode base
    void Init() {
        m_pMaze = nullptr;
        m_bPlayMode = 0;
        m_byModeResult = 0;
    }

    // Virtual methods for derived classes
    virtual void Start() {}
    virtual void Update(float fDelta) {}
    virtual void Finish() {}

    // Accessors
    XMaze* GetMaze() const { return m_pMaze; }
    void SetMaze(XMaze* pMaze) { m_pMaze = pMaze; }

    bool IsPlayMode() const { return m_bPlayMode != 0; }
    void SetPlayMode(bool bPlay) { m_bPlayMode = bPlay ? 1 : 0; }

    std::uint8_t GetModeResult() const { return m_byModeResult; }
    void SetModeResult(std::uint8_t byResult) { m_byModeResult = byResult; }

protected:
    XMaze* m_pMaze;          // Pointer to the maze
    std::int32_t m_bPlayMode;  // Play mode flag
    std::uint8_t m_byModeResult;  // Mode result
};
