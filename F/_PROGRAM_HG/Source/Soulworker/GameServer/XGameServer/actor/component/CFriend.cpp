// CFriend - Friend entry wrapper class implementation
// IDA verified from GameServer.exe
//
// Verified functions:
// - CFriend::CFriend(ST_FRIEND_INFO*) at 0x140085F30
// - CFriend::GetUCID at 0x140085F90
// - CFriend::GetType at 0x140085FA0
// - CFriend::GetName at 0x140085FB0
// - CFriend::GetInfo at 0x140086230
// - CFriend::UpdateInfo(ST_FRIEND_INFO*) at 0x140086280
// - CFriend::UpdateInfo(ST_FRIEND_COMMUNITY*) at 0x140086430

#include "CFriend.h"
#include <cwchar>

CFriend::CFriend() {
    // Zero initialize
    m_stFriendInfo = {};
}

// Constructor from ST_FRIEND_INFO - IDA pattern (primary constructor)
// IDA: 0x140085F30 - ??0CFriend@@QEAA@AEAUST_FRIEND_INFO@@@Z
// IDA decompiled:
//   ST_FRIEND_INFO::ST_FRIEND_INFO(&this->m_stFriendInfo);
//   qmemcpy(v3, stFriendInfo, sizeof(v3));
//   qmemcpy(this, v3, sizeof(CFriend));
CFriend::CFriend(const ST_FRIEND_INFO* stInfo) {
    if (stInfo) {
        m_stFriendInfo = *stInfo;
    } else {
        m_stFriendInfo = {};
    }
}

// Constructor from DB_FRIEND_INFO (for DB loading)
CFriend::CFriend(const DB_FRIEND_INFO* stInfo) {
    if (stInfo) {
        // Convert DB_FRIEND_INFO to ST_FRIEND_INFO
        m_stFriendInfo = {};
        wcscpy_s(m_stFriendInfo.strName, stInfo->strName);
        m_stFriendInfo.dwID = stInfo->dwID;
        m_stFriendInfo.byLevel = stInfo->byLevel;
        m_stFriendInfo.byClass = stInfo->byClass;
        m_stFriendInfo.byAwaken = stInfo->byAwaken;
        m_stFriendInfo.dwProfilePhotoID = stInfo->dwProfilePhotoID;
        m_stFriendInfo.byType = stInfo->byType;
        m_stFriendInfo.byState = stInfo->byState;
        wcscpy_s(m_stFriendInfo.strMemo, stInfo->strMemo);
        m_stFriendInfo.byChannel = stInfo->byChannel;
        m_stFriendInfo.wMapID = stInfo->wMapID;
        m_stFriendInfo.nFriendPoint = stInfo->nFriendPoint;
        m_stFriendInfo.bLogin = stInfo->bLogin;
        m_stFriendInfo.tLogOut = stInfo->tLogOut;
        m_stFriendInfo.tRemain = stInfo->tRemain;
    } else {
        m_stFriendInfo = {};
    }
}

// Constructor from DB friend format
CFriend::CFriend(const PS_DB_FRIEND* stDbFriend) {
    if (stDbFriend) {
        // Convert from DB format to internal format
        m_stFriendInfo = {};
        wcscpy_s(m_stFriendInfo.strName, stDbFriend->strName);
        m_stFriendInfo.dwID = stDbFriend->dwUCID;
        m_stFriendInfo.byLevel = stDbFriend->byLevel;
        m_stFriendInfo.byClass = stDbFriend->byClass;
        m_stFriendInfo.byAwaken = stDbFriend->byAwaken;
        m_stFriendInfo.dwProfilePhotoID = stDbFriend->dwProfilePhotoID;
        m_stFriendInfo.byType = stDbFriend->byType;
        m_stFriendInfo.byState = stDbFriend->byState;
        wcscpy_s(m_stFriendInfo.strMemo, stDbFriend->strMemo);
        m_stFriendInfo.nFriendPoint = stDbFriend->nFriendPoint;
        m_stFriendInfo.tLogOut = stDbFriend->tLogOut;
        m_stFriendInfo.tRemain = stDbFriend->tRemain;
    } else {
        m_stFriendInfo = {};
    }
}

CFriend::~CFriend() {
    // Nothing to clean up - ST_FRIEND_INFO has no dynamic allocations
}

// Get friend name - IDA returns std::wstring
// IDA: 0x140085FB0 - ?GetName@CFriend@@QEBA?BV?$basic_string@_WU?$char_traits@_W@std@@V?$allocator@_W@2@@std@@XZ
// IDA decompiled:
//   const std::wstring *__fastcall CFriend::GetName(CFriend *this, std::wstring *result)
//   {
//     std::wstring::wstring(result, this->m_stFriendInfo.strName);
//     return result;
//   }
std::wstring CFriend::GetName() const {
    return std::wstring(m_stFriendInfo.strName);
}

// UpdateInfo from ST_FRIEND_INFO - IDA: CFriend::UpdateInfo (primary)
// IDA: 0x140086280 - ?UpdateInfo@CFriend@@QEAAXAEAUST_FRIEND_INFO@@@Z
// IDA decompiled:
//   void __fastcall CFriend::UpdateInfo(CFriend *this, ST_FRIEND_INFO *stUpdate)
//   {
//     if ( stUpdate->byType )
//       this->m_stFriendInfo.byType = stUpdate->byType;
//     // Copy strName (wchar_t by wchar_t until null terminator)
//     this->m_stFriendInfo.dwID = stUpdate->dwID;
//     this->m_stFriendInfo.byLevel = stUpdate->byLevel;
//     this->m_stFriendInfo.byClass = stUpdate->byClass;
//     this->m_stFriendInfo.byAwaken = stUpdate->byAwaken;
//     this->m_stFriendInfo.dwProfilePhotoID = stUpdate->dwProfilePhotoID;
//     this->m_stFriendInfo.byState = stUpdate->byState;
//     // Copy strMemo (wchar_t by wchar_t until null terminator)
//     this->m_stFriendInfo.byChannel = stUpdate->byChannel;
//     this->m_stFriendInfo.wMapID = stUpdate->wMapID;
//     this->m_stFriendInfo.bLogin = stUpdate->bLogin;
//     this->m_stFriendInfo.tLogOut = stUpdate->tLogOut;
//     // Note: nFriendPoint and tRemain are NOT updated per IDA
//   }
void CFriend::UpdateInfo(const ST_FRIEND_INFO* stInfo) {
    if (!stInfo) {
        return;
    }

    // IDA: byType only updated if non-zero
    if (stInfo->byType) {
        m_stFriendInfo.byType = stInfo->byType;
    }

    // IDA: Copy strName character by character
    wcscpy_s(m_stFriendInfo.strName, stInfo->strName);

    // IDA: Update scalar fields
    m_stFriendInfo.dwID = stInfo->dwID;
    m_stFriendInfo.byLevel = stInfo->byLevel;
    m_stFriendInfo.byClass = stInfo->byClass;
    m_stFriendInfo.byAwaken = stInfo->byAwaken;
    m_stFriendInfo.dwProfilePhotoID = stInfo->dwProfilePhotoID;
    m_stFriendInfo.byState = stInfo->byState;

    // IDA: Copy strMemo character by character
    wcscpy_s(m_stFriendInfo.strMemo, stInfo->strMemo);

    // IDA: Update remaining fields
    m_stFriendInfo.byChannel = stInfo->byChannel;
    m_stFriendInfo.wMapID = stInfo->wMapID;
    m_stFriendInfo.bLogin = stInfo->bLogin;
    m_stFriendInfo.tLogOut = stInfo->tLogOut;

    // Note: IDA does NOT update nFriendPoint and tRemain
}

// UpdateInfo from DB_FRIEND_INFO (overload for DB loading)
void CFriend::UpdateInfo(const DB_FRIEND_INFO* stInfo) {
    if (!stInfo) {
        return;
    }

    // Preserve byType if stInfo->byType is zero
    if (stInfo->byType) {
        m_stFriendInfo.byType = stInfo->byType;
    }

    // Convert and update
    wcscpy_s(m_stFriendInfo.strName, stInfo->strName);
    m_stFriendInfo.dwID = stInfo->dwID;
    m_stFriendInfo.byLevel = stInfo->byLevel;
    m_stFriendInfo.byClass = stInfo->byClass;
    m_stFriendInfo.byAwaken = stInfo->byAwaken;
    m_stFriendInfo.dwProfilePhotoID = stInfo->dwProfilePhotoID;
    m_stFriendInfo.byState = stInfo->byState;
    wcscpy_s(m_stFriendInfo.strMemo, stInfo->strMemo);
    m_stFriendInfo.byChannel = stInfo->byChannel;
    m_stFriendInfo.wMapID = stInfo->wMapID;
    m_stFriendInfo.bLogin = stInfo->bLogin;
    m_stFriendInfo.tLogOut = stInfo->tLogOut;

    // Note: nFriendPoint and tRemain preserved per IDA pattern
}

// UpdateInfo from ST_FRIEND_COMMUNITY
// IDA: 0x140086430 - ?UpdateInfo@CFriend@@QEAAXAEAUST_FRIEND_COMMUNITY@@@Z
// IDA decompiled:
//   void __fastcall CFriend::UpdateInfo(CFriend *this, ST_FRIEND_COMMUNITY *stUpdate)
//   {
//     wchar_t *strMemo;
//     wchar_t *v3;
//     wchar_t v4;
//
//     this->m_stFriendInfo.byState = stUpdate->byState;
//     strMemo = stUpdate->strMemo;
//     v3 = this->m_stFriendInfo.strMemo;
//     do
//     {
//       v4 = *strMemo;
//       *v3++ = *strMemo++;
//     }
//     while ( v4 );
//   }
void CFriend::UpdateInfo(const ST_FRIEND_COMMUNITY* stCommunity) {
    if (!stCommunity) {
        return;
    }

    m_stFriendInfo.byState = stCommunity->byState;

    // IDA: Copy strMemo character by character until null terminator
    wcscpy_s(m_stFriendInfo.strMemo, stCommunity->strMemo);
}

// GetInfo to ST_FRIEND_INFO - IDA: CFriend::GetInfo (primary)
// IDA: 0x140086230 - ?GetInfo@CFriend@@QEAAXAEAUST_FRIEND_INFO@@@Z
// IDA decompiled:
//   void __fastcall CFriend::GetInfo(CFriend *this, ST_FRIEND_INFO *stInfo)
//   {
//     _BYTE v2[160]; // [rsp+0h] [rbp-B8h] BYREF
//
//     qmemcpy(v2, this, sizeof(v2));
//     qmemcpy(stInfo, v2, sizeof(ST_FRIEND_INFO));
//   }
void CFriend::GetInfo(ST_FRIEND_INFO* stInfo) const {
    if (stInfo) {
        *stInfo = m_stFriendInfo;
    }
}

// GetInfo to DB_FRIEND_INFO (overload for DB saving)
void CFriend::GetInfo(DB_FRIEND_INFO* stInfo) const {
    if (stInfo) {
        // Convert ST_FRIEND_INFO to DB_FRIEND_INFO
        *stInfo = {};
        wcscpy_s(stInfo->strName, m_stFriendInfo.strName);
        stInfo->dwID = m_stFriendInfo.dwID;
        stInfo->byLevel = m_stFriendInfo.byLevel;
        stInfo->byClass = m_stFriendInfo.byClass;
        stInfo->byAwaken = m_stFriendInfo.byAwaken;
        stInfo->dwProfilePhotoID = m_stFriendInfo.dwProfilePhotoID;
        stInfo->byType = m_stFriendInfo.byType;
        stInfo->byState = m_stFriendInfo.byState;
        wcscpy_s(stInfo->strMemo, m_stFriendInfo.strMemo);
        stInfo->byChannel = m_stFriendInfo.byChannel;
        stInfo->wMapID = m_stFriendInfo.wMapID;
        stInfo->nFriendPoint = m_stFriendInfo.nFriendPoint;
        stInfo->bLogin = m_stFriendInfo.bLogin;
        stInfo->tLogOut = m_stFriendInfo.tLogOut;
        stInfo->tRemain = m_stFriendInfo.tRemain;
    }
}

// SetCommunityInfo - wrapper for UpdateInfo(ST_FRIEND_COMMUNITY*)
void CFriend::SetCommunityInfo(std::uint8_t byState, const wchar_t* szComment) {
    ST_FRIEND_COMMUNITY stCommunity = {};
    stCommunity.byState = byState;
    if (szComment) {
        wcscpy_s(stCommunity.strMemo, szComment);
    }
    UpdateInfo(&stCommunity);
}

// AddFriendPoint
void CFriend::AddFriendPoint(std::int64_t nPoint) {
    m_stFriendInfo.nFriendPoint += nPoint;
}
