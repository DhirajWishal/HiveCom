#include "AES256.hpp"

#include <oqs/aes.h>

namespace HiveCom
{
    void encrypt()
    {
        // TODO: Remove this and make a proper API.
        uint8_t *bytes = nullptr;
        uint8_t *input = nullptr;
        uint8_t *key = nullptr;
        OQS_AES128_ECB_enc(input, strlen("Hello world"), key, bytes);
    }
} // namespace HiveCom