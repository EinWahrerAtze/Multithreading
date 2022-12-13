//
//  bar.cpp
//  6.2.2 v.2
//
//  Created by Denis Fedorov on 11.12.2022.
//

#include "bar.h"

Bar::Bar(std::size_t id, std::size_t lenght) :	_done(false),
												_colour(),
												_arrow('>'),
												_filler('-'),
												_remainder(' '),
												_id(id),
												_width(60),
												_lenght(lenght),
												_progress(),
												_duration() {}

Bar::Bar(std::size_t id, std::size_t lenght, Code code) :	_done(false),
															_colour(code),
															_arrow('>'),
															_filler('-'),
															_remainder(' '),
															_id(id),
															_width(60),
															_lenght(lenght),
															_progress(),
															_duration() {}

void Bar::update(std::size_t progress)
{
	this->set_progress(progress);
	this->output(std::cout);
}

void Bar::set_progress(std::size_t value)
{
	std::unique_lock lk {_mutex};
	std::call_once(_flag, [this](){this->_thread_id = std::this_thread::get_id();});
	_progress = value;
}

void Bar::output(std::ostream & os)
{
	std::unique_lock lk {_mutex};
	if (_done)
	{
		return;
	}
	else
	{
		if (_progress == _lenght) {	_done = true; }
		_colour.set();
		static auto start {std::chrono::system_clock::now()};
		std::size_t completed {static_cast<std::size_t>(_progress * _width  / _lenght)};
		os << '\r' << _id << '\t' << _thread_id << _colour << "\t[";
		for (std::size_t i {}; i < _width; ++i)
		{
			if (i < completed)
			{
				os << _filler;
			}
			else if (i == completed)
			{
				os << _arrow;
			}
			else
			{
				os << _remainder;
			}
		}
		_colour.reset();
		_duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start);
		os << "] " << _colour << static_cast<std::size_t>(_progress * 100 / _lenght) << '\t' << _duration.count();
	}
}
