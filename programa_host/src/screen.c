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
*-------*-------------------------------------------*--------------------------------------------------------*\
| >>                                                |                                                        |\
*---------------------------------------------------*--------------------------------------------------------*";

char screen_input_line[PRINT_COLS + 1] = "\
| >>                                                |                                                        |";
char cmd_status_buffer[100] = {0};
char cmd[CMD_LENGTH] = {0};
clock_t time_val = {0};
int cursor = 0;

void clear_to_top(void)
{
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

void print_cmd(char *cmd_buffer, int cursor)
{
    memcpy(&screen[PRINT_COLS * CMD_ROW], screen_input_line, strlen(screen_input_line));
    memcpy(&screen[PRINT_COLS * CMD_ROW + CMD_COL], cmd_buffer, strlen(cmd_buffer));
    memcpy(&screen[PRINT_COLS * CMD_ROW + CMD_STATUS_COL], cmd_status_buffer, strlen(cmd_status_buffer));
    if ((time_val / (CLOCKS_PER_SEC / 4)) % 2 == 0) {
        screen[PRINT_COLS * CMD_ROW + CMD_COL + cursor] = '_';
    }

    time_val = clock();
}

void print_cmd_status(int status) {
    char status_msg[40]; 
    switch (status)
    {
    case 0:
        strcpy(status_msg, "OK!");
        break;
    case -1:
        strcpy(status_msg, "Comando erroneo!");
        break;
    case -2:
        strcpy(status_msg, "Argumento invalido!");
        break;
    case -3:
        strcpy(status_msg, "Valor invalido!");
        break;
    case 1:
        strcpy(status_msg, "");
    default:
        break;
    }

    strcpy(cmd_status_buffer, status_msg);
}

void print_screen(void)
{
    print_cmd(cmd, cursor);

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
    static int valor_anterior = 0;
    if (column == 0) {
        valor_anterior = value;
    }

    for (int i = 0; i < GRAPH_ROWS; i++)
    {
        if (((i > valor_anterior) && (i < value)) || ((i < valor_anterior) && (i > value) || (i == value)))
        {
            EscribirEnVentana(i, column, '#');
            continue;
        }

        EscribirEnVentana(i, column, ' ');
    }

    valor_anterior = value;
}
