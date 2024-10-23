#ifndef PAWN_H
#define PAWN_H
#include "PlayerColor.h"

struct Pawn {

Pawn() : playerColor(EMPTY_SLOT) {}
Pawn(PlayerColor _playerColor) : playerColor{_playerColor} {}

bool isPlayerPawn = false;
PlayerColor getPlayerColor() const;

private:
    PlayerColor playerColor;

};

#endif //PAWN_H
