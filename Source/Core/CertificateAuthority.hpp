#pragma once

#include "Certificate.hpp"

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
        /// @brief Generate a new certificate
        /// @param publicKey The public key to generate the certificate to.
        /// @return The certificate.
        [[nodiscard]] Certificate createCertificate(ByteView publicKey);

        /// @brief Decode the certificate.
        /// @param certificate The certificate data.
        /// @return The decoded certificate.
        [[nodiscard]] Certificate decodeCertificate(std::string_view certificate);

      private:
        std::mutex m_mutex;

        Dilithium3 m_dilithium;
        Dilithium3Key m_dilithiumKey;
    };
} // namespace HiveCom