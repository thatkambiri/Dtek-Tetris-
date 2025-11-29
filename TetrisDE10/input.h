#ifndef INPUT_H
#define INPUT_H

typedef struct {
    int left;
    int right;
    int down;
    int rotate;
} InputState;

InputState input_read(void);

#endif
