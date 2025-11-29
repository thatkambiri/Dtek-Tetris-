#include <stdio.h>
#include "board.h"
#include "pieces.h"
#include "graphic.h"
#include "input.h"

int main(void) {
    TetrisBoard board;
    TetrisPiece current;

    board_init(&board);
    piece_new(&current, 0);  // Start with piece type 0

    while (1) {
        InputState in = input_read();

        if (in.left)  piece_move(&board, &current, -1, 0);
        if (in.right) piece_move(&board, &current, 1, 0);
        if (in.down)  piece_drop(&board, &current);
        if (in.rotate) piece_rotate(&board, &current);

        // Make the piece fall naturally
        piece_fall(&board, &current);

        // Draw everything
        graphics_draw(&board, &current);
    }
}
