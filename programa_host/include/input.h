#pragma once

#include <stdio.h>
#include <termios.h>
#include "screen.h"
#include "update.h"
#include "uart.h"

#define CMD_HISTORY_LENGTH 100

extern unsigned char cmdU[2];
extern unsigned char cmdT[2];
extern unsigned char cmdM[2];

extern int exit_signal;

void config_input(struct termios *, struct termios *);

void clear_cmd_str(char *);
void * get_input(void *);
void copy_cmd(char *);

int decode_cmd(char *);
