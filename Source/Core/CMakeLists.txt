# Set the project name.
project(
	Core
)

# Set the project sources.
set(
	SOURCES

	"AES256.cpp"
	"AES256.hpp"
    "Key.hpp"
    "AES256Key.hpp"
    "Types.hpp"
    "Initializer.cpp"
	"Initializer.hpp"
    "Base64.cpp"
	"Base64.hpp"
    "Kyber768.cpp"
	"Kyber768.hpp"
	"Kyber768Key.hpp"
	"Logging.hpp"
    "Certificate.cpp"
	"Certificate.hpp"
	"SHA256.hpp"
    "Dilithium3.cpp"
    "Dilithium3.hpp"
    "Dilithium3Key.hpp"
)

# Setup the library information.
add_library(
	Core 
	STATIC

	${SOURCES}
)

# Setup OpenSSL library.
set(OPENSSL_LIBRARY OpenSSL::SSL)

if (DEFINED OPENSSL_SSL_LIBRARIES)
    message(STATUS "OpenSSL SSL libraries found.")

elseif (DEFINED OPENSSL_CRYPTO_LIBRARIES)
    message(STATUS "OpenSSL crypto libraries found.")
    set(OPENSSL_LIBRARY OpenSSL::Crypto)

elseif (DEFINED OPENSSL_APPLINK_SOURCE)
    message(STATUS "OpenSSL applink libraries found.")
    set(OPENSSL_LIBRARY OpenSSL::applink)

else ()
    message(FATAL_ERROR "No valid OpenSSL library found!")
endif()

# Setup the link libraries.
target_link_libraries(
    Core 
    
    PRIVATE oqs
    PRIVATE ${OPENSSL_LIBRARY}
)

# Setup include directories.
target_include_directories(
    Core 
    
    PRIVATE ${LIBOQS_INCLUDE_DIR}
    PRIVATE ${OPENSSL_INCLUDE_DIR}
    PUBLIC ${SPDLOG_INCLUDE_DIR}
)
