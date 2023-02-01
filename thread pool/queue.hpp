//
//  queue.hpp
//  queue
//
//  Created by Denis Fedorov on 01.02.2023.
//

#pragma once
#include <memory>
#include <stdexcept>

// Simple implementation of queue data structure based on linked list. It uses a unique pointer to first (head) node to help
// manage dynamicaly allocated memory and a raw pointer to the last (tail) node. The 'node' structure contains of value itself
// and a pointer to the next node. The class has no arithmetic operators available. The 'front' method returns first element by value,
// if queue is empty, then std::runtime_error is thrown. The 'pop' class function removes first element from the queue, and the 'push'
// function adds data to the tail. The destructor removes any nodes left with the unique pointer and thus ensures there are no memory leaks.

namespace mtd 
{
	template <typename T>
	class queue
	{
	public:
		queue();
		~queue();
		queue(const queue & other);
		queue(queue && other) noexcept;
		queue & operator = (const queue & other);
		queue & operator = (queue && other) noexcept;
	public:
		T front();
		void pop();
		void push(T new_value);
		bool empty() const;
		uint32_t size() const;
	private:
		struct node
		{
			node(T new_data) : data(std::move(new_data)), next() {}
			T data;
			node * next;
		};
		uint32_t m_size;
		std::unique_ptr<node> m_head;
		node * m_tail;
	};

	template <typename T>
	queue<T>::queue() : m_size(), m_tail()
	{

	}

	template <typename T>
	queue<T>::~queue()
	{
		// destroy any nodes left in the queue
		while (m_head)
		{
			m_head.reset(m_head->next);
		}
	}

	template <typename T>
	queue<T>::queue(const queue<T> & other)
	{
		// first value will be added as m_head pointer, so the size == 1
		m_size = 1;
		m_head = std::make_unique<node>(other.m_head->data);
		// m_size variable and m_tail pointer will be corrected in the 'push' method
		m_tail = m_head.get();
		// temporary pointer tp get values of other queue
		node * other_ptr {other.m_head->next};
		while (other_ptr)
		{
			this->push(other_ptr->data);
			other_ptr = other_ptr->next;
		}
	}

	template <typename T>
	queue<T>::queue(queue<T> && other) noexcept
	{
		m_size = other.m_size;
		m_head = std::move(other.m_head);
		m_tail = other.m_tail;
	}

	template <typename T>
	queue<T> & queue<T>::operator = (const queue<T> & other)
	{
		if (this == &other)
		{
			return *this;
		}
		else
		{
			// same algorithm as copy constructor
			m_size = 1;
			m_head = std::make_unique<node>(other.m_head->data);
			m_tail = m_head.get();
			node * other_ptr {other.m_head->next};
			while (other_ptr)
			{
				this->push(other_ptr->data);
				other_ptr = other_ptr->next;
			}
			return *this;
		}
	}

	template <typename T>
	queue<T> & queue<T>::operator = (queue<T> && other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}
		else
		{
			m_size = other.m_size;
			m_head = std::move(other.m_head);
			m_tail = other.m_tail;
			return *this;
		}
	}

	template <typename T>
	T queue<T>::front()
	{
		if (!m_head)
		{
			throw std::runtime_error {"queue is empty!"};
		}
		T data {std::move(m_head->data)};
		// remove first element from the queue
		this->pop();
		if (!m_head)
		{
			m_tail = nullptr;
		}
		--m_size;
		return data;
	}

	template <typename T>
	void queue<T>::pop()
	{
		m_head.reset(m_head->next);
	}

	template <typename T>
	void queue<T>::push(T new_data)
	{
		if (!m_head)
		{
			m_head = std::make_unique<node>(std::move(new_data));
			m_tail = m_head.get();
		}
		else
		{
			node * new_tail {new node(std::move(new_data))};
			m_tail->next = new_tail;
			m_tail = new_tail;
		}
		++m_size;
	}

	template <typename T>
	bool queue<T>::empty() const
	{
		return m_size == 0;
	}

	template <typename T>
	uint32_t queue<T>::size() const
	{
		return m_size;
	}
}
