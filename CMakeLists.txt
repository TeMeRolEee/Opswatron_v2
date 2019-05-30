cmake_minimum_required(VERSION 3.14)
project(Opswatron_v2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC)
set(CMAKE_AUTORCC)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)

add_executable(Opswatron_v2 main.cpp core.cpp clihandler.cpp gamemap.cpp enginehandler.cpp enginehandler.h engine.cpp engine.h)

target_link_libraries(Opswatron_v2 Qt5::Core)