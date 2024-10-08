cmake_minimum_required(VERSION 3.26)
project(Hydrogen)

set(CMAKE_CXX_STANDARD 23)

add_executable(Hydrogen src/main.cpp
        src/tokenization.h
        src/parser.h
        src/generation.h)
