cmake_minimum_required(VERSION 3.25)

project(calor VERSION 0.1 LANGUAGES C CXX)

find_package(Boost REQUIRED COMPONENTS program_options)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(calor calor.cc)

target_link_libraries(calor PRIVATE Boost::program_options)
