
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

    static void setColor(int textColor);
    static void sayInColor(int textColor,std::string text);
    static void resetColor();

};

#endif //BOARD_H
