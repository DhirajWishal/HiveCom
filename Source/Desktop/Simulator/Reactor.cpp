#include "Reactor.hpp"

namespace HiveCom
{
	Reactor::Reactor()
		: m_worker([this]
				   { workerMethod(); })
	{
	}

	void Reactor::execute(TaskType task)
	{
		const auto locker = std::scoped_lock(m_mutex);
		m_tasks.emplace_back(std::move(task));
	}

	void Reactor::workerMethod()
	{
		while (m_bShouldRun)
		{
			auto lock = std::unique_lock<std::mutex>(m_mutex);
			m_conditionVariable.wait(lock, [this]
									 { return m_tasks.empty() || m_bShouldRun == false; });

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
}