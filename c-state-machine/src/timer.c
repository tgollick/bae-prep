#include "timer.h"
#include <stddef.h>

int check_tick(Timer t, size_t limit) {
  if ((t.curr_tick - t.start_tick) >= limit) {
    return 1;
  } else {
    return 0;
  }
}
