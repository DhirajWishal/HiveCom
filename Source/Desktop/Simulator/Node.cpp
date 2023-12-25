#include "Node.hpp"
#include "NetworkGrid.hpp"

#include <iostream>

namespace HiveCom
{
    void Node::onMessageReceived(const MessagePtr& message)
    {
        m_reactor.execute([this, &message]() mutable
                          { handleMessage(message); });
    }

    void Node::handleMessage(const MessagePtr& message)
    {
        // Check if this is the required destination.
        if (message->getDestination() == m_identifier)
        {
            const auto duration = message->getTravelTime();
            const auto seconds = duration / 1'000'000.0f;

            // TODO: Notify the network grid that the message was received.
            std::cout << "Message received! Message: " << message->getMessage() << std::endl;
            std::cout << "Travel time: " << duration << "ns (" << seconds << "ms)" << std::endl;

            // Notify that the message was received.
            message->received();

            auto response = Message(m_identifier, message->getDestination());
        }
        else
        {
            // If not check if the destination is in the connection list.
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
    }
} // namespace HiveCom
