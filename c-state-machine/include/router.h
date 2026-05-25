#ifndef DISPATCH_H
#define DISPATCH_H

#include <stdbool.h>

#include "events.h"
#include "interlock.h"
#include "state_machine.h"

typedef enum { MACHINE_MAIN = 0, MACHINE_INTERLOCK = 1, MACHINE_COUNT } Machine;

extern const bool routing_table[CMD_COUNT][MACHINE_COUNT];

void router(Command cmd, StateMachine *sm,
            void (*main_dispatch)(StateMachine *, Command),
            InterlockStateMachine *ism,
            void (*interlock_dispatch)(InterlockStateMachine *, Command));

#endif
