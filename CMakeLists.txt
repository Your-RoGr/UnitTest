cmake_minimum_required(VERSION 3.25)
project(UnitTest)

set(CMAKE_CXX_STANDARD 20)


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "unit-test")
    file(COPY Profiler/_visualization_profiler.py DESTINATION ${CMAKE_BINARY_DIR})
    add_executable(UnitTest main.cpp unit_test.cpp unit_test.h
            Timer/timer.cpp
            Timer/timer.h
            Logger/logger.cpp
            Logger/logger.h
            Profiler/profiler.cpp
            Profiler/profiler.h
            Profiler/Source/get_memory_usage.cpp
            Profiler/Source/get_memory_usage.h
            Profiler/Source/usleep.cpp
            Profiler/Source/usleep.h
    )

    add_executable(Profiler
            Profiler/main_profiler.cpp
    )
endif ()

