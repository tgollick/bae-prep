#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "events.h"

// This file outlines the states, commands and a struct to hold a basic state
// machine object.

// All states outlined in the state-diagram.md file
typedef enum {
  STATE_SAFE,
  STATE_ARMING,
  STATE_ARMED,
  STATE_ENABLED,
  STATE_FIRED,
  STATE_FAULT,
  STATE_DISARMING,
  STATE_COUNT,
} State;

typedef struct {
  State current_state;

  // Will outline more members of this object later down the line
} StateMachine;

extern const State translation_table[STATE_COUNT][CMD_COUNT];

const char *state_to_string(State s);

void dispatch(StateMachine *sm, Command cmd);

#endif
