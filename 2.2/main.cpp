//
//  main.cpp
//  6.2.2 v.2
//
//  Created by Denis Fedorov on 11.12.2022.
//

#include <iostream>
#include "multibar.h"

void clear()
{
#if defined(_WIN32)
	system("cls");
#else
	system("clear");
#endif
}

template <typename T>
void check_input(T & variable)
{
	while (!(std::cin >> variable) || variable > std::numeric_limits<T>::max() / 2)
	{
		std::cin.clear();
		while(std::cin.get() != '\n') { continue; }
		std::cout << "Please, enter valid value: ";
	}
}

int main(int argc, const char * argv[])
{
	clear();
	std::size_t lenght {};
	std::cout << "Enter lenght: ";
	check_input<std::size_t>(lenght);
	std::size_t threads {};
	std::cout << "Enter number of threads: ";
	check_input<std::size_t>(threads);
	clear();
//	Multibar multibar(threads, lenght, Speed::STATIC);
	Multibar multibar(threads, lenght, Speed::DYNAMIC);
	multibar.run();
	std::cout << '\n';
	return 0;
}
