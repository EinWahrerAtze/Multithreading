cmake_minimum_required (VERSION 3.8)
set (CMAKE_CXX_STANDARD 20)
project ("Thread pool")
add_executable (CMakeTarget main.cpp queue.hpp
                                     threadsafe_queue.hpp
                                     thread_pool.hpp)