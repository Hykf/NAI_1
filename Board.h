
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
bool bIsPlaying = true;

    void displayBoard();
    bool addPawn(int column, PlayerColor playerColor);
    bool checkForWinner(int lastRow, int lastCol, PlayerColor currentPlayer);

    void setColor(int textColor);
    void sayInColor(int textColor,std::string text);
    void resetColor();

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
