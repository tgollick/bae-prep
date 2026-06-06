#include <stddef.h>

typedef struct {
  size_t start_tick;
  size_t curr_tick;
} Timer;

int check_tick(Timer t, size_t limit);
