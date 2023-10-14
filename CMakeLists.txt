cmake_minimum_required(VERSION 3.25)
project(UnitTest)

set(CMAKE_CXX_STANDARD 20)

add_executable(UnitTest main.cpp unit_test.cpp unit_test.h
        Timer/timer.cpp
        Timer/timer.h
        Logger/logger.cpp
        Logger/logger.h
)

add_executable(Profiler
        Profiler/get_memory_usage.cpp
        Profiler/get_memory_usage.h
        Profiler/profiler.h
        Profiler/profiler.cpp
        Profiler/main_profiler.cpp
)