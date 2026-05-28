#include "gocpost.h"

CGocPost::CGocPost()
    : GOComponent(E_GOC_TYPE_NONE)
    , m_nUnreadCount(0)
    , m_nTotalCount(0)
{
}

CGocPost::~CGocPost() {
    // TODO: Cleanup if needed
}

bool CGocPost::Initialize() {
    return GOComponent::Initialize();
}

void CGocPost::Shutdown() {
    GOComponent::Shutdown();
}

void CGocPost::Update(float fDeltaTime) {
    GOComponent::Update(fDeltaTime);
}

int CGocPost::GetUnreadMailCount() const {
    return m_nUnreadCount;
}

int CGocPost::GetTotalMailCount() const {
    return m_nTotalCount;
}

bool CGocPost::HasUnreadMail() const {
    return m_nUnreadCount > 0;
}

void CGocPost::MarkMailRead(int nMailId) {
    // TODO: Implement mark mail as read
    (void)nMailId;
}

bool CGocPost::SendMail(int nRecipientId, const char* szSubject, const char* szBody) {
    // TODO: Implement send mail
    (void)nRecipientId;
    (void)szSubject;
    (void)szBody;
    return false;
}

bool CGocPost::DeleteMail(int nMailId) {
    // TODO: Implement delete mail
    (void)nMailId;
    return false;
}

bool CGocPost::SendMailWithItem(int nRecipientId, const char* szSubject, const char* szBody,
                                 int nItemId, int nItemCount) {
    // TODO: Implement send mail with item
    (void)nRecipientId;
    (void)szSubject;
    (void)szBody;
    (void)nItemId;
    (void)nItemCount;
    return false;
}

bool CGocPost::ClaimMailItem(int nMailId) {
    // TODO: Implement claim mail item
    (void)nMailId;
    return false;
}

bool CGocPost::HasMail() const {
    return m_nTotalCount > 0;
}

bool CGocPost::HasMailFrom(int nSenderId) const {
    // TODO: Implement mail sender check
    (void)nSenderId;
    return false;
}
