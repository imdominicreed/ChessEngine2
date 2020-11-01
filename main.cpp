#include <iostream>

struct Board {
    /*
     * whitePawn Rank =65280;
     */
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
    unsigned long long calculateKnightMoves(bool white) {
        unsigned long long knightMoves = 0;
        for (int i = 0; i < 64; ++i) {
            unsigned long long square = 1;
            square <<= i;
            unsigned long long kingMask = 1;
            if(white) {
                kingMask <<= whiteKing;
            } else {
                kingMask <<= blackKing;
            }
            if (white) {
                if ((whitePieces & linearPieces & diagonalPieces & pawns & kingMask & square) == 0) {
                    continue;
                }
            } else {
                if((blackPieces & linearPieces & diagonalPieces & pawns & kingMask & square) == 0) {
                    continue;
                }
            }

        }
    }
    unsigned long long calculatePawnMoves(bool white) {
        unsigned long long pawnMoves = 0;
        for (int i = 0; i < 64; ++i) {
            unsigned long long square = 1;
            square <<= i;
            if ((pawns & square) != 0) {
                if (white) {
                    if ((whitePieces & square) == 0) {
                        continue;
                    }
                } else {
                    if ((blackPieces & square) == 0) {
                        continue;
                    }
                }
                unsigned long long possibleMove = 1;
                short direction = 1;
                if(!white) {
                    direction = -1;
                }
                //Go Forward One
                possibleMove <<= (i+8*direction);
                if(((blackPieces | whitePieces) & (possibleMove))== 0) {
                    pawnMoves |= possibleMove;
                    //Go Forward 2
                    possibleMove <<= (8*direction);
                    if (((blackPieces | whitePieces) & (possibleMove)) == 0 && (white &&(i<= 15) && (i >=8) || (!white && (i<=55) && (i>=48)))) {
                        pawnMoves |= possibleMove;
                    }
                }
                //Attack left Diagonal
                if ((!white && ((i+1)%8 == 0)) || (white && (i%8) == 0)) {
                    possibleMove = 1;
                    possibleMove <<= (i + 7 * direction);
                    if (white) {
                        if ((blackPieces & possibleMove) == 1) {
                            pawnMoves |= possibleMove;
                        }
                    } else {
                        if ((whitePieces & possibleMove) == 1) {
                            pawnMoves |= possibleMove;
                        }
                    }
                }
                //Attack right Diagonal
                if ((white && ((i+1)%8 == 0)) || (!white && (i%8) == 0)) {
                    possibleMove = 1;
                    possibleMove <<= (i + 9 * direction);
                    if (white) {
                        if ((blackPieces & possibleMove) == 1) {
                            pawnMoves |= possibleMove;
                        }
                    } else {
                        if ((whitePieces & possibleMove) == 1) {
                            pawnMoves |= possibleMove;
                        }
                    }
                }
            }
        }
        return pawnMoves;
    }
};

int main() {
    Board board;
    board.initBoard();
    std::cout << board.calculatePawnMoves(true);

}
