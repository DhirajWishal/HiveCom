#include <iostream>

#include "Simulator/NetworkGrid.hpp"
#include "Simulator/RandomizedRouterNode.hpp"

#include "../Core/AES256.hpp"
#include "../Core/Base64.hpp"

void printBytes(HiveCom::ByteView view)
{
    for (const auto byte : view)
        std::cout << static_cast<int>(byte);
}

void TestEncryption()
{
    // AES256 test.
    HiveCom::KeyAES256::KeyType key = HiveCom::ToFixedBytes("01234567890123456789012345678901");
    HiveCom::KeyAES256::IVType iv = HiveCom::ToFixedBytes("0123456789012345");
    HiveCom::KeyAES256::AuthType auth = {};

    const auto aesKey = HiveCom::KeyAES256(key, iv, HiveCom::ToBytes("Hello World"));
    auto encryption = HiveCom::AES256(aesKey);
    encryption.encrypt(HiveCom::ToBytes("Hello World"));

    std::cout << "Encrypted bytes: ";
    printBytes(encryption.getCiphertext());
    std::cout << std::endl;

    const auto plaintext = encryption.decrypt();
    std::cout << "Decrypted bytes: " << HiveCom::ToString(plaintext) << std::endl;

    // Base64 test.
    const auto decoded = HiveCom::Base64(plaintext);
    std::cout << "Encoded bytes: ";
    printBytes(decoded.encode());
    std::cout << std::endl;

    const auto encoded = HiveCom::Base64(decoded.encode());
    std::cout << "Decoded bytes: " << HiveCom::ToString(encoded.decode()) << std::endl;
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
