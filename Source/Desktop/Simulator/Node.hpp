#pragma once

#include "Reactor.hpp"

#include <string>
#include <vector>

/// @brief Macro to quickly convert serial number literals to IDs.
#define GENERATE_ID(serial_number) #serial_number

namespace HiveCom
{
    /// @brief Forward declaration of the network grid.
    class NetworkGrid;

    /// @brief Message structure.
    /// This contains information about the source, destination and the information that is being
    /// communicated.
    struct Message final
    {
        /// @brief Default constructor.
        Message() = default;

        /// @brief Explicit constructor.
        /// @param source The source of the message.
        /// @param destination The destination of the message.
        /// @param message The message to be sent.
        explicit Message(std::string_view source, std::string_view destination, std::string_view message)
            : m_source(source.data()), m_destination(destination.data()), m_message(message.data()) {}

        std::string m_source;
        std::string m_destination;
        std::string m_message;
    };

    /// @brief Node class.
    /// This class contains information about a single node in a communications network.
    class Node final
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

        /// @brief Handle a message that is received by the current node.
        /// @param message The message that has been received.
        void onMessageReceived(const Message &message);

    private:
        Reactor m_reactor;
        std::string m_identifier;
        std::vector<std::string> m_connections;

        NetworkGrid *m_pNetworkGrid = nullptr;
    };
}
