//
//  menu.cpp
//  Threads 6.1.1
//
//  Created by Denis Fedorov on 25.11.2022.
//

#include <iostream>
#include <thread>
#include <chrono>

void client(int & clients, int max_value) noexcept
{
	do
	{
		std::cout << "Clients: " << ++clients << '\n';
		std::this_thread::sleep_for(std::chrono::seconds(1));
	} while (clients != max_value);
	std::cout << "No more clients for today!\n";
}

void manager(int & clients) noexcept
{
	do
	{
		std::cout << "Manager: " << --clients << '\n';
		std::this_thread::sleep_for(std::chrono::seconds(2));
	} while (clients != 0);
	std::cout << "All clients served!\n";
}

int main()
{
	std::cout << "Enter maximum number of clients: ";
	int clients {};
	int max_value {};
	while (!(std::cin >> max_value) || max_value < 0)
	{
		std::cin.clear();
		while (std::cin.get() != '\n') {
			continue;
		}
		std::cout << "Please enter a valid value: ";
	}
	std::thread thread_client {client, std::ref(clients), max_value};
	std::thread thread_manager {manager, std::ref(clients)};
	thread_client.join();
	thread_manager.join();
	return 0;
}