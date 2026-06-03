#include "actions.h"
#include "interlock.h"
#include "state_machine.h"
#include <stdio.h>

const State_Action main_action_table[STATE_COUNT] = {
    // SAFE
    [STATE_SAFE] = {.on_entry = &main_safe_entry, .on_exit = &main_safe_exit},

    // ARMING
    [STATE_ARMING] = {.on_entry = &main_arming_entry,
                      .on_exit = &main_arming_exit},

    // ARMED
    [STATE_ARMED] = {.on_entry = &main_armed_entry,
                     .on_exit = &main_armed_exit},

    // ENABLED
    [STATE_ENABLED] = {.on_entry = &main_enabled_entry,
                       .on_exit = &main_enabled_exit},

    // FIRED
    [STATE_FIRED] = {.on_entry = &main_fired_entry,
                     .on_exit = &main_fired_exit},

    // FAULT
    [STATE_FAULT] = {.on_entry = &main_fault_entry,
                     .on_exit = &main_fault_exit},

    // DISARMING
    [STATE_DISARMING] = {.on_entry = &main_disarming_entry,
                         .on_exit = &main_disarming_exit},
};
const State_Action interlock_action_table[STATE_COUNT_INTERLOCK] = {
    // CLOSED
    {},
    // OPEN
    {},
    // OPENING
    {},
    // CLOSING
    {},
    // FAULT_INTERLOCK
    {},
};

// ----------------------------
// MAIN STATE MACHINE ACTIONS
// ----------------------------

// Safe State Actions
Action_Result main_safe_entry(void) {
  printf("LOG: Entered into SAFE state.");
  printf("LOG: Engaging bleed resistors.");
  printf("LOG: Capacitors drained, system SAFE.");

  return ACTION_OK;
}

Action_Result main_safe_exit(void) {
  printf("LOG: Exited SAFE state.");

  return ACTION_OK;
}

// Arming State Actions
Action_Result main_arming_entry(void) {
  printf("LOG: Entered into ARMING state.");

  return ACTION_OK;
}

Action_Result main_arming_exit(void) {
  printf("LOG: Exited ARMING state.");

  return ACTION_OK;
}

// Armed State Actions
Action_Result main_armed_entry(void) {
  printf("LOG: Entered into ARMED state.");
  printf("LOG: Master pin power ENABLED");

  return ACTION_OK;
}

Action_Result main_armed_exit(void) {
  printf("LOG: Exited ARMED state.");

  return ACTION_OK;
}

// Enabled State Actions
Action_Result main_enabled_entry(void) {
  printf("LOG: Entered into ENABLED state.");
  printf("LOG: Secondary pin power ENABLED.");
  printf("LOG: Firing System is live and active!");

  return ACTION_OK;
}

Action_Result main_enabled_exit(void) {
  printf("LOG: Exited ENABLED state.");

  return ACTION_OK;
}

// Fired State Actions
Action_Result main_fired_entry(void) {
  printf("LOG: Entered into FIRED state.");
  printf("LOG: Weapon Fired");
  printf("WARNING: HIGH VOLTAGE! Maintaining state may cause hardware "
         "complications!");

  return ACTION_OK;
}

Action_Result main_fired_exit(void) {
  printf("LOG: Exited FIRED state.");
  printf("LOG: Removing power from main and secondary pins.");

  return ACTION_OK;
}

// Disarming State Actions
Action_Result main_disarming_entry(void) {
  printf("LOG: Entered into DISARMING state.");

  return ACTION_OK;
}

Action_Result main_disarming_exit(void) {
  printf("LOG: Exited DISARMING state.");

  return ACTION_OK;
}

// Fault State Actions
Action_Result main_fault_entry(void) {
  printf("LOG: Entered into FAULT state.");
  printf("LOG: FAULT TRIGGERED! ALARM SOUNDED!");

  return ACTION_OK;
}

Action_Result main_fault_exit(void) {
  printf("LOG: Exited FAULT state.");

  return ACTION_OK;
}

// -------------------------------
// INTERLOCK STATE MACHINE ACTIONS
// -------------------------------

// Closed State Actions
Action_Result interlock_closed_entry(void) {
  printf("INTERLOCK LOG: Entered into CLOSED state.");
  printf("INTERLOCK LOG: Checking Interlock is closed");
  printf("INTERLOCK LOG: Interlock Hatch CLOSED");

  return ACTION_OK;
}
Action_Result interlock_closed_exit(void) {
  printf("INTERLOCK LOG: Exited CLOSED state.");

  return ACTION_OK;
}

// Open State Actions
Action_Result interlock_open_entry(void) {
  printf("INTERLOCK LOG: Entered into OPEN state.");
  printf("INTERLOCK LOG: Checking Interlock is open");
  printf("INTERLOCK LOG: Interlock Hatch OPEN");

  return ACTION_OK;
}

Action_Result interlock_open_exit(void) {
  printf("INTERLOCK LOG: Exited OPEN state.");

  return ACTION_OK;
}

// Opening State Actions
Action_Result interlock_opening_entry(void) {
  printf("INTERLOCK LOG: Entered into OPENING state.");
  printf("INTERLOCK LOG: GPIO Set to 1, powering motor...");
  printf("INTERLOCK LOG: Timeout timer stated, 5 seconds until expiry.");

  return ACTION_OK;
}
Action_Result interlock_opening_exit(void) {
  printf("INTERLOCK LOG: Exited OPENING state.");
  printf("INTERLOCK LOG: Sensor Triggered, Interlock opened.");
  printf("INTERLOCK LOG: GPIO Set to 0, powering down motor...");

  return ACTION_OK;
}

// Closing State Actions
Action_Result interlock_closing_entry(void) {
  printf("INTERLOCK LOG: Entered into CLOSING state.");
  printf("INTERLOCK LOG: GPIO Set to 1, powering motor...");
  printf("INTERLOCK LOG: Timeout timer stated, 5 seconds until expiry.");

  return ACTION_OK;
}
Action_Result interlock_closing_exit(void) {
  printf("INTERLOCK LOG: Exited CLOSING state.");
  printf("INTERLOCK LOG: Sensor Triggered, Interlock closed.");
  printf("INTERLOCK LOG: GPIO Set to 0, powering down motor...");

  return ACTION_OK;
}

// Fault State Actions
Action_Result interlock_fault_entry(void) {
  printf("INTERLOCK LOG: Entered into FAULT state.");
  printf("INTERLOCK LOG: FAULT TRIGGERED! ALARM SOUNDED!");

  return ACTION_OK;
}
Action_Result interlock_fault_exit(void) {
  printf("INTERLOCK LOG: Exited FAULT state.");

  return ACTION_OK;
}
