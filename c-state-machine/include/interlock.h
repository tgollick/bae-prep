#ifndef INTERLOCK_STATE_H
#define INTERLOCK_STATE_H

#include "events.h"
#include "watchdog.h"
#include <stdint.h>

typedef enum {
  STATE_CLOSED,
  STATE_OPEN,
  STATE_OPENING,
  STATE_CLOSING,
  STATE_FAULT_INTERLOCK,
  STATE_COUNT_INTERLOCK,
} InterlockState;

typedef struct {
  InterlockState current_state;
  Watchdog wd;

} InterlockStateMachine;

extern const InterlockState interlock_transition_table[STATE_COUNT_INTERLOCK]
                                                      [CMD_COUNT];
extern const uint32_t interlock_timeout_table[STATE_COUNT_INTERLOCK];

const char *interlock_state_to_string(InterlockState s);

void interlock_dispatch(InterlockStateMachine *sm, Command cmd);

#endif
