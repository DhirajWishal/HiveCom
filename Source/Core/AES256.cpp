#include "AES256.hpp"

#include <algorithm>

#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/evp.h>

namespace HiveCom
{
    void AES256::encrypt(const Bytes &bytes, Byte padding /*= Byte()*/)
    {
        int len = 0, ciphertext_len = 0;

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
        if (m_key.getAuthentication().size() > 0)
        {
            validate(EVP_EncryptUpdate(pContext, nullptr, &len, m_key.getAuthentication().data(),
                                       m_key.getAuthentication().size()));
        }

        // Encrypt the data.
        for (const auto block : splitBytes(bytes, padding))
        {
            Bytes ciphertext = Bytes(BlockSize, padding);
            validate(EVP_EncryptUpdate(pContext, ciphertext.data(), &len, block.data(), block.size()));
            ciphertext_len = len;

            // Finalize the encryption.
            validate(EVP_EncryptFinal_ex(pContext, ciphertext.data() + len, &len));
            ciphertext_len += len;

            // Append the ciphertext.
            m_cipherText += ciphertext;
        }

        // Get the GCM tag.
        FixedBytes<16> tag;
        validate(EVP_CIPHER_CTX_ctrl(pContext, EVP_CTRL_GCM_GET_TAG, tag.size(), tag.data()));

        // Free the pContext.
        EVP_CIPHER_CTX_free(pContext);
    }

    Bytes AES256::decrypt(Byte padding /*= Byte()*/) const
    {
        int len = 0, plaintext_len = 0, ret;

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
        if (m_key.getAuthentication().size())
        {
            validate(EVP_DecryptUpdate(pContext, nullptr, &len, m_key.getAuthentication().data(),
                                       m_key.getAuthentication().size()));
        }

        // Decrypt the data.
        Bytes plaintext;
        for (const auto block : splitBytes(m_cipherText, padding))
        {
            Bytes plaintextBlock(BlockSize, padding);
            validate(EVP_DecryptUpdate(pContext, plaintextBlock.data(), &len, block.data(), block.size()));

            plaintext_len = len;
            plaintext += plaintextBlock;
        }

        // Set the tag value.
        FixedBytes<16> tag;
        validate(EVP_CIPHER_CTX_ctrl(pContext, EVP_CTRL_GCM_SET_TAG, tag.size(), tag.data()));

        // Finalize the decryption.
        ret = EVP_DecryptFinal_ex(pContext, plaintext.data() + len, &len);

        // Free the previously created pContext.
        EVP_CIPHER_CTX_free(pContext);

        // Remove padding and return.
        plaintext.erase(plaintext.find_first_of(padding));
        return plaintext;
    }

    void AES256::validate(int value) const
    {
        // Skip if the value is 1.
        if (value == 1)
            return;

        unsigned long errCode = 0;

        printf("An error occurred\n");
        while ((errCode = ERR_get_error()))
        {
            char *err = ERR_error_string(errCode, NULL);
            printf("%s\n", err);
        }

        abort();
    }

    std::vector<AES256::ByteBlock> AES256::splitBytes(ByteView bytes, Byte padding) const
    {
        // Check if the incoming bytes are less than a block size.
        if (bytes.size() < BlockSize)
        {
            ByteBlock block;
            block.fill(padding);

            // Copy the data.
            std::copy_n(bytes.begin(), bytes.size(), block.begin());

            return {block};
        }

        // Else split and add them to multiple blocks.
        std::size_t offset = 0;
        std::vector<ByteBlock> blocks(bytes.size() / BlockSize);
        for (auto &block : blocks)
        {
            const auto copySize = bytes.size() - offset > BlockSize ? BlockSize : bytes.size();
            const auto subString = bytes.substr(offset, copySize);

            // Copy the data.
            std::copy_n(subString.begin(), copySize, block.begin());

            // Add padding if necessary.
            std::fill_n(block.data() + copySize, BlockSize - copySize, padding);

            offset += copySize;
        }

        return blocks;
    }
} // namespace HiveCom