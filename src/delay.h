#ifndef __DELAY__
#define __DELAY__

#ifndef __OPTIMIZE__
/* No optimizations, probably a debug build */
# define _delay_us(...)
#else
# include <util/delay.h>
#endif

#endif /* __DELAY__ */
