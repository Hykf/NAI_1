#include "Pawn.h"

/*
Gets the player color of the pawn.

    @Parameters:
    void

    @Returns:
    PlayerColor: The color of the pawn (RED, BLUE, or EMPTY_SLOT).

    @Details:
    This function returns the current player color associated with the Pawn object.
*/
PlayerColor Pawn::getPlayerColor() const {
    return playerColor;
}
