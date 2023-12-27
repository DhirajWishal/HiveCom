#include "Base64.hpp"
#include "Logging.hpp"

#include <openssl/err.h>
#include <openssl/evp.h>

namespace HiveCom
{
    Bytes Base64::encode() const
    {
        // Return if we don't have data to encode.
        if (m_bytes.empty())
            return {};

        Bytes bytes(4 * ((m_bytes.size() + 2) / 3), 0);
        validate(bytes.size() == EVP_EncodeBlock(bytes.data(), m_bytes.data(), m_bytes.size()));

        return bytes;
    }

    Bytes Base64::decode() const
    {
        // Return if we don't have data to decode.
        if (m_bytes.empty())
            return {};

        Bytes bytes(3 * m_bytes.size() / 4, 0);
        validate(bytes.size() == EVP_DecodeBlock(bytes.data(), m_bytes.data(), m_bytes.size()));

        return bytes;
    }

    void Base64::validate(int value) const
    {
        // Skip if the value is 1.
        if (value == 1)
            return;

        unsigned long errCode = 0;

        HC_LOG_ERROR("An error occurred!");
        while ((errCode = ERR_get_error()))
            HC_LOG_ERROR("Error: {}", ERR_error_string(errCode, nullptr));

        abort();
    }
} // namespace HiveCom
