// CForce.cpp
// CForce Force/军团类实现
// 对齐 IDA ControlServer.exe

#include "CForce.h"

CForce::CForce()
    : m_nForceID(0)
    , m_uxMapID{}
{
}

CForce::CForce(int nForceID)
    : m_nForceID(nForceID)
    , m_uxMapID{}
{
}

CForce::~CForce()
{
    m_mapMemberInfo.clear();
}

// 对齐 IDA 0x1400398D0: SetMember 设置成员位置
// 如果成员已存在则更新位置，否则添加新成员
void CForce::SetMember(int nActorID, const UXMapID& uxMapID)
{
    auto it = m_mapMemberInfo.find(nActorID);
    if (it != m_mapMemberInfo.end()) {
        // 成员已存在，更新位置
        it->second = uxMapID;
    } else {
        // 成员不存在，添加新成员
        m_mapMemberInfo[nActorID] = uxMapID;
    }
}