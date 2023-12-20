#include "input.h"

int exit_signal = 0;

void config_input(struct termios* old_tio, struct termios* new_tio) {
    /* get the terminal settings for stdin */
    tcgetattr(STDIN_FILENO, &old_tio);

    /* we want to keep the old setting to restore them a the end */
    new_tio = old_tio;

    /* disable canonical mode (buffered i/o) and local echo */
    new_tio->c_lflag &= (~ICANON & ~ECHO);

    /* set the new settings immediately */
    tcsetattr(STDIN_FILENO, TCSANOW, new_tio);
}

void * get_input(void *) {
    char c;
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
    char command[10] = {0};
    char arg1[10] = {0};
    char arg2[10] = {0};
    // char * command = strtok(cmd_buffer, " ");
    sscanf(cmd_buffer, "%s %s %s", command, arg1, arg2);
    if (strcmp(command, "exit") == 0) {
        return EXIT;
    }

    if (strcmp(command, "test") == 0) {
        time_scale = 200;
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