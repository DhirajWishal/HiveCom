#include "Base64.hpp"

#include <openssl/err.h>
#include <openssl/evp.h>

#include <iostream>

namespace HiveCom
{
    Bytes Base64::encode() const
    {
        // Return if we don't have data to encode.
        if (m_bytes.empty())
            return {};

        const auto expectedResult = 4 * ((m_bytes.size() + 2) / 3);
        Bytes bytes(expectedResult, 0);
        const auto result = EVP_EncodeBlock(bytes.data(), m_bytes.data(), m_bytes.size());
        if (bytes.size() != result)
            return {};

        return bytes;
    }

    Bytes Base64::decode() const
    {
        // Return if we don't have data to decode.
        if (m_bytes.empty())
            return {};

        const auto expectedResult = 3 * m_bytes.size() / 4;
        Bytes bytes(expectedResult, 0);
        const auto result = EVP_DecodeBlock(bytes.data(), m_bytes.data(), m_bytes.size());
        if (bytes.size() != result)
            return {};

        return bytes;
    }

    void Base64::validate(int value) const
    {
        // Skip if the value is 1.
        if (value == 1)
            return;

        unsigned long errCode = 0;

        std::cerr << "An error occurred!" << std::endl;
        while ((errCode = ERR_get_error()))
            std::cerr << ERR_error_string(errCode, nullptr) << std::endl;

        abort();
    }
} // namespace HiveCom
