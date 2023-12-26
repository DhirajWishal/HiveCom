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
    template <std::size_t Size> inline FixedBytes<Size - 1> ToFixedBytes(const char (&string)[Size])
    {
        FixedBytes<Size - 1> bytes;
        for (std::size_t i = 0; i < Size - 1; i++)
            bytes[i] = string[i];

        return bytes;
    }
} // namespace HiveCom
