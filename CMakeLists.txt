cmake_minimum_required(VERSION 3.27)
project(BankApp)

set(CMAKE_CXX_STANDARD 11)

add_executable(BankApp account.cpp
        account.h
        date.h
        date.cpp
        main.cpp
        accumulator.h
        user.h
        user.cpp)
