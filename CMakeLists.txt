cmake_minimum_required(VERSION 3.25)
project(NAI_1)

set(CMAKE_EXE_LINKER_FLAGS "-static")

set(CMAKE_CXX_STANDARD 20)

add_executable(NAI_1
        main.cpp
        Board.h
        Board.cpp
        Pawn.cpp
        Pawn.h
        GameManager.cpp
        GameManager.h
        PlayerColor.h
        Ai.cpp
        Ai.h
        Config.cpp
        Config.h)
