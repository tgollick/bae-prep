#ifndef ACTIONS_H
#define ACTIONS_H

#include "interlock.h"
#include "state_machine.h"

typedef enum { ACTION_OK = 0, ACTION_FAULT } Action_Result;

typedef Action_Result (*action_fn)(void);

typedef struct {
  action_fn on_entry;
  action_fn on_exit;
} StateAction;

extern const StateAction actions_table[STATE_COUNT + STATE_COUNT_INTERLOCK];

// ----------------------------
// MAIN STATE MACHINE ACTIONS
// ----------------------------

// Safe State Actions
Action_Result safe_entry(void);
Action_Result safe_exit(void);

// Arming State Actions
Action_Result arming_entry(void);
Action_Result arming_exit(void);

// Armed State Actions
Action_Result armed_entry(void);
Action_Result armed_exit(void);

// Enabled State Actions
Action_Result enabled_entry(void);
Action_Result enabled_exit(void);

// Fired State Actions
Action_Result fired_entry(void);
Action_Result fired_exit(void);

// Disarming State Actions
Action_Result disarming_entry(void);
Action_Result disarming_exit(void);

// Fault State Actions
Action_Result fault_entry(void);
Action_Result fault_exit(void);

// -------------------------------
// INTERLOCK STATE MACHINE ACTIONS
// -------------------------------

// Closed State Actions
Action_Result closed_entry(void);
Action_Result closed_exit(void);

// Open State Actions
Action_Result open_entry(void);
Action_Result open_exit(void);

// Opening State Actions
Action_Result opening_entry(void);
Action_Result opening_exit(void);

// Closing State Actions
Action_Result closing_entry(void);
Action_Result closing_exit(void);

// Fault State Actions
Action_Result fault_entry(void);
Action_Result fault_exit(void);

#endif
