#include "GameManager.h"
#include "Ai.h"
#include "Config.h"
#include <iostream>
#include <limits>
/**
* @file GameManager.h
*/
/**
* 
* Game loop for managing turns between the human player and AI in the Connect Four game.
* Handles input, checks for validity, and updates the game state accordingly.
*/
void GameManager::gameLoop() {
    int newTurnColumn;
    const bool AISTART = (bAiStart)%2==0;
    std::cout<< "FIRST TURN: ";
    if ((Config::TURN+AISTART)%2==0)
        Board::sayInColor(31,"Red (Human)");
    else
        Board::sayInColor(34, "Blue (Computer)");
    std::cout<< "\n";
    gameBoard.displayBoard();

    while (gameBoard.bIsPlaying){
        if ((Config::TURN+AISTART)%2==0)
            Board::sayInColor(31,"Red (Human)");
        else
            Board::sayInColor(34, "Blue (Computer)");
        std::cout <<" 's turn"<< std::endl;

        if((Config::TURN+AISTART)%2==0){
            std::cout << "Which column?: ";
            if (!(std::cin >> newTurnColumn)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a valid column number." << std::endl;
                continue;
            }
            if(!gameBoard.addPawn(newTurnColumn,RED))
                continue;
        }else{
            newTurnColumn = Ai::calculateMove(gameBoard);
            if(!gameBoard.addPawn(newTurnColumn,BLUE))
                continue;
        }
        gameBoard.displayBoard();
        Config::updateTurn();
    }
}


