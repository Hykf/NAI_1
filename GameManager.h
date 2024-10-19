#ifndef NAI_1_GAMEMANAGER_H
#define NAI_1_GAMEMANAGER_H

#include "Board.h"

struct GameManager {

    GameManager(){gameLoop();}
    GameManager(bool aiStart): bAiStart{aiStart} {gameLoop();}; //TODO przyda sie jak z konsoli bedzie odpalane

    int tura{0};
    bool bPlaying = true;
    bool bAiStart = false;
    Board gameBoard;

    void gameLoop();

};

#endif //NAI_1_GAMEMANAGER_H
