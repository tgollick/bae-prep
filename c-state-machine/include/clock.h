#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

void clock_init(void);
void clock_tick(void);
uint32_t now(void);

#endif
