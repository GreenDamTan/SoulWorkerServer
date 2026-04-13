#pragma once

#include <cstdint>
#include <string>

#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"

class XClient;

class IXComponent {
public:
    IXComponent() = default;
    virtual ~IXComponent() = default;

    void SetCmd(std::uint8_t cmd) {
        cmd_ = cmd;
    }

    std::uint8_t GetCmd() const {
        return cmd_;
    }

    void SetName(const std::string& name) {
        name_ = name;
    }

    void SetName(const char* name) {
        name_ = name ? name : "";
    }

    const std::string& GetName() const {
        return name_;
    }

private:
    std::uint8_t cmd_ = 0;
    std::string name_;
};

class IXProcess : public IXComponent {
public:
    IXProcess() = default;
    virtual ~IXProcess() = default;

    // IDA shows Register() reading IXComponent::ucCmd while
    // TXProcess<CUser>::SendErrorMessage() reads IXProcess::m_ucCmd.
    // Keep both bytes mirrored so the reconstructed path stays consistent.
    void SetCmd(std::uint8_t cmd) {
        IXComponent::SetCmd(cmd);
        m_ucCmd = cmd;
    }

    virtual bool Init(XClient* client) = 0;
    virtual bool Clear() = 0;
    virtual bool Parse(XPacket& packet) = 0;

protected:
    XClient* m_pClient = nullptr;
    std::uint8_t m_ucCmd = 0;
};

template <typename TClient>
class TXProcess : public IXProcess {
public:
    TXProcess() = default;
    ~TXProcess() override = default;

    bool Init(XClient* client) override {
        m_pClient = client;
        return true;
    }

    bool Clear() override {
        m_pClient = nullptr;
        return true;
    }

    TClient* GetClientPtr() {
        return dynamic_cast<TClient*>(m_pClient);
    }

    bool SendErrorMessage(std::uint8_t byPacketSubHeader, std::uint16_t wErrorCode) {
        if (!m_pClient) {
            return false;
        }
        return m_pClient->SendErrorMessage(m_ucCmd, byPacketSubHeader, wErrorCode);
    }
};
