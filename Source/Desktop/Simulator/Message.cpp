#include "Message.hpp"

#include <chrono>

#define CURRENT_TIME                                                                                                   \
    std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()

namespace HiveCom
{
    Message::Message(const std::string& source, const std::string& destination, const std::string& message,
                     MessageFlag flag /*= MessageFlag::Message*/)
        : m_latch(0), m_source(source.data()), m_destination(destination.data()), m_message(message.data()),
          m_timestamp(CURRENT_TIME), m_flag(flag)
    {
    }

    Message::Message(const std::string& source, const std::string& destination, MessageFlag flag)
        : m_latch(0), m_source(source.data()), m_destination(destination.data()), m_timestamp(CURRENT_TIME),
          m_flag(flag)
    {
    }

    void Message::setMessage(std::string_view message)
    {
        m_message = message.data();
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
