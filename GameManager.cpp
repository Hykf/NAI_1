#include "GameManager.h"
#include <iostream>

void GameManager::gameLoop() {

    int newTurnColumn;
    const bool bAiStart = (tura+bAiStart)%2==0;
    std::cout<< "FIRST TURN: "<< ((bAiStart)?"Player":"AI") <<std::endl;
    gameBoard.displayBoard();

    while (gameBoard.bIsPlaying){

        std::cout<< "Tura gracza: ";
        if ((tura+bAiStart)%2==0) {
            Board::sayInColor(31,"Red");
        } else {
            Board::sayInColor(34, "Blue");
        }
        std::cout << std::endl;
        std::cout << "Which column?: ";
        if (!(std::cin >> newTurnColumn)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a valid column number." << std::endl;
            continue;
        }

        if((tura+bAiStart)%2==0){
            if(!gameBoard.addPawn(newTurnColumn,RED))
                continue;
        }else{
            if(!gameBoard.addPawn(newTurnColumn,BLUE))
                continue;
        }

        gameBoard.displayBoard();
        tura++;

    }

}


