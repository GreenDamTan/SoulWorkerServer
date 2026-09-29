#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"

struct PS_SOCIALITEM_USER {
    PS_SOCIALITEM_USER()
        : dwPhotoID(0), byClass(0), byAwaken(0), bOwner(false), dwUCID(0), bLeave(false)
    {
        szName[0] = 0;
    }

    std::uint32_t dwPhotoID;
    char16_t szName[21];
    std::uint8_t byClass;
    std::uint8_t byAwaken;
    bool bOwner;
    std::uint32_t dwUCID;
    bool bLeave;
};

static_assert(sizeof(PS_SOCIALITEM_USER) == 60);
static_assert(offsetof(PS_SOCIALITEM_USER, szName) == 4);
static_assert(offsetof(PS_SOCIALITEM_USER, byClass) == 46);
static_assert(offsetof(PS_SOCIALITEM_USER, byAwaken) == 47);
static_assert(offsetof(PS_SOCIALITEM_USER, bOwner) == 48);
static_assert(offsetof(PS_SOCIALITEM_USER, dwUCID) == 52);
static_assert(offsetof(PS_SOCIALITEM_USER, bLeave) == 56);

struct PS_SOCIALITEM_PLAY_POINT {
    PS_SOCIALITEM_PLAY_POINT()
    {
        nTotalPoint = 0;
        for (char c = 0; c < 7; ++c)
            nCardFaction[static_cast<std::size_t>(c)] = 0;
        bTurn = false;
    }

    int nTotalPoint;
    std::uint8_t nCardFaction[7];
    bool bTurn;
};

static_assert(sizeof(PS_SOCIALITEM_PLAY_POINT) == 12);
static_assert(offsetof(PS_SOCIALITEM_PLAY_POINT, nCardFaction) == 4);
static_assert(offsetof(PS_SOCIALITEM_PLAY_POINT, bTurn) == 11);

struct PS_SOCIALITEM_PLAY {
    PS_SOCIALITEM_PLAY()
    {
        dwSocialObjectID = 0;
        dwCardID = 0;
        byCardIdx = 0;
        bFinish = false;
        nPlayCount = 0;
        nType = 0;
    }

    std::uint32_t dwSocialObjectID;
    std::uint32_t dwCardID;
    std::uint8_t byCardIdx;
    bool bFinish;
    int nPlayCount;
    int nType;
    PS_SOCIALITEM_PLAY_POINT psOwnerInfo;
    PS_SOCIALITEM_PLAY_POINT psGuestInfo;
};

static_assert(sizeof(PS_SOCIALITEM_PLAY) == 44);
static_assert(offsetof(PS_SOCIALITEM_PLAY, nPlayCount) == 12);
static_assert(offsetof(PS_SOCIALITEM_PLAY, psOwnerInfo) == 20);
static_assert(offsetof(PS_SOCIALITEM_PLAY, psGuestInfo) == 32);

struct ST_SOCIALITEM_CARD {
    ST_SOCIALITEM_CARD() : byCardIndex(0), dwCardID(0) {}

    std::uint8_t byCardIndex;
    std::uint32_t dwCardID;
};

static_assert(sizeof(ST_SOCIALITEM_CARD) == 8);
static_assert(offsetof(ST_SOCIALITEM_CARD, dwCardID) == 4);

struct PS_SOCIAL_ITEM_PLAY_START {
    std::uint32_t dwSocialObjectID = 0;
    std::vector<ST_SOCIALITEM_CARD> vecCardInfo;
};

#if defined(_WIN32) && defined(_ITERATOR_DEBUG_LEVEL) && _ITERATOR_DEBUG_LEVEL != 0
static_assert(sizeof(PS_SOCIAL_ITEM_PLAY_START) == 40);
static_assert(offsetof(PS_SOCIAL_ITEM_PLAY_START, vecCardInfo) == 8);
#endif

struct ST_SOCIAL_ITEM_SLOT_INFO {
    ST_SOCIAL_ITEM_SLOT_INFO() { reset(); }

    void reset()
    {
        dwUserID = 0;
        byAniIndex = 0;
    }

    std::uint32_t dwUserID;
    std::uint8_t byAniIndex;
};

static_assert(sizeof(ST_SOCIAL_ITEM_SLOT_INFO) == 8);
static_assert(offsetof(ST_SOCIAL_ITEM_SLOT_INFO, byAniIndex) == 4);

struct ST_SOCIAL_ITEM_INFO {
    ST_SOCIAL_ITEM_INFO() { reset(); }
    ~ST_SOCIAL_ITEM_INFO() = default;

    void reset()
    {
        dwObjectID = 0;
        dwOwnerID = 0;
        dwItemID = 0;
        wSocialItemID = 0;
        lRemainTime = 0;
        vecUsers.clear();
        std::memset(stUsedSlot, 0, sizeof(stUsedSlot));
    }

    std::uint32_t dwObjectID;
    std::uint32_t dwOwnerID;
    std::uint32_t dwItemID;
    std::uint16_t wSocialItemID;
    std::int64_t lRemainTime;
    std::vector<std::uint32_t> vecUsers;
    ST_SOCIAL_ITEM_SLOT_INFO stUsedSlot[4];
};

static_assert(offsetof(ST_SOCIAL_ITEM_INFO, lRemainTime) == 16);
static_assert(offsetof(ST_SOCIAL_ITEM_INFO, vecUsers) == 24);

struct ST_SOCIAL_ITEM_RES {
    ST_SOCIAL_ITEM_RES()
    {
        itemInfo.reset();
        fPosX = 0.0f;
        fPosY = 0.0f;
        fPosZ = 0.0f;
        fRot = 0.0f;
    }
    ~ST_SOCIAL_ITEM_RES() = default;

    ST_SOCIAL_ITEM_INFO itemInfo;
    float fPosX;
    float fPosY;
    float fPosZ;
    float fRot;
};

#if defined(_WIN32) && defined(_ITERATOR_DEBUG_LEVEL) && _ITERATOR_DEBUG_LEVEL != 0
static_assert(sizeof(ST_SOCIAL_ITEM_INFO) == 88);
static_assert(offsetof(ST_SOCIAL_ITEM_INFO, stUsedSlot) == 56);
static_assert(sizeof(ST_SOCIAL_ITEM_RES) == 104);
#endif

XPacket& operator<<(XPacket& packet, PS_SOCIALITEM_USER& value);
XPacket& operator<<(XPacket& packet, PS_SOCIALITEM_PLAY_POINT& value);
XPacket& operator>>(XPacket& packet, PS_SOCIALITEM_PLAY_POINT& value);
XPacket& operator<<(XPacket& packet, PS_SOCIALITEM_PLAY& value);
XPacket& operator>>(XPacket& packet, PS_SOCIALITEM_PLAY& value);
XPacket& operator<<(XPacket& packet, ST_SOCIALITEM_CARD& value);
XPacket& operator>>(XPacket& packet, ST_SOCIALITEM_CARD& value);
XPacket& operator<<(XPacket& packet, PS_SOCIAL_ITEM_PLAY_START& value);
XPacket& operator>>(XPacket& packet, PS_SOCIAL_ITEM_PLAY_START& value);
XPacket& operator<<(XPacket& packet, ST_SOCIAL_ITEM_SLOT_INFO& value);
XPacket& operator<<(XPacket& packet, ST_SOCIAL_ITEM_INFO& value);
XPacket& operator<<(XPacket& packet, ST_SOCIAL_ITEM_RES& value);
