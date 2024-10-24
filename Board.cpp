#include "Board.h"
#include <iostream>

/*
Displays the current state of the game board in the console.

    @Parameters:
    None.

    @Returns:
    void: This function does not return a value. It directly outputs the game board's state to the console.

    @Details:
    The function iterates through the 2D array representing the game board and prints each row
    with a corresponding letter (A, B, C, ...) to the left. Each cell displays a colored "O"
    for player pieces (Red or Blue) or a dot for empty slots. Column numbers are printed
    at the bottom for user reference.
*/
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

/*
Adds a pawn of the specified player color to the specified column on the game board.

    @Parameters:
    int column: The column number (1 to 7) where the pawn should be added.
    PlayerColor playerColor: The color of the player (RED or BLUE) whose pawn is being added.

    @Returns:
    bool: Returns true if the pawn was successfully added, false otherwise.

    @Details:
    Validates the column, finds the first empty slot from the bottom, and places the pawn.
    Checks for a winner and ends the game if there is one.
*/
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

/*
Checks if the current player has won the game by connecting four pawns in any direction.

    @Parameters:
    int lastRow: The row index of the last placed pawn.
    int lastCol: The column index of the last placed pawn.
    PlayerColor currentPlayer: The color of the player who made the last move.

    @Returns:
    bool: Returns true if the current player has won, false otherwise.

    @Details:
    Checks for four consecutive pawns horizontally, vertically, and diagonally.
*/
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

/*
Sets the text color for console output.

    @Parameters:
    int textColor: The ANSI color code to set the text color.

    @Returns:
    void

    @Details:
    This function changes the text color in the console using ANSI escape codes.
*/
void Board::setColor(int textColor) {
    std::cout << "\033[" << textColor << "m";
}

/*
Resets the text color in the console to default.

    @Parameters:
    void

    @Returns:
    void

    @Details:
    This function resets the text color to the default console color using ANSI escape codes.
*/
void Board::resetColor() { std::cout << "\033[0m"; }

/*
Prints text in a specified color in the console.

    @Parameters:
    int textColor: The ANSI color code to set the text color.
    std::string text: The text to be printed.

    @Returns:
    void

    @Details:
    This function changes the text color, prints the specified text, and then resets the color back to default.
*/
void Board::sayInColor(int textColor, std::string text) {
    setColor(textColor);
    std::cout<< text;
    resetColor();
}






