#include "uart.h"
#include <string.h>

extern working_mode_t time_scale;
int fd_uart = 0;

int set_interface_attribs(int fd, int speed, int parity)
{
    struct termios tty;
    if (tcgetattr(fd, &tty) != 0)
    {
        fprintf(stderr, "error %d from tcgetattr", errno);
        return -1;
    }

    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    // tty.c_iflag &= ~IGNBRK; // disable break processing
    tty.c_lflag = 0;        // no signaling chars, no echo,
                            // no canonical processing
    tty.c_oflag = 0;        // no remapping, no delays
    tty.c_cc[VMIN] = 0;     // read doesn't block
    tty.c_cc[VTIME] = 5;    // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY | INLCR | ICRNL); // shut off xon/xoff ctrl //Se añaden las opciones de INLCR/ICRNL para que el dato 0xD no se transforme en 0xA

    tty.c_cflag |= (CLOCAL | CREAD);   // ignore modem controls,
                                       // enable reading
    tty.c_cflag &= ~(PARENB | PARODD); // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        fprintf(stderr, "error %d from tcsetattr", errno);
        return -1;
    }
    return 0;
}

void set_blocking(int fd, int should_block)
{
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(fd, &tty) != 0)
    {
        fprintf(stderr, "error %d from tggetattr", errno);
        return;
    }

    tty.c_cc[VMIN] = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
        fprintf(stderr, "error %d setting term attributes", errno);
}

void enviar_comando_uart(unsigned char *comando)
{
    write(fd_uart, comando, 2);
    usleep(5000);

}

int config_uart()
{
    char *portname = "/dev/ttyACM0";

    fd_uart = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd_uart < 0)
    {
        return -1;
    }
    set_interface_attribs(fd_uart, B115200, 0); // set speed to 115,200 bps, 8n1 (no parity)
    set_blocking(fd_uart, 0);                   // set no blocking

    unsigned char comando_uart_inicial[2] = {'U', 100};
    unsigned char comando_sample_time_inicial[2] = {'S', 50};
    unsigned char comando_mode_inicial[2] = {'M', 1};


    enviar_comando_uart(comando_uart_inicial);
    enviar_comando_uart(comando_sample_time_inicial);
    enviar_comando_uart(comando_mode_inicial);

    return 0;
}

void insertar_buffer(RingBuffer * buffer, unsigned char * data_buffer, int index) {
    uint16_t * data_buffer_ptr = (uint16_t *) data_buffer;
    
    //fprintf(log_file, "PALABRA: ");

    for (int i = 0; i < index / sizeof(uint16_t); i++) {
        push_ring_buffer(buffer, data_buffer_ptr[i] % 4096);

        //fprintf(log_file, "%x", data_buffer_ptr[i]);
    }
    //fprintf(log_file, "\n");

    //fflush(log_file);
}

void *recieve_data(void *buffer)
{
    // TODO
    buffer = (RingBuffer *)buffer;

    int num, index, data_ready;
    unsigned char buf_tmp[250], buf[250];
    uint32_t *p_buf;
    unsigned char c;
    estado_uart estado;

    for (;;)
    {
        if (exit_signal)
        {
            break;
        }

        num = read(fd_uart, buf_tmp, 250);

        for (int i = 0; i < num; i++)
        {
            c = buf_tmp[i];

            //fprintf(log_file, "BYTE: %x\n", c);

            switch (estado) {
                case INIT:
                    if (c == BYTE_CABECERA) {
                        estado = CABECERA;
                    }
                    break;
                case CABECERA:
                    if (c == BYTE_CABECERA) {
                        estado = DATO;
                        index = 0;
                    } else {
                        estado = INIT;
                    }
                    break;
                case DATO:
                    buf[index] = c;
                    index++;
                    if (c == BYTE_COLA) {
                        estado = COLA;
                    }
                    break;
                case COLA:
                    if (c == BYTE_COLA) {
                        estado = INIT;
                        index--;
                        insertar_buffer(buffer, buf, index);
                    } else {
                        estado = DATO;
                        buf[index] = c;
                        index++;
                    }
            }
        }
    }
}

void* comunication(void *) {
    unsigned char comando_estado[2] = {'C', 1};

    for(;;) {
        enviar_comando_uart(comando_estado);
        sleep(2);
    }
}