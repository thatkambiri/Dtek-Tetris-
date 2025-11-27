#include "system.h"      // From BSP (defines hardware base addresses)
#include "io.h"          // For IORD/IOWR macros
#include "tetris.h"
#include <stdio.h>
#include <unistd.h>      // For usleep()

// --- Hardware interface definitions ---
#define BUTTONS_BASE   BUTTON_PIO_BASE   // Replace with actual PIO name from Qsys
#define LEDS_BASE      LED_PIO_BASE      // Replace with actual PIO name from Qsys

// Polling input (simple version)
static unsigned int read_buttons(void) {
    return IORD_ALTERA_AVALON_PIO_DATA(BUTTONS_BASE);
}

static void write_leds(unsigned int value) {
    IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, value);
}

int main(void) {
    printf("=== DE10-Lite Tetris ===\n");

    TetrisGame game;
    tetris_init(&game);

    unsigned int frame = 0;

    while (1) {
        unsigned int buttons = read_buttons();

        // Map buttons to actions (example)
        if (buttons & 0x1) tetris_move_left(&game);
        if (buttons & 0x2) tetris_move_right(&game);
        if (buttons & 0x4) tetris_rotate(&game);
        if (buttons & 0x8) tetris_drop(&game);

        // Advance game
        if (frame % 20 == 0) {  // slow fall timer
            tetris_tick(&game);
        }

        // Update LEDs to show number of lines cleared (example)
        write_leds(game.lines_cleared & 0x3FF);

        // Draw frame to terminal (for debugging)
        tetris_draw(&game);

        usleep(50 * 1000); // 50 ms delay
        frame++;
    }

    return 0;
}
