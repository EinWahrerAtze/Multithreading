//
//  main.cpp
//  6.3.1
//
//  Created by Denis Fedorov on 21.01.2023.
//

#include <span>
#include <array>
#include <vector>
#include <future>
#include <iostream>
#include <functional>

namespace reinvent::sort
{
	enum class order
	{
		INCREASING,
		DECREASING
	};
	
	template <typename T>
	std::promise<uint32_t> find_index(std::span<T> container, std::function<bool(T, T)> predicate, uint32_t i)
	{
		uint32_t index {i};
		for (uint32_t j {index + 1}; j < container.size(); ++j)
		{
			if (predicate(container[j], container[index]))
			{
				index = j;
			}
		}
		std::promise<uint32_t> p;
		p.set_value(index);
		return p;
	}
	
	template <typename T>
	void selection_sort(std::span<T> container, order order)
	{
		std::function<bool(T, T)> predicate;
		switch (order)
		{
			case order::INCREASING:
			{
				predicate = [](const T & lhs, const T & rhs) -> bool
				{
					return lhs < rhs;
				};
				break;
			}
			case order::DECREASING:
			{
				predicate = [](const T & lhs, const T & rhs) -> bool
				{
					return lhs > rhs;
				};
				break;
			}
		}

		for (uint32_t i {}; i < container.size() - 1; ++i)
		{
			std::promise<uint32_t> promise {find_index(container, predicate, i)};
			std::future<uint32_t> future {promise.get_future()};
			uint32_t index {future.get()};
			if (index != i)
			{
				std::swap(container[i], container[index]);
			}
		}
	}
}

template <typename T>
void print_container(std::span<T> array)
{
	for (const auto & i : array)
	{
		std::cout << i << ' ';
	}
	std::cout << '\n';
}

int main(int argc, const char * argv[])
{
//	std::array<float, 10> array {0.0f, -8.8f, 8.4f, 0.3f, 0.5f, 1.2f, -0.5f, 2.3f, 9.9f, 4.5f};
	std::array array {0.0f, -8.8f, 8.4f, 0.3f, 0.5f, 1.2f, -0.5f, 2.3f, 9.9f, 4.5f};
//	std::vector<int> vector {4, -7, 1, 9, 0, 6, 5, 4, 3, -2, 11, 9};
	std::vector vector {4, -7, 1, 9, 0, 6, 5, 4, 3, -2, 11, 9};
	double c_array[] {0.0, 5.4, 6.34, 12.21, 0.4, 5.55};
	
	std::cout << "array before sorting:\n";
	print_container<float>(array);
	std::cout << "vector before sorting:\n";
	print_container<int>(vector);
	std::cout << "c-array before sorting:\n";
	print_container<double>(c_array);
	
	reinvent::sort::selection_sort<float>(array, reinvent::sort::order::INCREASING);
	reinvent::sort::selection_sort<int>(vector, reinvent::sort::order::DECREASING);
	reinvent::sort::selection_sort<double>(c_array, reinvent::sort::order::INCREASING);
	
	std::cout << '\n';
	std::cout << "array after sorting:\n";
	print_container<float>(array);
	std::cout << "vector after sorting:\n";
	print_container<int>(vector);
	std::cout << "c-array after sorting:\n";
	print_container<double>(c_array);
	return 0;
}