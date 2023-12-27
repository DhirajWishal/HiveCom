#pragma once

#include "AES256Key.hpp"

#include <type_traits>
#include <vector>

namespace HiveCom
{
    /// @brief AES256 class.
    /// This class is used to perform AES256 encryption and decryption on a block of data.
    class AES256 final
    {
      public:
        static constexpr auto BlockSize = 256 / 8;

        using ByteBlock = FixedBytes<BlockSize>;

      public:
        /// @brief Default constructor.
        AES256() = default;

        /// @brief Explicit constructor.
        /// @param key The key to encrypt/ decrypt the data with.
        explicit AES256(const AES256Key &key) : m_key(key)
        {
        }

        /// @brief Set the key.
        /// @param key The key to set.
        void setKey(const AES256Key &key)
        {
            m_key = key;
        }

        /// @brief Encrypt a block of bytes.
        /// This will encrypt the data and store internally as ciphertext.
        /// @param bytes The input bytes.
        /// @param padding The padding which will be added when encrypting. Default is `\0`.
        void encrypt(const Bytes &bytes, Byte padding = Byte());

        /// @brief Decrypt all the cipher text once.
        /// @param padding The padding which will be removed once decrypted. Default is `\0`.
        [[nodiscard]] Bytes decrypt(Byte padding = Byte()) const;

        /// @brief Update the internally stored ciphertext.
        /// @param ciphertext The ciphertext to set.
        void setCiphertext(ByteView ciphertext);

        /// @brief Get the generated ciphertext.
        /// @return The ciphertext view.
        [[nodiscard]] ByteView getCiphertext() const
        {
            return m_cipherText;
        }

      private:
        /// @brief Validate a cryptographic operation.
        /// @param value The return value of the operation.
        void validate(int value) const;

        /// @brief Split bytes to a vector of bytes.
        /// Add padding if necessary.
        /// @param bytes The bytes to split.
        /// @param padding The padding to add.
        /// @return The byte blocks.
        [[nodiscard]] std::vector<ByteBlock> splitBytes(ByteView bytes, Byte padding) const;

        /// @brief Remove padding in bytes.
        /// This will remove paddings in the end of the bytes.
        /// @param bytes The bytes to remove the padding in.
        /// @param padding The padding byte.
        void removePadding(Bytes &bytes, Byte padding) const;

      private:
        AES256Key m_key;
        Bytes m_cipherText;
    };
} // namespace HiveCom
