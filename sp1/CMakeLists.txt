cmake_minimum_required(VERSION 3.23)
project(KIV_CPP)

set(CMAKE_CXX_STANDARD 20)

add_executable(sp1
        Transformable.cpp Transformable.h
        DataLoader.cpp DataLoader.h
        Drawing.cpp Drawing.h
        Main.cpp)