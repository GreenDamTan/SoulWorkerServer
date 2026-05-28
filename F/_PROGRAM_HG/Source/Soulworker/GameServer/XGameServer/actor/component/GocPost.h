#pragma once

#include "GOComponent.h"
#include <cstdint>

/**
 * @brief CGocPost - Game Object Component for mail/post system
 *
 * Handles in-game mail, messages, and post-related functionality.
 */
class CGocPost : public GOComponent {
public:
    CGocPost();
    virtual ~CGocPost();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Mail operations
    int GetUnreadMailCount() const;
    int GetTotalMailCount() const;

    bool HasUnreadMail() const;
    void MarkMailRead(int nMailId);

    bool SendMail(int nRecipientId, const char* szSubject, const char* szBody);
    bool DeleteMail(int nMailId);

    // Mail with items
    bool SendMailWithItem(int nRecipientId, const char* szSubject, const char* szBody,
                          int nItemId, int nItemCount);
    bool ClaimMailItem(int nMailId);

    // Mail queries
    bool HasMail() const;
    bool HasMailFrom(int nSenderId) const;

protected:
    int m_nUnreadCount;
    int m_nTotalCount;
};
