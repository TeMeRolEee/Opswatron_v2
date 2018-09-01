cmake_minimum_required(VERSION 3.12)
project(Opswatron_v2)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core HINTS ~/Qt/5.11.1/gcc_64/)

add_executable(Opswatron_v2 main.cpp Core.cpp Core.h)

target_link_libraries(Opswatron_v2 Qt5::Core)
