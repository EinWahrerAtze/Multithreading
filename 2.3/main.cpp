//
//  main.cpp
//  6.2.3
//
//  Created by Denis Fedorov on 14.12.2022.
//

#include <mutex>
#include <iostream>

class Big_object;
void swap (Big_object & lhs, Big_object & rhs);

class Wrapper
{
public:
	Wrapper(Big_object & bg) : _big_object(bg) {}
	friend void swap_with_lock(Wrapper & lhs, Wrapper & rhs);
	friend void swap_with_scoped_lock(Wrapper & lhs, Wrapper & rhs);
	friend void swap_with_unique_lock(Wrapper & lhs, Wrapper & rhs);
private:
	std::mutex _mutex;
	Big_object _big_object;
};

void swap_with_lock(Wrapper & lhs, Wrapper & rhs)
{
	if (&lhs == &rhs)
	{
		return;
	}
	else
	{
		std::lock(lhs, rhs);
		std::lock_guard<std::mutex> lock_lhs(lhs._mutex, std::adopt_lock);
		std::lock_guard<std::mutex> lock_rhs(rhs._mutex, std::adopt_lock);
		swap(lhs._big_object, rhs._big_object);
	}
}

void swap_with_scoped_lock(Wrapper & lhs, Wrapper & rhs)
{
	if (&lhs == &rhs)
	{
		return;
	}
	else
	{
		std::scoped_lock<std::mutex, std::mutex> lk(lhs._mutex, rhs._mutex);
		swap(lhs._big_object, rhs._big_object);
	}
}

void swap_with_unique_lock(Wrapper & lhs, Wrapper & rhs)
{
	if (&lhs == &rhs)
	{
		return;
	}
	else
	{
		std::unique_lock<std::mutex> lock_lhs(lhs._mutex, std::defer_lock);
		std::unique_lock<std::mutex> lock_rhs(rhs._mutex, std::defer_lock);
		std::lock(lhs, rhs);
		swap(lhs._big_object, rhs._big_object);
	}
}

int main(int argc, const char * argv[])
{
	// ...
	return 0;
}