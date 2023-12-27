#include "Message.hpp"

#include <chrono>

#define CURRENT_TIME                                                                                                   \
    std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()

namespace HiveCom
{
    void ActiveMessageBlock::increment()
    {
        ++m_Count;
    }

    void ActiveMessageBlock::decrement()
    {
        --m_Count;
    }

    void ActiveMessageBlock::wait() const
    {
        while (m_Count > 0)
            ;
    }

    ActiveMessageBlock g_ActiveMessageBlock;

    Message::Message(std::string source, std::string destination, std::string message,
                     const MessageFlag flag /*= MessageFlag::Message*/)
        : m_latch(1), m_source(std::move(source)), m_destination(std::move(destination)), m_message(std::move(message)),
          m_timestamp(CURRENT_TIME), m_flag(flag)
    {
        g_ActiveMessageBlock.increment();
    }

    Message::Message(std::string source, std::string destination, const MessageFlag flag)
        : m_latch(1), m_source(std::move(source)), m_destination(std::move(destination)), m_timestamp(CURRENT_TIME),
          m_flag(flag)
    {
        g_ActiveMessageBlock.increment();
    }

    void Message::setMessage(const std::string &message)
    {
        m_message = message;
    }

    uint64_t Message::getTravelTime() const
    {
        return CURRENT_TIME - m_timestamp;
    }

    void Message::received()
    {
        m_latch.count_down();
        g_ActiveMessageBlock.decrement();
    }

    void Message::wait() const
    {
        m_latch.wait();
    }
} // namespace HiveCom
