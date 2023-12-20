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
    tty.c_iflag &= ~IGNBRK; // disable break processing
    tty.c_lflag = 0;        // no signaling chars, no echo,
                            // no canonical processing
    tty.c_oflag = 0;        // no remapping, no delays
    tty.c_cc[VMIN] = 0;     // read doesn't block
    tty.c_cc[VTIME] = 5;    // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

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

void enviar_comando_uart(char * comando)
{
    write(fd_uart,comando, strlen(comando));
}

void config_uart()
{
    char *portname = "/dev/ttyACM0";

    fd_uart = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd_uart < 0)
    {
        error_message("error %d opening %s: %s", errno, portname, strerror(errno));
        return;
    }

    set_interface_attribs(fd_uart, B115200, 0); // set speed to 115,200 bps, 8n1 (no parity)
    set_blocking(fd_uart, 0);                   // set no blocking
}

void *recieve_data(void *buffer)
{
    // TODO
    buffer = (RingBuffer *)buffer;

    int val = 2048;
    int num, index, data_ready;
    unsigned char buf_tmp[100],buf[100];
    uint32_t * p_buf;

    for (;;)
    {
        if (exit_signal)
        {
            break;
        }

        num = read(fd_uart, buf_tmp, 100);

        for (int i = 0; i < num; i++)
        {
            if (buf_tmp[i] == 0xFE)
            {
                index = 0;
                data_ready = 0;
            }
            else if (buf_tmp[i] == 0xFF)
            {
                data_ready = 1;
            }
            else
            {
                if (index > 100) //Maximo buf
                {
                    printf("Demasiados datos:\n");
                    index = 0;
                    data_ready = 0;
                }
                else //Rellenar buf
                {
                    buf[index] = buf_tmp[i];
                    index++;
                }
            }

            if (data_ready)
            {
                data_ready = 0;
                p_buf = (uint32_t *) buf;

                for (int i = 0; i < (index/sizeof(uint32_t)); i++)
                {
                    push_ring_buffer(buffer, p_buf[i] % 4096);  
                }

            }

    }
}
