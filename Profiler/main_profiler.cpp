#include <cstdlib>
#include <string>
#include <iostream>

int main(int argc, char* argv[]) {

    std::string command = "python _visualization_profiler.py ";
    command += argv[1];

    system(command.c_str());
    return 0;
}