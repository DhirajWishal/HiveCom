#pragma once

#include "Node.hpp"

#include <unordered_map>
#include <memory>

namespace HiveCom
{
    /// @brief Connection structure.
    /// This structure specifies what which nodes a given node is connected to.
    struct Connection final
    {
        /// @brief Explicit constructor.
        /// @param source The source node.
        /// @param connections The connections.
        explicit Connection(const std::string &source, const std::vector<std::string> &connections) : m_source(source), m_connections(connections) {}

        /// @brief Explicit constructor.
        /// @param source The source node.
        /// @param connections Connection string. This treats each character as the node ID.
        explicit Connection(const std::string &source, std::string_view connections);

        std::string m_source;
        std::vector<std::string> m_connections;
    };

    /// @brief Network grid class.
    /// This class stores all the network nodes and simulates a real life network grid.
    class NetworkGrid final
    {
    public:
        /// @brief Explicit constructor.
        /// The initial node will be considered as the root node.
        /// @param connections The connections in the grid. Make sure that each source in the connections are unique!
        explicit NetworkGrid(const std::vector<Connection> &connections);

        /// @brief Send a message through the network.
        /// @param message The message to be sent.
        void sendMessage(const Message &message);

        /// @brief Send a message through the network.
        /// @param message The message to be sent.
        /// @param hop The immediate hope that handles this message.
        void sendMessage(const Message &message, std::string_view hop);

    private:
        std::unordered_map<std::string, std::unique_ptr<Node>> m_nodeMap;
        std::string m_rootNode;
    };
} // namespace HiveCom
