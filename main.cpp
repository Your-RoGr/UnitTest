#include <iostream>
#include <random>
#include "unit_test.h"


void my_test_func_1() {
    std::cout << "my_test_func_1 done" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
}

void my_test_func_2() {
    std::cout << "my_test_func_2 done" << std::endl;
    throw std::runtime_error("An error occurred");
}

void my_test_exception_func_1() {
    throw std::runtime_error("An error occurred");
}

void my_test_exception_func_2() {
    throw std::logic_error("An error occurred");
}

void my_test_time_func_1() {
    std::vector<int> vec {};
    for (int i = 0; i < 10000; ++i) {
        vec.push_back(i);
    }
}

void my_test_time_func_2(int x) {
    std::vector<int> vec {};
    for (int i = 0; i < x; ++i) {
        vec.push_back(i);
    }
}

void my_test_time_func_3(int y, int z) {
    std::vector<int> vec {};
    for (int i = 0; i < y; ++i) {
        vec.push_back(z);
    }
}

std::mutex mtx_1 {};
void my_test_thread_func_1(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        std::lock_guard<std::mutex> lock(mtx_1);
    }
}

void my_test_thread_func_2() {
    throw std::runtime_error("my_test_thread_func_2 is joking");
}

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> distribution(1, 10);

std::mutex mtx_3 {};
void my_test_thread_func_3() {
    if (distribution(gen) == 5) {
        throw std::runtime_error("my_test_thread_func_2 is joking");
    } else {
        std::lock_guard<std::mutex> lock(mtx_3);
        std::cout << "Nope" << std::endl;
    }
}

int main() {
    std::cout << std::endl << "TestRunner" << std::endl;

    UnitTest::TestRunner runner;
    UnitTest::TestRunner::clear_all_logs(true);

    runner.add_tests([]() {

        int x = 5;
        int y = 10;

        UnitTest::EqualityTest::assert_equals(x, y, ".Equality", "x and y should be equal");
        UnitTest::EqualityTest::assert_equals(x, y, ".Equality");
        UnitTest::EqualityTest::assert_not_equals(x, y, ".Equality", "x and y should not be equal");
        UnitTest::EqualityTest::assert_not_equals(x, y, ".Equality");

        y = 5;

        UnitTest::EqualityTest::assert_equals(x, y, ".Equality", "x and y should be equal");
        UnitTest::EqualityTest::assert_equals(x, y);
        UnitTest::EqualityTest::assert_not_equals(x, y, ".Equality", "x and y should not be equal");
        UnitTest::EqualityTest::assert_not_equals(x, y);
    });

    runner.add_tests([]() {

        UnitTest::ExceptionTest::assert_throws<std::runtime_error>(my_test_exception_func_1, ".Exception");
        UnitTest::ExceptionTest::assert_throws<std::runtime_error>(my_test_exception_func_2, ".Exception");
        UnitTest::ExceptionTest::assert_does_not_throw(my_test_exception_func_1, ".Exception");
        UnitTest::ExceptionTest::assert_does_not_throw(my_test_exception_func_2, ".Exception");
        UnitTest::ExceptionTest::assert_does_not_throw(my_test_func_1, ".Exception");
        UnitTest::ExceptionTest::assert_does_not_throw(my_test_func_2, ".Exception");
    });

    runner.add_tests([]() {
        int value1 = 10;
        int value2 = 20;

        UnitTest::BoundaryTest::assert_less(value1, value2, ".Boundary", "error");
        UnitTest::BoundaryTest::assert_less_or_equal(value1, value2, ".Boundary", "error");
        UnitTest::BoundaryTest::assert_greater(value1, value2, ".Boundary", "error");
        UnitTest::BoundaryTest::assert_greater_or_equal(value1, value2, ".Boundary", "error");

        UnitTest::BoundaryTest::assert_less(value1, value2, ".Boundary");
        UnitTest::BoundaryTest::assert_less_or_equal(value1, value2, ".Boundary");
        UnitTest::BoundaryTest::assert_greater(value1, value2, ".Boundary");
        UnitTest::BoundaryTest::assert_greater_or_equal(value1, value2, ".Boundary");

        value1 = 20;
        value2 = 10;

        UnitTest::BoundaryTest::assert_less(value1, value2, ".Boundary", "error");
        UnitTest::BoundaryTest::assert_less_or_equal(value1, value2, ".Boundary", "error");
        UnitTest::BoundaryTest::assert_greater(value1, value2, ".Boundary", "error");
        UnitTest::BoundaryTest::assert_greater_or_equal(value1, value2, ".Boundary", "error");

        UnitTest::BoundaryTest::assert_less(value1, value2);
        UnitTest::BoundaryTest::assert_less_or_equal(value1, value2);
        UnitTest::BoundaryTest::assert_greater(value1, value2);
        UnitTest::BoundaryTest::assert_greater_or_equal(value1, value2);
    });

    runner.add_tests([]() {
        UnitTest::PerformanceTest::time_test(my_test_time_func_1, ".Performance", 1.0f);
        UnitTest::PerformanceTest::time_test(my_test_time_func_2, ".Performance", 10.0f, 1000000);
        UnitTest::PerformanceTest::time_test(my_test_time_func_3, ".Performance", 5.0f, 1000000, 10);
    });

    runner.add_tests([]() {

        const int num_threads = 4;

        UnitTest::ConcurrencyTest::thread_test(my_test_thread_func_1, ".Concurrency", num_threads, 1000);
        UnitTest::ConcurrencyTest::thread_test(my_test_thread_func_1, ".Concurrency", 8, 1000);
        UnitTest::ConcurrencyTest::thread_test(my_test_thread_func_1, ".Concurrency", 16, 1000);
        UnitTest::ConcurrencyTest::thread_test(my_test_thread_func_2, ".Concurrency", num_threads);
        UnitTest::ConcurrencyTest::thread_test(my_test_thread_func_2, ".Concurrency", 8);
        UnitTest::ConcurrencyTest::thread_test(my_test_thread_func_2, ".Concurrency", 16);
        UnitTest::ConcurrencyTest::thread_test(my_test_thread_func_3, ".Concurrency", num_threads);
        UnitTest::ConcurrencyTest::thread_test(my_test_thread_func_3, ".Concurrency", 8);
        UnitTest::ConcurrencyTest::thread_test(my_test_thread_func_3, ".Concurrency", 16);
        UnitTest::ConcurrencyTest::thread_test(my_test_thread_func_3, ".Concurrency", 100);
    });

    runner.run_tests();

    //system("pause");
    return 0;
}
