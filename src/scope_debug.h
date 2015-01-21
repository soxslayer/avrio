#ifndef __SCOPE_DEBUG__
#define __SCOPE_DEBUG__

#include <stdint.h>

void debug_init();
void debug_strobe_pin();
void debug_strobe_byte(uint8_t b);
void debug_assert_pin();
void debug_deassert_pin();
void debug_toggle_pin();

#endif /* __SCOPE_DEBUG__ */
