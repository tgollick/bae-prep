#include <stdbool.h>

#include "actions.h"
#include "event_queue.h"
#include "events.h"
#include "interlock.h"
#include "router.h"
#include "state_machine.h"

const bool routing_table[CMD_COUNT][MACHINE_COUNT] = {
    // CMD_ARM
    {true, true},

    // CMD_ENABLE
    {true, false},

    // CMD_FIRE
    {true, false},

    // CMD_ABORT
    {true, true},

    // CMD_RESET
    {true, true},

    // CMD_FAULT_DETECTED
    {true, true},

    // CMD_INTERLOCK_CLOSED
    {true, true},

    // CMD_INTERLOCK_OPENED
    {true, true},

    // CMD_INTERLOCK_FAULT_DETECTED
    {true, true},
};

void router(Command cmd, StateMachine *sm,
            void (*main_dispatch)(StateMachine *, Command),
            InterlockStateMachine *ism,
            void (*interlock_dispatch)(InterlockStateMachine *, Command)) {
  // Check if incoming command is valid
  if (cmd < 0 || cmd >= CMD_COUNT)
    return;

  // Using a simple if function to decide the order of the call
  // NOTE: this is a dependency on the number of commands and number of state
  // machines, if we include another, we have to change this block of code to
  // reflect

  if (cmd >= CMD_INTERLOCK_OPENED) {
    // Handling the routing of the Interlock State Machine
    if (routing_table[cmd][MACHINE_INTERLOCK]) {
      interlock_action_table[ism->current_state].on_exit();
      interlock_dispatch(ism, cmd);

      if (interlock_action_table[ism->current_state].on_entry()) {
        enqueue(CMD_INTERLOCK_FAULT_DETECTED);
      }
    }

    // Handling the routing of the main State Machine
    if (routing_table[cmd][MACHINE_MAIN]) {
      main_action_table[sm->current_state].on_exit();
      main_dispatch(sm, cmd);

      if (main_action_table[sm->current_state].on_entry()) {
        enqueue(CMD_FAULT_DETECTED);
      }
    }
  } else {
    // Handling the routing of the main State Machine
    if (routing_table[cmd][MACHINE_MAIN]) {
      main_action_table[sm->current_state].on_exit();
      main_dispatch(sm, cmd);

      if (main_action_table[sm->current_state].on_entry()) {
        enqueue(CMD_FAULT_DETECTED);
      }
    }

    // Handling the routing of the Interlock State Machine
    if (routing_table[cmd][MACHINE_INTERLOCK]) {
      interlock_action_table[ism->current_state].on_exit();
      interlock_dispatch(ism, cmd);

      if (interlock_action_table[ism->current_state].on_entry()) {
        enqueue(CMD_INTERLOCK_FAULT_DETECTED);
      }
    }
  }
}
