#pragma once

#include "GOComponent.h"
#include <cstdint>

/**
 * @brief CGocNetwork - Game Object Component for network/connection state
 *
 * Handles connection state, latency tracking, and network-related functionality.
 */
class CGocNetwork : public GOComponent {
public:
    CGocNetwork();
    virtual ~CGocNetwork();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Connection state
    bool IsConnected() const;
    int GetLatency() const;
    int GetConnectionQuality() const;

    // Network operations
    void UpdateLatency(int nLatencyMs);
    void OnPacketReceived(int nPacketSize);
    void OnPacketSent(int nPacketSize);

    // Statistics
    int GetPacketsReceived() const;
    int GetPacketsSent() const;
    int GetBytesReceived() const;
    int GetBytesSent() const;

protected:
    int m_nLatency;
    int m_nPacketsReceived;
    int m_nPacketsSent;
    int m_nBytesReceived;
    int m_nBytesSent;
    bool m_bConnected;
};
