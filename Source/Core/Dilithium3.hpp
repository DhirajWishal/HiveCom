#pragma once

#include "Dilithium3Key.hpp"

namespace HiveCom
{
    /// @brief Dilithium-3 class.
    /// This class uses the Dilithium-3 algorithm for digital signatures.
    class Dilithium3 final
    {
      private:
        /// @brief Implementation forward declaration.
        class Implementation;

      public:
        static constexpr auto SignatureSize = 3293; // Size taken from `OQS_SIG_dilithium_3_length_signature`.

        using SignatureType = FixedBytes<SignatureSize>;

      public:
        /// @brief Constructor.
        Dilithium3();

        /// @brief Destructor.
        ~Dilithium3();

        /// @brief Create a new Dilithium3 key.
        /// @return The key pair.
        [[nodiscard]] Dilithium3Key generateKey();

        /// @brief Sign the incoming bytes with a generated private key.
        /// @param key The private key to sign with.
        /// @param bytes The bytes to sign.
        /// @return The signature.
        [[nodiscard]] SignatureType sign(ByteView key, ByteView bytes);

        /// @brief Verify a signature.
        /// @param key The public key to verify with.
        /// @param signature The signature to verify.
        /// @param bytes The bytes to verify.
        [[nodiscard]] bool verify(ByteView key, const SignatureType &signature,
                                           ByteView bytes);

      private:
        Implementation *m_pImplementation = nullptr;
    };
} // namespace HiveCom
