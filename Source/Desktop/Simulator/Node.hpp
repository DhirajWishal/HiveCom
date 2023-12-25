#pragma once

#include "Reactor.hpp"
#include "Message.hpp"

#include <vector>

/// @brief Macro to quickly convert serial number literals to IDs.
#define GENERATE_ID(serial_number) #serial_number

namespace HiveCom
{
    /// @brief Forward declaration of the network grid.
    class NetworkGrid;

    /// @brief Node class.
    /// This abstract class contains information about a single node in a communications network.
    class Node
    {
    public:
        /// @brief Explicit constructor.
        /// @param identifier The node identifier.
        /// @param connections The connections of this node.
        /// @param pGrid The network grid pointer to which this node is attached to.
        explicit Node(std::string_view identifier, const std::vector<std::string> &connections, NetworkGrid *pGrid)
            : m_identifier(identifier.data()), m_connections(connections), m_pNetworkGrid(pGrid) {}

        /// @brief Get the node identifier.
        /// @return The identifier string view.
        [[nodiscard]] std::string_view getIdentifier() const { return m_identifier; }

        /// @brief Get all the connections of this node.
        /// @return The connections.
        [[nodiscard]] const std::vector<std::string> &getConnections() const { return m_connections; }

        void onHandshake() {}
        void onKeepAlive() {}

        /// @brief Handle a message that is received by the current node.
        /// @param message The message that has been received.
        void onMessageReceived(const MessagePtr& message);

    protected:
        /// @brief Pure virtual route method.
        /// This method is used to route the incoming message through the network to the destination.
        /// It is invoked if the current node is not the destination or if the destination is not within
        /// the connections list.
        /// @param message The message to be routed.
        virtual void route(const MessagePtr& message) = 0;

    private:
        /// @brief Handle the incoming message in another thread.
        /// @param message The message to handle.
        void handleMessage(const MessagePtr& message);

    private:
        Reactor m_reactor;

    protected:
        std::string m_identifier;
        std::vector<std::string> m_connections;

        NetworkGrid *m_pNetworkGrid = nullptr;
    };
}
