#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include <sys/time.h>
#include "input.h"
#include "uart.h"
#include "screen.h"
#include "update.h"

int main()
{
    int result;
    struct termios old_tio, new_tio;
    RingBuffer data_buffer = create_ring_buffer();
    struct timeval t1, t2, tstart;
    double elapsed = 0.0, total_elapased = 0.0;

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

    gettimeofday(&t1, NULL);
    tstart = t1;
    
    for(;;) {
        if (exit_signal) {
            break;
        }

        // update(&data_buffer, total_elapased);

        gettimeofday(&t2, NULL);
        elapsed = ((t2.tv_sec - t1.tv_sec) * 1000000) + (t2.tv_usec - t1.tv_usec);
        total_elapased += ((t2.tv_sec - tstart.tv_sec) * 1000000) + (t2.tv_usec - tstart.tv_usec);
        update(&data_buffer, total_elapased);

        if (elapsed >= REFRESH_RATE) {
            clear_to_top();
            print_screen();
            gettimeofday(&t1, NULL);

            fprintf(log_file, "TOTAL ELAPSED: %lf\n", total_elapased);
        }
    }

    pthread_join(handle_input, NULL);
    pthread_join(handle_data_stream, NULL);

    /* restore the former settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

    fclose(log_file);

    return 0;
}
