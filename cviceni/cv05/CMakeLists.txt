cmake_minimum_required(VERSION 3.23)
project(KIV_CPP)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(05)
add_executable(cv05
        Main.cpp)