#include "Board.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

void Board::displayBoard() {
    char letter = 65;

    for(auto row : GameBoard) {
        std::cout << letter++ << ": ";
        for(auto collumn : row) {
            switch (collumn.getPlayerColor()) {
                case EMPTY_SLOT:
                    std::cout << "  " << " ";
                    break;
                case RED:
                    std::cout << "R " << " ";
                    break;
                case BLUE:
                    std::cout << "B " << " "; //TODO dorobic kolory w konsoli
                    break;
            }

        }
        std::cout<<std::endl;
    }
    std::cout<<"   ";
    for(int i = 0; i<=GameBoard.size();i++) std::cout << i+1 << "  ";
    std::cout<<"\n\n";
}

bool Board::addPawn(int column, PlayerColor playerColor) {

    if (column < 1 || column > 7) {
        std::cout << "Invalid column. Choose between 1 and 7.\n";
        return false ;
    }
    --column;
    for (int row = 5; row >= 0; --row) {
        if (GameBoard[row][column].getPlayerColor() == EMPTY_SLOT) {
            GameBoard[row][column] = Pawn(playerColor);
            return true;
        }
    }

    std::cout << "Column is full!\n";
    return false;

}






