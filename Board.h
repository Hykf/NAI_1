
#ifndef BOARD_H
#define BOARD_H

#include "Pawn.h"
#include <array>
#include <utility>

struct Board {

std::array<std::array<Pawn,7>,6> GameBoard;

void displayBoard();
void addPawn(int x, int y);


};



#endif //BOARD_H
