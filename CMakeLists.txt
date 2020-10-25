cmake_minimum_required(VERSION 3.17)
project(Lock_free_queue C)

set(CMAKE_C_STANDARD 11)

add_executable(Lock_free_queue main.c Queue.c Queue.h)