#include "tetris.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int tetromino_shapes[7][4][4] = {
    // I
    {{0,0,0,0},
     {1,1,1,1},
     {0,0,0,0},
     {0,0,0,0}},
    // O
    {{1,1,0,0},
     {1,1,0,0},
     {0,0,0,0},
     {0,0,0,0}},
    // T
    {{0,1,0,0},
     {1,1,1,0},
     {0,0,0,0},
     {0,0,0,0}},
    // L
    {{0,0,1,0},
     {1,1,1,0},
     {0,0,0,0},
     {0,0,0,0}},
    // J
    {{1,0,0,0},
     {1,1,1,0},
     {0,0,0,0},
     {0,0,0,0}},
    // S
    {{0,1,1,0},
     {1,1,0,0},
     {0,0,0,0},
     {0,0,0,0}},
    // Z
    {{1,1,0,0},
     {0,1,1,0},
     {0,0,0,0},
     {0,0,0,0}}
};

static void new_tetromino(TetrisGame *g) {
    int type = rand() % 7;
    memcpy(g->current.shape, tetromino_shapes[type], sizeof(int) * 16);
    g->current.x = BOARD_WIDTH / 2 - 2;
    g->current.y = 0;
}

void tetris_init(TetrisGame *g) {
    memset(g->board, 0, sizeof(g->board));
    g->lines_cleared = 0;
    new_tetromino(g);
}

static int check_collision(TetrisGame *g, Tetromino *t) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (!t->shape[y][x]) continue;
            int bx = t->x + x;
            int by = t->y + y;
            if (bx < 0 || bx >= BOARD_WIDTH || by >= BOARD_HEIGHT)
                return 1;
            if (by >= 0 && g->board[by][bx])
                return 1;
        }
    }
    return 0;
}

static void place_tetromino(TetrisGame *g) {
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            if (g->current.shape[y][x]) {
                int bx = g->current.x + x;
                int by = g->current.y + y;
                if (by >= 0 && by < BOARD_HEIGHT)
                    g->board[by][bx] = 1;
            }
}

static void clear_lines(TetrisGame *g) {
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        int full = 1;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (!g->board[y][x]) {
                full = 0;
                break;
            }
        }
        if (full) {
            g->lines_cleared++;
            for (int yy = y; yy > 0; yy--)
                memcpy(g->board[yy], g->board[yy - 1], sizeof(g->board[0]));
            memset(g->board[0], 0, sizeof(g->board[0]));
        }
    }
}

void tetris_tick(TetrisGame *g) {
    g->current.y++;
    if (check_collision(g, &g->current)) {
        g->current.y--;
        place_tetromino(g);
        clear_lines(g);
        new_tetromino(g);
    }
}

void tetris_move_left(TetrisGame *g) {
    g->current.x--;
    if (check_collision(g, &g->current)) g->current.x++;
}

void tetris_move_right(TetrisGame *g) {
    g->current.x++;
    if (check_collision(g, &g->current)) g->current.x--;
}

void tetris_rotate(TetrisGame *g) {
    Tetromino tmp = g->current;
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            tmp.shape[y][x] = g->current.shape[3 - x][y];
    if (!check_collision(g, &tmp))
        g->current = tmp;
}

void tetris_drop(TetrisGame *g) {
    while (!check_collision(g, &g->current))
        g->current.y++;
    g->current.y--;
}

void tetris_draw(TetrisGame *g) {
    // Simple terminal visualization (for debug)
    printf("\033[H"); // Move cursor to top-left
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            int filled = g->board[y][x];
            for (int ty = 0; ty < 4; ty++)
                for (int tx = 0; tx < 4; tx++)
                    if (g->current.shape[ty][tx] &&
                        g->current.y + ty == y &&
                        g->current.x + tx == x)
                        filled = 1;
            printf(filled ? "[]" : " .");
        }
        printf("\n");
    }
    printf("Lines cleared: %d\n", g->lines_cleared);
}
