//
//  colour.h
//  6.2.2 v.2
//
//  Created by Denis Fedorov on 11.12.2022.
//

#pragma once
#include <random>
#include <iostream>

enum class Code
{
	RED			= 31,
	GREEN		= 32,
	YELLOW		= 33,
	BLUE		= 34,
	MAGENTA		= 35,
	CYAN		= 36,
	WHITE		= 37,
	GREY		= 38,
	DEFAULT		= 39
};

class Colour
{
public:
	Colour();
	Colour(Code code);
	void set();
	void reset();
	void change(Code code);
	friend std::ostream & operator<<(std::ostream & os, const Colour & colour);
private:
	std::size_t _code;
	std::size_t _saved_code;
};
