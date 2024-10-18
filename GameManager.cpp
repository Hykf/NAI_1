#include "GameManager.h"
#include <iostream>

void GameManager::gameLoop() {

    int newTurnColumn;
    const bool bAiStart = (tura+bAiStart)%2==0;
    std::cout<< "FIRST TURN: "<< ((bAiStart)?"Player":"AI") <<std::endl;
    gameBoard.displayBoard();

    while (bPlaying){

        std::cout<< "Tura gracza: "<< (((tura+bAiStart)%2==0)?"Red":"Blue") <<std::endl;
        std::cout<< "Which collumn?: ";
        std::cin>> newTurnColumn;

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


