#ifndef __STDDEF__
#define __STDDEF__

#define offsetof(st,m) __builtin_offsetof(st,m)
#define container_of(ptr,type,member) ({ \
  const __typeof__( ((type *)0)->member ) *__mptr = (ptr); \
  (type *)( (char *)__mptr - offsetof(type,member) ); })


#endif /* __STDDEF__ */
