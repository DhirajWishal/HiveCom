#pragma once

#include <thread>
#include <functional>
#include <list>
#include <atomic>
#include <condition_variable>
#include <mutex>

namespace HiveCom
{
	/// @brief Reactor class.
	/// This class contains a single worker thread and will remain blocked while a task is assigned to it.
	class Reactor final
	{
		using TaskType = std::function<void()>;

	public:
		/// @brief Default constructor.
		Reactor();

		/// @brief Default destructor.
		~Reactor();

		/// @brief Execute a new task.
		/// @param task The task to be executed asynchronously.
		void execute(TaskType task);

	private:
		/// @brief  Worker method function.
		/// This function is called by the worker thread and will execute the tasks accordingly.
		void workerMethod();

	private:
		std::thread m_worker;
		std::list<TaskType> m_tasks;

		std::mutex m_mutex;

		std::atomic_bool m_bShouldRun = true;
		std::condition_variable m_conditionVariable;
	};
}