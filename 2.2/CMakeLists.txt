cmake_minimum_required (VERSION 3.8)
set (CMAKE_CXX_STANDARD 20)
project ("Thread 6.2.2")
add_executable (CMakeTarget main.cpp
							bar.h bar.cpp
							multibar.h multibar.cpp
							colour.h colour.cpp)