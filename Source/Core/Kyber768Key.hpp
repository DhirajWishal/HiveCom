#pragma once

#include "Key.hpp"
#include "Types.hpp"

namespace HiveCom
{
    /// @brief Kyber 768 encryption key storage.
    class Kyber768Key final : public Key
    {
      public:
        static constexpr auto PublicKeySize = 1184;  // Size taken from `OQS_KEM_kyber_768_length_public_key`.
        static constexpr auto PrivateKeySize = 2400; // Size taken from `OQS_KEM_kyber_768_length_secret_key`.

        using PublicKeyType = FixedBytes<PublicKeySize>;
        using PrivateKeyType = FixedBytes<PrivateKeySize>;

      public:
        /// @brief Default constructor.
        Kyber768Key() = default;

        /// @brief Explicit constructor.
        /// @param publicKey The public key.
        /// @param privateKey The private key.
        explicit Kyber768Key(const PublicKeyType &publicKey, const PrivateKeyType &privateKey)
            : m_publicKey(publicKey), m_privateKey(privateKey)
        {
        }

        /// @brief Get the public key.
        /// @return The stored public key.
        [[nodiscard]] ByteView getPublicKey() const
        {
            return ToView(m_publicKey);
        }

        /// @brief Get the private key.
        /// @return The stored private key.
        [[nodiscard]] ByteView getPrivateKey() const
        {
            return ToView(m_privateKey);
        }

      private:
        PublicKeyType m_publicKey;
        PrivateKeyType m_privateKey;
    };
} // namespace HiveCom
