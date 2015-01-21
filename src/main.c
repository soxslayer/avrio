#include <avr/interrupt.h>
#include <avr/io.h>

#include "avrio.h"
#include "init.h"
#include "uart.h"
#include "scope_debug.h"
#include "delay.h"

int main()
{
  uint8_t baud = UART_B9600;
  fd_t uart = open(AVRIO_UART0);

  ioctl(uart, UART_IOCTL_SET_BAUD, &baud);

  sei();

  while (1) {
    write(uart, "1234567890\r\n", 12);
    _delay_us(100);
  }

  return 0;
}
