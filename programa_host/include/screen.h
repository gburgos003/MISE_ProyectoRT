#pragma once

#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "ringbuffer.h"
#include "stdio.h"

#define clear() printf("\033[H\033[J")
#define COLS 70

extern char screen[80 * 24 + 1];
extern char screen_input_line[81];
extern int refresh_rate;

extern int time_scale; // MILISEGUNDOS

extern FILE * log_file;

void clear_to_top(void);
void print_cmd(char *);
void print_screen(void);
void update_screen(RingBuffer *);
void row_pass(uint32_t, int);
