#pragma once

#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "ringbuffer.h"
#include "stdio.h"

#define clear() printf("\033[H\033[J")

#define COLS 100
#define PRINT_COLS 110
#define PRINT_ROWS 28
#define CMD_ROW (PRINT_ROWS - 2)
#define GRAPH_ROWS 21
#define GRAPH_ROW_OFFSET 1
#define GRAPH_COL_OFFSET 9
#define REFRESH_RATE 10000 // uSecs

extern char screen[PRINT_COLS * PRINT_ROWS + 1];
extern char screen_input_line[PRINT_COLS + 1];
extern int refresh_rate;


extern FILE * log_file;

void clear_to_top(void);
void move_top_left(void);
void print_cmd(char *);
void print_screen(void);
void y_Axi_scale(float maxValue, int precision);
void col_pass(uint32_t, int);
