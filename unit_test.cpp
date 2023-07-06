#include "unit_test.h"


// UnitTest class implementation
int UnitTest::failed_tests = 0;
int UnitTest::equality_tests = 0;
int UnitTest::exception_tests = 0;
int UnitTest::boundary_tests = 0;

// TestRunner class implementation
void UnitTest::TestRunner::add_tests(std::function<int()> tests_) {
    tests.push_back(std::move(tests_));
}

void UnitTest::TestRunner::run_tests() {
    failed_tests = 0;
    int total_tests = 0;
    for (const auto& test : tests) total_tests += test();

    std::cout << std::endl << "Total tests passed: " << total_tests - failed_tests << "/" << total_tests << std::endl;
}

