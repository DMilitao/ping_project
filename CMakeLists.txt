cmake_minimum_required(VERSION 3.12)
project(ping_project)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(test)
