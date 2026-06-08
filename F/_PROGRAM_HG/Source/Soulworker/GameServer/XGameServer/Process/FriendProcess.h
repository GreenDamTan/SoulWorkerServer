#pragma once

#include <cstdint>
#include <string>

// Windows type definitions
typedef unsigned char BYTE;
typedef unsigned long DWORD;
typedef unsigned __int64 ULONGLONG;

// Block information structure
struct ST_BLOCK_INFO
{
    DWORD dwUCID;           // User character ID
    BYTE byLevel;           // User level
    wchar_t strName[51];    // User name (wide string)
    
    ST_BLOCK_INFO()
    {
        dwUCID = 0;
        byLevel = 0;
        strName[0] = L'\0';
    }
};

// Block list information
struct ST_BLOCKLIST_INFO
{
    DWORD dwCount;                  // Number of blocked users
    ST_BLOCK_INFO* pBlockInfo;      // Array of block info
    
    ST_BLOCKLIST_INFO()
    {
        dwCount = 0;
        pBlockInfo = nullptr;
    }
};

// Packet structures for block operations
struct PS_FRIEND_BLOCK_ADD
{
    ST_BLOCK_INFO stBlockInfo;
};

struct PS_FRIEND_BLOCK_DELETE
{
    DWORD dwTargetUCID;
    wchar_t strTargetName[51];
};

struct PS_RES_BLOCKLIST_ADD
{
    BYTE byResult;
    ST_BLOCK_INFO stBlockInfo;
};

struct PS_RES_BLOCKLIST_DELETE
{
    BYTE byResult;
    DWORD dwTargetUCID;
    wchar_t strTargetName[51];
};

// Kick user information
struct PS_KICK_USER_INFO
{
    BYTE byKickType;        // Type of kick
    int nParam;             // Additional parameter
    // TODO: Add other fields as needed
};

// Block user class
class CBlockUser
{
public:
    CBlockUser(ST_BLOCK_INFO& stBlockInfo);
    ~CBlockUser();
    
    // Getters
    DWORD GetUCID() const;
    std::wstring GetName() const;
    
private:
    ST_BLOCK_INFO m_stBlockInfo;
};

// Forward declaration
class CUser;

// Friend/Block process class
class CFriendProcess
{
public:
    // Block list request handlers
    static bool ReqBlockListLoad(class XPacket& packet);
    static bool ReqBlockListAdd(class XPacket& packet);
    static bool ReqBlockListDel(class XPacket& packet);
};

// Server friend process class
class CServerFriendProcess
{
public:
    // Block list receive handlers
    static bool RecvBlockListLoad(class XPacket& packet);
    static bool RecvBlockListAdd(class XPacket& packet);
    static bool RecvBlockListDelete(class XPacket& packet);
};

// GOC Friend class for block functionality
class CGocFriend
{
public:
    // Block check functions
    bool IsBlock(DWORD dwUCID) const;
    bool IsBlock(wchar_t* strName) const;
    
    // Block management functions
    bool AddBlock(ST_BLOCK_INFO& stBlockInfo, bool bSend);
    void DeleteBlock(DWORD dwFriendID, wchar_t* strName, bool bSend);
    
    // Block list functions
    void SetBlockList(ST_BLOCKLIST_INFO& stBlockListInfo);
    void GetBlockList(ST_BLOCKLIST_INFO& stBlockListInfo);
    void SendBlockList();
    
    // Prepare functions
    bool PrepareAddBlock(PS_FRIEND_BLOCK_ADD* psAdd);
    bool PrepareDelBlock(PS_FRIEND_BLOCK_DELETE* psDelete);
    
    // Result handlers
    void AddBlockList(PS_RES_BLOCKLIST_ADD& psRes);
    void DeleteBlockList(PS_RES_BLOCKLIST_DELETE& psRes);
    
private:
    // TODO: Add member variables for block list storage
    // boost::multi_index_container for block users
};

// Auto block check type enumeration
enum AUTO_BLOCK_CHECK_TYPE
{
    AUTO_BLOCK_CHECK_TYPE_0 = 0,
    AUTO_BLOCK_CHECK_TYPE_1 = 1,
    // TODO: Add other types as discovered
};

// GOC Entity class for auto block
class CGocEntity
{
public:
    void CheckAutoBlockCount(AUTO_BLOCK_CHECK_TYPE eType, int nAdd, DWORD dwData);
};

// Chat process for GM commands
class CChatProcess
{
public:
    static BYTE COMMAND_GM_CMD_AUTO_BLOCK_ENG(
        CUser* pUser,
        wchar_t* szCmd1,
        wchar_t* szCmd2,
        wchar_t* szCmd3,
        int* pParam,
        __int64& refParam);
};
