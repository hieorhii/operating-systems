cmake_minimum_required(VERSION 3.10)

project(OS_Lab2)

set(CMAKE_CXX_STANDARD 98)

add_executable(OS_Lab2 OS_Lab2.cpp)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)
enable_testing()

add_executable(test_os_lab2 test_os_lab2.cpp)
target_link_libraries(test_os_lab2 Boost::unit_test_framework)

add_test(NAME OS_Lab2_Tests COMMAND test_os_lab2)
