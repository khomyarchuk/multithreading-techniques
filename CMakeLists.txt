cmake_minimum_required(VERSION 3.30)
project(MultiThreads)

set(CMAKE_CXX_STANDARD 20)

add_executable(MultiThreads
        mutex/mutex_sum.cpp
)
