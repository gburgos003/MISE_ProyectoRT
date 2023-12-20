#pragma once

#include <stdio.h>
#include "screen.h"
#include "update.h"

typedef enum {
    INVALID = 0,
    EXIT,
} command_t;

extern int exit_signal;

void clear_cmd_str(char *);
void * get_input(void *);
void copy_cmd(char *);

command_t decode_cmd(char *);
void execute_cmd(command_t);
