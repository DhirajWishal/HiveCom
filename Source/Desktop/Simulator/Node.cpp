#include "Node.hpp"
#include "NetworkGrid.hpp"

#include <iostream>

namespace HiveCom
{
    void Node::onMessageReceived(const Message &message)
    {
        m_reactor.execute([this, message]
                          { handleMessage(message); });
    }

    void Node::handleMessage(const Message &message)
    {
        // Check if this is the required destination.
        if (message.m_destination == m_identifier)
        {
            // TODO: Notify the network grid that the message was received.
            std::cout << "Message received! Message: " << message.m_message << std::endl;
            std::cout << "Travel time: " << message.getTravelTime() << std::endl;
        }
        else
        {
            // If not check if the destination is in the connection list.
            for (const auto connection : m_connections)
            {
                if (message.m_destination == connection)
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
