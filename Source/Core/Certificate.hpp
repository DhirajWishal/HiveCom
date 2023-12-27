#pragma once

#include "Dilithium3.hpp"
#include "Kyber768Key.hpp"
#include "SHA256.hpp"

#include <cstdint>

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
        using DigestType = typename SHA256::DigestType;

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

        /// @brief Get the stored certificate.
        /// @return The generated certificate.
        [[nodiscard]] std::string_view getCertificate() const
        {
            return m_certificate;
        }

      private:
        std::string m_certificate;
    };
} // namespace HiveCom
