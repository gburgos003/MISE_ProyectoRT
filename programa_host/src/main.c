#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include "input.h"

#define clear() printf("\033[H\033[J")

void clear_to_top(void);
void print_screen(void);
void update_screen(void);

int main()
{
    int result;

    struct termios old_tio, new_tio;

    /* get the terminal settings for stdin */
    tcgetattr(STDIN_FILENO, &old_tio);

    /* we want to keep the old setting to restore them a the end */
    new_tio = old_tio;

    /* disable canonical mode (buffered i/o) and local echo */
    new_tio.c_lflag &= (~ICANON & ~ECHO);

    /* set the new settings immediately */
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    // init_screen(screen);

    pthread_t hilo_lectura;

    result = pthread_create(&hilo_lectura, NULL, get_input, NULL);

    for(;;) {
        if (exit_signal) {
            break;
        }
        update_screen();
        clear_to_top();
        print_screen();
        usleep(20000);
    }

    pthread_join(hilo_lectura, NULL);

    /* restore the former settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

    return 0;
}

void clear_to_top(void) {
    clear();
    fflush(stdout);
}

void print_screen(void) {
    printf("%s", screen);
    fflush(stdout);
}

void update_screen() {
    static char caracter = '1';
    static char index = 0;

    screen[500] = caracter + index;

    index = (++index) % 10;
}