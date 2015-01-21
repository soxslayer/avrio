#ifndef __UTIL__
#define __UTIL__

#define set_bit(d,b) ((d)|=1<<(b))
#define clr_bit(d,b) ((d)&=~(1<<(b)))
#define toggle_bit(d,b) ((d)^=1<<(b))

#endif /* __UTIL__ */
