cmake_minimum_required(VERSION 3.15)
project(CLE_WordLengths C)

set(CMAKE_C_STANDARD 11)

add_executable(CLE_WordLengths main.c testEnconding.h reader.h)