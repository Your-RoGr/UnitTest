#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <utility>


class UnitTest {
public:
    // TestRunner class for running tests
    /*
     * TestRunner.add_tests([]() {
     *  std::cout << std::endl; // You can use other console functions if you need
     *
     *  int x = 5; // You can initialize the objects if you need
     *
     *  UnitTest::[Test class]::[Test for...](args...); // You can add more tests if you need
     *  return int(count tests)
     * } */
    class TestRunner {
    public:
        void add_tests(std::function<int()> tests_);
        void run_tests();
    private:
        std::vector<std::function<int()>> tests {};
    };

    // EqualityTest class for testing equality of values
    class EqualityTest {
    public:
        template<typename T>
        static void assert_equals(const T& actual, const T& expected, const std::string& name = ".Equality",
                                  const std::string& message = "actual and expected should be equal") {

            ++equality_tests;
            if (actual == expected) {
                std::cout << '#' << equality_tests << name << " EqualityTest PASSED" << std::endl;
            } else {
                ++failed_tests;
                std::cout << '#' << equality_tests << name << " EqualityTest FAILED: " << message << ". Expected: "
                << expected << ", Actual: " << actual << std::endl;
                failed.push_back('#' + std::to_string(failed_tests) + name);
            }
        }

        template<typename T>
        static void assert_not_equals(const T& actual, const T& expected, const std::string& name = ".Equality",
                                      const std::string& message = "actual and expected should not be equal") {

            ++equality_tests;
            if (actual != expected) {
                std::cout << '#' << equality_tests << name << " No EqualityTest PASSED" << std::endl;
            } else {
                ++failed_tests;
                std::cout << '#' << equality_tests << name << " No EqualityTest FAILED: " << message << ". Expected: "
                << expected << ", Actual: " << actual << std::endl;
                failed.push_back('#' + std::to_string(failed_tests) + name);
            }
        }
    };

    // ExceptionTest class for testing exception cases
    class ExceptionTest {
    public:
        template<typename ExceptionType, typename FuncType>
        static void assert_throws(const FuncType& func, const std::string& name = ".Exception") {

            ++exception_tests;
            try {
                func();
                ++failed_tests;
                std::cout << '#' << exception_tests << name << " ExceptionTest FAILED. Expected an exception, but no "
                                                               "exception was thrown." << std::endl;
                failed.push_back('#' + std::to_string(exception_tests) + name);
            } catch (const ExceptionType& e) {
                std::cout << '#' << exception_tests << name << " ExceptionTest PASSED" << std::endl;
            } catch (const std::exception& e) {
                ++failed_tests;
                std::cout << '#' << exception_tests << name << " ExceptionTest FAILED: " << e.what()
                << ". Expected an exception of type " << typeid(ExceptionType).name()
                << ", but a different type of exception was thrown." << std::endl;
                failed.push_back('#' + std::to_string(exception_tests) + name);
            } catch (...) {
                ++failed_tests;
                std::cout << '#' << exception_tests << name << " ExceptionTest FAILED. Expected an exception of type "
                << typeid(ExceptionType).name() << ", but a different type of exception was thrown." << std::endl;
                failed.push_back('#' + std::to_string(exception_tests) + name);
            }
        }

        template<typename FuncType>
        static void assert_does_not_throw(const FuncType& func, const std::string& name = ".Exception") {

            ++exception_tests;
            try {
                func();
                std::cout << '#' << exception_tests << name << " No ExceptionTest PASSED" << std::endl;
            } catch (const std::exception& e) {
                ++failed_tests;
                std::cout << '#' << exception_tests << name << " No ExceptionTest FAILED: " << e.what() << std::endl;
                failed.push_back('#' + std::to_string(exception_tests) + name);
            } catch (...) {
                ++failed_tests;
                std::cout << '#' << exception_tests << name << " No ExceptionTest FAILED. Expected no exception to be "
                                                               "thrown, but a different type of exception was thrown."
                                                               << std::endl;
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
                std::cout << '#' << boundary_tests << name << " assert_less PASSED" << std::endl;
            } else {
                ++failed_tests;
                std::cout << '#' << boundary_tests << name << " assert_less FAILED: " << message << ". Expected "
                << value1 << " to be less than " << value2 << std::endl;
                failed.push_back('#' + std::to_string(boundary_tests) + name);
            }
        }

        template<typename T>
        static void assert_less_or_equal(const T& value1, const T& value2, const std::string& name = ".Boundary",
            const std::string& message = "value2 should be greater than or equal to value1") {

            ++boundary_tests;
            if (value1 <= value2) {
                std::cout << '#' << boundary_tests << name << " assert_less_or_equal PASSED" << std::endl;
            } else {
                ++failed_tests;
                std::cout << '#' << boundary_tests << name << " assert_less_or_equal FAILED: " << message
                << ". Expected " << value1 << " to be less than or equal to " << value2 << std::endl;
                failed.push_back('#' + std::to_string(boundary_tests) + name);
            }
        }

        template<typename T>
        static void assert_greater(const T& value1, const T& value2, const std::string& name = ".Boundary",
                                   const std::string& message = "value1 should be greater than value2") {

            ++boundary_tests;
            if (value1 > value2) {
                std::cout << '#' << boundary_tests << name << " assert_greater PASSED" << std::endl;
            } else {
                ++failed_tests;
                std::cout << '#' << boundary_tests << name << " assert_greater FAILED: " << message << ". Expected "
                << value1 << " to be greater than " << value2 << std::endl;
                failed.push_back('#' + std::to_string(boundary_tests) + name);
            }
        }

        template<typename T>
        static void assert_greater_or_equal(const T& value1, const T& value2, const std::string& name = ".Boundary",
            const std::string& message = "value2 should be less than or equal to value1") {

            ++boundary_tests;
            if (value1 >= value2) {
                std::cout << '#' << boundary_tests << name << " assert_greater_or_equal PASSED" << std::endl;
            } else {
                ++failed_tests;
                std::cout << '#' << boundary_tests << name << " assert_greater_or_equal FAILED: " << message
                << ". Expected " << value1 << " to be greater than or equal to " << value2 << std::endl;
                failed.push_back('#' + std::to_string(boundary_tests) + name);
            }
        }
    };

private:
    int static failed_tests;
    int static equality_tests;
    int static exception_tests;
    int static boundary_tests;
    std::vector<std::string> static failed;
};
#endif //UNIT_TEST_H
