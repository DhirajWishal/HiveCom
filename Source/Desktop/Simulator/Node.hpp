#pragma once

#include "Message.hpp"
#include "Reactor.hpp"

#include "Core/AES256.hpp"
#include "Core/Certificate.hpp"
#include "Core/Kyber768.hpp"

#include <unordered_map>
#include <vector>

/// @brief Macro to quickly convert serial number literals to IDs.
#define GENERATE_ID(serial_number) #serial_number

namespace HiveCom
{
    /// @brief Forward declaration of the network grid.
    class NetworkGrid;

    /// @brief Node class.
    /// This abstract class contains information about a single node in a
    /// communications network.
    class Node
    {
      public:
        /// @brief Explicit constructor.
        /// @param identifier The node identifier.
        /// @param connections The connections of this node.
        /// @param pGrid The network grid pointer to which this node is attached to.
        explicit Node(std::string_view identifier, const std::vector<std::string> &connections, NetworkGrid *pGrid);

        /// @brief Virtual destructor.
        virtual ~Node() = default;

        /// @brief Get the node identifier.
        /// @return The identifier string view.
        [[nodiscard]] std::string_view getIdentifier() const
        {
            return m_identifier;
        }

        /// @brief Get all the connections of this node.
        /// @return The connections.
        [[nodiscard]] const std::vector<std::string> &getConnections() const
        {
            return m_connections;
        }

        /// @brief Send a message through the network.
        /// @param message The message to be sent.
        void sendMessage(const MessagePtr &message);

      protected:
        /// @brief Pure virtual route method.
        /// This method is used to route the incoming message through the network to
        /// the destination. It is invoked if the current node is not the destination
        /// or if the destination is not within the connections list.
        /// @param message The message to be routed.
        virtual void route(const MessagePtr &message) = 0;

      private:
        /// @brief Initialize the connections in another thread.
        /// This is to simulate how an actual node would setup connections.
        void initialize();

        /// @brief Handle the incoming message in another thread.
        /// @param message The message to handle.
        void handleMessage(const MessagePtr &message);

        /// @brief Handle the message if the destination is the current node.
        /// @param message The message to handle.
        /// @param shouldAck If we should send an acknowledgement packet.
        void handleMessageAccepted(const MessagePtr &message, bool shouldAck);

        /// @brief Handle routing and forward the message to a connected node.
        /// @param message The message to handle.
        void handleRouting(const MessagePtr &message);

        /// @brief Handle an acknowledgement message received by the current node.
        /// @param message The message to handle.
        void onAcknowledgementReceived(const MessagePtr &message);

        /// @brief Handle a discovery message received by the current node.
        /// @param message The message to handle.
        void onDiscoveryReceived(const MessagePtr &message);

        /// @brief Handle an authorization message received by the current node.
        /// @param message The message to handle.
        void onAuthorizationReceived(const MessagePtr &message);

        /// @brief Handle a message that is received by the current node.
        /// @param message The message that has been received.
        void onMessageReceived(const MessagePtr &message);

      private:
        /// @brief Split the content into multiple sub-strings by the `\n` character.
        /// @param content The content to split.
        /// @return The split string vector.
        [[nodiscard]] std::vector<std::string> splitContent(std::string_view content) const;

        /// @brief Encrypt a message.
        /// @param message The message to encrypt. 
        /// @param destination Used to find the key.
        /// @return The encrypted message.
        [[nodiscard]] std::string encryptMessage(std::string_view message, std::string_view destination);

        /// @brief Decrypt a message.
        /// @param message The message to decrypt.
        /// @param destination Used to find the key.
        /// @return The decrypted message.
        [[nodiscard]] std::string decryptMessage(std::string_view message, std::string_view destination);

      private:
        Reactor m_reactor;

        Kyber768 m_kyber;
        Kyber768Key m_kyberKey;
        Certificate m_certificate;

      protected:
        std::string m_identifier;
        std::vector<std::string> m_connections;
        std::unordered_map<std::string, AES256> m_connectionKeys;

        NetworkGrid *m_pNetworkGrid = nullptr;
    };
} // namespace HiveCom
