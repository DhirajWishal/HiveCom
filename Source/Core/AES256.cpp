#include "AES256.hpp"

#include <algorithm>
#include <iostream>

#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/evp.h>

namespace HiveCom
{
    void AES256::encrypt(const Bytes &bytes, Byte padding /*= Byte()*/)
    {
        // Create the pContext.
        EVP_CIPHER_CTX *pContext = nullptr;
        if (!(pContext = EVP_CIPHER_CTX_new()))
            validate(0);

        // Initialize teh encryption operation.
        validate(EVP_EncryptInit_ex(pContext, EVP_aes_256_gcm(), nullptr, nullptr, nullptr));

        // Set the IV size.
        validate(EVP_CIPHER_CTX_ctrl(pContext, EVP_CTRL_GCM_SET_IVLEN, KeyAES256::IVSize, nullptr));

        // Initialize the key and initialization vector.
        validate(EVP_EncryptInit_ex(pContext, nullptr, nullptr, m_key.getKey().data(), m_key.getIV().data()));

        /// Provide the authentication data.
        int length = 0;
        if (m_key.getAuthentication().size() > 0)
        {
            validate(EVP_EncryptUpdate(pContext, nullptr, &length, m_key.getAuthentication().data(),
                                       m_key.getAuthentication().size()));
        }

        // Encrypt the data.
        for (const auto block : splitBytes(bytes, padding))
        {
            Bytes ciphertext = Bytes(BlockSize, padding);
            validate(EVP_EncryptUpdate(pContext, ciphertext.data(), &length, block.data(), block.size()));

            // Append the ciphertext.
            m_cipherText += ciphertext;
        }

        // Finalize the encryption.
        Bytes ciphertext = Bytes(BlockSize, padding);
        validate(EVP_EncryptFinal_ex(pContext, ciphertext.data(), &length));

        if (length > 0)
            m_cipherText += ciphertext;

        // Get the GCM tag.
        FixedBytes<16> tag;
        validate(EVP_CIPHER_CTX_ctrl(pContext, EVP_CTRL_GCM_GET_TAG, tag.size(), tag.data()));

        // Free the pContext.
        EVP_CIPHER_CTX_free(pContext);
    }

    Bytes AES256::decrypt(Byte padding /*= Byte()*/) const
    {
        // Crate the pContext.
        EVP_CIPHER_CTX *pContext = nullptr;
        if (!(pContext = EVP_CIPHER_CTX_new()))
            validate(0);

        // Initialize the decryption operation.
        validate(EVP_DecryptInit_ex(pContext, EVP_aes_256_gcm(), nullptr, nullptr, nullptr));

        // Initialize the initializing vector.
        validate(EVP_CIPHER_CTX_ctrl(pContext, EVP_CTRL_GCM_SET_IVLEN, KeyAES256::IVSize, nullptr));

        // Set the key and initialization vector.
        validate(EVP_DecryptInit_ex(pContext, nullptr, nullptr, m_key.getKey().data(), m_key.getIV().data()));

        // Provide the authentication data.
        int length = 0;
        if (m_key.getAuthentication().size())
        {
            validate(EVP_DecryptUpdate(pContext, nullptr, &length, m_key.getAuthentication().data(),
                                       m_key.getAuthentication().size()));
        }

        // Decrypt the data.
        Bytes plaintext;
        for (const auto block : splitBytes(m_cipherText, padding))
        {
            Bytes plaintextBlock(BlockSize, padding);
            validate(EVP_DecryptUpdate(pContext, plaintextBlock.data(), &length, block.data(), block.size()));

            plaintext += plaintextBlock;
        }

        // Finalize the decryption.
        Bytes plaintextBlock(BlockSize, padding);
        EVP_DecryptFinal_ex(pContext, plaintextBlock.data(), &length);

        if (length > 0)
            plaintext += plaintextBlock;

        // Get the tag value.
        FixedBytes<16> tag;
        validate(EVP_CIPHER_CTX_ctrl(pContext, EVP_CTRL_GCM_SET_TAG, tag.size(), tag.data()));

        // Free the previously created pContext.
        EVP_CIPHER_CTX_free(pContext);

        // Remove padding and return.
        removePadding(plaintext, padding);
        return plaintext;
    }

    void AES256::validate(int value) const
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

    std::vector<AES256::ByteBlock> AES256::splitBytes(ByteView bytes, Byte padding) const
    {
        std::size_t offset = 0;
        const auto requiredBlocks = static_cast<std::size_t>(std::ceil(bytes.size() / static_cast<float>(BlockSize)));
        std::vector<ByteBlock> blocks(requiredBlocks);
        for (auto &block : blocks)
        {
            const auto remainingBytes = bytes.size() - offset;
            const auto copySize = remainingBytes > BlockSize ? BlockSize : remainingBytes;
            const auto subString = bytes.substr(offset, copySize);

            // Copy the data.
            std::copy_n(subString.begin(), copySize, block.begin());

            // Add padding if necessary.
            std::fill_n(block.data() + copySize, BlockSize - copySize, padding);

            offset += copySize;
        }

        return blocks;
    }

    void AES256::removePadding(Bytes &bytes, Byte padding) const
    {
        for (auto i = bytes.size() - 1; i >= 0; i--)
        {
            if (bytes[i] != padding)
            {
                bytes.erase(i + 1);
                return;
            }
        }
    }
} // namespace HiveCom