#ifndef __IO_PORT__
#define __IO_PORT__

#include <stdint.h>

typedef struct _io
{
  volatile uint8_t *port;
  volatile uint8_t *ddr;
  volatile uint8_t pin;
} io_t;

#define IO_INIT(PRT,DDR,PIN) { .port = &PRT, .ddr = &DDR, .pin = PIN }

#define IO_IN 0
#define IO_OUT 1

void io_set_dir(io_t *io, uint8_t dir);
void io_assert(io_t *io);
void io_deassert(io_t *io);
void io_toggle(io_t *io);

#endif /* __IO_PORT__ */
