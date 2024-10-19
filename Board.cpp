#include "Board.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

void Board::displayBoard() {

#ifdef _WIN32
    setConsoleColor(7);
#else
    setConsoleColor("\033[48;5;7m");  // Ustaw tło na szare
        std::cout << "\033[0m";  // Resetowanie kolorów
#endif

    char letter = 65;

    for(auto row : GameBoard) {
        std::cout << letter++ << ": ";
        for(auto collumn : row) {
            switch (collumn.getPlayerColor()) {
                case EMPTY_SLOT:
                    std::cout << "  " << " ";
                    break;
                case RED:
                    sayInColor(31,"O  ");
                    break;
                case BLUE:
                    sayInColor(34,"O  ");
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


            if (checkForWinner(row, column, playerColor)) {
                std::cout << "Player " << (playerColor == RED ? "Red" : "Blue") << " wins!\n";
                bIsPlaying = false;
            }

            return true;
        }
    }

    std::cout << "Column is full!\n";
    return false;

}

bool Board::checkForWinner(int lastRow, int lastCol, PlayerColor currentPlayer) {

    //POZIOM
    int count = 0;
    for (int col = 0; col < 7; ++col) {
        if (GameBoard[lastRow][col].getPlayerColor() == currentPlayer) {
            count++;
            if (count == 4) return true;
        } else {
            count = 0;
        }
    }

    //PION
    count = 0;
    for (int row = 0; row < 6; ++row) {
        if (GameBoard[row][lastCol].getPlayerColor() == currentPlayer) {
            count++;
            if (count == 4) return true;
        } else {
            count = 0;
        }
    }

    // D
    count = 0;
    for (int i = -3; i <= 3; ++i) {
        int row = lastRow + i;
        int col = lastCol + i;
        if (row >= 0 && row < 6 && col >= 0 && col < 7) {
            if (GameBoard[row][col].getPlayerColor() == currentPlayer) {
                count++;
                if (count == 4) return true;
            } else {
                count = 0;
            }
        }
    }

    count = 0;
    for (int i = -3; i <= 3; ++i) {
        int row = lastRow + i;
        int col = lastCol - i;
        if (row >= 0 && row < 6 && col >= 0 && col < 7) {
            if (GameBoard[row][col].getPlayerColor() == currentPlayer) {
                count++;
                if (count == 4) return true;
            } else {
                count = 0;
            }
        }
    }

    return false;
}

void Board::setColor(int textColor) {
    std::cout << "\033[" << textColor << "m";
}

void Board::resetColor() { std::cout << "\033[0m"; }

void Board::sayInColor(int textColor, std::string text) {
    setColor(textColor);
    std::cout<< text;
    resetColor();
}






