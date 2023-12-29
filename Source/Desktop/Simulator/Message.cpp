#include "Message.hpp"

#include <chrono>

#define CURRENT_TIME                                                                                                   \
    std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()

namespace HiveCom
{
    Message::Message(std::string source, std::string destination, std::string message,
                     const MessageFlag flag /*= MessageFlag::Message*/)
        : m_latch(1), m_source(std::move(source)), m_destination(std::move(destination)), m_message(std::move(message)),
          m_timestamp(CURRENT_TIME), m_flag(flag)
    {
    }

    Message::Message(std::string source, std::string destination, const MessageFlag flag)
        : m_latch(1), m_source(std::move(source)), m_destination(std::move(destination)), m_timestamp(CURRENT_TIME),
          m_flag(flag)
    {
    }

    std::shared_ptr<Message> Message::createAcknowledgementPacket() const
    {
        const auto ptr = std::make_shared<Message>(m_destination, m_source, MessageFlag::Acknowledgement);
        ptr->m_timestamp = m_timestamp;

        return ptr;
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
    }

    void Message::wait() const
    {
        m_latch.wait();
    }
} // namespace HiveCom
