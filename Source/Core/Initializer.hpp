#pragma once

namespace HiveCom
{
    /// @brief OpenSSL initializer class.
    /// This class is used to initialize OpenSSL upon instantiation.
    class OpenSSLInitializer final
    {
      public:
        /// @brief Default constructor.
        /// This will initialize OpenSSL.
        OpenSSLInitializer();

        /// @brief Default destructor.
        /// This will free memory allocated by OpenSSL.
        ~OpenSSLInitializer();
    };
} // namespace HiveCom

/// @brief Initializer declaration.
extern HiveCom::OpenSSLInitializer initializer;
