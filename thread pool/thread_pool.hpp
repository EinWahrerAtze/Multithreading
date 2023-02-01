//
//  thread_pool.hpp
//  thread_pool
//
//  Created by Denis Fedorov on 01.02.2023.
//

#pragma once
#include <atomic>
#include <thread>
#include <vector>
#include <functional>
#include "threadsafe_queue.hpp"

// This simple implementaion of thread pool based on mtd::threadsafe_queue reproduced in threadsafe_queue.hpp uses
// std::vector of worker threads. The number of worker threads returned by std::thread::hardware_concurency(). Each worked thread
// takes work off the m_queue, runs the task and then goes back to the queue for more work. The 'submit' class function wraps any
// function or callable object inside std::function<void()> instance (thus they can't return any values) and pushes it on the m_queue. 
// If starting thread fails, exception goes inside catch block and m_done bool variable set to true, so already started threads stopped
// and cleaned up. The same works with destructor. The 'work' function is just a loop waitig until m_done flag is set, pulling tasks from
// the queue and executing them. If no tasks available, std::this_thread::yield() is called. The class is neither copyable nor moveable.

namespace mtd
{
	class thread_pool
	{
	public:
		thread_pool();
		~thread_pool();
		thread_pool(const thread_pool & other) = delete;
		thread_pool(thread_pool && other) noexcept = delete;
		thread_pool & operator = (const thread_pool & other) = delete;
		thread_pool & operator = (thread_pool && other) noexcept = delete;
	public:
		template <typename F>
		void submit(F func);
	private:
		void work();
	private:
		std::atomic<bool> m_done;
		std::vector<std::thread> m_threads;
		threadsafe_queue<std::function<void()>> m_queue;
	};

	thread_pool::thread_pool() : m_done(false)
	{
		const auto threads {std::thread::hardware_concurrency()};
//		std::cout << "threads: " << threads << "\n\n";
		try
		{
			for (unsigned int i {}; i < threads; ++i)
			{
				m_threads.push_back(std::thread(&thread_pool::work, this));
			}
		}
		catch (...)
		{
			m_done = true;
			throw;
		}
	}

	thread_pool::~thread_pool()
	{
		m_done = true;
		for (auto & thread : m_threads)
		{
			if (thread.joinable())
			{
				thread.join();
			}
		}
	}

	template <typename F>
	void thread_pool::submit(F func)
	{
		m_queue.push(std::function<void()>(func));
	}

	void thread_pool::work()
	{
		while (!m_done)
		{
			std::function<void()> task;
			if (m_queue.pop(task))
			{
				task();
			}
			else
			{
				std::this_thread::yield();
//				std::this_thread::sleep_for(std::chrono::milliseconds(5000));
			}
		}
	}
}