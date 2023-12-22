#include "update.h"

working_mode_t time_scale = TS_1S;

void update(RingBuffer *buffer)
{
    switch (time_scale)
    {
    case TS_2_5MS:
    case TS_5MS:
    case TS_10MS:
        mode_0(buffer);
        break;

    case TS_100MS:
    case TS_1S:
    case TS_10S:
        mode_1(buffer);
        break;

    default:
        break;
    }
}

void mode_0(RingBuffer *buffer)
{
}

void mode_1(RingBuffer *buffer)
{
    static int current_col = 0;
    float value;
    uint16_t val;

    /*Leer 1 dato del buffer del UART*/
    while (pop_ring_buffer(buffer, &val) != -1)
    {
        // fprintf(log_file, "%d\n", val);
        value = ((float) val / 4095.0) * (float) (GRAPH_ROWS - 1); //0-20 unidad de medida

        col_pass(round(value), current_col);

        current_col = (current_col + 1) % COLS;
    }
}