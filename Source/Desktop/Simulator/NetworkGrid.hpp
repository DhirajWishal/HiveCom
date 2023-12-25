#pragma once

#include "Node.hpp"

#include <memory>
#include <unordered_map>

namespace HiveCom
{
    /// @brief Connection structure.
    /// This structure specifies what which nodes a given node is connected to.
    struct Connection final
    {
        /// @brief Explicit constructor.
        /// @param source The source node.
        /// @param connections The connections.
        explicit Connection(const std::string &source, const std::vector<std::string> &connections)
            : m_source(source), m_connections(connections)
        {
        }

        /// @brief Explicit constructor.
        /// @param source The source node.
        /// @param connections Connection string. This treats each character as the
        /// node ID.
        explicit Connection(const std::string &source, std::string_view connections);

        std::string m_source;
        std::vector<std::string> m_connections;
    };

    /// @brief Network grid forward declaration.
    class NetworkGrid;

    /// @brief Node factory class.
    /// This helper class is used to create new nodes of a given type.
    /// @tparam NodeType The type of node.
    template <class NodeType> struct NodeBuilder final
    {
        /// @brief Functor operator overload.
        /// This will create a new node of the given type.
        /// @param identifier The node's identifier.
        /// @param connections The node's connections.
        /// @param pGrid The grid pointer.
        /// @return The created unique node pointer.
        std::unique_ptr<NodeType> operator()(std::string_view identifier, const std::vector<std::string> &connections,
                                             NetworkGrid *pGrid)
        {
            return std::make_unique<NodeType>(identifier, connections, pGrid);
        }
    };

    /// @brief Network grid class.
    /// This class stores all the network nodes and simulates a real life network
    /// grid.
    class NetworkGrid final
    {
      public:
        /// @brief Explicit constructor.
        /// The initial node will be considered as the root node.
        /// @param connections The connections in the grid. Make sure that each source
        /// in the connections are unsique!
        template <class NodeType>
        explicit NetworkGrid(const std::vector<Connection> &connections, NodeBuilder<NodeType> builder)
        {
            // Setup the node grid.
            for (const auto &connection : connections)
                m_nodeMap[connection.m_source] = builder(connection.m_source, connection.m_connections, this);
        }

        /// @brief Get the node with the ID.
        /// @param node The node to access.
        /// @return The node pointer.
        [[nodiscard]] Node *getNode(std::string_view node)
        {
            return m_nodeMap[node.data()].get();
        }

        /// @brief Send a message through the network.
        /// @param message The message to be sent.
        /// @param hop The immediate hope that handles this message.
        void sendMessage(const MessagePtr &message, std::string_view hop);

      private:
        std::unordered_map<std::string, std::unique_ptr<Node>> m_nodeMap;
    };
} // namespace HiveCom
