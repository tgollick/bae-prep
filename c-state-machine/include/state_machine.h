// This file outlines the states, commands and a struct to hold a basic state machine object.

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

// All commands outlined in the state-diagram.md file
typedef enum {
    CMD_ARM,
    CMD_ABORT,
    CMD_ENABLE,
    CMD_FIRE,
    CMD_RESET,
    CMD_ARMING_COMPLETE,
    CMD_DISARMING_COMPLETE,
    CMD_FAULT_DETECTED,
    CMD_COUNT,
} Command;

typedef struct {
    State current_state;

    // Will outline more members of this object later down the line
} StateMachine;
