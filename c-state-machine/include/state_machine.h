// This file outlines the states, commands and a struct to hold a basic state machine object.

// All states outlined in the state-diagram.md file
typedef enum
{
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
typedef enum
{
    CMD_ARM,
    CMD_ENABLE,
    CMD_FIRE,
    CMD_ABORT,
    CMD_RESET,
    CMD_FAULT_DETECTED,
    CMD_ARMING_COMPLETE,
    CMD_DISARMING_COMPLETE,
    CMD_COUNT,
} Command;

typedef struct
{
    State current_state;

    // Will outline more members of this object later down the line
} StateMachine;

const State translation_table[STATE_COUNT][CMD_COUNT] = {
    {
        // SAFE ROW
        STATE_ARMING, // ARM_CMD
        STATE_COUNT,  // ENABLE_CMD - INVALID
        STATE_COUNT,  // FIRE_CMD - INVALID
        STATE_COUNT,  // ABORT_CMD - INVALID
        STATE_COUNT,  // RESET_CMD - INVALID
        STATE_COUNT,  // FAULT_DETECTED - INVALID
        STATE_COUNT,  // ARMING_COMPLETE - INVALID
        STATE_COUNT,  // DISARMING_COMPLETE - INVALID
    },

    {
        // ARMING ROW
        STATE_COUNT,     // ARM_CMD - INVALID
        STATE_COUNT,     // ENABLE_CMD - INVALID
        STATE_COUNT,     // FIRE_CMD - INVALID
        STATE_DISARMING, // ABORT_CMD
        STATE_COUNT,     // RESET_CMD - INVALID
        STATE_FAULT,     // FAULT_DETECTED
        STATE_ARMED,     // ARMING_COMPLETE
        STATE_COUNT,     // DISARMING_COMPLETE - INVALID
    },

    {
        // ARMED ROW
        STATE_COUNT,     // ARM_CMD - INVALID
        STATE_ENABLED,   // ENABLE_CMD
        STATE_COUNT,     // FIRE_CMD - INVALID
        STATE_DISARMING, // ABORT_CMD
        STATE_COUNT,     // RESET_CMD - INVALID
        STATE_FAULT,     // FAULT_DETECTED
        STATE_COUNT,     // ARMING_COMPLETE - INVALID
        STATE_COUNT,     // DISARMING_COMPLETE - INVALID
    },

    {
        // ENABLED ROW
        STATE_COUNT,     // ARM_CMD - INVALID
        STATE_COUNT,     // ENABLE_CMD - INVALID
        STATE_FIRED,     // FIRE_CMD
        STATE_DISARMING, // ABORT_CMD
        STATE_COUNT,     // RESET_CMD - INVALID
        STATE_FAULT,     // FAULT_DETECTED
        STATE_COUNT,     // ARMING_COMPLETE - INVALID
        STATE_COUNT,     // DISARMING_COMPLETE - INVALID
    },

    {
        // FIRED ROW
        STATE_COUNT, // ARM_CMD - INVALID
        STATE_COUNT, // ENABLE_CMD - INVALID
        STATE_COUNT, // FIRE_CMD - INVALID
        STATE_COUNT, // ABORT_CMD - INVALID
        STATE_COUNT, // RESET_CMD - INVALID
        STATE_COUNT, // FAULT_DETECTED - INVALID
        STATE_COUNT, // ARMING_COMPLETE - INVALID
        STATE_COUNT, // DISARMING_COMPLETE - INVALID
    },

    {
        // DISARMING ROW
        STATE_COUNT, // ARM_CMD - INVALID
        STATE_COUNT, // ENABLE_CMD - INVALID
        STATE_COUNT, // FIRE_CMD - INVALID
        STATE_COUNT, // ABORT_CMD - INVALID
        STATE_COUNT, // RESET_CMD - INVALID
        STATE_FAULT, // FAULT_DETECTED
        STATE_COUNT, // ARMING_COMPLETE - INVALID
        STATE_SAFE,  // DISARMING_COMPLETE
    },

    {
        // FAULT ROW
        STATE_COUNT,     // ARM_CMD - INVALID
        STATE_COUNT,     // ENABLE_CMD - INVALID
        STATE_COUNT,     // FIRE_CMD - INVALID
        STATE_COUNT,     // ABORT_CMD - INVALID
        STATE_DISARMING, // RESET_CMD
        STATE_COUNT,     // FAULT_DETECTED - INVALID
        STATE_COUNT,     // ARMING_COMPLETE - INVALID
        STATE_COUNT,     // DISARMING_COMPLETE - INVALID
    },
};