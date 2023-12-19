#include "input.h"

int exit_signal = 0;


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

        print_cmd(cmd);
    }
}

void clear_cmd_str(char * cmd_buffer) {
    for(int i = 0; i < sizeof(cmd_buffer); i++) {
        cmd_buffer[i] = '\0';
    }
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