#include "Node.hpp"
#include "NetworkGrid.hpp"

#include "Core/Base64.hpp"
#include "Core/CertificateAuthority.hpp"
#include "Core/Logging.hpp"

#include <memory>
#include <sstream>

namespace HiveCom
{
    Node::Node(std::string identifier, const std::vector<std::string> &connections, NetworkGrid *pGrid)
        : m_identifier(std::move(identifier)), m_connections(connections), m_pNetworkGrid(pGrid)
    {
        m_reactor.execute([this]() mutable { initialize(); });
    }

    void Node::sendMessage(const MessagePtr &message)
    {
        m_reactor.execute([this, message]() mutable { handleMessage(message); });
    }

    void Node::initialize()
    {
        // Generate the certificate and keys.
        m_kyberKey = m_kyber.generateKey();
        m_certificate = CertificateAuthority::Instance().createCertificate(m_kyberKey.getPublicKey());
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
        }
    }

    void Node::handleMessageAccepted(const MessagePtr &message, bool shouldAck)
    {
        const auto duration = message->getTravelTime();
        const auto seconds = static_cast<double>(duration) / 1'000'000.0;

        HC_LOG_INFO("Message received! Message: {}", decryptMessage(message->getMessage(), message->getSource()));
        HC_LOG_INFO("Travel time: {}ns ({}ms)", duration, seconds);

        // Notify that the message was received.
        message->received();

        // Reply if this isn't an acknowledgement.
        if (shouldAck)
            handleRouting(message->createAcknowledgementPacket());
    }

    Message *Node::handleRouting(const MessagePtr &message)
    {
        // Check if the destination is in the connection list.
        for (const auto &connection : m_connections)
        {
            if (message->getDestination() == connection)
            {
                m_pNetworkGrid->sendMessage(message, connection);
                return message.get();
            }
        }

        // If not, route the message through the network.
        route(message);

        return message.get();
    }

    void Node::onAcknowledgementReceived(const MessagePtr &message)
    {
        // Check if this is the required destination.
        if (message->getDestination() == m_identifier)
            message->received();

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
            const auto seconds = static_cast<double>(duration) / 1'000'000.0;

            HC_LOG_INFO("Discovery packet received from {}", message->getSource().data());
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
                const auto encodedCertificate = ToString(Base64(ToBytes(m_certificate.getCertificate())).encode());
                const auto response = std::string(encodedCertificate) + "\n" + encodedCiphertext;

                // Send the authorization data.
                handleRouting(std::make_shared<Message>(m_identifier, messageSource.data(), response,
                                                        MessageFlag::Authorization));

                // Store the connection key.
                m_connectionKeys[messageSource.data()] =
                    AES256(AES256Key(secret, ToFixedBytes("0123456789012345"), ToBytes("Hello World")));
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
            const auto content = message->getMessage();
            const auto duration = message->getTravelTime();
            const auto seconds = static_cast<double>(duration) / 1'000'000.0;

            HC_LOG_INFO("Authorization packet received from {}", message->getSource().data());
            HC_LOG_INFO("Travel time: {}ns ({}ms)", duration, seconds);

            // Notify that the message was received.
            message->received();

            // Send the acknowledgement packet.
            handleRouting(message->createAcknowledgementPacket());

            // Split the content into certificate + ciphertext.
            const auto splits = SplitContent(content);
            if (splits.size() < 2)
            {
                HC_LOG_FATAL("Invalid authorization packet!");
                return;
            }

            const auto certificate = ToString(Base64(ToBytes(splits[0])).decode());
            const auto &ciphertext = splits[1];

            // Validate the certificate.
            const auto senderCertificate = CertificateAuthority::Instance().decodeCertificate(certificate);
            if (senderCertificate.isValid())
            {
                // Decode the incoming bytes and decapsulate.
                const auto decodedBytes = Base64(ToBytes(ciphertext)).decode();
                const auto secret = m_kyber.decapsulate(m_kyberKey.getPrivateKey(),
                                                        ToFixedBytes<Kyber768::CiphertextSize>(decodedBytes));

                // Store the connection key.
                m_connectionKeys[messageSource.data()] =
                    AES256(AES256Key(secret, ToFixedBytes("0123456789012345"), ToBytes("Hello World")));

                // Send the pending messages.
                if (m_pendingMessages.contains(messageSource.data()))
                {
                    for (const auto &pendingMessage : m_pendingMessages[messageSource.data()])
                    {
                        // Encrypt the message and send.
                        pendingMessage->setMessage(encryptMessage(pendingMessage->getMessage(), messageSource));
                        handleRouting(pendingMessage);
                    }

                    m_pendingMessages.erase(messageSource.data());
                }
            }
            else
            {
                handleRouting(message->createAcknowledgementPacket());
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
        // If the message source is the current node, check if we have set up a secure connection.
        if (message->getSource() == m_identifier && !m_connectionKeys.contains(message->getDestination().data()))
        {
            m_pNetworkGrid->sendMessage(std::make_shared<Message>(m_identifier, message->getDestination().data(),
                                                                  m_certificate.getCertificate().data(),
                                                                  MessageFlag::Discovery),
                                        message->getDestination().data());

            m_pendingMessages[message->getDestination().data()].emplace_back(message);
        }
        // Check if this is the required destination.
        else if (message->getDestination() == m_identifier)
        {
            handleMessageAccepted(message, true);
        }
        else
        {
            handleRouting(message);
        }
    }

    std::vector<std::string> Node::SplitContent(std::string_view content)
    {
        std::string line;
        std::vector<std::string> splits;
        std::istringstream stream{content.data()};

        while (std::getline(stream, line))
            splits.emplace_back(std::move(line));

        return splits;
    }

    std::string Node::encryptMessage(const std::string_view message, const std::string_view destination)
    {
        if (m_connectionKeys.contains(destination.data()))
        {
            auto &engine = m_connectionKeys[destination.data()];
            engine.encrypt(ToBytes(message));
            return ToString(Base64(engine.getCiphertext()).encode());
        }

        return message.data();
    }

    std::string Node::decryptMessage(const std::string_view message, const std::string_view destination)
    {
        if (m_connectionKeys.contains(destination.data()))
        {
            auto &engine = m_connectionKeys[destination.data()];
            engine.setCiphertext(Base64(ToBytes(message)).decode());
            return ToString(engine.decrypt());
        }

        return message.data();
    }
} // namespace HiveCom
