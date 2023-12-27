#include "CertificateAuthority.hpp"

namespace HiveCom
{
    CertificateAuthority::CertificateAuthority()
    {
        // Generate the root sign key.
        m_dilithiumKey = m_dilithium.generateKey();
    }

    CertificateAuthority &CertificateAuthority::Instance()
    {
        static CertificateAuthority instance;
        return instance;
    }

    Certificate CertificateAuthority::createCertificate(const ByteView publicKey)
    {
        const auto lock = std::scoped_lock(m_mutex);
        return Certificate(1, "1", "HiveCom::CertificateAuthority", publicKey, m_dilithiumKey.getPrivateKey(),
                           m_dilithium);
    }

    Certificate CertificateAuthority::decodeCertificate(const std::string_view certificate)
    {
        const auto lock = std::scoped_lock(m_mutex);
        return Certificate(certificate, m_dilithiumKey.getPublicKey(), m_dilithium);
    }
} // namespace HiveCom
