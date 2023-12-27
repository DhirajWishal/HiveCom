#include "Certificate.hpp"
#include "Base64.hpp"
#include "Logging.hpp"

#include <chrono>
#include <sstream>

namespace HiveCom
{
    Certificate::Certificate(uint32_t version, const std::string &serial, const std::string &issuerName,
                             const ByteView &publicKey, const ByteView &privateKey, Dilithium3 &tool)
        : m_publicKey(ToFixedBytes<Kyber768Key::PublicKeySize>(publicKey)), m_serial(serial), m_issuerName(issuerName),
          m_version(version)
    {
        // Set up the raw certificate.
        std::stringstream rawCertificate;
        rawCertificate << version << "\n";
        rawCertificate << serial << "\n";
        rawCertificate << issuerName << "\n";
        rawCertificate << std::chrono::system_clock::now().time_since_epoch().count() << "\n";
        rawCertificate << ToString(Base64(publicKey).encode()) << "\n";

        const auto rawCertificateString = rawCertificate.str();
        const auto signature = tool.sign(privateKey, ToBytes(rawCertificateString));
        const auto encodedSignature = ToString(Base64(ToView(signature)).encode());
        rawCertificate << encodedSignature << "\n";

        // Finalize the certificate.
        m_certificate = rawCertificate.str();
    }

    Certificate::Certificate(std::string_view certificate, const ByteView &publicKey, Dilithium3 &tool)
        : m_certificate(certificate.data())
    {
        // Split the certificate to splits.
        const auto splits = SplitCertificate(certificate);

        // Validate the splits.
        if (splits.size() < 6)
        {
            HC_LOG_FATAL("Invalid certificate!");
            return;
        }

        // Decode the data.
        m_version = std::stoi(splits[0]);
        m_serial = splits[1];
        m_issuerName = splits[2];
        m_timestamp = splits[3];
        m_publicKey = ToFixedBytes<Kyber768Key::PublicKeySize>(Base64(ToBytes(splits[4])).decode());

        // Create the raw certificate.
        std::string rawCertificate;
        for (std::size_t i = 0; i < 5; i++)
            rawCertificate += splits[i] + "\n";

        // Verify the signature. TODO: Trusted keychain check.
        const auto signature = ToFixedBytes<Dilithium3::SignatureSize>(Base64(ToBytes(splits[5])).decode());
        m_isValid = tool.verify(publicKey, signature, ToBytes(rawCertificate)) && IsPeriodValid(m_timestamp);
    }

    std::vector<std::string> Certificate::SplitCertificate(std::string_view certificate)
    {
        std::string line;
        std::vector<std::string> splits;
        std::istringstream stream{certificate.data()};

        while (std::getline(stream, line))
            splits.emplace_back(std::move(line));

        return splits;
    }

    bool Certificate::IsPeriodValid(const std::string_view timestamp)
    {
        return std::stoull(timestamp.data()) >
               (std::chrono::system_clock::now() - std::chrono::months(ValidityPeriod)).time_since_epoch().count();
    }
} // namespace HiveCom
