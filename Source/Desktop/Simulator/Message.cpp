#include "Message.hpp"

#include <chrono>

#define CURRENT_TIME                                                                                                   \
    std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()

namespace HiveCom
{
    Message::Message(std::string_view source, std::string_view destination, std::string_view message)
        : m_latch(0), m_source(source.data()), m_destination(destination.data()), m_message(message.data()),
          m_timestamp(CURRENT_TIME)
    {
    }

    Message::Message(std::string_view source, std::string_view destination, MessageFlag flag)
        : m_latch(0), m_source(source.data()), m_destination(destination.data()), m_timestamp(CURRENT_TIME),
          m_flag(flag)
    {
    }

    uint64_t Message::getTravelTime() const
    {
        return CURRENT_TIME - m_timestamp;
    }

    void Message::received()
    {
        m_latch.count_down();
    }

    void Message::wait() const
    {
        m_latch.wait();
    }
} // namespace HiveCom
