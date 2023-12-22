#include "screen.h"

FILE *log_file;
char screen[PRINT_COLS * PRINT_ROWS + 1] = "\
*-------*----------------------------------------------------------------------------------------------------*\
| 10 v  |                                                                                                    |\
|       |                                                                                                    |\
|  9 v  |                                                                                                    |\
|       |                                                                                                    |\
|  8 v  |                                                                                                    |\
|       |                                                                                                    |\
|  7 v  |                                                                                                    |\
|       |                                                                                                    |\
|  6 v  |                                                                                                    |\
|       |                                                                                                    |\
|  5 v  |                                                                                                    |\
|       |                                                                                                    |\
|  4 v  |                                                                                                    |\
|       |                                                                                                    |\
|  3 v  |                                                                                                    |\
|       |                                                                                                    |\
|  2 v  |                                                                                                    |\
|       |                                                                                                    |\
|  1 v  |                                                                                                    |\
|       |                                                                                                    |\
|  0 v  |                                                                                                    |\
*-------*----------------------------------------------------------------------------------------------------*\
| Time  |^                                                                                                  ^|\
| Scale |0s                                                                                               10s|\
*-------*----------------------------------------------------------------------------------------------------*\
| >>                                                                                                         |\
*------------------------------------------------------------------------------------------------------------*";

char screen_input_line[PRINT_COLS + 1] = "\
| >>                                                                                                         |";

// int refresh_rate = 1000; // MICROSEGUNDOS

void clear_to_top(void)
{
    // clear();
    move_top_left();
}

void y_Axi_scale(float maxValue, int precision)
{
    char text[10];
    float dato;
    for (int i = 0; i < precision; i+=2)
    {
        dato = ((float) maxValue*i / (float) (precision-1));
        sprintf(text,"%1.2fV",dato);
        // for (int j=0; j < strlen(text); j++){
        //     screen[(GRAPH_ROWS -i) * PRINT_COLS + (2+j)] = text[j];
        // }
        memcpy(&screen[(GRAPH_ROWS-i) * PRINT_COLS + 2], text, strlen(text));

    }
}

void move_top_left(void)
{
    printf("\033[0;0H");
}

void print_cmd(char *cmd_buffer)
{
    memcpy(&screen[PRINT_COLS * CMD_ROW], screen_input_line, strlen(screen_input_line));
    memcpy(&screen[PRINT_COLS * CMD_ROW + 5], cmd_buffer, strlen(cmd_buffer));
}

void print_screen(void)
{
    printf("%s", screen);
    fflush(stdout);
}

void EscribirEnVentana(int fila, int columna, char valor)
{
    screen[((GRAPH_ROWS) - fila) * PRINT_COLS + columna + GRAPH_COL_OFFSET] = valor;
}
void cambiar_eje_x(working_mode_t t_s){
    char tiempo[10];
    switch (t_s)
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
    memcpy(&screen[(PRINT_ROWS - 3) * PRINT_COLS + (PRINT_COLS - 6)], tiempo, strlen(tiempo));
}

void col_pass(uint32_t value, int column)
{
    // TODO
    static int valor_anterior = 0;

    fprintf(log_file, "%d\n", value);

    for (int i = 0; i < GRAPH_ROWS; i++)
    {
        if (((i > valor_anterior) && (i < value)) || ((i < valor_anterior) && (i > value) || (i == value)))
        //if(i == value)
        {
            EscribirEnVentana(i, column, '#');
            continue;
        }

        EscribirEnVentana(i, column, ' ');
    }

    valor_anterior = value;
}
