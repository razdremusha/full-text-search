cmake_minimum_required(VERSION 3.26)
project(labwork11_souffl)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(src)
add_subdirectory(bin)

enable_testing()
add_subdirectory(test)