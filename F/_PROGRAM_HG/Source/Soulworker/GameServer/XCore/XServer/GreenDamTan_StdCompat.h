#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

namespace std {
namespace tr1 {

using std::allocate_shared;
using std::bad_function_call;
using std::const_pointer_cast;
using std::dynamic_pointer_cast;
using std::enable_shared_from_this;
using std::make_shared;
using std::shared_ptr;
using std::static_pointer_cast;
using std::weak_ptr;

template <typename Signature>
class function;

template <typename Return, typename... Arguments>
class function<Return(Arguments...)> {
private:
    struct Operations {
        Return (*invoke)(void*, Arguments&&...);
        void (*destroy)(void*);
        void* (*clone)(const void*);
    };

    void* m_target = nullptr;
    const Operations* m_operations = nullptr;
    std::uint64_t m_reserved[2] = {0, 0};

    template <typename Callable>
    static Return Invoke(void* target, Arguments&&... arguments) {
        if constexpr (std::is_void_v<Return>) {
            std::invoke(
                *static_cast<Callable*>(target),
                std::forward<Arguments>(arguments)...);
        } else {
            return std::invoke(
                *static_cast<Callable*>(target),
                std::forward<Arguments>(arguments)...);
        }
    }

    template <typename Callable>
    static void Destroy(void* target) {
        delete static_cast<Callable*>(target);
    }

    template <typename Callable>
    static void* Clone(const void* target) {
        return new Callable(*static_cast<const Callable*>(target));
    }

    template <typename Callable>
    static const Operations& GetOperations() {
        static const Operations operations = {
            &Invoke<Callable>,
            &Destroy<Callable>,
            &Clone<Callable>};
        return operations;
    }

    void Reset() noexcept {
        if (m_operations) {
            m_operations->destroy(m_target);
        }
        m_target = nullptr;
        m_operations = nullptr;
        m_reserved[0] = 0;
        m_reserved[1] = 0;
    }

    template <typename Callable>
    void Assign(Callable&& callable) {
        using StoredCallable = std::decay_t<Callable>;
        void* target = new StoredCallable(std::forward<Callable>(callable));
        m_target = target;
        m_operations = &GetOperations<StoredCallable>();
    }

public:
    function() noexcept = default;
    function(std::nullptr_t) noexcept {}

    function(const function& other) {
        if (other.m_operations) {
            m_target = other.m_operations->clone(other.m_target);
            m_operations = other.m_operations;
        }
    }

    function(function&& other) noexcept
        : m_target(other.m_target)
        , m_operations(other.m_operations)
        , m_reserved{other.m_reserved[0], other.m_reserved[1]} {
        other.m_target = nullptr;
        other.m_operations = nullptr;
        other.m_reserved[0] = 0;
        other.m_reserved[1] = 0;
    }

    template <typename Callable,
              typename = std::enable_if_t<!std::is_same_v<
                  std::decay_t<Callable>, function>>>
    function(Callable&& callable) {
        Assign(std::forward<Callable>(callable));
    }

    ~function() {
        Reset();
    }

    function& operator=(const function& other) {
        if (this != &other) {
            function copy(other);
            swap(copy);
        }
        return *this;
    }

    function& operator=(function&& other) noexcept {
        if (this != &other) {
            Reset();
            m_target = other.m_target;
            m_operations = other.m_operations;
            m_reserved[0] = other.m_reserved[0];
            m_reserved[1] = other.m_reserved[1];
            other.m_target = nullptr;
            other.m_operations = nullptr;
            other.m_reserved[0] = 0;
            other.m_reserved[1] = 0;
        }
        return *this;
    }

    function& operator=(std::nullptr_t) noexcept {
        Reset();
        return *this;
    }

    template <typename Callable,
              typename = std::enable_if_t<!std::is_same_v<
                  std::decay_t<Callable>, function>>>
    function& operator=(Callable&& callable) {
        function replacement(std::forward<Callable>(callable));
        swap(replacement);
        return *this;
    }

    explicit operator bool() const noexcept {
        return m_operations != nullptr;
    }

    Return operator()(Arguments... arguments) const {
        if (!m_operations) {
            throw std::bad_function_call();
        }
        if constexpr (std::is_void_v<Return>) {
            m_operations->invoke(
                m_target,
                std::forward<Arguments>(arguments)...);
        } else {
            return m_operations->invoke(
                m_target,
                std::forward<Arguments>(arguments)...);
        }
    }

    void swap(function& other) noexcept {
        std::swap(m_target, other.m_target);
        std::swap(m_operations, other.m_operations);
        std::swap(m_reserved[0], other.m_reserved[0]);
        std::swap(m_reserved[1], other.m_reserved[1]);
    }
};

static_assert(sizeof(function<void()>) == 32);

} // namespace tr1
} // namespace std
