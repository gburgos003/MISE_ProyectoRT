#include "update.h"

working_mode_t time_scale = TS_1S;

void update(RingBuffer *buffer)
{
    switch (time_scale)
    {
    case TS_2_5MS:
    case TS_5MS:
    case TS_10MS:
        mode_1(buffer);
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
    uint16_t val = 0;
    int index = 0;

    while (pop_ring_buffer(buffer, &val) != -1)
    {
        
    }
    
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

void cambiar_eje_x(working_mode_t t_scale){
    char tiempo[10];
    switch (t_scale)
    {
    case TS_2_5MS:
        memcpy(tiempo,"2.5ms",6);
        break;
    case TS_5MS:
        memcpy(tiempo,"  5ms",6);
        break;
    case TS_10MS:
        memcpy(tiempo," 10ms",6);
        break;
    case TS_100MS:
        memcpy(tiempo,"100ms",6);
        break;
    case TS_1S:
        memcpy(tiempo,"   1s",6);
        break;
    case TS_10S:
        memcpy(tiempo,"  10s",6);
        break;
    
    default:
        break;
    }
    // sprintf(tiempo,"%5d",t_s/1000000);
    memcpy(&screen[(PRINT_ROWS - 4) * PRINT_COLS + (PRINT_COLS - 6)], tiempo, strlen(tiempo));
}