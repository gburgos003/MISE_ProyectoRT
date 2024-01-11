#include "input.h"

int exit_signal = 0;

unsigned char cmdC[2] = {'C', 0};
unsigned char cmdU[2] = {'U', 100};
unsigned char cmdT[2] = {'T', 50};
unsigned char cmdM[2] = {'M', 1}; //3 modos, Modo 0 = 100 datos cada X uart time, Modo 1 = 1 datos cada X uart time, Modo 2 = Trigger rising
unsigned char cmdP[2] = {'P', 0}; // Unidad de tiempo usegundo por 10
unsigned char cmdD[2] = {'D', 0}; // Rango de 0-100 en %

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
    char cmd[CMD_LENGTH];
    int index;

    char cmd_historal[100][CMD_LENGTH];
    int index_historial = 0;
    int index_usuario = 0;

    while(!exit_signal) {
        c = getchar();

        if (c == '\n') {
            int cmd_status = decode_cmd(cmd);

            print_cmd_status(cmd_status);
            if (cmd_status != 1) {
                strcpy(cmd_historal[index_historial], cmd);
                index_historial++;
                index_usuario = index_historial;
            }

            clear_cmd_str(cmd);
            index = 0;
        } else if (c == 127) {
            if (index > 0) {
                index--;
            }
            cmd[index] = 0;
        } else if (isalnum(c) || isspace(c) || c == '.') {
            if (index < CMD_LENGTH) {
                index++;
            }
            cmd[index-1] = c;
        } else if (c = '\033') {
            getchar();
            char arrow = getchar();
            switch (arrow)
            {
            case 'A':
                if (index_usuario > 0) {
                    index_usuario--;
                    strcpy(cmd,cmd_historal[index_usuario]);
                }
                break;
            case 'B':
                if (index_usuario < index_historial) {
                    index_usuario++;
                    strcpy(cmd,cmd_historal[index_usuario]);
                }
                break;
            default:
                break;
            }
        }

        print_cmd(cmd);
    }
}

void clear_cmd_str(char * cmd_buffer) {
    memset(cmd_buffer, '\0', CMD_LENGTH);
}

void enviar_comando_timescale(char uart_time, char timescale, char mode, working_mode_t working_mode) {
    cmdU[1] = uart_time;
    cmdT[1] = time_scale;
    cmdM[1] = mode;

    enviar_comando_uart(cmdU);
    enviar_comando_uart(cmdT);
    enviar_comando_uart(cmdM);
    time_scale = working_mode;
}

int decode_cmd(char * cmd_buffer) {
    char command[CMD_LENGTH] = {0};
    char arg1[CMD_LENGTH] = {0};
    char arg2[CMD_LENGTH] = {0};

    if (strlen(cmd_buffer) == 0) {
        return 1;
    }

    // char * command = strtok(cmd_buffer, " ");
    sscanf(cmd_buffer, "%s %s %s", command, arg1, arg2);
    if (strcmp(command, "exit") == 0) {
        exit_signal = 1;
        cmdC[1] = 0;
        enviar_comando_uart(cmdC);

    } else if (strcmp(command, "timescale") == 0) {
        if (strcmp(arg1, "set") == 0) {
            if(strcmp(arg2, "10s") == 0) {
                enviar_comando_timescale(100, 50, 1, TS_10S);
            } else if(strcmp(arg2, "1s") == 0) {
                enviar_comando_timescale(10, 50, 1, TS_1S);
            } else if(strcmp(arg2, "100ms") == 0) {
                enviar_comando_timescale(1, 50, 1, TS_100MS);
            } else if(strcmp(arg2, "10ms") == 0) {
                enviar_comando_timescale(255, 100, 0, TS_10MS);
            } else if(strcmp(arg2, "5ms") == 0) {
                enviar_comando_timescale(255, 50, 0, TS_5MS);
            } else if(strcmp(arg2, "2.5ms") == 0) {
                enviar_comando_timescale(255, 25, 0, TS_2_5MS);
            } else {
                return -3;
            }

            vaciar_ring_buffer(&data_buffer);
            cambiar_eje_x(time_scale);
            clear_graph();
            start_col();
            fprintf(log_file, "User Command = %s %s %s, UartTime: %dms, SampleTime: %dus, Mode: %d\n", command, arg1, arg2, cmdU[1], cmdT[1], cmdM[1]);
        } else {
            return -2;
        }

    } else if (strcmp(command, "test") == 0) {
        if (strcmp(arg1, "duty") == 0) {
            int duty = atoi(arg2);
            if ((duty > 0) && (duty <= 100)){
                cmdD[1] = duty;
                enviar_comando_uart(cmdD);
                fprintf(log_file, "User Command = %s %s %s, Duty: %d%%\n", command, arg1, arg2, cmdD[1]);
            } else {
                return -3;
            }
        }
        else if(strcmp(arg1, "period") == 0){
            int period = atoi(arg2);
            if ((period > 0) && (period < 256)){
                cmdP[1] = period;
                enviar_comando_uart(cmdP);
                fprintf(log_file, "User Command = %s %s %s, Period:%dus\n", command, arg1, arg2, cmdP[1]);
            } else {
                return -3;
            }
        } else {
            return -2;
        }
    } else {
        return -1;
    }

    fflush(log_file);
    return 0;
}
