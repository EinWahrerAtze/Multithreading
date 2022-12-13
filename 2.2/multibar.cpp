//
//  multibar.cpp
//  6.2.2 v.2
//
//  Created by Denis Fedorov on 11.12.2022.
//

#include "multibar.h"

Multibar::Multibar(std::size_t threads, std::size_t lenght, Speed speed) : _speed(speed), _started(false), _lenght(lenght)
{
	for (std::size_t id {}; id < threads; ++id)
	{
		_bars.push_back(std::make_unique<Bar>(id, _lenght));
		if (_speed == Speed::STATIC)
		{
			_sleeping_time.push_back(get_random_value());
		}
		_threads.push_back(std::thread([this, id]()
		{
			for (std::size_t progress {}; progress <= _lenght; ++progress)
			{
				this->update(id, progress);
				if (_speed == Speed::STATIC)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(_sleeping_time[id]));
				}
				else
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(get_random_value()));
				}
			}
		}));
	}
}

void Multibar::run()
{
	for (auto & thread : _threads)
	{
		thread.join();
	}
}

void Multibar::update(std::size_t id, std::size_t progress)
{
	_bars[id]->set_progress(progress);
	output(std::cout);
}

void Multibar::output(std::ostream & os)
{
	std::unique_lock lk {_mutex};
	std::call_once(_flag, [&os]()
	{	
		os << std::format("{}{:>10}{:>42}{:>28}{:>12}\n", "#", "id", "Progress Bar", "%", "Time, s");
	});
	if (_started)
	{
		for (std::size_t i {}; i < _bars.size(); ++i)
		{
			os << "\x1b[A";
		}
	}
	for (std::size_t i {}; i < _bars.size(); ++i)
	{
		_bars[i]->output(os);
		os << '\n';
	}
	if (!_started)
	{
		_started = true;
	}
}

std::size_t Multibar::get_random_value()
{
	std::unique_lock lk {_mutex};
	std::random_device rd;
	std::mt19937 engine {rd()};
	std::uniform_int_distribution<std::size_t> distribution {50, 750};
	return distribution(engine);
}
