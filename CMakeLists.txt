cmake_minimum_required(VERSION 3.25)
project(UnitTest)

set(CMAKE_CXX_STANDARD 20)

file(COPY Profiler/_visualization_profiler.py DESTINATION ${CMAKE_BINARY_DIR})

add_executable(UnitTest main.cpp unit_test.cpp unit_test.h
        Timer/timer.cpp
        Timer/timer.h
        Logger/logger.cpp
        Logger/logger.h
        Profiler/get_memory_usage.cpp
        Profiler/get_memory_usage.h
        Profiler/profiler.h
        Profiler/profiler.cpp
)

add_executable(Profiler
        Profiler/main_profiler.cpp
)
