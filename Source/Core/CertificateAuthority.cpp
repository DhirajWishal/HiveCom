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

    std::pair<Certificate, Kyber768Key> CertificateAuthority::generateKeyPair()
    {
        const auto lock = std::scoped_lock(m_mutex);

        auto keyPair = m_kyber.generateKey();
        auto certificate = Certificate(1, "1", "HiveCom::CertificateAuthority", keyPair.getPublicKey(),
                                       m_dilithiumKey.getPrivateKey(), m_dilithium);

        return std::make_pair(certificate, keyPair);
    }

    Certificate CertificateAuthority::decodeCertificate(std::string_view certificate)
    {
        return Certificate(certificate, m_dilithiumKey.getPublicKey(), m_dilithium);
    }
} // namespace HiveCom