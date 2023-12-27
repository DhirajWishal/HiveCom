#include "Certificate.hpp"
#include "Base64.hpp"

#include <chrono>
#include <sstream>

namespace HiveCom
{
    Certificate::Certificate(uint32_t version, const std::string &serial, const ByteView &publicKey,
                             const std::string &issuerName, const ByteView &privateKey, Dilithium3 &tool)
    {
        // Setup the raw certificate.
        std::stringstream rawCertificate;
        rawCertificate << version << "\n";
        rawCertificate << serial << "\n";
        rawCertificate << issuerName << "\n";
        rawCertificate << std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::system_clock::now())
                       << "\n";
        rawCertificate << ToString(Base64(publicKey).encode()) << "\n";

        const auto signature = tool.sign(privateKey, ToBytes(rawCertificate.str()));
        rawCertificate << ToString(Base64(ToView(signature)).encode()) << "\n";

        // Finalize the certificate.
        m_certificate = rawCertificate.str();
    }
} // namespace HiveCom
