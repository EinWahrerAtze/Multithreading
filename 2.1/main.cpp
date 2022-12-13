//
//  main.cpp
//  6.2.1
//
//  Created by Denis Fedorov on 07.12.2022.
//

#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>

void client(std::atomic<std::size_t> & clients, const int max_value) noexcept
{
//	последовательно согласованное упорядочение
//	while (clients.load(std::memory_order_seq_cst) != max_value)
//	нестрогое упорядочение
//	while (clients.load(std::memory_order_relaxed) != max_value)
// 	упорядочение на основе захвата-освобождения
	while (clients.load(std::memory_order_acquire) != max_value)
	{
		std::cout << "Clients: " << ++clients << '\n';
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	std::cout << "No more clients for today!\n";
}

void manager(std::atomic<std::size_t> & clients) noexcept
{
	do
	{
		std::cout << "Manager: " << --clients << '\n';
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
//	последовательно согласованное упорядочение
//	while (clients.load(std::memory_order_seq_cst) != 0);
//	нестрогое упорядочение
//	while (clients.load(std::memory_order_relaxed) != 0);
// 	упорядочение на основе захвата-освобождения
	while (clients.load(std::memory_order_acquire) != 0);
	std::cout << "All clients served!\n";
}

int main()
{
	std::cout << "Enter maximum number of clients: ";
	std::size_t max_value {};
	std::atomic<std::size_t> clients {};
	while (!(std::cin >> max_value) || max_value > std::numeric_limits<std::size_t>::max() / 2)
	{
		std::cin.clear();
		while (std::cin.get() != '\n') { continue; }
		std::cout << "Please, enter valid value: ";
	}
	std::thread thread_client {client, std::ref(clients), max_value};
	std::thread thread_manager {manager, std::ref(clients)};
	thread_client.join();
	thread_manager.join();
	return 0;
}