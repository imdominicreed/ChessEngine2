#include <iostream>

struct Board {
    short whiteKing;
    short blackKing;
    unsigned long long whitePieces;
    unsigned long long blackPieces;
    unsigned long long linearPieces;
    unsigned long long diagonalPieces;
    unsigned long long pawns;

    void initBoard() {
        whiteKing = 3;
        blackKing = 59;
        whitePieces = 65535;
        blackPieces = 18446462598732840960;
        linearPieces = 448351135499550865;
        diagonalPieces = 3746994889972252724;
        pawns = 71776119061282560;
    }

    int calculatePawnMoves(bool white) {
        unsigned long long pawnMoves = 0;
        for (int i = 0; i < 64; ++i) {
            unsigned long long square = 1;
            square <<= i;
            if ((pawnMoves & square) != 0) {
                if (white) {
                    if ((whitePieces & square) == 0) {
                        continue;
                    }
                } else {
                    if ((blackPieces & square) == 0) {
                        continue;
                    }
                }
                if ()
            }
        }
    }
};

int main() {
    Board board;
    board.initBoard();

}
