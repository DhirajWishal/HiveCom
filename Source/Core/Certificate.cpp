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

    Certificate::Certificate(std::string_view certificate, const ByteView &publicKey, Dilithium3 &tool)
        : m_certificate(certificate.data())
    {
        // Split the certificate to splits.
        const auto splits = splitCertificate(certificate);

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
        const auto signature = ToFixedBytes<Dilithium3::SignatureSize>(Base64(ToBytes(splits[5])).decode());

        // Create the raw certificate.
        std::string rawCertificate;
        for (std::size_t i = 0; i < 5; i++)
            rawCertificate += splits[i] + "\n";

        // Verify the signature.
        m_isValid = tool.verify(publicKey, signature, ToBytes(rawCertificate)) && isPeriodValid(m_timestamp);
    }

    std::vector<std::string> Certificate::splitCertificate(std::string_view certificate) const
    {
        std::string line;
        std::vector<std::string> splits;
        std::istringstream stream{certificate.data()};

        while (std::getline(stream, line))
            splits.emplace_back(std::move(line));

        return splits;
    }

    bool Certificate::isPeriodValid(std::string_view timestamp) const
    {
#if defined __cpp_lib_chrono && __cpp_lib_chrono >= 201907L
        std::istringstream stream{timestamp.data()};

        std::string line;
        std::getline(stream, line);

        std::chrono::sys_seconds seconds;
        stream >> std::chrono::parse("%Y-%m-%dT%H:%M:%S%z", seconds);
        return (std::chrono::system_clock::now() - std::chrono::months(ValidityPeriod)) < seconds;

#else
        return true;

#endif
    }
} // namespace HiveCom
