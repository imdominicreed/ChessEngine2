#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

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

    std::string calculateBestMove(short depth) {

    }

    std::string calculatePawnMoves(short depth, bool white) {
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
                if (!white) {
                    direction = -1;
                }
                //Go Forward One
                /*possibleMove <<= (i+8*direction);
                if(((blackPieces | whitePieces) & (possibleMove))== 0) {
                    pawnMoves |= possibleMove;
                    //Go Forward 2
                    possibleMove <<= (8*direction);
                    if (((blackPieces | whitePieces) & (possibleMove)) == 0 && (white &&(i<= 15) && (i >=8) || (!white && (i<=55) && (i>=48)))) {
                        pawnMoves |= possibleMove;
                    }
                }*/
                //Attack left Diagonal
                if ((!white && ((i + 1) % 8 == 0)) || (white && (i % 8) == 0)) {
                    possibleMove = 1;
                    possibleMove <<= (i + 7 * direction);
                    if (white) {
                        if ((blackPieces & possibleMove) == 1) {
                            Board clone = *this;
                            clone.calculateBestMove(depth - 1);
                        }
                    } else {
                        if ((whitePieces & possibleMove) == 1) {
                            //pawnMoves |= possibleMove;
                        }
                    }
                }
                //Attack right Diagonal
                if ((white && ((i + 1) % 8 == 0)) || (!white && (i % 8) == 0)) {
                    possibleMove = 1;
                    possibleMove <<= (i + 9 * direction);
                    if (white) {
                        if ((blackPieces & possibleMove) == 1) {
                            //pawnMoves |= possibleMove;
                        }
                    } else {
                        if ((whitePieces & possibleMove) == 1) {
                            //pawnMoves |= possibleMove;
                        }
                    }
                }
            }
        }
    }

    std::string calculateBishopMoves(short depth, bool white) {
        unsigned long long bishopMoves = 0;
        short moves[4] = {-7, 7, 9, -9};
        for (short i = 0; i < 64; ++i) {
            unsigned long long square = 1;
            square <<= i;
            if (white) {
                if ((whitePieces & diagonalPieces & square) == 0) {
                    continue;
                }
            } else {
                if ((blackPieces & diagonalPieces & square) == 0) {
                    continue;
                }
            }
            for (short move : moves) {
                short currentSquare = i;
                while (0 <= currentSquare + move && 63 >= currentSquare + move) {
                    currentSquare += move;
                    if ((firstColumn(currentSquare) && move == 7) || (seventhColumn(currentSquare) && move == -7))
                        break;
                    unsigned long long mask = 1;
                    mask <<= currentSquare;
                    if ((mask & whitePieces) != 0) {
                        if (!white) {
                            bishopMoves |= mask;
                        }
                        break;
                    } else if ((mask & blackPieces) != 0) {
                        if (white) {
                            bishopMoves |= mask;
                        }
                        break;
                    } else {
                        bishopMoves |= mask;
                    }
                }
            }
        }
    }

    void doMove(std::string move) {
        unsigned long long piece = 1;

        piece <<= ((move.at(0) - 98)) + ((move.at(1) - 49) * 8);
        unsigned long long replacement = 1;
        replacement <<= (move.at(2) - 98) + ((move.at(3) - 49) * 8);
        if ((linearPieces & piece) != 0) {
            linearPieces ^= piece;
            deleteFromPiece(replacement);
            linearPieces |= replacement;
        } else if ((diagonalPieces & piece) != 0) {
            diagonalPieces ^= piece;
            deleteFromPiece(replacement);
            diagonalPieces |= replacement;
        } else if ((pawns & piece) != 0) {
            pawns ^= piece;
            deleteFromPiece(replacement);
            pawns |= replacement;
        }
        if ((piece & whitePieces) != 0) {
            whitePieces ^= piece;
        } else {
            blackPieces ^= piece;
        }
    }

    void deleteFromPiece(unsigned long long piece) {
        if ((linearPieces & piece) != 0) {
            linearPieces ^= piece;
        } else if ((diagonalPieces & piece) != 0) {
            diagonalPieces ^= piece;
        } else if ((pawns & piece) != 0) {
            pawns ^= piece;
        }
        if ((piece & whitePieces) != 0) {
            whitePieces ^= piece;
        } else {
            blackPieces ^= piece;
        }
    }

    unsigned long long calculateKingAttacks(bool white) {
        unsigned long long kingAttacks = 0;
        short moves[8] = {-1, 1, 7, -7, -8, 8, -9, 9};
        unsigned long long square = 1;
        short location;
        if (white) {
            square <<= whiteKing;
            location = whiteKing;
        } else {
            square <<= blackKing;
            location = blackKing;
        }
        for (short move: moves) {
            if (-1 >= (location + move) || 64 <= location + move)
                continue;
            if ((firstColumn(whiteKing) && (move == -1 || move == 9 || move == -7)) ||
                (eighthColumn(whiteKing) && (move == 1 || move == -9 || move == 7)))
                continue;
            unsigned long long mask = square;
            mask <<= move;
            if ((blackPieces & mask) != 0) {
                if (white) {
                    kingAttacks |= mask;
                }
                break;
            } else if ((whitePieces & mask) != 0) {
                if (!white) {
                    kingAttacks |= mask;
                }
                break;
            } else {
                kingAttacks |= mask;
            }
        }
        return kingAttacks;
    }

    unsigned long long calculateLinearAttacks(bool white) {
        unsigned long long rookMoves = 0;
        short moves[4] = {-8, 8, 8, -8};
        for (short i = 0; i < 64; ++i) {
            unsigned long long square = 1;
            square <<= i;
            if (white) {
                if ((whitePieces & linearPieces & square) == 0) {
                    continue;
                }
            } else {
                if ((blackPieces & linearPieces & square) == 0) {
                    continue;
                }
            }
            for (short move : moves) {
                short currentSquare = i;
                while (0 <= currentSquare + move && 63 >= currentSquare + move) {
                    currentSquare += move;
                    if ((firstColumn(currentSquare) && move == 7) || (seventhColumn(currentSquare) && move == -7))
                        break;
                    unsigned long long mask = 1;
                    mask <<= currentSquare;
                    if ((mask & whitePieces) != 0) {
                        if (!white) {
                            rookMoves |= mask;
                        }
                        break;
                    } else if ((mask & blackPieces) != 0) {
                        if (white) {
                            rookMoves |= mask;
                        }
                        break;
                    } else {
                        rookMoves |= mask;
                    }
                }
            }
        }
        return rookMoves;
    }

    unsigned long long calculateDiagonalAttacks(bool white) {
        unsigned long long bishopMoves = 0;
        short moves[4] = {-7, 7, 9, -9};
        for (short i = 0; i < 64; ++i) {
            unsigned long long square = 1;
            square <<= i;
            if (white) {
                if ((whitePieces & diagonalPieces & square) == 0) {
                    continue;
                }
            } else {
                if ((blackPieces & diagonalPieces & square) == 0) {
                    continue;
                }
            }
            for (short move : moves) {
                short currentSquare = i;
                while (0 <= currentSquare + move && 63 >= currentSquare + move) {
                    currentSquare += move;
                    if ((firstColumn(currentSquare) && move == 7) || (seventhColumn(currentSquare) && move == -7))
                        break;
                    unsigned long long mask = 1;
                    mask <<= currentSquare;
                    if ((mask & whitePieces) != 0) {
                        if (!white) {
                            bishopMoves |= mask;
                        }
                        break;
                    } else if ((mask & blackPieces) != 0) {
                        if (white) {
                            bishopMoves |= mask;
                        }
                        break;
                    } else {
                        bishopMoves |= mask;
                    }
                }
            }
        }
        return bishopMoves;
    }

    unsigned long long calculateKnightAttacks(bool white) {
        unsigned long long knightMoves = 0;
        for (int i = 0; i < 64; ++i) {
            unsigned long long square = 1;
            square <<= i;
            short moves[8] = {6, -6, 10, -10, 15, -15, 17, -17};
            unsigned long long kingMask = 1;
            if (white) {
                kingMask <<= whiteKing;
            } else {
                kingMask <<= blackKing;
            }
            if (white) {
                if (((whitePieces ^ whitePieces & (linearPieces | diagonalPieces | pawns | kingMask)) & square) ==
                    0) {
                    continue;
                }
            } else {
                if ((blackPieces & linearPieces & diagonalPieces & pawns & kingMask & square) == 0) {
                    continue;
                }
            }
            for (short move : moves) {
                if (knightException(i, move)) {
                    continue;
                }
                if (0 <= i + move && 63 >= i + move) {
                    unsigned long long knightMask = 1;
                    knightMask <<= i + move;
                    if ((knightMask & whitePieces) == 0) {
                        knightMoves |= knightMask;
                    }
                }
            }
        }
        return knightMoves;
    }

    bool knightException(int i, short move) {
        return ((firstColumn(i) && (move == -17 || move == 15 || move == 6 || move == -10) ||
                 (secondColumn(i) && (move == 6 || move == -10))) ||
                (eighthColumn(i) && (move == 17 || move == -15 || move == -6 || move == 10) ||
                 (seventhColumn(i) && (move == -6 || move == 10))));
    }

    bool firstColumn(int square) {
        return square % 8 == 0;
    }

    bool secondColumn(int square) {
        return (square - 1) % 8 == 0;
    }

    bool seventhColumn(int square) {
        return (square + 2) % 8 == 0;
    }

    bool eighthColumn(int square) {
        return (square + 1) % 8 == 0;
    }

    unsigned long long calculatePawnAttacks(bool white) {
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
                if (!white) {
                    direction = -1;
                }
                //Go Forward One
                /*possibleMove <<= (i+8*direction);
                if(((blackPieces | whitePieces) & (possibleMove))== 0) {
                    pawnMoves |= possibleMove;
                    //Go Forward 2
                    possibleMove <<= (8*direction);
                    if (((blackPieces | whitePieces) & (possibleMove)) == 0 && (white &&(i<= 15) && (i >=8) || (!white && (i<=55) && (i>=48)))) {
                        pawnMoves |= possibleMove;
                    }
                }*/
                //Attack left Diagonal
                if ((!white && ((i + 1) % 8 == 0)) || (white && (i % 8) == 0)) {
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
                if ((white && ((i + 1) % 8 == 0)) || (!white && (i % 8) == 0)) {
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
      Board clone = board;
      clone.whitePieces = 1;
      std::cout << board.whitePieces;
}
