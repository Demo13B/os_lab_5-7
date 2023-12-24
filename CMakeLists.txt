cmake_minimum_required(VERSION 3.10)
project(ZMQ_Tree)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

find_package(cppzmq)

add_executable(server server.cpp)
add_executable(client client.cpp)

target_link_libraries(server cppzmq)
target_link_libraries(client cppzmq)