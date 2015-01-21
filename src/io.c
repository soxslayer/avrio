#include "io.h"
#include "util.h"

void io_set_dir(io_t *io, uint8_t dir)
{
  if (!io)
    return;

  if (dir)
    set_bit(*(io->ddr), io->pin);
  else
    clr_bit(*(io->ddr), io->pin);
}

void io_assert(io_t *io)
{
  if (!io)
    return;

  set_bit(*(io->port), io->pin);
}

void io_deassert(io_t *io)
{
  if (!io)
    return;

  clr_bit(*(io->port), io->pin);
}

void io_toggle(io_t *io)
{
  if (!io)
    return;

  toggle_bit(*(io->port), io->pin);
}
