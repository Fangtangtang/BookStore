cmake_minimum_required(VERSION 3.22)
project(BookStore)

set(CMAKE_CXX_STANDARD 14)

add_executable(BookStore main.cpp account.h account.cpp linkList.h   )
