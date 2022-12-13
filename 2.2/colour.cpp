//
//  colour.cpp
//  6.2.2 v.2
//
//  Created by Denis Fedorov on 11.12.2022.
//

#include "colour.h"

Colour::Colour(Code code) : _code(static_cast<std::size_t>(code)), _saved_code(_code) {}

Colour::Colour()
{
	std::random_device rd;
	std::mt19937 engine {rd()};
	std::uniform_int_distribution<std::size_t> distribution {31, 39};
	_code = distribution(engine);
	_saved_code = _code;
}

std::ostream & operator<<(std::ostream & os, const Colour & colour)
{
	return os << "\033[" << colour._code << "m";
}

void Colour::set()
{
	_code = _saved_code;
}

void Colour::reset()
{
	_code = static_cast<std::size_t>(Code::DEFAULT);
}

void Colour::change(Code code)
{
	_code = static_cast<std::size_t>(code);
}
