#include <avr/io.h>

#include "scope_debug.h"
#include "delay.h"
#include "init.h"
#include "io.h"

static io_t sync = IO_INIT(PORTC, DDRC, PORT4);
static io_t data = IO_INIT(PORTC, DDRC, PORT5);

INIT_USR2(debug_init)
{
  io_set_dir(&sync, IO_OUT);
  io_set_dir(&data, IO_OUT);

  io_deassert(&sync);
  io_deassert(&data);
}

void debug_strobe_pin()
{
  debug_toggle_pin();
  debug_toggle_pin();
}

void debug_strobe_byte(uint8_t b)
{
  uint8_t mask;

  io_deassert(&data);
  io_deassert(&sync);

  for (mask = 0x80; mask; mask >>= 1) {
    if (b & mask)
      debug_assert_pin();
    else
      debug_deassert_pin();

    io_toggle(&sync);
  }

  io_deassert(&data);
  io_deassert(&sync);
}

void debug_assert_pin()
{
  io_assert(&data);
}

void debug_deassert_pin()
{
  io_deassert(&data);
}

void debug_toggle_pin()
{
  io_toggle(&data);
}
