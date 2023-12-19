#include "input.h"
#include <string.h>
#include <ctype.h>


char screen[80 * 24 + 1] = "\
*---*--------------------------------------------------------------------------*\
|   |                                                                          |\
|   |                                                                          |\
|   |                                                                          |\
|   |                                                                          |\
|   |                                                                          |\
|   |                                                                          |\
|   |                                                                          |\
|   |                                                                          |\
|   |                                                                          |\
|   |                                                                          |\
| 0 |--------------------------------------------------------------------------|\
|   |                                                                          |\
|   |                                                                          |\
|   |                                                                          |\
|   |                                                                          |\
|   |                                                                          |\
|   |                                                                          |\
|   |                                                                          |\
|   |                                                                          |\
|   |                                                                          |\
*---*--------------------------------------------------------------------------*\
| >>                                                                           |\
*------------------------------------------------------------------------------*";

int exit_signal = 0;

char screen_line22[81] = "| >>                                                                           |";

void * get_input(void *) {
    unsigned char c;
    char cmd[40];
    int index;

    while(!exit_signal) {
        c = getchar();

        if (c == '\n') {
            command_t decoded_cmd = decode_cmd(cmd);
            execute_cmd(decoded_cmd);

            clear_cmd_str(cmd);
            index = 0;
        } else if (c == 127 || c == 8) {
            if (index > 0) {
                index--;
            }
            cmd[index] = 0;
        } else if (isalnum(c) || isspace(c)) {
            cmd[index] = c;
            index++;
        }

        copy_cmd(cmd);
    }
}

void clear_cmd_str(char * cmd_buffer) {
    for(int i = 0; i < sizeof(cmd_buffer); i++) {
        cmd_buffer[i] = '\0';
    }
}


void copy_cmd(char * cmd_buffer) {
    int i = 0;
    char c = cmd_buffer[i];

    memcpy(&screen[80 * 22], screen_line22, 80);
    memcpy(&screen[80 * 22 + 5], cmd_buffer, strlen(cmd_buffer));   
}

command_t decode_cmd(char * cmd_buffer) {
    char * command = strtok(cmd_buffer, " ");
    
    if (strcmp(command, "exit") == 0) {
        return EXIT;
    }

    return INVALID;
}

void execute_cmd(command_t decoded_cmd) {
    switch (decoded_cmd)
    {
    case EXIT:
        exit_signal = 1;
        break;
    
    default:
        break;
    }
}