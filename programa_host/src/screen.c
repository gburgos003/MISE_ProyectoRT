#include "screen.h"

FILE * log_file;
char screen[80 * 24 + 1] = "\
*-------*----------------------------------------------------------------------*\
| 10 v  |                                                                      |\
|       |                                                                      |\
|  9 v  |                                                                      |\
|       |                                                                      |\
|  8 v  |                                                                      |\
|       |                                                                      |\
|  7 v  |                                                                      |\
|       |                                                                      |\
|  6 v  |                                                                      |\
|       |                                                                      |\
|  5 v  |                                                                      |\
|       |                                                                      |\
|  4 v  |                                                                      |\
|       |                                                                      |\
|  3 v  |                                                                      |\
|       |                                                                      |\
|  2 v  |                                                                      |\
|       |                                                                      |\
|  1 v  |                                                                      |\
|       |                                                                      |\
*-------*----------------------------------------------------------------------*\
| >>                                                                           |\
*------------------------------------------------------------------------------*";

char screen_input_line[81] = "\
| >>                                                                           |";

int time_scale = 1000;
int refresh_rate = 10000; // MICROSEGUNDOS

void clear_to_top(void) {
    clear();
    fflush(stdout);
}

void print_cmd(char * cmd_buffer) {
    memcpy(&screen[80 * 22], screen_input_line, 80);
    memcpy(&screen[80 * 22 + 5], cmd_buffer, strlen(cmd_buffer));   
}

void print_screen(void) {
    printf("%s", screen);
    fflush(stdout);
}

void update_screen(RingBuffer * buffer) {
    static int current_col = 0;
    static int number_of_frames = 0;

    uint32_t val, display_val = 0, iters = 0;
    int ret = 0;

    float time_per_col_millis = time_scale / COLS;
    current_col = (int) ((number_of_frames * refresh_rate / 1000) / time_per_col_millis) % 70;

    // fprintf(log_file, "%d\n", current_col);

    while (pop_ring_buffer(buffer, &val) != -1)
    {
        display_val = val;
        iters++;
    }
    
    // if (iters > 0) {
    //     display_val /= iters;
    // }
    
    float media_float = (float) display_val;
    int reversed_row = (int) ((media_float / 4095.0) * 20.0);
    int value = 20 - reversed_row;

    row_pass(value, current_col);

    // screen[row * 80 + (current_col + 9)] = '#';


    number_of_frames++;
    // memcpy(screen, test, strlen(test));
}

void row_pass(uint32_t value, int column) {
    // TODO
    for (int i = 0; i < 19; i++) {
        if (i == value) {
            screen[(i + 1) * 80 + (column + 9)] = '#';
            continue;
        }

        screen[(i + 1) * 80 + (column + 9)] = ' ';
    }
}