cmake_minimum_required(VERSION 3.25)
project(UnitTest)

set(CMAKE_CXX_STANDARD 20)

add_executable(UnitTest main.cpp unit_test.cpp unit_test.h)
