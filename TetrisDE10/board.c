#include "board.h"

void board_init(TetrisBoard *b) {
    for (int y = 0; y < BOARD_H; y++)
        for (int x = 0; x < BOARD_W; x++)  /*Initialize the board (Height and Width). Set */ 
            b->cells[y][x] = 0;            /*the number of lines cleared to zero          */  

    b->lines_cleared = 0;
}

int board_collision(TetrisBoard *b, int shape[4][4], int bx, int by) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {                       /*We run a loop through the board and */
            if (shape[y][x]== 0) continue;                      
            int px = bx + x;                                    /*first of all check that there colissions with other blocks*/
            int py = by + y;                                    
            if (px < 0 || px >= BOARD_W || py >= BOARD_H)       /*We also check if there are collisiosn with the walls of the board*/
                return 1;
            if (py >= 0 && b->cells[py][px])
                return 1;
        }
    }
    return 0;
}

void board_place(TetrisBoard *b, int shape[4][4], int bx, int by) {
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)                                 /*We place the blocks properly taking account of other*/
            if (shape[y][x])                                        /*previously added blocks*/
                b->cells[by + y][bx + x] = 1;
}

void board_clear_lines(TetrisBoard *b) {
    for (int y = 0; y < BOARD_H; y++) {
        int full = 1;                                   /*Goes through the board and checks to fee rows that are full*/
        for (int x = 0; x < BOARD_W; x++)               /*Assumes all rows are full and uses contradiction*/
            if (!b->cells[y][x]) full = 0;

        if (full) {
            b->lines_cleared++;
            for (int yy = y; yy > 0; yy--)
                for (int x = 0; x < BOARD_W; x++)            /*If lines are full, we clear and we copy subsequent lines one by one*/
                    b->cells[yy][x] = b->cells[yy - 1][x];

            for (int x = 0; x < BOARD_W; x++)
                b->cells[0][x] = 0;                             /*We reset the topmost line to zero*/
        }
    }
}
