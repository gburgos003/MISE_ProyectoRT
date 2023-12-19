#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include "input.h"
#include "uart.h"
#include "screen.h"

int main()
{
    int result;
    struct termios old_tio, new_tio;
    RingBuffer data_buffer = create_ring_buffer();

    log_file = fopen("log.txt", "w");
    
    if (log_file == NULL) {
        printf("Error abriendo archivo de log\n");
        exit(1);
    }
    
    /* get the terminal settings for stdin */
    tcgetattr(STDIN_FILENO, &old_tio);

    /* we want to keep the old setting to restore them a the end */
    new_tio = old_tio;

    /* disable canonical mode (buffered i/o) and local echo */
    new_tio.c_lflag &= (~ICANON & ~ECHO);

    /* set the new settings immediately */
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    pthread_t handle_input;
    pthread_t handle_data_stream;

    result = pthread_create(&handle_input, NULL, get_input, NULL);
    result = pthread_create(&handle_data_stream, NULL, recieve_data, (void *) &data_buffer);

    for(;;) {
        if (exit_signal) {
            break;
        }
        update_screen(&data_buffer);
        clear_to_top();
        print_screen();
        usleep(refresh_rate);
    }

    pthread_join(handle_input, NULL);
    pthread_join(handle_data_stream, NULL);

    /* restore the former settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

    fclose(log_file);

    return 0;
}
