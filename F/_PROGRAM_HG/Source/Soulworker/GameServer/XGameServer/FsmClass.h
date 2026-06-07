#pragma once

#include <cstdint>
#include <functional>
#include <vector>
#include <map>

// 前置声明
class CAi;
class CFsmTransition;
class CFsmCondition;

// FSMSTATES 枚举在 Monster.h 中定义
// E_FSMSTATES 是 FSMSTATES 的别名，在某些上下文中使用
enum FSMSTATES;
using E_FSMSTATES = FSMSTATES;

// ============================================================================
// CEvanStack - 状态栈模板类 (IDA 反编译确认)
// 用于管理 FSM 状态的历史记录
// ============================================================================
template<typename T>
class CEvanStack {
public:
    // 栈节点结构
    struct STRUCT_StackNode {
        T data;
        STRUCT_StackNode* p_NextNode;
    };

    // 构造函数 IDA 0x1402724D0
    CEvanStack()
        : m_nMaxNodeCount(10)
        , m_nNodeCount(0)
        , m_pTopNode(nullptr)
    {
    }

    // 析构函数 IDA 0x140272500
    ~CEvanStack() {
        while (!IsEmpty()) {
            Pop();
        }
    }

    // 压栈 IDA 0x140272540
    void Push(T _data) {
        STRUCT_StackNode* pNewNode = new STRUCT_StackNode();
        pNewNode->data = _data;

        if (!IsEmpty()) {
            pNewNode->p_NextNode = m_pTopNode;
        }
        m_pTopNode = pNewNode;

        if (++m_nNodeCount > m_nMaxNodeCount) {
            m_nNodeCount = m_nMaxNodeCount;

            // 找到最后一个节点
            STRUCT_StackNode* pLastNode = m_pTopNode;
            for (int nCount = 1; nCount < m_nMaxNodeCount; ++nCount) {
                pLastNode = pLastNode->p_NextNode;
            }

            // 删除超出限制的节点
            while (pLastNode->p_NextNode) {
                STRUCT_StackNode* pDeleteNode = pLastNode->p_NextNode;
                pLastNode->p_NextNode = pDeleteNode->p_NextNode;
                delete pDeleteNode;
            }
        }
    }

    // 查看栈顶 IDA 0x1402726A0
    T Peek() const {
        if (IsEmpty()) {
            return static_cast<T>(-1);
        }
        return m_pTopNode->data;
    }

    // 弹栈 IDA 0x140273150
    T Pop() {
        if (IsEmpty()) {
            return static_cast<T>(-1);
        }

        STRUCT_StackNode* pDeleteNode = m_pTopNode;
        m_pTopNode = m_pTopNode->p_NextNode;
        T data = pDeleteNode->data;
        delete pDeleteNode;
        --m_nNodeCount;
        return data;
    }

    // 是否为空 IDA 0x140273120
    bool IsEmpty() const {
        return m_nNodeCount == 0;
    }

protected:
    int m_nMaxNodeCount;      // 最大节点数
    int m_nNodeCount;         // 当前节点数
    STRUCT_StackNode* m_pTopNode;  // 栈顶节点
};

// ============================================================================
// FsmData - FSM 数据结构 (IDA 确认)
// ============================================================================
struct FsmData {
    int _eCurrentState;      // 当前状态
    int _eOutPutState;       // 输出状态
    int _eVarDataType;       // 变量数据类型
    int _eVariable;          // 变量
    int _eConditionFunc;     // 条件函数
    int* _pIntData;          // 整数数据指针
    float* _pFloatData;      // 浮点数据指针
    int _nTransitionIndex;   // 转换索引
};

// ============================================================================
// FsmDataEx - FSM 扩展数据结构 (IDA 确认)
// ============================================================================
struct FsmDataEx {
    int _nState;             // 状态
    int _nOutPutState;       // 输出状态
    int _nIndex[5];          // 索引数组
};

// ============================================================================
// CFsmState - FSM 状态类 (IDA 反编译确认)
// 模板类 CFsmState<T> 用于管理单个 AI 状态
// 继承自 std::vector<CFsmTransition*> 存储转换列表
// ============================================================================
template<typename T>
class CFsmState : public std::vector<CFsmTransition*> {
public:
    // 函数指针类型
    typedef void (T::*OnUpdateFunc)(float);
    typedef void (T::*OnEnterFunc)();
    typedef void (T::*OnExitFunc)();

    // 构造函数 IDA 0x140273DD0
    CFsmState();

    // 析构函数
    ~CFsmState();

    // 初始化 IDA 0x140273E20
    void Initialize(T* pInstance);

    // 添加转换 IDA 0x140273E40
    void AddTransition(CFsmTransition* pTransition);

    // 设置状态函数 IDA 0x140273E70
    void SetStateFunctions(OnUpdateFunc pOnUpdate, OnEnterFunc pOnEnter, OnExitFunc pOnExit);

    // 进入状态 IDA 0x140273EB0
    void OnEnter();

    // 退出状态 IDA 0x140274080
    void OnExit();

    // 更新攻击转换计时器 IDA 0x1402740C0
    void UpdateAttackTransitionTimer(float fTime);

    // 更新移动转换计时器 IDA 0x140274140
    void UpdateMoveTransitionTimer(float fTime);

    // 获取转换数量 IDA 0x140274220
    int GetTransitionSize() const;

    // 获取更新函数 IDA 0x14005AC70
    OnUpdateFunc GetUpdateFunc() const { return m_OnUpdateFunc; }

    // 获取状态
    int GetState() const { return m_eState; }

    // 设置状态
    void SetState(int nState) { m_eState = nState; }

    // 排序转换顺序 IDA 0x140274240
    void SortTransitionOrder(int* eOrder);

    // 销毁 IDA 0x140275780
    void Destroy();

    // 更新状态 IDA 0x1402754C0
    void OnUpdate(float fElapsedTime);

    // 重置转换计时器 IDA 0x140363FD0
    void ResetTransitionTimer();

    // 获取转换开始迭代器
    std::vector<CFsmTransition*>::iterator GetTransitionBegin() { return this->begin(); }

    // 获取转换结束迭代器
    std::vector<CFsmTransition*>::iterator GetTransitionEnd() { return this->end(); }

protected:
    // IDA 确认的成员变量
    int m_eState;                    // 状态值 (FSMSTATES_NONE = 0)
    T* m_pInstance;                  // 所有者实例指针
    OnEnterFunc m_OnEnterFunc;       // 进入状态回调
    OnUpdateFunc m_OnUpdateFunc;     // 更新状态回调
    OnExitFunc m_OnExitFunc;         // 退出状态回调
};

// ============================================================================
// CFsmClass - 有限状态机类模板 (基础模板)
// 模板类 CFsmClass<T> 用于管理 AI 状态机
// 注意: CAi 特化版本有不同布局，见下方特化
// ============================================================================
template<typename T>
class CFsmClass {
public:
    // 函数指针类型
    typedef int (T::*GetIntConditionFunc)(int);
    typedef float (T::*GetFloatConditionFunc)(int);
    typedef void (T::*StartStateFunc)();

    // 构造函数
    CFsmClass();
    ~CFsmClass();

    // 初始化
    void Initialize(T* pOwner, GetIntConditionFunc pGetIntFunc,
                    GetFloatConditionFunc pGetFloatFunc, StartStateFunc pStartStateFunc);

    // 获取当前状态
    int GetState() const { return m_nCurrentState; }

    // 设置状态
    void SetState(int nState) { m_nCurrentState = nState; }

    // 重置
    void Reset();

    // 设置脚本数据
    void SetScriptData(int nCurrentState, int nOutPutState, int nVarDataType,
                       int nVariable, int nConditionFunc, int* pIntData,
                       float* pFloatData, int nTransitionIndex);

    // 评估转换
    int EvaluateTransition();

    // 获取状态变量
    void* GetStateVar(int nState);

    // 排序转换顺序
    void SortTransitionOrder(int* eOrder);

protected:
    // 通用成员变量
    T* m_pOwner;
    GetIntConditionFunc m_pGetIntFunc;
    GetFloatConditionFunc m_pGetFloatFunc;
    StartStateFunc m_pStartStateFunc;
    int m_nCurrentState;
    int m_nPreviousState;
    std::map<int, void*> m_mapStateData;
    std::vector<CFsmTransition*> m_vecTransitions;
};

// ============================================================================
// CFsmTransition - FSM 转换类 (IDA 确认大小 0x40 字节)
// ============================================================================
class CFsmTransition {
public:
    CFsmTransition();
    ~CFsmTransition();

    // 获取条件向量开始迭代器
    std::vector<CFsmCondition*>::iterator GetConditionVectorBegin();

    // 获取条件向量结束迭代器
    std::vector<CFsmCondition*>::iterator GetConditionVectorEnd();

    // 添加条件 IDA 0x14025DEF0
    void AddCondition(CFsmCondition* pCondition);

    // 设置输出状态 IDA 0x14025DED0
    void SetOutputState(int nState);

    // 获取输出状态
    int GetOutputState() const { return m_nOutputState; }

    // 添加时间 IDA 0x14025DF20
    void AddTime(float fTime) { m_fTimer += fTime; }

    // 重置计时器 IDA 0x14025DF50
    void ResetTimer() { m_fTimer = 0.0f; }

    // 获取计时器 IDA 0x14025E0B0
    float GetTimer() const { return m_fTimer; }

    // 添加攻击时间 IDA 0x14025DF70
    void AddAttackTime(float fTime) { m_fAttackTimer += fTime; }

    // 重置攻击计时器 IDA 0x14025DFA0
    void ResetAttackTimer() { m_fAttackTimer = 0.0f; }

    // 获取攻击计时器 IDA 0x14025DFC0
    float GetAttackTimer() const { return m_fAttackTimer; }

    // 添加移动时间 IDA 0x14025DFD0
    void AddMoveTime(float fTime) { m_fMoveTimer += fTime; }

    // 重置移动计时器 IDA 0x14025E000
    void ResetMoveTimer() { m_fMoveTimer = 0.0f; }

    // 获取移动计时器 IDA 0x14025E020
    float GetMoveTimer() const { return m_fMoveTimer; }

protected:
    // IDA 确认的成员变量 (总大小约 0x40 字节)
    std::vector<CFsmCondition*> m_vecConditions;  // offset 0x00 - 条件列表
    int m_nOutputState;                            // offset 0x18 - 输出状态
    float m_fTimer;                                // 计时器
    float m_fAttackTimer;                          // 攻击计时器
    float m_fMoveTimer;                            // 移动计时器
};

// ============================================================================
// CFsmCondition - FSM 条件类 (IDA 确认大小约 0x60 字节)
// ============================================================================
class CFsmCondition {
public:
    CFsmCondition();
    ~CFsmCondition();

    // 复制数据
    void CopyData(CFsmCondition* pOther);

    // 评估条件
    bool Evaluate();

    // 获取条件类型
    int GetConditionType() const { return m_eVariableDataType; }

    // 设置条件类型
    void SetConditionType(int nType) { m_eVariableDataType = nType; }

    // 获取变量索引
    int GetVariableIndex() const { return m_eVariable; }

    // 设置变量索引
    void SetVariableIndex(int nIndex) { m_eVariable = nIndex; }

    // 获取比较运算符
    int GetCompareOp() const { return m_eConditionFunction; }

    // 设置比较运算符
    void SetCompareOp(int nOp) { m_eConditionFunction = nOp; }

    // 设置值(浮点) IDA 0x140275F80
    void SetValue(int nIndex, float fVal) { m_fValue[nIndex] = fVal; }

    // 设置值(整数) IDA 0x140275FB0
    void SetValue(int nIndex, int nVal) { m_nValue[nIndex] = nVal; }

    // 获取浮点值 IDA 0x140276390
    float GetValueFloat(int nIndex) const { return m_fValue[nIndex]; }

    // 获取整数值 IDA 0x1402763B0
    int GetValueInt(int nIndex) const { return m_nValue[nIndex]; }

    // 设置销毁标志 IDA 0x1402768E0
    void SetDestroy(bool bDestroy) { m_bDestroy = bDestroy; }

    // 获取销毁标志 IDA 0x140281940
    bool GetDestroy() const { return m_bDestroy; }

    // 设置条件函数 IDA 0x140276900
    void SetFunctions(int eVal) { m_eConditionFunction = eVal; }

    // 设置变量 IDA 0x140276940
    void SetVariables(int nId) { m_eVariable = nId; }

    // 条件是否满足(整数) IDA 0x1402810E0
    bool ConditionFulfilled(int nInputValue);

    // 条件是否满足(浮点) IDA 0x140281320
    bool ConditionFulfilled(float fInputValue);

    // 设置值(指针版本) IDA 0x140281070
    void SetValue(const int* pnValue, const float* pfValue) {
        m_nValue[0] = pnValue[0];
        m_nValue[1] = pnValue[1];
        m_fValue[0] = pfValue[0];
        m_fValue[1] = pfValue[1];
    }

    // 设置技能 IDA 0x1402810B0
    void SetSkill(int nGroup, int nIndex) {
        m_nSkillGroup = nGroup;
        m_nSkillIndex = nIndex;
    }

protected:
    // IDA 确认的成员变量 (从 0x140280FF0 构造函数推断)
    int m_eVariable;                // 变量类型 (FSMVAR_NONE = 0)
    int m_eVariableDataType;        // 变量数据类型 (FSMDTYPE_NONE = 0)
    int m_eConditionFunction;       // 条件函数 (CONDITION_NONE = 0)
    int m_nValue[2];                // 整数值数组 (索引0和1)
    float m_fValue[2];              // 浮点值数组 (索引0和1)
    int m_nSkillGroup;              // 技能组 (-1 = 未设置)
    int m_nSkillIndex;              // 技能索引 (-1 = 未设置)
    bool m_bDestroy;                // 销毁标志 (默认 true)
};

// ============================================================================
// 模板类实现
// ============================================================================

template<typename T>
CFsmClass<T>::CFsmClass()
    : m_pOwner(nullptr)
    , m_pGetIntFunc(nullptr)
    , m_pGetFloatFunc(nullptr)
    , m_pStartStateFunc(nullptr)
    , m_nCurrentState(0)
    , m_nPreviousState(0)
{
}

template<typename T>
CFsmClass<T>::~CFsmClass() {
    m_mapStateData.clear();
    for (auto pTransition : m_vecTransitions) {
        delete pTransition;
    }
    m_vecTransitions.clear();
}

template<typename T>
void CFsmClass<T>::Initialize(T* pOwner, GetIntConditionFunc pGetIntFunc,
                               GetFloatConditionFunc pGetFloatFunc, StartStateFunc pStartStateFunc) {
    m_pOwner = pOwner;
    m_pGetIntFunc = pGetIntFunc;
    m_pGetFloatFunc = pGetFloatFunc;
    m_pStartStateFunc = pStartStateFunc;
}

template<typename T>
void CFsmClass<T>::Reset() {
    m_nCurrentState = 0;
    m_nPreviousState = 0;
}

template<typename T>
void CFsmClass<T>::SetScriptData(int nCurrentState, int nOutPutState, int nVarDataType,
                                   int nVariable, int nConditionFunc, int* pIntData,
                                   float* pFloatData, int nTransitionIndex) {
    // TODO: 实现脚本数据设置
}

template<typename T>
int CFsmClass<T>::EvaluateTransition() {
    // TODO: 实现转换评估
    return 0;
}

template<typename T>
void* CFsmClass<T>::GetStateVar(int nState) {
    auto it = m_mapStateData.find(nState);
    if (it != m_mapStateData.end()) {
        return it->second;
    }
    return nullptr;
}

template<typename T>
void CFsmClass<T>::SortTransitionOrder(int* eOrder) {
    // IDA: 根据指定的顺序重新排列转换列表
    if (!eOrder || m_vecTransitions.empty()) {
        return;
    }

    // 创建新的转换列表
    std::vector<CFsmTransition*> vecSorted;
    vecSorted.reserve(m_vecTransitions.size());

    // 按照指定顺序添加转换
    for (int i = 0; i < 10; ++i) {
        int nStateIndex = eOrder[i];
        if (nStateIndex >= 0 && nStateIndex < static_cast<int>(m_vecTransitions.size())) {
            vecSorted.push_back(m_vecTransitions[nStateIndex]);
        }
    }

    // 更新转换列表
    if (!vecSorted.empty()) {
        m_vecTransitions = vecSorted;
    }
}

// ============================================================================
// CFsmTransition 实现
// ============================================================================

inline CFsmTransition::CFsmTransition()
    : m_nOutputState(0)
{
}

inline CFsmTransition::~CFsmTransition() {
    for (auto pCondition : m_vecConditions) {
        delete pCondition;
    }
    m_vecConditions.clear();
}

inline std::vector<CFsmCondition*>::iterator CFsmTransition::GetConditionVectorBegin() {
    return m_vecConditions.begin();
}

inline std::vector<CFsmCondition*>::iterator CFsmTransition::GetConditionVectorEnd() {
    return m_vecConditions.end();
}

inline void CFsmTransition::AddCondition(CFsmCondition* pCondition) {
    m_vecConditions.push_back(pCondition);
}

inline void CFsmTransition::SetOutputState(int nState) {
    m_nOutputState = nState;
}

// ============================================================================
// CFsmCondition 实现
// ============================================================================

inline CFsmCondition::CFsmCondition()
    : m_eVariable(0)
    , m_eVariableDataType(0)
    , m_eConditionFunction(0)
    , m_nValue{0, 0}
    , m_fValue{0.0f, 0.0f}
    , m_nSkillGroup(-1)
    , m_nSkillIndex(-1)
    , m_bDestroy(true)
{
}

inline CFsmCondition::~CFsmCondition() {
    // 无需释放资源
}

inline void CFsmCondition::CopyData(CFsmCondition* pOther) {
    if (pOther) {
        m_eVariable = pOther->m_eVariable;
        m_eVariableDataType = pOther->m_eVariableDataType;
        m_eConditionFunction = pOther->m_eConditionFunction;
        m_nValue[0] = pOther->m_nValue[0];
        m_nValue[1] = pOther->m_nValue[1];
        m_fValue[0] = pOther->m_fValue[0];
        m_fValue[1] = pOther->m_fValue[1];
        m_nSkillGroup = pOther->m_nSkillGroup;
        m_nSkillIndex = pOther->m_nSkillIndex;
        m_bDestroy = pOther->m_bDestroy;
    }
}

inline bool CFsmCondition::Evaluate() {
    // TODO: 实现条件评估
    return false;
}

// ConditionFulfilled 整数版本 IDA 0x1402810E0
inline bool CFsmCondition::ConditionFulfilled(int nInputValue) {
    switch (m_eConditionFunction) {
        case 0: // CONDITION_EQUAL
        case 6: // CONDITION_RANGE_EQUAL
            return nInputValue == m_nValue[0];
        case 1: // CONDITION_NOT_EQUAL
            return nInputValue != m_nValue[0];
        case 2: // CONDITION_GREATER_THAN
            return nInputValue > m_nValue[0];
        case 3: // CONDITION_LESS_THAN
            return nInputValue < m_nValue[0];
        case 4: // CONDITION_BIT_EQUAL
            return (m_nValue[0] & nInputValue) != 0;
        case 7: // CONDITION_RANGE_TRUE
            return m_nValue[0] <= nInputValue && nInputValue < m_nValue[1];
        case 8: // CONDITION_RANGE_FALSE
            return m_nValue[0] > nInputValue || nInputValue >= m_nValue[1];
        default:
            return false;
    }
}

// ConditionFulfilled 浮点版本 IDA 0x140281320
inline bool CFsmCondition::ConditionFulfilled(float fInputValue) {
    switch (m_eConditionFunction) {
        case 0: // CONDITION_EQUAL
        case 6: // CONDITION_RANGE_EQUAL
            return fInputValue == m_fValue[0];
        case 1: // CONDITION_NOT_EQUAL
            return fInputValue != m_fValue[0];
        case 2: // CONDITION_GREATER_THAN
            return fInputValue > m_fValue[0];
        case 3: // CONDITION_LESS_THAN
            return m_fValue[0] > fInputValue;
        case 7: // CONDITION_RANGE_TRUE
            return fInputValue >= m_fValue[0] && m_fValue[1] > fInputValue;
        case 8: // CONDITION_RANGE_FALSE
            return m_fValue[0] > fInputValue || fInputValue >= m_fValue[1];
        default:
            return false;
    }
}

// ============================================================================
// CFsmState 模板类实现 IDA 0x140273DD0
// ============================================================================

template<typename T>
CFsmState<T>::CFsmState()
    : m_eState(0)
    , m_pInstance(nullptr)
    , m_OnEnterFunc(nullptr)
    , m_OnUpdateFunc(nullptr)
    , m_OnExitFunc(nullptr)
{
    // IDA: 继承自 std::vector<CFsmTransition*>, 默认初始化
}

template<typename T>
CFsmState<T>::~CFsmState() {
    // 清理转换对象
    for (auto pTransition : *this) {
        delete pTransition;
    }
    this->clear();
}

template<typename T>
void CFsmState<T>::Initialize(T* pInstance) {
    // IDA 0x140273E20: 设置实例指针
    m_pInstance = pInstance;
}

template<typename T>
void CFsmState<T>::AddTransition(CFsmTransition* pTransition) {
    // IDA 0x140273E40: 添加转换到列表
    this->push_back(pTransition);
}

template<typename T>
void CFsmState<T>::SetStateFunctions(OnUpdateFunc pOnUpdate, OnEnterFunc pOnEnter, OnExitFunc pOnExit) {
    // IDA 0x140273E70: 设置状态回调函数
    m_OnUpdateFunc = pOnUpdate;
    m_OnEnterFunc = pOnEnter;
    m_OnExitFunc = pOnExit;
}

template<typename T>
void CFsmState<T>::OnEnter() {
    // IDA 0x140273EB0: 进入状态处理
    // 遍历所有转换, 重置条件中的随机值
    for (auto pTransition : *this) {
        bool bResetTimer = false;

        // 遍历条件列表
        auto iterCondi = pTransition->GetConditionVectorBegin();
        auto iterEnd = pTransition->GetConditionVectorEnd();

        while (iterCondi != iterEnd) {
            CFsmCondition* pCondition = *iterCondi;

            // TODO: 检查条件类型并重置随机值
            // 条件类型 3: 整数随机值
            // 条件类型 4: 浮点随机值
            // 条件类型 7: 重置计时器

            ++iterCondi;
        }

        if (bResetTimer) {
            // TODO: CFsmTransition::ResetTimer(pTransition);
        }
    }

    // 调用进入状态回调
    if (m_pInstance && m_OnEnterFunc) {
        (m_pInstance->*m_OnEnterFunc)();
    }
}

template<typename T>
void CFsmState<T>::OnExit() {
    // IDA 0x140274080: 退出状态处理
    if (m_pInstance && m_OnExitFunc) {
        (m_pInstance->*m_OnExitFunc)();
    }
}

template<typename T>
void CFsmState<T>::UpdateAttackTransitionTimer(float fTime) {
    // IDA 0x1402740C0: 更新攻击转换计时器
    for (auto pTransition : *this) {
        // TODO: CFsmTransition::AddAttackTime(pTransition, fTime);
    }
}

template<typename T>
void CFsmState<T>::UpdateMoveTransitionTimer(float fTime) {
    // IDA 0x140274140: 更新移动转换计时器
    for (auto pTransition : *this) {
        // TODO: CFsmTransition::AddMoveTime(pTransition, fTime);
    }
}

template<typename T>
int CFsmState<T>::GetTransitionSize() const {
    // IDA 0x140274220: 返回转换数量
    return static_cast<int>(this->size());
}

template<typename T>
void CFsmState<T>::SortTransitionOrder(int* eOrder) {
    // IDA 0x140274240: 按指定顺序排序转换列表
    if (!eOrder) {
        return;
    }

    // 从后向前遍历排序数组
    for (int i = 9; i >= 0; --i) {
        int nState = eOrder[i];
        if (nState == 0) {
            continue;
        }

        // 查找匹配的转换
        for (auto iterTr = this->begin(); iterTr != this->end(); ++iterTr) {
            CFsmTransition* pTransition = *iterTr;
            // TODO: 检查转换的输出状态是否匹配
            // if (pTransition->GetOutputState() == nState) {
            //     移动到列表开头
            // }
        }
    }
}

template<typename T>
void CFsmState<T>::Destroy() {
    // IDA 0x140275780: 销毁所有转换
    while (!this->empty()) {
        CFsmTransition* pTransition = this->front();
        if (pTransition) {
            delete pTransition;
        }
        this->erase(this->begin());
    }
}

template<typename T>
void CFsmState<T>::OnUpdate(float fElapsedTime) {
    // IDA 0x1402754C0: 更新状态回调
    if (m_pInstance && m_OnUpdateFunc) {
        (m_pInstance->*m_OnUpdateFunc)(fElapsedTime);
    }
}

template<typename T>
void CFsmState<T>::ResetTransitionTimer() {
    // IDA 0x140363FD0: 重置所有转换计时器
    for (auto pTransition : *this) {
        // TODO: CFsmTransition::ResetTimer(pTransition);
        // TODO: CFsmTransition::ResetAttackTimer(pTransition);
        // TODO: CFsmTransition::ResetMoveTimer(pTransition);
    }
}

// ============================================================================
// CFsmClass<CAi> 特化版本 - CAi 专用的 FSM 状态机
// IDA 反编译确认成员布局
// ============================================================================
template<>
class CFsmClass<CAi> {
public:
    // 函数指针类型
    typedef int (CAi::*GetIntConditionFunc)(int, int);
    typedef float (CAi::*GetFloatConditionFunc)(int, int);
    typedef void (CAi::*StartStateFunc)();

    // 构造函数 IDA 0x14026FF20
    CFsmClass()
        : m_pInstance(nullptr)
        , m_pGetConditionIntFunc(nullptr)
        , m_pGetConditionFloatFunc(nullptr)
        , m_pStartStateFunc(nullptr)
        , m_eCurrentStateName(static_cast<E_FSMSTATES>(0))
    {
        // m_mapState 默认构造
        // m_stackState 构造, 设置最大节点数为 10
    }

    // 析构函数 IDA 0x14026FF90
    ~CFsmClass() {
        Destroy();
    }

    // 初始化 IDA 0x14026FFE0
    void Initialize(CAi* pInstance, GetIntConditionFunc pGetIntFunc,
                    GetFloatConditionFunc pGetFloatFunc, StartStateFunc pStartStateFunc) {
        m_pInstance = pInstance;
        m_pGetConditionIntFunc = pGetIntFunc;
        m_pGetConditionFloatFunc = pGetFloatFunc;
        m_pStartStateFunc = pStartStateFunc;
    }

    // 设置状态 IDA 0x140270030
    void SetState(E_FSMSTATES _eStateName);

    // 设置脚本数据 IDA 0x1402700E0
    void SetScriptData(E_FSMSTATES _eCurrentState, E_FSMSTATES _eOutPutState,
                       int _eVarDataType, int _eVariable, int _eConditionFunc,
                       int* _pIntData, float* _pFloatData, int _nTransitionIndex);

    // 设置脚本数据扩展 IDA 0x140270440
    void SetScriptDataEx(E_FSMSTATES _eCurrentState, E_FSMSTATES _eOutPutState,
                         CFsmCondition* pCon1, CFsmCondition* pCon2,
                         CFsmCondition* pCon3, CFsmCondition* pCon4, CFsmCondition* pCon5);

    // 添加状态函数 IDA 0x140270690
    void AddStateFunctions(E_FSMSTATES _eStateName,
                           void (CAi::*_pOnUpdate)(float),
                           void (CAi::*_pOnEnter)(),
                           void (CAi::*_pOnExit)());

    // 排序转换顺序 IDA 0x140270800
    void SortTransitionOrder(E_FSMSTATES* eState);

    // 更新攻击计时器 IDA 0x140270870
    void OnUpdateAttackTimer(float _fElapsedTime);

    // 更新移动计时器 IDA 0x1402708D0
    void OnUpdateMoveTimer(float _fElapsedTime);

    // 销毁 IDA 0x140273B20
    void Destroy();

    // 是否拥有状态 IDA 0x140273BF0
    bool IsHaveState(E_FSMSTATES _eStateName) const;

    // 更新当前状态 IDA 0x140273C60
    void OnUpdateCurrentState(float _fElapsedTime);

    // 返回前一状态 IDA 0x140273CE0
    void GotoBeforeState(E_FSMSTATES _ePreState);

    // 评估当前状态转换 IDA 0x140270930
    E_FSMSTATES EvaluateCurrentStateTransition(float _fElapsedTime);

    // 获取当前状态
    E_FSMSTATES GetState() const { return m_eCurrentStateName; }

protected:
    // IDA 确认的成员变量布局
    std::map<E_FSMSTATES, CFsmState<CAi>*> m_mapState;  // 状态映射
    CEvanStack<E_FSMSTATES> m_stackState;               // 状态栈
    CAi* m_pInstance;                                    // CAi 实例指针
    GetIntConditionFunc m_pGetConditionIntFunc;          // 获取整数条件函数
    GetFloatConditionFunc m_pGetConditionFloatFunc;      // 获取浮点条件函数
    StartStateFunc m_pStartStateFunc;                    // 启动状态函数
    E_FSMSTATES m_eCurrentStateName;                     // 当前状态名
};

// CFsmClass<CAi> 成员函数实现

inline void CFsmClass<CAi>::SetState(E_FSMSTATES _eStateName) {
    // IDA 0x140270030: 设置新状态
    if (IsHaveState(_eStateName)) {
        // 获取当前状态并调用 OnExit
        auto it = m_mapState.find(m_eCurrentStateName);
        if (it != m_mapState.end() && it->second) {
            it->second->OnExit();
        }

        // 更新当前状态
        m_eCurrentStateName = _eStateName;

        // 调用启动状态函数
        if (m_pStartStateFunc && m_pInstance) {
            (m_pInstance->*m_pStartStateFunc)();
        }

        // 获取新状态并调用 OnEnter
        auto itNew = m_mapState.find(_eStateName);
        if (itNew != m_mapState.end() && itNew->second) {
            itNew->second->OnEnter();
        }

        // 将新状态压入栈
        m_stackState.Push(_eStateName);
    }
}

inline void CFsmClass<CAi>::Destroy() {
    // IDA 0x140273B20: 销毁所有状态
    for (auto& pair : m_mapState) {
        CFsmState<CAi>* pFsmState = pair.second;
        if (pFsmState) {
            delete pFsmState;
        }
    }
    m_mapState.clear();
}

inline bool CFsmClass<CAi>::IsHaveState(E_FSMSTATES _eStateName) const {
    // IDA 0x140273BF0: 检查状态是否存在
    return m_mapState.find(_eStateName) != m_mapState.end();
}

inline void CFsmClass<CAi>::OnUpdateCurrentState(float _fElapsedTime) {
    // IDA 0x140273C60: 更新当前状态
    E_FSMSTATES eStateName = m_stackState.Peek();
    if (eStateName == static_cast<E_FSMSTATES>(-1)) {
        eStateName = static_cast<E_FSMSTATES>(0);
    }

    if (eStateName != static_cast<E_FSMSTATES>(0) && IsHaveState(eStateName)) {
        auto it = m_mapState.find(eStateName);
        if (it != m_mapState.end() && it->second) {
            it->second->OnUpdate(_fElapsedTime);
        }
    }
}

inline void CFsmClass<CAi>::GotoBeforeState(E_FSMSTATES _ePreState) {
    // IDA 0x140273CE0: 返回前一状态
    // FSMSTATES_WAIT = 8 在 Monster.h 中定义
    const E_FSMSTATES FSMSTATES_WAIT_LOCAL = static_cast<E_FSMSTATES>(8);

    m_stackState.Pop();

    E_FSMSTATES state = m_stackState.Peek();
    if (state == static_cast<E_FSMSTATES>(-1)) {
        state = FSMSTATES_WAIT_LOCAL;
        m_stackState.Push(FSMSTATES_WAIT_LOCAL);
    }

    m_eCurrentStateName = state;

    // 对前一状态调用 OnExit
    if (IsHaveState(_ePreState)) {
        auto it = m_mapState.find(_ePreState);
        if (it != m_mapState.end() && it->second) {
            it->second->OnExit();
        }
    }

    // 调用启动状态函数
    if (m_pStartStateFunc && m_pInstance) {
        (m_pInstance->*m_pStartStateFunc)();
    }

    // 对新状态调用 OnEnter
    if (IsHaveState(state)) {
        auto it = m_mapState.find(state);
        if (it != m_mapState.end() && it->second) {
            it->second->OnEnter();
        }
    }
}

inline E_FSMSTATES CFsmClass<CAi>::EvaluateCurrentStateTransition(float _fElapsedTime) {
    // IDA 0x140270930: 评估当前状态转换
    E_FSMSTATES prevState = m_eCurrentStateName;

    // 查找当前状态
    auto stateIter = m_mapState.find(m_eCurrentStateName);
    if (stateIter == m_mapState.end() || !stateIter->second) {
        return static_cast<E_FSMSTATES>(0);
    }

    CFsmState<CAi>* pFsmState = stateIter->second;

    // 检查是否有转换或更新函数
    if (pFsmState->GetTransitionSize() == 0 && !pFsmState->GetUpdateFunc()) {
        GotoBeforeState(prevState);
        return m_eCurrentStateName;
    }

    // 遍历所有转换
    auto transitionIter = pFsmState->GetTransitionBegin();
    auto transitionEnd = pFsmState->GetTransitionEnd();

    while (transitionIter != transitionEnd) {
        CFsmTransition* pTransition = *transitionIter;
        pTransition->AddTime(_fElapsedTime);

        // TODO: 完整的条件评估逻辑
        // 这里需要遍历条件并评估

        ++transitionIter;
    }

    // 如果状态没有改变，更新当前状态
    if (prevState == m_eCurrentStateName) {
        OnUpdateCurrentState(_fElapsedTime);
    }

    return m_eCurrentStateName;
}

// SetScriptData IDA 0x1402700E0
inline void CFsmClass<CAi>::SetScriptData(E_FSMSTATES _eCurrentState, E_FSMSTATES _eOutPutState,
                                           int _eVarDataType, int _eVariable, int _eConditionFunc,
                                           int* _pIntData, float* _pFloatData, int _nTransitionIndex) {
    CFsmCondition* pCondition = nullptr;

    // 创建条件对象
    if (_eConditionFunc) {
        pCondition = new CFsmCondition();
        pCondition->SetVariables(_eVariable);
        pCondition->SetConditionType(_eVarDataType);
        pCondition->SetValue(_pIntData, _pFloatData);
        pCondition->SetFunctions(_eConditionFunc);
    }

    // 查找或创建状态
    CFsmState<CAi>* pNewState = nullptr;
    auto stateIter = m_mapState.find(_eCurrentState);

    if (stateIter != m_mapState.end()) {
        pNewState = stateIter->second;
    } else {
        pNewState = new CFsmState<CAi>();
        pNewState->Initialize(m_pInstance);
        pNewState->SetState(_eCurrentState);
        m_mapState[_eCurrentState] = pNewState;
        // 注册状态函数
        if (m_pInstance) {
            // TODO: CAi::RegisterStateFunctions(m_pInstance, _eCurrentState);
        }
    }

    // 查找是否已有相同输出状态的转换
    int nSkipTransition = 0;
    bool bExistTransition = false;

    for (auto& pTransition : *pNewState) {
        if (pTransition->GetOutputState() == _eOutPutState) {
            if (nSkipTransition == _nTransitionIndex) {
                if (pCondition) {
                    pTransition->AddCondition(pCondition);
                }
                bExistTransition = true;
                break;
            }
            ++nSkipTransition;
        }
    }

    // 如果不存在，创建新转换
    if (!bExistTransition) {
        CFsmTransition* pTransition = new CFsmTransition();
        if (pCondition) {
            pTransition->AddCondition(pCondition);
        }
        pTransition->SetOutputState(_eOutPutState);
        pNewState->AddTransition(pTransition);
    }
}

// SetScriptDataEx IDA 0x140270440
inline void CFsmClass<CAi>::SetScriptDataEx(E_FSMSTATES _eCurrentState, E_FSMSTATES _eOutPutState,
                                             CFsmCondition* pCon1, CFsmCondition* pCon2,
                                             CFsmCondition* pCon3, CFsmCondition* pCon4, CFsmCondition* pCon5) {
    // 查找或创建状态
    CFsmState<CAi>* pNewState = nullptr;
    auto stateIter = m_mapState.find(_eCurrentState);

    if (stateIter != m_mapState.end()) {
        pNewState = stateIter->second;
    } else {
        pNewState = new CFsmState<CAi>();
        pNewState->Initialize(m_pInstance);
        pNewState->SetState(_eCurrentState);
        m_mapState[_eCurrentState] = pNewState;
        if (m_pInstance) {
            // TODO: CAi::RegisterStateFunctions(m_pInstance, _eCurrentState);
        }
    }

    // 创建新转换
    CFsmTransition* pTransition = new CFsmTransition();
    if (pCon1) pTransition->AddCondition(pCon1);
    if (pCon2) pTransition->AddCondition(pCon2);
    if (pCon3) pTransition->AddCondition(pCon3);
    if (pCon4) pTransition->AddCondition(pCon4);
    if (pCon5) pTransition->AddCondition(pCon5);
    pTransition->SetOutputState(_eOutPutState);
    pNewState->AddTransition(pTransition);
}

// AddStateFunctions IDA 0x140270690
inline void CFsmClass<CAi>::AddStateFunctions(E_FSMSTATES _eStateName,
                                               void (CAi::*_pOnUpdate)(float),
                                               void (CAi::*_pOnEnter)(),
                                               void (CAi::*_pOnExit)()) {
    auto iter = m_mapState.find(_eStateName);
    if (iter != m_mapState.end()) {
        iter->second->SetStateFunctions(_pOnUpdate, _pOnEnter, _pOnExit);
    } else {
        CFsmState<CAi>* pNewState = new CFsmState<CAi>();
        pNewState->Initialize(m_pInstance);
        pNewState->SetState(_eStateName);
        pNewState->SetStateFunctions(_pOnUpdate, _pOnEnter, _pOnExit);
        m_mapState[_eStateName] = pNewState;
    }
}

// SortTransitionOrder IDA 0x140270800
inline void CFsmClass<CAi>::SortTransitionOrder(E_FSMSTATES* eState) {
    E_FSMSTATES eStateName = m_stackState.Peek();
    if (eStateName != static_cast<E_FSMSTATES>(0) && IsHaveState(eStateName)) {
        auto it = m_mapState.find(eStateName);
        if (it != m_mapState.end() && it->second) {
            it->second->SortTransitionOrder(reinterpret_cast<int*>(eState));
        }
    }
}

// OnUpdateAttackTimer IDA 0x140270870
inline void CFsmClass<CAi>::OnUpdateAttackTimer(float _fElapsedTime) {
    if (m_eCurrentStateName != static_cast<E_FSMSTATES>(0) && IsHaveState(m_eCurrentStateName)) {
        auto it = m_mapState.find(m_eCurrentStateName);
        if (it != m_mapState.end() && it->second) {
            it->second->UpdateAttackTransitionTimer(_fElapsedTime);
        }
    }
}

// OnUpdateMoveTimer IDA 0x1402708D0
inline void CFsmClass<CAi>::OnUpdateMoveTimer(float _fElapsedTime) {
    if (m_eCurrentStateName != static_cast<E_FSMSTATES>(0) && IsHaveState(m_eCurrentStateName)) {
        auto it = m_mapState.find(m_eCurrentStateName);
        if (it != m_mapState.end() && it->second) {
            it->second->UpdateMoveTransitionTimer(_fElapsedTime);
        }
    }
}
