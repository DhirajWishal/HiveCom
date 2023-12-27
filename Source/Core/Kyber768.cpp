#include "Kyber768.hpp"
#include "oqs/kem_kyber.h"

#include <iostream>
#include <oqs/kem.h>
#include <utility>

#define HC_OQS_ASSERT(exp, msg)                                                                                        \
    if (exp != OQS_SUCCESS)                                                                                            \
    std::cerr << msg << std::endl

namespace HiveCom
{
    /// @brief Implementation class of kyber.
    /// This class will abstract out the implementation details.
    class Kyber768::Implementation final
    {
      public:
        /// @brief Constructor.
        Implementation() : m_kem(OQS_KEM_new(OQS_KEM_alg_kyber_768))
        {
        }

        /// @brief Destructor.
        ~Implementation()
        {
            OQS_KEM_free(m_kem);
        }

        /// @brief Get the key encapsulation mechanism.
        /// @return The KEM pointer.
        [[nodiscard]] OQS_KEM *getKEM()
        {
            return m_kem;
        }

        /// @brief Get the key encapsulation mechanism.
        /// @return The KEM pointer.
        [[nodiscard]] const OQS_KEM *getKEM() const
        {
            return m_kem;
        }

      private:
        OQS_KEM *m_kem = nullptr;
    };

    Kyber768::Kyber768() : m_pImplementation(new Implementation())
    {
        static_assert(Kyber768::SharedSecretSize == OQS_KEM_kyber_768_length_shared_secret,
                      "Invalid shared secret size!");
    }

    Kyber768::~Kyber768()
    {
        delete m_pImplementation;
    }

    KeyKyber768 Kyber768::generateKey()
    {
        static_assert(KeyKyber768::PublicKeySize == OQS_KEM_kyber_768_length_public_key, "Invalid public key size!");
        static_assert(KeyKyber768::PrivateKeySize == OQS_KEM_kyber_768_length_secret_key, "Invalid public key size!");

        KeyKyber768::PublicKeyType publicKey;
        KeyKyber768::PrivateKeyType privateKey;
        HC_OQS_ASSERT(OQS_KEM_keypair(m_pImplementation->getKEM(), publicKey.data(), privateKey.data()),
                      "Failed to generate key pair!");

        return KeyKyber768(publicKey, privateKey);
    }

    Kyber768::EncapsulationType Kyber768::encapsulate(const ByteView &key)
    {
        EncapsulationType data;
        HC_OQS_ASSERT(OQS_KEM_encaps(m_pImplementation->getKEM(), data.second.data(), data.first.data(), key.data()),
                      "Failed to encapsulate the key!");
        return data;
    }

    Kyber768::SharedSecretType Kyber768::decapsulate(const ByteView &key, const Kyber768::CiphertextType &ciphertext)
    {
        SharedSecretType sharedSecret;
        HC_OQS_ASSERT(OQS_KEM_decaps(m_pImplementation->getKEM(), sharedSecret.data(), ciphertext.data(), key.data()),
                      "Failed to decapsulate the key!");

        return sharedSecret;
    }
} // namespace HiveCom