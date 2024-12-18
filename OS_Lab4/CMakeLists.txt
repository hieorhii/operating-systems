cmake_minimum_required(VERSION 3.10)
project(IPC_Messaging)

set(CMAKE_CXX_STANDARD 98)

add_executable(receiver main_receiver.cpp)
add_executable(sender main_sender.cpp)
