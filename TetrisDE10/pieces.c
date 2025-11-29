#include "pieces.h"

const int TETROMINOES[7][4][4] = {
    {{0,0,0,0},
     {1,1,1,1},
     {0,0,0,0},
     {0,0,0,0}}, // I

    {{1,1,0,0},
     {1,1,0,0},
     {0,0,0,0},
     {0,0,0,0}}, // O

    {{1,1,1,0},
     {0,1,0,0},
     {0,0,0,0},
     {0,0,0,0}}, // T

    {{1,0,0,0},
     {1,1,1,0},
     {0,0,0,0},
     {0,0,0,0}}, // L

    {{0,0,1,0},
     {1,1,1,0},
     {0,0,0,0},
     {0,0,0,0}}, // J

    {{0,1,1,0},
     {1,1,0,0},
     {0,0,0,0},
     {0,0,0,0}}, // S

    {{1,1,0,0},
     {0,1,1,0},
     {0,0,0,0},
     {0,0,0,0}}  // Z
};

void piece_new(TetrisPiece *p, int type) {
    p->type = type;
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            p->shape[y][x] = TETROMINOES[type][y][x];   /*Spawn a block type and let it fall */

    p->x = BOARD_W / 2 - 2;                                /*Centre the fall properly*/
    p->y = 0;
}

void piece_rotate(TetrisPiece *p, TetrisBoard *b) {
    int temp[4][4];
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            temp[x][3 - y] = p->shape[y][x];

    if (!board_collision(b, temp, p->x, p->y)) {
        for (int y = 0; y < 4; y++)
            for (int x = 0; x < 4; x++)
                p->shape[y][x] = temp[y][x];
    }
}

void piece_move(TetrisBoard *b, TetrisPiece *p, int dx, int dy) {
    if (!board_collision(b, p->shape, p->x + dx, p->y + dy)) {
        p->x += dx;
        p->y += dy;
    }
}

void piece_fall(TetrisBoard *b, TetrisPiece *p) {
    if (board_collision(b, p->shape, p->x, p->y + 1)) {
        board_place(b, p->shape, p->x, p->y);
        board_clear_lines(b);
        piece_new(p, (p->type + 1) % 7);
    } else {
        p->y++;
    }
}

void piece_drop(TetrisBoard *b, TetrisPiece *p) {
    while (!board_collision(b, p->shape, p->x, p->y + 1)) {
        p->y++;
    }
}
