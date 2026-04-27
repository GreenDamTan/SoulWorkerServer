#pragma once

#include <cstdint>
#include <list>
#include <memory>
#include <vector>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XCommon/XSWCommand.h"

class CServer;

class CModeMazeMatchginMember {
public:
    // 对齐 IDA 0x14003CBE0: 构造函数先构造 m_stMemberInfo，然后调用 Clear()
    CModeMazeMatchginMember() { Clear(); }
    explicit CModeMazeMatchginMember(CServer* pServer) : m_pCurServer(pServer) { Clear(); }

    std::uint32_t GetActorID() const { return m_stMemberInfo.dwActorID; }
    std::uint32_t GetUAID() const { return m_stMemberInfo.dwUAID; }

    // 对齐 IDA 0x14003C920: 直接返回 m_wRank
    std::uint16_t GetRank() const { return m_wRank; }

    // 对齐 IDA 0x14003CAC0: wRank == 0 时设置哨兵值 0xFA00 (64000)
    void SetRank(std::uint16_t wRank) {
        if (wRank) {
            m_wRank = wRank;
        } else {
            m_wRank = static_cast<std::uint16_t>(-1536);  // 0xFA00 = 64000
        }
    }

    // 对齐 IDA 0x14003CBA0: 清空所有字段并设置哨兵 rank
    void Clear() {
        m_pCurServer = nullptr;
        m_stMemberInfo = ST_MODE_MAZE_MEMBER_INFO{};
        m_wRank = static_cast<std::uint16_t>(-1536);  // 0xFA00 = 64000
    }

    CServer* m_pCurServer = nullptr;
    ST_MODE_MAZE_MEMBER_INFO m_stMemberInfo{};
    std::uint16_t m_wRank = static_cast<std::uint16_t>(-1536);  // 哨兵值 0xFA00
};

class CModeMazeMatching {
public:
    friend class CModeMazeMatchingMgr;

    bool AutoMatchingCreate(std::uint16_t wMapID, std::uint32_t dwMatchingID, std::uint32_t dwEventRoomID);
    // 对齐 IDA: V?$shared_ptr@... = 按值传递 shared_ptr
    bool AutoMatchingEnter(std::shared_ptr<CModeMazeMatchginMember> pMember);
    bool OnUpdate();
    bool MatchingPossible();
    void MatchingWait();
    bool MakeOperationMaze();
    bool ExitMatching(std::uint32_t dwActorID, std::uint32_t dwUAID, std::uint8_t byReason);
    void SendMatchingWait();
    void SendMatchingExit(std::uint32_t dwExitActorID, std::uint8_t byReason);
    void SendCreateMatchingModeMaze(ST_CREATE_MODE_MAZE& stCreateModeMaze);
    // 对齐 IDA 0x140034580: QEAA = public, 获取匹配成员列表
    void GetMatchingMember(std::vector<std::uint32_t>& vecMember);

    std::uint32_t GetMatchingID() { return m_dwMatchingID; }  // 对齐 IDA: 非const (inline未单独出现)
    std::uint8_t GetMatchingState() { return m_byState; }  // 对齐 IDA: QEAAEXZ 非const
    std::uint8_t GetMatchingProcess() { return m_byProcess; }  // 对齐 IDA: QEAAEXZ 非const
    std::uint32_t GetEventRoomID() { return m_dwEventRoomID; }  // 对齐 IDA: QEAAKXZ 非const (IDA名: GetEventID)
    int GetMemberCount() { return static_cast<int>(m_listMatchingUser.size()); }  // 对齐 IDA: QEAAHXZ 非const
    void SetMatchingState(std::uint8_t byState) { m_byState = byState; }

private:
    std::list<std::shared_ptr<CModeMazeMatchginMember>> m_listMatchingUser;
    std::uint32_t m_dwMatchingID = 0;
    std::uint16_t m_wMapID = 0;
    int m_nModeType = 0;
    std::uint8_t m_byState = 0;
    std::uint8_t m_byProcess = 1;  // 对齐 IDA: 构造函数初始化为 1
    std::uint64_t m_dw64CheckTick = 0;
    bool m_bSendSucc = false;
    int m_nMinMember = 4;
    int m_nMaxMember = 8;
    std::uint64_t m_dw64WaitTime = 0;
    std::uint32_t m_dwEventRoomID = 0;
};
