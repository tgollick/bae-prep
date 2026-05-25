#ifndef EVENTS_H
#define EVENTS_H

typedef enum {
  // Main State Commands
  CMD_ARM,
  CMD_ENABLE,
  CMD_FIRE,
  CMD_ABORT,
  CMD_RESET,
  CMD_FAULT_DETECTED,
  // Interlock Commands
  CMD_INTERLOCK_CLOSED,
  CMD_INTERLOCK_OPENED,
  CMD_INTERLOCK_FAULT_DETECTED,
  // Command count
  CMD_COUNT
} Command;

const char *command_to_string(Command c);
#endif
