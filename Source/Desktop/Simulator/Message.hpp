#pragma once

#include <string>

namespace HiveCom
{
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
        explicit Message(std::string_view source, std::string_view destination, std::string_view message);

        /// @brief Find the travel time taken by the packet to get to the destination.
        /// This does not validate the destination, it'll give the time from the creation to this function call.
        /// @return The time duration.
        [[nodiscard]] uint64_t getTravelTime() const;

        std::string m_source;
        std::string m_destination;
        std::string m_message;

    private:
        uint64_t m_timestamp = 0;
    };
} // namespace HiveCom
