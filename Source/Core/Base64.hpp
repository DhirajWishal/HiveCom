#pragma once

#include "Types.hpp"

namespace HiveCom
{
    /// @brief Base64 class.
    /// This class can be used to either encode or decode base64 data.
    class Base64 final
    {
      public:
        /// @brief Explicit constructor.
        /// @param bytes The bytes to initialize the class with.
        explicit Base64(ByteView bytes) : m_bytes(bytes)
        {
        }

        /// @brief Encode the stored data.
        /// Note that this will not update the stored data.
        /// @return The encoded data.
        [[nodiscard]] Bytes encode() const;

        /// @brief Decode the stored data.
        /// Note that this will not update the stored data.
        /// @return The decode data.
        [[nodiscard]] Bytes decode() const;

      private:
        /// @brief Validate a cryptographic operation.
        /// @param value The return value of the operation.
        static void Validate(int value);

      private:
        ByteView m_bytes;
    };
} // namespace HiveCom
