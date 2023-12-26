#include "Initializer.hpp"

#include <openssl/err.h>
#include <openssl/evp.h>

namespace HiveCom
{
    OpenSSLInitializer::OpenSSLInitializer()
    {
        OpenSSL_add_all_algorithms();
        ERR_load_crypto_strings();
    }

    OpenSSLInitializer::~OpenSSLInitializer()
    {
        ERR_free_strings();
    }
} // namespace HiveCom

/// @brief Initializer definition.
HiveCom::OpenSSLInitializer _initializer = HiveCom::OpenSSLInitializer();
