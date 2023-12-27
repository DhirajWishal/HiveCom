#pragma once

///
/// @section Key exchange mechanism
/// This section describes how the key exchange mechanism work with the Kyber768 class.
///
/// Let there be two communicating parties, A and B.
///
/// 1. A generates a key-pair, 'pk' and 'sk' (public key and private key respectively).
/// 2. A shares the 'pk' with B.
/// 3. B encapsulates the 'pk' and receives ciphertext 'c' and shared secret 's'.
/// 4. B shares the ciphertext 'c' with A.
/// 5. A decapsulates 'c' with 'sk' and receives 's'.
/// 6. A and B uses 's' to encrypt communications.
///

#include "Kyber768Key.hpp"

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
        [[nodiscard]] Kyber768Key generateKey() const;

        /// @brief Encapsulate a generated key.
        /// @param key The public key to encapsulate.
        /// @return The pair of shared secret and ciphertext.
        [[nodiscard]] EncapsulationType encapsulate(const ByteView &key) const;

        /// @brief Decapsulate a generated key.
        /// @param key The private key to decapsulate with.
        /// @param ciphertext The cipher text.
        /// @return The shared secret.
        [[nodiscard]] SharedSecretType decapsulate(const ByteView &key, const CiphertextType &ciphertext) const;

      private:
        Implementation *m_pImplementation = nullptr;
    };
} // namespace HiveCom
