//
//  menu.cpp
//  Thread 6.1.1
//
//  Created by Denis Fedorov on 25.11.2022.
//

#include <array>
#include <vector>
#include <thread>
#include <random>
#include <chrono>
#include <format>
#include <iostream>
#include <algorithm>

void calculation(const std::vector<int> & one, const std::vector<int> & two, std::vector<int> & result, std::size_t threads, std::size_t thread) noexcept
{
	std::size_t size {one.size() / threads};
	for (auto i {thread * size}; i < thread * size + size; ++i)
	{
		result.push_back(one[i] + two[i]);
	}
}

void get_random_values(std::vector<int> & vector) noexcept
{
	std::random_device seeder;
	const auto seed {seeder.entropy() ? seeder() : std::time(nullptr)};
	std::mt19937 engine {static_cast<std::mt19937::result_type>(seed)};
	std::uniform_int_distribution<int> distribution {std::numeric_limits<short>::min(), std::numeric_limits<short>::max()};
	std::generate(vector.begin(), vector.end(), [&engine, &distribution]() -> int
	{
		return distribution(engine);
	});
}

int main()
{
	std::array<std::size_t, 4> limits {1000, 10'000, 100'000, 1'000'000};
	std::array<std::size_t, 5> threads {1, 2, 4, 8, 16};
	std::vector<std::thread> vec_threads;
	std::vector<int> vec_one;
	std::vector<int> vec_two;
	std::vector<int> vec_result;

	std::cout << std::format("Hardware concurrency: {}\n\n", std::thread::hardware_concurrency());
	std::cout << std::format("{:26}{:16}{:16}{:16}{:16}\n", threads[0], threads[1], threads[2], threads[3], threads[4]);
	std::cout << std::format("{:-<95}\n", "-");
	
	for (std::size_t i {}; i < limits.size(); ++i)
	{
		std::cout << std::format("{:<15}", limits[i]);
		vec_one.resize(limits[i]);
		vec_two.resize(limits[i]);
		vec_result.resize(limits[i]);
		get_random_values(vec_one);
		get_random_values(vec_two);
		for (auto current_threads : threads)
		{
			for (std::size_t t {}; t < current_threads; ++t)
			{
				vec_threads.push_back(std::thread(calculation, std::cref(vec_one), std::cref(vec_two), std::ref(vec_result), current_threads, t));
			}
			auto begin {std::chrono::steady_clock::now()};
			for (std::size_t t {}; t < current_threads; ++t)
			{
				vec_threads[t].join();
			}
			std::chrono::duration<float> duration {std::chrono::steady_clock::now() - begin};
			std::cout << std::format("{:13.5f} s.", duration.count());
			vec_threads.clear();
			vec_result.clear();
		}
		vec_one.clear();
		vec_two.clear();
		std::cout << "\n";
	}
	return 0;
}