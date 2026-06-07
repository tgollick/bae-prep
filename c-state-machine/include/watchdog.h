#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint32_t start_tick;
  uint32_t limit;
  bool enabled;
} Watchdog;

bool check_time(const Watchdog *wd);

void set_time(Watchdog *wd, uint32_t time);

#endif
