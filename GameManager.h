#ifndef NAI_1_GAMEMANAGER_H
#define NAI_1_GAMEMANAGER_H

#include "Board.h"

struct GameManager {

    GameManager(){gameLoop();}
    GameManager(bool aiStart): bAiStart{aiStart} {gameLoop();};

    bool bAiStart = false;
    Board gameBoard;

    void gameLoop();
};

#endif //NAI_1_GAMEMANAGER_H
