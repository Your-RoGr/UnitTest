#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include <string>
#include <functional>
#include <vector>
#include <utility>
#include <memory>
#include <thread>
#include "Timer/timer.h"
#include "Logger/logger.h"

class UnitTest {
private:
    int static failed_tests;
    int static equality_tests;
    int static exception_tests;
    int static boundary_tests;
    int static performance_tests;
    int static concurrency_tests;
    std::vector<std::string> static failed;
    Logger static logger;
public:
    // TestRunner class for running tests
    /*
     * TestRunner test_runner {};
     * test_runner.add_tests([]() {
     *
     *  int x = 5; // You can initialize the objects if you need
     *
     *  UnitTest::[Test class]::[Test for...](args...); // You can add more tests if you need
     * }
     * test_runner.run_tests();
     * */
    class TestRunner {
    public:
        TestRunner();
        static void clear_all_logs(bool clear_all_);
        void add_tests(std::function<void()> tests_);
        void run_tests();
    private:
        std::vector<std::function<void()>> tests {};
    };

    // EqualityTest class for testing equality of values
    class EqualityTest {
    public:
        template<typename T>
        static void assert_equals(const T& actual, const T& expected, const std::string& name = ".Equality",
                                  const std::string& message = "actual and expected should be equal") {

            ++equality_tests;
            if (actual == expected) {
                logger.info('#' + std::to_string(equality_tests) + name + " EqualityTest PASSED");
            } else {
                ++failed_tests;
                logger.warning('#' + std::to_string(equality_tests) + name + " EqualityTest FAILED: "
                + message + ". Expected: " + std::to_string(expected) + ", Actual: " + std::to_string(actual));
                failed.push_back('#' + std::to_string(failed_tests) + name);
            }
        }

        template<typename T>
        static void assert_not_equals(const T& actual, const T& expected, const std::string& name = ".Equality",
                                      const std::string& message = "actual and expected should not be equal") {

            ++equality_tests;
            if (actual != expected) {
                logger.info('#' + std::to_string(equality_tests) + name + " No EqualityTest PASSED");
            } else {
                ++failed_tests;
                logger.warning('#' + std::to_string(equality_tests) + name + " No EqualityTest FAILED: "
                               + message + ". Expected: " + std::to_string(expected) + ", Actual: "
                               + std::to_string(actual));
                failed.push_back('#' + std::to_string(failed_tests) + name);
            }
        }
    };

    // ExceptionTest class for testing exception cases
    class ExceptionTest {
    public:
        template<typename ExceptionType, typename FuncType, typename... Args>
        static void assert_throws(const FuncType& func, const std::string& name, Args... args) {

            ++exception_tests;
            try {
                func(args...);
                ++failed_tests;
                logger.warning('#' + std::to_string(exception_tests) + name
                + " ExceptionTest FAILED. Expected an exception, but no exception was thrown.");
                failed.push_back('#' + std::to_string(exception_tests) + name);
            } catch (const ExceptionType& e) {
                logger.info('#' + std::to_string(exception_tests) + name + " ExceptionTest PASSED");
            } catch (const std::exception& e) {
                ++failed_tests;
                logger.warning('#' + std::to_string(exception_tests) + name
                               + " ExceptionTest FAILED: " + e.what() + ". Expected an exception of type "
                               + typeid(ExceptionType).name() + ", but a different type of exception was thrown.");
                failed.push_back('#' + std::to_string(exception_tests) + name);
            } catch (...) {
                ++failed_tests;
                logger.error('#' + std::to_string(exception_tests) + name
                               + " ExceptionTest FAILED. Expected an exception of type "
                               + typeid(ExceptionType).name() + ", but a different type of exception was thrown.");
                failed.push_back('#' + std::to_string(exception_tests) + name);
            }
        }

        template<typename FuncType, typename... Args>
        static void assert_does_not_throw(const FuncType& func, const std::string& name, Args... args) {

            ++exception_tests;
            try {
                func(args...);
                logger.info('#' + std::to_string(exception_tests) + name + " No ExceptionTest PASSED");
            } catch (const std::exception& e) {
                ++failed_tests;
                logger.warning('#' + std::to_string(exception_tests) + name
                + " No ExceptionTest FAILED: " + e.what());
                failed.push_back('#' + std::to_string(exception_tests) + name);
            } catch (...) {
                ++failed_tests;
                logger.error('#' + std::to_string(exception_tests) + name
                + " No ExceptionTest FAILED. Expected no exception to be thrown, but a different type of "
                  "exception was thrown.");
                failed.push_back('#' + std::to_string(exception_tests) + name);
            }
        }
    };

    // BoundaryTest class for testing boundary cases
    class BoundaryTest {
    public:
        template<typename T>
        static void assert_less(const T& value1, const T& value2, const std::string& name = ".Boundary",
                                const std::string& message = "value1 should be less than value2") {

            ++boundary_tests;
            if (value1 < value2) {
                logger.info('#' + std::to_string(boundary_tests) + name + " assert_less PASSED");
            } else {
                ++failed_tests;
                logger.warning('#' + std::to_string(boundary_tests) + name + " assert_less FAILED: "
                + message + ". Expected " + std::to_string(value1) + " to be less than " + std::to_string(value2));
                failed.push_back('#' + std::to_string(boundary_tests) + name);
            }
        }

        template<typename T>
        static void assert_less_or_equal(const T& value1, const T& value2, const std::string& name = ".Boundary",
            const std::string& message = "value2 should be greater than or equal to value1") {

            ++boundary_tests;
            if (value1 <= value2) {
                logger.info('#' + std::to_string(boundary_tests) + name + " assert_less_or_equal PASSED");
            } else {
                ++failed_tests;
                logger.warning('#' + std::to_string(boundary_tests) + name
                + " assert_less_or_equal FAILED: " + message + ". Expected " + std::to_string(value1)
                + " to be less than or equal to " + std::to_string(value2));
                failed.push_back('#' + std::to_string(boundary_tests) + name);
            }
        }

        template<typename T>
        static void assert_greater(const T& value1, const T& value2, const std::string& name = ".Boundary",
                                   const std::string& message = "value1 should be greater than value2") {

            ++boundary_tests;
            if (value1 > value2) {
                logger.info('#' + std::to_string(boundary_tests) + name + " assert_greater PASSED");
            } else {
                ++failed_tests;
                logger.warning('#' + std::to_string(boundary_tests) + name
                + " assert_greater FAILED: " + message + ". Expected " + std::to_string(value1) + " to be greater than "
                + std::to_string(value2));
                failed.push_back('#' + std::to_string(boundary_tests) + name);
            }
        }

        template<typename T>
        static void assert_greater_or_equal(const T& value1, const T& value2, const std::string& name = ".Boundary",
            const std::string& message = "value2 should be less than or equal to value1") {

            ++boundary_tests;
            if (value1 >= value2) {
                logger.info('#' + std::to_string(boundary_tests) + name + " assert_greater_or_equal PASSED");
            } else {
                ++failed_tests;
                logger.warning('#' + std::to_string(boundary_tests) + name
                + " assert_greater_or_equal FAILED: " + message + ". Expected " + std::to_string(value1)
                + " to be greater than or equal to " + std::to_string(value2));
                failed.push_back('#' + std::to_string(boundary_tests) + name);
            }
        }
    };

    class PerformanceTest {
    public:
        template<typename FuncType, typename... Args>
        static void time_test(const FuncType& func, const std::string& name, const float expected_time, Args... args) {

            ++performance_tests;
            try {

                auto get_time = [&func, &args...]() {
                    Timer T {};
                    func(args...);
                    return T.get_current_time();
                };

                auto time = get_time() * 1000;

                if (expected_time > time) {
                    logger.info('#' + std::to_string(performance_tests) + name + " time_test PASSED"
                    + ": " + std::to_string(time) + " ms");
                } else {
                    ++failed_tests;
                    logger.warning('#' + std::to_string(performance_tests) + name
                    + " time_test FAILED" + ": " + std::to_string(time) + " ms");
                    failed.push_back('#' + std::to_string(performance_tests) + name);
                }
            } catch (const std::exception& e) {
                ++failed_tests;
                logger.error('#' + std::to_string(performance_tests) + name
                + " time_test FAILED: " + e.what());
                failed.push_back('#' + std::to_string(performance_tests) + name);
            } catch (...) {
                ++failed_tests;
                logger.error('#' + std::to_string(performance_tests) + name
                + " time_test FAILED. Expected no exception to be thrown, but a different type of exception "
                 "was thrown.");
                failed.push_back('#' + std::to_string(performance_tests) + name);
            }
        }
    };

    class ConcurrencyTest {
    public:
        template<typename FuncType, typename... Args>
        static void thread_test(const FuncType& func, const std::string& name, const size_t expected_threads, Args... args) {

            ++concurrency_tests;

            auto get_time = [&func, &args..., &name]() {
                Timer T {};
                try {
                    func(args...);
                } catch (const std::exception& e) {
                    logger.error('#' + std::to_string(concurrency_tests) + name
                                 + " thread_test: " + e.what());
                } catch (...) {
                    logger.error('#' + std::to_string(concurrency_tests) + name
                                 + " thread_test. Expected no exception to be thrown, but a different type of exception "
                                   "was thrown.");
                }
                return T.get_current_time();
            };

            auto time = (get_time() * 1000) * expected_threads * 2;

            std::atomic<size_t> counter {};
            auto new_func = [&counter, &func, &args..., &name](){
                try {
                    func(args...);
                    ++counter;
                } catch (const std::exception& e) {
                    logger.error('#' + std::to_string(concurrency_tests) + name
                                 + " thread_test#" + std::to_string(counter) + ": " + e.what());
                } catch (...) {
                    logger.error('#' + std::to_string(concurrency_tests) + name
                    + " thread_test#" + std::to_string(counter) + ". Expected no exception to be thrown, "
                                                                  "but a different type of exception "
                                                                  "was thrown.");
                }
            };

            for (int i = 0; i < expected_threads; ++i) {
                std::thread t(new_func);
                t.detach();
            }

            if (static_cast<long long int>(time) > 1) {
                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long int>(time)));
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            if (counter == expected_threads) {
                logger.info('#' + std::to_string(concurrency_tests) + name + " thread_test PASSED: "
                + std::to_string(counter) + '/' + std::to_string(expected_threads));
            } else {
                ++failed_tests;
                logger.warning('#' + std::to_string(concurrency_tests) + name + " thread_test FAILED: "
                + std::to_string(counter) + '/' + std::to_string(expected_threads));
                failed.push_back('#' + std::to_string(concurrency_tests) + name);
            }
        }
    };
};
#endif //UNIT_TEST_H
