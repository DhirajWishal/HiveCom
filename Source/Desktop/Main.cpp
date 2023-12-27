#include <iostream>

#include "Core/Types.hpp"
#include "Simulator/NetworkGrid.hpp"
#include "Simulator/RandomizedRouterNode.hpp"

#include "Core/AES256.hpp"
#include "Core/Base64.hpp"
#include "Core/Certificate.hpp"
#include "Core/Dilithium3.hpp"
#include "Core/Kyber768.hpp"

void PrintBytes(HiveCom::ByteView view)
{
    for (const auto byte : view)
        std::cout << static_cast<int>(byte);
}

void TestAES()
{
    // constexpr auto InputData = R"(
    //     Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
    //     labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris
    //     nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate
    //     velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident,
    //     sunt in culpa qui officia deserunt mollit anim id est laborum.
    // )";

    constexpr auto InputData = R"(Hello world)";

    HiveCom::AES256Key::KeyType key = HiveCom::ToFixedBytes("01234567890123456789012345678901");
    HiveCom::AES256Key::IVType iv = HiveCom::ToFixedBytes("0123456789012345");
    HiveCom::AES256Key::AuthType auth = {};

    const auto aesKey = HiveCom::AES256Key(key, iv, HiveCom::ToBytes("Hello World"));
    auto encryption = HiveCom::AES256(aesKey);
    encryption.encrypt(HiveCom::ToBytes(InputData));

    std::cout << "Encrypted bytes: ";
    PrintBytes(encryption.getCiphertext());
    std::cout << std::endl;

    const auto plaintext = encryption.decrypt();
    std::cout << "Decrypted bytes: " << HiveCom::ToString(plaintext) << std::endl;
}

void TestBase64()
{
    const auto decoded = HiveCom::Base64(HiveCom::ToBytes("Hello world"));
    const auto encodedData = decoded.encode();
    std::cout << "Encoded bytes: " << HiveCom::ToString(encodedData) << std::endl;

    const auto encoded = HiveCom::Base64(encodedData);
    std::cout << "Decoded bytes: " << HiveCom::ToString(encoded.decode()) << std::endl;
}

void TestKyber768()
{
    HiveCom::Kyber768 kyber;
    const auto key = kyber.generateKey();
    const auto [sharedSecret, ciphertext] = kyber.encapsulate(key.getPublicKey());
    const auto ddata = kyber.decapsulate(key.getPrivateKey(), ciphertext);

    std::cout << "Generated Kyber-768 public key: " << HiveCom::ToString(HiveCom::Base64(key.getPublicKey()).encode())
              << std::endl;
    std::cout << "Generated Kyber-768 private key: " << HiveCom::ToString(HiveCom::Base64(key.getPrivateKey()).encode())
              << std::endl;

    std::cout << "Ciphertext: " << HiveCom::ToString(HiveCom::Base64(HiveCom::ToView(ciphertext)).encode())
              << std::endl;
    std::cout << "Expected shared secret: "
              << HiveCom::ToString(HiveCom::Base64(HiveCom::ToView(sharedSecret)).encode()) << std::endl;
    std::cout << "Actual shared secret:   " << HiveCom::ToString(HiveCom::Base64(HiveCom::ToView(ddata)).encode())
              << std::endl;
}

void TestDilithium3()
{
    HiveCom::Dilithium3 dilithium;
    const auto key = dilithium.generateKey();
    const auto signature = dilithium.sign(key.getPrivateKey(), HiveCom::ToBytes("Hello world"));
    const auto isValid = dilithium.verify(key.getPublicKey(), signature, HiveCom::ToBytes("Hello world"));

    std::cout << "Generated Dilithium-3 public key: " << HiveCom::ToString(HiveCom::Base64(key.getPublicKey()).encode())
              << std::endl;
    std::cout << "Generated Dilithium-3 private key: "
              << HiveCom::ToString(HiveCom::Base64(key.getPrivateKey()).encode()) << std::endl;

    std::cout << "Signature: " << HiveCom::ToString(HiveCom::Base64(HiveCom::ToView(signature)).encode()) << std::endl;
    std::cout << "Is valid: " << (isValid ? "True" : "False") << std::endl;
}

void TestCertificate()
{
    HiveCom::Dilithium3 dilithium;
    const auto authKey = dilithium.generateKey();

    HiveCom::Kyber768 kyber;
    const auto kemKey = kyber.generateKey();

    const auto certificate =
        HiveCom::Certificate(1, "0001", "HiveCom", kemKey.getPublicKey(), authKey.getPrivateKey(), dilithium);
    std::cout << "Certificate: " << std::endl << certificate.getCertificate() << std::endl;
    std::cout << "Certificate decode valid: "
              << (HiveCom::Certificate(certificate.getCertificate(), authKey.getPublicKey(), dilithium).isValid()
                      ? "True"
                      : "False")
              << std::endl;
}

void TestNetworkingSimple()
{
    auto grid = HiveCom::NetworkGrid(
        {
            HiveCom::Connection("A", "B"), // Root node.
            HiveCom::Connection("B", "A"),
        },
        HiveCom::NodeBuilder<HiveCom::RandomizedRouterNode>());

    auto message = std::make_shared<HiveCom::Message>("A", "B", "Hello world");
    grid.getNode("A")->sendMessage(message);
    message->wait();

    HiveCom::g_ActiveMessageBlock.wait();
}

void TestNetworkingComplex()
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
    TestAES();
    TestBase64();
    TestKyber768();
    TestDilithium3();
    TestCertificate();
    TestNetworkingSimple();
    // TestNetworkingComplex(); // TODO: Add this with proper routing.
}
