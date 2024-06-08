cmake_minimum_required(VERSION 3.27)
project(BankApp)

set(CMAKE_CXX_STANDARD 11)
add_compile_options(-l sqlite3)
add_executable(BankApp account.cpp
        account.h
        date.h
        date.cpp
        main.cpp
        accumulator.h
        user.h
        user.cpp
        Analysis.cpp
        Analysis.h
        FinancialManagement.h
db/sqlite3.c
        OutputUtils.h)
