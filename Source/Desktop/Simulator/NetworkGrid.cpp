#include "NetworkGrid.hpp"

#include <cassert>
#include <string_view>

namespace HiveCom
{
    Connection::Connection(const std::string &source, std::string_view connections)
        : m_source(source)
    {
        for (const auto character : connections)
        {
            std::string string;
            string.push_back(character);
            m_connections.emplace_back(std::move(string));
        }
    }

    // NetworkGrid::NetworkGrid(const std::vector<Connection> &connections)
    // {
    //     // Setup the node grid.
    //     for (const auto &connection : connections)
    //         m_nodeMap[connection.m_source] = std::make_unique<Node>(connection.m_source, connection.m_connections, this);

    //     // Setup the root node.
    //     if (!connections.empty())
    //         m_rootNode = connections.front().m_source;
    // }

    void NetworkGrid::sendMessage(const MessagePtr& message)
    {
        assert(!m_nodeMap.empty());
        m_nodeMap[m_rootNode]->onMessageReceived(message);
    }

    void NetworkGrid::sendMessage(const MessagePtr& message, std::string_view hop)
    {
        assert(!m_nodeMap.empty());
        m_nodeMap[hop.data()]->onMessageReceived(message);
    }
} // namespace HiveCom
