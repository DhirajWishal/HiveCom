#pragma once

#include "Key.hpp"
#include "Types.hpp"

namespace HiveCom
{
    /// @brief Dilithium-3 key class.
    /// This class contains the public and private key pairs of the algorithm.
    class Dilithium3Key final : public Key
    {
      public:
        static constexpr auto PublicKeySize = 1952;  // Size taken from `OQS_SIG_dilithium_3_length_public_key`.
        static constexpr auto PrivateKeySize = 4000; // Size taken from `OQS_SIG_dilithium_3_length_secret_key`.

        using PublicKeyType = FixedBytes<PublicKeySize>;
        using PrivateKeyType = FixedBytes<PrivateKeySize>;

      public:
        /// @brief Default constructor.
        Dilithium3Key() = default;

        /// @brief Explicit constructor.
        /// @param publicKey The public key.
        /// @param privateKey The private key.
        explicit Dilithium3Key(const PublicKeyType &publicKey, const PrivateKeyType &privateKey)
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