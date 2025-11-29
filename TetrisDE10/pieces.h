#ifndef PIECES_H
#define PIECES_H

#include "board.h"

typedef struct {
    int shape[4][4];
    int x, y;
    int type;
} TetrisPiece;

void piece_new(TetrisPiece *p, int type);
void piece_rotate(TetrisPiece *p, TetrisBoard *b);
void piece_move(TetrisBoard *b, TetrisPiece *p, int dx, int dy);
void piece_fall(TetrisBoard *b, TetrisPiece *p);
void piece_drop(TetrisBoard *b, TetrisPiece *p);

extern const int TETROMINOES[7][4][4];

#endif
