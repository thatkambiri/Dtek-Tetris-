#ifndef BOARD_H
#define BOARD_H

#define BOARD_W 10
#define BOARD_H 20

typedef struct {
    int cells[BOARD_H][BOARD_W];
    int lines_cleared;
} TetrisBoard;

void board_init(TetrisBoard *b);
int board_collision(TetrisBoard *b, int shape[4][4], int bx, int by);
void board_place(TetrisBoard *b, int shape[4][4], int bx, int by);
void board_clear_lines(TetrisBoard *b);

#endif
