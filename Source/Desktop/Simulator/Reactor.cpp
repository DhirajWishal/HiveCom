#include "Reactor.hpp"

namespace HiveCom
{
    Reactor::Reactor() : m_worker([this] { workerMethod(); })
    {
        m_startLatch.wait();
    }

    Reactor::~Reactor()
    {
        m_bShouldRun = false;
        m_conditionVariable.notify_one();
        m_worker.join();
    }

    void Reactor::execute(TaskType task)
    {
        auto const locker = std::scoped_lock(m_mutex);
        m_tasks.emplace_back(std::move(task));
        m_conditionVariable.notify_one();
    }

    void Reactor::workerMethod()
    {
        // Notify that the worker thread has started.
        m_startLatch.count_down();

        // Run all the tasks till we're about to exit.
        while (m_bShouldRun || !m_tasks.empty())
        {
            auto lock = std::unique_lock(m_mutex);
            m_conditionVariable.wait(lock, [this] { return !m_tasks.empty() || m_bShouldRun == false; });

            // Run any remaining tasks if there are any.
            if (!m_tasks.empty())
            {
                // Get the next task and unlock the lock.
                auto const task = m_tasks.front();
                m_tasks.pop_front();
                lock.unlock();

                // Run the task.
                task();

                // Lock it back again and run.
                lock.lock();
            }
        }
    }
} // namespace HiveCom