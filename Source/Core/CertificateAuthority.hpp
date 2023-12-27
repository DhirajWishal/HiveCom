#pragma once

#include "Certificate.hpp"
#include "Kyber768.hpp"

#include <mutex>

namespace HiveCom
{
    /// @brief Certificate authority class.
    /// This is a singleton class that created certificates on demand.
    class CertificateAuthority final
    {
        /// @brief Constructor.
        CertificateAuthority();

        /// @brief Default destructor.
        ~CertificateAuthority() = default;

      public:
        CertificateAuthority(const CertificateAuthority &) = delete;
        CertificateAuthority(CertificateAuthority &&) = delete;

        CertificateAuthority &operator=(const CertificateAuthority &) = delete;
        CertificateAuthority &operator=(CertificateAuthority &&) = delete;

        /// @brief Get the CA instance.
        /// @return The instance reference.
        static CertificateAuthority &Instance();

      public:
        /// @brief Generate a new key pair for KEM.
        /// @return The certificate and key pair.
        [[nodiscard]] std::pair<Certificate, Kyber768Key> generateKeyPair();

        /// @brief Decode the certificate.
        /// @param certificate The certificate data.
        /// @return The decoded certificate.
        [[nodiscard]] Certificate decodeCertificate(std::string_view certificate);

        /// @brief Get the kyber engine used for encapsulation/ decapsulation.
        /// @return The kyber engine reference.
        [[nodiscard]] Kyber768 &getKyberEngine()
        {
            return m_kyber;
        }

      private:
        std::mutex m_mutex;

        Kyber768 m_kyber;

        Dilithium3 m_dilithium;
        Dilithium3Key m_dilithiumKey;
    };
} // namespace HiveCom