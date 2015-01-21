#include <avr/pgmspace.h>
#include <stdlib.h>
#include <util/atomic.h>

#include "avrio.h"
#include "init.h"
#include "stddef.h"

extern uint8_t dbg;

static avrio_device_t *_open_fds[MAX_FDS];
static list_t *_device_list = NULL;

INIT_USR2(avrio_init)
{
  int i;

  for (i = 0; i < MAX_FDS; ++i)
    _open_fds[i] = NULL;
}

void avrio_register(avrio_device_t *device)
{
  if (!device->open || !device->close || !device->read || !device->write
      || !device->ioctl)
    return;

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    list_insert(&_device_list, &device->device_list);
  }
}

fd_t open(device_t device)
{
  fd_t fd;

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    list_t *d;

    for (fd = 0; fd < MAX_FDS && _open_fds[fd] != NULL; ++fd) ;

    if (fd == MAX_FDS)
      return INVALID_FD;

    for (d = _device_list; d; d = d->next) {
      avrio_device_t *dev = container_of(d, avrio_device_t, device_list);

      if (dev->device == device) {
        _open_fds[fd] = dev;
        break;
      }
    }
  }

  if (!_open_fds[fd])
    return INVALID_FD;

  if (_open_fds[fd]->open(_open_fds[fd])) {
    _open_fds[fd] = NULL;
    return INVALID_FD;
  }

  return fd;
}

void close(fd_t fd)
{
  avrio_device_t *dev;

  if (!VALID_FD(fd))
    return;

  dev = _open_fds[fd];
  _open_fds[fd] = NULL;

  if (dev)
    dev->close(dev);
}

int16_t read(fd_t fd, void *dest, uint8_t size)
{
  if (!VALID_FD(fd))
    return -1;

  if (_open_fds[fd])
    return _open_fds[fd]->read(_open_fds[fd], dest, size);

  return -1;
}

int16_t write(fd_t fd, const void *src, uint8_t size)
{
  if (!VALID_FD(fd))
    return -1;

  if (_open_fds[fd])
    return _open_fds[fd]->write(_open_fds[fd], src, size);

  return -1;
}

int8_t ioctl(fd_t fd, uint8_t request, void *data)
{
  if (!VALID_FD(fd))
    return -1;

  if (_open_fds[fd])
    return _open_fds[fd]->ioctl(_open_fds[fd], request, data);

  return -1;
}
