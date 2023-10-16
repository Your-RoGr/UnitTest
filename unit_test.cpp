#include "unit_test.h"


// UnitTest class implementation
std::vector<std::string> UnitTest::failed {};
int UnitTest::failed_tests = 0;
int UnitTest::equality_tests = 0;
int UnitTest::exception_tests = 0;
int UnitTest::boundary_tests = 0;
int UnitTest::performance_tests = 0;
int UnitTest::concurrency_tests = 0;
Logger UnitTest::logger {"_unit_tests.log"};
Profiler UnitTest::profiler {100};

UnitTest::TestRunner::TestRunner() {
    UnitTest::logger.set_use_console_log(false);
    UnitTest::logger.set_log_level_file(Logger::LogLevel::INFO);
}

UnitTest::TestRunner::~TestRunner() {
    if (t.joinable()) t.join();
}

void UnitTest::TestRunner::clear_all_logs(bool clear_all_) {
    UnitTest::logger.set_clear_all(clear_all_);
    UnitTest::logger.set_filename("_unit_tests.log");
}

// TestRunner class implementation
void UnitTest::TestRunner::add_tests(std::function<void()> tests_) {
    tests.push_back(std::move(tests_));
}

void UnitTest::TestRunner::run_tests() {
    failed_tests = 0;
    for (const auto& test : tests) test();
    int total_tests = equality_tests + exception_tests + boundary_tests + performance_tests + concurrency_tests;
    logger.info("Total tests passed: " + std::to_string(total_tests- failed_tests) + '/'
    + std::to_string(total_tests));

    if (total_tests - failed_tests != total_tests) {
        std::string str {};
        for (const auto& val : failed) {
            str += '\n' + val;
        }
        logger.warning("The following tests failed:" + str);
    }
}

