# vim: tw=1000
cmake_minimum_required(VERSION 2.8.8)
project(Soccer)

set(soccer_src main.cpp data.cpp)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wextra")

add_executable(soccer ${soccer_src})
