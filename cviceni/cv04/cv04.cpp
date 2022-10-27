#include <iostream>

class Func {
public:
    int operator()(int par) const {
        return 2 * (par + 1);
    }
};

int main(int argc, char** argv) {

    int a = 5;
    int b = 10;

    auto f = [&a, b]() mutable -> int {
        b = a + 1;

        return a * 2;
    };

    a = 20;

    Func fnc;


    std::cout << f() << std::endl;
    std::cout << fnc(9) << std::endl;


    return EXIT_SUCCESS;
}