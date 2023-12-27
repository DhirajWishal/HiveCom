#pragma once

#include <array>
#include <string>
#include <string_view>

namespace HiveCom
{
    /// @brief Byte type.
    using Byte = unsigned char;

    /// @brief Fixed bytes type.
    /// This is an array of bytes with a fixed size.
    template <std::size_t Size> using FixedBytes = std::array<Byte, Size>;

    /// @brief Bytes type.
    /// This is an array of bytes of variable length.
    using Bytes = std::basic_string<Byte>;

    /// @brief Byte view type.
    /// This contains information about the start and end of bytes without copying it.
    using ByteView = std::basic_string_view<Byte>;

    /// @brief Convert a string to bytes.
    /// @param string The incoming string.
    /// @return The converted bytes.
    inline Bytes ToBytes(std::string_view string)
    {
        return Bytes(string.begin(), string.end());
    }

    /// @brief Convert a string to fixed bytes.
    /// @tparam Size The string size constant.
    /// @param string The incoming string.
    /// @return The converted fixed bytes.
    template <std::size_t Size> FixedBytes<Size - 1> ToFixedBytes(const char (&string)[Size])
    {
        FixedBytes<Size - 1> bytes;
        for (std::size_t i = 0; i < Size - 1; i++)
            bytes[i] = string[i];

        return bytes;
    }

    /// @brief Convert a string of bytes to fixed bytes.
    /// @tparam Size The byte size constant.
    /// @param input The incoming string of bytes.
    /// @return The converted fixed bytes.
    template <std::size_t Size> FixedBytes<Size> ToFixedBytes(const ByteView &input)
    {
        FixedBytes<Size> bytes;
        for (std::size_t i = 0; i < Size - 1; i++)
            bytes[i] = input[i];

        return bytes;
    }

    /// @brief Convert a bytes to a string.
    /// @param bytes The bytes to convert.
    /// @return The converted string.
    inline std::string ToString(ByteView bytes)
    {
        return std::string(bytes.begin(), bytes.end());
    }

    /// @brief Convert fixed bytes to a string.
    /// @param bytes The bytes to convert.
    /// @return The converted string.
    template <std::size_t Size> std::string ToString(const FixedBytes<Size> &bytes)
    {
        return std::string(bytes.begin(), bytes.end());
    }

    /// @brief Convert fixed bytes to a byte view.
    /// @param bytes The bytes to convert.
    /// @return The converted view.
    template <std::size_t Size> ByteView ToView(const FixedBytes<Size> &bytes)
    {
        return ByteView(bytes.data(), bytes.size());
    }
} // namespace HiveCom
