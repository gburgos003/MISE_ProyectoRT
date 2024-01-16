#pragma once

#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>

#include "ringbuffer.h"
#include "stdio.h"

#define clear() printf("\033[H\033[J")

#define COLS 100
#define PRINT_COLS 110
#define PRINT_ROWS 28
#define CMD_ROW (PRINT_ROWS - 2)
#define CMD_COL 5
#define CMD_STATUS_COL 54
#define GRAPH_ROWS 21
#define GRAPH_ROW_OFFSET 1
#define GRAPH_COL_OFFSET 9
#define REFRESH_RATE 10000 // uSecs
#define CMD_LENGTH 41

extern char screen[PRINT_COLS * PRINT_ROWS + 1];
extern char screen_input_line[PRINT_COLS + 1];
extern char cmd[CMD_LENGTH];
extern char cmd_status_buffer[100];
extern int refresh_rate;
extern int cursor;

extern FILE * log_file;

extern clock_t time_val;

void clear_to_top(void);
void clear_graph();
void move_top_left(void);
void print_cmd(char *, int);
void print_cmd_status(int);
void print_screen(void);
void cambiar_eje_y(float, int);
void col_pass(uint32_t, int);
