#pragma once

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include "input.h"

int set_interface_attribs(int, int, int);
void set_blocking(int, int);
void * recieve_data(void *);