cmake_minimum_required(VERSION 3.10)

project(MarkerApp)

set(CMAKE_CXX_STANDARD 98)

add_executable(marker_app OS_Lab3.cpp)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)
add_executable(test_marker test_marker.cpp)
target_link_libraries(test_marker Boost::unit_test_framework)
enable_testing()
add_test(NAME MarkerTest COMMAND test_marker)
