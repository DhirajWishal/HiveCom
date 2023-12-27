#pragma once

#include "Dilithium3.hpp"
#include "Kyber768Key.hpp"

#include <cstdint>
#include <string_view>
#include <vector>

namespace HiveCom
{
    /// @brief Certificate class.
    /// This class stores the public key and other information to authenticate another communicating party.
    ///
    /// Certificate format:
    /// Version             + `\n`
    /// Serial number       + `\n`
    /// Issuer name         + `\n`
    /// Time stamp          + `\n`
    /// Public key          + `\n`
    /// Issuer signature    + `\n`
    ///
    /// Note that everything is in parsable string characters.
    class Certificate final
    {
      public:
        static constexpr auto ValidityPeriod = 6; // 6 months.

        using PublicKeyType = typename Kyber768Key::PublicKeyType;

      public:
        /// @brief Explicit constructor.
        /// This will automatically generate the issuer ID.
        /// @param version The version of the certificate.
        /// @param serial The serial number.
        /// @param publicKey The public key to be stored.
        /// @param issuerName The issuer name.
        /// @param privateKey The private key of the issuer.
        /// @param tool Signing tool.
        explicit Certificate(uint32_t version, const std::string &serial, const ByteView &publicKey,
                             const std::string &issuerName, const ByteView &privateKey, Dilithium3 &tool);

        /// @brief Explicit constructor.
        /// This will extract all the necessary information from the certificate.
        /// @param certificate The certificate data.
        /// @param publicKey The issuer public key.
        /// @param tool The dilithium-3 tool to verify.
        explicit Certificate(std::string_view certificate, const ByteView &publicKey, Dilithium3 &tool);

        /// @brief Get the stored full certificate.
        /// @return The generated full certificate.
        [[nodiscard]] std::string_view getCertificate() const
        {
            return m_certificate;
        }

        /// @brief Get the stored version.
        /// @return The version.
        [[nodiscard]] uint32_t getVersion() const
        {
            return m_version;
        }

        /// @brief Get the stored serial.
        /// @return The serial.
        [[nodiscard]] std::string_view getSerial() const
        {
            return m_serial;
        }

        /// @brief Get the stored issuer name.
        /// @return The issuer name.
        [[nodiscard]] std::string_view getIssuerName() const
        {
            return m_issuerName;
        }

        /// @brief Get the stored timestamp.
        /// @return The timestamp.
        [[nodiscard]] std::string_view getTimestamp() const
        {
            return m_timestamp;
        }

        /// @brief Get the stored public key.
        /// @return The public key.
        [[nodiscard]] const PublicKeyType &getPublicKey() const
        {
            return m_publicKey;
        }

        /// @brief Check if the certificate is valid or not.
        /// @return True if the certificate is valid. Else false.
        [[nodiscard]] bool isValid() const
        {
            return m_isValid;
        }

      private:
        /// @brief Split the certificate into multiple sub-strings by the `\n` character.
        /// @param certificate The certificate to split.
        /// @return The split string vector.
        [[nodiscard]] std::vector<std::string> splitCertificate(std::string_view certificate) const;

        /// @brief Check if the certificate's period is valid.
        /// @param timestamp The certificate generation timestamp.
        /// @return True if the time period is valid, else false.s
        [[nodiscard]] bool isPeriodValid(std::string_view timestamp) const;

      private:
        PublicKeyType m_publicKey;

        std::string m_certificate;

        std::string m_serial;
        std::string m_issuerName;
        std::string m_timestamp;

        uint32_t m_version;

        bool m_isValid = true;
    };
} // namespace HiveCom
