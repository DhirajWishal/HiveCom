#include "Node.hpp"
#include "NetworkGrid.hpp"

#include "Core/Base64.hpp"
#include "Core/CertificateAuthority.hpp"
#include "Core/Logging.hpp"

#include <memory>

namespace HiveCom
{
    Node::Node(std::string_view identifier, const std::vector<std::string> &connections, NetworkGrid *pGrid)
        : m_identifier(identifier.data()), m_connections(connections), m_pNetworkGrid(pGrid)
    {
        m_reactor.execute([this]() mutable { initialize(); });
    }

    void Node::sendMessage(const MessagePtr &message)
    {
        m_reactor.execute([this, &message]() mutable { handleMessage(message); });
    }

    void Node::initialize()
    {
        // Generate the certificate and keys.
        m_kyberKey = m_kyber.generateKey();
        m_certificate = CertificateAuthority::Instance().createCertificate(m_kyberKey.getPublicKey());

        // Setup the certificate.
        const auto message = m_certificate.getCertificate();

        // Share the keys with the connections.
        for (const auto connection : m_connections)
        {
            m_pNetworkGrid->sendMessage(
                std::make_shared<Message>(m_identifier, connection, message.data(), MessageFlag::Discovery),
                connection);
        }
    }

    void Node::handleMessage(const MessagePtr &message)
    {
        switch (message->getFlag())
        {
        case MessageFlag::Acknowledgement:
            onAcknowledgementReceived(message);
            break;

        case MessageFlag::Discovery:
            onDiscoveryReceived(message);
            break;

        case MessageFlag::Authorization:
            onAuthorizationReceived(message);
            break;

        case MessageFlag::Message:
            onMessageReceived(message);
            break;

        default:
            break;
        }
    }

    void Node::handleMessageAccepted(const MessagePtr &message, bool shouldAck)
    {
        const auto duration = message->getTravelTime();
        const auto seconds = duration / 1'000'000.0f;

        // TODO: Notify the network grid that the message was received.
        HC_LOG_INFO("Message received! Message: {}", decryptMessage(message->getMessage(), message->getSource()));
        HC_LOG_INFO("Travel time: {}ns ({}ms)", duration, seconds);

        // Notify that the message was received.
        message->received();

        // Reply if this isn't an acknowledgement.
        if (shouldAck)
            handleRouting(message->createAcknowledgementPacket());
    }

    void Node::handleRouting(const MessagePtr &message)
    {
        // Check if the destination is in the connection list.
        for (const auto connection : m_connections)
        {
            if (message->getDestination() == connection)
            {
                m_pNetworkGrid->sendMessage(message, connection);
                return;
            }
        }

        // If not, route the message through the network.
        route(message);
    }

    void Node::onAcknowledgementReceived(const MessagePtr &message)
    {
        // Check if this is the required destination.
        if (message->getDestination() == m_identifier)
            handleMessageAccepted(message, false);

        else
            handleRouting(message);
    }

    void Node::onDiscoveryReceived(const MessagePtr &message)
    {
        if (message->getDestination() == m_identifier)
        {
            // Extract the important information.
            const auto messageSource = message->getSource();
            const auto certificate = message->getMessage();
            const auto duration = message->getTravelTime();
            const auto seconds = duration / 1'000'000.0f;

            HC_LOG_INFO("Discovery received! Message: {}", certificate);
            HC_LOG_INFO("Travel time: {}ns ({}ms)", duration, seconds);

            // Notify that the message was received.
            message->received();

            // Send the acknowledgement packet.
            handleRouting(message->createAcknowledgementPacket());

            // Decode the incoming bytes and encapsulate the key.
            const auto senderCertificate = CertificateAuthority::Instance().decodeCertificate(certificate);
            if (senderCertificate.isValid())
            {
                const auto [secret, ciphertext] = m_kyber.encapsulate(ToView(senderCertificate.getPublicKey()));
                const auto encodedCiphertext = ToString(Base64(ToView(ciphertext)).encode());
                const auto response = std::string(m_certificate.getCertificate()) + "\n" + encodedCiphertext;

                // Store the connection key.
                m_connectionKeys[std::string(messageSource)] = AES256(
                    AES256Key(secret, HiveCom::ToFixedBytes("0123456789012345"), HiveCom::ToBytes("Hello World")));

                // Send the authorization data.
                handleRouting(std::make_shared<Message>(m_identifier, messageSource.data(),
                                                        encryptMessage(response, messageSource),
                                                        MessageFlag::Authorization));
            }
            else
            {
                HC_LOG_FATAL("Invalid discovery packet!");
            }
        }
        else
        {
            handleRouting(message);
        }
    }

    void Node::onAuthorizationReceived(const MessagePtr &message)
    {
        if (message->getDestination() == m_identifier)
        {
            // Extract the important information.
            const auto messageSource = message->getSource();
            const auto content = decryptMessage(message->getMessage(), message->getSource());
            const auto duration = message->getTravelTime();
            const auto seconds = duration / 1'000'000.0f;

            HC_LOG_INFO("Authorization received! Message: {}", content);
            HC_LOG_INFO("Travel time: {}ns ({}ms)", duration, seconds);

            // Notify that the message was received.
            message->received();

            // Send the acknowledgement packet.
            handleRouting(message->createAcknowledgementPacket());

            // Split the content into certificate + ciphertext.
            const auto splits = splitContent(content);
            if (splits.size() < 2)
            {
                HC_LOG_FATAL("Invalid authorization packet!");
                return;
            }

            const auto certificate = splits[0];
            const auto ciphertext = splits[1];

            // Validate the certificate.
            const auto senderCertificate = CertificateAuthority::Instance().decodeCertificate(certificate);
            if (senderCertificate.isValid())
            {
                // Decode the incoming bytes and decapsulate.
                const auto decodedBytes = Base64(ToBytes(ciphertext)).decode();
                const auto secret = m_kyber.decapsulate(m_kyberKey.getPrivateKey(),
                                                        ToFixedBytes<Kyber768::CiphertextSize>(decodedBytes));

                // Store the connection key.
                m_connectionKeys[std::string(messageSource)] = AES256(
                    AES256Key(secret, HiveCom::ToFixedBytes("0123456789012345"), HiveCom::ToBytes("Hello World")));
            }
            else
            {
                HC_LOG_FATAL("Invalid authorization packet!");
            }
        }
        else
        {
            handleRouting(message);
        }
    }

    void Node::onMessageReceived(const MessagePtr &message)
    {
        // Check if this is the required destination.
        if (message->getDestination() == m_identifier)
            handleMessageAccepted(message, true);

        else
            handleRouting(message);
    }

    std::vector<std::string> Node::splitContent(std::string_view content) const
    {
        std::string line;
        std::vector<std::string> splits;
        std::istringstream stream{content.data()};

        while (std::getline(stream, line))
            splits.emplace_back(std::move(line));

        return splits;
    }

    std::string Node::encryptMessage(std::string_view message, std::string_view destination)
    {
        // TODO: Extended encryption (two disconnected nodes).
        if (m_connectionKeys.contains(destination.data()))
        {
            auto &engine = m_connectionKeys[destination.data()];
            engine.encrypt(ToBytes(message));
            return ToString(Base64(engine.getCiphertext()).encode());
        }

        return message.data();
    }

    std::string Node::decryptMessage(std::string_view message, std::string_view destination)
    {
        // TODO: Extended encryption (two disconnected nodes).
        if (m_connectionKeys.contains(destination.data()))
        {
            auto &engine = m_connectionKeys[destination.data()];
            engine.setCiphertext(ToBytes(message));
            return ToString(Base64(engine.decrypt()).encode());
        }

        return message.data();
    }
} // namespace HiveCom
