//
//  main.cpp
//  6.3.2
//
//  Created by Denis Fedorov on 22.01.2023.
//

#include <future>
#include <vector>
#include <iostream>
#include <algorithm>

namespace reinvent::algo
{
	template <typename Iterator, typename Function>
	void for_each(Iterator begin, Iterator end, Function function)
	{
		const auto length {std::distance(begin, end)};
		if (!length)
		{
			return;
		}
		const uint32_t min_per_thread {10};
		if (length < (2 * min_per_thread))
		{
			std::for_each(begin, end, function);
		}
		else
		{
			const Iterator middle {begin + length / 2};
			std::future<void> first_half {std::async(&reinvent::algo::for_each<Iterator, Function>, begin, middle, function)};
			reinvent::algo::for_each(middle, end, function);
			first_half.get();
		}
	}
}

int main(int argc, const char * argv[])
{
	std::vector v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
                   21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40};

	auto func {[](int & n)
		{
			n *= 2;
		}};

	reinvent::algo::for_each(v.begin(), v.end(), func);
	for (auto n : v)
	{
		std::cout << n << ' ';
	}
	std::cout << '\n';
	return 0;
}