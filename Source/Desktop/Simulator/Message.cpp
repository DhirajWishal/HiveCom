#include "Message.hpp"

#include <chrono>

namespace HiveCom
{
    Message::Message(std::string_view source, std::string_view destination, std::string_view message)
        : m_source(source.data()), m_destination(destination.data()), m_message(message.data()), m_timestamp(std::chrono::steady_clock::now().time_since_epoch().count())
    {
    }

    uint64_t Message::getTravelTime() const
    {
        return std::chrono::steady_clock::now().time_since_epoch().count() - m_timestamp;
    }
} // namespace HiveCom
