#pragma once

#include "KeyKyber768.hpp"

namespace HiveCom
{
    /// @brief Kyber 768-bit class.
    /// This class contains the kyber algorithm implementation.
    class Kyber768 final
    {
      private:
        /// @brief Implementation forward declaration.
        class Implementation;

      public:
        static constexpr auto SharedSecretSize = 32; // Size taken from `OQS_KEM_kyber_768_length_shared_secret`.
        static constexpr auto CiphertextSize = 1088; // Size taken from `OQS_KEM_kyber_768_length_ciphertext`.

        using SharedSecretType = FixedBytes<SharedSecretSize>;
        using CiphertextType = FixedBytes<CiphertextSize>;
        using EncapsulationType = std::pair<SharedSecretType, CiphertextType>;

      public:
        /// @brief Constructor.
        Kyber768();

        /// @brief Destructor.
        ~Kyber768();

        /// @brief Generate a new Kyber key.
        /// @return The generated key.
        [[nodiscard]] KeyKyber768 generateKey();

        /// @brief Encapsulate a generated key.
        /// @param key The public key to encapsulate.
        /// @return The pair of shared secret and ciphertext.
        [[nodiscard]] EncapsulationType encapsulate(const ByteView &key);

        /// @brief Decapsulate a generated key.
        /// @param key The private key to decapsulate with.
        /// @param ciphertext The cipher text.
        /// @return The shared secret.
        [[nodiscard]] SharedSecretType decapsulate(const ByteView &key, const CiphertextType &ciphertext);

      private:
        Implementation *m_pImplementation = nullptr;
    };
} // namespace HiveCom