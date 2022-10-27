#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>
#include <numeric>
#include <ranges>
#include <random>

std::random_device rdev;

class Func {
public:
    int operator()(int par) const {
        return 2 * (par + 1);
    }
};

//class MyException : public std::exception {
//private:
//    int mErrorCode;
//
//public:
//    MyException(int errorCode) : mErrorCode(errorCode){};
//    virtual const char* what() const override {
//        return "No fuj";
//    }
//
//    int getErrorCode() const {
//        return mErrorCode;
//    }
//
//};

int main(int argc, char** argv) {

    std::default_random_engine reng( rdev() );
    std::normal_distribution<double> kostka(0.0, 2.0);

    for (size_t i = 0; i < 10; ++i) {
        std::cout << kostka(reng) << std::endl;
    }


//    std::cout << "BEGIN" << std::endl;
//
//    try {
//        MyException ex(10689);
//        throw ex;
////        int* pole = new int[100000000000000];
////        throw std::runtime_error{"Runtime error"};
//    }
//    catch (std::runtime_error& ex) {
//        std::cerr << "Caught some runtime error: " << ex.what() << std::endl;
//    }
//    catch (MyException& ex) {
//        std::cerr << "My error: " << ex.what() << std::endl;
//    }
//    catch (std::exception& ex) {
//        std::cerr << "Caught some error: " << ex.what() << std::endl;
//    }
//
//    std::cout << "END" << std::endl;

//    int a = 5;
//    int b = 10;
//
//    auto f = [&a, b]() mutable -> int {
//        b = a + 1;
//
//        return a * 2;
//    };
//
//    auto lfnc = [](int par, int mul) {
//        return mul * (par + 1);
//    };
//
//    std::cout << lfnc(8, 2) << std::endl;
//
//    auto lfnc_2 = std::bind(lfnc, std::placeholders::_1, 2);
//
//    std::cout << lfnc_2(8) << std::endl;
//
//
//    a = 20;
//    Func fnc;
//
//
//    std::cout << f() << std::endl;
//    std::cout << fnc(9) << std::endl;

//    std::vector<int> numbers(30);
//    std::iota(numbers.begin(), numbers.end(), 1);
//
//    std::transform(numbers.begin(), numbers.end(), numbers.begin(), [](int a){
//        return a * 2;
//    });

//    int num = 1;
//    std::generate(numbers.begin(), numbers.end(), []() mutable {
//        num += 2;
//        return num;
//    });

//    auto dbl = [](int a) { return a * 2; };
////    auto numbers = std::ranges::iota_view{1, 30} | std::views::transform([](dbl));
//
//    for (auto i : std::views::iota(1) | std::views::take(30) | std::views::transform(dbl)) {
//        std::cout << i << std::endl;
//    }


    return EXIT_SUCCESS;
}