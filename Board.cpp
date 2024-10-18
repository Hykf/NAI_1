//
// Created by s24298 on 12.10.2024.
//

#include "Board.h"

#include <iostream>

void Board::displayBoard() {

    int counter = 0;
    char letter = 65;

    for(auto x : GameBoard) {
        std::cout << letter++ << ": ";
        for(auto y : x) {
            std::cout << "x " << " ";
        }
        std::cout  <<std::endl;
    }
    std::cout<<"   ";
    for(int i = 1; i!= 8;i++) std::cout << i << "  ";
}
