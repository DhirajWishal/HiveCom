#include "Certificate.hpp"

namespace HiveCom
{
    Certificate::Certificate(uint32_t version, const std::string &serial, const PublicKeyType &publicKey,
                             const std::string &issuerName, const PrivateKeyType &privateKey)
        : m_publicKey(publicKey), m_serial(serial), m_issuerName(issuerName), m_version(version)
    {
    }
} // namespace HiveCom