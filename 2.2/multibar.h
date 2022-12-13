//
//  multibar.h
//  6.2.2 v.2
//
//  Created by Denis Fedorov on 11.12.2022.
//

#pragma once
#include <mutex>
#include <memory>
#include <vector>
#include <thread>
#include <atomic>
#include <random>
#include <format>
#include <iostream>
#include "bar.h"

enum class Speed
{
	STATIC,
	DYNAMIC
};

class Multibar
{
public:
	Multibar(std::size_t threads, std::size_t lenght, Speed speed);
	void run();
private:
	void update(std::size_t id, std::size_t progress);
	void output(std::ostream & os);
	std::size_t get_random_value();
private:
	Speed _speed;
	std::mutex _mutex;
	std::once_flag _flag;
	std::atomic<bool> _started;
	const std::size_t _lenght;
	std::vector<std::thread> _threads;
	std::vector<std::size_t> _sleeping_time;
	std::vector<std::unique_ptr<Bar>> _bars;
};
