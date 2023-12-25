#pragma once

#include <latch>
#include <memory>
#include <string>
#include <string_view>

namespace HiveCom
{
    /// @brief Message flags enum.
    /// This enum specifies the message flags used by each message.
    enum class MessageFlag : uint8_t
    {
        /// @brief Acknowledgement flag.
        /// This means the message content can be ignored.
        Acknowledgement,

        /// @brief Discovery flag.
        /// This message contains the certificate of the sender.
        Discovery,

        /// @brief Authorization flag.
        /// This message contains the receivers certificate and is used as an
        /// acknowledgement.
        Authorization,

        /// @brief Message flag.
        /// This flag means the message contains usual data that should be
        /// transferred.
        Message,
    };

    /// @brief Message class.
    /// This contains information about the source, destination and the information
    /// that is being communicated.
    class Message final
    {
      public:
        /// @brief Explicit constructor.
        /// @param source The source of the message.
        /// @param destination The destination of the message.
        /// @param message The message to be sent.
        explicit Message(std::string_view source, std::string_view destination, std::string_view message);

        /// @brief Explicit constructor.
        /// This will be an acknowledgement packet (although can be overridden).
        /// @param source The source of the message.
        /// @param destination The destination of the message.
        /// @param flags The flag to set. Default is `MessageFlag::Acknowledgement`.
        explicit Message(std::string_view source, std::string_view destination,
                         MessageFlag flag = MessageFlag::Acknowledgement);

        /// @brief Get the source of the message.
        /// @return Source string view.
        [[nodiscard]] std::string_view getSource() const
        {
            return m_source;
        }

        /// @brief Get the destination of the message.
        /// @return Destination string view.
        [[nodiscard]] std::string_view getDestination() const
        {
            return m_destination;
        }

        /// @brief Get the message content.
        /// @return The message contents.
        [[nodiscard]] std::string_view getMessage() const
        {
            return m_message;
        }

        /// @brief Find the travel time taken by the packet to get to the destination.
        /// This does not validate the destination, it'll give the time from the
        /// creation to this function call.
        /// @return The time duration in nanoseconds.
        [[nodiscard]] uint64_t getTravelTime() const;

        /// @brief Get the message flag.
        /// @return The flag set to this message.
        [[nodiscard]] MessageFlag getFlag() const
        {
            return m_flag;
        }

        /// @brief Received function.
        /// This function is called by the node when it is at the destination.
        void received();

        /// @brief Wait function.
        /// The sender can use this method to wait till the message is received.
        void wait() const;

      private:
        std::latch m_latch;

        std::string m_source;
        std::string m_destination;
        std::string m_message;

        uint64_t m_timestamp = 0;

        MessageFlag m_flag = MessageFlag::Message;
    };

    /// @brief Message pointer type.
    using MessagePtr = std::shared_ptr<Message>;
} // namespace HiveCom
