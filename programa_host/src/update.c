#include "update.h"

working_mode_t time_scale = TS_100MS;

void update(RingBuffer * buffer, double elapsed) {
    switch (time_scale)
    {
    case TS_100US:
    case TS_1MS:
    case TS_10MS:
        mode_0(buffer, elapsed);
        break;

    case TS_100MS:
    case TS_1S:
    case TS_10S:
        mode_1(buffer, elapsed);
        break;

    default:
        break;
    }

}

void mode_0(RingBuffer * buffer, double elapsed) {

}

void mode_1(RingBuffer * buffer, double elapsed) {
    static int current_col = 0;
    static int number_of_frames = 0;
    static int prev_col = 0;

    uint32_t val, display_val = 0, iters = 0;
    int ret = 0;

    float time_per_col_millis = (time_scale / COLS) / 1000;

    // fprintf(log_file, "TPC ms %f\n", time_per_col_millis);

    // current_col = ((int) ((number_of_frames * REFRESH_RATE / 1000) / time_per_col_millis) % COLS) + 9;
    current_col = ((int) ((elapsed / 1000) / time_per_col_millis) % COLS) + 9;

    while (pop_ring_buffer(buffer, &val) != -1)
    {
        display_val = val;
        iters++;
    }
    
    if (iters == 0) {
        return;
    }

    // if (prev_col + 2 >= current_col) {
    //     fprintf(log_file, "PREV: %d ACTUAL :%d\n", prev_col, current_col);
    // }

    prev_col = current_col;
    // if (iters > 0) {
    //     display_val /= iters;
    // }
    
    float media_float = (float) display_val;
    // float media_float = 4095;
    int value = (int) ((media_float / 4095.0) * 20.0);
    value = 20 - value;

    row_pass(value, current_col);

    number_of_frames++;
}