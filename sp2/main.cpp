#include <iostream>
#include "mp_terminal.hpp"

int main(int argc, char** argv) {
    MPTerm<15> shell{};
    shell.run();

    return EXIT_SUCCESS;
}