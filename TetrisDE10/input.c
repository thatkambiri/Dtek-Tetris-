#include <stdio.h>
#include "input.h"

// Very simple placeholder input (keyboard in UART terminal)
InputState input_read(void) {
    InputState in = {0};

    int c = getchar_nonblock(); // use your dtekv-lib version

    if (c == 'a') in.left = 1;
    if (c == 'd') in.right = 1;
    if (c == 's') in.down = 1;
    if (c == 'w') in.rotate = 1;

    return in;
}
