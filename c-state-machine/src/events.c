#include "events.h"

const char *command_to_string(Command c) {
  switch (c) {
  case CMD_ARM:
    return "ARM";
  case CMD_ENABLE:
    return "ENABLE";
  case CMD_FIRE:
    return "FIRE";
  case CMD_ABORT:
    return "ABORT";
  case CMD_RESET:
    return "RESET";
  case CMD_FAULT_DETECTED:
    return "FAULT_DETECTED";
  case CMD_INTERLOCK_CLOSED:
    return "INTERLOCK_CLOSED";
  case CMD_INTERLOCK_OPENED:
    return "INTERLOCK_OPENED";
  case CMD_INTERLOCK_FAULT_DETECTED:
    return "INTERLOCK_FAULT_DETECED";
  default:
    return "UNKNOWN";
  }
}
