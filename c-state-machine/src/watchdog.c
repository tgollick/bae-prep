#include "watchdog.h"
#include "clock.h"
#include <stddef.h>
#include <stdint.h>

bool check_time(const Watchdog *wd) {
  return wd->enabled && ((now() - wd->start_tick) >= wd->limit);
}

void set_time(Watchdog *wd, uint32_t time) { wd->start_tick = time; }
