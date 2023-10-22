# UnitTest
Данный репозиторий посвящен программе для юнит-тестирования на C++

- [Features](#Features)
- [Tech](#Tech)
- [Installation](#Installation)
- [Requirements](#Requirements)
- [Usage](#Usage)
- [Files](#Files)

## Features

Имеются следующие функции тестирования в class UnitTest:

|      Класс      |     Функция тестирования     |                                                 Аргументы                                                 |                                                       Описание                                                        |
|:---------------:|:----------------------------:|:---------------------------------------------------------------------------------------------------------:|:---------------------------------------------------------------------------------------------------------------------:| 
|  EqualityTest   |        assert_equals         |  const T& actual, <br/>const T& expected, <br/>const std::string& name, <br/>const std::string& message   |                                   Проверяет равенство аргументов actual и expected                                    |
|  EqualityTest   |      assert_not_equals       |  const T& actual, <br/>const T& expected, <br/>const std::string& name, <br/>const std::string& message   |                                  Проверяет неравенство аргументов actual и expected                                   |
|  ExceptionTest  | assert_throws<ExceptionType> |                   const FuncType& func, <br/>const std::string& name, <br/>Args... args                   |                                               Проверяет наличие ошибки                                                |
|  ExceptionTest  |    assert_does_not_throw     |                   const FuncType& func, <br/>const std::string& name, <br/>Args... args                   |                                              Проверяет отсутствие ошибки                                              |
|  BoundaryTest   |         assert_less          |   const T& value1, <br/>const T& value2, <br/>const std::string& name, <br/>const std::string& message    |                                    Проверяет соответствие условию value1 < value2                                     |
|  BoundaryTest   |     assert_less_or_equal     |   const T& value1, <br/>const T& value2, <br/>const std::string& name, <br/>const std::string& message    |                                    Проверяет соответствие условию value1 <= value2                                    |
|  BoundaryTest   |        assert_greater        |   const T& value1, <br/>const T& value2, <br/>const std::string& name, <br/>const std::string& message    |                                    Проверяет соответствие условию value1 > value2                                     |
|  BoundaryTest   |   assert_greater_or_equal    |   const T& value1, <br/>const T& value2, <br/>const std::string& name, <br/>const std::string& message    |                                    Проверяет соответствие условию value1 >= value2                                    |
| PerformanceTest |          time_test           |   const FuncType& func, <br/>const std::string& name, <br/>const float expected_time, <br/>Args... args   |  Проверяет соответствие условию время выполнения функции <= expected_time, expected_time измеряется в миллисекундах   |
| ConcurrencyTest |         thread_test          | const FuncType& func, <br/>const std::string& name, <br/>const size_t expected_threads, <br/>Args... args | Проверяет без ошибочное выполнение всех потоков, не проверяет deadlock (зависание) из-за двойного блокирования потока |

Единицы объема данных для профилирования реализованы в виде enum class DataSize:

| Единицы объема данных |       Представление       |
|:---------------------:|:-------------------------:|
|         BYTES         | UnitTest::DataSize::BYTES |
|          KB           |  UnitTest::DataSize::KB   |
|          MB           |  UnitTest::DataSize::MB   |
|          GB           |  UnitTest::DataSize::GB   |

## Tech

Для работы UnitTest использует стандарт&nbsp; _C++20_ и _Python 3.10_:

- [MinGW](https://winlibs.com/) - Компилятор MinGW
> При тестировании использовался следующий компилятор:
> GCC 13.2.0 (with POSIX threads) + LLVM/Clang/LLD/LLDB 16.0.6 + MinGW-w64 11.0.0 (UCRT) - release 1
- [cppreference](https://ru.cppreference.com/w/cpp/20) - Всё о стандарте&nbsp; _C++20_
- [Python](https://www.python.org/downloads/release/python-3100/) - Python 3.10 (Используется для работы профайлера)

Библиотеки Python используется в следующих версиях:

- matplotlib==3.8.0
- pandas==2.1.1
- numpy==1.25.2

## Installation

1. Архив с компилятором необходимо разархивировать в удобное место (обычно в _C:\mingw64_)
2. Добавить в переменные среды в системные переменные в Path: _C:\mingw64_ и _C:\mingw64\bin_
3. Проверить работоспособность компилятора в консоли:
```sh
gcc -v
```
4. Выбрать MinGW в IDE для компиляции (Если используете IDE)

## Requirements

Предварительно необходимо установить Python 3.10 или выше с добавлением в переменные среды в системные переменные в Path:

1. C:\Program Files\Python310\
2. C:\Program Files\Python310\include\
3. C:\Program Files\Python310\Scripts\

##### Universal

```sh
pip install -r requirements.txt
```

##### Windows

- Запустить install-requirements.bat

##### Linux

- Запустить install-requirements.sh

## Usage

Для работы с юнит-тестированием используется объект class TestRunner.

|    Функция     |          Аргументы           |           Описание            |
|:--------------:|:----------------------------:|:-----------------------------:|
|   TestRunner   |      DataSize data_size      | Конструктор класса TestRunner |
| void add_tests | std::function<void()> _tests |  Добавляет функцию с тестами  |
| void run_tests |              -               |    Запускает тестирование     |

- Пример использования EqualityTest:
```С++
#include "unit_test.h"

int main() {
    UnitTest::TestRunner runner {UnitTest::DataSize::BYTES};
    UnitTest::set_us(50);

    runner.add_tests([]() {

        int x = 5;
        int y = 10;

        UnitTest::EqualityTest::assert_equals(x, y, ".Equality", "x and y should be equal");
        UnitTest::EqualityTest::assert_equals(x, y, ".Equality");
        UnitTest::EqualityTest::assert_not_equals(x, y, ".Equality", "x and y should not be equal");
        UnitTest::EqualityTest::assert_not_equals(x, y, ".Equality");
    });

    runner.run_tests();
}
```

- Вывод в файле #0_unit_tests.log:
```
[19-10-2023 21:49:49] [WARNING] #1.Equality EqualityTest FAILED: x and y should be equal. Expected: 10, Actual: 5
[19-10-2023 21:49:49] [WARNING] #2.Equality EqualityTest FAILED: actual and expected should be equal. Expected: 10, Actual: 5
[19-10-2023 21:49:49] [INFO] #3.Equality No EqualityTest PASSED
[19-10-2023 21:49:49] [INFO] #4.Equality No EqualityTest PASSED
[19-10-2023 21:49:49] [INFO] Total tests passed: 2/4
[19-10-2023 21:49:49] [WARNING] The following tests failed:
#1.Equality
#2.Equality
```

- Пример использования ExceptionTest:
```С++
#include "unit_test.h"

void my_test_exception_func_1() {
    throw std::runtime_error("An error occurred");
}

int main() {
    UnitTest::TestRunner runner {UnitTest::DataSize::BYTES};
    UnitTest::set_us(50);

    runner.add_tests([]() {
        UnitTest::ExceptionTest::assert_throws<std::runtime_error>(my_test_exception_func_1, ".Exception");
        UnitTest::ExceptionTest::assert_does_not_throw(my_test_exception_func_1, ".Exception");
    });

    runner.run_tests();
}
```

- Вывод в файле #0_unit_tests.log:
```
[19-10-2023 22:46:05] [INFO] #1.Exception ExceptionTest PASSED
[19-10-2023 22:46:05] [WARNING] #2.Exception No ExceptionTest FAILED: An error occurred
[19-10-2023 22:46:05] [INFO] Total tests passed: 1/2
[19-10-2023 22:46:05] [WARNING] The following tests failed:
#2.Exception
```

- Пример использования BoundaryTest:
```С++
#include "unit_test.h"

int main() {
    UnitTest::TestRunner runner {UnitTest::DataSize::BYTES};
    
    runner.add_tests([]() {
        int value1 = 10;
        int value2 = 20;

        UnitTest::BoundaryTest::assert_less(value1, value2);
        UnitTest::BoundaryTest::assert_less_or_equal(value1, value2);
        UnitTest::BoundaryTest::assert_greater(value1, value2);
        UnitTest::BoundaryTest::assert_greater_or_equal(value1, value2);
    });
    
    runner.run_tests();
}
```

- Вывод в файле #0_unit_tests.log:
```
[19-10-2023 22:46:53] [INFO] #1.Boundary assert_less PASSED
[19-10-2023 22:46:53] [INFO] #2.Boundary assert_less_or_equal PASSED
[19-10-2023 22:46:53] [WARNING] #3.Boundary assert_greater FAILED: value1 should be greater than value2. Expected 10 to be greater than 20
[19-10-2023 22:46:53] [WARNING] #4.Boundary assert_greater_or_equal FAILED: value2 should be less than or equal to value1. Expected 10 to be greater than or equal to 20
[19-10-2023 22:46:53] [INFO] Total tests passed: 2/4
[19-10-2023 22:46:53] [WARNING] The following tests failed:
#3.Boundary
#4.Boundary
```

- Пример использования PerformanceTest:
```С++
#include "unit_test.h"

void my_test_time_func_1(int x) {
    std::vector<int> vec {};
    for (int i = 0; i < x; ++i) {
        vec.push_back(i);
    }
}

int main() {
    UnitTest::TestRunner runner {UnitTest::DataSize::KB};

    runner.add_tests([]() {
        UnitTest::PerformanceTest::time_test(my_test_time_func_1, ".Performance", 10.0f, 1000000);
    });

    runner.run_tests();
}
```

- Вывод в файле #0_unit_tests.log:
```
[19-10-2023 22:47:59] [INFO] #1.Performance time_test PASSED: 6.613700 ms
[19-10-2023 22:47:59] [INFO] Total tests passed: 1/1
```

- Пример использования ConcurrencyTest:
```С++
#include "unit_test.h"

std::mutex mtx_1 {};
void my_test_thread_func_1(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        std::lock_guard<std::mutex> lock(mtx_1);
    }
}

int main() {
    UnitTest::TestRunner runner {UnitTest::DataSize::KB};

    runner.add_tests([]() {
        const int num_threads = 16;
        UnitTest::ConcurrencyTest::thread_test(my_test_thread_func_1, ".Concurrency", num_threads, 1000);
    });
    
    runner.run_tests();
}
```

- Вывод в файле #0_unit_tests.log:
```
[19-10-2023 22:49:21] [INFO] #1.Concurrency thread_test PASSED: 16/16
[19-10-2023 22:49:21] [INFO] Total tests passed: 1/1
```

## Files

- Logger
- - logger.cpp - файл с реализацией логера
- - logger.h - файл с объявлениями класса логера и его функций

- Profiler
- - Source
- - - get_memory_usage.cpp - файл с реализацией функции получения объёма оперативной памяти текущего процесса
- - - get_memory_usage.h - файл с объявлением функции для get_memory_usage.cpp
- - - usleep.cpp - файл с реализацией функции приостановки выполнения потока в микросекундах
- - - usleep.h - файл с объявлением функции для usleep.cpp
- - _visualization_profiler.py - файл с реализацией UI профайлера
- - main_profiler.cpp - файл для компиляции исполняемого файла для профайлера
- - profiler.cpp - файл с реализацией профайлера
- - profiler.h - файл с объявлениями функций и классов для profiler.cpp

- Requirements - хранит файлы для установки пакетов Python

- Timer
- - timer.cpp - файл с реализацией таймера
- - timer.h - файл с объявлениями функций и классов для timer.cpp

- CMakeLists.txt - файл настроек для сборки программного проекта с помощью CMake
- main.cpp - файл с нативными тестами
- unit_test.cpp - файл с реализацией UnitTest
- unit_test.h - файл с объявлениями функций и классов для unit_test.cpp

## License

MIT

**Free Software, Hell Yeah!**