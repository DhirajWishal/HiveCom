#pragma once

#include "Types.hpp"

namespace HiveCom
{
    /// @brief SHA256 class.
    class SHA256 final
    {
      public:
        static constexpr auto DigestSize = 256 / 8;

        using DigestType = FixedBytes<DigestSize>;

      public:
        /// @brief Default constructor.
        SHA256() = default;

        [[nodiscard]] DigestType digest(ByteView bytes) const;
    };
} // namespace HiveCom
