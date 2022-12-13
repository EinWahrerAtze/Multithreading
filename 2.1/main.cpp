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

//	последовательно согласованное упорядочение
#define memory_order std::memory_order_seq_cst
//	нестрогое упорядочение
//#define memory_order std::memory_order_relaxed
// 	упорядочение на основе захвата-освобождения
//#define memory_order std::memory_order_acquire

void client(std::atomic<std::size_t> & clients, const std::atomic<std::size_t> & max_value)
{
	while (clients.load(memory_order) != max_value.load(memory_order))
	{
		std::cout << "Clients: " << ++clients << '\n';
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	std::cout << "No more clients for today!\n";
}

void manager(std::atomic<std::size_t> & clients)
{
	do
	{
		std::cout << "Manager: " << --clients << '\n';
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
	while (clients.load(memory_order) != 0);
	std::cout << "All clients served!\n";
}

int main()
{
	std::cout << "Enter maximum number of clients: ";
	std::size_t temp {};
	while (!(std::cin >> temp) || temp > std::numeric_limits<std::size_t>::max() / 2)
	{
		std::cin.clear();
		while (std::cin.get() != '\n') { continue; }
		std::cout << "Please, enter valid value: ";
	}

	std::atomic<std::size_t> clients {};
	std::atomic<std::size_t> max_value {};
	max_value.store(temp);
//	max_value.store(temp, memory_order);
	std::thread thread_client {client, std::ref(clients), std::cref(max_value)};
	std::thread thread_manager {manager, std::ref(clients)};
	thread_client.join();
	thread_manager.join();
	return 0;
}