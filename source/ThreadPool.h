#include <condition_variable>
#include <functional>
#include <vector>
#include <thread>
#include <queue>
#include <future>

#pragma once
class ThreadPool
{
public:
	using Task = std::function<void()>;
	explicit ThreadPool(size_t numThreads);

	~ThreadPool();

	template<class T>
	auto Enqueue(T task)->std::future<decltype(task())>
	{
		auto wrapper = std::make_shared<std::packaged_task<decltype(task()) ()>>(std::move(task));

		{
			std::unique_lock<std::mutex> lock{ m_EventMutex };
			m_Tasks.emplace([=] {
				(*wrapper)();
				});
		}

		m_EventVariable.notify_one();
		return wrapper->get_future();
	}

	void JoinThreads()
	{
		for (std::thread& thread : m_ThreadVector)
		{
			if (thread.joinable())
			{
				m_EventVariable.notify_all();
				thread.join();
			}
		}
	}

	int GetMaxAmountOfThreads()const
	{
		return int(m_ThreadVector.size());
	}
private:
	std::vector<std::thread>m_ThreadVector;
	std::condition_variable m_EventVariable;

	std::mutex m_EventMutex;
	bool m_IsStopping = false;

	std::queue<Task>m_Tasks;

	void Start(size_t numThreads);
	void Stop();
};
