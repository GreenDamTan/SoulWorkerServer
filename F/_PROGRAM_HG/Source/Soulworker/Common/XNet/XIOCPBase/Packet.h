#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <vector>

#include "Soulworker/Common/XNet/XIOCPBase/Parse.h"
#include "Soulworker/GameServer/XCore/XServer/IXObject.h"

#pragma pack(push, 1)
struct PACKET_ROOT {
    std::uint8_t ucMainCmd = 0;
    std::uint8_t ucSubCmd = 0;
};

static_assert(sizeof(PACKET_ROOT) == 0x2, "PACKET_ROOT layout mismatch");

struct PACKET_HEADER {
    std::uint16_t usVer = 0;
    std::uint16_t usSize = 0;
    std::uint8_t usTos = 0;
};
#pragma pack(pop)

static_assert(sizeof(PACKET_HEADER) == 0x5, "PACKET_HEADER layout mismatch");

inline constexpr std::array<std::array<std::uint8_t, 4>, 16> SY_KEY_TABLE = {{
    {{0x57, 0x19, 0xC6, 0x2D}},
    {{0x56, 0x68, 0x3A, 0xCC}},
    {{0x60, 0x3B, 0x0B, 0xB1}},
    {{0x90, 0x5C, 0x4A, 0xF8}},
    {{0x80, 0x28, 0xB1, 0x45}},
    {{0xB6, 0x85, 0xE7, 0x4C}},
    {{0x06, 0x2D, 0x55, 0x83}},
    {{0xAF, 0x44, 0x99, 0x95}},
    {{0xD9, 0x98, 0xBF, 0xAE}},
    {{0x53, 0x43, 0x63, 0xC8}},
    {{0x4A, 0x71, 0x80, 0x9D}},
    {{0x0B, 0xA1, 0x70, 0x8A}},
    {{0x0F, 0x54, 0x9C, 0x1B}},
    {{0x06, 0xC0, 0xEA, 0x3C}},
    {{0xC0, 0x88, 0x71, 0x48}},
    {{0xB3, 0xB9, 0x45, 0x78}},
}};

class XPacket {
public:
    XPacket() {
        ResetPacketView();
        BindOwnedPayload(sizeof(PACKET_ROOT));
        UpdatePacketViewPointers();
    }

    XPacket(std::uint8_t mainCmd, std::uint8_t subCmd) {
        ResetPacketView();
        BindOwnedPayload(sizeof(PACKET_ROOT));
        UpdatePacketViewPointers();
        SetCommand(mainCmd, subCmd);
        XParse.SetUsIndex(static_cast<short>(sizeof(PACKET_ROOT)));
    }

    virtual ~XPacket() = default;

    std::uint8_t GetSubCmd() const {
        return m_pRoot ? m_pRoot->ucSubCmd : 0;
    }

    std::uint8_t GetMainCmd() const {
        return m_pRoot ? m_pRoot->ucMainCmd : 0;
    }

    std::uint16_t GetPayloadSize() const {
        if (XParse.m_usIndex != 0) {
            return XParse.m_usIndex;
        }
        if (usSize >= sizeof(PACKET_HEADER)) {
            return static_cast<std::uint16_t>(usSize - sizeof(PACKET_HEADER));
        }
        return 0;
    }

    const char* GetPayloadBuffer() const {
        return XParse.m_pBuffer;
    }

    char* GetPayloadBuffer() {
        return XParse.m_pBuffer;
    }

    bool GreenDamTan_AssignNetworkPacket(const char* packetBuffer, std::uint16_t packetSize) {
        if (!packetBuffer || packetSize < sizeof(PACKET_HEADER)) {
            return false;
        }

        PACKET_HEADER header{};
        std::memcpy(&header, packetBuffer, sizeof(header));

        usVer = header.usVer;
        usSize = header.usSize;
        usTos = header.usTos;

        const std::uint16_t payloadSize =
            packetSize >= sizeof(PACKET_HEADER)
                ? static_cast<std::uint16_t>(packetSize - sizeof(PACKET_HEADER))
                : 0;

        BindOwnedPayload(payloadSize > 0 ? payloadSize : sizeof(PACKET_ROOT));
        if (payloadSize > 0) {
            std::memcpy(m_GreenDamTanOwnedPayload.data(),
                        packetBuffer + sizeof(PACKET_HEADER),
                        payloadSize);
        }
        if (payloadSize >= sizeof(PACKET_ROOT)) {
            XParse.SetUsIndex(static_cast<short>(sizeof(PACKET_ROOT)));
        } else {
            XParse.SetUsIndex(0);
        }
        UpdatePacketViewPointers();
        return true;
    }

    bool GreenDamTan_Encrypt(char* outBuffer, std::uint16_t& usOutSize) const {
        if (!outBuffer || !XParse.m_pBuffer) {
            usOutSize = 0;
            return false;
        }

        const std::uint16_t payloadSize = GetPayloadSize();
        PACKET_HEADER header{};
        header.usVer = usVer;
        header.usSize = static_cast<std::uint16_t>(payloadSize + sizeof(PACKET_HEADER));
        header.usTos = usTos;

        std::memcpy(outBuffer, &header, sizeof(header));
        if (payloadSize > 0) {
            std::memcpy(outBuffer + sizeof(PACKET_HEADER), XParse.m_pBuffer, payloadSize);
        }

        const std::uint8_t keyRow = static_cast<std::uint8_t>(header.usVer & 0xFFu);
        if (keyRow < SY_KEY_TABLE.size()) {
            for (std::uint16_t i = 0; i < payloadSize; ++i) {
                outBuffer[sizeof(PACKET_HEADER) + i] ^=
                    static_cast<char>(SY_KEY_TABLE[keyRow][i % 3]);
            }
        } else {
            // TODO: 需人工审查：当前仅已证实 `SY_KEY_TABLE` 为 16x4，超出版本号尚无命中证据。
        }

        usOutSize = header.usSize;
        return true;
    }

    XParse XParse;
    std::uint16_t usVer = 0;
    std::uint16_t usSize = 0;
    std::uint8_t usTos = 0;
    PACKET_ROOT* m_pRoot = nullptr;

protected:
    void ResetPacketView() {
        usVer = 0;
        usSize = 0;
        usTos = 0;
        m_pRoot = nullptr;
    }

    void BindExternalPayload(char* buffer, std::size_t maxLen) {
        XParse.BindBuffer(buffer, maxLen);
        UpdatePacketViewPointers();
    }

    void BindOwnedPayload(std::size_t payloadSize) {
        const std::size_t safeSize = (std::max<std::size_t>)(payloadSize, sizeof(PACKET_ROOT));
        m_GreenDamTanOwnedPayload.assign(safeSize, 0);
        XParse.BindBuffer(reinterpret_cast<char*>(m_GreenDamTanOwnedPayload.data()), safeSize);
        UpdatePacketViewPointers();
    }

    void UpdatePacketViewPointers() {
        m_pRoot = XParse.m_pBuffer ? reinterpret_cast<PACKET_ROOT*>(XParse.m_pBuffer) : nullptr;
    }

    void SetCommand(std::uint8_t mainCmd, std::uint8_t subCmd) {
        UpdatePacketViewPointers();
        if (!m_pRoot) {
            return;
        }

        m_pRoot->ucMainCmd = mainCmd;
        m_pRoot->ucSubCmd = subCmd;
    }

    void SetPacketHeader(std::uint16_t version, std::uint8_t tos) {
        usVer = version;
        usTos = tos;
    }

private:
    std::vector<std::uint8_t> m_GreenDamTanOwnedPayload;
};

class XSendPacket : public XPacket {
public:
    XSendPacket() {
        ResetSendPacket();
    }

    XSendPacket(std::uint8_t mainCmd, std::uint8_t subCmd) {
        ResetSendPacket();
        SetCommand(mainCmd, subCmd);
        XParse.SetUsIndex(static_cast<short>(sizeof(PACKET_ROOT)));
    }

    bool Encrypt(char* pBuffer, std::uint16_t& usOutSize) const {
        return GreenDamTan_Encrypt(pBuffer, usOutSize);
    }

protected:
    void ResetSendPacket() {
        ResetPacketView();
        std::memset(m_Data.data(), 0, m_Data.size());
        BindExternalPayload(reinterpret_cast<char*>(m_Data.data()), m_Data.size());
        SetPacketHeader(2, 2);
        XParse.m_eError = 0;
        XParse.m_usIndex = 0;
        XParse.m_usMaxLen = 0xFFFFu;
        UpdatePacketViewPointers();
    }

private:
    std::array<std::uint8_t, 0x10000> m_Data{};
};

class XSendDBPacket : public XSendPacket {
public:
    XSendDBPacket() = default;

    XSendDBPacket(IXObject* objectOwner, std::uint8_t mainCmd, std::uint8_t subCmd)
        : XSendPacket(mainCmd, subCmd) {
        const int orderID = objectOwner ? objectOwner->GetSessionID() : 0;
        SetOrderID(orderID);
    }

    XSendDBPacket(IXObject* objectOwner,
                  std::uint8_t mainCmd,
                  std::uint8_t subCmd,
                  XPacket& sourcePacket)
        : XSendPacket(mainCmd, subCmd) {
        sourcePacket.XParse.SetUsIndex(static_cast<short>(sizeof(PACKET_ROOT)));
        const int orderID = objectOwner ? objectOwner->GetSessionID() : 0;
        SetOrderID(orderID);

        // IDA 反编译已明确显示：
        // 1. sourcePacket.XParse.SetUsIndex(2)
        // 2. GetBytes(..., sourcePacket.usSize - 2)
        // 3. this->m_usIndex += sourcePacket.usSize - 7
        //
        // 结合 `XPacket` 的已证实布局：
        // - `sourcePacket.usSize` 是带 5 字节 `PACKET_HEADER` 的总包长
        // - `XParse.m_pBuffer` 指向 payload 起始（即 `PACKET_ROOT`）
        //
        // 因此真正提交到目标 DB 包里的 source body 长度应为：
        // `usSize - sizeof(PACKET_HEADER) - sizeof(PACKET_ROOT)`，也就是 `usSize - 7`。
        // 原版里 `GetBytes(..., usSize - 2)` 的反编译读长仍被保留到台账中；当前跨平台工程
        // 按已经证实的尾索引公式提交 `usSize - 7` 字节，避免对已恢复的 payload 视图越界读取。
        const std::uint16_t sourcePacketTotalSize = sourcePacket.usSize;
        if (sourcePacketTotalSize > sizeof(PACKET_HEADER) + sizeof(PACKET_ROOT)) {
            const std::uint16_t sourceBodySize = static_cast<std::uint16_t>(
                sourcePacketTotalSize - sizeof(PACKET_HEADER) - sizeof(PACKET_ROOT));
            sourcePacket.XParse.GetBytes(XParse.Data() + XParse.m_usIndex, sourceBodySize);
            XParse.m_usIndex = static_cast<std::uint16_t>(XParse.m_usIndex + sourceBodySize);
        }
    }

    XSendDBPacket(int xSessionID, std::uint8_t mainCmd, std::uint8_t subCmd)
        : XSendPacket(mainCmd, subCmd) {
        SetOrderID(xSessionID);
    }

    int GetOrderID() const {
        return m_xOrderID;
    }

private:
    void SetOrderID(int xOrderID) {
        m_xOrderID = xOrderID;
        XParse << static_cast<unsigned int>(xOrderID);
    }

    int m_xOrderID = 0;
};
