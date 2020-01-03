cmake_minimum_required(VERSION 3.15)
project(OS_course)

set(CMAKE_C_STANDARD 99)

add_executable(Lab_04_center_filter Lab_04/center_filter.c)
add_executable(Lab_04_hex_filter Lab_04/hex_filter.c)
add_executable(Lab_04_task1 Lab_04/task1.c)
add_executable(Lab_04_task2 Lab_04/task2.c)
add_executable(Lab_04_client Lab_04/server.h Lab_04/client.c)
add_executable(Lab_04_server Lab_04/server.h Lab_04/server.c)

add_executable(Lab_05_server Lab_05/header.h Lab_05/server.c)
add_executable(Lab_05_client Lab_05/header.h Lab_05/client.c)
