cmake_minimum_required(VERSION 3.23)
project(KIV_CPP)

set(CMAKE_CXX_STANDARD 20)

add_executable(cv11
        analyzer.hpp analyzer.cpp
        drawing.hpp drawing.cpp
        generator.hpp generator.cpp
        shared.hpp main.cpp
        )