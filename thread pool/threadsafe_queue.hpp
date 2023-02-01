//
//  threadsafe_queue.hpp
//  thread_pool
//
//  Created by Denis Fedorov on 01.02.2023.
//

#pragma once
#include <mutex>
#include <memory>
#include <condition_variable>
#include "queue.hpp"

// Threadsafe queue modeled after mtd::queue data type reproduced in queue.hpp. To avoid any possible deadlocks, std::mutex
// is used to protect data, thus only one thread can get access to data at a time. The 'push' class function uses
// std::condition_variable to notify any waiting thread when new data available. The class has two function to get elements
// from the queue: first is simple 'pop' method which return bool value indicating whether a value was retrived or not. Second
// is 'wait_and_pop' which also returns bool and allow thread to wait till there are some data available. The call to m_interaction
// won't return until the underlying queue has at least one element. The class is exception save, not copyable and non movable.

namespace mtd
{
	template <typename T>
	class threadsafe_queue
	{
	public:
		threadsafe_queue();
		threadsafe_queue(const threadsafe_queue & other) = delete;
		threadsafe_queue(threadsafe_queue && other) noexcept = delete;
		threadsafe_queue & operator = (const threadsafe_queue & other) = delete;
		threadsafe_queue & operator = (threadsafe_queue && other) noexcept = delete;
	public:
		void push(T new_value);
		bool pop(T & value);
		bool wait_and_pop(T & value);
		bool empty() const;
		uint32_t size() const;
	private:
		mtd::queue<T> m_queue;
		mutable std::mutex m_mutex;
		std::condition_variable m_interaction;
	};

	template <typename T>
	threadsafe_queue<T>::threadsafe_queue()
	{

	}

	template <typename T>
	void threadsafe_queue<T>::push(T new_value)
	{
		std::lock_guard<std::mutex> lk(m_mutex);
		m_queue.push(std::move(new_value));
		m_interaction.notify_one();
	}

	template <typename T>
	bool threadsafe_queue<T>::pop(T & value)
	{
		std::lock_guard<std::mutex> lk(m_mutex);
		if (m_queue.empty())
		{
			return false;
		}
		value = std::move(m_queue.front());
		return true;
	}

	template <typename T>
	bool threadsafe_queue<T>::wait_and_pop(T & value)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_interaction.wait(lock, [this]()
		{
			return !m_queue.empty();
		});
		value = std::move(m_queue.front());
		return true;
	}

	template <typename T>
	bool threadsafe_queue<T>::empty() const
	{
		std::lock_guard<std::mutex> lk(m_mutex);
		return m_queue.empty();
	}

	template <typename T>
	uint32_t threadsafe_queue<T>::size() const
	{
		std::lock_guard<std::mutex> lk(m_mutex);
		return m_queue.size();
	}
}