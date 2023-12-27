#include "NetworkGrid.hpp"

#include <cassert>
#include <string_view>

namespace HiveCom
{
    Connection::Connection(std::string source, const std::string &connections) : m_source(std::move(source))
    {
        for (const auto character : connections)
        {
            std::string string;
            string.push_back(character);
            m_connections.emplace_back(std::move(string));
        }
    }

    void NetworkGrid::sendMessage(const MessagePtr &message, const std::string_view hop)
    {
        assert(!m_nodeMap.empty());
        m_nodeMap[hop.data()]->sendMessage(message);
    }
} // namespace HiveCom
