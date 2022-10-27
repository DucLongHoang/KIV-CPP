CMAKE_MINIMUM_REQUIRED(VERSION 3.23)

PROJECT(KIV_CPP)

set(CMAKE_CXX_STANDARD 20)

ADD_EXECUTABLE(05
        main.cpp DataLoader.cpp DataLoader.h Drawing.cpp Drawing.h)