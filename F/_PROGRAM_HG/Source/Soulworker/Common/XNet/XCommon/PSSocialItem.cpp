#include "PSSocialItem.h"

#include <string>

XPacket& operator<<(XPacket& packet, PS_SOCIALITEM_USER& value)
{
    packet.XParse << value.dwPhotoID;
    std::wstring name;
    for (char16_t character : value.szName) {
        if (character == 0)
            break;
        name.push_back(static_cast<wchar_t>(character));
    }
    packet.XParse << name;
    packet.XParse << value.byClass;
    packet.XParse << value.byAwaken;
    packet.XParse << value.bOwner;
    packet.XParse << value.bLeave;
    return packet;
}

XPacket& operator<<(XPacket& packet, PS_SOCIALITEM_PLAY_POINT& value)
{
    packet.XParse << value.nTotalPoint;
    for (char c = 0; c < 7; ++c)
        packet.XParse << value.nCardFaction[static_cast<std::size_t>(c)];
    packet.XParse << value.bTurn;
    return packet;
}

XPacket& operator>>(XPacket& packet, PS_SOCIALITEM_PLAY_POINT& value)
{
    packet.XParse >> value.nTotalPoint;
    for (char c = 0; c < 7; ++c)
        packet.XParse >> value.nCardFaction[static_cast<std::size_t>(c)];
    packet.XParse >> value.bTurn;
    return packet;
}

XPacket& operator<<(XPacket& packet, PS_SOCIALITEM_PLAY& value)
{
    packet.XParse << value.dwSocialObjectID;
    packet.XParse << value.dwCardID;
    packet.XParse << value.byCardIdx;
    packet.XParse << value.bFinish;
    packet.XParse << value.nPlayCount;
    packet.XParse << value.nType;
    packet << value.psOwnerInfo;
    packet << value.psGuestInfo;
    return packet;
}

XPacket& operator>>(XPacket& packet, PS_SOCIALITEM_PLAY& value)
{
    packet.XParse >> value.dwSocialObjectID;
    packet.XParse >> value.dwCardID;
    packet.XParse >> value.byCardIdx;
    packet.XParse >> value.bFinish;
    packet.XParse >> value.nPlayCount;
    packet.XParse >> value.nType;
    packet >> value.psOwnerInfo;
    packet >> value.psGuestInfo;
    return packet;
}

XPacket& operator<<(XPacket& packet, ST_SOCIALITEM_CARD& value)
{
    packet.XParse << value.byCardIndex;
    packet.XParse << value.dwCardID;
    return packet;
}

XPacket& operator>>(XPacket& packet, ST_SOCIALITEM_CARD& value)
{
    char byCardIndex = 0;
    packet.XParse >> byCardIndex;
    value.byCardIndex = static_cast<std::uint8_t>(byCardIndex);
    packet.XParse >> value.dwCardID;
    return packet;
}

XPacket& operator<<(XPacket& packet, PS_SOCIAL_ITEM_PLAY_START& value)
{
    packet.XParse << value.dwSocialObjectID;
    const char cCount = static_cast<char>(value.vecCardInfo.size());
    packet.XParse << cCount;
    for (char c = 0; c < cCount; ++c)
        packet << value.vecCardInfo[static_cast<std::size_t>(c)];
    return packet;
}

XPacket& operator>>(XPacket& packet, PS_SOCIAL_ITEM_PLAY_START& value)
{
    packet.XParse >> value.dwSocialObjectID;
    char cCount = 0;
    packet.XParse >> cCount;
    for (char c = 0; c < cCount; ++c)
    {
        ST_SOCIALITEM_CARD stInfo;
        packet >> stInfo;
        value.vecCardInfo.push_back(stInfo);
    }
    return packet;
}

XPacket& operator<<(XPacket& packet, ST_SOCIAL_ITEM_SLOT_INFO& value)
{
    packet.XParse << value.dwUserID;
    packet.XParse << value.byAniIndex;
    return packet;
}

XPacket& operator<<(XPacket& packet, ST_SOCIAL_ITEM_INFO& value)
{
    packet.XParse << value.dwObjectID;
    packet.XParse << value.dwOwnerID;
    packet.XParse << value.dwItemID;
    packet.XParse << value.wSocialItemID;
    packet.XParse << value.lRemainTime;

    const std::int8_t cCount = static_cast<std::int8_t>(value.vecUsers.size());
    packet.XParse << static_cast<std::uint8_t>(cCount);
    for (std::int8_t c = 0; c < cCount; ++c)
        packet.XParse << value.vecUsers[static_cast<std::size_t>(c)];

    for (int i = 0; i < 4; ++i)
        packet << value.stUsedSlot[i];
    return packet;
}

XPacket& operator<<(XPacket& packet, ST_SOCIAL_ITEM_RES& value)
{
    packet << value.itemInfo;
    packet.XParse << value.fPosX;
    packet.XParse << value.fPosY;
    packet.XParse << value.fPosZ;
    packet.XParse << value.fRot;
    return packet;
}
