
#ifndef BOARD_H
#define BOARD_H

#include "Pawn.h"
#include <array>
#include <utility>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

struct Board {

std::array<std::array<Pawn,7>,6> GameBoard;

void displayBoard();
bool addPawn(int column, PlayerColor playerColor);

#ifdef _WIN32
    void setConsoleColor(int color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }
#else
    void setConsoleColor(const std::string& color) {
        std::cout << color;
    }
#endif

};



#endif //BOARD_H
