#include "Soulworker/GameServer/XDBAgent/ServerCon.h"
#include "Soulworker/GameServer/XDBAgent/SQLProcessImpl.h"
#include "Soulworker/GameServer/XCore/XServer/TXDBSocket.h"

// Per IDA 0x1400CAD80: XServerCon constructor
XServerCon::XServerCon() {
    // Per IDA: calls XClient::XClient and IXObject::IXObject (base constructors)
    // Then creates XDBStmt object (0x220 bytes) and calls RegisterProcess
    m_pDBStmt = new XDBStmt();
    RegisterProcess();
}

// Per IDA 0x1400CAE80: XServerCon destructor
XServerCon::~XServerCon() {
    // Per IDA: deletes m_pDBStmt
    if (m_pDBStmt) {
        delete m_pDBStmt;
        m_pDBStmt = nullptr;
    }
}

// Per IDA 0x1400CAF40: Register all SQL processors
bool XServerCon::RegisterProcess() {
    // Per IDA: each processor is 0x48 (72 bytes)
    // Registers in specific order with MainCmd values

    // MainCmd = 0x01
    auto* pSystemProcess = new XSQLSystemPorcess();
    if (!Register(0x01, pSystemProcess)) return false;

    // MainCmd = 0x02
    auto* pLoginProcess = new XSQLLoginProcess();
    if (!Register(0x02, pLoginProcess)) return false;

    // MainCmd = 0x03
    auto* pCharacterProcess = new XSQLCharacterProcess();
    if (!Register(0x03, pCharacterProcess)) return false;

    // MainCmd = 0x04
    auto* pPartyProcess = new XSQLPartyProcess();
    if (!Register(0x04, pPartyProcess)) return false;

    // MainCmd = 0x21
    auto* pItemProcess = new XSQLItemProcess();
    if (!Register(0x21, pItemProcess)) return false;

    // MainCmd = 0x22
    auto* pShopProcess = new XSQLShopProcess();
    if (!Register(0x22, pShopProcess)) return false;

    // MainCmd = 0x23
    auto* pTradeProcess = new XSQLTradeProcess();
    if (!Register(0x23, pTradeProcess)) return false;

    // MainCmd = 0x24
    auto* pItemUpgradeProcess = new XSQLItemUpgradeProcess();
    if (!Register(0x24, pItemUpgradeProcess)) return false;

    // MainCmd = 0x41
    auto* pQuestProcess = new XSQLQuestProcess();
    if (!Register(0x41, pQuestProcess)) return false;

    // MainCmd = 0x42
    auto* pLogGameProcess = new XSQLLogGameProcess();
    if (!Register(0x42, pLogGameProcess)) return false;

    // MainCmd = 0x44
    auto* pSkillProcess = new XSQLSkillProcess();
    if (!Register(0x44, pSkillProcess)) return false;

    // MainCmd = 0x45
    auto* pOptionProcess = new XSQLOptionProcess();
    if (!Register(0x45, pOptionProcess)) return false;

    // MainCmd = 0x81
    auto* pItemSetupProcess = new XSQLItemSetupProcess();
    if (!Register(0x81, pItemSetupProcess)) return false;

    // MainCmd = 0x05
    auto* pFriendProcess = new XSQLFriendProcess();
    if (!Register(0x05, pFriendProcess)) return false;

    // MainCmd = 0x06
    auto* pPostProcess = new XSQLPostProcess();
    if (!Register(0x06, pPostProcess)) return false;

    // MainCmd = 0x46
    auto* pSoulMetryProcess = new XSQLSoulMetryProcess();
    if (!Register(0x46, pSoulMetryProcess)) return false;

    // MainCmd = 0x07
    auto* pLeagueProcess = new XSQLLeagueProcess();
    if (!Register(0x07, pLeagueProcess)) return false;

    // MainCmd = 0x43
    auto* pWorldProcess = new XSQLWorldProcess();
    if (!Register(0x43, pWorldProcess)) return false;

    // MainCmd = 0x47
    auto* pGestureProcess = new XSQLGestureProcess();
    if (!Register(0x47, pGestureProcess)) return false;

    // MainCmd = 0x48
    auto* pDailyMissionProcess = new XSQLDailyMissionProcess();
    if (!Register(0x48, pDailyMissionProcess)) return false;

    // MainCmd = 0x25
    auto* pMyRoomProcess = new XSQLMyRoomProcess();
    if (!Register(0x25, pMyRoomProcess)) return false;

    // MainCmd = 0x26
    auto* pHelperProcess = new XSQLHelperProcess();
    if (!Register(0x26, pHelperProcess)) return false;

    // MainCmd = 0x49
    auto* pEventProcess = new XSQLEvent();
    if (!Register(0x49, pEventProcess)) return false;

    // MainCmd = 0x27
    auto* pExchangeProcess = new XSQLExchange();
    if (!Register(0x27, pExchangeProcess)) return false;

    // MainCmd = 0x28
    auto* pRankingProcess = new XSQLRankingProcess();
    if (!Register(0x28, pRankingProcess)) return false;

    // MainCmd = 0xF0
    auto* pStatisticsProcess = new XSQLStatisticsProcess();
    if (!Register(0xF0, pStatisticsProcess)) return false;

    // MainCmd = 0x08
    auto* pForceProcess = new XSQLForceProcess();
    if (!Register(0x08, pForceProcess)) return false;

    // MainCmd = 0x4A
    auto* pWeeklyMissionProcess = new XSQLWeeklyMissionProcess();
    if (!Register(0x4A, pWeeklyMissionProcess)) return false;

    // MainCmd = 0xF2
    auto* pSGNetCafeProcess = new XSQLSGNetCafeProcess();
    if (!Register(0xF2, pSGNetCafeProcess)) return false;

    // MainCmd = 0xF3
    auto* pCommonProcess = new XSQLCommonProcess();
    return Register(0xF3, pCommonProcess) != 0;
}

// Per IDA 0x1400CBCC0: OnLogOut - sets state to Finish and returns true
bool XServerCon::OnLogOut() {
    SetState(eStateFinish);
    return true;
}