#include "ThreadPool.h"
#include <iostream>
ThreadPool::ThreadPool(size_t numThreads)
{
	Start(numThreads);
}

ThreadPool::~ThreadPool()
{
	Stop();
}

void ThreadPool::Start(size_t numThreads)
{
	for (auto i = 0; i < numThreads; i++)
	{
		m_ThreadVector.emplace_back([=] {
			while (true)
			{
				Task task;
				{
					std::unique_lock<std::mutex> lock{ m_EventMutex };

					m_EventVariable.wait(lock, [=] {return m_IsStopping || !m_Tasks.empty(); });

					if (m_IsStopping && m_Tasks.empty())
					{
						break;
					}

					task = std::move(m_Tasks.front());
					m_Tasks.pop();
				}
				task();
			}
			});
	}
}

void ThreadPool::Stop()
{
	{
		std::unique_lock<std::mutex> lock{ m_EventMutex };
		m_IsStopping = true;
	}

	m_EventVariable.notify_all();

	for (std::thread& thread : m_ThreadVector)
	{
		thread.join();
	}
}