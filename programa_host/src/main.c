#include <unistd.h>
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
    data_buffer = create_ring_buffer();
    struct timeval t1, t2;
    double elapsed = 0.0;

    log_file = fopen("log.txt", "w");
    
    if (log_file == NULL) {
        printf("Error abriendo archivo de log\n");
        exit(1);
    }

    config_input(&old_tio, &new_tio);
    if (config_uart() == -1) {
    }
    
    pthread_t handle_input;
    pthread_t handle_data_stream;
    pthread_t handle_comunication;

    result = pthread_create(&handle_input, NULL, get_input, NULL);
    result = pthread_create(&handle_data_stream, NULL, recieve_data, (void *) &data_buffer);
    result = pthread_create(&handle_comunication, NULL, comunication, NULL);

    gettimeofday(&t1, NULL);
    cambiar_eje_y(3.33,GRAPH_ROWS);
    
    for(;;) {
        if (exit_signal) {
            break;
        }

        gettimeofday(&t2, NULL);
        elapsed = ((t2.tv_sec - t1.tv_sec) * 1000000) + (t2.tv_usec - t1.tv_usec);
        update(&data_buffer);

        if (elapsed >= REFRESH_RATE) {
            clear_to_top();
            print_screen();
            gettimeofday(&t1, NULL);

            //fprintf(log_file, "TOTAL ELAPSED: %lf\n", total_elapased);
        }
    }

    pthread_join(handle_input, NULL);
    pthread_join(handle_data_stream, NULL);

    /* restore the former settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

    fclose(log_file);

    return 0;
}
