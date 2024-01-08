#include "input.h"

int exit_signal = 0;

unsigned char cmdU[2] = {'U', 100};
unsigned char cmdT[2] = {'T', 50};
unsigned char cmdM[2] = {'M', 1}; //3 modos, Modo 0 = 100 datos cada X uart time, Modo 1 = 1 datos cada X uart time, Modo 2 = Trigger rising

void config_input(struct termios * old_tio, struct termios * new_tio) {
    /* get the terminal settings for stdin */
    tcgetattr(STDIN_FILENO, old_tio);

    /* we want to keep the old setting to restore them a the end */
    *new_tio = *old_tio;

    /* disable canonical mode (buffered i/o) and local echo */
    new_tio->c_lflag &= (~ICANON & ~ECHO);

    /* set the new settings immediately */
    tcsetattr(STDIN_FILENO, TCSANOW, new_tio);
}

void * get_input(void *) {
    char c;
    char cmd[100];
    int index;

    while(!exit_signal) {
        c = getchar();

        // fprintf(log_file, "%c\n", c);

        if (c == '\n') {
            int cmd_status = decode_cmd(cmd);
            // TODO Hacer algo con este status

            clear_cmd_str(cmd);
            index = 0;
        } else if (c == 127 || c == 8) {
            if (index > 0) {
                index--;
            }
            cmd[index] = 0;
        } else if (isalnum(c) || isspace(c) || c == '.') {
            cmd[index] = c;
            if (index < sizeof(cmd)) {
                index++;
            }
        }

        print_cmd(cmd);
    }
}

void clear_cmd_str(char * cmd_buffer) {
    memset(cmd_buffer, '\0', 100);
}

int decode_cmd(char * cmd_buffer) {
    char command[10] = {0};
    char arg1[10] = {0};
    char arg2[10] = {0};

    // char * command = strtok(cmd_buffer, " ");
    sscanf(cmd_buffer, "%s %s %s", command, arg1, arg2);
    if (strcmp(command, "exit") == 0) {
        exit_signal = 1;
        return 0;
    }

    if (strcmp(command, "timescale") == 0) {
        if (strcmp(arg1, "set") == 0) {
            if(strcmp(arg2, "10s") == 0) {
                cmdU[1] = 100;
                cmdT[1] = 50;
                cmdM[1] = 1;

                fprintf(log_file, "CMDU: %d %d\nCMDT: %d %d\n", cmdU[0], cmdU[1], cmdT[0], cmdT[1]);

                enviar_comando_uart(cmdU);
                enviar_comando_uart(cmdT);
                enviar_comando_uart(cmdM);
                time_scale = TS_10S;
            } else if(strcmp(arg2, "1s") == 0) {
                cmdU[1] = 10;
                cmdT[1] = 50;
                cmdM[1] = 1;

                enviar_comando_uart(cmdU);
                enviar_comando_uart(cmdT);
                enviar_comando_uart(cmdM);
                time_scale = TS_1S;
            } else if(strcmp(arg2, "100ms") == 0) {
                cmdU[1] = 1;
                cmdT[1] = 50;
                cmdM[1] = 1;

                enviar_comando_uart(cmdU);
                enviar_comando_uart(cmdT);
                enviar_comando_uart(cmdM);
                time_scale = TS_100MS;
            } else if(strcmp(arg2, "10ms") == 0) {
                cmdU[1] = 255;
                cmdT[1] = 100;
                cmdM[1] = 0;

                enviar_comando_uart(cmdU);
                enviar_comando_uart(cmdT);
                enviar_comando_uart(cmdM);
                time_scale = TS_10MS;
            } else if(strcmp(arg2, "5ms") == 0) {
                cmdU[1] = 255;
                cmdT[1] = 50;
                cmdM[1] = 0;

                enviar_comando_uart(cmdU);
                enviar_comando_uart(cmdT);
                enviar_comando_uart(cmdM);
                time_scale = TS_5MS;
            } else if(strcmp(arg2, "2.5ms") == 0) {
                cmdU[1] = 255;
                cmdT[1] = 25;
                cmdM[1] = 0;

                enviar_comando_uart(cmdU);
                enviar_comando_uart(cmdT);
                enviar_comando_uart(cmdM);
                time_scale = TS_2_5MS;
            } else {
                return -1;
            }

            vaciar_ring_buffer(&data_buffer);
            cambiar_eje_x(time_scale);
            clear_graph();
            start_col();
        } else {
            return -1;
        }
    }


    return -1;
}
