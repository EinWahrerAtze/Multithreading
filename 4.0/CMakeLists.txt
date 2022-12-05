cmake_minimum_required (VERSION 3.8)
set (CMAKE_CXX_STANDARD 20)
project ("Thread 6.4")
add_executable (CMakeTarget main.cpp
							shapes.h shapes.cpp
							transform.h transform.cpp)