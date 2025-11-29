#include <stdio.h>
#include "graphics.h"

void graphics_draw(TetrisBoard *b, TetrisPiece *p) {
    printf("\033[2J\033[H"); // clear terminal

    for (int y = 0; y < BOARD_H; y++) {
        for (int x = 0; x < BOARD_W; x++) {
            int filled = b->cells[y][x];

            // draw current falling piece
            for (int py = 0; py < 4; py++)
                for (int px = 0; px < 4; px++)
                    if (p->shape[py][px])
                        if (p->y + py == y && p->x + px == x)
                            filled = 1;

            printf(filled ? "[]" : " .");
        }
        printf("\n");
    }
}
