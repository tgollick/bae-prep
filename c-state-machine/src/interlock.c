#include "interlock.h"
#include "actions.h"
#include "clock.h"
#include "events.h"
#include <stdio.h>

const InterlockState
    interlock_transition_table[STATE_COUNT_INTERLOCK][CMD_COUNT] = {
        {
            // CLOSED ROW
            STATE_OPENING,         // ARM_CMD
            STATE_COUNT_INTERLOCK, // ENABLE_CMD - INVALID
            STATE_COUNT_INTERLOCK, // FIRE_CMD - INVALID
            STATE_COUNT_INTERLOCK, // ABORT_CMD - INVALID
            STATE_COUNT_INTERLOCK, // RESET_CMD - INVALID
            STATE_FAULT_INTERLOCK, // FAULT_DETECTED

            STATE_COUNT_INTERLOCK, // INTERLOCK_CLOSED - INVALID
            STATE_COUNT_INTERLOCK, // INTERLOCK_OPENED - INVALID
            STATE_FAULT_INTERLOCK, // INTERLOCK_FAULT_DETECTED
        },

        {
            // OPEN ROW
            STATE_COUNT_INTERLOCK, // ARM_CMD - INVALID
            STATE_COUNT_INTERLOCK, // ENABLE_CMD - INVALID
            STATE_COUNT_INTERLOCK, // FIRE_CMD - INVALID
            STATE_CLOSING,         // ABORT_CMD
            STATE_COUNT_INTERLOCK, // RESET_CMD - INVALID
            STATE_FAULT_INTERLOCK, // FAULT_DETECTED

            STATE_COUNT_INTERLOCK, // INTERLOCK_CLOSED - INVALID
            STATE_COUNT_INTERLOCK, // INTERLOCK_OPENED - INVALID
            STATE_FAULT_INTERLOCK, // INTERLOCK_FAULT_DETECTED
        },

        {
            // OPENING ROW
            STATE_COUNT_INTERLOCK, // ARM_CMD - INVALID
            STATE_COUNT_INTERLOCK, // ENABLE_CMD - INVALID
            STATE_COUNT_INTERLOCK, // FIRE_CMD - INVALID
            STATE_CLOSING,         // ABORT_CMD
            STATE_COUNT_INTERLOCK, // RESET_CMD - INVALID
            STATE_FAULT_INTERLOCK, // FAULT_DETECTED

            STATE_COUNT_INTERLOCK, // INTERLOCK_CLOSED - INVALID
            STATE_OPEN,            // INTERLOCK_OPENED
            STATE_FAULT_INTERLOCK, // INTERLOCK_FAULT_DETECTED
        },

        {
            // CLOSING ROW
            STATE_COUNT_INTERLOCK, // ARM_CMD - INVALID
            STATE_COUNT_INTERLOCK, // ENABLE_CMD - INVALID
            STATE_COUNT_INTERLOCK, // FIRE_CMD - INVALID
            STATE_COUNT_INTERLOCK, // ABORT_CMD - INVALID
            STATE_COUNT_INTERLOCK, // RESET_CMD - INVALID
            STATE_FAULT_INTERLOCK, // FAULT_DETECTED

            STATE_CLOSED,          // INTERLOCK_CLOSED
            STATE_COUNT_INTERLOCK, // INTERLOCK_OPENED - INVALID
            STATE_FAULT_INTERLOCK, // INTERLOCK_FAULT_DETECTED
        },

        {
            // FAULT ROW
            STATE_COUNT_INTERLOCK, // ARM_CMD - INVALID
            STATE_COUNT_INTERLOCK, // ENABLE_CMD - INVALID
            STATE_COUNT_INTERLOCK, // FIRE_CMD - INVALID
            STATE_COUNT_INTERLOCK, // ABORT_CMD - INVALID
            STATE_CLOSED,          // RESET_CMD
            STATE_COUNT_INTERLOCK, // FAULT_DETECTED - INVALID

            STATE_COUNT_INTERLOCK, // INTERLOCK_CLOSED - INVALID
            STATE_COUNT_INTERLOCK, // INTERLOCK_OPENED - INVALID
            STATE_COUNT_INTERLOCK, // INTERLOCK_FAULT_DETECTED - INVALID
        },

};

const uint32_t interlock_timeout_table[STATE_COUNT_INTERLOCK] = {
    0, // STATE_CLOSED,
    0, // STATE_OPEN,
    1, // STATE_OPENING,
    1, // STATE_CLOSING,
    0, // STATE_FAULT_INTERLOCK,
};

const char *interlock_state_to_string(InterlockState s) {
  switch (s) {
  case STATE_CLOSED:
    return "CLOSED";
  case STATE_OPEN:
    return "OPEN";
  case STATE_OPENING:
    return "OPENING";
  case STATE_CLOSING:
    return "CLOSING";
  case STATE_FAULT_INTERLOCK:
    return "FAULT";
  default:
    return "UNKNOWN";
  }
}

void interlock_dispatch(InterlockStateMachine *sm, Command cmd) {
  // First validate the command and current state
  // Although the current state or incoming command should never be
  // STATE_COUNT_INTERLOCK or CMD_COUNT its worth including the value it
  // represents in our comparisons to be water tight regarding validation
  if (sm->current_state < 0 || sm->current_state >= STATE_COUNT_INTERLOCK) {
    printf("INTERLOCK ERROR: Current state is invalid\n");
    printf("INTERLOCK ERROR: Transitioning to FAULT State\n");
    sm->current_state = STATE_FAULT_INTERLOCK;
    return;
  }

  if (cmd < 0 || cmd >= CMD_COUNT) {
    printf("INTERLOCK ERROR: Invalid Command\n");
    printf("INTERLOCK ERROR: Transitioning to FAULT State\n");
    sm->current_state = STATE_FAULT_INTERLOCK;
    return;
  }

  // Look up next state in the transition table
  InterlockState next_state =
      interlock_transition_table[sm->current_state][cmd];

  // Check if the transition is valid
  if (next_state == STATE_COUNT_INTERLOCK) {
    printf("INTERLOCK ERROR: This transition is invalid!\n");
    return;
  }
  // Otherwise state is valid, update the state
  else {
    // Now we know the next transition is value, execute the on_exit function
    // for the current state
    interlock_action_table[sm->current_state].on_exit();

    // Then update the current_state to the newly transitioned state
    sm->current_state = next_state;

    // Check if the next state should be monitored by the watchdog,if so set
    // enabled to true and mark the start_tick
    sm->wd.enabled = interlock_timeout_table[sm->current_state];

    if (sm->wd.enabled) {
      sm->wd.start_tick = now();
    }

    // Now state has transitioned we have 'entered' the next state, check if the
    // on_entry is valid, otherwise transition to FAULT to ensure safety is
    // prioritised.
    if (interlock_action_table[sm->current_state].on_entry()) {
      sm->current_state = STATE_FAULT_INTERLOCK;
    }
  }
}
