#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>

#include "uart.h"
#include "init.h"
#include "stddef.h"

int8_t uart_open(avrio_device_t *device);
void uart_close(avrio_device_t *device);
int16_t uart_read(avrio_device_t *device, void *dest, uint8_t size);
int16_t uart_write(avrio_device_t *device, const void *src, uint8_t size);
int8_t uart_ioctl(avrio_device_t *device, uint8_t request, void *data);

uart_device_t uart0 = { .dev = {
  .device = AVRIO_UART0,
  .open = uart_open,
  .close = uart_close,
  .read = uart_read,
  .write = uart_write,
  .ioctl = uart_ioctl
}};

#define UART_BAUD_ARRAY_SIZE ((UART_B1000000) + 1)

#define UART_USE_2X_FLAG 0x1
#define UART_VALID_FLAG 0x2

const uint8_t baud_rates[UART_BAUD_ARRAY_SIZE][3] PROGMEM = {
#define BAUD 2400
#include "baud.h"
  { UBRR_VALID << 1 | USE_2X, UBRRH_VALUE, UBRRL_VALUE },
#undef BAUD

#define BAUD 4800
#include "baud.h"
  { UBRR_VALID << 1 | USE_2X, UBRRH_VALUE, UBRRL_VALUE },
#undef BAUD

#define BAUD 9600
#include "baud.h"
  { UBRR_VALID << 1 | USE_2X, UBRRH_VALUE, UBRRL_VALUE },
#undef BAUD

#define BAUD 14400
#include "baud.h"
  { UBRR_VALID << 1 | USE_2X, UBRRH_VALUE, UBRRL_VALUE },
#undef BAUD

#define BAUD 19200
#include "baud.h"
  { UBRR_VALID << 1 | USE_2X, UBRRH_VALUE, UBRRL_VALUE },
#undef BAUD

#define BAUD 28800
#include "baud.h"
  { UBRR_VALID << 1 | USE_2X, UBRRH_VALUE, UBRRL_VALUE },
#undef BAUD

#define BAUD 38400
#include "baud.h"
  { UBRR_VALID << 1 | USE_2X, UBRRH_VALUE, UBRRL_VALUE },
#undef BAUD

#define BAUD 57600
#include "baud.h"
  { UBRR_VALID << 1 | USE_2X, UBRRH_VALUE, UBRRL_VALUE },
#undef BAUD

#define BAUD 76800
#include "baud.h"
  { UBRR_VALID << 1 | USE_2X, UBRRH_VALUE, UBRRL_VALUE },
#undef BAUD

#define BAUD 115200
#include "baud.h"
  { UBRR_VALID << 1 | USE_2X, UBRRH_VALUE, UBRRL_VALUE },
#undef BAUD

#define BAUD 230400
#include "baud.h"
  { UBRR_VALID << 1 | USE_2X, UBRRH_VALUE, UBRRL_VALUE },
#undef BAUD

#define BAUD 250000
#include "baud.h"
  { UBRR_VALID << 1 | USE_2X, UBRRH_VALUE, UBRRL_VALUE },
#undef BAUD

#define BAUD 500000
#include "baud.h"
  { UBRR_VALID << 1 | USE_2X, UBRRH_VALUE, UBRRL_VALUE },
#undef BAUD

#define BAUD 1000000
#include "baud.h"
  { UBRR_VALID << 1 | USE_2X, UBRRH_VALUE, UBRRL_VALUE }
#undef BAUD
};

static int8_t uart_set_baud(uart_device_t *uart, uint8_t baud)
{
  uint8_t flags, ubrrh, ubrrl;

  if (baud >= UART_BAUD_ARRAY_SIZE)
    return -1;

  flags = pgm_read_byte(&baud_rates[baud][0]);

  if (!(flags & UART_VALID_FLAG))
    return -1;

  ubrrh = pgm_read_byte(&baud_rates[baud][1]);
  ubrrl = pgm_read_byte(&baud_rates[baud][2]);

  UBRR0H = ubrrh;
  UBRR0L = ubrrl;

  if (flags & UART_USE_2X_FLAG)
    UCSR0A |= _BV(U2X0);
  else
    UCSR0A &= ~_BV(U2X0);

  return 0;
}

INIT_USR3(uart_init)
{
  avrio_register(&uart0.dev);
}

int8_t uart_open(avrio_device_t *device)
{
  uart_device_t *uart_dev = container_of(device, uart_device_t, dev);

  uart_dev->tx_read = 0;
  uart_dev->tx_write = 0;

  uart_dev->rx_read = 0;
  uart_dev->rx_write = 0;

  uart_dev->flags = 0;

  UCSR0B |= _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0);

  return 0;
}

void uart_close(avrio_device_t *device)
{
  UCSR0B &= ~(_BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0) | _BV(UDRIE0));
}

int16_t uart_read(avrio_device_t *device, void *dest, uint8_t size)
{
  uart_device_t *uart_dev = container_of(device, uart_device_t, dev);
  char *buff = (char *)dest;
  int16_t n_read = 0;

  while (size > 0) {
    if ((uart_dev->flags & UART_NON_BLOCKING)
        && (uart_dev->rx_read == uart_dev->rx_write))
      break;

    while (uart_dev->rx_read == uart_dev->rx_write) ;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
      buff[n_read] = uart_dev->rx_buff[uart_dev->rx_read];
      uart_dev->rx_read = (uart_dev->rx_read + 1) % UART_BUFF_SIZE;
    }

    ++n_read;
    --size;
  }

  return n_read;
}

int16_t uart_write(avrio_device_t *device, const void *src, uint8_t size)
{
  uart_device_t *uart_dev = container_of(device, uart_device_t, dev);
  char *buff = (char *)src;
  int16_t n_write = 0;

  while (size > 0) {
    uint8_t next_write = (uart_dev->tx_write + 1) % UART_BUFF_SIZE;

    if ((uart_dev->flags & UART_NON_BLOCKING)
        && (uart_dev->tx_read == next_write))
      break;

    while (uart_dev->tx_read == next_write)
      next_write = (uart_dev->tx_write + 1) % UART_BUFF_SIZE;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
      uart_dev->tx_buff[uart_dev->tx_write] = buff[n_write];
      uart_dev->tx_write = next_write;
    }

    ++n_write;
    --size;

    UCSR0B |= _BV(UDRIE0);
  }

  return n_write;
}

int8_t uart_ioctl(avrio_device_t *device, uint8_t request, void *data)
{
  uart_device_t *uart_dev = container_of(device, uart_device_t, dev);

  if (request == UART_IOCTL_SET_BAUD) {
    uint8_t baud = *((uint8_t *)data);
    return uart_set_baud(uart_dev, baud);
  }

  return -1;
}

void uart_udre_vect_bottom(uart_device_t *uart_dev)
{
  if (uart_dev->tx_read == uart_dev->tx_write) {
    UCSR0B &= ~_BV(UDRIE0);
    return;
  }

  UDR0 = uart_dev->tx_buff[uart_dev->tx_read];
  uart_dev->tx_read = (uart_dev->tx_read + 1) % UART_BUFF_SIZE;
}

void uart_rx_vect_bottom(uart_device_t *uart_dev)
{
  uint8_t tmp_write = (uart_dev->rx_write + 1) % UART_BUFF_SIZE;

  if (tmp_write == uart_dev->rx_read) {
    uart_dev->rx_read = (uart_dev->rx_read + 1) % UART_BUFF_SIZE;
    /* Set error condition for overflow here */
  }

  uart_dev->rx_buff[uart_dev->rx_write] = UDR0;
  uart_dev->rx_write = tmp_write;
}

ISR(USART_UDRE_vect)
{
  uart_udre_vect_bottom(&uart0);
}

ISR(USART_RX_vect)
{
  uart_rx_vect_bottom(&uart0);
}
