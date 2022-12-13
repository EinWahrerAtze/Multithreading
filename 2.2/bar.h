//
//  bar.h
//  6.2.2 v.2
//
//  Created by Denis Fedorov on 11.12.2022.
//

#pragma once
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>
#include "colour.h"

class Bar
{
public:
	friend class Multibar;
	Bar(std::size_t id, std::size_t lenght);
	Bar(std::size_t id, std::size_t lenght, Code code);
	void update(std::size_t value);
private:
	void set_progress(std::size_t progress);
	void output(std::ostream & os);
private:
	Colour _colour;
	std::mutex _mutex;
	std::once_flag _flag;
	std::atomic<bool> _done;
	std::thread::id _thread_id;
	const unsigned char _arrow;
	const unsigned char _filler;
	const unsigned char _remainder;
	std::size_t _progress;
	const std::size_t _id;
	const std::size_t _width;
	const std::size_t _lenght;
	std::chrono::seconds _duration;
};
