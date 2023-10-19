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


// TestRunner class implementation
UnitTest::TestRunner::TestRunner(DataSize data_size) {
    std::string command = "Profiler.exe " + get_data_size_string(data_size);
    t = std::thread {[command](){
        system(command.c_str());
    }};
    UnitTest::logger.set_use_console_log(false);
    UnitTest::logger.set_log_level_file(Logger::LogLevel::INFO);
}

UnitTest::TestRunner::~TestRunner() {
    if (t.joinable()) t.join();
}

void UnitTest::TestRunner::add_tests(std::function<void()> tests_) {
    tests.push_back(std::move(tests_));
}

void UnitTest::TestRunner::run_tests() {
    failed_tests = 0;

    profiler.add_test("Start of tests");
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    for (const auto& test : tests) test();

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    profiler.add_test("End of tests");
    profiler.add_test("");

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

