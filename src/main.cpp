/// Driver code for j1 interpreter.
#include <cstdint>
#include <cstdlib>
#include <iostream>

#include "Logger.hpp"
#include "VM.hpp"

void print_usage(const char *argv0) { std::cout << "Usage: " << argv0 << " [image]" << std::endl; }

int main(int argc, char *argv[])
{
    if (argc > 2) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    auto vm = VM();
    auto rc = 0;

    if (argc == 1) {
        vm.StartInteractive();
    } else {
        rc = vm.ExecuteImage(argv[1], true);
    }

    return rc;
}
