//
//  main.cpp
//  thread_pool
//
//  Created by Denis Fedorov on 01.02.2023.
//

#include <chrono>
#include <iostream>
#include <source_location>
#include "thread_pool.hpp"

class test
{
public:
	test() : m_count_one(), m_count_two()
	{

	}

	void test_func_one()
	{
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_location = std::source_location::current();
			std::cout << std::setw(3) << std::left << m_count_one;
			std::cout << m_location.function_name() << ' ' << std::this_thread::get_id() << '\n';
		}
		++m_count_one;
	}

	void test_func_two()
	{
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_location = std::source_location::current();
			std::cout << std::setw(3) << std::left << m_count_two;
			std::cout << m_location.function_name() << ' ' << std::this_thread::get_id() << '\n';
		}
		++m_count_two;
	}
private:
	std::mutex m_mutex;
	uint32_t m_count_one;
	uint32_t m_count_two;
	std::source_location m_location;
};

int main(int argc, const char * argv[])
{
	test t;
	constexpr int iterations {30};
	try
	{
		mtd::thread_pool pool;
		for (int i {}; i < iterations; ++i)
		{
			pool.submit([&]()
			{
				t.test_func_one();
			});
			pool.submit([&t]()
			{
				t.test_func_two();
			});
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		}
	}
	catch (...)
	{
		return -1;
	}
	return 0;
}
