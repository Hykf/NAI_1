#include <vector>
#include <climits>
#include "Board.h"
#include "Config.h"

#ifndef NAI_1_AI_H
#define NAI_1_AI_H


class Ai {

    static std::array<int, 2> miniMax(std::vector<std::vector<int>> integerBoard, unsigned int MAX_DEPTH, int alf, int bet, unsigned int p);
    static std::vector<std::vector<int>> pawnsToInt(const std::array<std::array<Pawn, 7>, 6>& pawns);
    static std::array<std::array<Pawn, 7>, 6>& intToPawn(std::vector<std::vector<int>> integerBoard);

public:
    static int calculateMove(Board gameBoard);
};


#endif //NAI_1_AI_H
