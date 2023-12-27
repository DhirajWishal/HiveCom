#include <iostream>

#include "Simulator/NetworkGrid.hpp"
#include "Simulator/RandomizedRouterNode.hpp"

#include "../Core/AES256.hpp"
#include "../Core/Base64.hpp"
#include "../Core/Kyber768.hpp"

void printBytes(HiveCom::ByteView view)
{
    for (const auto byte : view)
        std::cout << static_cast<int>(byte);
}

void TestEncryption()
{
    // constexpr auto InputData = R"(
    //     Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
    //     labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris
    //     nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate
    //     velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident,
    //     sunt in culpa qui officia deserunt mollit anim id est laborum.
    // )";

    constexpr auto InputData = R"(Hello world)";

    // AES256 test.
    HiveCom::KeyAES256::KeyType key = HiveCom::ToFixedBytes("01234567890123456789012345678901");
    HiveCom::KeyAES256::IVType iv = HiveCom::ToFixedBytes("0123456789012345");
    HiveCom::KeyAES256::AuthType auth = {};

    const auto aesKey = HiveCom::KeyAES256(key, iv, HiveCom::ToBytes("Hello World"));
    auto encryption = HiveCom::AES256(aesKey);
    encryption.encrypt(HiveCom::ToBytes(InputData));

    std::cout << "Encrypted bytes: ";
    printBytes(encryption.getCiphertext());
    std::cout << std::endl;

    const auto plaintext = encryption.decrypt();
    std::cout << "Decrypted bytes: " << HiveCom::ToString(plaintext) << std::endl;

    // Base64 test.
    const auto decoded = HiveCom::Base64(plaintext);
    const auto encodedData = decoded.encode();
    std::cout << "Encoded bytes: " << HiveCom::ToString(encodedData) << std::endl;

    const auto encoded = HiveCom::Base64(encodedData);
    std::cout << "Decoded bytes: " << HiveCom::ToString(encoded.decode()) << std::endl;

    // Kyber test.
    HiveCom::Kyber768 kyber;
    const auto kyberKey = kyber.generateKey();
    const auto [sharedSecret, ciphertext] = kyber.encapsulate(kyberKey.getPublicKey());
    const auto ddata = kyber.decapsulate(kyberKey.getPrivateKey(), ciphertext);

    std::cout << "Generated public key: " << HiveCom::ToString(HiveCom::Base64(kyberKey.getPublicKey()).encode())
              << std::endl;
    std::cout << "Generated private key: " << HiveCom::ToString(HiveCom::Base64(kyberKey.getPrivateKey()).encode())
              << std::endl;

    std::cout << "Ciphertext: " << HiveCom::ToString(HiveCom::Base64(HiveCom::ToView(ciphertext)).encode())
              << std::endl;
    std::cout << "Expected shared secret: " << HiveCom::ToString(HiveCom::Base64(HiveCom::ToView(sharedSecret)).encode())
              << std::endl;
    std::cout << "Actual shared secret:   " << HiveCom::ToString(HiveCom::Base64(HiveCom::ToView(ddata)).encode())
              << std::endl;
}

void TestNetworking()
{
    auto grid = HiveCom::NetworkGrid(
        {
            HiveCom::Connection("A", "BDNO"), // Root node.
            HiveCom::Connection("B", "ACFO"),
            HiveCom::Connection("C", "B"),
            HiveCom::Connection("D", "AE"),
            HiveCom::Connection("E", "DFP"),
            HiveCom::Connection("F", "BFG"),
            HiveCom::Connection("G", "FHI"),
            HiveCom::Connection("H", "GI"),
            HiveCom::Connection("I", "GHJKL"),
            HiveCom::Connection("J", "IK"),
            HiveCom::Connection("K", "JLM"),
            HiveCom::Connection("L", "IK"),
            HiveCom::Connection("M", "K"),
            HiveCom::Connection("N", "AO"),
            HiveCom::Connection("O", "ABN"),
            HiveCom::Connection("P", "E"),
        },
        HiveCom::NodeBuilder<HiveCom::RandomizedRouterNode>());

    auto message = std::make_shared<HiveCom::Message>("N", "M", "Hello world");
    grid.getNode("N")->sendMessage(message);
    message->wait();
}

int main()
{
    TestEncryption();
    TestNetworking();
}
