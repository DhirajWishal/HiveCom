#pragma once

#include "Kyber768Key.hpp"
#include "SHA256.hpp"

#include <cstdint>

namespace HiveCom
{
    /// @brief Certificate class.
    /// This class stores the public key and other information to authenticate another communicating party.
    class Certificate final
    {
      public:
        using PublicKeyType = typename Kyber768Key::PublicKeyType;
        using PrivateKeyType = typename Kyber768Key::PrivateKeyType;
        using DigestType = typename SHA256::DigestType;

      public:
        /// @brief Explicit constructor.
        /// This will automatically generate the issuer ID.
        /// @param version The version of the certificate.
        /// @param serial The serial number.
        /// @param publicKey The public key to be stored.
        /// @param issuerName The issuer name.
        /// @param privateKey The private key of the issuer.
        explicit Certificate(uint32_t version, const std::string &serial, const PublicKeyType &publicKey,
                             const std::string &issuerName, const PrivateKeyType &privateKey);

      private:
        PublicKeyType m_publicKey;
        DigestType m_issuerID;

        std::string m_serial;
        std::string m_issuerName;

        uint32_t m_version;
    };
} // namespace HiveCom