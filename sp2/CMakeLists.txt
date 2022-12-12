cmake_minimum_required(VERSION 3.23)
project(KIV_CPP)

set(CMAKE_CXX_STANDARD 20)

add_executable(sp2
        mp_arithmetic.hpp
        mp_terminal.hpp
        main.cpp
        )