#include "Pawn.h"
/**
* @file Pawn.h 
*/
/**
* 
* Gets the player color of the pawn.
*
* @param
* void
*
* @return
* PlayerColor: The color of the pawn (RED, BLUE, or EMPTY_SLOT).
*
* @brief
* This function returns the current player color associated with the Pawn object.
*/
PlayerColor Pawn::getPlayerColor() const {
    return playerColor;
}
