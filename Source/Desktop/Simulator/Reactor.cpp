#include "Reactor.hpp"

namespace HiveCom {
Reactor::Reactor() : m_worker([this] { workerMethod(); }) {
  m_startLatch.wait();
}

Reactor::~Reactor() {
  m_bShouldRun = false;
  m_worker.join();
}

void Reactor::execute(TaskType task) {
  const auto locker = std::scoped_lock(m_mutex);
  m_tasks.emplace_back(std::move(task));
}

void Reactor::workerMethod() {
  // Notify that the worker thread has started.
  m_startLatch.count_down();

  // Run all the tasks till we're about to exit.
  while (m_bShouldRun) {
    auto lock = std::unique_lock<std::mutex>(m_mutex);
    m_conditionVariable.wait(
        lock, [this] { return m_tasks.empty() || m_bShouldRun == false; });

    // Run any remaining tasks if there are any.
    if (!m_tasks.empty()) {
      // Get the next task and unlock the lock.
      const auto task = m_tasks.front();
      m_tasks.pop_front();
      lock.unlock();

      // Run the task.
      task();

      // Lock it back again and run.
      lock.lock();
    }
  }

  // If we need to quit, quickly finish everything.
  const auto locker = std::scoped_lock(m_mutex);
  for (const auto &task : m_tasks)
    task();
}
} // namespace HiveCom