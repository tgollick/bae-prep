#include "state_machine.h"
#include "events.h"
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
    // Then update the current_state to the newly transitioned state
    sm->current_state = next_state;
  }
}
