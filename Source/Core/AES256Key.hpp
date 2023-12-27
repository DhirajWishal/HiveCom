#pragma once

#include "Key.hpp"
#include "Types.hpp"

namespace HiveCom
{
    /// @brief Key class for the AES256 encryption.
    /// This class contains the key, IV and authentication (salt) for AES256 block encryption.
    class AES256Key final : public Key
    {
      public:
        static constexpr auto KeySize = 256 / 8;
        static constexpr auto IVSize = 128 / 8;

        using KeyType = FixedBytes<KeySize>;
        using IVType = FixedBytes<IVSize>;
        using AuthType = Bytes;

      public:
        /// @brief Default constructor.
        AES256Key() = default;

        /// @brief Explicit constructor.
        /// @param key The key to initialize with.
        /// @param iv The initialization vector to initialize with.
        /// @param auth The authentication bytes.
        explicit AES256Key(const KeyType &key, const IVType &iv, AuthType auth)
            : m_key(key), m_iv(iv), m_authentication(std::move(auth))
        {
        }

        /// @brief Get the key.
        /// @return The key view.
        [[nodiscard]] ByteView getKey() const
        {
            return ToView(m_key);
        }

        /// @brief Get the initialization vector.
        /// @return The initialization vector view.
        [[nodiscard]] ByteView getIV() const
        {
            return ToView(m_iv);
        }

        /// @brief Get the authentication bytes.
        /// @return The authentication view.
        [[nodiscard]] ByteView getAuthentication() const
        {
            return m_authentication;
        }

      private:
        KeyType m_key;
        IVType m_iv;
        AuthType m_authentication;
    };
} // namespace HiveCom
