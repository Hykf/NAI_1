
#include "Ai.h"

using namespace std;

std::vector<std::vector<int>> Ai::pawnsToInt(const std::array<std::array<Pawn, 7>, 6>& pawns) {
    // Initialize the output 2D vector of integers with the same dimensions
    std::vector<std::vector<int>> result(6, std::vector<int>(7));

    // Transform the Pawn objects to integers
    for (size_t i = 0; i < pawns.size(); ++i) {
        for (size_t j = 0; j < pawns[i].size(); ++j) {
            // Determine the integer value based on player color
            switch (pawns[i][j].getPlayerColor()) {
                case EMPTY_SLOT:
                    result[i][j] = 0; // Empty slot
                    break;
                case RED:
                    result[i][j] = 1; // Player pawn
                    break;
                case BLUE:
                    result[i][j] = 2; // Computer pawn
                    break;
                default:
                    result[i][j] = 0; // Fallback (should not occur)
                    break;
            }
        }
    }

    return result; // Return the new 2D vector of integers
}

int heurFunction(unsigned int g, unsigned int b, unsigned int z) {
    int score = 0;
    if (g == 4) { score += 500001; } // preference to go for winning move vs. block
    else if (g == 3 && z == 1) { score += 5000; }
    else if (g == 2 && z == 2) { score += 500; }
    else if (b == 2 && z == 2) { score -= 501; } // preference to block
    else if (b == 3 && z == 1) { score -= 5001; } // preference to block
    else if (b == 4) { score -= 500000; }
    return score;
}

int scoreSet(vector<unsigned int> v, unsigned int p) {
    unsigned int good = 0; // points in favor of p
    unsigned int bad = 0; // points against p
    unsigned int empty = 0; // neutral spots
    for (unsigned int i = 0; i < v.size(); i++) { // just enumerate how many of each
        good += (v[i] == p) ? 1 : 0;
        bad += (v[i] == Config::PLAYER || v[i] == Config::COMPUTER) ? 1 : 0;
        empty += (v[i] == 0) ? 1 : 0;
    }
    // bad was calculated as (bad + good), so remove good
    bad -= good;
    return heurFunction(good, bad, empty);
}

int tabScore(vector<vector<int> > b, unsigned int p) {
    int score = 0;
    vector<unsigned int> rs(Config::NUM_COL);
    vector<unsigned int> cs(Config::NUM_ROW);
    vector<unsigned int> set(4);
    /**
     * horizontal checks, we're looking for sequences of 4
     * containing any combination of AI, PLAYER, and empty pieces
     */
    for (unsigned int r = 0; r < Config::NUM_ROW; r++) {
        for (unsigned int c = 0; c < Config::NUM_COL; c++) {
            rs[c] = b[r][c]; // this is a distinct row alone
        }
        for (unsigned int c = 0; c < Config::NUM_COL - 3; c++) {
            for (int i = 0; i < 4; i++) {
                set[i] = rs[c + i]; // for each possible "set" of 4 spots from that row
            }
            score += scoreSet(set, p); // find score
        }
    }
    // vertical
    for (unsigned int c = 0; c < Config::NUM_COL; c++) {
        for (unsigned int r = 0; r < Config::NUM_ROW; r++) {
            cs[r] = b[r][c];
        }
        for (unsigned int r = 0; r < Config::NUM_ROW - 3; r++) {
            for (int i = 0; i < 4; i++) {
                set[i] = cs[r + i];
            }
            score += scoreSet(set, p);
        }
    }
    // diagonals
    for (unsigned int r = 0; r < Config::NUM_ROW - 3; r++) {
        for (unsigned int c = 0; c < Config::NUM_COL; c++) {
            rs[c] = b[r][c];
        }
        for (unsigned int c = 0; c < Config::NUM_COL - 3; c++) {
            for (int i = 0; i < 4; i++) {
                set[i] = b[r + i][c + i];
            }
            score += scoreSet(set, p);
        }
    }
    for (unsigned int r = 0; r < Config::NUM_ROW - 3; r++) {
        for (unsigned int c = 0; c < Config::NUM_COL; c++) {
            rs[c] = b[r][c];
        }
        for (unsigned int c = 0; c < Config::NUM_COL - 3; c++) {
            for (int i = 0; i < 4; i++) {
                set[i] = b[r + 3 - i][c + i];
            }
            score += scoreSet(set, p);
        }
    }
    return score;
}

bool winningMove(vector<vector<int> > &b, unsigned int p) {
    unsigned int winSequence = 0; // to count adjacent pieces
    // for horizontal checks
    for (unsigned int c = 0; c < Config::NUM_COL - 3; c++) { // for each column
        for (unsigned int r = 0; r < Config::NUM_ROW; r++) { // each row
            for (int i = 0; i < 4; i++) { // recall you need 4 to win
                if ((unsigned int)b[r][c + i] == p) { // if not all pieces match
                    winSequence++; // add sequence count
                }
                if (winSequence == 4) { return true; } // if 4 in row
            }
            winSequence = 0; // reset counter
        }
    }
    // vertical checks
    for (unsigned int c = 0; c < Config::NUM_COL; c++) {
        for (unsigned int r = 0; r < Config::NUM_ROW - 3; r++) {
            for (int i = 0; i < 4; i++) {
                if ((unsigned int)b[r + i][c] == p) {
                    winSequence++;
                }
                if (winSequence == 4) { return true; }
            }
            winSequence = 0;
        }
    }
    // the below two are diagonal checks
    for (unsigned int c = 0; c < Config::NUM_COL - 3; c++) {
        for (unsigned int r = 3; r < Config::NUM_ROW; r++) {
            for (int i = 0; i < 4; i++) {
                if ((unsigned int)b[r - i][c + i] == p) {
                    winSequence++;
                }
                if (winSequence == 4) { return true; }
            }
            winSequence = 0;
        }
    }
    for (unsigned int c = 0; c < Config::NUM_COL - 3; c++) {
        for (unsigned int r = 0; r < Config::NUM_ROW - 3; r++) {
            for (int i = 0; i < 4; i++) {
                if ((unsigned int)b[r + i][c + i] == p) {
                    winSequence++;
                }
                if (winSequence == 4) { return true; }
            }
            winSequence = 0;
        }
    }
    return false; // otherwise no winning move
}

vector<vector<int> > copyBoard(vector<vector<int> > b) {
    vector<vector<int>> newBoard(Config::NUM_ROW, vector<int>(Config::NUM_COL));
    for (unsigned int r = 0; r < Config::NUM_ROW; r++) {
        for (unsigned int c = 0; c < Config::NUM_COL; c++) {
            newBoard[r][c] = b[r][c]; // just straight copy
        }
    }
    return newBoard;
}

void makeMove(vector<vector<int> >& b, int c, unsigned int p) {
    // start from bottom of board going up
    for (unsigned int r = 0; r < Config::NUM_ROW; r++) {
        if (b[r][c] == 0) { // first available spot
            b[r][c] = p; // set piece
            break;
        }
    }
}

array<int, 2> Ai::miniMax(vector<vector<int>> integerBoard, unsigned int maxDepth, int alf, int bet, unsigned int p) {
//    const array<array<Pawn, 7>, 6>& pawns = gameBoard.GameBoard;
//    vector<vector<int>> integerBoard = Ai::transformPawnsToInt(pawns);
    if (maxDepth == 0 || maxDepth >= (Config::NUM_COL * Config::NUM_ROW) - Config::TURN) {
        // get current score to return
        return array<int, 2>{tabScore(integerBoard, Config::COMPUTER), -1};
    }
    if (p == Config::COMPUTER) { // if AI player
        array<int, 2> moveSoFar = {INT_MIN, -1}; // since maximizing, we want lowest possible value
        if (winningMove(integerBoard, Config::PLAYER)) { // if player about to win
            return moveSoFar; // force it to say it's worst possible score, so it knows to avoid move
        } // otherwise, business as usual
        for (unsigned int c = 0; c < Config::NUM_COL; c++) { // for each possible move
            if (integerBoard[Config::NUM_ROW - 1][c] == 0) { // but only if that column is non-full
                vector<vector<int> > newBoard = copyBoard(integerBoard); // make a copy of the board
                makeMove(newBoard, c, p); // try the move
                int score = miniMax(newBoard, maxDepth - 1, alf, bet, Config::PLAYER)[0]; // find move based on that new board state
                if (score > moveSoFar[0]) { // if better score, replace it, and consider that best move (for now)
                    moveSoFar = {score, (int)c};
                }
                alf = max(alf, moveSoFar[0]);
                if (alf >= bet) { break; } // for pruning
            }
        }
        return moveSoFar; // return best possible move
    }
    else {
        array<int, 2> moveSoFar = {INT_MAX, -1}; // since PLAYER is minimized, we want moves that diminish this score
        if (winningMove(integerBoard, Config::COMPUTER)) {
            return moveSoFar; // if about to win, report that move as best
        }
        for (unsigned int c = 0; c < Config::NUM_COL; c++) {
            if (integerBoard[Config::NUM_ROW - 1][c] == 0) {
                vector<vector<int> > newBoard = copyBoard(integerBoard);
                makeMove(newBoard, c, p);
                int score = miniMax(newBoard, maxDepth - 1, alf, bet, Config::COMPUTER)[0];
                if (score < moveSoFar[0]) {
                    moveSoFar = {score, (int)c};
                }
                bet = min(bet, moveSoFar[0]);
                if (alf >= bet) { break; }
            }
        }
        return moveSoFar;
    }
}

std::array<std::array<Pawn, 7>, 6> &Ai::intToPawn(const std::vector<std::vector<int>> integerBoard) {
        std::array<std::array<Pawn, 7>, 6> pawns;

        // Iterate through the 2D vector
        for (size_t i = 0; i < integerBoard.size(); ++i) {
            for (size_t j = 0; j < integerBoard[i].size(); ++j) {
                // Map the integer values back to PlayerColor
                switch (integerBoard[i][j]) {
                    case 0:
                        pawns[i][j] = Pawn(EMPTY_SLOT); // Empty slot
                        break;
                    case 1:
                        pawns[i][j] = Pawn(RED);         // Player pawn
                        break;
                    case 2:
                        pawns[i][j] = Pawn(BLUE);        // Computer pawn
                        break;
                    default:
                        pawns[i][j] = Pawn(EMPTY_SLOT); // Fallback (should not occur)
                        break;
                }
            }
        }

        return pawns; // Return the new 2D array of Pawns
    }

int Ai::calculateMove(Board b) {
    vector<vector<int>> integerBoard = Ai::pawnsToInt(b.GameBoard);
    return miniMax(integerBoard, Config::MAX_DEPTH, 0 - INT_MAX, INT_MAX, Config::COMPUTER)[1];
}

