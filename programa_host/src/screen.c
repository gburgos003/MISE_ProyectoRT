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

void move_top_left(void)
{
    printf("\033[0;0H");
}

void print_cmd(char *cmd_buffer)
{
    memcpy(&screen[PRINT_COLS * 22], screen_input_line, strlen(screen_input_line));
    memcpy(&screen[PRINT_COLS * 22 + 5], cmd_buffer, strlen(cmd_buffer));
}

void print_screen(void)
{
    printf("%s", screen);
    fflush(stdout);
}

void EscribirEnVentana(int fila, int columna, char valor)
{
    screen[(fila+1) * PRINT_COLS + columna + 9] = valor;
}

void row_pass(uint32_t value, int column)
{
    // TODO
    static int valor_anterior = 0;
    value = 20 - value;

    for (int i = 0; i < 20; i++)
    {
        if ((i == value) || (valor_anterior < value) || (valor_anterior > value))
        {
            EscribirEnVentana(i, column, '#');
            continue;
        }
        EscribirEnVentana(i, column, ' ');
    }

    valor_anterior = value;
}
