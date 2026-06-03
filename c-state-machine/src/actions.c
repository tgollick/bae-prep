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
    [STATE_CLOSED] = {.on_entry = &interlock_closed_entry,
                      .on_exit = &interlock_closed_exit},

    // OPEN
    [STATE_OPEN] = {.on_entry = &interlock_open_entry,
                    .on_exit = &interlock_open_exit},

    // OPENING
    [STATE_OPENING] = {.on_entry = &interlock_open_entry,
                       .on_exit = &interlock_open_exit},

    // CLOSING
    [STATE_CLOSING] = {.on_entry = &interlock_closing_entry,
                       .on_exit = &interlock_closing_exit},

    // FAULT_INTERLOCK
    [STATE_FAULT_INTERLOCK] = {.on_entry = &interlock_fault_entry,
                               .on_exit = &interlock_fault_exit},
};

// ----------------------------
// MAIN STATE MACHINE ACTIONS
// ----------------------------

// Safe State Actions
Action_Result main_safe_entry(void) {
  printf("LOG: Entered into SAFE state.\n");
  printf("LOG: Engaging bleed resistors.\n");
  printf("LOG: Capacitors drained, system SAFE.\n");

  return ACTION_OK;
}

Action_Result main_safe_exit(void) {
  printf("LOG: Exited SAFE state.\n");

  return ACTION_OK;
}

// Arming State Actions
Action_Result main_arming_entry(void) {
  printf("LOG: Entered into ARMING state.\n");

  return ACTION_OK;
}

Action_Result main_arming_exit(void) {
  printf("LOG: Exited ARMING state.\n");

  return ACTION_OK;
}

// Armed State Actions
Action_Result main_armed_entry(void) {
  printf("LOG: Entered into ARMED state.\n");
  printf("LOG: Master pin power ENABLED\n");

  return ACTION_OK;
}

Action_Result main_armed_exit(void) {
  printf("LOG: Exited ARMED state.\n");

  return ACTION_OK;
}

// Enabled State Actions
Action_Result main_enabled_entry(void) {
  printf("LOG: Entered into ENABLED state.\n");
  printf("LOG: Secondary pin power ENABLED.\n");
  printf("LOG: Firing System is live and active!\n");

  return ACTION_OK;
}

Action_Result main_enabled_exit(void) {
  printf("LOG: Exited ENABLED state.\n");

  return ACTION_OK;
}

// Fired State Actions
Action_Result main_fired_entry(void) {
  printf("LOG: Entered into FIRED state.\n");
  printf("LOG: Weapon Fired\n");
  printf("WARNING: HIGH VOLTAGE! Maintaining state may cause hardware "
         "complications!\n");

  return ACTION_OK;
}

Action_Result main_fired_exit(void) {
  printf("LOG: Exited FIRED state.\n");
  printf("LOG: Removing power from main and secondary pins.\n");

  return ACTION_OK;
}

// Disarming State Actions
Action_Result main_disarming_entry(void) {
  printf("LOG: Entered into DISARMING state.\n");

  return ACTION_OK;
}

Action_Result main_disarming_exit(void) {
  printf("LOG: Exited DISARMING state.\n");

  return ACTION_OK;
}

// Fault State Actions
Action_Result main_fault_entry(void) {
  printf("LOG: Entered into FAULT state.\n");
  printf("LOG: FAULT TRIGGERED! ALARM SOUNDED!\n");

  return ACTION_OK;
}

Action_Result main_fault_exit(void) {
  printf("LOG: Exited FAULT state.\n");

  return ACTION_OK;
}

// -------------------------------
// INTERLOCK STATE MACHINE ACTIONS
// -------------------------------

// Closed State Actions
Action_Result interlock_closed_entry(void) {
  printf("INTERLOCK LOG: Entered into CLOSED state.\n");
  printf("INTERLOCK LOG: Checking Interlock is closed\n");
  printf("INTERLOCK LOG: Interlock Hatch CLOSED\n");

  return ACTION_OK;
}
Action_Result interlock_closed_exit(void) {
  printf("INTERLOCK LOG: Exited CLOSED state.\n");

  return ACTION_OK;
}

// Open State Actions
Action_Result interlock_open_entry(void) {
  printf("INTERLOCK LOG: Entered into OPEN state.\n");
  printf("INTERLOCK LOG: Checking Interlock is open\n");
  printf("INTERLOCK LOG: Interlock Hatch OPEN\n");

  return ACTION_OK;
}

Action_Result interlock_open_exit(void) {
  printf("INTERLOCK LOG: Exited OPEN state.\n");

  return ACTION_OK;
}

// Opening State Actions
Action_Result interlock_opening_entry(void) {
  printf("INTERLOCK LOG: Entered into OPENING state.\n");
  printf("INTERLOCK LOG: GPIO Set to 1, powering motor...\n");
  printf("INTERLOCK LOG: Timeout timer stated, 5 seconds until expiry.\n");

  return ACTION_OK;
}
Action_Result interlock_opening_exit(void) {
  printf("INTERLOCK LOG: Exited OPENING state.\n");
  printf("INTERLOCK LOG: Sensor Triggered, Interlock opened.\n");
  printf("INTERLOCK LOG: GPIO Set to 0, powering down motor...\n");

  return ACTION_OK;
}

// Closing State Actions
Action_Result interlock_closing_entry(void) {
  printf("INTERLOCK LOG: Entered into CLOSING state.\n");
  printf("INTERLOCK LOG: GPIO Set to 1, powering motor...\n");
  printf("INTERLOCK LOG: Timeout timer stated, 5 seconds until expiry.\n");

  return ACTION_OK;
}
Action_Result interlock_closing_exit(void) {
  printf("INTERLOCK LOG: Exited CLOSING state.\n");
  printf("INTERLOCK LOG: Sensor Triggered, Interlock closed.\n");
  printf("INTERLOCK LOG: GPIO Set to 0, powering down motor...\n");

  return ACTION_OK;
}

// Fault State Actions
Action_Result interlock_fault_entry(void) {
  printf("INTERLOCK LOG: Entered into FAULT state.\n");
  printf("INTERLOCK LOG: FAULT TRIGGERED! ALARM SOUNDED!\n");

  return ACTION_OK;
}
Action_Result interlock_fault_exit(void) {
  printf("INTERLOCK LOG: Exited FAULT state.\n");

  return ACTION_OK;
}
