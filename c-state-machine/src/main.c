#include "state_machine.h"
#include <Windows.h>

int main(void)
{
    // Fault during arming
    StateMachine sm = {.current_state = STATE_SAFE};
    dispatch(&sm, CMD_ARM);
    Sleep(1000);
    dispatch(&sm, CMD_FAULT_DETECTED); // ARMING -> FAULT
    Sleep(1000);
    dispatch(&sm, CMD_RESET); // FAULT -> DISARMING
    Sleep(1000);
    dispatch(&sm, CMD_DISARMING_COMPLETE); // DISARMING -> SAFE

    return 0;
}
