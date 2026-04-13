#pragma once

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <cwchar>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

class XParse {
public:
    using Value = std::variant<int,
                               unsigned int,
                               std::int64_t,
                               std::uint64_t,
                               std::uint16_t,
                               std::uint8_t,
                               float,
                               bool,
                               std::string,
                               std::wstring>;

    XParse() = default;

    template <typename T>
    XParse& operator<<(const T& value) {
        if constexpr (std::is_same_v<std::decay_t<T>, std::string>) {
            values_.emplace_back(value);
            WriteByteString(value);
        } else if constexpr (std::is_same_v<std::decay_t<T>, std::wstring>) {
            values_.emplace_back(value);
            WriteWideString(value);
        } else if constexpr (std::is_same_v<std::decay_t<T>, bool>) {
            values_.emplace_back(value);
            const std::uint8_t byteValue = value ? 1u : 0u;
            AppendRaw(&byteValue, sizeof(byteValue));
        } else if constexpr (std::is_integral_v<std::decay_t<T>>) {
            values_.emplace_back(value);
            const std::decay_t<T> rawValue = value;
            AppendRaw(&rawValue, sizeof(rawValue));
        } else if constexpr (std::is_floating_point_v<std::decay_t<T>>) {
            values_.emplace_back(value);
            const std::decay_t<T> rawValue = value;
            AppendRaw(&rawValue, sizeof(rawValue));
        } else {
            static_assert(!sizeof(T), "Unsupported XParse insertion type");
        }
        return *this;
    }

    XParse& operator>>(int& value) {
        if (TryReadRaw(value)) {
            return *this;
        }
        if (!ReadIntegral(value)) {
            m_eError = 1;
            value = 0;
        }
        return *this;
    }

    XParse& operator>>(unsigned int& value) {
        if (TryReadRaw(value)) {
            return *this;
        }
        if (!ReadIntegral(value)) {
            m_eError = 1;
            value = 0;
        }
        return *this;
    }

    XParse& operator>>(std::int64_t& value) {
        if (TryReadRaw(value)) {
            return *this;
        }
        if (!ReadIntegral(value)) {
            m_eError = 1;
            value = 0;
        }
        return *this;
    }

    XParse& operator>>(std::uint64_t& value) {
        if (TryReadRaw(value)) {
            return *this;
        }
        if (!ReadIntegral(value)) {
            m_eError = 1;
            value = 0;
        }
        return *this;
    }

    XParse& operator>>(std::uint16_t& value) {
        if (TryReadRaw(value)) {
            return *this;
        }
        if (!ReadIntegral(value)) {
            m_eError = 1;
            value = 0;
        }
        return *this;
    }

    XParse& operator>>(std::int16_t& value) {
        if (TryReadRaw(value)) {
            return *this;
        }
        if (!ReadIntegral(value)) {
            m_eError = 1;
            value = 0;
        }
        return *this;
    }

    XParse& operator>>(std::uint8_t& value) {
        if (TryReadRaw(value)) {
            return *this;
        }
        if (!ReadIntegral(value)) {
            m_eError = 1;
            value = 0;
        }
        return *this;
    }

    XParse& operator>>(float& value) {
        if (TryReadRaw(value)) {
            return *this;
        }
        if (!ReadFloating(value)) {
            m_eError = 1;
            value = 0.0f;
        }
        return *this;
    }

    XParse& operator>>(bool& value) {
        std::uint8_t rawValue = 0;
        if (TryReadRaw(rawValue)) {
            value = (rawValue != 0);
            return *this;
        }

        if (cursor_ >= values_.size()) {
            m_eError = 1;
            value = false;
            return *this;
        }

        const Value& current = values_[cursor_++];
        if (const bool* typed = std::get_if<bool>(&current)) {
            value = *typed;
            return *this;
        }
        if (const std::uint8_t* typed = std::get_if<std::uint8_t>(&current)) {
            value = (*typed != 0);
            return *this;
        }
        if (const int* typed = std::get_if<int>(&current)) {
            value = (*typed != 0);
            return *this;
        }
        if (const unsigned int* typed = std::get_if<unsigned int>(&current)) {
            value = (*typed != 0);
            return *this;
        }

        m_eError = 1;
        value = false;
        return *this;
    }

    XParse& operator>>(std::string& value) {
        if (CanReadRaw(sizeof(std::uint16_t))) {
            short outLen = 0;
            const std::uint16_t byteLength = PeekRaw<std::uint16_t>();
            if (byteLength > 0) {
                value.resize(byteLength);
                GetString(value.data(), static_cast<short>(byteLength + 1), &outLen);
                if (!m_eError) {
                    value.resize(static_cast<std::size_t>(outLen));
                    return *this;
                }
            } else {
                GetWORD();
                m_eError = 1;
            }
        }

        if (!ReadExact(value)) {
            m_eError = 1;
            value.clear();
        }
        return *this;
    }

    XParse& operator>>(std::wstring& value) {
        if (CanReadRaw(sizeof(std::uint16_t))) {
            const std::uint16_t byteLength = PeekRaw<std::uint16_t>();
            if (byteLength == 0) {
                value.clear();
                GetWORD();
                return *this;
            }

            const short maxLen = static_cast<short>(byteLength / 2 + 1);
            short outLen = 0;
            std::vector<wchar_t> buffer(static_cast<std::size_t>(maxLen), L'\0');
            GetWString(buffer.data(), maxLen, outLen);
            if (!m_eError) {
                value.assign(buffer.data());
                return *this;
            }
        }

        if (!ReadExact(value)) {
            m_eError = 1;
            value.clear();
        }
        return *this;
    }

    void BindBuffer(char* buffer, std::size_t maxLen, std::size_t initialIndex = 0) {
        ownedBuffer_.clear();
        m_pBuffer = buffer;
        m_usMaxLen = static_cast<std::uint16_t>((std::min<std::size_t>)(maxLen, 0xFFFFu));
        m_usIndex =
            static_cast<std::uint16_t>((std::min<std::size_t>)(initialIndex, m_usMaxLen));
        m_eError = 0;
        cursor_ = 0;
    }

    void AssignRawBuffer(const char* buffer, std::size_t len) {
        ownedBuffer_.assign(buffer, buffer + len);
        m_pBuffer = ownedBuffer_.empty() ? nullptr : ownedBuffer_.data();
        m_usMaxLen =
            static_cast<std::uint16_t>((std::min<std::size_t>)(ownedBuffer_.size(), 0xFFFFu));
        m_usIndex = 0;
        m_eError = 0;
        cursor_ = 0;
    }

    void GetString(char* buffer, short maxLen, short* outLen) {
        if (CanReadRaw(sizeof(std::uint16_t))) {
            const std::uint16_t stringLength = GetWORD();
            if (outLen) {
                *outLen = static_cast<short>(stringLength);
            }

            if (stringLength > 0 && stringLength < static_cast<std::uint16_t>(maxLen)) {
                GetBytes(buffer, stringLength);
                if (buffer) {
                    buffer[stringLength] = '\0';
                }
                return;
            }

            m_eError = 1;
            if (buffer && maxLen > 0) {
                buffer[0] = '\0';
            }
            return;
        }

        std::string value;
        *this >> value;
        if (m_eError) {
            if (outLen) {
                *outLen = 0;
            }
            if (buffer && maxLen > 0) {
                buffer[0] = '\0';
            }
            return;
        }

        const std::size_t safeMaxLen = maxLen > 0 ? static_cast<std::size_t>(maxLen - 1) : 0U;
        const std::size_t copyLen = (std::min)(value.size(), safeMaxLen);
        if (buffer && maxLen > 0) {
            std::fill_n(buffer, static_cast<std::size_t>(maxLen), '\0');
            std::copy_n(value.data(), copyLen, buffer);
            buffer[copyLen] = '\0';
        }
        if (outLen) {
            *outLen = static_cast<short>(copyLen);
        }
    }

    void GetBytes(char* buffer, std::size_t len) {
        if (CanReadRaw(len)) {
            if (buffer && len > 0) {
                std::memcpy(buffer, m_pBuffer + m_usIndex, len);
            }
            m_usIndex = static_cast<std::uint16_t>(m_usIndex + len);
            return;
        }

        std::string value;
        *this >> value;
        if (buffer && len > 0) {
            std::fill_n(buffer, len, '\0');
        }
        if (m_eError || !buffer || len == 0) {
            return;
        }

        const std::size_t copyLen = (std::min)(value.size(), len);
        std::copy_n(value.data(), copyLen, buffer);
    }

    void GetWString(wchar_t* buffer, short maxLen, short& outLen) {
        if (CanReadRaw(sizeof(std::uint16_t))) {
            const std::uint16_t byteLength = GetWORD();
            outLen = static_cast<short>(byteLength);

            const int maxByteLength = static_cast<int>(maxLen) * 2 - 2;
            if (byteLength > 0 && byteLength <= maxByteLength) {
                std::vector<std::uint8_t> raw(static_cast<std::size_t>(byteLength));
                GetBytes(reinterpret_cast<char*>(raw.data()), raw.size());
                WriteUtf16ToWideBuffer(raw, buffer, maxLen);
                return;
            }

            if (buffer && maxLen > 0) {
                buffer[0] = L'\0';
            }
            if (byteLength != 0) {
                m_eError = 1;
            }
            return;
        }

        std::wstring value;
        *this >> value;
        if (m_eError) {
            outLen = 0;
            if (buffer && maxLen > 0) {
                buffer[0] = L'\0';
            }
            return;
        }

        const std::size_t safeMaxLen = maxLen > 0 ? static_cast<std::size_t>(maxLen - 1) : 0U;
        const std::size_t copyLen = (std::min)(value.size(), safeMaxLen);
        if (buffer && maxLen > 0) {
            std::wmemset(buffer, 0, static_cast<std::size_t>(maxLen));
            std::wmemcpy(buffer, value.c_str(), copyLen);
            buffer[copyLen] = L'\0';
        }
        outLen = static_cast<short>(copyLen * sizeof(wchar_t));
    }

    void SetBytes(const char* buffer, std::size_t len) {
        if (!buffer || len == 0) {
            values_.emplace_back(std::string());
            return;
        }

        values_.emplace_back(std::string(buffer, len));
        AppendRaw(buffer, len);
    }

    void SetUsIndex(short usIndex) {
        m_usIndex = static_cast<std::uint16_t>(usIndex);
    }

    std::uint16_t GetWORD() {
        std::uint16_t value = 0;
        if (TryReadRaw(value)) {
            return value;
        }
        m_eError = 1;
        return 0;
    }

    unsigned int GetDWORD() {
        unsigned int value = 0;
        if (TryReadRaw(value)) {
            return value;
        }
        m_eError = 1;
        return 0;
    }

    std::uint8_t GetBYTE() {
        std::uint8_t value = 0;
        if (TryReadRaw(value)) {
            return value;
        }
        m_eError = 1;
        return 0;
    }

    void SetWORD(unsigned short& inValue) {
        AppendRaw(&inValue, sizeof(inValue));
    }

    void SetDWORD(unsigned long& inValue) {
        const std::uint32_t value = static_cast<std::uint32_t>(inValue);
        AppendRaw(&value, sizeof(value));
    }

    void SetBYTE(unsigned char& inValue) {
        AppendRaw(&inValue, sizeof(inValue));
    }

    int GetType() const {
        return m_eError;
    }

    int GetLastError() const {
        return m_eError;
    }

    void ClearError() {
        m_eError = 0;
    }

    std::size_t Size() const {
        return values_.size();
    }

    const std::vector<Value>& Values() const {
        return values_;
    }

    void Append(const std::vector<Value>& values) {
        values_.insert(values_.end(), values.begin(), values.end());
    }

    void Assign(const std::vector<Value>& values) {
        values_ = values;
        cursor_ = 0;
        m_eError = 0;
        m_usIndex = 0;
    }

    const char* Data() const {
        return m_pBuffer;
    }

    char* Data() {
        return m_pBuffer;
    }

    char* m_pBuffer = nullptr;
    int m_eError = 0;
    std::uint16_t m_usIndex = 0;
    std::uint16_t m_usMaxLen = 0xFFFFu;

private:
    template <typename T>
    bool ReadIntegral(T& value) {
        static_assert(std::is_integral_v<T>);
        if (cursor_ >= values_.size()) {
            return false;
        }

        const Value& current = values_[cursor_++];
        if (const int* typed = std::get_if<int>(&current)) {
            value = static_cast<T>(*typed);
            return true;
        }
        if (const unsigned int* typed = std::get_if<unsigned int>(&current)) {
            value = static_cast<T>(*typed);
            return true;
        }
        if (const std::int64_t* typed = std::get_if<std::int64_t>(&current)) {
            value = static_cast<T>(*typed);
            return true;
        }
        if (const std::uint64_t* typed = std::get_if<std::uint64_t>(&current)) {
            value = static_cast<T>(*typed);
            return true;
        }
        if (const std::uint16_t* typed = std::get_if<std::uint16_t>(&current)) {
            value = static_cast<T>(*typed);
            return true;
        }
        if (const std::uint8_t* typed = std::get_if<std::uint8_t>(&current)) {
            value = static_cast<T>(*typed);
            return true;
        }
        if (const bool* typed = std::get_if<bool>(&current)) {
            value = static_cast<T>(*typed ? 1 : 0);
            return true;
        }
        if (const float* typed = std::get_if<float>(&current)) {
            value = static_cast<T>(*typed);
            return true;
        }
        return false;
    }

    template <typename T>
    bool ReadFloating(T& value) {
        static_assert(std::is_floating_point_v<T>);
        if (cursor_ >= values_.size()) {
            return false;
        }

        const Value& current = values_[cursor_++];
        if (const float* typed = std::get_if<float>(&current)) {
            value = static_cast<T>(*typed);
            return true;
        }
        if (const int* typed = std::get_if<int>(&current)) {
            value = static_cast<T>(*typed);
            return true;
        }
        if (const unsigned int* typed = std::get_if<unsigned int>(&current)) {
            value = static_cast<T>(*typed);
            return true;
        }
        return false;
    }

    template <typename T>
    bool ReadExact(T& value) {
        if (cursor_ >= values_.size()) {
            return false;
        }

        const Value& current = values_[cursor_++];
        if (const T* typed = std::get_if<T>(&current)) {
            value = *typed;
            return true;
        }
        return false;
    }

    template <typename T>
    bool TryReadRaw(T& value) {
        if (!CanReadRaw(sizeof(T))) {
            return false;
        }

        std::memcpy(&value, m_pBuffer + m_usIndex, sizeof(T));
        m_usIndex = static_cast<std::uint16_t>(m_usIndex + sizeof(T));
        return true;
    }

    template <typename T>
    T PeekRaw() const {
        T value{};
        if (CanReadRaw(sizeof(T))) {
            std::memcpy(&value, m_pBuffer + m_usIndex, sizeof(T));
        }
        return value;
    }

    bool CanReadRaw(std::size_t byteCount) const {
        return m_pBuffer != nullptr &&
               static_cast<std::size_t>(m_usIndex) + byteCount <= static_cast<std::size_t>(m_usMaxLen);
    }

    bool EnsureWriteCapacity(std::size_t byteCount) {
        const std::size_t required = static_cast<std::size_t>(m_usIndex) + byteCount;
        if (required > 0xFFFFu) {
            m_eError = 1;
            return false;
        }

        if (m_pBuffer && required <= static_cast<std::size_t>(m_usMaxLen)) {
            return true;
        }

        if (!ownedBuffer_.empty() &&
            reinterpret_cast<char*>(ownedBuffer_.data()) == m_pBuffer &&
            required <= 0xFFFFu) {
            ownedBuffer_.resize(required);
            m_pBuffer = ownedBuffer_.empty() ? nullptr : ownedBuffer_.data();
            m_usMaxLen = static_cast<std::uint16_t>(ownedBuffer_.size());
            return true;
        }

        if (m_pBuffer == nullptr) {
            ownedBuffer_.resize(required);
            m_pBuffer = ownedBuffer_.data();
            m_usMaxLen = static_cast<std::uint16_t>(ownedBuffer_.size());
            return true;
        }

        m_eError = 1;
        return false;
    }

    void AppendRaw(const void* data, std::size_t len) {
        if (!data || len == 0 || !EnsureWriteCapacity(len)) {
            return;
        }

        std::memcpy(m_pBuffer + m_usIndex, data, len);
        m_usIndex = static_cast<std::uint16_t>(m_usIndex + len);
    }

    void WriteByteString(const std::string& value) {
        const std::uint16_t byteLength =
            static_cast<std::uint16_t>((std::min<std::size_t>)(value.size(), 0xFFFFu));
        AppendRaw(&byteLength, sizeof(byteLength));
        if (byteLength > 0) {
            AppendRaw(value.data(), byteLength);
        }
    }

    void WriteWideString(const std::wstring& value) {
        std::vector<std::uint8_t> utf16Bytes;
        utf16Bytes.reserve(value.size() * 2);

        for (wchar_t codeUnit : value) {
            const std::uint16_t wideChar = static_cast<std::uint16_t>(codeUnit);
            utf16Bytes.push_back(static_cast<std::uint8_t>(wideChar & 0xFF));
            utf16Bytes.push_back(static_cast<std::uint8_t>((wideChar >> 8) & 0xFF));
        }

        const std::uint16_t byteLength =
            static_cast<std::uint16_t>((std::min<std::size_t>)(utf16Bytes.size(), 0xFFFFu));
        AppendRaw(&byteLength, sizeof(byteLength));
        if (byteLength > 0) {
            AppendRaw(utf16Bytes.data(), byteLength);
        }
    }

    static void WriteUtf16ToWideBuffer(const std::vector<std::uint8_t>& raw,
                                       wchar_t* buffer,
                                       short maxLen) {
        if (!buffer || maxLen <= 0) {
            return;
        }

        const std::size_t wcharCount =
            (std::min<std::size_t>)(raw.size() / 2, static_cast<std::size_t>(maxLen - 1));
        for (std::size_t i = 0; i < wcharCount; ++i) {
            const std::uint16_t value =
                static_cast<std::uint16_t>(raw[i * 2]) |
                static_cast<std::uint16_t>(raw[i * 2 + 1] << 8);
            buffer[i] = static_cast<wchar_t>(value);
        }
        buffer[wcharCount] = L'\0';
    }

private:
    std::vector<char> ownedBuffer_;
    std::vector<Value> values_;
    std::size_t cursor_ = 0;
};
