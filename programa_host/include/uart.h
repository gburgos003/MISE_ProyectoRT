#pragma once

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include "input.h"

#define BYTE_CABECERA 0xFE
#define BYTE_COLA 0xFF

int set_interface_attribs(int, int, int);
void set_blocking(int, int);
void * recieve_data(void *);
void * comunication(void *);
void enviar_comando_uart(unsigned char *);
int config_uart();

typedef enum {
    INIT,
    CABECERA,
    DATO,
    COLA
} estado_uart;