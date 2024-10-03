cmake_minimum_required(VERSION 3.23)
project(OS_Lab1)

set(CMAKE_CXX_STANDARD 98)

add_executable(main main.cpp employee.h)
add_executable(creator creator.cpp employee.h)
add_executable(reporter reporter.cpp employee.h)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

if(Boost_FOUND)
    enable_testing()

    add_executable(CreateBinaryFileTest test_creator.cpp)
    target_link_libraries(CreateBinaryFileTest Boost::unit_test_framework)

    add_test(NAME CreateBinaryFileTest COMMAND CreateBinaryFileTest)
endif()
