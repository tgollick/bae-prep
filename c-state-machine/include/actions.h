#ifndef ACTIONS_H
#define ACTIONS_H

#include "interlock.h"
#include "state_machine.h"

typedef enum { ACTION_OK = 0, ACTION_FAULT } Action_Result;

typedef Action_Result (*action_fn)(void);

typedef struct {
  action_fn on_entry;
  action_fn on_exit;
} State_Action;

extern const State_Action interlock_action_table[STATE_COUNT_INTERLOCK];
extern const State_Action main_action_table[STATE_COUNT];

// ----------------------------
// MAIN STATE MACHINE ACTIONS
// ----------------------------

// Safe State Actions
Action_Result main_safe_entry(void);
Action_Result main_safe_exit(void);

// Arming State Actions
Action_Result main_arming_entry(void);
Action_Result main_arming_exit(void);

// Armed State Actions
Action_Result main_armed_entry(void);
Action_Result main_armed_exit(void);

// Enabled State Actions
Action_Result main_enabled_entry(void);
Action_Result main_enabled_exit(void);

// Fired State Actions
Action_Result main_fired_entry(void);
Action_Result main_fired_exit(void);

// Disarming State Actions
Action_Result main_disarming_entry(void);
Action_Result main_disarming_exit(void);

// Fault State Actions
Action_Result main_fault_entry(void);
Action_Result main_fault_exit(void);

// -------------------------------
// INTERLOCK STATE MACHINE ACTIONS
// -------------------------------

// Closed State Actions
Action_Result interlock_closed_entry(void);
Action_Result interlock_closed_exit(void);

// Open State Actions
Action_Result interlock_open_entry(void);
Action_Result interlock_open_exit(void);

// Opening State Actions
Action_Result interlock_opening_entry(void);
Action_Result interlock_opening_exit(void);

// Closing State Actions
Action_Result interlock_closing_entry(void);
Action_Result interlock_closing_exit(void);

// Fault State Actions
Action_Result interlock_fault_entry(void);
Action_Result interlock_fault_exit(void);

#endif
