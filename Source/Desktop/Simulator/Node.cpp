#include "Node.hpp"
#include "NetworkGrid.hpp"

#include "Core/Base64.hpp"
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
        // Generate the Kyber key.
        m_kyberKey = m_kyber.generateKey();

        // Setup the encoded message to be sent to the peers.
        const auto encodedMessage = ToString(Base64(m_kyberKey.getPublicKey()).encode());

        // Share the keys with the connections.
        for (const auto connection : m_connections)
        {
            m_pNetworkGrid->sendMessage(
                std::make_shared<Message>(m_identifier, connection, encodedMessage, MessageFlag::Discovery),
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
        HC_LOG_INFO("Message received! Message: {}", message->getMessage());
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
            const auto content = message->getMessage();
            const auto duration = message->getTravelTime();
            const auto seconds = duration / 1'000'000.0f;

            HC_LOG_INFO("Discovery received! Message: {}", content);
            HC_LOG_INFO("Travel time: {}ns ({}ms)", duration, seconds);

            // Notify that the message was received.
            message->received();

            // Decode the incoming bytes and encapsulate the key.
            const auto decodedBytes = Base64(ToBytes(content)).decode();
            const auto [secret, ciphertext] = m_kyber.encapsulate(decodedBytes);
            const auto encodedCiphertext = ToString(Base64(ToView(ciphertext)).encode());

            // Send the acknowledgement packet.
            handleRouting(message->createAcknowledgementPacket());

            // Send the authorization data.
            handleRouting(
                std::make_shared<Message>(m_identifier, messageSource, encodedCiphertext, MessageFlag::Authorization));

            // Store the connection key.
            m_connectionKeys[std::string(messageSource)] =
                AES256(AES256Key(secret, HiveCom::ToFixedBytes("0123456789012345"), HiveCom::ToBytes("Hello World")));
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
            const auto seconds = duration / 1'000'000.0f;

            HC_LOG_INFO("Authorization received! Message: {}", content);
            HC_LOG_INFO("Travel time: {}ns ({}ms)", duration, seconds);

            // Notify that the message was received.
            message->received();

            // Send the acknowledgement packet.
            handleRouting(message->createAcknowledgementPacket());

            // Decode the incoming bytes and decapsulate.
            const auto decodedBytes = Base64(ToBytes(content)).decode();
            const auto secret =
                m_kyber.decapsulate(m_kyberKey.getPrivateKey(), ToFixedBytes<Kyber768::CiphertextSize>(decodedBytes));

            // Store the connection key.
            m_connectionKeys[std::string(messageSource)] =
                AES256(AES256Key(secret, HiveCom::ToFixedBytes("0123456789012345"), HiveCom::ToBytes("Hello World")));
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
} // namespace HiveCom
