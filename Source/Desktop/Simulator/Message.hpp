#pragma once

#include <string>
#include <latch>
#include <memory>

namespace HiveCom
{
    /// @brief Message flags enum.
    /// This enum specifies the message flags used by each message.
    enum class MessageFlag : uint32_t
    {
        Acknowledgement = 1 << 0,
        Message = 1 << 1,
        KeepAlive = 1 << 2,
    };

    /// @brief Bitwise OR operator overload for the message flag enum.
    /// @param lhs The left hand side argument.
    /// @param rhs The right hand side argument.
    /// @return The returning flag.
    inline MessageFlag operator|(MessageFlag lhs, MessageFlag rhs)
    {
        using MessageFlagType = std::underlying_type_t<MessageFlag>;
        return static_cast<MessageFlag>(static_cast<MessageFlagType>(lhs) | static_cast<MessageFlagType>(rhs));
    }

    /// @brief Bitwise OR assignment operator override.
    /// @param lhs The left hand side argument.
    /// @param rhs The right hand side argument.
    /// @return The returning flag reference.
    inline MessageFlag &operator|=(MessageFlag &lhs, MessageFlag rhs) { return lhs = lhs | rhs; }

    /// @brief Bitwise AND operator overload for the message flag enum.
    /// @param lhs The left hand side argument.
    /// @param rhs The right hand side argument.
    /// @return The returning flag.
    inline MessageFlag operator&(MessageFlag lhs, MessageFlag rhs)
    {
        using MessageFlagType = std::underlying_type_t<MessageFlag>;
        return static_cast<MessageFlag>(static_cast<MessageFlagType>(lhs) & static_cast<MessageFlagType>(rhs));
    }

    /// @brief Bitwise AND assignment operator override.
    /// @param lhs The left hand side argument.
    /// @param rhs The right hand side argument.
    /// @return The returning flag reference.
    inline MessageFlag &operator&=(MessageFlag &lhs, MessageFlag rhs) { return lhs = lhs & rhs; }

    /// @brief Message class.
    /// This contains information about the source, destination and the information that is being
    /// communicated.
    class Message final
    {
    public:
        /// @brief Default constructor.
        Message() = default;

        /// @brief Explicit constructor.
        /// @param source The source of the message.
        /// @param destination The destination of the message.
        /// @param message The message to be sent.
        explicit Message(std::string_view source, std::string_view destination, std::string_view message);

        /// @brief Explicit constructor.
        /// This will be an acknowledgement packet (although can be overridden).
        /// @param source The source of the message.
        /// @param destination The destination of the message.
        /// @param flags The flags to set. Default is `MessageFlag::Acknowledgement`.
        explicit Message(std::string_view source, std::string_view destination, MessageFlag flags = MessageFlag::Acknowledgement);

        /// @brief Get the source of the message.
        /// @return Source string view.
        [[nodiscard]] std::string_view getSource() const { return m_source; }

        /// @brief Get the destination of the message.
        /// @return Destination string view.
        [[nodiscard]] std::string_view getDestination() const { return m_destination; }

        /// @brief Get the message content.
        /// @return The message contents.
        [[nodiscard]] std::string_view getMessage() const { return m_message; }

        /// @brief Find the travel time taken by the packet to get to the destination.
        /// This does not validate the destination, it'll give the time from the creation to this function call.
        /// @return The time duration in nanoseconds.
        [[nodiscard]] uint64_t getTravelTime() const;

        /// @brief Get the message flags.
        /// @return The flags set to this message.
        [[nodiscard]] MessageFlag getFlags() const { return m_flags; }

        /// @brief Set flags to the internal flag component.
        /// @param flag The flag(s) to set.
        void setFlags(MessageFlag flag) { m_flags |= flag; }

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

        MessageFlag m_flags = MessageFlag::Message;
    };

    /// @brief Message pointer type.
    using MessagePtr = std::shared_ptr<Message>;
} // namespace HiveCom
