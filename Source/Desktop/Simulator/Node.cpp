#include "Node.hpp"
#include "NetworkGrid.hpp"

#include <iostream>

namespace HiveCom
{
    void Node::sendMessage(const MessagePtr &message)
    {
        m_reactor.execute([this, &message]() mutable { handleMessage(message); });
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
        std::cout << "Message received! Message: " << message->getMessage() << std::endl;
        std::cout << "Travel time: " << duration << "ns (" << seconds << "ms)" << std::endl;

        // Notify that the message was received.
        message->received();

        // Reply if this isn't an acknowledgement.
        if (shouldAck)
        {
            auto response = Message(m_identifier, message->getDestination());
        }
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
        // TODO: Handle proper discovery.
        handleMessageAccepted(message, true);
    }

    void Node::onAuthorizationReceived(const MessagePtr &message)
    {
        // TODO: Handle proper authorization.
        handleMessageAccepted(message, false);
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
