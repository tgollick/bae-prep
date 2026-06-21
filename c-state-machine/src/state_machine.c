#include "state_machine.h"
#include "actions.h"
#include "clock.h"
#include "events.h"
#include <stdint.h>
#include <stdio.h>

const State translation_table[STATE_COUNT][CMD_COUNT] = {
    {
        // SAFE ROW
        STATE_ARMING, // ARM_CMD
        STATE_COUNT,  // ENABLE_CMD - INVALID
        STATE_COUNT,  // FIRE_CMD - INVALID
        STATE_COUNT,  // ABORT_CMD - INVALID
        STATE_COUNT,  // RESET_CMD - INVALID
        STATE_FAULT,  // FAULT_DETECTED

        STATE_COUNT, // INTERLOCK_CLOSED - INVALID
        STATE_COUNT, // INTERLOCK_OPENED - INVALID
        STATE_FAULT, // INTERLOCK_FAULT_DETECTED
    },

    {
        // ARMING ROW
        STATE_COUNT,     // ARM_CMD - INVALID
        STATE_COUNT,     // ENABLE_CMD - INVALID
        STATE_COUNT,     // FIRE_CMD - INVALID
        STATE_DISARMING, // ABORT_CMD
        STATE_COUNT,     // RESET_CMD - INVALID
        STATE_FAULT,     // FAULT_DETECTED

        STATE_COUNT, // INTERLOCK_CLOSED - INVALID
        STATE_ARMED, // INTERLOCK_OPENED
        STATE_FAULT, // INTERLOCK_FAULT_DETECTED
    },

    {
        // ARMED ROW
        STATE_COUNT,     // ARM_CMD - INVALID
        STATE_ENABLED,   // ENABLE_CMD
        STATE_COUNT,     // FIRE_CMD - INVALID
        STATE_DISARMING, // ABORT_CMD
        STATE_COUNT,     // RESET_CMD - INVALID
        STATE_FAULT,     // FAULT_DETECTED

        STATE_COUNT, // INTERLOCK_CLOSED - INVALID
        STATE_COUNT, // INTERLOCK_OPENED - INVALID
        STATE_FAULT, // INTERLOCK_FAULT_DETECTED
    },

    {
        // ENABLED ROW
        STATE_COUNT,     // ARM_CMD - INVALID
        STATE_COUNT,     // ENABLE_CMD - INVALID
        STATE_FIRED,     // FIRE_CMD
        STATE_DISARMING, // ABORT_CMD
        STATE_COUNT,     // RESET_CMD - INVALID
        STATE_FAULT,     // FAULT_DETECTED

        STATE_COUNT, // INTERLOCK_CLOSED - INVALID
        STATE_COUNT, // INTERLOCK_OPENED - INVALID
        STATE_FAULT, // INTERLOCK_FAULT_DETECTED
    },

    {
        // FIRED ROW
        STATE_COUNT, // ARM_CMD - INVALID
        STATE_COUNT, // ENABLE_CMD - INVALID
        STATE_COUNT, // FIRE_CMD - INVALID
        STATE_COUNT, // ABORT_CMD - INVALID
        STATE_COUNT, // RESET_CMD - INVALID
        STATE_COUNT, // FAULT_DETECTED - INVALID

        STATE_COUNT, // INTERLOCK_CLOSED - INVALID
        STATE_COUNT, // INTERLOCK_OPENED - INVALID
        STATE_COUNT, // INTERLOCK_FAULT_DETECTED - INVALID
    },

    {
        // FAULT ROW
        STATE_COUNT,     // ARM_CMD - INVALID
        STATE_COUNT,     // ENABLE_CMD - INVALID
        STATE_COUNT,     // FIRE_CMD - INVALID
        STATE_COUNT,     // ABORT_CMD - INVALID
        STATE_DISARMING, // RESET_CMD
        STATE_COUNT,     // FAULT_DETECTED - INVALID

        STATE_COUNT, // INTERLOCK_CLOSED - INVALID
        STATE_COUNT, // INTERLOCK_OPENED - INVALID
        STATE_COUNT  // INTERLOCK_FAULT_DETECTED - INVALID
    },

    {
        // DISARMING ROW
        STATE_COUNT, // ARM_CMD - INVALID
        STATE_COUNT, // ENABLE_CMD - INVALID
        STATE_COUNT, // FIRE_CMD - INVALID
        STATE_COUNT, // ABORT_CMD - INVALID
        STATE_COUNT, // RESET_CMD - INVALID
        STATE_FAULT, // FAULT_DETECTED

        STATE_SAFE,  // INTERLOCK_CLOSED
        STATE_COUNT, // INTERLOCK_OPENED - INVALID
        STATE_FAULT, // INTERLOCK_FAULT_DETECTED
    },

};

const uint32_t main_timeout_table[STATE_COUNT] = {
    0,   // STATE_SAFE      — resting, not timed
    800, // STATE_ARMING    — backstop over the interlock opening
    0,   // STATE_ARMED     — awaits operator ENABLE_CMD
    0,   // STATE_ENABLED   — awaits operator FIRE_CMD
    0,   // STATE_FIRED     — terminal
    0,   // STATE_FAULT     — terminal until RESET
    800, // STATE_DISARMING — backstop over the interlock closing
};

const char *state_to_string(State s) {
  switch (s) {
  case STATE_SAFE:
    return "SAFE";
  case STATE_ARMING:
    return "ARMING";
  case STATE_ARMED:
    return "ARMED";
  case STATE_ENABLED:
    return "ENABLED";
  case STATE_FIRED:
    return "FIRED";
  case STATE_FAULT:
    return "FAULT";
  case STATE_DISARMING:
    return "DISARMING";
  default:
    return "UNKNOWN";
  }
}

void dispatch(StateMachine *sm, Command cmd) {
  // First validate the command and current state
  // Although the current state or incoming command should never be STATE_COUNT
  // or CMD_COUNT its worth including the value it represents in our comparisons
  // to be water tight regarding validation
  if (sm->current_state < 0 || sm->current_state >= STATE_COUNT) {
    printf("MAIN ERROR: Current state is invalid\n");
    printf("MAIN ERROR: Transitioning to FAULT State\n");
    sm->current_state = STATE_FAULT;
    return;
  }

  if (cmd < 0 || cmd >= CMD_COUNT) {
    printf("MAIN ERROR: Invalid Command\n");
    printf("MAIN ERROR: Transitioning to FAULT State\n");
    sm->current_state = STATE_FAULT;
    return;
  }

  // Look up next state in the transition table
  State next_state = translation_table[sm->current_state][cmd];

  // Check if the transition is valid
  if (next_state == STATE_COUNT) {
    printf("MAIN ERROR: This transition is invalid!\n");
    return;
  }
  // Otherwise state is valid, update the state
  else {
    // Now we know the transition to next state is valid, execute the on_exit
    // command
    main_action_table[sm->current_state].on_exit();

    // Update state to next state
    sm->current_state = next_state;

    // Turn on the watchdog if the next state requires and set the start_tick to
    // the current time
    sm->wd.enabled = main_timeout_table[sm->current_state];

    if (sm->wd.enabled) {
      sm->wd.start_tick = now();
    }

    // Now we have 'entered' the next state, execute the on_entry function, if
    // returned ACTION_OK do nothing, otherwise transition to fault
    // This is the safety standard if the entry command faults for any reason
    if (main_action_table[sm->current_state].on_entry()) {
      sm->current_state = STATE_FAULT;
    }
  }
}
