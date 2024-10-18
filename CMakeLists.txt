cmake_minimum_required(VERSION 3.29)
project(NAI_1)

set(CMAKE_CXX_STANDARD 20)

add_executable(NAI_1 main.cpp
        Board.cpp
        Board.h
        Pawn.cpp
        Pawn.h)
