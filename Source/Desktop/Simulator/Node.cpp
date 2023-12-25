#include "Node.hpp"
#include "NetworkGrid.hpp"

namespace HiveCom
{
    void Node::onMessageReceived(const Message &message)
    {
        // If we're not the destination, check if the destination is in our list.
        if (message.m_destination != m_identifier)
        {
            for (const auto &connection : m_connections)
            {
                if (message.m_destination == connection)
                {
                    m_pNetworkGrid->sendMessage(message, connection);
                    return;
                }
            }

            // If it's not in the network list, send it to a random person in our connection list.
            
        }
        else
        {
            // TODO: Notify the network grid that the message was received.
        }
    }
} // namespace HiveCom
