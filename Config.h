//
// Created by mechlon on 23.10.24.
//

#ifndef NAI_1_CONFIG_H
#define NAI_1_CONFIG_H

// Config.h
#ifndef CONFIG_H
#define CONFIG_H

class Config {
public:
    static int MAX_DEPTH;
    static int NUM_COL;
    static int NUM_ROW;
    static int PLAYER;
    static int COMPUTER;
    static int TURN;

    static void updateTurn() {
        TURN++;
    }

};

#endif // CONFIG_H


#endif //NAI_1_CONFIG_H
