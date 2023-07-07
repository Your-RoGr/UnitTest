#include <iostream>
#include "unit_test.h"


void my_test_func_1() {
    std::cout << "my_test_func_1 done" << std::endl;
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

int main() {

    std::cout << std::endl << "TestRunner" << std::endl;

    UnitTest::TestRunner runner;

    runner.add_tests([]() {
        std::cout << std::endl;

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
        return 8;
    });

    runner.add_tests([]() {
        std::cout << std::endl;

        UnitTest::ExceptionTest::assert_throws<std::runtime_error>(my_test_exception_func_1, ".Exception");
        UnitTest::ExceptionTest::assert_throws<std::runtime_error>(my_test_exception_func_2);
        UnitTest::ExceptionTest::assert_does_not_throw(my_test_exception_func_1, ".Exception");
        UnitTest::ExceptionTest::assert_does_not_throw(my_test_exception_func_2);
        UnitTest::ExceptionTest::assert_does_not_throw(my_test_func_1);
        UnitTest::ExceptionTest::assert_does_not_throw(my_test_func_2);
        return 6;
    });

    runner.add_tests([]() {
        std::cout << std::endl;
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

        std::cout << std::endl;

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
        return 16;
    });

    runner.run_tests();

    std::cout << std::endl;

    system("pause");
    return 0;
}
