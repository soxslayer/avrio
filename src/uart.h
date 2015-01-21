#ifndef __UART__
#define __UART__

#include "avrio.h"

#define UART_BUFF_SIZE 8

typedef struct _uart_device
{
  avrio_device_t dev;

  volatile uint8_t tx_read;
  volatile uint8_t tx_write;
  uint8_t tx_buff[UART_BUFF_SIZE];

  volatile uint8_t rx_read;
  volatile uint8_t rx_write;
  uint8_t rx_buff[UART_BUFF_SIZE];

  uint8_t flags;
} uart_device_t;

#define UART_B2400 0
#define UART_B4800 1
#define UART_B9600 2
#define UART_B14400 3
#define UART_B19200 4
#define UART_B28800 5
#define UART_B38400 6
#define UART_B57600 7
#define UART_B76800 8
#define UART_B115200 9
#define UART_B230400 10
#define UART_B250000 11
#define UART_B500000 12
#define UART_B1000000 13

void uart_init();

int8_t uart_open(avrio_device_t *device);
void uart_close(avrio_device_t *device);
int16_t uart_read(avrio_device_t *device, void *dest, uint8_t size);
int16_t uart_write(avrio_device_t *device, const void *src, uint8_t size);
int8_t uart_ioctl(avrio_device_t *device, uint8_t request, void *data);

// UART flags
#define UART_NON_BLOCKING 1

// ioctl commands
#define UART_IOCTL_SET_BAUD 0

#endif /* __UART__ */
