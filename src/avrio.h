#ifndef __AVRIO__
#define __AVRIO__

#include <stdint.h>

#include "list.h"

typedef int8_t fd_t;
typedef uint8_t device_t;

typedef struct _avrio_device
{
  device_t device;
  int8_t (*open)(struct _avrio_device *);
  void (*close)(struct _avrio_device *);
  int16_t (*read)(struct _avrio_device *, void *, uint8_t);
  int16_t (*write)(struct _avrio_device *, const void *, uint8_t);
  int8_t (*ioctl)(struct _avrio_device *, uint8_t, void *);
  list_t device_list;
} avrio_device_t;

void avrio_init();
void avrio_register(avrio_device_t *device);

#define INVALID_FD -1
#define MAX_FDS 8
#define VALID_FD(fd)(fd >= 0 && fd < MAX_FDS)

fd_t open(device_t device);
void close(fd_t fd);
int16_t read(fd_t fd, void *dest, uint8_t size);
int16_t write(fd_t fd, const void *src, uint8_t size);
int8_t ioctl(fd_t fd, uint8_t request, void *data);

/* Available I/O devices */
#define AVRIO_UART0 0
#define AVRIO_UART1 1
#define AVRIO_UART2 2

#endif /* __AVRIO__ */
