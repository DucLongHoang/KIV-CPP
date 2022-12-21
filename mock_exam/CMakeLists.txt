cmake_minimum_required(VERSION 3.23)
project(KIV_CPP)

set(CMAKE_CXX_STANDARD 20)

add_executable(exam
        DataLoader.hpp Sentence.hpp Text.hpp Transformer.hpp Main.cpp
        )