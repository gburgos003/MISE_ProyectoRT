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

void cambiar_eje_y(float maxValue, int precision)
{
    char text[10];
    float dato;
    for (int i = 0; i < precision; i+=2)
    {
        dato = ((float) maxValue*i / (float) (precision-1));
        sprintf(text,"%1.2fV",dato);
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

void clear_graph(){
    for(int i = 0; i < (COLS); i++){
        for(int j = 0; j < (GRAPH_ROWS); j++){
            EscribirEnVentana(j,i,' ');
        }
    }
}


void col_pass(uint32_t value, int column)
{
    // TODO
    static int valor_anterior = 0;

    // fprintf(log_file, "%d\n", value);

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
