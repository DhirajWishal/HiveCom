#include "Dilithium3.hpp"
#include "Logging.hpp"

#include <oqs/sig.h>

#define HC_OQS_ASSERT(exp, ...)                                                                                        \
    if (exp != OQS_SUCCESS)                                                                                            \
    HC_LOG_ERROR(__VA_ARGS__)

namespace HiveCom
{
    /// @brief Implementation class.
    /// This class contains the implementation details which are abstracted out by the public API.
    class Dilithium3::Implementation final
    {
      public:
        /// @brief Constructor.
        Implementation() : m_sig(OQS_SIG_new(OQS_SIG_alg_dilithium_3))
        {
        }

        /// @brief Destructor.
        ~Implementation()
        {
        }

        /// @brief Get the signature instance.
        /// @return The context pointer.
        [[nodiscard]] OQS_SIG *getSIG()
        {
            return m_sig;
        }

        /// @brief Get the signature instance.
        /// @return The context pointer.
        [[nodiscard]] const OQS_SIG *getSIG() const
        {
            return m_sig;
        }

      private:
        OQS_SIG *m_sig = nullptr;
    };

    Dilithium3::Dilithium3() : m_pImplementation(new Implementation())
    {
        static_assert(SignatureSize == OQS_SIG_dilithium_3_length_signature, "Invalid shared secret size!");
    }

    Dilithium3::~Dilithium3()
    {
        delete m_pImplementation;
    }

    Dilithium3Key Dilithium3::generateKey() const
    {
        static_assert(Dilithium3Key::PublicKeySize == OQS_SIG_dilithium_3_length_public_key,
                      "Invalid public key size!");
        static_assert(Dilithium3Key::PrivateKeySize == OQS_SIG_dilithium_3_length_secret_key,
                      "Invalid private key size!");

        Dilithium3Key::PublicKeyType publicKey;
        Dilithium3Key::PrivateKeyType privateKey;

        HC_OQS_ASSERT(OQS_SIG_keypair(m_pImplementation->getSIG(), publicKey.data(), privateKey.data()),
                      "Failed to create the key pair!");

        return Dilithium3Key(publicKey, privateKey);
    }

    Dilithium3::SignatureType Dilithium3::sign(const ByteView key, const ByteView bytes) const
    {
        SignatureType signature;
        std::size_t signatureLength = 0;
        HC_OQS_ASSERT(OQS_SIG_sign(m_pImplementation->getSIG(), signature.data(), &signatureLength, bytes.data(), bytes.size(),
                                   key.data()),
                      "Failed to sign the message!");

        return signature;
    }

    bool Dilithium3::verify(const ByteView key, const SignatureType &signature,
                            const ByteView bytes) const
    {
        return OQS_SIG_verify(m_pImplementation->getSIG(), bytes.data(), bytes.size(), signature.data(),
                              signature.size(), key.data()) == OQS_SUCCESS;
    }
} // namespace HiveCom
