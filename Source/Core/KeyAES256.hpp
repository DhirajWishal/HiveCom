#pragma once

#include "Key.hpp"
#include "Types.hpp"

namespace HiveCom
{
    /// @brief Key class for the AES256 encryption.
    /// This class contains the key, IV and authentication (salt) for AES256 block encryption.
    class KeyAES256 final : public Key
    {
      public:
        static constexpr auto KeySize = 256 / 8;
        static constexpr auto IVSize = 128 / 8;

        using KeyType = FixedBytes<KeySize>;
        using IVType = FixedBytes<IVSize>;
        using AuthType = Bytes;

      public:
        /// @brief Default constructor.
        KeyAES256() = default;

        /// @brief Explicit constructor.
        /// @param key The key to initialize with.
        /// @param iv The initialization vector to initialize with.
        /// @param auth The authentication bytes.
        explicit KeyAES256(const KeyType &key, const IVType &iv, const AuthType &auth)
            : m_key(key), m_iv(iv), m_authentication(auth)
        {
        }

        /// @brief Get the key.
        /// @return The key view.
        [[nodiscard]] ByteView getKey() const
        {
            return ByteView(m_key.begin(), m_key.end());
        }

        /// @brief Get the initialization vector.
        /// @return The initialization vector view.
        [[nodiscard]] ByteView getIV() const
        {
            return ByteView(m_iv.begin(), m_iv.end());
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
