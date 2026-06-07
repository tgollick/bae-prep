#include "clock.h"
#include <stdint.h>

static uint32_t tick_count;

void clock_init(void) { tick_count = 0; }

void clock_tick(void) { tick_count++; }

uint32_t now(void) { return tick_count; }
